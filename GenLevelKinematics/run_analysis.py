#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQCut, TQSampleFolder
from rooutil.qutils import loop, exportTQCutsToTextFile, loadTQCutsFromTextFile, autoplot, autotable

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
        "ncore" : 4,

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
    autoplot (samples,          bkg_path={"WWW":"/sig/www", "WHWWW":"/sig/whwww"}, sig_path={"WZ":"/bkg/WZ"}, options={"remove_underflow": True, "signal_scale":"auto"})
    autotable(samples, "yield", bkg_path={"WWW":"/sig/www", "WHWWW":"/sig/whwww"}, sig_path={"WZ":"/bkg/WZ"}, options={"cuts": "cuts.cfg"})

#_____________________________________________________________________________________________________
def generate_cuts_config():

    tqcuts = {}

    # Create cuts
    tqcuts["Root"]                 = TQCut("Root"                 , "Root"                                         , "1"                                                            , "evt_scale1fb*purewgt*35.9*1.0384615385")
    tqcuts["SameSignDecay"]        = TQCut("SameSignDecay"        , "WWW events with gen-level same-sign decay"    , "{'$(treename)'=='t_www'?www_channel == 2:1}"                  , "1")
    tqcuts["SameSignDecayLepPt"]   = TQCut("SameSignDecayLepPt"   , "Leptons > 25 GeV"                             , "{'$(treename)'=='t_www'?l_p4[1].pt() >= 25:1}"                , "1")
    tqcuts["SameSignDecayJetPt"]   = TQCut("SameSignDecayJetPt"   , "Quarks to pass pt #geq 30 GeV"                , "{'$(treename)'=='t_www'?q_p4[1].pt()>=30:1}"                  , "1")
    tqcuts["SameSignDecayNjet1"]   = TQCut("SameSignDecayNjet1"   , "Lead q pt #geq 30 && sublead q pt < 30"       , "{'$(treename)'=='t_www'?q_p4[0].pt()>=30&&q_p4[1].pt()<30:1}" , "1")
    tqcuts["SameSignDecayNj2Reco"] = TQCut("SameSignDecayNj2Reco" , "n_{jets} #geq 2"                              , "nj30 >= 2"                                                    , "1")
    tqcuts["SameSignDecayMjjW"]    = TQCut("SameSignDecayMjjW"    , "|m_{jj} - m_{W}| #leq 15"                     , "abs(Mjj-80) <= 15"                                            , "1")
    tqcuts["SameSignDecayMjjSB"]   = TQCut("SameSignDecayMjjSB"   , "|m_{jj} - m_{W}| > 15"                        , "abs(Mjj-80) >  15"                                            , "1")
    tqcuts["SameSignDecayMjjHigh"] = TQCut("SameSignDecayMjjHigh" , "m_{jj} #geq 150"                              , "Mjj >= 150"                                                   , "1")
    tqcuts["ThreeLeptonDecay"]     = TQCut("ThreeLeptonDecay"     , "WWW events with gen-level three-lepton decay" , "{'$(treename)'=='t_www'?www_channel == 3:1}"                  , "1")
    tqcuts["SameSignDecayHighPt"]  = TQCut("SameSignDecayHighPt"  , "The W->jj boson has pt #geq 150 GeV"          , "{'$(treename)'=='t_www'?q_w_pt[0] >= 150:1}"                  , "1")

    # Build selection tree
    tqcuts["Root"].addCut(tqcuts["SameSignDecay"])
    tqcuts["Root"].addCut(tqcuts["ThreeLeptonDecay"])

    tqcuts["SameSignDecay"].addCut(tqcuts["SameSignDecayLepPt"])
    tqcuts["SameSignDecayLepPt"].addCut(tqcuts["SameSignDecayHighPt"])
    tqcuts["SameSignDecayLepPt"].addCut(tqcuts["SameSignDecayJetPt"])
    tqcuts["SameSignDecayLepPt"].addCut(tqcuts["SameSignDecayNjet1"])
    tqcuts["SameSignDecayJetPt"].addCut(tqcuts["SameSignDecayNj2Reco"])
    tqcuts["SameSignDecayNj2Reco"].addCut(tqcuts["SameSignDecayMjjW"])
    tqcuts["SameSignDecayNj2Reco"].addCut(tqcuts["SameSignDecayMjjSB"])
    tqcuts["SameSignDecayMjjSB"].addCut(tqcuts["SameSignDecayMjjHigh"])

    exportTQCutsToTextFile(tqcuts["Root"], "cuts.cfg")

    cuts = loadTQCutsFromTextFile("cuts.cfg")

    cuts.printCuts("trd")

if __name__ == "__main__":

    main()

