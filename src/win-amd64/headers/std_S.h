#ifndef _Z_Std_S
#define _Z_Std_S
#include "zsr_std.h"

/* <<< define ASM HEADER >>> */

void* zsrcall _Z_alloc(zsize_t __size);
void* zsrcall _Z_calloc(zsize_t __size);
void* zsrcall _Z_realloc(void* __ptr, zsize_t __nsize);
int zsrcall _Z_free(void* __ptr);
zsize_t zsrcall _Z_allocs(const void* __ptr);

void zsrcall _Z_mcopy(const void* __src, void* __dest, zsize_t __s);
void zsrcall _Z_mswap(void* __p1, void* __p2, zsize_t __s);

#endif // _Z_Std_S