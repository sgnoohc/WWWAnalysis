#!/bin/env python

import os
import sys
from QFramework import TQSampleFolder, TQCut, TQWWWVariables
from rooutil import qutils

#_____________________________________________________________________________________________________
def main(argv):

    def help():
    
        print "Usage:"
        print ""
        print "    python {} INDEX".format(argv[0])
        print ""
        print "  INDEX =-1 : Skip looping and directly remake plots and tables"
        print "  INDEX = 0 : Nominal"
        print "  INDEX = 1 : JEC_Up variation"
        print "  INDEX = 2 : JEC_Down variation"
        print ""
        print ""
        sys.exit()

    options = {

        # The main root TQSampleFolder name
        "master_sample_name" : "samples",

        # Where the ntuples are located
        #"ntuple_path" : "/nfs-7/userdata/phchang/WWW_babies/WWW_v1.2.2/skim/", # Freeze analysis July 17
        "ntuple_path" : "/nfs-7/userdata/phchang/WWW_babies/WWW2016_v3.0.1/skim/",

        # Path to the config file that defines how the samples should be organized
        "sample_config_path" : "../samples.cfg",

        # The samples with "priority" (defined in sample_config_pat) values satisfying the following condition is looped over
        "priority_value" : "<-1",

        # The samples with "priority" (defined in sample_config_pat) values satisfying the following condition is NOT looped over
        "exclude_priority_value" : "<-2",

        # N-cores
        "ncore" : 16,

        # TQCuts config file
        "cuts" : "cuts.cfg",

        # Histogram config file
        "histo" : "histo.cfg",

        # Eventlist histogram
        "eventlist" : "eventlist.cfg",

        # Custom observables (dictionary)
        "customobservables" : {
            "Trigger": TQWWWVariables("Trigger"),
            "MTlvlvjj": TQWWWVariables("MTlvlvjj"),
            "MTlvlv": TQWWWVariables("MTlvlv"),
            },

        # Custom observables (dictionary)
        "output_dir" : "outputs/",

        # Run on certain path only
        #"path" : "/data/ee",

    }

    # Check number of arguments
    if len(argv) < 2: help()
    index = int(argv[1])

    # First generate cuts.cfg file
    skiplooping = False
    if   index == 0:
        generate_www_analysis_cuts()
    elif index == 1:
        generate_www_analysis_cuts(jecvar_suffix="_up")
        options["output_suffix"] = "_jec_up"
    elif index == 2:
        generate_www_analysis_cuts(jecvar_suffix="_dn")
        options["output_suffix"] = "_jec_dn"
    elif index == -1:
        skiplooping = True
    else: help()

    # Analyze
    if not skiplooping: qutils.loop(options)

    # Retreive the result
    samples = TQSampleFolder.loadSampleFolder("{}/output.root:samples".format(options["output_dir"]))

    # Create plots and tables
    bkg_path = [
            ("#gamma#rightarrowlepton" , "/typebkg/photon"  ) ,
            ("Charge mis-id"           , "/typebkg/qflip"   ) ,
            ("Non-prompt"              , "/fake"            ) ,
            ("Lost/three lep"          , "/typebkg/lostlep" ) ,
            ("Irredu."                 , "/typebkg/prompt"  ) 
            ]
    sig_path_plots = [
            ("WWW", "/sig"),
            ("H^{#pm#pm} [200 GeV]", "/bsm/hpmpm/200"),
            ("H^{#pm#pm} [1 TeV]", "/bsm/hpmpm/1500"),
            ]

    sig_path_table = [
            ("WWW", "/sig"),
            ("SM WWW", "/sig/www"),
            ("WHWWW", "/sig/whwww"),
            ("H [200 GeV]", "/bsm/hpmpm/200"),
            #("H [300 GeV]", "/bsm/hpmpm/300"),
            #("H [400 GeV]", "/bsm/hpmpm/400"),
            #("H [500 GeV]", "/bsm/hpmpm/500"),
            ("H [600 GeV]", "/bsm/hpmpm/600"),
            #("H [900 GeV]", "/bsm/hpmpm/900"),
            #("H [1000 GeV]", "/bsm/hpmpm/1000"),
            ("H [1500 GeV]", "/bsm/hpmpm/1500"),
            #("H [2000 GeV]", "/bsm/hpmpm/2000"),
            ]

    histnames = [
            "{SRSSeeFull, SRSSemFull, SRSSmmFull, SideSSeeFull, SideSSemFull, SideSSmmFull, SR0SFOSFull, SR1SFOSFull, SR2SFOSFull}",
            "{SRNj1SSeeFull, SRNj1SSemFull, SRNj1SSmmFull}",
            "{WZCRSSeeFull, WZCRSSemFull, WZCRSSmmFull, WZCR1SFOSFull, WZCR2SFOSFull}",
            "{WZCRNj1SSeeFull, WZCRNj1SSemFull, WZCRNj1SSmmFull}",
            "SRSSeeFull/MTlvlv" ,
            "SRSSemFull/MTlvlv" ,
            "SRSSmmFull/MTlvlv" ,
            "SRSSeeFull/MllSS_wide" ,
            "SRSSemFull/MllSS_wide" ,
            "SRSSmmFull/MllSS_wide" ,
            "SRNj1SSeeFull/MTmax" ,
            "SRNj1SSemFull/MTmax" ,
            "SRNj1SSmmFull/MTmax" ,
            "SRNj1SSeeFull/MTmin" ,
            "SRNj1SSemFull/MTmin" ,
            "SRNj1SSmmFull/MTmin" ,
            "SRNj1SSeeFull/MllSS" ,
            "SRNj1SSemFull/MllSS" ,
            "SRNj1SSmmFull/MllSS" ,
            "SRNj1SSeeFull/MET" ,
            "SRNj1SSemFull/MET" ,
            "SRNj1SSmmFull/MET" ,
            "SRNj1SSeeFull/lep_pt0",
            "SRNj1SSemFull/lep_pt0",
            "SRNj1SSmmFull/lep_pt0",
            "SRNj1SSeeFull/lep_pt1",
            "SRNj1SSemFull/lep_pt1",
            "SRNj1SSmmFull/lep_pt1",
            "SRNj1SSeeFull/jets_pt0",
            "SRNj1SSemFull/jets_pt0",
            "SRNj1SSmmFull/jets_pt0",
            "SRNj1SSmmFull/MTmin" ,
            "SRSSeeFull/MllSS_wide+SRSSemFull/MllSS_wide+SRSSmmFull/MllSS_wide+SideSSeeFull/MllSS_wide+SideSSemFull/MllSS_wide+SideSSmmFull/MllSS_wide" ,
            "SRSSeeFull/MllSS_varbin+SRSSemFull/MllSS_varbin+SRSSmmFull/MllSS_varbin+SideSSeeFull/MllSS_varbin+SideSSemFull/MllSS_varbin+SideSSmmFull/MllSS_varbin" ,
            ]
    qutils.autoplot(samples, histnames, bkg_path=bkg_path, sig_path=sig_path_plots, data_path="/data", options={"blind":["SR"]})

    # Make cutflow table
    cutnames = [
            "SRSSee",
            "SRSSem",
            "SRSSmm",
            "SideSSee",
            "SideSSem",
            "SideSSmm",
            "SR0SFOS",
            "SR1SFOS",
            "SR2SFOS",
            "SRNj1SSee",
            "SRNj1SSem",
            "SRNj1SSmm",
            "WZCRSSee",
            "WZCRSSem",
            "WZCRSSmm",
            "WZCR1SFOS",
            "WZCR2SFOS",
            "WZCRNj1SSee",
            "WZCRNj1SSem",
            "WZCRNj1SSmm",
            #"Root",
            ]
    qutils.autotable(samples, cutnames, bkg_path=bkg_path, sig_path=sig_path_table, options={"cuts": "cuts.cfg"})

    # Make summary cutflow table
    summary_cuts = [
            "SRSSeeFull",
            "SRSSemFull",
            "SRSSmmFull",
            "|",
            "SideSSeeFull",
            "SideSSemFull",
            "SideSSmmFull",
            "|",
            "SR0SFOSFull",
            "SR1SFOSFull",
            "SR2SFOSFull",
            "|",
            "SRNj1SSeeFull",
            "SRNj1SSemFull",
            "SRNj1SSmmFull",
            "|",
            "WZCRSSeeFull",
            "WZCRSSemFull",
            "WZCRSSmmFull",
            "|",
            "WZCR1SFOSFull",
            "WZCR2SFOSFull",
            "|",
            "WZCRNj1SSeeFull",
            "WZCRNj1SSemFull",
            "WZCRNj1SSmmFull",
            ]
    qutils.table(samples, "Root", bkg_path=bkg_path, sig_path=sig_path_table, data_path="/data", options={"cuts": "cuts.cfg", "cuts_list": summary_cuts, "output_name": "summary"})

