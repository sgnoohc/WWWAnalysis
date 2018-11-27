#!/bin/bash

if [ -z $1 ]; then
    echo "EXAMPLE: sh hadd.sh v1.0.9"
    exit
fi

VERSION=$1
HADOOPDIR=/hadoop/cms/store/user/phchang/metis/wwwbaby/${VERSION}/*${VERSION}/merged/
DIR=/nfs-7/userdata/phchang/WWW_babies/${VERSION}/skim/

mkdir -p ${DIR}
cp ${HADOOPDIR}/*.root ${DIR}
