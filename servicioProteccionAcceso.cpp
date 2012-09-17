#include <string>
#include <mysql++.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioProteccionAcceso.h"
#include <errno.h>

using namespace std;
using namespace mysqlpp;

cServicioProteccionAcceso::cServicioProteccionAcceso(cDominio *dom):cServicio(dom) {
  servicioDB = "PROTECCION DIRECTORIO ACCESO";
}

bool cServicioProteccionAcceso::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioProteccionAcceso::agregarDirectorioComun(cDominio &dominio) {
  std::string directorio, comando;

  //verificar si existe el directorio en comun, si no crearlo
  directorio = "/www/docs/" + dominio.getDominio() + "/.prot";
  if (existeDirectorio(directorio))
    return(0);

  //Crear el directorio
  if (mkdir(directorio.c_str(), 0555))
    if (errno != EEXIST) {
      comando = "Error al crear el directorio " + directorio;
      dominio.loguear(comando);
      return(-1);
    }
  comando = "chown nobody:nobody " + directorio;
  system(comando.c_str());

  return(0);
}

int cServicioProteccionAcceso::agregarFileHtaccess(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir + "/.htaccess";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Agregar el archivo
  out << "AuthType Basic" << endl
      << "AuthName \"www." << dominio.getDominio() << "\"" << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/.prot/." + dir + "passlist" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/.prot/." + dir + "grouplist" << endl
      << "require group admin" << endl;

  //Cerrar el archivo
  out.close();

  //Darle al archivo permisos nobody:nobody, 644, +i
  comando = "chown nobody:nobody " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioProteccionAcceso::agregarFilePasswords(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "passlist";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Agregar el archivo
  out << endl;

  //Cerrar el archivo
  out.close();

  //Darle al archivo permisos nobody:nobody, 644, +i
  comando = "chown nobody:nobody " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioProteccionAcceso::agregarFileUsuarios(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "grouplist";
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Agregar el archivo
  out << "admin:" << endl;

  //Cerrar el archivo
  out.close();

  //Darle al archivo permisos nobody:nobody, 644, +i
  comando = "chown nobody:nobody " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioProteccionAcceso::agregarProteccionAcceso(const std::string &dir, cDominio &dominio,
						       const std::string &optBorrarDirectorio) {
  std::string directorio, comando, error;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir;

  //Si esta seteada optBorrarDirectorio eliminarlo
  if (optBorrarDirectorio.length() > 0) {
    comando = "rm -fr " + directorio;
    system(comando.c_str());
  }

  //Crear el directorio que se va a proteger contra accesos
  if (mkdir(directorio.c_str(), 0755)) {
    if (errno != EEXIST) {
      comando = "Error al crear el directorio " + directorio;
      dominio.loguear(comando);
      return(-1);
    }
  } else {
    comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + directorio;
    system(comando.c_str());
  }

  //Si no existe el directorio comun para todos las protecciones de accesos, crearlo
  if ((agregarDirectorioComun(dominio)) < 0) {
    error = "Error al agregar el directorio comun para todos los accesos";
    dominio.loguear(error);
    return(-1);
  }

  //Agregar el archivo de usuarios para el directorio que se esta protegiendo
  if ((agregarFileUsuarios(dir, dominio)) < 0) {
    error = "Error al agregar el archivo de usuarios para la proteccion " + dir;
    dominio.loguear(error);
    return(-1);
  }

  //Agregar el archivo de password para el directorio que se esta protegiendo
  if ((agregarFilePasswords(dir, dominio)) < 0) {
    error = "Error al agregar el archivo de passwords para la proteccion " + dir;
    dominio.loguear(error);
    return(-1);
  }

  //Agregar el archivo htaccess
  if ((agregarFileHtaccess(dir, dominio)) < 0) {
    error = "Error al agregar el archivo htaccess para la proteccion " + dir;
    dominio.loguear(error);
    return(-1);
  }

  return(0);
}

int cServicioProteccionAcceso::agregarProteccionAccesoDB(const std::string &directorio,
							 cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO PROTECCION_DIR_ACCESO(ID_DOMINIO, DIRECTORIO) VALUES('"
	<< dominio.getIdDominio() << "', '" << directorio << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar la proteccion de acceso para el directorio " + directorio;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioProteccionAcceso::quitarDirectorioComun(cDominio &dominio) {
  std::string directorio, comando;

  //Eliminar el directorio
  directorio = "/www/docs/" + dominio.getDominio() + "/.prot";
  comando = "rm -fr " + directorio;
  system(comando.c_str());

  return(0);
}

int cServicioProteccionAcceso::quitarFileHtaccess(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Sacarle al archivo los permisos de inmutable
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir + "/.htaccess";
  comando = "chattr -i " + archivo;
  system(comando.c_str());

  //Eliminar el archivo
  comando = "rm -f " + archivo;
  system(comando.c_str());

  //Devolver el OK
  return(0);
}

int cServicioProteccionAcceso::quitarFilePasswords(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Sacarle al archivo los permisos de inmutable
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "passlist";
  comando = "chattr -i " + archivo;
  system(comando.c_str());

  //Eliminar el archivo
  comando = "rm -f " + archivo;
  system(comando.c_str());

  //Devolver el OK
  return(0);
}

int cServicioProteccionAcceso::quitarFileUsuarios(const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Sacarle al archivo los permisos de inmutable
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "grouplist";
  comando = "chattr -i " + archivo;
  system(comando.c_str());

  //Eliminar el archivo
  comando = "rm -f " + archivo;
  system(comando.c_str());

  //Devolver el OK
  return(0);
}

int cServicioProteccionAcceso::quitarProteccionAcceso(const std::string &dir, cDominio &dominio,
						      const std::string &optBorrarDirectorio) {
  std::string directorio, comando, error;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Quitar el archivo htaccess
  if ((quitarFileHtaccess(dir, dominio)) < 0) {
    error = "Error al quitar el archivo htaccess para la proteccion " + dir;
    dominio.loguear(error);
    return(-1);
  }

  //Si esta seteada optBorrarDirectorio eliminarlo
  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/" + dir;
  if (optBorrarDirectorio.length() > 0) {
    comando = "rm -fr " + directorio;
    system(comando.c_str());
  }

  //Quitar el archivo de usuarios para el directorio
  if ((quitarFileUsuarios(dir, dominio)) < 0) {
    error = "Error al quitar el archivo de usuarios para la proteccion " + dir;
    dominio.loguear(error);
  }

  //Quitar el archivo de password para el directorio
  if ((quitarFilePasswords(dir, dominio)) < 0) {
    error = "Error al agregar el archivo de passwords para la proteccion " + dir;
    dominio.loguear(error);
  }

  //Verificar si existe mas protecciones de listado para el dominio, si no existen
  //eliminar el directorio comun
  if ((verificarExistenProteccionAccesoDB(dominio)) <= 1)
    quitarDirectorioComun(dominio);

  return(0);
}

int cServicioProteccionAcceso::quitarProteccionAccesoDB(const std::string &directorio, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM PROTECCION_DIR_ACCESO WHERE DIRECTORIO = '" << directorio
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar la proteccion de acceso para el directorio " + directorio;
    dominio.loguear(error);
    return(-2);
 }
}

int cServicioProteccionAcceso::quitarUsuariosAccesoDB(const std::string &idProteccion,
						      cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM PROTECCION_DIR_ACCESO_USR WHERE ID_PROTECCION_DIR_ACCESO = '"
	<< idProteccion << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar todos los usuarios para la proteccion " + idProteccion;
    return(-2);
 }
}

std::string cServicioProteccionAcceso::recuperarIdProteccionAccesoDB(const std::string &directorio, cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT ID_PROTECCION_DIR_ACCESO FROM PROTECCION_DIR_ACCESO WHERE DIRECTORIO = '"
	<< directorio << "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'";
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
    error = "Error al recuperar la identidad de proteccion de acceso para el directorio "
      + directorio;
    dominio.loguear(error);
    return(resultado);
  }
}


std::string cServicioProteccionAcceso::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO WHERE ID_DOMINIO = '" 
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

int cServicioProteccionAcceso::verificarExisteProteccionAccesoDB(const std::string &directorio,
								 cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO WHERE DIRECTORIO = '" << directorio 
	<< "' AND ID_DOMINIO = '" << dominio.getIdDominio() << "'" << endl;
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
    error = "Error al verificar si existe la proteccion de acceso para el directorio "
      + directorio;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioProteccionAcceso::verificarExistenProteccionAccesoDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'" << endl;
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
    error = "Error al verificar si existen protecciones de acceso";
    dominio.loguear(error);
    return(resultado);
  }
}
