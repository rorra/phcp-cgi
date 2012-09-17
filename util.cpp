#include <string>
#include <cstdio>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

using namespace std;

/* Funciones de cadena ------------------------------------------------------------------------- */

// Borra los espacios en blanco a los costados de la cadena
std::string &trimString(std::string &cad) {
  //Si la cadena esta vacia devolver, si el primer caracter es un newline, devolver
  if (cad.length() <= 0)
    return cad;

  //Remover los espacios en blanco del principio de la cadena
  while (cad[0] == ' ')
    cad.erase(1);

  //Remover los espacios en blanco del final de la cadena
  while (cad[cad.length() - 1] == ' ')
    cad.erase(cad.length() - 1);

  return cad;
}

// Borra los espacios en blanco y newlines a los costados de la cadena
std::string& strtrimString(std::string &cad) {
  //Si la cadena esta vacia devolver; si el primer caracter es un newline, devolver
  if (cad.length() <= 0)
    return cad;

  if (cad[0] == '\n') {
    cad.erase(cad.begin(), cad.end());
    return cad;
  }

  //Remover los espacios en blanco y newlines del principio de la cadena
  while ((cad[0] == '\n') || (cad[0] == ' '))
    cad.erase(1);

  //Remover los espacios en blanco y newlines del final de la cadena
  while ((cad[cad.length() - 1] == '\n') || (cad[cad.length() - 1] == ' '))
    cad.erase(cad.length() - 1);

  return cad;
}

//Hace una copia a mayusculas de una cadena
std::string upperCase(std::string s) {
  char *buf = new char[s.length()];
  s.copy(buf, s.length());
  for (unsigned int i = 0; i < s.length(); i++)
    buf[i] = toupper(buf[i]);
  string r(buf, s.length());
  delete buf;
  return r;
}

//Hace una copia a minusculas de una cadena
std::string lowerCase(std::string s) {
  char *buf = new char[s.length()];
  s.copy(buf, s.length());
  for (unsigned int i = 0; i < s.length(); i++)
    buf[i] = tolower(buf[i]);
  string r(buf, s.length());
  delete buf;
  return(r);
}
/* Funciones de cadena ------------------------------------------------------------------------- */

/* Funciones varias ---------------------------------------------------------------------------- */
std::string dominioEmail(std::string &cad) {
  int pos = cad.find("@");
  if (pos > 0) {
    string resultado(cad, pos + 1);
    return(resultado);
  }
  else
    return("");
}

std::string usuarioEmail(std::string &cad) {
  int pos = cad.find("@");
  if (pos > 0) {
    string resultado(cad, 0, pos);
    return(resultado);
  }
  else
    return("");
}

std::string dominioApache(const std::string &dom) {
  //La funcion devuelve el nombre de un dominio
  unsigned int pos;

  //Buscar el primer .
  pos = dom.find('.');
  if (pos == string::npos)
    return(dom);

  //Declarar un nuevo string con el dominio
  string ret(dom, 0, pos);

  //Devolver la cadena
  return(ret);
}

std::string extensionApache(const std::string &dom) {
  /* La funcion devuelve la extension de un dominio con \ delante . */
  unsigned int pos;

  //Buscar el primer .
  pos = dom.find('.');
  if (pos == string::npos)
    return(dom);

  //Declarar un nuevo string con la extension
  string ret(dom, pos + 1);

  //Remplazar todos los . por /
  pos = ret.find('.');
  while (pos != string::npos) {
    ret.insert(pos, "\\");
    pos = ret.find('.', pos + 1);
  }

  //Devolver la cadena
  return(ret);
}

int existeArchivo(const std::string &path) {
  struct stat buf;
  int fd;
  mode_t mode;

  /* Tratar de abrir el archivo */
  if (( fd = open(path.c_str(), O_RDONLY)) < 0)
    return(0);

  /* Obtener la informacion del archivo */
  if ((fstat(fd, &buf)) < 0) {
    close(fd);
    return(0);
  }

  mode = buf.st_mode;

  if (S_ISREG(mode)) {
    close(fd);
    return(1);
  } else {
    close(fd);
    return(0);
  }
}

int existeDirectorio(const std::string &path) {
  struct stat buf;
  int fd;
  mode_t mode;

  /* Tratar de abrir el directorio */
  if (( fd = open(path.c_str(), O_RDONLY)) < 0)
    return(0);

  /* Obtener la informacion del archivo */
  if ((fstat(fd, &buf)) < 0) {
    close(fd);
    return(0);
  }

  mode = buf.st_mode;

  if (S_ISDIR(mode)) {
    close(fd);
    return(1);
  } else {
    close(fd);
    return(0);
  }
}

long obtenerTamanio(std::string archivo, std::string directorio) {
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

/* Funciones varias ---------------------------------------------------------------------------- */

/* Funciones de codificación ------------------------------------------------------------------- */
std::string codificarPassword(std::string password) {
  for (unsigned int x = 0; x < password.length(); x++)
    password[x] += 1;
  return(password);
}

std::string decodificarPassword(std::string password) {
  for (unsigned int x = 0; x < password.length(); x++)
    password[x] -= 1;
  return(password);
}

std::string passwordAleatorio() {
  string password;
  int numero;

  srand(time(NULL));
  numero = (rand() % 99999999) + 1;

  char *temp = new char[9];
  snprintf(temp, 9, "%d", numero);
  password = temp;
  delete temp;

  return(password);
}
/* Funciones de codificación ------------------------------------------------------------------- */
