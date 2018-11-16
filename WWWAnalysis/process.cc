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
    bool doFakeEstimation = TString(output_file_name).Contains("ddfakes") or TString(output_file_name).Contains("ewksubt");
    bool doEwkSubtraction = TString(output_file_name).Contains("ewksubt");
    bool isData = TString(input_paths).Contains("data_");

    // Cutflow utility object that creates a tree structure of cuts
    RooUtil::Cutflow cutflow(ofile);

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
    histograms.addHistogram("ptcorretarolledcoarse"    ,     (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  ,     (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );
    histograms.addHistogram("ptcorretarolledcoarseemu" , 2 * (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  , 2 * (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );

    // Scale factors
    LeptonScaleFactors leptonScaleFactors;
    FakeRates fakerates;
    TheoryWeight theoryweight;

    // Luminosity setting
    float lumi = isData ? 1 : (is2017 == 1 ? 41.3 : 35.9);

    // variables
    float ffwgt;
    bool presel;
    bool trigger;
    float weight;
    bool isdilep;
    bool istrilep;
    bool isfakedilep;
    bool isfaketrilep;
    bool iswzcrtrilep;
    bool isfakewzcrtrilep;
    float ee_sf, em_sf, mm_sf, threelep_sf;
    float btag_sf;
    float trig_sf;
    bool hasz_ss, hasz_3l;

#ifdef USE_CUTLAMBDA
#include "setcuttree.h"
#endif

    // Now book cutflows
    cutflow.bookCutflows();

    // Now book histograms at the end of each cut structures (the CutTree nodes that terminates)
    cutflow.bookHistogramsForEndCuts(histograms);

    // Print the cut structure for review
    cutflow.printCuts();

    //
    //
    // Looping events
    //
    //
    while (looper.nextEvent())
    {

        // Fake factor weights
        ffwgt = 1;
        if (doFakeEstimation)
        {
            ffwgt = is2017 == 1 ? fakerates.getFakeFactor() : www.ffwgt();
            if (doEwkSubtraction && !www.bkgtype().EqualTo("fakes")) ffwgt *= -1; // subtracting non-fakes
            if (doEwkSubtraction &&  www.bkgtype().EqualTo("fakes")) ffwgt *=  0; // do not subtract fakes
        }

        // Compute preselection
        presel = 1;
        presel &= (www.firstgoodvertex()      == 0);
        presel &= (www.Flag_AllEventFilters() >  0);
        presel &= (www.vetophoton()           == 0);
        presel &= (www.evt_passgoodrunlist()  >  0);

        // Compute trigger variable (TODO for 2016 baby, the tertiary statement may be outdated)
        trigger = is2017 == 1 ? www.passTrigger() * www.pass_duplicate_ee_em_mm() : passTrigger2016();

        // Event weight
        weight = (isData and !doFakeEstimation) ? 1 : www.evt_scale1fb() * www.purewgt() * lumi * ffwgt;
        if (isWWW and !is2017) weight *= 1.0384615385; // NLO cross section v. MadGraph cross section

        // Lepton counter to define dilep or trilep region
        isdilep          = (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 2);
        istrilep         = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 3) * (www.lep_pt()[0]>25.);
        isfakedilep      = (www.nVlep() == 2) * (www.nLlep() == 2) * (www.nTlep() == 1) * (www.lep_pt()[0]>25.) * (www.lep_pt()[1]>25.);
        isfaketrilep     = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 2);
        iswzcrtrilep     = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 3);
        isfakewzcrtrilep = (www.nVlep() == 3) * (www.nLlep() == 3) * (www.nTlep() == 2);

        // Compute the scale factors
        std::tie(ee_sf, em_sf, mm_sf, threelep_sf) = leptonScaleFactors.getScaleFactors(is2017, doFakeEstimation, isData);
        btag_sf = isData ? 1 : www.weight_btagsf();
        trig_sf = isData ? 1 : www.trigsf();

        // Theory related weights from h_neventsinfile in each input root file but only set files when new file opens
        // NOTE if there was a continue statement prior to this it can mess it up
        if (looper.isNewFileInChain()) theoryweight.setFile(looper.getCurrentFileName());

#ifndef USE_CUTLAMBDA
#include "setcut.h"
#endif

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
        int ibin = RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, ptcorr, abseta);
        const int nbin = (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1);
        cutflow.setVariable("ptcorretarolledcoarse"    , ibin);
        cutflow.setVariable("ptcorretarolledcoarseemu" , ibin + nbin * (abs(www.lep_pdgId()[index]) == 11 ? 1 : 0));

        // The yield variable is set to the bin number depending on the signal region channel
        // This is to create the yield money plot
        cutflow.setVariable("yield", yield());
        cutflow.setVariable("wzcryield", wzcryield());

        // Set the event list variables
        cutflow.setEventID(www.run(), www.lumi(), www.evt());

        // Systematic variations
        if (doSystematics)
        {
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
            cutflow.setWgtSyst("PDFUp"      , www.weight_pdf_up()          / www.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.pdfup() );
            cutflow.setWgtSyst("PDFDown"    , www.weight_pdf_down()        / www.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.pdfdn() );
            cutflow.setWgtSyst("QsqUp"      , www.weight_fr_r2_f2()        / www.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.qsqup() );
            cutflow.setWgtSyst("QsqDown"    , www.weight_fr_r0p5_f0p5()    / www.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.qsqdn() );
            cutflow.setWgtSyst("AlphaSUp"   , www.weight_alphas_up()       / www.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.alsup() );
            cutflow.setWgtSyst("AlphaSDown" , www.weight_alphas_down()     / www.weight_fr_r1_f1() * theoryweight.nominal() / theoryweight.alsdn() );

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

