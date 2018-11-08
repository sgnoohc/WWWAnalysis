#include "frtree.h"
#include "rooutil/rooutil.h"

int closureEvtType();

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
        std::cout << "  [LEPVERSION]    0: SS 1: 3L" << std::endl;
        std::cout << "  [NEVENTS=-1]    # of events to run over" << std::endl;
        std::cout << std::endl;
        return 1;
    }

    // Creating output file where we will put the outputs of the processing
    TFile* ofile = new TFile(argv[2], "recreate");

    // Create a TChain of the input files
    // The input files can be comma separated (e.g. "file1.root,file2.root") or with wildcard (n.b. be sure to escape)
    TChain* ch = RooUtil::FileUtil::createTChain("t", argv[1]);

    // Version of lepton to run
    int lepversion = argc > 3 ? atoi(argv[3]) : 0;

    // Number of events to loop over
    int nEvents = argc > 4 ? atoi(argv[4]) : -1;

    // Create a Looper object to loop over input files
    RooUtil::Looper<frtree> looper(ch, &fr, nEvents);

    // Cutflow utility object that creates a tree structure of cuts
    RooUtil::Cutflow cutflow(ofile);
    cutflow.addCut("Presel");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("TwoMuHLT8");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("TwoMuHLT17");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("TwoElHLT8");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("TwoElHLT23");

    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneMu");
    cutflow.addCutToLastActiveCut("OneMuHighMET");
    cutflow.addCutToLastActiveCut("OneMuEWKCR");
    cutflow.getCut("OneMu");
    cutflow.addCutToLastActiveCut("OneMuTightMR");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneEl");
    cutflow.addCutToLastActiveCut("OneElHighMET");
    cutflow.addCutToLastActiveCut("OneElEWKCR");
    cutflow.getCut("OneEl");
    cutflow.addCutToLastActiveCut("OneElTightMR");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneMuLoose");
    cutflow.addCutToLastActiveCut("OneMuMR");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneElLoose");
    cutflow.addCutToLastActiveCut("OneElMR");

    cutflow.getCut("OneMuEWKCR");
    cutflow.addCutToLastActiveCut("OneMuEWKCREta0Pt1");
    cutflow.getCut("OneMuEWKCR");
    cutflow.addCutToLastActiveCut("OneMuEWKCREta0Pt2");
    cutflow.getCut("OneMuEWKCR");
    cutflow.addCutToLastActiveCut("OneMuEWKCREta0Pt3");
    cutflow.getCut("OneMuEWKCR");
    cutflow.addCutToLastActiveCut("OneMuEWKCREta1Pt1");
    cutflow.getCut("OneMuEWKCR");
    cutflow.addCutToLastActiveCut("OneMuEWKCREta1Pt2");
    cutflow.getCut("OneMuEWKCR");
    cutflow.addCutToLastActiveCut("OneMuEWKCREta1Pt3");

    cutflow.getCut("OneMuMR");
    cutflow.addCutToLastActiveCut("OneMuMREta0Pt1");
    cutflow.getCut("OneMuMR");
    cutflow.addCutToLastActiveCut("OneMuMREta0Pt2");
    cutflow.getCut("OneMuMR");
    cutflow.addCutToLastActiveCut("OneMuMREta0Pt3");
    cutflow.getCut("OneMuMR");
    cutflow.addCutToLastActiveCut("OneMuMREta1Pt1");
    cutflow.getCut("OneMuMR");
    cutflow.addCutToLastActiveCut("OneMuMREta1Pt2");
    cutflow.getCut("OneMuMR");
    cutflow.addCutToLastActiveCut("OneMuMREta1Pt3");

    cutflow.getCut("OneElMR");
    cutflow.addCutToLastActiveCut("OneElMREta0Pt1");
    cutflow.getCut("OneElMR");
    cutflow.addCutToLastActiveCut("OneElMREta0Pt2");
    cutflow.getCut("OneElMR");
    cutflow.addCutToLastActiveCut("OneElMREta0Pt3");
    cutflow.getCut("OneElMR");
    cutflow.addCutToLastActiveCut("OneElMREta1Pt1");
    cutflow.getCut("OneElMR");
    cutflow.addCutToLastActiveCut("OneElMREta1Pt2");
    cutflow.getCut("OneElMR");
    cutflow.addCutToLastActiveCut("OneElMREta1Pt3");

    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("MuClosure");
    cutflow.getCut("MuClosure");
    cutflow.addCutToLastActiveCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureTight");
    cutflow.getCut("MuClosureTight");
    cutflow.addCutToLastActiveCut("MuClosureTightBVeto");
    cutflow.getCut("MuClosureTight");
    cutflow.addCutToLastActiveCut("MuClosureTightNbgeq2");
    cutflow.getCut("MuClosureTight");
    cutflow.addCutToLastActiveCut("MuClosureTightNbgeq1");
    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureTightPredict");
    cutflow.getCut("MuClosureTightPredict");
    cutflow.addCutToLastActiveCut("MuClosureTightBVetoPredict");
    cutflow.getCut("MuClosureTightPredict");
    cutflow.addCutToLastActiveCut("MuClosureTightNbgeq2Predict");
    cutflow.getCut("MuClosureTightPredict");
    cutflow.addCutToLastActiveCut("MuClosureTightNbgeq1Predict");

    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureLooseEta0Pt1");
    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureLooseEta0Pt2");
    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureLooseEta0Pt3");
    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureLooseEta1Pt1");
    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureLooseEta1Pt2");
    cutflow.getCut("MuClosureLoose");
    cutflow.addCutToLastActiveCut("MuClosureLooseEta1Pt3");

    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("ElClosure");
    cutflow.getCut("ElClosure");
    cutflow.addCutToLastActiveCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureTight");
    cutflow.getCut("ElClosureTight");
    cutflow.addCutToLastActiveCut("ElClosureTightBVeto");
    cutflow.getCut("ElClosureTight");
    cutflow.addCutToLastActiveCut("ElClosureTightNbgeq2");
    cutflow.getCut("ElClosureTight");
    cutflow.addCutToLastActiveCut("ElClosureTightNbgeq1");
    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureTightPredict");
    cutflow.getCut("ElClosureTightPredict");
    cutflow.addCutToLastActiveCut("ElClosureTightBVetoPredict");
    cutflow.getCut("ElClosureTightPredict");
    cutflow.addCutToLastActiveCut("ElClosureTightNbgeq2Predict");
    cutflow.getCut("ElClosureTightPredict");
    cutflow.addCutToLastActiveCut("ElClosureTightNbgeq1Predict");

    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureLooseEta0Pt1");
    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureLooseEta0Pt2");
    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureLooseEta0Pt3");
    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureLooseEta1Pt1");
    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureLooseEta1Pt2");
    cutflow.getCut("ElClosureLoose");
    cutflow.addCutToLastActiveCut("ElClosureLooseEta1Pt3");

    RooUtil::HistMap purewgt("histmap/puw_2017.root:puw_central");
    RooUtil::HistMap qcd_mu("histmap/fakerate.root:Mu_ptcorretarolledcoarse_qcd_fakerate");
    RooUtil::HistMap qcd_el("histmap/fakerate.root:El_ptcorretarolledcoarse_qcd_fakerate");

    // Print cut structure
    cutflow.printCuts();

    // Histogram utility object that is used to define the histograms
    RooUtil::Histograms histograms;
    histograms.addHistogram("Mll"             , 180 , 60 , 120 );
    histograms.addHistogram("MT"              , 180 , 0  , 180 );
    histograms.addHistogram("MET"             , 180 , 0  , 250 );
    histograms.addHistogram("Nvtx"            , 80  , 0  , 80  );
    histograms.addHistogram("eta"             , 180 , -3 , 3   );
    histograms.addHistogram("pt"              , 180 , 0  , 250 );
    histograms.addHistogram("ptcorr"          , 180 , 0  , 250 );
    histograms.addHistogram("nj"              , 5   , 0  , 5   );
    histograms.addHistogram("nVlep"           , 4   , 0  , 4   );
    histograms.addHistogram("nLlep"           , 4   , 0  , 4   );
    histograms.addHistogram("nTlep"           , 4   , 0  , 4   );
    histograms.addHistogram("iso"             , 180 , 0  , 0.4 );
    histograms.addHistogram("muiso"           , 180 , 0  , 0.4 );
    histograms.addHistogram("eliso"           , 180 , 0  , 0.4 );
    histograms.addHistogram("lepmotherid"     ,   7 , -4 , 3   );
    histograms.addHistogram("mumotherid"      ,   7 , -4 , 3   );
    histograms.addHistogram("elmotherid"      ,   7 , -4 , 3   );

    // The pt corr v. eta are used to parametrize the fake rates
    // The boundaries are stored in std::vector
    const std::vector<float> eta_bounds = {0.0, 1.6, 2.4};
    const std::vector<float> etafine_bounds = {0.0, 0.8, 1.6, 2.4};
    const std::vector<float> ptcorr_bounds = {0., 20., 25., 30., 35., 50., 150.};
    const std::vector<float> ptcorrcoarse_bounds = {0., 20., 25., 30., 35., 150.};

    // 1D plots to understand the behavior in general
    histograms.addHistogram("etavarbin"    , eta_bounds);
    histograms.addHistogram("ptcorrvarbin"    , ptcorr_bounds);
    histograms.addHistogram("ptcorrvarbincoarse"    , ptcorrcoarse_bounds);

    // The histogram for the 2d fake rate will be simplified to 1d histogram rolled out
    histograms.addHistogram("ptcorretarolled" , (eta_bounds.size()-1) * (ptcorr_bounds.size()-1)  , 0  , (eta_bounds.size()-1) * (ptcorr_bounds.size()-1)  );

    // The histogram that merges the last two bins
    histograms.addHistogram("ptcorretarolledcoarse" , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );
    histograms.addHistogram("muptcorretarolledcoarse" , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );
    histograms.addHistogram("elptcorretarolledcoarse" , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  , (eta_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );
    histograms.addHistogram("muptcorretarolledfineeta" , (etafine_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  , 0  , (etafine_bounds.size()-1) * (ptcorrcoarse_bounds.size()-1)  );

    // Book cutflows
    cutflow.bookCutflows();

    // Book Histograms
    cutflow.bookHistograms(histograms); // if just want to book everywhere

    // Looping input file
    while (looper.nextEvent())
    {
        // Preliminary calculations
        //float weight = fr.isData() ? 1 : fr.evt_scale1fb() * 41.3 * purewgt.eval(fr.nTrueInt());
        float weight = fr.isData() ? 1 : fr.evt_scale1fb() * 41.3;
        bool presel = fr.firstgoodvertex() == 0;
        presel &= fr.Flag_AllEventFilters() > 0;
        presel &= fr.evt_passgoodrunlist() > 0;

        const float muiso_thresh = lepversion == 0 ? 0.03 : 0.07;
        const float eliso_thresh = lepversion == 0 ? 0.03 : 0.05;

        float jet_pt0 = fr.jets_p4().size() > 0 ? fr.jets_p4()[0].pt() : -999;
        float MT = (TMath::Sqrt(2*fr.met_pt()*fr.lep_pt()[0]*(1.0-TMath::Cos(fr.lep_phi()[0]-fr.met_phi()))));
        int muidx = abs(fr.lep_pdgId()[0]) == 13 ? 0 : 1;
        int elidx = abs(fr.lep_pdgId()[0]) == 11 ? 0 : 1;
        float muptcorr = fr.lep_pt()[muidx]*(1 + max((double) 0. , (double) fr.lep_relIso03EAv2Lep()[muidx]-muiso_thresh));
        float elptcorr = fr.lep_pt()[elidx]*(1 + max((double) 0. , (double) fr.lep_relIso03EAv2Lep()[elidx]-eliso_thresh));
        float ptcorr = abs(fr.lep_pdgId()[0]) == 13 ? muptcorr : elptcorr;
        bool onemu_cuts      = (fr.nVlep() == 1) * (fr.lep_pt()[0] > 25.) * (fr.lep_pass_VVV_cutbased_tight()[0] == 1) * (abs(fr.lep_pdgId()[0])==13) * (fr.mc_HLT_SingleIsoMu17() > 0) * (jet_pt0>40.);
        bool oneel_cuts      = (fr.nVlep() == 1) * (fr.lep_pt()[0] > 25.) * (fr.lep_pass_VVV_cutbased_tight()[0] == 1) * (abs(fr.lep_pdgId()[0])==11) * (fr.mc_HLT_SingleIsoEl23() > 0) * (jet_pt0>40.);
        bool onemuloose_cuts = (fr.nVlep() == 1) * (fr.lep_pt()[0] > 25.) * (fr.lep_pass_VVV_cutbased_fo()[0] == 1) * (abs(fr.lep_pdgId()[0])==13) * (fr.mc_HLT_SingleIsoMu17() > 0) * (jet_pt0>40.);
        bool oneelloose_cuts = (fr.nVlep() == 1) * (fr.lep_pt()[0] > 25.) * (fr.lep_pass_VVV_cutbased_fo()[0] == 1) * (abs(fr.lep_pdgId()[0])==11) * (fr.mc_HLT_SingleIsoEl23() > 0) * (jet_pt0>40.);

        cutflow.setCut("Presel"          , presel                                                                                       , weight                    );

        cutflow.setCut("TwoMuHLT17"      , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoMu17() > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.)                   , fr.mc_HLT_SingleIsoMu17() );
        cutflow.setCut("TwoMuHLT8"       , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoMu8()  > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.)                   , fr.mc_HLT_SingleIsoMu8()  );
        cutflow.setCut("TwoElHLT23"      , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoEl23() > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.) * (jet_pt0 > 40.) , fr.mc_HLT_SingleIsoEl23() );
        cutflow.setCut("TwoElHLT8"       , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoEl8()  > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.) * (jet_pt0 > 40.) , fr.mc_HLT_SingleIsoEl8()  );

        cutflow.setCut("OneMu"        , onemu_cuts                       , fr.mc_HLT_SingleIsoMu17() );
        cutflow.setCut("OneEl"        , oneel_cuts                       , fr.mc_HLT_SingleIsoEl23() );
        cutflow.setCut("OneMuHighMET" , (fr.met_pt() > 30.)              , 1.                        );
        cutflow.setCut("OneMuEWKCR"   , (MT > 80.) * (MT < 120.)         , 1.                        );
        cutflow.setCut("OneElHighMET" , (fr.met_pt() > 30.)              , 1.                        );
        cutflow.setCut("OneElEWKCR"   , (MT > 80.) * (MT < 120.)         , 1.                        );
        cutflow.setCut("OneMuTightMR" , (fr.met_pt() < 20.) * (MT < 20.) , 1.                        );
        cutflow.setCut("OneElTightMR" , (fr.met_pt() < 20.) * (MT < 20.) , 1.                        );

        cutflow.setCut("OneMuEWKCREta0Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 2, 1.);
        cutflow.setCut("OneMuEWKCREta0Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 3, 1.);
        cutflow.setCut("OneMuEWKCREta0Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 4, 1.);
        cutflow.setCut("OneMuEWKCREta1Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 7, 1.);
        cutflow.setCut("OneMuEWKCREta1Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 8, 1.);
        cutflow.setCut("OneMuEWKCREta1Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 9, 1.);

        cutflow.setCut("OneMuLoose" , onemuloose_cuts  , fr.mc_HLT_SingleIsoMu17() );
        cutflow.setCut("OneElLoose" , oneelloose_cuts  , fr.mc_HLT_SingleIsoEl23() );
        cutflow.setCut("OneMuMR"    , (fr.met_pt() < 20.) * (MT < 20.) , 1. );
        cutflow.setCut("OneElMR"    , (fr.met_pt() < 20.) * (MT < 20.) , 1. );

        cutflow.setCut("OneMuMREta0Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 2, 1.);
        cutflow.setCut("OneMuMREta0Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 3, 1.);
        cutflow.setCut("OneMuMREta0Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 4, 1.);
        cutflow.setCut("OneMuMREta1Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 7, 1.);
        cutflow.setCut("OneMuMREta1Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 8, 1.);
        cutflow.setCut("OneMuMREta1Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[0])) == 9, 1.);

        cutflow.setCut("OneElMREta0Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[0])) == 2, 1.);
        cutflow.setCut("OneElMREta0Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[0])) == 3, 1.);
        cutflow.setCut("OneElMREta0Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[0])) == 4, 1.);
        cutflow.setCut("OneElMREta1Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[0])) == 7, 1.);
        cutflow.setCut("OneElMREta1Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[0])) == 8, 1.);
        cutflow.setCut("OneElMREta1Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[0])) == 9, 1.);

        cutflow.setCut("MuClosureLooseEta0Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])) == 2, 1.);
        cutflow.setCut("MuClosureLooseEta0Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])) == 3, 1.);
        cutflow.setCut("MuClosureLooseEta0Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])) == 4, 1.);
        cutflow.setCut("MuClosureLooseEta1Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])) == 7, 1.);
        cutflow.setCut("MuClosureLooseEta1Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])) == 8, 1.);
        cutflow.setCut("MuClosureLooseEta1Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])) == 9, 1.);

        cutflow.setCut("ElClosureLooseEta0Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])) == 2, 1.);
        cutflow.setCut("ElClosureLooseEta0Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])) == 3, 1.);
        cutflow.setCut("ElClosureLooseEta0Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])) == 4, 1.);
        cutflow.setCut("ElClosureLooseEta1Pt1", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])) == 7, 1.);
        cutflow.setCut("ElClosureLooseEta1Pt2", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])) == 8, 1.);
        cutflow.setCut("ElClosureLooseEta1Pt3", RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])) == 9, 1.);

        cutflow.setCut("MuClosure" , (closureEvtType() == 0) * (fr.nj() >= 2) * (fr.nVlep() == 2) * (fr.lep_pt()[0] > 25.) * (fr.lep_pt()[1] > 25.) , 1. );
        cutflow.setCut("ElClosure" , (closureEvtType() == 1) * (fr.nj() >= 2) * (fr.nVlep() == 2) * (fr.lep_pt()[0] > 25.) * (fr.lep_pt()[1] > 25.) , 1. );

        cutflow.setCut("MuClosureLoose"  , fr.lep_pass_VVV_cutbased_fo()[muidx] == 1, 1. );
        cutflow.setCut("MuClosureTight"  , fr.lep_pass_VVV_cutbased_tight()[muidx] == 1, 1. );
        cutflow.setCut("MuClosureTightBVeto"  , fr.nb() == 0, 1. );
        cutflow.setCut("MuClosureTightNbgeq2"  , fr.nb() >= 2, 1. );
        cutflow.setCut("MuClosureTightNbgeq1"  , fr.nb() >= 1, 1. );
        cutflow.setCut("MuClosureTightPredict"  , 1, qcd_mu.eval(muptcorr, fabs(fr.lep_eta()[muidx])) );
        cutflow.setCut("MuClosureTightBVetoPredict"  , fr.nb() == 0, 1);
        cutflow.setCut("MuClosureTightNbgeq2Predict"  , fr.nb() >= 2, 1);
        cutflow.setCut("MuClosureTightNbgeq1Predict"  , fr.nb() >= 1, 1);

        cutflow.setCut("ElClosureLoose"  , fr.lep_pass_VVV_cutbased_fo()[elidx] == 1, 1. );
        cutflow.setCut("ElClosureTight"  , fr.lep_pass_VVV_cutbased_tight()[elidx] == 1, 1. );
        cutflow.setCut("ElClosureTightBVeto"  , fr.nb() == 0, 1. );
        cutflow.setCut("ElClosureTightNbgeq2"  , fr.nb() >= 2, 1. );
        cutflow.setCut("ElClosureTightNbgeq1"  , fr.nb() >= 1, 1. );
        cutflow.setCut("ElClosureTightPredict"  , 1, qcd_el.eval(elptcorr, fabs(fr.lep_eta()[elidx])) );
        cutflow.setCut("ElClosureTightBVetoPredict"  , fr.nb() == 0, 1);
        cutflow.setCut("ElClosureTightNbgeq2Predict"  , fr.nb() >= 2, 1);
        cutflow.setCut("ElClosureTightNbgeq1Predict"  , fr.nb() >= 1, 1);

        cutflow.setVariable("Mll"    , fr.MllSS());
        cutflow.setVariable("MT"     , MT);
        cutflow.setVariable("MET"    , fr.met_pt());
        cutflow.setVariable("Nvtx"   , fr.nVert());
        cutflow.setVariable("eta"    , fr.lep_eta()[0]);
        cutflow.setVariable("pt"     , fr.lep_pt()[0]);
        cutflow.setVariable("ptcorr" , ptcorr);
        cutflow.setVariable("nj"     , fr.nj());
        cutflow.setVariable("nVlep"  , fr.nVlep());
        cutflow.setVariable("nLlep"  , fr.nLlep());
        cutflow.setVariable("nTlep"  , fr.nTlep());
        cutflow.setVariable("iso"    , fr.lep_relIso03EAv2Lep()[0]);
        cutflow.setVariable("muiso"  , fr.lep_relIso03EAv2Lep()[muidx]);
        cutflow.setVariable("eliso"  , fr.lep_relIso03EAv2Lep()[elidx]);
        cutflow.setVariable("etavarbin" , min((double) fabs(fr.lep_eta()[0]), 2.3999));
        cutflow.setVariable("ptcorrvarbin" , min((double) ptcorr, 149.99));
        cutflow.setVariable("ptcorrvarbincoarse" , min((double) ptcorr, 149.99));
        cutflow.setVariable("ptcorretarolled" , RooUtil::Calc::calcBin2D(ptcorr_bounds, eta_bounds, ptcorr, fabs(fr.lep_eta()[0])));
        cutflow.setVariable("ptcorretarolledcoarse" , RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, ptcorr, fabs(fr.lep_eta()[0])));

        cutflow.setVariable("muptcorretarolledcoarse" , RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, muptcorr, fabs(fr.lep_eta()[muidx])));
        cutflow.setVariable("elptcorretarolledcoarse" , RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, elptcorr, fabs(fr.lep_eta()[elidx])));
        cutflow.setVariable("muptcorretarolledfineeta" , RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, etafine_bounds, muptcorr, fabs(fr.lep_eta()[muidx])));

        cutflow.setVariable("lepmotherid" , fr.lep_motherIdSS()[0]);
        cutflow.setVariable("mumotherid"  , fr.lep_motherIdSS()[muidx]);
        cutflow.setVariable("elmotherid"  , fr.lep_motherIdSS()[elidx]);

        cutflow.fill();
    }

    // Writing output file
    cutflow.saveOutput();

    // The below can be sometimes crucial
    delete ofile;
}

int closureEvtType()
{
    const std::vector<int>& genPart_pdgId = fr.genPart_pdgId();
    const std::vector<int>& genPart_motherId = fr.genPart_motherId();
    const std::vector<int>& lep_pdgId = fr.lep_pdgId();
    const std::vector<int>& lep_pass_VVV_cutbased_tight = fr.lep_pass_VVV_cutbased_tight();
    int evt_type = -1;
    int ngenlepW = 0;
//    DEBUGclass("genPart_pdgId->size() == %d", genPart_pdgId->size());
    for (unsigned int igen = 0; igen < genPart_pdgId.size(); ++igen)
    {
//        DEBUGclass("genPart_pdgId = %d, genPart_motherId = %d", genPart_pdgId->at(igen), genPart_motherId->at(igen));
        if (abs(genPart_pdgId.at(igen)) == 11 && abs(genPart_motherId.at(igen)) == 24)
        {
            evt_type = 0;
            ngenlepW++;
        }
        if (abs(genPart_pdgId.at(igen)) == 13 && abs(genPart_motherId.at(igen)) == 24)
        {
            evt_type = 1;
            ngenlepW++;
        }
        if (abs(genPart_pdgId.at(igen)) == 15 && abs(genPart_motherId.at(igen)) == 24)
        {
            evt_type = -1;
            ngenlepW++;
        }
    }
    if (ngenlepW == 0)
        evt_type = -2;
    else if (ngenlepW > 1)
        evt_type = -3;
    else if (ngenlepW != 1)
        evt_type = -4;

    if (
            (lep_pdgId.size() == 2) and
            (abs(lep_pdgId[0]*lep_pdgId[1]) == 143) and
            ((abs(lep_pdgId[0]) == 11 and lep_pass_VVV_cutbased_tight[0] == 1) or (abs(lep_pdgId[1]) == 11 and lep_pass_VVV_cutbased_tight[1] == 1)) and
            (evt_type == 0)
       )
        return 0;
    else if (
            (lep_pdgId.size() == 2) and
            (abs(lep_pdgId[0]*lep_pdgId[1]) == 143) and
            ((abs(lep_pdgId[0]) == 13 and lep_pass_VVV_cutbased_tight[0] == 1) or (abs(lep_pdgId[1]) == 13 and lep_pass_VVV_cutbased_tight[1] == 1)) and
            (evt_type == 1)
       )
        return 1;
    else
        return -1;
}
