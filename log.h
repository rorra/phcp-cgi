#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <cstdio>

class cLog {

 public:
  //Constructor
  cLog(std::string &);

  //Loguea un mensaje
  void loguear(const std::string &mensaje);

 private:
  std::string dominio;                           //Dominio al cual se loguea
  //Archivos para loguear
  FILE *fileDominio;                             //Archivo privado del dominio
  FILE *fileComun;                               //Archivo comun para todos los dominios

  //Metodos para abrir y cerrar los archivos de logueo
  bool abrirFileDominio();
  bool abrirFileComun();
  bool cerrarFileDominio();
  bool cerrarFileComun();

  //No permitir el copy constructor o operador= creacion o operador() creacion
  cLog(cLog&);
  cLog& operator=(cLog&);
};

//Reporta un error rarisimo con el directorio donde se origino y la funcion que lo origino
void reportarErrorRarisimo(std::string mensaje);

//Obtiene la fecha y la hora actual del sistema
void obtenerDiaHora(std::string &);

#endif // _LOG_H
