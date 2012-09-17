#include <mysql++.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicio::cServicio(cDominio *dom):dominio(dom) {
}

bool cServicio::iniciar() {
  //Recuperar la identidad del servicio
  try {
    Query qry = dominio->con.query();
    if (dominio->getTipo() == "Empresa")
      qry << "SELECT ID_SERV FROM DOM_EMP WHERE ID_DOMINIO = '" << dominio->getIdDominio() << "'";
    if (dominio->getTipo() == "RevendedorPaquete")
      qry << "SELECT ID_SERV FROM DOM_REV_PAQ WHERE ID_DOMINIO = '" << dominio->getIdDominio()
	  << "'";
    if (dominio->getTipo() == "RevendedorFree")
      qry << "SELECT ID_SERV_DOM_REV FROM DOM_REV_FREE WHERE ID_DOMINIO = '"
	  << dominio->getIdDominio() << "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      idServicio = std::string(row[0]);
    }
  }
  catch (BadQuery er) {
    dominio->loguear(er.error);
  }
  catch (...) {
    string error;
    error = "Error al recuperar el servicio";
    dominio->loguear(error);
  }

  //Recuperar el nombre del servicio
  try {
    Query qry = dominio->con.query();
    if (dominio->getTipo() == "Empresa")
      qry << "SELECT SERV FROM SERV WHERE ID_SERV = '" << idServicio << "'";
    if (dominio->getTipo() == "RevendedorPaquete")
      qry << "SELECT SERV_PAQ FROM SERV_PAQ WHERE ID_SERV = '" << idServicio << "'";
    if (dominio->getTipo() == "RevendedorFree")
      qry << "SELECT SERV_DOM_REV WHERE ID_SERV_REV = '" << idServicio << "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      servicio = std::string(row[0]);
    }
  }
  catch (BadQuery er) {
    dominio->loguear(er.error);
  }
  catch (...) {
    string error;
    error = "Error al recuperar el servicio";
    dominio->loguear(error);
  }

  //Verificar que todo se haya inicializado
  if ((idServicio.length() > 0) && (servicio.length() > 0) && (cantidad.length() > 0) &&
      (limite.length() > 0))
    return(true);
  else
    return(false);
}

