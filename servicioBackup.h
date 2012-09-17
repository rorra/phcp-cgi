#ifndef _SERVICIOBACKUP_H
#define _SERVICIOBACKUP_H

#include "servicio.h"
#include "dominio.h"

class cServicioBackup:public cServicio {
 public:
  //Constructor e iniciador
  cServicioBackup(std::string tipo, cDominio *dominio);
  bool iniciar();

  //Agrega un backup en el sistema
  int agregarBackup(const std::string &, const std::string &, cDominio &,
		    const std::string &nombreDB = "");
  //Agrega un backup en la base de datos
  int agregarBackupDB(const std::string &, cDominio &);
  //Trae el tamanio de un backup con formate texto
  std::string tamanioBackup(const std::string &, cDominio &dominio);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica una compresion valida para el backup
  int verificarCompresion(std::string &);
  //Verifica la existencia de un backup de las ultimas 24 horas en la base de datos
  int verificarExisteBackupDB(cDominio &);

  //Obtiene un nombre del archivo para el backup
  std::string getNombreArchivo(const std::string &, cDominio &);
 private:
  std::string obtenerEmails(cDominio &dominio);

  std::string tipo;          //Tipo de backup: Email, FTPA, MySQL, Web
};

#endif // _SERVICIOBACKUP_H
