#import os
import shlex, shutil, getpass
#import subprocess

import FWCore.ParameterSet.Config as cms

process = cms.Process("SiPixelInclusiveBuilder")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))

process.load("Configuration.StandardSequences.MagneticField_cff")

#hptopo
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.autoCond_condDBv2 import autoCond
process.GlobalTag.globaltag = autoCond['run2_design']
print process.GlobalTag.globaltag
process.load("Configuration.StandardSequences.GeometryDB_cff")

process.load("CondTools.SiPixel.SiPixelGainCalibrationService_cfi")

process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.source = cms.Source("EmptyIOVSource",
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    timetype = cms.string('runnumber'),
    interval = cms.uint64(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

#to get the user running the process
user = getpass.getuser()

#try:
#    user = os.environ["USER"]
#except KeyError:
#    user = subprocess.call('whoami')
#    # user = commands.getoutput('whoami')
 
#file = "/tmp/" + user + "/SiPixelDynamicInefficiency.db"
file = "siPixelDynamicInefficiency.db"
sqlfile = "sqlite_file:" + file
print '\n-> Uploading as user %s into file %s, i.e. %s\n' % (user, file, sqlfile)


#standard python libraries instead of spawn processes
shutil.move("siPixelDynamicInefficiency.db", "siPixelDynamicInefficiency_old.db")
#subprocess.call(["/bin/cp", "siPixelDynamicInefficiency.db", file])
#subprocess.call(["/bin/mv", "siPixelDynamicInefficiency.db", "siPixelDynamicInefficiency.db"])

##### DATABASE CONNNECTION AND INPUT TAGS ######
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string('.'),
        connectionRetrialPeriod = cms.untracked.int32(10),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(1),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        connectionTimeOut = cms.untracked.int32(0),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False)
    ),
    timetype = cms.untracked.string('runnumber'),
    connect = cms.string(sqlfile),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('SiPixelDynamicInefficiencyRcd'),
            tag = cms.string('SiPixelDynamicInefficiency_v1')
        ),
                     )
)

###### DYNAMIC INEFFICIENCY OBJECT ######
process.SiPixelDynamicInefficiency = cms.EDAnalyzer("SiPixelDynamicInefficiencyDB",
    #in case of PSet
    thePixelGeomFactors = cms.untracked.VPSet(
      cms.PSet(
        det = cms.string("bpix"),
        factor = cms.double(1)
        ),
      cms.PSet(
        det = cms.string("fpix"),
        factor = cms.double(0.999)
        ),
      ),
    thePixelPUEfficiency = cms.untracked.VPSet(
      cms.PSet(
        det = cms.string("bpix"),
        factor = cms.vdouble(1),
        ),
      cms.PSet(
        det = cms.string("fpix"),
        factor = cms.vdouble(1),
        ),
      ),
    theColGeomFactors = cms.untracked.VPSet(
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(1),
        factor = cms.double(0.978351)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(2),
        factor = cms.double(0.971877)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(3),
        factor = cms.double(0.974283)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(4),
        factor = cms.double(0.969328)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(5),
        factor = cms.double(0.972922)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(6),
        factor = cms.double(0.970964)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(7),
        factor = cms.double(0.975762)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(8),
        factor = cms.double(0.974786)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(9),
        factor = cms.double(0.980244)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(10),
        factor = cms.double(0.978452)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(11),
        factor = cms.double(0.982129)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(12),
        factor = cms.double(0.979737)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(13),
        factor = cms.double(0.984381)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(14),
        factor = cms.double(0.983971)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(15),
        factor = cms.double(0.98186)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(16),
        factor = cms.double(0.983283)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(17),
        factor = cms.double(0.981485)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(18),
        factor = cms.double(0.979753)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(19),
        factor = cms.double(0.980287)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        ladder = cms.uint32(20),
        factor = cms.double(0.975195)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(1),
        factor = cms.double(0.953582)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(2),
        factor = cms.double(0.961242)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(3),
        factor = cms.double(0.999371)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(4),
        factor = cms.double(1.00361)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(5),
        factor = cms.double(1.00361)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(6),
        factor = cms.double(0.999371)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(7),
        factor = cms.double(0.961242)
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        module = cms.uint32(8),
        factor = cms.double(0.953582)
        ),
      cms.PSet(
        det = cms.string("fpix"),
        factor = cms.double(1)
        ),
      ),
    theColPUEfficiency = cms.untracked.VPSet(
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(1),
        factor = cms.vdouble(
          1.0181,
          -2.28345e-07,
          -1.30042e-09
          ),
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(2),
        factor = cms.vdouble(
          1.00648,
          -1.28515e-06,
          -1.85915e-10
          ),
        ),
      cms.PSet(
        det = cms.string("bpix"),
        layer = cms.uint32(3),
        factor = cms.vdouble(
          1.0032,
          -1.96206e-08,
          -1.99009e-10
          ),
        ),
      cms.PSet(
        det = cms.string("fpix"),
        in_out = cms.uint32(1),
        factor= cms.vdouble(
          1.0
          ),
        ),
      cms.PSet(
        det = cms.string("fpix"),
        in_out = cms.uint32(2),
        factor = cms.vdouble(
          1.0
          ),
        )
      ),
    theChipGeomFactors = cms.untracked.VPSet(
      cms.PSet(
        det = cms.string("bpix"),
        factor = cms.double(1)
        ),
      cms.PSet(
        det = cms.string("fpix"),
        factor = cms.double(0.999)
        ),
      ),
    theChipPUEfficiency = cms.untracked.VPSet(
      cms.PSet(
        det = cms.string("bpix"),
        factor = cms.vdouble(1),
        ),
      cms.PSet(
        det = cms.string("fpix"),
        factor = cms.vdouble(1),
        ),
      ),
    theInstLumiScaleFactor = cms.untracked.double(221.95)
    )


process.p = cms.Path(
    process.SiPixelDynamicInefficiency
)

