#ifndef process_h
#define process_h

#include "wwwtree.h"
#include "rooutil/rooutil.h"

//_______________________________________________________________________________________________________
bool passTrigger2016()
{
    if (www.nLlep() < 2)
        return false;

    const vector<int>& lep_pdgId = www.lep_pdgId();
    const int mc_HLT_DoubleEl    = www.mc_HLT_DoubleEl();
    const int mc_HLT_DoubleEl_DZ = www.mc_HLT_DoubleEl_DZ();
    const int mc_HLT_MuEG        = www.mc_HLT_MuEG();
    const int mc_HLT_DoubleMu    = www.mc_HLT_DoubleMu();
    const int nVlep              = www.nVlep();
    const int nLlep              = www.nLlep();


    if (nVlep != 2 && nVlep != 3)
        return 0;

    if (nLlep != 2 && nLlep != 3)
        return 0;

    if (lep_pdgId.size() < 2)
        return 0;

    if (nVlep == 2 && nLlep == 2)
    {
        int lepprod = lep_pdgId.at(0)*lep_pdgId.at(1);
        if (abs(lepprod) == 121)
            return (mc_HLT_DoubleEl || mc_HLT_DoubleEl_DZ);
        else if (abs(lepprod) == 143)
            return mc_HLT_MuEG;
        else if (abs(lepprod) == 169)
            return mc_HLT_DoubleMu;
        else
            return 0;
    }
    else if (nVlep == 3 && nLlep == 3)
    {
        int lepprod01 = lep_pdgId.at(0)*lep_pdgId.at(1);
        if (abs(lepprod01) == 121 && (mc_HLT_DoubleEl || mc_HLT_DoubleEl_DZ))
            return true;
        else if (abs(lepprod01) == 143 && mc_HLT_MuEG)
            return true;
        else if (abs(lepprod01) == 169 && mc_HLT_DoubleMu)
            return true;

        int lepprod02 = lep_pdgId.at(0)*lep_pdgId.at(2);
        if (abs(lepprod02) == 121 && (mc_HLT_DoubleEl || mc_HLT_DoubleEl_DZ))
            return true;
        else if (abs(lepprod02) == 143 && mc_HLT_MuEG)
            return true;
        else if (abs(lepprod02) == 169 && mc_HLT_DoubleMu)
            return true;

        int lepprod12 = lep_pdgId.at(1)*lep_pdgId.at(2);
        if (abs(lepprod12) == 121 && (mc_HLT_DoubleEl || mc_HLT_DoubleEl_DZ))
            return true;
        else if (abs(lepprod12) == 143 && mc_HLT_MuEG)
            return true;
        else if (abs(lepprod12) == 169 && mc_HLT_DoubleMu)
            return true;

        return false;
    }
    else
    {
        return 0;
    }
}

//_______________________________________________________________________________________________________
int yield()
{
    int yield = -1;
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
    return yield;
}

//_______________________________________________________________________________________________________
int wzcryield()
{
    int yield = -1;
    if (www.nj30() >= 2)
    {
        if (www.passSSee()) yield = 0;
        if (www.passSSem()) yield = 1;
        if (www.passSSmm()) yield = 2;
    }
    else if (www.nj() <= 1)
    {
        if (www.nSFOS() == 1) yield = 4;
        if (www.nSFOS() == 2) yield = 5;
    }
    return yield;
}

