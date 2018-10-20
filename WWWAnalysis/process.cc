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
    cutflow.addCutToLastActiveCut("SRSSemFull");

    // Histogram utility object that is used to define the histograms
    RooUtil::Histograms histograms;
    histograms.addHistogram("Mjj", 180, 0, 250);
    histograms.addHistogram("Mll", 180, 0, 250);

    // Now book cutflows
    cutflow.bookCutflows();

    // Cutflow object that takes the histograms and books them to a cutflow for histogramming
    cutflow.bookHistogramsForCutAndBelow(histograms, "SRSSmm");

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
        // setCut("CutName", <boolean value to say whether it passes>, <float value to define weight>);
        cutflow.setCut("CutWeight"     , 1                                                            , weight              );
        cutflow.setCut("CutPresel"     , presel                                                       , 1                   );
        cutflow.setCut("CutTrigger"    , www.passTrigger() * www.pass_duplicate_ee_em_mm()            , www.trigsf()        );
        cutflow.setCut("CutSRDilep"    , (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 2) , www.lepsf()         );
        cutflow.setCut("SRSSmm"        , (www.passSSmm())*(www.MllSS()>40.)                           , 1                   );
        cutflow.setCut("SRSSmmTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSmmNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSmmNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSmmMjjW"    , abs(www.Mjj()-80.)<15.                                       , 1                   );
        cutflow.setCut("SRSSmmMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSmmDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSmmMET"     , 1.                                                           , 1                   );
        cutflow.setCut("SRSSmmMllSS"   , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSmmFull"    , 1                                                            , 1                   );
        cutflow.setCut("SRSSem"        , (www.passSSem())*(www.MllSS()>40.)                           , 1                   );
        cutflow.setCut("SRSSemTVeto"   , www.nisoTrack_mt2_cleaned_VVV_cutbased_veto()==0             , 1                   );
        cutflow.setCut("SRSSemNj2"     , www.nj30()>= 2                                               , 1                   );
        cutflow.setCut("SRSSemNb0"     , www.nb()==0                                                  , www.weight_btagsf() );
        cutflow.setCut("SRSSemMjjW"    , abs(www.Mjj()-80.)<15.                                       , 1                   );
        cutflow.setCut("SRSSemMjjL"    , www.MjjL()<400.                                              , 1                   );
        cutflow.setCut("SRSSemDetajjL" , www.DetajjL()<1.5                                            , 1                   );
        cutflow.setCut("SRSSemMET"     , 1.                                                           , 1                   );
        cutflow.setCut("SRSSemMllSS"   , www.MllSS()>40.                                              , 1                   );
        cutflow.setCut("SRSSemFull"    , 1                                                            , 1                   );
        cutflow.setVariable("Mjj" , www.Mjj()   );
        cutflow.setVariable("Mll" , www.MllSS() );
        // Once every cut bits are set, now fill the cutflows that are booked
        cutflow.fill();
    }

    cutflow.saveOutput();
}
