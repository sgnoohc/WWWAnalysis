#!/bin/bash

if [ -z $1 ]; then
    echo "EXAMPLE: sh hadd.sh v1.0.9"
fi

VERSION=$1
HADOOPDIR=/hadoop/cms/store/user/phchang/metis/wwwbaby/WWW_${VERSION}/*${VERSION}/merged/
DIR=/nfs-7/userdata/phchang/WWW_babies/WWW_${VERSION}/skim/

mkdir -p ${DIR}
cp ${HADOOPDIR}/*.root ${DIR}
python rooutil/duplicate.py --treename t_ss --output data_ss.root $DIR/data_Run2016*_ee_* $DIR/data_Run2016*_em_* $DIR/data_Run2016*_mm_*
cp -v data_ss.root $DIR/data_ss.root

#hadd -f $DIR/bkg.root $(ls $DIR/*.root | grep -v data_ | grep -v bkg_ | grep -v www_2l)
#TREENAMES="qflip photon fakes prompt lostlep"
#for TREENAME in ${TREENAMES}; do
#    python rooutil/split.py -t t_${TREENAME} --output $DIR/bkg_${TREENAME}.root $(ls $DIR/*.root | grep -v data_ | grep -v bkg_ | grep -v www_2l | wc -l)
#done

#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

#mkdir -p ${DIR}
#cp ${HADOOPDIR}/*.root ${DIR}
##ln -s ${HADOOPDIR}/*.root ${DIR}
#
##hadd -f $DIR/bkg.root $(ls $DIR/*.root | grep -v data_ | grep -v www_2l | grep -v bkg.root | grep -v ttbar_incl)
##hadd -f $DIR/bkg.root $DIR/dy_m1050_madgraph_skim_1_1* $DIR/dy_m50_madgraph_ext1_skim_1_1* $DIR/wjets_ln_madgraph_ext1_skim_1_1* $DIR/ttbar_1l_powheg_skim_1_1* $DIR/tth_nonbb_powheg_skim_1_1* $DIR/ttw_ln_amcnlo_skim_1_1* $DIR/ttz_m10llnn_amcnlo_skim_1_1* $DIR/ttz_m1to10ll_amcnlo_skim_1_1* $DIR/ww_dblsctincl_pythia_skim_1_1* $DIR/ww_incl_pythia_skim_1_1* $DIR/wz_incl_pythia_skim_1_1* $DIR/zz_4l_powheg_skim_1_1* $DIR/tzq_ll_amcnlo_skim_1_1* $DIR/sttw_incltbr_powheg_skim_1_1* $DIR/sttw_incltop_powheg_skim_1_1* $DIR/wwz_incl_amcnlo_skim_1_1* $DIR/wzg_incl_amcnlo_skim_1_1* $DIR/wzz_incl_amcnlo_skim_1_1* $DIR/zzz_incl_amcnlo_skim_1_1*
##hadd -f $DIR/bkg.root $DIR/dy_m50_madgraph_ext1_skim_1_1* $DIR/wjets_ln_madgraph_ext1_skim_1_1* $DIR/ttbar_1l_powheg_skim_1_1* $DIR/ttw_ln_amcnlo_skim_1_1* $DIR/ttz_m10llnn_amcnlo_skim_1_1* $DIR/ttz_m1to10ll_amcnlo_skim_1_1* $DIR/ww_dblsctincl_pythia_skim_1_1* $DIR/wz_incl_pythia_skim_1_1* $DIR/zz_4l_powheg_skim_1_1* $DIR/tzq_ll_amcnlo_skim_1_1* $DIR/sttw_incltbr_powheg_skim_1_1* $DIR/sttw_incltop_powheg_skim_1_1* $DIR/wwz_incl_amcnlo_skim_1_1* $DIR/wzg_incl_amcnlo_skim_1_1* $DIR/wzz_incl_amcnlo_skim_1_1* $DIR/zzz_incl_amcnlo_skim_1_1*
#TREENAMES="qflip photon fakes prompt lostlep"
#for TREENAME in ${TREENAMES}; do
#    #python rooutil/split.py -t t_${TREENAME} --output $DIR/bkg_${TREENAME}.root $(ls $DIR/*.root | grep -v data_ | grep -v www_2l | grep -v os_split | grep -v bkg.root | grep -v bkg_1.root | grep -v bkg__1.root | grep -v ttbar_incl)
#    python rooutil/split.py -t t_${TREENAME} --output $DIR/bkg_${TREENAME}.root $DIR/dy_m50_madgraph_ext1_skim_1_1* $DIR/wjets_ln_madgraph_ext1_skim_1_1* $DIR/ttbar_1l_powheg_skim_1_1* $DIR/ttw_ln_amcnlo_skim_1_1* $DIR/ttz_m10llnn_amcnlo_skim_1_1* $DIR/ttz_m1to10ll_amcnlo_skim_1_1* $DIR/ww_dblsctincl_pythia_skim_1_1* $DIR/wz_incl_pythia_skim_1_1* $DIR/zz_4l_powheg_skim_1_1* $DIR/tzq_ll_amcnlo_skim_1_1* $DIR/sttw_incltbr_powheg_skim_1_1* $DIR/sttw_incltop_powheg_skim_1_1* $DIR/wwz_incl_amcnlo_skim_1_1* $DIR/wzg_incl_amcnlo_skim_1_1* $DIR/wzz_incl_amcnlo_skim_1_1* $DIR/zzz_incl_amcnlo_skim_1_1*
#done
##python rooutil/duplicate.py --output data.root $DIR/data_Run2016*_ee_* $DIR/data_Run2016*_em_* $DIR/data_Run2016*_mm_*
##python rooutil/duplicate.py --treename t_os --output data_os.root $DIR/data_Run2016*_ee_* $DIR/data_Run2016*_em_* $DIR/data_Run2016*_mm_*
#python rooutil/duplicate.py --treename t_ss --output data_ss.root $DIR/data_Run2016*_ee_* $DIR/data_Run2016*_em_* $DIR/data_Run2016*_mm_*
##cp -v data_os.root $DIR/data_os.root
#cp -v data_ss.root $DIR/data_ss.root
##python rooutil/split.py --output /nfs-7/userdata/phchang/WWW_babies/WWW_v1.0.23/skim/dy_m50_mgmlm_os_split_ext1_skim.root -t t_os /nfs-7/userdata/phchang/WWW_babies/WWW_v1.0.23/skim/dy_m50_mgmlm_ext1_skim_1_1.root
##python rooutil/split.py --output /nfs-7/userdata/phchang/WWW_babies/WWW_v1.0.23/skim/ttbar_incl_powheg_os_split_skim_1_1.root -t t_os /nfs-7/userdata/phchang/WWW_babies/WWW_v1.0.23/skim/ttbar_incl_powheg_skim_1_1.root
#
##$DIR/dy_m1050_madgraph_skim_1_1* $DIR/dy_m50_madgraph_ext1_skim_1_1* $DIR/wjets_ln_madgraph_ext1_skim_1_1* $DIR/ttbar_1l_powheg_skim_1_1* $DIR/tth_nonbb_powheg_skim_1_1* $DIR/ttw_ln_amcnlo_skim_1_1* $DIR/ttz_m10llnn_amcnlo_skim_1_1* $DIR/ttz_m1to10ll_amcnlo_skim_1_1* $DIR/ww_dblsctincl_pythia_skim_1_1* $DIR/ww_incl_pythia_skim_1_1* $DIR/wz_incl_pythia_skim_1_1* $DIR/zz_4l_powheg_skim_1_1* $DIR/tzq_ll_amcnlo_skim_1_1* $DIR/sttw_incltbr_powheg_skim_1_1* $DIR/sttw_incltop_powheg_skim_1_1* $DIR/wwz_incl_amcnlo_skim_1_1* $DIR/wzg_incl_amcnlo_skim_1_1* $DIR/wzz_incl_amcnlo_skim_1_1* $DIR/zzz_incl_amcnlo_skim_1_1*
