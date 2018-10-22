#!/bin/bash

VERSION=$1
TREEVARIATIONS="qflip photon fakes prompt lostlep"
OUTPUTDIR=outputs/${VERSION}
mkdir -p ${OUTPUTDIR}

for f in $(ls /nfs-7/userdata/phchang/WWW_babies/${VERSION}/skim/*.root); do
    echo "Launched a job for $f"
    if [[ $f == *"/www_"* ]]; then
        ./doAnalysis $f t_www ${OUTPUTDIR}/t_www_$(basename $f) -1 > ${OUTPUTDIR}/t_www_$(basename $f).log 2>&1 &
    elif [[ $f == *"/vh_"* ]]; then
        ./doAnalysis $f t_www ${OUTPUTDIR}/t_www_$(basename $f) -1 > ${OUTPUTDIR}/t_www_$(basename $f).log 2>&1 &
        for TREEVARIATION in ${TREEVARIATIONS}; do
            ./doAnalysis $f t_${TREEVARIATION} ${OUTPUTDIR}/t_${TREEVARIATION}_$(basename $f) -1 > ${OUTPUTDIR}/t_${TREEVARIATION}_$(basename $f).log 2>&1 &
        done
    elif [[ $f == *"/data_"* ]]; then
        ./doAnalysis $f t ${OUTPUTDIR}/$(basename $f) -1 > ${OUTPUTDIR}/$(basename $f).log 2>&1 &
    else
        for TREEVARIATION in ${TREEVARIATIONS}; do
            ./doAnalysis $f t_${TREEVARIATION} ${OUTPUTDIR}/t_${TREEVARIATION}_$(basename $f) -1 > ${OUTPUTDIR}/t_${TREEVARIATION}_$(basename $f).log 2>&1 &
        done
    fi
done

wait
