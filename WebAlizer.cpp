#include <fstream>
#include <string>
#include <mysql++.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "log.h"
#include "configuracion.h"
#include "util.h"
#include "WebAlizer.h"

using namespace std;
using namespace mysqlpp;

int main(int argc, char **argv) {

  //Obtener permisos de root
  if (setuid(0) < 0)
    return(-1);

  //Conectarse a la base de datos
  con = new Connection(use_exceptions);
  con.connect(DB_NAME, DB_HOST, DB_USER, DB_PASS);
  
  //Recuperar los dominios
  std::vector<std::string> idDominio, dominio;
  RecuperarDominios(idDominio, dominio, con);

  //Crear las estadisticas
  for (unsigned i = 0; i < idDominio.size(); i++)
    CrearEstadisticas(idDominio[i], dominio[i]);
}

int CrearEstadisticas(std::string &idDominio, std::string &dominio) {
  std::string directorio = "/www/docs/webs/phcm/" + dominio + "/stats/", comando;

  //Si no existe el directorio crearlo
  if(!existeDirectorio(directorio)) {
    if (mkdir(directorio.c_str(), 0755) < 0) {
      return(0);
    }
  }

  //Si no hay archivo de log o esta vacio salir
  std::string archivo = "/www/docs/" + dominio + "/access-log.txt";
  if((!existeArchivo(archivo)) || (obtenerTamanio(archivo) <= 0))
    return(1);

  //Crear las estadisticas
  comando = "/usr/local/bin/webalizer -n www." + dominio + " -t Dominio: -o " +  
  "/www/docs/webs/phcm/" + dominio + "/stats/ " + archivo;
  system(comando.c_str());

  //Parsear las estadisticas en el index.php
  if (!ParsearEstadisticas(dominio, idDominio))
    return(0);

  return(1);
}

int ParsearEstadisticas(std::string &dominio, std::string &idDominio) {
  std::string archivo, archivo2, comando;
  archivo = "/www/docs/webs/phcm/" + dominio + "/stats/index.php";
  archivo2 = "/www/docs/webs/phcm/" + dominio + "/stats/index.php.bak";

  //Copiar el archivo para actualizarlo
  comando = "cp -f " + archivo + " " + archivo2;
  system(comando.c_str());

  //Abrir el archivo para lectura
  ifstream in(archivo2.c_str());
  if (!in.is_open()) {
    comando = "cp -f " + archivo2 + " " + archivo;
    system(comando.c_str());
    return(0);
  }

  //Abrir el archivo para escritura
  ofstream out(archivo.c_str());
  if (!out.is_open()) {
    comando = "cp -f " + archivo2 + " " + archivo;
    system(comando.c_str());
    return(0);
  }

  //Agregar la linea al archivo
  char buffer[MINLINE];
  std::string linea;
  out << "<?PHP $requiredUserLabel = array(" << idDominio
      << ");include (\"../secure/secure.php\");?>" <<  endl;
  while(in.getline(buffer, MINLINE)) {
    linea = buffer;
    out << linea << endl;
  }

  //Cerrar los archivos
  in.close();
  out.close();

  return(0);
}

int RecuperarDominios(std::vector<std::string> &idDominio, std::vector<std::string> &dominio,
		      Connection &con) {
  //Recuperar la identidad del dominio, el usuario, el password y el password FTP
  try {
    Query qry = con.query();
    qry << "SELECT D.ID_DOMINIO, D.DOMINIO FROM DOMINIO AS D, DOM_EMP AS E, SERV AS S WHERE D.ID_DOMINIO = E.ID_DOMINIO AND E.ID_SERV = S.ID_SERV AND S.ID_SERV BETWEEN 1 AND 6 ORDER BY D.ID_DOMINIO";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    for (i = res.begin(); i < res.end(); i++) {
      row = *i;
      idDominio.push_back(std::string(row[0]));
      dominio.push_back(std::string(row[1]));
    }
    return(1);
  }
  catch (BadQuery er) {
    return(0);
  }
  catch (...) {
    return(0);
  }
  return(0);
}
