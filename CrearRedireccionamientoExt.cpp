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
#include "CrearRedireccionamientoExt.h"

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
  cServicioDominio servicioDom(&dominio);
  if (servicioDom.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioDominio"));
  cServicioRedireccionamiento servicioRed(&dominio);
  if (!servicioRed.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioRedireccionamiento"));
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
  string errorFile(ERROR_CREAR_REDIRECCIONAMIENTO_EXT);
  string okFile(OK_CREAR_REDIRECCIONAMIENTO_EXT);

  //Verificar que la pagina haya sido llamada por CrearRedireccionamientoExt.php
  if (interfaz.verificarPagina(string("CrearRedireccionamientoExt.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Redireccionamientos Externos";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de cuentas de Redireccionamientos Externos "
      "permitidos para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtRedireccionamiento, txtURL;
  if ((interfaz.obtenerVariable("txtRedireccionamiento", txtRedireccionamiento, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtURL", txtURL, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtRedireccionamiento);
  strtrimString(txtURL);

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
  if (derror > 0) {
    error = "El Redireccionamiento Externo " + txtRedireccionamiento + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el redireccionamiento externo existe como un dominio
  derror = servicioDom.verificarExisteDominioDB(txtRedireccionamientoM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento Externo " + txtRedireccionamiento
      + " ya existe como un Dominio";
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
    error = "El Redireccionamiento " + txtRedireccionamiento + " ya existe como un Dominio Multiple";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Agregar el redireccionamiento externo en el sistema
  derror = servicio.agregarRedireccionamientoExt(txtRedireccionamientoM, txtURL, dominio);

  //Crear el redireccionamiento externo en la base de datos
  derror = servicio.agregarRedireccionamientoExtDB(txtRedireccionamientoM, txtURL,
						   dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Redireccionamiento Externo " + txtRedireccionamiento + " a " + txtURL;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento Externo " + txtRedireccionamiento + " fue creado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
