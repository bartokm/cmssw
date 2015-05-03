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

  inline void putPixelGeomFactors (std::map<unsigned int,double>& PixelGeomFactors){m_PixelGeomFactors=PixelGeomFactors;} 
  inline const std::map<unsigned int,double>&  getPixelGeomFactors () const {return m_PixelGeomFactors;}

  inline void putPixelPUFactors (std::map<unsigned int,std::vector<double> >& PixelPUFactors){m_PixelPUFactors=PixelPUFactors;} 
  inline const std::map<unsigned int,std::vector<double> >&  getPixelPUFactors () const {return m_PixelPUFactors;}

  inline void putColGeomFactors (std::map<unsigned int,double>& ColGeomFactors){m_ColGeomFactors=ColGeomFactors;} 
  inline const std::map<unsigned int,double>&  getColGeomFactors () const {return m_ColGeomFactors;}

  inline void putColPUFactors (std::map<unsigned int,std::vector<double> >& ColPUFactors){m_ColPUFactors=ColPUFactors;} 
  inline const std::map<unsigned int,std::vector<double> >&  getColPUFactors () const {return m_ColPUFactors;}

  inline void putChipGeomFactors (std::map<unsigned int,double>& ChipGeomFactors){m_ChipGeomFactors=ChipGeomFactors;} 
  inline const std::map<unsigned int,double>&  getChipGeomFactors () const {return m_ChipGeomFactors;}

  inline void putChipPUFactors (std::map<unsigned int,std::vector<double> >& ChipPUFactors){m_ChipPUFactors=ChipPUFactors;} 
  inline const std::map<unsigned int,std::vector<double> >&  getChipPUFactors () const {return m_ChipPUFactors;}

  inline void puttheInstLumiScaleFactors(double& InstLumiScaleFactor){theInstLumiScaleFactor_=InstLumiScaleFactor;}
  inline const double gettheInstLumiScaleFactors() const {return theInstLumiScaleFactor_;}

  //inline void putPixelEfficiencies(bool& bpix_fpix, std::vector<std::vector<double> >&);
  //inline const std::vector<std::vector<double> > getPixelEfficiencies(std::string& det) const {if (det=="bpix") return pixelBPixEfficiency_;else if (det=="fpix") return pixelFPixEfficiency_; else {std::vector<std::vector<double> > empty; return empty;}}

  bool   putPixelGeomFactor (const uint32_t&, double&);
  double  getPixelGeomFactor (const uint32_t&) const;

  bool putPixelPUFactor (const uint32_t&, std::vector<double>&);
  std::vector<double> getPixelPUFactor (const uint32_t&) const;

  bool   putColGeomFactor (const uint32_t&, double&);
  double  getColGeomFactor (const uint32_t&) const;

  bool putColPUFactor (const uint32_t&, std::vector<double>&);
  std::vector<double> getColPUFactor (const uint32_t&) const;

  bool   putChipGeomFactor (const uint32_t&, double&);
  double  getChipGeomFactor (const uint32_t&) const;

  bool putChipPUFactor (const uint32_t&, std::vector<double>&);
  std::vector<double> getChipPUFactor (const uint32_t&) const;

  bool puttheInstLumiScaleFactor(double&);
  double gettheInstLumiScaleFactor() const;

  bool putPixelEfficiency(std::string&, std::vector<std::vector<double> >&);
  std::vector<std::vector<double> > getPixelEfficiency(std::string&) const;

 private:
  std::map<unsigned int,double> m_PixelGeomFactors;
  std::map<unsigned int,std::vector<double> > m_PixelPUFactors;
  std::map<unsigned int,double> m_ColGeomFactors;
  std::map<unsigned int,std::vector<double> > m_ColPUFactors;
  std::map<unsigned int,double> m_ChipGeomFactors;
  std::map<unsigned int,std::vector<double> > m_ChipPUFactors;
  double theInstLumiScaleFactor_;
  //std::vector<std::vector<double> > pixelBPixEfficiency_;
  //std::vector<std::vector<double> > pixelFPixEfficiency_;

 COND_SERIALIZABLE;
};

#endif
