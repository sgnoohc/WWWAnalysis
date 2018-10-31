#include "frtree.h"
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
    // The input files can be comma separated (e.g. "file1.root,file2.root") or with wildcard (n.b. be sure to escape)
    TChain* ch = RooUtil::FileUtil::createTChain("t", argv[1]);

    // Number of events to loop over
    int nEvents = argc > 3 ? atoi(argv[3]) : -1;

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
    cutflow.addCutToLastActiveCut("OneMuEWKCR");
    cutflow.getCut("OneMu");
    cutflow.addCutToLastActiveCut("OneMuTightMR");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneEl");
    cutflow.addCutToLastActiveCut("OneElEWKCR");
    cutflow.getCut("OneEl");
    cutflow.addCutToLastActiveCut("OneElTightMR");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneMuLoose");
    cutflow.addCutToLastActiveCut("OneMuMR");
    cutflow.getCut("Presel");
    cutflow.addCutToLastActiveCut("OneElLoose");
    cutflow.addCutToLastActiveCut("OneElMR");

    RooUtil::HistMap purewgt("../scalefactors/puw_2017.root:puw_central");
    RooUtil::HistMap onelep_trig_purewgt("../scalefactors/onelep_trig_puw_2017.root:onelep_aux_trig_purewgt");

    // Print cut structure
    cutflow.printCuts();

    // Histogram utility object that is used to define the histograms
    RooUtil::Histograms histograms;
    histograms.addHistogram("Mll"             , 180 , 60 , 120 );
    histograms.addHistogram("MT"              , 180 , 0  , 180 );
    histograms.addHistogram("MET"             , 180 , 0  , 250 );
    histograms.addHistogram("Nvtx"            , 50  , 0  , 50  );
    histograms.addHistogram("eta"             , 180 , -3 , 3   );
    histograms.addHistogram("pt"              , 180 , 0  , 250 );
    histograms.addHistogram("ptcorr"          , 180 , 0  , 250 );
    histograms.addHistogram("nj"              , 5   , 0  , 5   );
    histograms.addHistogram("nVlep"           , 4   , 0  , 4   );
    histograms.addHistogram("nLlep"           , 4   , 0  , 4   );
    histograms.addHistogram("nTlep"           , 4   , 0  , 4   );

    // The pt corr v. eta are used to parametrize the fake rates
    // The boundaries are stored in std::vector
    const std::vector<float> eta_bounds = {0.0, 1.6, 2.4};
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

        float jet_pt0 = fr.jets_p4().size() > 0 ? fr.jets_p4()[0].pt() : -999;
        float MT = (TMath::Sqrt(2*fr.met_pt()*fr.lep_pt()[0]*(1.0-TMath::Cos(fr.lep_phi()[0]-fr.met_phi()))));
        float ptcorr = fr.lep_pt()[0]*(1 + max((double) 0. , (double) fr.lep_relIso03EAv2Lep()[0]-0.03));
        bool onemu_cuts      = (fr.nVlep() == 1) * (fr.nTlep() == 1) * (abs(fr.lep_pdgId()[0])==13) * (fr.mc_HLT_SingleIsoMu17() > 0) * (jet_pt0>40.);
        bool oneel_cuts      = (fr.nVlep() == 1) * (fr.nTlep() == 1) * (abs(fr.lep_pdgId()[0])==11) * (fr.mc_HLT_SingleIsoEl23() > 0) * (jet_pt0>40.);
        bool onemuloose_cuts = (fr.nVlep() == 1) * (fr.nLlep() == 1) * (abs(fr.lep_pdgId()[0])==13) * (fr.mc_HLT_SingleIsoMu17() > 0) * (jet_pt0>40.);
        bool oneelloose_cuts = (fr.nVlep() == 1) * (fr.nLlep() == 1) * (abs(fr.lep_pdgId()[0])==11) * (fr.mc_HLT_SingleIsoEl23() > 0) * (jet_pt0>40.);

        cutflow.setCut("Presel"          , presel                                                                                       , weight                    );

        cutflow.setCut("TwoMuHLT17"      , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoMu17() > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.)                   , fr.mc_HLT_SingleIsoMu17() );
        cutflow.setCut("TwoMuHLT8"       , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoMu8()  > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.)                   , fr.mc_HLT_SingleIsoMu8()  );
        cutflow.setCut("TwoElHLT23"      , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoEl23() > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.) * (jet_pt0 > 40.) , fr.mc_HLT_SingleIsoEl23() );
        cutflow.setCut("TwoElHLT8"       , (fr.nVlep()==2) * (fr.mc_HLT_SingleIsoEl8()  > 0) * (fr.MllSS() > 60.) * (fr.MllSS() < 120.) * (jet_pt0 > 40.) , fr.mc_HLT_SingleIsoEl8()  );

        cutflow.setCut("OneMu"        , onemu_cuts                                     , fr.mc_HLT_SingleIsoMu17() );
        cutflow.setCut("OneEl"        , oneel_cuts                                     , fr.mc_HLT_SingleIsoEl23() );
        cutflow.setCut("OneMuEWKCR"   , (fr.met_pt() > 30.) * (MT > 80.) * (MT < 120.) , 1.                        );
        cutflow.setCut("OneElEWKCR"   , (fr.met_pt() > 30.) * (MT > 80.) * (MT < 120.) , 1.                        );
        cutflow.setCut("OneMuTightMR" , (fr.met_pt() < 20.) * (MT < 20.)               , 1.                        );
        cutflow.setCut("OneElTightMR" , (fr.met_pt() < 20.) * (MT < 20.)               , 1.                        );

        cutflow.setCut("OneMuLoose" , onemuloose_cuts                 , fr.mc_HLT_SingleIsoMu17() );
        cutflow.setCut("OneElLoose" , oneelloose_cuts                 , fr.mc_HLT_SingleIsoEl23() );
        cutflow.setCut("OneMuMR"    , (fr.met_pt() < 20.) * (MT < 20.), 1.                        );
        cutflow.setCut("OneElMR"    , (fr.met_pt() < 20.) * (MT < 20.), 1.                        );

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
        cutflow.setVariable("etavarbin" , min((double) fabs(fr.lep_eta()[0]), 2.3999));
        cutflow.setVariable("ptcorrvarbin" , min((double) ptcorr, 149.99));
        cutflow.setVariable("ptcorrvarbincoarse" , min((double) ptcorr, 149.99));
        cutflow.setVariable("ptcorretarolled" , RooUtil::Calc::calcBin2D(ptcorr_bounds, eta_bounds, ptcorr, fabs(fr.lep_eta()[0])));
        cutflow.setVariable("ptcorretarolledcoarse" , RooUtil::Calc::calcBin2D(ptcorrcoarse_bounds, eta_bounds, ptcorr, fabs(fr.lep_eta()[0])));

        cutflow.fill();
    }

    // Writing output file
    cutflow.saveOutput();

    // The below can be sometimes crucial
    delete ofile;
}
