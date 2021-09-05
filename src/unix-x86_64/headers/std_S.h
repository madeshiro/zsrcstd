#ifndef _Z_Std_S
#define _Z_Std_S
#include "zsr_std.h"

#define zS_IRWXA 00777

#define zS_IRUSR 00400
#define zS_IWUSR 00200
#define zS_IXUSR 00100
#define zS_IRWXU 00700

#define zS_IRGRP 00040
#define zS_IWGRP 00020
#define zS_IXGRP 00010
#define zS_IRWXG 00070

#define zS_IROTH 00004
#define zS_IWOTH 00002
#define zS_IXOTH 00001
#define zS_IRWXO 00007

#ifdef __cplusplus
extern "C" {
#endif // c++

    void __zu64_errno();

#ifdef __cplusplus
};
#endif

#endif // _Z_Std_S