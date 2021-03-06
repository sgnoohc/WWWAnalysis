##!/bin/env python

from rooutil import rooutil as ru
import glob
import os

def main():

    data_year = "2017"
    job_tag = "WWW{}_analysis_v0.6.1".format(data_year) # where i think fakerate is set correctly
    job_tag = "WWW{}_analysis_v0.7.1".format(data_year) # Added ptcorretarolledcoarse added
    job_tag = "WWW{}_analysis_v0.8.1".format(data_year) # Fixed baby mode handling
    job_tag = "WWW{}_analysis_v0.9.1".format(data_year) # Added 3lep fakerate
    job_tag = "WWW{}_analysis_v0.10.1".format(data_year) # Added 3lep fakerate
    job_tag = "WWW{}_analysis_v0.11.1".format(data_year) # All systematics included
    job_tag = "WWW{}_analysis_v0.12.1".format(data_year) # All CR included (Failed campaign)
    job_tag = "WWW{}_analysis_v0.13.1".format(data_year) # All CR included (histogram/cutflow fixed)
    job_tag = "WWW{}_analysis_v0.14.1".format(data_year) # All ARTrilep included
    job_tag = "WWW{}_analysis_v0.15.1".format(data_year) # All AR BTag/LMET (failed as lepsf were incorrectly applied to AR regions)
    job_tag = "WWW{}_analysis_v0.16.1".format(data_year) # All AR BTag/LMET
    job_tag = "WWW{}_analysis_v0.17.1".format(data_year) # All AR BTag/LMET (BTCRAR had wrong two lep selection for in Mjj)
    job_tag = "WWW{}_analysis_v0.18.1".format(data_year) # Including prompt subtraction in fake estimation (failed)
    job_tag = "WWW{}_analysis_v0.19.1".format(data_year) # Including prompt subtraction in fake estimation
    job_tag = "WWW{}_analysis_v0.20.1".format(data_year) # Including prompt subtraction in fake estimation (all histograms)
    job_tag = "WWW{}_analysis_v0.21.1".format(data_year) # LXECR/LXECRAR only
    job_tag = "WWW{}_analysis_v0.22.1".format(data_year) # LXECR/LXECRAR only (ptcorremu rolled)
    job_tag = "WWW{}_analysis_v0.23.1".format(data_year) # Newer setCut (failed)
    job_tag = "WWW{}_analysis_v0.24.1".format(data_year) # Newer setCut (failed)
    job_tag = "WWW{}_analysis_v0.25.1".format(data_year) # Newer setCut (hasz messed up)
    job_tag = "WWW{}_analysis_v0.26.1".format(data_year) # Newer setCut (hasz_ss sf messed up)
    job_tag = "WWW{}_analysis_v0.27.1".format(data_year) # Newer setCut
    job_tag = "WWW{}_analysis_v0.28.1".format(data_year) # Newer histogramming
    job_tag = "WWW{}_analysis_v0.29.1".format(data_year) # NaN problem fixed
    job_tag = "WWW{}_analysis_v0.30.1".format(data_year) # Exclude WZ_Tune
    job_tag = "WWW{}_analysis_v0.31.1".format(data_year) # Sumw2 fixed on cutflow histograms
    job_tag = "WWW{}_analysis_v0.32.1".format(data_year) # Sumw2 fixed on cutflow histograms
    job_tag = "WWW{}_analysis_v0.33.1".format(data_year) # Run OS sample (accidentally ran everything. too big to handle)
    job_tag = "WWW{}_analysis_v0.34.1".format(data_year) # Run OS sample (option error)
    job_tag = "WWW{}_analysis_v0.35.1".format(data_year) # Run OS sample
    job_tag = "WWW{}_analysis_v0.36.1".format(data_year) # Run OS sample
    job_tag = "WWW{}_analysis_v0.37.1".format(data_year) # Run OS sample
    job_tag = "WWW{}_analysis_v0.38.1".format(data_year) # Run OS sample
    job_tag = "WWW{}_analysis_v0.39.1".format(data_year) # Run OS sample (correct pileup?)
    job_tag = "WWW{}_analysis_v0.40.1".format(data_year) # Run OS sample (correct pileup?)
    job_tag = "WWW{}_analysis_v0.41.1".format(data_year) # Run OS sample (failed)
    job_tag = "WWW{}_analysis_v0.42.1".format(data_year) # Run OS sample (failed)
    job_tag = "WWW{}_analysis_v0.43.1".format(data_year) # Run OS sample (correct trigger)
    job_tag = "WWW{}_analysis_v0.44.1".format(data_year) # Run OS sample (correct trigger)
    job_tag = "WWW{}_analysis_v0.45.1".format(data_year) # Run WWW2017
    input_ntup_tag = "WWW2017_v4.0.5"
    #input_ntup_tag = "OS2017_v4.0.5"
    base_dir_path = "/hadoop/cms/store/user/phchang/metis/wwwbaby/{}/".format(input_ntup_tag)
    tar_files = ["doAnalysis", "setup.sh", "scalefactors/*.root", "scalefactors/*/*/*/*/sf.root"]
    hadoop_dirname = "wwwanalysis"
    trees = ["t"] if "OS" in input_ntup_tag else ["t_lostlep", "t_fakes", "t_prompt", "t_qflip", "t_photon"]

    regions="SRSSee,SRSSem,SRSSmm,SRSSSideee,SRSSSideem,SRSSSidemm,SR0SFOS,SR1SFOS,SR2SFOS,WZCRSSee,WZCRSSem,WZCRSSmm,WZCR1SFOS,WZCR2SFOS,OSCRee,OSCRem,OSCRmm"

    all_samples = glob.glob("{}/*".format(base_dir_path))

    samples_map = {}
    arguments_map = {}

    # Bkg sample
    for sample_dir_path in [ x for x in all_samples if "WWW_" not in x and "_Run2017" not in x and "tZq" not in x]:
        for tree in trees:
            sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
            samples_map[sample_name] = sample_dir_path
            arguments_map[sample_name] = "{} {} {}".format(tree, tree, regions)

    # Signal sample
    for sample_dir_path in [ x for x in all_samples if "WWW_" in x or "VHToNonbb" in x ]:
        tree = "t" if "OS" in input_ntup_tag else "t_www"
        sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
        samples_map[sample_name] = sample_dir_path
        arguments_map[sample_name] = "{} {} {}".format(tree, tree, regions)

    # Data sample
    for sample_dir_path in [ x for x in all_samples if "_Run2017" in x ]:
        tree = "t" if "OS" in input_ntup_tag else "t_ss"
        sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
        samples_map[sample_name] = sample_dir_path
        arguments_map[sample_name] = "{} {} {}".format(tree, tree, regions)

    # Data-driven fake estimate and ewksubtraction only for SS analysis and not for opposite sign (OS)
    if "OS" not in input_ntup_tag:
        # Data-driven fake sample
        for sample_dir_path in [ x for x in all_samples if "_Run2017" in x ]:
            tree = "t_ddfakes"
            sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
            samples_map[sample_name] = sample_dir_path
            arguments_map[sample_name] = "{} {} {}".format("t_ss", tree, regions)

        # Electroweak subtraction in data-drivek fake estimate
        for sample_dir_path in [ x for x in all_samples if "_Run2017" not in x and "tZq" not in x]:
            tree = "t_ewksubt"
            sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
            samples_map[sample_name] = sample_dir_path
            arguments_map[sample_name] = "{} {} {}".format("t_ss", tree, regions)

    ru.submit_metis(job_tag, samples_map, arguments_map=arguments_map, tar_files=tar_files, hadoop_dirname=hadoop_dirname, files_per_output=1, globber="merged/*.root" if "OS" not in input_ntup_tag else "*.root")

if __name__ == "__main__":
    main()

#eof
