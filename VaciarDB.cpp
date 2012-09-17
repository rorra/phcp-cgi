#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioDB.h"
#include "CrearDB.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioDB servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPOP3"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_VACIAR_DB);
  string okFile(OK_VACIAR_DB);

  //Verificar que la pagina haya sido llamada por VaciarDB.php
  if (interfaz.verificarPagina(string("VaciarDB.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite vaciar Bases de Datos";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtDB;
  if ((interfaz.obtenerVariable("txtDB", txtDB, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtDB);

  //Verificar si existe la base de datos que se quiere eliminar
  derror = servicio.verificarExisteDB(txtDB, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Base de Datos " + txtDB + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Vaciar la base de datos
  derror = servicio.vaciarDB(txtDB, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se vacio la Base de Datos " + txtDB;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Base de datos " + txtDB + " fue vaciada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
