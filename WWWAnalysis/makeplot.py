#!/bin/env python

from rooutil import rooutil as ru
from rooutil import plottery_wrapper as p
import glob

def main():
    histnames = ["SRSSeeFull__yield", "SRSSemFull__yield", "SRSSmmFull__yield", "SRSSSideeeFull__yield", "SRSSSideemFull__yield", "SRSSSidemmFull__yield", "SR0SFOSFull__yield", "SR1SFOSFull__yield", "SR2SFOSFull__yield"]
    h_lostlep = ru.get_summed_histogram(glob.glob("outputs/t_lostlep_*.root"), histnames)
    h_photon  = ru.get_summed_histogram(glob.glob("outputs/t_photon_*.root"), histnames)
    h_qflip   = ru.get_summed_histogram(glob.glob("outputs/t_qflip_*.root"), histnames)
    h_fakes   = ru.get_summed_histogram(glob.glob("outputs/t_fakes_*.root"), histnames)
    h_prompt  = ru.get_summed_histogram(glob.glob("outputs/t_prompt_*.root"), histnames)
    h_sig     = ru.get_summed_histogram(glob.glob("outputs/t_www_www*.root") + glob.glob("outputs/t_www_vh*.root"), histnames)

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
                }
    p.plot_hist(
            sigs = [h_sig],
            bgs  = [h_photon, h_qflip, h_fakes, h_lostlep, h_prompt],
            data = None,
            colors = colors,
            syst = None,
            options=alloptions)

if __name__ == "__main__":
    main()
