#include <string>
#include <time.h>
#include "tiempo.h"
#include "configuracion.h"

using namespace std;

Tiempo::Tiempo() {
  time_t tiempo_t;
  struct tm *tiempo_s;

  //Obtener la fecha local
  tiempo_t = time(NULL);
  tiempo_s = localtime(&tiempo_t);

  char *buffer = new char[MAXLINE];
  snprintf(buffer, MAXLINE, "%d", tiempo_s->tm_year + 1900);
  anio = buffer;
  snprintf(buffer, MAXLINE, "%02d", tiempo_s->tm_mon + 1);
  mes = buffer;
  snprintf(buffer, MAXLINE, "%02d", tiempo_s->tm_mday);
  dia = buffer;
  snprintf(buffer, MAXLINE, "%02d", tiempo_s->tm_hour);
  hora = buffer;
  snprintf(buffer, MAXLINE, "%02d", tiempo_s->tm_min);
  minuto = buffer;
  snprintf(buffer, MAXLINE, "%02d", tiempo_s->tm_sec);
  segundo = buffer;
  delete[] buffer;
}

std::string Tiempo::getSegundo() {
  return(segundo);
}

std::string Tiempo::getMinuto() {
  return(minuto);
}

std::string Tiempo::getHora() {
  return(hora);
}

std::string Tiempo::getDia() {
  return(dia);
}

std::string Tiempo::getMes() {
  return(mes);
}

std::string Tiempo::getAnio() {
  return(anio);
}


