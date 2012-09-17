#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioFTPAdmin.h"

using namespace std;
using namespace mysqlpp;

cServicioFTPAdmin::cServicioFTPAdmin(cDominio *dom):cServicio(dom) {
  servicioDB = "ACCESO_FTP";
}

bool cServicioFTPAdmin::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  if (limite.length() <= 0)
    resultado = false;
  return(resultado);
}

int cServicioFTPAdmin::cambiarClaveFTPAdmin(const std::string &password, cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Cambiar el password del usuario en el sistema
  comando = "echo pdu" + dominio.getIdDominio() + ":" + password + " | chpasswd";
  if(system(comando.c_str()) < 0) {
    comando = "Error al cambiar el password del sistema para el usuario pdu"
      + dominio.getIdDominio();
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioFTPAdmin::cambiarClaveFTPAdminDB(const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE DOMINIO SET PASSWORD_FTP = '" << password << "' WHERE ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password FTP del administrador";
    dominio.loguear(error);
    return(-2);
  }
}
