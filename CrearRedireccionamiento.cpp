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
#include "CrearRedireccionamiento.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioRedireccionamiento servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamiento"));
  cServicioDominio servicioDom(&dominio);
  if (servicioDom.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioDominio"));
  cServicioRedireccionamientoExt servicioExt(&dominio);
  if (!servicioExt.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoExt"));
  cServicioRedireccionamientoSub servicioSub(&dominio);
  if (!servicioSub.iniciar())
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
  string errorFile(ERROR_CREAR_REDIRECCIONAMIENTO);
  string okFile(OK_CREAR_REDIRECCIONAMIENTO);

  //Verificar que la pagina haya sido llamada por CrearRedireccionamiento.php
  if (interfaz.verificarPagina(string("CrearRedireccionamiento.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Redireccionamientos";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Redireccionamientos permitidos para su Plan";
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

  //Verificar que el redireccionamiento no sea el mismo que el dominio
  if (!txtRedireccionamientoM.compare(dominio.getDominio())) {
    error = "El Multidominio debe ser distinto del Dominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe
  derror = servicio.verificarExisteRedireccionamientoDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtRedireccionamiento + " ya existe";
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
    error = "El Redireccionamiento " + txtRedireccionamiento + " ya existe como un dominio";
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

  //Verificar si el redireccionamiento ya existe como redireccionamiento a subdominio
  derror = servicioSub.verificarExisteRedireccionamientoSubDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtRedireccionamiento
      + " ya existe como un Redireccionamiento a Subdominio";
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

  //Crear el redireccionamiento en el sistema
  derror = servicio.agregarRedireccionamiento(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el redireccionamiento en la base de datos
  derror = servicio.agregarRedireccionamientoDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Redireccionamiento " + txtRedireccionamiento;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento " + txtRedireccionamiento + " fue creado con �xito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
