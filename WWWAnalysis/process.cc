#include "process.h"

//_______________________________________________________________________________________________________
int process(const char* input_paths, const char* input_tree_name, const char* output_file_name, int nEvents)
{
    // Creating output file where we will put the outputs of the processing
    TFile* ofile = new TFile(output_file_name, "recreate");

    // Create a TChain of the input files
    // The input files can be comma separated (e.g. "file1.root,file2.root")
    TChain* ch = RooUtil::FileUtil::createTChain(input_tree_name, input_paths);

    // Create a Looper object to loop over input files
    RooUtil::Looper<wwwtree> looper(ch, &www, nEvents);

    // Some case-by-case checking needed for WWW_v1.2.2 (should be no longer necessary later on)
    bool is2017 = TString(input_paths).Contains("WWW2017");
    bool isWWW = TString(input_paths).Contains("www_2l_");

    // For fake estimations, we use data-driven method.
    // When looping over data and the output_path is set to have a "fakes" substring included we turn on the fake-weight settings
    const bool doSystematics = not TString(input_paths).Contains("data_");
    bool doFakeEstimation = TString(input_paths).Contains("data_") && TString(output_file_name).Contains("fakes");
    bool isData = TString(input_paths).Contains("data_");

    // Cutflow utility object that creates a tree structure of cuts
    RooUtil::Cutflow cutflow(ofile);
    cutflow.addCut("CutWeight");
    cutflow.addCutToLastActiveCut("CutPresel");
    cutflow.addCutToLastActiveCut("CutTrigger");

    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutSRDilep");
    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutSRTrilep");
    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutWZCRDilep");
    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutWZCRTrilep");
    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutARDilep");
    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutARTrilep");

    // Same-sign Mjj on-W region
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("SRSSmm");
    cutflow.addCutToLastActiveCut("SRSSmmTVeto");
    cutflow.addCutToLastActiveCut("SRSSmmNj2");
    cutflow.addCutToLastActiveCut("SRSSmmNb0");
    cutflow.addCutToLastActiveCut("SRSSmmMjjW");
    cutflow.addCutToLastActiveCut("SRSSmmMjjL");
    cutflow.addCutToLastActiveCut("SRSSmmDetajjL");
    cutflow.addCutToLastActiveCut("SRSSmmMET");
    cutflow.addCutToLastActiveCut("SRSSmmMllSS");
    cutflow.addCutToLastActiveCut("SRSSmmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("SRSSem");
    cutflow.addCutToLastActiveCut("SRSSemTVeto");
    cutflow.addCutToLastActiveCut("SRSSemNj2");
    cutflow.addCutToLastActiveCut("SRSSemNb0");
    cutflow.addCutToLastActiveCut("SRSSemMjjW");
    cutflow.addCutToLastActiveCut("SRSSemMjjL");
    cutflow.addCutToLastActiveCut("SRSSemDetajjL");
    cutflow.addCutToLastActiveCut("SRSSemMET");
    cutflow.addCutToLastActiveCut("SRSSemMllSS");
    cutflow.addCutToLastActiveCut("SRSSemMTmax");
    cutflow.addCutToLastActiveCut("SRSSemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("SRSSee");
    cutflow.addCutToLastActiveCut("SRSSeeZeeVt");
    cutflow.addCutToLastActiveCut("SRSSeeTVeto");
    cutflow.addCutToLastActiveCut("SRSSeeNj2");
    cutflow.addCutToLastActiveCut("SRSSeeNb0");
    cutflow.addCutToLastActiveCut("SRSSeeMjjW");
    cutflow.addCutToLastActiveCut("SRSSeeMjjL");
    cutflow.addCutToLastActiveCut("SRSSeeDetajjL");
    cutflow.addCutToLastActiveCut("SRSSeeMET");
    cutflow.addCutToLastActiveCut("SRSSeeMllSS");
    cutflow.addCutToLastActiveCut("SRSSeeFull");

    // Same-sign Mjj off-W region
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("SRSSSidemm");
    cutflow.addCutToLastActiveCut("SRSSSidemmTVeto");
    cutflow.addCutToLastActiveCut("SRSSSidemmNj2");
    cutflow.addCutToLastActiveCut("SRSSSidemmNb0");
    cutflow.addCutToLastActiveCut("SRSSSidemmMjjW");
    cutflow.addCutToLastActiveCut("SRSSSidemmMjjL");
    cutflow.addCutToLastActiveCut("SRSSSidemmDetajjL");
    cutflow.addCutToLastActiveCut("SRSSSidemmMET");
    cutflow.addCutToLastActiveCut("SRSSSidemmMllSS");
    cutflow.addCutToLastActiveCut("SRSSSidemmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("SRSSSideem");
    cutflow.addCutToLastActiveCut("SRSSSideemTVeto");
    cutflow.addCutToLastActiveCut("SRSSSideemNj2");
    cutflow.addCutToLastActiveCut("SRSSSideemNb0");
    cutflow.addCutToLastActiveCut("SRSSSideemMjjW");
    cutflow.addCutToLastActiveCut("SRSSSideemMjjL");
    cutflow.addCutToLastActiveCut("SRSSSideemDetajjL");
    cutflow.addCutToLastActiveCut("SRSSSideemMET");
    cutflow.addCutToLastActiveCut("SRSSSideemMllSS");
    cutflow.addCutToLastActiveCut("SRSSSideemMTmax");
    cutflow.addCutToLastActiveCut("SRSSSideemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("SRSSSideee");
    cutflow.addCutToLastActiveCut("SRSSSideeeZeeVt");
    cutflow.addCutToLastActiveCut("SRSSSideeeTVeto");
    cutflow.addCutToLastActiveCut("SRSSSideeeNj2");
    cutflow.addCutToLastActiveCut("SRSSSideeeNb0");
    cutflow.addCutToLastActiveCut("SRSSSideeeMjjW");
    cutflow.addCutToLastActiveCut("SRSSSideeeMjjL");
    cutflow.addCutToLastActiveCut("SRSSSideeeDetajjL");
    cutflow.addCutToLastActiveCut("SRSSSideeeMET");
    cutflow.addCutToLastActiveCut("SRSSSideeeMllSS");
    cutflow.addCutToLastActiveCut("SRSSSideeeFull");

    // Trilep regions
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("SR0SFOS");
    cutflow.addCutToLastActiveCut("SR0SFOSNj1");
    cutflow.addCutToLastActiveCut("SR0SFOSNb0");
    cutflow.addCutToLastActiveCut("SR0SFOSPt3l");
    cutflow.addCutToLastActiveCut("SR0SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("SR0SFOSMET");
    cutflow.addCutToLastActiveCut("SR0SFOSMll");
    cutflow.addCutToLastActiveCut("SR0SFOSM3l");
    cutflow.addCutToLastActiveCut("SR0SFOSZVt");
    cutflow.addCutToLastActiveCut("SR0SFOSMTmax");
    cutflow.addCutToLastActiveCut("SR0SFOSFull");
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("SR1SFOS");
    cutflow.addCutToLastActiveCut("SR1SFOSNj1");
    cutflow.addCutToLastActiveCut("SR1SFOSNb0");
    cutflow.addCutToLastActiveCut("SR1SFOSPt3l");
    cutflow.addCutToLastActiveCut("SR1SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("SR1SFOSMET");
    cutflow.addCutToLastActiveCut("SR1SFOSMll");
    cutflow.addCutToLastActiveCut("SR1SFOSM3l");
    cutflow.addCutToLastActiveCut("SR1SFOSZVt");
    cutflow.addCutToLastActiveCut("SR1SFOSMT3rd");
    cutflow.addCutToLastActiveCut("SR1SFOSFull");
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("SR2SFOS");
    cutflow.addCutToLastActiveCut("SR2SFOSNj1");
    cutflow.addCutToLastActiveCut("SR2SFOSNb0");
    cutflow.addCutToLastActiveCut("SR2SFOSPt3l");
    cutflow.addCutToLastActiveCut("SR2SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("SR2SFOSMET");
    cutflow.addCutToLastActiveCut("SR2SFOSMll");
    cutflow.addCutToLastActiveCut("SR2SFOSM3l");
    cutflow.addCutToLastActiveCut("SR2SFOSZVt");
    cutflow.addCutToLastActiveCut("SR2SFOSFull");

    // Same-sign WZ CR
    cutflow.getCut("CutWZCRDilep");
    cutflow.addCutToLastActiveCut("WZCRSSmm");
    cutflow.addCutToLastActiveCut("WZCRSSmmTVeto");
    cutflow.addCutToLastActiveCut("WZCRSSmmNj2");
    cutflow.addCutToLastActiveCut("WZCRSSmmNb0");
    cutflow.addCutToLastActiveCut("WZCRSSmmMjjL");
    cutflow.addCutToLastActiveCut("WZCRSSmmDetajjL");
    cutflow.addCutToLastActiveCut("WZCRSSmmMET");
    cutflow.addCutToLastActiveCut("WZCRSSmmMllSS");
    cutflow.addCutToLastActiveCut("WZCRSSmmFull");
    cutflow.getCut("CutWZCRDilep");
    cutflow.addCutToLastActiveCut("WZCRSSem");
    cutflow.addCutToLastActiveCut("WZCRSSemTVeto");
    cutflow.addCutToLastActiveCut("WZCRSSemNj2");
    cutflow.addCutToLastActiveCut("WZCRSSemNb0");
    cutflow.addCutToLastActiveCut("WZCRSSemMjjL");
    cutflow.addCutToLastActiveCut("WZCRSSemDetajjL");
    cutflow.addCutToLastActiveCut("WZCRSSemMET");
    cutflow.addCutToLastActiveCut("WZCRSSemMllSS");
    cutflow.addCutToLastActiveCut("WZCRSSemMTmax");
    cutflow.addCutToLastActiveCut("WZCRSSemFull");
    cutflow.getCut("CutWZCRDilep");
    cutflow.addCutToLastActiveCut("WZCRSSee");
    cutflow.addCutToLastActiveCut("WZCRSSeeZeeVt");
    cutflow.addCutToLastActiveCut("WZCRSSeeTVeto");
    cutflow.addCutToLastActiveCut("WZCRSSeeNj2");
    cutflow.addCutToLastActiveCut("WZCRSSeeNb0");
    cutflow.addCutToLastActiveCut("WZCRSSeeMjjL");
    cutflow.addCutToLastActiveCut("WZCRSSeeDetajjL");
    cutflow.addCutToLastActiveCut("WZCRSSeeMET");
    cutflow.addCutToLastActiveCut("WZCRSSeeMllSS");
    cutflow.addCutToLastActiveCut("WZCRSSeeFull");

    // Trilep WZ CR
    cutflow.getCut("CutWZCRTrilep");
    cutflow.addCutToLastActiveCut("WZCR1SFOS");
    cutflow.addCutToLastActiveCut("WZCR1SFOSNj1");
    cutflow.addCutToLastActiveCut("WZCR1SFOSNb0");
    cutflow.addCutToLastActiveCut("WZCR1SFOSPt3l");
    cutflow.addCutToLastActiveCut("WZCR1SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("WZCR1SFOSMET");
    cutflow.addCutToLastActiveCut("WZCR1SFOSMll");
    cutflow.addCutToLastActiveCut("WZCR1SFOSM3l");
    cutflow.addCutToLastActiveCut("WZCR1SFOSZVt");
    cutflow.addCutToLastActiveCut("WZCR1SFOSMT3rd");
    cutflow.addCutToLastActiveCut("WZCR1SFOSFull");
    cutflow.getCut("CutWZCRTrilep");
    cutflow.addCutToLastActiveCut("WZCR2SFOS");
    cutflow.addCutToLastActiveCut("WZCR2SFOSNj1");
    cutflow.addCutToLastActiveCut("WZCR2SFOSNb0");
    cutflow.addCutToLastActiveCut("WZCR2SFOSPt3l");
    cutflow.addCutToLastActiveCut("WZCR2SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("WZCR2SFOSMET");
    cutflow.addCutToLastActiveCut("WZCR2SFOSMll");
    cutflow.addCutToLastActiveCut("WZCR2SFOSM3l");
    cutflow.addCutToLastActiveCut("WZCR2SFOSZVt");
    cutflow.addCutToLastActiveCut("WZCR2SFOSFull");

    // Same-sign Mjj on-W region
    cutflow.getCut("CutARDilep");
    cutflow.addCutToLastActiveCut("ARSSmm");
    cutflow.addCutToLastActiveCut("ARSSmmTVeto");
    cutflow.addCutToLastActiveCut("ARSSmmNj2");
    cutflow.addCutToLastActiveCut("ARSSmmNb0");
    cutflow.addCutToLastActiveCut("ARSSmmMjjW");
    cutflow.addCutToLastActiveCut("ARSSmmMjjL");
    cutflow.addCutToLastActiveCut("ARSSmmDetajjL");
    cutflow.addCutToLastActiveCut("ARSSmmMET");
    cutflow.addCutToLastActiveCut("ARSSmmMllSS");
    cutflow.addCutToLastActiveCut("ARSSmmFull");
    cutflow.getCut("CutARDilep");
    cutflow.addCutToLastActiveCut("ARSSem");
    cutflow.addCutToLastActiveCut("ARSSemTVeto");
    cutflow.addCutToLastActiveCut("ARSSemNj2");
    cutflow.addCutToLastActiveCut("ARSSemNb0");
    cutflow.addCutToLastActiveCut("ARSSemMjjW");
    cutflow.addCutToLastActiveCut("ARSSemMjjL");
    cutflow.addCutToLastActiveCut("ARSSemDetajjL");
    cutflow.addCutToLastActiveCut("ARSSemMET");
    cutflow.addCutToLastActiveCut("ARSSemMllSS");
    cutflow.addCutToLastActiveCut("ARSSemMTmax");
    cutflow.addCutToLastActiveCut("ARSSemFull");
    cutflow.getCut("CutARDilep");
    cutflow.addCutToLastActiveCut("ARSSee");
    cutflow.addCutToLastActiveCut("ARSSeeZeeVt");
    cutflow.addCutToLastActiveCut("ARSSeeTVeto");
    cutflow.addCutToLastActiveCut("ARSSeeNj2");
    cutflow.addCutToLastActiveCut("ARSSeeNb0");
    cutflow.addCutToLastActiveCut("ARSSeeMjjW");
    cutflow.addCutToLastActiveCut("ARSSeeMjjL");
    cutflow.addCutToLastActiveCut("ARSSeeDetajjL");
    cutflow.addCutToLastActiveCut("ARSSeeMET");
    cutflow.addCutToLastActiveCut("ARSSeeMllSS");
    cutflow.addCutToLastActiveCut("ARSSeeFull");

    // Same-sign Mjj off-W region
    cutflow.getCut("CutARDilep");
    cutflow.addCutToLastActiveCut("ARSSSidemm");
    cutflow.addCutToLastActiveCut("ARSSSidemmTVeto");
    cutflow.addCutToLastActiveCut("ARSSSidemmNj2");
    cutflow.addCutToLastActiveCut("ARSSSidemmNb0");
    cutflow.addCutToLastActiveCut("ARSSSidemmMjjW");
    cutflow.addCutToLastActiveCut("ARSSSidemmMjjL");
    cutflow.addCutToLastActiveCut("ARSSSidemmDetajjL");
    cutflow.addCutToLastActiveCut("ARSSSidemmMET");
    cutflow.addCutToLastActiveCut("ARSSSidemmMllSS");
    cutflow.addCutToLastActiveCut("ARSSSidemmFull");
    cutflow.getCut("CutARDilep");
    cutflow.addCutToLastActiveCut("ARSSSideem");
    cutflow.addCutToLastActiveCut("ARSSSideemTVeto");
    cutflow.addCutToLastActiveCut("ARSSSideemNj2");
    cutflow.addCutToLastActiveCut("ARSSSideemNb0");
    cutflow.addCutToLastActiveCut("ARSSSideemMjjW");
    cutflow.addCutToLastActiveCut("ARSSSideemMjjL");
    cutflow.addCutToLastActiveCut("ARSSSideemDetajjL");
    cutflow.addCutToLastActiveCut("ARSSSideemMET");
    cutflow.addCutToLastActiveCut("ARSSSideemMllSS");
    cutflow.addCutToLastActiveCut("ARSSSideemMTmax");
    cutflow.addCutToLastActiveCut("ARSSSideemFull");
    cutflow.getCut("CutARDilep");
    cutflow.addCutToLastActiveCut("ARSSSideee");
    cutflow.addCutToLastActiveCut("ARSSSideeeZeeVt");
    cutflow.addCutToLastActiveCut("ARSSSideeeTVeto");
    cutflow.addCutToLastActiveCut("ARSSSideeeNj2");
    cutflow.addCutToLastActiveCut("ARSSSideeeNb0");
    cutflow.addCutToLastActiveCut("ARSSSideeeMjjW");
    cutflow.addCutToLastActiveCut("ARSSSideeeMjjL");
    cutflow.addCutToLastActiveCut("ARSSSideeeDetajjL");
    cutflow.addCutToLastActiveCut("ARSSSideeeMET");
    cutflow.addCutToLastActiveCut("ARSSSideeeMllSS");
    cutflow.addCutToLastActiveCut("ARSSSideeeFull");

    // Trilep regions
    cutflow.getCut("CutARTrilep");
    cutflow.addCutToLastActiveCut("AR0SFOS");
    cutflow.addCutToLastActiveCut("AR0SFOSNj1");
    cutflow.addCutToLastActiveCut("AR0SFOSNb0");
    cutflow.addCutToLastActiveCut("AR0SFOSPt3l");
    cutflow.addCutToLastActiveCut("AR0SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("AR0SFOSMET");
    cutflow.addCutToLastActiveCut("AR0SFOSMll");
    cutflow.addCutToLastActiveCut("AR0SFOSM3l");
    cutflow.addCutToLastActiveCut("AR0SFOSZVt");
    cutflow.addCutToLastActiveCut("AR0SFOSMTmax");
    cutflow.addCutToLastActiveCut("AR0SFOSFull");
    cutflow.getCut("CutARTrilep");
    cutflow.addCutToLastActiveCut("AR1SFOS");
    cutflow.addCutToLastActiveCut("AR1SFOSNj1");
    cutflow.addCutToLastActiveCut("AR1SFOSNb0");
    cutflow.addCutToLastActiveCut("AR1SFOSPt3l");
    cutflow.addCutToLastActiveCut("AR1SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("AR1SFOSMET");
    cutflow.addCutToLastActiveCut("AR1SFOSMll");
    cutflow.addCutToLastActiveCut("AR1SFOSM3l");
    cutflow.addCutToLastActiveCut("AR1SFOSZVt");
    cutflow.addCutToLastActiveCut("AR1SFOSMT3rd");
    cutflow.addCutToLastActiveCut("AR1SFOSFull");
    cutflow.getCut("CutARTrilep");
    cutflow.addCutToLastActiveCut("AR2SFOS");
    cutflow.addCutToLastActiveCut("AR2SFOSNj1");
    cutflow.addCutToLastActiveCut("AR2SFOSNb0");
    cutflow.addCutToLastActiveCut("AR2SFOSPt3l");
    cutflow.addCutToLastActiveCut("AR2SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("AR2SFOSMET");
    cutflow.addCutToLastActiveCut("AR2SFOSMll");
    cutflow.addCutToLastActiveCut("AR2SFOSM3l");
    cutflow.addCutToLastActiveCut("AR2SFOSZVt");
    cutflow.addCutToLastActiveCut("AR2SFOSFull");

    // Btagged CR for Same-sign Mjj on-W region
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("BTCRSSmm");
    cutflow.addCutToLastActiveCut("BTCRSSmmTVeto");
    cutflow.addCutToLastActiveCut("BTCRSSmmNj2");
    cutflow.addCutToLastActiveCut("BTCRSSmmNbgeq1");
    cutflow.addCutToLastActiveCut("BTCRSSmmMjjW");
    cutflow.addCutToLastActiveCut("BTCRSSmmMjjL");
    cutflow.addCutToLastActiveCut("BTCRSSmmDetajjL");
    cutflow.addCutToLastActiveCut("BTCRSSmmMET");
    cutflow.addCutToLastActiveCut("BTCRSSmmMllSS");
    cutflow.addCutToLastActiveCut("BTCRSSmmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("BTCRSSem");
    cutflow.addCutToLastActiveCut("BTCRSSemTVeto");
    cutflow.addCutToLastActiveCut("BTCRSSemNj2");
    cutflow.addCutToLastActiveCut("BTCRSSemNbgeq1");
    cutflow.addCutToLastActiveCut("BTCRSSemMjjW");
    cutflow.addCutToLastActiveCut("BTCRSSemMjjL");
    cutflow.addCutToLastActiveCut("BTCRSSemDetajjL");
    cutflow.addCutToLastActiveCut("BTCRSSemMET");
    cutflow.addCutToLastActiveCut("BTCRSSemMllSS");
    cutflow.addCutToLastActiveCut("BTCRSSemMTmax");
    cutflow.addCutToLastActiveCut("BTCRSSemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("BTCRSSee");
    cutflow.addCutToLastActiveCut("BTCRSSeeZeeVt");
    cutflow.addCutToLastActiveCut("BTCRSSeeTVeto");
    cutflow.addCutToLastActiveCut("BTCRSSeeNj2");
    cutflow.addCutToLastActiveCut("BTCRSSeeNbgeq1");
    cutflow.addCutToLastActiveCut("BTCRSSeeMjjW");
    cutflow.addCutToLastActiveCut("BTCRSSeeMjjL");
    cutflow.addCutToLastActiveCut("BTCRSSeeDetajjL");
    cutflow.addCutToLastActiveCut("BTCRSSeeMET");
    cutflow.addCutToLastActiveCut("BTCRSSeeMllSS");
    cutflow.addCutToLastActiveCut("BTCRSSeeFull");

    // Same-sign Mjj off-W region
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("BTCRSSSidemm");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmTVeto");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmNj2");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmNbgeq1");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmMjjW");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmMjjL");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmDetajjL");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmMET");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmMllSS");
    cutflow.addCutToLastActiveCut("BTCRSSSidemmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("BTCRSSSideem");
    cutflow.addCutToLastActiveCut("BTCRSSSideemTVeto");
    cutflow.addCutToLastActiveCut("BTCRSSSideemNj2");
    cutflow.addCutToLastActiveCut("BTCRSSSideemNbgeq1");
    cutflow.addCutToLastActiveCut("BTCRSSSideemMjjW");
    cutflow.addCutToLastActiveCut("BTCRSSSideemMjjL");
    cutflow.addCutToLastActiveCut("BTCRSSSideemDetajjL");
    cutflow.addCutToLastActiveCut("BTCRSSSideemMET");
    cutflow.addCutToLastActiveCut("BTCRSSSideemMllSS");
    cutflow.addCutToLastActiveCut("BTCRSSSideemMTmax");
    cutflow.addCutToLastActiveCut("BTCRSSSideemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("BTCRSSSideee");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeZeeVt");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeTVeto");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeNj2");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeNbgeq1");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeMjjW");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeMjjL");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeDetajjL");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeMET");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeMllSS");
    cutflow.addCutToLastActiveCut("BTCRSSSideeeFull");

    // Trilep regions
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("BTCR0SFOS");
    cutflow.addCutToLastActiveCut("BTCR0SFOSNj1");
    cutflow.addCutToLastActiveCut("BTCR0SFOSNbgeq1");
    cutflow.addCutToLastActiveCut("BTCR0SFOSPt3l");
    cutflow.addCutToLastActiveCut("BTCR0SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("BTCR0SFOSMET");
    cutflow.addCutToLastActiveCut("BTCR0SFOSMll");
    cutflow.addCutToLastActiveCut("BTCR0SFOSM3l");
    cutflow.addCutToLastActiveCut("BTCR0SFOSZVt");
    cutflow.addCutToLastActiveCut("BTCR0SFOSMTmax");
    cutflow.addCutToLastActiveCut("BTCR0SFOSFull");
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("BTCR1SFOS");
    cutflow.addCutToLastActiveCut("BTCR1SFOSNj1");
    cutflow.addCutToLastActiveCut("BTCR1SFOSNbgeq1");
    cutflow.addCutToLastActiveCut("BTCR1SFOSPt3l");
    cutflow.addCutToLastActiveCut("BTCR1SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("BTCR1SFOSMET");
    cutflow.addCutToLastActiveCut("BTCR1SFOSMll");
    cutflow.addCutToLastActiveCut("BTCR1SFOSM3l");
    cutflow.addCutToLastActiveCut("BTCR1SFOSZVt");
    cutflow.addCutToLastActiveCut("BTCR1SFOSMT3rd");
    cutflow.addCutToLastActiveCut("BTCR1SFOSFull");
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("BTCR2SFOS");
    cutflow.addCutToLastActiveCut("BTCR2SFOSNj1");
    cutflow.addCutToLastActiveCut("BTCR2SFOSNbgeq1");
    cutflow.addCutToLastActiveCut("BTCR2SFOSPt3l");
    cutflow.addCutToLastActiveCut("BTCR2SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("BTCR2SFOSMET");
    cutflow.addCutToLastActiveCut("BTCR2SFOSMll");
    cutflow.addCutToLastActiveCut("BTCR2SFOSM3l");
    cutflow.addCutToLastActiveCut("BTCR2SFOSZVt");
    cutflow.addCutToLastActiveCut("BTCR2SFOSFull");

    // VBS control region
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("VBSCRSSmm");
    cutflow.addCutToLastActiveCut("VBSCRSSmmTVeto");
    cutflow.addCutToLastActiveCut("VBSCRSSmmNj2");
    cutflow.addCutToLastActiveCut("VBSCRSSmmNb0");
    cutflow.addCutToLastActiveCut("VBSCRSSmmVBF");
    cutflow.addCutToLastActiveCut("VBSCRSSmmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("VBSCRSSem");
    cutflow.addCutToLastActiveCut("VBSCRSSemTVeto");
    cutflow.addCutToLastActiveCut("VBSCRSSemNj2");
    cutflow.addCutToLastActiveCut("VBSCRSSemNb0");
    cutflow.addCutToLastActiveCut("VBSCRSSemVBF");
    cutflow.addCutToLastActiveCut("VBSCRSSemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("VBSCRSSee");
    cutflow.addCutToLastActiveCut("VBSCRSSeeZeeVt");
    cutflow.addCutToLastActiveCut("VBSCRSSeeTVeto");
    cutflow.addCutToLastActiveCut("VBSCRSSeeNj2");
    cutflow.addCutToLastActiveCut("VBSCRSSeeNb0");
    cutflow.addCutToLastActiveCut("VBSCRSSeeVBF");
    cutflow.addCutToLastActiveCut("VBSCRSSeeFull");

    // ttW control region mjj window
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("TTWCRSSmm");
    cutflow.addCutToLastActiveCut("TTWCRSSmmTVeto");
    cutflow.addCutToLastActiveCut("TTWCRSSmmNj4");
    cutflow.addCutToLastActiveCut("TTWCRSSmmNbgeq1");
    cutflow.addCutToLastActiveCut("TTWCRSSmmMjjW");
    cutflow.addCutToLastActiveCut("TTWCRSSmmMjjL");
    cutflow.addCutToLastActiveCut("TTWCRSSmmDetajjL");
    cutflow.addCutToLastActiveCut("TTWCRSSmmMET");
    cutflow.addCutToLastActiveCut("TTWCRSSmmMllSS");
    cutflow.addCutToLastActiveCut("TTWCRSSmmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("TTWCRSSem");
    cutflow.addCutToLastActiveCut("TTWCRSSemTVeto");
    cutflow.addCutToLastActiveCut("TTWCRSSemNj4");
    cutflow.addCutToLastActiveCut("TTWCRSSemNbgeq1");
    cutflow.addCutToLastActiveCut("TTWCRSSemMjjW");
    cutflow.addCutToLastActiveCut("TTWCRSSemMjjL");
    cutflow.addCutToLastActiveCut("TTWCRSSemDetajjL");
    cutflow.addCutToLastActiveCut("TTWCRSSemMET");
    cutflow.addCutToLastActiveCut("TTWCRSSemMllSS");
    cutflow.addCutToLastActiveCut("TTWCRSSemMTmax");
    cutflow.addCutToLastActiveCut("TTWCRSSemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("TTWCRSSee");
    cutflow.addCutToLastActiveCut("TTWCRSSeeZeeVt");
    cutflow.addCutToLastActiveCut("TTWCRSSeeTVeto");
    cutflow.addCutToLastActiveCut("TTWCRSSeeNj4");
    cutflow.addCutToLastActiveCut("TTWCRSSeeNbgeq1");
    cutflow.addCutToLastActiveCut("TTWCRSSeeMjjW");
    cutflow.addCutToLastActiveCut("TTWCRSSeeMjjL");
    cutflow.addCutToLastActiveCut("TTWCRSSeeDetajjL");
    cutflow.addCutToLastActiveCut("TTWCRSSeeMET");
    cutflow.addCutToLastActiveCut("TTWCRSSeeMllSS");
    cutflow.addCutToLastActiveCut("TTWCRSSeeFull");

    // ttW control region mjj sideband
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemm");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmTVeto");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmNj4");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmNbgeq1");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmMjjW");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmMjjL");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmDetajjL");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmMET");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmMllSS");
    cutflow.addCutToLastActiveCut("TTWCRSSSidemmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("TTWCRSSSideem");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemTVeto");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemNj4");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemNbgeq1");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemMjjW");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemMjjL");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemDetajjL");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemMET");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemMllSS");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemMTmax");
    cutflow.addCutToLastActiveCut("TTWCRSSSideemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("TTWCRSSSideee");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeZeeVt");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeTVeto");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeNj4");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeNbgeq1");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeMjjW");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeMjjL");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeDetajjL");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeMET");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeMllSS");
    cutflow.addCutToLastActiveCut("TTWCRSSSideeeFull");

    // TTZ control region
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("TTZCR0SFOS");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSNj2");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSNb1");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSPt3l");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSMET");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSMll");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSM3l");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSZVt");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSMTmax");
    cutflow.addCutToLastActiveCut("TTZCR0SFOSFull");
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("TTZCR1SFOS");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSNj2");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSNb1");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSPt3l");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSMET");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSMll");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSM3l");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSZVt");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSMT3rd");
    cutflow.addCutToLastActiveCut("TTZCR1SFOSFull");
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("TTZCR2SFOS");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSNj2");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSNb1");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSPt3l");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSMET");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSMll");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSM3l");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSZVt");
    cutflow.addCutToLastActiveCut("TTZCR2SFOSFull");

    // Low MET mjj side band 
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("LXECRSSmm");
    cutflow.addCutToLastActiveCut("LXECRSSmmTVeto");
    cutflow.addCutToLastActiveCut("LXECRSSmmNj2");
    cutflow.addCutToLastActiveCut("LXECRSSmmNb0");
    cutflow.addCutToLastActiveCut("LXECRSSmmMjjW");
    cutflow.addCutToLastActiveCut("LXECRSSmmMET");
    cutflow.addCutToLastActiveCut("LXECRSSmmMllSS");
    cutflow.addCutToLastActiveCut("LXECRSSmmFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("LXECRSSem");
    cutflow.addCutToLastActiveCut("LXECRSSemTVeto");
    cutflow.addCutToLastActiveCut("LXECRSSemNj2");
    cutflow.addCutToLastActiveCut("LXECRSSemNb0");
    cutflow.addCutToLastActiveCut("LXECRSSemMjjW");
    cutflow.addCutToLastActiveCut("LXECRSSemMET");
    cutflow.addCutToLastActiveCut("LXECRSSemMllSS");
    cutflow.addCutToLastActiveCut("LXECRSSemFull");
    cutflow.getCut("CutSRDilep");
    cutflow.addCutToLastActiveCut("LXECRSSee");
    cutflow.addCutToLastActiveCut("LXECRSSeeZeeVt");
    cutflow.addCutToLastActiveCut("LXECRSSeeTVeto");
    cutflow.addCutToLastActiveCut("LXECRSSeeNj2");
    cutflow.addCutToLastActiveCut("LXECRSSeeNb0");
    cutflow.addCutToLastActiveCut("LXECRSSeeMjjW");
    cutflow.addCutToLastActiveCut("LXECRSSeeMET");
    cutflow.addCutToLastActiveCut("LXECRSSeeMllSS");
    cutflow.addCutToLastActiveCut("LXECRSSeeFull");

    // Gamma control region
    cutflow.getCut("CutSRTrilep");
    cutflow.addCutToLastActiveCut("GCR0SFOS");
    cutflow.addCutToLastActiveCut("GCR0SFOSNj1");
    cutflow.addCutToLastActiveCut("GCR0SFOSNb0");
    cutflow.addCutToLastActiveCut("GCR0SFOSPt3l");
    cutflow.addCutToLastActiveCut("GCR0SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("GCR0SFOSMET");
    cutflow.addCutToLastActiveCut("GCR0SFOSMll");
    cutflow.addCutToLastActiveCut("GCR0SFOSM3l");
    cutflow.addCutToLastActiveCut("GCR0SFOSZVt");
    cutflow.addCutToLastActiveCut("GCR0SFOSMTmax");
    cutflow.addCutToLastActiveCut("GCR0SFOSFull");

    // Systematics
    if (doSystematics)
    {
        // Declare cut varying systematics to cuts with the patterns provided in the vector
        cutflow.addCutSyst("JESUp"  , {"jj", "MET", "Nj", "Nb", "VBF"});
        cutflow.addCutSyst("JESDown", {"jj", "MET", "Nj", "Nb", "VBF"});

        cutflow.addWgtSyst("LepSFUp");
        cutflow.addWgtSyst("LepSFDown");
        cutflow.addWgtSyst("TrigSFUp");
        cutflow.addWgtSyst("TrigSFDown");
        cutflow.addWgtSyst("BTagLFUp");
        cutflow.addWgtSyst("BTagLFDown");
        cutflow.addWgtSyst("BTagHFUp");
        cutflow.addWgtSyst("BTagHFDown");
        cutflow.addWgtSyst("PileupUp");
        cutflow.addWgtSyst("PileupDown");
        if (doFakeEstimation)
        {
            cutflow.addWgtSyst("FakeUp");
            cutflow.addWgtSyst("FakeDown");
            cutflow.addWgtSyst("FakeRateUp");
            cutflow.addWgtSyst("FakeRateDown");
            cutflow.addWgtSyst("FakeRateElUp");
            cutflow.addWgtSyst("FakeRateElDown");
            cutflow.addWgtSyst("FakeRateMuUp");
            cutflow.addWgtSyst("FakeRateMuDown");
            cutflow.addWgtSyst("FakeClosureUp");
            cutflow.addWgtSyst("FakeClosureDown");
            cutflow.addWgtSyst("FakeClosureElUp");
            cutflow.addWgtSyst("FakeClosureElDown");
            cutflow.addWgtSyst("FakeClosureMuUp");
            cutflow.addWgtSyst("FakeClosureMuDown");
            cutflow.addWgtSyst("FakeClosureMuDown");
        }
    }

    // Remove to filter
//    cutflow.removeCut("CutSRDilep");
//    cutflow.removeCut("CutWZCRDilep");
//    cutflow.removeCut("CutWZCRTrilep");
//    cutflow.removeCut("SR0SFOS");
//    cutflow.removeCut("SR2SFOS");

    // Histogram utility object that is used to define the histograms
    RooUtil::Histograms histograms;
    histograms.addHistogram("MllSS"                ,  180 , 0.      , 300.   );
    histograms.addHistogram("MllSS_wide"           ,  180 , 0.      , 2000.  );
    histograms.addHistogram("MllZ"                 ,  180 , 60.     , 120.   );
    histograms.addHistogram("MllZZoom"             ,  180 , 80.     , 100.   );
    histograms.addHistogram("Mll3L"                ,  180 , 0.      , 300.   );
    histograms.addHistogram("Mll3L1"               ,  180 , 0.      , 300.   );
    histograms.addHistogram("nSFOSinZ"             ,  3   , 0.      , 3.     );
    histograms.addHistogram("M3l"                  ,  180 , 0.      , 150.   );
    histograms.addHistogram("Pt3lGCR"              ,  180 , 0.      , 100.   );
    histograms.addHistogram("Pt3l"                 ,  180 , 0.      , 300.   );
    histograms.addHistogram("Ptll"                 ,  180 , 0.      , 300.   );
    histograms.addHistogram("nvtx"                 ,  60  , 0.      , 60.    );
    histograms.addHistogram("Mjj"                  ,  180 , 0.      , 300.   );
    histograms.addHistogram("MjjL"                 ,  180 , 0.      , 750.   );
    histograms.addHistogram("DetajjL"              ,  180 , 0.      , 5.     );
    histograms.addHistogram("MjjVBF"               ,  180 , 0.      , 750.   );
    histograms.addHistogram("DetajjVBF"            ,  180 , 0.      , 8.     );
    histograms.addHistogram("MET"                  ,  180 , 0.      , 180.   );
    histograms.addHistogram("lep_pt0"              ,  180 , 0.      , 250    );
    histograms.addHistogram("lep_pt1"              ,  180 , 0.      , 150    );
    histograms.addHistogram("lep_pt2"              ,  180 , 0.      , 150    );
    histograms.addHistogram("lep_eta0"             ,  180 , -2.5    , 2.5    );
    histograms.addHistogram("lep_eta1"             ,  180 , -2.5    , 2.5    );
    histograms.addHistogram("lep_phi0"             ,  180 , -3.1416 , 3.1416 );
    histograms.addHistogram("lep_phi1"             ,  180 , -3.1416 , 3.1416 );
    histograms.addHistogram("lep_relIso03EAv2Lep0" ,  180 , 0.0     , 0.2    );
    histograms.addHistogram("lep_relIso03EAv2Lep1" ,  180 , 0.0     , 0.2    );
    histograms.addHistogram("lep_relIso03EAv2Lep2" ,  180 , 0.0     , 0.2    );
    histograms.addHistogram("nj"                   ,  7   , 0.      , 7.     );
    histograms.addHistogram("nj30"                 ,  7   , 0.      , 7.     );
    histograms.addHistogram("nb"                   ,  5   , 0.      , 5.     );
    histograms.addHistogram("MTmin"                ,  180 , 0.      , 300.   );
    histograms.addHistogram("MTmax"                ,  180 , 0.      , 300.   );
    histograms.addHistogram("MTmax3L"              ,  180 , 0.      , 300.   );
    histograms.addHistogram("MT3rd"                ,  180 , 0.      , 300.   );
    histograms.addHistogram("yield"                ,  9   , 0.      , 9.     );
    histograms.addHistogram("wzcryield"            ,  6   , 0.      , 6.     );

    // Fake rate estimation histogram
    const std::vector<float> eta_bounds = {0.0, 1.6, 2.4};
    const std::vector<float> ptcorrcoarse_bounds = {0., 20., 25., 30., 35., 150.};
    histograms.addHistogram("ptcorretarolledcoarse" , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );

    // Now book cutflows
    cutflow.bookCutflows();

    // Cutflow object that takes the histograms and books them to a cutflow for histogramming
    TString suffix = "";
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSmm"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSem"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSee"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSidemm"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSideem"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSideee"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR0SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR1SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR2SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCRSSmm"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCRSSem"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCRSSee"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCR1SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCR2SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "ARSSmm"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "ARSSem"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "ARSSee"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "ARSSSidemm"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "ARSSSideem"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "ARSSSideee"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "AR0SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "AR1SFOS"+suffix);
    cutflow.bookHistogramsForCutAndBelow(histograms, "AR2SFOS"+suffix);

    // Event list
//    cutflow.bookEventLists();

    // Print the cut structure for review
    cutflow.printCuts();

    // Scale factors
    LeptonScaleFactors leptonScaleFactors;
    FakeRates fakerates;

    //
    //
    // Looping events
    //
    //
    while (looper.nextEvent())
    {

        // Luminosity setting
        float lumi = is2017 == 1 ? 41.3 : 35.9;
        float ffwgt = is2017 == 1 ? fakerates.getFakeFactor() : www.ffwgt();
        lumi = doFakeEstimation ? ffwgt : lumi;

        // Compute preselection
        bool presel = (www.firstgoodvertex() == 0);
        presel &= (www.Flag_AllEventFilters() > 0);
        presel &= (www.vetophoton() == 0);
        presel &= (www.evt_passgoodrunlist() > 0);

        // Compute trigger variable
        //bool trigger = is2017 == 1 ? www.passTrigger() * www.pass_duplicate_ee_em_mm() : passTrigger2016();
        bool trigger = www.passTrigger() * www.pass_duplicate_ee_em_mm();

        // Event weight
        float weight = www.evt_scale1fb() * www.purewgt() * lumi;
        if (isWWW && !is2017) weight *= 1.0384615385;
        if (isData && !doFakeEstimation) weight = 1;

        // Lepton counter to define dilep or trilep region
        bool isdilep      = (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 2);
        bool istrilep     = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 3) * (www.lep_pt()[0]>25.);
        bool isfakedilep  = (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 1) * (www.lep_pt()[0]>25.) * (www.lep_pt()[1]>25.);
        bool isfaketrilep = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 2);
        bool iswzcrtrilep     = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 3);
        bool isfakewzcrtrilep = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 2);

        // Compute the scale factors
        float ee_sf, em_sf, mm_sf, threelep_sf;
        std::tie(ee_sf, em_sf, mm_sf, threelep_sf) = leptonScaleFactors.getScaleFactors(is2017, doFakeEstimation);
        float btagsf = !isData ? www.weight_btagsf() : 1;
        float trigsf = !isData ? www.trigsf() : 1;

        //      setCut("CutName"       , <boolean value to say whether it passes>           , <float value to define weight>);
        cutflow.setCut("CutWeight"         , 1                                                            , weight                             );
        cutflow.setCut("CutPresel"         , presel                                                       , 1                                  );
        cutflow.setCut("CutTrigger"        , trigger                                                      , trigsf                             );
        cutflow.setCut("CutSRDilep"        , doFakeEstimation ? isfakedilep : isdilep                     , 1                                  );
        cutflow.setCut("CutSRTrilep"       , doFakeEstimation ? isfaketrilep : istrilep                   , 1                                  );
        cutflow.setCut("CutWZCRDilep"      , doFakeEstimation ? isfakewzcrtrilep : iswzcrtrilep           , 1                                  );
        cutflow.setCut("CutWZCRTrilep"     , doFakeEstimation ? isfaketrilep : istrilep                   , 1                                  );
        cutflow.setCut("CutARDilep"        , isfakedilep                                                  , 1                                  );
        cutflow.setCut("CutARTrilep"       , isfaketrilep                                                 , 1                                  );

        cutflow.setCut("SRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("SRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSmmNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SRSSmmMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("SRSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("SRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("SRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("SRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSemNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SRSSemMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("SRSSemMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSemDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSemMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSemMllSS"       , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("SRSSemMTmax"       , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("SRSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("SRSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("SRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("SRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSeeNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSeeNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SRSSeeMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("SRSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("SRSSSidemm"        , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("SRSSSidemmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSSidemmNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSSidemmNb0"     , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SRSSSidemmMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("SRSSSidemmMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSSidemmDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSSidemmMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSSidemmMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSSidemmFull"    , 1                                                            , 1                                  );

        cutflow.setCut("SRSSSideem"        , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("SRSSSideemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSSideemNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSSideemNb0"     , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SRSSSideemMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("SRSSSideemMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSSideemDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSSideemMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSSideemMllSS"   , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("SRSSSideemMTmax"   , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("SRSSSideemFull"    , 1                                                            , 1                                  );

        cutflow.setCut("SRSSSideee"        , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("SRSSSideeeZeeVt"   , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("SRSSSideeeTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSSideeeNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSSideeeNb0"     , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SRSSSideeeMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("SRSSSideeeMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSSideeeDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSSideeeMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSSideeeMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSSideeeFull"    , 1                                                            , 1                                  );

        cutflow.setCut("SR0SFOS"           , (www.nSFOS()==0)                                             , threelep_sf                        );
        cutflow.setCut("SR0SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("SR0SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SR0SFOSPt3l"       , 1.                                                           , 1                                  );
        cutflow.setCut("SR0SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("SR0SFOSMET"        , www.met_pt()>30.                                             , 1                                  );
        cutflow.setCut("SR0SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("SR0SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("SR0SFOSZVt"        , abs(www.Mee3L()-91.1876) > 15.                               , 1                                  );
        cutflow.setCut("SR0SFOSMTmax"      , www.MTmax3L()>90.                                            , 1                                  );
        cutflow.setCut("SR0SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("SR1SFOS"           , (www.nSFOS()==1)                                             , threelep_sf                        );
        cutflow.setCut("SR1SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("SR1SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SR1SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("SR1SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("SR1SFOSMET"        , www.met_pt()>40.                                             , 1                                  );
        cutflow.setCut("SR1SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("SR1SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("SR1SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("SR1SFOSMT3rd"      , www.MT3rd()>90.                                              , 1                                  );
        cutflow.setCut("SR1SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("SR2SFOS"           , (www.nSFOS()==2)                                             , threelep_sf                        );
        cutflow.setCut("SR2SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("SR2SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("SR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("SR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("SR2SFOSMET"        , www.met_pt()>55.                                             , 1                                  );
        cutflow.setCut("SR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                                  );
        cutflow.setCut("SR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("SR2SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("SR2SFOSFull"       , 1                                                            , 1                                  );

        // Condition for SS WZ CR to check whether it had a Z
        bool hasz_ss = (abs(www.Mll3L()-91.1876)<10.||abs(www.Mll3L1()-91.1876)<10.);

        cutflow.setCut("WZCRSSmm"            , (hasz_ss)*(www.passSSmm())*(www.MllSS()>40.)                 , threelep_sf                        );
        cutflow.setCut("WZCRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("WZCRSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("WZCRSSmmNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("WZCRSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("WZCRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("WZCRSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("WZCRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("WZCRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("WZCRSSem"            , (hasz_ss)*(www.passSSem())*(www.MllSS()>30.)                 , threelep_sf                        );
        cutflow.setCut("WZCRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("WZCRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("WZCRSSemNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("WZCRSSemMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("WZCRSSemDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("WZCRSSemMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("WZCRSSemMllSS"       , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("WZCRSSemMTmax"       , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("WZCRSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("WZCRSSee"            , (hasz_ss)*(www.passSSee())*(1)*(www.MllSS()>40.)             , threelep_sf                        );
        cutflow.setCut("WZCRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("WZCRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("WZCRSSeeNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("WZCRSSeeNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("WZCRSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("WZCRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("WZCRSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("WZCRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("WZCRSSeeFull"        , 1                                                            , 1                                  );

        // Condition for SS WZ CR to check whether it had a Z
        bool hasz_3l = (abs(www.Mll3L()-91.1876)<20.||abs(www.Mll3L1()-91.1876)<20.);

        cutflow.setCut("WZCR1SFOS"           , (www.nSFOS()==1)                                             , threelep_sf                        );
        cutflow.setCut("WZCR1SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("WZCR1SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("WZCR1SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("WZCR1SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("WZCR1SFOSMET"        , www.met_pt()>40.                                             , 1                                  );
        cutflow.setCut("WZCR1SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("WZCR1SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("WZCR1SFOSZVt"        , hasz_3l                                                      , 1                                  );
        cutflow.setCut("WZCR1SFOSMT3rd"      , www.MT3rd()>90.                                              , 1                                  );
        cutflow.setCut("WZCR1SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("WZCR2SFOS"           , (www.nSFOS()==2)                                             , threelep_sf                        );
        cutflow.setCut("WZCR2SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("WZCR2SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("WZCR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("WZCR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("WZCR2SFOSMET"        , www.met_pt()>55.                                             , 1                                  );
        cutflow.setCut("WZCR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                                  );
        cutflow.setCut("WZCR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("WZCR2SFOSZVt"        , hasz_3l                                                      , 1                                  );
        cutflow.setCut("WZCR2SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("ARSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , 1                                  );
        cutflow.setCut("ARSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("ARSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("ARSSmmNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("ARSSmmMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("ARSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("ARSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("ARSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("ARSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("ARSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("ARSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , 1                                  );
        cutflow.setCut("ARSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("ARSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("ARSSemNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("ARSSemMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("ARSSemMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("ARSSemDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("ARSSemMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("ARSSemMllSS"       , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("ARSSemMTmax"       , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("ARSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("ARSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , 1                                  );
        cutflow.setCut("ARSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("ARSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("ARSSeeNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("ARSSeeNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("ARSSeeMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("ARSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("ARSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("ARSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("ARSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("ARSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("ARSSSidemm"        , (www.passSSmm())*(www.MllSS()>40.)                           , 1                                  );
        cutflow.setCut("ARSSSidemmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("ARSSSidemmNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("ARSSSidemmNb0"     , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("ARSSSidemmMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("ARSSSidemmMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("ARSSSidemmDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("ARSSSidemmMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("ARSSSidemmMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("ARSSSidemmFull"    , 1                                                            , 1                                  );

        cutflow.setCut("ARSSSideem"        , (www.passSSem())*(www.MllSS()>30.)                           , 1                                  );
        cutflow.setCut("ARSSSideemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("ARSSSideemNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("ARSSSideemNb0"     , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("ARSSSideemMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("ARSSSideemMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("ARSSSideemDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("ARSSSideemMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("ARSSSideemMllSS"   , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("ARSSSideemMTmax"   , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("ARSSSideemFull"    , 1                                                            , 1                                  );

        cutflow.setCut("ARSSSideee"        , (www.passSSee())*(1)*(www.MllSS()>40.)                       , 1                                  );
        cutflow.setCut("ARSSSideeeZeeVt"   , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("ARSSSideeeTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("ARSSSideeeNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("ARSSSideeeNb0"     , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("ARSSSideeeMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("ARSSSideeeMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("ARSSSideeeDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("ARSSSideeeMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("ARSSSideeeMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("ARSSSideeeFull"    , 1                                                            , 1                                  );

        cutflow.setCut("AR0SFOS"           , (www.nSFOS()==0)                                             , threelep_sf                        );
        cutflow.setCut("AR0SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("AR0SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("AR0SFOSPt3l"       , 1.                                                           , 1                                  );
        cutflow.setCut("AR0SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("AR0SFOSMET"        , www.met_pt()>30.                                             , 1                                  );
        cutflow.setCut("AR0SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("AR0SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("AR0SFOSZVt"        , abs(www.Mee3L()-91.1876) > 15.                               , 1                                  );
        cutflow.setCut("AR0SFOSMTmax"      , www.MTmax3L()>90.                                            , 1                                  );
        cutflow.setCut("AR0SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("AR1SFOS"           , (www.nSFOS()==1)                                             , threelep_sf                        );
        cutflow.setCut("AR1SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("AR1SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("AR1SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("AR1SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("AR1SFOSMET"        , www.met_pt()>40.                                             , 1                                  );
        cutflow.setCut("AR1SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("AR1SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("AR1SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("AR1SFOSMT3rd"      , www.MT3rd()>90.                                              , 1                                  );
        cutflow.setCut("AR1SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("AR2SFOS"           , (www.nSFOS()==2)                                             , threelep_sf                        );
        cutflow.setCut("AR2SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("AR2SFOSNb0"        , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("AR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("AR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("AR2SFOSMET"        , www.met_pt()>55.                                             , 1                                  );
        cutflow.setCut("AR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                                  );
        cutflow.setCut("AR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("AR2SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("AR2SFOSFull"       , 1                                                            , 1                                  );

        // B tagged control regions
        cutflow.setCut("BTCRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("BTCRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("BTCRSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("BTCRSSmmNbgeq1"      , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCRSSmmMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("BTCRSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("BTCRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("BTCRSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("BTCRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("BTCRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("BTCRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("BTCRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("BTCRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("BTCRSSemNbgeq1"      , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCRSSemMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("BTCRSSemMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("BTCRSSemDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("BTCRSSemMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("BTCRSSemMllSS"       , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("BTCRSSemMTmax"       , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("BTCRSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("BTCRSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("BTCRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("BTCRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("BTCRSSeeNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("BTCRSSeeNbgeq1"      , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCRSSeeMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("BTCRSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("BTCRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("BTCRSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("BTCRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("BTCRSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("BTCRSSSidemm"        , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("BTCRSSSidemmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("BTCRSSSidemmNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("BTCRSSSidemmNbgeq1"  , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCRSSSidemmMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("BTCRSSSidemmMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("BTCRSSSidemmDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("BTCRSSSidemmMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("BTCRSSSidemmMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("BTCRSSSidemmFull"    , 1                                                            , 1                                  );

        cutflow.setCut("BTCRSSSideem"        , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("BTCRSSSideemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("BTCRSSSideemNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("BTCRSSSideemNbgeq1"  , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCRSSSideemMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("BTCRSSSideemMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("BTCRSSSideemDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("BTCRSSSideemMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("BTCRSSSideemMllSS"   , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("BTCRSSSideemMTmax"   , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("BTCRSSSideemFull"    , 1                                                            , 1                                  );

        cutflow.setCut("BTCRSSSideee"        , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("BTCRSSSideeeZeeVt"   , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("BTCRSSSideeeTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("BTCRSSSideeeNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("BTCRSSSideeeNbgeq1"  , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCRSSSideeeMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("BTCRSSSideeeMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("BTCRSSSideeeDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("BTCRSSSideeeMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("BTCRSSSideeeMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("BTCRSSSideeeFull"    , 1                                                            , 1                                  );

        cutflow.setCut("BTCR0SFOS"           , (www.nSFOS()==0)                                             , threelep_sf                        );
        cutflow.setCut("BTCR0SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("BTCR0SFOSNbgeq1"     , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCR0SFOSPt3l"       , 1.                                                           , 1                                  );
        cutflow.setCut("BTCR0SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("BTCR0SFOSMET"        , www.met_pt()>30.                                             , 1                                  );
        cutflow.setCut("BTCR0SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("BTCR0SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("BTCR0SFOSZVt"        , abs(www.Mee3L()-91.1876) > 15.                               , 1                                  );
        cutflow.setCut("BTCR0SFOSMTmax"      , www.MTmax3L()>90.                                            , 1                                  );
        cutflow.setCut("BTCR0SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("BTCR1SFOS"           , (www.nSFOS()==1)                                             , threelep_sf                        );
        cutflow.setCut("BTCR1SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("BTCR1SFOSNbgeq1"     , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCR1SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("BTCR1SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("BTCR1SFOSMET"        , www.met_pt()>40.                                             , 1                                  );
        cutflow.setCut("BTCR1SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("BTCR1SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("BTCR1SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("BTCR1SFOSMT3rd"      , www.MT3rd()>90.                                              , 1                                  );
        cutflow.setCut("BTCR1SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("BTCR2SFOS"           , (www.nSFOS()==2)                                             , threelep_sf                        );
        cutflow.setCut("BTCR2SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("BTCR2SFOSNbgeq1"     , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("BTCR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("BTCR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("BTCR2SFOSMET"        , www.met_pt()>55.                                             , 1                                  );
        cutflow.setCut("BTCR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                                  );
        cutflow.setCut("BTCR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("BTCR2SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("BTCR2SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("VBSCRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("VBSCRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("VBSCRSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("VBSCRSSmmNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("VBSCRSSmmVBF"         , www.MjjL() > 400 or www.DetajjL() > 1.5                      , 1                                  );
        cutflow.setCut("VBSCRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("VBSCRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("VBSCRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("VBSCRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("VBSCRSSemNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("VBSCRSSemVBF"         , www.MjjL() > 400 or www.DetajjL() > 1.5                      , 1                                  );
        cutflow.setCut("VBSCRSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("VBSCRSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("VBSCRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("VBSCRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("VBSCRSSeeNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("VBSCRSSeeNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("VBSCRSSeeVBF"         , www.MjjL() > 400 or www.DetajjL() > 1.5                      , 1                                  );
        cutflow.setCut("VBSCRSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("TTWCRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("TTWCRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("TTWCRSSmmNj4"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("TTWCRSSmmNbgeq1"      , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("TTWCRSSmmMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("TTWCRSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("TTWCRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("TTWCRSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("TTWCRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("TTWCRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("TTWCRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("TTWCRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("TTWCRSSemNj4"         , www.nj30()>= 4                                               , 1                                  );
        cutflow.setCut("TTWCRSSemNbgeq1"      , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("TTWCRSSemMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("TTWCRSSemMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("TTWCRSSemDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("TTWCRSSemMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("TTWCRSSemMllSS"       , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("TTWCRSSemMTmax"       , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("TTWCRSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("TTWCRSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("TTWCRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("TTWCRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("TTWCRSSeeNj4"         , www.nj30()>= 4                                               , 1                                  );
        cutflow.setCut("TTWCRSSeeNbgeq1"      , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("TTWCRSSeeMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("TTWCRSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("TTWCRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("TTWCRSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("TTWCRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("TTWCRSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("TTWCRSSSidemm"        , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("TTWCRSSSidemmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("TTWCRSSSidemmNj4"     , www.nj30()>= 4                                               , 1                                  );
        cutflow.setCut("TTWCRSSSidemmNbgeq1"  , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("TTWCRSSSidemmMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("TTWCRSSSidemmMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSidemmDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("TTWCRSSSidemmMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("TTWCRSSSidemmMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSidemmFull"    , 1                                                            , 1                                  );

        cutflow.setCut("TTWCRSSSideem"        , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("TTWCRSSSideemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("TTWCRSSSideemNj4"     , www.nj30()>= 4                                               , 1                                  );
        cutflow.setCut("TTWCRSSSideemNbgeq1"  , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("TTWCRSSSideemMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("TTWCRSSSideemMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSideemDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("TTWCRSSSideemMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("TTWCRSSSideemMllSS"   , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSideemMTmax"   , www.MTmax()>90.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSideemFull"    , 1                                                            , 1                                  );

        cutflow.setCut("TTWCRSSSideee"        , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("TTWCRSSSideeeZeeVt"   , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("TTWCRSSSideeeTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("TTWCRSSSideeeNj4"     , www.nj30()>= 4                                               , 1                                  );
        cutflow.setCut("TTWCRSSSideeeNbgeq1"  , www.nb()>=1                                                  , btagsf                             );
        cutflow.setCut("TTWCRSSSideeeMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("TTWCRSSSideeeMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSideeeDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("TTWCRSSSideeeMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("TTWCRSSSideeeMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("TTWCRSSSideeeFull"    , 1                                                            , 1                                  );

        cutflow.setCut("TTZCR0SFOS"           , (www.nSFOS()==0)                                             , threelep_sf                        );
        cutflow.setCut("TTZCR0SFOSNj2"        , www.nj()<=2                                                  , 1                                  );
        cutflow.setCut("TTZCR0SFOSNb1"        , www.nb()==1                                                  , btagsf                             );
        cutflow.setCut("TTZCR0SFOSPt3l"       , 1.                                                           , 1                                  );
        cutflow.setCut("TTZCR0SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("TTZCR0SFOSMET"        , www.met_pt()>30.                                             , 1                                  );
        cutflow.setCut("TTZCR0SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("TTZCR0SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("TTZCR0SFOSZVt"        , abs(www.Mee3L()-91.1876) > 15.                               , 1                                  );
        cutflow.setCut("TTZCR0SFOSMTmax"      , www.MTmax3L()>90.                                            , 1                                  );
        cutflow.setCut("TTZCR0SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("TTZCR1SFOS"           , (www.nSFOS()==1)                                             , threelep_sf                        );
        cutflow.setCut("TTZCR1SFOSNj2"        , www.nj()<=2                                                  , 1                                  );
        cutflow.setCut("TTZCR1SFOSNb1"        , www.nb()==1                                                  , btagsf                             );
        cutflow.setCut("TTZCR1SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("TTZCR1SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("TTZCR1SFOSMET"        , www.met_pt()>40.                                             , 1                                  );
        cutflow.setCut("TTZCR1SFOSMll"        , www.Mll3L() > 20.                                            , 1                                  );
        cutflow.setCut("TTZCR1SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("TTZCR1SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("TTZCR1SFOSMT3rd"      , www.MT3rd()>90.                                              , 1                                  );
        cutflow.setCut("TTZCR1SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("TTZCR2SFOS"           , (www.nSFOS()==2)                                             , threelep_sf                        );
        cutflow.setCut("TTZCR2SFOSNj2"        , www.nj()<=2                                                  , 1                                  );
        cutflow.setCut("TTZCR2SFOSNb1"        , www.nb()==1                                                  , btagsf                             );
        cutflow.setCut("TTZCR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("TTZCR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("TTZCR2SFOSMET"        , www.met_pt()>55.                                             , 1                                  );
        cutflow.setCut("TTZCR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                                  );
        cutflow.setCut("TTZCR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("TTZCR2SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                                  );
        cutflow.setCut("TTZCR2SFOSFull"       , 1                                                            , 1                                  );

        cutflow.setCut("LXECRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("LXECRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("LXECRSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("LXECRSSmmNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("LXECRSSmmMjjW"        , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("LXECRSSmmMET"         , www.met_pt()<60.                                             , 1                                  );
        cutflow.setCut("LXECRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("LXECRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("LXECRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("LXECRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("LXECRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("LXECRSSemNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("LXECRSSemMjjW"        , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("LXECRSSemMET"         , www.met_pt()<60.                                             , 1                                  );
        cutflow.setCut("LXECRSSemMllSS"       , www.MllSS()>30.                                              , 1                                  );
        cutflow.setCut("LXECRSSemFull"        , 1                                                            , 1                                  );

        cutflow.setCut("LXECRSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , ee_sf                              );
        cutflow.setCut("LXECRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                                  );
        cutflow.setCut("LXECRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("LXECRSSeeNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("LXECRSSeeNb0"         , www.nb()==0                                                  , btagsf                             );
        cutflow.setCut("LXECRSSeeMjjW"        , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("LXECRSSeeMET"         , www.met_pt()<60. and fabs(www.MllSS()-91.1876)>10.           , 1                                  );
        cutflow.setCut("LXECRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("LXECRSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("GCR0SFOS"           , (www.nSFOSinZ()==0)*(www.met_pt()<30)*(fabs(www.M3l()-91.1876)<20)  , threelep_sf                        );
        cutflow.setCut("GCR0SFOSNj1"        , www.nj()<=1                                                         , 1                                  );
        cutflow.setCut("GCR0SFOSNb0"        , www.nb()==0                                                         , btagsf                             );
        cutflow.setCut("GCR0SFOSPt3l"       , 1.                                                                  , 1                                  );
        cutflow.setCut("GCR0SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                                 , 1                                  );
        cutflow.setCut("GCR0SFOSMET"        , www.met_pt()>30.                                                    , 1                                  );
        cutflow.setCut("GCR0SFOSMll"        , www.Mll3L() > 20.                                                   , 1                                  );
        cutflow.setCut("GCR0SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                        , 1                                  );
        cutflow.setCut("GCR0SFOSZVt"        , abs(www.Mee3L()-91.1876) > 15.                                      , 1                                  );
        cutflow.setCut("GCR0SFOSMTmax"      , www.MTmax3L()>90.                                                   , 1                                  );
        cutflow.setCut("GCR0SFOSFull"       , 1                                                                   , 1                                  );

        // Set the variables used for histogramming
        cutflow.setVariable("MllSS"                ,  www.MllSS()                  );
        cutflow.setVariable("MllSS_wide"           ,  www.MllSS()                  );
        cutflow.setVariable("MllZ"                 ,  www.MllSS()                  );
        cutflow.setVariable("MllZZoom"             ,  www.MllSS()                  );
        cutflow.setVariable("Mll3L"                ,  www.Mll3L()                  );
        cutflow.setVariable("Mll3L1"               ,  www.Mll3L1()                 );
        cutflow.setVariable("nSFOSinZ"             ,  www.nSFOSinZ()               );
        cutflow.setVariable("M3l"                  ,  www.M3l()                    );
        cutflow.setVariable("Pt3lGCR"              ,  www.Pt3l()                   );
        cutflow.setVariable("Pt3l"                 ,  www.Pt3l()                   );
        cutflow.setVariable("Ptll"                 ,  www.Pt3l()                   );
        cutflow.setVariable("nvtx"                 ,  www.nVert()                  );
        cutflow.setVariable("Mjj"                  ,  www.Mjj()                    );
        cutflow.setVariable("MjjL"                 ,  www.MjjL()                   );
        cutflow.setVariable("DetajjL"              ,  www.DetajjL()                );
        cutflow.setVariable("MjjVBF"               ,  www.MjjVBF()                 );
        cutflow.setVariable("DetajjVBF"            ,  www.DetajjVBF()              );
        cutflow.setVariable("MET"                  ,  www.met_pt()                 );
        cutflow.setVariable("lep_pt0"              ,  www.lep_pt()[0]              );
        cutflow.setVariable("lep_pt1"              ,  www.lep_pt()[1]              );
        cutflow.setVariable("lep_pt2"              ,  www.lep_pt()[2]              );
        cutflow.setVariable("lep_eta0"             ,  www.lep_eta()[0]             );
        cutflow.setVariable("lep_eta1"             ,  www.lep_eta()[1]             );
        cutflow.setVariable("lep_phi0"             ,  www.lep_phi()[0]             );
        cutflow.setVariable("lep_phi1"             ,  www.lep_phi()[1]             );
        cutflow.setVariable("lep_relIso03EAv2Lep0" ,  www.lep_relIso03EAv2Lep()[0] );
        cutflow.setVariable("lep_relIso03EAv2Lep1" ,  www.lep_relIso03EAv2Lep()[1] );
        cutflow.setVariable("lep_relIso03EAv2Lep2" ,  www.lep_relIso03EAv2Lep()[2] );
        cutflow.setVariable("nj"                   ,  www.nj()                     );
        cutflow.setVariable("nj30"                 ,  www.nj30()                   );
        cutflow.setVariable("nb"                   ,  www.nb()                     );
        cutflow.setVariable("MTmin"                ,  www.MTmin()                  );
        cutflow.setVariable("MTmax"                ,  www.MTmax()                  );
        cutflow.setVariable("MTmax3L"              ,  www.MTmax3L()                );
        cutflow.setVariable("MT3rd"                ,  www.MT3rd()                  );

        int index = fakerates.getFakeLepIndex();
        float ptcorr = index >= 0 ? fakerates.getPtCorr() : -999;
        float abseta = index >= 0 ? fabs(www.lep_eta()[index]) : -999;
        cutflow.setVariable("ptcorretarolledcoarse" , RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, ptcorr, abseta));

        // The yield variable is set to the bin number depending on the signal region channel
        // This is to create the yield money plot
        cutflow.setVariable("yield", yield());
        cutflow.setVariable("wzcryield", wzcryield());

        // Set the event list variables
        cutflow.setEventID(www.run(), www.lumi(), www.evt());

        // Systematic variations
        if (doSystematics)
        {

            // Systematics that affect the raw number of events
            cutflow.setCutSyst("SRSSmmMET"                      , "JESUp"   , 1.                                                    , 1 );
            cutflow.setCutSyst("SRSSmmMjjW"                     , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("SRSSmmMjjL"                     , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSmmDetajjL"                  , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSemMjjW"                     , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("SRSSemMjjL"                     , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSemDetajjL"                  , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSemMET"                      , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSeeMjjW"                     , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("SRSSeeMjjL"                     , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSeeDetajjL"                  , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSeeMET"                      , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSSidemmMjjW"                 , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("SRSSSidemmMjjL"                 , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSSidemmDetajjL"              , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSSidemmMET"                  , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSSideemMjjW"                 , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("SRSSSideemMjjL"                 , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSSideemDetajjL"              , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSSideemMET"                  , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSSideeeMjjW"                 , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("SRSSSideeeMjjL"                 , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSSideeeDetajjL"              , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSSideeeMET"                  , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SR0SFOSDPhi3lMET"               , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("SR0SFOSMET"                     , "JESUp"   , www.met_up_pt()>30.                                   , 1 );
            cutflow.setCutSyst("SR1SFOSDPhi3lMET"               , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("SR1SFOSMET"                     , "JESUp"   , www.met_up_pt()>40.                                   , 1 );
            cutflow.setCutSyst("SR2SFOSDPhi3lMET"               , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("SR2SFOSMET"                     , "JESUp"   , www.met_up_pt()>55.                                   , 1 );
            cutflow.setCutSyst("WZCRSSmmMET"                    , "JESUp"   , 1.                                                    , 1 );
            cutflow.setCutSyst("WZCRSSmmMjjL"                   , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("WZCRSSmmDetajjL"                , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("WZCRSSemMjjL"                   , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("WZCRSSemDetajjL"                , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("WZCRSSemMET"                    , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("WZCRSSeeMjjL"                   , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("WZCRSSeeDetajjL"                , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("WZCRSSeeMET"                    , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("WZCR1SFOSDPhi3lMET"             , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("WZCR1SFOSMET"                   , "JESUp"   , www.met_up_pt()>40.                                   , 1 );
            cutflow.setCutSyst("WZCR2SFOSDPhi3lMET"             , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("WZCR2SFOSMET"                   , "JESUp"   , www.met_up_pt()>55.                                   , 1 );
            cutflow.setCutSyst("ARSSmmMjjW"                     , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("ARSSmmMjjL"                     , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSmmDetajjL"                  , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSmmMET"                      , "JESUp"   , 1.                                                    , 1 );
            cutflow.setCutSyst("ARSSemMjjW"                     , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("ARSSemMjjL"                     , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSemDetajjL"                  , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSemMET"                      , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSeeMjjW"                     , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("ARSSeeMjjL"                     , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSeeDetajjL"                  , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSeeMET"                      , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSSidemmMjjW"                 , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("ARSSSidemmMjjL"                 , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSSidemmDetajjL"              , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSSidemmMET"                  , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSSideemMjjW"                 , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("ARSSSideemMjjL"                 , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSSideemDetajjL"              , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSSideemMET"                  , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSSideeeMjjW"                 , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("ARSSSideeeMjjL"                 , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSSideeeDetajjL"              , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSSideeeMET"                  , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("AR0SFOSDPhi3lMET"               , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("AR0SFOSMET"                     , "JESUp"   , www.met_up_pt()>30.                                   , 1 );
            cutflow.setCutSyst("AR1SFOSDPhi3lMET"               , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("AR1SFOSMET"                     , "JESUp"   , www.met_up_pt()>40.                                   , 1 );
            cutflow.setCutSyst("AR2SFOSDPhi3lMET"               , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("AR2SFOSMET"                     , "JESUp"   , www.met_up_pt()>55.                                   , 1 );
            cutflow.setCutSyst("BTCRSSmmMjjW"                   , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("BTCRSSmmMjjL"                   , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSmmDetajjL"                , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSemMjjW"                   , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("BTCRSSemMjjL"                   , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSemDetajjL"                , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSeeMjjW"                   , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("BTCRSSeeMjjL"                   , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSeeDetajjL"                , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSSidemmMjjW"               , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSSidemmMjjL"               , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSSidemmDetajjL"            , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSSideemMjjW"               , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSSideemMjjL"               , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSSideemDetajjL"            , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSSideeeMjjW"               , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSSideeeMjjL"               , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSSideeeDetajjL"            , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("VBSCRSSmmVBF"                   , "JESUp"   , www.MjjL_up() > 400 or www.DetajjL_up() > 1.5         , 1 );
            cutflow.setCutSyst("VBSCRSSemVBF"                   , "JESUp"   , www.MjjL_up() > 400 or www.DetajjL_up() > 1.5         , 1 );
            cutflow.setCutSyst("VBSCRSSeeVBF"                   , "JESUp"   , www.MjjL_up() > 400 or www.DetajjL_up() > 1.5         , 1 );
            cutflow.setCutSyst("TTWCRSSmmMjjW"                  , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("TTWCRSSmmMjjL"                  , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSmmDetajjL"               , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSemMjjW"                  , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("TTWCRSSemMjjL"                  , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSemDetajjL"               , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSeeMjjW"                  , "JESUp"   , fabs(www.Mjj_up()-80.)<15.                            , 1 );
            cutflow.setCutSyst("TTWCRSSeeMjjL"                  , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSeeDetajjL"               , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmMjjW"              , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmMjjL"              , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmDetajjL"           , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSSideemMjjW"              , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("TTWCRSSSideemMjjL"              , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSSideemDetajjL"           , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeMjjW"              , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeMjjL"              , "JESUp"   , www.MjjL_up()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeDetajjL"           , "JESUp"   , www.DetajjL_up()<1.5                                  , 1 );
            cutflow.setCutSyst("LXECRSSmmMjjW"                  , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("LXECRSSemMjjW"                  , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("LXECRSSeeMjjW"                  , "JESUp"   , fabs(www.Mjj_up()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSmmMET"                    , "JESUp"   , 1.                                                    , 1 );
            cutflow.setCutSyst("BTCRSSemMET"                    , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSeeMET"                    , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSSidemmMET"                , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSSideemMET"                , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSSideeeMET"                , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCR0SFOSDPhi3lMET"             , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("BTCR0SFOSMET"                   , "JESUp"   , www.met_up_pt()>30.                                   , 1 );
            cutflow.setCutSyst("BTCR1SFOSDPhi3lMET"             , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("BTCR1SFOSMET"                   , "JESUp"   , www.met_up_pt()>40.                                   , 1 );
            cutflow.setCutSyst("BTCR2SFOSDPhi3lMET"             , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("BTCR2SFOSMET"                   , "JESUp"   , www.met_up_pt()>55.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSmmMET"                   , "JESUp"   , 1.                                                    , 1 );
            cutflow.setCutSyst("TTWCRSSemMET"                   , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSeeMET"                   , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmMET"               , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSSideemMET"               , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeMET"               , "JESUp"   , www.met_up_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTZCR0SFOSDPhi3lMET"            , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("TTZCR0SFOSMET"                  , "JESUp"   , www.met_up_pt()>30.                                   , 1 );
            cutflow.setCutSyst("TTZCR1SFOSDPhi3lMET"            , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("TTZCR1SFOSMET"                  , "JESUp"   , www.met_up_pt()>40.                                   , 1 );
            cutflow.setCutSyst("TTZCR2SFOSDPhi3lMET"            , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("TTZCR2SFOSMET"                  , "JESUp"   , www.met_up_pt()>55.                                   , 1 );
            cutflow.setCutSyst("LXECRSSmmMET"                   , "JESUp"   , www.met_up_pt()<60.                                   , 1 );
            cutflow.setCutSyst("LXECRSSemMET"                   , "JESUp"   , www.met_up_pt()<60.                                   , 1 );
            cutflow.setCutSyst("LXECRSSeeMET"                   , "JESUp"   , www.met_up_pt()<60. and fabs(www.MllSS()-91.1876)>10. , 1 );
            cutflow.setCutSyst("GCR0SFOSDPhi3lMET"              , "JESUp"   , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("GCR0SFOSMET"                    , "JESUp"   , www.met_up_pt()>30.                                   , 1 );

            cutflow.setCutSyst("SRSSmmMET"                      , "JESDown" , 1.                                                    , 1 );
            cutflow.setCutSyst("SRSSmmMjjW"                     , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("SRSSmmMjjL"                     , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSmmDetajjL"                  , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSemMjjW"                     , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("SRSSemMjjL"                     , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSemDetajjL"                  , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSemMET"                      , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSeeMjjW"                     , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("SRSSeeMjjL"                     , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSeeDetajjL"                  , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSeeMET"                      , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSSidemmMjjW"                 , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("SRSSSidemmMjjL"                 , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSSidemmDetajjL"              , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSSidemmMET"                  , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSSideemMjjW"                 , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("SRSSSideemMjjL"                 , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSSideemDetajjL"              , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSSideemMET"                  , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SRSSSideeeMjjW"                 , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("SRSSSideeeMjjL"                 , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("SRSSSideeeDetajjL"              , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("SRSSSideeeMET"                  , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("SR0SFOSDPhi3lMET"               , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("SR0SFOSMET"                     , "JESDown" , www.met_dn_pt()>30.                                   , 1 );
            cutflow.setCutSyst("SR1SFOSDPhi3lMET"               , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("SR1SFOSMET"                     , "JESDown" , www.met_dn_pt()>40.                                   , 1 );
            cutflow.setCutSyst("SR2SFOSDPhi3lMET"               , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("SR2SFOSMET"                     , "JESDown" , www.met_dn_pt()>55.                                   , 1 );
            cutflow.setCutSyst("WZCRSSmmMET"                    , "JESDown" , 1.                                                    , 1 );
            cutflow.setCutSyst("WZCRSSmmMjjL"                   , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("WZCRSSmmDetajjL"                , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("WZCRSSemMjjL"                   , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("WZCRSSemDetajjL"                , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("WZCRSSemMET"                    , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("WZCRSSeeMjjL"                   , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("WZCRSSeeDetajjL"                , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("WZCRSSeeMET"                    , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("WZCR1SFOSDPhi3lMET"             , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("WZCR1SFOSMET"                   , "JESDown" , www.met_dn_pt()>40.                                   , 1 );
            cutflow.setCutSyst("WZCR2SFOSDPhi3lMET"             , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("WZCR2SFOSMET"                   , "JESDown" , www.met_dn_pt()>55.                                   , 1 );
            cutflow.setCutSyst("ARSSmmMjjW"                     , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("ARSSmmMjjL"                     , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSmmDetajjL"                  , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSmmMET"                      , "JESDown" , 1.                                                    , 1 );
            cutflow.setCutSyst("ARSSemMjjW"                     , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("ARSSemMjjL"                     , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSemDetajjL"                  , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSemMET"                      , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSeeMjjW"                     , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("ARSSeeMjjL"                     , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSeeDetajjL"                  , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSeeMET"                      , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSSidemmMjjW"                 , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("ARSSSidemmMjjL"                 , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSSidemmDetajjL"              , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSSidemmMET"                  , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSSideemMjjW"                 , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("ARSSSideemMjjL"                 , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSSideemDetajjL"              , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSSideemMET"                  , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("ARSSSideeeMjjW"                 , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("ARSSSideeeMjjL"                 , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("ARSSSideeeDetajjL"              , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("ARSSSideeeMET"                  , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("AR0SFOSDPhi3lMET"               , "JESDown" , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("AR0SFOSMET"                     , "JESDown" , www.met_up_pt()>30.                                   , 1 );
            cutflow.setCutSyst("AR1SFOSDPhi3lMET"               , "JESDown" , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("AR1SFOSMET"                     , "JESDown" , www.met_up_pt()>40.                                   , 1 );
            cutflow.setCutSyst("AR2SFOSDPhi3lMET"               , "JESDown" , www.DPhi3lMET_up()>2.5                                , 1 );
            cutflow.setCutSyst("AR2SFOSMET"                     , "JESDown" , www.met_up_pt()>55.                                   , 1 );
            cutflow.setCutSyst("BTCRSSmmMjjW"                   , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("BTCRSSmmMjjL"                   , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSmmDetajjL"                , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSemMjjW"                   , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("BTCRSSemMjjL"                   , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSemDetajjL"                , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSeeMjjW"                   , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("BTCRSSeeMjjL"                   , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSeeDetajjL"                , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSSidemmMjjW"               , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSSidemmMjjL"               , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSSidemmDetajjL"            , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSSideemMjjW"               , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSSideemMjjL"               , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSSideemDetajjL"            , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("BTCRSSSideeeMjjW"               , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSSideeeMjjL"               , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("BTCRSSSideeeDetajjL"            , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("VBSCRSSmmVBF"                   , "JESDown" , www.MjjL_dn() > 400 or www.DetajjL_dn() > 1.5         , 1 );
            cutflow.setCutSyst("VBSCRSSemVBF"                   , "JESDown" , www.MjjL_dn() > 400 or www.DetajjL_dn() > 1.5         , 1 );
            cutflow.setCutSyst("VBSCRSSeeVBF"                   , "JESDown" , www.MjjL_dn() > 400 or www.DetajjL_dn() > 1.5         , 1 );
            cutflow.setCutSyst("TTWCRSSmmMjjW"                  , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("TTWCRSSmmMjjL"                  , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSmmDetajjL"               , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSemMjjW"                  , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("TTWCRSSemMjjL"                  , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSemDetajjL"               , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSeeMjjW"                  , "JESDown" , fabs(www.Mjj_dn()-80.)<15.                            , 1 );
            cutflow.setCutSyst("TTWCRSSeeMjjL"                  , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSeeDetajjL"               , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmMjjW"              , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmMjjL"              , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmDetajjL"           , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSSideemMjjW"              , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("TTWCRSSSideemMjjL"              , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSSideemDetajjL"           , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeMjjW"              , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeMjjL"              , "JESDown" , www.MjjL_dn()<400.                                    , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeDetajjL"           , "JESDown" , www.DetajjL_dn()<1.5                                  , 1 );
            cutflow.setCutSyst("LXECRSSmmMjjW"                  , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("LXECRSSemMjjW"                  , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("LXECRSSeeMjjW"                  , "JESDown" , fabs(www.Mjj_dn()-80.)>=15.                           , 1 );
            cutflow.setCutSyst("BTCRSSmmMET"                    , "JESDown" , 1.                                                    , 1 );
            cutflow.setCutSyst("BTCRSSemMET"                    , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSeeMET"                    , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSSidemmMET"                , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSSideemMET"                , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCRSSSideeeMET"                , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("BTCR0SFOSDPhi3lMET"             , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("BTCR0SFOSMET"                   , "JESDown" , www.met_dn_pt()>30.                                   , 1 );
            cutflow.setCutSyst("BTCR1SFOSDPhi3lMET"             , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("BTCR1SFOSMET"                   , "JESDown" , www.met_dn_pt()>40.                                   , 1 );
            cutflow.setCutSyst("BTCR2SFOSDPhi3lMET"             , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("BTCR2SFOSMET"                   , "JESDown" , www.met_dn_pt()>55.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSmmMET"                   , "JESDown" , 1.                                                    , 1 );
            cutflow.setCutSyst("TTWCRSSemMET"                   , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSeeMET"                   , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmMET"               , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSSideemMET"               , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeMET"               , "JESDown" , www.met_dn_pt()>60.                                   , 1 );
            cutflow.setCutSyst("TTZCR0SFOSDPhi3lMET"            , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("TTZCR0SFOSMET"                  , "JESDown" , www.met_dn_pt()>30.                                   , 1 );
            cutflow.setCutSyst("TTZCR1SFOSDPhi3lMET"            , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("TTZCR1SFOSMET"                  , "JESDown" , www.met_dn_pt()>40.                                   , 1 );
            cutflow.setCutSyst("TTZCR2SFOSDPhi3lMET"            , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("TTZCR2SFOSMET"                  , "JESDown" , www.met_dn_pt()>55.                                   , 1 );
            cutflow.setCutSyst("LXECRSSmmMET"                   , "JESDown" , www.met_dn_pt()<60.                                   , 1 );
            cutflow.setCutSyst("LXECRSSemMET"                   , "JESDown" , www.met_dn_pt()<60.                                   , 1 );
            cutflow.setCutSyst("LXECRSSeeMET"                   , "JESDown" , www.met_dn_pt()<60. and fabs(www.MllSS()-91.1876)>10. , 1 );
            cutflow.setCutSyst("GCR0SFOSDPhi3lMET"              , "JESDown" , www.DPhi3lMET_dn()>2.5                                , 1 );
            cutflow.setCutSyst("GCR0SFOSMET"                    , "JESDown" , www.met_dn_pt()>30.                                   , 1 );

            cutflow.setCutSyst("SRSSmmNj2"                      , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSemNj2"                      , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSeeNj2"                      , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSSidemmNj2"                  , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSSideemNj2"                  , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSSideeeNj2"                  , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("SR0SFOSNj1"                     , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("SR1SFOSNj1"                     , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("SR2SFOSNj1"                     , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("WZCRSSmmNj2"                    , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("WZCRSSemNj2"                    , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("WZCRSSeeNj2"                    , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("WZCR1SFOSNj1"                   , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("WZCR2SFOSNj1"                   , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("ARSSmmNj2"                      , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSemNj2"                      , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSeeNj2"                      , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSSidemmNj2"                  , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSSideemNj2"                  , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSSideeeNj2"                  , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("AR0SFOSNj1"                     , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("AR1SFOSNj1"                     , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("AR2SFOSNj1"                     , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("BTCRSSmmNj2"                    , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSemNj2"                    , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSeeNj2"                    , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSSidemmNj2"                , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSSideemNj2"                , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSSideeeNj2"                , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCR0SFOSNj1"                   , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("BTCR1SFOSNj1"                   , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("BTCR2SFOSNj1"                   , "JESUp"   , www.nj_up()<=1                                        , 1 );
            cutflow.setCutSyst("VBSCRSSmmNj2"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("VBSCRSSemNj2"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("VBSCRSSeeNj2"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("TTWCRSSmmNj4"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("TTWCRSSemNj4"                   , "JESUp"   , www.nj30_up()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSeeNj4"                   , "JESUp"   , www.nj30_up()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmNj4"               , "JESUp"   , www.nj30_up()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSSideemNj4"               , "JESUp"   , www.nj30_up()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeNj4"               , "JESUp"   , www.nj30_up()>= 4                                     , 1 );
            cutflow.setCutSyst("TTZCR0SFOSNj2"                  , "JESUp"   , www.nj_up()<=2                                        , 1 );
            cutflow.setCutSyst("TTZCR1SFOSNj2"                  , "JESUp"   , www.nj_up()<=2                                        , 1 );
            cutflow.setCutSyst("TTZCR2SFOSNj2"                  , "JESUp"   , www.nj_up()<=2                                        , 1 );
            cutflow.setCutSyst("LXECRSSmmNj2"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("LXECRSSemNj2"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("LXECRSSeeNj2"                   , "JESUp"   , www.nj30_up()>= 2                                     , 1 );
            cutflow.setCutSyst("GCR0SFOSNj1"                    , "JESUp"   , www.nj_up()<=1                                        , 1 );

            cutflow.setCutSyst("SRSSmmNj2"                      , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSemNj2"                      , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSeeNj2"                      , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSSidemmNj2"                  , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSSideemNj2"                  , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("SRSSSideeeNj2"                  , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("SR0SFOSNj1"                     , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("SR1SFOSNj1"                     , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("SR2SFOSNj1"                     , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("WZCRSSmmNj2"                    , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("WZCRSSemNj2"                    , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("WZCRSSeeNj2"                    , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("WZCR1SFOSNj1"                   , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("WZCR2SFOSNj1"                   , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("ARSSmmNj2"                      , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSemNj2"                      , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSeeNj2"                      , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSSidemmNj2"                  , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSSideemNj2"                  , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("ARSSSideeeNj2"                  , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("AR0SFOSNj1"                     , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("AR1SFOSNj1"                     , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("AR2SFOSNj1"                     , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("BTCRSSmmNj2"                    , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSemNj2"                    , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSeeNj2"                    , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSSidemmNj2"                , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSSideemNj2"                , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCRSSSideeeNj2"                , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("BTCR0SFOSNj1"                   , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("BTCR1SFOSNj1"                   , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("BTCR2SFOSNj1"                   , "JESDown" , www.nj_dn()<=1                                        , 1 );
            cutflow.setCutSyst("VBSCRSSmmNj2"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("VBSCRSSemNj2"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("VBSCRSSeeNj2"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("TTWCRSSmmNj4"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("TTWCRSSemNj4"                   , "JESDown" , www.nj30_dn()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSeeNj4"                   , "JESDown" , www.nj30_dn()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSSidemmNj4"               , "JESDown" , www.nj30_dn()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSSideemNj4"               , "JESDown" , www.nj30_dn()>= 4                                     , 1 );
            cutflow.setCutSyst("TTWCRSSSideeeNj4"               , "JESDown" , www.nj30_dn()>= 4                                     , 1 );
            cutflow.setCutSyst("TTZCR0SFOSNj2"                  , "JESDown" , www.nj_dn()<=2                                        , 1 );
            cutflow.setCutSyst("TTZCR1SFOSNj2"                  , "JESDown" , www.nj_dn()<=2                                        , 1 );
            cutflow.setCutSyst("TTZCR2SFOSNj2"                  , "JESDown" , www.nj_dn()<=2                                        , 1 );
            cutflow.setCutSyst("LXECRSSmmNj2"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("LXECRSSemNj2"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("LXECRSSeeNj2"                   , "JESDown" , www.nj30_dn()>= 2                                     , 1 );
            cutflow.setCutSyst("GCR0SFOSNj1"                    , "JESDown" , www.nj_dn()<=1                                        , 1 );

            cutflow.setCutSyst("SRSSmmNb0"                      , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSemNb0"                      , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSeeNb0"                      , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSSidemmNb0"                  , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSSideemNb0"                  , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSSideeeNb0"                  , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SR0SFOSNb0"                     , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SR1SFOSNb0"                     , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("SR2SFOSNb0"                     , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("WZCRSSmmNb0"                    , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("WZCRSSemNb0"                    , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("WZCRSSeeNb0"                    , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("WZCR1SFOSNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("WZCR2SFOSNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSmmNb0"                      , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSemNb0"                      , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSeeNb0"                      , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSSidemmNb0"                  , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSSideemNb0"                  , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSSideeeNb0"                  , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("AR0SFOSNb0"                     , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("AR1SFOSNb0"                     , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("AR2SFOSNb0"                     , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("BTCRSSmmNbgeq1"                 , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSemNbgeq1"                 , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSeeNbgeq1"                 , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSSidemmNbgeq1"             , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSSideemNbgeq1"             , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSSideeeNbgeq1"             , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCR0SFOSNbgeq1"                , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCR1SFOSNbgeq1"                , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCR2SFOSNbgeq1"                , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("VBSCRSSmmNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("VBSCRSSemNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("VBSCRSSeeNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSmmNbgeq1"                , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSemNbgeq1"                , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSeeNbgeq1"                , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSSidemmNbgeq1"            , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSSideemNbgeq1"            , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSSideeeNbgeq1"            , "JESUp"   , www.nb_up()>=1                                        , btagsf );
            cutflow.setCutSyst("TTZCR0SFOSNb1"                  , "JESUp"   , www.nb_up()==1                                        , btagsf );
            cutflow.setCutSyst("TTZCR1SFOSNb1"                  , "JESUp"   , www.nb_up()==1                                        , btagsf );
            cutflow.setCutSyst("TTZCR2SFOSNb1"                  , "JESUp"   , www.nb_up()==1                                        , btagsf );
            cutflow.setCutSyst("LXECRSSmmNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("LXECRSSemNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("LXECRSSeeNb0"                   , "JESUp"   , www.nb_up()==0                                        , btagsf );
            cutflow.setCutSyst("GCR0SFOSNb0"                    , "JESUp"   , www.nb_up()==0                                        , btagsf );

            cutflow.setCutSyst("SRSSmmNb0"                      , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSemNb0"                      , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSeeNb0"                      , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSSidemmNb0"                  , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSSideemNb0"                  , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SRSSSideeeNb0"                  , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SR0SFOSNb0"                     , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SR1SFOSNb0"                     , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("SR2SFOSNb0"                     , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("WZCRSSmmNb0"                    , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("WZCRSSemNb0"                    , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("WZCRSSeeNb0"                    , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("WZCR1SFOSNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("WZCR2SFOSNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSmmNb0"                      , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSemNb0"                      , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSeeNb0"                      , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSSidemmNb0"                  , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSSideemNb0"                  , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("ARSSSideeeNb0"                  , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("AR0SFOSNb0"                     , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("AR1SFOSNb0"                     , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("AR2SFOSNb0"                     , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("BTCRSSmmNbgeq1"                 , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSemNbgeq1"                 , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSeeNbgeq1"                 , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSSidemmNbgeq1"             , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSSideemNbgeq1"             , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCRSSSideeeNbgeq1"             , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCR0SFOSNbgeq1"                , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCR1SFOSNbgeq1"                , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("BTCR2SFOSNbgeq1"                , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("VBSCRSSmmNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("VBSCRSSemNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("VBSCRSSeeNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSmmNbgeq1"                , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSemNbgeq1"                , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSeeNbgeq1"                , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSSidemmNbgeq1"            , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSSideemNbgeq1"            , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("TTWCRSSSideeeNbgeq1"            , "JESDown" , www.nb_dn()>=1                                        , btagsf );
            cutflow.setCutSyst("TTZCR0SFOSNb1"                  , "JESDown" , www.nb_dn()==1                                        , btagsf );
            cutflow.setCutSyst("TTZCR1SFOSNb1"                  , "JESDown" , www.nb_dn()==1                                        , btagsf );
            cutflow.setCutSyst("TTZCR2SFOSNb1"                  , "JESDown" , www.nb_dn()==1                                        , btagsf );
            cutflow.setCutSyst("LXECRSSmmNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("LXECRSSemNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("LXECRSSeeNb0"                   , "JESDown" , www.nb_dn()==0                                        , btagsf );
            cutflow.setCutSyst("GCR0SFOSNb0"                    , "JESDown" , www.nb_dn()==0                                        , btagsf );

            cutflow.setWgtSyst("LepSFUp"    , www.lepsf_up()               / www.lepsf()         );
            cutflow.setWgtSyst("LepSFDown"  , www.lepsf_dn()               / www.lepsf()         );
            cutflow.setWgtSyst("TrigSFUp"   , www.trigsf_up()              / www.trigsf()        );
            cutflow.setWgtSyst("TrigSFDown" , www.trigsf_dn()              / www.trigsf()        );
            cutflow.setWgtSyst("BTagLFUp"   , www.weight_btagsf_light_DN() / www.weight_btagsf() );
            cutflow.setWgtSyst("BTagLFDown" , www.weight_btagsf_light_UP() / www.weight_btagsf() );
            cutflow.setWgtSyst("BTagHFUp"   , www.weight_btagsf_heavy_DN() / www.weight_btagsf() );
            cutflow.setWgtSyst("BTagHFDown" , www.weight_btagsf_heavy_UP() / www.weight_btagsf() );
            cutflow.setWgtSyst("PileupUp"   , www.purewgt_dn()             / www.purewgt()       );
            cutflow.setWgtSyst("PileupDown" , www.purewgt_up()             / www.purewgt()       );

            if (doFakeEstimation)
            {
                if (!is2017)
                {
                    cutflow.setWgtSyst("FakeUp"            , www.ffwgt_full_up()       / www.ffwgt() );
                    cutflow.setWgtSyst("FakeDown"          , www.ffwgt_full_dn()       / www.ffwgt() );
                    cutflow.setWgtSyst("FakeRateUp"        , www.ffwgt_up()            / www.ffwgt() );
                    cutflow.setWgtSyst("FakeRateDown"      , www.ffwgt_dn()            / www.ffwgt() );
                    cutflow.setWgtSyst("FakeRateElUp"      , www.ffwgt_el_up()         / www.ffwgt() );
                    cutflow.setWgtSyst("FakeRateElDown"    , www.ffwgt_el_dn()         / www.ffwgt() );
                    cutflow.setWgtSyst("FakeRateMuUp"      , www.ffwgt_mu_up()         / www.ffwgt() );
                    cutflow.setWgtSyst("FakeRateMuDown"    , www.ffwgt_mu_dn()         / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureUp"     , www.ffwgt_closure_up()    / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureDown"   , www.ffwgt_closure_dn()    / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureElUp"   , www.ffwgt_closure_el_up() / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureElDown" , www.ffwgt_closure_el_dn() / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureMuUp"   , www.ffwgt_closure_mu_up() / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureMuDown" , www.ffwgt_closure_mu_dn() / www.ffwgt() );
                    cutflow.setWgtSyst("FakeClosureMuDown" , www.ffwgt_closure_mu_dn() / www.ffwgt() );
                }
                else
                {
                    // TODO
                    cutflow.setWgtSyst("FakeUp"            , 1 );
                    cutflow.setWgtSyst("FakeDown"          , 1 );
                    cutflow.setWgtSyst("FakeRateUp"        , 1 );
                    cutflow.setWgtSyst("FakeRateDown"      , 1 );
                    cutflow.setWgtSyst("FakeRateElUp"      , 1 );
                    cutflow.setWgtSyst("FakeRateElDown"    , 1 );
                    cutflow.setWgtSyst("FakeRateMuUp"      , 1 );
                    cutflow.setWgtSyst("FakeRateMuDown"    , 1 );
                    cutflow.setWgtSyst("FakeClosureUp"     , 1 );
                    cutflow.setWgtSyst("FakeClosureDown"   , 1 );
                    cutflow.setWgtSyst("FakeClosureElUp"   , 1 );
                    cutflow.setWgtSyst("FakeClosureElDown" , 1 );
                    cutflow.setWgtSyst("FakeClosureMuUp"   , 1 );
                    cutflow.setWgtSyst("FakeClosureMuDown" , 1 );
                    cutflow.setWgtSyst("FakeClosureMuDown" , 1 );
                }
            }
        }
        // Once every cut bits are set, now fill the cutflows that are booked
        cutflow.fill();
    }

//    cutflow.getCut("SR2SFOSFull").sortEventList();
//    cutflow.getCut("SR2SFOSFull").printEventList();

    // Save output
    cutflow.saveOutput();

    return 0;
}

//_______________________________________________________________________________________________________
int help()
{
    // Help function
    std::cout << "Usage:" << std::endl;
    std::cout << std::endl;
    std::cout << "  $ ./process INPUTFILES INPUTTREENAME OUTPUTFILE [NEVENTS]" << std::endl;
    std::cout << std::endl;
    std::cout << "  INPUTFILES      comma separated file list" << std::endl;
    std::cout << "  INPUTTREENAME   tree name in the file" << std::endl;
    std::cout << "  OUTPUTFILE      output file name" << std::endl;
    std::cout << "  [NEVENTS=-1]    # of events to run over" << std::endl;
    std::cout << std::endl;
    return 1;
}

//_______________________________________________________________________________________________________
int main(int argc, char** argv)
{
    if (argc == 4)
    {
        return process(argv[1], argv[2], argv[3], -1);
    }
    else if (argc == 5)
    {
        return process(argv[1], argv[2], argv[3], atoi(argv[4]));
    }
    else
    {
        return help();
    }
}

