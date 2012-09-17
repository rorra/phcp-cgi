#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioRedireccionamientoExt.h"
#include "EliminarRedireccionamientoExt.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioRedireccionamientoExt servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoExt"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_REDIRECCIONAMIENTO_EXT);
  string okFile(OK_ELIMINAR_REDIRECCIONAMIENTO_EXT);

  //Verificar que la pagina haya sido llamada por EliminarRedireccionamientoExt.php
  if (interfaz.verificarPagina(string("EliminarRedireccionamientoExt.php")) < 0)
    return(-1);

  //Obtener las variables
  string txtRedireccionamiento;
  if ((interfaz.obtenerVariable("txtRedireccionamiento", txtRedireccionamiento, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtRedireccionamiento);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDominio(txtRedireccionamiento, "Redireccionamiento");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas el redireccionamiento
  string txtRedireccionamientoM = lowerCase(txtRedireccionamiento);

  //Verificar si el redireccionamiento externo ya existe
  derror = servicio.verificarExisteRedireccionamientoExtDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El Redireccionamiento Externo " + txtRedireccionamiento + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar el redireccionamiento externo del sistema
  derror = servicio.quitarRedireccionamientoExt(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar el redireccionamiento externo en la base de datos
  derror = servicio.quitarRedireccionamientoExtDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino el Redireccionamiento Externo " + txtRedireccionamiento;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento Externo " + txtRedireccionamiento + " fue eliminado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
