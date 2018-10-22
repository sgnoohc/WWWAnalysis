#include "wwwtree.h"
#include "rooutil/rooutil.h"
#include "process.h"

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

//_______________________________________________________________________________________________________
int help()
{
    // Help function
    std::cout << "Usage:" << std::endl;
    std::cout << std::endl;
    std::cout << "CASE 1: 3 or more arguments" << std::endl;
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
    cutflow.addCutToLastActiveCut("CutSRDilep");
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
    cutflow.getCut("CutTrigger");
    cutflow.addCutToLastActiveCut("CutSRTrilep");
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

    // Now book cutflows
    cutflow.bookCutflows();

    // Cutflow object that takes the histograms and books them to a cutflow for histogramming
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSmmFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSemFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSeeFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSidemmFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSideemFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSSideeeFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR0SFOSFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR1SFOSFull");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SR2SFOSFull");

    // Event list
    cutflow.bookEventLists();

    // Print the cut structure for review
    cutflow.printCuts();

    //
    //
    // Looping events
    //
    //
    while (looper.nextEvent())
    {

        // Luminosity setting
//        float lumi = www.is2016() == 1 ? 35.9 : 41.3;
        float lumi = 41.3;

        // Compute preselection
        bool presel = (www.firstgoodvertex() == 0);
        presel &= (www.Flag_AllEventFilters() > 0);
        presel &= (www.vetophoton() == 0);
        presel &= (www.evt_passgoodrunlist() > 0);

        // Event weight
        float weight = www.evt_scale1fb() * www.purewgt() * lumi;

        //      setCut("CutName"       , <boolean value to say whether it passes>           , <float value to define weight>);
        cutflow.setCut("CutWeight"         , 1                                                            , weight              );
        cutflow.setCut("CutPresel"         , presel                                                       , 1                   );
        cutflow.setCut("CutTrigger"        , www.passTrigger() * www.pass_duplicate_ee_em_mm()            , www.trigsf()        );
        cutflow.setCut("CutSRDilep"        , (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 2) , www.lepsf()         );
        cutflow.setCut("CutSRTrilep"       , (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 3) , www.lepsf()         );

        cutflow.setCut("SRSSmm"            , (www.passSSmm())*(www.MllSS()>40.)                           , 1                   );
        cutflow.setCut("SRSSmmTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSmmNj2"         , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSmmNb0"         , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSmmMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                   );
        cutflow.setCut("SRSSmmMjjL"        , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSmmDetajjL"     , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSmmMET"         , 1.                                                           , 1                   );
        cutflow.setCut("SRSSmmMllSS"       , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSmmFull"        , 1                                                            , 1                   );

        cutflow.setCut("SRSSem"            , (www.passSSem())*(www.MllSS()>30.)                           , 1                   );
        cutflow.setCut("SRSSemTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSemNj2"         , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSemNb0"         , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSemMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                   );
        cutflow.setCut("SRSSemMjjL"        , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSemDetajjL"     , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSemMET"         , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSemMllSS"       , www.MllSS()>30.                                              , 1                   );
        cutflow.setCut("SRSSemMTmax"       , www.MTmax()>90.                                              , 1                   );
        cutflow.setCut("SRSSemFull"        , 1                                                            , 1                   );

        cutflow.setCut("SRSSee"            , (www.passSSee())*(1)*(www.MllSS()>40.)                       , 1                   );
        cutflow.setCut("SRSSeeZeeVt"       , fabs(www.MllSS()-91.1876)>10.                                , 1                   );
        cutflow.setCut("SRSSeeTVeto"       , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSeeNj2"         , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSeeNb0"         , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSeePre"         , 1                                                            , 1                   );
        cutflow.setCut("SRSSeeMjjW"        , fabs(www.Mjj()-80.)<15.                                      , 1                   );
        cutflow.setCut("SRSSeeMjjL"        , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSeeDetajjL"     , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSeeMET"         , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSeeMllSS"       , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSeeFull"        , 1                                                            , 1                   );

        cutflow.setCut("SRSSSidemm"        , (www.passSSmm())*(www.MllSS()>40.)                           , 1                   );
        cutflow.setCut("SRSSSidemmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSSidemmNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSSidemmNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSSidemmMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                   );
        cutflow.setCut("SRSSSidemmMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSSidemmDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSSidemmMET"     , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSSidemmMllSS"   , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSSidemmFull"    , 1                                                            , 1                   );

        cutflow.setCut("SRSSSideem"        , (www.passSSem())*(www.MllSS()>30.)                           , 1                   );
        cutflow.setCut("SRSSSideemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSSideemNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSSideemNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSSideemMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                   );
        cutflow.setCut("SRSSSideemMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSSideemDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSSideemMET"     , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSSideemMllSS"   , www.MllSS()>30.                                              , 1                   );
        cutflow.setCut("SRSSSideemMTmax"   , www.MTmax()>90.                                              , 1                   );
        cutflow.setCut("SRSSSideemFull"    , 1                                                            , 1                   );

        cutflow.setCut("SRSSSideee"        , (www.passSSee())*(1)*(www.MllSS()>40.)                       , 1                   );
        cutflow.setCut("SRSSSideeeZeeVt"   , fabs(www.MllSS()-91.1876)>10.                                , 1                   );
        cutflow.setCut("SRSSSideeeTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSSideeeNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSSideeeNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSSideeePre"     , 1                                                            , 1                   );
        cutflow.setCut("SRSSSideeeMjjW"    , fabs(www.Mjj()-80.)>=15.                                     , 1                   );
        cutflow.setCut("SRSSSideeeMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSSideeeDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSSideeeMET"     , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSSideeeMllSS"   , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSSideeeFull"    , 1                                                            , 1                   );

        cutflow.setCut("SR0SFOS"           , (www.nSFOS()==0)                                             , 1                   );
        cutflow.setCut("SR0SFOSNj1"        , www.nj()<=1                                                  , 1                   );
        cutflow.setCut("SR0SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SR0SFOSPre"        , 1                                                            , 1                   );
        cutflow.setCut("SR0SFOSPt3l"       , 1.                                                           , 1                   );
        cutflow.setCut("SR0SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                   );
        cutflow.setCut("SR0SFOSMET"        , www.met_pt()>30.                                             , 1                   );
        cutflow.setCut("SR0SFOSMll"        , www.Mll3L() > 20.                                            , 1                   );
        cutflow.setCut("SR0SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                   );
        cutflow.setCut("SR0SFOSZVt"        , abs(www.Mee3L()-91.1876) > 15.                               , 1                   );
        cutflow.setCut("SR0SFOSMTmax"      , www.MTmax3L()>90.                                            , 1                   );
        cutflow.setCut("SR0SFOSFull"       , 1                                                            , 1                   );

        cutflow.setCut("SR1SFOS"           , (www.nSFOS()==1)                                             , 1                   );
        cutflow.setCut("SR1SFOSNj1"        , www.nj()<=1                                                  , 1                   );
        cutflow.setCut("SR1SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SR1SFOSPre"        , 1                                                            , 1                   );
        cutflow.setCut("SR1SFOSPt3l"       , www.Pt3l()>60.                                               , 1                   );
        cutflow.setCut("SR1SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                   );
        cutflow.setCut("SR1SFOSMET"        , www.met_pt()>40.                                             , 1                   );
        cutflow.setCut("SR1SFOSMll"        , www.Mll3L() > 20.                                            , 1                   );
        cutflow.setCut("SR1SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                   );
        cutflow.setCut("SR1SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                   );
        cutflow.setCut("SR1SFOSMT3rd"      , www.MT3rd()>90.                                              , 1                   );
        cutflow.setCut("SR1SFOSFull"       , 1                                                            , 1                   );

        cutflow.setCut("SR2SFOS"           , (www.nSFOS()==2)                                             , 1                   );
        cutflow.setCut("SR2SFOSNj1"        , www.nj()<=1                                                  , 1                   );
        cutflow.setCut("SR2SFOSNb0"        , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SR2SFOSPre"        , 1                                                            , 1                   );
        cutflow.setCut("SR2SFOSPt3l"       , www.Pt3l()>60.                                               , 1                   );
        cutflow.setCut("SR2SFOSDPhi3lMET"  , www.DPhi3lMET()>2.5                                          , 1                   );
        cutflow.setCut("SR2SFOSMET"        , www.met_pt()>55.                                             , 1                   );
        cutflow.setCut("SR2SFOSMll"        , (www.Mll3L() > 20. && www.Mll3L1() > 20.)                    , 1                   );
        cutflow.setCut("SR2SFOSM3l"        , abs(www.M3l()-91.1876) > 10.                                 , 1                   );
        cutflow.setCut("SR2SFOSZVt"        , www.nSFOSinZ() == 0                                          , 1                   );
        cutflow.setCut("SR2SFOSFull"       , 1                                                            , 1                   );

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
        int yield = 0;
        if (www.nVlep() == 2)
        {
            if (fabs(www.Mjj() - 80.) < 15.)
            {
                if (www.passSSee()) yield = 0;
                if (www.passSSem()) yield = 1;
                if (www.passSSmm()) yield = 2;
            }
            else if (fabs(www.Mjj() - 80.) > 15.)
            {
                if (www.passSSee()) yield = 3;
                if (www.passSSem()) yield = 4;
                if (www.passSSmm()) yield = 5;
            }
        }
        else
        {
            if (www.nSFOS() == 0) yield = 6;
            if (www.nSFOS() == 1) yield = 7;
            if (www.nSFOS() == 2) yield = 8;
        }
        cutflow.setVariable("yield", yield);

        // Set the event list variables
        cutflow.setEventID(www.run(), www.lumi(), www.evt());

        // Once every cut bits are set, now fill the cutflows that are booked
        cutflow.fill();
    }

//    cutflow.getCut("SRSSmmFull").sortEventList();
//    cutflow.getCut("SRSSmmFull").printEventList();

    // Save output
    cutflow.saveOutput();

    return 0;
}
