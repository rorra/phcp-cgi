#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTP.h"
#include "DeshabilitarFTP.h"

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
  string errorFile(ERROR_DESHABILITAR_FTP);
  string okFile(OK_DESHABILITAR_FTP);

  //Verificar que la pagina haya sido llamada por DeshabilitarFTP.php
  if (interfaz.verificarPagina(string("DeshabilitarFTP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Cuentas FTP Extras";
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
    error = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la cuenta FTP se encuentra habilitada
  derror = servicio.verificarHabilitadaFTPDB(txtUsuario, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio()
      + " no se encuentra habilitada";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar el password de la cuenta FTP
  std::string txtPassword = servicio.recuperarPasswordDB(txtUsuario, dominio);
  if (txtPassword.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Codificar el password
  txtPassword = codificarPassword(txtPassword);

  //Cambiar la clave de la cuenta FTP en el sistema
  derror = servicio.cambiarClaveFTP(txtUsuario, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Deshabilitar la cuenta FTP en la base de datos
  derror = servicio.deshabilitarFTPDB(txtUsuario, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se deshabilito la Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() 
    + " fue deshabilitada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
