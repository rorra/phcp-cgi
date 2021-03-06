#include <string>
#include <mysql++.h>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include "tiempo.h"
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioMultidominio.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicioMultidominio::cServicioMultidominio(cDominio *dom):cServicio(dom) {
  servicioDB = "MULTIDOMINIO";
}

bool cServicioMultidominio::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioMultidominio::agregarFileHttpdConf(const std::string &multi,
						cDominio &dominio) {
  //Abrir el archivo para escritura
  ofstream out("/usr/local/apache/conf/extra/httpd-vhosts.conf", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /usr/local/apache/conf/extra/httpd-vhosts.conf");
    return(-1);
  }

  //Agregar las lineas
  out << endl
      << "#################### Multiple Domain: www." << multi << " ####################" << endl
      << "<VirtualHost " << SERVER_IP << ":80>" << endl
      << "ServerAdmin info@" << dominio.getDominio() << endl
      << "DocumentRoot /www/docs/" << dominio.getDominio() << "/public_html" << endl
      << "ServerName www." << multi << endl
      << "ServerAlias *." << multi << endl
      << "RewriteEngine on" << endl
      << "RewriteRule ^/phcm(.*) http://panel2.powered-hosting.com:81/phcm/" << dominio.getDominio() << " [R,L]" << endl
      << "RewriteRule ^/webmail(.*) http://panel2.powered-hosting.com:81/phcm/" << dominio.getDominio() << "/webmail [R,L]" << endl
      << "RewriteRule ^/soporte(.*) http://panel2.powered-hosting.com:81/phcm/" << dominio.getDominio() << "/SoportePD.php [R,L]" << endl
      << "ErrorLog /www/docs/" << dominio.getDominio() << "/error-log.txt" << endl
      << "CustomLog /www/docs/" << dominio.getDominio() << "/access-log.txt combined" << endl
      << "Options ExecCGI Includes Indexes" << endl
      << "php_admin_value safe_mode 1" << endl
      << "php_admin_value safe_mode_exec_dir /www/docs/" << dominio.getDominio() << "/" << endl
      << "</VirtualHost>" << endl
      << "#####################################################################################" << endl;

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioMultidominio::agregarFileNamed(const std::string &multi,
					    cDominio &dominio) {
  std::string archivo;
  Tiempo tiempo;

  //Abrir el archivo para escritura
  archivo = "/var/named/named." + multi;
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear(string("No se pudo abrir el archivo " + archivo));
    return(-1);
  }

  //Agregar al archivo la configuracion
  out << "$TTL 1H" << endl
      << ";" << endl
      << ";zone file for " << multi << endl
      << ";" << endl
      << "@\tIN\tSOA\t" << multi << ". root." << multi << ". (" << endl
      << "\t\t\t" << tiempo.getAnio() << tiempo.getMes() << tiempo.getDia() << "00" << endl
      << "\t\t\t8H" << endl
      << "\t\t\t2H" << endl
      << "\t\t\t1W" << endl
      << "\t\t\t1D )" << endl
      << "\t\tNS\tns" << endl
      << "\t\tTXT\t\"Powered Hosting\"" << endl
      << "\t\tTXT\t\"v=spf1 ip4:" << SERVER_IP << " ip4:" << SERVER_IP2 << " mx ptr -all\"" << endl
      << "\t\tMX\t10\tmail." << multi << "." << endl
      << "\t\tMX\t20\tmail2." << multi << "." << endl
      << "@\t\tA\t" << SERVER_IP << endl
      << "www\t\tA\t" << SERVER_IP << endl
      << "ftp\t\tA\t" << SERVER_IP << endl
      << "irc\t\tA\t" << SERVER_IP << endl
      << "pop3\t\tA\t" << SERVER_IP << endl
      << "stmp\t\tA\t" << SERVER_IP << endl
      << "mail\t\tA\t" << SERVER_IP << endl
      << "mail2\t\tA\t" << SERVER_IP2 << endl
      << "webmail\t\tA\t" << SERVER_IP << endl
      << "ns\t\tA\t" << SERVER_IP << endl
      << "*\t\tA\t" << SERVER_IP << endl;

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioMultidominio::agregarFileNamedConf(const std::string &redir,
						cDominio &dominio) {
  //Abrir el archivo para escritura
  ofstream out("/etc/named.conf", ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo /etc/named.conf");
    return(-1);
  }

  //Agregar las lineas
  out << endl
      << "zone \"" << redir << "\" {" << endl
      << "\ttype master;" << endl
      << "\tnotify no;" << endl
      << "\tfile \"named." << redir << "\";" << endl
      << "};" << endl;

  //Cerrar el archivo
  out.close();

  return(0);
}


int cServicioMultidominio::agregarMultidominio(const std::string &multidominio,
					       cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Crear el archivo named.dominio
  if (agregarFileNamed(multidominio, dominio) < 0) {
    comando = "Error al crear el archivo named." + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Modificar el archivo named.conf
  if (agregarFileNamedConf(multidominio, dominio) < 0) {
    comando = "Error al modificar el archivo named.conf";
    dominio.loguear(comando);
    return(-1);
  }

  //Modificar el archivo httpd-vhosts.conf
  if (agregarFileHttpdConf(multidominio, dominio) < 0) {
    comando = "Error al modificar el archivo httpd-vhosts.conf";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar los DNS
  actualizarDNS();

  return(0);
}

int cServicioMultidominio::agregarMultidominioDB(const std::string &multidominio,
						 cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO MULTIDOMINIO(MULTIDOMINIO, ID_DOMINIO) VALUES('" << multidominio
	<< "', '" << dominio.getIdDominio() << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar el multidominio " + multidominio;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioMultidominio::quitarFileHttpdConf(const std::string &multi,
					       cDominio &dominio) {
  std::string archivo, archivo2, comando;
  archivo = "/usr/local/apache/conf/extra/httpd-vhosts.conf";
  archivo2 = "/usr/local/apache/conf/extra/httpd-vhosts.conf.bak";

  //Mover el archivo para actualizarlo
  comando = "mv -f " + archivo + " " + archivo2;
  system(comando.c_str());

  //Abrir el archivo para lectura
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "mv -f " + archivo2 + " " + archivo;
    system(comando.c_str());
    comando = "No se pudo abrir el archivo " + archivo;
    dominio.loguear(comando);
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "mv -f " + archivo2 + " " + archivo;
    system(comando.c_str());
    comando = "No se pudo abrir el archivo " + archivo;
    dominio.loguear(comando);
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada, hasta;
  buscada = "#################### Multiple Domain: www." + multi + " #";
  hasta = "</VirtualHost>";
  int estado = 1;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    
    if (estado == 1) {
      if (linea.find(buscada) != string::npos)
	estado = 2;
      else
	out << linea << endl;
    }
    else if (estado == 2) {
      if (linea.find(hasta) != string::npos) {
	in.getline(buffer, MINLINE);
	in.getline(buffer, MINLINE);
	estado = 1;
      }
    }
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

int cServicioMultidominio::quitarFileNamed(const std::string &multi,
					   cDominio &dominio) {
  std::string archivo, comando;

  //Eliminar el archivo
  archivo = "/var/named/named." + multi;
  if (unlink(archivo.c_str()) < 0) {
    comando = "Error al eliminar el archivo " + archivo;
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioMultidominio::quitarFileNamedConf(const std::string &multi,
					       cDominio &dominio) {
  //Mover el archivo para actualizarlo
  system("mv -f /etc/named.conf /etc/named.conf.bak");

  //Abrir el archivo para lectura
  ifstream in("/etc/named.conf.bak");
  if (!in.is_open()) {
    system("mv -f /etc/named.conf.bak /etc/named.conf");
    dominio.loguear("No se pudo abrir el archivo /etc/named.conf");
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out("/etc/named.conf");
  if (!out.is_open()) {
    system("mv -f /etc/named.conf.bak /etc/named.conf");
    dominio.loguear("No se pudo abrir el archivo /etc/named.conf");
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada, hasta;
  buscada = "zone \"" + multi + "\" {";
  hasta = "};";
  int estado = 1;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    
    if (estado == 1) {
      if (linea.find(buscada) != string::npos)
	estado = 2;
      else
	out << linea << endl;
    }
    else if (estado == 2) {
      if (linea.find(hasta) != string::npos) {
	in.getline(buffer, MINLINE);
	estado = 1;
      }
    }
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

int cServicioMultidominio::quitarMultidominio(const std::string &multidominio,
					      cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Eliminar el archivo named.dominio
  if (quitarFileNamed(multidominio, dominio) < 0) {
    comando = "Error al eliminar el archivo named." + dominio.getDominio();
    dominio.loguear(comando);
    return(-1);
  }

  //Modificar el archivo named.conf
  if (quitarFileNamedConf(multidominio, dominio) < 0) {
    comando = "Error al modificar el archivo named.conf";
    dominio.loguear(comando);
    return(-1);
  }

  //Modificar el archivo httpd-vhosts.conf
  if (quitarFileHttpdConf(multidominio, dominio) < 0) {
    comando = "Error al modificar el archivo httpd-vhosts.conf";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar los DNS
  actualizarDNS();

  return(0);
}

int cServicioMultidominio::quitarMultidominioDB(const std::string &multidominio,
						      cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM MULTIDOMINIO WHERE MULTIDOMINIO = '"
	<< multidominio << "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar el multidominio " + multidominio;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioMultidominio::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM MULTIDOMINIO WHERE ID_DOMINIO = '"
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

int cServicioMultidominio::verificarExisteMultidominioDB(const std::string &multidominio,
							       cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM MULTIDOMINIO WHERE MULTIDOMINIO = '" << multidominio << "'";
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
    error = "Error al verificar si existe el multidominio " + multidominio;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioMultidominio::verificarExisteMultidominioDB(const std::string &multidominio,
							       cDominio &dominio, std::string) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM MULTIDOMINIO WHERE MULTIDOMINIO = '"
	<< multidominio << "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
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
    error = "Error al verificar si existe el multidominio " + multidominio;
    dominio.loguear(error);
    return(resultado);
  }
}
