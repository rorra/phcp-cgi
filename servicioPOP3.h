#ifndef _SERVICIOPOP3_H
#define _SERVICIOPOP3_H

#include "servicio.h"
#include "dominio.h"

class cServicioPOP3:public cServicio {
 public:
  //Constructor e iniciador
  cServicioPOP3(cDominio *dominio);
  bool iniciar();

  //Agrega una cuenta POP3 en el sistema
  int agregarEmail(const std::string &, const std::string &, cDominio &);
  //Agrega una cuenta POP3 en la base de datos
  int agregarEmailDB(const std::string &, const std::string &, cDominio &);
  //Agrega una cuenta POP3 al archivo access
  int agregarFileAccess(const std::string &, cDominio &);
  //Agrega una cuenta POP3 al archivo virtusertable
  int agregarFileVirtusertable(const std::string &, cDominio&);
  //Agrega una cuenta POP3 al archivo spam.whitelist.rules
  int agregarFileSpamWhitelist(const std::string &, cDominio&);
  //Cambia el password de una cuenta POP3 en el sistema
  int cambiarClavePOP3(const std::string &, const std::string &, cDominio &);
  //Cambia el password de una cuenta POP3 en la base de datos
  int cambiarClavePOP3DB(const std::string &, const std::string &, cDominio &);
  //Quita una cuenta POP3 del sistema
  int quitarEmail(const std::string &, cDominio &);
  //Quita una cuenta POP3 de la base de datos
  int quitarEmailDB(const std::string &, cDominio &);
  //Quita una cuenta POP3 del archivo access
  int quitarFileAccess(const std::string &, cDominio &);
  //Quita una cuenta POP3 del archivo virtusertable
  int quitarFileVirtusertable(const std::string &, cDominio &);
  //Quita una cuenta POP3 del archivo spam.whitelist.rules
  int quitarFileSpamWhitelist(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una cuenta POP3 en la base de datos
  int verificarExisteEmailDB(const std::string &, cDominio &);
  //Verifica la existencia de cuenntas POP3 en la base de datos
  int verificarExisteEmailsDB(cDominio &);
};

#endif // _SERVICIOPOP3_H
