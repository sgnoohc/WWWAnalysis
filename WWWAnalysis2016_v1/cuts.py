#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob, TQHWWPlotter, TQEventlistAnalysisJob
from rooutil.qutils import *

# weight counter expressions for simplicity
version = "v1.2.2"
#whfilename = "whsusy_fullscan_skim_1_1.root"
whfilename = "whsusy_200_50_fullsim_skim_1_1.root"
isr_nominal = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],19)]".format(version, whfilename)
isr_up      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],20)]".format(version, whfilename)
isr_dn      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],21)]".format(version, whfilename)
wgt_nominal = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],1)]".format(version, whfilename)
pdf_up      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],10)]".format(version, whfilename)
pdf_dn      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],11)]".format(version, whfilename)
qsq_up      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],5)]".format(version, whfilename)
qsq_dn      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],9)]".format(version, whfilename)
als_up      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],12)]".format(version, whfilename)
als_dn      = "[TH3Map:/nfs-7/userdata/phchang/WWW_babies/WWW_{}/skim/{}:h_counterSMS([chimass],[lspmass],13)]".format(version, whfilename)

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
        "ISRUp"             : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_isr_up/weight_isr]*{}/{}:1}}".format(isr_nominal, isr_up) , 
        "ISRDown"           : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_isr_down/weight_isr]*{}/{}:1}}".format(isr_nominal, isr_dn) , 
        "PDFUp"             : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_pdf_up]/[weight_fr_r1_f1]*{}/{}:1}}".format(wgt_nominal, pdf_up) , 
        "PDFDown"           : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_pdf_down]/[weight_fr_r1_f1]*{}/{}:1}}".format(wgt_nominal, pdf_dn) , 
        "QsqUp"             : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_fr_r2_f2]/[weight_fr_r1_f1]*{}/{}:1}}".format(wgt_nominal, qsq_up) , 
        "QsqDown"           : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_fr_r0p5_f0p5]/[weight_fr_r1_f1]*{}/{}:1}}".format(wgt_nominal, qsq_dn) , 
        "AlphaSUp"          : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_alphas_up]/[weight_fr_r1_f1]*{}/{}:1}}".format(wgt_nominal, als_up) , 
        "AlphaSDown"        : "{{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?[weight_alphas_down]/[weight_fr_r1_f1]*{}/{}:1}}".format(wgt_nominal, als_dn) , 
        }

