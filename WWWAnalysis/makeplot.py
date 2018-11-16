#!/bin/env python

from rooutil import rooutil as ru
from rooutil import plottery_wrapper as p
import glob
import ROOT as r

def get_histnames(fpath, region):

    f = r.TFile(fpath)

    rtn = []

    for key in f.GetListOfKeys():
        if region in str(key.GetName()) and "Full_cutflow" in str(key.GetName()):
            name = str(key.GetName())
            name = name.replace("_cutflow", "")
            rtn.append(name)

    return rtn;

def region_index(s):
    if "ee" in s and "Side" not in s: return 0
    if "em" in s and "Side" not in s: return 1
    if "mm" in s and "Side" not in s: return 2
    if "ee" in s and "Side" in s: return 3
    if "em" in s and "Side" in s: return 4
    if "mm" in s and "Side" in s: return 5
    if "0SFOS" in s: return 6
    if "1SFOS" in s: return 7
    if "2SFOS" in s: return 8
    else: return 999

def main():

    output_dirpath = "outputs/WWW2017_analysis_v0.27.1"
    is2017 = "WWW2017" in output_dirpath

#    histnames = [
#            "SRSSeeNb0__lep_pt1",
#            "SRSSemNb0__lep_pt1",
#            "SRSSmmNb0__lep_pt1",
#            "SRSSSideeeNb0__lep_pt1",
#            "SRSSSideemNb0__lep_pt1",
#            "SRSSSidemmNb0__lep_pt1",
#            "SR0SFOSNb0__lep_pt1",
#            "SR1SFOSNb0__lep_pt1",
#            "SR2SFOSNb0__lep_pt1"
#           ]

#    histnames = [
#            "ARSSeeNb0__lep_pt1",
#            "ARSSemNb0__lep_pt1",
#            "ARSSmmNb0__lep_pt1",
#           ]

#    histnames = [
#            "ARSSeeNj2__lep_pt1",
#            "ARSSemNj2__lep_pt1",
#            "ARSSmmNj2__lep_pt1",
#           ]

#    histnames = [
#            "BTCRSSeeFull",
#            "BTCRSSemFull",
#            "BTCRSSmmFull",
#            "BTCRSSSideeeFull",
#            "BTCRSSSideemFull",
#            "BTCRSSSidemmFull",
#            "BTCR0SFOSFull",
#            "BTCR1SFOSFull",
#            "BTCR2SFOSFull"
#            ]

#    histnames = [
#            "SRSSeeFull",
#            "SRSSemFull",
#            "SRSSmmFull",
#            "SRSSSideeeFull",
#            "SRSSSideemFull",
#            "SRSSSidemmFull",
#            "SR0SFOSFull",
#            "SR1SFOSFull",
#            "SR2SFOSFull"
#            ]

#    histnames = [
#            "LXECRSSeeFull",
#            "LXECRSSemFull",
#            "LXECRSSmmFull",
#            ]

#    histnames = [
#            "BTCRARSSeeFull",
#            "BTCRARSSemFull",
#            "BTCRARSSmmFull",
#            "BTCRARSSSideeeFull",
#            "BTCRARSSSideemFull",
#            "BTCRARSSSidemmFull",
#            "BTCRAR0SFOSFull",
#            "BTCRAR1SFOSFull",
#            "BTCRAR2SFOSFull"
#            ]

#    histnames = [
#            "ARSSeeFull",
#            "ARSSemFull",
#            "ARSSmmFull",
#            "ARSSSideeeFull",
#            "ARSSSideemFull",
#            "ARSSSidemmFull",
#            "AR0SFOSFull",
#            "AR1SFOSFull",
#            "AR2SFOSFull"
#            ]

#    histnames = [
#            "LXECRARSSeeFull",
#            "LXECRARSSemFull",
#            "LXECRARSSmmFull",
#            ]

#    histnames = [
#            "LXECRSSeeFull",
#            "LXECRSSemFull",
#            "LXECRSSmmFull",
#            ]

#    histnames = [
#            "LXECRARSSmmFull__ptcorretarolledcoarseemu",
#            "LXECRARSSemFull__ptcorretarolledcoarseemu",
#            "LXECRARSSeeFull__ptcorretarolledcoarseemu",
#            ]

#    histnames = [
#            "ARSSSideeeFull__ptcorretarolledcoarseemu",
#            ]

