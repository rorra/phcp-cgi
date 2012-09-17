#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioCatchAll.h"
#include "HabilitarCatchAll.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioCatchAll servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioCatchAll"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_HABILITAR_CATCHALL);
  string okFile(OK_HABILITAR_CATCHALL);

  //Verificar que la pagina haya sido llamada por HabilitarCatchAll.php
  if (interfaz.verificarPagina(string("HabilitarCatchAll.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite habilitar la Cuenta Catch All";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la cuenta CatchAll que se quiere habilitar
  derror = servicio.verificarExisteCatchAllDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta Catch All no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la cuenta CatchAll se encuentra deshabilitada
  derror = servicio.verificarDeshabilitadaCatchAllDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta Catch All no se encuentra deshabilitada";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Habilitar la cuenta Shell en el sistema
    derror = servicio.agregarFileVirtusertable( dominio);
    if (derror < 0) {
      interfaz.reportarErrorFatal();
      return(-1);
    }

  //Habilitar la cuenta CatchAll en la base de datos
  derror = servicio.habilitarCatchAllDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se habilito la Cuenta Catch All";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta Catch All fue habilitada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
