#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTPAdmin.h"
#include "CambiarClaveFTPAdmin.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioFTPAdmin servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioFTPAdmin"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CAMBIAR_CLAVE_FTP_ADMIN);
  string okFile(OK_CAMBIAR_CLAVE_FTP_ADMIN);

  //Verificar que la pagina haya sido llamada por CambiarClaveFTPAdmin.php
  if (interfaz.verificarPagina(string("CambiarClaveFTPAdmin.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
 /* derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite cambiar la clave de la Cuenta FTP Principal";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }*/

  //Obtener las variables
  string txtPassword, txtPassword2;
  if ((interfaz.obtenerVariable("txtPassword", txtPassword, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtPassword2", txtPassword2, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtPassword);
  strtrimString(txtPassword2);

  //Verificar los caracteres validos para los campos
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

  //Cambiar el password FTP del administrador en el sistema
  derror = servicio.cambiarClaveFTPAdmin(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Cambiar el password FTP del administrador en la base de datos
  derror = servicio.cambiarClaveFTPAdminDB(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se actualizó el password la Cuenta FTP Principal";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La clave de la Cuenta FTP Principal " + dominio.getUsuario() + " fue cambiada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
