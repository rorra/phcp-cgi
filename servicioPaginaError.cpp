#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioPaginaError.h"
#include "configuracion.h"
#include <errno.h>

using namespace std;
using namespace mysqlpp;

cServicioPaginaError::cServicioPaginaError(cDominio *dom):cServicio(dom) {
  servicioDB = "PAGINAS ERROR";
}

bool cServicioPaginaError::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioPaginaError::agregarFileHttpdConf(const std::string &codigo, cDominio &dominio) {
  std::string archivo, archivo2, comando;

  //Copiar el archivo de configuracion para parsearlo
  archivo = "/usr/local/apache/conf/extra/httpd-vhosts.conf";
  archivo2 = "/usr/local/apache/conf/extra/httpd-vhosts.conf.bak";
  comando = "cp -f " + archivo + " " + archivo2;
  if (system(comando.c_str()) < 0)
    return(-1);

  /* Abrir el archivo httpd-vhosts.conf para actualizar */
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las paginas de errores";
    return(-1);
  }

  /* Abrir el archivo httpd-vhosts.conf.bak en modo lectura para parsear */
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo2 + " para las paginas de errores";
    return(-1);
  }

  std::string buscada, hasta, info;
  //Llenar en buscada la cadena que se debe de buscar
  buscada = "#################### Virtual Domain: www." + dominio.getDominio();
  //Llenar en hasta la cadena donde se debe dejar de ignorar
  hasta = "</VirtualHost>";
  //Llenar en info la cadena que se debe agregar a la configuracion
  info = "ErrorDocument " + codigo + " /errores/" + codigo + ".html";

  //Parsear el archivo
  char buffer[MINLINE];
  std::string linea;
  while (in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) != string::npos) {
      while (linea.find(hasta) == string::npos) {
	out << linea << endl;
	in.getline(buffer, MINLINE);
	linea = buffer;
      }
      out << info << endl;
    }
    out << linea << endl;
  }

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioPaginaError::agregarFilePagina(const std::string &codigo, cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/errores/" + codigo + ".html";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Agregar las linias
  out << "<HTML>" << endl
      << "<HEAD>" << endl
      << "<TITLE>Pagina de error " << codigo << "</TITLE>" << endl
      << "</HEAD>" << endl
      << "<BODY>" << endl
      << "Aqui debe personalizar la página de error para el codigo " << codigo << "." << endl
      << "</BODY>" << endl
      << "</HTML>" << endl;

  //Cerrar el archivo
  out.close();

  //Darle al directorio los permisos correspondientes
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + "/www/docs/" + dominio.getDominio() + "/public_html/errores/";
  system(comando.c_str());

  //Darle al archivo los permisos correspondientes
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + archivo;
  system(comando.c_str());

  return(0);  
}

int cServicioPaginaError::agregarPaginaError(const std::string &codigo, cDominio &dominio) {
  std::string comando, directorio;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Si no existe crear el directorio para la pagina de error
  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/errores";
  if (!existeDirectorio(directorio))
    if (mkdir(directorio.c_str(), 0755))
      if (errno != EEXIST) {
	dominio.loguear("Error al crear el directorio de paginas de errores");
	return(-1);
      }

  //Crear la pagina de error
  if (agregarFilePagina(codigo, dominio) < 0) {
    dominio.loguear("Error al crear la pagina de error para el codigo " + codigo);
    return(-1);
  }

  //Agregar la pagina de error a la configuracion del apache
  if (agregarFileHttpdConf(codigo, dominio) < 0) {
    dominio.loguear("Error al agregar la pagina de error a la configuracion del apache");
    return(-1);
  }

  return(0);
}

int cServicioPaginaError::agregarPaginaErrorDB(const std::string &codigo, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO PAGINA_ERROR(CODIGO, ID_DOMINIO) VALUES('" << codigo << "', '"
	<< dominio.getIdDominio() << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la pagina de error " + codigo;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioPaginaError::quitarFileHttpdConf(const std::string &codigo, cDominio &dominio) {
  std::string archivo, archivo2, comando;

  //Copiar el archivo de configuracion para parsearlo
  archivo = "/usr/local/apache/conf/extra/httpd-vhosts.conf";
  archivo2 = "/usr/local/apache/conf/extra/httpd-vhosts.conf.bak";
  comando = "cp -f " + archivo + " " + archivo2;
  if (system(comando.c_str()) < 0)
    return(-1);

  /* Abrir el archivo httpd-vhosts.conf para actualizar */
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las paginas de errores";
    return(-1);
  }

  /* Abrir el archivo httpd-vhosts.conf.bak en modo lectura para parsear */
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo2 + " para las paginas de errores";
    return(-1);
  }

  std::string buscada, hasta, info;
  //Llenar en buscada la cadena que se debe de buscar
  buscada = "#################### Virtual Domain: www." + dominio.getDominio();
  //Llenar en hasta la cadena donde se debe dejar de ignorar
  hasta = "</VirtualHost>";
  //Llenar en info la cadena que se debe quitar a la configuracion
  info = "ErrorDocument " + codigo + " /errores/" + codigo + ".html";

  //Parsear el archivo
  char buffer[MINLINE];
  std::string linea;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(buscada) != string::npos) {
      while (linea.find(hasta) == string::npos) {
	if (linea.find(info) == string::npos)
	  out << linea << endl;
	in.getline(buffer, MINLINE);
	linea = buffer;
      }
    }
    out << linea << endl;
  }

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioPaginaError::quitarPaginaError(const std::string &codigo, cDominio &dominio) {
  std::string comando, archivo;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Eliminar el archivo que contiene la pagina de error
  archivo = "/www/docs/" + dominio.getDominio () + "/public_html/errores/" + codigo + ".html";
  unlink(archivo.c_str());

  //Eliminar la pagina de error de la configuracion del apache
  if (quitarFileHttpdConf(codigo, dominio) < 0) {
    dominio.loguear("Error al quitar la pagina de error a la configuracion del apache");
    return(-1);
  }

  //Verificar si existen paginas de error para el dominio
  //si no existen hacer mierda el directorio
  std::string dir = "/www/docs/" + dominio.getDominio() + "/public_html/errores";
  if (!verificarExistenPaginaErrorDB(dominio))
    if(existeDirectorio(dir)) {
      comando = "rm -fr " + dir;
      system(comando.c_str());
    }  

  return(0);
}

int cServicioPaginaError::quitarPaginaErrorDB(const std::string &codigo, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM PAGINA_ERROR WHERE CODIGO = '" << codigo << "' AND ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la pagina de error " + codigo;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioPaginaError::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM PAGINA_ERROR WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioPaginaError::verificarExisteCodigoErrorDB(const std::string &codigo,
						       cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM DESCRIPCION_PAGINAS_ERROR WHERE CODIGO = '" << codigo << "'";
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
    error = "Error al verificar si existe la pagina de error " + codigo;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioPaginaError::verificarExistePaginaErrorDB(const std::string &codigo,
						       cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PAGINA_ERROR WHERE CODIGO = '" << codigo << "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'" << endl;
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
    error = "Error al verificar si existe la pagina de error " + codigo;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioPaginaError::verificarExistenPaginaErrorDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PAGINA_ERROR WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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
    error = "Error al verificar si existen paginas de errores";
    dominio.loguear(error);
    return(resultado);
  }
}
