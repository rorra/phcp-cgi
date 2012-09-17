#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioRedireccionamientoSub.h"
#include "EliminarRedireccionamientoSub.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioRedireccionamientoSub servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoSub"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_REDIRECCIONAMIENTO_SUB);
  string okFile(OK_ELIMINAR_REDIRECCIONAMIENTO_SUB);

  //Verificar que la pagina haya sido llamada por EliminarRedireccionamientoSub.php
  if (interfaz.verificarPagina(string("EliminarRedireccionamientoSub.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Redireccionamientos a Subdominios";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

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

  //Verificar si el redireccionamiento a subdominio ya existe
  derror = servicio.verificarExisteRedireccionamientoSubDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El Redireccionamiento a Subdominio " + txtRedireccionamiento + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar el redireccionamiento a subdominio en el sistema
  derror = servicio.quitarRedireccionamientoSub(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar el redireccionamiento a subdominio en la base de datos
  derror = servicio.quitarRedireccionamientoSubDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino el Redireccionamiento a Subdominio " + txtRedireccionamiento;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento a Subdominio " + txtRedireccionamiento 
    + " fue eliminado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
