#include "CondFormats/SiPixelObjects/interface/SiPixelDynamicInefficiency.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

bool SiPixelDynamicInefficiency::putPixelGeomFactor (const uint32_t& detid, double& value){
  std::map<unsigned int,double>::const_iterator id=m_PixelGeomFactors.find(detid);
  if(id!=m_PixelGeomFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency PixelGeomFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_PixelGeomFactors[detid]=value;
  return true;
}

double SiPixelDynamicInefficiency::getPixelGeomFactor (const uint32_t& detid) const  {
  std::map<unsigned int,double>::const_iterator id=m_PixelGeomFactors.find(detid);
  if(id!=m_PixelGeomFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency PixelGeomFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  return 0;
}

bool SiPixelDynamicInefficiency::putColGeomFactor (const uint32_t& detid, double& value){
  std::map<unsigned int,double>::const_iterator id=m_ColGeomFactors.find(detid);
  if(id!=m_ColGeomFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ColGeomFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_ColGeomFactors[detid]=value;
  return true;
}

double SiPixelDynamicInefficiency::getColGeomFactor (const uint32_t& detid) const  {
  std::map<unsigned int,double>::const_iterator id=m_ColGeomFactors.find(detid);
  if(id!=m_ColGeomFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ColGeomFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  return 0;
}

bool SiPixelDynamicInefficiency::putChipGeomFactor (const uint32_t& detid, double& value){
  std::map<unsigned int,double>::const_iterator id=m_ChipGeomFactors.find(detid);
  if(id!=m_ChipGeomFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ChipGeomFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_ChipGeomFactors[detid]=value;
  return true;
}

double SiPixelDynamicInefficiency::getChipGeomFactor (const uint32_t& detid) const  {
  std::map<unsigned int,double>::const_iterator id=m_ChipGeomFactors.find(detid);
  if(id!=m_ChipGeomFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ChipGeomFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  return 0;
}

bool SiPixelDynamicInefficiency::putPixelPUFactor (const uint32_t& detid, std::vector<double>& v_value){
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_PixelPUFactors.find(detid);
  if(id!=m_PixelPUFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency PixelPUFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_PixelPUFactors[detid]=v_value;
  return true;
}

std::vector<double> SiPixelDynamicInefficiency::getPixelPUFactor (const uint32_t& detid) const {
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_PixelPUFactors.find(detid);
  if(id!=m_PixelPUFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency PixelPUFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  std::vector<double> empty;
  return empty;
}

bool SiPixelDynamicInefficiency::putColPUFactor (const uint32_t& detid, std::vector<double>& v_value){
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_ColPUFactors.find(detid);
  if(id!=m_ColPUFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ColPUFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_ColPUFactors[detid]=v_value;
  return true;
}

std::vector<double> SiPixelDynamicInefficiency::getColPUFactor (const uint32_t& detid) const {
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_ColPUFactors.find(detid);
  if(id!=m_ColPUFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ColPUFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  std::vector<double> empty;
  return empty;
}

bool SiPixelDynamicInefficiency::putChipPUFactor (const uint32_t& detid, std::vector<double>& v_value){
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_ChipPUFactors.find(detid);
  if(id!=m_ChipPUFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ChipPUFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_ChipPUFactors[detid]=v_value;
  return true;
}

std::vector<double> SiPixelDynamicInefficiency::getChipPUFactor (const uint32_t& detid) const {
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_ChipPUFactors.find(detid);
  if(id!=m_ChipPUFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency ChipPUFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  std::vector<double> empty;
  return empty;
}

bool SiPixelDynamicInefficiency::puttheInstLumiScaleFactor(double& theInstLumiScaleFactor){
  if (theInstLumiScaleFactor_ == theInstLumiScaleFactor){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency theInstLumiScaleFactor is already stored!" << std::endl;
    return false;
  }
  else {
    theInstLumiScaleFactor_ = theInstLumiScaleFactor;
    return true;
  }
}

double SiPixelDynamicInefficiency::gettheInstLumiScaleFactor() const {
  return theInstLumiScaleFactor_;
}

/*
bool SiPixelDynamicInefficiency::putPixelEfficiency(std::string& det, std::vector<std::vector<double> >& pixelEfficiency){
  if (det == "bpix"){
    if (pixelBPixEfficiency_.size()!=0){
      edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency pixelBPixEfficiency is not empty!" << std::endl;
      return false;
    }
    else {
      pixelBPixEfficiency_ = pixelEfficiency;
      return true;
    }
  }
  else if (det == "fpix"){
    if (pixelFPixEfficiency_.size()!=0){
      edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency pixelFPixEfficiency is not empty!" << std::endl;
      return false;
    }
    else {
      pixelFPixEfficiency_ = pixelEfficiency;
      return true;
    }
  }
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency pixelEfficiency detector part is neither BPix nor FPix!" << std::endl;
    return false;
  }
}

std::vector<std::vector<double> > SiPixelDynamicInefficiency::getPixelEfficiency(std::string& det) const{
  if (det == "bpix"){
    if (pixelBPixEfficiency_.size()==0){
      edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency pixelBPixEfficiency is empty!" << std::endl;
      std::vector<std::vector<double> > empty;
      return empty;
    }
    else return pixelBPixEfficiency_;
  }
  else if (det == "fpix"){
    if (pixelFPixEfficiency_.size()==0){
      edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency pixelFPixEfficiency is empty!" << std::endl;
      std::vector<std::vector<double> > empty;
      return empty;
    }
    else return pixelFPixEfficiency_;
  }
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency pixelEfficiency detector part is neither BPix nor FPix!" << std::endl;
    std::vector<std::vector<double> > empty;
    return empty;
  }
}
*/
