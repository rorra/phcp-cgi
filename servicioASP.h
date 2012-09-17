#ifndef _SERVICIOASP_H
#define _SERVICIOASP_H

#include "servicio.h"
#include "dominio.h"

#include <vector>

class cServicioASP:public cServicio {
 public:
  //Constructor e iniciador
  cServicioASP(cDominio *dominio);
  bool iniciar();

  //Agrega la cuenta ASP en el sistema
  int agregarASP(std::vector<std::string> &, std::vector<std::string> &, cDominio &dominio);
  //Agrega una cuenta ASP en la base de datos
  int agregarASPDB(cDominio &);
  //Modifica el archivo obdc.ini
  int agregarFileOdbcIni(std::vector<std::string> &, std::vector<std::string> &, cDominio &);
  //Modificiar la clave de la cuenta ASP en el sistema
  int cambiarClave(std::vector<std::string> &, std::vector<std::string> &, cDominio &dominio);
  //Modifica el archivo obdc.ini
  int modificarFileOdbcIni(std::vector<std::string> &, std::vector<std::string> &, cDominio &);
  //quitar la cuenta ASP en el sistema
  int quitarASP(std::vector<std::string> &, std::vector<std::string> &, cDominio &dominio);
  //Quita una cuenta ASP de la base de datos
  int quitarASPDB(cDominio &);
  //Modifica el archivo obdc.ini
  int quitarFileOdbcIni(std::vector<std::string> &, std::vector<std::string> &, cDominio &);
  //Reinicia el servicio
  int reiniciarApache();
  //Reinicia el servicio
  int reiniciarCaspd();
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una cuenta ASP en la base de datos
  int verificarExisteASPDB(cDominio &dominio);
};

#endif // _SERVICIOASP_H
