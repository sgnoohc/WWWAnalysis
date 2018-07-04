#!/bin/env python

import os
import sys
import ROOT
from QFramework import TQSampleFolder, TQXSecParser, TQCut, TQAnalysisSampleVisitor, TQSampleInitializer, TQCutflowAnalysisJob, TQCutflowPrinter, TQHistoMakerAnalysisJob
from rooutil.qutils import *
from errors import E
from cuts import systvars
import math

ROOT.gROOT.SetBatch(True)

#########################################################################################################################################################
def main(model="sm", mass0=-1, mass1=-1):

    # Print the model name and mass points
    print model, mass0, mass1

    # Suffis that will be attached to output file names for bookkeeping
    suffix = make_suffix(model, mass0, mass1)

    # Create directory where the outputs will be
    makedir("statinputs")

    # Open input files
    filename = "output_sf_applied.root"
    samples = TQSampleFolder.loadSampleFolder("{}:samples".format(filename))
    samples_jec_up = TQSampleFolder.loadSampleFolder("output_jec_up.root:samples")
    samples_jec_dn = TQSampleFolder.loadSampleFolder("output_jec_dn.root:samples")
    samples_gen_met = TQSampleFolder.loadSampleFolder("output_gen_met.root:samples")

    # Set the histogram name to perform the fit on (we use the 9 bin histogram
    histname = "{SRSSeeFull,SRSSemFull,SRSSmmFull,SideSSeeFull,SideSSemFull,SideSSmmFull,SR0SFOSFull,SR1SFOSFull,SR2SFOSFull}"

    # We have 8 categories for the fit
    processes = [ "vbsww" , "ttw" , "lostlep" , "photon" , "qflip" , "prompt" , "fake" , "www" ,]

    #######
    # NOTE "www" means "signal" - i.e. for whsusy model www = whsusy and www is included in prompt
    #######

    # Set the diectionary of the paths where we will retrieve the histograms from
    sampledirpaths = {
            "vbsww"   : "/typebkg/?/VBSWW",
            "ttw"     : "/typebkg/?/ttW",
            "lostlep" : "/typebkg/lostlep/[ttZ+WZ+Other]",
            "photon"  : "/typebkg/photon/[ttZ+WZ+Other]",
            "qflip"   : "/typebkg/qflip/[ttZ+WZ+Other]",
            "prompt"  : "/typebkg/prompt/[ttZ+WZ+Other]" if model == "sm" else "/typebkg/prompt/[ttZ+WZ+Other]+sig",
            "fake"    : "/fake",
            "www"     : get_sigmodel_path(model, mass0, mass1),
            }

    # Create output file
    ofile = ROOT.TFile("statinputs/hist_{}.root".format(suffix), "recreate")
    ofile.cd()

    # Array of numbers where we will aggregate some results for nice tables
    rates = {}

    # Write histograms
    for process in processes:
        #print process, histname

        # Get nominal histogram
        h_nom = samples.getHistogram(sampledirpaths[process], histname).Clone(process)

        # If lost lepton get the nominal number directly from the AN Table 13
        if process == "lostlep": h_nom = set_to_lostlep_nominal_hist(h_nom)

        # If whsusy model with signal then get the average of the two histogram
        if model == "whsusy" and process == "www": set_to_average_and_write_genmet_syst_hist(h_nom, samples_gen_met.getHistogram(sampledirpaths[process], histname).Clone(process))

        # Write nominal histogram
        h_nom.Write()

        # Save the total number that will be used to output to datacards
        rates[process] = h_nom.Integral()

        # Nominal histogram errors are needed to be varied one by one to create an effective uncorrelated histograms
        if process != "lostlep" and process != "fake": write_nominal_stat_variations(h_nom, process)

        # Write systematic histograms that are from weight variations
        for systvar in systvars:

            # Some process or some variations do not need to be written
            if do_not_write_syst_hist(process, systvar, model):
                continue

            # Write the systvariation histograms
            samples.getHistogram(sampledirpaths[process], histname.replace("Full", "Full" + systvar)).Clone(process + "_" + systvar).Write()

        # JEC systematic histograms need to be called from a different sample output
        if process != "fake" and process != "lostlep":
            samples_jec_up.getHistogram(sampledirpaths[process], histname).Clone(process + "_JECUp").Write()
            samples_jec_dn.getHistogram(sampledirpaths[process], histname).Clone(process + "_JECDown").Write()

        # Lost lepton has special treatment
        if process == "lostlep":
            #write_lostlep_stat_variations(h_nom)
            #write_lostlep_syst_variations(h_nom)
            write_lostlep_CRstat_variations(h_nom)
            write_lostlep_TFstat_variations(h_nom)
            write_lostlep_TFsyst_variations(h_nom)
            write_lostlep_Mjjsyst_variations(h_nom)
            write_lostlep_MllSSsyst_variations(h_nom)
            write_lostlep_Mll3lsyst_variations(h_nom)

        # WWW signal theory systematics
        if model == "sm":
            if process == "www":
                write_www_theory_syst_variations(h_nom)

        # Fake has AR statistics
        if process == "fake":
            write_fake_ARstat_variations(h_nom)

    # Write data histogram
    h_data = samples.getHistogram("/data", histname).Clone("data_obs")
    h_data.Write()

    datacard="""imax 1 number of bins
jmax * number of processes
kmax * number of nuisance parameters
----------------------------------------------------------------------------------------------------------------------------------
shapes * * statinputs/hist_{}.root $PROCESS $PROCESS_$SYSTEMATIC
----------------------------------------------------------------------------------------------------------------------------------
bin          SR
observation  {:.1f}
----------------------------------------------------------------------------------------------------------------------------------
bin                                     SR           SR           SR           SR           SR           SR           SR           SR
process                                 0            1            2            3            4            5            6            7
process                                 www          fake         photon       lostlep      qflip        prompt       ttw          vbsww
rate                                    {:<6.3f}       {:<6.3f}       {:<6.3f}       {:<6.3f}       {:<6.3f}       {:<6.3f}       {:<6.3f}       {:<6.3f}
----------------------------------------------------------------------------------------------------------------------------------
JEC                     shape           1            -            1            -            -            1            1            1
LepSF                   shape           1            -            1            -            -            1            1            1
TrigSF                  shape           1            -            1            -            -            1            1            1
BTagHF                  shape           1            -            1            -            -            1            1            1
BTagLF                  shape           1            -            1            -            -            1            1            1
Pileup                  shape           1            -            1            -            -            1            1            1
FakeRateEl              shape           -            1            -            -            -            -            -            -
FakeRateMu              shape           -            1            -            -            -            -            -            -
FakeClosureEl           shape           -            1            -            -            -            -            -            -
FakeClosureMu           shape           -            1            -            -            -            -            -            -
LostLepSyst             shape           -            -            -            1            -            -            -            -
MjjModeling             shape           -            -            -            1            -            -            -            -
MllSSModeling           shape           -            -            -            1            -            -            -            -
Mll3lModeling           shape           -            -            -            1            -            -            -            -
SigXSec                 lnN             1.06         -            -            -            -            -            -            -
LumSyst                 lnN             1.025        -            1.025        -            1.025        1.025        1.025        1.025
vbsww_xsec              lnN             -            -            -            -            -            -            -            1.20
vbsww_validation        lnN             -            -            -            -            -            -            -            1.22
ttw_xsec                lnN             -            -            -            -            -            -            1.20         -
ttw_validation          lnN             -            -            -            -            -            -            1.18         -
photon_syst             lnN             -            -            1.50         -            -            -            -            -
qflip_syst              lnN             -            -            -            -            1.50         -            -            -
www_stat_in_ee          shape           1            -            -            -            -            -            -            -
www_stat_in_em          shape           1            -            -            -            -            -            -            -
www_stat_in_mm          shape           1            -            -            -            -            -            -            -
www_stat_out_ee         shape           1            -            -            -            -            -            -            -
www_stat_out_em         shape           1            -            -            -            -            -            -            -
www_stat_out_mm         shape           1            -            -            -            -            -            -            -
www_stat_0sfos          shape           1            -            -            -            -            -            -            -
www_stat_1sfos          shape           1            -            -            -            -            -            -            -
www_stat_2sfos          shape           1            -            -            -            -            -            -            -
fake_ARstat_in_ee       shape           -            1            -            -            -            -            -            -
fake_ARstat_in_em       shape           -            1            -            -            -            -            -            -
fake_ARstat_in_mm       shape           -            1            -            -            -            -            -            -
fake_ARstat_out_ee      shape           -            1            -            -            -            -            -            -
fake_ARstat_out_em      shape           -            1            -            -            -            -            -            -
fake_ARstat_out_mm      shape           -            1            -            -            -            -            -            -
fake_ARstat_0sfos       shape           -            1            -            -            -            -            -            -
fake_ARstat_1sfos       shape           -            1            -            -            -            -            -            -
fake_ARstat_2sfos       shape           -            1            -            -            -            -            -            -
photon_stat_in_ee       shape           -            -            1            -            -            -            -            -
photon_stat_in_em       shape           -            -            1            -            -            -            -            -
photon_stat_in_mm       shape           -            -            1            -            -            -            -            -
photon_stat_out_ee      shape           -            -            1            -            -            -            -            -
photon_stat_out_em      shape           -            -            1            -            -            -            -            -
photon_stat_out_mm      shape           -            -            1            -            -            -            -            -
photon_stat_0sfos       shape           -            -            1            -            -            -            -            -
photon_stat_1sfos       shape           -            -            1            -            -            -            -            -
photon_stat_2sfos       shape           -            -            1            -            -            -            -            -
lostlep_stat_in_ee      shape           -            -            -            1            -            -            -            -
lostlep_stat_in_em      shape           -            -            -            1            -            -            -            -
lostlep_stat_in_mm      shape           -            -            -            1            -            -            -            -
lostlep_stat_out_ee     shape           -            -            -            1            -            -            -            -
lostlep_stat_out_em     shape           -            -            -            1            -            -            -            -
lostlep_stat_out_mm     shape           -            -            -            1            -            -            -            -
lostlep_stat_0sfos      shape           -            -            -            1            -            -            -            -
lostlep_stat_1sfos      shape           -            -            -            1            -            -            -            -
lostlep_stat_2sfos      shape           -            -            -            1            -            -            -            -
qflip_stat_in_ee        shape           -            -            -            -            1            -            -            -
qflip_stat_in_em        shape           -            -            -            -            1            -            -            -
qflip_stat_in_mm        shape           -            -            -            -            1            -            -            -
qflip_stat_out_ee       shape           -            -            -            -            1            -            -            -
qflip_stat_out_em       shape           -            -            -            -            1            -            -            -
qflip_stat_out_mm       shape           -            -            -            -            1            -            -            -
qflip_stat_0sfos        shape           -            -            -            -            1            -            -            -
qflip_stat_1sfos        shape           -            -            -            -            1            -            -            -
qflip_stat_2sfos        shape           -            -            -            -            1            -            -            -
prompt_stat_in_ee       shape           -            -            -            -            -            1            -            -
prompt_stat_in_em       shape           -            -            -            -            -            1            -            -
prompt_stat_in_mm       shape           -            -            -            -            -            1            -            -
prompt_stat_out_ee      shape           -            -            -            -            -            1            -            -
prompt_stat_out_em      shape           -            -            -            -            -            1            -            -
prompt_stat_out_mm      shape           -            -            -            -            -            1            -            -
prompt_stat_0sfos       shape           -            -            -            -            -            1            -            -
prompt_stat_1sfos       shape           -            -            -            -            -            1            -            -
prompt_stat_2sfos       shape           -            -            -            -            -            1            -            -
ttw_stat_in_ee          shape           -            -            -            -            -            -            1            -
ttw_stat_in_em          shape           -            -            -            -            -            -            1            -
ttw_stat_in_mm          shape           -            -            -            -            -            -            1            -
ttw_stat_out_ee         shape           -            -            -            -            -            -            1            -
ttw_stat_out_em         shape           -            -            -            -            -            -            1            -
ttw_stat_out_mm         shape           -            -            -            -            -            -            1            -
ttw_stat_0sfos          shape           -            -            -            -            -            -            1            -
ttw_stat_1sfos          shape           -            -            -            -            -            -            1            -
ttw_stat_2sfos          shape           -            -            -            -            -            -            1            -
vbsww_stat_in_ee        shape           -            -            -            -            -            -            -            1
vbsww_stat_in_em        shape           -            -            -            -            -            -            -            1
vbsww_stat_in_mm        shape           -            -            -            -            -            -            -            1
vbsww_stat_out_ee       shape           -            -            -            -            -            -            -            1
vbsww_stat_out_em       shape           -            -            -            -            -            -            -            1
vbsww_stat_out_mm       shape           -            -            -            -            -            -            -            1
vbsww_stat_0sfos        shape           -            -            -            -            -            -            -            1
vbsww_stat_1sfos        shape           -            -            -            -            -            -            -            1
vbsww_stat_2sfos        shape           -            -            -            -            -            -            -            1
lostlep_CRstat_ee       shape           -            -            -            1            -            -            -            -
lostlep_CRstat_em       shape           -            -            -            1            -            -            -            -
lostlep_CRstat_mm       shape           -            -            -            1            -            -            -            -
lostlep_CRstat_0sfos    shape           -            -            -            1            -            -            -            -
lostlep_CRstat_1sfos    shape           -            -            -            1            -            -            -            -
lostlep_CRstat_2sfos    shape           -            -            -            1            -            -            -            -
""".format(suffix, h_data.Integral(), rates["www"], rates["fake"], rates["photon"], rates["lostlep"], rates["qflip"], rates["prompt"], rates["ttw"], rates["vbsww"])

    if model == "sm":
        datacard += """SigPDF                  shape           1            -            -            -            -            -            -            -
SigQsq                  shape           1            -            -            -            -            -            -            -
SigAlpha                shape           1            -            -            -            -            -            -            -
"""
    if model == "whsusy":
        datacard += """ISR                     shape           1            -            -            -            -            -            -            -
GenMET                  shape           1            -            -            -            -            -            -            -
"""

    f = open('statinputs/datacard_{}.txt'.format(suffix), 'w')
    f.write(datacard)
    f.close()


    ofile.Close()

