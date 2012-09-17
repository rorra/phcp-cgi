#ifndef _SERVICIOREDIRECCIONAMIENTOSUB_H
#define _SERVICIOREDIRECCIONAMIENTOSUB_H

#include "servicio.h"
#include "dominio.h"

class cServicioRedireccionamientoSub:public cServicio {
 public:
  //Constructor e iniciador
  cServicioRedireccionamientoSub(cDominio *dominio);
  bool iniciar();

  //Agrega el redireccionamiento al httpd-vhosts.conf
  int agregarFileHttpdConf(const std::string &, const std::string &, cDominio &);
  //Crea el archivo named.dominio
  int agregarFileNamed(const std::string &, cDominio &);
  //Agrega la configuracion al archivo named.conf
  int agregarFileNamedConf(const std::string &, cDominio &);
  //Agrega un redireccionamiento a subdominio en el sistema
  int agregarRedireccionamientoSub(const std::string &, const std::string &, cDominio &);
  //Agrega un redireccionamiento a subdominio en la base de datos
  int agregarRedireccionamientoSubDB(const std::string &, const std::string &, cDominio &);
  //Quita el redireccionamiento del httpd-vhosts.conf
  int quitarFileHttpdConf(const std::string &, cDominio &);
  //Elimina el archivo named.dominio
  int quitarFileNamed(const std::string &, cDominio &);
  //Elimina la configuracion al archivo named.conf
  int quitarFileNamedConf(const std::string &, cDominio &);
  //Quita un redireccionamiento a subdominio en el sistema
  int quitarRedireccionamientoSub(const std::string &, cDominio &);
  //Quita un redireccionamiento a subdominio de la base de datos
  int quitarRedireccionamientoSubDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un redireccionamiento a subdominio en la base de datos
  int verificarExisteRedireccionamientoSubDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de un redireccionamiento a subodminio para cierto dominio en la 
  //base de datos
  int verificarExisteRedireccionamientoSubDB(const std::string &, cDominio &dominio,
					     std::string);
};

#endif // _SERVICIOREDIRECCIONAMIENTOSUB_H
