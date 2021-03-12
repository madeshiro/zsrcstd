# Unix x86_64 assembly files
# @file thread.S - system call
# @version 2021/03 0.4.1
.att_syntax
.text

.set KILL_SIGTERM, 15
.set KILL_SIGKILL, 9

.set CSIGNAL,                 0x000000ff
.set CLONE_VM,                0x00000100
.set CLONE_FS,                0x00000200
.set CLONE_FILES,             0x00000400
.set CLONE_SIGHAND,           0x00000800
.set CLONE_PTRACE,            0x00002000
.set CLONE_VFORK,             0x00004000
.set CLONE_PARENT,            0x00008000
.set CLONE_THREAD,            0x00010000
.set CLONE_NEWNS,             0x00020000
.set CLONE_SYSVSEM,           0x00040000
.set CLONE_SETTLS,            0x00080000
.set CLONE_PARENT_SETTID,     0x00100000
.set CLONE_CHILD_CLEARTID,    0x00200000
.set CLONE_DETACHED,          0x00400000
.set CLONE_UNTRACED,          0x00800000
.set CLONE_CHILD_SETTID,      0x01000000
.set CLONE_NEWCGROUP,         0x02000000
.set CLONE_NEWUTS,            0x04000000
.set CLONE_NEWIPC,            0x08000000
.set CLONE_NEWUSER,           0x10000000
.set CLONE_NEWPID,            0x20000000
.set CLONE_NEWNET,            0x40000000
.set CLONE_IO,                0x80000000

.set SIGCHLD, 20

.set WNOHANG,		0x00000001
.set WUNTRACED,	    0x00000002
.set WSTOPPED,	    WUNTRACED
.set WEXITED,		0x00000004
.set WCONTINUED,	0x00000008
.set WNOWAIT,		0x01000000

.set __WNOTHREAD,	0x20000000	# Don't wait on children of other threads in this group
.set __WALL,		0x40000000	# Wait on all children, regardless of type
.set __WCLONE,	    0x80000000	# Wait only on non-SIGCHLD children

# First argument to waitid: #
.set P_ALL,	    0
.set P_PID, 	1
.set P_PGID,    2

#### <<< THREAD / unix-x86_64 >>> #####
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
## >> ZSR_THREAD DESCRIPTION << ##
# :: zsr_thread {
#
#   0x0  > (4bytes) uint32 - handler id         ]  he\
#   0x4  > (4bytes) uint32 - tid                ]    a\d
#   0x8  > (8bytes) char*  - name               ]      \er
#   0x10 > (8bytes) int (*)(void*)  - function  ] d\
#   0x18 > (4bytes) int32           - ret       ]  a\t
#   0x1c > (4byte)  bool            - running   ]    \a
#
# };
# > zsr_thread's size = (0x20) 32 bytes
#
#
# \/

.globl _Z_sys_clone
.globl _Z_sys_futex
.globl _Z_sys_tgkill
.globl _Z_sys_getpid
.globl _Z_sys_nanosleep

_Z_sys_getpid:
    mov $0x27, %rax
    syscall
    ret

# @brief _Z_sys_clone
# @param %rdi (enum) - flags
# @param %rsi (void*) - stacks
# @param %rdx (void*) - parent_tid
# @param %r10 (void*) - child_tid
# @param %r8  (uint) - tid
_Z_sys_clone:
    mov %rcx, %r10
    mov $0x38, %rax
    syscall
    ret

# @brief _Z_sys_tgkill
# @param %rdi (int) - tgid
# @param %rsi (int) - tid
# @param %rdx (int) - sig (0x9 = SIGKILL)
_Z_sys_tgkill:
    mov $0xea, %rax
    syscall
    ret

# @brief _Z_sys_nanosleep
# @param %rdi (timespec_t)
# @param %rsi (timespec_t)
_Z_sys_nanosleep:
    mov $0x23, %rax
    syscall
    ret

# @brief _Z_sys_waitid
# @param %edi (int) - which
# @param %esi (pid_t) - pid
# @param %rdx (siginfo_t*) - infop
# @param %r8d (int) - opt
_Z_sys_waitid:
    mov %r8d, %r10d
    mov $0xf6, %rax
    syscall
    ret

# @brief __zsr_thread_saferunning
# @param %rdi (zsr_thread) - thread
# @param %rsi (void*) - args
# @exit
__zsr_thread_saferunning:
    push %rdi
    mov 0x10(%rdi), %rax
    mov %rsi, %rdi
    callq *%rax
    popq %rsi
    mov %eax, %edi
    mov %edi, 0x18(%rsi)
    movl $0x0, (%rsi)
    mov %rsi, %rdi
    call __zsr_thread_safestop
    mov $0x0, %rdi
    mov $0x3c, %rax
    syscall
    pop %rbp
    ret

# @brief __zsr_thread_safestop
# @param %rdi (zsr_thread) - thread
__zsr_thread_safestop:
    movl $0x0, 0x1c(%rdi)
    lea 0x1c(%rdi), %rdi
    mov $0x7fffffff, %esi # INT_MAX
    call _Z_sys_futex
    ret

