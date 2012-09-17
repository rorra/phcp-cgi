#ifndef _SERVICIODB_H
#define _SERVICIODB_H

#include "servicio.h"
#include "dominio.h"

#include <string>
#include <vector>

class cServicioDB:public cServicio {
 public:
  //Constructor e iniciador
  cServicioDB(cDominio *dominio);
  bool iniciar();

  //Agrega una base de datos en la base de datos
  int agregarDB(const std::string &, const std::string &, cDominio &);
  //Cambia el password en el sistema
  int cambiarClave(const std::string &, cDominio &);
  //Cambiar el password de las bases de datos
  int cambiarClaveDB(const std::string &, cDominio &);
  //Cambiar el password en msyql
  int cambiarClaveMySQL(const std::string &, cDominio &);
  //Modifica el archivo config.inc.php
  int modificarFileConfigIncPHP(const std::string &, cDominio &);
  //Recupera los usuarios y password para un dominio de la base de datos
  int recuperarNombrePasswordDB(std::vector<std::string> &, std::vector<std::string> &,
				cDominio &dominio);
  //Quita una base de datos de la base de datos
  int quitarDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Vaicar una base de datos
  int vaciarDB(const std::string &, cDominio &);
  //Verifica la existencia de una base de datos en la base de datos
  int verificarExisteDB(const std::string &, cDominio &);
  //Verifica la existencia de bases de datos
  int verificarExistenDB(cDominio &);
};

#endif // _SERVICIODB_H
