#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioFTPAnonimo.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicioFTPAnonimo::cServicioFTPAnonimo(cDominio *dom):cServicio(dom) {
  servicioDB = "FTP ANONIMO";
}

bool cServicioFTPAnonimo::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioFTPAnonimo::agregarFileWelcome(const std::string &mens, cDominio &dominio) {
  //Abrir el archivo para escritura
  std::string archivo, error;
  archivo = "/www/docs/" + dominio.getDominio() + "/ftp_anonimo/welcome.msg";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    error = "No se puedo abrir el archivo " + archivo;
    dominio.loguear(error);
    return(-1);
  }

  //Agregar las lineas
  out << endl << mens << endl << endl;

  //Cerrar el archivo
  out.close();

  //Darle permisos al archivo
  std::string comando;
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());

  //Setear el inmutable para el archivo
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioFTPAnonimo::agregarFileProftpdConf(const std::string &puerto, cDominio &dominio) {
  std::string archivo, error;
  //Abrir el archivo para escritura
  archivo = "/etc/proftpd.conf";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    error = "No se pudo abrir el archivo " + archivo;
    dominio.loguear(error);
    return(-1);
  }

  //Agregar las lineas
  out << endl
      << "#################### Virtual FTP: ftp." << dominio.getDominio() << " ####################" << endl
      << "<VirtualHost ftp." << dominio.getDominio() << ">" << endl
      << " Port " << puerto << endl
      << " Umask 022" << endl
      << " DefaultRoot ~" << endl
      << "AllowOverwrite                 on" << endl
      << endl
      << "<Anonymous /www/docs/" << dominio.getDominio() << "/ftp_anonimo>" << endl
      << " RequireValidShell              off" << endl
      << " User                           ftp" << endl
      << " Group                          ftp" << endl
      << " UserAlias                      anonymous ftp" << endl
      << " MaxClients                     20" << endl
      << " DisplayLogin                   welcome.msg" << endl
      << endl
      << " <Limit WRITE>" << endl
      << "     DenyAll" << endl
      << " </Limit>" << endl
      << "</Anonymous>" << endl
      << "</VirtualHost>" << endl
      << "#####################################################################################" << endl;

  //Cerrar el archivo
  out.close();

  return(0);
}

