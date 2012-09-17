#include "log.h"
#include "dominio.h"
#include "servicio.h"
#include "interfaz.h"
#include "util.h"
#include "configuracion.h"
#include "servicioAliasRedir.h"
#include "servicioPOP3.h"
#include "CrearAliasRedir.h"

using namespace std;

int main(int argc, char *argv[]) {
  //Inicializar el dominio
  cDominio dominio;
  if (!dominio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto dominio"));
  //Inicializar el servicio
  cServicioAliasRedir servicio(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioAliasRedir"));
  cServicioPOP3 servicioPOP3(&dominio);
  if (!servicio.iniciar())
    reportarErrorRarisimo(string("Error al iniciar el objeto servicioPOP3"));
  //Inicializar la clase interfaz
  cInterfaz interfaz;
  //Inicializar variables miscelaneas
  string mensaje;
  string error;
  int derror;
  string errorFile(ERROR_CREAR_ALIAS_REDIR);
  string okFile(OK_CREAR_ALIAS_REDIR);

  //Verificar que la pagina haya sido llamada por CrearAliasRedir.php
  if (interfaz.verificarPagina(string("CrearAliasRedir.php")) < 0)
    return(-1);

  //Verificar si el cliente puede crear el servicio
  derror = servicio.verificarCrearServicio();
  if (derror == 0) {
    error = "Su Plan no le permite crear Redireccionamientos de E-Mails";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  } else if(derror < -1) {
    error = "Usted ya ha creado el maximo de Redireccionamientos permitidos para su Plan";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Obtener las variables
  string txtCuenta, txtAlias, txtAlias1, txtAlias2, txtAlias3, txtAlias4, txtAlias5, txtAlias6, txtAlias7, txtAlias8, txtAlias9;
  if ((interfaz.obtenerVariable("txtCuenta", txtCuenta, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAlias", txtAlias, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAlias1", txtAlias1, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAlias2", txtAlias2, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAlias3", txtAlias3, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if ((interfaz.obtenerVariable("txtAlias4", txtAlias4, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  } 
  if ((interfaz.obtenerVariable("txtAlias5", txtAlias5, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  } 
  if ((interfaz.obtenerVariable("txtAlias6", txtAlias6, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  } 
  if ((interfaz.obtenerVariable("txtAlias7", txtAlias7, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  } 
  if ((interfaz.obtenerVariable("txtAlias8", txtAlias8, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  } 
  if ((interfaz.obtenerVariable("txtAlias9", txtAlias9, dominio)) < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
 
  //Borar los espacios en blanco y los newlines a los costados de los campos
  strtrimString(txtCuenta);
  strtrimString(txtAlias);
  strtrimString(txtAlias1);
  strtrimString(txtAlias2);
  strtrimString(txtAlias3);
  strtrimString(txtAlias4);
  strtrimString(txtAlias5);
  strtrimString(txtAlias6);  
  strtrimString(txtAlias7);  
  strtrimString(txtAlias8);  
  strtrimString(txtAlias9);  
  
  //Verificar los caracteres validos para los campos
  error = servicio.caracterUsuarioEmail(txtCuenta, "Origen");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterEmail(txtAlias, "Destino1");
  if (error.length() > 0) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterEmail(txtAlias1, "Destino2");
  if ((txtAlias1.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }
  error = servicio.caracterEmail(txtAlias2, "Destino3");
  if ((txtAlias2.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias3, "Destino4");
  if ((txtAlias3.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias4, "Destino5");
  if ((txtAlias4.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias5, "Destino6");
  if ((txtAlias5.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias6, "Destino7");
  if ((txtAlias6.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias7, "Destino8");
  if ((txtAlias7.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias8, "Destino9");
  if ((txtAlias8.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  error = servicio.caracterEmail(txtAlias9, "Destino10");
  if ((txtAlias9.length() > 0) && (error.length() > 0)) {
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }  
  
  //Pasar a minusculas la cuenta alias y la cuenta de correo
  string txtCuentaM = lowerCase(txtCuenta);
  string txtAliasM = lowerCase(txtAlias);
  string txtAliasM1 = lowerCase(txtAlias1);
  string txtAliasM2 = lowerCase(txtAlias2);  
  string txtAliasM3 = lowerCase(txtAlias3);  
  string txtAliasM4 = lowerCase(txtAlias4);
  string txtAliasM5 = lowerCase(txtAlias5);
  string txtAliasM6 = lowerCase(txtAlias6);
  string txtAliasM7 = lowerCase(txtAlias7);
  string txtAliasM8 = lowerCase(txtAlias8);
  string txtAliasM9 = lowerCase(txtAlias9);
  
  //Verificar si el alias ya existe
  derror = servicio.verificarExisteAliasRedirDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "El Redireccionamiento " + txtCuenta + "@" + dominio.getDominio() + " ya existe";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }

  //Verificar si existe el Redir como una cuenta Alias
  /*derror = servicioAlias.verificarExisteAliasDB(txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }
  if (derror > 0) {
    error = "La Cuenta " + txtAlias + "@" + dominio.getDominio()
      + " ya existe como una Cuenta Alias";
    interfaz.reportarErrorComando(error, errorFile, dominio);
    return(-1);
  }*/

  //Crear la cuenta alias en el sistema
  derror = servicio.agregarAliasRedir(txtAliasM, txtAliasM1, txtAliasM2, txtAliasM3, txtAliasM4, txtAliasM5, txtAliasM6, txtAliasM7, txtAliasM8, txtAliasM9, txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Crear la cuenta alias en la base de datos
  derror = servicio.agregarAliasRedirDB(txtAliasM, txtAliasM1, txtAliasM2, txtAliasM3, txtAliasM4, txtAliasM5, txtAliasM6, txtAliasM7, txtAliasM8, txtAliasM9, txtCuentaM, dominio);
  if (derror < 0) {
    interfaz.reportarErrorFatal();
    return(-1);
  }

  //Agregar al historial de la base de datos
  mensaje = "Se creo el Redireccionamiento " + txtCuenta + "@" + dominio.getDominio();
     // + " para la cuenta de E-Mail " + txtAlias;
  servicio.agregarHistorial(mensaje, dominio);

  //Reportar el mensaje de ok al navegador
  mensaje = "El Redireccionamiento " + txtCuenta + "@" + dominio.getDominio()
    + " fue creado con éxito.";
  interfaz.reportarOkComando(mensaje, okFile, dominio);

  return(0);
}
