#ifndef _SERVICIOAUTORRESPUESTA_H
#define _SERVICIOAUTORRESPUESTA_H

#include "servicio.h"
#include "dominio.h"

class cServicioAutorrespuesta:public cServicio {
 public:
  //Constructor e iniciador
  cServicioAutorrespuesta(cDominio *dominio);
  bool iniciar();

  //Agrega una autorrespuesta en el sistema
  int agregarAutorrespuesta(const std::string &, const std::string &, const std::string &,
			    cDominio &);
  //Agrega una autorrespuesta en la base de datos
  int agregarAutorrespuestaDB(const std::string &, const std::string &, const std::string &,
			      cDominio &);
  //Crea el archivo autorresponder
  int agregarFileAutorresponder(const std::string &, const std::string &, cDominio&);
  //Crea el archivo procmailrc
  int agregarFileProcmailrc(const std::string &, const std::string &, cDominio &);
  //Cambia el mensaje de bienvenida y el usuario de una autorrespuesta en el sistema
  int cambiarAutorrespuesta(const std::string &, const std::string &, const std::string &,
			    cDominio &);
  //Cambia el mensaje de bienvenida y el usuario de una autorrespuesta en la base de datos
  int cambiarAutorrespuestaDB(const std::string &, const std::string &, const std::string &,
			      cDominio &);
  //Quita una autorrespuesta del sistema
  int quitarAutorrespuesta(const std::string &, cDominio &);
  //Quita una autorrespuesta de la base de datos
  int quitarAutorrespuestaDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una autorrespuesta en la base de datos
  int verificarExisteAutorrespuestaDB(const std::string &, cDominio &);
};

#endif // _SERVICIOAUTORRESPUESTA_H