#########################################################################################################################################################
def fill_systfrac(h_nom, h_frac, systfrac, process, systvar):
    if systvar.find("Down") != -1 and (systvar.find("Pileup") == -1):
        return
    systfrac[process + "_" + systvar] = []
    for i in xrange(9):
        ibin = i + 1
        bc = h_nom.GetBinContent(ibin)
        be = h_frac.GetBinContent(ibin)
        fracerr = abs(be / bc - 1) if bc != 0 else 0
        systfrac[process + "_" + systvar].append(fracerr)

#########################################################################################################################################################
def set_hist(th1, bincontents):
    for index, bc in enumerate(bincontents):
        th1.SetBinContent(index + 1, bc)
        th1.SetBinError(index + 1, 0)
    return th1

#########################################################################################################################################################
def stat_error_N(x): return x - 0.5 * ROOT.TMath.ChisquareQuantile(    0.3173 / 2, 2 * (x    ))
def stat_error_P(x): return     0.5 * ROOT.TMath.ChisquareQuantile(1 - 0.3173 / 2, 2 * (x + 1)) - x

# Lost lepton prediction from Table 13.
nominal        = [0.80, 1.31, 3.02, 3.60, 4.86, 4.39, 0.47, 3.14,10.10]
stats_up_systs = [0.48, 0.48, 0.50, 2.15, 1.79, 0.73, 0.00, 0.66, 0.89]
stats_dn_systs = [0.32, 0.37, 0.43, 1.43, 1.36, 0.63, 0.00, 0.55, 0.82]
systs_up_systs = [0.32, 0.30, 0.54, 0.84, 0.85, 0.67, 0.20, 0.62, 1.30]
systs_dn_systs = [0.40, 0.30, 0.60, 0.86, 0.82, 0.68, 0.19, 0.55, 1.30]

