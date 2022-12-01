#include <params.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    hyc_params params;

    hyc_params_parse(argc, argv, &params);

    printf(
        "Input file: `%s`\n"
        "Output file: `%s`\n"
        "Optimization level: `%d`\n"
        "Stage: `%d`\n",
        params.ifname,
        params.ofname,
        params.opt_level,
        params.stage
    );

}
