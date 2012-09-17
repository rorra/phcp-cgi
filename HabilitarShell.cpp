#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioShell.h"
#include "HabilitarShell.h"

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
  string errorFile(ERROR_HABILITAR_SHELL);
  string okFile(OK_HABILITAR_SHELL);

  //Verificar que la pagina haya sido llamada por HabilitarShell.php
  if (interfaz.verificarPagina(string("HabilitarShell.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite habilitar la Cuenta Shell";
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

  //Verificar si la cuenta Shell se encuentra deshabilitada
  derror = servicio.verificarDeshabilitadaShellDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta Shell no se encuentra deshabilitada";
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
  txtPassword = decodificarPassword(txtPassword);

  //Habilitar la cuenta Shell en el sistema
  derror = servicio.cambiarClaveShell(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Habilitar la cuenta Shell en la base de datos
  derror = servicio.habilitarShellDB(txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se habilito la Cuenta Shell";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta Shell fue habilitada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