//_______________________________________________________________________________________________________
class LeptonScaleFactors
{
    public:
        RooUtil::HistMap* histmap_lead_mu_recoid_sf;
        RooUtil::HistMap* histmap_subl_mu_recoid_sf;
        RooUtil::HistMap* histmap_lead_el_recoid_sf;
        RooUtil::HistMap* histmap_subl_el_recoid_sf;
        RooUtil::HistMap* histmap_lead_el_mva_sf;
        RooUtil::HistMap* histmap_subl_el_mva_sf;
        RooUtil::HistMap* histmap_emu_mu_recoid_sf;
        RooUtil::HistMap* histmap_emu_el_recoid_sf;
        RooUtil::HistMap* histmap_emu_el_mva_sf;
        RooUtil::HistMap* histmap_lead_mu_recoid_3l_sf;
        RooUtil::HistMap* histmap_subl_mu_recoid_3l_sf;
        RooUtil::HistMap* histmap_lead_el_recoid_3l_sf;
        RooUtil::HistMap* histmap_subl_el_recoid_3l_sf;
        RooUtil::HistMap* histmap_lead_el_mva_3l_sf;
        RooUtil::HistMap* histmap_subl_el_mva_3l_sf;
        RooUtil::HistMap* histmap_tert_mu_recoid_3l_sf;
        RooUtil::HistMap* histmap_tert_el_recoid_3l_sf;
        RooUtil::HistMap* histmap_tert_el_mva_3l_sf;
        RooUtil::HistMap* histmap_lead_mu_isoip_sf;
        RooUtil::HistMap* histmap_subl_mu_isoip_sf;
        RooUtil::HistMap* histmap_lead_el_isoip_sf;
        RooUtil::HistMap* histmap_subl_el_isoip_sf;
        RooUtil::HistMap* histmap_emu_mu_isoip_sf;
        RooUtil::HistMap* histmap_emu_el_isoip_sf;
        RooUtil::HistMap* histmap_lead_mu_isoip_3l_sf;
        RooUtil::HistMap* histmap_subl_mu_isoip_3l_sf;
        RooUtil::HistMap* histmap_lead_el_isoip_3l_sf;
        RooUtil::HistMap* histmap_subl_el_isoip_3l_sf;
        RooUtil::HistMap* histmap_tert_mu_isoip_3l_sf;
        RooUtil::HistMap* histmap_tert_el_isoip_3l_sf;

