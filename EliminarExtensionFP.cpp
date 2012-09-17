#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioExtensionFP.h"
#include "EliminarExtensionFP.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioExtensionFP servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioExtensionFP"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_EXTENSION_FP);
  string okFile(OK_ELIMINAR_EXTENSION_FP);

  //Verificar que la pagina haya sido llamada por EliminarExtensionFP.php
  if (interfaz.verificarPagina(string("EliminarExtensionFP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite desinstalar las Extensiones de FrontPage";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existen las extensiones FP que se quiere eliminar
  derror = servicio.verificarExisteExtensionFPDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "Las Extensiones de FrontPage no se encuentran instaladas";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar las extensiones del Front Page en la base de datos
  derror = servicio.quitarExtensionFP(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar las extensiones del Front Page en la base de datos
  derror = servicio.quitarExtensionFPDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se eliminaron las Extensiones de FrontPage";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "Las Extensiones de FrontPage fueron desinstaladas con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
