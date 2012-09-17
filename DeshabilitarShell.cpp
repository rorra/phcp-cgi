#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioShell.h"
#include "DeshabilitarShell.h"

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
  string errorFile(ERROR_DESHABILITAR_SHELL);
  string okFile(OK_DESHABILITAR_SHELL);

  //Verificar que la pagina haya sido llamada por DeshabilitarShell.php
  if (interfaz.verificarPagina(string("DeshabilitarShell.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite Deshabilitar la Cuenta Shell";
    interfaz.reportarErrorComando(error, errorFile, dominio);
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

  //Recuperar el password de la cuenta Shell
  std::string txtPassword = servicio.recuperarPasswordDB(dominio);
  if (txtPassword.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Decodificar el password
  txtPassword = codificarPassword(txtPassword);

  //Deshabilitar la cuenta Shell en el sistema
  derror = servicio.cambiarClaveShell(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Deshabilitar la cuenta Shell en la base de datos
  derror = servicio.deshabilitarShellDB(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se deshabilito la Cuenta Shell";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta Shell fue deshabilitada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