        LeptonScaleFactors()
        {
            histmap_lead_mu_recoid_sf     = new RooUtil::HistMap("scalefactors/RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta");
            histmap_subl_mu_recoid_sf     = new RooUtil::HistMap("scalefactors/RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta");
            histmap_lead_el_recoid_sf     = new RooUtil::HistMap("scalefactors/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D");
            histmap_subl_el_recoid_sf     = new RooUtil::HistMap("scalefactors/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D");
            histmap_lead_el_mva_sf        = new RooUtil::HistMap("scalefactors/gammaEffi.txt_EGM2D_runBCDEF_passingMVA94Xwp80noiso.root:EGamma_SF2D");
            histmap_subl_el_mva_sf        = new RooUtil::HistMap("scalefactors/gammaEffi.txt_EGM2D_runBCDEF_passingMVA94Xwp80noiso.root:EGamma_SF2D");
            histmap_emu_mu_recoid_sf      = new RooUtil::HistMap("scalefactors/RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta");
            histmap_emu_el_recoid_sf      = new RooUtil::HistMap("scalefactors/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D");
            histmap_emu_el_mva_sf         = new RooUtil::HistMap("scalefactors/gammaEffi.txt_EGM2D_runBCDEF_passingMVA94Xwp80noiso.root:EGamma_SF2D");
            histmap_lead_mu_recoid_3l_sf  = new RooUtil::HistMap("scalefactors/RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta");
            histmap_subl_mu_recoid_3l_sf  = new RooUtil::HistMap("scalefactors/RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta");
            histmap_lead_el_recoid_3l_sf  = new RooUtil::HistMap("scalefactors/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D");
            histmap_subl_el_recoid_3l_sf  = new RooUtil::HistMap("scalefactors/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D");
            histmap_lead_el_mva_3l_sf     = new RooUtil::HistMap("scalefactors/gammaEffi.txt_EGM2D_runBCDEF_passingMVA94Xwp90noiso.root:EGamma_SF2D");
            histmap_subl_el_mva_3l_sf     = new RooUtil::HistMap("scalefactors/gammaEffi.txt_EGM2D_runBCDEF_passingMVA94Xwp90noiso.root:EGamma_SF2D");
            histmap_tert_mu_recoid_3l_sf  = new RooUtil::HistMap("scalefactors/RunBCDEF_SF_ID.root:NUM_MediumID_DEN_genTracks_pt_abseta");
            histmap_tert_el_recoid_3l_sf  = new RooUtil::HistMap("scalefactors/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root:EGamma_SF2D");
            histmap_tert_el_mva_3l_sf     = new RooUtil::HistMap("scalefactors/gammaEffi.txt_EGM2D_runBCDEF_passingMVA94Xwp90noiso.root:EGamma_SF2D");
            histmap_lead_mu_isoip_sf      = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/MuonID_2017www/muon/MuMediumPOG_MuTightVVV/sf.root:h_sf_pt_vs_eta");
            histmap_subl_mu_isoip_sf      = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/MuonID_2017www/muon/MuMediumPOG_MuTightVVV/sf.root:h_sf_pt_vs_eta");
            histmap_lead_el_isoip_sf      = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/ElectronID_2017www/electron/EGammaMVA80POG2017_EGammaTightVVV/sf.root:h_sf_pt_vs_eta");
            histmap_subl_el_isoip_sf      = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/ElectronID_2017www/electron/EGammaMVA80POG2017_EGammaTightVVV/sf.root:h_sf_pt_vs_eta");
            histmap_emu_mu_isoip_sf       = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/MuonID_2017www/muon/MuMediumPOG_MuTightVVV/sf.root:h_sf_pt_vs_eta");
            histmap_emu_el_isoip_sf       = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/ElectronID_2017www/electron/EGammaMVA80POG2017_EGammaTightVVV/sf.root:h_sf_pt_vs_eta");
            histmap_lead_mu_isoip_3l_sf   = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/MuonID_2017www/muon/MuMediumPOG_MuTightVVV3l/sf.root:h_sf_pt_vs_eta");
            histmap_subl_mu_isoip_3l_sf   = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/MuonID_2017www/muon/MuMediumPOG_MuTightVVV3l/sf.root:h_sf_pt_vs_eta");
            histmap_lead_el_isoip_3l_sf   = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/ElectronID_2017www/electron/EGammaMVA90POG2017_EGammaTightVVV3l/sf.root:h_sf_pt_vs_eta");
            histmap_subl_el_isoip_3l_sf   = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/ElectronID_2017www/electron/EGammaMVA90POG2017_EGammaTightVVV3l/sf.root:h_sf_pt_vs_eta");
            histmap_tert_mu_isoip_3l_sf   = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/MuonID_2017www/muon/MuMediumPOG_MuTightVVV3l/sf.root:h_sf_pt_vs_eta");
            histmap_tert_el_isoip_3l_sf   = new RooUtil::HistMap("/home/users/phchang/public_html/analysis/tnp/CMSSW_7_4_2/src/TagAndProbe/Analysis/plots/ElectronID_2017www/electron/EGammaMVA90POG2017_EGammaTightVVV3l/sf.root:h_sf_pt_vs_eta");
        }

        ~LeptonScaleFactors()
        {
            delete histmap_lead_mu_recoid_sf;
            delete histmap_subl_mu_recoid_sf;
            delete histmap_lead_el_recoid_sf;
            delete histmap_subl_el_recoid_sf;
            delete histmap_lead_el_mva_sf;
            delete histmap_subl_el_mva_sf;
            delete histmap_emu_mu_recoid_sf;
            delete histmap_emu_el_recoid_sf;
            delete histmap_emu_el_mva_sf;
            delete histmap_lead_mu_recoid_3l_sf;
            delete histmap_subl_mu_recoid_3l_sf;
            delete histmap_lead_el_recoid_3l_sf;
            delete histmap_subl_el_recoid_3l_sf;
            delete histmap_lead_el_mva_3l_sf;
            delete histmap_subl_el_mva_3l_sf;
            delete histmap_tert_mu_recoid_3l_sf;
            delete histmap_tert_el_recoid_3l_sf;
            delete histmap_tert_el_mva_3l_sf;
            delete histmap_lead_mu_isoip_sf;
            delete histmap_subl_mu_isoip_sf;
            delete histmap_lead_el_isoip_sf;
            delete histmap_subl_el_isoip_sf;
            delete histmap_emu_mu_isoip_sf;
            delete histmap_emu_el_isoip_sf;
            delete histmap_lead_mu_isoip_3l_sf;
            delete histmap_subl_mu_isoip_3l_sf;
            delete histmap_lead_el_isoip_3l_sf;
            delete histmap_subl_el_isoip_3l_sf;
            delete histmap_tert_mu_isoip_3l_sf;
            delete histmap_tert_el_isoip_3l_sf;
        }

