#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "configuracion.h"
#include "servicio.h"
#include "servicioASP.h"

#include <vector>

#include <iostream>

using namespace std;
using namespace mysqlpp;

cServicioASP::cServicioASP(cDominio *dom):cServicio(dom) {
  servicioDB = "ASP";
}

bool cServicioASP::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioASP::agregarASP(std::vector<std::string> &nombreDB,
			     std::vector<std::string> &password,
			     cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Modificar el archivo odbc.ini para el nuevo dominio
  if (agregarFileOdbcIni(nombreDB, password, dominio) < 0) {
    comando = "Error al modificar el archivo odbc.ini";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar Casp.cnfg
  comando = "/opt/casp/configure-server function=configure-engine engine=asp-server-3000 mode=add 'section=virtual hosts' key='www." + dominio.getDominio() + ":80'";
  if (system(comando.c_str()) < 0)
    return(-1);

  // Reinicar el servicio
  reiniciarCaspd();

  // Reinicar el servicio
  reiniciarApache();
  
  return(0);
}

int cServicioASP::agregarASPDB(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO ASP(ID_DOMINIO, ESTADO) VALUES('" << dominio.getIdDominio() << "', '1')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar el soporte ASP";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioASP::agregarFileOdbcIni(std::vector<std::string> &nombreDB,
				     std::vector<std::string> &password,
				     cDominio &dominio) {
  std::string archivo, archivo2, comando;

  //Copiar el archivo de configuracion para parsearlo
  ///opt/casp/asp-server-3000/odbc.ini
  archivo = "/opt/casp/asp-server-3000/odbc.ini";
  archivo2 = "/opt/casp/asp-server-3000/odbc.ini.bak";
  comando = "cp -f " + archivo + " " + archivo2;
  if (system(comando.c_str()) < 0)
    return(-1);

  // Abrir el archivo obdc.ini para actualizar
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las paginas de errores";
    return(-1);
  }

  // Abrir el archivo odbc.ini.bak en modo lectura para parsear
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo2 + " para las paginas de errores";
    return(-1);
  }

  std::string buscada, hasta, info;
  //Llenar en buscada la cadena que se debe de buscar
  buscada = "Text_template=";
  //Llenar en hasta la cadena donde se debe dejar de ignorar
  hasta = "[DB2_template]";

  //Parsear el archivo
  char buffer[MINLINE];
  std::string linea;
  while (in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) != string::npos) {
      while (linea.find(hasta) == string::npos) {
	if (linea != "")
	  out << linea << endl;
	in.getline(buffer, MINLINE);
	linea = buffer;
      }
      for (unsigned int p = 0; p < nombreDB.size(); p++)
	out << nombreDB[p] << "=www." << dominio.getDominio() << endl;
      out << endl;
    }
    out << linea << endl;
  }

  //Agregar la seccion de las bases de datos
  //Si la ultima linea no era una linea vacia, agregar una
  if (linea.length() > 0)
    out << endl;
  //Agregar las secciones
  for (unsigned int p = 0; p < nombreDB.size(); p++) {
    if (p != 0)
      out << endl;
    out << "[" << nombreDB[p] << "]" << endl
	<< "Description=www." << dominio.getDominio() << endl
	<< "Driver=/opt/casp/odbc/opensource/lib/libmyodbc3.so" << endl
	<< "Server=localhost" << endl
	<< "Port=3306" << endl
	<< "Database=" << nombreDB[p] << endl
	<< "User=pdu" << dominio.getIdDominio() << endl
	<< "Password=" << password[p] << endl
	<< "UseCursorLib=1" << endl
	<< "Type=MySQL" << endl;
  }

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioASP::cambiarClave(std::vector<std::string> &nombreDB,
			       std::vector<std::string> &password,
			       cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Modificar el archivo odbc.ini para el nuevo dominio
  if (modificarFileOdbcIni(nombreDB, password, dominio) < 0) {
    comando = "Error al modificar el archivo odbc.ini";
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioASP::modificarFileOdbcIni(std::vector<std::string> &nombreDB,
				       std::vector<std::string> &password,
				       cDominio &dominio) {
  std::string archivo, archivo2, comando;

  //Copiar el archivo de configuracion para parsearlo
  archivo = "/opt/casp/asp-server-3000/odbc.ini";
  archivo2 = "/opt/casp/asp-server-3000/odbc.ini.bak";
  comando = "cp -f " + archivo + " " + archivo2;
  if (system(comando.c_str()) < 0)
    return(-1);

  /* Abrir el archivo obdc.ini para actualizar */
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las paginas de errores";
    return(-1);
  }

  /* Abrir el archivo odbc.ini.bak en modo lectura para parsear */
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo2 + " para las paginas de errores";
    return(-1);
  }

  std::string buscada, hasta, buscada2;
  //Llenar en buscada la cadena que se debe de buscar
  buscada = "[pdu";
  buscada += dominio.getIdDominio();
  buscada2 = "Password=";
  //Llenar en hasta la cadena donde se debe dejar de ignorar
  hasta = "[";

  //Parsear el archivo
  char buffer[MINLINE];
  std::string linea;
  int bandera = 1;
  int ind = 0;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) != string::npos)
      bandera = 0;
    else if ((!bandera) && (linea.find(hasta) != string::npos)) {
	if (linea.find(buscada) == string::npos)
	  bandera = 1;
    } else if ((!bandera) && (linea.find(buscada2) != string::npos)) {
      linea = "Password=";
      linea += password[ind];
      ind++;
    }
    out << linea << endl;
  }

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioASP::quitarASP(std::vector<std::string> &nombreDB,
			    std::vector<std::string> &password,
			    cDominio &dominio) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Modificar el archivo odbc.ini para el nuevo dominio
  if (quitarFileOdbcIni(nombreDB, password, dominio) < 0) {
    comando = "Error al modificar el archivo odbc.ini";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar Casp.cnfg
  comando = "/opt/casp/configure-server function=configure-engine engine=asp-server-3000 mode=del 'section=virtual hosts' key='www." + dominio.getDominio() + ":80'";
  if (system(comando.c_str()) < 0)
    return(-1);

  // Reinicar el servicio
  reiniciarCaspd();

  // Reinicar el servicio
  reiniciarApache();

  return(0);
}

