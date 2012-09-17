#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "configuracion.h"
#include "servicio.h"
#include "servicioAlias.h"

#include <iostream>

using namespace std;
using namespace mysqlpp;

cServicioAlias::cServicioAlias(cDominio *dom):cServicio(dom) {
  servicioDB = "ALIAS";
}

bool cServicioAlias::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioAlias::agregarAlias(const std::string &alias, const std::string &cuenta,
				 cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Crear el archivo virtusertable
  if (agregarFileVirtusertable(alias, cuenta, dominio) < 0) {
    comando = "Error al agregar el alias " + alias + " para la cuenta " + cuenta + "@"
      + dominio.getDominio() + " en el archivo virtusertable";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar el archivo virtusertable y access
  //actualizarFileAccess();
  actualizarFileVirtusertable();

  return(0);
}

int cServicioAlias::agregarAliasDB(const std::string &alias, const std::string &cuenta,
				  cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO CUENTA_ALIAS(CUENTA, ID_DOMINIO, CUENTA_ALIAS) VALUES('"
	<< cuenta << "', '" << dominio.getIdDominio() << "', '" << alias << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la cuenta alias " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioAlias::agregarFileVirtusertable(const std::string &alias, const std::string &cuenta,
					    cDominio &dominio) {
  //Abrir el archivo para escritura
  ofstream out("/etc/mail/virtusertable", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /etc/mail/virtusertable");
    return(-1);
  }

  //Agregar la linea
  out << cuenta << "@" << dominio.getDominio() << "\t" << alias << endl;

  //Cerar el archivo
  out.close();

  return(0);
}

std::string cServicioAlias::recuperarAliasDB(const std::string &cuenta, cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT CUENTA_ALIAS FROM CUENTA_ALIAS WHERE CUENTA = '" << cuenta
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      resultado = std::string(row[0]);
    }
    return(resultado);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(resultado);
  } catch(...) {
    string error;
    error = "Error al recuperar el alias de la cuenta alias " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}


int cServicioAlias::quitarAlias(const std::string &cuenta, const std::string &alias,
				cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Quitar el alias del archivo virtusertable
  if (quitarFileVirtusertable(cuenta, alias, dominio) < 0) {
    comando = "Error al quitar el alias para la cuenta " + cuenta + "@" + dominio.getDominio()
      + " del archivo virtusertable";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar el file access y virtusertable
  //actualizarFileAccess();
  actualizarFileVirtusertable();

  return(0);
}

int cServicioAlias::quitarAliasDB(const std::string &cuenta, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM CUENTA_ALIAS WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la cuenta alias " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioAlias::quitarFileVirtusertable(const std::string &cuenta,
					    const std::string &alias,
					    cDominio &dominio) {
  //Mover el archivo para actualizarlo
  system("mv -f /etc/mail/virtusertable /etc/mail/virtusertable.bak");

  //Abrir el archivo para lectura
  ifstream in("/etc/mail/virtusertable.bak");
  if (!in.is_open()) {
    system("mv -f /etc/mail/virtusertable.bak /etc/mail/virtusertable");
    dominio.loguear("No se pudo abrir el archivo /etc/mail/virtusertable.bak");
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out("/etc/mail/virtusertable");
  if (!out.is_open()) {
    system("mv -f /etc/mail/virtusertable.bak /etc/mail/virtusertable");
    dominio.loguear("No se pudo abrir el archivo /etc/mail/virtusertable");
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada;
  buscada = cuenta + "@" + dominio.getDominio() + "\t" + alias;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) == string::npos)
      out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

std::string cServicioAlias::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_ALIAS WHERE ID_DOMINIO = '" << dominio->getIdDominio()
	<< "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      cantidad = std::string(row[0]);
    }
    return(cantidad);
  } catch(BadQuery er) {
    dominio->loguear(er.error);
    return(cantidad);
  } catch(...) {
    string error;
    error = "Error al recuperar la cantidad del servicio";
    dominio->loguear(error);
    return(cantidad);
  }
}

int cServicioAlias::verificarExisteAliasDB(const std::string &cuenta, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_ALIAS WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'" << endl;
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      resultado = row[0];
    }
    return(resultado);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(resultado);
  } catch(...) {
    string error;
    error = "Error al verificar si existe la cuenta alias " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}
