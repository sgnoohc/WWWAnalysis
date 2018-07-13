#!/bin/env python

from QFramework import TQSampleFolder
from rooutil import qutils

#########################################################################################################################################################
def main(mass=""):

    samples = TQSampleFolder.loadSampleFolder("outputs/output.root:samples")
    samples_jec_up = TQSampleFolder.loadSampleFolder("outputs/output_jec_up.root:samples")
    samples_jec_dn = TQSampleFolder.loadSampleFolder("outputs/output_jec_dn.root:samples")

    options = {

            # Signal name and TQSampleFolder path
            "sig" : ("www", "/sig" if mass == "" else "/bsm/hpmpm/{}".format(mass)),

            # Background names and TQSampelFolder paths
            "bkgs" : [
                ("lostlep" , "/typebkg/lostlep/[ttZ+WZ+Other]"),
                ("fake"    , "/fake"),
                ("vbsww"   , "/typebkg/?/VBSWW"),
                ("ttw"     , "/typebkg/?/ttW"),
                ("photon"  , "/typebkg/photon/[ttZ+WZ+Other]"),
                ("qflip"   , "/typebkg/qflip/[ttZ+WZ+Other]"),
                ("prompt"  , "/typebkg/prompt/[ttZ+WZ+Other]" if mass == "" else "/typebkg/prompt/[ttZ+WZ+Other]+sig"),
                ],

            # Data TQSampleFolder paths
            "data" : "/data",

            # Output histogram
            "hist_output_file" : "hist.root",

            # Histogram names and full path
            "hists" : [
                ("MllSS_varbin"   , "SRSSeeFull/MllSS_varbin+SRSSemFull/MllSS_varbin+SRSSmmFull/MllSS_varbin+SideSSeeFull/MllSS_varbin+SideSSemFull/MllSS_varbin+SideSSmmFull/MllSS_varbin"   ),
                #("SRSSeeFull"   , "{SRSSeeFull}"   ),
                #("SRSSemFull"   , "{SRSSemFull}"   ),
                #("SRSSmmFull"   , "{SRSSmmFull}"   ),
                #("SideSSeeFull" , "{SideSSeeFull}" ),
                #("SideSSemFull" , "{SideSSemFull}" ),
                #("SideSSmmFull" , "{SideSSmmFull}" ),
                #("SR0SFOSFull"  , "{SR0SFOSFull}"  ),
                #("SR1SFOSFull"  , "{SR1SFOSFull}"  ),
                #("SR2SFOSFull"  , "{SR2SFOSFull}"  ),
                #("SRNj1SSeeFull", "{SRNj1SSeeFull}"),
                #("SRNj1SSemFull", "{SRNj1SSemFull}"),
                #("SRNj1SSmmFull", "{SRNj1SSmmFull}"),
                ],

            # TQSampleFolder object
            "nominal_sample" : samples,

            # Control regions
            # The control regions will normalize the counts
            # The systematics
            "control_regions" : {
                ("MllSS_varbin"   , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("WZCRSSeeFull/MllSS_varbin+WZCRSSemFull/MllSS_varbin+WZCRSSmmFull/MllSS_varbin"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SRSSeeFull"   , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRSSeeFull}"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SRSSemFull"   , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRSSemFull}"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SRSSmmFull"   , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRSSmmFull}"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SideSSeeFull" , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRSSeeFull}"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SideSSemFull" , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRSSemFull}"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SideSSmmFull" , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRSSmmFull}"    , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SR1SFOSFull"  , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCR1SFOSFull}"   , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SR2SFOSFull"  , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCR2SFOSFull}"   , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SRNj1SSeeFull", "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRNj1SSeeFull}" , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SRNj1SSemFull", "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRNj1SSemFull}" , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                #("SRNj1SSmmFull", "/typebkg/lostlep/[ttZ+WZ+Other]") : ("{WZCRNj1SSmmFull}" , "/data-typebkg/qflip-typebkg/photon-typebkg/prompt-typebkg/fakes-typebkg/lostlep/VBSWW-typebkg/lostlep/ttW-sig"),
                },

            # Weight variation systematics that are saved in the "nominal_sample" TQSampleFolder
            # The nomenclature of the coutner names must be <BIN_COUNTER><SYSTS>Up and <BIN_COUNTER><SYSTS>Down
            # The keyword are the systematics and then the items list the processes to apply the systematics
            "systematics" : [
                ("LepSF"         , { "procs_to_apply" : ["vbsww", "ttw", "photon", "qflip", "prompt", "lostlep"]                                                                          }),
                ("TrigSF"        , { "procs_to_apply" : ["vbsww", "ttw", "photon", "qflip", "prompt", "lostlep"]                                                                          }),
                ("BTagLF"        , { "procs_to_apply" : ["vbsww", "ttw", "photon", "qflip", "prompt", "lostlep"]                                                                          }),
                ("BTagHF"        , { "procs_to_apply" : ["vbsww", "ttw", "photon", "qflip", "prompt", "lostlep"]                                                                          }),
                ("Pileup"        , { "procs_to_apply" : ["vbsww", "ttw", "photon", "qflip", "prompt", "lostlep"]                                                                          }),
                ("FakeRateEl"    , { "procs_to_apply" : ["fake"]                                                                                                                          }),
                ("FakeRateMu"    , { "procs_to_apply" : ["fake"]                                                                                                                          }),
                ("FakeClosureEl" , { "procs_to_apply" : ["fake"]                                                                                                                          }),
                ("FakeClosureMu" , { "procs_to_apply" : ["fake"]                                                                                                                          }),
                ("PDF"           , { "procs_to_apply" : ["www"]                                                                                                                           }),
                ("AlphaS"        , { "procs_to_apply" : ["www"]                                                                                                                           }),
                ("Qsq"           , { "procs_to_apply" : ["www"]                                                                                                                           }),
                ("JEC"           , { "procs_to_apply" : ["www", "vbsww", "ttw", "photon", "qflip", "prompt", "lostlep"], "syst_samples" : {"Up" : samples_jec_up, "Down": samples_jec_dn} }),
                ],

            "statistical" : [ "www", "vbsww", "ttw", "photon", "qflip", "prompt", "fake", "lostlep" ],

            "flat_systematics" : [
                    ("VBSWWXSec"            , ["vbsww"                                           ] , "1.2"  , ""     ) ,
                    ("ttWXSec"              , ["ttw"                                             ] , "1.2"  , ""     ) ,
                    ("VBSWWVRSyst"          , ["vbsww"                                           ] , "1.22" , ""     ) ,
                    ("ttWVRSyst"            , ["ttw"                                             ] , "1.18" , ""     ) ,
                    ("QFlipVRSyst"          , ["qflip"                                           ] , "1.5"  , ""     ) ,
                    ("PhotonVRSyst"         , ["photon"                                          ] , "1.5"  , ""     ) ,
                    ("LostLepMll3LModeling" , ["lostlep"                                         ] , "1.082", "SFOS" ) ,
                    ("LostLepMllSSModeling" , ["lostlep"                                         ] , "1.053", "SS"   ) ,
                    ("LostLepMjjModeling"   , ["lostlep"                                         ] , "1.049", "SS"   ) ,
                    ("LumSyst"              , ["vbsww", "ttw", "photon", "qflip", "prompt", "www"] , "1.025", ""     ) ,
                ],

            }


    print qutils.make_shape_experiment_statistics_data_card(options)

if __name__ == "__main__":

    main("1000")