int cServicio::agregarHistorial(const std::string &mensaje, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO HISTORIAL_DOMINIO(FECHA_HORA, ACCION, ID_DOMINIO) VALUES(NOW(), '"
	<< mensaje << "', '" << dominio.getIdDominio() << "')";
    qry.execute();
    return(0);
  }
  catch (BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  }
  catch (...) {
    string error;
    error = "Error al ingresar al historial del dominio el mensaje " + mensaje;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicio::traerLimite() {
  try {
    Query qry = dominio->con.query();
    if ((dominio->getTipo() == "Empresa") || (dominio->getTipo() == "RevendedorPaquete"))
      qry << "SELECT R_SERV_SERV_DOM.VALOR FROM SERV_DOM, R_SERV_SERV_DOM WHERE "
	  << "R_SERV_SERV_DOM.ID_SERV = '" << idServicio << "' AND "
	  << "R_SERV_SERV_DOM.ID_SERV_DOM = SERV_DOM.ID_SERV_DOM AND "
	  << "SERV_DOM.SERV_DOM = '" << servicioDB << "'";
    if (dominio->getTipo() == "RevendedorFree")
      qry << "SELECT R_SERV_DOM_REV_SERV_DOM.VALOR FROM R_SERV_DOM_REV_SERV_DOM, SERV_DOM WHERE "
	  << "R_SERV_DOM_REV_SERV_DOM.ID_SERV_DOM_REV = '" << idServicio << "' AND "
	  << "SERV_DOM.SERV_DOM = '" << servicioDB << "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    if (res.size() > 0) {
      i = res.begin();
      row = *i;
      limite = std::string(row[0]);
    }
    return(limite);
  }
  catch (BadQuery er) {
    dominio->loguear(er.error);
    return(limite);
  }
  catch (...) {
    string error;
    error = "Error al recuperar el limite del servicio";
    dominio->loguear(error);
    return(limite);
  }
}

std::string cServicio::traerCantidad() {
  return(cantidad);
}

int cServicio::verificarCrearServicio() {
  long lLimite = 0, lCantidad = 0;
  /*
  char *sLimite = new char[MINLINE];
  char *sCantidad = new char[MINLINE];
  snprintf(sLimite, MINLINE, "%s", limite.c_str());
  snprintf(sCantidad, MINLINE, "%s", cantidad.c_str());
  sscanf(sLimite, "%ld", &lLimite);
  sscanf(sCantidad, "%ld", &lCantidad);
  delete[] sLimite;
  delete[] sCantidad;
  */

  sscanf(limite.c_str(), "%ld", &lLimite);
  sscanf(cantidad.c_str(), "%ld", &lCantidad);

  if (lLimite == 0)
    return(0);
  if (lLimite == -1)
    return(-1);

  if (lLimite <= lCantidad)
    return(-2);
  return(2);
}

std::string cServicio::getIdServicio() {
  return(idServicio);
}

std::string cServicio::getServicio() {
  return(servicio);
}

std::string cServicio::getServicioDB() {
  return(servicioDB);
}

std::string cServicio::getCantidad() {
  return(cantidad);
}

std::string cServicio::getLimite() {
  return(limite);
}

int cServicio::actualizarDNS() {
  if (system("/usr/local/sbin/rndc reload 1>/dev/null 2>/dev/null") < 0)
    return(-1);
  return(0);
}

int cServicio::actualizarFTPA() {
  if (system("killall -HUP proftpd") < 0)
    return(-1);
  return(0);
}

int cServicio::actualizarFileAccess() {
  return(0);
  if (system("makemap hash /etc/mail/access < /etc/mail/access") < 0)
    return(-1);
  return(0);
}

int cServicio::actualizarFileVirtusertable() {
  if (system("makemap btree /etc/mail/virtusertable < /etc/mail/virtusertable") < 0)
    return(-1);
  return(0);
}

int cServicio::actualizarFileAliases() {
  if (system("/usr/bin/newaliases") < 0)
    return(-1);
  return(0);
}

int cServicio::actualizarPrivilegiosMySQL(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "FLUSH PRIVILEGES" << endl;
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al flushear los privilegios de MySQL";
    dominio.loguear(error);
    return(-2);
  }
}

long cServicio::obtenerTamanio(std::string archivo, std::string directorio) {
  /* Funcion que devuelve el tamanio de un archivo */
  mode_t mode;
  int fd;
  std::string strfile;
  struct stat buf;
  long ret;
 
  /* Seleccionar el archivo a abrir */
  if (directorio.length() <= 0)
    strfile = archivo;
  else {
    strfile = directorio;
    strfile += "/";
    strfile += archivo;
  }

  /* Abrir el archivo */
  if (( fd = open(strfile.c_str(), O_RDONLY)) < 0)
    return(-1);

  /* Obtener el status */
  if ((fstat(fd, &buf)) < 0)
    return(-1);
  mode = buf.st_mode;

  /* Obtener el tamanio */
  ret = buf.st_size;

  /* Cerrar el archivo */
  close(fd);

  return(ret);
}


std::string cServicio::caracterBackup(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar si existen esas compresiones de backup
  string cad = lowerCase(cadena);
  bool valido = false;
  if (cad == "gzip")
    valido = true;
  if (cad == "zip")
    valido = true;
  if (cad == "rar")
    valido = true;
  if (cad == "bzip2")
    valido = true;
  if (!valido) {
    error = "El campo " + campo + " especifica una compresión no conocida por el sistema";
    return(error);
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterDirectorio(const std::string &cadena, const std::string campo) {
  string error = "";

  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_')
	  )) {
      error += "El campo " + campo + 
	" solo debe contener letras, números y el caracter _";
      return(error);
    }
    it++;
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterDirectorioL(const std::string &cadena, const std::string campo) {
  string error = "";

  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '/') || (*it == '_')
	  )) {
      error += "El campo " + campo + 
	" solo debe contener letras, números, el caracter / y el caracter _";
      return(error);
    }
    it++;
  }

  //Verificar que no haya dos '/' seguidas
  it = cadena.begin();
  while (it != cadena.end()) {
    if (*it == '/') {
      it++;
      if (*it == '/') {
	error += "En el campo " + campo + " no puede haber un caracter / seguido de otro"
	  " caracter /";
	return(error);
      }
      it--;
    }
    it++;
  }	

  //Devolver el OK
  return("");
}

