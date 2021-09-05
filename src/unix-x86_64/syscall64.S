# Unix x86_64 assembly files
# @file stdio.S - system call
# @version 2021/09 0.5.0
.att_syntax
.text

#### <<< syscall64.S / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
# //

.globl __z_syscall0
.globl __z_syscall1
.globl __z_syscall2
.globl __z_syscall3
.globl __z_syscall4
.globl __z_syscall5
.globl __z_syscall6

__z_syscall0:
    mov %edi, %eax
    syscall
    ret
__z_syscall1:
    mov %edi, %eax
    mov %rsi, %rdi
    syscall
    ret
__z_syscall2:
    mov %edi, %eax
    mov %rsi, %rdi
    mov %rdx, %rsi
    syscall
    ret
__z_syscall3:
    mov %edi, %eax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    syscall
    ret
__z_syscall4:
    mov %edi, %eax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    syscall
    ret
__z_syscall5:
    mov %edi, %eax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
    syscall
    ret
__z_syscall6:
    mov %edi, %eax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
    mov 0x20(%rsp), %r9
    syscall
    ret

.set zu64sys_read               , 0x0
.set zu64sys_write              , 0x1
.set zu64sys_open               , 0x2
.set zu64sys_close              , 0x3
.set zu64sys_stat               , 0x4
.set zu64sys_fstat              , 0x5
.set zu64sys_lstat              , 0x6
.set zu64sys_poll               , 0x7
.set zu64sys_lseek              , 0x8
.set zu64sys_mmap               , 0x9
.set zu64sys_mprotect           , 0xa
.set zu64sys_munmap             , 0xb
.set zu64sys_brk                , 0xc
.set zu64sys_rt_sigaction       , 0xd
.set zu64sys_rt_sigprocmask     , 0xe
.set zu64sys_sigreturn          , 0xf
.set zu64sys_ioctl              , 0x10
.set zu64sys_pread64            , 0x11
.set zu64sys_pwrite64           , 0x12
.set zu64sys_readv              , 0x13
.set zu64sys_writev             , 0x14
.set zu64sys_access             , 0x15
.set zu64sys_pipe               , 0x16
.set zu64sys_select             , 0x17
.set zu64sys_sched_yield        , 0x18
.set zu64sys_mremap             , 0x19
.set zu64sys_msync              , 0x1a
.set zu64sys_mincore            , 0x1b
.set zu64sys_madvise            , 0x1c
.set zu64sys_shmget             , 0x1d
.set zu64sys_shmat              , 0x1e
.set zu64sys_shmctl             , 0x1f
.set zu64sys_dup                , 0x20
.set zu64sys_dup2               , 0x21
.set zu64sys_pause              , 0x22
.set zu64sys_nanosleep          , 0x23
.set zu64sys_getitimer          , 0x24
.set zu64sys_alarm              , 0x25
.set zu64sys_setitimer          , 0x26
.set zu64sys_getpid             , 0x27
.set zu64sys_sendfile           , 0x28
.set zu64sys_socket             , 0x29
.set zu64sys_connect            , 0x2a
.set zu64sys_accept             , 0x2b
.set zu64sys_sendto             , 0x2c
.set zu64sys_recvfrom           , 0x2d
.set zu64sys_sendmsg            , 0x2e
.set zu64sys_recvmsg            , 0x2f
.set zu64sys_shutdown           , 0x30
.set zu64sys_blind              , 0x31
.set zu64sys_listen             , 0x32
.set zu64sys_getsockname        , 0x33
.set zu64sys_getpeername        , 0x34
.set zu64sys_socketpair         , 0x35
.set zu64sys_setsockopt         , 0x36
.set zu64sys_getsockopt         , 0x37
.set zu64sys_clone              , 0x38
.set zu64sys_fork               , 0x39
.set zu64sys_vfork              , 0x3a
.set zu64sys_execve             , 0x3b
.set zu64sys_exit               , 0x3c
.set zu64sys_wait4              , 0x3d
.set zu64sys_kill               , 0x3e
.set zu64sys_uname              , 0x3f
.set zu64sys_semget             , 0x40
.set zu64sys_semop              , 0x41
.set zu64sys_semctl             , 0x42
.set zu64sys_shmdt              , 0x43
.set zu64sys_msgget             , 0x44
.set zu64sys_msgsnd             , 0x45
.set zu64sys_msgrcv             , 0x46
.set zu64sys_msgctl             , 0x47
.set zu64sys_fcntl              , 0x48
.set zu64sys_flock              , 0x49
.set zu64sys_fsync              , 0x4a
.set zu64sys_fdatasync          , 0x4b
.set zu64sys_truncate           , 0x4c
.set zu64sys_ftruncate          , 0x4d
.set zu64sys_getdents           , 0x4e
.set zu64sys_getcwd             , 0x4f
.set zu64sys_chdir              , 0x50
.set zu64sys_fchdir             , 0x51
.set zu64sys_rename             , 0x52
.set zu64sys_mkdir              , 0x53
.set zu64sys_rmdir              , 0x54
.set zu64sys_creat              , 0x55
.set zu64sys_link               , 0x56
.set zu64sys_unlink             , 0x57
.set zu64sys_symlink            , 0x58
.set zu64sys_readlink           , 0x59
.set zu64sys_chmod              , 0x5a
.set zu64sys_fchmod             , 0x5b
.set zu64sys_chown              , 0x5c
.set zu64sys_fchown             , 0x5d
.set zu64sys_lchown             , 0x5e
.set zu64sys_umask              , 0x5f
.set zu64sys_gettimeofday       , 0x60
.set zu64sys_getrlimit          , 0x61
.set zu64sys_getrusage          , 0x62
.set zu64sys_sysinfo            , 0x63
.set zu64sys_times              , 0x64
.set zu64sys_ptrace             , 0x65
.set zu64sys_getuid             , 0x66
.set zu64sys_syslog             , 0x67
.set zu64sys_getgid             , 0x68
.set zu64sys_setuid             , 0x69
.set zu64sys_setgid             , 0x6a
.set zu64sys_geteuid            , 0x6b
.set zu64sys_getegid            , 0x6c
.set zu64sys_setpgid            , 0x6d
.set zu64sys_getppid            , 0x6e
.set zu64sys_getpgrp            , 0x6f
.set zu64sys_setsid             , 0x70
