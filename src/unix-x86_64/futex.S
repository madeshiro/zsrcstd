# Unix x86_64 assembly files
# @file thread.S - system call
# @version 2021/03 0.4.1
.att_syntax
.text
.include "zsr_asm_const.S"

.set FUTEX_PRIVATE_FLAG,      128
.set FUTEX_CLOCK_REALTIME,	  256

.set FUTEX_WAIT,              0x0
.set FUTEX_WAKE,              0x1
.set FUTEX_FD,                0x2
.set FUTEX_REQUEUE,           0x3
.set FUTEX_CMP_REQUEUE,       0x4
.set FUTEX_WAKE_OP,           0x5
.set FUTEX_LOCK_PI,           0x6
.set FUTEX_UNLOCK_PI,         0x7
.set FUTEX_TRYLOCK_PI,        0x8
.set FUTEX_WAIT_BITSET,       0x9
.set FUTEX_WAKE_BITSET,       0xa
.set FUTEX_WAIT_REQUEUE_PI,	  0xb
.set FUTEX_CMP_REQUEUE_PI,	  0xc
.set SIGCHLD, 20

#### <<< THREAD / unix-x86_64 >>> #####
# > functions declared in `unix-86_64\zsr_stdio.c` (unix-x86_64 decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
## >> ZSR_MUTEX DESCRIPTION << ##
# :: zsr_mutex {
#
#   0x0 > (4bytes) uint32 - handler id ] header
#   0x4 > (4bytes) uint32 - status     ] da\
#   0x8 > (4bytes) uint32 - tid        ]    \ta
# };
#
# > sizeof(zsr_mutex) = 8bytes
#
## >> ZSR_COND DESCRIPTION << ##
# :: zsr_cond {
#   0x0 > (4bytes) uint32 - handler id ] header
#   0x4 > (4bytes) uint32 - condition  ] data
# };
#
# > sizeof(zsr_cond) = 8bytes
# \/

.globl _Z_sys_futex

# @brief _Z_sys_futex
# @param %rdi (u32*) uaddr
# @param %rsi (int) futex_op
# @param %rdx (u32) val
# @param %r10 (void*) timespec:timeout
# @param %r8  (u32*) uaddr2
# @param %r9  (u32) val3
# @return
_Z_sys_futex:
    mov $0xCA, %rax
    syscall
    ret

.globl zMutexCreate
zMutexCreate:
    mov $0xc, %rdi
    call _Z_alloc
    movl $0x00000081, (%rax)
    movl $ZMUTEX_UNLOCK, 0x4(%rax)
    movl $0x0, 0x8(%rax)
    ret

.globl zMutexDestroy
zMutexDestroy:
    cmpl $ZMUTEX_LOCK, 0x4(%rdi)
    jne 1f
    mov $ZEBUSY, %edi
    call zsetlasterror
    mov $0x0, %rax
    ret
1:  call _Z_free
    ret

.globl zMutexLock
# @brief zMutexLock
# @param %rdi (zsr_mutex)
# @return %rax (bool) - True if mutex has been locked
zMutexLock:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    mov $0x0, %r10
    mov $ZMUTEX_LOCK, %edx
    mov $FUTEX_WAIT, %esi
    lea 0x4(%rdi), %rdi
    call _Z_sys_futex
    mov -0x8(%rbp), %rdi
    mov $ZMUTEX_UNLOCK, %eax
    mov $ZMUTEX_LOCK, %edi
    lock cmpxchg %edi, 0x4(%rdi)
    je 1f
    mov $ZENOLCK, %edi
    call zsetlasterror
    mov $0x0, %rax
    jmp 2f
1:  call zThreadGetPID
    mov -0x8(%rbp), %rdi
    mov %eax, 0x8(%rdi)
    mov $0x1, %rax
2:  add $0x10, %rsp
    pop %rbp
    ret

