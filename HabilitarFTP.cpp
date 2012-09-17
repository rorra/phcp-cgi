#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTP.h"
#include "HabilitarFTP.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioFTP servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioFTP"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_HABILITAR_FTP);
  string okFile(OK_HABILITAR_FTP);

  //Verificar que la pagina haya sido llamada por HabilitarFTP.php
  if (interfaz.verificarPagina(string("HabilitarFTP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite habilitar Cuentas FTP Extras";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtUsuario;
  if ((interfaz.obtenerVariable("txtUsuario", txtUsuario, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtUsuario);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterNumeroLetra(txtUsuario, "Usuario");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la cuenta FTP que se quiere habilitar
  derror = servicio.verificarExisteFTPDB(txtUsuario, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La cuenta FTP ftp" + txtUsuario + "-" + dominio.getIdDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la cuenta FTP se encuentra deshabilitada
  derror = servicio.verificarDeshabilitadaFTPDB(txtUsuario, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio()
      + " no se encuentra deshabilitada";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar el password de la cuenta FTP
  std::string txtPassword = servicio.recuperarPasswordDB(txtUsuario, dominio);
  if (txtPassword.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Decodificar el password
  txtPassword = decodificarPassword(txtPassword);

  //Cambiar la clave de la cuenta FTP en el sistema
  derror = servicio.cambiarClaveFTP(txtUsuario, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Habilitar la cuenta FTP en la base de datos
  derror = servicio.habilitarFTPDB(txtUsuario, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se habilito la Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() 
    + " fue habilitada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
