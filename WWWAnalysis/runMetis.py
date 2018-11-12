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
    job_tag = "WWW{}_analysis_v0.12.1".format(data_year) # All CR included
    input_ntup_tag = "WWW2017_v4.0.5"
    base_dir_path = "/hadoop/cms/store/user/phchang/metis/wwwbaby/{}/".format(input_ntup_tag)
    tar_files = ["doAnalysis", "setup.sh", "scalefactors/*.root", "scalefactors/*/*/*/*/sf.root"]
    hadoop_dirname = "wwwanalysis"
    trees = ["t_lostlep", "t_fakes", "t_prompt", "t_qflip", "t_photon"]

    all_samples = glob.glob("{}/*".format(base_dir_path))

    samples_map = {}
    arguments_map = {}

    # Bkg sample
    for sample_dir_path in [ x for x in all_samples if "WWW_" not in x and "_Run2017" not in x]:
        for tree in trees:
            sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
            samples_map[sample_name] = sample_dir_path
            arguments_map[sample_name] = "{} {}".format(tree, tree)

    # Signal sample
    for sample_dir_path in [ x for x in all_samples if "WWW_" in x or "VHToNonbb" in x ]:
        tree = "t_www"
        sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
        samples_map[sample_name] = sample_dir_path
        arguments_map[sample_name] = "{} {}".format(tree, tree)

    # Data sample
    for sample_dir_path in [ x for x in all_samples if "_Run2017" in x ]:
        tree = "t_ss"
        sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
        samples_map[sample_name] = sample_dir_path
        arguments_map[sample_name] = "{} {}".format(tree, tree)

    # Data-driven fake sample
    for sample_dir_path in [ x for x in all_samples if "_Run2017" in x ]:
        tree = "t_fakes"
        sample_name = sample_dir_path.split("MAKER_")[1].split("_"+input_ntup_tag)[0] + "_" + tree
        samples_map[sample_name] = sample_dir_path
        arguments_map[sample_name] = "{} {}".format("t_ss", tree)

#    keys = samples_map.keys()
#    keys.sort()
#
#    for key in keys:
#        print key
#        print arguments_map[key]
#        print samples_map[key]

    ru.submit_metis(job_tag, samples_map, arguments_map=arguments_map, tar_files=tar_files, hadoop_dirname=hadoop_dirname, files_per_output=1, globber="merged/*.root")

if __name__ == "__main__":
    main()

#eof
