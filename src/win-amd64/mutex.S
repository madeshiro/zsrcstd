# Windows AMD64 assembly files
# @file mutex.S - system call
# @version 2020/11 0.3.0
.att_syntax
.data

_WIN32_WAIT_ABANDONED: .int 0x80
_WIN32_WAIT_OBJECT_0: .int 0x0
_WIN32_WAIT_TIMEOUT: .int 0x102
_WIN32_WAIT_FAILED: .int 0xffffffff
_WIN32_INFINITE: .int 0xffffffff

_Z_MUTEX_LOCKSUCCESS: .int 0x0
_Z_MUTEX_LOCKTIMEOUT: .int 0x1
_Z_MUTEX_LOCKFAILED:  .int 0xffffffff
_Z_MUTEX_INVALID:     .int 0xe0

.text

#### <<< MUTEX / win-amd64 >>> #####
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_MUTEX DESCRIPTION << ##
# :: zsr_mutex {
#
#   0x0  > (8bytes) HANDLE - mutex's handle
#
# };
# > zsr_mutex's size = (0x8) 8 bytes
#
#
# \/

.globl zMutexCreate
.globl zMutexDestroy
.globl zMutexLock
.globl zMutexUnlock
.globl zMutexHoldsLock

# @brief zMutexCreate
# @return %rax (zsr_mutex)
zMutexCreate:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov $0x0, %rcx
    mov $0, %rdx
    mov $0, %r8
    callq CreateMutexA
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zMutexDestroy
# @param %rcx (zsr_mutex)
# @return %rax (bool)
zMutexDestroy:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    callq CloseHandle
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zMutexLock
# @param %rcx (zsr_mutex)
# @return %rax (zenum)
zMutexLock:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov %rcx, -0x8(%rbp)
    mov _WIN32_INFINITE, %edx
    call WaitForSingleObject
    cmpl _WIN32_WAIT_OBJECT_0, %eax
    je 1f
    cmpl _WIN32_WAIT_ABANDONED, %eax
    je 2f
    cmpl _WIN32_WAIT_TIMEOUT, %eax
    je 3f
    cmpl _WIN32_WAIT_FAILED, %eax
    je 4f
    mov $-1, %eax
    jmp 5f
4:  movl _Z_MUTEX_LOCKFAILED, %eax
    jmp 5f
3:  movl _Z_MUTEX_LOCKTIMEOUT, %eax
    jmp 5f
2:  movl _Z_MUTEX_INVALID, %eax
    jmp 5f
1:  movl _Z_MUTEX_LOCKSUCCESS, %eax
5:  add $0x20, %rsp
    pop %rbp
    retq

# @brief zMutexUnlock
# @param %rcx (zsr_mutex)
# @return %rax (zbool)
zMutexUnlock:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    call ReleaseMutex
    add $0x20, %rsp
    pop %rbp
    retq

#### <<< COND / win-amd64 >>> #####
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_cond DESCRIPTION << ##
# :: zsr_cond {
#
#   0x0  > (8bytes) HANDLE - cond's handle
#
# };
# > zsr_cond's size = (0x8) 8 bytes
#
#
# \/

