#ifndef HYC_PARAMS_H
#define HYC_PARAMS_H

#include <dtc/status.h>

#define HYC_DEF_SOURCE ("source.hy")

enum HYC_STAGE;
typedef enum HYC_STAGE hyc_stage;
enum HYC_STAGE
{
    HYC_STAGE_LEX = 1,
    HYC_STAGE_PARSE,
    HYC_STAGE_COMPILE,
    HYC_STAGE_LINK
};

enum HYC_OPT_LEVEL;
typedef enum HYC_OPT_LEVEL hyc_opt_level;
enum HYC_OPT_LEVEL
{
    HYC_OPT_NONE = 1,
    HYC_OPT_FULL
};

struct HYC_PARAMS;
typedef struct HYC_PARAMS hyc_params;
struct HYC_PARAMS
{
    char *ifname; /* If not specified, NULL */
    char *ofname; /* If not specified, depens on stage*/
    hyc_stage stage; /* If not specified, LINK */
    hyc_opt_level opt_level; /* If not specified FULL */
};

status hyc_params_parse(int argc, char **argv, hyc_params *out_params);

#endif