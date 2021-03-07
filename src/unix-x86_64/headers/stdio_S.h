#ifndef _Z_Stdio_S
#define _Z_Stdio_S
#include "zsr_std.h"
#include "zsr_file.h"

/* <<< define ASM HEADER >>> */

int zsrcall _Z_sys_open(const char* __fname, int flags, int mode);
int zsrcall _Z_sys_close(int fd);
int zsrcall _Z_sys_read(char* __buf, unsigned long __len, int fd);
int zsrcall _Z_sys_write(const char* __buf, unsigned long __len, int fd);
int zsrcall _Z_sys_link(const char* __oldname, const char* __newname);
int zsrcall _Z_sys_unlink(const char* __fname);
int zsrcall _Z_sys_lseek(int fd, long __offset, int __whence);

/* <<< define C HEADER : file.c >>> */

zfd_t _Z_unix_open(const char* fname, zflag openm, zbool create);

struct _z_file
{
    char* name;
    zfd_t fd;
};

#endif // _Z_Stdio_S