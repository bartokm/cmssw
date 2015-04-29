#include "CondFormats/SiPixelObjects/interface/SiPixelDynamicInefficiency.h"
#include "CondFormats/DataRecord/interface/SiPixelDynamicInefficiencyRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "CondTools/SiPixel/test/SiPixelDynamicInefficiencyReader.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include <iostream>
#include <stdio.h>
#include <sys/time.h>


using namespace cms;

SiPixelDynamicInefficiencyReader::SiPixelDynamicInefficiencyReader( const edm::ParameterSet& iConfig ):
  printdebug_(iConfig.getUntrackedParameter<bool>("printDebug",false))
{
  int i=0;
  thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix1");
  thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix2");
  thePixelColEfficiency[i++] = iConfig.getParameter<double>("thePixelColEfficiency_BPix3");
  i=0;
  thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix1");
  thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix2");
  thePixelEfficiency[i++] = iConfig.getParameter<double>("thePixelEfficiency_BPix3");
  i=0;
  thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix1");
  thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix2");
  thePixelChipEfficiency[i++] = iConfig.getParameter<double>("thePixelChipEfficiency_BPix3");
  i=0;
  theLadderEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theLadderEfficiency_BPix1");
  theLadderEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theLadderEfficiency_BPix2");
  theLadderEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theLadderEfficiency_BPix3");
  if ( (theLadderEfficiency_BPix[0].size()!=20) || (theLadderEfficiency_BPix[1].size()!=32) ||
        (theLadderEfficiency_BPix[2].size()!=44))  
    throw cms::Exception("Configuration") << "Wrong ladder number in efficiency config!";
    //		     
  i=0;
  theModuleEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theModuleEfficiency_BPix1");
  theModuleEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theModuleEfficiency_BPix2");
  theModuleEfficiency_BPix[i++] = iConfig.getParameter<std::vector<double> >("theModuleEfficiency_BPix3");
  if ((theModuleEfficiency_BPix[0].size()!=4) || (theModuleEfficiency_BPix[1].size()!=4) ||
        (theModuleEfficiency_BPix[2].size()!=4))  
    throw cms::Exception("Configuration") << "Wrong module number in efficiency config!";
  //
  i=0;		     
  thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix1");
  thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix2");
  thePUEfficiency[i++] = iConfig.getParameter<std::vector<double> >("thePUEfficiency_BPix3");		    		    
}

SiPixelDynamicInefficiencyReader::~SiPixelDynamicInefficiencyReader(){}

