#!/bin/env python

import ROOT as r
from rooutil import rooutil as ru
from rooutil import plottery_wrapper as p
from plottery import utils as u
import glob
import math

def main():

    prescale_muHLT17 = get_prescale("TwoMuHLT17__Mll")
    prescale_elHLT23 = get_prescale("TwoElHLT23__Mll")
    print prescale_muHLT17 , prescale_elHLT23
    prescale_muHLT17 = 1
    prescale_elHLT23 = 1

#    plot("OneElEWKCR__MT"             , prescale_elHLT23)
#    plot("OneElEWKCR__MET"            , prescale_elHLT23)
#    plot("OneElEWKCR__Nvtx"           , prescale_elHLT23)
#    plot("OneElMR__MT"                , prescale_elHLT23)
#    plot("OneElMR__ptcorr"            , prescale_elHLT23)
#    plot("OneElTightMR__ptcorr"       , prescale_elHLT23)
    el_sf, el_sferr = ewksf("OneElEWKCR__MT")
#    plot("OneElMR__ptcorrvarbin"      , prescale_elHLT23, el_sf)
#    plot("OneElTightMR__ptcorrvarbin" , prescale_elHLT23, el_sf)
    fakerate("OneElTightMR__ptcorrvarbin", "OneElMR__ptcorrvarbin", prescale_elHLT23, el_sf, el_sferr)

#    plot("OneMuEWKCR__MT"             , prescale_muHLT17)
#    plot("OneMuEWKCR__MET"            , prescale_muHLT17)
#    plot("OneMuEWKCR__Nvtx"           , prescale_muHLT17)
#    plot("OneMuMR__MT"                , prescale_muHLT17)
#    plot("OneMuMR__ptcorr"            , prescale_muHLT17)
#    plot("OneMuTightMR__ptcorr"       , prescale_muHLT17)
    mu_sf, mu_sferr = ewksf("OneMuEWKCR__MT")
#    plot("OneMuMR__ptcorrvarbin"      , prescale_muHLT17)
#    plot("OneMuTightMR__ptcorrvarbin" , prescale_muHLT17)
    fakerate("OneMuTightMR__ptcorrvarbin", "OneMuMR__ptcorrvarbin", prescale_muHLT17, mu_sf, mu_sferr)

def get_prescale(histname):
    _, h_d, h_b, _, _, _ = plot(histname)
    h_b.Rebin(h_b.GetNbinsX())
    h_d.Rebin(h_d.GetNbinsX())
    h_d.Print("all")
    h_b.Print("all")
    h_b.Divide(h_d)
    return h_b.GetBinContent(1)

def select_mt_window(h):
    h.Rebin(10)
    for i in [1, 2, 3, 4, 5, 6, 7, 8]:
        h.SetBinContent(i, 0)
        h.SetBinError(i, 0)
    for i in [13, 14, 15, 16, 17, 18]:
        h.SetBinContent(i, 0)
        h.SetBinError(i, 0)
    h.Rebin(18)
    return h

def ewksf(histname, ps=0):
    _, h_d, h_b, _, _, _ = plot(histname)
    h_d = select_mt_window(h_d)
    h_b = select_mt_window(h_b)
    h_d.Print("all")
    h_b.Print("all")
    h_d.Divide(h_b)
    h_d.Print("all")
    return h_d.GetBinContent(1), h_d.GetBinError(1)

