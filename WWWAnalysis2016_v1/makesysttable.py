#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob
from rooutil import plottery_wrapper as p
from plottery import plottery as ply
import math

##############################################################################
def main():

    f = ROOT.TFile("statinputs/hist_sm.root")

    hist_name_pairs = []
    for key in f.GetListOfKeys():
        name = str(key.GetName())
        if name.find("data_obs") != -1: continue # data doesn't have systematics...
        if name.find("vbsww") != -1: continue # skip vbs ww and handle later when looping over prompt
        if name.find("ttw") != -1: continue # skip ttw and handle later when looping over prompt
        process = name.split("_", 1)[0] if name.find("_") != -1 else name
        systvar = name.split("_", 1)[1] if name.find("_") != -1 else ""
        if systvar != "" and systvar.find("Up") != -1:
            systvar = systvar.replace("Up", "")
            hist_name_pairs.append((process, systvar))

    systs = {}
    for proc, syst in hist_name_pairs:
        minval, maxval = get_min_max_syst(f, proc, syst)
        key = (proc, syst_name(syst))
        if key in systs:
            if systs[key][0] > minval and minval != 0: systs[key][0] = minval
            if systs[key][1] < maxval and maxval != 0: systs[key][1] = maxval
        else:
            systs[key] = [minval, maxval]

    # Text friendly output
    #for syst in sorted(systs):
    #    print "{:<25s}".format(syst[0] + "_" + syst[1]), "{:>10s}".format(nice_format_range(*systs[syst]))

    def g(proc, syst):
        if syst == "---": return syst
        if syst == "FILL ME": return syst
        if syst.find("\%") != -1: return syst
        return nice_format_range(*systs[(proc, syst)])

    def s(s1, s2, s3, s4, s5):
        return "& {:>12s} & {:>12s} & {:>12s} & {:>12s} & {:>12s} \\\\".format(g("lostlep", s1), g("fake", s2), g("photon", s3), g("qflip", s4), g("prompt", s5))

    n = "---"
    t = "FILL ME"

    # Control data statistics
    print "Control data statistics              {}" .format(s("CRstat"     , "ARstat"        , n        , n        , n          ))
    print "Simulation statistics                {}" .format(s("stat"       , n               , "stat"   , "stat"   , "stat"     ))
    print "Control region contamination         {}" .format(s("$<1$--2\%"  , "4--40\%"       , n        , n        , n          ))
    print "\Mjj modeling (SS only)              {}" .format(s("4.3\%"      , n               , n        , n        , n          ))
    print "\MSFOS extrapolation                 {}" .format(s("5.3--8.2\%" , n               , n        , n        , n          ))
    print "\\ttZ/WZ fraction                     {}".format(s("$<1\%$"     , n               , n        , n        , n          ))
    print "Fake rate measurement (e)            {}" .format(s(n            , "FakeRateEl"    , n        , n        , n          ))
    print "Fake rate measurement (\mu)          {}" .format(s(n            , "FakeRateMu"    , n        , n        , n          ))
    print "Validation of fake rate method (e)   {}" .format(s(n            , "FakeClosureEl" , n        , n        , n          ))
    print "Validation of fake rate method (\mu) {}" .format(s(n            , "FakeClosureMu" , n        , n        , n          ))
    print "Fake rate measurement                {}" .format(s(n            , "FakeRate"      , n        , n        , n          ))
    print "Validation of fake rate method       {}" .format(s(n            , "FakeClosure"   , n        , n        , n          ))
    print "Lepton reconstruction efficiency     {}" .format(s("$<1$--1\%"  , n               , "LepSF"  , "LepSF"  , "LepSF"    ))
    print "Lepton energy resolution             {}" .format(s(n            , n               , n        , n        , n          ))
    print "JEC uncertainties                    {}" .format(s("$<1$--6\%"  , n               , "JEC"    , "JEC"    , "JEC"      ))
    print "b-tagging SF (light-flavor)          {}" .format(s(n            , n               , "BTagLF" , "BTagLF" , "BTagLF"   ))
    print "b-tagging SF (heavy-flavor)          {}" .format(s(n            , n               , "BTagHF" , "BTagHF" , "BTagHF"   ))
    print "Pile-up reweighting                  {}" .format(s("$<1$--8\%"  , n               , "Pileup" , "Pileup" , "Pileup"   ))
    print "Trigger SF                           {}" .format(s(n            , n               , "TrigSF" , "TrigSF" , "TrigSF"   ))
    print "Luminosity                           {}" .format(s(n            , n               , "2.5\%"  , "2.5\%"  , "2.5\%"    ))
    print "Cross section measurement            {}" .format(s(n            , n               , n        , n        , "20\%"     ))
    print "Validation uncertainty               {}" .format(s(n            , n               , "50\%"   , "50\%"   , "18--22\%" ))

