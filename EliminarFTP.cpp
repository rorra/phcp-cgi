#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioProteccionListado.h"
#include "servicioProteccionAcceso.h"
#include "servicioFTP.h"
#include "EliminarFTP.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioFTP servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioFTP"));
  cServicioProteccionListado servicioProteccionL(&dominio);
  if (!servicioProteccionL.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionListado"));
  cServicioProteccionAcceso servicioProteccionA(&dominio);
  if (!servicioProteccionA.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioProteccionAcceso"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  string txtDirectorio;
  int derror;
  string errorFile(ERROR_ELIMINAR_FTP);
  string okFile(OK_ELIMINAR_FTP);

  //Verificar que la pagina haya sido llamada por EliminarFTP.php
  if (interfaz.verificarPagina(string("EliminarFTP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite eliminar Cuentas FTP Extas";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtUsuario;
  if ((interfaz.obtenerVariable("txtUsuario", txtUsuario, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtUsuario);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterNumeroLetra(txtUsuario, "Usuario");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la cuenta FTP que se quiere eliminar
  derror = servicio.verificarExisteFTPDB(txtUsuario, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror == 0) {
    error = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() + " no existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Recuperar el directorio de la cuenta FTP
  txtDirectorio = servicio.recuperarDirectorioDB(txtUsuario, dominio);
  if (txtDirectorio.length() <= 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Verificar si el directorio pertenece a una proteccion de acceso
  if ((servicioProteccionA.verificarExisteProteccionAccesoDB(txtDirectorio, dominio)) > 0) {
    error = "El directorio " + txtDirectorio + " posee una proteccion de acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio pertenece a una proteccion de listado
  if ((servicioProteccionL.verificarExisteProteccionListadoDB(txtDirectorio, dominio)) > 0) {
    error = "El directorio " + txtDirectorio + " pose una proteccion de listado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Eliminar la cuenta FTP del sistema
  derror = servicio.quitarFTP(txtUsuario, txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Eliminar la cuenta FTP en la base de datos
  derror = servicio.quitarFTPDB(txtUsuario, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se Elimino la Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() 
    + " fue eliminada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
