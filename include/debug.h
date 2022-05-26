#ifndef HYC_DEBUG_H
#define HYC_DEBUG_H

#include <stdio.h>

#ifdef HYC_DEBUG
#define DPRINTF(template, ...) fprintf(stderr, template, ##__VA_ARGS__)
#else
#define DPRINTF(template, ...) if(0)
#endif

#endif