std::string cServicio::caracterDominio(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra o un numero
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z')) ||
	((cadena[0] >= '0') && (cadena[0] <= '9')) 
	)) {
    error += "El campo " + campo + " debe comenzar con una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_') || (*it == '-') || (*it == '.')
	  )) {
      error += "El campo " + campo + " solo debe contener letras, números, el caracter -," +
	" el caracter _ y el caracter .";
      return(error);
    }
    it++;
  }

  //Verificar que solamente haya dos puntos, y un caracter despues de cada punto
  int puntos = 0;
  it = cadena.begin();
  while (it != cadena.end()) {
    if (*it == '.') {
      puntos++;
      it++;
      if (*it == '.') {
	error += "En el campo " + campo + " el caracter . no puede estar seguido de otro" + 
	  " caracter .";
	return(error);
      }
      it--;
    }
    it++;
  }
  if (puntos > 3) {
    error += "En el campo " + campo + " no puede haber mas de tres caracteres .";
    return(error);
  }
  if (puntos == 0) {
    error += "Al menos debe existir un caracter . en el campo " + campo;
    return(error);
  }

  //Verificar la terminacion del dominio
  bool terminacion = false;
  int pos1, pos2;
  pos1 = cadena.find(".");
  pos2 = cadena.rfind(".");
  string end1, end2;
  if (puntos == 1)
    end1 = cadena.substr(pos1 + 1);
  if (puntos == 2)
    end1 = cadena.substr(pos1 + 1, pos2 - pos1 - 1);
  end2 = cadena.substr(pos2 + 1);

  if (end1 == "aero")
    terminacion = true;
  if (end1 == "biz")
    terminacion = true;
  if (end1 == "bz")
    terminacion = true;
  if (end1 == "com")
    terminacion = true;
  if (end1 == "cop")
    terminacion = true;
  if (end1 == "cc")
    terminacion = true;
  if (end1 == "edu")
    terminacion = true;
  if (end1 == "info")
    terminacion = true;
  if (end1 == "gov")
    terminacion = true;
  if (end1 == "mil")
    terminacion = true;
  if (end1 == "museum")
    terminacion = true;
  if (end1 == "name")
    terminacion = true;
  if (end1 == "net")
    terminacion = true;
  if (end1 == "org")
    terminacion = true;
  if (end1 == "pro")
    terminacion = true;
  if (end1 == "tv")
    terminacion = true;
  if (end1 == "ws")
    terminacion = true;
  if (end1 == "py")
    terminacion = true;
  if (end1 == "cl")
    terminacion = true;
  if (end1 == "sc")
    terminacion = true;    
  if (end1 == "vc")
    terminacion = true;        
  if (end1 == "mn")
    terminacion = true;            
  if (end1 == "hn")
    terminacion = true;
  if (end1 == "bz")
    terminacion = true;
  if (end1 == "ag")
    terminacion = true;    
  if (end1 == "mobi")
    terminacion = true;    
  if (end1 == "cn")
    terminacion = true;
  if (end1 == "us")
    terminacion = true;       
  if (end1 == "asia")
    terminacion = true;                   
  if (end1 == "de")
    terminacion = true;                        

  //Si hay dos puntos
  if (puntos == 2)
    if (end2.length() != 2) {
      terminacion = false;
      error = "Solamente se permiten dos caracteres después del segundo punto. Estos, deben referenciar a un país.";
      return(error);
    }

  if (!terminacion) {
    error = "El dominio del campo " + campo + " es invalido";
    return(error);
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterEmail(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra o numero
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z')) ||
	((cadena[0] >= '0') && (cadena[0] <= '9'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra o numero";
    return(error);
  }
  
  //Obtener lo que esta antes del arroba y despues del arroba
  int pos, aux;
  pos = cadena.find("@");
  if (pos <= 0) {
    error += "El campo " + campo + " debe contener un caracter @";
    return(error);
  }
  if (pos == 0) {
    error += "Debe existir al menos un caracter delante del caracter @";
    return(error);
  }
  aux = cadena.length() - 1;
  if (pos == aux) {
    error += "Debe existir al menos un caracter detras del caracter @";
    return(error);
  }
  string delante(cadena, 0, pos);
  string detras(cadena, pos + 1);
  
  //Verificar los caracteres validos para la parte de delante
  //Verificar los caracteres validos del campo
  string::const_iterator it = delante.begin();
  while (it != delante.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_') || (*it == '.')
	  )) {
      error += "El campo " + campo + ", delante del caracter @ solo debe haber letras,"
	" números, el caracter _ y el caracter .";
      return(error);
    }
    it++;
  }

  //Verificar cuantos puntos hay
  int puntos = 0;
  it = delante.begin();
  while (it != delante.end()) {
    if (*it == '.')
      puntos++;
    it++;
  }
  if (puntos > 1) {
    error += "Solo puede haber un caracter . en el campo " + campo + " delante del caracter @";
    return(error);
  }
  if (puntos > 0) {
    if (delante[0] == '.') {
      error += "No puede haber un punto al comienzo del campo " + campo;
      return(error);
    }
    if (delante[delante.length() - 1] == '.') {
      error += "No puede haber un punto delante del caracter @ en el campo " + campo;
      return(error);
    }
  }

  //Verificar los caracteres validos para la parte de detras (dominio)
  //Verificar que el primer caracter sea una letra
  if (!(
	((detras[0] >= 'a') && (detras[0] <= 'z')) ||
	((detras[0] >= 'A') && (detras[0] <= 'Z'))
        )) {
    error += "En el campo " + campo + ", despues del caracter @ debe haber una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  it = detras.begin();
  while (it != detras.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '-') || (*it == '.')
	  )) {
      error += "En el campo " + campo + ", despues del caracter @ solo debe contener letras,"
	" números, el caracter - y el caracter .";
      return(error);
    }
    it++;
  }

  //Verificar que solamente haya dos puntos, y un caracter despues de cada punto
  puntos = 0;
  it = detras.begin();
  while (it != detras.end()) {
    if (*it == '.') {
      puntos++;
      it++;
      if (*it == '.') {
	error += "En el campo " + campo + " el caracter . no puede estar seguido de otro" + 
	  " caracter .";
	return(error);
      }
      it--;
    }
    it++;
  }
  if (puntos > 3) {
    error += "En el campo " + campo + " no puede haber mas de tres caracteres .";
    return(error);
  }
  if (puntos == 0) {
    error += "Al menos debe existir un caracter . en el dominio del campo " + campo;
    return(error);
  }

  //Verificar la terminacion del dominio
  bool terminacion = false;
  int pos1, pos2;
  pos1 = detras.find(".");
  pos2 = detras.rfind(".");
  string end1, end2;
  if (puntos == 1)
    end1 = detras.substr(pos1 + 1);
  if (puntos == 2)
    end1 = detras.substr(pos1 + 1, pos2 - pos1 - 1);
  end2 = detras.substr(pos2 + 1);

  if (end1 == "aero")
    terminacion = true;
  if (end1 == "biz")
    terminacion = true;
  if (end1 == "bz")
    terminacion = true;
  if (end1 == "com")
    terminacion = true;
  if (end1 == "cop")
    terminacion = true;
  if (end1 == "cc")
    terminacion = true;
  if (end1 == "edu")
    terminacion = true;
  if (end1 == "info")
    terminacion = true;
  if (end1 == "gov")
    terminacion = true;
  if (end1 == "mil")
    terminacion = true;
  if (end1 == "museum")
    terminacion = true;
  if (end1 == "name")
    terminacion = true;
  if (end1 == "net")
    terminacion = true;
  if (end1 == "org")
    terminacion = true;
  if (end1 == "pro")
    terminacion = true;
  if (end1 == "tv")
    terminacion = true;
  if (end1 == "ws")
    terminacion = true;
  if (end1 == "py")
    terminacion = true;
  if (end1 == "cl")
    terminacion = true;
  if (end1 == "sc")
    terminacion = true;    
  if (end1 == "vc")
    terminacion = true;        
  if (end1 == "mn")
    terminacion = true;            
  if (end1 == "hn")
    terminacion = true;
  if (end1 == "bz")
    terminacion = true;
  if (end1 == "ag")
    terminacion = true;    
  if (end1 == "mobi")
    terminacion = true;    
  if (end1 == "cn")
    terminacion = true;   
  if (end1 == "us")
    terminacion = true;       
  if (end1 == "asia")
    terminacion = true;                   
  if (end1 == "de")
    terminacion = true;                           

  //Si hay dos puntos
  if (puntos == 3)
    if (end2.length() != 3) {
      terminacion = false;
      error = "El dominio del campo " + campo + ", el dominio debe contener solamente tres caracteres luego del segundo punto. Estos caracteres deben representar un país";
      return(error);
    }

  if (!terminacion) {
    error = "El dominio del campo " + campo + " es invalido";
    return(error);
  }


  //Devolver el OK
  return("");
}