#########################################################################################################################################################
def set_to_lostlep_nominal_hist(th1): return set_hist(th1, nominal)
def set_to_lostlep_systup_hist(th1): return set_hist(th1, [ x + y for (x, y) in zip(nominal, systs_up_systs) ])
def set_to_lostlep_systdn_hist(th1): return set_hist(th1, [ x - y for (x, y) in zip(nominal, systs_dn_systs) ])
def set_to_lostlep_statup_hist(th1, ibin): return set_hist(th1, [ x + y if ibin == index else x for index, (x, y) in enumerate(zip(nominal, systs_up_systs)) ])
def set_to_lostlep_statdn_hist(th1, ibin): return set_hist(th1, [ x - y if ibin == index else x for index, (x, y) in enumerate(zip(nominal, systs_dn_systs)) ])

# Lost lepton prediction errors with stat syst separated
CRstats_error = [0.4082, 0.2831, 0.1449, 0.4082, 0.2831, 0.1449, 0.0000, 0.1774, 0.0822] # err(Data - non-3l) / (Data-non-3l) from Table 13 (n.b. mjj-in and mjj-out are correlated)
CRstats_count = [     6,     13,     50,      6,     13,     50,      0,     34,    155] # err(Data - non-3l) / (Data-non-3l) from Table 13 (n.b. mjj-in and mjj-out are correlated)
TFstats_error = [0.3857, 0.2076, 0.1634, 0.2178, 0.1283, 0.1355, 0.3191, 0.1475, 0.0896] # stat.  column from Table 12 (0SFOS gets the stat error from MC simulation in Table 13)
TFsysts_lepsf = [0.0083, 0.0034, 0.0043, 0.0125, 0.0036, 0.0044, 0.0020, 0.0002, 0.0020] # lep.SF column from Table 12 (0SFOS takes larger value from 1SFOS or 2SFOS)
TFsysts_puwgt = [0.0642, 0.0713, 0.0024, 0.0036, 0.0804, 0.0086, 0.0376, 0.0376, 0.0042] # PU     column from Table 12 (0SFOS takes larger value from 1SFOS or 2SFOS)
TFsysts_ttzwz = [0.0010, 0.0013, 0.0010, 0.0009, 0.0012, 0.0003, 0.0024, 0.0024, 0.0009] # ttz/wz column from Table 12 (0SFOS takes larger value from 1SFOS or 2SFOS)
TFsysts_jecor = [0.0547, 0.0142, 0.0532, 0.0594, 0.0301, 0.0205, 0.0152, 0.0152, 0.0018] # JEC    column from Table 12 (0SFOS takes larger value from 1SFOS or 2SFOS)
TFsysts_full  = [ n * math.sqrt(x**2 + y**2 + z**2 + a**2) for (x, y, z, a, n) in zip(TFsysts_lepsf, TFsysts_puwgt, TFsysts_ttzwz, TFsysts_jecor, nominal) ]
TFstats_full  = [ n * x for (x, n) in zip(TFstats_error, nominal) ]
CRstats_full  = [ n * x for (x, n) in zip(CRstats_error, nominal) ]
CRstats_full_up  = [ n * (stat_error_P(x)/x) if x != 0 else 0 for (x, n) in zip(CRstats_count, nominal) ]
CRstats_full_dn  = [ n * (stat_error_N(x)/x) if x != 0 else 0 for (x, n) in zip(CRstats_count, nominal) ]

