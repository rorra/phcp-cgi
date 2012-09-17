#ifndef _SERVICIOALIAS_H
#define _SERVICIOALIAS_H

#include "servicio.h"
#include "dominio.h"

class cServicioAlias:public cServicio {
 public:
  //Constructor e iniciador
  cServicioAlias(cDominio *dominio);
  bool iniciar();

  //Agrega una cuenta alias en el sistema
  int agregarAlias(const std::string &, const std::string &, cDominio &);
  //Agrega una cuenta alias en la base de datos
  int agregarAliasDB(const std::string &, const std::string &, cDominio &);
  //Agrega el alias al archivo virtusertable
  int agregarFileVirtusertable(const std::string &, const std::string &, cDominio &);
  //Recupera el alias de una cuenta alias
  std::string recuperarAliasDB(const std::string &, cDominio &);
  //Quita una cuenta Alias del sistema
  int quitarAlias(const std::string &, const std::string &, cDominio &);
  //Quita una cuenta Alias de la base de datos
  int quitarAliasDB(const std::string &, cDominio &);
  //Elimina un alias del archivo virtusertable
  int quitarFileVirtusertable(const std::string &, const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una cuenta alias en la base de datos
  int verificarExisteAliasDB(const std::string &, cDominio &dominio);
};

#endif // _SERVICIOALIAS_H