int cServicioFTPAnonimo::agregarFTPA(const std::string &puerto, const std::string &mensaje,
				    cDominio &dominio) {
  std::string comando, directorio;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Crear el directorio para la cuenta ftp anonima
  directorio = "/www/docs/" + dominio.getDominio() + "/ftp_anonimo";
  if (mkdir(directorio.c_str(), 0755))
    if (errno != EEXIST) {
      comando = "Error al crear el directorio del ftp anonimo";
      dominio.loguear(comando);
      return(-1);
    }

  //Crear el archivo welcome
  if (agregarFileWelcome(mensaje, dominio) < 0) {
    comando = "Error al crear el archivo welcome";
    dominio.loguear(comando);
    return(-1);
  }

  //Darles permisos al directorio de la cuenta ftp anonima
  comando = "chown -R pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + directorio;
  system(comando.c_str());

  //Modificar el archivo proftpd.conf
  if (agregarFileProftpdConf(puerto, dominio) < 0) {
    comando = "Error al modificar el archivo proftpd.conf";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar el ftp anonimo
  actualizarFTPA();

  return(0);
}

int cServicioFTPAnonimo::agregarFTPADB(const std::string &puerto, const std::string &mensaje,
				       cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO CUENTA_FTP_ANONIMA(PUERTO, ID_DOMINIO, MENSAJE) VALUES('"
	<< puerto << "', '" << dominio.getIdDominio() << "', '" << mensaje << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la cuenta FTP Anonima";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioFTPAnonimo::cambiarMensajeFTPA(const std::string &mensaje, cDominio &dominio) {
  std::string comando, directorio, archivo, error;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Sacar el inmutable del mensaje de bienvenida
  directorio = "/www/docs/" + dominio.getDominio() + "/ftp_anonimo";
  archivo = directorio + "/welcome.msg";
  comando = "chattr -i " + archivo;
  if ((system(comando.c_str())) < 0) {
    error = "Error al quitar el inmutable del mensaje de bienvendia de la cuenta ftp anonima";
    dominio.loguear(error);
    return(-1);
  }

  //Eliminar el archivo de bienvenida
  comando = "rm -f " + archivo;
  system(comando.c_str());

  //Crear el archivo welcome
  if (agregarFileWelcome(mensaje, dominio) < 0) {
    comando = "Error al crear el archivo welcome";
    dominio.loguear(comando);
    return(-1);
  }

  return(0);
}

int cServicioFTPAnonimo::cambiarMensajeFTPADB(const std::string &mensaje, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE CUENTA_FTP_ANONIMA SET MENSAJE = '" << mensaje << "' WHERE ID_DOMINIO = '"
	<< dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el mensaje de bienvenida de la cuenta FTP Anonima";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioFTPAnonimo::quitarFileProftpdConf(cDominio &dominio) {
  std::string archivo, archivo2, comando;
  archivo = "/etc/proftpd.conf";
  archivo2 = "/etc/proftpd.conf.bak";

  //Copiar el archivo para actualizarlo
  comando = "cp -f " + archivo + " " + archivo2;
  system(comando.c_str());

  //Abrir el archivo para lectura
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "cp -f " + archivo2 + " " + archivo;
    system(comando.c_str());
    comando = "No se pudo abrir el archivo " + archivo;
    dominio.loguear(comando);
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "cp -f " + archivo2 + " " + archivo;
    system(comando.c_str());
    comando = "No se pudo abrir el archivo " + archivo;
    dominio.loguear(comando);
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada, hasta;
  buscada = "#################### Virtual FTP: ftp." + dominio.getDominio() + " #";
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

int cServicioFTPAnonimo::quitarFTPA(cDominio &dominio) {
  std::string comando, directorio, archivo, error;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Sacar el inmutable del mensaje de bienvenida
  directorio = "/www/docs/" + dominio.getDominio() + "/ftp_anonimo";
  archivo = directorio + "/welcome.msg";
  comando = "chattr -i " + archivo;
  if ((system(comando.c_str())) < 0) {
    error = "Error al quitar el inmutable del mensaje de bienvendia de la cuenta ftp anonima";
    dominio.loguear(error);
    return(-1);
  }

  //Eliminar el directorio de la cuenta FTP Anonima
  comando = "rm -fr " + directorio;
  if ((system(comando.c_str())) < 0) {
    error = "Error al eliminar el directorio de la cuenta ftp anonima";
    dominio.loguear(error);
    return(-1);
  }

  //Modificar el archivo proftpd.conf
  if (quitarFileProftpdConf(dominio) < 0) {
    comando = "Error al modificar el archivo proftpd.conf";
    dominio.loguear(comando);
    return(-1);
  }

  //Actualizar el ftp anonimo
  actualizarFTPA();

  return(0);
}

int cServicioFTPAnonimo::quitarFTPADB(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM CUENTA_FTP_ANONIMA WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la cuenta FTP Anonima";
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioFTPAnonimo::recuperarPuertoDB(cDominio &dominio) {
  string spuerto("");
  try {
    Query qry = dominio.con.query();
    qry << "SELECT PUERTO FROM CUENTA_FTP_ANONIMA ORDER BY PUERTO";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    i = res.begin();
    
    long puerto = PUERTO_FTP_ANONIMA;
    long rpuerto;
    while (i != res.end()) {
      row = *i;
      rpuerto = row[0];
      if (puerto != rpuerto)
	break;
      puerto += 2;
      i++;
    }
    char *aux = new char[6];
    snprintf(aux, 6, "%ld", puerto);
    spuerto = aux;
    delete[] aux;
    return(spuerto);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(cantidad);
  } catch(...) {
    string error;
    error = "Error al recuperar un puerto para la cuenta FTP Anonima";
    dominio.loguear(error);
    return(cantidad);
  }
}

std::string cServicioFTPAnonimo::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP_ANONIMA WHERE ID_DOMINIO = '"
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

int cServicioFTPAnonimo::verificarExisteFTPADB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM CUENTA_FTP_ANONIMA WHERE ID_DOMINIO = '" 
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
    error = "Error al verificar si existe la cuenta FTP Anonima";
    dominio.loguear(error);
    return(resultado);
  }
}