# Modeling
Mjjsyst_error   = [0.0490, 0.0490, 0.0490, 0.0490, 0.0490, 0.0490, 0.0000, 0.0000, 0.0000] # 4.9% for Mjj modeling Table 11
MllSSsyst_error = [0.0530, 0.0530, 0.0530, 0.0530, 0.0530, 0.0530, 0.0000, 0.0000, 0.0000] # 5.3% for MSFOS modeling Table 10
Mll3lsyst_error = [0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0820, 0.0820, 0.0820] # 8.2% for MSFOS modeling Table 10

#########################################################################################################################################################
def set_to_lostlep_nominal_hist(th1): return set_hist(th1, nominal)
def set_to_lostlep_TFsystup_hist(th1): return set_hist(th1, [ x + y for (x, y) in zip(nominal, TFsysts_full) ])
def set_to_lostlep_TFsystdn_hist(th1): return set_hist(th1, [ x - y for (x, y) in zip(nominal, TFsysts_full) ])
def set_to_lostlep_TFstatup_hist(th1, ibin): return set_hist(th1, [ x + y if ibin == index else x for index, (x, y) in enumerate(zip(nominal, TFstats_full)) ])
def set_to_lostlep_TFstatdn_hist(th1, ibin): return set_hist(th1, [ x - y if ibin == index else x for index, (x, y) in enumerate(zip(nominal, TFstats_full)) ])
def set_to_lostlep_CRstatup_hist(th1, ibin): return set_hist(th1, [ x + y if ((ibin == index) or (ibin == index % 3 and index < 6)) else x for index, (x, y) in enumerate(zip(nominal, CRstats_full_up)) ])
def set_to_lostlep_CRstatdn_hist(th1, ibin): return set_hist(th1, [ x - y if ((ibin == index) or (ibin == index % 3 and index < 6)) else x for index, (x, y) in enumerate(zip(nominal, CRstats_full_dn)) ])
def set_to_lostlep_Mjjsystup_hist(th1): return set_hist(th1, [ x + y  for index, (x, y) in enumerate(zip(nominal, Mjjsyst_error)) ])
def set_to_lostlep_Mjjsystdn_hist(th1): return set_hist(th1, [ x - y  for index, (x, y) in enumerate(zip(nominal, Mjjsyst_error)) ])
def set_to_lostlep_MllSSsystup_hist(th1): return set_hist(th1, [ x + y  for index, (x, y) in enumerate(zip(nominal, MllSSsyst_error)) ])
def set_to_lostlep_MllSSsystdn_hist(th1): return set_hist(th1, [ x - y  for index, (x, y) in enumerate(zip(nominal, MllSSsyst_error)) ])
def set_to_lostlep_Mll3lsystup_hist(th1): return set_hist(th1, [ x + y  for index, (x, y) in enumerate(zip(nominal, Mll3lsyst_error)) ])
def set_to_lostlep_Mll3lsystdn_hist(th1): return set_hist(th1, [ x - y  for index, (x, y) in enumerate(zip(nominal, Mll3lsyst_error)) ])

