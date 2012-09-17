#ifndef _SERVICIOUSUARIOACCESO_H
#define _SERVICIOUSUARIOACCESO_H

#include "servicio.h"
#include "dominio.h"

class cServicioUsuarioAcceso:public cServicio {
 public:
  //Constructor e iniciador
  cServicioUsuarioAcceso(cDominio *dominio);
  bool iniciar();

  //Agrega el archivo con los passwords de la proteccion de acceso
  int agregarFilePassword(const std::string &, const std::string &, const std::string &,
			  cDominio &);
  //Agrega el archivo con los usuarios de la proteccion de acceso
  int agregarFileUsuarios(const std::string &, const std::string &, cDominio &);
  //Agrega un usuariod e proteccion de acceso en el sistema
  int agregarUsuarioAcceso(const std::string &, const std::string &, const std::string &,
			   const std::string &, cDominio &);
  //Agrega un usuario de proteccion de acceso en la base de datos
  int agregarUsuarioAccesoDB(const std::string &, const std::string &, const std::string &,
			     cDominio &);
  //Cambia el password de un usuario de proteccion de acceso en el sistema
  int cambiarClaveUsuarioAcceso(const std::string &, const std::string &, const std::string &,
				const std::string &, cDominio &);
  //Cambia el password de un usuario de proteccion de acceso en la base de datos
  int cambiarClaveUsuarioAccesoDB(const std::string &, const std::string &, const std::string &,
				  cDominio &);
  //Deshabilita un usuario de una proteccion de acceso del sistema
  int deshabilitarUsuarioAcceso(const std::string &, const std::string &, cDominio &);
  //Deshabilita un usuario de una proteccion de acceso en la base de datos
  int deshabilitarUsuarioAccesoDB(const std::string &, const std::string &, cDominio &);
  //Habilita un usuario de uan proteccion de acceso en el sistema
  int habilitarUsuarioAcceso(const std::string &, const std::string &, cDominio &);
  //Habilita un usuario de una proteccion de acceso en la base de datos
  int habilitarUsuarioAccesoDB(const std::string &, const std::string &, cDominio &);
  //Quita un usuario de proteccion de acceso del archivo de passwords
  int quitarFilePassword(const std::string &, const std::string &, cDominio &);
  //Quita un usuario de proteccion de acceso del sistema
  int quitarUsuarioAcceso(const std::string &, const std::string &, const std::string &,
			  cDominio &);
  //Quita un usuario de proteccion de acceso en la base de datos
  int quitarUsuarioAccesoDB(const std::string &, const std::string &, cDominio &);
  //Quita los usuarios de una proteccion de acceso en la base de datos
  int quitarUsuariosAccesoDB(const std::string &, cDominio &);
  //Recupera el password de un usuario de proteccion de acceso
  std::string recuperarPasswordDB(const std::string &, const std::string &, cDominio &);
  //Recupera una lista de usuarios de acceso para una proteccion de acceso
  int recuperarUsuariosAccesoDB(const std::string &, cDominio &, std::string &,
				std::string usuario = "", std::string = "0");
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica si un usuario de proteccion de listado se encuentra deshabilitado
  int verificarDeshabilitadoUsuarioDB(const std::string &, const std::string &, cDominio &);
  //Verifica la existencia de un usuario de proteccion de acceso en la base de datos
  int verificarExisteUsuarioAccesoDB(const std::string &, const std::string &,
				     cDominio &dominio);
  //Verifica la existencia de usuarios de acceso en la base de datos
  int verificarExistenUsuarioAccesoDB(const std::string &, cDominio &dominio);
  //Verifica si un usuario de proteccion de acceso se encuentra habilitado
  int verificarHabilitadoUsuarioDB(const std::string &, const std::string &, cDominio &);
};

#endif // _SERVICIOUSUARIOACCESO_H
