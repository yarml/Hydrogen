#ifndef HYC_DYNSTR_H
#define HYC_DYNSTR_H

#include <stdbool.h>
#include <stddef.h>

struct DYN_STR_HANDLE
{
    char* str;
    size_t size;
    size_t allocated;
};

typedef struct DYN_STR_HANDLE dynstr;

dynstr* dynstr_init();
void dynstr_fini(dynstr* ds);

char* dynstr_data(dynstr* ds);
size_t dynstr_len(dynstr* ds);

bool dynstr_appends(dynstr* ds, char const* s);
bool dynstr_appendc(dynstr* ds, char c);
bool dynstr_sets(dynstr* ds, char const* s);
bool dynstr_setc(dynstr* ds, size_t idx, char c);

char dynstr_getc(dynstr* ds, size_t idx);

#endif