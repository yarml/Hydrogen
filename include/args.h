#ifndef HYC_ARGS_H
#define HYC_ARGS_H

#include <stdbool.h>
#include <stdio.h>

struct HYC_ARGS
{
    int    argc;
    char** argv;
};

typedef struct HYC_ARGS hyc_args;

hyc_args* args_init(int argc, char** argv);
void      args_fini(hyc_args* args);

#endif