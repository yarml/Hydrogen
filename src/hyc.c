#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <debug.h>
#include <args.h>
#include <hyc.h>
#include <lex.h>

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
    fprintf(stderr, "Reached invalid state in %s:%d\n", fname, fln);
    exit(EXIT_INVLST);
}

noreturn void exit_lexerr(char* details, int line, int colm, char* lines, char* iss)
{
    int ref = 0;
    fprintf(stderr, "Syntax error at `%s:%d:%d` `%n%s`: %s\n", iss, line, colm, &ref, lines, details);
    fprintf(stderr, "%*c^\n", ref + colm, ' ');
    exit(EXIT_LEXERR);
}

/* print_* */
// TODO: print_help, print_ver
void print_help()
{
    fprintf(stderr, "<TODO: help>\n");
}
void print_ver()
{
    fprintf(stderr, "<TODO: version>\n");
}

#include <fcntl.h>

int main(int argc, char** argv)
{
    hyc_args* args = args_init(argc, argv);
    
    args_fini(args);   
}

