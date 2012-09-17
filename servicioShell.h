#ifndef _SERVICIOSHELL_H
#define _SERVICIOSHELL_H

#include "servicio.h"
#include "dominio.h"

class cServicioShell:public cServicio {
 public:
  //Constructor e iniciador
  cServicioShell(cDominio *dominio);
  bool iniciar();

  //Cambia el password de una cuenta Shell en el sistema
  int cambiarClaveShell(const std::string &, cDominio &);
  //Cambia el password de una cuenta Shell en la base de datos
  int cambiarClaveShellDB(const std::string &, cDominio &);
  //Deshabilita una cuenta Shell en la base de datos
  int deshabilitarShellDB(const std::string &, cDominio &);
  //Habilita una cuenta Shell en la base de datos
  int habilitarShellDB(const std::string &, cDominio &);
  //Recupera el password de una cuenta Shell
  std::string recuperarPasswordDB(cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica si una cuenta Shell esta deshabilitada
  int verificarDeshabilitadaShellDB(cDominio &);
  //Verifica la existencia de una cuenta Shell en la base de datos
  int verificarExisteShellDB(cDominio &);
  //Verifica si una cuenta Shell esta habilitada
  int verificarHabilitadaShellDB(cDominio &);
};

#endif // _SERVICIOSHELL_H