def get_fakerate_histograms(num, den, ps=0, sf=0):

    h_num, _, _, h_num_qcd_mu, h_num_qcd_el, h_num_qcd_bc = plot(num, ps, sf)
    h_den, _, _, h_den_qcd_mu, h_den_qcd_el, h_den_qcd_bc = plot(den, ps, sf)

    # Creating a summed histogram (EM + HF sourced e-fake) where the ratio will be only of importance as we will divide the histograms to get fake rate
    h_num_qcd_esum = h_num_qcd_el.Clone("QCD(e)")
    h_den_qcd_esum = h_den_qcd_el.Clone("QCD(e)")
    h_num_qcd_esum.Add(h_num_qcd_bc)
    h_den_qcd_esum.Add(h_den_qcd_bc)

    # Data
    u.move_in_overflows(h_num)
    u.move_in_overflows(h_den)
    h_num.Divide(h_den)

    # Mu fake rate
    u.move_in_overflows(h_num_qcd_mu)
    u.move_in_overflows(h_den_qcd_mu)
    h_num_qcd_mu.Divide(h_den_qcd_mu)

    # EM fake rate
    u.move_in_overflows(h_num_qcd_el)
    u.move_in_overflows(h_den_qcd_el)
    h_num_qcd_el.Divide(h_den_qcd_el)

    # HF fake rate
    u.move_in_overflows(h_num_qcd_bc)
    u.move_in_overflows(h_den_qcd_bc)
    h_num_qcd_bc.Divide(h_den_qcd_bc)

    # Total summed electron fake rate
    u.move_in_overflows(h_num_qcd_esum)
    u.move_in_overflows(h_den_qcd_esum)
    h_num_qcd_esum.Divide(h_den_qcd_esum)

    return h_num, h_num_qcd_mu, h_num_qcd_esum

def add_systematics(h_num, herr_num):
    h_num.Print("all")
    herr_num.Print("all")
    for i in xrange(1, h_num.GetNbinsX()+1):
        derr = abs(herr_num.GetBinContent(i) - h_num.GetBinContent(i))
        h_num.SetBinError(i, math.sqrt(h_num.GetBinError(i)**2 + derr**2))

def fakerate(num, den, ps=0, sf=0, sferr=0):

    # Obtain histograms
    h_num    , h_num_qcd_mu    , h_num_qcd_esum    = get_fakerate_histograms(num , den , ps , sf)
    herr_num , herr_num_qcd_mu , herr_num_qcd_esum = get_fakerate_histograms(num , den , ps , sf+sferr)

    # Set data-driven QCD estimate systematics stemming from EWK SF uncertainty
    add_systematics(h_num, herr_num)

    # Options
    alloptions= {
                "ratio_range":[0.0,2.0],
                "nbins": 30,
                "autobin": False,
                "legend_scalex": 1.8,
                "legend_scaley": 1.1,
                "output_name": "plots/{}.pdf".format(num+"__"+den),
                "bkg_sort_method": "unsorted",
                "no_ratio": False,
                "print_yield": True,
                "draw_points": True,
                }

    bgs_list = [h_num_qcd_mu] if "Mu" in num else [h_num_qcd_esum]

    p.plot_hist(
            bgs = bgs_list,
            data = h_num,
            syst = None,
            options=alloptions)

