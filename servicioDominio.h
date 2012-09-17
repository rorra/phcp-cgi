#ifndef _SERVICIODOMINIO_H
#define _SERVICIODOMINIO_H

#include "servicio.h"
#include "dominio.h"

class cServicioDominio:public cServicio {
 public:
  //Constructor e iniciador
  cServicioDominio(cDominio *dominio);
  bool iniciar();

  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de un dominio en la base de datos
  int verificarExisteDominioDB(const std::string &, cDominio &dominio);
};

#endif // _SERVICIODOMINIO_H
