#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioShell.h"
#include "CambiarClaveShell.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioShell servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioShell"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CAMBIAR_CLAVE_SHELL);
  string okFile(OK_CAMBIAR_CLAVE_SHELL);

  //Verificar que la pagina haya sido llamada por CambiarClaveShell.php
  if (interfaz.verificarPagina(string("CambiarClaveShell.php")) < 0)
    return(-1);

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

  //Verificar si existe la cuenta Shell que se quiere habilitar
  derror = servicio.verificarExisteShellDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta Shell no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la cuenta Shell se encuentra habilitada
  derror = servicio.verificarHabilitadaShellDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta Shell no se encuentra habilitada";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Modifica la cuenta Shell en el sistema
  derror = servicio.cambiarClaveShell(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Modificar la cuenta Shell en la base de datos
  derror = servicio.cambiarClaveShellDB(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se cambio la clave de la Cuenta Shell";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La clave de la Cuenta Shell fue modificada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
