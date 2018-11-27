#!/bin/bash

# This script loops over the baby directory area and aggregates all the commands that need to be called to run the job.
# The output will be stored into .jobs.txt
# Then rooutil/xargs.sh will call the .jobs.txt to execute the jobs in parallel (36 cores)


# Help message
help()
{
    echo "Error - Usage:"
    echo "$0 VERSION [REGIONS]"
    exit
}
if [ -z $1 ]; then help; fi

# The first argument will be the WWW baby tag (e.g. WWW2017_v4.0.5 produced around November of 2018)
VERSION=$1

# The second argument will decide which regions to run over.
# If set to "all" it runs over all regions defined in the analysis program.
if [ -z $2 ]; then
    regions="all"
else
    regions=$2
fi

# These are constant variables that do not need to be changed
TREEVARIATIONS="qflip photon fakes prompt lostlep"

# Now create the output directory
OUTPUTDIR=outputs/${VERSION}
mkdir -p ${OUTPUTDIR}

# Clear the output .jobs.txt
rm .jobs.txt

# Loop over the baby ntuple files
for f in $(ls /nfs-7/userdata/phchang/WWW_babies/${VERSION}/skim/*.root); do

    # These are to skip over the BSM interpretation files that was done for 2016 analysis.
    if [[ $f == *"hpmpm"* ]]; then continue; fi
    if [[ $f == *"wprime"* ]]; then continue; fi
    if [[ $f == *"whsusy"* ]]; then continue; fi

    # Print out which ntuple is being considered to write out jobs to .jobs.txt
    echo "Writing jobs for $f to .jobs.txt"

    #
    # 1. If the file name matches "www_" then this is signal sample
    #
    if [[ $f == *"/www_"* ]]; then

        # The TTree that will be run over from this file is always t_"www" (i.e. TTree named "t_www")
        # So this variable will be used to write out the command with the proper argument set to "t_www"
        TREEVARIATION="www"

        # If the filename contains "_bw15_" this is the sample where Mia regenerated the official sample with proper W width set for the WWW sample. (Official WWW sample had narrow-width W which is incorrect.)
        # Ask around HJ, Mia, or Philip who worked on it during Nov. 2018
        if [[ $f == *"_bw15_"* ]]; then
            # if BW15
            echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
        # Normally, you'd be falling under this category
        else
            # Write out signal sample job
            echo './doAnalysis '$f' 't' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
        fi
    #
    # 2. If the file name matches "vh_*" then this is also a signal sample. However, the non WH->WWW events in VH samples will also need to be included as backgrounds.
    #
    elif [[ $f == *"/vh_"* ]]; then

        # So first process the signal part of the "vh_*" sample
        # So this variable will be used to write out the command with the proper argument set to "t_www"
        TREEVARIATION="www"

        # Write out the VH signal portion of the command line
        echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt

        # Now write out the background part of the VH sample
        # Loop over the tree varations. (e.g. qflip, prompt, lostlep, etc.)
        for TREEVARIATION in ${TREEVARIATIONS}; do

            # Now write out the command to run over VH background events in various TTree's like t_qflip, t_lostlep, etc.
            echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt

        done
    # (not used for 2017 and onwards)
    # 3. If the file name matches "data_ss" this is for the 2016 analysis primarily for the sample used for final results.
    #    So at that time, the data_ss.root was the duplicate removed data hadded ntuple.
    #    (Normally, when you just had DoubleEG/MuonEG/DoubleMuon you get duplicate events, due to same event firing various trigger and ending up in various primary datasets.)
    #    So at the time, Philip merged but also duplicate removed and put all of them in one file called "data_ss.root"
    #    For 2017 and onwards, the duplicate removal was handled via a flag in the ntuple baby. (e.g. pass_duplicate_ee_em_mm, etc. See the implementation in github.com:cmstas/VVVBabyMaker for more detail)
    #
    #    There are mainly two things to do. (1) processing as data events, and (2) data-driven fake estimate
    #
    elif [[ $f == *"/data_ss"* ]]; then

        # Write out the job command for processing data events
        echo './doAnalysis '$f' 't_ss' '${OUTPUTDIR}'/t_ss_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt

        # Write out the job command for data-driven fake estimate
        # The analysis source file (i.e. process.cc) recognizes the output file pattern "ddfakes" and flips the internal switch to process it as a data-driven fake estimate
        echo './doAnalysis '$f' 't_ss' '${OUTPUTDIR}'/t_ddfakes_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_ddfakes_'$(basename $f)'.log 2>&1' >> .jobs.txt

    #
    # 4. If the file name starts with "data*", then process the data events.
    #    There are mainly two things to do. (1) processing as data events, and (2) data-driven fake estimate
    #
    elif [[ $f == *"/data"* ]]; then

        # If the sample is for 2017 events
        if [[ $f == *"WWW2017"* ]]; then

            # Write out the job command for processing data events
            echo './doAnalysis '$f' 't' '${OUTPUTDIR}'/t_ss_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt

            # Write out the job command for data-driven fake estimate
            # The analysis source file (i.e. process.cc) recognizes the output file pattern "ddfakes" and flips the internal switch to process it as a data-driven fake estimate
            echo './doAnalysis '$f' 't' '${OUTPUTDIR}'/t_ddfakes_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt

        # TODO: The consistent treatment of 2016 analysis with the same version of baby maker hasn't been done yet. (As of Nov. 2018)
        #       Once the VVVBabyMaker/master version produced 2016 AND 2017 with a same setup, this will be implemented and the section "3." will be deprecated.
        else
            :
            # if data but not data_ss nor WWW_2017, then skip
            #echo './doAnalysis '$f' 't' '${OUTPUTDIR}'/'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt
        fi
    #
    # 5. Background MC
    #    There are two parts (1) regular job processing, (2) bkg subtraction for dd-fake estimates
    #
    else

        # The tzq samples are HUGE and is very TINY. We skip it. Just takes up time and does nothing.
        if [[ $f == *"tzq"* ]]; then continue; fi

        # Loop over the various TTree suffixes and write out jobs for each TTree
        for TREEVARIATION in ${TREEVARIATIONS}; do

            # Write out jobs for t_{$TREEVARIATION} for regular mc bkg processing
            echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt

        done

        # Write out job commands for processing non-"non-prompt" backgrounds in application regions
        echo './doAnalysis '$f' 't_ss' '${OUTPUTDIR}'/t_ewksubt_'$(basename $f)' -1 '${regions}'  > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
    fi
done

sh rooutil/xargs.sh .jobs.txt
