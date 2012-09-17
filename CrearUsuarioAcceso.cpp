#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioUsuarioAcceso.h"
#include "servicioProteccionAcceso.h"
#include "CrearUsuarioAcceso.h"

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
  string errorFile(ERROR_CREAR_USUARIO_ACCESO);
  string okFile(OK_CREAR_USUARIO_ACCESO);

  //Verificar que la pagina haya sido llamada por CrearUsuarioAcceso.php
  if (interfaz.verificarPagina(string("CrearUsuarioAcceso.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Usuarios de Protecciones de Acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Protecciones de Acceso permitidos para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtDirectorio, txtUsuario, txtPassword, txtPassword2;
  if ((interfaz.obtenerVariable("txtDirectorio", txtDirectorio, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
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
  strtrimString(txtDirectorio);
  strtrimString(txtUsuario);
  strtrimString(txtPassword);
  strtrimString(txtPassword2);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterDirectorio(txtDirectorio, "Directorio");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
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
  if (derror > 0) {
    error = "La Proteccion de Acceso del directorio " + txtDirectorio + " para el Usuario "
      + txtUsuario + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear el usuario de proteccion de acceso en el sistema
  derror = servicio.agregarUsuarioAcceso(txtUsuario, txtPassword, txtDirectorio, 
					 idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear el usuario de proteccion de acceso en la base de datos
  derror = servicio.agregarUsuarioAccesoDB(txtUsuario, txtPassword, idProteccionAcceso,
					   dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Usuario " + txtUsuario + " para la Proteccion de Acceso del directorio "
    + txtDirectorio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Proteccion de Acceso del directorio " + txtDirectorio + " para el Usuario "
    + txtUsuario + " fue creada con éxito";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
