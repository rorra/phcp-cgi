#ifndef _DOMINIO_H
#define _DOMINIO_H

#include <string>
#include <mysql++.h>
#include "log.h"

class cDominio {
 public:
  mysqlpp::Connection con;			      //Conexion a la base de datos
  //Constructor e iniciador
  cDominio();
  bool iniciar();

  void loguear(std::string);                          //Loguea un mensaje de error
  void setIdDominio(std::string);                     //Setea la identidad del dominio
  void setTipo(std::string);                          //Setea el tipo de dominio
  std::string getIdDominio() const;                   //Recupera el ID del dominio
  std::string getDominio() const;                     //Recupera el dominio
  std::string getPassword() const;                    //Recupera el password del panel
  std::string getPasswordFTP() const;                 //Recupera el password FTP del dominio
  std::string getTipo() const;                        //Recupera el tipo de dominio
  std::string getUrl() const;                         //Recupera la URL del dominio
  std::string getUsuario() const;                     //Recupera el usuario del dominio

 private:
  cLog *log;                                          //Clase de logueo
  std::string dominio;                                //Nombre del dominio
  std::string idDominio;                              //Id del dominio
  std::string tipo;                                   //Tipo de dominio
  std::string usuario;                                //Usuario del dominio
  std::string password;                               //Password del panel
  std::string passwordFTP;                            //Password FTP del dominio

  //No permitir copy constructor ni operator= en creacion
  cDominio(cDominio&);
  cDominio &operator=(cDominio&);
};

#endif // _DOMINIO_H
