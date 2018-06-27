#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob, TQHWWPlotter, TQEventlistAnalysisJob, TQObservable
from rooutil.qutils import *

samplescfgpath = "../samples.cfg"
nfspath = "/nfs-7/userdata/phchang/WWW_babies/WWW_v1.1.1/skim/" # First set of WWW baby that has one lepton events
nfspath = "/nfs-7/userdata/phchang/WWW_babies/WWW_v1.1.2/skim/" # Second set of WWW baby that has QCD one lepton event and data one lepton events for fake rate measurement

def main(index, mode, donotrun):

    # Determine JEC mode
    jecvar = ""
    if mode == 1: jecvar = "_up"
    if mode == 2: jecvar = "_dn"

    #
    #
    # Create the master TQSampleFolder
    #
    #
    samples = TQSampleFolder("samples")

    #
    #
    # Connect input baby ntuple
    #
    #
    connectNtuples(samples, samplescfgpath, nfspath, ">4", ">5")

    #
    #
    # Define cuts
    #
    #
    PreselCuts = [
    ["1"                                         , "{$(usefakeweight)?1.:evt_scale1fb*35.9}" ] ,
    ["1"                                         , "{$(usefakeweight)?1.:purewgt}"           ] ,
    ["Flag_AllEventFilters"                      , "1"                                       ] ,
    ["nj30>=1"                                   , "1"                                       ] ,
    ["firstgoodvertex==0"                        , "1"                                       ] ,
    ["evt_passgoodrunlist"                       , "1"                                       ] ,
    ["mc_HLT_SingleIsoMu17+mc_HLT_SingleIsoEl17" , "1"                                       ] ,
    ]
    PreselCutExpr, PreselWgtExpr = combexpr(PreselCuts)

    # Complicated string construction for looes and tight ID muon
    mu_loosetemp = "(TMath::Abs(lep_eta[{idx}]<2.4))*(abs(lep_dz[{idx}])<0.1)*(abs(lep_dxy[{idx}])<0.05)*(abs(lep_ip3d[{idx}])<0.015)*(abs(lep_ip3derr[{idx}]/lep_ip3d[{idx}])<4.)*(abs(lep_pterr[{idx}]/lep_trk_pt[{idx}])<0.2)*(lep_isMediumPOG[{idx}])*(lep_relIso03EAv2Lep[{idx}]<0.4)*(lep_pt[{idx}]>20.)"
    mu_tighttemp = "({loose})*(lep_relIso03EAv2Lep[{idx}]<0.03)".format(loose=mu_loosetemp, idx="{idx}")
    leadmu_loose = mu_loosetemp.format(idx="0")
    leadmu_tight = mu_tighttemp.format(idx="0")
    trailmu_loose = mu_loosetemp.format(idx="1")
    trailmu_tight = mu_tighttemp.format(idx="1")
    bothmu_loose = "({})&&({})".format(leadmu_loose, trailmu_loose)
    bothmu_tight = "({})&&({})".format(leadmu_tight, trailmu_tight)

    # Complicated string construction for looes and tight ID muon
    mu3l_loosetemp = "(TMath::Abs(lep_eta[{idx}]<2.4))*(abs(lep_dz[{idx}])<0.1)*(abs(lep_dxy[{idx}])<0.05)*(abs(lep_ip3d[{idx}])<0.015)*(abs(lep_ip3derr[{idx}]/lep_ip3d[{idx}])<4.)*(abs(lep_pterr[{idx}]/lep_trk_pt[{idx}])<0.2)*(lep_isMediumPOG[{idx}])*(lep_relIso03EAv2Lep[{idx}]<0.4)*(lep_pt[{idx}]>20.)"
    mu3l_tighttemp = "({loose})*(lep_relIso03EAv2Lep[{idx}]<0.07)".format(loose=mu3l_loosetemp, idx="{idx}")
    leadmu3l_loose = mu3l_loosetemp.format(idx="0")
    leadmu3l_tight = mu3l_tighttemp.format(idx="0")
    trailmu3l_loose = mu3l_loosetemp.format(idx="1")
    trailmu3l_tight = mu3l_tighttemp.format(idx="1")
    bothmu3l_loose = "({})&&({})".format(leadmu3l_loose, trailmu3l_loose)
    bothmu3l_tight = "({})&&({})".format(leadmu3l_tight, trailmu3l_tight)

    # Complicated string construction for looes and tight ID electron
    el_loosetemp = "(TMath::Abs(lep_eta[{idx}]<2.4))*(abs(lep_dz[{idx}])<0.1)*(abs(lep_dxy[{idx}])<0.05)*(abs(lep_ip3d[{idx}])<0.01)*(lep_tightCharge[{idx}]==2)*((abs(lep_etaSC[{idx}])<=1.479)*(lep_MVA[{idx}]>0.941)+(abs(lep_etaSC[{idx}])>1.479)*(lep_MVA[{idx}]>0.925))*(lep_isTriggerSafe_v1[{idx}])*(lep_relIso03EAv2Lep[{idx}]<0.4)*(lep_pt[{idx}]>20.)"
    el_tighttemp = "({loose})*(lep_relIso03EAv2Lep[{idx}]<0.03)".format(loose=el_loosetemp, idx="{idx}")
    leadel_loose = el_loosetemp.format(idx="0")
    leadel_tight = el_tighttemp.format(idx="0")
    trailel_loose = el_loosetemp.format(idx="1")
    trailel_tight = el_tighttemp.format(idx="1")
    bothel_loose = "({})&&({})".format(leadel_loose, trailel_loose)
    bothel_tight = "({})&&({})".format(leadel_tight, trailel_tight)

    # Complicated string construction for looes and tight ID electron for three lepton region
    el3l_loosetemp = "(TMath::Abs(lep_eta[{idx}]<2.4))*(abs(lep_dz[{idx}])<0.1)*(abs(lep_dxy[{idx}])<0.05)*(abs(lep_ip3d[{idx}])<0.015)*((abs(lep_etaSC[{idx}])<=1.479)*(lep_MVA[{idx}]>0.92)+(abs(lep_etaSC[{idx}])>1.479)*(lep_MVA[{idx}]>0.88))*(lep_isTriggerSafe_v1[{idx}])*(lep_relIso03EAv2Lep[{idx}]<0.4)*(lep_pt[{idx}]>20.)"
    el3l_tighttemp = "({loose})*(lep_relIso03EAv2Lep[{idx}]<0.05)".format(loose=el3l_loosetemp, idx="{idx}")
    leadel3l_loose = el3l_loosetemp.format(idx="0")
    leadel3l_tight = el3l_tighttemp.format(idx="0")
    trailel3l_loose = el3l_loosetemp.format(idx="1")
    trailel3l_tight = el3l_tighttemp.format(idx="1")
    bothel3l_loose = "({})&&({})".format(leadel3l_loose, trailel3l_loose)
    bothel3l_tight = "({})&&({})".format(leadel3l_tight, trailel3l_tight)

    # Expressions to divide heavy flavor and !(heavy flavor)
    leadhf = "((lep_motherIdSS[0]==-1)+(lep_motherIdSS[0]==-2))"
    leadlf = "((lep_motherIdSS[0]!=-1)*(lep_motherIdSS[0]!=-2))"
    trailhf = "((lep_motherIdSS[1]==-1)+(lep_motherIdSS[1]==-2))"
    traillf = "((lep_motherIdSS[1]!=-1)*(lep_motherIdSS[1]!=-2))"

    # MT expression (as I forgot to add a one lepton MT variable in the WWW baby.)
    MTexpr = "(TMath::Sqrt(2*met"+jecvar+"_pt*lep_pt[0]*(1.0-TMath::Cos(lep_phi[0]-met"+jecvar+"_phi))))"

    # One lepton kinematic selection
    onelep_cuts = "(jets"+jecvar+"_p4[0].pt()>40.)"
    twolep_cuts = "(lep_pdgId[0]*lep_pdgId[1]>0)*(nj30"+jecvar+">=2)" # if removing bveto
    twolepos_cuts = "(lep_pdgId[0]*lep_pdgId[1]<0)"

    # Electroweak control region selection
    # TwoMuHLT17/Mll_Z fSumw[1]=155.889, x=90, error=1.35664
    # TwoElHLT17/Mll_Z fSumw[1]=650.599, x=90, error=18.1318
    # The reason they are not integer is because the prescales are run/lumi dependent and this number is an "effective" prescale value calculated by comparing MC to data in a dilepton z-peak from this trigger
    hlt_mu17_prescale = 155.889
    hlt_el17_prescale = 650.599
    onelepewkcr_cuts = "(jets"+jecvar+"_p4[0].pt()>40.)*(met_pt>30.)"
    onelepewkcr2_cuts = "(jets"+jecvar+"_p4[0].pt()>40.)*(lep_pt[0]>30.)*(met_pt<20.)"
    onelepewkcr3_cuts = "(jets"+jecvar+"_p4[0].pt()>40.)*(lep_pt[0]>50.)"
    onelepmr_cuts = "(met_pt<20.)*("+MTexpr+"<20.)*(jets"+jecvar+"_p4[0].pt()>40.)"
    oneleptrig_cuts = "(abs(lep_pdgId[0])==11)*(mc_HLT_SingleIsoEl17)+(abs(lep_pdgId[0])==13)*(mc_HLT_SingleIsoMu17)"
    oneleptrig_wgts = "{$(usefakeweight)?([abs(lep_pdgId[0])==11])*([mc_HLT_SingleIsoEl17])*("+str(hlt_el17_prescale)+")+([abs(lep_pdgId[0])==13])*([mc_HLT_SingleIsoMu17])*("+str(hlt_mu17_prescale)+"):1.}"
    onelepnvtx_wgts = "{$(usefakeweight)?1.:([abs(lep_pdgId[0])==11])*([TH1Map:nvtxreweight.root:OneElTightEWKCR3NoNvtxRewgt_nvtx([nVert])])+([abs(lep_pdgId[0])==13])*([TH1Map:nvtxreweight.root:OneMuTightEWKCR3NoNvtxRewgt_nvtx([nVert])])}"

    # These weights are for closure tests. The closure tests are performed for same-sign channel only.
    weight_elcomb = "([abs(lep_pdgId[0])==11])*([TH2Map:qcd_fakerates.root:qcdel([abs(lep_eta[0])],[lep_pt[0]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[0]-0.03))])])+([abs(lep_pdgId[1])==11])*([TH2Map:qcd_fakerates.root:qcdel([abs(lep_eta[1])],[lep_pt[1]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[1]-0.03))])])"
    weight_el = "([abs(lep_pdgId[0])==11])*([TH2Map:qcd_fakerates.root:qcdelbcToE([abs(lep_eta[0])],[lep_pt[0]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[0]-0.03))])])+([abs(lep_pdgId[1])==11])*([TH2Map:qcd_fakerates.root:qcdelbcToE([abs(lep_eta[1])],[lep_pt[1]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[1]-0.03))])])"
    weight_mu = "([abs(lep_pdgId[0])==13])*([TH2Map:qcd_fakerates.root:qcdmu([abs(lep_eta[0])],[lep_pt[0]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[0]-0.03))])])+([abs(lep_pdgId[1])==13])*([TH2Map:qcd_fakerates.root:qcdmu([abs(lep_eta[1])],[lep_pt[1]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[1]-0.03))])])"
    weight_elEM1D = "([abs(lep_pdgId[0])==11])*([TH1Map:qcd_fakerates.root:qcdelEM1D([lep_pt[0]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[0]-0.03))])])+([abs(lep_pdgId[1])==11])*([TH1Map:qcd_fakerates.root:qcdelEM1D([lep_pt[1]*(1.0+TMath::Max(0.0, lep_relIso03EAv2Lep[1]-0.03))])])"

    # TQCut objects
    tqcuts = {}
    tqcuts["Presel"] = TQCut("Presel", "Presel", PreselCutExpr, PreselWgtExpr)
    tqcuts["OneLep"] = TQCut("OneLep", "OneLep", "(nVlep==1)*({})*({})".format(oneleptrig_cuts, onelep_cuts), "({})*({})".format(oneleptrig_wgts, onelepnvtx_wgts))
    tqcuts["OneLepNoNvtxRewgt"] = TQCut("OneLepNoNvtxRewgt", "OneLepNoNvtxRewgt", "(nVlep==1)*({})*({})".format(oneleptrig_cuts, onelep_cuts), "({})".format(oneleptrig_wgts))
    tqcuts["TwoLep"] = TQCut("TwoLep", "TwoLep", "(nVlep==2)*({})".format(twolep_cuts), "1")
    tqcuts["TwoLepOS"] = TQCut("TwoLepOS", "TwoLepOS", "(nVlep==2)*({})".format(twolepos_cuts), "1")
    tqcuts["OneLepMR"] = TQCut("OneLepMR", "OneLepMR", "(nVlep==1)*({})".format(onelepmr_cuts), "1")
    tqcuts["OneLepEWKCR"] = TQCut("OneLepEWKCR", "OneLepEWKCR", "(nVlep==1)*({})".format(onelepewkcr_cuts), "1")
    tqcuts["OneLepEWKCR2"] = TQCut("OneLepEWKCR2", "OneLepEWKCR2", "(nVlep==1)*({})".format(onelepewkcr2_cuts), "1")
    tqcuts["OneLepEWKCR3"] = TQCut("OneLepEWKCR3", "OneLepEWKCR3", "(nVlep==1)*({})".format(onelepewkcr3_cuts), "1")
    tqcuts["OneLepEWKCR3NoNvtxRewgt"] = TQCut("OneLepEWKCR3NoNvtxRewgt", "OneLepEWKCR3NoNvtxRewgt", "(nVlep==1)*({})".format(onelepewkcr3_cuts), "1")

    tqcuts["OneMu"] = TQCut("OneMu", "OneMu", "(abs(lep_pdgId[0])==13)", "1")
    tqcuts["OneMuLoose"] = TQCut("OneMuLoose", "OneMuLoose", leadmu_loose, "1")
    tqcuts["OneMuTight"] = TQCut("OneMuTight", "OneMuTight", leadmu_tight, "1")
    tqcuts["OneMu3lLoose"] = TQCut("OneMu3lLoose", "OneMu3lLoose", leadmu3l_loose, "1")
    tqcuts["OneMu3lTight"] = TQCut("OneMu3lTight", "OneMu3lTight", leadmu3l_tight, "1")

    tqcuts["OneEl"] = TQCut("OneEl", "OneEl", "(abs(lep_pdgId[0])==11)", "1")
    tqcuts["OneElLoose"] = TQCut("OneElLoose", "OneElLoose", leadel_loose, "1")
    tqcuts["OneElTight"] = TQCut("OneElTight", "OneElTight", leadel_tight, "1")
    tqcuts["OneEl3lLoose"] = TQCut("OneEl3lLoose", "OneEl3lLoose", leadel3l_loose, "1")
    tqcuts["OneEl3lTight"] = TQCut("OneEl3lTight", "OneEl3lTight", leadel3l_tight, "1")

    tqcuts["OneMuEWKCR"] = TQCut("OneMuEWKCR", "OneMuEWKCR", "(abs(lep_pdgId[0])==13)", "1")
    tqcuts["OneElEWKCR"] = TQCut("OneElEWKCR", "OneElEWKCR", "(abs(lep_pdgId[0])==11)", "1")
    tqcuts["OneMuTightEWKCR"] = TQCut("OneMuTightEWKCR", "OneMuTightEWKCR", leadmu_tight, "1")
    tqcuts["OneElTightEWKCR"] = TQCut("OneElTightEWKCR", "OneElTightEWKCR", leadel_tight, "1")
    tqcuts["OneMu3lTightEWKCR"] = TQCut("OneMu3lTightEWKCR", "OneMu3lTightEWKCR", leadmu3l_tight, "1")
    tqcuts["OneEl3lTightEWKCR"] = TQCut("OneEl3lTightEWKCR", "OneEl3lTightEWKCR", leadel3l_tight, "1")

    tqcuts["OneMuEWKCR2"] = TQCut("OneMuEWKCR2", "OneMuEWKCR2", "(abs(lep_pdgId[0])==13)", "1")
    tqcuts["OneElEWKCR2"] = TQCut("OneElEWKCR2", "OneElEWKCR2", "(abs(lep_pdgId[0])==11)", "1")
    tqcuts["OneMuTightEWKCR2"] = TQCut("OneMuTightEWKCR2", "OneMuTightEWKCR2", leadmu_tight, "1")
    tqcuts["OneElTightEWKCR2"] = TQCut("OneElTightEWKCR2", "OneElTightEWKCR2", leadel_tight, "1")
    tqcuts["OneMu3lTightEWKCR2"] = TQCut("OneMu3lTightEWKCR2", "OneMu3lTightEWKCR2", leadmu3l_tight, "1")
    tqcuts["OneEl3lTightEWKCR2"] = TQCut("OneEl3lTightEWKCR2", "OneEl3lTightEWKCR2", leadel3l_tight, "1")

    tqcuts["OneMuEWKCR3"] = TQCut("OneMuEWKCR3", "OneMuEWKCR3", "(abs(lep_pdgId[0])==13)", "1")
    tqcuts["OneElEWKCR3"] = TQCut("OneElEWKCR3", "OneElEWKCR3", "(abs(lep_pdgId[0])==11)", "1")
    tqcuts["OneMuTightEWKCR3"] = TQCut("OneMuTightEWKCR3", "OneMuTightEWKCR3", leadmu_tight, "1")
    tqcuts["OneElTightEWKCR3"] = TQCut("OneElTightEWKCR3", "OneElTightEWKCR3", leadel_tight, "1")
    tqcuts["OneMu3lTightEWKCR3"] = TQCut("OneMu3lTightEWKCR3", "OneMu3lTightEWKCR3", leadmu3l_tight, "1")
    tqcuts["OneEl3lTightEWKCR3"] = TQCut("OneEl3lTightEWKCR3", "OneEl3lTightEWKCR3", leadel3l_tight, "1")

    tqcuts["OneMuEWKCR3NoNvtxRewgt"] = TQCut("OneMuEWKCR3NoNvtxRewgt", "OneMuEWKCR3NoNvtxRewgt", "(abs(lep_pdgId[0])==13)", "1")
    tqcuts["OneElEWKCR3NoNvtxRewgt"] = TQCut("OneElEWKCR3NoNvtxRewgt", "OneElEWKCR3NoNvtxRewgt", "(abs(lep_pdgId[0])==11)", "1")
    tqcuts["OneMuTightEWKCR3NoNvtxRewgt"] = TQCut("OneMuTightEWKCR3NoNvtxRewgt", "OneMuTightEWKCR3NoNvtxRewgt", leadmu_tight, "1")
    tqcuts["OneElTightEWKCR3NoNvtxRewgt"] = TQCut("OneElTightEWKCR3NoNvtxRewgt", "OneElTightEWKCR3NoNvtxRewgt", leadel_tight, "1")
    tqcuts["OneMu3lTightEWKCR3NoNvtxRewgt"] = TQCut("OneMu3lTightEWKCR3NoNvtxRewgt", "OneMu3lTightEWKCR3NoNvtxRewgt", leadmu3l_tight, "1")
    tqcuts["OneEl3lTightEWKCR3NoNvtxRewgt"] = TQCut("OneEl3lTightEWKCR3NoNvtxRewgt", "OneEl3lTightEWKCR3NoNvtxRewgt", leadel3l_tight, "1")

    tqcuts["TwoMu"] = TQCut("TwoMu", "TwoMu", "([ClosureEvtType]==0)*[(abs(lep_pdgId[0]*lep_pdgId[1])==143)]*[(abs(lep_pdgId[0])==11)*(lep_pass_VVV_cutbased_tight[0])+(abs(lep_pdgId[1])==11)*(lep_pass_VVV_cutbased_tight[1])]", "1") # one any muon and one real tight electron with two total leptons
    tqcuts["TwoMuLoose"] = TQCut("TwoMuLoose", "TwoMuLoose", "(abs(lep_pdgId[0])==13)*({})+(abs(lep_pdgId[1])==13)*({})".format(leadmu_loose, trailmu_loose), "1")
    tqcuts["TwoMuTight"] = TQCut("TwoMuTight", "TwoMuTight", "(abs(lep_pdgId[0])==13)*({})+(abs(lep_pdgId[1])==13)*({})".format(leadmu_tight, trailmu_tight), "1")
    tqcuts["TwoMuLoosePredict"] = TQCut("TwoMuLoosePredict", "TwoMuLoosePredict", "(abs(lep_pdgId[0])==13)*({})+(abs(lep_pdgId[1])==13)*({})".format(leadmu_tight, trailmu_tight), "1")
    tqcuts["TwoMuTightPredict"] = TQCut("TwoMuTightPredict", "TwoMuTightPredict", "(abs(lep_pdgId[0])==13)*({})*(!({}))+(abs(lep_pdgId[1])==13)*({})*(!({}))".format(leadmu_loose, leadmu_tight, trailmu_loose, trailmu_tight), weight_mu)
    tqcuts["TwoMuLoosePredictBVeto"] = TQCut("TwoMuLoosePredictBVeto", "TwoMuLoosePredictBVeto", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==13)*({})+(abs(lep_pdgId[1])==13)*({}))".format(leadmu_tight, trailmu_tight), "1")
    tqcuts["TwoMuTightPredictBVeto"] = TQCut("TwoMuTightPredictBVeto", "TwoMuTightPredictBVeto", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==13)*({})*(!({}))+(abs(lep_pdgId[1])==13)*({})*(!({})))".format(leadmu_loose, leadmu_tight, trailmu_loose, trailmu_tight), weight_mu)

    tqcuts["TwoEl"] = TQCut("TwoEl", "TwoEl", "([ClosureEvtType]==1)*[(abs(lep_pdgId[0]*lep_pdgId[1])==143)]*[(abs(lep_pdgId[0])==13)*(lep_pass_VVV_cutbased_tight[0])+(abs(lep_pdgId[1])==13)*(lep_pass_VVV_cutbased_tight[1])]", "1") # one any electron and one real tight muon with two total leptons
    tqcuts["TwoElLoose"] = TQCut("TwoElLoose", "TwoElLoose", "(abs(lep_pdgId[0])==11)*({})+(abs(lep_pdgId[1])==11)*({})".format(leadel_loose, trailel_loose), "1")
    tqcuts["TwoElTight"] = TQCut("TwoElTight", "TwoElTight", "(abs(lep_pdgId[0])==11)*({})+(abs(lep_pdgId[1])==11)*({})".format(leadel_tight, trailel_tight), "1")
    tqcuts["TwoElLoosePredict"] = TQCut("TwoElLoosePredict", "TwoElLoosePredict", "(abs(lep_pdgId[0])==11)*({})+(abs(lep_pdgId[1])==11)*({})".format(leadel_tight, trailel_tight), "1")
    tqcuts["TwoElTightPredict"] = TQCut("TwoElTightPredict", "TwoElTightPredict", "(abs(lep_pdgId[0])==11)*({})*(!({}))+(abs(lep_pdgId[1])==11)*({})*(!({}))".format(leadel_loose, leadel_tight, trailel_loose, trailel_tight), weight_el)
    tqcuts["TwoElLoosePredictHF"] = TQCut("TwoElLoosePredictHF", "TwoElLoosePredictHF", "(abs(lep_pdgId[0])==11)*({})*({})+(abs(lep_pdgId[1])==11)*({})*({})".format(leadel_tight, leadhf, trailel_tight, trailhf), "1")
    tqcuts["TwoElTightPredictHF"] = TQCut("TwoElTightPredictHF", "TwoElTightPredictHF", "(abs(lep_pdgId[0])==11)*({})*(!({}))*({})+(abs(lep_pdgId[1])==11)*({})*(!({}))*({})".format(leadel_loose, leadel_tight, leadhf, trailel_loose, trailel_tight, trailhf), weight_el)
    tqcuts["TwoElLoosePredictEM1DLF"] = TQCut("TwoElLoosePredictEM1DLF", "TwoElLoosePredictEM1DLF", "(abs(lep_pdgId[0])==11)*({})*({})+(abs(lep_pdgId[1])==11)*({})*({})".format(leadel_tight, leadlf, trailel_tight, traillf), "1")
    tqcuts["TwoElTightPredictEM1DLF"] = TQCut("TwoElTightPredictEM1DLF", "TwoElTightPredictEM1DLF", "(abs(lep_pdgId[0])==11)*({})*(!({}))*({})+(abs(lep_pdgId[1])==11)*({})*(!({}))*({})".format(leadel_loose, leadel_tight, leadlf, trailel_loose, trailel_tight, traillf), weight_elEM1D)
    tqcuts["TwoElLoosePredictComb"] = TQCut("TwoElLoosePredictComb", "TwoElLoosePredictComb", "(abs(lep_pdgId[0])==11)*({})+(abs(lep_pdgId[1])==11)*({})".format(leadel_tight, trailel_tight), "1")
    tqcuts["TwoElTightPredictComb"] = TQCut("TwoElTightPredictComb", "TwoElTightPredictComb", "(abs(lep_pdgId[0])==11)*({})*(!({}))+(abs(lep_pdgId[1])==11)*({})*(!({}))".format(leadel_loose, leadel_tight, trailel_loose, trailel_tight), weight_elcomb)
    tqcuts["TwoElLoosePredictBVeto"] = TQCut("TwoElLoosePredictBVeto", "TwoElLoosePredictBVeto", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})+(abs(lep_pdgId[1])==11)*({}))".format(leadel_tight, trailel_tight), "1")
    tqcuts["TwoElTightPredictBVeto"] = TQCut("TwoElTightPredictBVeto", "TwoElTightPredictBVeto", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})*(!({}))+(abs(lep_pdgId[1])==11)*({})*(!({})))".format(leadel_loose, leadel_tight, trailel_loose, trailel_tight), weight_el)
    tqcuts["TwoElLoosePredictBVetoHF"] = TQCut("TwoElLoosePredictBVetoHF", "TwoElLoosePredictBVetoHF", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})*({})+(abs(lep_pdgId[1])==11)*({})*({}))".format(leadel_tight, leadhf, trailel_tight, trailhf), "1")
    tqcuts["TwoElTightPredictBVetoHF"] = TQCut("TwoElTightPredictBVetoHF", "TwoElTightPredictBVetoHF", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})*(!({}))*({})+(abs(lep_pdgId[1])==11)*({})*(!({}))*({}))".format(leadel_loose, leadel_tight, leadhf, trailel_loose, trailel_tight, trailhf), weight_el)
    tqcuts["TwoElLoosePredictBVetoEM1DLF"] = TQCut("TwoElLoosePredictBVetoEM1DLF", "TwoElLoosePredictBVetoEM1DLF", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})*({})+(abs(lep_pdgId[1])==11)*({})*({}))".format(leadel_tight, leadlf, trailel_tight, traillf), "1")
    tqcuts["TwoElTightPredictBVetoEM1DLF"] = TQCut("TwoElTightPredictBVetoEM1DLF", "TwoElTightPredictBVetoEM1DLF", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})*(!({}))*({})+(abs(lep_pdgId[1])==11)*({})*(!({}))*({}))".format(leadel_loose, leadel_tight, leadlf, trailel_loose, trailel_tight, traillf), weight_elEM1D)
    tqcuts["TwoElLoosePredictBVetoComb"] = TQCut("TwoElLoosePredictBVetoComb", "TwoElLoosePredictBVetoComb", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})+(abs(lep_pdgId[1])==11)*({}))".format(leadel_tight, trailel_tight), "1")
    tqcuts["TwoElTightPredictBVetoComb"] = TQCut("TwoElTightPredictBVetoComb", "TwoElTightPredictBVetoComb", "(nb"+jecvar+"==0)*((abs(lep_pdgId[0])==11)*({})*(!({}))+(abs(lep_pdgId[1])==11)*({})*(!({})))".format(leadel_loose, leadel_tight, trailel_loose, trailel_tight), weight_elcomb)

    tqcuts["TwoMuHLT8"] = TQCut("TwoMuHLT8", "TwoMuHLT8", "(mc_HLT_SingleIsoMu8)*(MllSS>60.)*(MllSS<120.)", "1")
    tqcuts["TwoMuHLT17"] = TQCut("TwoMuHLT17", "TwoMuHLT17", "(mc_HLT_SingleIsoMu17)*(MllSS>60.)*(MllSS<120.)", "1")
    tqcuts["TwoElHLT8"] = TQCut("TwoElHLT8", "TwoElHLT8", "(mc_HLT_SingleIsoEl8)*(MllSS>60.)*(MllSS<120.)", "1")
    tqcuts["TwoElHLT17"] = TQCut("TwoElHLT17", "TwoElHLT17", "(mc_HLT_SingleIsoEl17)*(MllSS>60.)*(MllSS<120.)", "1")

    # Linking TQCut objects
    tqcuts["Presel"].addCut(tqcuts["OneLep"])
    tqcuts["Presel"].addCut(tqcuts["OneLepNoNvtxRewgt"])
    tqcuts["Presel"].addCut(tqcuts["TwoLep"])
    tqcuts["Presel"].addCut(tqcuts["TwoLepOS"])

    tqcuts["OneLep"].addCut(tqcuts["OneLepMR"])
    tqcuts["OneLep"].addCut(tqcuts["OneLepEWKCR"])
    tqcuts["OneLep"].addCut(tqcuts["OneLepEWKCR2"])
    tqcuts["OneLep"].addCut(tqcuts["OneLepEWKCR3"])
    tqcuts["OneLepNoNvtxRewgt"].addCut(tqcuts["OneLepEWKCR3NoNvtxRewgt"])

    tqcuts["OneLepMR"].addCut(tqcuts["OneMu"])
    tqcuts["OneMu"].addCut(tqcuts["OneMuLoose"])
    tqcuts["OneMuLoose"].addCut(tqcuts["OneMuTight"])
    tqcuts["OneMu"].addCut(tqcuts["OneMu3lLoose"])
    tqcuts["OneMu3lLoose"].addCut(tqcuts["OneMu3lTight"])

    tqcuts["OneLepMR"].addCut(tqcuts["OneEl"])
    tqcuts["OneEl"].addCut(tqcuts["OneElLoose"])
    tqcuts["OneElLoose"].addCut(tqcuts["OneElTight"])
    tqcuts["OneEl"].addCut(tqcuts["OneEl3lLoose"])
    tqcuts["OneEl3lLoose"].addCut(tqcuts["OneEl3lTight"])

    tqcuts["OneLepEWKCR"].addCut(tqcuts["OneMuEWKCR"])
    tqcuts["OneLepEWKCR"].addCut(tqcuts["OneElEWKCR"])
    tqcuts["OneMuEWKCR"].addCut(tqcuts["OneMuTightEWKCR"])
    tqcuts["OneElEWKCR"].addCut(tqcuts["OneElTightEWKCR"])
    tqcuts["OneMuEWKCR"].addCut(tqcuts["OneMu3lTightEWKCR"])
    tqcuts["OneElEWKCR"].addCut(tqcuts["OneEl3lTightEWKCR"])

    tqcuts["OneLepEWKCR2"].addCut(tqcuts["OneMuEWKCR2"])
    tqcuts["OneLepEWKCR2"].addCut(tqcuts["OneElEWKCR2"])
    tqcuts["OneMuEWKCR2"].addCut(tqcuts["OneMuTightEWKCR2"])
    tqcuts["OneElEWKCR2"].addCut(tqcuts["OneElTightEWKCR2"])
    tqcuts["OneMuEWKCR2"].addCut(tqcuts["OneMu3lTightEWKCR2"])
    tqcuts["OneElEWKCR2"].addCut(tqcuts["OneEl3lTightEWKCR2"])

    tqcuts["OneLepEWKCR3"].addCut(tqcuts["OneMuEWKCR3"])
    tqcuts["OneLepEWKCR3"].addCut(tqcuts["OneElEWKCR3"])
    tqcuts["OneMuEWKCR3"].addCut(tqcuts["OneMuTightEWKCR3"])
    tqcuts["OneElEWKCR3"].addCut(tqcuts["OneElTightEWKCR3"])
    tqcuts["OneMuEWKCR3"].addCut(tqcuts["OneMu3lTightEWKCR3"])
    tqcuts["OneElEWKCR3"].addCut(tqcuts["OneEl3lTightEWKCR3"])

    tqcuts["OneLepEWKCR3NoNvtxRewgt"].addCut(tqcuts["OneMuEWKCR3NoNvtxRewgt"])
    tqcuts["OneLepEWKCR3NoNvtxRewgt"].addCut(tqcuts["OneElEWKCR3NoNvtxRewgt"])
    tqcuts["OneMuEWKCR3NoNvtxRewgt"].addCut(tqcuts["OneMuTightEWKCR3NoNvtxRewgt"])
    tqcuts["OneElEWKCR3NoNvtxRewgt"].addCut(tqcuts["OneElTightEWKCR3NoNvtxRewgt"])
    tqcuts["OneMuEWKCR3NoNvtxRewgt"].addCut(tqcuts["OneMu3lTightEWKCR3NoNvtxRewgt"])
    tqcuts["OneElEWKCR3NoNvtxRewgt"].addCut(tqcuts["OneEl3lTightEWKCR3NoNvtxRewgt"])

    tqcuts["TwoLep"].addCut(tqcuts["TwoMu"])
    tqcuts["TwoMu"].addCut(tqcuts["TwoMuLoosePredict"])
    tqcuts["TwoMu"].addCut(tqcuts["TwoMuTightPredict"])
    tqcuts["TwoMu"].addCut(tqcuts["TwoMuLoosePredictBVeto"])
    tqcuts["TwoMu"].addCut(tqcuts["TwoMuTightPredictBVeto"])
    tqcuts["TwoMu"].addCut(tqcuts["TwoMuLoose"])
    tqcuts["TwoMuLoose"].addCut(tqcuts["TwoMuTight"])

    tqcuts["TwoLep"].addCut(tqcuts["TwoEl"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredict"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredict"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictComb"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictComb"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictHF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictHF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictEM1DLF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictEM1DLF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictBVeto"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictBVeto"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictBVetoComb"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictBVetoComb"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictBVetoHF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictBVetoHF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoosePredictBVetoEM1DLF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElTightPredictBVetoEM1DLF"])
    tqcuts["TwoEl"].addCut(tqcuts["TwoElLoose"])
    tqcuts["TwoElLoose"].addCut(tqcuts["TwoElTight"])

    tqcuts["TwoLepOS"].addCut(tqcuts["TwoMuHLT8"])
    tqcuts["TwoLepOS"].addCut(tqcuts["TwoMuHLT17"])
    tqcuts["TwoLepOS"].addCut(tqcuts["TwoElHLT8"])
    tqcuts["TwoLepOS"].addCut(tqcuts["TwoElHLT17"])

    # Grand cut
    cuts = tqcuts["Presel"]

    #
    #
    # Define histograms
    #
    #
    # N.B. Any 2D histogram must have "_vs_" in the name. This is an important conventino for the makeplot.py script to be able to distinguish the 1D vs. 2D histogram.
    filename = ".histo.mr.{}.cfg".format(index)
    f = open(filename, "w")
    f.write("""
    TH2F('lep_pt_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{20, 30, 40, 50, 60, 70, 150, 2000}} ) << (abs(lep_eta[0]) : '|\#eta|', lep_pt[0] : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: lep_pt_vs_eta;

    TH2F('lep_ptcorr_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 60., 80., 120.}} ) << (abs(lep_eta[0]) : '|\#eta|', TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.) : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: lep_ptcorr_vs_eta;

    TH2F('lep_ptcorrcoarse_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << (abs(lep_eta[0]) : '|\#eta|', TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.) : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: lep_ptcorrcoarse_vs_eta;

    TH2F('lep_ptcorrcoarse_vs_etacoarse' , '' , {{0, 1.6, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << (abs(lep_eta[0]) : '|\#eta|', TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.) : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: lep_ptcorrcoarse_vs_etacoarse;

    TH2F('el3l_ptcorrcoarse_vs_etacoarse' , '' , {{0, 1.6, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << (abs(lep_eta[0]) : '|\#eta|', TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.05)),119.) : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: el3l_ptcorrcoarse_vs_etacoarse;

    TH2F('mu3l_ptcorrcoarse_vs_etacoarse' , '' , {{0, 1.6, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << (abs(lep_eta[0]) : '|\#eta|', TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.07)),119.) : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: mu3l_ptcorrcoarse_vs_etacoarse;

    TH1F('lep_pt' , '' , 180 , 0. , 250 ) << (lep_pt[0] : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: lep_pt;

    TH1F('lep_pt' , '' , 180 , 0. , 250 ) << (lep_pt[0] : '\#it{{p}}_{{T}} [GeV]');
    @OneLep/*: lep_pt;

    TH1F('lep_pdgId' , '' , 40 , -20. , 20 ) << (lep_pdgId[0] : 'Lepton PDG ID');
    @OneLep/*: lep_pdgId;

    TH1F('lep_ptcorr' , '' , 180 , 0. , 250 ) << (lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)) : '\#it{{p}}_{{T, cone-corr}} [GeV]');
    @OneLep/*: lep_ptcorr;

    TH1F('lep_ptcorrvarbin' , '' , {{0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 60., 80., 120.}}) << (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.) : '\#it{{p}}_{{T, cone-corr}} [GeV]');
    @OneLep/*: lep_ptcorrvarbin;

    TH1F('lep_ptcorrvarbincoarse' , '' , {{0., 10., 20., 25., 30., 40., 120.}}) << (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.) : '\#it{{p}}_{{T, cone-corr}} [GeV]');
    @OneLep/*: lep_ptcorrvarbincoarse;

    TH1F('el3l_ptcorrvarbincoarse' , '' , {{0., 10., 20., 25., 30., 40., 120.}}) << (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.05)),119.) : '\#it{{p}}_{{T, cone-corr}} [GeV]');
    @OneLep/*: el3l_ptcorrvarbincoarse;

    TH1F('mu3l_ptcorrvarbincoarse' , '' , {{0., 10., 20., 25., 30., 40., 120.}}) << (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.07)),119.) : '\#it{{p}}_{{T, cone-corr}} [GeV]');
    @OneLep/*: mu3l_ptcorrvarbincoarse;

    TH1F('lep_yield' , '' , 1, 0, 1) << (0 : 'yield');
    @OneLep/*: lep_yield;

    TH1F('lep_eta' , '' , 180 , -2.5 , 2.5 ) << (lep_eta[0] : '\#eta');
    @OneLep/*: lep_eta;

    TH1F('lep_etavarbin' , '' , {{-2.5, -2.1, -1.6, -1.0, 0.0, 1.0, 1.6, 2.1, 2.5}} ) << (lep_eta[0] : '\#eta');
    @OneLep/*: lep_etavarbin;

    TH1F('lep_relIso03EAv2Lep' , '' , 180 , 0.0 , 0.6 ) << (lep_relIso03EAv2Lep[0] : 'I_{{R=0.3,EA,Lep}}');
    @OneLep/*: lep_relIso03EAv2Lep;

    TH1F('mu_ptcorrvarbin' , '' , {{0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 60., 80., 120.}}) << ((TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==13)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==13) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoMu/*: mu_ptcorrvarbin;

    TH1F('mu_ptcorrvarbincoarse' , '' , {{0., 10., 20., 25., 30., 40., 120.}}) << ((TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==13)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==13) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoMu/*: mu_ptcorrvarbincoarse;

    TH1F('mu_yield' , '' , 1, 0, 1) << (0 : 'yield');
    @TwoMu/*: mu_yield;

    TH2F('mu_ptcorr_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 60., 80., 120.}} ) << ((abs(lep_eta[0]))*(abs(lep_pdgId[0])==13)+(abs(lep_eta[1]))*(abs(lep_pdgId[1])==13) : '|\#eta|', (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==13)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==13) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoMu/*: mu_ptcorr_vs_eta;

    TH2F('mu_ptcorrcoarse_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{0., 10., 20., 25., 30., 40., 60., 120.}} ) << ((abs(lep_eta[0]))*(abs(lep_pdgId[0])==13)+(abs(lep_eta[1]))*(abs(lep_pdgId[1])==13) : '|\#eta|', (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==13)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==13) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoMu/*: mu_ptcorrcoarse_vs_eta;

    TH2F('mu_ptcorrcoarse_vs_etacoarse' , '' , {{0, 1.6, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << ((abs(lep_eta[0]))*(abs(lep_pdgId[0])==13)+(abs(lep_eta[1]))*(abs(lep_pdgId[1])==13) : '|\#eta|', (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==13)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==13) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoMu/*: mu_ptcorrcoarse_vs_etacoarse;

    TH1F('mu_pt' , '' , 180 , 0., 250) << ((lep_pt[0])*(abs(lep_pdgId[0])==13)+(lep_pt[1])*(abs(lep_pdgId[1])==13) : '\#it{{p}}_{{T, \#mu}} [GeV]');
    @TwoMu/*: mu_pt;

    TH1F('mu_eta' , '' , 180 , -2.5, 2.5) << ((lep_eta[0])*(abs(lep_pdgId[0])==13)+(lep_eta[1])*(abs(lep_pdgId[1])==13) : '\#eta_{{\#mu}}');
    @TwoMu/*: mu_eta;

    TH1F('mu_etavarbin' , '' , {{-2.5, -2.1, -1.6, -1.0, 0.0, 1.0, 1.6, 2.1, 2.5}} ) << (lep_eta[0] : '\#eta');
    @TwoMu/*: mu_etavarbin;

    TH1F('mu_relIso03EAv2Lep' , '' , 180 , 0., 0.6) << ((lep_relIso03EAv2Lep[0])*(abs(lep_pdgId[0])==13)+(lep_relIso03EAv2Lep[1])*(abs(lep_pdgId[1])==13) : 'I_{{R=0.3,EA,Lep,\#mu}}');
    @TwoMu/*: mu_relIso03EAv2Lep;

    TH2F('el_ptcorr_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 60., 80., 120.}} ) << ((abs(lep_eta[0]))*(abs(lep_pdgId[0])==11)+(abs(lep_eta[1]))*(abs(lep_pdgId[1])==11) : '|\#eta|', (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==11)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==11) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoEl/*: el_ptcorr_vs_eta;

    TH2F('el_ptcorrcoarse_vs_eta' , '' , {{0, 0.9, 1.6, 1.9, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << ((abs(lep_eta[0]))*(abs(lep_pdgId[0])==11)+(abs(lep_eta[1]))*(abs(lep_pdgId[1])==11) : '|\#eta|', (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==11)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==11) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoEl/*: el_ptcorrcoarse_vs_eta;

    TH2F('el_ptcorrcoarse_vs_etacoarse' , '' , {{0, 1.6, 2.4}}, {{0., 10., 20., 25., 30., 40., 120.}} ) << ((abs(lep_eta[0]))*(abs(lep_pdgId[0])==11)+(abs(lep_eta[1]))*(abs(lep_pdgId[1])==11) : '|\#eta|', (TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==11)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==11) : '\#it{{p}}_{{T, cone-corr, mu}} [GeV]');
    @TwoEl/*: el_ptcorrcoarse_vs_etacoarse;

    TH1F('el_ptcorrvarbin' , '' , {{0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 60., 80., 120.}}) << ((TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==11)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==11) : '\#it{{p}}_{{T, cone-corr, el}} [GeV]');
    @TwoEl/*: el_ptcorrvarbin;

    TH1F('el_ptcorrvarbincoarse' , '' , {{0., 10., 20., 25., 30., 40., 120.}}) << ((TMath::Min(lep_pt[0]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[0]-0.03)),119.))*(abs(lep_pdgId[0])==11)+(TMath::Min(lep_pt[1]*(1.+TMath::Max(0.,lep_relIso03EAv2Lep[1]-0.03)),119.))*(abs(lep_pdgId[1])==11) : '\#it{{p}}_{{T, cone-corr, el}} [GeV]');
    @TwoEl/*: el_ptcorrvarbincoarse;

    TH1F('el_yield' , '' , 1, 0, 1) << (0 : 'yield');
    @TwoEl/*: el_yield;

    TH1F('el_pt' , '' , 180 , 0., 250) << ((lep_pt[0])*(abs(lep_pdgId[0])==11)+(lep_pt[1])*(abs(lep_pdgId[1])==11) : '\#it{{p}}_{{T, el}} [GeV]');
    @TwoEl/*: el_pt;

    TH1F('el_eta' , '' , 180 , -2.5, 2.5) << ((lep_eta[0])*(abs(lep_pdgId[0])==11)+(lep_eta[1])*(abs(lep_pdgId[1])==11) : '\#eta_{{el}}');
    @TwoEl/*: el_eta;

    TH1F('el_etavarbin' , '' , {{-2.5, -2.1, -1.6, -1.0, 0.0, 1.0, 1.6, 2.1, 2.5}} ) << (lep_eta[0] : '\#eta');
    @TwoEl/*: el_etavarbin;

    TH1F('el_relIso03EAv2Lep' , '' , 180 , 0., 0.6) << ((lep_relIso03EAv2Lep[0])*(abs(lep_pdgId[0])==11)+(lep_relIso03EAv2Lep[1])*(abs(lep_pdgId[1])==11) : 'I_{{R=0.3,EA,Lep,el}}');
    @TwoEl/*: el_relIso03EAv2Lep;

    TH1F('Mjj_el' , '' , 180 , 0., 180. ) << ({Mjj} : '\#it{{m}}_{{jj}} [GeV]');
    @TwoEl/*: Mjj_el;

    TH1F('Mjj_mu' , '' , 180 , 0., 180. ) << ({Mjj} : '\#it{{m}}_{{jj}} [GeV]');
    @TwoMu/*: Mjj_mu;

    TH1F('Mll_el' , '' , 180 , 0., 180. ) << (MllSS : '\#it{{m}}_{{ll}} [GeV]');
    @TwoEl/*: Mll_el;

    TH1F('Mll_mu' , '' , 180 , 0., 180. ) << (MllSS : '\#it{{m}}_{{ll}} [GeV]');
    @TwoMu/*: Mll_mu;

    TH1F('DPhill_el' , '' , 180 , 0., 3.1416 ) << (TMath::Abs(TVector2::Phi_mpi_pi(lep_phi[0]-lep_phi[1])) : '\#it{{m}}_{{ll}} [GeV]');
    @TwoEl/*: DPhill_el;

    TH1F('DPhill_mu' , '' , 180 , 0., 3.1416 ) << (TMath::Abs(TVector2::Phi_mpi_pi(lep_phi[0]-lep_phi[1])) : '\#it{{m}}_{{ll}} [GeV]');
    @TwoMu/*: DPhill_mu;

    TH1F('MET_el' , '' , 180 , 0., 180. ) << ({MET} : 'MET [GeV]');
    @TwoEl/*: MET_el;

    TH1F('MET_mu' , '' , 180 , 0., 180. ) << ({MET} : 'MET [GeV]');
    @TwoMu/*: MET_mu;

    TH1F('MTmax_el' , '' , 180 , 0., 180. ) << ({MTmax} : '\#it{{m}}_{{T,max}} [GeV]');
    @TwoEl/*: MTmax_el;

    TH1F('MTmax_mu' , '' , 180 , 0., 180. ) << ({MTmax} : '\#it{{m}}_{{T,max}} [GeV]');
    @TwoMu/*: MTmax_mu;

    TH1F('nb_el' , '' , 5, 0., 5.) << ({nb} : 'N_{{b-jets}}');
    @TwoEl/*: nb_el;

    TH1F('nb_mu' , '' , 5, 0., 5.) << ({nb} : 'N_{{b-jets}}');
    @TwoMu/*: nb_mu;

    TH1F('nj30_el' , '' , 5, 0., 5.) << ({nj30} : 'N_{{jets,30,cent}}');
    @TwoEl/*: nj30_el;

    TH1F('nj30_mu' , '' , 5, 0., 5.) << ({nj30} : 'N_{{jets,30,cent}}');
    @TwoMu/*: nj30_mu;

    TH1F('nj_el' , '' , 5, 0., 5.) << ({nj} : 'N_{{jets,all}}');
    @TwoEl/*: nj_el;

    TH1F('nj_mu' , '' , 5, 0., 5.) << ({nj} : 'N_{{jets,all}}');
    @TwoMu/*: nj_mu;

    TH1F('Mll_Z' , '' , 180 , 60., 120. ) << (MllSS : '\#it{{m}}_{{ll}} [GeV]');
    @TwoLepOS/*: Mll_Z;

    TH1F('MTOneLep' , '' , 180 , 0., 180. ) << ({MT} : '\#it{{m}}_{{T}} [GeV]');
    @*/*: MTOneLep;

    TH1F('MTOneLepFixed' , '' , 20 , 0., 200. ) << ({MT} : '\#it{{m}}_{{T}} [GeV]');
    @*/*: MTOneLepFixed;

    TH1F('nvtx' , '' , 60 , 0., 60. ) << (nVert : 'N_{{vtx}}');
    @*/*: nvtx;

    """.format(Mjj="Mjj"+jecvar, MET="met"+jecvar+"_pt", nb="nb"+jecvar, nj30="nj30"+jecvar, nj="nj"+jecvar, MT=MTexpr, MTmax="MTmax"+jecvar))
    f.close()

    #
    #
    # Book Analysis Jobs (Histogramming, Cutflow, Event lists)
    #
    #
    histojob = TQHistoMakerAnalysisJob()
    histojob.importJobsFromTextFiles(filename, cuts, "*", True if index < 0 else False)

    # Analysis jobs
    cutflowjob = TQCutflowAnalysisJob("cutflow")
    cuts.addAnalysisJob(cutflowjob, "*")

    # Eventlist jobs (use this if we want to print out some event information in a text format e.g. run, lumi, evt or other variables.)
    #eventlistjob = TQEventlistAnalysisJob("eventlist")
    #eventlistjob.importJobsFromTextFiles("eventlist.cfg", cuts, "*", True)

    # Print cuts and numebr of booked analysis jobs for debugging purpose
    if index < 0:
        samples.printContents("t[*status]dr")
        cuts.printCut("trd")
        return

    #
    #
    # Add custom tqobservable that can do more than just string based draw statements
    #
    #
    from QFramework import TQWWWMTOneLep, TQWWWClosureEvtType
    customobservables = {}
    customobservables["MTOneLep"] = TQWWWMTOneLep("MTOneLep")
    customobservables["ClosureEvtType"] = TQWWWClosureEvtType("ClosureEvtType")
    TQObservable.addObservable(customobservables["MTOneLep"], "MTOneLep")
    TQObservable.addObservable(customobservables["ClosureEvtType"], "ClosureEvtType")

    #
    #
    # Loop over the samples
    #
    #

    # setup a visitor to actually loop over ROOT files
    vis = TQAnalysisSampleVisitor(cuts,True)
    #vis.setMaxEvents(30000) # to debug by restricting the looping to 30k max events

    if index >= 0:

        # Get all sample lists
        sample_names, sample_full_names = getSampleLists(samples)

        # Select the job based on the index
        sample_name = sample_names[index]
        sample_full_name = sample_full_names[sample_name]

        # Run the job!
        samples.visitSampleFolders(vis, "/*/{}".format(sample_full_name))

        # Write the output histograms and cutflow cut values and etc.
        samples.writeToFile(".output_{}.root".format(sample_name), True)

    else:
        # Run the job!
        samples.visitSampleFolders(vis)

        # Write the output histograms and cutflow cut values and etc.
        samples.writeToFile("output.root", True)

if __name__ == "__main__":

    try:
        mode = int(sys.argv[1]) # mode determines which selection to run for QCD
    except:
        print "Usage:"
        print ""
        print " python {} MODE [DONOTRUN]".format(sys.argv[0])
        print ""
        print "  MODE determines which QCD selection run"
        print "      = 0 (default) runs nominal variation"
        print "      = 1           runs JEC up"
        print "      = 2           runs JEC dn"
        print "  type anything as 2nd argument to not run but just print the samples/cuts/histogram defn."
        print ""
        print "NOTE : Running with default mode of MODE=0!"
        print "NOTE : Running with default mode of MODE=0!"
        print "NOTE : Running with default mode of MODE=0!"
        print "NOTE : Running with default mode of MODE=0!"
        mode = 0

    donotrun = len(sys.argv) >= 3

    # Delete previous remnants
    os.system("rm -f .output_*.root")
    os.system("rm -f .histo.mr.*.cfg")

    import multiprocessing

    samples = TQSampleFolder("samples")
    connectNtuples(samples, samplescfgpath, nfspath, ">4", ">5")
    # Get all sample lists
    sample_names, sample_full_names = getSampleLists(samples)
    njobs = len(sample_names)

    if donotrun:
        main(-1, mode, donotrun)
        sys.exit()

    jobs = []
    for i in range(njobs):
        p = multiprocessing.Process(target=main, args=(i, mode, donotrun,))
        jobs.append(p)
        p.start()

    for index, job in enumerate(jobs):
        #print "{} jobs done out of {}".format(index, len(jobs))
        job.join()

    # Determine JEC mode
    jecvar = ""
    if mode == 1: jecvar = "_up"
    if mode == 2: jecvar = "_dn"

    os.system("rooutil/qframework/share/tqmerge -o output"+jecvar+".root -t analysis .output_*.root")
    os.system("rm .output_*.root")
    os.system("rm .histo.mr.*.cfg")

#eof
