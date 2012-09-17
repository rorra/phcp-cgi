#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioSubdominio.h"
#include "servicioFTP.h"
#include "servicioProteccionAcceso.h"
#include "servicioProteccionListado.h"
#include "CrearSubdominio.h"

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
  cServicioFTP servicioFTP(&dominio);
  if (!servicioFTP.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioFTP"));
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
  string errorFile(ERROR_CREAR_SUBDOMINIO);
  string okFile(OK_CREAR_SUBDOMINIO);

  //Verificar que la pagina haya sido llamada por CrearSubdominio.php
  if (interfaz.verificarPagina(string("CrearSubdominio.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Subdominios";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Subdominios permitidos para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtSubdominio, optBorrarDirectorio;
  if ((interfaz.obtenerVariable("txtSubdominio", txtSubdominio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  interfaz.obtenerVariable("optBorrarDirectorio", optBorrarDirectorio, dominio);

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtSubdominio);
  strtrimString(optBorrarDirectorio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDirectorio(txtSubdominio, "Subdominio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el subdominio es el directorio "errores"
  if (lowerCase(txtSubdominio) == std::string("errores")) {
    error = "No se puede crear un subdominio llamado errores";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe el subdominio que se quiere crear
  derror = servicio.verificarExisteSubdominioDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Subdominio " + txtSubdominio + "." + dominio.getDominio() + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio que se quiere crear existe para un subdominio
  derror = servicioFTP.verificarExisteDirectorioDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El directorio " + txtSubdominio + " pertenece a una Cuenta FTP Extra";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio existe como una proteccion de acceso
  derror = servicioProteccionA.verificarExisteProteccionAccesoDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El directorio " + txtSubdominio + " existe como una proteccion de acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio existe como una proteccion de listado
  derror = servicioProteccionL.verificarExisteProteccionListadoDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El directorio " + txtSubdominio + " existe como una proteccion de listado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Si no esta seteado borrar directorio y existe el directorio, devolver el error
  std::string dir;
  dir = "/www/docs/" + dominio.getDominio() + "/public_html/" + txtSubdominio;
  if (optBorrarDirectorio.length() <= 0)
    if (existeDirectorio(dir)) {
      error = "Existe el directorio " + txtSubdominio;
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
    }

  //Verificar que el directorio no sea un directorio indispensable del dominio
  //Si el directorio es igual a alguno de las extensiones del frontpage detener
  if (txtSubdominio == "icons")
    error = "No puede elegir el Subdominio icons";
  if (txtSubdominio == "images")
    error = "No puede elegir el Subdominio images";
  if (txtSubdominio == "_private")
    error = "No puede elegir el Subdominio _private";
  if (txtSubdominio == "_vti_bin")
    error = "No puede elegir el Subdominio _vti_bin";
  if (txtSubdominio == "_vti_cnf")
    error = "No puede elegir el Subdominio _vti_cnf";
  if (txtSubdominio == "_vti_log")
    error = "No puede elegir el Subdominio _vti_log";
  if (txtSubdominio == "_vti_pvt")
    error = "No puede elegir el Subdominio _vti_pvt";
  if (txtSubdominio == "_vti_txt")
    error = "No puede elegir el Subdominio _vti_txt";
  if (txtSubdominio == ".frontpage")
    error = "No puede elegir el Subdominio .frontpage";
  if (txtSubdominio == "postinfo.html")
    error = "No puede elegir el Subdominio postinfo.html";
  if (txtSubdominio == "_vti_inf.html")
    error = "No puede elegir el directorio _vti_inf.html";
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear el subdominio en el sistema
  derror = servicio.agregarSubdominio(txtSubdominio, dominio, optBorrarDirectorio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el subdominio en la base de datos
  derror = servicio.agregarSubdominioDB(txtSubdominio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Subdominio " + txtSubdominio + "." + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Subdominio " + txtSubdominio + "." + dominio.getDominio() 
    + " fue creado con éxtio.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
