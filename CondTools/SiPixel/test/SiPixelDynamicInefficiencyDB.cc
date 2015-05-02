#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondTools/SiPixel/test/SiPixelDynamicInefficiencyDB.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelDynamicInefficiency.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementError.h"
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

using namespace std;
using namespace edm;

  //Constructor

SiPixelDynamicInefficiencyDB::SiPixelDynamicInefficiencyDB(edm::ParameterSet const& conf) : 
  conf_(conf){
	recordName_ = conf_.getUntrackedParameter<std::string>("record","SiPixelDynamicInefficiencyRcd");

       theGeomFactors_ = conf_.getUntrackedParameter<Parameters>("theGeomFactors");
       thePUEfficiency_ = conf_.getUntrackedParameter<Parameters>("thePUEfficiency");
       theInstLumiScaleFactor_ = conf_.getUntrackedParameter<double>("theInstLumiScaleFactor");
       thePixelEfficiency_ = conf_.getUntrackedParameter<Parameters>("thePixelEfficiency");
       thePixelColEfficiency_ = conf_.getUntrackedParameter<Parameters>("thePixelColEfficiency");
       thePixelChipEfficiency_ = conf_.getUntrackedParameter<Parameters>("thePixelChipEfficiency");
}

  //BeginJob

void SiPixelDynamicInefficiencyDB::beginJob(){
  
}

// Virtual destructor needed.

SiPixelDynamicInefficiencyDB::~SiPixelDynamicInefficiencyDB() {  

}  

// Analyzer: Functions that gets called by framework every event

