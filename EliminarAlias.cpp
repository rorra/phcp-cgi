#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAlias.h"
#include "EliminarAlias.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioAlias servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioAlias"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_ALIAS);
  string okFile(OK_ELIMINAR_ALIAS);

  //Verificar que la pagina haya sido llamada por EliminarAlias.php
  if (interfaz.verificarPagina(string("EliminarAlias.php")) < 0)
    return(-1);

  //Verificar si el cliente puede borrar el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Cuentas E-Mails Alias";
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
    error = "Su Plan no le permite eliminar Cuentas E-Mail Alias";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas la cuenta alias
  string txtCuentaM = lowerCase(txtCuenta);

  //Verificar si el alias ya existe
  derror = servicio.verificarExisteAliasDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta E-Mail Alias " + txtCuenta + "@" + dominio.getDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar la cuenta alias
  std::string alias = servicio.recuperarAliasDB(txtCuentaM, dominio);
  if (alias.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta alias del sistema
  derror = servicio.quitarAlias(txtCuentaM, alias, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta alias en la base de datos
  derror = servicio.quitarAliasDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino la Cuenta E-Mail Alias " + txtCuenta + "@" + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta E-Mail Alias " + txtCuenta + "@" + dominio.getDominio()
    + " fue eliminada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
