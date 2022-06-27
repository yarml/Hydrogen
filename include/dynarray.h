#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

struct DYN_ARRAY_HANDLE
{
    void* array;
    size_t elsize; /* element size  */
    size_t arlen; /* array length */
    size_t allen; /* allocated length */
};

typedef struct DYN_ARRAY_HANDLE dynarray;


dynarray* dynarray_init(size_t elsize, size_t insize);
void dynarray_fini(dynarray* da);

size_t dynarray_size(dynarray* da);
void* dynarray_access(dynarray* da);

bool dynarray_insert(dynarray* da, size_t idx, void* el);
bool dynarray_append(dynarray* da, void* el);
bool dynarray_remove(dynarray* da, size_t idx);
bool dynarray_resize(dynarray* da, size_t allen);
bool dynarray_strip (dynarray* da);

#endif
