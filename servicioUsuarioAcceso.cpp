#include <string>
#include <mysql++.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioUsuarioAcceso.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicioUsuarioAcceso::cServicioUsuarioAcceso(cDominio *dom):cServicio(dom) {
  servicioDB = "USUARIO ACCESO";
}

bool cServicioUsuarioAcceso::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioUsuarioAcceso::agregarFilePassword(const std::string &usuario,
						const std::string &password,
						const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;
  
  //Archivo
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "passlist";

  //Quitarle los permisos anteriores
  comando = "chattr -i " + archivo;
  system(comando.c_str());

  //Actualizar el archivo
  comando = "/usr/local/apache/bin/htpasswd -b ";
  comando += archivo;
  comando += " ";
  comando += usuario;
  comando += " ";
  comando += password;
  comando += " > /dev/null";
  system(comando.c_str());

  //Darle al archivo permisos nobody:nobody, 644, +i
  comando = "chown nobody:nobody " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioUsuarioAcceso::agregarFileUsuarios(const std::string &usuarios,
						const std::string &dir, cDominio &dominio) {
  std::string archivo, comando;

  //Archivo
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "grouplist";

  //Eliminar la copa anterior
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm -f " + archivo;
  system(comando.c_str());

  //Abrir el archivo para escritura
  ofstream out(archivo.c_str(), ios::app);
  if (!out.is_open()) {
    dominio.loguear("No se pudo abrir el archivo " + archivo);
    return(-1);
  }

  //Agregar el archivo
  out << "admin:" << usuarios << endl;

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

int cServicioUsuarioAcceso::agregarUsuarioAcceso(const std::string &usuario,
						 const std::string &password,
						 const std::string &directorio,
						 const std::string &idDirectorio,
						 cDominio &dominio) {
  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Recuperar todos los usuarios para el directorio sobre el que se esta trabajando
  std::string usuarios;
  if ((recuperarUsuariosAccesoDB(idDirectorio, dominio, usuarios)) < 0)
    return(-1);
  usuarios += " ";
  usuarios += usuario;

  //Modificar el archivo con todos los usuarios para la proteccion de listado
  agregarFileUsuarios(usuarios, directorio, dominio);

  //Modificar el archivo con todos los passwords para la proteccion de listado
  agregarFilePassword(usuario, password, directorio, dominio);

  //Devolver el OK
  return(0);
}

int cServicioUsuarioAcceso::agregarUsuarioAccesoDB(const std::string &usuario,
						   const std::string &password,
						   const std::string &idDirectorio,
						   cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO PROTECCION_DIR_ACCESO_USR(ID_PROTECCION_DIR_ACCESO, USUARIO, PASSWORD,"
	<< " ESTADO) VALUES('" << idDirectorio << "', '" << usuario << "', '" << password
	<< "', '1')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar el usuario de proteccion de acceso para el idDirectorio"
      + idDirectorio;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioUsuarioAcceso::cambiarClaveUsuarioAcceso(const std::string &usuario,
						      const std::string &password,
						      const std::string &directorio,
						      const std::string &idProteccion,
						      cDominio &dominio) {
  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Cambiar el password en el sistema
  agregarFilePassword(usuario, password, directorio, dominio);

  //Devolver el OK
  return(0);
}

int cServicioUsuarioAcceso::cambiarClaveUsuarioAccesoDB(const std::string &usuario,
							const std::string &password,
							const std::string &idProteccion,
							cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE PROTECCION_DIR_ACCESO_USR SET PASSWORD = '" << password
	<< "' WHERE USUARIO = '" << usuario << "' AND ID_PROTECCION_DIR_ACCESO = '"
	<< idProteccion << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password del usuario " + usuario
      + " de la proteccion de acceso para el directorio " + idProteccion;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioUsuarioAcceso::deshabilitarUsuarioAccesoDB(const std::string &usuario,
							const std::string &idProteccion,
							cDominio &dominio) {
  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Devolver el OK
  return(0);
}

int cServicioUsuarioAcceso::deshabilitarUsuarioAcceso(const std::string &usuario,
						      const std::string &idProteccion,
						      cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE PROTECCION_DIR_ACCESO_USR SET ESTADO = '0' WHERE USUARIO = '" << usuario
	<< "' AND ID_PROTECCION_DIR_ACCESO = '" << idProteccion << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al deshabilitar el usuario " + usuario
      + " de proteccion de acceso para el directorio " + idProteccion;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioUsuarioAcceso::habilitarUsuarioAcceso(const std::string &usuario,
						   const std::string &idProteccion,
						   cDominio &dominio) {
  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Devolver el OK
  return(0);
}

int cServicioUsuarioAcceso::habilitarUsuarioAccesoDB(const std::string &usuario,
						     const std::string &idProteccion,
						     cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE PROTECCION_DIR_ACCESO_USR SET ESTADO = '1' WHERE USUARIO = '" << usuario
	<< "' AND ID_PROTECCION_DIR_ACCESO = '" << idProteccion << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al habilitar el usuario " + usuario
      + " de proteccion de acceso para el directorio " + idProteccion;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioUsuarioAcceso::recuperarPasswordDB(const std::string &usuario,
							const std::string &idProteccion,
							cDominio &dominio) {
  string resultado = "";
  try {
    Query qry = dominio.con.query();
    qry << "SELECT PASSWORD FROM PROTECCION_DIR_ACCESO_USR WHERE USUARIO = '" << usuario
	<< "' AND ID_PROTECCION_DIR_ACCESO = '" << idProteccion << "'";
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
    error = "Error al recuperar el password del usuario " + usuario
      + " de la proteccion de listado " + idProteccion;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioUsuarioAcceso::recuperarUsuariosAccesoDB(const std::string &idProteccion,
						      cDominio &dominio,
						      std::string &usuarios,
						      std::string usuario,
						      std::string tipo) {
  try {
    Query qry = dominio.con.query();
    if (tipo == "1")
      qry << "SELECT USUARIO FROM PROTECCION_DIR_ACCESO_USR WHERE ID_PROTECCION_DIR_ACCESO = '"
	  << idProteccion << "' AND USUARIO <> '" << usuario << "'";
    else
      qry << "SELECT USUARIO FROM PROTECCION_DIR_ACCESO_USR WHERE ID_PROTECCION_DIR_ACCESO = '"
	  << idProteccion << "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    for (i = res.begin(); i != res.end(); i++) {
      row = *i;
      usuarios += " ";
      usuarios += std::string(row[0]);
    }
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al recuperar los usuarios para el directorio " + idProteccion;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioUsuarioAcceso::quitarFilePassword(const std::string &usuario,
					       const std::string &dir,
					       cDominio &dominio) {
  std::string archivo, archivo2, comando, error;
  
  //Archivos
  archivo = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "passlist";
  archivo2 = "/www/docs/" + dominio.getDominio() + "/.prot/." + dir + "passlist.bak";

  //Quitarle los permisos anteriores
  comando = "chattr -i " + archivo;
  system(comando.c_str());

  //Actualizar el archivo
  //Copiar el archivo para actualizarlo
  comando = "cp -f " + archivo + " " + archivo2;
  system(comando.c_str());

  //Abrir el archivo para lectura
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    error = "No se pudo abrir el archvivo " + archivo2;
    dominio.loguear(error);
    return(-1);
  }

  //Abrir el archivo para escritura
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    error = "No se pudo abrir el archvivo " + archivo;
    dominio.loguear(error);
    return(-1);
  }

  //Filtrar el archivo
  char buffer[MINLINE];
  std::string linea;
  std::string buscada;
  buscada = usuario;
  buscada += ":";
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    if (linea.find(usuario) == string::npos)
      out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  //Darle al archivo permisos nobody:nobody, 644, +i
  comando = "chown nobody:nobody " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  //Borrar el archivo de backup
  comando = "rm -f " + archivo2;
  system(comando.c_str());

  return(0);
}

int cServicioUsuarioAcceso::quitarUsuarioAcceso(const std::string &usuario,
						const std::string &directorio,
						const std::string &idDirectorio,
						cDominio &dominio) {
  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Recuperar todos los usuarios para el directorio sobre el que se esta trabajando menos
  //el usuario que se esta eliminando
  std::string usuarios;
  if ((recuperarUsuariosAccesoDB(idDirectorio, dominio, usuarios, usuario, "1")) < 0)
    return(-1);

  //Modificar el archivo con todos los usuarios para la proteccion de listado
  agregarFileUsuarios(usuarios, directorio, dominio);

  //Modificar el archivo con todos los passwords para la proteccion de listado
  quitarFilePassword(usuario, directorio, dominio);

  //Devolver el OK
  return(0);
}

int cServicioUsuarioAcceso::quitarUsuarioAccesoDB(const std::string &usuario,
						  const std::string &idDirectorio,
						  cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM PROTECCION_DIR_ACCESO_USR WHERE ID_PROTECCION_DIR_ACCESO = '"
	<< idDirectorio << "' AND USUARIO = '" << usuario << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar el usuario de proteccion de listado " + usuario
      + " para el idDirectorio " + idDirectorio;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioUsuarioAcceso::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO_USR PU, PROTECCION_DIR_ACCESO P "
	<< "WHERE P.ID_PROTECCION_DIR_ACCESO = PU.ID_PROTECCION_DIR_ACCESO AND "
	<< "P.ID_DOMINIO = '" << dominio->getIdDominio() << "'";
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

int cServicioUsuarioAcceso::verificarDeshabilitadoUsuarioDB(const std::string &usuario,
							    const std::string &idProteccion,
							    cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO_USR WHERE USUARIO = '" << usuario
	<< "' AND ID_PROTECCION_DIR_ACCESO = '" << idProteccion << "' AND ESTADO = '0'";
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
    error = "Error al verificar si el usuario " + usuario
      + " de la proteccion de acceso del directorio " + idProteccion
      + " se encuetnra deshabilitado";
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioUsuarioAcceso::verificarExisteUsuarioAccesoDB(const std::string &usuario,
							   const std::string &idProteccion,
							   cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO_USR WHERE USUARIO = '" << usuario
	<< "' AND ID_PROTECCION_DIR_ACCESO = '" << idProteccion << "'";
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
    error = "Error al verificar si existe la proteccion de listado para el usuario "
      + usuario + " del directorio " + idProteccion;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioUsuarioAcceso::verificarExistenUsuarioAccesoDB(const std::string &idDirectorio,
						      cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO_USR WHERE ID_PROTECCION_DIR_ACCESO = '"
	<< idDirectorio << "'";
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
    error = "Error al verificar si existen usuarios para el directorio de proteccion de listado "
      + idDirectorio;
    dominio.loguear(error);
    return(resultado);
  }
}

int cServicioUsuarioAcceso::verificarHabilitadoUsuarioDB(const std::string &usuario,
							 const std::string &idProteccion,
							 cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM PROTECCION_DIR_ACCESO_USR WHERE USUARIO = '" << usuario
	<< "' AND ID_PROTECCION_DIR_ACCESO = '" << idProteccion << "' AND ESTADO = '1'";
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
    error = "Error al verificar si el usuario " + usuario
      + " de la proteccion de listado del directorio " + idProteccion
      + " se encuentra habilitado";
    dominio.loguear(error);
    return(resultado);
  }
}

