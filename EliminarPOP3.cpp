#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioPOP3.h"
#include "servicioAutorrespuesta.h"
#include "EliminarPOP3.h"

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
  cServicioAutorrespuesta servicioAutorrespuesta(&dominio);
  if (!servicioAutorrespuesta.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioAutorrespuesta"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_POP3);
  string okFile(OK_ELIMINAR_POP3);

  //Verificar que la pagina haya sido llamada por EliminarPOP3.php
  if (interfaz.verificarPagina(string("EliminarPOP3.php")) < 0)
    return(-1);

  //Verificar si el cliente puede eliminar el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Cuentas POP3";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCuenta;
  if ((interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCuenta);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Email");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
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

  //Verificar si la cuenta POP3 posee una autorrespuesta
  derror = servicioAutorrespuesta.verificarExisteAutorrespuestaDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio()
      + " posee una Autorrespuesta";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar la cuenta POP3 del sistem
  derror = servicio.quitarEmail(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta POP3 en la base de datos
  derror = servicio.quitarEmailDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino la Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() + " fue eliminada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
