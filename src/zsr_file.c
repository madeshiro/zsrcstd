/** global `C` files
 * @file zsr_file.c - implementation
 * @version 2021/03 0.4.2
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_file.h"
#include "zsr_string.h"
#include "zsr_prvt.h"

struct __z_hndl_file
{
    __z_defhndl();
    char* fname;
    zflag fencoding;
};

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
};
#endif