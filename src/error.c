#include <stdarg.h>
#include <stdlib.h>
#include <error.h>
#include <stdio.h>

noreturn
status hyc_exit(int exit_code, char const* exit_msg, ...)
{
    va_list va;

    va_start(va, exit_msg);
    vfprintf(stderr, exit_msg, va);
    va_end(va);

    putc('\n', stderr);

    exit(exit_code);
}
