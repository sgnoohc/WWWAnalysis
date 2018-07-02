#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob
from rooutil import plottery_wrapper as p
from plottery import plottery as ply
from rooutil.syncfiles.pyfiles.errors import E
import math

##############################################################################
def main():

    f = ROOT.TFile("statinputs/hist_sm.root")

    processes = get_processes(f)
    systs_up = get_systs_up(f, processes)
    systs_dn = get_systs_dn(f, processes)
    rates_up, rates_dn = get_rates(f, processes, systs_up, systs_dn)
    compute_total_bkg(rates_up, rates_dn)
    print_order = ["qflip", "photon", "fake", "lostlep", "prompt", "total", "www"]
    for process in print_order:
        print '{:30s}'.format(nice_name(process)), ' '.join('& ${:4.1f}^{{+{:4.1f}}}_{{-{:4.1f}}}$'.format(p.val if p.val > 0 else 0, u.err if p.val > 0 else 0, d.err if p.val > 0 else 0) for p, u, d in zip(rates_up[process], rates_up[process], rates_dn[process])), '\\\\'
        #print '{:30s}'.format(nice_name(process)), ' '.join('{:6.3f} +{:6.3f} -{:6.3f}'.format(p.val if p.val > 0 else 0, u.err if p.val > 0 else 0, d.err if p.val > 0 else 0) for p, u, d in zip(rates_up[process], rates_up[process], rates_dn[process])), '\\\\'

##############################################################################
def get_processes(f):
    processes = []
    for key in f.GetListOfKeys():
        name = str(key.GetName())
        if name.find("_") == -1:
            processes.append(name)
    return processes

##############################################################################
def get_systs(f, processes, var):
    systs = {}
    for process in processes:
        systs[process] = []
    for key in f.GetListOfKeys():
        name = str(key.GetName())
        process = name.split("_", 1)[0] if name.find("_") != -1 else name
        systvar = name.split("_", 1)[1] if name.find("_") != -1 else ""
        if systvar != "" and systvar.find(var) != -1:
            systs[process].append(systvar)
    return systs

##############################################################################
def get_systs_up(f, processes): return get_systs(f, processes, "Up")
def get_systs_dn(f, processes): return get_systs(f, processes, "Down")

##############################################################################
def get_rates(f, processes, systs_up, systs_dn):
    rates_up = {}
    rates_dn = {}
    for process in processes:
        h_nom = f.Get(process).Clone(process)
        if process == "prompt": add_vbs_ttw(f, h_nom)
        rates_up[process] = [ E(h_nom.GetBinContent(ibin), 0) for ibin in xrange(1, h_nom.GetNbinsX() + 1) ]
        rates_dn[process] = [ E(h_nom.GetBinContent(ibin), 0) for ibin in xrange(1, h_nom.GetNbinsX() + 1) ]
        for syst in systs_up[process]:
            h = f.Get(process + "_" + syst)
            if process == "prompt": add_vbs_ttw(f, h, syst)
            rates_up[process] = [ x + E(0, abs(x.val - h.GetBinContent(i + 1))) for i, x in enumerate(rates_up[process]) ]
        for syst in systs_dn[process]:
            h = f.Get(process + "_" + syst)
            if process == "prompt": add_vbs_ttw(f, h, syst)
            rates_dn[process] = [ x + E(0, abs(x.val - h.GetBinContent(i + 1))) for i, x in enumerate(rates_dn[process]) ]
    return rates_up, rates_dn

##############################################################################
def add_vbs_ttw(f, h_nom, syst=""):
    suffix = "_" + syst if syst != "" else ""
    vbs_ww = f.Get("vbsww"+suffix.replace("prompt", "vbsww")).Clone()
    ttw    = f.Get("ttw"+suffix.replace("prompt", "ttw")).Clone()
    h_nom.Add(vbs_ww)
    h_nom.Add(ttw)

##############################################################################
def nice_name(process):
    if process == "www"     : return "WWW signal"
    if process == "prompt"  : return "Irreducible"
    if process == "lostlep" : return "Lost/three $\ell$"
    if process == "fake"    : return "Non-prompt $\ell$"
    if process == "qflip"   : return "charge flips"
    if process == "photon"  : return "$\gamma\\to$non-prompt $\ell$"
    if process == "total"   : return "Background sum"
    print "why are you here?"
    return ""

##############################################################################
def compute_total_bkg(rates_up, rates_dn):
    totalbkg_up = [ E(0,0) for i in xrange(9) ]
    totalbkg_dn = [ E(0,0) for i in xrange(9) ]
    for process in rates_up:
        if process == "vbsww": continue
        if process == "ttw": continue
        if process == "www": continue
        totalbkg_up = [ a + b for a, b in zip(totalbkg_up, rates_up[process]) ]
        totalbkg_dn = [ a + b for a, b in zip(totalbkg_dn, rates_dn[process]) ]
    rates_up["total"] = totalbkg_up
    rates_dn["total"] = totalbkg_dn


if __name__ == "__main__":

    main()
