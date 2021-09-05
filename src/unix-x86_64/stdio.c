/** Unix x86_64 `C` files
 * @file stdio.c - implementation
 * @version 2021/09 0.5.0
 * @author Rin Baudelet (madeshiro)
 */
#include <sys/file.h>
#include "zsr_std.h"
#include "headers/std_S.h"
#include "headers/stdio_S.h"
#include "headers/syscall_S.h"

zfd_t __z_xapi_stdio(int fd)
{
    switch (fd)
    {
        case ZSR_IO_STDIN:
            return __z_os_stdio_fileno.stdin;
        case ZSR_IO_STDOUT:
            return __z_os_stdio_fileno.stdout;
        case ZSR_IO_STDERR:
            return __z_os_stdio_fileno.stderr;
        default:
            return -1;
    }
}

int __zu64_dup2_(int fd1, int fd2, zbool* __dx)
{
    *__dx = ztrue;
    if (fd1 == __z_os_stdio_fileno.stdin)
    {
        if (__z_os_stdio_fileno.dup_stdin == -1 && fd1 == ZSR_IO_STDIN)
            __z_os_stdio_fileno.dup_stdin = __z_xapi_dup(fd1);
        __z_os_stdio_fileno.stdin = __z_syscall2(ZU64SYS_DUP2, fd1, fd2);
        return __z_os_stdio_fileno.stdin;
    }
    else if (fd1 == __z_os_stdio_fileno.stdout)
    {
        if (__z_os_stdio_fileno.stdout == -1 && fd1 == ZSR_IO_STDOUT)
            __z_os_stdio_fileno.dup_stdout = __z_xapi_dup(fd1);
        __z_os_stdio_fileno.stdout = __z_syscall2(ZU64SYS_DUP2, fd1, fd2);
        return __z_os_stdio_fileno.stdout;
    }
    else if (fd1 == __z_os_stdio_fileno.stderr)
    {
        if (__z_os_stdio_fileno.stderr == -1 && fd1 == ZSR_IO_STDERR)
            __z_os_stdio_fileno.dup_stderr = __z_xapi_dup(fd1);
        __z_os_stdio_fileno.stderr = __z_syscall2(ZU64SYS_DUP2, fd1, fd2);
        return __z_os_stdio_fileno.stderr;
    }
    else
    {
        *__dx = zfalse;
        return -1;
    }
}

int __z_xapi_stdiosrc(int fd)
{
    switch (fd)
    {
        case ZSR_IO_STDIN:
            return __z_os_stdio_fileno.dup_stdin == -1 ? __z_os_stdio_fileno.stdin : __z_os_stdio_fileno.dup_stdin;
        case ZSR_IO_STDOUT:
            return __z_os_stdio_fileno.dup_stdout == -1 ? __z_os_stdio_fileno.stdout : __z_os_stdio_fileno.dup_stdout;
        case ZSR_IO_STDERR:
            return __z_os_stdio_fileno.dup_stderr == -1 ? __z_os_stdio_fileno.stderr : __z_os_stdio_fileno.dup_stderr;
        default:
            zsetlasterror(ZEINVAL);
            return -1;
    }
}

int __z_xapi_stdioback(int channel)
{
    int fd = -1, *fileno, *dup_fileno;
    switch (channel)
    {
        case ZSR_IO_STDIN:
            fileno = &__z_os_stdio_fileno.stdin;
            dup_fileno = &__z_os_stdio_fileno.dup_stdin;
            break;
        case ZSR_IO_STDOUT:
            fileno = &__z_os_stdio_fileno.stdout;
            dup_fileno = &__z_os_stdio_fileno.dup_stdout;
            break;
        case ZSR_IO_STDERR:
            fileno = &__z_os_stdio_fileno.stderr;
            dup_fileno = &__z_os_stdio_fileno.dup_stderr;
            break;
        default:
            zsetlasterror(ZEINVAL);
            return -1;
    }



    if (*dup_fileno != -1)
    {
        fd = __z_xapi_dup(*fileno);
        *fileno = __z_xapi_dup2(*fileno, *dup_fileno);
        *dup_fileno = -1;
    }
    else
    {
        zsetlasterror(ZEAGAIN); // the channel is already back to its normal value
    }

    return fd;
}

zfd_t __z_xapi_fopen(const char* name, zflag openm, zbool creat)
{
    int mode = 0, flags = 0;
    if (creat)
    {
        flags |= O_CREAT;
        mode = zS_IRWXA;
    }

    if ((openm & ZSR_IO_OPENM_IN) && (openm & ZSR_IO_OPENM_OUT))
        flags |= O_RDWR;
    else if (openm & ZSR_IO_OPENM_OUT)
        flags |= O_WRONLY;
    else if (openm & ZSR_IO_OPENM_IN)
        flags |= O_RDONLY;

    flags |= openm & ZSR_IO_OPENM_TRUNC ? O_TRUNC : 0;
    flags |= openm & ZSR_IO_OPENM_APP ? O_APPEND : 0;

    return __z_syscall3(ZU64SYS_OPEN, name, flags, mode);
}
