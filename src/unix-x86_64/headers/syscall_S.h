#ifndef _Z_Syscall_S
#define _Z_Syscall_S
#include "zsr_std.h"

zsrcall zint64 __z_syscall0(int);
zsrcall zint64 __z_syscall1(int, void*);
zsrcall zint64 __z_syscall2(int, void*, void*);
zsrcall zint64 __z_syscall3(int, void*, void*, void*);
zsrcall zint64 __z_syscall4(int, void*, void*, void*, void*);
zsrcall zint64 __z_syscall5(int, void*, void*, void*, void*, void*);
zsrcall zint64 __z_syscall6(int, void*, void*, void*, void*, void*, void*);

#define ZU64SYS_READ                    0x0
#define ZU64SYS_WRITE                   0x1
#define ZU64SYS_OPEN                    0x2
#define ZU64SYS_CLOSE                   0x3
#define ZU64SYS_STAT                    0x4
#define ZU64SYS_FSTAT                   0x5
#define ZU64SYS_LSTAT                   0x6
#define ZU64SYS_POLL                    0x7
#define ZU64SYS_LSEEK                   0x8
#define ZU64SYS_MMAP                    0x9
#define ZU64SYS_MPROTECT                0xa
#define ZU64SYS_MUNMAP                  0xb
#define ZU64SYS_BRK                     0xc
#define ZU64SYS_RT_SIGACTION            0xd
#define ZU64SYS_RT_SIGPROCMASK          0xe
#define ZU64SYS_SIGRETURN               0xf
#define ZU64SYS_IOCTL                   0x10
#define ZU64SYS_PREAD64                 0x11
#define ZU64SYS_PWRITE64                0x12
#define ZU64SYS_READV                   0x13
#define ZU64SYS_WRITEV                  0x14
#define ZU64SYS_ACCESS                  0x15
#define ZU64SYS_PIPE                    0x16
#define ZU64SYS_SELECT                  0x17
#define ZU64SYS_SCHED_YIELD             0x18
#define ZU64SYS_MREMAP                  0x19
#define ZU64SYS_MSYNC                   0x1a
#define ZU64SYS_MINCORE                 0x1b
#define ZU64SYS_MADVISE                 0x1c
#define ZU64SYS_SHMGET                  0x1d
#define ZU64SYS_SHMAT                   0x1e
#define ZU64SYS_SHMCTL                  0x1f
#define ZU64SYS_DUP                     0x20
#define ZU64SYS_DUP2                    0x21
#define ZU64SYS_PAUSE                   0x22
#define ZU64SYS_NANOSLEEP               0x23
#define ZU64SYS_GETITIMER               0x24
#define ZU64SYS_ALARM                   0x25
#define ZU64SYS_SETITIMER               0x26
#define ZU64SYS_GETPID                  0x27
#define ZU64SYS_SENDFILE                0x28
#define ZU64SYS_SOCKET                  0x29
#define ZU64SYS_CONNECT                 0x2a
#define ZU64SYS_ACCEPT                  0x2b
#define ZU64SYS_SENDTO                  0x2c
#define ZU64SYS_RECVFROM                0x2d
#define ZU64SYS_SENDMSG                 0x2e
#define ZU64SYS_RECVMSG                 0x2f
#define ZU64SYS_SHUTDOWN                0x30
#define ZU64SYS_BLIND                   0x31
#define ZU64SYS_LISTEN                  0x32
#define ZU64SYS_GETSOCKNAME             0x33
#define ZU64SYS_GETPEERNAME             0x34
#define ZU64SYS_SOCKETPAIR              0x35
#define ZU64SYS_SETSOCKOPT              0x36
#define ZU64SYS_GETSOCKOPT              0x37
#define ZU64SYS_CLONE                   0x38
#define ZU64SYS_FORK                    0x39
#define ZU64SYS_VFORK                   0x3a
#define ZU64SYS_EXECVE                  0x3b
#define ZU64SYS_EXIT                    0x3c
#define ZU64SYS_WAIT4                   0x3d
#define ZU64SYS_KILL                    0x3e
#define ZU64SYS_UNAME                   0x3f
#define ZU64SYS_SEMGET                  0x40
#define ZU64SYS_SEMOP                   0x41
#define ZU64SYS_SEMCTL                  0x42
#define ZU64SYS_SHMDT                   0x43
#define ZU64SYS_MSGGET                  0x44
#define ZU64SYS_MSGSND                  0x45
#define ZU64SYS_MSGRCV                  0x46
#define ZU64SYS_MSGCTL                  0x47
#define ZU64SYS_FCNTL                   0x48
#define ZU64SYS_FLOCK                   0x49
#define ZU64SYS_FSYNC                   0x4a
#define ZU64SYS_FDATASYNC               0x4b
#define ZU64SYS_TRUNCATE                0x4c
#define ZU64SYS_FTRUNCATE               0x4d
#define ZU64SYS_GETDENTS                0x4e
#define ZU64SYS_GETCWD                  0x4f
#define ZU64SYS_CHDIR                   0x50
#define ZU64SYS_FCHDIR                  0x51
#define ZU64SYS_RENAME                  0x52
#define ZU64SYS_MKDIR                   0x53
#define ZU64SYS_RMDIR                   0x54
#define ZU64SYS_CREAT                   0x55
#define ZU64SYS_LINK                    0x56
#define ZU64SYS_UNLINK                  0x57
#define ZU64SYS_SYMLINK                 0x58
#define ZU64SYS_READLINK                0x59
#define ZU64SYS_CHMOD                   0x5a
#define ZU64SYS_FCHMOD                  0x5b
#define ZU64SYS_CHOWN                   0x5c
#define ZU64SYS_FCHOWN                  0x5d
#define ZU64SYS_LCHOWN                  0x5e
#define ZU64SYS_UMASK                   0x5f
#define ZU64SYS_GETTIMEOFDAY            0x60
#define ZU64SYS_GETRLIMIT               0x61
#define ZU64SYS_GETRUSAGE               0x62
#define ZU64SYS_SYSINFO                 0x63
#define ZU64SYS_TIMES                   0x64
#define ZU64SYS_PTRACE                  0x65
#define ZU64SYS_GETUID                  0x66
#define ZU64SYS_SYSLOG                  0x67
#define ZU64SYS_GETGID                  0x68
#define ZU64SYS_SETUID                  0x69
#define ZU64SYS_SETGID                  0x6a
#define ZU64SYS_GETEUID                 0x6b
#define ZU64SYS_GETEGID                 0x6c
#define ZU64SYS_SETPGID                 0x6d
#define ZU64SYS_GETPPID                 0x6e
#define ZU64SYS_GETPGRP                 0x6f
#define ZU64SYS_SETSID                  0x70

#endif // _Z_Syscall_S
