#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioPaginaError.h"
#include "EliminarPaginaError.h"

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
  string errorFile(ERROR_ELIMINAR_PAGINA_ERROR);
  string okFile(OK_ELIMINAR_PAGINA_ERROR);

  //Verificar que la pagina haya sido llamada por EliminarPaginaError.php
  if (interfaz.verificarPagina(string("EliminarPaginaError.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Paginas de Error";
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
  error = servicio.verificarExistePaginaErrorDB(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Pagina de Error " + txtCodigo + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar la pagina de error del sistema
  derror = servicio.quitarPaginaError(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la pagina de error en la base de datos
  derror = servicio.quitarPaginaErrorDB(txtCodigo, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Si la cantidad de paginas de error es 0, borrar el directorio
  derror = servicio.verificarExistenPaginaErrorDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    std::string comando;
    comando = "rm -fr /www/docs/" + dominio.getDominio() + "/public_html/errores";
    system(comando.c_str());
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino la Pagina de Error " + txtCodigo;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Pagina de Error " + txtCodigo + " fue eliminada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
