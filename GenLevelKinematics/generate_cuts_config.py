#!/bin/env python

import os
import sys
import ROOT
from QFramework import *
from rooutil.qutils import *

def main():

    tqcuts = {}

    # Create cuts
    tqcuts["Root"]                = TQCut("Root"                , "Root"                                         , "1"                , "evt_scale1fb")
    tqcuts["SameSignDecay"]       = TQCut("SameSignDecay"       , "WWW events with gen-level same-sign decay"    , "www_channel == 2" , "1")
    tqcuts["SameSignDecayHighPt"] = TQCut("SameSignDecayHighPt" , "The W->jj boson has pt #geq 150 GeV"          , "q_w_pt[0] >= 150" , "1")
    tqcuts["ThreeLeptonDecay"]    = TQCut("ThreeLeptonDecay"    , "WWW events with gen-level three-lepton decay" , "www_channel == 3" , "1")

    # Build selection tree
    tqcuts["Root"].addCut(tqcuts["SameSignDecay"])
    tqcuts["Root"].addCut(tqcuts["ThreeLeptonDecay"])

    tqcuts["SameSignDecay"].addCut(tqcuts["SameSignDecayHighPt"])

    exportTQCutsToTextFile(tqcuts["Root"], "cuts.cfg")

    cuts = loadTQCutsFromTextFile("cuts.cfg")

    cuts.printCuts("trd")

if __name__ == "__main__":

    main()
