#include <fstream>
#include <string>
#include <mysql++.h>
#include "log.h"
#include "configuracion.h"
#include "dominio.h"

using namespace std;
using namespace mysqlpp;

cDominio::cDominio() {
  //Recuperar el nombre del dominio
  ifstream in("./.site");
  if (in.is_open()) {
    getline(in, dominio);
    in.close();
  }
  //Inicia la clase de logueo
  log = new cLog(dominio);
  //Conectarse a la base de datos
  con = new Connection(use_exceptions);
  con.connect(DB_NAME, DB_HOST, DB_USER, DB_PASS);
}

bool cDominio::iniciar() {
  bool resultado = true;

  //Recuperar la identidad del dominio, el usuario, el password y el password FTP
  try {
    Query qry = con.query();
    qry << "SELECT ID_DOMINIO, USUARIO, PASSWORD, PASSWORD_FTP FROM DOMINIO WHERE DOMINIO = '" 
	<< dominio << "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      idDominio = std::string(row[0]);
      usuario = std::string(row[1]);
      password = std::string(row[2]);
      passwordFTP = std::string(row[3]);
    }
  }
  catch (BadQuery er) {
    log->loguear(er.error);
  }
  catch (...) {
    string error;
    error = "Error al recuperar la identidad del dominio";
    log->loguear(error);
  }

  //Recuperar el tipo de dominio
  try {
    Query qry = con.query();
    qry << "SELECT * FROM DOM_EMP, DOMINIO WHERE DOM_EMP.ID_DOMINIO = DOMINIO.ID_DOMINIO AND "
	<< "DOMINIO.ID_DOMINIO = '" << idDominio << "'";
    Result res = qry.store();
    Row row;
    if (res.size() > 0) {
      tipo = "Empresa";
    } else {
      qry << "SELECT * FROM DOM_REV_PAQ, DOMINIO WHERE DOM_REV_PAQ.ID_DOMINIO = DOMINIO.ID_DOMINIO"
	  << " AND DOMINIO.ID_DOMINIO = '" << idDominio << "'";
      res = qry.store();
      if (res.size() > 0) {
	tipo = "RevendedorPaquete";
      } else {
	qry << "SELECT * FROM DOM_REV_FREE, DOMINIO WHERE DOM_REV_FREE.ID_DOMINIO = "
	    << "DOMINIO.ID_DOMINIO AND DOMINIO.ID_DOMINIO = '" << idDominio << "'";
	res = qry.store();
	if (res.size() > 0)
	  tipo = "RevendedorFree";
      }
    }
  }
  catch (BadQuery er) {
    log->loguear(er.error);
  }
  catch (...) {
    string error;
    error = "Error al recuperar el tipo de dominio";
    log->loguear(error);
  }

  //Verificar que todo se haya asignado y que la base de datos este conectada
  if ((dominio.length() <= 0) && (idDominio.length() <= 0) && (tipo.length() <= 0))
    resultado = false;
  if (!con.connected())
    resultado = false;
  return(resultado);
}

void cDominio::loguear(std::string mensaje) {
  log->loguear(mensaje);
}

void cDominio::setIdDominio(std::string idDom) {
  idDominio = idDom;
}

void cDominio::setTipo(std::string t) {
  tipo = t;
}

std::string cDominio::getIdDominio() const {
  return(idDominio);
}

std::string cDominio::getPassword() const {
  return(password);
}

std::string cDominio::getPasswordFTP() const {
  return(passwordFTP);
}

std::string cDominio::getDominio() const {
  return(dominio);
}

std::string cDominio::getTipo() const {
  return(tipo);
}

std::string cDominio::getUrl() const {
  string url("www." + dominio);
  return(url);
}

std::string cDominio::getUsuario() const {
  return(usuario);
}
