#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTP.h"
#include "servicioSubdominio.h"
#include "servicioProteccionListado.h"
#include "EliminarProteccionListado.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioProteccionListado servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionListado"));
  cServicioFTP servicioFTP(&dominio);
  if (!servicioFTP.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objecto servicioFTP"));
  cServicioSubdominio servicioSubdominio(&dominio);
  if (!servicioSubdominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objecto servicioSubdominio"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_PROTECCION_LISTADO);
  string okFile(OK_ELIMINAR_PROTECCION_LISTADO);

  //Verificar que la pagina haya sido llamada por EliminarProteccionListado.php
  if (interfaz.verificarPagina(string("EliminarProteccionListado.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Protecciones de Listado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtDirectorio, optBorrarDirectorio;
  if ((interfaz.obtenerVariable("txtDirectorio", txtDirectorio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //No verificar error debido a que puede no ingresarse ninguna variable optBorrarDirectorio
  interfaz.obtenerVariable("optBorrarDirectorio", optBorrarDirectorio, dominio);
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtDirectorio);
  strtrimString(optBorrarDirectorio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDirectorioL(txtDirectorio, "Directorio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la proteccion de listado ya existe
  derror = servicio.verificarExisteProteccionListadoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Proteccion de Listado para el directorio " + txtDirectorio + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Si esta marcado borrar el directorio y si el directorio pertenece a una cuenta FTP
  // o a un subdominio abortar
  if (optBorrarDirectorio.length() > 0) {
    if ((servicioSubdominio.verificarExisteSubdominioDB(txtDirectorio, dominio)) > 0) {
      error = "El directorio " + txtDirectorio + " no puede ser eliminado porque pertenece al subdominio " + txtDirectorio + "." + dominio.getDominio();
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
    }
    if ((servicioFTP.verificarExisteDirectorioDB(txtDirectorio, dominio)) > 0) {
      error = "El directorio " + txtDirectorio + " no puede ser eliminado porque pertenece a una cuenta FTP";
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
    }
  }

  //Elimina la proteccion de listado del sistema
  derror = servicio.quitarProteccionListado(txtDirectorio, dominio, optBorrarDirectorio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la proteccion de listado en la base de datos
  derror = servicio.quitarProteccionListadoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino la Protección de Listado para el directorio " +  txtDirectorio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Protección de Listado para el directorio " + txtDirectorio
    + " fue eliminada con éxito";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