.globl zThreadCreate
.globl zThreadDestroy
.globl zThreadStart
.globl zThreadJoin
.globl zThreadIsRunning
.globl zThreadCancel

# %rdi	flags  (enum)
# %rsi	stacks (void*)
# %rdx	parent_tid (void*) = null
# %r10	child_tid (void*)  = null
# %r8	tid (uint)	   = null
#
# -----
#
# stacks : set pos to higher address -> decrease addr op (x64: 8192*1024 = 8Mio)
# > -0x8(%rax) = args
# > -0x10(%rax) = function
#
# -----
#
# ZSR_THREAD needed vars :
# - TID (int)	    	32bits - 4bytes
# - name (char*)		64bits - 8bytes
# - function (void*)	64bits - 8bytes
# - ret (int)		    32bits - 4bytes
# - running (bool)	     8bits - 1byte
# === total : 25bytes = 0x19 ===
#
# flags = 0x018b11


# @brief zThreadCreate
# @param %rdi (int (*)(void*)) - function
# @return %rax (zsr_thread)
zThreadCreate:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    mov $0x20, %rdi
    callq _Z_calloc
    mov -0x8(%rbp), %rdx
    mov %rdx, 0x10(%rax)
    add $0x10, %rsp
    pop %rbp
    ret

# @brief zThreadDestroy
# @param %rdi (void*) - zsr_thread
zThreadDestroy:
    mov $0, %rax
    cmpb $0x0, 0x1c(%rdi)
    jne 1f
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    mov 0x4(%rdi), %rsi
    test %rsi, %rsi
    je 2f
    mov %rsi, %rdi
    call _Z_free
    mov -0x8(%rbp), %rdi
 2: call _Z_free
    add $0x10, %rsp
    pop %rbp
 1: ret


# @brief zThreadStart
# @param %rdi (void*) - zsr_thread
# @param %rsi (void*) - params
zThreadStart:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    mov %rsi, -0x10(%rbp)
    mov $0x800000, %rdi # 8Mio
    callq _Z_alloc
    lea 0x800000(%rax), %rax
    mov -0x8(%rbp), %rdi
    mov -0x10(%rsp), %rsi
    mov %rdi, -0x8(%rax)
    mov %rsi, -0x10(%rax)
    mov $0x18b14, %edi # CLONE_VM|CLONE_FS|CLONE_PARENT|CLONE_SIGHAND|CLONE_THREAD|SIGCHLD
    mov %rax, %rsi
    mov $0x0, %rdx
    mov $0x0, %r10
    mov $0x0, %r8
    mov $0x38, %rax
    syscall # sys_clone
    cmp $0, %eax
    jne 2f
    mov -0x8(%rbp), %rdi
    mov -0x10(%rbp), %rsi
    sub $0x10, %rsp
    call __zsr_thread_saferunning
 2: cmpl $-1, %eax
    je 1f
    mov -0x8(%rbp), %rdi
    mov %eax, 0x4(%rdi)
    movb $0x1, 0x1c(%rdi)
 1: add $0x10, %rsp
    pop %rbp
    ret

# @brief zThreadJoin
# @param %rdi (void*) - thread
zThreadJoin:
    lea 0x1c(%rdi), %rdi
    mov $0x0, %rsi # FUTEX_WAIT
    mov $0x1, %edx # IF thread running, wait
    mov $0x0, %r10 # INFINITE WAIT (timespec* = null)
    call _Z_sys_futex
    ret

# @brief zThreadCancel
# @param %rdi (void*) - thread
# @return %rax (bool)
zThreadCancel:
    mov $0, %rax
    mov 0x1c(%rdi), %al
    cmpb $0, %al
    je 1f
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    call _Z_sys_getpid
    mov %rax, %rdi
    mov -0x8(%rbp), %rsi
    mov 0x4(%rsi), %esi
    mov $0x9, %edx
    call _Z_sys_tgkill
    mov -0x8(%rbp), %rdi
    call _zsr_thread_safestop
    movl $0x0, 0x4(%rdi)
    movl $0x9, 0x18(%rdi)
    add $0x1, %eax
    add $0x10, %rsp
    pop %rbp
 1: ret

# @brief zThreadIsRunning
# @param %rdi (void*) - zsr_thread
# @return %ah (bool)
zThreadIsRunning:
    mov 0x1c(%rdi), %ah
    ret

.globl zThreadSetName
.globl zThreadGetName
.globl zThreadTID
.globl zThreadGetPID

# @brief zThreadGetPID
# @return %rax (uint) - process id
zThreadGetPID:
    mov $0x27, %rax
    syscall
    ret

# @brief zThreadSetName
# @param %rdi (void*) - zsr_thread
# @param %rsi (char*) - name
zThreadSetName:
    push %rdi
    mov %rsi, %rdi
    callq zstrcopy
    pop %rdi
    mov %rax, 0x8(%rdi)
    ret

# @brief zThreadGetName
# @param %rdi (void*) - zsr_thread
# @return %rax (char*)
zThreadGetName:
    mov 0x8(%rdi), %rax
    ret

# @brief zThreadTID
# @param %rdi (void*) - zsr_thread
# @return %rax (zulong)
zThreadTID:
    movl 0x4(%rdi), %eax
    ret

