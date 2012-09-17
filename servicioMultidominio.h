#ifndef _SERVICIOMULTIDOMINIO_H
#define _SERVICIOMULTIDOMINIO_H

#include "servicio.h"
#include "dominio.h"

class cServicioMultidominio:public cServicio {
 public:
  //Constructor e iniciador
  cServicioMultidominio(cDominio *dominio);
  bool iniciar();

  //Agrega el redireccionamiento al httpd-vhosts.conf
  int agregarFileHttpdConf(const std::string &, cDominio &);
  //Crea el archivo named.dominio
  int agregarFileNamed(const std::string &, cDominio &);
  //Agrega la configuracion al archivo named.conf
  int agregarFileNamedConf(const std::string &, cDominio &);
  //Agrega un multidominio en el sistema
  int agregarMultidominio(const std::string &, cDominio &);
  //Agrega un multidominio en la base de datos
  int agregarMultidominioDB(const std::string &, cDominio &);
  //Quita el redireccionamiento del httpd-vhosts.conf
  int quitarFileHttpdConf(const std::string &, cDominio &);
  //Elimina el archivo named.dominio
  int quitarFileNamed(const std::string &, cDominio &);
  //Elimina la configuracion al archivo named.conf
  int quitarFileNamedConf(const std::string &, cDominio &);
  //Quita un multidominio del sistema
  int quitarMultidominio(const std::string &, cDominio &);
  //Quita un multidominio de la base de datos
  int quitarMultidominioDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un multidominio en la base de datos
  int verificarExisteMultidominioDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de un multidominio para cierto dominio en la base de datos
  int verificarExisteMultidominioDB(const std::string &, cDominio &dominio,
				    std::string);
};

#endif // _SERVICIOMULTIDOMINIO_H
