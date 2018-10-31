#!/bin/bash

VERSION=$1
OUTPUTDIR=outputs/${VERSION}
mkdir -p ${OUTPUTDIR}

rm .jobs.txt
for f in $(ls /nfs-7/userdata/phchang/WWW_babies/${VERSION}/link/*.root); do
    echo './doAnalysis '$f' '${OUTPUTDIR}'/'$(basename $f)' -1 > '${OUTPUTDIR}'/'$(basename $f)'.log 2>&1' >> .jobs.txt
done

#sh rooutil/xargs.sh .jobs.txt
