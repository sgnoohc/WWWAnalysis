#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob, TQHWWPlotter, TQEventlistAnalysisJob
from rooutil.qutils import *
sys.path.append("..")

#_____________________________________________________________________________________________________
def main():

    options = {

        # The main root TQSampleFolder name
        "master_sample_name" : "samples",

        # Where the ntuples are located
        "ntuple_path" : "/nfs-7/userdata/phchang/WWW_babies/WWW_v1.2.3/skim/",

        # Path to the config file that defines how the samples should be organized
        "sample_config_path" : "samples.cfg",

        # The samples with "priority" (defined in sample_config_pat) values satisfying the following condition is looped over
        "priority_value" : ">0",

        # The samples with "priority" (defined in sample_config_pat) values satisfying the following condition is NOT looped over
        "exclude_priority_value" : "<-1",

        # N-cores
        "ncore" : 3,

        # TQCuts config file
        "cuts" : "cuts.cfg",

        # Histogram config file
        "histo" : "histo.cfg",

        # Eventlist histogram
        "eventlist" : "eventlist.cfg",

        # Custom observables (dictionary)
        "customobservables" : {},

        # Custom observables (dictionary)
        "output_dir" : "outputs/"

    }

    # First generate cuts.cfg file
    generate_cuts_config()

    # Analyze
    loop(options)

    # Create plots and tables
    samples = TQSampleFolder.loadSampleFolder("outputs/output.root:samples")
    autoplot (samples,          bkg_path={"WWW":"/sig/www", "WHWWW":"/sig/whwww"}, options={"remove_underflow": True})
    autotable(samples, "yield", bkg_path={"WWW":"/sig/www", "WHWWW":"/sig/whwww"}, options={"cuts": "cuts.cfg"})

#_____________________________________________________________________________________________________
def generate_cuts_config():

    tqcuts = {}

    # Create cuts
    tqcuts["Root"]                = TQCut("Root"                , "Root"                                         , "1"                                 , "evt_scale1fb*purewgt*35.9*1.0384615385")
    tqcuts["SameSignDecay"]       = TQCut("SameSignDecay"       , "WWW events with gen-level same-sign decay"    , "www_channel == 2"                  , "1")
    tqcuts["SameSignDecayLepPt"]  = TQCut("SameSignDecayLepPt"  , "Leptons > 25 GeV"                             , "l_p4[1].pt() >= 25"                , "1")
    tqcuts["SameSignDecayHighPt"] = TQCut("SameSignDecayHighPt" , "The W->jj boson has pt #geq 150 GeV"          , "q_w_pt[0] >= 150"                  , "1")
    tqcuts["SameSignDecayJetPt"]  = TQCut("SameSignDecayJetPt"  , "Quarks to pass pt #geq 30 GeV"                , "q_p4[1].pt()>=30"                  , "1")
    tqcuts["SameSignDecayNjet1"]  = TQCut("SameSignDecayNjet1"  , "Lead q pt #geq 30 && sublead q pt < 30"       , "q_p4[0].pt()>=30&&q_p4[1].pt()<30" , "1")
    tqcuts["ThreeLeptonDecay"]    = TQCut("ThreeLeptonDecay"    , "WWW events with gen-level three-lepton decay" , "www_channel == 3"                  , "1")

    # Build selection tree
    tqcuts["Root"].addCut(tqcuts["SameSignDecay"])
    tqcuts["Root"].addCut(tqcuts["ThreeLeptonDecay"])

    tqcuts["SameSignDecay"].addCut(tqcuts["SameSignDecayLepPt"])
    tqcuts["SameSignDecayLepPt"].addCut(tqcuts["SameSignDecayHighPt"])
    tqcuts["SameSignDecayLepPt"].addCut(tqcuts["SameSignDecayJetPt"])
    tqcuts["SameSignDecayLepPt"].addCut(tqcuts["SameSignDecayNjet1"])

    exportTQCutsToTextFile(tqcuts["Root"], "cuts.cfg")

    cuts = loadTQCutsFromTextFile("cuts.cfg")

    cuts.printCuts("trd")

if __name__ == "__main__":

    main()

