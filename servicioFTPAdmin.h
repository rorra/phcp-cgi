#ifndef _SERVICIOFTPADMIN_H
#define _SERVICIOFTPADMIN_H

#include "servicio.h"
#include "dominio.h"

class cServicioFTPAdmin:public cServicio {
 public:
  //Constructor e iniciador
  cServicioFTPAdmin(cDominio *dominio);
  bool iniciar();


  //Cambia el password FTP del administrador en el sistema
  int cambiarClaveFTPAdmin(const std::string &, cDominio &);
  //Cambia el password FTP del administrador en la base de datos
  int cambiarClaveFTPAdminDB(const std::string &, cDominio &);
};

#endif // _SERVICIOFTPADMIN_H
