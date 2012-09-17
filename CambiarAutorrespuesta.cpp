#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAutorrespuesta.h"
#include "CambiarAutorrespuesta.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioAutorrespuesta servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioAutorrespuesta"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CAMBIAR_AUTORRESPUESTA);
  string okFile(OK_CAMBIAR_AUTORRESPUESTA);

  //Verificar que la pagina haya sido llamada por CambiarAutorrespuesta.php
  if (interfaz.verificarPagina(string("CambiarAutorrespuesta.php")) < 0)
    return(-1);

  //Verificar si el cliente puede modificar el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite modificar E-Mails Autorrespuestas";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCuenta, txtNombre, txtAutorrespuesta;
  if ((interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtNombre", txtNombre, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAutorrespuesta", txtAutorrespuesta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCuenta);
  strtrimString(txtNombre);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Cuenta");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterLetraNumeroEspacio(txtNombre, "Nombre");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas la cuenta
  string txtCuentaM = lowerCase(txtCuenta);

  //Verificar si existe la autorrespuesta
  derror = servicio.verificarExisteAutorrespuestaDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Autorrespuesta para la cuenta POP3 " + txtCuenta + "@" + dominio.getDominio()
      + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Modificar la autorrespuesta en el sistema
  derror = servicio.cambiarAutorrespuesta(txtCuentaM, txtNombre, txtAutorrespuesta, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Modificar la autorrespuesta en la base de datos
  derror = servicio.cambiarAutorrespuestaDB(txtCuentaM, txtNombre, txtAutorrespuesta, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se modifico la Autorrespuesta para la cuenta POP3 " + txtCuenta + "@" 
    + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Autorrespuesta para la cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() 
    + " fue modificada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