void SiPixelDynamicInefficiencyReader::analyze( const edm::Event& e, const edm::EventSetup& iSetup){
  edm::ESHandle<SiPixelDynamicInefficiency> SiPixelDynamicInefficiency_; 
  iSetup.get<SiPixelDynamicInefficiencyRcd>().get(SiPixelDynamicInefficiency_);
  edm::LogInfo("SiPixelDynamicInefficiencyReader") << "[SiPixelDynamicInefficiencyReader::analyze] End Reading SiPixelDynamicInefficiency" << std::endl;

  std::map<unsigned int,double> map_geomfactor = SiPixelDynamicInefficiency_->getGeomFactors();
  std::map<unsigned int,std::vector<double> > map_pufactor = SiPixelDynamicInefficiency_->getPUFactors();
  std::map<unsigned int,double>::const_iterator it_geom;
  std::map<unsigned int,std::vector<double> >::const_iterator it_pu;
  for (it_geom=map_geomfactor.begin();it_geom!=map_geomfactor.end();it_geom++)
  {
    std::cout  << "geom detid " << it_geom->first << " \t" << " factor "<<it_geom->second<<std::endl;
  }
  /*for (unsigned int i = 0;i<theLadderEfficiency_BPix[0].size();i++)
  {
    std::cout  << "conf factor " <<theLadderEfficiency_BPix[0][i]<<std::endl;
  }
  for (unsigned int i = 0;i<theModuleEfficiency_BPix[0].size();i++)
  {
    std::cout  << "conf factor " <<theModuleEfficiency_BPix[0][i]<<std::endl;
  }
  */
  for (it_pu=map_pufactor.begin();it_pu!=map_pufactor.end();it_pu++)
  {
    std::cout  << "pu detid " << it_pu->first << " \t" << " Size of vector "<<it_pu->second.size()<<" elements:";
    if (it_pu->second.size()>1) {
      for (unsigned int i=0;i<it_pu->second.size();i++) {
        std::cout<<" "<<it_pu->second.at(i);
      }
      std::cout<<std::endl;
    }
    else {
      std::cout<<" "<<it_pu->second.at(0)<<std::endl;
    }
  }
  double theInstLumiScaleFactor = SiPixelDynamicInefficiency_->gettheInstLumiScaleFactors();
  std::cout<<"theInstLumiScaleFactor "<<theInstLumiScaleFactor<<std::endl;
  std::string bpix_fpix="bpix";
  std::string efficiencies[3];
  efficiencies[0]+="pixel";
  efficiencies[1]+="double column";
  efficiencies[2]+="chip";
  for (unsigned int i=0;i<SiPixelDynamicInefficiency_->getPixelEfficiencies(bpix_fpix).size();i++){
    std::cout<<"PixelEfficiencies bpix "<<efficiencies[i]<<" ";
    for (unsigned int j=0;j<SiPixelDynamicInefficiency_->getPixelEfficiencies(bpix_fpix).at(i).size();j++){
      std::cout<<SiPixelDynamicInefficiency_->getPixelEfficiencies(bpix_fpix)[i][j]<<" ";
    }
    std::cout<<std::endl;
  }
  bpix_fpix="fpix";
  for (unsigned int i=0;i<SiPixelDynamicInefficiency_->getPixelEfficiencies(bpix_fpix).size();i++){
    std::cout<<"PixelEfficiencies fpix "<<efficiencies[i]<<" ";
    for (unsigned int j=0;j<SiPixelDynamicInefficiency_->getPixelEfficiencies(bpix_fpix).at(i).size();j++){
      std::cout<<SiPixelDynamicInefficiency_->getPixelEfficiencies(bpix_fpix)[i][j]<<" ";
    }
    std::cout<<std::endl;
  }

  edm::ESHandle<TrackerTopology> tTopoHandle;
  iSetup.get<IdealGeometryRecord>().get(tTopoHandle);
  const TrackerTopology* const tTopo = tTopoHandle.product();

	edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );
  edm::LogInfo("SiPixelDynamicInefficiency (old)") <<" There are "<<pDD->detUnits().size() <<" detectors (old)"<<std::endl;
      
  const size_t pu_det = map_pufactor.size();
  double _pu_scale[pu_det];

  for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
    if( dynamic_cast<PixelGeomDetUnit const*>((*it))==0) continue;
    const DetId detid=(*it)->geographicalId();
    double scale_db=1;

    std::vector<std::pair<unsigned int, unsigned int> > startbitsMasks;
    tTopo->getBits(detid, startbitsMasks);

    for (it_geom=map_geomfactor.begin();it_geom!=map_geomfactor.end();it_geom++){
      //if ((it_geom->first & detid.rawId()) != detid.rawId()) continue;
      size_t __i=0;
      for (; __i<startbitsMasks.size();__i++) {
        uint32_t detid_field=(detid.rawId()>>startbitsMasks[__i].first)&startbitsMasks[__i].second;
        uint32_t mask_field=(it_geom->first>>startbitsMasks[__i].first)&startbitsMasks[__i].second;
        if (detid_field!=mask_field && mask_field!=startbitsMasks[__i].second) break;
      }
      if (__i!=startbitsMasks.size()) continue;
      scale_db *= it_geom->second;
      //printf("mask %x detid %x & %x \t factor %f, scaledb %f\n",it_geom->first,detid.rawId(),it_geom->first & detid.rawId(),it_geom->second,scale_db);
    }
    for (it_pu=map_pufactor.begin();it_pu!=map_pufactor.end();it_pu++){
      if ((it_pu->first & detid.rawId()) != detid.rawId()) continue;
      double instlumi = 30*theInstLumiScaleFactor;
      double instlumi_pow=1.;
      for (size_t i=0; i<pu_det; i++){
        _pu_scale[i] = 0;
        for  (size_t j=0; j<it_pu->second.size(); j++){
          _pu_scale[i]+=instlumi_pow*it_pu->second[j];
          instlumi_pow*=instlumi;
        }
      }
    }
    if(detid.subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel)) {
      int layerIndex=tTopo->pxbLayer(detid.rawId());
      //pixelEfficiency  = thePixelEfficiency[layerIndex-1];
      double columnEfficiency = thePixelColEfficiency[layerIndex-1];
      //chipEfficiency   = thePixelChipEfficiency[layerIndex-1];
      //std::cout <<"Using BPix columnEfficiency = "<<columnEfficiency<< " for layer = "<<layerIndex <<"\n";
      int ladder=tTopo->pxbLadder(detid.rawId());
      int module=tTopo->pxbModule(detid.rawId());
      if (module<=4) module=5-module;
      else module-=4;
      
      //columnEfficiency *= theLadderEfficiency_BPix[layerIndex-1][ladder-1]*theModuleEfficiency_BPix[layerIndex-1][module-1]*_pu_scale[layerIndex-1];
      columnEfficiency *= theLadderEfficiency_BPix[layerIndex-1][ladder-1]*theModuleEfficiency_BPix[layerIndex-1][module-1];
      if (_pu_scale[0]==0 || scale_db == 1) continue;
      //std::cout<<"scale_db\t"<<scale_db<<" pu scale "<<_pu_scale[0]<<" product: "<<_pu_scale[0]*scale_db<<std::endl;
      //std::cout<<"config  \t"<<columnEfficiency<<std::endl;
      if (scale_db == columnEfficiency) printf("Config match, detid %x\tfactor %f\n",detid.rawId(),columnEfficiency);
      else printf("Config mismatch!")
    }
  }
}
