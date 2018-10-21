#include "wwwtree.h"
#include "rooutil/rooutil.h"

// ./process INPUTFILEPATH OUTPUTFILEPATH [NEVENTS]
int main(int argc, char** argv)
{
    // Argument checking
    if (argc < 3)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "  $ ./process INPUTFILES OUTPUTFILE [NEVENTS]" << std::endl;
        std::cout << std::endl;
        std::cout << "  INPUTFILES      comma separated file list" << std::endl;
        std::cout << "  OUTPUTFILE      output file name" << std::endl;
        std::cout << "  [NEVENTS=-1]    # of events to run over" << std::endl;
        std::cout << std::endl;
        return 1;
    }

    // Creating output file where we will put the outputs of the processing
    TFile* ofile = new TFile(argv[2], "recreate");

    // Create a TChain of the input files
    // The input files can be comma separated (e.g. "file1.root,file2.root")
    TChain* ch = RooUtil::FileUtil::createTChain("t", argv[1]);

    // Number of events to loop over
    int nEvents = argc > 3 ? atoi(argv[3]) : -1;

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

    // Now book cutflows
    cutflow.bookCutflows();

    // Cutflow object that takes the histograms and books them to a cutflow for histogramming
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSmm");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSem");
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSee");

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
        float lumi = www.is2016() == 1 ? 35.9 : 41.3;

        // Compute preselection
        bool presel = (www.firstgoodvertex() == 0);
        presel &= (www.Flag_AllEventFilters() > 0);
        presel &= (www.vetophoton() == 0);
        presel &= (www.evt_passgoodrunlist() > 0);

        // Event weight
        float weight = www.evt_scale1fb() * www.purewgt() * lumi;

        //      setCut("CutName"       , <boolean value to say whether it passes>           , <float value to define weight>);
        cutflow.setCut("CutWeight"     , 1                                                            , weight              );
        cutflow.setCut("CutPresel"     , presel                                                       , 1                   );
        cutflow.setCut("CutTrigger"    , www.passTrigger() * www.pass_duplicate_ee_em_mm()            , www.trigsf()        );
        cutflow.setCut("CutSRDilep"    , (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 2) , www.lepsf()         );
        cutflow.setCut("SRSSmm"        , (www.passSSmm())*(www.MllSS()>40.)                           , 1                   );
        cutflow.setCut("SRSSmmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSmmNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSmmNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSmmMjjW"    , fabs(www.Mjj()-80.)<15.                                      , 1                   );
        cutflow.setCut("SRSSmmMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSmmDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSmmMET"     , 1.                                                           , 1                   );
        cutflow.setCut("SRSSmmMllSS"   , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSmmFull"    , 1                                                            , 1                   );
        cutflow.setCut("SRSSem"        , (www.passSSem())*(www.MllSS()>30.)                           , 1                   );
        cutflow.setCut("SRSSemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSemNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSemNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSemMjjW"    , fabs(www.Mjj()-80.)<15.                                      , 1                   );
        cutflow.setCut("SRSSemMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSemDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSemMET"     , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSemMllSS"   , www.MllSS()>30.                                              , 1                   );
        cutflow.setCut("SRSSemMTmax"   , www.MTmax()>90.                                              , 1                   );
        cutflow.setCut("SRSSemFull"    , 1                                                            , 1                   );
        cutflow.setCut("SRSSee"        , (www.passSSee())*(1)*(www.MllSS()>40.)                       , 1                   );
        cutflow.setCut("SRSSeeZeeVt"   , fabs(www.MllSS()-91.1876)>10.                                , 1                   );
        cutflow.setCut("SRSSeeTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSeeNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSeeNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSeePre"     , 1                                                            , 1                   );
        cutflow.setCut("SRSSeeMjjW"    , fabs(www.Mjj()-80.)<15.                                      , 1                   );
        cutflow.setCut("SRSSeeMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSeeDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSeeMET"     , www.met_pt()>60.                                             , 1                   );
        cutflow.setCut("SRSSeeMllSS"   , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSeeFull"    , 1                                                            , 1                   );

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

        // Once every cut bits are set, now fill the cutflows that are booked
        cutflow.fill();
    }

    // Save output
    cutflow.saveOutput();
}
