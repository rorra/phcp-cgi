#ifndef _SERVICIOSUBDOMINIO_H
#define _SERVICIOSUBDOMINIO_H

#include "servicio.h"
#include "dominio.h"

class cServicioSubdominio:public cServicio {
 public:
  //Constructor e iniciador
  cServicioSubdominio(cDominio *dominio);
  bool iniciar();

  //Agrega un subdominio al sistema
  int agregarSubdominio(const std::string &, cDominio &, const std::string &);
  //Agrega un subdominio en la base de datos
  int agregarSubdominioDB(const std::string &, cDominio &);
  //Quita un subdominio del sistema
  int quitarSubdominio(const std::string &, cDominio &);
  //Quita un subdominio en la base de datos
  int quitarSubdominioDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un Subdominio en la base de datos
  int verificarExisteSubdominioDB(const std::string &, cDominio &);
};

#endif // _SERVICIOSUBDOMINIO_H
