#ifndef _SERVICIOEXTENSIONFP_H
#define _SERVICIOEXTENSIONFP_H

#include "servicio.h"
#include "dominio.h"

class cServicioExtensionFP:public cServicio {
 public:
  //Constructor e iniciador
  cServicioExtensionFP(cDominio *dominio);
  bool iniciar();

  //Agrega las extensiones del FP en el sistema
  int agregarExtensionFP(const std::string &, const std::string &, cDominio &);
  //Agrega una extension FP en la base de datos
  int agregarExtensionFPDB(const std::string &, const std::string &, cDominio &);
  //Cambia el password de las extensiones del FP en el sistema
  int cambiarClaveExtensionFP(const std::string &, const std::string &, cDominio &);
  //Cambia el password de las extensiones del FP en la base de datos
  int cambiarClaveExtensionFPDB(const std::string &, const std::string &, cDominio &);
  //Quita las extensiones del FP del sistema
  int quitarExtensionFP(cDominio &);
  //Quita las extensiones del FP de la base de datos
  int quitarExtensionFPDB(cDominio &);
  //Trae la cantidad del servicio
  std::string traerCantidad();
  //Verifica la existencia de las extensiones del FP en la base de datos
  int verificarExisteExtensionFPDB(cDominio &);

 protected:
  int agregarFileFP1(cDominio &);
  int agregarFileFP2(cDominio &);
  int agregarFileFP3(cDominio &);
  int agregarFileFP4(cDominio &);
  int agregarFileFP5(cDominio &);
  int agregarFileFP6(cDominio &);
  int agregarFileFP7(cDominio &);
  int agregarFileFP8(const std::string &, cDominio &);
  int agregarFileFP9(cDominio &);
  int agregarFileFP10(cDominio &);
  int agregarFileFP11(cDominio &);
  int agregarFileFP12(cDominio &);
  int agregarFileFP13(cDominio &);
  int agregarFileFP14(cDominio &);
  int agregarFileFP15(const std::string &, cDominio &);
  int agregarFileFP16(cDominio &);
  int agregarFileFP17(const std::string &, const std::string &, cDominio &);
  int agregarFileFP18(cDominio &);
  int agregarFileFP19(cDominio &);
  int agregarFileFP20(cDominio &);
  int agregarFileFP21(cDominio &);
  int agregarFileFP22(cDominio &);
  int agregarFileFP23(cDominio &);
  int agregarFileFP24(cDominio &);
  int agregarFileFP25(cDominio &);
};

#endif // _SERVICIOEXTENSIONFP_H
