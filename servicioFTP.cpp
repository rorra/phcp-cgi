#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioFTP.h"

using namespace std;
using namespace mysqlpp;

cServicioFTP::cServicioFTP(cDominio *dom):cServicio(dom) {
  servicioDB = "FTP";
}

bool cServicioFTP::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioFTP::agregarFTP(const std::string &cuenta, const std::string &dir,
			     const std::string &password, cDominio &dominio,
			     const std::string &optBorrarDirectorio) {
  std::string comando, directorio;

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

  //Ingresar el nuevo usuario en el sistema
  comando = "useradd -d " + directorio + " -m -g pdu" + dominio.getIdDominio()
    + " -c ftp" + cuenta + "-" + dominio.getIdDominio() + " -s /bin/false ftp" + cuenta
    + "-" + dominio.getIdDominio();
  if (system(comando.c_str()) < 0) {
    comando = "Error al agregar el usuario de sistema ftp" + cuenta + "-" + dominio.getIdDominio()
      + " para la cuenta FTP";
    dominio.loguear(comando);
    return(-1);
  }

  //Cambiar el password del usuario de sistema creado
  comando = "echo ftp" + cuenta + "-" + dominio.getIdDominio() + ":" + password + " | chpasswd";
  if (system(comando.c_str()) < 0) {
    comando = "Error al cambiar el password del usuario de sistema ftp" + cuenta + "-" 
      + dominio.getIdDominio() + " para la cuenta FTP";
    dominio.loguear(comando);
    return(-1);
  }

  //cambiar los permisos del directorio 
  if (chmod(directorio.c_str(), 0775) < 0) {
    comando = "Error al cambiar los permisos para el directorio " + directorio;
    return(-1);
  }

  //Armar el comando para eliminar los archivos del directorio creado
  comando = "rm -f " + directorio + "/.*";
  system(comando.c_str());

  return(0);
}

int cServicioFTP::agregarFTPDB(const std::string &cuenta, const std::string &directorio,
			       const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO CUENTA_FTP(ID_DOMINIO, CUENTA, DIRECTORIO, PASSWORD, ESTADO) VALUES ('"
	<< dominio.getIdDominio() << "', '" << cuenta << "', '" << directorio << "', '"
	<< password << "', '1')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioFTP::cambiarClaveFTP(const std::string &cuenta, const std::string &password,
				  cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Cambiar el password de la cuenta FTP
  comando = "echo ftp" + cuenta + "-" + dominio.getIdDominio() + ":" + password + " | chpasswd";
  if (system(comando.c_str()) < 0) {
    comando = "Error al cambiar el password del usuario de sistema ftp" + cuenta + "-" 
      + dominio.getIdDominio() + " para la cuenta FTP";
    dominio.loguear(comando);
    return(-1);
  }
  
  return(0);
}

int cServicioFTP::cambiarClaveFTPDB(const std::string &cuenta, const std::string &password,
				    cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE CUENTA_FTP SET PASSWORD = '" << password << "' WHERE CUENTA = '" << cuenta
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password de la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioFTP::habilitarFTPDB(const std::string &cuenta, const std::string &password,
				 cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE CUENTA_FTP SET PASSWORD = '" << password << "', ESTADO = '1' WHERE CUENTA = '"
	<< cuenta << "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioFTP::deshabilitarFTPDB(const std::string &cuenta, const std::string &password,
				    cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE CUENTA_FTP SET PASSWORD = '" << password << "', ESTADO = '0' WHERE CUENTA = '"
	<< cuenta << "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioFTP::quitarFTP(const std::string &cuenta, const std::string &dir, cDominio &dominio) {
  std::string comando, directorio;

  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Eliminar el usurio de sistema FTP
  comando = "userdel ftp" + cuenta + "-" + dominio.getIdDominio();
  if (system(comando.c_str()) < 0) {
    comando = "No se pudo eliminar el usuario de sistema ftp" + cuenta + "-"
      + dominio.getIdDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Armar el comando para eliminar el directorio del usuario ftp
  comando = "rm -fr " + directorio;
  system(comando.c_str());
  
  return(0);
}

int cServicioFTP::quitarFTPDB(const std::string &cuenta, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM CUENTA_FTP WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioFTP::recuperarDirectorioDB(const std::string &cuenta, cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT DIRECTORIO FROM CUENTA_FTP WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
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
    error = "Error al recuperar el directorio de la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}

std::string cServicioFTP::recuperarPasswordDB(const std::string &cuenta, cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT PASSWORD FROM CUENTA_FTP WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
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
    error = "Error al recuperar el password de la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}

std::string cServicioFTP::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioFTP::verificarDeshabilitadaFTPDB(const std::string &cuenta, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "' AND ESTADO = '0'";
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
    error = "Error al verificar si la cuenta FTP " + cuenta + " se encuenta deshabilitada";
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioFTP::verificarExisteDirectorioDB(const std::string &directorio, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP WHERE DIRECTORIO = '" << directorio
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
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
    error = "Error al verificar si existe el directorio " + directorio;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioFTP::verificarExisteFTPDB(const std::string &cuenta, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
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
    error = "Error al verificar si existe la cuenta FTP " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioFTP::verificarHabilitadaFTPDB(const std::string &cuenta, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "' AND ESTADO = '1'";
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
    error = "Error al verificar si la cuenta FTP " + cuenta + " se encuenta habilitada";
    dominio.loguear(error);
    return(resultado);
  }
}