std::string cServicio::caracterLetraNumero(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_')
	  )) {
      error += "El campo " + campo + " solo debe contener letras, números y el caracter _";
      return(error);
    }
    it++;
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterNumeroLetra(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z')) ||
	((cadena[0] >= '0') && (cadena[0] <= '9'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra o número.";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_')
	  )) {
      error += "El campo " + campo + " solo debe contener letras, números y el caracter _";
      return(error);
    }
    it++;
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterLetraNumeroEspacio(const std::string &cadena,
						  const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_') || (*it == ' ')
	  )) {
      error += "El campo " + campo + " solo debe contener letras, números, el caracter _ " +
	"y espacios en blanco";
      return(error);
    }
    it++;
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterNumero(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!((*it >= '0') && (*it <= '9'))) {
      error += "El campo " + campo + " solo debe contener números";
      return(error);
    }
    it++;
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterPassword(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_')
	  )) {
      error += "El campo " + campo + " solo debe contener letras, números y el caracter _";
      return(error);
    }
    it++;
  }

  //Verificar el tamaño minimo del campo
  if (cadena.length() < 6) {
    error += "El campo " + campo + " debe contener al menos 6 caracteres";
    return(error);
  }

  //Devolver el OK
  return("");
}

std::string cServicio::caracterUsuarioEmail(const std::string &cadena, const std::string campo) {
  string error;
  //Verificar si la cadena esta vacia
  if (cadena.length() <= 0)
    return(string("El campo " + campo + " esta vacío"));

  //Verificar que el primer caracter sea una letra
  if (!(
	((cadena[0] >= 'a') && (cadena[0] <= 'z')) ||
	((cadena[0] >= 'A') && (cadena[0] <= 'Z'))
	)) {
    error += "El campo " + campo + " debe comenzar con una letra";
    return(error);
  }

  //Verificar los caracteres validos del campo
  string::const_iterator it = cadena.begin();
  while (it != cadena.end()) {
    if (!(
	  ((*it >= '0') && (*it <= '9')) ||
	  ((*it >= 'a') && (*it <= 'z')) ||
	  ((*it >= 'A') && (*it <= 'Z')) ||
	  (*it == '_') || (*it == '.')
	  )) {
      error += "El campo " + campo + " solo debe contener letras, números, el caracter _ " +
	"y el caracter .";
      return(error);
    }
    it++;
  }

  //Verificar cuantos puntos hay
  int puntos = 0;
  it = cadena.begin();
  while (it != cadena.end()) {
    if (*it == '.')
      puntos++;
    it++;
  }
  if (puntos > 1) {
    error += "Solo puede haber un caracter . en el campo " + campo;
    return(error);
  }
  if (puntos > 0) {
    if (cadena[0] == '.') {
      error += "No puede haber un punto al comienzo del campo " + campo;
      return(error);
    }
    if (cadena[cadena.length() - 1] == '.') {
      error += "No puede haber un punto al final del campo " + campo;
      return(error);
    }
  }

  //Devolver el OK
  return("");
}
