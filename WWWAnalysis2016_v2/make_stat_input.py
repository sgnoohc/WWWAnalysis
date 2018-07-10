#!/bin/env python

from QFramework import TQSampleFolder
from rooutil import qutils

#########################################################################################################################################################
def main():

    samples = TQSampleFolder.loadSampleFolder("outputs/output.root:samples")
    samples_jec_up = TQSampleFolder.loadSampleFolder("outputs/output_jec_up.root:samples")
    samples_jec_dn = TQSampleFolder.loadSampleFolder("outputs/output_jec_dn.root:samples")

    options = {

            # Signal name and TQSampleFolder path
            "sig" : ("www", "/sig"),

            # Background names and TQSampelFolder paths
            "bkgs" : [
                ("lostlep" , "/typebkg/lostlep/[ttZ+WZ+Other]"),
                ("fake"    , "/fake"),
                ("vbsww"   , "/typebkg/?/VBSWW"),
                ("ttw"     , "/typebkg/?/ttW"),
                ("photon"  , "/typebkg/photon/[ttZ+WZ+Other]"),
                ("qflip"   , "/typebkg/qflip/[ttZ+WZ+Other]"),
                ("prompt"  , "/typebkg/prompt/[ttZ+WZ+Other]"),
                ],

            # Data TQSampleFolder paths
            "data" : "/data",

            # Counter names for getting yields
            "bins" : [
                "SRSSeeFull",
                "SRSSemFull",
                "SRSSmmFull",
                "SideSSeeFull",
                "SideSSemFull",
                "SideSSmmFull",
                "SR0SFOSFull",
                "SR1SFOSFull",
                "SR2SFOSFull",
                #"SRNj1SSeeFull",
                #"SRNj1SSemFull",
                #"SRNj1SSmmFull",
                ],

            # TQSampleFolder object
            "nominal_sample" : samples,

            # Control regions
            # The control regions will normalize the counts
            # The systematics
            "control_regions" : {
                ("SRSSeeFull"  , "/typebkg/lostlep/[ttZ+WZ+Other]") : ("WZCRSSeeFull", "/data-typebkg/[qflip+photon+prompt+fakes]-sig"),
                ("SideSSeeFull", "/typebkg/lostlep/[ttZ+WZ+Other]") : ("WZCRSSeeFull", "/data-typebkg/[qflip+photon+prompt+fakes]-sig"),
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

            "statistical" : [ "www", "vbsww", "ttw", "photon", "qflip", "prompt", "fake" ],

            }


    print qutils.make_counting_experiment_statistics_data_card(options)

if __name__ == "__main__":

    main()
