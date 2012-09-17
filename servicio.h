#ifndef _SERVICIO_H
#define _SERVICIO_H

#include "dominio.h"

class cServicio {
 public:
  std::string idServicio;                  //Identificador del servicio
  std::string servicio;                    //Nombre del servicio
  std::string servicioDB;                  //Nombre del servicio en la base de datos
  std::string cantidad;                    //Cantidad del servicio
  std::string limite;                      //Limite de creacion del servicio

  //Constructor e iniciador
  cServicio(cDominio *);
  bool iniciar();

  //Agrega un mensaje al historial del dominio
  int agregarHistorial(const std::string &, cDominio &);
  //Trae el limite del servicio para el tipo de dominio
  std::string traerLimite();
  //Trae la cantidad del servicio para el tipo de dominio
  std::string traerCantidad();
  //Verifica si el cliente puede crear el servicio
  int verificarCrearServicio();

  //Recupera la identidad del servicio
  std::string getIdServicio();
  //Recupera el servicio
  std::string getServicio();
  //Recupera el servicio como esta especificado en la base de datos
  std::string getServicioDB();
  //Recupera la cantidad del servicio
  std::string getCantidad();
  //Recupera el limite
  std::string getLimite();

  /* Funciones que actualizan y reinician servicios -------------------------------------------- */

  int actualizarDNS();
  int actualizarFTPA();
  int actualizarFileAccess();
  int actualizarFileVirtusertable();
  int actualizarFileAliases();
  int actualizarPrivilegiosMySQL(cDominio &);

  /* Funciones que actualizan y reinician servicios -------------------------------------------- */

  /* Funciones varias ----------------------------- -------------------------------------------- */
  long obtenerTamanio(std::string archivo, std::string directorio = "");
  /* Funciones varias ----------------------------- -------------------------------------------- */

  /* Funciones de verificacion de caracteres --------------------------------------------------- */

  std::string caracterBackup(const std::string &, const std::string);
  std::string caracterDirectorio(const std::string &, const std::string);
  std::string caracterDirectorioL(const std::string &, const std::string);
  std::string caracterDominio(const std::string &, const std::string);
  std::string caracterEmail(const std::string &, const std::string);
  std::string caracterLetraNumero(const std::string &, const std::string);
  std::string caracterLetraNumeroEspacio(const std::string &, const std::string);
  std::string caracterNumeroLetra(const std::string &, const std::string);
  std::string caracterNumero(const std::string &, const std::string);
  std::string caracterPassword(const std::string &, const std::string);
  std::string caracterUsuarioEmail(const std::string &, const std::string);

  /* Funciones de verificacion de caracteres --------------------------------------------------- */

 protected:
  cDominio *dominio;               //Apuntador a la clase dominio
  //No permitir copy constructor ni operator= creacion
  cServicio(cServicio&);
  cServicio &operator=(cServicio&);
};

#endif // _SERVICIO_H
