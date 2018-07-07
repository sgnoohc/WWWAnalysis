#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob, TQHWWPlotter, TQEventlistAnalysisJob
from rooutil.qutils import *
sys.path.append("..")

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

    # Analyze
    loop(options)

    # Create plots and tables
    samples = TQSampleFolder.loadSampleFolder("outputs/output.root:samples")
    autoplot (samples,          bkg_path={"WWW":"/sig/www", "WHWWW":"/sig/whwww"}, options={"remove_underflow": True})
    autotable(samples, "yield", bkg_path={"WWW":"/sig/www", "WHWWW":"/sig/whwww"}, options={"cuts": "cuts.cfg"})

if __name__ == "__main__":

    main()

