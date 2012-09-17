#include <string>
#include <mysql++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unistd.h>
#include "dominio.h"
#include "util.h"
#include "servicio.h"
#include "servicioBackup.h"
#include "configuracion.h"
#include <sys/wait.h>


#include <iostream>

using namespace std;
using namespace mysqlpp;

cServicioBackup::cServicioBackup(std::string t, cDominio *dom):cServicio(dom) {
  servicioDB = t;
  if (t == "BACKUP DB")
    tipo = "DB";
  if (t == "BACKUP FTP")
    tipo = "FTPA";
  if (t == "BACKUP MAIL")
    tipo = "EMAIL";
  if (t == "BACKUP WEB")
    tipo = "WEB";
}

bool cServicioBackup::iniciar() {
  bool resultado = true;
  cServicio::iniciar();
  traerLimite();
  traerCantidad();
  if ((limite.length() <= 0) || (cantidad.length() <= 0))
    resultado = false;
  return(resultado);
}

int cServicioBackup::agregarBackup(const std::string &compresion, const std::string &archivo,
				   cDominio &dominio, const std::string &nombreDB) {
  std::string comando;

  //Obtener permisos de root
  if (setuid(0) < 0) {
    dominio.loguear("Error al obtener permisos de root");
    return(-1);
  }

  //Directorio y nombre de archivo destino
  std::string destino =* new string("/www/docs/" + dominio.getDominio() + "/backups/"
				    + archivo);
  //Crear Directorio de Backups
  comando = "mkdir -p /www/docs/" + dominio.getDominio() + "/backups/";
  system(comando.c_str());
  
  //Copiar archivo index.php
  comando = "cp -f /www/docs/webs/backups/index.php /www/docs/" + dominio.getDominio() + "/backups/index.php";
  system(comando.c_str());

  //Eliminar los backups anteriores
  //comando = "rm -f /www/docs/webs/backups/*" + dominio.getDominio() + "/*" + tipo + "*";
  //system(comando.c_str());
  
  //Crear el backup
  //Backup de base de datos
  if (tipo == "DB") {
    //Moverse al directorio donde se encuentra el origen
    comando = "/www/docs/" + dominio.getDominio() + "/";
    if ((chdir(comando.c_str())) < 0) {
      comando = "Error al moverse al directorio " + comando;
      dominio.loguear(comando);
      return(-1);
    }

    std::string origen;
    origen = nombreDB + ".SQL";

    int pid = 0;
     
    if ((pid = fork()) == -1) {
	}
    if (pid == 0) { /* Proceso Hijo */

    comando = "/usr/local/mysql/bin/mysqldump -pclave " + nombreDB + " > " + origen;
    system(comando.c_str());   
    
    if (compresion == "Gzip") 
      comando = "tar cvzf " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Rar")
      comando = "/usr/local/bin/rar a " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Zip")
      comando = "zip -r " + destino + " " + origen + " > /dev/null";

    //Comprimir la base de datos
    system(comando.c_str());

    //Eliminar el archivo temporal
    comando = "rm " + origen;
    system(comando.c_str());
    
    //Cambiar los permisos del directorio
    comando = "chown -R pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + "/www/docs/" + dominio.getDominio() + "/backups/";
    system(comando.c_str());
}    
  }   else { 
  
  //Backup de ftp anonimo
    if (tipo == "FTPA") {
    //Origen
    std::string origen;
    origen = "ftp_anonimo/";

    //Moverse al directorio donde se encuentra el origen
    comando = "/www/docs/" + dominio.getDominio() + "/";
    if ((chdir(comando.c_str())) < 0) {
      comando = "Error al moverse al directorio " + comando;
      dominio.loguear(comando);
      return(-1);
    }

    int pid = 0;
     
    if ((pid = fork()) == -1) {
	}
    if (pid == 0) { /* Proceso Hijo */

    //Armar el comando para comprimir
    if (compresion == "Gzip")
      comando = "tar zcvf " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Rar")
      comando = "/usr/local/bin/rar a " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Zip")
      comando = "zip -r " + destino + " " + origen + " > /dev/null";

    //Comprimir el ftp anonimo
    system(comando.c_str());

    //Cambiar los permisos del directorio
    comando = "chown -R pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + "/www/docs/" + dominio.getDominio() + "/backups/";
    system(comando.c_str());    

}    
  }   else { 

  //Backup de Emails
  if (tipo == "EMAIL") {
    //Moverse al directorio de los e-mails
    comando = "/var/spool/mail";
    if ((chdir(comando.c_str())) < 0) {
      comando = "Error al moverse al directorio " + comando;
      dominio.loguear(comando);
      return(-1);
    }

    //Origen
    std::string origen = obtenerEmails(dominio);

    int pid = 0;
     
    if ((pid = fork()) == -1) {
	}
    if (pid == 0) { /* Proceso Hijo */

    //Armar el comando para comprimir
    if (compresion == "Gzip")
      comando = "tar zcvf " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Rar")
      comando = "/usr/local/bin/rar a " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Zip")
      comando = "zip -r " + destino + " " + origen + " > /dev/null";

    //Comprimir el ftp anonimo
    system(comando.c_str());
    
    //Cambiar los permisos del directorio
    comando = "chown -R pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + "/www/docs/" + dominio.getDominio() + "/backups/";
    system(comando.c_str());
}    
  }   else {     

  //Backup de web
    if (tipo == "WEB") {
    //Moverse al directorio donde se encuentra el origen
    comando = "/www/docs/" + dominio.getDominio() + "/";
    if ((chdir(comando.c_str())) < 0) {
      comando = "Error al moverse al directorio " + comando;
      dominio.loguear(comando);
      return(-1);
    }

    //Origen
    std::string origen;
    origen = "public_html/";

    int pid = 0;
     
    if ((pid = fork()) == -1) {
	}
    if (pid == 0) { /* Proceso Hijo */

    //Armar el comando para comprimir
    if (compresion == "Gzip")
      comando = "tar zcvf " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Rar")
      comando = "/usr/local/bin/rar a -r " + destino + " " + origen + " > /dev/null";
    else if (compresion == "Zip")
      comando = "zip -r " + destino + " " + origen + " > /dev/null";

    //Comprimir el sitio web
    system(comando.c_str());
    
    //Cambiar los permisos del directorio
    comando = "chown -R pdu" + dominio.getIdDominio() + ":pdu" + dominio.getIdDominio() + " " + "/www/docs/" + dominio.getDominio() + "/backups/";
    system(comando.c_str());
		    return(0);
		    }
	        return(0);
		}
	    return(0);
	    }
	return(0);
    }
    return(0);
    }    
return(0);
}    

