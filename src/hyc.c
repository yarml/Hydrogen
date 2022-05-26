#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <args.h>
#include <hyc.h>

/* exit_* */
noreturn void exit_error(int exit_code, char* template, ...)
{
    va_list vargs;
    va_start(vargs, template);
    vfprintf(stderr, template, vargs);
    va_end(vargs);
    exit(exit_code);
}

noreturn void exit_invlst(char* fname, int fln)
{
    fprintf(stderr, "Reached invalid state in %s:%d!\n", fname, fln);
    exit(EXIT_INVLST);
}

/* print_* */
// TODO: print_help, print_ver
void print_help()
{
    fprintf(stderr, "<TODO>\n");
}
void print_ver()
{
    fprintf(stderr, "<TODO>\n");
}

#include <fcntl.h>

int main(int argc, char** argv)
{
    hyc_args* args = args_parse(argc, argv);    

    args_clean(args);   
}