        std::tuple<float, float, float, float> getScaleSactors(bool is2017, bool doFakeEstimation)
        {
            if (is2017)
            {
                const double b500 = 499.9;
                const double b120 = 119.9;
                float lead_mu_recoid_sf    = histmap_lead_mu_recoid_sf    -> eval(min((double)www.lep_pt()[0],b120)   ,abs((double)www.lep_eta()[0])     ); 
                float subl_mu_recoid_sf    = histmap_subl_mu_recoid_sf    -> eval(min((double)www.lep_pt()[1],b120)   ,abs((double)www.lep_eta()[1])     ); 
                float lead_el_recoid_sf    = histmap_lead_el_recoid_sf    -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b500) ); 
                float subl_el_recoid_sf    = histmap_subl_el_recoid_sf    -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b500) ); 
                float lead_el_mva_sf       = histmap_lead_el_mva_sf       -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b500) ); 
                float subl_el_mva_sf       = histmap_subl_el_mva_sf       -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b500) ); 
                float emu_mu_recoid_sf     = histmap_emu_mu_recoid_sf     -> eval(min((double)www.mu_pt(),b120)       ,abs((double)www.mu_eta())         ); 
                float emu_el_recoid_sf     = histmap_emu_el_recoid_sf     -> eval(abs((double)www.el_eta())           ,min((double)www.el_pt(),b500)     ); 
                float emu_el_mva_sf        = histmap_emu_el_mva_sf        -> eval(abs((double)www.el_eta())           ,min((double)www.el_pt(),b500)     ); 
                float lead_mu_recoid_3l_sf = histmap_lead_mu_recoid_3l_sf -> eval(min((double)www.lep_pt()[0],b120)   ,abs((double)www.lep_eta()[0])     ); 
                float subl_mu_recoid_3l_sf = histmap_subl_mu_recoid_3l_sf -> eval(min((double)www.lep_pt()[1],b120)   ,abs((double)www.lep_eta()[1])     ); 
                float lead_el_recoid_3l_sf = histmap_lead_el_recoid_3l_sf -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b500) ); 
                float subl_el_recoid_3l_sf = histmap_subl_el_recoid_3l_sf -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b500) ); 
                float lead_el_mva_3l_sf    = histmap_lead_el_mva_3l_sf    -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b500) ); 
                float subl_el_mva_3l_sf    = histmap_subl_el_mva_3l_sf    -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b500) ); 
                float tert_mu_recoid_3l_sf = histmap_tert_mu_recoid_3l_sf -> eval(min((double)www.lep_pt()[2],b120)   ,abs((double)www.lep_eta()[2])     ); 
                float tert_el_recoid_3l_sf = histmap_tert_el_recoid_3l_sf -> eval(abs((double)www.lep_eta()[2])       ,min((double)www.lep_pt()[2],b500) ); 
                float tert_el_mva_3l_sf    = histmap_tert_el_mva_3l_sf    -> eval(abs((double)www.lep_eta()[2])       ,min((double)www.lep_pt()[2],b500) ); 
                float lead_mu_isoip_sf     = histmap_lead_mu_isoip_sf     -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b120) ); 
                float subl_mu_isoip_sf     = histmap_subl_mu_isoip_sf     -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b120) ); 
                float lead_el_isoip_sf     = histmap_lead_el_isoip_sf     -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b500) ); 
                float subl_el_isoip_sf     = histmap_subl_el_isoip_sf     -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b500) ); 
                float emu_mu_isoip_sf      = histmap_emu_mu_isoip_sf      -> eval(abs((double)www.mu_eta())           ,min((double)www.mu_pt(),199.99)   ); 
                float emu_el_isoip_sf      = histmap_emu_el_isoip_sf      -> eval(abs((double)www.el_eta())           ,min((double)www.el_pt(),b500)     ); 
                float lead_mu_isoip_3l_sf  = histmap_lead_mu_isoip_3l_sf  -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b120) ); 
                float subl_mu_isoip_3l_sf  = histmap_subl_mu_isoip_3l_sf  -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b120) ); 
                float lead_el_isoip_3l_sf  = histmap_lead_el_isoip_3l_sf  -> eval(abs((double)www.lep_eta()[0])       ,min((double)www.lep_pt()[0],b500) ); 
                float subl_el_isoip_3l_sf  = histmap_subl_el_isoip_3l_sf  -> eval(abs((double)www.lep_eta()[1])       ,min((double)www.lep_pt()[1],b500) ); 
                float tert_mu_isoip_3l_sf  = histmap_tert_mu_isoip_3l_sf  -> eval(abs((double)www.lep_eta()[2])       ,min((double)www.lep_pt()[2],b120) ); 
                float tert_el_isoip_3l_sf  = histmap_tert_el_isoip_3l_sf  -> eval(abs((double)www.lep_eta()[2])       ,min((double)www.lep_pt()[2],b500) ); 

                // Scale factors
                float ee_sf = lead_el_recoid_sf * lead_el_mva_sf * lead_el_isoip_sf * subl_el_recoid_sf * subl_el_mva_sf * subl_el_isoip_sf;
                float em_sf = emu_mu_recoid_sf * emu_mu_isoip_sf * emu_el_recoid_sf * emu_el_mva_sf * emu_el_isoip_sf;
                float mm_sf = lead_mu_recoid_sf * lead_mu_isoip_sf * subl_mu_recoid_sf * subl_mu_isoip_sf;
                float lead_el = lead_el_recoid_3l_sf * lead_el_mva_3l_sf * lead_el_isoip_3l_sf;
                float subl_el = subl_el_recoid_3l_sf * subl_el_mva_3l_sf * subl_el_isoip_3l_sf;
                float tert_el = tert_el_recoid_3l_sf * tert_el_mva_3l_sf * tert_el_isoip_3l_sf;
                float lead_mu = lead_mu_recoid_3l_sf * lead_mu_isoip_3l_sf;
                float subl_mu = subl_mu_recoid_3l_sf * subl_mu_isoip_3l_sf;
                float tert_mu = tert_mu_recoid_3l_sf * tert_mu_isoip_3l_sf;
                float threelep_sf = ((abs(www.lep_pdgId()[0])==11)*(lead_el)+(abs(www.lep_pdgId()[0])!=11)*(lead_mu))
                    *((abs(www.lep_pdgId()[1])==11)*(subl_el)+(abs(www.lep_pdgId()[1])!=11)*(subl_mu))
                    *((abs(www.lep_pdgId()[2])==11)*(tert_el)+(abs(www.lep_pdgId()[2])!=11)*(tert_mu));

                ee_sf       = doFakeEstimation ? 1 : ee_sf;
                em_sf       = doFakeEstimation ? 1 : em_sf;
                mm_sf       = doFakeEstimation ? 1 : mm_sf;
                threelep_sf = doFakeEstimation ? 1 : threelep_sf;
                return std::make_tuple(ee_sf, em_sf, mm_sf, threelep_sf);
            }
            else
            {
                // Set the lepton scale factors based on
                return std::make_tuple(www.lepsf(), www.lepsf(), www.lepsf(), www.lepsf());
            }
        }
};
#endif
