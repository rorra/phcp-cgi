#ifndef _SERVICIOFTPANONIMO_H
#define _SERVICIOFTPANONIMO_H

#include "servicio.h"
#include "dominio.h"

class cServicioFTPAnonimo:public cServicio {
 public:
  //Constructor e iniciador
  cServicioFTPAnonimo(cDominio *dominio);
  bool iniciar();

  //Agrega la configuracion al archivo proftpd.conf
  int agregarFileProftpdConf(const std::string &, cDominio &);
  //Crea el archivo welcome.msg
  int agregarFileWelcome(const std::string &, cDominio &);
  //Agrega una cuenta FTP Anonima en el sistema
  int agregarFTPA(const std::string &, const std::string &, cDominio &);
  //Agrega una cuenta FTP Anonimo en la base de datos
  int agregarFTPADB(const std::string &, const std::string &, cDominio &);
  //Cambia el mensaje de bienvenida de una cuenta FTP Anonima en el sistema
  int cambiarMensajeFTPA(const std::string &, cDominio &);
  //Cambia el mensaje de bienvenida de una cuenta FTP Anonima en la base de datos
  int cambiarMensajeFTPADB(const std::string &, cDominio &);
  //Quita la configuracion al archivo proftpd.conf
  int quitarFileProftpdConf(cDominio &);
  //Quita una cuenta FTP Anonima del sistema
  int quitarFTPA(cDominio &);
  //Quita una cuenta FTP Anonimo de la base de datos
  int quitarFTPADB(cDominio &);
  //Trae un puerto para la cuenta FTP Anonima
  std::string recuperarPuertoDB(cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una cuenta FTP Anonimo en la base de datos
  int verificarExisteFTPADB(cDominio &dominio);
};

#endif // _SERVICIOFTPANONIMO_H
