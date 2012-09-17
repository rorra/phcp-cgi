#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioASP.h"
#include "servicioDB.h"
#include "HabilitarASP.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioASP servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioASP"));
  cServicioDB servicioDB(&dominio);
  if (!servicioDB.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto serivioASP"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_HABILITAR_ASP);
  string okFile(OK_HABILITAR_ASP);

  //Verificar que la pagina haya sido llamada por HabilitarASP.php
  if (interfaz.verificarPagina(string("HabilitarASP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite habilitar el Soporte ASP";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el soporte ASP ya existe
  derror = servicio.verificarExisteASPDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Soporte ASP ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existen bases de datos
  derror = servicioDB.verificarExistenDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "Su Dominio no posee Bases de Datos ODBC";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar todas las bases de datos del dominio con su respectivo password
  std::vector<std::string> nombreDB;
  std::vector<std::string> passwords;
  servicioDB.recuperarNombrePasswordDB(nombreDB, passwords, dominio);

  //Crear el soporte ASP en el sistema
  derror = servicio.agregarASP(nombreDB, passwords, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el soporte ASP en la base de datos
  derror = servicio.agregarASPDB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se habilito el Soporte ASP";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Soporte ASP fue activado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
