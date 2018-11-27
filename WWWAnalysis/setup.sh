export SCRAM_ARCH=slc6_amd64_gcc530   # or whatever scram_arch you need for your desired CMSSW release
export CMSSW_VERSION=CMSSW_9_2_0
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
eval `scramv1 runtime -sh`
cd - > /dev/null

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export LD_LIBRARY_PATH=$DIR:$LD_LIBRARY_PATH

export TEMPLATE_TREE_PATH=/hadoop/cms/store/user/phchang/metis/wwwbaby/OS2017_v4.0.5/MAKER_ZZZ_TuneCP5_13TeV-amcatnlo-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_OS2017_v4.0.5/output_1.root
