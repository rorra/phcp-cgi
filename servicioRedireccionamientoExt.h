#ifndef _SERVICIOREDIRECCIONAMIENTOEXT_H
#define _SERVICIOREDIRECCIONAMIENTOEXT_H

#include "servicio.h"
#include "dominio.h"

class cServicioRedireccionamientoExt:public cServicio {
 public:
  //Constructor e iniciador
  cServicioRedireccionamientoExt(cDominio *dominio);
  bool iniciar();

  //Agrega el redireccionamiento externo al httpd-vhosts.conf
  int agregarFileHttpdConf(const std::string &, const std::string &, cDominio &);
  //Crea el archivo named.dominio
  int agregarFileNamed(const std::string &, cDominio &);
  //Agrega la configuracion al archivo named.conf
  int agregarFileNamedConf(const std::string &, cDominio &);
  //Agrega un redireccionamiento externo en el sistema
  int agregarRedireccionamientoExt(const std::string &, const std::string &, cDominio &);
  //Agrega un redireccionamiento externo en la base de datos
  int agregarRedireccionamientoExtDB(const std::string &, const std::string &, cDominio &);
  //Quita el redireccionamiento externo del httpd-vhosts.conf
  int quitarFileHttpdConf(const std::string &, cDominio &);
  //Elimina el archivo named.dominio
  int quitarFileNamed(const std::string &, cDominio &);
  //Elimina la configuracion al archivo named.conf
  int quitarFileNamedConf(const std::string &, cDominio &);
  //Quita un redireccionamiento externo del sistema
  int quitarRedireccionamientoExt(const std::string &, cDominio &);
  //Quita un redireccionamiento externo de la base de datos
  int quitarRedireccionamientoExtDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un redireccionamiento externo en la base de datos
  int verificarExisteRedireccionamientoExtDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de un redireccionamiento externo para cierto dominio en la 
  //base de datos
  int verificarExisteRedireccionamientoExtDB(const std::string &, cDominio &dominio,
					     std::string);
};

#endif // _SERVICIOREDIRECCIONAMIENTOEXT_H
