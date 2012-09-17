#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioBackup.h"
#include "servicioPOP3.h"
#include "CrearBackupEmail.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioBackup servicio("BACKUP MAIL", &dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioBackup"));
  cServicioPOP3 servicioPOP3(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPOP3"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_BACKUP_EMAIL);
  string okFile(OK_CREAR_BACKUP_EMAIL);

  //Verificar que la pagina haya sido llamada por CrearBackupEmail.php
  if (interfaz.verificarPagina(string("CrearBackupEmail.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Backups de sus Cuentas de E-Mails";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCompresion;
  if ((interfaz.obtenerVariable("txtCompresion", txtCompresion, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCompresion);

  //Verificar que la compresion sea valida
  derror = servicio.verificarCompresion(txtCompresion);
  if (derror > 0) {
    error = "La compresion " + txtCompresion + " no esta soportada";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si se puede crear el backup
  /*derror = servicio.verificarExisteBackupDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "Solo se puede crear un Backup de E-Mails por día";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }*/

  //Verificar si existen cuentas de email para el dominio
  derror = servicioPOP3.verificarExisteEmailsDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "No existen Cuentas de E-Mails para su dominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener el nombre del archivo
  std::string txtArchivo = servicio.getNombreArchivo(txtCompresion, dominio);

  //Crear el backup en el sistema
  derror = servicio.agregarBackup(txtCompresion, txtArchivo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el backup en la base de datos
  derror = servicio.agregarBackupDB(txtCompresion, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Backup de E-Mails";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  std::string link = "/backups/" + dominio.getDominio() + "/" + txtArchivo;
  mensaje = "El Backup " + txtArchivo + " fue creado con éxito, el mismo puede bajarlo via FTP desde el directorio /backups en su Home Dir.-";
  interfaz.reportarOkComandoBackup(mensaje, link, txtArchivo, okFile, dominio);

  return(0);
}
