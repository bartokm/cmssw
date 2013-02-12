# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms

from TauAnalysis.MCEmbeddingTools.rerunParticleFlow import rerunParticleFlow

import os

def customise(process):

  # Define configuration parameter default values
  from TauAnalysis.MCEmbeddingTools.setDefaults import setDefaults
  setDefaults(process)
   
  inputProcess = process.customization_options.inputProcessRECO.value()
  print "Input process set to '%s'" % inputProcess
  
  process._Process__name = "EmbeddedRECO"

  # DQM store output for muon acceptance histograms
  process.DQMStore = cms.Service("DQMStore")

  if process.customization_options.isMC.value():
    print "Input is MC"
    
    # select generator level muons
    process.genMuonsFromZs = cms.EDProducer("GenParticlesFromZsSelectorForMCEmbedding",
      src = cms.InputTag("genParticles", "", process.customization_options.inputProcessSIM.value()),
      pdgIdsMothers = cms.vint32(23, 22),
      pdgIdsDaughters = cms.vint32(13),
      maxDaughters = cms.int32(2),
      minDaughters = cms.int32(2),
      before_or_afterFSR = cms.string("afterFSR"),
      verbosity = cms.int32(0)
    )
    process.ProductionFilterSequence.replace(process.cleanedPFCandidates, process.genMuonsFromZs*process.cleanedPFCandidates)
    process.ProductionFilterSequence.replace(process.cleanedInnerTracks, process.genMuonsFromZs*process.cleanedInnerTracks)

    # produce generator level MET (= sum of neutrino momenta)
    process.genMetTrue = cms.EDProducer("MixedGenMEtProducer",
      srcGenParticles1 = cms.InputTag("genParticles", "", process.customization_options.inputProcessSIM.value()),
      srcGenParticles2 = cms.InputTag("genParticles"),
      srcGenRemovedMuons = cms.InputTag("genMuonsFromZs")
    )
    for p in process.paths:
      pth = getattr(process, p)
      if "genParticles" in pth.moduleNames():
        pth.replace(process.genParticles, process.genParticles*process.genMetTrue)
  else:
    print "Input is Data"
  
  # update InputTags defined in PFEmbeddingSource_cff.py
  print "Setting collection of Z->mumu candidates to '%s'" % process.customization_options.ZmumuCollection.getModuleLabel()
  if not (hasattr(process, "cleanedPFCandidates") and hasattr(process, "cleanedInnerTracks")):
    process.load("TauAnalysis.MCEmbeddingTools.PFEmbeddingSource_cff")
  if process.customization_options.replaceGenOrRecMuonMomenta.value() == 'gen':
    print "Taking momenta of generated tau leptons from generator level muons"
    process.generator.src = cms.InputTag('genMuonsFromZs')
    process.cleanedPFCandidates.selectedMuons = cms.InputTag('genMuonsFromZs')
    process.cleanedInnerTracks.selectedMuons = cms.InputTag('genMuonsFromZs')
  elif process.customization_options.replaceGenOrRecMuonMomenta.value() == 'rec':
    print "Taking momenta of generated tau leptons from reconstructed muons"
    process.generator.src = process.customization_options.ZmumuCollection
    process.cleanedPFCandidates.selectedMuons = process.customization_options.ZmumuCollection
    process.cleanedInnerTracks.selectedMuons = process.customization_options.ZmumuCollection
  else:
    raise ValueError("Invalid Configuration parameter 'replaceGenOrRecMuonMomenta' = %s !!" % process.customization_options.replaceGenOrRecMuonMomenta.value())
  # update option for removing tracks/charged PFCandidates matched to reconstructed muon
  if process.customization_options.muonTrackCleaningMode.value() == 1:
    process.cleanedPFCandidates.removeDuplicates = cms.bool(False)
    process.cleanedInnerTracks.removeDuplicates = cms.bool(False)
  elif process.customization_options.muonTrackCleaningMode.value() == 2:
    process.cleanedPFCandidates.removeDuplicates = cms.bool(True)
    process.cleanedInnerTracks.removeDuplicates = cms.bool(True)
  else:
    raise ValueError("Invalid Configuration parameter 'muonTrackCleaningMode' = %i !!" % process.customization_options.muonTrackCleaningMode.value())

  try:
    outputModule = process.output
  except:
    pass
  try:
    outputModule = getattr(process, str(getattr(process, list(process.endpaths)[-1])))
  except:
    pass

  print "Changing event-content to AODSIM + Z->mumu candidates"
  outputModule.outputCommands = cms.untracked.vstring("drop *")
  outputModule.outputCommands.extend(process.AODSIMEventContent.outputCommands)

  # get rid of second "drop *"
  index = 0
  for item in outputModule.outputCommands[:]:
    if item == "drop *" and index != 0:
      del outputModule.outputCommands[index]
      index -= 1
    index += 1  

  # keep collections of generator level muons
  outputModule.outputCommands.extend([
    'keep *_genMuonsFromZs_*_*'
  ])

  # keep collections of reconstructed muons
  outputModule.outputCommands.extend([
    'keep *_goodMuons_*_*',
    'keep *_goodMuonsPFIso_*_*',
    'keep *_highestPtMuPlus_*_*',
    'keep *_highestPtMuMinus_*_*',
    'keep *_highestPtMuPlusPFIso_*_*',
    'keep *_highestPtMuMinusPFIso_*_*'    
  ])

  # keep collections of reconstructed Z -> mumu candidates
  # (with different muon isolation criteria applied)
  outputModule.outputCommands.extend([
    'keep *_goldenZmumuCandidatesGe0IsoMuons_*_*',
    'keep *_goldenZmumuCandidatesGe1IsoMuons_*_*',
    'keep *_goldenZmumuCandidatesGe2IsoMuons_*_*',
    'keep TH2DMEtoEDM_MEtoEDMConverter_*_*'
  ])

  # keep flag indicating whether event passes or fails
  #  o Z -> mumu event selection
  #  o muon -> muon + photon radiation filter
  outputModule.outputCommands.extend([
    'keep *_goldenZmumuFilterResult_*_*',
    'keep *_muonRadiationFilterResult_*_*'
  ])

  # CV: keep HepMCProduct for embedded event,
  #     in order to run Validation/EventGenerator/python/TauValidation_cfi.py
  #     for control plots of tau polarization and decay mode information
  outputModule.outputCommands.extend([
    'keep *HepMCProduct_generator_*_*'
  ])

  # keep distance of muons traversed in ECAL and HCAL,
  # expected and removed muon energy deposits
  outputModule.outputCommands.extend([
    'keep *_muonCaloEnergyDepositsByDistance_totalDistanceMuPlus_*',
    'keep *_muonCaloEnergyDepositsByDistance_totalEnergyDepositMuPlus_*',
    'keep *_muonCaloEnergyDepositsByDistance_totalDistanceMuMinus_*',
    'keep *_muonCaloEnergyDepositsByDistance_totalEnergyDepositMuMinus_*',
    'keep *_castorreco_removedEnergyMuMinus*_*',
    'keep *_castorreco_removedEnergyMuPlus*_*',
    'keep *_hfreco_removedEnergyMuMinus*_*',
    'keep *_hfreco_removedEnergyMuPlus*_*',
    'keep *_ecalPreshowerRecHit_removedEnergyMuMinus*_*',
    'keep *_ecalPreshowerRecHit_removedEnergyMuPlus*_*', 
    'keep *_ecalRecHit_removedEnergyMuMinus*_*',
    'keep *_ecalRecHit_removedEnergyMuPlus*_*',
    'keep *_hbhereco_removedEnergyMuMinus*_*',
    'keep *_hbhereco_removedEnergyMuPlus*_*',
    'keep *_horeco_removedEnergyMuMinus*_*',
    'keep *_horeco_removedEnergyMuPlus*_*',
  ])
  
  # replace HLT process name
  # (needed for certain reprocessed Monte Carlo samples)
  hltProcessName = "HLT"
  try:
    hltProcessName = __HLT__
  except:
    pass	
  try:
    process.dimuonsHLTFilter.TriggerResultsTag.processName = hltProcessName
    process.goodZToMuMuAtLeast1HLT.TrigTag.processName = hltProcessName
    process.goodZToMuMuAtLeast1HLT.triggerEvent.processName = hltProcessName
    process.hltTrigReport,HLTriggerResults.processName = hltProcessName
  except:
    pass

  # disable L1GtTrigReport module
  # (not used for anything yet, just prints error for every single event)
  process.HLTAnalyzerEndpath.remove(process.hltL1GtTrigReport)
  
  # apply configuration parameters
  print "Setting mdtau to %i" % process.customization_options.mdtau.value()
  process.generator.Ztautau.TauolaOptions.InputCards.mdtau = process.customization_options.mdtau
  if hasattr(process.generator, "ParticleGun"):
    process.generator.ParticleGun.ExternalDecays.Tauola.InputCards.mdtau = process.customization_options.mdtau 
  
  print "Setting minVisibleTransverseMomentum to '%s'" % process.customization_options.minVisibleTransverseMomentum.value()
  process.generator.Ztautau.minVisibleTransverseMomentum = process.customization_options.minVisibleTransverseMomentum

  print "Setting transformationMode to %i" % process.customization_options.transformationMode.value()
  process.generator.Ztautau.transformationMode = process.customization_options.transformationMode

  print "Setting rfRotationAngle to %1.0f" % process.customization_options.rfRotationAngle.value()
  process.generator.Ztautau.rfRotationAngle = process.customization_options.rfRotationAngle
  
  if process.customization_options.overrideBeamSpot.value():
    bs = cms.string("BeamSpotObjects_2009_LumiBased_SigmaZ_v26_offline") # 52x data
    ##bs = cms.string("BeamSpotObjects_2009_LumiBased_SigmaZ_v21_offline") # 42x data
    process.GlobalTag.toGet = cms.VPSet(
      cms.PSet(
        record = cms.string("BeamSpotObjectsRcd"),
        tag = bs,
        connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BEAMSPOT")
      )
    )
    print "BeamSpot in globaltag set to '%s'" % bs 
  else:
    print "BeamSpot in globaltag not changed"

  # CV: disable gen. vertex smearing
  #    (embed tau leptons exactly at Z->mumu event vertex)
  print "Disabling gen. vertex smearing"
  process.VtxSmeared = cms.EDProducer("FlatEvtVtxGenerator", 
    MaxZ = cms.double(0.0),
    MaxX = cms.double(0.0),
    MaxY = cms.double(0.0),
    MinX = cms.double(0.0),
    MinY = cms.double(0.0),
    MinZ = cms.double(0.0),
    TimeOffset = cms.double(0.0),
    src = cms.InputTag("generator")
  )  

  if process.customization_options.useJson.value():
    print "Enabling event selection by JSON file"
    import PhysicsTools.PythonAnalysis.LumiList as LumiList
    import FWCore.ParameterSet.Types as CfgTypes
    myLumis = LumiList.LumiList(filename = 'my.json').getCMSSWString().split(',')
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)

  #----------------------------------------------------------------------------------------------------------------------
  # CV: need to rerun particle-flow algorithm in order to create links between PFMuon -> PFBlocks -> PFClusters -> PFRecHits
  #    (configure particle-flow sequence before overwriting modules in order to mix collections
  #     of objects reconstructed and Z -> mu+ mu- event with simulated tau decay products)
  if process.customization_options.embeddingMode.value() == "RH" and process.customization_options.cleaningMode == 'PF':
    rerunParticleFlow(process, inputProcess)
    process.ProductionFilterSequence += process.rerunParticleFlowSequenceForPFMuonCleaning
  #----------------------------------------------------------------------------------------------------------------------  

  # mix "general" Track collection
  process.generalTracksORG = process.generalTracks.clone()
  process.generalTracks = cms.EDProducer("RecoTracksMixer",
      trackCol1 = cms.InputTag("generalTracksORG", "", "EmbeddedRECO"),
      trackCol2 = cms.InputTag("cleanedInnerTracks")
  )
  
  for p in process.paths:
    pth = getattr(process,p)
    if "generalTracks" in pth.moduleNames():
      pth.replace(process.generalTracks, process.generalTracksORG*process.generalTracks)

  #----------------------------------------------------------------------------------------------------------------------
  # CV/TF: mixing of std::vector<Trajectory> from Zmumu event and embedded tau decay products does not work yet.
  #        For the time-being, we need to use the Trajectory objects from the embedded event
  process.trackerDrivenElectronSeedsORG = process.trackerDrivenElectronSeeds.clone()
  process.trackerDrivenElectronSeedsORG.TkColList = cms.VInputTag(
    cms.InputTag("generalTracksORG")
  )

  process.trackerDrivenElectronSeeds = cms.EDProducer("ElectronSeedTrackRefUpdater",
    PreIdLabel = process.trackerDrivenElectronSeedsORG.PreIdLabel,
    PreGsfLabel = process.trackerDrivenElectronSeedsORG.PreGsfLabel,
    targetTracks = cms.InputTag("generalTracks"),
    inSeeds = cms.InputTag("trackerDrivenElectronSeedsORG", process.trackerDrivenElectronSeedsORG.PreGsfLabel.value()),
    inPreId = cms.InputTag("trackerDrivenElectronSeedsORG", process.trackerDrivenElectronSeedsORG.PreIdLabel.value()),
  )

  for p in process.paths:
    pth = getattr(process,p)
    if "trackerDrivenElectronSeeds" in pth.moduleNames():
        pth.replace(process.trackerDrivenElectronSeeds, process.trackerDrivenElectronSeedsORG*process.trackerDrivenElectronSeeds)

  process.gsfConversionTrackProducer.TrackProducer = cms.string('electronGsfTracksORG')

  # CV: need to keep 'generalTracksORG' collection in event-content,
  #     as (at least electron based) PFCandidates will refer to it,
  #     causing exception in 'pfNoPileUp' module otherwise
  outputModule.outputCommands.extend(['keep recoTracks_generalTracksORG_*_*'])
  #----------------------------------------------------------------------------------------------------------------------

  # mix collections of GSF electron tracks
  process.electronGsfTracksORG = process.electronGsfTracks.clone()
  process.electronGsfTracks = cms.EDProducer("GsfTrackMixer", 
      col1 = cms.InputTag("electronGsfTracksORG", "", "EmbeddedRECO"),
      col2 = cms.InputTag("electronGsfTracks", "", inputProcess)
  )

  process.gsfConversionTrackProducer.TrackProducer = cms.string('electronGsfTracksORG')

  for p in process.paths:
    pth = getattr(process,p)
    if "electronGsfTracks" in pth.moduleNames():
        pth.replace(process.electronGsfTracks, process.electronGsfTracksORG*process.electronGsfTracks)

  process.generalConversionTrackProducer.TrackProducer = cms.string('generalTracksORG')
  process.uncleanedOnlyGeneralConversionTrackProducer.TrackProducer = cms.string('generalTracksORG')

  process.gsfElectronsORG = process.gsfElectrons.clone()
  process.gsfElectrons = cms.EDProducer("GSFElectronsMixer",
      col1 = cms.InputTag("gsfElectronsORG"),
      col2 = cms.InputTag("gsfElectrons","",inputProcess),
  )
  for p in process.paths:
    pth = getattr(process,p)
    if "gsfElectrons" in pth.moduleNames():
      pth.replace(process.gsfElectrons, process.gsfElectronsORG*process.gsfElectrons)

  # dE/dx information for mixed track collections not yet implemented in 'RecoTracksMixer' module,
  # disable usage of dE/dx information in all event reconstruction modules for now
  for p in process.paths:
    pth = getattr(process,p)
    for mod in pth.moduleNames():
      if mod.find("dedx") != -1 and mod.find("Zmumu") == -1:
        if mod.find("ForPFMuonCleaning") == -1:
          print "Removing %s" % mod
        module = getattr(process, mod)
        pth.remove(module)

  # CV: Compute expected energy deposits of muon in EB/EE, HB/HE and HO:
  #      (1) compute distance traversed by muons produced in Z -> mu+ mu- decay
  #          through individual calorimeter cells
  #      (2) scale distances by expected energy loss dE/dx of muon
  from TrackingTools.TrackAssociator.default_cfi import TrackAssociatorParameterBlock
  process.muonCaloDistances = cms.EDProducer('MuonCaloDistanceProducer',
    trackAssociator = TrackAssociatorParameterBlock.TrackAssociatorParameters,
    selectedMuons = process.customization_options.ZmumuCollection)
  process.ProductionFilterSequence += process.muonCaloDistances

  # mix collections of L1Extra objects
  l1ExtraCollections = [
      [ "L1EmParticle",     "Isolated"    ],
      [ "L1EmParticle",     "NonIsolated" ],
      [ "L1EtMissParticle", "MET"         ],
      [ "L1EtMissParticle", "MHT"         ],
      [ "L1JetParticle",    "Central"     ],
      [ "L1JetParticle",    "Forward"     ],
      [ "L1JetParticle",    "Tau"         ],
      [ "L1MuonParticle",   ""            ]
  ]
  l1extraParticleCollections = []
  for l1ExtraCollection in l1ExtraCollections:
      inputType = l1ExtraCollection[0]
      pluginType = None
      srcVeto = cms.InputTag('')
      dRveto = 0.
      if inputType == "L1EmParticle":
          pluginType = "L1ExtraEmParticleMixerPlugin"
          srcSelectedMuons = process.customization_options.ZmumuCollection
          dRveto = 0.3
      elif inputType == "L1EtMissParticle":
          pluginType = "L1ExtraMEtMixerPlugin"
      elif inputType == "L1JetParticle":
          pluginType = "L1ExtraJetParticleMixerPlugin"
          srcSelectedMuons = process.customization_options.ZmumuCollection
          dRveto = 0.3
      elif inputType == "L1MuonParticle":
          pluginType = "L1ExtraMuonParticleMixerPlugin"
          srcSelectedMuons = process.customization_options.ZmumuCollection
          dRveto = 0.8
      else:
          raise ValueError("Invalid L1Extra type = %s !!" % inputType)
      instanceLabel = l1ExtraCollection[1]
      l1extraParticleCollections.append(cms.PSet(
          pluginType = cms.string(pluginType),
	  instanceLabel = cms.string(instanceLabel),
	  srcSelectedMuons2 = srcSelectedMuons,
          dRveto2 = cms.double(dRveto)))
      if inputType == 'L1EtMissParticle':
        l1extraParticleCollections[-1].srcMuons = cms.InputTag("muonCaloDistances", "muons")
        l1extraParticleCollections[-1].distanceMapMuPlus = cms.InputTag("muonCaloDistances", "distancesMuPlus")
        l1extraParticleCollections[-1].distanceMapMuMinus = cms.InputTag("muonCaloDistances", "distancesMuPlus")
        l1extraParticleCollections[-1].H_Calo_AbsEtaLt12 = cms.double(process.customization_options.muonCaloCleaningSF.value()*0.75)
        l1extraParticleCollections[-1].H_Calo_AbsEta12to17 = cms.double(process.customization_options.muonCaloCleaningSF.value()*0.6)
        l1extraParticleCollections[-1].H_Calo_AbsEtaGt17 = cms.double(process.customization_options.muonCaloCleaningSF.value()*0.3)        

  process.l1extraParticlesORG = process.l1extraParticles.clone()
  process.l1extraParticles = cms.EDProducer('L1ExtraMixer',
      src1 = cms.InputTag("l1extraParticlesORG"),                                          
      src2 = cms.InputTag("l1extraParticles", "", inputProcess),
      collections = cms.VPSet(l1extraParticleCollections)
  )
  for p in process.paths:
      pth = getattr(process,p)
      if "l1extraParticles" in pth.moduleNames():
          pth.replace(process.l1extraParticles, process.l1extraParticlesORG*process.l1extraParticles)

  if process.customization_options.embeddingMode.value() == "PF":
    print "Using PF-embedding"
    from TauAnalysis.MCEmbeddingTools.embeddingCustomizePF import customise as customisePF
    customisePF(process)
  elif process.customization_options.embeddingMode.value() == "RH":
    print "Using RH-embedding"
    from TauAnalysis.MCEmbeddingTools.embeddingCustomizeRH import customise as customiseRH
    customiseRH(process, inputProcess)
  else:
    raise ValueError("Invalid Configuration parameter 'embeddingMode' = %s !!" % process.customization_options.embeddingMode.value())

  # it should be the best solution to take the original beam spot for the
  # reconstruction of the new primary vertex
  # use the one produced earlier, do not produce your own
  for s in process.sequences:
     seq = getattr(process,s)
     seq.remove(process.offlineBeamSpot) 

  try:
    process.metreco.remove(process.BeamHaloId)
  except:
    pass

  try:
    outputModule = process.output
  except:
    pass
  try:
    outputModule = getattr(process, str(getattr(process, list(process.endpaths)[-1])))
  except:
    pass

  process.filterEmptyEv.src = cms.untracked.InputTag("generator", "", "EmbeddedRECO")

  try:
    process.schedule.remove(process.DQM_FEDIntegrity_v3)
  except:
    pass

  process.load("TauAnalysis/MCEmbeddingTools/ZmumuStandaloneSelection_cff")
  process.goldenZmumuFilter.src = process.customization_options.ZmumuCollection
  if process.customization_options.applyZmumuSkim.value():
    print "Enabling Zmumu skim"
    process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
    for path in process.paths:
      getattr(process,path)._seq = process.goldenZmumuFilterSequence * getattr(process,path)._seq
    process.options = cms.untracked.PSet(
      wantSummary = cms.untracked.bool(True)
    )
  else:
    print "Zmumu skim disabled"
  # CV: keep track of Z->mumu selection efficiency
  process.goldenZmumuFilterResult = cms.EDProducer("CandViewCountEventSelFlagProducer",
    src = process.customization_options.ZmumuCollection,
    minNumber = cms.uint32(1)
  )
  process.goldenZmumuFilterEfficiencyPath = cms.Path(process.goldenZmumuSelectionSequence + process.goldenZmumuFilterResult + process.MEtoEDMConverter)
  process.schedule.append(process.goldenZmumuFilterEfficiencyPath)

  process.load("TauAnalysis/MCEmbeddingTools/muonRadiationFilter_cfi")
  process.muonRadiationFilter.srcSelectedMuons = process.customization_options.ZmumuCollection
  if process.customization_options.applyMuonRadiationFilter.value():
    print "Muon -> muon + photon radiation filter enabled"
    # CV: add filter at the end of reconstruction path
    #    (filter needs mixed 'pfPileUp' and 'pfNoPileUp' collections)
    process.reconstruction_step += process.muonRadiationFilterSequence
    process.options = cms.untracked.PSet(
      wantSummary = cms.untracked.bool(True)
    )   
  else:
    print "Muon -> muon + photon radiation filter disabled"
  # CV: keep track of which events pass/fail muon -> muon + photon radiation filter
  process.muonRadiationFilterResult = cms.EDProducer("DummyBoolEventSelFlagProducer")
  process.muonRadiationFilterEfficiencyPath = cms.Path(process.goldenZmumuSelectionSequence + process.muonRadiationFilterSequence + process.muonRadiationFilterResult)
  process.schedule.append(process.muonRadiationFilterEfficiencyPath)

  # CV: disable ECAL/HCAL noise simulation
  if process.customization_options.disableCaloNoise.value():
    print "Disabling ECAL/HCAL noise simulation" 
    process.simEcalUnsuppressedDigis.doNoise = cms.bool(False)
    process.simEcalUnsuppressedDigis.doESNoise = cms.bool(False)
    process.simHcalUnsuppressedDigis.doNoise = cms.bool(False)
    process.simHcalUnsuppressedDigis.doThermalNoise = cms.bool(False)
  else:
    print "Keeping ECAL/HCAL noise simulation enabled"
    
  # CV: apply/do not apply muon momentum corrections determined by Rochester group
  if process.customization_options.replaceGenOrRecMuonMomenta.value() == "rec" and hasattr(process, "goldenZmumuSelectionSequence"):
    if process.customization_options.applyRochesterMuonCorr.value():
      print "Enabling Rochester muon momentum corrections"
      process.patMuonsForZmumuSelectionRochesterMomentumCorr = cms.EDProducer("RochesterCorrPATMuonProducer",
        src = cms.InputTag('patMuonsForZmumuSelection'),
        isMC = cms.bool(process.customization_options.isMC.value())
      )
      process.goldenZmumuSelectionSequence.replace(process.patMuonsForZmumuSelection, process.patMuonsForZmumuSelection*process.patMuonsForZmumuSelectionRochesterMomentumCorr)
      process.goodMuons.src = cms.InputTag('patMuonsForZmumuSelectionRochesterMomentumCorr')
  else:
    print "Rochester muon momentum corrections disabled"

  if process.customization_options.applyMuonRadiationCorrection.value() != "":
    print "Muon -> muon + photon radiation correction enabled"
    process.load("TauAnalysis/MCEmbeddingTools/muonRadiationCorrWeightProducer_cfi")
    if process.customization_options.applyMuonRadiationCorrection.value() == "photos":
      process.muonRadiationCorrWeightProducer.lutDirectoryRef = cms.string('genMuonRadCorrAnalyzerPHOTOS')
      process.muonRadiationCorrWeightProducer.lutDirectoryOthers = cms.PSet(                              
        Summer12mcMadgraph = cms.string('genMuonRadCorrAnalyzer')
      )
    elif process.customization_options.applyMuonRadiationCorrection.value() == "pythia":
      process.muonRadiationCorrWeightProducer.lutDirectoryRef = cms.string('genMuonRadCorrAnalyzerPYTHIA')
      process.muonRadiationCorrWeightProducer.lutDirectoryOthers = cms.PSet(                              
        Summer12mcMadgraph = cms.string('genMuonRadCorrAnalyzer')
      )
    else:
      raise ValueError("Invalid Configuration parameter 'applyMuonRadiationCorrection' = %s !!" % process.customization_options.applyMuonRadiationCorrection.value())
    process.reconstruction_step += process.muonRadiationCorrWeightProducer
    outputModule.outputCommands.extend([
      'keep *_muonRadiationCorrWeightProducer_*_*',      
      'keep *_generator_muonsBeforeRad_*',
      'keep *_generator_muonsAfterRad_*'
    ])
  else:
    print "Muon -> muon + photon radiation correction disabled"
    
  return(process)