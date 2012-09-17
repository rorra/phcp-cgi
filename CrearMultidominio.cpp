#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioRedireccionamiento.h"
#include "servicioDominio.h"
#include "servicioRedireccionamientoExt.h"
#include "servicioRedireccionamientoSub.h"
#include "servicioMultidominio.h"
#include "CrearMultidominio.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioMultidominio servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioMultidominio"));
  cServicioDominio servicioDom(&dominio);
  if (servicioDom.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioDominio"));
  cServicioRedireccionamiento servicioRed(&dominio);
  if (!servicioRed.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamiento"));
  cServicioRedireccionamientoExt servicioExt(&dominio);
  if (!servicioExt.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoExt"));
  cServicioRedireccionamientoSub servicioSub(&dominio);
  if (!servicioSub.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamientoSub"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_MULTIDOMINIO);
  string okFile(OK_CREAR_MULTIDOMINIO);

  //Verificar que la pagina haya sido llamada por CrearMultidominio.php
  if (interfaz.verificarPagina(string("CrearMultidominio.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Multidominios";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Dominios Múltiples permitidos para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtMultidominio;
  if ((interfaz.obtenerVariable("txtMultidominio", txtMultidominio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtMultidominio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDominio(txtMultidominio, "Multidominio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas el redireccionamiento
  string txtMultidominioM = lowerCase(txtMultidominio);

  //Verificar si el multidominio ya existe
  derror = servicio.verificarExisteMultidominioDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Dominio Múltiple " + txtMultidominio + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el multidominio existe como un dominio
  derror = servicioDom.verificarExisteDominioDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Dominio Múltiple " + txtMultidominio + " ya existe como un Dominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe como redireccionamiento externo
  derror = servicioExt.verificarExisteRedireccionamientoExtDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtMultidominioM
      + " ya existe como un Redireccionamiento Externo";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe como redireccionamiento a subdominio
  derror = servicioSub.verificarExisteRedireccionamientoSubDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtMultidominio
      + " ya existe como un Redireccionamiento a Subdominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento ya existe como un redireccionamiento
  derror = servicioRed.verificarExisteRedireccionamientoDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtMultidominio
      + " ya existe como un Redireccionamiento";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear el multidominio en el sistema
  derror = servicio.agregarMultidominio(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el multidominio en la base de datos
  derror = servicio.agregarMultidominioDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Dominio Múltiple " + txtMultidominio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Dominio Múltiple " + txtMultidominio + " fue creado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
