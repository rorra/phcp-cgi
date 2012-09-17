#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPRedirectHeader.h>
#include "dominio.h"
#include "interfaz.h"
#include "configuracion.h"

using namespace std;

cInterfaz::cInterfaz() {
}

int cInterfaz::obtenerVariable(std::string varName, std::string &strVar, cDominio &dominio) {
  //Obtener la variable varName y almacenarla en strVar
  cgicc::form_iterator fiComando = cgi.getElement(varName);
  if (fiComando == cgi.getElements().end()) {
    dominio.loguear(string("Error/Warning al obtener la variable " + varName + " del html interfaz"));
    return(-1);
  }
  strVar = fiComando->getValue();

  return(0);
}

void cInterfaz::redirigir(const std::string &url) {
  //cout << cgicc::HTTPRedirectHeader("http://" + url);
  cout << cgicc::HTTPHTMLHeader() << endl
       << "<html>" << endl
       << "<head>" << endl
       << "<title>Powered-Hosting</title>" << endl
       << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">" << endl
       << "<script type=\"text/javascript\">" << endl
       << "<!--" << endl
       << "window.location = \"" << url << "\"" << endl
       << "//-->" << endl
       << "</script>" << endl
       << "</head>" << endl
       << "</html>" << endl;
}

void cInterfaz::redirigirDom(const std::string &pagina, const cDominio &dominio) {
  cout << "Location: http://" << dominio.getUrl() << "/" << pagina << endl << endl;
}

void cInterfaz::reportarErrorComando(const std::string &error, const std::string &file,
				     const cDominio &dominio) {
  string ubicacion;
  ubicacion += "error/";
  ubicacion += file;
  ubicacion += "?errores=";
  ubicacion += error;
  redirigir(ubicacion);
}

void cInterfaz::reportarErrorFatal() {
  string ubicacion;
  ubicacion += "error/";
  ubicacion += ERROR_FATAL;
  redirigir(ubicacion);
}

void cInterfaz::reportarMensaje(const std::string &mensaje) {
  cout << cgicc::HTTPHTMLHeader() << endl
       << cgicc::head() << cgicc::head() << endl
       << cgicc::html() << endl
       << mensaje << "<BR>" << endl
       << cgicc::html() << endl;
}

void cInterfaz::reportarOkComando(const std::string &mensaje, const std::string &file,
				  const cDominio &dominio) {
  string ubicacion;
  ubicacion += "error/";
  ubicacion += file;
  ubicacion += "?ok=";
  ubicacion += mensaje;
  redirigir(ubicacion);
}

void cInterfaz::reportarOkComandoBackup(const std::string &mensaje, const std::string &link,
					const std::string &archivo, const std::string &file,
					const cDominio &dominio) {
  string ubicacion;
  ubicacion += "error/";
  ubicacion += file;
  ubicacion += "?ok=";
  ubicacion += mensaje;
  ubicacion += "&link=";
  ubicacion += link;
  ubicacion += "&archivo=";
  ubicacion += archivo;
  redirigir(ubicacion);
}

int cInterfaz::verificarPagina(const std::string &pagina) {
  cgicc::CgiEnvironment env = cgi.getEnvironment();
  string path = env.getReferrer();
  int pos = path.rfind("/");
  if (pos <= 0)
    return(-1);
  string realpage(path, pos + 1);
  if (realpage == pagina)
    return(0);
  if (realpage.find(pagina) != string::npos)
    return(1);
  else
    return(-1);
}
