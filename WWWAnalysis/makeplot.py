#!/bin/env python

from rooutil import rooutil as ru
from rooutil import plottery_wrapper as p
import glob

def main():

    output_dirpath = "outputs/WWW2016_v4.0.5"
    output_dirpath = "outputs/WWW_v1.2.2"
    output_dirpath = "outputs/WWW2017_v4.0.5"
    output_dirpath = "/hadoop/cms/store/user/phchang/metis/wwwanalysis/WWW2017_analysis_v0.9.1/"
    is2017 = "WWW2017" in output_dirpath

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

#    histnames = [
#            "ARSSee__yield",
#            "ARSSem__yield",
#            "ARSSmm__yield",
#            "ARSSSideee__yield",
#            "ARSSSideem__yield",
#            "ARSSSidemm__yield",
#            ]

    def veto(x):
        if "Run2017" in x:
            if "t_fakes" in x:
                return False
            else:
                return True
        return True

    bkg_list_lostlep = [ x for x in glob.glob(output_dirpath+"/*t_lostlep_*/*.root") if veto(x) ]
    bkg_list_photon  = [ x for x in glob.glob(output_dirpath+"/*t_photon_*/*.root")  if veto(x) ]
    bkg_list_qflip   = [ x for x in glob.glob(output_dirpath+"/*t_qflip_*/*.root")   if veto(x) ]
    #bkg_list_fakes   = [ x for x in glob.glob(output_dirpath+"/*Run2017*t_fakes_*/*.root")   ]
    bkg_list_fakes   = [ x for x in glob.glob(output_dirpath+"/*t_fakes_*/*.root")   if veto(x) ]
    bkg_list_prompt  = [ x for x in glob.glob(output_dirpath+"/*t_prompt_*/*.root")  if veto(x) ]

    print bkg_list_fakes

    sig_list  = glob.glob(output_dirpath+"/*t_www_*/*.root")
    vh_list = glob.glob(output_dirpath+"/*VH*t_www_*/*.root")
    www_list = glob.glob(output_dirpath+"/*WWW*t_www_*/*.root")
    data_list = glob.glob(output_dirpath+"/*Run2017*_t_ss*/*.root")

    h_lostlep = ru.get_summed_histogram(bkg_list_lostlep , histnames)
    h_photon  = ru.get_summed_histogram(bkg_list_photon  , histnames)
    h_qflip   = ru.get_summed_histogram(bkg_list_qflip   , histnames)
    h_fakes   = ru.get_summed_histogram(bkg_list_fakes   , histnames)
    h_prompt  = ru.get_summed_histogram(bkg_list_prompt  , histnames)
    h_sig     = ru.get_summed_histogram(sig_list         , histnames)
    h_vh      = ru.get_summed_histogram(vh_list          , histnames)
    h_www     = ru.get_summed_histogram(www_list         , histnames)
    h_data    = ru.get_summed_histogram(data_list        , histnames)

    h_lostlep .SetName("Lost/three lep")
    h_photon  .SetName("#gamma#rightarrowlepton")
    h_qflip   .SetName("Charge mis-id")
    h_fakes   .SetName("Non-prompt")
    h_prompt  .SetName("Irredu.")
    h_sig     .SetName("WWW")
    h_vh      .SetName("vh")
    h_www     .SetName("www")

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
                #"blind": True if "SR" in histnames[0] else False,
                "blind": False,
                }
    p.plot_hist(
            sigs = [h_sig],
            bgs  = [h_photon, h_qflip, h_fakes, h_lostlep, h_prompt, h_sig.Clone()],
            data = h_data,
            colors = colors,
            syst = None,
            options=alloptions)

if __name__ == "__main__":
    main()

