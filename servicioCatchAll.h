#ifndef _SERVICIOCATCHALL_H
#define _SERVICIOCATCHALL_H

#include "servicio.h"
#include "dominio.h"

class cServicioCatchAll:public cServicio {
 public:
  //Constructor e iniciador
  cServicioCatchAll(cDominio *dominio);
  bool iniciar();

  //Habilitar cuenta CatchAll en el sistema
  int agregarCatchAll(cDominio &);
   //Deshabilitar cuenta CatchAll en el sistema
  int quitarCatchAll(cDominio &);
  //Cambia el password de una cuenta CatchAll en la base de datos
  int cambiarClaveCatchAllDB(cDominio &);
  //Deshabilita una cuenta CatchAll en la base de datos
  int deshabilitarCatchAllDB(cDominio &);
  //Habilita una cuenta CatchAll en la base de datos
  int habilitarCatchAllDB(cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica si una cuenta CatchAll esta deshabilitada
  int verificarDeshabilitadaCatchAllDB(cDominio &);
  //Verifica la existencia de una cuenta CatchAll en la base de datos
  int verificarExisteCatchAllDB(cDominio &);
  //Verifica si una cuenta CatchAll esta habilitada
  int verificarHabilitadaCatchAllDB(cDominio &);
};

#endif // _SERVICIOCATCHALL_H
