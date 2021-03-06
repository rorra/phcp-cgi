#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTPAnonimo.h"
#include "CambiarMensajeFTPA.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioFTPAnonimo servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioFTPAnonimo"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CAMBIAR_MENSAJE_FTP_A);
  string okFile(OK_CAMBIAR_MENSAJE_FTP_A);

  //Verificar que la pagina haya sido llamada por CambiarMensajeFTPA.php
  if (interfaz.verificarPagina(string("CambiarMensajeFTPA.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite modificar la Cuenta FTP Anonima";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtMensaje;
  if ((interfaz.obtenerVariable("txtMensaje", txtMensaje, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Limpiar la cadena
  strtrimString(txtMensaje);

  //Verificar si existe la cuenta FTP Anonima que se quiere modificar
  derror = servicio.verificarExisteFTPADB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta FTP Anonima no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Cambiar el mensaje de la cuenta FTP Anonima en el sistema
  derror = servicio.cambiarMensajeFTPA(txtMensaje, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Cambiar el mensaje de la cuenta FTP Anonima en la base de datos
  derror = servicio.cambiarMensajeFTPADB(txtMensaje, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se cambio el Mensaje de Bienvenida de la Cuenta FTP Anonima";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Mensaje de Bienvenida de la Cuenta FTP Anonima fue cambiado con �xito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