.globl zMutexUnlock
# @brief zMutexUnlock
# @param %rdi (zsr_mutex)
# @return %rax (bool) - True if mutex has been unlocked
zMutexUnlock:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    call zThreadGetPID
    mov %eax, -0xc(%rbp)
    mov %eax, %esi
    mov -0x8(%rbp), %rdi
    cmpl %esi, 0x8(%rdi)
    jne 1f
    mov $ZMUTEX_LOCK, %eax
    mov $ZMUTEX_UNLOCK, %esi
    lock cmpxchg %esi, 0x4(%rdi)
    jne 1f
    movl $0x0, 0x8(%rdi)
    lea 0x4(%rdi), %rdi
    mov $FUTEX_WAKE, %esi
    mov $0x1, %r10d
    call _Z_sys_futex
    mov $0x1, %rax
    jmp 2f
1:  mov $ZEINVAL, %edi
    call zsetlasterror
    mov $0x0, %rax
2:  add $0x10, %rsp
    pop %rbp
    ret

.globl zMutexIsLock
# @brief zMutexIsLock
# @param %rdi (zsr_mutex)
# @return %rax (bool) - Mutex lock status
zMutexIsLock:
    mov $0x1, %rsi
    cmpl $ZMUTEX_LOCK, 0x4(%rdi)
    mov $0x0, %rax
    cmove %rsi, %rax
    ret

.globl zCondCreate
# @brief zCondCreate
# @return %rax (zsr_cond)
zCondCreate:
    mov $0x8, %rdi
    call _Z_alloc
    movl $0x0, 0x4(%rax)
    movl $ZSR_HANDLER_COND, 0x0(%rax)
    movl $ZCOND_FREE, 0x4(%rax)
    ret

.globl zCondDestroy
# @brief zCondDestroy
# @param %rdi (zsr_cond)
# @return %rax (bool)
zCondDestroy:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    mov $ZCOND_FREE, %eax
    lock cmpxchg %eax, 0x4(%rdi)
    je 1f
    movl %eax, 0x4(%rdi)
    call zThreadCondSignal
    test %rax, %rax
    jne 1f
    mov $ZEBUSY, %edi
    call zsetlasterror
    jmp 2f
1:  mov -0x8(%rbp), %rdi
    call _Z_free
2:  add $0x10, %rsp
    pop %rbp
    ret

.globl zThreadCondWait
# @brief zThreadCondWait
# @param %rdi (zsr_cond)
# @param %rsi (zsr_mutex)
zThreadCondWait:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    mov %rsi, -0x10(%rbp)
    mov $ZMUTEX_UNLOCK, %edx
    mov 0x4(%rsi), %eax
    lock cmpxchg %edx, 0x4(%rsi)
    jne 2f
    movl $ZCOND_BUSY, 0x4(%rdi)
    lea 0x4(%rsi), %rdi
    mov $FUTEX_WAKE, %esi
    mov 0x1, %edx
    call _Z_sys_futex
    mov -0x8(%rbp), %rdi
    lea 0x4(%rdi), %rdi
    mov $FUTEX_WAIT, %esi
    mov $ZCOND_BUSY, %edx
    mov $0x0, %r10
    call _Z_sys_futex
    mov -0x10(%rbp), %rdi
    call zMutexLock
    test %rax, %rax
    jne 1f
    mov $ZENOLCK, %edi
    call zsetlasterror
    jmp 1f
2:  mov $ZEINVAL, %edi
    call zsetlasterror
    mov $0x0, %rax
1:  add $0x10, %rsp
    pop %rbp
    ret

.globl zThreadCondSignal
# @brief zThreadCondSignal
# @param %rdi (zsr_cond)
# @return %rax (bool)
zThreadCondSignal:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    lea 0x4(%rdi), %rdi
    mov $FUTEX_WAKE, %esi
    mov $0x7fffffff, %edx
    call _Z_sys_futex
    mov -0x8(%rbp), %rdi
    mov $ZCOND_FREE, %esi
    lock xchg %esi, 0x4(%rdi)
    add $0x10, %rsp
    pop %rbp
    ret