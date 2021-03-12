#ifndef _Z_Std_S
#define _Z_Std_S
#include "zsr_std.h"

/* <<< define ASM HEADER >>> */

void* zsrcall   _Z_alloc(zsize_t __size);
void* zsrcall   _Z_calloc(zsize_t __size);
void* zsrcall   _Z_realloc(void* __p, zsize_t __size);
int   zsrcall   _Z_free(const void* __p);
zsize_t zsrcall _Z_allocs(const void* __p);

void zsrcall _Z_mcopy(const void* __src, void* __dest, zsize_t __s);
void zsrcall _Z_mswap(void* __p1, void* __p2, zsize_t __s);

int zsrcall  _Z_hndlid(zhandler __hndl);
void zsrcall _Z_errno(void);

#endif // _Z_Std_S