##############################################################################
def get_min_max_syst(f, process, systvar):
    nominal = f.Get(process).Clone() # Crucial to clone it otherwise accumulates in each loop
    syst_up = f.Get(process + "_" + systvar + "Up").Clone() # Crucial to clone it otherwise accumulates in each loop
    syst_dn = f.Get(process + "_" + systvar + "Down").Clone() # Crucial to clone it otherwise accumulates in each loop
    if process == "prompt": #
        add_vbsww_and_ttw(f, process, systvar, nominal, syst_up, syst_dn)
    frac_errors = []
    for i in xrange(1,syst_up.GetNbinsX()+1):
        if i == 1: continue
        if i == 4: continue
        if i == 5: continue
        if i == 6: continue
        if i == 8: continue
        nom = nominal.GetBinContent(i)
        upe = syst_up.GetBinContent(i)
        dne = syst_dn.GetBinContent(i)
        aupe = abs(upe-nom) / nom if nom != 0 else 0
        adne = abs(dne-nom) / nom if nom != 0 else 0
        if aupe != 0: frac_errors.append(aupe)
        if adne != 0: frac_errors.append(adne)
    min_frac_error = sorted(frac_errors)[0]  if len(frac_errors) > 0 else 0
    max_frac_error = sorted(frac_errors)[-1] if len(frac_errors) > 0 else 0
    return min_frac_error, max_frac_error
#
#
#    nice_format_error = nice_format_range(min_frac_error, max_frac_error)
#
#    print "{:<35s} {:>9s}".format(process + "_" + systvar, nice_format_error)

##############################################################################
def nice_format(number):
    if number < 0.01:
        nice_format = "$<1$"
    else:
        nice_format = "{:.0f}".format(number*100)
    return nice_format

##############################################################################
def nice_format_range(minval, maxval):
    nf_min = nice_format(minval)
    nf_max = nice_format(maxval)

    if nf_min == nf_max:
        return "{}\%".format(nf_min)
    else:
        return "{}--{}\%".format(nf_min, nf_max)

##############################################################################
def syst_name(syst):
    if syst.find("_stat_") != -1:
        return "stat"
    if syst.find("_CRstat_") != -1:
        return "CRstat"
    if syst.find("_ARstat_") != -1:
        return "ARstat"
    return syst

##############################################################################
def add_vbsww_and_ttw(f, process, systvar, nominal, syst_up, syst_dn):
    nominal_vbsww = f.Get("vbsww")
    nominal_ttw   = f.Get("ttw")
    syst_up_vbsww = f.Get(process.replace("prompt","vbsww") + "_" + systvar.replace("prompt","vbsww") + "Up")
    syst_dn_vbsww = f.Get(process.replace("prompt","vbsww") + "_" + systvar.replace("prompt","vbsww") + "Down")
    syst_up_ttw   = f.Get(process.replace("prompt","ttw")   + "_" + systvar.replace("prompt","ttw")   + "Up")
    syst_dn_ttw   = f.Get(process.replace("prompt","ttw")   + "_" + systvar.replace("prompt","ttw")   + "Down")
    for i in xrange(1,nominal.GetNbinsX()+1):
        nom       = nominal      .GetBinContent(i)
        nom_vbsww = nominal_vbsww.GetBinContent(i)
        nom_ttw   = nominal_ttw  .GetBinContent(i)
        eup       = syst_up      .GetBinContent(i)
        eup_vbsww = syst_up_vbsww.GetBinContent(i)
        eup_ttw   = syst_up_ttw  .GetBinContent(i)
        edn       = syst_dn      .GetBinContent(i)
        edn_vbsww = syst_dn_vbsww.GetBinContent(i)
        edn_ttw   = syst_dn_ttw  .GetBinContent(i)
        new_nom = nom + nom_vbsww + nom_ttw
        new_eup = new_nom + math.sqrt(abs(nom-eup)**2 + abs(nom_vbsww-eup_vbsww)**2 + abs(nom_ttw-eup_ttw)**2)
        new_edn = new_nom - math.sqrt(abs(nom-edn)**2 + abs(nom_vbsww-edn_vbsww)**2 + abs(nom_ttw-edn_ttw)**2)
        #new_nom = nom_vbsww + nom_ttw
        #new_eup = new_nom + math.sqrt(abs(nom_vbsww-eup_vbsww)**2 + abs(nom_ttw-eup_ttw)**2)
        #new_edn = new_nom - math.sqrt(abs(nom_vbsww-edn_vbsww)**2 + abs(nom_ttw-edn_ttw)**2)
        nominal.SetBinContent(i, new_nom)
        syst_up.SetBinContent(i, new_eup)
        syst_dn.SetBinContent(i, new_edn)

if __name__ == "__main__":

    main()
