#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioProteccionAcceso.h"
#include "servicioProteccionListado.h"
#include "servicioSubdominio.h"
#include "servicioFTP.h"
#include "CrearProteccionListado.h"

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
  cServicioProteccionAcceso servicioAcceso(&dominio);
  if (!servicioAcceso.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objecto servicioProteccionAcceso"));
  cServicioSubdominio servicioSubdominio(&dominio);
  if (!servicioSubdominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioSubdominio"));
  cServicioFTP servicioFTP(&dominio);
  if (!servicioFTP.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objecto servicioFTP"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_PROTECCION_LISTADO);
  string okFile(OK_CREAR_PROTECCION_LISTADO);

  //Verificar que la pagina haya sido llamada por CrearProteccionListado.php
  if (interfaz.verificarPagina(string("CrearProteccionListado.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Protecciones de Listado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Protecciones de Listado permitidos para su Plan";
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

  //Verificar si el directorio es "errores"
  if (lowerCase(txtDirectorio) == std::string("errores")) {
    error = "No se puede crear una proteccion para el directorio errores";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la proteccion de listado ya existe
  derror = servicio.verificarExisteProteccionListadoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Proteccion de Listado del directorio " + txtDirectorio + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe una proteccion de acceso para el directorio
  derror = servicioAcceso.verificarExisteProteccionAccesoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "Existe la Proteccion de Acceso para el directorio " + txtDirectorio;
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

  //Si no esta seteado borrar directorio y existe el directorio, devolver el error
  std::string dir;
  /*
  dir = "/www/docs/" + dominio.getDominio() + "/public_html/" + txtDirectorio;
  if (optBorrarDirectorio.length() <= 0)
    if (existeDirectorio(dir)) {
      error = "Existe el directorio " + txtDirectorio;
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
    }
  */
	
  //Verificar si existe el archivo .htaccess
  dir = "/www/docs/" + dominio.getDominio() + "/public_html/" + txtDirectorio + "/.htaccess";
  if (existeArchivo(dir)) {
    error = "Existe el archivo .htaccess en el directorio " + txtDirectorio;
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear la proteccion de listado en el sistema
  derror = servicio.agregarProteccionListado(txtDirectorio, dominio, optBorrarDirectorio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear la proteccion de listado en la base de datos
  derror = servicio.agregarProteccionListadoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo la Protección de Listado del directorio " +  txtDirectorio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Protección de Listado del directorio " + txtDirectorio
    + " fue creada con éxito";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
