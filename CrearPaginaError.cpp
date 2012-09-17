#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioPaginaError.h"
#include "CrearPaginaError.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioPaginaError servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPaginaError"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_PAGINA_ERROR);
  string okFile(OK_CREAR_PAGINA_ERROR);

  //Verificar que la pagina haya sido llamada por CrearPaginaError.php
  if (interfaz.verificarPagina(string("CrearPaginaError.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Paginas de Error";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Paginas de Error permitidas para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCodigo;
  if ((interfaz.obtenerVariable("txtCodigo", txtCodigo, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCodigo);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterNumero(txtCodigo, "Codigo");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe el codigo de error
  derror = servicio.verificarExisteCodigoErrorDB(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El codigo " + txtCodigo + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la pagina de error
  derror = servicio.verificarExistePaginaErrorDB(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Pagina de Error " + txtCodigo + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existen paginas de error para el dominio
  //si no existen y existen el directorio reportar el error
  std::string dir = "/www/docs/" + dominio.getDominio() + "/public_html/errores";
  if (!servicio.verificarExistenPaginaErrorDB(dominio))
    if(existeDirectorio(dir)) {
      error = "El directorio errores donde se alojan las paginas de errores existe";
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
    }

  //Crear la pagina de error en el sistema
  derror = servicio.agregarPaginaError(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear la pagina de error en la base de datos
  derror = servicio.agregarPaginaErrorDB(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo la Pagina de Error " + txtCodigo;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Pagina de Error " + txtCodigo + " fue creada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
