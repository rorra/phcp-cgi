#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioProteccionListado.h"
#include <errno.h>

using namespace std;
using namespace mysqlpp;

cServicioProteccionListado::cServicioProteccionListado(cDominio *dom):cServicio(dom) {
  servicioDB = "PROTECCION DIRECTORIO LISTADO";
}

bool cServicioProteccionListado::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioProteccionListado::agregarFileHtaccess(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir + "/.htaccess";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Agregar las lineas
  out << "Options None" << endl
      << endl
      << "<Limit GET POST PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "allow from all" << endl;

  //Cerrar el archivo
  out.close();

  //Darle al archivo permisos root:root, 644, +i
  comando = "chown root:root " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioProteccionListado::agregarProteccionListado(const std::string &dir, cDominio &dominio,
							 const std::string &optBorrarDirectorio) {
  std::string directorio, comando, error;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir;

  //Si esta seteada optBorrarDirectorio eliminarlo
  if (optBorrarDirectorio.length() > 0) {
    comando = "rm -fr " + directorio;
    system(comando.c_str());
  }

  //Crear el directorio y darles los permisos correspondientes
  if (mkdir(directorio.c_str(), 0755)) {
    if (errno != EEXIST) {
      comando = "Error al crear el directorio " + directorio;
      dominio.loguear(comando);
      return(-1);
    }
  } else {
    comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + directorio;
    system(comando.c_str());
  }

  //Crear el archivo .htaccess
  if ((agregarFileHtaccess(dir, dominio)) < 0) {
    error = "Error al crear el archivo .htaccess";
    dominio.loguear(error);
    return(-1);
  }

  return(0);
}

int cServicioProteccionListado::agregarProteccionListadoDB(const std::string &directorio,
							   cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO PROTECCION_DIR_LISTADO(ID_DOMINIO, DIRECTORIO) VALUES('"
	<< dominio.getIdDominio() << "', '" << directorio << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la proteccion de listado para el directorio " + directorio;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioProteccionListado::quitarProteccionListado(const std::string &dir, cDominio &dominio,
							const std::string &optBorrarDirectorio) {
  std::string archivo, directorio, comando, error;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Setear el directorio y el archivo
  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir;
  archivo = directorio + "/.htaccess";

  //Sacarle el flag al archivo
  comando = "chattr -i " + archivo;
  system(comando.c_str());

  //Eliminar el archivo de configuracion
  comando = "rm -f " + archivo;
  system(comando.c_str());

  //Si esta seteada optBorrarDirectorio eliminarlo
  if (optBorrarDirectorio.length() > 0) {
    comando = "rm -fr " + directorio;
    system(comando.c_str());
  }

  return(0);
}

int cServicioProteccionListado::quitarProteccionListadoDB(const std::string &directorio,
							  cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM PROTECCION_DIR_LISTADO WHERE DIRECTORIO = '" << directorio
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la proteccion de listado para el directorio " + directorio;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioProteccionListado::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_LISTADO WHERE ID_DOMINIO = '" 
	<< dominio->getIdDominio() << "'";
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

int cServicioProteccionListado::verificarExisteProteccionListadoDB(const std::string &directorio,
								   cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_LISTADO WHERE DIRECTORIO = '" << directorio 
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
    error = "Error al verificar si existe la proteccion de listado para el directorio "
      + directorio;
    dominio.loguear(error);
    return(resultado);
  }
}