#_____________________________________________________________________________________________________
def generate_www_analysis_cuts(lepsfvar_suffix="",trigsfvar_suffix="",jecvar_suffix="",btagsfvar_suffix="",genmet_prefix="",genmet_suffix=""): #define _up _dn etc.

    #
    #
    # Define cuts
    #
    #
    PreselCuts = [
    # https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PAPERS/EXOT-2016-10/fig_05b.png taking 2pb -> 2 pb / 0.57 * 0.21 * (0.21 * (1-0.21)) / 8107
    # Took 2pb from the 600 GeV point (rounded)
    # 8107 = total semileptonic same-sign (from Wprime sample itself with splitWprime function in dilepbabymaker)
    # only one factor of (0.21 * (1-0.21)) because it's counted with same sign
    # 0.21 = HWW or W->lv (l = e or mu only, as 8107 was determined with e/mu only)
    ["1"                                                                                                , "{\'$(path)\'==\'/sig/www\'?1.0384615385:1}" ] , # Theory paper vs. 208 fb
    ["1"                                                                                                , "{\'$(type)\'==\'hpmpm\'?{$(mass)==600?1.0335276365*0.3258*0.3258:0.3258*0.3258}:1}" ] , # BR(W->lv)^2 for m=600 the scale1fb of 0.01 assumed 100k events. But due to some inefficiency need to rescale by 1.0335276.
    ["1"                                                                                                , "evt_scale1fb"                  ] , 
    ["1"                                                                                                , "purewgt"                       ] , 
    ["1"                                                                                                , "{$(usefakeweight)?ffwgt:35.9}" ] , 
    ["firstgoodvertex==0"                                                                               , "1"                             ] , 
    ["Flag_AllEventFilters"                                                                             , "1"                             ] , 
    ["vetophoton==0"                                                                                    , "1"                             ] , 
    ["evt_passgoodrunlist"                                                                              , "1"                             ] , 
    ["{\'$(path)\'==\'/bsm/whsusy/$(mchi)/$(mlsp)\'?fastsimfilt==0:1}"                                  , "1"                             ] ,
    #["{\'$(path)\'==\'/data/ee\'?(( [mc_HLT_DoubleEl_DZ])+( [mc_HLT_MuEG])+( [mc_HLT_DoubleMu])):[mc_HLT_DoubleEl_DZ]||[mc_HLT_MuEG]||[mc_HLT_DoubleMu]}"  , "1"                             ] ,
    #["{\'$(path)\'==\'/data/em\'?((![mc_HLT_DoubleEl_DZ])&&( [mc_HLT_MuEG])||( [mc_HLT_DoubleMu])):[mc_HLT_DoubleEl_DZ]||[mc_HLT_MuEG]||[mc_HLT_DoubleMu]}"  , "1"                             ] ,
    #["{\'$(path)\'==\'/data/mm\'?((![mc_HLT_DoubleEl_DZ])&&(![mc_HLT_MuEG])&&( [mc_HLT_DoubleMu])):[mc_HLT_DoubleEl_DZ]||[mc_HLT_MuEG]||[mc_HLT_DoubleMu]}"  , "1"                             ] ,
    #["run==1" , "1"],
    #["lumi==1520" , "1"],
    #["evt==727193" , "1"],
    ]
    PreselCutExpr, PreselWgtExpr = qutils.combexpr(PreselCuts)

    #____________________________________________________________________________________________________________________________________________________________________________
    # This object holds all of the TQCut instances in a dictionary.
    # The TQCut object will have a name, title, cut definition, and weight definition.
    # The TQCut object has a tree-like structure. (i.e. TQCut class cand add children and parents.)
    # The "children" cuts can be added via TQCut::addCut(TQCut* cut) function.
    tqcuts = {}

    # Mother of all cuts
    tqcuts["Root"] = TQCut("Root", "Root", "1", "1")

    # Preselection TQCut object
    # This object will have all the cuts added into a tree structure via adding "children" using TQCut::addCut.
    # Eventually at the end of the function this object will be returned
    tqcuts["Presel"] = TQCut("Presel", "Presel", PreselCutExpr, PreselWgtExpr)

    # Trigger cuts
    tqcuts["Trigger"] = TQCut("Trigger", "Trigger", "(passTrigger)&&(pass_duplicate_ee_em_mm)", "trigsf")

    # The dilepton channel base cut
    tqcuts["SRDilep"] = TQCut("SRDilep" , "SRDilep" , "{$(usefakeweight)?(nVlep==2)*(nLlep==2)*(nTlep==1)*(lep_pt[0]>25.)*(lep_pt[1]>25.):(nVlep==2)*(nLlep==2)*(nTlep==2)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}")

    # The trilepton channel base cut
    tqcuts["SRTrilep"] = TQCut("SRTrilep" , "SRTrilep" , "({$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)})*(lep_pt[0]>25.)" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}")

    # The cut hierarchies are defined by adding "children" cuts via function TQCut::addCut
    tqcuts["Root"].addCut(tqcuts["Trigger"])
    tqcuts["Trigger"].addCut(tqcuts["Presel"])
    tqcuts["Presel"].addCut(tqcuts["SRDilep"])
    tqcuts["Presel"].addCut(tqcuts["SRTrilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # SSee region
    #
    # The same-sign dielectron channel signal region selection cuts
    tqcuts["SRSSee"]        = TQCut("SRSSee"        , "SRSSee:"                     , "(passSSee)*(1)*(MllSS>40.)"                       , "1")
    tqcuts["SRSSeeZeeVt"]   = TQCut("SRSSeeZeeVt"   , "SRSSee: 0. Z veto"           , "abs(MllSS-91.1876)>10."                           , "1")
    tqcuts["SRSSeeTVeto"]   = TQCut("SRSSeeTVeto"   , "SRSSee: 1. n_{isotrack} = 0" , "nisoTrack_mt2_cleaned_VVV_cutbased_veto==0"       , "1")
    tqcuts["SRSSeeNj2"]     = TQCut("SRSSeeNj2"     , "SRSSee: 2. n_{j} #geq 2"     , "nj30"+jecvar_suffix+">= 2"                        , "1")
    tqcuts["SRSSeeNb0"]     = TQCut("SRSSeeNb0"     , "SRSSee: 3. n_{b} = 0"        , "nb"+jecvar_suffix+"==0"                           , "weight_btagsf"+btagsfvar_suffix)
    tqcuts["SRSSeePre"]     = TQCut("SRSSeePre"     , "SRSS-ee: Preselection"       , "1"                                                , "1")
    tqcuts["SRSSeeMjjW"]    = TQCut("SRSSeeMjjW"    , "SRSSee: 4. |Mjj-80| < 15"    , "abs(Mjj"+jecvar_suffix+"-80.)<15."                , "1")
    tqcuts["SRSSeeMjjL"]    = TQCut("SRSSeeMjjL"    , "SRSSee: 5. MjjL < 400"       , "MjjL"+jecvar_suffix+"<400."                       , "1")
    tqcuts["SRSSeeDetajjL"] = TQCut("SRSSeeDetajjL" , "SRSSee: 6. DetajjL < 1.5"    , "DetajjL"+jecvar_suffix+"<1.5"                     , "1")
    tqcuts["SRSSeeMET"]     = TQCut("SRSSeeMET"     , "SRSSee: 7. MET > 60"         , "met"+genmet_suffix+jecvar_suffix+"_pt>60."        , "1")
    tqcuts["SRSSeeMllSS"]   = TQCut("SRSSeeMllSS"   , "SRSSee: 8. MllSS > 40"       , "MllSS>40."                                        , "1")
    tqcuts["SRSSeeFull"]    = TQCut("SRSSeeFull"    , "SR ee"                       , "1"                                                , "1")
    # Define same-sign dielectron region cut hierarchy structure
    tqcuts["SRDilep"]      .addCut( tqcuts["SRSSee"]        ) 
    tqcuts["SRSSee"]       .addCut( tqcuts["SRSSeeZeeVt"]   ) 
    tqcuts["SRSSeeZeeVt"]  .addCut( tqcuts["SRSSeeTVeto"]   ) 
    tqcuts["SRSSeeTVeto"]  .addCut( tqcuts["SRSSeeNj2"]     ) 
    tqcuts["SRSSeeNj2"]    .addCut( tqcuts["SRSSeeNb0"]     ) 
    tqcuts["SRSSeeNb0"]    .addCut( tqcuts["SRSSeePre"]     ) 
    tqcuts["SRSSeePre"]    .addCut( tqcuts["SRSSeeMjjW"]    ) 
    tqcuts["SRSSeeMjjW"]   .addCut( tqcuts["SRSSeeMjjL"]    ) 
    tqcuts["SRSSeeMjjL"]   .addCut( tqcuts["SRSSeeDetajjL"] ) 
    tqcuts["SRSSeeDetajjL"].addCut( tqcuts["SRSSeeMET"]     ) 
    tqcuts["SRSSeeMET"]    .addCut( tqcuts["SRSSeeMllSS"]   ) 
    tqcuts["SRSSeeMllSS"]  .addCut( tqcuts["SRSSeeFull"]    ) 

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # SSem region
    #
    # The same-sign emu channel signal region selection cuts
    tqcuts["SRSSem"]        = TQCut("SRSSem"        , "SRSSem:"                     , "(passSSem)*(1)*(MllSS>30.)"                   , "1")
    tqcuts["SRSSemTVeto"]   = TQCut("SRSSemTVeto"   , "SRSSem: 1. n_{isotrack} = 0" , "nisoTrack_mt2_cleaned_VVV_cutbased_veto==0"   , "1")
    tqcuts["SRSSemNj2"]     = TQCut("SRSSemNj2"     , "SRSSem: 2. n_{j} #geq 2"     , "nj30"+jecvar_suffix+">= 2"                    , "1")
    tqcuts["SRSSemNb0"]     = TQCut("SRSSemNb0"     , "SRSSem: 3. n_{b} = 0"        , "nb"+jecvar_suffix+"==0"                       , "weight_btagsf"+btagsfvar_suffix)
    tqcuts["SRSSemPre"]     = TQCut("SRSSemPre"     , "SRSSem: Preselection"        , "1"                                            , "1")
    tqcuts["SRSSemMjjW"]    = TQCut("SRSSemMjjW"    , "SRSSem: 4. |Mjj-80| < 15"    , "abs(Mjj"+jecvar_suffix+"-80.)<15."            , "1")
    tqcuts["SRSSemMjjL"]    = TQCut("SRSSemMjjL"    , "SRSSem: 5. MjjL < 400"       , "MjjL"+jecvar_suffix+"<400."                   , "1")
    tqcuts["SRSSemDetajjL"] = TQCut("SRSSemDetajjL" , "SRSSem: 6. DetajjL < 1.5"    , "DetajjL"+jecvar_suffix+"<1.5"                 , "1")
    tqcuts["SRSSemMET"]     = TQCut("SRSSemMET"     , "SRSSem: 7. MET > 60"         , "met"+genmet_suffix+jecvar_suffix+"_pt>60."    , "1")
    tqcuts["SRSSemMllSS"]   = TQCut("SRSSemMllSS"   , "SRSSem: 8. MllSS > 30"       , "MllSS>30."                                    , "1")
    tqcuts["SRSSemMTmax"]   = TQCut("SRSSemMTmax"   , "SRSSem: 9. MTmax"            , "MTmax"+jecvar_suffix+genmet_suffix+">90."     , "1")
    tqcuts["SRSSemFull"]    = TQCut("SRSSemFull"    , "SR e#mu"                     , "1"                                            , "1")
    # Define same-sign emu region cut hierarchy structure
    tqcuts["SRDilep"]       .addCut( tqcuts ["SRSSem"]        ) 
    tqcuts["SRSSem"]        .addCut( tqcuts ["SRSSemTVeto"]   ) 
    tqcuts["SRSSemTVeto"]   .addCut( tqcuts ["SRSSemNj2"]     ) 
    tqcuts["SRSSemNj2"]     .addCut( tqcuts ["SRSSemNb0"]     ) 
    tqcuts["SRSSemNb0"]     .addCut( tqcuts ["SRSSemPre"]     ) 
    tqcuts["SRSSemPre"]     .addCut( tqcuts ["SRSSemMjjW"]    ) 
    tqcuts["SRSSemMjjW"]    .addCut( tqcuts ["SRSSemMjjL"]    ) 
    tqcuts["SRSSemMjjL"]    .addCut( tqcuts ["SRSSemDetajjL"] ) 
    tqcuts["SRSSemDetajjL"] .addCut( tqcuts ["SRSSemMET"]     ) 
    tqcuts["SRSSemMET"]     .addCut( tqcuts ["SRSSemMllSS"]   ) 
    tqcuts["SRSSemMllSS"]   .addCut( tqcuts ["SRSSemMTmax"]   ) 
    tqcuts["SRSSemMTmax"]   .addCut( tqcuts ["SRSSemFull"]    ) 

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # SSmm region
    #
    # The same-sign dimuon channel signal region selection cuts
    tqcuts["SRSSmm"]        = TQCut("SRSSmm"        , "SRSSmm:"                     , "(passSSmm)*(1)*(MllSS>40.)"                 , "1")
    tqcuts["SRSSmmTVeto"]   = TQCut("SRSSmmTVeto"   , "SRSSmm: 1. n_{isotrack} = 0" , "nisoTrack_mt2_cleaned_VVV_cutbased_veto==0" , "1")
    tqcuts["SRSSmmNj2"]     = TQCut("SRSSmmNj2"     , "SRSSmm: 2. n_{j} #geq 2"     , "nj30"+jecvar_suffix+">= 2"                  , "1")
    tqcuts["SRSSmmNb0"]     = TQCut("SRSSmmNb0"     , "SRSSmm: 3. n_{b} = 0"        , "nb"+jecvar_suffix+"==0"                     , "weight_btagsf"+btagsfvar_suffix)
    tqcuts["SRSSmmPre"]     = TQCut("SRSSmmPre"     , "SRSSmm: Preselection"        , "1"                                          , "1")
    tqcuts["SRSSmmMjjW"]    = TQCut("SRSSmmMjjW"    , "SRSSmm: 4. |Mjj-80| < 15"    , "abs(Mjj"+jecvar_suffix+"-80.)<15."          , "1")
    tqcuts["SRSSmmMjjL"]    = TQCut("SRSSmmMjjL"    , "SRSSmm: 5. MjjL < 400"       , "MjjL"+jecvar_suffix+"<400."                 , "1")
    tqcuts["SRSSmmDetajjL"] = TQCut("SRSSmmDetajjL" , "SRSSmm: 6. DetajjL < 1.5"    , "DetajjL"+jecvar_suffix+"<1.5"               , "1")
    tqcuts["SRSSmmMET"]     = TQCut("SRSSmmMET"     , "SRSSmm: 7. MET > 0"          , "1."                                         , "1")
    tqcuts["SRSSmmMllSS"]   = TQCut("SRSSmmMllSS"   , "SRSSmm: 8. MllSS > 40"       , "MllSS>40."                                  , "1")
    tqcuts["SRSSmmFull"]    = TQCut("SRSSmmFull"    , "SR #mu#mu"                   , "1"                                          , "1")
    # Define same-sign dimuon region cut hierarchy structure
    tqcuts["SRDilep"]       .addCut( tqcuts["SRSSmm"]        ) 
    tqcuts["SRSSmm"]        .addCut( tqcuts["SRSSmmTVeto"]   ) 
    tqcuts["SRSSmmTVeto"]   .addCut( tqcuts["SRSSmmNj2"]     ) 
    tqcuts["SRSSmmNj2"]     .addCut( tqcuts["SRSSmmNb0"]     ) 
    tqcuts["SRSSmmNb0"]     .addCut( tqcuts["SRSSmmPre"]     ) 
    tqcuts["SRSSmmPre"]     .addCut( tqcuts["SRSSmmMjjW"]    ) 
    tqcuts["SRSSmmMjjW"]    .addCut( tqcuts["SRSSmmMjjL"]    ) 
    tqcuts["SRSSmmMjjL"]    .addCut( tqcuts["SRSSmmDetajjL"] ) 
    tqcuts["SRSSmmDetajjL"] .addCut( tqcuts["SRSSmmMET"]     ) 
    tqcuts["SRSSmmMET"]     .addCut( tqcuts["SRSSmmMllSS"]   ) 
    tqcuts["SRSSmmMllSS"]   .addCut( tqcuts["SRSSmmFull"]    ) 

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # 0SFOS region
    #
    # The three lepton region with 0 opposite-sign pair with same flavor
    tqcuts["SR0SFOS"]           = TQCut("SR0SFOS"          , "SR0SFOS:"                                , "(nSFOS==0)"                                     , "1")
    tqcuts["SR0SFOSNj1"]        = TQCut("SR0SFOSNj1"       , "SR0SFOS: 1. n_{j} #leq 1"                , "nj"+jecvar_suffix+"<=1"                         , "1")
    tqcuts["SR0SFOSNb0"]        = TQCut("SR0SFOSNb0"       , "SR0SFOS: 2. n_{b} = 0"                   , "nb"+jecvar_suffix+"==0"                         , "weight_btagsf"+btagsfvar_suffix)
    tqcuts["SR0SFOSPre"]        = TQCut("SR0SFOSPre"       , "SR0SFOS: Preselection"                   , "1"                                              , "1")
    tqcuts["SR0SFOSPt3l"]       = TQCut("SR0SFOSPt3l"      , "SR0SFOS: 3. p_{T, lll} > 0"              , "1."                                             , "1")
    tqcuts["SR0SFOSDPhi3lMET"]  = TQCut("SR0SFOSDPhi3lMET" , "SR0SFOS: 4. #Delta#phi_{lll, MET} > 2.5" , "DPhi3lMET"+jecvar_suffix+genmet_suffix+">2.5"   , "1")
    tqcuts["SR0SFOSMET"]        = TQCut("SR0SFOSMET"       , "SR0SFOS: 5. MET > 30"                    , "met"+genmet_suffix+jecvar_suffix+"_pt>30."      , "1")
    tqcuts["SR0SFOSMll"]        = TQCut("SR0SFOSMll"       , "SR0SFOS: 6. Mll > 20"                    , "Mll3L > 20."                                    , "1")
    tqcuts["SR0SFOSM3l"]        = TQCut("SR0SFOSM3l"       , "SR0SFOS: 7. |M3l-MZ| > 10"               , "abs(M3l-91.1876) > 10."                         , "1")
    tqcuts["SR0SFOSZVt"]        = TQCut("SR0SFOSZVt"       , "SR0SFOS: 8. |Mee-MZ| > 15"               , "abs(Mee3L-91.1876) > 15."                       , "1")
    tqcuts["SR0SFOSMTmax"]      = TQCut("SR0SFOSMTmax"     , "SR0SFOS: 9. MTmax > 90"                  , "MTmax3L"+jecvar_suffix+genmet_suffix+">90."     , "1")
    tqcuts["SR0SFOSFull"]       = TQCut("SR0SFOSFull"      , "SR 0SFOS"                                , "1"                                              , "1")
    # Define three lepton with 0 opposite-sign pair with same flavor cut hierarchy
    tqcuts["SRTrilep"]         .addCut( tqcuts["SR0SFOS"]          ) 
    tqcuts["SR0SFOS"]          .addCut( tqcuts["SR0SFOSNj1"]       ) 
    tqcuts["SR0SFOSNj1"]       .addCut( tqcuts["SR0SFOSNb0"]       ) 
    tqcuts["SR0SFOSNb0"]       .addCut( tqcuts["SR0SFOSPre"]       ) 
    tqcuts["SR0SFOSPre"]       .addCut( tqcuts["SR0SFOSPt3l"]      ) 
    tqcuts["SR0SFOSPt3l"]      .addCut( tqcuts["SR0SFOSDPhi3lMET"] ) 
    tqcuts["SR0SFOSDPhi3lMET"] .addCut( tqcuts["SR0SFOSMET"]       ) 
    tqcuts["SR0SFOSMET"]       .addCut( tqcuts["SR0SFOSMll"]       ) 
    tqcuts["SR0SFOSMll"]       .addCut( tqcuts["SR0SFOSM3l"]       ) 
    tqcuts["SR0SFOSM3l"]       .addCut( tqcuts["SR0SFOSZVt"]       ) 
    tqcuts["SR0SFOSZVt"]       .addCut( tqcuts["SR0SFOSMTmax"]     ) 
    tqcuts["SR0SFOSMTmax"]     .addCut( tqcuts["SR0SFOSFull"]      ) 

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # 1SFOS region
    #
    # The three lepton region with 1 opposite-sign pair with same flavor
    tqcuts["SR1SFOS"]           = TQCut("SR1SFOS"          , "SR1SFOS:"                                , "(nSFOS==1)"                                       , "1")
    tqcuts["SR1SFOSNj1"]        = TQCut("SR1SFOSNj1"       , "SR1SFOS: 1. n_{j} #leq 1"                , "nj"+jecvar_suffix+"<=1"                           , "1")
    tqcuts["SR1SFOSNb0"]        = TQCut("SR1SFOSNb0"       , "SR1SFOS: 2. n_{b} = 0"                   , "nb"+jecvar_suffix+"==0"                           , "weight_btagsf"+btagsfvar_suffix)
    tqcuts["SR1SFOSPre"]        = TQCut("SR1SFOSPre"       , "SR1SFOS: Preselection"                   , "1"                                                , "1")
    tqcuts["SR1SFOSPt3l"]       = TQCut("SR1SFOSPt3l"      , "SR1SFOS: 3. p_{T, lll} > 60"             , "Pt3l>60."                                         , "1")
    tqcuts["SR1SFOSDPhi3lMET"]  = TQCut("SR1SFOSDPhi3lMET" , "SR1SFOS: 4. #Delta#phi_{lll, MET} > 2.5" , "DPhi3lMET"+jecvar_suffix+genmet_suffix+">2.5"     , "1")
    tqcuts["SR1SFOSMET"]        = TQCut("SR1SFOSMET"       , "SR1SFOS: 5. MET > 40"                    , "met"+genmet_suffix+jecvar_suffix+"_pt>40."        , "1")
    tqcuts["SR1SFOSMll"]        = TQCut("SR1SFOSMll"       , "SR1SFOS: 6. Mll > 20"                    , "Mll3L > 20."                                      , "1")
    tqcuts["SR1SFOSM3l"]        = TQCut("SR1SFOSM3l"       , "SR1SFOS: 7. |M3l-MZ| > 10"               , "abs(M3l-91.1876) > 10."                           , "1")
    tqcuts["SR1SFOSZVt"]        = TQCut("SR1SFOSZVt"       , "SR1SFOS: 8. |MSFOS-MZ| > 20"             , "nSFOSinZ == 0"                                    , "1")
    tqcuts["SR1SFOSMT3rd"]      = TQCut("SR1SFOSMT3rd"     , "SR1SFOS: 9. MT3rd > 90"                  , "MT3rd"+jecvar_suffix+genmet_suffix+">90."         , "1")
    tqcuts["SR1SFOSFull"]       = TQCut("SR1SFOSFull"      , "SR 1SFOS"                                , "1"                                                , "1")
    # Define three lepton with 1 opposite-sign pair with same flavor cut hierarchy
    tqcuts["SRTrilep"]         .addCut( tqcuts["SR1SFOS"]          ) 
    tqcuts["SR1SFOS"]          .addCut( tqcuts["SR1SFOSNj1"]       ) 
    tqcuts["SR1SFOSNj1"]       .addCut( tqcuts["SR1SFOSNb0"]       ) 
    tqcuts["SR1SFOSNb0"]       .addCut( tqcuts["SR1SFOSPre"]       ) 
    tqcuts["SR1SFOSPre"]       .addCut( tqcuts["SR1SFOSPt3l"]      ) 
    tqcuts["SR1SFOSPt3l"]      .addCut( tqcuts["SR1SFOSDPhi3lMET"] ) 
    tqcuts["SR1SFOSDPhi3lMET"] .addCut( tqcuts["SR1SFOSMET"]       ) 
    tqcuts["SR1SFOSMET"]       .addCut( tqcuts["SR1SFOSMll"]       ) 
    tqcuts["SR1SFOSMll"]       .addCut( tqcuts["SR1SFOSM3l"]       ) 
    tqcuts["SR1SFOSM3l"]       .addCut( tqcuts["SR1SFOSZVt"]       ) 
    tqcuts["SR1SFOSZVt"]       .addCut( tqcuts["SR1SFOSMT3rd"]     ) 
    tqcuts["SR1SFOSMT3rd"]     .addCut( tqcuts["SR1SFOSFull"]      ) 

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # 2SFOS region
    #
    # The three lepton region with 2 opposite-sign pair with same flavor
    tqcuts["SR2SFOS"]           = TQCut("SR2SFOS"          , "SR2SFOS:"                                , "(nSFOS==2)"                                     , "1")
    tqcuts["SR2SFOSNj1"]        = TQCut("SR2SFOSNj1"       , "SR2SFOS: 1. n_{j} #leq 1"                , "nj"+jecvar_suffix+"<=1"                         , "1")
    tqcuts["SR2SFOSNb0"]        = TQCut("SR2SFOSNb0"       , "SR2SFOS: 2. n_{b} = 0"                   , "nb"+jecvar_suffix+"==0"                         , "weight_btagsf"+btagsfvar_suffix)
    tqcuts["SR2SFOSPre"]        = TQCut("SR2SFOSPre"       , "SR2SFOS: Preselection"                   , "1"                                              , "1")
    tqcuts["SR2SFOSPt3l"]       = TQCut("SR2SFOSPt3l"      , "SR2SFOS: 3. p_{T, lll} > 60"             , "Pt3l>60."                                       , "1")
    tqcuts["SR2SFOSDPhi3lMET"]  = TQCut("SR2SFOSDPhi3lMET" , "SR2SFOS: 4. #Delta#phi_{lll, MET} > 2.5" , "DPhi3lMET"+jecvar_suffix+genmet_suffix+">2.5"   , "1")
    tqcuts["SR2SFOSMET"]        = TQCut("SR2SFOSMET"       , "SR2SFOS: 5. MET > 55"                    , "met"+genmet_suffix+jecvar_suffix+"_pt>55."      , "1")
    tqcuts["SR2SFOSMll"]        = TQCut("SR2SFOSMll"       , "SR2SFOS: 6. Mll > 20"                    , "(Mll3L > 20. && Mll3L1 > 20.)"                  , "1")
    tqcuts["SR2SFOSM3l"]        = TQCut("SR2SFOSM3l"       , "SR2SFOS: 7. |M3l-MZ| > 10"               , "abs(M3l-91.1876) > 10."                         , "1")
    tqcuts["SR2SFOSZVt"]        = TQCut("SR2SFOSZVt"       , "SR2SFOS: 8. |MSFOS-MZ| > 20"             , "nSFOSinZ == 0"                                  , "1")
    tqcuts["SR2SFOSFull"]       = TQCut("SR2SFOSFull"      , "SR 2SFOS"                                , "1"                                              , "1")
    # Define three lepton with 2 opposite-sign pair with same flavor cut hierarchy
    tqcuts["SRTrilep"]         .addCut( tqcuts["SR2SFOS"]          ) 
    tqcuts["SR2SFOS"]          .addCut( tqcuts["SR2SFOSNj1"]       ) 
    tqcuts["SR2SFOSNj1"]       .addCut( tqcuts["SR2SFOSNb0"]       ) 
    tqcuts["SR2SFOSNb0"]       .addCut( tqcuts["SR2SFOSPre"]       ) 
    tqcuts["SR2SFOSPre"]       .addCut( tqcuts["SR2SFOSPt3l"]      ) 
    tqcuts["SR2SFOSPt3l"]      .addCut( tqcuts["SR2SFOSDPhi3lMET"] ) 
    tqcuts["SR2SFOSDPhi3lMET"] .addCut( tqcuts["SR2SFOSMET"]       ) 
    tqcuts["SR2SFOSMET"]       .addCut( tqcuts["SR2SFOSMll"]       ) 
    tqcuts["SR2SFOSMll"]       .addCut( tqcuts["SR2SFOSM3l"]       ) 
    tqcuts["SR2SFOSM3l"]       .addCut( tqcuts["SR2SFOSZVt"]       ) 
    tqcuts["SR2SFOSZVt"]       .addCut( tqcuts["SR2SFOSFull"]      ) 

    #############################################################################################################################################################################
    #
    # Starting from the existing signal region we define control regions by modifying cuts in signal region and perhaps adding a few more afterwards
    #
    #############################################################################################################################################################################

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # SR dilepton Mjj sideband
    #
    # Take cuts starting from SRDilep and modify names in each
    # Then also swap SRDilep by "SBDilep" defined by below
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"SRNj1"},
            cut_edits={
                "SRSSeeNj2" : TQCut("SRNj1SSeeNj1" , "SRNj1SSee: 2. n_{j} = 1" , "nj30"+jecvar_suffix+"==1" , "1"),
                "SRSSemNj2" : TQCut("SRNj1SSemNj1" , "SRNj1SSem: 2. n_{j} = 1" , "nj30"+jecvar_suffix+"==1" , "1"),
                "SRSSmmNj2" : TQCut("SRNj1SSmmNj1" , "SRNj1SSmm: 2. n_{j} = 1" , "nj30"+jecvar_suffix+"==1" , "1"),
                "SRSSeePre" : TQCut("SRNj1SSeeFull", "SRNj1 ee"     , "1" , "1"),
                "SRSSemPre" : TQCut("SRNj1SSemFull", "SRNj1 e#mu"   , "1" , "1"),
                "SRSSmmPre" : TQCut("SRNj1SSmmFull", "SRNj1 #mu#mu" , "1" , "1"),
                },
            cutdict=tqcuts,
            terminate=["SRSSeePre", "SRSSemPre", "SRSSmmPre"]
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["SRNj1Dilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # SR dilepton nj==1
    #
    # Take cuts starting from SRDilep and modify names in each
    # Then also swap SRDilep by "SBDilep" defined by below
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"Side"},
            cut_edits={
                "SRSSeeMjjW" : TQCut("SideSSeeMjj" , "SideSSee: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSemMjjW" : TQCut("SideSSemMjj" , "SideSSem: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSmmMjjW" : TQCut("SideSSmmMjj" , "SideSSmm: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSmmMET" : TQCut("SideSSmmMET" , "SideSSmm: 7. MET > 60" , "met"+genmet_suffix+jecvar_suffix+"_pt>60." , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["SideDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # Add weight variation systematics
    #

    # h_neventsinfile
    wgt_nominal = "[TH1Map:$(.init.filepath):h_neventsinfile(1)]"
    pdf_up      = "[TH1Map:$(.init.filepath):h_neventsinfile(10)]"
    pdf_dn      = "[TH1Map:$(.init.filepath):h_neventsinfile(11)]"
    als_up      = "[TH1Map:$(.init.filepath):h_neventsinfile(13)]"
    als_dn      = "[TH1Map:$(.init.filepath):h_neventsinfile(12)]"
    qsq_up      = "[TH1Map:$(.init.filepath):h_neventsinfile(5)]"
    qsq_dn      = "[TH1Map:$(.init.filepath):h_neventsinfile(9)]"

    # Systematic variations as dictionary
    systvars = {
            "LepSFUp"           : "lepsf_up/lepsf",
            "LepSFDown"         : "lepsf_dn/lepsf",
            "TrigSFUp"          : "trigsf_up/trigsf",
            "TrigSFDown"        : "trigsf_dn/trigsf",
            "BTagLFUp"          : "weight_btagsf_light_DN/weight_btagsf",
            "BTagLFDown"        : "weight_btagsf_light_UP/weight_btagsf",
            "BTagHFUp"          : "weight_btagsf_heavy_DN/weight_btagsf",
            "BTagHFDown"        : "weight_btagsf_heavy_UP/weight_btagsf",
            "PileupUp"          : "purewgt_dn/purewgt",
            "PileupDown"        : "purewgt_up/purewgt",
            "FakeUp"            : "{$(usefakeweight)?ffwgt_full_up/ffwgt:1}",
            "FakeDown"          : "{$(usefakeweight)?ffwgt_full_dn/ffwgt:1}",
            "FakeRateUp"        : "{$(usefakeweight)?ffwgt_up/ffwgt:1}",
            "FakeRateDown"      : "{$(usefakeweight)?ffwgt_dn/ffwgt:1}",
            "FakeRateElUp"      : "{$(usefakeweight)?ffwgt_el_up/ffwgt:1}",
            "FakeRateElDown"    : "{$(usefakeweight)?ffwgt_el_dn/ffwgt:1}",
            "FakeRateMuUp"      : "{$(usefakeweight)?ffwgt_mu_up/ffwgt:1}",
            "FakeRateMuDown"    : "{$(usefakeweight)?ffwgt_mu_dn/ffwgt:1}",
            "FakeClosureUp"     : "{$(usefakeweight)?ffwgt_closure_up/ffwgt:1}",
            "FakeClosureDown"   : "{$(usefakeweight)?ffwgt_closure_dn/ffwgt:1}",
            "FakeClosureElUp"   : "{$(usefakeweight)?ffwgt_closure_el_up/ffwgt:1}",
            "FakeClosureElDown" : "{$(usefakeweight)?ffwgt_closure_el_dn/ffwgt:1}",
            "FakeClosureMuUp"   : "{$(usefakeweight)?ffwgt_closure_mu_up/ffwgt:1}",
            "FakeClosureMuDown" : "{$(usefakeweight)?ffwgt_closure_mu_dn/ffwgt:1}",
            "FakeClosureMuDown" : "{$(usefakeweight)?ffwgt_closure_mu_dn/ffwgt:1}",
            "PDFUp"             : "{{\'$(treename)\'==\'t_www\'?[weight_pdf_up]       / [weight_fr_r1_f1] * {} / {}:1}}".format(wgt_nominal, pdf_up) ,
            "PDFDown"           : "{{\'$(treename)\'==\'t_www\'?[weight_pdf_down]     / [weight_fr_r1_f1] * {} / {}:1}}".format(wgt_nominal, pdf_dn) ,
            "QsqUp"             : "{{\'$(treename)\'==\'t_www\'?[weight_fr_r2_f2]     / [weight_fr_r1_f1] * {} / {}:1}}".format(wgt_nominal, qsq_up) ,
            "QsqDown"           : "{{\'$(treename)\'==\'t_www\'?[weight_fr_r0p5_f0p5] / [weight_fr_r1_f1] * {} / {}:1}}".format(wgt_nominal, qsq_dn) ,
            "AlphaSUp"          : "{{\'$(treename)\'==\'t_www\'?[weight_alphas_up]    / [weight_fr_r1_f1] * {} / {}:1}}".format(wgt_nominal, als_up) ,
            "AlphaSDown"        : "{{\'$(treename)\'==\'t_www\'?[weight_alphas_down]  / [weight_fr_r1_f1] * {} / {}:1}}".format(wgt_nominal, als_dn) ,
            }

    qutils.addWeightSystematics(tqcuts["SRSSeeFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SRSSemFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SRSSmmFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SideSSeeFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SideSSemFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SideSSmmFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SR0SFOSFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SR1SFOSFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SR2SFOSFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SRNj1SSeeFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SRNj1SSemFull"], systvars, tqcuts)
    qutils.addWeightSystematics(tqcuts["SRNj1SSmmFull"], systvars, tqcuts)

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # AR dilepton regions
    #
    # Take cuts starting from SRDilep and modify names in each by SR to AR
    # Then also swap SRDilep by "ARDilep" defined by below
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"AR"},
            cut_edits={"SRDilep" : TQCut("ARDilep" , "ARDilep" , "(nVlep==2)*(nLlep==2)*(nTlep==1)*(lep_pt[0]>25.)*(lep_pt[1]>25.)" , "lepsf"+lepsfvar_suffix)},
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["ARDilep"])
 
    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # AR trilepton regions
    #
    # Take cuts starting from SRTrilep and modify names in each by SR to AR
    # Then also swap SRTrilep by "ARTrilep" defined by below
    qutils.copyEditCuts(
            cut=tqcuts["SRTrilep"],
            name_edits={"SR":"AR"},
            cut_edits={"SRTrilep" : TQCut("ARTrilep" , "ARTrilep" , "(nVlep==3)*(nLlep==3)*(nTlep==2)" , "lepsf"+lepsfvar_suffix)},
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["ARTrilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # AR dilepton Mjj side band region
    #
    qutils.copyEditCuts(
            cut=tqcuts["SideDilep"],
            name_edits={"Side":"ARSide"},
            cut_edits={"SideDilep" : TQCut("ARSideDilep" , "ARSideDilep" , "(nVlep==2)*(nLlep==2)*(nTlep==1)*(lep_pt[0]>25.)*(lep_pt[1]>25.)" , "lepsf"+lepsfvar_suffix)},
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["ARSideDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # lost-lep (e.g. WZ, ttZ) control region (WZCR)
    #
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"WZCR"},
            cut_edits={
                "SRDilep" : TQCut("WZCRDilep" , "WZCRDilep" , "{$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}"),
                "SRSSee" : TQCut("WZCRSSee" , "WZCRSSee:" , "(abs(Mll3L-91.1876)<10.||abs(Mll3L1-91.1876)<10.)*(passSSee)*(1)*(MllSS>10.)" , "1"),
                "SRSSem" : TQCut("WZCRSSem" , "WZCRSSem:" , "(abs(Mll3L-91.1876)<10.||abs(Mll3L1-91.1876)<10.)*(passSSem)*(1)*(MllSS>10.)" , "1"),
                "SRSSmm" : TQCut("WZCRSSmm" , "WZCRSSmm:" , "(abs(Mll3L-91.1876)<10.||abs(Mll3L1-91.1876)<10.)*(passSSmm)*(1)*(MllSS>10.)" , "1"),
                "SRSSeeMjjW" : TQCut("WZCRSSeeMjjW" , "WZCRSSee: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSemMjjW" : TQCut("WZCRSSemMjjW" , "WZCRSSem: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSmmMjjW" : TQCut("WZCRSSmmMjjW" , "WZCRSSmm: 4. |Mjj-80| < 15" , "1" , "1"),
                },
            cutdict=tqcuts,
            )
    qutils.copyEditCuts(
            cut=tqcuts["SRTrilep"],
            name_edits={"SR":"WZCR"},
            cut_edits={
                "SR1SFOSZVt": TQCut("WZCR1SFOSZVt" , "WZCR1SFOS: 8. |MSFOS-MZ| > 20" , "(abs(Mll3L-91.1876)<20.||abs(Mll3L1-91.1876)<20.)" , "1"),
                "SR2SFOSZVt": TQCut("WZCR2SFOSZVt" , "WZCR2SFOS: 8. |MSFOS-MZ| > 20" , "(abs(Mll3L-91.1876)<20.||abs(Mll3L1-91.1876)<20.)" , "1"),
                },
            cutdict=tqcuts,
            )
    qutils.copyEditCuts(
            cut=tqcuts["SRNj1Dilep"],
            name_edits={"SRNj1":"WZCRNj1"},
            cut_edits={
                "SRNj1Dilep" : TQCut("WZCRNj1Dilep" , "WZCRNj1Dilep" , "{$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}"),
                "SRNj1SSee" : TQCut("WZCRNj1SSee" , "WZCRNj1SSee:" , "(abs(Mll3L-91.1876)<10.||abs(Mll3L1-91.1876)<10.)*(passSSee)*(1)*(MllSS>10.)*(met"+genmet_suffix+jecvar_suffix+"_pt<30.)" , "1"),
                "SRNj1SSem" : TQCut("WZCRNj1SSem" , "WZCRNj1SSem:" , "(abs(Mll3L-91.1876)<10.||abs(Mll3L1-91.1876)<10.)*(passSSem)*(1)*(MllSS>10.)*(met"+genmet_suffix+jecvar_suffix+"_pt<45.)" , "1"),
                "SRNj1SSmm" : TQCut("WZCRNj1SSmm" , "WZCRNj1SSmm:" , "(abs(Mll3L-91.1876)<10.||abs(Mll3L1-91.1876)<10.)*(passSSmm)*(1)*(MllSS>10.)*(met"+genmet_suffix+jecvar_suffix+"_pt<55.)" , "1"),
                "SRNj1SSeeNj1" : TQCut("WZCRNj1SSeeNj1" , "WZCRNj1SSee: 2. n_{j} #leq 1" , "nj30"+jecvar_suffix+"<=1" , "1"),
                "SRNj1SSemNj1" : TQCut("WZCRNj1SSemNj1" , "WZCRNj1SSem: 2. n_{j} #leq 1" , "nj30"+jecvar_suffix+"<=1" , "1"),
                "SRNj1SSmmNj1" : TQCut("WZCRNj1SSmmNj1" , "WZCRNj1SSmm: 2. n_{j} #leq 1" , "nj30"+jecvar_suffix+"<=1" , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["WZCRDilep"])
    tqcuts["Presel"].addCut(tqcuts["WZCRTrilep"])
    tqcuts["Presel"].addCut(tqcuts["WZCRNj1Dilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # B-tagged control regions (BTCR)
    #
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"BTCR"},
            cut_edits={
                "SRSSeeNb0" : TQCut("BTCRSSeeNbgeq1" , "BTCRSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSemNb0" : TQCut("BTCRSSemNbgeq1" , "BTCRSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSmmNb0" : TQCut("BTCRSSmmNbgeq1" , "BTCRSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    qutils.copyEditCuts(
            cut=tqcuts["SideDilep"],
            name_edits={"Side":"BTCRSide"},
            cut_edits={
                "SideSSeeNb0" : TQCut("BTCRSideSSeeNbgeq1" , "BTCRSideSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSemNb0" : TQCut("BTCRSideSSemNbgeq1" , "BTCRSideSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSmmNb0" : TQCut("BTCRSideSSmmNbgeq1" , "BTCRSideSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    qutils.copyEditCuts(
            cut=tqcuts["SRTrilep"],
            name_edits={"SR":"BTCR"},
            cut_edits={
                "SR0SFOSNb0" : TQCut("BTCR0SFOSNbgeq1" , "BTCR0SFOSNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SR1SFOSNb0" : TQCut("BTCR1SFOSNbgeq1" , "BTCR1SFOSNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SR2SFOSNb0" : TQCut("BTCR2SFOSNbgeq1" , "BTCR2SFOSNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["BTCRDilep"])
    tqcuts["Presel"].addCut(tqcuts["BTCRSideDilep"])
    tqcuts["Presel"].addCut(tqcuts["BTCRTrilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # VBS control region
    #
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"VBSCR"},
            cut_edits={
                "SRSSeeMjjW" : TQCut("VBSCRSSeeMjjW" , "VBSCRSSee: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSemMjjW" : TQCut("VBSCRSSemMjjW" , "VBSCRSSem: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSmmMjjW" : TQCut("VBSCRSSmmMjjW" , "VBSCRSSmm: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSeeMjjL" : TQCut("VBSCRSSeeFull" , "VBSCRSSee: 5. MjjL > 400 || DetajjL > 1.5" , "(MjjL"+jecvar_suffix+">400.)+(DetajjL"+jecvar_suffix+">1.5)" , "1"),
                "SRSSemMjjL" : TQCut("VBSCRSSemFull" , "VBSCRSSem: 5. MjjL > 400 || DetajjL > 1.5" , "(MjjL"+jecvar_suffix+">400.)+(DetajjL"+jecvar_suffix+">1.5)" , "1"),
                "SRSSmmMjjL" : TQCut("VBSCRSSmmFull" , "VBSCRSSmm: 5. MjjL > 400 || DetajjL > 1.5" , "(MjjL"+jecvar_suffix+">400.)+(DetajjL"+jecvar_suffix+">1.5)" , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["VBSCRDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # ttW control region
    #
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"TTWCR"},
            cut_edits={
                "SRSSeeNj2" : TQCut("TTWCRSSeeNj4" , "TTWCRSSeeNj4" , "nj30"+jecvar_suffix+">=4" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSemNj2" : TQCut("TTWCRSSemNj4" , "TTWCRSSemNj4" , "nj30"+jecvar_suffix+">=4" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSmmNj2" : TQCut("TTWCRSSmmNj4" , "TTWCRSSmmNj4" , "nj30"+jecvar_suffix+">=4" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSeeNb0" : TQCut("TTWCRSSeeNbgeq1" , "TTWCRSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSemNb0" : TQCut("TTWCRSSemNbgeq1" , "TTWCRSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSmmNb0" : TQCut("TTWCRSSmmNbgeq1" , "TTWCRSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    qutils.copyEditCuts(
            cut=tqcuts["SideDilep"],
            name_edits={"Side":"TTWCRSide"},
            cut_edits={
                "SideSSeeNj2" : TQCut("TTWCRSideSSeeNj4" , "TTWCRSideSSeeNj4" , "nj30"+jecvar_suffix+">=4" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSemNj2" : TQCut("TTWCRSideSSemNj4" , "TTWCRSideSSemNj4" , "nj30"+jecvar_suffix+">=4" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSmmNj2" : TQCut("TTWCRSideSSmmNj4" , "TTWCRSideSSmmNj4" , "nj30"+jecvar_suffix+">=4" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSeeNb0" : TQCut("TTWCRSideSSeeNbgeq1" , "TTWCRSideSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSemNb0" : TQCut("TTWCRSideSSemNbgeq1" , "TTWCRSideSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSmmNb0" : TQCut("TTWCRSideSSmmNbgeq1" , "TTWCRSideSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["TTWCRDilep"])
    tqcuts["Presel"].addCut(tqcuts["TTWCRSideDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # ttZ control region
    #
    qutils.copyEditCuts(
            cut=tqcuts["SRTrilep"],
            name_edits={"SR":"TTZCR"},
            cut_edits={
                "SR0SFOS"    : TQCut("TTZCR0SFOS"    , "TTZCR0SFOS:"                 , "(nSFOS==0)" , "1"),
                "SR0SFOSNj1" : TQCut("TTZCR0SFOSNj2" , "TTZCR0SFOS: 1. n_{j} #geq 2" , "nj"+jecvar_suffix+">=2"                                          , "1"),
                "SR0SFOSNb0" : TQCut("TTZCR0SFOSNb1" , "TTZCR0SFOS: 2. n_{b} #geq 1" , "nb"+jecvar_suffix+">=1"                                          , "weight_btagsf"+btagsfvar_suffix),
                "SR1SFOS"    : TQCut("TTZCR1SFOS"    , "TTZCR1SFOS:"                 , "(nSFOS==1)" , "1"),
                "SR1SFOSNj1" : TQCut("TTZCR1SFOSNj2" , "TTZCR1SFOS: 1. n_{j} #geq 2" , "nj"+jecvar_suffix+">=2"                                          , "1"),
                "SR1SFOSNb0" : TQCut("TTZCR1SFOSNb1" , "TTZCR1SFOS: 2. n_{b} $geq 1" , "nb"+jecvar_suffix+">=1"                                          , "weight_btagsf"+btagsfvar_suffix),
                "SR2SFOS"    : TQCut("TTZCR2SFOS"    , "TTZCR2SFOS:"                 , "(nSFOS==2)" , "1"),
                "SR2SFOSNj1" : TQCut("TTZCR2SFOSNj2" , "TTZCR2SFOS: 1. n_{j} #geq 2" , "nj"+jecvar_suffix+">=2"                                          , "1"),
                "SR2SFOSNb0" : TQCut("TTZCR2SFOSNb1" , "TTZCR2SFOS: 2. n_{b} $geq 1" , "nb"+jecvar_suffix+">=1"                                          , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["TTZCRTrilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # Mjj sideband low MET
    #
    qutils.copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"LMETCR"},
            cut_edits={
                "SRSSeeMjjW" : TQCut("LMETCRSSeeMjj" , "LMETCRSSee: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSemMjjW" : TQCut("LMETCRSSemMjj" , "LMETCRSSem: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSmmMjjW" : TQCut("LMETCRSSmmMjj" , "LMETCRSSmm: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSeeMjjL" : TQCut("LMETCRSSeeMjjL" , "LMETCRSSee: 5. MjjL < inf" , "1" , "1"),
                "SRSSemMjjL" : TQCut("LMETCRSSemMjjL" , "LMETCRSSem: 5. MjjL < inf" , "1" , "1"),
                "SRSSmmMjjL" : TQCut("LMETCRSSmmMjjL" , "LMETCRSSmm: 5. MjjL < inf" , "1" , "1"),
                "SRSSeeDetajjL" : TQCut("LMETCRSSeeDetajjL" , "LMETCRSSee: 6. DetajjL < inf" , "1" , "1"),
                "SRSSemDetajjL" : TQCut("LMETCRSSemDetajjL" , "LMETCRSSem: 6. DetajjL < inf" , "1" , "1"),
                "SRSSmmDetajjL" : TQCut("LMETCRSSmmDetajjL" , "LMETCRSSmm: 6. DetajjL < inf" , "1" , "1"),
                "SRSSeeMET" : TQCut("LMETCRSSeeMET" , "LMETCRSSee: 7. MET < 60" , "("+"met"+genmet_suffix+jecvar_suffix+"_pt<60.)*(abs(MllSS-91.1876)>10.)" , "1"),
                "SRSSemMET" : TQCut("LMETCRSSemMET" , "LMETCRSSem: 7. MET < 60" , "met"+genmet_suffix+jecvar_suffix+"_pt<60." , "1"),
                "SRSSmmMET" : TQCut("LMETCRSSmmMET" , "LMETCRSSmm: 7. MET < 60" , "met"+genmet_suffix+jecvar_suffix+"_pt<60." , "1"),
                "SRSSemMTmax" : TQCut("LMETCRSSemMTmax" , "LMETCRSSem: 9. MTmax"    , "1." , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["LMETCRDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # Gamma Control region
    #
    qutils.copyEditCuts(
            cut=tqcuts["SR0SFOS"],
            name_edits={"SR":"GCR"},
            cut_edits={
                "SR0SFOS" : TQCut("GCR0SFOS" , "GCR0SFOS:" , "(nSFOSinZ==0)*(met"+genmet_suffix+"_pt<30)*(abs(M3l-91.1876)<20)" , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["SRTrilep"].addCut(tqcuts["GCR0SFOS"])

    # Return the "Root node" which holds all cuts in a tree structure
    qutils.exportTQCutsToTextFile(tqcuts["Root"], "cuts.cfg")

    cuts = qutils.loadTQCutsFromTextFile("cuts.cfg")

    #cuts.printCuts("trd")

if __name__ == "__main__":

    main(sys.argv)

