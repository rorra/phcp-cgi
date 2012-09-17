#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioAutorrespuesta.h"

using namespace std;
using namespace mysqlpp;

cServicioAutorrespuesta::cServicioAutorrespuesta(cDominio *dom):cServicio(dom) {
  servicioDB = "AUTORRESPUESTA";
}

bool cServicioAutorrespuesta::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioAutorrespuesta::agregarAutorrespuesta(const std::string &cuenta,
						   const std::string &nombre,
						   const std::string &autorrespuesta,
						   cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Crear el archivo autorresponder
  if (agregarFileAutorresponder(cuenta, autorrespuesta, dominio) < 0) {
    comando = "Error al crear el archivo autorresponder para la cuenta " + cuenta + "@"
      + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Crear el archivo procmailrc
  if (agregarFileProcmailrc(cuenta, nombre, dominio) < 0) {
    comando = "Error al crear el archivo autorresponder para la cuenta " + cuenta + "@"
      + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Darle permisos root.nobody a autorresponder
  comando = "chown root.nobody /home/" + cuenta + "-" + dominio.getIdDominio() + "/autorresponder";
  system(comando.c_str());

  //Darle permisos root.root a procmailrc
  comando = "chown root.root /home/" + cuenta + "-" + dominio.getIdDominio() + "/.procmailrc";
  system(comando.c_str());
  
  return(0);
}

int cServicioAutorrespuesta::agregarAutorrespuestaDB(const std::string &cuenta, 
						     const std::string &nombre,
						     const std::string &autorrespuesta,
						     cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO AUTORRESPUESTA(CUENTA, ID_DOMINIO, TEXTO, TITULO) VALUES ('" << cuenta
	<< "', '" << dominio.getIdDominio() << "', '" << autorrespuesta << "', '" << nombre
	<< "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la autorrespuesta " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioAutorrespuesta::agregarFileAutorresponder(const std::string &cuenta,
						       const std::string &autorrespuesta,
						       cDominio &dominio) {
  std::string archivo;

  //Abrir el archivo para escritura
  archivo = "/home/" + cuenta + "-" + dominio.getIdDominio() + "/autorresponder";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear(string("No se pudo abrir el archivo " + archivo));
    return(-1);
  }

  //Agregar la linea
  out << autorrespuesta;

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioAutorrespuesta::agregarFileProcmailrc(const std::string &cuenta,
						   const std::string &nombre,
						   cDominio &dominio) {
  std::string archivo;

  //Abrir el archivo para escritura
  archivo = "/home/" + cuenta + "-" + dominio.getIdDominio() + "/.procmailrc";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear(string("No se pudo abrir el archivo " + archivo));
    return(-1);
  }

  //Agregar las lineas
  out << "SHELL=/bin/bash" << endl
      << ":0 h c" << endl
      << "* !^FROM_DAEMON" << endl
      << "* !^X-Loop: " << cuenta << "@" << dominio.getDominio() << endl
      << "| (formail -r -I'From: " << nombre << "<" << cuenta << "@" << dominio.getDominio() 
      << ">' -I'Content-Type: text/html; charset=\"iso-8859-1\"' -I'Precedence: Auto Reply' -A'X-Loop: " << cuenta << "@" << dominio.getDominio()
      << "' ; cat /home/" << cuenta << "-" << dominio.getIdDominio() << "/autorresponder ;) | "
    "/usr/sbin/sendmail -t" << endl;

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioAutorrespuesta::cambiarAutorrespuesta(const std::string &cuenta,
						   const std::string &nombre,
						   const std::string &autorrespuesta,
						   cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Eliminar el archivo autorresponder
  comando = "rm -f /home/" + cuenta + "-" + dominio.getIdDominio() + "/autorresponder";
  system(comando.c_str());

  //Eliminar el archivo procmailrc
  comando = "rm -fr /home/" + cuenta + "-" + dominio.getIdDominio() + "/.procmailrc";
  system(comando.c_str());


  //Crear el archivo autorresponder
  if (agregarFileAutorresponder(cuenta, autorrespuesta, dominio) < 0) {
    comando = "Error al crear el archivo autorresponder para la cuenta " + cuenta + "@"
      + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Crear el archivo procmailrc
  if (agregarFileProcmailrc(cuenta, nombre, dominio) < 0) {
    comando = "Error al crear el archivo autorresponder para la cuenta " + cuenta + "@"
      + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Darle permisos root.nobody a autorresponder
  comando = "chown root.nobody /home/" + cuenta + "-" + dominio.getIdDominio() + "/autorresponder";
  system(comando.c_str());

  //Darle permisos root.root a procmailrc
  comando = "chown root.root /home/" + cuenta + "-" + dominio.getIdDominio() + "/.procmailrc";
  system(comando.c_str());
  
  return(0);
}


int cServicioAutorrespuesta::cambiarAutorrespuestaDB(const std::string &cuenta,
						     const std::string &nombre,
						     const std::string &autorrespuesta,
						     cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE AUTORRESPUESTA SET TEXTO = '" << autorrespuesta << "', TITULO = '"
	<< nombre << "' WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar la autorrespuesta " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioAutorrespuesta::quitarAutorrespuesta(const std::string &cuenta,
						  cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Eliminar el archivo autorresponder
  comando = "rm -f /home/" + cuenta + "-" + dominio.getIdDominio() + "/autorresponder";
  system(comando.c_str());

  //Eliminar el archivo procmailrc
  comando = "rm -fr /home/" + cuenta + "-" + dominio.getIdDominio() + "/.procmailrc";
  system(comando.c_str());
  
  return(0);
}


int cServicioAutorrespuesta::quitarAutorrespuestaDB(const std::string &cuenta, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM AUTORRESPUESTA WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la autorrespuesta " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioAutorrespuesta::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM AUTORRESPUESTA WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioAutorrespuesta::verificarExisteAutorrespuestaDB(const std::string &cuenta,
							     cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM AUTORRESPUESTA WHERE CUENTA = '" << cuenta 
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'" << endl;
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
    error = "Error al verificar si existe la autorrespuesta " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}
