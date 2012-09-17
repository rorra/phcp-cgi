#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAlias.h"
#include "servicioPOP3.h"
#include "CrearAlias.h"

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
  cServicioPOP3 servicioPOP3(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPOP3"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_ALIAS);
  string okFile(OK_CREAR_ALIAS);

  //Verificar que la pagina haya sido llamada por CrearAlias.php
  if (interfaz.verificarPagina(string("CrearAlias.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Cuentas E-Mails Alias";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Cuentas de E-Mail Alias permitidas para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCuenta, txtAlias;
  if ((interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAlias", txtAlias, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCuenta);
  strtrimString(txtAlias);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Alias");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterEmail(txtAlias, "E-Mail");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas la cuenta alias y la cuenta de correo
  string txtCuentaM = lowerCase(txtCuenta);
  string txtAliasM = lowerCase(txtAlias);

  //Verificar si el alias ya existe
  derror = servicio.verificarExisteAliasDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Cuenta Alias " + txtCuenta + "@" + dominio.getDominio() + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe el alias como una cuenta POP3
  derror = servicioPOP3.verificarExisteEmailDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Cuenta " + txtCuenta + "@" + dominio.getDominio()
      + " ya existe como una Cuenta POP3";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear la cuenta alias en el sistema
  derror = servicio.agregarAlias(txtAliasM, txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear la cuenta alias en la base de datos
  derror = servicio.agregarAliasDB(txtAliasM, txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo la Cuenta E-Mail Alias " + txtCuenta + "@" + dominio.getDominio()
    + " para la cuenta de E-Mail " + txtAlias;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta E-Mail Alias " + txtCuenta + "@" + dominio.getDominio()
    + " fue creada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