def plot(histnames, ps=0, sf=0):

    # The path for the plotting
    output_dirpath = "outputs/FR2017_v3.0.13"
    is2017 = "FR2017" in output_dirpath

    # Glob the file lists
    bkg_list_wjets  = glob.glob(output_dirpath+"/WJ*.root")
    bkg_list_dy     = glob.glob(output_dirpath+"/DY*.root")
    bkg_list_ttbar  = glob.glob(output_dirpath+"/TT*.root")
    bkg_list_vv     = glob.glob(output_dirpath+"/WW*.root") + glob.glob(output_dirpath+"/WW*.root")
    bkg_list_qcd_mu = glob.glob(output_dirpath+"/QCD*MuEn*.root")
    bkg_list_qcd_el = glob.glob(output_dirpath+"/QCD*EMEn*.root")
    bkg_list_qcd_bc = glob.glob(output_dirpath+"/QCD*bcToE*.root")
    bkg_list_all = bkg_list_wjets + bkg_list_dy + bkg_list_ttbar + bkg_list_vv

    # Glob the data file list depending on the region
    if "Mu" in histnames:
        data_list       = glob.glob(output_dirpath+"/DoubleMuon*.root")
    elif "El" in histnames:
        data_list       = glob.glob(output_dirpath+"/SingleElectron*.root")
    else:
        data_list       = glob.glob(output_dirpath+"/Double*.root") + glob.glob(output_dirpath+"/SingleElectron*.root")

    # Get all the histogram objects
    h_wjets  = ru.get_summed_histogram(bkg_list_wjets , histnames)
    h_dy     = ru.get_summed_histogram(bkg_list_dy    , histnames)
    h_ttbar  = ru.get_summed_histogram(bkg_list_ttbar , histnames)
    h_vv     = ru.get_summed_histogram(bkg_list_vv    , histnames)
    h_qcd_mu = ru.get_summed_histogram(bkg_list_qcd_mu, histnames)
    h_qcd_el = ru.get_summed_histogram(bkg_list_qcd_el, histnames)
    h_qcd_bc = ru.get_summed_histogram(bkg_list_qcd_bc, histnames)
    h_data   = ru.get_summed_histogram(data_list      , histnames)

    # Set the names of the histograms
    h_wjets  .SetName("W")
    h_dy     .SetName("Z")
    h_ttbar  .SetName("Top")
    h_vv     .SetName("VV")
    h_qcd_mu .SetName("QCD(#mu)")
    h_qcd_el .SetName("QCD(e)")
    h_qcd_bc .SetName("QCD(bc)")
    h_data   .SetName("Data")

    # Scale the histograms appropriately from SF from the EWKCR
    if sf > 0:
        h_wjets  .Scale(sf)
        h_dy     .Scale(sf)
        h_ttbar  .Scale(sf)
        h_vv     .Scale(sf)

    # If the data needs some additional correction for the prescale
    if ps > 0:
        h_data.Scale(ps)

    # Color settings
    colors = [ 2007, 2005, 2003, 2001, 920, 2 ]

    # Options
    alloptions= {
                "ratio_range":[0.0,2.0],
                "nbins": 30,
                "autobin": False,
                "legend_scalex": 1.8,
                "legend_scaley": 1.1,
                "output_name": "plots/{}.pdf".format(histnames),
                "bkg_sort_method": "unsorted",
                "no_ratio": False,
                "print_yield": False,
                "yaxis_log": True if "ptcorr" in histnames else False,
                "legend_smart": False if "ptcorr" in histnames else True,
                }

    # The bkg histogram list
    bgs_list = [h_vv , h_ttbar , h_dy , h_wjets, h_qcd_mu ],
    bgs_list = [h_vv , h_ttbar , h_dy , h_wjets, h_qcd_mu ] if "Mu" in histnames else [h_vv , h_ttbar , h_dy , h_wjets, h_qcd_el ]

    # Plot them
    p.plot_hist(
            bgs = bgs_list,
            data = h_data.Clone("Data"),
            colors = colors,
            syst = None,
            options=alloptions)

    # Obtain the histogram again to return the object for further calculations

    # Data-driven QCD = data - bkg
    h_ddqcd  = ru.get_summed_histogram(data_list      , histnames)
    h_bkg    = ru.get_summed_histogram(bkg_list_all   , histnames)
    if sf > 0:
        h_bkg.Scale(sf)
    h_ddqcd.Add(h_bkg, -1)

    # MC QCD
    h_qcd_mu = ru.get_summed_histogram(bkg_list_qcd_mu, histnames).Clone("QCD(#mu)")
    h_qcd_el = ru.get_summed_histogram(bkg_list_qcd_el, histnames).Clone("QCD(EM)")
    h_qcd_bc = ru.get_summed_histogram(bkg_list_qcd_bc, histnames).Clone("QCD(HF)")

    return h_ddqcd, h_data, h_bkg, h_qcd_mu, h_qcd_el, h_qcd_bc

if __name__ == "__main__":
    main()