int cServicioASP::quitarASPDB(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM ASP WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar el soporte ASP";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioASP::reiniciarCaspd() {
  //Actualizar Casp.cnfg
  std::string comando;
  comando = "/opt/casp/asp-server-3000/restartcaspd";
  if (system(comando.c_str()) < 0)
    return(-1);
  return(0);
}

int cServicioASP::reiniciarApache() {
  //Reiniciar servicio
  std::string comando;
  comando = "/opt/casp/asp-server-3000/restartapache";
  if (system(comando.c_str()) < 0)
    return(-1);
  return(0);
}

std::string cServicioASP::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM ASP WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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


int cServicioASP::quitarFileOdbcIni(std::vector<std::string> &nombreDB,
				    std::vector<std::string> &password,
				    cDominio &dominio) {
  std::string archivo, archivo2, comando;

  //Copiar el archivo de configuracion para parsearlo
  archivo = "/opt/casp/asp-server-3000/odbc.ini";
  archivo2 = "/opt/casp/asp-server-3000/odbc.ini.bak";
  comando = "cp -f " + archivo + " " + archivo2;
  if (system(comando.c_str()) < 0)
    return(-1);

  // Abrir el archivo obdc.ini para actualizar
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para la deshabilitacion del ODBC";
    return(-1);
  }

  // Abrir el archivo odbc.ini.bak en modo lectura para parsear
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo2 + " para la deshabilitacion del ODCB";
    return(-1);
  }

  std::string buscada, hasta, buscada2, buscada3, buscada4;
  //Llenar en buscada la cadena que se debe de buscar
  buscada = "Text_template=";
  buscada2 = dominio.getDominio();
  buscada3 = "[pdu";
  buscada3 += dominio.getIdDominio();
  buscada3 += "_";
  buscada4 = "[";
  //Llenar en hasta la cadena donde se debe dejar de ignorar
  hasta = "[DB2_template]";

  //Parsear el archivo
  char buffer[MINLINE];
  std::string linea;
  int bandera = 1;
  while (bandera) {
    in.getline(buffer, MINLINE);
    linea = buffer;
    if (linea.find(buscada) != string::npos) {
      while (linea.find(hasta) == string::npos) {
	if (linea.find(buscada2) == string::npos)
	  out << linea << endl;
	in.getline(buffer, MINLINE);
	linea = buffer;
      }
      bandera = 0;
    }
    out << linea << endl;
  }

  //Parsear las secciones
  bandera = 1;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada3) != string::npos)
      bandera = 0;
    else
      if ((!bandera) && (linea.find(buscada4) != string::npos)) {
	if (linea.find(buscada3) == string::npos)
	  bandera = 1;
      }
    if (bandera)
      out << linea << endl;
  }

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioASP::verificarExisteASPDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM ASP WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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
    error = "Error al verificar si existe el soporte ASP";
    dominio.loguear(error);
    return(resultado);
  }
}