#########################################################################################################################################################
def make_suffix(model, mass0, mass1):
    suffix = model
    if int(mass0) > 0: suffix += "_{}".format(mass0)
    if int(mass1) > 0: suffix += "_{}".format(mass1)
    return suffix

#########################################################################################################################################################
def get_sigmodel_path(model, mass0, mass1):
    sigmodel = "/sig"
    if model == "wprime": sigmodel = "/bsm/wprime/{}".format(mass0)
    if model == "hpmpm": sigmodel = "/bsm/hpmpm/{}".format(mass0)
    if model == "whsusy": sigmodel = "/bsm/whsusy/{}/{}".format(mass0, mass1)
    return sigmodel

#########################################################################################################################################################
def write_nominal_stat_variations(h_nom, process):
    # Nominal histogram errors are needed to be varied one by one to create an effective uncorrelated histograms
    for i in xrange(h_nom.GetNbinsX()):
        ibin = i + 1
        h_statvarup   = h_nom.Clone(process + "_" + process + "_stat" + bin_suffix(ibin) + "Up")
        h_statvardown = h_nom.Clone(process + "_" + process + "_stat" + bin_suffix(ibin) + "Down")
        bc = h_statvarup.GetBinContent(ibin)
        be = h_statvarup.GetBinError(ibin)
        varup   = max(bc + be, 1e-6)
        vardown = max(bc - be, 1e-6)
        h_statvarup  .SetBinContent(ibin, varup)
        h_statvardown.SetBinContent(ibin, vardown)
        h_statvarup  .Write()
        h_statvardown.Write()

#########################################################################################################################################################
def write_lostlep_stat_variations(h_nom):
    for i in xrange(h_nom.GetNbinsX()):
        ibin = i + 1
        h_staterr_up = set_to_lostlep_statup_hist(h_nom.Clone("lostlep_lostlep_stat" + bin_suffix(ibin) + "Up"), i)
        h_staterr_dn = set_to_lostlep_statdn_hist(h_nom.Clone("lostlep_lostlep_stat" + bin_suffix(ibin) + "Down"), i)
        h_staterr_up.Write()
        h_staterr_dn.Write()

