#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTP.h"
#include "CambiarClaveFTP.h"

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
  string errorFile(ERROR_CAMBIAR_CLAVE_FTP);
  string okFile(OK_CAMBIAR_CLAVE_FTP);

  //Verificar que la pagina haya sido llamada por CambiarClaveFTP.php
  if (interfaz.verificarPagina(string("CambiarClaveFTP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  /*derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite modificar Cuentas FTP Extras";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }*/

  //Obtener las variables
  string txtUsuario, txtPassword, txtPassword2;
  if ((interfaz.obtenerVariable("txtUsuario", txtUsuario, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtPassword", txtPassword, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtPassword2", txtPassword2, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtUsuario);
  strtrimString(txtPassword);
  strtrimString(txtPassword2);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterNumeroLetra(txtUsuario, "Usuario");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterPassword(txtPassword, "Password");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  //Verificar que los passwords sean iguales
  if (txtPassword != txtPassword2) {
    interfaz.reportarErrorComando("Los passwords deben ser identicos", errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la cuenta FTP que se quiere modificar
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

  //Cambiar la clave de la cuenta FTP en el sistema
  derror = servicio.cambiarClaveFTP(txtUsuario, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Modificar la cuenta FTP en la base de datos
  derror = servicio.cambiarClaveFTPDB(txtUsuario, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se modifico la clave de la Cuenta FTP ftp" + txtUsuario + "-"
    + dominio.getIdDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La clave de la Cuenta FTP ftp" + txtUsuario + "-" + dominio.getIdDominio() 
    + " fue creada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
