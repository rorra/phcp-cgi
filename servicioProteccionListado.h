#ifndef _SERVICIOPROTECCIONLISTADO_H
#define _SERVICIOPROTECCIONLISTADO_H

#include "servicio.h"
#include "dominio.h"

class cServicioProteccionListado:public cServicio {
 public:
  //Constructor e iniciador
  cServicioProteccionListado(cDominio *dominio);
  bool iniciar();

  //Agrega el archivo .htaccess con la proteccion de listado
  int agregarFileHtaccess(const std::string &, cDominio &);
  //Agrega una proteccion de listado en el sistema
  int agregarProteccionListado(const std::string &, cDominio &, const std::string &);
  //Agrega una proteccion de listado en la base de datos
  int agregarProteccionListadoDB(const std::string &, cDominio &);
  //quita una proteccion de listado del sistema
  int quitarProteccionListado(const std::string &, cDominio &, const std::string &);
  //Quita una proteccion de listado de la base de datos
  int quitarProteccionListadoDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una proteccion de listado en la base de datos
  int verificarExisteProteccionListadoDB(const std::string &, cDominio &dominio);
};

#endif // _SERVICIOPROTECCIONLISTADO_H