int cServicioBackup::agregarBackupDB(const std::string &compresion, cDominio &dominio) {
  try {
    Query qry = dominio.con.query();
    qry << "REPLACE INTO BACKUP(ID_DOMINIO, FECHA_HORA, COMPRESION, TIPO) VALUES('" 
	<< dominio.getIdDominio() << "', NOW(), '" << compresion << "', '" << tipo << "')";
    qry.execute();
    return(0);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(-1);
  } catch(...) {
    string error;
    error = "Error al agregar el backup " + tipo + " con compresión " + compresion;
    dominio.loguear(error);
    return(-2);
  }
}

std::string cServicioBackup::obtenerEmails(cDominio &dominio) {
  std::string resultado;
  try {
    Query qry = dominio.con.query();
    qry << "SELECT CUENTA FROM CUENTA_POP3 WHERE ID_DOMINIO = '" << dominio.getIdDominio()
	<< "'";
    Result res = qry.store();
    Row row;
    Result::iterator i;
    for (i = res.begin(); i != res.end(); i++) {
      row = *i;
      resultado += " ";
      resultado += std::string(row[0]);
      resultado += "-";
      resultado += dominio.getIdDominio();
    }
    return(resultado);
  } catch(BadQuery er) {
    dominio.loguear(er.error);
    return(resultado);
  } catch(...) {
    string error;
    error = "Error al recuperar las cuentas de emails";
    dominio.loguear(error);
    return(resultado);
  }
}

std::string cServicioBackup::traerCantidad() {
  try {
    Query qry = dominio->con.query();
    qry << "SELECT COUNT(*) FROM BACKUP WHERE ID_DOMINIO = '" << dominio->getIdDominio()
	<< "' AND TIPO = '" << tipo << "' AND FECHA_HORA > (NOW() - INTERVAL 1 HOUR)";
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

std::string cServicioBackup::tamanioBackup(const std::string &arch, cDominio &dominio) {
  std::string archivo;
  std::string path;

  path = "/www/docs/webs/backups/" + dominio.getDominio();

  float tamanio = obtenerTamanio(arch, path);
  float tam;

  char *strtam = new char[MINLINE];

  //Armar el tamanio
  if (tamanio > 1000000) {
    tam = tamanio / ((1024) * (1024));
    snprintf(strtam, MINLINE, "%.2f Mb", tam);
  } else if (tamanio > 1024) {
    tam = tamanio / 1024;
    snprintf(strtam, MINLINE, "%.2f Kb", tam);
  } else
    snprintf(strtam, MINLINE, "%.0f bytes", tamanio);

  std::string ret(strtam);
  delete[] strtam;

  return(ret);
}

int cServicioBackup::verificarCompresion(std::string &compresion) {
  if ((compresion == "Zip") || (compresion == "Rar") || (compresion == "Gzip"))
    return(0);
  else
    return(1);
}

//int cServicioBackup::verificarExisteBackupDB(cDominio &dominio) {
//  int resultado = 0;
//  try {
//    Query qry = dominio.con.query();
//    qry << "SELECT COUNT(*) FROM BACKUP WHERE ID_DOMINIO = '" << dominio.getIdDominio()
//	<< "' AND TIPO = '" << tipo << "' AND FECHA_HORA > (NOW() - INTERVAL 1 HOUR)";
//    Result res = qry.store();
//    Row row;
//    Result::iterator i;
//    if (res.size() > 0) {
//      i = res.begin();
//      row = *i;
//      resultado = row[0];
//    }
//    return(resultado);
//  } catch(BadQuery er) {
//    dominio.loguear(er.error);
//    return(resultado);
//  } catch(...) {
//    string error;
//    error = "Error al verificar si existe el backup " + tipo;
//    dominio.loguear(error);
//    return(resultado);
//  }
//}

std::string cServicioBackup::getNombreArchivo(const std::string &compresion, cDominio &dominio) {
  std::string nombreArchivo;
  time_t tiempo_t;
  struct tm *tiempo;

  //Obtener la fecha local
  tiempo_t = time(NULL);
  tiempo = localtime(&tiempo_t);

  char *buffer = new char[MINLINE];
  snprintf(buffer, MAXLINE, "%d-%02d-%d-%02d-%02d-%02d",
	   tiempo->tm_mday, tiempo->tm_mon + 1, tiempo->tm_year + 1900,
	   tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec);
  nombreArchivo += dominio.getDominio();
  nombreArchivo += "-";
  nombreArchivo += tipo;
  nombreArchivo += "-";
  nombreArchivo += buffer;
  delete[] buffer;
  if (compresion == "Gzip")
    nombreArchivo += ".tar.gz";
  if (compresion == "Zip")
    nombreArchivo += ".zip";
  if (compresion == "Rar")
    nombreArchivo += ".rar";
  return(nombreArchivo);
}

