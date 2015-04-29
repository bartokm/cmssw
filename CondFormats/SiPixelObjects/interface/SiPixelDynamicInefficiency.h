#ifndef SiPixelDynamicInefficiency_h
#define SiPixelDynamicInefficiency_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include<vector>
#include<map>
#include<iostream>
#include<boost/cstdint.hpp>


class SiPixelDynamicInefficiency {

 public:
 
  SiPixelDynamicInefficiency(){};
  ~SiPixelDynamicInefficiency(){};

  inline void putGeomFactors (std::map<unsigned int,double>& GeomFactors){m_GeomFactors=GeomFactors;} 
  inline const std::map<unsigned int,double>&  getGeomFactors () const {return m_GeomFactors;}

  inline void putPUFactors (std::map<unsigned int,std::vector<double> >& PUFactors){m_PUFactors=PUFactors;} 
  inline const std::map<unsigned int,std::vector<double> >&  getPUFactors () const {return m_PUFactors;}

  inline void puttheInstLumiScaleFactors(double& InstLumiScaleFactor){theInstLumiScaleFactor_=InstLumiScaleFactor;}
  inline const double gettheInstLumiScaleFactors() const {return theInstLumiScaleFactor_;}

  //inline void putPixelEfficiencies(bool& bpix_fpix, std::vector<std::vector<double> >&);
  inline const std::vector<std::vector<double> > getPixelEfficiencies(std::string& det) const {if (det=="bpix") return pixelBPixEfficiency_;else if (det=="fpix") return pixelFPixEfficiency_; else {std::vector<std::vector<double> > empty; return empty;}}

  bool   putGeomFactor (const uint32_t&, double&);
  double  getGeomFactor (const uint32_t&) const;

  bool putPUFactor (const uint32_t&, std::vector<double>&);
  std::vector<double> getPUFactor (const uint32_t&) const;

  bool puttheInstLumiScaleFactor(double&);
  double gettheInstLumiScaleFactor() const;

  bool putPixelEfficiency(std::string&, std::vector<std::vector<double> >&);
  std::vector<std::vector<double> > getPixelEfficiency(std::string&) const;

 private:
  std::map<unsigned int,double> m_GeomFactors;
  std::map<unsigned int,std::vector<double> > m_PUFactors;
  double theInstLumiScaleFactor_;
  std::vector<std::vector<double> > pixelBPixEfficiency_;
  std::vector<std::vector<double> > pixelFPixEfficiency_;

 COND_SERIALIZABLE;
};

#endif
