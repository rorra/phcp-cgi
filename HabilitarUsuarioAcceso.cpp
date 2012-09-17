#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioUsuarioAcceso.h"
#include "servicioProteccionAcceso.h"
#include "HabilitarUsuarioAcceso.h"

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
  string errorFile(ERROR_HABILITAR_USUARIO_ACCESO);
  string okFile(OK_HABILITAR_USUARIO_ACCESO);

  //Verificar que la pagina haya sido llamada por HabilitarUsuarioAcceso.php
  if (interfaz.verificarPagina(string("HabilitarUsuarioAccesoData.php")) != 1)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite habilitar Usuarios de Protecciones de Acceso";
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

  //Verificar si el usuario de proteccion de listado se encuentra deshabilitado
  derror = servicio.verificarDeshabilitadoUsuarioDB(txtUsuario, idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "El Usuario " + txtUsuario + " de la Proteccion de Acceso del directorio "
      + txtDirectorio + " no se encuentra deshabilitado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar el password de la cuenta FTP
  std::string txtPassword = servicio.recuperarPasswordDB(txtUsuario, idProteccionAcceso,
							 dominio);
  if (txtPassword.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Decodificar el password
  txtPassword = decodificarPassword(txtPassword);

  //Cambiar el password del usuario de acceso en el sistema
  derror = servicio.cambiarClaveUsuarioAcceso(txtUsuario, txtPassword, txtDirectorio,
					      idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Cambiar el password del usuario de acceso en la base de datos
  derror = servicio.cambiarClaveUsuarioAccesoDB(txtUsuario, txtPassword,
						idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Habilitar el usuario de acceso en el sistema
  derror = servicio.habilitarUsuarioAcceso(txtUsuario, idProteccionAcceso, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Habilitar el usuario de acceso en la base de datos
  derror = servicio.habilitarUsuarioAccesoDB(txtUsuario, idProteccionAcceso,
						dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se habilito el Usuario " + txtUsuario
    + " para la Proteccion de Acceso del directorio " + txtDirectorio;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Proteccion de acceso del directorio " + txtDirectorio + " para el Usuario "
    + txtUsuario + " fue habilitada con éxito";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
