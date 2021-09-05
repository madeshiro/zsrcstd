#ifndef _Z_Stdio_S
#define _Z_Stdio_S
#include "src/zsr_xapi.h"
#include "zsr_file.h"

#ifdef __cplusplus
extern "C" {
#endif // c++

    zsrcall int zu64_dup2(int fd1, int fd2);
    int __zu64_dup2_(int fd1, int fd2, zbool* __dx);

#ifdef __cplusplus
};
#endif

#endif // _Z_Stdio_S