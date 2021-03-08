# Unix x86_64 assembly files
# @file thread.S - system call
# @version 2021/03 0.3.0.0004
.att_syntax
.text

# TODO futex.S_unix

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
#   0x4 > (4bytes) uint32 - status     ] data
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