#########################################################################################################################################################
def write_lostlep_syst_variations(h_nom):
    h_systerr_up = set_to_lostlep_systup_hist(h_nom.Clone("lostlep_LostLepSystUp"))
    h_systerr_dn = set_to_lostlep_systdn_hist(h_nom.Clone("lostlep_LostLepSystDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()

#########################################################################################################################################################
def write_lostlep_TFstat_variations(h_nom):
    for i in xrange(h_nom.GetNbinsX()):
        ibin = i + 1
        h_staterr_up = set_to_lostlep_TFstatup_hist(h_nom.Clone("lostlep_lostlep_stat" + bin_suffix(ibin) + "Up"), i)
        h_staterr_dn = set_to_lostlep_TFstatdn_hist(h_nom.Clone("lostlep_lostlep_stat" + bin_suffix(ibin) + "Down"), i)
        h_staterr_up.Write()
        h_staterr_dn.Write()

#########################################################################################################################################################
def write_lostlep_CRstat_variations(h_nom):
    for i in xrange(h_nom.GetNbinsX()):
        ibin = i + 1
        if i == 3 or i == 4 or i == 5: continue
        h_staterr_up = set_to_lostlep_CRstatup_hist(h_nom.Clone("lostlep_lostlep_CRstat" + bin_suffix_comb(ibin) + "Up"), i)
        h_staterr_dn = set_to_lostlep_CRstatdn_hist(h_nom.Clone("lostlep_lostlep_CRstat" + bin_suffix_comb(ibin) + "Down"), i)
        h_staterr_up.Write()
        h_staterr_dn.Write()

#########################################################################################################################################################
def write_lostlep_TFsyst_variations(h_nom):
    h_systerr_up = set_to_lostlep_TFsystup_hist(h_nom.Clone("lostlep_LostLepSystUp"))
    h_systerr_dn = set_to_lostlep_TFsystdn_hist(h_nom.Clone("lostlep_LostLepSystDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()

#########################################################################################################################################################
def write_lostlep_Mjjsyst_variations(h_nom):
    h_systerr_up = set_to_lostlep_Mjjsystup_hist(h_nom.Clone("lostlep_MjjModelingUp"))
    h_systerr_dn = set_to_lostlep_Mjjsystdn_hist(h_nom.Clone("lostlep_MjjModelingDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()

#########################################################################################################################################################
def write_lostlep_MllSSsyst_variations(h_nom):
    h_systerr_up = set_to_lostlep_MllSSsystup_hist(h_nom.Clone("lostlep_MllSSModelingUp"))
    h_systerr_dn = set_to_lostlep_MllSSsystdn_hist(h_nom.Clone("lostlep_MllSSModelingDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()

#########################################################################################################################################################
def write_lostlep_Mll3lsyst_variations(h_nom):
    h_systerr_up = set_to_lostlep_Mll3lsystup_hist(h_nom.Clone("lostlep_Mll3lModelingUp"))
    h_systerr_dn = set_to_lostlep_Mll3lsystdn_hist(h_nom.Clone("lostlep_Mll3lModelingDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()

#########################################################################################################################################################
def do_not_write_syst_hist(process, systvar, model):
    if systvar.find("Fake") != -1 and process.find("fake") == -1: return True
    if systvar.find("Fake") == -1 and process.find("fake") != -1: return True
    if process.find("lostlep") != -1: return True
    if systvar.find("ISR") != -1 and ((model == "sm") or (model == "whsusy" and process.find("www") == -1)): return True
    return False

#########################################################################################################################################################
def bin_suffix(ibin):
    if ibin == 1: return "_in_ee"
    if ibin == 2: return "_in_em"
    if ibin == 3: return "_in_mm"
    if ibin == 4: return "_out_ee"
    if ibin == 5: return "_out_em"
    if ibin == 6: return "_out_mm"
    if ibin == 7: return "_0sfos"
    if ibin == 8: return "_1sfos"
    if ibin == 9: return "_2sfos"

#########################################################################################################################################################
def bin_suffix_comb(ibin):
    if ibin == 1: return "_ee"
    if ibin == 2: return "_em"
    if ibin == 3: return "_mm"
    if ibin == 7: return "_0sfos"
    if ibin == 8: return "_1sfos"
    if ibin == 9: return "_2sfos"

pdf_up_error = [ 0.043159   , 0.02767    , 0.0227303     , 0.0231684  , 0.0218397   , 0.00903931 , 0.0337773   , 0.043174   , 0.0169784  , ]
pdf_dn_error = [ 0.035867   , 0.0223932  , 0.019016      , 0.0187451  , 0.0180389   , 0.00729971 , 0.0269668   , 0.0349376  , 0.0134577  , ]
q2_up_error  = [ 0.012439   , 0.0110485  , 0.0036819     , 0.0768119  , 0.00946869  , 0.0909396  , 0.0123575   , 0.0608722  , 0.075198   , ]
q2_dn_error  = [ 0.00389075 , 0.0187065  , 0.0151138     , 0.102129   , 0.0354567   , 0.135643   , 0.0291748   , 0.0822849  , 0.0942069  , ]
aS_up_error  = [ 0.00278907 , 0.00310995 , 0.000380084   , 0.00636957 , 0.000984983 , 0.00583416 , 0.000283398 , 0.00391803 , 0.00787921 , ]
aS_dn_error  = [ 0.00299298 , 0.00221614 , 0.00000864175 , 0.00420784 , 0.00206036  , 0.00561643 , 0.00140796  , 0.0017675  , 0.0121466  , ]

def set_to_www_pdfsystup_hist(nom, th1): return set_hist(th1, [ x + x*y  for index, (x, y) in enumerate(zip([nom.GetBinContent(ibin) for ibin in xrange(1,nom.GetNbinsX()+1)], pdf_up_error)) ])
def set_to_www_pdfsystdn_hist(nom, th1): return set_hist(th1, [ x - x*y  for index, (x, y) in enumerate(zip([nom.GetBinContent(ibin) for ibin in xrange(1,nom.GetNbinsX()+1)], pdf_dn_error)) ])
def set_to_www_q2systup_hist (nom, th1): return set_hist(th1, [ x + x*y  for index, (x, y) in enumerate(zip([nom.GetBinContent(ibin) for ibin in xrange(1,nom.GetNbinsX()+1)], q2_up_error)) ])
def set_to_www_q2systdn_hist (nom, th1): return set_hist(th1, [ x - x*y  for index, (x, y) in enumerate(zip([nom.GetBinContent(ibin) for ibin in xrange(1,nom.GetNbinsX()+1)], q2_dn_error)) ])
def set_to_www_aSsystup_hist (nom, th1): return set_hist(th1, [ x + x*y  for index, (x, y) in enumerate(zip([nom.GetBinContent(ibin) for ibin in xrange(1,nom.GetNbinsX()+1)], aS_up_error)) ])
def set_to_www_aSsystdn_hist (nom, th1): return set_hist(th1, [ x - x*y  for index, (x, y) in enumerate(zip([nom.GetBinContent(ibin) for ibin in xrange(1,nom.GetNbinsX()+1)], aS_dn_error)) ])

#########################################################################################################################################################
def write_www_theory_syst_variations(h_nom):
    h_systerr_up = set_to_www_pdfsystup_hist(h_nom, h_nom.Clone("www_SigPDFUp"))
    h_systerr_dn = set_to_www_pdfsystdn_hist(h_nom, h_nom.Clone("www_SigPDFDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()
    h_systerr_up = set_to_www_q2systup_hist(h_nom, h_nom.Clone("www_SigQsqUp"))
    h_systerr_dn = set_to_www_q2systdn_hist(h_nom, h_nom.Clone("www_SigQsqDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()
    h_systerr_up = set_to_www_aSsystup_hist(h_nom, h_nom.Clone("www_SigAlphaUp"))
    h_systerr_dn = set_to_www_aSsystdn_hist(h_nom, h_nom.Clone("www_SigAlphaDown"))
    h_systerr_up.Write()
    h_systerr_dn.Write()

#########################################################################################################################################################
def write_whsusy_theory_syst_variations(h_nom):
    pass

#########################################################################################################################################################
ARstats_count = [ 8, 17, 57, 5, 41, 47, 17, 2, 6] # fake AR counts
ARstats_full_up  = [ (stat_error_P(x)/x) if x != 0 else 0 for x in ARstats_count ]
ARstats_full_dn  = [ (stat_error_N(x)/x) if x != 0 else 0 for x in ARstats_count ]
print ARstats_full_up
def set_to_fake_ARstatup_hist(nom, th1, ibin): return set_hist(th1, [ x + x*y if ibin == index else x for index, (x, y) in enumerate(zip([nom.GetBinContent(ii) for ii in xrange(1,nom.GetNbinsX()+1)], ARstats_full_up)) ])
def set_to_fake_ARstatdn_hist(nom, th1, ibin): return set_hist(th1, [ x - x*y if ibin == index else x for index, (x, y) in enumerate(zip([nom.GetBinContent(ii) for ii in xrange(1,nom.GetNbinsX()+1)], ARstats_full_dn)) ])
def write_fake_ARstat_variations(h_nom):
    for i in xrange(h_nom.GetNbinsX()):
        ibin = i + 1
        h_staterr_up = set_to_fake_ARstatup_hist(h_nom, h_nom.Clone("fake_fake_ARstat" + bin_suffix(ibin) + "Up"), i)
        h_staterr_dn = set_to_fake_ARstatdn_hist(h_nom, h_nom.Clone("fake_fake_ARstat" + bin_suffix(ibin) + "Down"), i)
        h_staterr_up.Write()
        h_staterr_dn.Write()

#########################################################################################################################################################
def set_to_average_and_write_genmet_syst_hist(h_nom, h_sys):
    h_sys_up = h_sys.Clone("www_GenMETUp")
    h_sys_dn = h_sys.Clone("www_GenMETDown")
    for i in xrange(1, h_nom.GetNbinsX()+1):
        nc = h_nom.GetBinContent(i)
        sc = h_sys.GetBinContent(i)
        n = (nc + sc) / 2.
        d = abs(n - sc)
        h_nom.SetBinContent(i, n)
        h_sys_up.SetBinContent(i, n + d)
        h_sys_dn.SetBinContent(i, n - d)
    h_sys_up.Write()
    h_sys_dn.Write()

#########################################################################################################################################################
#########################################################################################################################################################
#########################################################################################################################################################

if __name__ == "__main__":

    def help():
        print "Usage:"
        print ""
        print "  python {} MODEL [MASS] [MASS-LSP]"
        print ""
        print "  MODEL = sm, wprime, hpmpm (doubly charged higgs), whsusy"
        print ""
        sys.exit()

    if len(sys.argv) < 2:
        help()
    else:
        main(*sys.argv[1:])





#########################################################################################################################################################
#########################################################################################################################################################
#########################################################################################################################################################

#def apply_lostlep_syst(th1, isup, whichsyst=""):
#
#    # From AN Table 12.
#    stats_systs = [0.3857, 0.2076, 0.1634, 0.2178, 0.1283, 0.1355, 0.0000, 0.1475, 0.0896]
#    lepsf_systs = [0.0083, 0.0034, 0.0043, 0.0125, 0.0036, 0.0044, 0.0000, 0.0002, 0.0020]
#    purwt_systs = [0.0642, 0.0713, 0.0024, 0.0036, 0.0804, 0.0086, 0.0000, 0.0376, 0.0042]
#    ttzwz_systs = [0.0010, 0.0013, 0.0010, 0.0009, 0.0012, 0.0003, 0.0000, 0.0024, 0.0009]
#    jecor_systs = [0.0547, 0.0142, 0.0532, 0.0594, 0.0301, 0.0205, 0.0000, 0.0152, 0.0018]
#
#    # Quaratic sum except statistics error because qframework includes error into predicted yield
#    #quadr_systs = [0.1350, 0.0840, 0.1099, 0.0892, 0.1025, 0.0769, 0.1227, 0.1090, 0.1094]
#
#    if whichsyst == "stat" :
#        systs = stats_systs
#    elif whichsyst == "lepsf":
#        systs = lepsf_systs
#    elif whichsyst == "pu"   :
#        systs = purwt_systs
#    elif whichsyst == "ttzwz":
#        systs = ttzwz_systs
#    elif whichsyst == "jec"  :
#        systs = jecor_systs
#    else:
#        return
#
#    for index, syst in enumerate(systs):
#        if isup:
#            th1.SetBinContent(index + 1, th1.GetBinContent(index + 1) * (1 + syst))
#        else:
#            th1.SetBinContent(index + 1, th1.GetBinContent(index + 1) * (1 - syst))
#
#    return th1
#

##########################################################################################################################################################
#def create_lostlep_hist(th1, isup=True, whichsyst="", statibin=-1):
#    # From AN Table 13.
#    nominal        = [0.80, 1.31, 3.02, 3.60, 4.86, 4.39, 0.47, 3.14,10.10]
#    stats_up_systs = [0.48, 0.48, 0.50, 2.15, 1.79, 0.73, 0.00, 0.66, 0.89]
#    stats_dn_systs = [0.32, 0.37, 0.43, 1.43, 1.36, 0.63, 0.00, 0.55, 0.82]
#    systs_up_systs = [0.32, 0.30, 0.54, 0.84, 0.85, 0.67, 0.20, 0.62, 1.30]
#    systs_dn_systs = [0.40, 0.30, 0.60, 0.86, 0.82, 0.68, 0.19, 0.55, 1.30]
#    if whichsyst == "stat" and isup:
#        bincontents = [ x + y in for (x, y) in zip(nominal, stats_up_systs) ]
#    elif whichsyst == "stat" and not isup:
#        bincontents = [ x - y in for (x, y) in zip(nominal, stats_dn_systs) ]
#    elif whichsyst == "syst" and isup:
#        bincontents = [ x + y in for (x, y) in zip(nominal, systs_up_systs) ]
#    elif whichsyst == "syst" and not isup:
#        bincontents = [ x - y in for (x, y) in zip(nominal, systs_dn_systs) ]
#    else:
#        bincontents = nominal
#    for index, bc in enumerate(bincontents):
#        if isup:
#            th1.SetBinContent(index + 1, bc)
#            th1.SetBinError(index + 1, 0)
#        else:
#            th1.SetBinContent(index + 1, bc)
#            th1.SetBinError(index + 1, 0)
#    return th1

#    for key in sorted(systfrac.keys()):
#        process = key.split("_")[0]
#        systvar = key.split("_")[1]
#        allrates[process][systvar] = []
#        #pstr = []
#        #pstr.append("{:<25s}".format(key))
#        for i in xrange(nbins):
#            #pstr.append("{:7.3f}".format(systfrac[key][i]))
#            allrates[process][systvar].append("{:7.3f}".format(systfrac[key][i]))
#        #print " ".join(pstr)
#
#    for process in processes:
#        for key in sorted(allrates[process]):
#            pstr = []
#            pstr.append("{:<25s}".format(process + "_" + key))
#            for i in xrange(nbins):
#                pstr.append("{}".format(allrates[process][key][i]))
#            print " ".join(pstr)
#
