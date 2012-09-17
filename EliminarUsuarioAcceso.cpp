#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioUsuarioAcceso.h"
#include "servicioProteccionAcceso.h"
#include "EliminarUsuarioAcceso.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioUsuarioAcceso servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioUsuarioAcceso"));
  cServicioProteccionAcceso servicioProteccion(&dominio);
  if (!servicioProteccion.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionAcceso"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_ELIMINAR_USUARIO_ACCESO);
  string okFile(OK_ELIMINAR_USUARIO_ACCESO);

  //Verificar que la pagina haya sido llamada por EliminarUsuarioAccesoData.php
  if (interfaz.verificarPagina(string("EliminarUsuarioAccesoData.php")) != 1)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Usuarios de Protecciones de Acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtDirectorio, txtUsuario;
  if ((interfaz.obtenerVariable("txtDirectorio", txtDirectorio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtUsuario", txtUsuario, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtDirectorio);
  strtrimString(txtUsuario);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDirectorioL(txtDirectorio, "Directorio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterLetraNumero(txtUsuario, "Usuario");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si la proteccion de acceso ya existe
  derror = servicioProteccion.verificarExisteProteccionAccesoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Proteccion de Acceso para el directorio " + txtDirectorio + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar la identidad de la proteccion del directorio
  string idProteccionAcceso;
  idProteccionAcceso = servicioProteccion.recuperarIdProteccionAccesoDB(txtDirectorio, dominio);
  if (idProteccionAcceso.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Verificar si existe el usuario para la proteccion de acceso
  derror = servicio.verificarExisteUsuarioAccesoDB(txtUsuario, idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Proteccion de Acceso del directorio " + txtDirectorio + " para el Usuario "
      + txtUsuario + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar el usuario de proteccion de acceso del sistema
  derror = servicio.quitarUsuarioAcceso(txtUsuario, txtDirectorio, idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar el usuario de proteccion de acceso de la base de datos
  derror = servicio.quitarUsuarioAccesoDB(txtUsuario, idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se elimino el Usuario " + txtUsuario
    + " para la Proteccion de Acceso del directorio " + txtDirectorio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Proteccion de Acceso del directorio " + txtDirectorio + " para el Usuario "
    + txtUsuario + " fue eliminada con éxito";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
