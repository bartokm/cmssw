#include "CondFormats/SiPixelObjects/interface/SiPixelDynamicInefficiency.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

bool SiPixelDynamicInefficiency::putGeomFactor (const uint32_t& detid, double& value){
  std::map<unsigned int,double>::const_iterator id=m_GeomFactors.find(detid);
  if(id!=m_GeomFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency GeomFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_GeomFactors[detid]=value;
  return true;
}
double SiPixelDynamicInefficiency::getGeomFactor (const uint32_t& detid) const  {
  std::map<unsigned int,double>::const_iterator id=m_GeomFactors.find(detid);
  if(id!=m_GeomFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency GeomFactor for DetID " << detid << " is not stored" << std::endl; 
  } 
  return 0;
}
bool SiPixelDynamicInefficiency::putPUFactor (const uint32_t& detid, std::vector<double>& v_value){
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_PUFactors.find(detid);
  if(id!=m_PUFactors.end()){
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency PUFactor for DetID " << detid << " is already stored. Skippig this put" << std::endl;
    return false;
  }
  else m_PUFactors[detid]=v_value;
  return true;
}


std::vector<double> SiPixelDynamicInefficiency::getPUFactor (const uint32_t& detid) const {
  std::map<unsigned int,std::vector<double> >::const_iterator id=m_PUFactors.find(detid);
  if(id!=m_PUFactors.end()) return id->second;
  else {
    edm::LogError("SiPixelDynamicInefficiency") << "SiPixelDynamicInefficiency PUFactor for DetID " << detid << " is not stored" << std::endl; 
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
