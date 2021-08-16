# Unix x86_64 assembly files
# @file stdio.S - system call
# @version 2021/08 0.5.0
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

.globl _Z_sys64_call0
.globl _Z_sys64_call1
.globl _Z_sys64_call2
.globl _Z_sys64_call3
.globl _Z_sys64_call4
.globl _Z_sys64_call5
.globl _Z_sys64_call6

_Z_sys64_call0:
    mov %rdi, %rax
    syscall
    ret
_Z_sys64_call1:
    mov %rdi, %rax
    mov %rsi, %rdi
    syscall
    ret
_Z_sys64_call2:
    mov %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    syscall
    ret
_Z_sys64_call3:
    mov %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    syscall
    ret
_Z_sys64_call4:
    mov %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    syscall
    ret
_Z_sys64_call5:
    mov %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
    syscall
    ret
_Z_sys64_call6:
    mov %rdi, %rax
    mov %rsi, %rdi
    mov %rdx, %rsi
    mov %rcx, %rdx
    mov %r8, %r10
    mov %r9, %r8
    mov 0x20(%rsp), %r9
    syscall
    ret

.globl _Z_sys64_read
.globl _Z_sys64_write
.globl _Z_sys64_open
.globl _Z_sys64_close
.globl _Z_sys64_stat
.globl _Z_sys64_fstat
.globl _Z_sys64_lstat
.globl _Z_sys64_poll
.globl _Z_sys64_lseek
.globl _Z_sys64_mmap
.globl _Z_sys64_mprotect
.globl _Z_sys64_munmap
.globl _Z_sys64_brk
.globl _Z_sys64_rt_sigaction
.globl _Z_sys64_rt_sigprocmask
.globl _Z_sys64_sigreturn
.globl _Z_sys64_ioctl
.globl _Z_sys64_pread64
.globl _Z_sys64_pwrite64
.globl _Z_sys64_readv
.globl _Z_sys64_writev
.globl _Z_sys64_access
.globl _Z_sys64_pipe
.globl _Z_sys64_select
.globl _Z_sys64_sched_yield
.globl _Z_sys64_mremap
.globl _Z_sys64_msync
.globl _Z_sys64_mincore
.globl _Z_sys64_madvise
.globl _Z_sys64_shmget
.globl _Z_sys64_shmat
.globl _Z_sys64_shmctl
.globl _Z_sys64_dup
.globl _Z_sys64_dup2
.globl _Z_sys64_pause
.globl _Z_sys64_nanosleep
.globl _Z_sys64_getitimer
.globl _Z_sys64_alarm
.globl _Z_sys64_setitimer
.globl _Z_sys64_getpid
.globl _Z_sys64_sendfile
.globl _Z_sys64_socket
.globl _Z_sys64_connect
.globl _Z_sys64_accept
.globl _Z_sys64_sendto
.globl _Z_sys64_recvfrom
.globl _Z_sys64_sendmsg
.globl _Z_sys64_recvmsg
.globl _Z_sys64_shutdown
.globl _Z_sys64_blind
.globl _Z_sys64_listen
.globl _Z_sys64_getsockname
.globl _Z_sys64_getpeername
.globl _Z_sys64_socketpair
.globl _Z_sys64_setsockopt
.globl _Z_sys64_getsockopt
.globl _Z_sys64_clone
.globl _Z_sys64_fork
.globl _Z_sys64_vfork
.globl _Z_sys64_execve
.globl _Z_sys64_exit
.globl _Z_sys64_wait4
.globl _Z_sys64_kill
.globl _Z_sys64_uname
.globl _Z_sys64_semget
.globl _Z_sys64_semop
.globl _Z_sys64_semctl
.globl _Z_sys64_shmdt
.globl _Z_sys64_msgget
.globl _Z_sys64_msgsnd
.globl _Z_sys64_msgrcv
.globl _Z_sys64_msgctl
.globl _Z_sys64_fcntl
.globl _Z_sys64_flock
.globl _Z_sys64_fsync
.globl _Z_sys64_fdatasync
.globl _Z_sys64_truncate
.globl _Z_sys64_ftruncate
.globl _Z_sys64_getdents
.globl _Z_sys64_getcwd
.globl _Z_sys64_chdir
.globl _Z_sys64_fchdir
.globl _Z_sys64_rename
.globl _Z_sys64_mkdir
.globl _Z_sys64_rmdir
.globl _Z_sys64_creat
.globl _Z_sys64_link
.globl _Z_sys64_unlink
.globl _Z_sys64_symlink
.globl _Z_sys64_readlink
.globl _Z_sys64_chmod
.globl _Z_sys64_fchmod
.globl _Z_sys64_chown
.globl _Z_sys64_fchown
.globl _Z_sys64_lchown
.globl _Z_sys64_umask
.globl _Z_sys64_gettimeofday
.globl _Z_sys64_getrlimit
.globl _Z_sys64_getrusage
.globl _Z_sys64_sysinfo
.globl _Z_sys64_times
.globl _Z_sys64_ptrace
.globl _Z_sys64_getuid
.globl _Z_sys64_syslog
.globl _Z_sys64_getgid
.globl _Z_sys64_setuid
.globl _Z_sys64_setgid
.globl _Z_sys64_geteuid
.globl _Z_sys64_getegid
.globl _Z_sys64_setpgid
.globl _Z_sys64_getppid
.globl _Z_sys64_getpgrp
.globl _Z_sys64_setsid
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_
.globl _Z_sys64_

