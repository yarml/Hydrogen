#include <params.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <error.h>

#include <internal/common.h>

status hyc_params_parse(int argc, char **argv, hyc_params *out_params)
{
    DTC_ASSERT_PARAM_PTR_VALID(argv);
    DTC_ASSERT_PARAM_PTR_VALID(out_params);

    int opt;

    hyc_opt_level opt_level = HYC_OPT_FULL;
    hyc_stage stage = HYC_STAGE_LINK;
    char *ifname = 0;
    char *ofname = 0;

    static struct option lopts[] =
    {
        { "output", required_argument, 0, 'o'},
        { "optimization", required_argument, 0, 'O'},
        { 0, 0, 0, 0 }
    };

    optind = 1;

    while(1)
    {
        int oind = 0;

        opt = getopt_long(argc, argv, "o:O:LPC", lopts, &oind);

        if(opt == -1)
            break;
        switch(opt)
        {
            case 'o':
                ofname = optarg;
                break;
            case 'O':
                if(strlen(optarg) != 1)
                    hyc_exit(
                        HYC_EXIT_ARGS,
                        "Invalid argument for -O/--optimization `%s`.",
                        optarg
                    );
                switch (optarg[0])
                {
                    case 'f':
                        opt_level = HYC_OPT_FULL;
                        break;
                    case 'n':
                        opt_level = HYC_OPT_NONE;
                        break;
                    default:
                        hyc_exit(
                            HYC_EXIT_ARGS,
                            "Invalid argument for -O/--optimization `%c`.",
                            optarg[0]
                        );
                }
                break;
            case 'L':
                stage = HYC_STAGE_LEX;
                break;
            case 'P':
                stage = HYC_STAGE_PARSE;
                break;
            case 'C':
                stage = HYC_STAGE_COMPILE;
                break;
            case '?':
                exit(HYC_EXIT_ARGS);
            default:
                hyc_exit_invstate("Invalid getopt_long() `%c`", opt);
        }
    }

    if(!ofname)
    {
        switch(stage)
        {
            case HYC_STAGE_LEX:
                ofname = "out.lex";
                break;
            case HYC_STAGE_PARSE:
                ofname = "out.ast";
                break;
            case HYC_STAGE_COMPILE:
                ofname = "out.o";
                break;
            case HYC_STAGE_LINK:
                ofname = "out.out";
                break;
            default:
                hyc_exit_invstate("Unknwown stage `%d`", stage);
        }
    }

    switch(argc - optind)
    {
        case 0:
            ifname = HYC_DEF_SOURCE;
            break;
        case 1:
            ifname = argv[optind];
            break;
        default:
            hyc_exit(HYC_EXIT_ARGS, "Additional arguments");
    }

    out_params->ifname = ifname;
    out_params->ofname = ofname;
    out_params->opt_level = opt_level;
    out_params->stage = stage;

    return DTC_STATUS_SUCCESS;
}
