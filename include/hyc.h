#ifndef HYC_HYC_H
#define HYC_HYC_H

#include <stdnoreturn.h>

#define EXIT_ARGS   (2  )
#define EXIT_INVLST (255)


noreturn void exit_error(int exit_code, char* template, ...);
noreturn void exit_invlst(char* fname, int fln);


void print_help();
void print_ver();

#endif