void SiPixelDynamicInefficiencyDB::analyze(const edm::Event& e, const edm::EventSetup& es)
{

	SiPixelDynamicInefficiency* DynamicInefficiency = new SiPixelDynamicInefficiency();


        //Retrieve tracker topology from geometry
        edm::ESHandle<TrackerTopology> tTopoHandle;
        es.get<IdealGeometryRecord>().get(tTopoHandle);
        const TrackerTopology* const tTopo = tTopoHandle.product();
//DetID tesztelo
/*
  uint32_t layer=std::numeric_limits<uint32_t>::max();
  //uint32_t ladder=std::numeric_limits<uint32_t>::max();
  uint32_t ladder=1;
  uint32_t module=1;
  DetId testID=tTopo->pxbDetId(layer,ladder,module);
  bitset<32> temp (testID.rawId());
  std::cout<<"haho "<<temp<<std::endl;

  uint32_t side=std::numeric_limits<uint32_t>::max();
  uint32_t disk=1;
  uint32_t blade=1;
  uint32_t panel=1;
  uint32_t modulef=1;
  DetId testfID=tTopo->pxfDetId(side,disk,blade,panel,modulef);
  bitset<32> tempf (testfID.rawId());
  std::cout<<"haho "<<tempf<<std::endl;
*/

  uint32_t layer, LAYER = 0;
  uint32_t ladder, LADDER = 0;
  uint32_t module, MODULE = 0;
  uint32_t side, SIDE = 0;
  uint32_t disk, DISK = 0;
  uint32_t blade, BLADE = 0;
  uint32_t panel, PANEL = 0;

  for(Parameters::iterator it = theGeomFactors_.begin(); it != theGeomFactors_.end(); ++it) {
    string det = it->getParameter<string>("det");
    it->exists("layer") ? layer = it->getParameter<unsigned int>("layer") : layer = LAYER;
    it->exists("ladder") ? ladder = it->getParameter<unsigned int>("ladder") : ladder = LADDER;
    it->exists("module") ? module = it->getParameter<unsigned int>("module") : module = MODULE;
    it->exists("side") ? side = it->getParameter<unsigned int>("side") : side = SIDE;
    it->exists("disk") ? disk = it->getParameter<unsigned int>("disk") : disk = DISK;
    it->exists("blade") ? blade = it->getParameter<unsigned int>("blade") : blade = BLADE;
    it->exists("panel") ? panel = it->getParameter<unsigned int>("panel") : panel = PANEL;
    double factor = it->getParameter<double>("factor");
    //std::cout<<"layer, ladder, module "<<layer<<", "<<ladder<<", "<<module<<std::endl;
    if (det == "bpix") {
      DetId detID=tTopo->pxbDetId(layer,ladder,module);
      bitset<32> temp (detID.rawId());
      //std::cout<<"Geom BPix detID "<<temp<<std::endl;
      DynamicInefficiency->putGeomFactor(detID.rawId(),factor);
    }
    else if (det == "fpix") {
      DetId detID=tTopo->pxfDetId(side, disk, blade, panel, module);
      bitset<32> temp (detID.rawId());
      //std::cout<<"Geom FPix detID "<<temp<<std::endl;
      DynamicInefficiency->putGeomFactor(detID.rawId(),factor);
    }
    else edm::LogError("SiPixelDynamicInefficiencyDB")<<"SiPixelDynamicInefficiencyDB input detector part is neither bpix nor fpix"<<std::endl;
  }

  for(Parameters::iterator it = thePUEfficiency_.begin(); it != thePUEfficiency_.end(); ++it) {
    string det = it->getParameter<string>("det");
    it->exists("layer") ? layer = it->getParameter<unsigned int>("layer") : layer = LAYER;
    it->exists("ladder") ? ladder = it->getParameter<unsigned int>("ladder") : ladder = LADDER;
    it->exists("module") ? module = it->getParameter<unsigned int>("module") : module = MODULE;
    it->exists("side") ? side = it->getParameter<unsigned int>("side") : side = SIDE;
    it->exists("disk") ? disk = it->getParameter<unsigned int>("disk") : disk = DISK;
    it->exists("blade") ? blade = it->getParameter<unsigned int>("blade") : blade = BLADE;
    it->exists("panel") ? panel = it->getParameter<unsigned int>("panel") : panel = PANEL;
    std::vector<double> factor = it->getParameter<std::vector<double> >("factor");
    if (det == "bpix") {
      DetId detID=tTopo->pxbDetId(layer,ladder,module);
      bitset<32> temp (detID.rawId());
      //std::cout<<"PU BPix detID "<<temp<<std::endl;
      DynamicInefficiency->putPUFactor(detID.rawId(),factor);
    }

  }
//other efficiencies
    DynamicInefficiency->puttheInstLumiScaleFactor(theInstLumiScaleFactor_);
    std::vector<std::vector<double> > v_PixelBPixEfficiency;
    std::vector<std::vector<double> > v_PixelFPixEfficiency;
    for(Parameters::iterator it = thePixelEfficiency_.begin(); it != thePixelEfficiency_.end(); ++it) {
      string det = it->getParameter<string>("det");
      std::vector<double> factor = it->getParameter<std::vector<double> >("factor");
      if (det == "bpix"){
        v_PixelBPixEfficiency.push_back(factor);
      }
      else if (det == "fpix"){
        v_PixelFPixEfficiency.push_back(factor);
      }
      else edm::LogError("SiPixelDynamicInefficiencyDB")<<"SiPixelDynamicInefficiencyDB input detector part is neither bpix nor fpix"<<std::endl;
    }
    for(Parameters::iterator it = thePixelColEfficiency_.begin(); it != thePixelColEfficiency_.end(); ++it) {
      string det = it->getParameter<string>("det");
      std::vector<double> factor = it->getParameter<std::vector<double> >("factor");
      if (det == "bpix"){
        v_PixelBPixEfficiency.push_back(factor);
      }
      else if (det == "fpix"){
        v_PixelFPixEfficiency.push_back(factor);
      }
      else edm::LogError("SiPixelDynamicInefficiencyDB")<<"SiPixelDynamicInefficiencyDB input detector part is neither bpix nor fpix"<<std::endl;
    }
    for(Parameters::iterator it = thePixelChipEfficiency_.begin(); it != thePixelChipEfficiency_.end(); ++it) {
      string det = it->getParameter<string>("det");
      std::vector<double> factor = it->getParameter<std::vector<double> >("factor");
      if (det == "bpix"){
        v_PixelBPixEfficiency.push_back(factor);
      }
      else if (det == "fpix"){
        v_PixelFPixEfficiency.push_back(factor);
      }
      else edm::LogError("SiPixelDynamicInefficiencyDB")<<"SiPixelDynamicInefficiencyDB input detector part is neither bpix nor fpix"<<std::endl;
    }
    string bpix_fpix="bpix";
    DynamicInefficiency->putPixelEfficiency(bpix_fpix,v_PixelBPixEfficiency);
    bpix_fpix="fpix";
    DynamicInefficiency->putPixelEfficiency(bpix_fpix,v_PixelFPixEfficiency);

	edm::Service<cond::service::PoolDBOutputService> mydbservice;
	if( mydbservice.isAvailable() ){
		try{
			if( mydbservice->isNewTagRequest(recordName_) ){
				mydbservice->createNewIOV<SiPixelDynamicInefficiency>(DynamicInefficiency,
									       mydbservice->beginOfTime(),
									       mydbservice->endOfTime(),
									       recordName_);
			} else {
				mydbservice->appendSinceTime<SiPixelDynamicInefficiency>(DynamicInefficiency,
										  mydbservice->currentTime(),
										  recordName_);
			}
		}catch(const cond::Exception& er){
			edm::LogError("SiPixelDynamicInefficiencyDB")<<er.what()<<std::endl;
		}catch(const std::exception& er){
			edm::LogError("SiPixelDynamicInefficiencyDB")<<"caught std::exception "<<er.what()<<std::endl;
		}catch(...){
			edm::LogError("SiPixelDynamicInefficiencyDB")<<"Funny error"<<std::endl;
		}
	}else{
		edm::LogError("SiPixelDynamicInefficiencyDB")<<"Service is unavailable"<<std::endl;
	}
   
}
void SiPixelDynamicInefficiencyDB::endJob(){
}
