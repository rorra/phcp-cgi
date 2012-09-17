#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioFTP.h"
#include "servicioSubdominio.h"
#include "servicioProteccionAcceso.h"
#include "servicioProteccionListado.h"
#include "CrearFTP.h"

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
  cServicioSubdominio servicioSubdominio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioSubdominio"));
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
  int derror;
  string errorFile(ERROR_CREAR_FTP);
  string okFile(OK_CREAR_FTP);

  //Verificar que la pagina haya sido llamada por CrearFTP.php
  if (interfaz.verificarPagina(string("CrearFTP.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Cuentas FTP Extras";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Cuentas FTP Extras permitidas para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtUsuario, txtDirectorio, txtPassword, txtPassword2, optBorrarDirectorio;
  if ((interfaz.obtenerVariable("txtUsuario", txtUsuario, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtDirectorio", txtDirectorio, dominio)) < 0) {
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
  //No verificar error debido a que puede no ingresarse ninguna variable optBorrarDirectorio
  interfaz.obtenerVariable("optBorrarDirectorio", optBorrarDirectorio, dominio);

  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtUsuario);
  strtrimString(txtDirectorio);
  strtrimString(txtPassword);
  strtrimString(txtPassword2);
  strtrimString(optBorrarDirectorio);

  //Verificar los caracteres validos para los campos
  error = servicio.caracterNumeroLetra(txtUsuario, "Usuario");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterDirectorio(txtDirectorio, "Directorio");
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

  //Verificar si es el directorio "errores"
  if (lowerCase(txtDirectorio) == std::string("errores")) {
    error = "No se puede crear la cuenta FTP en un directorio llamado errores";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe la cuenta FTP que se quiere crear
  derror = servicio.verificarExisteFTPDB(txtUsuario, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el subdominio coincide con el directorio de un subdominio
  derror = servicioSubdominio.verificarExisteSubdominioDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El directorio " + txtDirectorio + " pertenece a un Subdominio";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio existe como una proteccion de acceso
  derror = servicioProteccionA.verificarExisteProteccionAccesoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El directorio " + txtDirectorio + " existe como una proteccion de acceso";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si el directorio existe como una proteccion de listado
  derror = servicioProteccionL.verificarExisteProteccionListadoDB(txtDirectorio, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El directorio " + txtDirectorio + " existe como una proteccion de listado";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Si no esta seteado borrar directorio y existe el directorio, devolver el error
  std::string dir;
  dir = "/www/docs/" + dominio.getDominio() + "/public_html/" + txtDirectorio;
  if (optBorrarDirectorio.length() <= 0)
    if (existeDirectorio(dir)) {
      error = "Existe el directorio " + txtDirectorio;
      interfaz.reportarErrorComando(error, errorFile, dominio);
      return(-1);
    }

  //Verificar que el directorio no sea un directorio indispensable del dominio
  //Si el directorio es igual a alguno de las extensiones del frontpage detener
  if (txtDirectorio == "icons")
    error = "No puede elegir el directorio icons para su Cuenta FTP Extra";
  if (txtDirectorio == "images")
    error = "No puede elegir el directorio images para su Cuenta FTP Extra";
  if (txtDirectorio == "_private")
    error = "No puede elegir el directorio _private para su Cuenta FTP Extra";
  if (txtDirectorio == "_vti_bin")
    error = "No puede elegir el directorio _vti_bin para su Cuenta FTP Extra";
  if (txtDirectorio == "_vti_cnf")
    error = "No puede elegir el directorio _vti_cnf para su Ccuenta FTP Extra";
  if (txtDirectorio == "_vti_log")
    error = "No puede elegir el directorio _vti_log para su Cuenta FTP Extra";
  if (txtDirectorio == "_vti_pvt")
    error = "No puede elegir el directorio _vti_pvt para su Cuenta FTP Extra";
  if (txtDirectorio == "_vti_txt")
    error = "No puede elegir el directorio _vti_txt para su Cuenta FTP Extra";
  if (txtDirectorio == ".frontpage")
    error = "No puede elegir el directorio .frontpage para su Cuenta FTP Extra";
  if (txtDirectorio == "postinfo.html")
    error = "No puede elegir el directorio postinfo.html para su Cuenta FTP Extra";
  if (txtDirectorio == "_vti_inf.html")
    error = "No puede elegir el directorio _vti_inf.html para su Cuenta FTP Extra";
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Crear la cuenta FTP en el sistema
  derror = servicio.agregarFTP(txtUsuario, txtDirectorio, txtPassword, dominio,
			       optBorrarDirectorio);

  //Crear la cuenta FTP en la base de datos
  derror = servicio.agregarFTPDB(txtUsuario, txtDirectorio, txtPassword, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo la Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio();
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "La Cuenta FTP Extra ftp" + txtUsuario + "-" + dominio.getIdDominio() 
    + " fue creada con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
