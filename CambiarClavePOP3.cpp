#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioPOP3.h"
#include "CambiarClavePOP3.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioPOP3 servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPOP3"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CAMBIAR_CLAVE_POP3);
  string okFile(OK_CAMBIAR_CLAVE_POP3);

  //Verificar que la pagina haya sido llamada por CambiarClavePOP3.php
  if (interfaz.verificarPagina(string("CambiarClavePOP3.php")) < 0)
    return(-1);

  //Verificar si el cliente puede cambiar la clave del servicio
  /*derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite modificar Cuentas POP3";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }*/

  //Obtener las variables
  string txtCuenta, txtPassword, txtPassword2;
  if ((interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio)) < 0) {
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
  strtrimString(txtCuenta);
  strtrimString(txtPassword);
  strtrimString(txtPassword2);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Email");
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

  //Pasar a minusculas la cuenta POP3
  string txtCuentaM = lowerCase(txtCuenta);

  //Verificar si existe la cuenta POP3 que se quiere crear
  derror = servicio.verificarExisteEmailDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Cambiar el password en el sistema
  derror = servicio.cambiarClavePOP3(txtCuentaM, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Cambiar la cuenta POP3 en la base de datos
  derror = servicio.cambiarClavePOP3DB(txtCuentaM, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se actualizó el password de la Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La clave de la Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio()
    + " fue cambiada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