#    histnames = [
#            "WZCRSSeeFull",
#            "WZCRSSemFull",
#            "WZCRSSmmFull",
#            "WZCR1SFOSFull",
#            "WZCR2SFOSFull"
#            ]

    region = "GCR"
    region = "LXECRSS"
    region = "VBSCRSS"
    region = "BTCR"
    #region = "SR"
    histnames = get_histnames(output_dirpath + "/signal.root", region)
    histnames.sort(key=region_index)

    bkg_list_lostlep = [ x for x in glob.glob(output_dirpath+"/lostlep.root") ]
    bkg_list_photon  = [ x for x in glob.glob(output_dirpath+"/photon.root")  ]
    bkg_list_qflip   = [ x for x in glob.glob(output_dirpath+"/qflip.root")   ]
    bkg_list_ddfakes = [ x for x in glob.glob(output_dirpath+"/ddfakes.root") ]
    bkg_list_ewksubt = [ x for x in glob.glob(output_dirpath+"/ewksubt.root") ]
    bkg_list_mcfakes = [ x for x in glob.glob(output_dirpath+"/fakes.root")   ]
    bkg_list_prompt  = [ x for x in glob.glob(output_dirpath+"/prompt.root")  ]

    bkg_list_fakes = bkg_list_mcfakes
    bkg_list_fakes = bkg_list_ddfakes

    sig_list  = glob.glob(output_dirpath+"/*t_www_*/*.root")
    vh_list   = glob.glob(output_dirpath+"/*VH*t_www_*/*.root")
    www_list  = glob.glob(output_dirpath+"/*WWW*t_www_*/*.root")
    data_list = glob.glob(output_dirpath+"/*Run2017*_t_ss*/*.root")
    sig_list  = glob.glob(output_dirpath+"/signal.root")
    data_list = glob.glob(output_dirpath+"/data.root")

    if "__" in histnames[0]:
        h_lostlep = ru.get_summed_histogram(bkg_list_lostlep , histnames)
        h_photon  = ru.get_summed_histogram(bkg_list_photon  , histnames)
        h_qflip   = ru.get_summed_histogram(bkg_list_qflip   , histnames)
        h_fakes   = ru.get_summed_histogram(bkg_list_fakes   , histnames)
        h_ewksubt = ru.get_summed_histogram(bkg_list_ewksubt , histnames)
        h_prompt  = ru.get_summed_histogram(bkg_list_prompt  , histnames)
        h_sig     = ru.get_summed_histogram(sig_list         , histnames)
        h_data    = ru.get_summed_histogram(data_list        , histnames)
    else:
        h_lostlep = ru.get_yield_histogram(bkg_list_lostlep , histnames)
        h_photon  = ru.get_yield_histogram(bkg_list_photon  , histnames)
        h_qflip   = ru.get_yield_histogram(bkg_list_qflip   , histnames)
        h_fakes   = ru.get_yield_histogram(bkg_list_fakes   , histnames)
        h_ewksubt = ru.get_yield_histogram(bkg_list_ewksubt , histnames)
        h_prompt  = ru.get_yield_histogram(bkg_list_prompt  , histnames)
        h_sig     = ru.get_yield_histogram(sig_list         , histnames)
        h_data    = ru.get_yield_histogram(data_list        , histnames)

    if bkg_list_fakes == bkg_list_ddfakes:
        for i in xrange(1, h_ewksubt.GetNbinsX()+1):
            if h_ewksubt.GetBinContent(i) > 0:
                h_ewksubt.SetBinContent(i, 0)
        h_fakes.Add(h_ewksubt)

    h_lostlep .SetName("Lost/three lep")
    h_photon  .SetName("#gamma#rightarrowlepton")
    h_qflip   .SetName("Charge mis-id")
    h_fakes   .SetName("Non-prompt")
    h_prompt  .SetName("Irredu.")
    h_sig     .SetName("WWW")
    h_data    .SetName("Data")

    colors = [ 920, 2007, 2005, 2003, 2001, 2 ]
    alloptions= {
                "ratio_range":[0.0,2.0],
                "nbins": 15,
                "autobin": False,
                "legend_scalex": 1.8,
                "legend_scaley": 1.1,
                "output_name": "plots/test.pdf",
                "bkg_sort_method": "unsorted",
                "no_ratio": False,
                "print_yield": True,
                "blind": True if "SR" in histnames[0] else False,
                #"blind": False,
                "lumi_value": "41.3",
                }
    p.plot_hist(
            sigs = [h_sig],
            bgs  = [h_photon, h_qflip, h_fakes, h_lostlep, h_prompt],
            data = h_data,
            colors = colors,
            syst = None,
            options=alloptions)

if __name__ == "__main__":
    main()

