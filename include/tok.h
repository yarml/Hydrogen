#ifndef HYC_TOK_H
#define HYC_TOK_H

#include <stddef.h>

struct HYC_TOK
{
    size_t line;
    size_t colm;
    char*  text;
};

typedef struct HYC_TOK token;

#endif