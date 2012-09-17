#ifndef _SERVICIOPAGINAERROR_H
#define _SERVICIOPAGINAERROR_H

#include "servicio.h"
#include "dominio.h"

class cServicioPaginaError:public cServicio {
 public:
  //Constructor e iniciador
  cServicioPaginaError(cDominio *dominio);
  bool iniciar();

  //Agrega la pagina de error a la configuracion del apache
  int agregarFileHttpdConf(const std::string &, cDominio &);
  //Agrega un archivo con la pagina de error en el sistema
  int agregarFilePagina(const std::string &, cDominio &);
  //Agrega una pagina de error en el sistema
  int agregarPaginaError(const std::string &, cDominio &);
  //Agrega un pagina de error en la base de datos
  int agregarPaginaErrorDB(const std::string &, cDominio &);
  //Quita una pagina de error de la configuracion del apache
  int quitarFileHttpdConf(const std::string &, cDominio &);
  //Quita una pagina de error del sistema
  int quitarPaginaError(const std::string &, cDominio &);
  //Quita una pagina de error de la base de datos
  int quitarPaginaErrorDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un codigo de error en la base de datos
  int verificarExisteCodigoErrorDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de una pagina de error en la base de datos
  int verificarExistePaginaErrorDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de las paginas de error
  int verificarExistenPaginaErrorDB(cDominio &dominio);
};

#endif // _SERVICIOPAGINAERROR_H
