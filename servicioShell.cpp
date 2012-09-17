#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioShell.h"

using namespace std;
using namespace mysqlpp;

cServicioShell::cServicioShell(cDominio *dom):cServicio(dom) {
  servicioDB = "ACCESO SSH";
}

bool cServicioShell::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioShell::cambiarClaveShell(const std::string &password, cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Cambiar el password de la cuenta shell
  comando = "echo pdu" + dominio.getIdDominio() + "-shell" + ":" + password + " | chpasswd";
  if (system(comando.c_str()) < 0) {
    comando = "Error al cambiar el password de la cuenta shell pdu" + dominio.getIdDominio()
      + "-shell";
    dominio.loguear(comando);
    return(-1);
  }
  
  return(0);
}

int cServicioShell::cambiarClaveShellDB(const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE USUARIO_SHELL SET PASSWORD = '" << password << "' WHERE ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password de la cuenta Shell";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioShell::habilitarShellDB(const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE USUARIO_SHELL SET PASSWORD = '" << password << "', ESTADO = '1' "
	<< "WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar la cuenta Shell";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioShell::deshabilitarShellDB(const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE USUARIO_SHELL SET PASSWORD = '" << password << "', ESTADO = '0' "
	<< "WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar la cuenta Shell";
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioShell::recuperarPasswordDB(cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT PASSWORD FROM USUARIO_SHELL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
	<< "'";
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
    error = "Error al recuperar el password de la cuenta Shell";
    dominio.loguear(error);
    return(resultado);
  }
}

std::string cServicioShell::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM USUARIO_SHELL WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioShell::verificarDeshabilitadaShellDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM USUARIO_SHELL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
	<< "' AND ESTADO = '0'" << endl;
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
    error = "Error al verificar si la cuenta Shell se encuenta deshabilitada";
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioShell::verificarExisteShellDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM USUARIO_SHELL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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
    error = "Error al verificar si existe la cuenta Shell";
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioShell::verificarHabilitadaShellDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM USUARIO_SHELL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
	<< "' AND ESTADO = '1'" << endl;
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
    error = "Error al verificar si la cuenta Shell se encuenta habilitada";
    dominio.loguear(error);
    return(resultado);
  }
}
