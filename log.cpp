#include <string>
#include <cstdio>
#include <unistd.h>
#include <time.h>
#include "configuracion.h"
#include "log.h"
#include <fstream>

using namespace std;

cLog::cLog(std::string &dom):dominio(dom){
  fileDominio = NULL;
  fileComun = NULL;
};

bool cLog::abrirFileDominio() {
  char* ubicacion = new char[MINLINE];
  snprintf(ubicacion, MINLINE, "%s%s/.errores_panel", PANEL_DIR, dominio.c_str());
  fileDominio = fopen(ubicacion, "a");
  delete(ubicacion);
  if (!fileDominio)
    return false;
  return true;
}

bool cLog::abrirFileComun() {
  fileComun = fopen(LOG_COMUN_FILE, "a");
  if (!fileComun)
    return false;
  return true;
}

bool cLog::cerrarFileDominio() {
  if (fileDominio) {
    fclose(fileDominio);
    return true;
  }
  return false;
}

bool cLog::cerrarFileComun() {
  if (fileComun) {
    fclose(fileComun);
    return true;
  }
  return false;
}

void cLog::loguear(const std::string &mensaje) {
  //Obtener la fecha y hora del sistema
  string tiempo;
  obtenerDiaHora(tiempo);

  //Loguear el error en el archivo log del dominio
  if (abrirFileDominio()) {
    fputs(string(tiempo + ": " + mensaje + "\n").c_str(), fileDominio);
    cerrarFileDominio();
  }

  //Loguear el error en el archivo log comun
  if (abrirFileComun()) {
    fputs(string(tiempo + " - " + dominio + ": " + mensaje + "\n").c_str(), fileDominio);
    cerrarFileComun();
  }
}

void reportarErrorRarisimo(std::string mensaje) {
  //Obtener la fecha y hora del sistema
  string tiempo;
  obtenerDiaHora(tiempo);

  //Recuperar el directorio donde ocurrio el error
  char *tempDir = new char[MINLINE];
  if ((tempDir = getcwd(tempDir, MAXLINE)) == NULL)
    return;

  //Abrir el archivo comun para reportar el error
  FILE *file = NULL;
  file = fopen(LOG_COMUN_FILE, "a");
  if (!file) {
    delete[] tempDir;
    return;
  }
    
  //Loguear el error
  fputs(string(tiempo + " - " + tempDir + ": " + "Error inesperado. " + mensaje + "\n").c_str(),
	file);

  //Eliminar la memoria dinamica
  delete[] tempDir;

  //Cerar el archivo
  fclose(file);
}

void obtenerDiaHora(std::string &diaHora) {
  time_t tiempo_t;
  struct tm *tiempo;

  //Obtener la fecha local
  tiempo_t = time(NULL);
  tiempo = localtime(&tiempo_t);

  char *buffer = new char[MAXLINE];
  snprintf(buffer, MAXLINE, "%d/%02d/%d %02d:%02d:%02d",
	   tiempo->tm_year + 1900, tiempo->tm_mon + 1, tiempo->tm_mday,
	   tiempo->tm_hour, tiempo->tm_min, tiempo->tm_sec);
  diaHora = buffer;
  delete[] buffer;
}


