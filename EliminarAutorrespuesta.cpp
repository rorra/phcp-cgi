#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAutorrespuesta.h"
#include "CrearAutorrespuesta.h"

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
  string errorFile(ERROR_ELIMINAR_AUTORRESPUESTA);
  string okFile(OK_ELIMINAR_AUTORRESPUESTA);

  //Verificar que la pagina haya sido llamada por EliminarAutorrespuesta.php
  if (interfaz.verificarPagina(string("EliminarAutorrespuesta.php")) < 0)
    return(-1);

  //Verificar si el cliente puede borrar el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar E-Mails Autorrespuestas";
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
  error = servicio.caracterUsuarioEmail(txtCuenta, "Cuenta");
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
    error = "No existe la Autorrespuesta para la Cuenta POP3 " + txtCuenta + "@"
      + dominio.getDominio();
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar la autorrespuesta del sistema
  derror = servicio.quitarAutorrespuesta(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la autorrespuesta en la base de datos
  derror = servicio.quitarAutorrespuestaDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino la Autorrespuesta para la Cuenta POP3 " + txtCuenta + "@" 
    + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Autorrespuesta para la Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() 
    + " fue eliminada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