_Z_sys64_read:
    mov $0, %rax
    syscall
    ret
_Z_sys64_write:
    mov $1, %rax
    syscall
    ret
_Z_sys64_open:
    mov $2, %rax
    syscall
    ret
_Z_sys64_close:
    mov $3, %rax
    syscall
    ret
_Z_sys64_stat:
    mov $4, %rax
    syscall
    ret
_Z_sys64_fstat:
    mov $5, %rax
    syscall
    ret
_Z_sys64_lstat:
    mov $6, %rax
    syscall
    ret
_Z_sys64_poll:
    mov $7, %rax
    syscall
    ret
_Z_sys64_lseek:
    mov $8, %rax
    syscall
    ret
_Z_sys64_mmap:
    mov %rcx, %r10
    mov $9, %rax
    syscall
    ret
_Z_sys64_mprotect:
_Z_sys64_munmap:
_Z_sys64_brk:
_Z_sys64_rt_sigaction:
_Z_sys64_rt_sigprocmask:
_Z_sys64_sigreturn:
_Z_sys64_ioctl:
_Z_sys64_pread64:
_Z_sys64_pwrite64:
_Z_sys64_readv:
_Z_sys64_writev:
_Z_sys64_access:
_Z_sys64_pipe:
_Z_sys64_select:
_Z_sys64_sched_yield:
_Z_sys64_mremap:
_Z_sys64_msync:
_Z_sys64_mincore:
_Z_sys64_madvise:
_Z_sys64_shmget:
_Z_sys64_shmat:
_Z_sys64_shmctl:
_Z_sys64_dup:
_Z_sys64_dup2:
_Z_sys64_pause:
_Z_sys64_nanosleep:
_Z_sys64_getitimer:
_Z_sys64_alarm:
_Z_sys64_setitimer:
_Z_sys64_getpid:
_Z_sys64_sendfile:
_Z_sys64_socket:
_Z_sys64_connect:
_Z_sys64_accept:
_Z_sys64_sendto:
_Z_sys64_recvfrom:
_Z_sys64_sendmsg:
_Z_sys64_recvmsg:
_Z_sys64_shutdown:
_Z_sys64_blind:
_Z_sys64_listen:
_Z_sys64_getsockname:
_Z_sys64_getpeername:
_Z_sys64_socketpair:
_Z_sys64_setsockopt:
_Z_sys64_getsockopt:
_Z_sys64_clone:
_Z_sys64_fork:
_Z_sys64_vfork:
_Z_sys64_execve:
_Z_sys64_exit:
_Z_sys64_wait4:
_Z_sys64_kill:
_Z_sys64_uname:
_Z_sys64_semget:
_Z_sys64_semop:
_Z_sys64_semctl:
_Z_sys64_shmdt:
_Z_sys64_msgget:
_Z_sys64_msgsnd:
_Z_sys64_msgrcv:
_Z_sys64_msgctl:
_Z_sys64_fcntl:
_Z_sys64_flock:
_Z_sys64_fsync:
_Z_sys64_fdatasync:
_Z_sys64_truncate:
_Z_sys64_ftruncate:
_Z_sys64_getdents:
_Z_sys64_getcwd:
_Z_sys64_chdir:
_Z_sys64_fchdir:
_Z_sys64_rename:
_Z_sys64_mkdir:
_Z_sys64_rmdir:
_Z_sys64_creat:
_Z_sys64_link:
_Z_sys64_unlink:
_Z_sys64_symlink:
_Z_sys64_readlink:
_Z_sys64_chmod:
_Z_sys64_fchmod:
_Z_sys64_chown:
_Z_sys64_fchown:
_Z_sys64_lchown:
_Z_sys64_umask:
_Z_sys64_gettimeofday:
_Z_sys64_getrlimit:
_Z_sys64_getrusage:
_Z_sys64_sysinfo:
_Z_sys64_times:
_Z_sys64_ptrace:
_Z_sys64_getuid:
_Z_sys64_syslog:
_Z_sys64_getgid:
_Z_sys64_setuid:
_Z_sys64_setgid:
_Z_sys64_geteuid:
_Z_sys64_getegid:
_Z_sys64_setpgid:
_Z_sys64_getppid:
_Z_sys64_getpgrp:
_Z_sys64_setsid:
