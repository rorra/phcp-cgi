#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAutorrespuesta.h"
#include "servicioPOP3.h"
#include "CrearAutorrespuesta.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioAutorrespuesta servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioAutorrespuesta"));
  cServicioPOP3 servicioPOP3(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPOP3"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_AUTORRESPUESTA);
  string okFile(OK_CREAR_AUTORRESPUESTA);

  //Verificar que la pagina haya sido llamada por CrearAutorrespuesta.php
  if (interfaz.verificarPagina(string("CrearAutorrespuesta.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear E-Mails Autorrespuestas";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de E-Mails Autorrespuestas permitidas para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCuenta, txtNombre, txtAutorrespuesta;
  interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio);
  if ((interfaz.obtenerVariable("txtNombre", txtNombre, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAutorrespuesta", txtAutorrespuesta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCuenta);
  strtrimString(txtNombre);
  strtrimString(txtAutorrespuesta);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Cuenta");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterLetraNumeroEspacio(txtNombre, "Nombre");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas la cuenta
  string txtCuentaM = lowerCase(txtCuenta);

  //Verificar si existe la cuenta POP3 que se quiere crear
  derror = servicioPOP3.verificarExisteEmailDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la cuenta POP3 ya posee una autorrespuesta
  derror = servicio.verificarExisteAutorrespuestaDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() + 
      " ya posee una Autorrespuesta";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear la autorrespuesta en el sistema
  derror = servicio.agregarAutorrespuesta(txtCuentaM, txtNombre, txtAutorrespuesta, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  
  //Crear la autorrespuesta en la base de datos
  derror = servicio.agregarAutorrespuestaDB(txtCuentaM, txtNombre, txtAutorrespuesta, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo la Autorrespuesta para la Cuenta POP3 " + txtCuenta + "@" 
    + dominio.getDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Autorrespuesta para la Cuenta POP3 " + txtCuenta + "@" + dominio.getDominio() 
    + " fue creada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
