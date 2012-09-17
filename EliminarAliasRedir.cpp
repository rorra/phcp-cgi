#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAliasRedir.h"
#include "EliminarAliasRedir.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioAliasRedir servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioAliasRedir"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_ALIAS_REDIR);
  string okFile(OK_ELIMINAR_ALIAS_REDIR);

  //Verificar que la pagina haya sido llamada por EliminarAliasRedir.php
  if (interfaz.verificarPagina(string("EliminarAliasRedir.php")) < 0)
    return(-1);

  //Verificar si el cliente puede borrar el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Redireccionamientos";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCuenta;
  if ((interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCuenta);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Alias");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el cliente puede eliminar el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Redireccionamientos";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas la cuenta alias
  string txtCuentaM = lowerCase(txtCuenta);

  //Verificar si el alias ya existe
  derror = servicio.verificarExisteAliasRedirDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El Redireccionamiento " + txtCuenta + "@" + dominio.getDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar la cuenta alias
  std::string alias = servicio.recuperarAliasRedirDB(txtCuentaM, dominio);
  if (alias.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta alias del sistema
  derror = servicio.quitarAliasRedir(txtCuentaM, alias, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta alias en la base de datos
  derror = servicio.quitarAliasRedirDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino el Redireccionamiento " + txtCuenta + "@" + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento " + txtCuenta + "@" + dominio.getDominio()
    + " fue eliminado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
