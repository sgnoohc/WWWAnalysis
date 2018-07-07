#!/bin/env python

import os
import sys
import ROOT
ROOT.gROOT.SetBatch(True)
from QFramework import *
from rooutil import plottery_wrapper as p
from plottery import plottery as ply

#_____________________________________________________________________________________
def main():

    samples = TQSampleFolder.loadSampleFolder("outputs/output.root:samples")

    plot(samples, "SameSignDecay/dRllSS"      , "plots/SameSignDecay_dRllSS"      )
    plot(samples, "SameSignDecayHighPt/dRllSS" , "plots/SameSignDecayHighPt_dRllSS" )
    plot(samples, "SameSignDecay/dRqqSS"      , "plots/SameSignDecay_dRqqSS"      )
    plot(samples, "SameSignDecayHighPt/dRqqSS" , "plots/SameSignDecayHighPt_dRqqSS" )
    plot(samples, "ThreeLeptonDecay/Mll_higgs", "plots/ThreeLeptonDecay_Mll_higgs")
    plot(samples, "ThreeLeptonDecay/MT_higgs", "plots/ThreeLeptonDecay_MT_higgs")
    plot(samples, "ThreeLeptonDecay/DPhill_higgs", "plots/ThreeLeptonDecay_DPhill_higgs")

#_____________________________________________________________________________________
def plot(samples, histname, output_name, systs=None, options={}, plotfunc=p.plot_hist):
    # Options
    alloptions= {
                "ratio_range":[0.0,2.0],
                "nbins": 45,
                "autobin": False,
                "legend_scalex": 1.4,
                "legend_scaley": 1.1,
                "output_name": "{}.pdf".format(output_name)
                }
    alloptions.update(options)
    bgs  = [
            p.remove_underflow(samples.getHistogram("/sig/www", histname).Clone("WWW")),
            samples.getHistogram("/sig/whwww", histname).Clone("WHWWW")
            ]
    sigs = []
    data = None
    colors = [ 2005, 2001, 2012, 2003, 920, 2007 ]
    plotfunc(
            sigs = sigs,
            bgs  = bgs,
            data = data,
            colors = colors,
            syst = systs,
            options=alloptions)

if __name__ == "__main__":

    main()
