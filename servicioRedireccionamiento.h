#ifndef _SERVICIOREDIRECCIONAMIENTO_H
#define _SERVICIOREDIRECCIONAMIENTO_H

#include "servicio.h"
#include "dominio.h"

class cServicioRedireccionamiento:public cServicio {
 public:
  //Constructor e iniciador
  cServicioRedireccionamiento(cDominio *dominio);
  bool iniciar();

  //Agrega el redireccionamiento al httpd-vhosts.conf
  int agregarFileHttpdConf(const std::string &, cDominio &);
  //Crea el archivo named.dominio
  int agregarFileNamed(const std::string &, cDominio &);
  //Agrega la configuracion al archivo named.conf
  int agregarFileNamedConf(const std::string &, cDominio &);
  //Agrega un redireccionamiento en el sistema
  int agregarRedireccionamiento(const std::string &, cDominio &);
  //Agrega un redireccionamiento en la base de datos
  int agregarRedireccionamientoDB(const std::string &, cDominio &);
  //Quita el redireccionamiento del httpd-vhosts.conf
  int quitarFileHttpdConf(const std::string &, cDominio &);
  //Elimina el archivo named.dominio
  int quitarFileNamed(const std::string &, cDominio &);
  //Elimina la configuracion al archivo named.conf
  int quitarFileNamedConf(const std::string &, cDominio &);
  //Quita un redireccionamiento del sistema
  int quitarRedireccionamiento(const std::string &, cDominio &);
  //Quita un redireccionamiento de la base de datos
  int quitarRedireccionamientoDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un redireccionamiento en la base de datos
  int verificarExisteRedireccionamientoDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de un redireccionamiento para cierto dominio en la base de datos
  int verificarExisteRedireccionamientoDB(const std::string &, cDominio &dominio,
					  std::string);
};

#endif // _SERVICIOREDIRECCIONAMIENTO_H
