#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioDB.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicioDB::cServicioDB(cDominio *dom):cServicio(dom) {
  servicioDB = "DBMYSQL";
}

bool cServicioDB::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioDB::agregarDB(const std::string &nombre, const std::string &password,
			      cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO BASE_DATOS(NOMBRE_DB, ID_DOMINIO, PASSWORD) VALUES ('" << nombre << "', '"
	<< dominio.getIdDominio() << "', '" << password << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la base de datos " + nombre;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioDB::cambiarClave(const std::string &password, cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Cambiar el password en mysql
  if ((cambiarClaveMySQL(password, dominio)) < 0) {
    comando = "Error al actualizar el password en msqyl";
    dominio.loguear(comando);
    return(-1);
  }
  //Flushar los privilegios
  actualizarPrivilegiosMySQL(dominio);

  /*
  //Modificar el archivo de configuracion config.inc.php
  if (modificarFileConfigIncPHP(password, dominio) < 0) {
    comando = "Error al actualizar el archivo config.inc.php para el password " + password;
    dominio.loguear(comando);
    return(-1);
  } */
  
  return(0);
}

int cServicioDB::cambiarClaveDB(const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE BASE_DATOS SET PASSWORD = '" << password << "' WHERE ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password de las bases de datos";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioDB::cambiarClaveMySQL(const std::string &password, cDominio &dominio) {
  try {
    dominio.con.select_db("mysql");
    Query qry = dominio.con.query();
    qry << "UPDATE user SET password = PASSWORD('" << password << "') WHERE user = 'pdu"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    dominio.con.select_db("PANEL_DB_2");
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password en mysql";
    dominio.loguear(error);
    return(-2);
  }
}
/*
int cServicioDB::modificarFileConfigIncPHP(const std::string &password, cDominio &dominio) {
  std::string archivo, archivoB, comando;
  archivo = "/www/docs/webs/phcm/" + dominio.getDominio() + "/mysql/config.inc.php";
  archivoB = "/www/docs/webs/phcm/" + dominio.getDominio() + "/mysql/config.inc.php.bak";
  
  //Mover el archivo para actualizarlo
  comando = "mv -f " + archivo + " " + archivoB;
  system(comando.c_str());

  //Abrir el archivo para lectura
  ifstream in(archivoB.c_str());
  if (!in.is_open()) {
    comando = "mv -f " + archivoB + " " + archivo;
    system(comando.c_str());
    dominio.loguear("No se pudo abrir el archivo " + archivoB);
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "mv -f " + archivoB + " " + archivo;
    system(comando.c_str());
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada1, buscada2;
  buscada1 = "$cfg['Servers'][$i]['user']";
  buscada2 = "$cfg['Servers'][$i]['password']";
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada1) != string::npos)
      out << "$cfg['Servers'][$i]['user']          = '" << dominio.getUsuario() << "';" << endl;
    else if (linea.find(buscada2) != string::npos)
      out << "$cfg['Servers'][$i]['password']      = '" << password << "';" << endl;
    else
      out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
} */

int cServicioDB::recuperarNombrePasswordDB(std::vector<std::string> &nombre,
					   std::vector<std::string> &password,
					   cDominio &dominio) {
  try {
    std::string temp;
    Query qry = dominio.con.query();
    qry << "SELECT NOMBRE_DB, PASSWORD FROM BASE_DATOS WHERE ID_DOMINIO = '" << 
      dominio.getIdDominio() << "' ORDER BY NOMBRE_DB";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    for (i = res.begin(); i != res.end(); i++) {
      row = *i;
      temp = std::string(row[0]);
      nombre.push_back(temp);
      temp = std::string(row[1]);
      password.push_back(temp);
    }
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al recuperar los nombres de las bases de datos y passwords";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioDB::quitarDB(const std::string &nombre, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM BASE_DATOS WHERE NOMBRE_DB = '" << nombre << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la base de datos " + nombre;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioDB::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM BASE_DATOS WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioDB::vaciarDB(const std::string &db, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DROP DATABASE " + db;
    qry.execute();
    qry << "CREATE DATABASE " + db;
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al vaciar la base de datos " + db;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioDB::verificarExisteDB(const std::string &nombre, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM BASE_DATOS WHERE NOMBRE_DB = '" << nombre 
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
    error = "Error al verificar si existe la base de datos " + nombre;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioDB::verificarExistenDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM BASE_DATOS WHERE ID_DOMINIO = '" << dominio.getIdDominio()
	<< "'" << endl;
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
    error = "Error al verificar si existen bases de datos";
    dominio.loguear(error);
    return(resultado);
  }
}
