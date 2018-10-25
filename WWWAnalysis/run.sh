#!/bin/bash

VERSION=$1
TREEVARIATIONS="qflip photon fakes prompt lostlep"
OUTPUTDIR=outputs/${VERSION}
mkdir -p ${OUTPUTDIR}

rm .jobs.txt
for f in $(ls /nfs-7/userdata/phchang/WWW_babies/${VERSION}/skim/*.root); do
    if [[ $f == *"hpmpm"* ]]; then continue; fi
    if [[ $f == *"wprime"* ]]; then continue; fi
    if [[ $f == *"whsusy"* ]]; then continue; fi
    if [[ $f == *"data_Run"* ]]; then continue; fi
    echo "Writing jobs for $f to .jobs.txt"
    if [[ $f == *"/www_"* ]]; then
        TREEVARIATION="www"
        echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
    elif [[ $f == *"/vh_"* ]]; then
        TREEVARIATION="www"
        echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
        for TREEVARIATION in ${TREEVARIATIONS}; do
            echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
        done
    elif [[ $f == *"/data_ss"* ]]; then
        echo './doAnalysis '$f' 't_ss' '${OUTPUTDIR}'/'$(basename $f)' -1 > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt
        # data-driven fake estimate
        echo './doAnalysis '$f' 't_ss' '${OUTPUTDIR}'/t_fakes_'$(basename $f)' -1 > '${OUTPUTDIR}'/t_fakes_'$(basename $f)'.log 2>&1' >> .jobs.txt
    elif [[ $f == *"/data"* ]]; then
        if [[ $f == *"WWW2017"* ]]; then
            echo './doAnalysis '$f' 't' '${OUTPUTDIR}'/'$(basename $f)' -1 > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt
        else
            :
            # if data but not data_ss nor WWW_2017, then skip
        fi
    else
        for TREEVARIATION in ${TREEVARIATIONS}; do
            echo './doAnalysis '$f' 't_${TREEVARIATION}' '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)' -1 > '${OUTPUTDIR}'/t_'${TREEVARIATION}'_'$(basename $f)'.log 2>&1' >> .jobs.txt
        done
    fi
done

sh rooutil/xargs.sh .jobs.txt
