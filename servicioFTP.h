#ifndef _SERVICIOFTP_H
#define _SERVICIOFTP_H

#include "servicio.h"
#include "dominio.h"

class cServicioFTP:public cServicio {
 public:
  //Constructor e iniciador
  cServicioFTP(cDominio *dominio);
  bool iniciar();

  //Agrega una cuenta FTP en el sistema
  int agregarFTP(const std::string &, const std::string &, const std::string &, cDominio &,
		 const std::string &);
  //Agrega una cuenta FTP en la base de datos
  int agregarFTPDB(const std::string &, const std::string &, const std::string &, cDominio &);
  //Cambia el password de uan cuenta FTP en el sistema
  int cambiarClaveFTP(const std::string &, const std::string &, cDominio &);
  //Cambia el password de una cuenta FTP en la base de datos
  int cambiarClaveFTPDB(const std::string &, const std::string &, cDominio &);
  //Deshabilita una cuenta FTP en la base de datos
  int deshabilitarFTPDB(const std::string &, const std::string &, cDominio &);
  //Habilita una cuenta FTP en la base de datos
  int habilitarFTPDB(const std::string &, const std::string &, cDominio &);
  //Quita una cuenta FTP del sistema
  int quitarFTP(const std::string &, const std::string &, cDominio &);
  //Quita una cuenta FTP de la base de datos
  int quitarFTPDB(const std::string &, cDominio &);
  //Recupera el directorio de una cuenta FTP
  std::string recuperarDirectorioDB(const std::string &, cDominio &);
  //Recupera el password de una cuenta FTP
  std::string recuperarPasswordDB(const std::string &, cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica si una cuenta FTP esta deshabilitada
  int verificarDeshabilitadaFTPDB(const std::string &, cDominio &);
  //Verifica si existe un directorio para las cuentas ftp de un dominio
  int verificarExisteDirectorioDB(const std::string &, cDominio &);
  //Verifica la existencia de una cuenta FTP en la base de datos
  int verificarExisteFTPDB(const std::string &, cDominio &);
  //Verifica si una cuenta FTP esta habilitada
  int verificarHabilitadaFTPDB(const std::string &, cDominio &);
};

#endif // _SERVICIOFTP_H
