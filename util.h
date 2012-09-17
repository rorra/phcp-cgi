#ifndef _UTIL_H
#define _UTIL_H

#include <string>

// Funciones de cadena
std::string &trimString(std::string &cad);
std::string& strtrimString(std::string &cad);
std::string upperCase(std::string);
std::string lowerCase(std::string);

// Funciones varias
std::string dominioEmail(std::string &cad);
std::string usuarioEmail(std::string &cad);
std::string dominioApache(const std::string&);
std::string extensionApache(const std::string&);
int existeArchivo(const std::string &);
int existeDirectorio(const std::string &);
long obtenerTamanio(std::string archivo, std::string directorio = "");

// Funciones de codificación
std::string codificarPassword(std::string);
std::string decodificarPassword(std::string);
std::string passwordAleatorio();

#endif // _UTIL_H
