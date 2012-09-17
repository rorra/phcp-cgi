#ifndef _SERVICIOPROTECCIONACCESO_H
#define _SERVICIOPROTECCIONACCESO_H

#include "servicio.h"
#include "dominio.h"

class cServicioProteccionAcceso:public cServicio {
 public:
  //Constructor e iniciador
  cServicioProteccionAcceso(cDominio *dominio);
  bool iniciar();

  //Agrega el directorio en comun para todas las protecciones
  int agregarDirectorioComun(cDominio &);
  //Agrega el archivo .htaccess para una proteccion de acceso
  int agregarFileHtaccess(const std::string &, cDominio &);
  //Agregar la lista de password para el directorio que se esta protegiendo
  int agregarFilePasswords(const std::string &, cDominio &);
  //Agrega la lista de usuarios para el directorio que se esta protegiendo
  int agregarFileUsuarios(const std::string &, cDominio &);
  //Agrega una proteccion de acceso en el sistema
  int agregarProteccionAcceso(const std::string &, cDominio &, const std::string &);
  //Agrega una proteccion de acceso en la base de datos
  int agregarProteccionAccesoDB(const std::string &, cDominio &);
  //Quita el directorio en comun para todas las protecciones
  int quitarDirectorioComun(cDominio &);
  //Quita el archivo .htaccess para una proteccion de acceso
  int quitarFileHtaccess(const std::string &, cDominio &);
  //Quita el archivo de passwords para el directorio
  int quitarFilePasswords(const std::string &, cDominio &);
  //Quita el archivo de usuarios para el directorio
  int quitarFileUsuarios(const std::string &, cDominio &);
  //Quita una proteccion de acceso del sistema
  int quitarProteccionAcceso(const std::string &, cDominio &, const std::string &);
  //Quita una proteccion de acceso de la base de datos
  int quitarProteccionAccesoDB(const std::string &, cDominio &);
  //Quita todos los usuarios de acceso de la base de datos
  int quitarUsuariosAccesoDB(const std::string &, cDominio &);
  //Recupera el id de una proteccion de acceso
  std::string recuperarIdProteccionAccesoDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de una proteccion de acceso en la base de datos
  int verificarExisteProteccionAccesoDB(const std::string &, cDominio &dominio);
  //Verifica la existencia de protecciones de acceso en la base de datos
  int verificarExistenProteccionAccesoDB(cDominio &dominio);
};

#endif // _SERVICIOPROTECCIONACCESO_H
