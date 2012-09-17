#ifndef _TIEMPO_H
#define _TIEMPO_H

#include <string>

class Tiempo {

 public:
  Tiempo();
  std::string getSegundo();
  std::string getMinuto();
  std::string getHora();
  std::string getDia();
  std::string getMes();
  std::string getAnio();

 private:
  std::string segundo;
  std::string minuto;
  std::string hora;
  std::string dia;
  std::string mes;
  std::string anio;

  //No permitir copy constructor ni asignador
  Tiempo(Tiempo&);
  Tiempo &operator=(Tiempo&);
};

#endif // _TIEMPO_H
