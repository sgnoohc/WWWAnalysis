#!/bin/env python

from rooutil import rooutil as ru
from rooutil import plottery_wrapper as p
import glob

def main():

    output_dirpath = "outputs/WWW_v1.2.2"
    #output_dirpath = "outputs/WWW2017_v4.0.5"
    is2017 = "WWW2017" in output_dirpath
    sig_globber = "www_" if is2017 else "www_2l"
    fake_globber = "t_fakes_*.root" if is2017 else "t_fakes_data*.root"

    histnames = [
            "SRSSeeFull__yield",
            "SRSSemFull__yield",
            "SRSSmmFull__yield",
            "SRSSSideeeFull__yield",
            "SRSSSideemFull__yield",
            "SRSSSidemmFull__yield",
            "SR0SFOSFull__yield",
            "SR1SFOSFull__yield",
            "SR2SFOSFull__yield"
            ]

    histnames = [
            "WZCRSSeeFull__wzcryield",
            "WZCRSSemFull__wzcryield",
            "WZCRSSmmFull__wzcryield",
            "WZCR1SFOSFull__wzcryield",
            "WZCR2SFOSFull__wzcryield"
            ]

    histnames = [
            "SR1SFOSFull_cutflow",
            ]

    def veto(x):
        #return "alp_" not in x and "wz_3lnu0" not in x and "wz_3lnu1" not in x and "wz_3lnu2" not in x and "wz_3lnu3" not in x
        return "alp_" not in x and "wz_3lnu_" not in x

    bkg_list_lostlep = [ x for x in glob.glob(output_dirpath+"/t_lostlep_*.root") if veto(x) ]
    bkg_list_photon  = [ x for x in glob.glob(output_dirpath+"/t_photon_*.root")  if veto(x) ]
    bkg_list_qflip   = [ x for x in glob.glob(output_dirpath+"/t_qflip_*.root")   if veto(x) ]
    bkg_list_fakes   = [ x for x in glob.glob(output_dirpath+"/"+fake_globber)    if veto(x) ]
    bkg_list_prompt  = [ x for x in glob.glob(output_dirpath+"/t_prompt_*.root")  if veto(x) ]
    sig_list  = glob.glob(output_dirpath+"/t_www_"+sig_globber+"*.root") + glob.glob(output_dirpath+"/t_www_vh*.root")
    data_list = glob.glob(output_dirpath+"/data_*.root")

    h_lostlep = ru.get_summed_histogram(bkg_list_lostlep , histnames)
    h_photon  = ru.get_summed_histogram(bkg_list_photon  , histnames)
    h_qflip   = ru.get_summed_histogram(bkg_list_qflip   , histnames)
    h_fakes   = ru.get_summed_histogram(bkg_list_fakes   , histnames)
    h_prompt  = ru.get_summed_histogram(bkg_list_prompt  , histnames)
    h_sig     = ru.get_summed_histogram(sig_list         , histnames)
    h_data    = ru.get_summed_histogram(data_list        , histnames)

    h_lostlep .SetName("Lost/three lep")
    h_photon  .SetName("#gamma#rightarrowlepton")
    h_qflip   .SetName("Charge mis-id")
    h_fakes   .SetName("Non-prompt")
    h_prompt  .SetName("Irredu.")
    h_sig     .SetName("WWW")

    colors = [ 920, 2007, 2005, 2003, 2001, 2 ]
    alloptions= {
                "ratio_range":[0.0,2.0],
                "nbins": 30,
                "autobin": False,
                "legend_scalex": 1.8,
                "legend_scaley": 1.1,
                "output_name": "plots/test.pdf",
                "bkg_sort_method": "unsorted",
                "no_ratio": True,
                "print_yield": True,
                }
    p.plot_hist(
            sigs = [h_sig],
            bgs  = [h_photon, h_qflip, h_fakes, h_lostlep, h_prompt],
            data = h_data,
            #data = None,
            colors = colors,
            syst = None,
            options=alloptions)

if __name__ == "__main__":
    main()
