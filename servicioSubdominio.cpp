#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioSubdominio.h"
#include <errno.h>

using namespace std;
using namespace mysqlpp;

cServicioSubdominio::cServicioSubdominio(cDominio *dom):cServicio(dom) {
  servicioDB = "SUBDOMINIO";
}

bool cServicioSubdominio::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioSubdominio::agregarSubdominio(const std::string &subdominio, cDominio &dominio,
					   const std::string &optBorrarDirectorio) {
  std::string comando;
  std::string directorio;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + subdominio;

  //Si esta seteado optBorrarDirectorio, eliminar el directorio
  if (optBorrarDirectorio.length() > 0) {
    comando = "rm -fr " + directorio;
    system(comando.c_str());
  }

  //Crear el subdirectorio para el subdominio
  if (mkdir(directorio.c_str(), 0755))
    if (errno != EEXIST) {
      comando = "Error al crear el directorio " + directorio + " para crear el subdominio";
      dominio.loguear(comando);
      return(-1);
    }

  //Cambiar los permisos del subdominio
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " +
    directorio;
  if (system(comando.c_str())) {
    comando = "Error al cambiar los permisos del directorio " + directorio
      + " para crear el subdominio";
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioSubdominio::agregarSubdominioDB(const std::string &subdominio,
					     cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO SUBDOMINIO(ID_DOMINIO, SUBDOMINIO) VALUES('" << dominio.getIdDominio()
	<< "', '" << subdominio << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar el subdominio " + subdominio;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioSubdominio::quitarSubdominio(const std::string &subdominio, cDominio &dominio) {
  std::string comando;
  std::string directorio;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + subdominio;

  //Eliminar el subdirectorio para el subdominio
  comando = "rm -fr " + directorio;
  if (system(comando.c_str()) < 0) {
    comando = "Error al eliminar el directorio " + directorio;
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioSubdominio::quitarSubdominioDB(const std::string &subdominio, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM SUBDOMINIO WHERE SUBDOMINIO = '" << subdominio << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar el subdominio " + subdominio;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioSubdominio::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM SUBDOMINIO WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioSubdominio::verificarExisteSubdominioDB(const std::string &subdominio,
						     cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM SUBDOMINIO WHERE SUBDOMINIO = '" << subdominio 
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
    error = "Error al verificar si existe el subdominio " + subdominio;
    dominio.loguear(error);
    return(resultado);
  }
}
