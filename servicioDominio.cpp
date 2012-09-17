#include <string>
#include <mysql++.h>
#include "tiempo.h"
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioDominio.h"

#include <iostream>

using namespace std;
using namespace mysqlpp;

cServicioDominio::cServicioDominio(cDominio *dom):cServicio(dom) {
  servicioDB = "DOMINIO";
}

bool cServicioDominio::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  limite = "0";
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

std::string cServicioDominio::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM DOMINIO WHERE ID_DOMINIO = '"
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

int cServicioDominio::verificarExisteDominioDB(const std::string &dom, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM DOMINIO WHERE DOMINIO = '" << dom << "'";
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
    error = "Error al verificar si existe el dominio " + dom;
    dominio.loguear(error);
    return(resultado);
  }
}
