#ifndef _WEBALIZER_H
#define _WEBALIZER_H

#include <vector>
#include <mysql++.h>

mysqlpp::Connection con; 

int CrearEstadisticas(std::string &, std::string &);
int ParsearEstadisticas(std::string &, std::string &);
int RecuperarDominios(std::vector<std::string> &, std::vector<std::string> &, mysqlpp::Connection &);

#endif // _WEBALIZER_H
