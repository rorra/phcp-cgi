#ifndef _HTTP_A_H 
#define _HTTP_A_H 
#include <vector> 
#include <mysql++.h>

mysqlpp::Connection con;

int CrearEstadisticas(std::string &, std::string &);
int ParsearEstadisticas(std::string &, std::string &);
int RecuperarDominios(std::vector<std::string> &, std::vector<std::string> &, mysqlpp::Connection &);

#endif // _HTTP_A_H
