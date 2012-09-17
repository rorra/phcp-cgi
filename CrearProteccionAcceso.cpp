#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioProteccionListado.h"
#include "servicioProteccionAcceso.h"
#include "servicioSubdominio.h"
#include "servicioFTP.h"
#include "CrearProteccionAcceso.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioProteccionAcceso servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionAcceso"));
  cServicioProteccionListado servicioListado(&dominio);
  if (!servicioListado.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionListado"));
  cServicioSubdominio servicioSubdominio(&dominio);
  if (!servicioSubdominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objecto servicioSubdominio"));
  cServicioFTP servicioFTP(&dominio);
  if (!servicioFTP.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objecto servicioFTP"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_PROTECCION_ACCESO);
  string okFile(OK_CREAR_PROTECCION_ACCESO);

  //Verificar que la pagina haya sido llamada por CrearProteccionAcceso.php
  if (interfaz.verificarPagina(string("CrearProteccionAcceso.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Protecciones de Acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Protecciones de Acceso permitidos para su Plan";
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

  //Verificar si la proteccion de acceso ya existe
  derror = servicio.verificarExisteProteccionAccesoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Proteccion de Acceso para el directorio " + txtDirectorio + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la proteccion de acceso no existe como una proteccion de listado
  derror = servicioListado.verificarExisteProteccionListadoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "Existe la Proteccion de Listado para el directorio " + txtDirectorio;
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

  //Crear la proteccion de acceso en el sistema
  derror = servicio.agregarProteccionAcceso(txtDirectorio, dominio, optBorrarDirectorio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear la proteccion de acceso en la base de datos
  derror = servicio.agregarProteccionAccesoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo la Protección de Acceso para el directorio " +  txtDirectorio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Protección de Acceso para el directorio " + txtDirectorio
    + " fue creada con éxito";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
