#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioCatchAll.h"

using namespace std;
using namespace mysqlpp;

cServicioCatchAll::cServicioCatchAll(cDominio *dom):cServicio(dom) {
  servicioDB = "CATCH ALL";
}

bool cServicioCatchAll::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}


int cServicioCatchAll::agregarCatchAll(cDominio &dominio) {

//Abrir el archivo para escritura
    ofstream out("/etc/mail/virtusertable", ios::app);
    if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /etc/mail/virtusertable");
    return(-1);
}
							    
//Agregar la linea
out << "@" + dominio.getDominio() + "\t" + "pdu" + "-" + dominio.getIdDominio() << endl;
								
//Cerar el archivo
out.close();
			    
return(0);
}

int cServicioCatchAll::quitarCatchAll(cDominio &dominio) {

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
    buscada = "@" + dominio.getDominio() + "\t" + "pdu" + "-" + dominio.getIdDominio();
    while(in.getline(buffer, MINLINE)) {
      linea = buffer;
      if (linea.find(buscada) == string::npos)
        out << linea << endl;
    }
    
//Cerrar los archivos
  in.close();
  out.close();

//Actualizar el archivo virtusertable
  actualizarFileVirtusertable();
    
  return(0);
  
}    

int cServicioCatchAll::habilitarCatchAllDB(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO CATCH_ALL (ID_DOMINIO , ESTADO) VALUES('" << dominio.getIdDominio() << "', '1')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar la cuenta Catch All";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioCatchAll::deshabilitarCatchAllDB(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE CATCH_ALL SET ESTADO = '0' "
	<< "WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar la cuenta Catch All";
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioCatchAll::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM CATCH_ALL WHERE ID_DOMINIO = '" << dominio->getIdDominio()
	<< "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      cantidad = row[0];
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

int cServicioCatchAll::verificarDeshabilitadaCatchAllDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CATCH_ALL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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

int cServicioCatchAll::verificarExisteCatchAllDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CATCH_ALL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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

int cServicioCatchAll::verificarHabilitadaCatchAllDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CATCH_ALL WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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
