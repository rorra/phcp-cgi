#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioMultidominio.h"
#include "EliminarMultidominio.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioMultidominio servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioMultidominio"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_MULTIDOMINIO);
  string okFile(OK_ELIMINAR_MULTIDOMINIO);

  //Verificar que la pagina haya sido llamada por EliminarMultidominio.php
  if (interfaz.verificarPagina(string("EliminarMultidominio.php")) < 0)
    return(-1);

  //Verificar si el cliente puede eliminar el servicio
      derror = servicio.verificarCrearServicio();
      if (derror == 0) {
	    error = "Su Plan no le permite eliminar Dominios Multiples";
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
  }
                              
  //Obtener las variables
  string txtMultidominio;
  if ((interfaz.obtenerVariable("txtMultidominio", txtMultidominio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtMultidominio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDominio(txtMultidominio, "Multidominio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Pasar a minusculas el redireccionamiento
  string txtMultidominioM = lowerCase(txtMultidominio);

  //Verificar si el multidominio ya existe
  derror = servicio.verificarExisteMultidominioDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El Dominio Múltiple " + txtMultidominio + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar el multidominio del sistema
  derror = servicio.quitarMultidominio(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar el multidominio en la base de datos
  derror = servicio.quitarMultidominioDB(txtMultidominioM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino el Dominio Múltiple " + txtMultidominio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Dominio Múltiple " + txtMultidominio + " fue eliminado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
