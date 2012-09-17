#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "configuracion.h"
#include "servicio.h"
#include "servicioAliasRedir.h"

#include <iostream>

using namespace std;
using namespace mysqlpp;

cServicioAliasRedir::cServicioAliasRedir(cDominio *dom):cServicio(dom) {
  servicioDB = "ALIASREDIR";
}

bool cServicioAliasRedir::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioAliasRedir::agregarAliasRedir(const std::string &alias,
    const std::string &alias1,
    const std::string &alias2,
    const std::string &alias3,
    const std::string &alias4,
    const std::string &alias5,
    const std::string &alias6,
    const std::string &alias7,
    const std::string &alias8,
    const std::string &alias9,
    const std::string &cuenta, 
    cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Crear el archivo aliases
  if (agregarFileAliases(alias, alias1, alias2, alias3, alias4, alias5, alias6, alias7, alias8, alias9, cuenta, dominio) < 0) {
    comando = "Error al agregar el alias " + alias + " para la cuenta " + cuenta + "@"
      + dominio.getDominio() + " en el archivo aliases";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar el archivo aliases
  actualizarFileAliases();

  return(0);
}

int cServicioAliasRedir::agregarAliasRedirDB(const std::string &alias,
    const std::string &alias1,
    const std::string &alias2,
    const std::string &alias3,
    const std::string &alias4,
    const std::string &alias5,
    const std::string &alias6,
    const std::string &alias7,
    const std::string &alias8,
    const std::string &alias9,
    const std::string &cuenta, 
    cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO CUENTA_ALIAS_REDIR(CUENTA, ID_DOMINIO, CUENTA_ALIAS_REDIR, CUENTA_ALIAS_REDIR1, "
        << "CUENTA_ALIAS_REDIR2, CUENTA_ALIAS_REDIR3, CUENTA_ALIAS_REDIR4, CUENTA_ALIAS_REDIR5, "
	<< "CUENTA_ALIAS_REDIR6, CUENTA_ALIAS_REDIR7, CUENTA_ALIAS_REDIR8, CUENTA_ALIAS_REDIR9) VALUES('"
	<< cuenta << "', '" << dominio.getIdDominio() << "', '" << alias << "', '" << alias1 << "', '" << alias2 
	<< "', '" << alias3 << "', '" << alias4 << "', '" << alias5 << "', '" << alias6 << "', '" << alias7
	<< "', '" << alias8 << "', '" << alias9 << "')";
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

int cServicioAliasRedir::agregarFileAliases(const std::string &alias, 
    const std::string &alias1,
    const std::string &alias2,
    const std::string &alias3,
    const std::string &alias4,    
    const std::string &alias5,    
    const std::string &alias6,    
    const std::string &alias7,    
    const std::string &alias8,    
    const std::string &alias9,    
    const std::string &cuenta, 
    cDominio &dominio) {
  //Abrir el archivo para escritura
  ofstream out("/etc/mail/aliases", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /etc/mail/aliases");
    return(-1);
  }

  //Agregar la linea
  out << cuenta << "-" << dominio.getIdDominio() << ":" << "\t" << cuenta << "@" << dominio.getDominio() 
    << ", " << alias;
  if (alias1.length() > 0) out << ", " << alias1;
  if (alias2.length() > 0) out << ", " << alias2;
  if (alias3.length() > 0) out << ", " << alias3;
  if (alias4.length() > 0) out << ", " << alias4;
  if (alias5.length() > 0) out << ", " << alias5;
  if (alias6.length() > 0) out << ", " << alias6;
  if (alias7.length() > 0) out << ", " << alias7;
  if (alias8.length() > 0) out << ", " << alias8;
  if (alias9.length() > 0) out << ", " << alias9;
  out << endl;

  //Cerar el archivo
  out.close();

  return(0);
}

std::string cServicioAliasRedir::recuperarAliasRedirDB(const std::string &cuenta, cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT CUENTA_ALIAS_REDIR FROM CUENTA_ALIAS_REDIR WHERE CUENTA = '" << cuenta
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


int cServicioAliasRedir::quitarAliasRedir(const std::string &cuenta, const std::string &alias,
     cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Quitar el alias del archivo aliases
  if (quitarFileAliases(cuenta, alias, dominio) < 0) {
    comando = "Error al quitar el alias para la cuenta " + cuenta + "@" + dominio.getDominio()
      + " del archivo aliases";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar aliases
  actualizarFileAliases();

  return(0);
}

int cServicioAliasRedir::quitarAliasRedirDB(const std::string &cuenta, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM CUENTA_ALIAS_REDIR WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
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

int cServicioAliasRedir::quitarFileAliases(const std::string &cuenta, const std::string &alias,
cDominio &dominio) {
				
  //Mover el archivo para actualizarlo
  system("mv -f /etc/mail/aliases /etc/mail/aliases.bak");

  //Abrir el archivo para lectura
  ifstream in("/etc/mail/aliases.bak");
  if (!in.is_open()) {
    system("mv -f /etc/mail/aliases.bak /etc/mail/aliases");
    dominio.loguear("No se pudo abrir el archivo /etc/mail/aliases.bak");
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out("/etc/mail/aliases");
  if (!out.is_open()) {
    system("mv -f /etc/mail/aliases.bak /etc/mail/aliases");
    dominio.loguear("No se pudo abrir el archivo /etc/mail/aliases");
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada;
  buscada = cuenta + "-" + dominio.getIdDominio() + ":" + "\t" + cuenta + "@" + dominio.getDominio() + ", " + alias;
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

std::string cServicioAliasRedir::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_ALIAS_REDIR WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioAliasRedir::verificarExisteAliasRedirDB(const std::string &cuenta, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_ALIAS_REDIR WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
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
