#ifndef _INTERFAZ_H
#define _INTERFAZ_H

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include "dominio.h"

class cInterfaz {
 public:
  //Constructor
  cInterfaz();

  //Obtiene una variable de un formulario
  int obtenerVariable(std::string, std::string &, cDominio &);
  //Redirige el navegador a una pagina
  void redirigir(const std::string &);
  //Redirige el navegador a una pagina del dominio
  void redirigirDom(const std::string &, const cDominio &);
  
  //Reporta un error de un comando
  void reportarErrorComando(const std::string &, const std::string &, const cDominio &);
  //Reporta un error fatal al navegador
  void reportarErrorFatal();
  //Reporta un mensaje al navegador
  void reportarMensaje(const std::string &);
  //Reporta un ok de un comando
  void reportarOkComando(const std::string &, const std::string &, const cDominio &);
  void reportarOkComandoBackup(const std::string &, const std::string &, const std::string &,
			       const std::string &, const cDominio &);
  //Verifica que el cgi haya sido llamada por una pagina en especial
  int verificarPagina(const std::string &);

 private:
  cgicc::Cgicc cgi;                        //Objeto referente al cgi
};

#endif // _INTERFAZ_H
