#ifndef _SERVICIOALIASREDIR_H
#define _SERVICIOALIASREDIR_H

#include "servicio.h"
#include "dominio.h"

class cServicioAliasRedir:public cServicio {
 public:
  //Constructor e iniciador
  cServicioAliasRedir(cDominio *dominio);
  bool iniciar();

  //Agrega una cuenta alias en el sistema
  int agregarAliasRedir(const std::string &, const std::string &,
                        const std::string &, const std::string &, const std::string &, const std::string &,
			const std::string &, const std::string &, const std::string &, const std::string &,
                        const std::string &, cDominio &);
  //Agrega una cuenta alias en la base de datos
  int agregarAliasRedirDB(const std::string &, const std::string &,
                        const std::string &, const std::string &, const std::string &, const std::string &,
                        const std::string &, const std::string &, const std::string &, const std::string &,
                          const std::string &, cDominio &);
  //Agrega el alias al archivo aliases
  int agregarFileAliases(const std::string &, const std::string &,
                         const std::string &, const std::string &, const std::string &, const std::string &,
                         const std::string &, const std::string &, const std::string &, const std::string &,
                         const std::string &, cDominio &);
  //Recupera el alias de una cuenta alias
  std::string recuperarAliasRedirDB(const std::string &, cDominio &);
  //Quita una cuenta Alias del sistema
  int quitarAliasRedir(const std::string &, const std::string &, cDominio &);
  //Quita una cuenta Alias de la base de datos
  int quitarAliasRedirDB(const std::string &, cDominio &);
  //Elimina un alias del archivo aliases
  int quitarFileAliases(const std::string &, const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una cuenta alias en la base de datos
  int verificarExisteAliasRedirDB(const std::string &, cDominio &dominio);
};

#endif // _SERVICIOALIASREDIR_H
