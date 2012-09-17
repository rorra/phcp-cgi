#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioDominio.h"
#include "servicioRedireccionamiento.h"
#include "servicioRedireccionamientoExt.h"
#include "servicioRedireccionamientoSub.h"
#include "servicioMultidominio.h"
#include "CrearRedireccionamientoSub.h"

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
  cServicioDominio servicioDom(&dominio);
  if (servicioDom.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioDominio"));
  cServicioRedireccionamientoExt servicioExt(&dominio);
  if (!servicioExt.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoExt"));
  cServicioRedireccionamiento servicioRed(&dominio);
  if (!servicioRed.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoSub"));
  cServicioMultidominio servicioMultidominio(&dominio);
  if (!servicioMultidominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioMultidominio"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_REDIRECCIONAMIENTO_SUB);
  string okFile(OK_CREAR_REDIRECCIONAMIENTO_SUB);

  //Verificar que la pagina haya sido llamada por CrearRedireccionamientoSub.php
  if (interfaz.verificarPagina(string("CrearRedireccionamientoSub.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Redireccionamientos a Subdominios";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de cuentas de Redireccionamientso a Subdominios "
      "permitidos para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtRedireccionamiento, txtSubdominio;
  if ((interfaz.obtenerVariable("txtRedireccionamiento", txtRedireccionamiento, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtSubdominio", txtSubdominio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtRedireccionamiento);
  strtrimString(txtSubdominio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDominio(txtRedireccionamiento, "Redireccionamiento");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterDirectorio(txtSubdominio, "Subdominio");
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
  if (derror > 0) {
    error = "El Redireccionamiento a Subdominio " + txtRedireccionamiento + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento existe como un dominio
  derror = servicioDom.verificarExisteDominioDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtRedireccionamiento + " ya existe como un Dominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe como redireccionamiento externo
  derror = servicioExt.verificarExisteRedireccionamientoExtDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtRedireccionamiento
      + " ya existe como un Redireccionamiento Externo";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe como redireccionamiento
  derror = servicioRed.verificarExisteRedireccionamientoDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtRedireccionamiento 
      + " ya existe como un Redireccionamiento";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe como un multidominio
  derror = servicioMultidominio.verificarExisteMultidominioDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtRedireccionamiento + " ya existe como un Multidominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear el redireccionamiento a subdominio en el sistema
  derror = servicio.agregarRedireccionamientoSub(txtRedireccionamientoM, txtSubdominio,
						 dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el redireccionamiento a subdominio en la base de datos
  derror = servicio.agregarRedireccionamientoSubDB(txtRedireccionamientoM, txtSubdominio,
						   dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Redireccionamiento " + txtRedireccionamiento + " al Subdominio "
    + txtSubdominio + "." + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento " + txtRedireccionamiento + " al subdominio "
    + txtSubdominio + "." + dominio.getDominio() + " fue creado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
