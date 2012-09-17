#include <string>
#include <mysql++.h>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <cstring>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioPOP3.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicioPOP3::cServicioPOP3(cDominio *dom):cServicio(dom) {
  servicioDB = "POP3";
}

bool cServicioPOP3::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioPOP3::agregarEmail(const std::string &cuenta, const std::string &password,
				cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Ingresar el nuevo usuario en el sistema
  comando = "useradd -d /home/" + cuenta + "-" + dominio.getIdDominio() + " -m -g nobody -c "
    + cuenta + "@" + dominio.getDominio() + " -s /bin/false " + cuenta + "-"
    + dominio.getIdDominio();
  if (system(comando.c_str()) < 0) {
    comando = "Error al agregar el usuario de sistema " + cuenta + "-" + dominio.getIdDominio()
      + " para la cuenta POP3 del dominio " + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Cambiar el password del usuario de sistema creado
  comando = "echo " + cuenta + "-" + dominio.getIdDominio() + ":" + password + " | chpasswd";
  if (system(comando.c_str()) < 0) {
    comando = "Error al cambiar el password del usuario de sistema " + cuenta + "-" 
      + dominio.getIdDominio() + " para la cuenta POP3 del dominio " + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Agregar la cuenta al archivo virtusertable
  if (agregarFileVirtusertable(cuenta, dominio) < 0) {
    comando = "Error al agregar la cuenta " + cuenta + "@" + dominio.getDominio()
      + " al archivo virtusertable";
    dominio.loguear(comando);
    return(-1);
  }
  
  //Agregar la cuenta al archivo spam.whitelist.rules
  /*if (agregarFileSpamWhitelist(cuenta, dominio) < 0) {
    comando = "Error al agregar la cuenta " + cuenta + "@" + dominio.getDominio()
      + " al archivo spam.whitelist.rules";
    dominio.loguear(comando);
    return(-1);
  }*/

  //Agregar la cuenta al archivo access
  /*
  if (agregarFileAccess(cuenta, dominio) < 0) {
    comando = "Error al agregar la cuenta " + cuenta + "@" + dominio.getDominio()
      + " al archivo access";
    dominio.loguear(comando);
    return(-1);
  }
  */

  //Agregarle la quota al usuario
  comando = "setquota -au " + cuenta + "-" + dominio.getIdDominio() + " 150000 150000 0 0";
  if(system(comando.c_str()) < 0) {
    comando = "Error al cambiar las cuotas del usuario de sistema " + cuenta + "-"
      + dominio.getIdDominio() + " para la cuenta POP3 del dominio " + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }
  
    //Agregarle la quota al usuario
  comando = "setquota -au " + cuenta + "-" + dominio.getIdDominio() + " 150000 150000 0 0";
  if(system(comando.c_str()) < 0) {
    comando = "Error al cambiar las cuotas del usuario de sistema " + cuenta + "-"
      + dominio.getIdDominio() + " para la cuenta POP3 del dominio " + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar el archivo access y virtusertable
  //actualizarFileAccess();
  actualizarFileVirtusertable();

  return(0);
}

int cServicioPOP3::agregarEmailDB(const std::string &cuenta, const std::string &password,
				  cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO CUENTA_POP3(CUENTA, ID_DOMINIO, PASSWORD) VALUES ('" << cuenta << "', '"
	<< dominio.getIdDominio() << "', '" << password << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la cuenta POP3 " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioPOP3::agregarFileAccess(const std::string &cuenta, cDominio &dominio) {
  return(0);

  //Abrir el archivo para escritura
  ofstream out("/etc/mail/access", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /etc/mail/access");
    return(-1);
  }

  //Agregar la linea
  out << "From:" << "\t" << cuenta << "@" << dominio.getDominio() << "\tyes" << endl;

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioPOP3::agregarFileVirtusertable(const std::string &cuenta, cDominio &dominio) {
  //Abrir el archivo para escritura
  ofstream out("/etc/mail/virtusertable", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /etc/mail/virtusertable");
    return(-1);
  }

  //Agregar la linea
  out << cuenta << "@" << dominio.getDominio() << "\t" << cuenta << "-" << dominio.getIdDominio()
      << endl;

  //Cerar el archivo
  out.close();

  return(0);
}

int cServicioPOP3::agregarFileSpamWhitelist(const std::string &cuenta, cDominio &dominio) {
  //Abrir el archivo para escritura
  ofstream out("/opt/MailScanner/etc/rules/spam.whitelist.rules", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /opt/MailScanner/etc/rules/spam.whitelist.rules");
    return(-1);
  }

  //Agregar la linea
  out << "From:" << "\t" << cuenta << "@" << dominio.getDominio() << "\tyes"
      << endl;

  //Cerar el archivo
  out.close();

  return(0);
}

int cServicioPOP3::cambiarClavePOP3(const std::string &cuenta, const std::string &password,
				    cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Cambiar el password del usuario de sistema creado
  comando = "echo " + cuenta + "-" + dominio.getIdDominio() + ":" + password + " | chpasswd";
  if (system(comando.c_str()) < 0) {
    comando = "Error al cambiar el password del usuario de sistema " + cuenta + "-" 
      + dominio.getIdDominio() + " para la cuenta POP3 del dominio " + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioPOP3::cambiarClavePOP3DB(const std::string &cuenta, const std::string &password,
				      cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE CUENTA_POP3 SET PASSWORD = '" << password << "' WHERE ID_DOMINIO = '"
	<< dominio.getIdDominio() << "' AND CUENTA = '" << cuenta << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password de la cuenta POP3 " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioPOP3::quitarEmail(const std::string &cuenta, cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Quitar el usuario del sistema
  comando = "userdel " + cuenta + "-" + dominio.getIdDominio();
  if (system(comando.c_str()) < 0) {
    comando = "Error al quitar el usuario de sistema " + cuenta + "-" + dominio.getIdDominio()
      + " de la cuenta POP3 del dominio " + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Eliminar los archivos que quedaron para el usuario de sistema
  comando = "rm -fr /home/" + cuenta + "-" + dominio.getIdDominio();
  system(comando.c_str());

  //Quitar la cuenta al archivo access
  /*
  if (quitarFileAccess(cuenta, dominio) < 0) {
    comando = "Error al quitar la cuenta " + cuenta + "@" + dominio.getDominio()
      + " del archivo access";
    dominio.loguear(comando);
    return(-1);
  }
  */

  //Quitar la cuenta del archivo virtusertable
  if (quitarFileVirtusertable(cuenta, dominio) < 0) {
    comando = "Error al quitar la cuenta " + cuenta + "@" + dominio.getDominio()
      + " al archivo virtusertable";
    dominio.loguear(comando);
    return(-1);
  }
  
    //Quitar la cuenta del archivo spam.whitelist.rules
  /*if (quitarFileSpamWhitelist(cuenta, dominio) < 0) {
    comando = "Error al quitar la cuenta " + cuenta + "@" + dominio.getDominio()
      + " al archivo spam.whitelist.rules";
    dominio.loguear(comando);
    return(-1);
  }*/

  //Actualizar el archivo access y virtusertable
  //actualizarFileAccess();
  actualizarFileVirtusertable();

  return(0);
}

int cServicioPOP3::quitarFileAccess(const std::string &cuenta, cDominio &dominio) {
  return(0);
  //Mover el archivo para actualizarlo
  system("mv -f /etc/mail/access /etc/mail/access.bak");

  //Abrir el archivo para lectura
  ifstream in("/etc/mail/access.bak");
  if (!in.is_open()) {
    system("mv -f /etc/mail/access.bak /etc/mail/access");
    dominio.loguear("No se pudo abrir el archivo /etc/mail/access.bak");
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out("/etc/mail/access");
  if (!out.is_open()) {
    system("mv -f /etc/mail/access.bak /etc/mail/access");
    dominio.loguear("No se pudo abrir el archivo /etc/mail/access");
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada;
  buscada = "From:" + cuenta + "@" + dominio.getDominio() + "\tRELAY";
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) == string::npos)
      out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

int cServicioPOP3::quitarFileVirtusertable(const std::string &cuenta, cDominio &dominio) {
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
  buscada = cuenta + "@" + dominio.getDominio() + "\t" + cuenta + "-" + dominio.getIdDominio();
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) == string::npos)
      out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

int cServicioPOP3::quitarFileSpamWhitelist(const std::string &cuenta, cDominio &dominio) {
  //Mover el archivo para actualizarlo
  system("mv -f /opt/MailScanner/etc/rules/spam.whitelist.rules /opt/MailScanner/etc/rules/spam.whitelist.rules.bak");

  //Abrir el archivo para lectura
  ifstream in("/opt/MailScanner/etc/rules/spam.whitelist.rules.bak");
  if (!in.is_open()) {
    system("mv -f /opt/MailScanner/etc/rules/spam.whitelist.rules.bak /opt/MailScanner/etc/rules/spam.whitelist.rules");
    dominio.loguear("No se pudo abrir el archivo /opt/MailScanner/etc/rules/spam.whitelist.rules.bak");
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out("/opt/MailScanner/etc/rules/spam.whitelist.rules");
  if (!out.is_open()) {
    system("mv -f /opt/MailScanner/etc/rules/spam.whitelist.rules.bak /opt/MailScanner/etc/rules/spam.whitelist.rules");
    dominio.loguear("No se pudo abrir el archivo /opt/MailScanner/etc/rules/spam.whitelist.rules");
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada;
    buscada = "From:\t" + cuenta + "@" + dominio.getDominio() + "\tyes";
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) == string::npos)
      out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

int cServicioPOP3::quitarEmailDB(const std::string &cuenta, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM CUENTA_POP3 WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la cuenta POP3 " + cuenta;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioPOP3::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_POP3 WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioPOP3::verificarExisteEmailDB(const std::string &cuenta, cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_POP3 WHERE CUENTA = '" << cuenta << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'" << endl;
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
    error = "Error al verificar si existe la cuenta POP3 " + cuenta;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioPOP3::verificarExisteEmailsDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_POP3 WHERE ID_DOMINIO = '" << dominio.getIdDominio()
	<< "'";
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
    error = "Error al verificar si existen cuentas POP3";
    dominio.loguear(error);
    return(resultado);
  }
}
