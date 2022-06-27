#include <dynarray.h>
#include <string.h>
#include <stdlib.h>

dynarray* dynarray_init(size_t elsize, size_t inlen)
{
    dynarray* da = calloc(1, sizeof(dynarray));

    if(!da)
        return NULL;

    da->elsize = elsize;
    da->allen = 0;
    da->arlen = inlen;
    da->array = calloc(inlen, elsize);

    if(!da->array)
    {
        free(da);
        return NULL;
    }

    return da;
}

void dynarray_fini(dynarray* da)
{
    if(da)
    {
        if(da->array)
            free(da->array);
        free(da);
    }
}


size_t dynarray_size(dynarray* da)
{
    if(da)
        return da->arlen;
    return 0;
}

void* dynarray_access(dynarray* da)
{
    if(da)
        return da->array;
    return NULL;
}


bool dynarray_insert(dynarray* da, size_t idx, void* el)
{
    if(!da || idx > da->arlen)
        return false;

    if(da->allen <= idx)
    {
        void* narray = realloc(da->array, 2 * da->allen * da->elsize);
        if(!narray)
            return false;
        da->array = narray;
        da->allen *= 2;
    }
    memmove(da->array + (da->elsize * (idx + 1)), da->array + (da->elsize * idx), da->elsize * (da->arlen - idx));
    if(el)
        memcpy(da->array + (da->elsize * idx), el, da->elsize);
    ++da->arlen;
    return true;
}

bool dynarray_append(dynarray* da, void* el)
{
    if(!da)
        return false;
    
    if(da->allen <= da->arlen)
    {
        void* narray = realloc(da->array, 2 * da->allen * da->elsize);
        if(!narray)
            return false;
        da->array = narray;
        da->allen *= 2;
    }
    if(el)
        memcpy(da->array + (da->elsize * da->arlen), el, da->elsize);
    ++da->arlen;
    return true;
}

bool dynarray_remove(dynarray* da, size_t idx)
{
    if(!da || idx >= da->arlen)
        return false;
    memmove(da->array + (da->elsize * idx), da->array + (da->elsize * (idx + 1)), da->elsize * (da->arlen - idx - 1));
    --da->arlen;
    return true;    
}

bool dynarray_resize(dynarray* da, size_t allen)
{
    if(!da)
        return false;
    
    void* narray = realloc(da->array, allen * da->elsize);
    if(!narray)
        return false;
    da->array = narray;
    da->allen = allen;
    if(allen < da->arlen)
    {
        da->arlen = allen;
        return true;
    }
    // TODO: If I add stuff after the if statement, remember to add a common exit routine for both cases.
    return false;
}

bool dynarray_strip (dynarray* da)
{
    if(!da)
        return false;
    if(da->allen == da->arlen)
        return true;
    
    void* narray = realloc(da->array, da->arlen * da->elsize);
    if(!narray)
        return false;
    da->array = narray;
    da->allen = da->arlen;
    return true;
}
