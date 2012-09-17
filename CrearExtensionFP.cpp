#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioExtensionFP.h"
#include "CrearExtensionFP.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioExtensionFP servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioExtensionFP"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_EXTENSION_FP);
  string okFile(OK_CREAR_EXTENSION_FP);

  //Verificar que la pagina haya sido llamada por CrearExtensionFP.php
  if (interfaz.verificarPagina(string("CrearExtensionFP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite instalar las Extensiones de FrontPage";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtUsuario, txtPassword, txtPassword2;
  if ((interfaz.obtenerVariable("txtUsuario", txtUsuario, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtPassword", txtPassword, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtPassword2", txtPassword2, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtUsuario);
  strtrimString(txtPassword);
  strtrimString(txtPassword2);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterLetraNumero(txtUsuario, "Usuario");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterPassword(txtPassword, "Password");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  //Verificar que los passwords sean iguales
  if (txtPassword != txtPassword2) {
    interfaz.reportarErrorComando("Los passwords deben ser identicos", errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas el usuario
  string txtUsuarioM = lowerCase(txtUsuario);

  //Verificar si existen las extensiones FP que se quiere crear
  derror = servicio.verificarExisteExtensionFPDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "Las Extensiones de FrontPage ya se encuentran instaladas";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear las extensiones del Front Page en el sistema
  derror = servicio.agregarExtensionFP(txtUsuarioM, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear las extensiones del Front Page en la base de datos
  derror = servicio.agregarExtensionFPDB(txtUsuarioM, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se instalaron las Extensiones de FrontPage con el usuario " + txtUsuario 
    + " y el password " + txtPassword;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "Las Extensiones de FrontPage para el usuario " + txtUsuario + " y el password "
    + txtPassword + " fueron creadas con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
