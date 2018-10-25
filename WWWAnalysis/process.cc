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
    cutflow.addCutToLastActiveCut("SRSSeePre");
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
    cutflow.addCutToLastActiveCut("SRSSSideeePre");
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
    cutflow.addCutToLastActiveCut("SR0SFOSPre");
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
    cutflow.addCutToLastActiveCut("SR1SFOSPre");
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
    cutflow.addCutToLastActiveCut("SR2SFOSPre");
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
    cutflow.addCutToLastActiveCut("WZCRSSeePre");
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
    cutflow.addCutToLastActiveCut("WZCR1SFOSPre");
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
    cutflow.addCutToLastActiveCut("WZCR2SFOSPre");
    cutflow.addCutToLastActiveCut("WZCR2SFOSPt3l");
    cutflow.addCutToLastActiveCut("WZCR2SFOSDPhi3lMET");
    cutflow.addCutToLastActiveCut("WZCR2SFOSMET");
    cutflow.addCutToLastActiveCut("WZCR2SFOSMll");
    cutflow.addCutToLastActiveCut("WZCR2SFOSM3l");
    cutflow.addCutToLastActiveCut("WZCR2SFOSZVt");
    cutflow.addCutToLastActiveCut("WZCR2SFOSFull");

    // Histogram utility object that is used to define the histograms
    RooUtil::Histograms histograms;
    histograms.addHistogram("MllSS"                ,  180 , 0.      , 300.   );
    histograms.addHistogram("MllSS_wide"           ,  180 , 0.      , 2000.  );
    histograms.addHistogram("MllZ"                 ,  180 , 60.     , 120.   );
    histograms.addHistogram("MllZZoom"             ,  180 , 80.     , 100.   );
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

    // Now book cutflows
    cutflow.bookCutflows();

    // Cutflow object that takes the histograms and books them to a cutflow for histogramming
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSmm");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSem");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSee");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSidemm");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSideem");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSideee");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR0SFOS");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR1SFOS");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR2SFOS");
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCRSSmm");
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCRSSem");
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCRSSee");
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCR1SFOS");
    cutflow.bookHistogramsForCutAndBelow(histograms, "WZCR2SFOS");

    // Event list
    cutflow.bookEventLists();

    // Print the cut structure for review
    cutflow.printCuts();

    // Scale factors
    LeptonScaleFactors leptonScaleFactors;

    // Some case-by-case checking needed for WWW_v1.2.2 (should be no longer necessary later on)
    bool is2017 = TString(input_paths).Contains("WWW2017");
    bool isWWW = TString(input_paths).Contains("www_2l_");

    // For fake estimations, we use data-driven method.
    // When looping over data and the output_path is set to have a "fakes" substring included we turn on the fake-weight settings
    bool doFakeEstimation = TString(input_paths).Contains("data_") && TString(output_file_name).Contains("fakes");
    bool isData = TString(input_paths).Contains("data_");

    //
    //
    // Looping events
    //
    //
    while (looper.nextEvent())
    {

        // Luminosity setting
        float lumi = is2017 == 1 ? 41.3 : 35.9;
        lumi = doFakeEstimation ? www.ffwgt() : lumi;

        // Compute preselection
        bool presel = (www.firstgoodvertex() == 0);
        presel &= (www.Flag_AllEventFilters() > 0);
        presel &= (www.vetophoton() == 0);
        presel &= (www.evt_passgoodrunlist() > 0);

        // Compute trigger variable
        bool trigger = is2017 == 1 ? www.passTrigger() * www.pass_duplicate_ee_em_mm() : passTrigger2016();

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
        std::tie(ee_sf, em_sf, mm_sf, threelep_sf) = leptonScaleFactors.getScaleSactors(is2017, doFakeEstimation);

        //      setCut("CutName"       , <boolean value to say whether it passes>           , <float value to define weight>);
        cutflow.setCut("CutWeight"         , 1                                                            , weight                             );
        cutflow.setCut("CutPresel"         , presel                                                       , 1                                  );
        cutflow.setCut("CutTrigger"        , trigger                                                      , www.trigsf()                       );
        cutflow.setCut("CutSRDilep"        , doFakeEstimation ? isfakedilep : isdilep                     , 1                                  );
        cutflow.setCut("CutSRTrilep"       , doFakeEstimation ? isfaketrilep : istrilep                   , 1                                  );
        cutflow.setCut("CutWZCRDilep"      , doFakeEstimation ? isfakewzcrtrilep : iswzcrtrilep           , 1                                  );
        cutflow.setCut("CutWZCRTrilep"     , doFakeEstimation ? isfaketrilep : istrilep                   , 1                                  );

        cutflow.setCut("SRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("SRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSmmNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSmmNb0"         , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SRSSmmMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("SRSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("SRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("SRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("SRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSemNb0"         , www.nb()==0                                                  , www.weight_btagsf()                );
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
        cutflow.setCut("SRSSeeNb0"         , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SRSSeePre"         , 1                                                            , 1                                  );
        cutflow.setCut("SRSSeeMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                                  );
        cutflow.setCut("SRSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSeeFull"        , 1                                                            , 1                                  );

        cutflow.setCut("SRSSSidemm"        , (www.passSSmm())*(www.MllSS()>40.)                           , mm_sf                              );
        cutflow.setCut("SRSSSidemmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSSidemmNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSSidemmNb0"     , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SRSSSidemmMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("SRSSSidemmMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSSidemmDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSSidemmMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSSidemmMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSSidemmFull"    , 1                                                            , 1                                  );

        cutflow.setCut("SRSSSideem"        , (www.passSSem())*(www.MllSS()>30.)                           , em_sf                              );
        cutflow.setCut("SRSSSideemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("SRSSSideemNj2"     , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("SRSSSideemNb0"     , www.nb()==0                                                  , www.weight_btagsf()                );
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
        cutflow.setCut("SRSSSideeeNb0"     , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SRSSSideeePre"     , 1                                                            , 1                                  );
        cutflow.setCut("SRSSSideeeMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                                  );
        cutflow.setCut("SRSSSideeeMjjL"    , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("SRSSSideeeDetajjL" , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("SRSSSideeeMET"     , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("SRSSSideeeMllSS"   , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("SRSSSideeeFull"    , 1                                                            , 1                                  );

        cutflow.setCut("SR0SFOS"           , (www.nSFOS()==0)                                             , threelep_sf                        );
        cutflow.setCut("SR0SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("SR0SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SR0SFOSPre"        , 1                                                            , 1                                  );
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
        cutflow.setCut("SR1SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SR1SFOSPre"        , 1                                                            , 1                                  );
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
        cutflow.setCut("SR2SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("SR2SFOSPre"        , 1                                                            , 1                                  );
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
        cutflow.setCut("WZCRSSmmNb0"         , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("WZCRSSmmMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("WZCRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("WZCRSSmmMET"         , 1.                                                           , 1                                  );
        cutflow.setCut("WZCRSSmmMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("WZCRSSmmFull"        , 1                                                            , 1                                  );

        cutflow.setCut("WZCRSSem"            , (hasz_ss)*(www.passSSem())*(www.MllSS()>30.)                 , threelep_sf                        );
        cutflow.setCut("WZCRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                                  );
        cutflow.setCut("WZCRSSemNj2"         , www.nj30()>= 2                                               , 1                                  );
        cutflow.setCut("WZCRSSemNb0"         , www.nb()==0                                                  , www.weight_btagsf()                );
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
        cutflow.setCut("WZCRSSeeNb0"         , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("WZCRSSeePre"         , 1                                                            , 1                                  );
        cutflow.setCut("WZCRSSeeMjjL"        , www.MjjL()<400.                                              , 1                                  );
        cutflow.setCut("WZCRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                                  );
        cutflow.setCut("WZCRSSeeMET"         , www.met_pt()>60.                                             , 1                                  );
        cutflow.setCut("WZCRSSeeMllSS"       , www.MllSS()>40.                                              , 1                                  );
        cutflow.setCut("WZCRSSeeFull"        , 1                                                            , 1                                  );

        // Condition for SS WZ CR to check whether it had a Z
        bool hasz_3l = (abs(www.Mll3L()-91.1876)<20.||abs(www.Mll3L1()-91.1876)<20.);

        cutflow.setCut("WZCR1SFOS"           , (www.nSFOS()==1)                                             , threelep_sf                        );
        cutflow.setCut("WZCR1SFOSNj1"        , www.nj()<=1                                                  , 1                                  );
        cutflow.setCut("WZCR1SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("WZCR1SFOSPre"        , 1                                                            , 1                                  );
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
        cutflow.setCut("WZCR2SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf()                );
        cutflow.setCut("WZCR2SFOSPre"        , 1                                                            , 1                                  );
        cutflow.setCut("WZCR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                                  );
        cutflow.setCut("WZCR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                                  );
        cutflow.setCut("WZCR2SFOSMET"        , www.met_pt()>55.                                             , 1                                  );
        cutflow.setCut("WZCR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                                  );
        cutflow.setCut("WZCR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                                  );
        cutflow.setCut("WZCR2SFOSZVt"        , hasz_3l                                                      , 1                                  );
        cutflow.setCut("WZCR2SFOSFull"       , 1                                                            , 1                                  );

        // Set the variables used for histogramming
        cutflow.setVariable("MllSS"                ,  www.MllSS()                  );
        cutflow.setVariable("MllSS_wide"           ,  www.MllSS()                  );
        cutflow.setVariable("MllZ"                 ,  www.MllSS()                  );
        cutflow.setVariable("MllZZoom"             ,  www.MllSS()                  );
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

        // The yield variable is set to the bin number depending on the signal region channel
        // This is to create the yield money plot
        cutflow.setVariable("yield", yield());
        cutflow.setVariable("wzcryield", wzcryield());

        // Set the event list variables
        cutflow.setEventID(www.run(), www.lumi(), www.evt());

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

