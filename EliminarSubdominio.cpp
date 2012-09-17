#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioProteccionAcceso.h"
#include "servicioProteccionListado.h"
#include "servicioSubdominio.h"
#include "EliminarSubdominio.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioSubdominio servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioSubdominio"));
  cServicioProteccionListado servicioProteccionL(&dominio);
  if (!servicioProteccionL.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionListado"));
  cServicioProteccionAcceso servicioProteccionA(&dominio);
  if (!servicioProteccionA.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionAcceso"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_SUBDOMINIO);
  string okFile(OK_ELIMINAR_SUBDOMINIO);

  //Verificar que la pagina haya sido llamada por EliminarSubdominio.php
  if (interfaz.verificarPagina(string("EliminarSubdominio.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Subdominios";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtSubdominio;
  if ((interfaz.obtenerVariable("txtSubdominio", txtSubdominio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtSubdominio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDirectorio(txtSubdominio, "Subdominio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe el subdominio que se quiere eliminar
  derror = servicio.verificarExisteSubdominioDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El Subdominio " + txtSubdominio + "." + dominio.getDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio pertenece a una proteccion de acceso
  if ((servicioProteccionA.verificarExisteProteccionAccesoDB(txtSubdominio, dominio)) > 0) {
    error = "El directorio " + txtSubdominio + " posee una proteccion de acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio pertenece a una proteccion de listado
  if ((servicioProteccionL.verificarExisteProteccionListadoDB(txtSubdominio, dominio)) > 0) {
    error = "El directorio " + txtSubdominio + " pose una proteccion de listado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar el subdominio del sistema
  derror = servicio.quitarSubdominio(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar el subdominio en la base de datos
  derror = servicio.quitarSubdominioDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino el Subdominio " + txtSubdominio + "." + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Subdominio " + txtSubdominio + "." + dominio.getDominio() 
    + " fue eliminado con éxtio.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