def getWWWAnalysisCuts(lepsfvar_suffix="",trigsfvar_suffix="",jecvar_suffix="",btagsfvar_suffix="",genmet_prefix="",genmet_suffix=""): #define _up _dn etc.

    #
    #
    # Add custom tqobservable that can do more than just string based draw statements
    #
    #
    from QFramework import TQObservable, TQWWWMTMax3L, TQWWWClosureEvtType, TQWWWVariables
    customobservables = {}
    customobservables["MTMax3L"] = TQWWWMTMax3L("")
    customobservables["MTMax3L_up"] = TQWWWMTMax3L("_up")
    customobservables["MTMax3L_dn"] = TQWWWMTMax3L("_dn")
    customobservables["Trigger"] = TQWWWVariables("Trigger")
    TQObservable.addObservable(customobservables["MTMax3L"], "MTMax3L")
    TQObservable.addObservable(customobservables["MTMax3L_up"], "MTMax3L_up")
    TQObservable.addObservable(customobservables["MTMax3L_dn"], "MTMax3L_dn")
    TQObservable.addObservable(customobservables["Trigger"], "Trigger")

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
    ["1"                                                                             , "{\"$(path)\"==\"/sig/www\"?1.0384615385:1}" ] , # Theory paper vs. 208 fb
    ["1"                                                                             , "evt_scale1fb"                  ] , 
    ["1"                                                                             , "purewgt"                       ] , 
    ["1"                                                                             , "{$(usefakeweight)?ffwgt:35.9}" ] , 
    ["firstgoodvertex==0"                                                            , "1"                             ] , 
    ["Flag_AllEventFilters"                                                          , "1"                             ] , 
    ["vetophoton==0"                                                                 , "1"                             ] , 
    ["evt_passgoodrunlist"                                                           , "1"                             ] , 
    ["{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?fastsimfilt==0:1}"               , "1"                             ] ,
    #["run==1" , "1"],
    #["lumi==1520" , "1"],
    #["evt==727193" , "1"],
    ]
    PreselCutExpr, PreselWgtExpr = combexpr(PreselCuts)

    #____________________________________________________________________________________________________________________________________________________________________________
    # This object holds all of the TQCut instances in a dictionary.
    # The TQCut object will have a name, title, cut definition, and weight definition.
    # The TQCut object has a tree-like structure. (i.e. TQCut class cand add children and parents.)
    # The "children" cuts can be added via TQCut::addCut(TQCut* cut) function.
    tqcuts = {}

    # Preselection TQCut object
    # This object will have all the cuts added into a tree structure via adding "children" using TQCut::addCut.
    # Eventually at the end of the function this object will be returned
    tqcuts["Presel"] = TQCut("Presel", "Presel", PreselCutExpr, PreselWgtExpr)

    # WH SUSY sample mass filter and xsec expression
    #BR = "(0.06272+0.2137*0.3258*0.3258+0.02619)*(0.3258)"
    BR = "0.134843696693296"
    tqcuts["SUSY"] = TQCut("SUSY", "SUSY", "{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?chimass==$(mchi)&&lspmass==$(mlsp):1}", "{\"$(path)\"==\"/bsm/whsusy/$(mchi)/$(mlsp)\"?["+BR+"]*[TH1Map:/home/users/phchang/public_html/analysis/www/code/VVVBabyMakerProduction/dilepbabymaker/xsec_susy_13tev.root:h_xsec_c1n2([chimass])]*1000./"+isr_nominal+"*[weight_isr]:1}")

    # Trigger cuts
    tqcuts["Trigger"] = TQCut("Trigger", "Trigger", "[Trigger]", "trigsf")

    # The dilepton channel base cut
    tqcuts["SRDilep"] = TQCut("SRDilep" , "SRDilep" , "{$(usefakeweight)?(nVlep==2)*(nLlep==2)*(nTlep==1)*(lep_pt[0]>25.)*(lep_pt[1]>25.):(nVlep==2)*(nLlep==2)*(nTlep==2)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}")

    # The trilepton channel base cut
    tqcuts["SRTrilep"] = TQCut("SRTrilep" , "SRTrilep" , "({$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)})*(lep_pt[0]>25.)" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}")

    # The cut hierarchies are defined by adding "children" cuts via function TQCut::addCut
    tqcuts["SUSY"].addCut(tqcuts["Trigger"])
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
    tqcuts["SRSSeePre"]     = TQCut("SRSSeePre"     , "SR-ee"                       , "1"                                                , "1")
    tqcuts["SRSSeeMjjW"]    = TQCut("SRSSeeMjjW"    , "SRSSee: 4. |Mjj-80| < 15"    , "abs(Mjj"+jecvar_suffix+"-80.)<15."                , "1")
    tqcuts["SRSSeeMjjL"]    = TQCut("SRSSeeMjjL"    , "SRSSee: 5. MjjL < 400"       , "MjjL"+jecvar_suffix+"<400."                       , "1")
    tqcuts["SRSSeeDetajjL"] = TQCut("SRSSeeDetajjL" , "SRSSee: 6. DetajjL < 1.5"    , "DetajjL"+jecvar_suffix+"<1.5"                     , "1")
    tqcuts["SRSSeeMET"]     = TQCut("SRSSeeMET"     , "SRSSee: 7. MET > 60"         , "met"+genmet_suffix+jecvar_suffix+"_pt>60."        , "1")
    tqcuts["SRSSeeMllSS"]   = TQCut("SRSSeeMllSS"   , "SRSSee: 8. MllSS > 40"       , "MllSS>40."                                        , "1")
    tqcuts["SRSSeeFull"]    = TQCut("SRSSeeFull"    , "SR-ee"                       , "1"                                                , "1")
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
    tqcuts["SRSSemPre"]     = TQCut("SRSSemPre"     , "SR-e#mu"                     , "1"                                            , "1")
    tqcuts["SRSSemMjjW"]    = TQCut("SRSSemMjjW"    , "SRSSem: 4. |Mjj-80| < 15"    , "abs(Mjj"+jecvar_suffix+"-80.)<15."            , "1")
    tqcuts["SRSSemMjjL"]    = TQCut("SRSSemMjjL"    , "SRSSem: 5. MjjL < 400"       , "MjjL"+jecvar_suffix+"<400."                   , "1")
    tqcuts["SRSSemDetajjL"] = TQCut("SRSSemDetajjL" , "SRSSem: 6. DetajjL < 1.5"    , "DetajjL"+jecvar_suffix+"<1.5"                 , "1")
    tqcuts["SRSSemMET"]     = TQCut("SRSSemMET"     , "SRSSem: 7. MET > 60"         , "met"+genmet_suffix+jecvar_suffix+"_pt>60."    , "1")
    tqcuts["SRSSemMllSS"]   = TQCut("SRSSemMllSS"   , "SRSSem: 8. MllSS > 30"       , "MllSS>30."                                    , "1")
    tqcuts["SRSSemMTmax"]   = TQCut("SRSSemMTmax"   , "SRSSem: 9. MTmax"            , "MTmax"+jecvar_suffix+genmet_suffix+">90."     , "1")
    tqcuts["SRSSemFull"]    = TQCut("SRSSemFull"    , "SR-e#mu"                     , "1"                                            , "1")
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
    tqcuts["SRSSmmPre"]     = TQCut("SRSSmmPre"     , "SR-#mu#mu"                   , "1"                                          , "1")
    tqcuts["SRSSmmMjjW"]    = TQCut("SRSSmmMjjW"    , "SRSSmm: 4. |Mjj-80| < 15"    , "abs(Mjj"+jecvar_suffix+"-80.)<15."          , "1")
    tqcuts["SRSSmmMjjL"]    = TQCut("SRSSmmMjjL"    , "SRSSmm: 5. MjjL < 400"       , "MjjL"+jecvar_suffix+"<400."                 , "1")
    tqcuts["SRSSmmDetajjL"] = TQCut("SRSSmmDetajjL" , "SRSSmm: 6. DetajjL < 1.5"    , "DetajjL"+jecvar_suffix+"<1.5"               , "1")
    tqcuts["SRSSmmMET"]     = TQCut("SRSSmmMET"     , "SRSSmm: 7. MET > 0"          , "1."                                         , "1")
    tqcuts["SRSSmmMllSS"]   = TQCut("SRSSmmMllSS"   , "SRSSmm: 8. MllSS > 40"       , "MllSS>40."                                  , "1")
    tqcuts["SRSSmmFull"]    = TQCut("SRSSmmFull"    , "SR-#mu#mu"                   , "1"                                          , "1")
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
    tqcuts["SR0SFOSPre"]        = TQCut("SR0SFOSPre"       , "0SFOS"                                   , "1"                                              , "1")
    tqcuts["SR0SFOSPt3l"]       = TQCut("SR0SFOSPt3l"      , "SR0SFOS: 3. p_{T, lll} > 0"              , "1."                                             , "1")
    tqcuts["SR0SFOSDPhi3lMET"]  = TQCut("SR0SFOSDPhi3lMET" , "SR0SFOS: 4. #Delta#phi_{lll, MET} > 2.5" , "DPhi3lMET"+jecvar_suffix+genmet_suffix+">2.5"   , "1")
    tqcuts["SR0SFOSMET"]        = TQCut("SR0SFOSMET"       , "SR0SFOS: 5. MET > 30"                    , "met"+genmet_suffix+jecvar_suffix+"_pt>30."      , "1")
    tqcuts["SR0SFOSMll"]        = TQCut("SR0SFOSMll"       , "SR0SFOS: 6. Mll > 20"                    , "Mll3L > 20."                                    , "1")
    tqcuts["SR0SFOSM3l"]        = TQCut("SR0SFOSM3l"       , "SR0SFOS: 7. |M3l-MZ| > 10"               , "abs(M3l-91.1876) > 10."                         , "1")
    tqcuts["SR0SFOSZVt"]        = TQCut("SR0SFOSZVt"       , "SR0SFOS: 8. |Mee-MZ| > 15"               , "abs(Mee3L-91.1876) > 15."                       , "1")
    tqcuts["SR0SFOSMTmax"]      = TQCut("SR0SFOSMTmax"     , "SR0SFOS: 9. MTmax > 90"                  , "MTmax3L"+jecvar_suffix+genmet_suffix+">90."     , "1")
    tqcuts["SR0SFOSFull"]       = TQCut("SR0SFOSFull"      , "0SFOS"                                   , "1"                                              , "1")
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
    tqcuts["SR1SFOSPre"]        = TQCut("SR1SFOSPre"       , "1SFOS"                                   , "1"                                                , "1")
    tqcuts["SR1SFOSPt3l"]       = TQCut("SR1SFOSPt3l"      , "SR1SFOS: 3. p_{T, lll} > 60"             , "Pt3l>60."                                         , "1")
    tqcuts["SR1SFOSDPhi3lMET"]  = TQCut("SR1SFOSDPhi3lMET" , "SR1SFOS: 4. #Delta#phi_{lll, MET} > 2.5" , "DPhi3lMET"+jecvar_suffix+genmet_suffix+">2.5"     , "1")
    tqcuts["SR1SFOSMET"]        = TQCut("SR1SFOSMET"       , "SR1SFOS: 5. MET > 40"                    , "met"+genmet_suffix+jecvar_suffix+"_pt>40."        , "1")
    tqcuts["SR1SFOSMll"]        = TQCut("SR1SFOSMll"       , "SR1SFOS: 6. Mll > 20"                    , "Mll3L > 20."                                      , "1")
    tqcuts["SR1SFOSM3l"]        = TQCut("SR1SFOSM3l"       , "SR1SFOS: 7. |M3l-MZ| > 10"               , "abs(M3l-91.1876) > 10."                           , "1")
    tqcuts["SR1SFOSZVt"]        = TQCut("SR1SFOSZVt"       , "SR1SFOS: 8. |MSFOS-MZ| > 20"             , "nSFOSinZ == 0"                                    , "1")
    tqcuts["SR1SFOSMT3rd"]      = TQCut("SR1SFOSMT3rd"     , "SR1SFOS: 9. MT3rd > 90"                  , "MT3rd"+jecvar_suffix+genmet_suffix+">90."         , "1")
    tqcuts["SR1SFOSFull"]       = TQCut("SR1SFOSFull"      , "1SFOS"                                   , "1"                                                , "1")
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
    tqcuts["SR2SFOSPre"]        = TQCut("SR2SFOSPre"       , "2SFOS"                                   , "1"                                              , "1")
    tqcuts["SR2SFOSPt3l"]       = TQCut("SR2SFOSPt3l"      , "SR2SFOS: 3. p_{T, lll} > 60"             , "Pt3l>60."                                       , "1")
    tqcuts["SR2SFOSDPhi3lMET"]  = TQCut("SR2SFOSDPhi3lMET" , "SR2SFOS: 4. #Delta#phi_{lll, MET} > 2.5" , "DPhi3lMET"+jecvar_suffix+genmet_suffix+">2.5"   , "1")
    tqcuts["SR2SFOSMET"]        = TQCut("SR2SFOSMET"       , "SR2SFOS: 5. MET > 55"                    , "met"+genmet_suffix+jecvar_suffix+"_pt>55."      , "1")
    tqcuts["SR2SFOSMll"]        = TQCut("SR2SFOSMll"       , "SR2SFOS: 6. Mll > 20"                    , "(Mll3L > 20. && Mll3L1 > 20.)"                  , "1")
    tqcuts["SR2SFOSM3l"]        = TQCut("SR2SFOSM3l"       , "SR2SFOS: 7. |M3l-MZ| > 10"               , "abs(M3l-91.1876) > 10."                         , "1")
    tqcuts["SR2SFOSZVt"]        = TQCut("SR2SFOSZVt"       , "SR2SFOS: 8. |MSFOS-MZ| > 20"             , "nSFOSinZ == 0"                                  , "1")
    tqcuts["SR2SFOSFull"]       = TQCut("SR2SFOSFull"      , "2SFOS"                                   , "1"                                              , "1")
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
    copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"Nj1"},
            cut_edits={
                "SRSSeeNj2" : TQCut("Nj1SSeeNj1" , "SRSSee: 2. n_{j} = 1" , "nj30"+jecvar_suffix+"==1" , "1"),
                "SRSSemNj2" : TQCut("Nj1SSemNj1" , "SRSSem: 2. n_{j} = 1" , "nj30"+jecvar_suffix+"==1" , "1"),
                "SRSSmmNj2" : TQCut("Nj1SSmmNj1" , "SRSSmm: 2. n_{j} = 1" , "nj30"+jecvar_suffix+"==1" , "1"),
                "SRSSeePre" : TQCut("Nj1SSeeFull", "SRSSee: Njet 1 Full" , "1" , "1"),
                "SRSSemPre" : TQCut("Nj1SSemFull", "SRSSem: Njet 1 Full" , "1" , "1"),
                "SRSSmmPre" : TQCut("Nj1SSmmFull", "SRSSmm: Njet 1 Full" , "1" , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["Nj1Dilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # SR dilepton nj==1
    #
    # Take cuts starting from SRDilep and modify names in each
    # Then also swap SRDilep by "SBDilep" defined by below
    copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"Side"},
            cut_edits={
                "SRSSeeMjjW" : TQCut("SideSSeeMjj" , "SRSSee: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSemMjjW" : TQCut("SideSSemMjj" , "SRSSem: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSmmMjjW" : TQCut("SideSSmmMjj" , "SRSSmm: 4. |Mjj-80| >= 15" , "abs(Mjj"+jecvar_suffix+"-80.)>=15." , "1"),
                "SRSSmmMET" : TQCut("SideSSmmMET" , "SRSSmm: 7. MET > 60" , "met"+genmet_suffix+jecvar_suffix+"_pt>60." , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["SideDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # Add weight variation systematics
    #
    addWeightSystematics(tqcuts["SRSSeeFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SRSSemFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SRSSmmFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SideSSeeFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SideSSemFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SideSSmmFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SR0SFOSFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SR1SFOSFull"], systvars, tqcuts)
    addWeightSystematics(tqcuts["SR2SFOSFull"], systvars, tqcuts)

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # AR dilepton regions
    #
    # Take cuts starting from SRDilep and modify names in each by SR to AR
    # Then also swap SRDilep by "ARDilep" defined by below
    copyEditCuts(
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
    copyEditCuts(
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
    copyEditCuts(
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
    copyEditCuts(
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
    copyEditCuts(
            cut=tqcuts["SRTrilep"],
            name_edits={"SR":"WZCR"},
            cut_edits={
                "SR1SFOSZVt": TQCut("WZCR1SFOSZVt" , "WZCR1SFOS: 8. |MSFOS-MZ| > 20" , "(abs(Mll3L-91.1876)<20.||abs(Mll3L1-91.1876)<20.)" , "1"),
                "SR2SFOSZVt": TQCut("WZCR2SFOSZVt" , "WZCR2SFOS: 8. |MSFOS-MZ| > 20" , "(abs(Mll3L-91.1876)<20.||abs(Mll3L1-91.1876)<20.)" , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["WZCRDilep"])
    tqcuts["Presel"].addCut(tqcuts["WZCRTrilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # B-tagged control regions (BTCR)
    #
    copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"BTCR"},
            cut_edits={
                "SRSSeeNb0" : TQCut("BTCRSSeeNbgeq1" , "BTCRSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSemNb0" : TQCut("BTCRSSemNbgeq1" , "BTCRSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSmmNb0" : TQCut("BTCRSSmmNbgeq1" , "BTCRSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    copyEditCuts(
            cut=tqcuts["SideDilep"],
            name_edits={"Side":"BTCRSide"},
            cut_edits={
                "SideSSeeNb0" : TQCut("BTCRSideSSeeNbgeq1" , "BTCRSideSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSemNb0" : TQCut("BTCRSideSSemNbgeq1" , "BTCRSideSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSmmNb0" : TQCut("BTCRSideSSmmNbgeq1" , "BTCRSideSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    copyEditCuts(
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
    # lost-lep (e.g. WZ, ttZ) control region (WZCR) for b-tagged region
    #
    copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"BTWZCR"},
            cut_edits={
                "SRDilep" : TQCut("BTWZCRDilep" , "BTWZCRDilep" , "{$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}"),
                "SRSSee" : TQCut("BTWZCRSSee" , "BTWZCRSSee:" , "(nSFOSinZ>=1)*(passSSee)*(1)*(MllSS>10.)" , "1"),
                "SRSSem" : TQCut("BTWZCRSSem" , "BTWZCRSSem:" , "(nSFOSinZ>=1)*(passSSem)*(1)*(MllSS>10.)" , "1"),
                "SRSSmm" : TQCut("BTWZCRSSmm" , "BTWZCRSSmm:" , "(nSFOSinZ>=1)*(passSSmm)*(1)*(MllSS>10.)" , "1"),
                "SRSSeeMjjW" : TQCut("BTWZCRSSeeMjjW" , "BTWZCRSSee: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSemMjjW" : TQCut("BTWZCRSSemMjjW" , "BTWZCRSSem: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSmmMjjW" : TQCut("BTWZCRSSmmMjjW" , "BTWZCRSSmm: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSeeNb0" : TQCut("BTWZCRSSeeNbgeq1" , "BTWZCRSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSemNb0" : TQCut("BTWZCRSSemNbgeq1" , "BTWZCRSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SRSSmmNb0" : TQCut("BTWZCRSSmmNbgeq1" , "BTWZCRSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    copyEditCuts(
            cut=tqcuts["SideDilep"],
            name_edits={"Side":"BTWZCR"},
            cut_edits={
                "SideDilep" : TQCut("BTWZCRSideDilep" , "BTWZCRSideDilep" , "{$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}"),
                "SideSSee" : TQCut("BTWZCRSideSSee" , "BTWZCRSideSSee:" , "(nSFOSinZ>=1)*(passSSee)*(1)" , "1"),
                "SideSSem" : TQCut("BTWZCRSideSSem" , "BTWZCRSideSSem:" , "(nSFOSinZ>=1)*(passSSem)*(1)" , "1"),
                "SideSSmm" : TQCut("BTWZCRSideSSmm" , "BTWZCRSideSSmm:" , "(nSFOSinZ>=1)*(passSSmm)*(1)" , "1"),
                "SideSSeeMjjW" : TQCut("BTWZCRSideSSeeMjjW" , "BTWZCRSideSSee: 4. |Mjj-80| < 15" , "1" , "1"),
                "SideSSemMjjW" : TQCut("BTWZCRSideSSemMjjW" , "BTWZCRSideSSem: 4. |Mjj-80| < 15" , "1" , "1"),
                "SideSSmmMjjW" : TQCut("BTWZCRSideSSmmMjjW" , "BTWZCRSideSSmm: 4. |Mjj-80| < 15" , "1" , "1"),
                "SideSSeeNb0" : TQCut("BTWZCRSideSSeeNbgeq1" , "BTWZCRSideSSeeNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSemNb0" : TQCut("BTWZCRSideSSemNbgeq1" , "BTWZCRSideSSemNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                "SideSSmmNb0" : TQCut("BTWZCRSideSSmmNbgeq1" , "BTWZCRSideSSmmNbgeq1" , "nb"+jecvar_suffix+">=1" , "weight_btagsf"+btagsfvar_suffix),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["BTWZCRDilep"])
    tqcuts["Presel"].addCut(tqcuts["BTWZCRSideDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # VBS control region
    #
    copyEditCuts(
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
    # VBS control region version 2
    #
    copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"VBSCR2"},
            cut_edits={
                "SRSSeeMjjW" : TQCut("VBSCR2SSeeMjjW" , "VBSCR2SSee: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSemMjjW" : TQCut("VBSCR2SSemMjjW" , "VBSCR2SSem: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSmmMjjW" : TQCut("VBSCR2SSmmMjjW" , "VBSCR2SSmm: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSeeMjjL" : TQCut("VBSCR2SSeeFull" , "VBSCR2SSee: 5. MjjVBF > 400" , "(MjjVBF"+jecvar_suffix+">400.)" , "1"),
                "SRSSemMjjL" : TQCut("VBSCR2SSemFull" , "VBSCR2SSem: 5. MjjVBF > 400" , "(MjjVBF"+jecvar_suffix+">400.)" , "1"),
                "SRSSmmMjjL" : TQCut("VBSCR2SSmmFull" , "VBSCR2SSmm: 5. MjjVBF > 400" , "(MjjVBF"+jecvar_suffix+">400.)" , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["VBSCR2Dilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # ttW control region
    #
    copyEditCuts(
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
    copyEditCuts(
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
    copyEditCuts(
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
    copyEditCuts(
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
    # lost-lep (e.g. WZ, ttZ) control region for low MET Mjj side-band validation region
    #
    copyEditCuts(
            cut=tqcuts["SRDilep"],
            name_edits={"SR":"LMETWZCR"},
            cut_edits={
                "SRDilep" : TQCut("LMETWZCRDilep" , "LMETWZCRDilep" , "{$(usefakeweight)?(nVlep==3)*(nLlep==3)*(nTlep==2):(nVlep==3)*(nLlep==3)*(nTlep==3)}" , "{$(usefakeweight)?1.:lepsf"+lepsfvar_suffix+"}"),
                "SRSSee" : TQCut("LMETWZCRSSee" , "LMETWZCRSSee:" , "(nSFOSinZ>=1)*(passSSee)*(1)*(MllSS>10.)" , "1"),
                "SRSSem" : TQCut("LMETWZCRSSem" , "LMETWZCRSSem:" , "(nSFOSinZ>=1)*(passSSem)*(1)*(MllSS>10.)" , "1"),
                "SRSSmm" : TQCut("LMETWZCRSSmm" , "LMETWZCRSSmm:" , "(nSFOSinZ>=1)*(passSSmm)*(1)*(MllSS>10.)" , "1"),
                "SRSSeeMjjW" : TQCut("LMETWZCRSSeeMjjW" , "LMETWZCRSSee: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSemMjjW" : TQCut("LMETWZCRSSemMjjW" , "LMETWZCRSSem: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSmmMjjW" : TQCut("LMETWZCRSSmmMjjW" , "LMETWZCRSSmm: 4. |Mjj-80| < 15" , "1" , "1"),
                "SRSSeeMjjL" : TQCut("LMETWZCRSSeeMjjL" , "LMETWZCRSSee: 5. MjjL < inf" , "1" , "1"),
                "SRSSemMjjL" : TQCut("LMETWZCRSSemMjjL" , "LMETWZCRSSem: 5. MjjL < inf" , "1" , "1"),
                "SRSSmmMjjL" : TQCut("LMETWZCRSSmmMjjL" , "LMETWZCRSSmm: 5. MjjL < inf" , "1" , "1"),
                "SRSSeeDetajjL" : TQCut("LMETWZCRSSeeDetajjL" , "LMETWZCRSSee: 6. DetajjL < inf" , "1" , "1"),
                "SRSSemDetajjL" : TQCut("LMETWZCRSSemDetajjL" , "LMETWZCRSSem: 6. DetajjL < inf" , "1" , "1"),
                "SRSSmmDetajjL" : TQCut("LMETWZCRSSmmDetajjL" , "LMETWZCRSSmm: 6. DetajjL < inf" , "1" , "1"),
                "SRSSeeMET" : TQCut("LMETWZCRSSeeFull" , "LMETCRSSee: 7. MET < 60" , "("+"met"+genmet_suffix+jecvar_suffix+"_pt<60.)*(abs(MllSS-91.1876)>10.)" , "1"),
                "SRSSemMET" : TQCut("LMETWZCRSSemFull" , "LMETCRSSem: 7. MET < 60" , "met"+genmet_suffix+jecvar_suffix+"_pt<60." , "1"),
                "SRSSmmMET" : TQCut("LMETWZCRSSmmFull" , "LMETCRSSmm: 7. MET < 60" , "met"+genmet_suffix+jecvar_suffix+"_pt<60." , "1"),
                },
            cutdict=tqcuts,
            )
    # Then add it to Presel
    tqcuts["Presel"].addCut(tqcuts["LMETWZCRDilep"])

    #____________________________________________________________________________________________________________________________________________________________________________
    #
    # Gamma Control region
    #
    copyEditCuts(
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
    return tqcuts["SUSY"]

if __name__ == "__main__":

    cuts = getWWWAnalysisCuts(lepsfvar_suffix="",trigsfvar_suffix="",jecvar_suffix="",btagsfvar_suffix="")
    cuts.printCuts("trd")
