#include <cstdlib>
#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioExtensionFP.h"
#include "configuracion.h"

using namespace std;
using namespace mysqlpp;

cServicioExtensionFP::cServicioExtensionFP(cDominio *dom):cServicio(dom) {
  servicioDB = "FRONTPAGE";
}

bool cServicioExtensionFP::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioExtensionFP::agregarExtensionFP(const std::string &usuario,
					   const std::string &password, cDominio &dominio) {
  std::string comando, comando2, directorio;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  /* Copiar todos los archivos y directorios del panel */
  directorio = "/www/docs/" + dominio.getDominio() + "/public_html/";
  comando = "cp -fr /phcm/panel/frontpage/* " + directorio;
  system(comando.c_str());

  /* Crear los archivos del frontpage */
  agregarFileFP1(dominio);
  agregarFileFP2(dominio);
  agregarFileFP3(dominio);
  agregarFileFP4(dominio);
  agregarFileFP5(dominio);
  agregarFileFP6(dominio);
  agregarFileFP7(dominio);
  agregarFileFP8(usuario, dominio);
  agregarFileFP9(dominio);
  agregarFileFP10(dominio);
  agregarFileFP11(dominio);
  agregarFileFP12(dominio);
  agregarFileFP13(dominio);
  agregarFileFP14(dominio);
  agregarFileFP15(usuario, dominio);
  agregarFileFP16(dominio);
  agregarFileFP17(usuario, password, dominio);
  agregarFileFP18(dominio);
  agregarFileFP19(dominio);
  agregarFileFP20(dominio);
  agregarFileFP21(dominio);
  agregarFileFP22(dominio);
  agregarFileFP23(dominio);
  agregarFileFP24(dominio);
  agregarFileFP25(dominio);

  /* Modificar los permisos de todos los directorios */
  comando2 = "chown -R pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + directorio + "/";

  comando = comando2 + "images";
  system(comando.c_str());

  comando = comando2 + "_private";
  system(comando.c_str());

  comando = comando2 + "_vti_bin";
  system(comando.c_str());

  comando = comando2 + "_vti_bin/_vti_adm";
  system(comando.c_str());

  comando = comando2 + "_vti_bin/_vti_auth";
  system(comando.c_str());

  comando = comando2 + "_vti_cnf";
  system(comando.c_str());

  comando = comando2 + "_vti_log";
  system(comando.c_str());

  comando = comando2 + "_vti_pvt";
  system(comando.c_str());

  comando = comando2 + "_vti_txt";
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarExtensionFPDB(const std::string &usuario,
					       const std::string &password, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "INSERT INTO EXTENSION_FP(ID_DOMINIO, USUARIO, PASSWORD, ESTADO) VALUES('"
	<< dominio.getIdDominio() << "', '" << usuario << "', '" << password << "', '1')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar las extensiones del FP para el usuario " + usuario;
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioExtensionFP::agregarFileFP1(cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_private/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "allow from all" << endl
      << "require group authors administrators" << endl
      << "</Limit>" << endl
      << "<Limit PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthType Basic" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " +
    archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP2(cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "allow from all" << endl
      << "</Limit>" << endl
      << "<Limit PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthName www." + dominio.getDominio() << endl
      << "AuthUserFile /www/docs/"+ dominio.getDominio() + "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " 
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP3(cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/_vti_adm/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "allow from all" << endl
      << "require group administrators" << endl
      << "</Limit>" << endl
      << "<Limit PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthType Basic" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP4(cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/_vti_aut/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "allow from all" << endl
      << "require group authors administrators" << endl
      << "</Limit>" << endl
      << "<Limit PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthType Basic" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " 
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP5(cDominio &dominio) {
  std::string archivo, comando;
  
  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_cnf/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "</Limit>" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP6(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_log/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST PUT DELET>" << endl
      << "deny from all" << endl
      << "</Limit>"<< endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP7(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP8(const std::string &usuario, cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/.roles";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "[Roles]" << endl
      << "advauthor=0CEEEE02| |View, add, and change pages, documents, themes, and borders; recalculate hyperlinks." << endl
      << "admin=FFFFFF03| |View, add, and change all server content; manage server settings and accounts." << endl
      << "author=0C80EC02| |View, add, and change pages and documents." << endl
      << "browser=08008800| |View pages and documents." << endl
      << endl
      << "[Anonymous]" << endl
      << "AnonymousRole=browser" << endl
      << endl
      << "[Users]" << endl
      << usuario << "=admin|FFFFFF03" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP9(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/access.cnf";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "vti_encoding:SR|utf8-nl" << endl
      << "RealName:www." << dominio.getDominio() << endl
      << "PasswordDir:/www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt" << endl
      << "InheritPermissions:false" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP10(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/botinfs.cnf";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "vti_encoding:SR|utf8-nl" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP11(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/bots.cnf";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "vti_encoding:SR|utf8-nl" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP12(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/deptodoc.btr";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP13(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/frontpg.lck";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 666 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP14(cDominio &dominio) {
  std::string archivo, comando, mes;

  /* Obtener la fecha local */
  time_t tiempo_t;
  struct tm *tiempo;
  tiempo_t = time(NULL);
  if (tiempo_t == -1)
    return(-1);
  if (( tiempo = localtime(&tiempo_t)) == NULL)
    return(-1);
  
  switch(tiempo->tm_mon + 1) {
  case(1):
    mes = "Jan";
    break;
  case(2):
    mes = "Feb";
    break;
  case(3):
    mes = "Mar";
    break;
  case(4):
    mes = "Apr";
    break;
  case(5):
    mes = "May";
    break;
  case(6):
    mes = "Jun";
    break;
  case(7):
    mes = "Jul";
    break;
  case(8):
    mes = "Ago";
    break;
  case(9):
    mes = "Sep";
    break;
  case(10):
    mes = "Oct";
    break;
  case(11):
    mes = "Nov";
    break;
  case(12):
    mes = "Dec";
    break;
  }

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.cnf";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "vti_encoding:SR|utf8-nl" << endl
      << "vti_restartmanual:IX|0" << endl
      << "vti_featurelist:VX|vti_ACGroups vti_ACUseDomains vti_ServiceMarkUrlDirBrowse vti_ServeMarkUrlDirScript vti_ACIPAddresses vti_ServerEmailTransport vti_ServerIndexServer vti_ServerODBC vti_ServerCollab vti_TimedDocEvents" << endl
      << "vti_webservertype:SR|apache-fp" << endl
      << "vti_categories:VR|Travel Expense\\\\ Report Business Competition Goals/Objetives Ideas Miscellaneous Waiting VIP In\\\\ Process Planning Schedule" << endl
      << "vti_approvalapproved:SR|Approved" << endl
      << "vti_approvaldenied:SR|Denied" << endl
      << "vti_timecreated:TR|" << tiempo->tm_mday << " " << mes << " " << (tiempo->tm_year + 1900) << " " << tiempo->tm_hour << ":" << tiempo->tm_min << ":" << tiempo->tm_sec << " -0000" << endl
      << "vti_oldestcompatibleversion:SR|2.0.0.0" << endl
      << "vti_defaultcharset:SR|windows-1252" << endl
      << "vti_autorecalc:IX|1" << endl
      << "disableautoimgsizeexts:SX|.asp" << endl
      << "createpostinfo:BX|false" << endl
      << "vti_extenderversion:SR|5.0.2.2623" << endl
      << "vti_publishmetainfokeys:VR|vti_assignedto vti_approvallevel vti_categories vti_description" << endl
      << "vti_textindexood:IR|0" << endl
      << "vti_approvallevels:VR|Approved Denied Pending\\\\ Review" << endl
      << "vti_hassearchbot:BR|false" << endl
      << "dependenciesood:IR|0" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP15(const std::string &usuario, cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.grp";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << "administrators: " << usuario << endl
      << "authors:" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP16(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.lck";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP17(const std::string &usuario, const std::string &password, cDominio &dominio) {
  std::string archivo, comando;

  //Archivo 
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/service.pwd";

  //Crear el archivo con el usuario y el password
  comando = "htpasswd -cbd " + archivo + " " + usuario + " " + password;
  system(comando.c_str());

  //Recuperar la linea con el usuario y el password
  char linea[MINLINE];
  ifstream in(archivo.c_str());
  if (in.is_open())
    in.getline(linea, MINLINE);
  else {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones";
    dominio.loguear(comando);
  }
  in.close();

  //Abrir el archivo
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  //Grabar el archivo
  out << "# -FrontPage-" << endl
      << linea << endl;

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + 
    " " + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP18(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo 
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/services.cnf";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP19(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo 
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/svcacl.cnf";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP20(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo 
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/writeto.cnf";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP21(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_txt/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "Options None" << endl
      << endl
      << "<Limit GET POST PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP22(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/.frontpage";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "# -FrontPage-" << endl
      << endl
      << "IndexIgnore .frontpage */.??* *~ *# */HEADER* */README* */_vti*" << endl
      << endl
      << "<Limit GET POST>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "allow from all" << endl
      << "</Limit>" << endl
      << "<Limit PUT DELETE>" << endl
      << "order deny,allow" << endl
      << "deny from all" << endl
      << "</Limit>" << endl
      << "AuthName www." << dominio.getDominio() << endl
      << "AuthUserFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.pwd" << endl
      << "AuthGroupFile /www/docs/" << dominio.getDominio() << "/public_html/_vti_pvt/service.grp" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + 
    archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP23(cDominio &dominio) {
  std::string archivo, archivo2, comando;

  /* Copiar el archivo postinfo.html para poder parsear */
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/postinfo.html";
  archivo2 = "/www/docs/" + dominio.getDominio() + "/public_html/postinfo.html.bak";
  comando = "cp -f " + archivo + " " + archivo2;
  if (system(comando.c_str()))
    return(-1);

  /* Abrir el archivo postinfo.html para actualizar */
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones del FP";
    return(-1);
  }

  /* Abrir el archivo postinfo.html.bak en modo lectura para parsear */
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo2 + " para las extensiones del FP";
    return(-1);
  }

  /* Llenar en buscada la cadena que se debe cambiar */
  std::string buscada("BaseUrl");

  /* Copiar el contenido de FILE_FP_23 en FILE_FP_23_BAK hasta que se encuentre la linea
     donde debe empezar a ignorarse */
  char buffer[MAXLINE];
  std::string linea;
  while (in.getline(buffer, MAXLINE)) {
    linea = buffer;
    /* Si se encuentra la cadena agregar la nueva linea */
    if (linea.find(buscada) != string::npos)
      linea = "    BaseUrl=\"http://www." + dominio.getDominio() + "\"";
    out << linea << endl;
  }

  /* Cerrar los archivos */
  in.close();
  out.close();

  /* Eliminar el archivo viejo */
  comando = "rm " + archivo2;
  system(comando.c_str());

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP24(cDominio &dominio) {
  std::string archivo, comando;

  //Archivo
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_inf.html";

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  /* Hacerlo inmutable */
  comando = "chattr +i " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::agregarFileFP25(cDominio &dominio) {
  std::string archivo, comando;

  //Abrir el archivo para escritura
  archivo = "/usr/local/frontpage/www." + dominio.getDominio() + ":80.cnf";
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "No se pudo abrir el archivo " + archivo + " para las extensiones"
      " del frontpage";
    dominio.loguear(comando);
  }

  /* Grabar el archivo con la configuracion */
  out << "vti_encoding:SR|utf8-nl" << endl
      << "servertype:apache-fp" << endl
      << "authoring:enabled" << endl
      << "extenderversion:5.0.2.2634" << endl
      << "frontpageroot:/usr/local/frontpage/version5.0" << endl
      << "serverconfig:/usr/local/apache/conf/extra/httpd-vhosts.conf" << endl;

  /* Cerrar el archivo */
  out.close();

  /* Darle al archivo los permisos correspondientes */
  comando = "chown pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " "
    + archivo;
  system(comando.c_str());
  comando = "chmod 644 " + archivo;
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::cambiarClaveExtensionFP(const std::string &usuario,
						  const std::string &password,
						  cDominio &dominio) {
  std::string archivo, comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //modificar los archivos con los passwords
  agregarFileFP8(usuario, dominio);
  agregarFileFP15(usuario, dominio);
  agregarFileFP17(usuario, password, dominio);

  return(0);
}

int cServicioExtensionFP::cambiarClaveExtensionFPDB(const std::string &usuario,
						    const std::string &password,
						    cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "UPDATE EXTENSION_FP SET USUARIO = '" << usuario << "', PASSWORD = '" << password
	<< "' WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al cambiar el password de las extensiones del FP";
    dominio.loguear(error);
    return(-2);
  }
}

int cServicioExtensionFP::quitarExtensionFP(cDominio &dominio) {
  std::string archivo, comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Eliminar los .frontpage
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_private/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/_vti_adm/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/_vti_aut/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_cnf/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_log/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_txt/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  //Eliminar los directorios de las extensiones
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_private";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/_vti_adm";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_bin/_vti_aut";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_cnf";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_log";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_pvt";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_txt";
  comando = "rm -fr " + archivo + "/*";
  system(comando.c_str());
  comando = "rm -fr " + archivo + "/.*";
  system(comando.c_str());
  comando = "rm -fr " + archivo;
  system(comando.c_str());

  //Eliminar los archivos basura
  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/.frontpage";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/postinfo.html";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/www/docs/" + dominio.getDominio() + "/public_html/_vti_inf.html";
  comando = "chattr -i " + archivo;
  system(comando.c_str());
  comando = "rm " + archivo;
  system(comando.c_str());

  archivo = "/usr/local/frontpage/www." + dominio.getDominio() + ":80.cnf";
  system(comando.c_str());

  return(0);
}

int cServicioExtensionFP::quitarExtensionFPDB(cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "DELETE FROM EXTENSION_FP WHERE ID_DOMINIO = '" << dominio.getIdDominio() << "'";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al quitar las extensiones del FP";
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioExtensionFP::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM EXTENSION_FP WHERE ID_DOMINIO = '" << dominio->getIdDominio()
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

int cServicioExtensionFP::verificarExisteExtensionFPDB(cDominio &dominio) {
  int resultado = 0;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT COUNT(*) FROM EXTENSION_FP WHERE ID_DOMINIO = '" << dominio.getIdDominio()
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
    error = "Error al verificar si existe las extensiones del FP";
    dominio.loguear(error);
    return(resultado);
  }
}
