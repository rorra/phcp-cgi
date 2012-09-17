#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTPAnonimo.h"
#include "EliminarFTPAnonimo.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioFTPAnonimo servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioFTPAnonimo"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_FTP_A);
  string okFile(OK_ELIMINAR_FTP_A);

  //Verificar que la pagina haya sido llamada por EliminarFTPAnonimo.php
  if (interfaz.verificarPagina(string("EliminarFTPAnonimo.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar la Cuenta FTP Anonima";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la cuenta FTP Anonima que se quiere crear
  derror = servicio.verificarExisteFTPADB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta FTP Anonima no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar la cuenta FTP Anonima del sistema
  derror = servicio.quitarFTPA(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta FTP Anonima en la base de datos
  derror = servicio.quitarFTPADB(dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino la Cuenta FTP Anonima";
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta FTP Anonima fue eliminada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
