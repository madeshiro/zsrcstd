# Zsr x86_64 assembly files
# @file zsyscall64.S - syscall
# @os Windows 7, 8.1, 10
# @os UNIX/Kernel system
# @os GNU Linux (unix-like system)
# @version 2021/08 0.1.0
.att_syntax # AT&T syntax
.include "zsr_asm_const.S"
.data

#### <<< zsyscall / x-platform >>> ####
# > All functions are aligned with syscall tables [64bit]
#
# >> REMINDER // 'Zsr' Calling convention [amd64] <<
# > Syscall : RAX
# > Integers & Pointers : RDI, RSI, RDX, R8, R9, R10
# > Floatings points : %xmm0-%xmm7
#
# PREFIX LIST >>
# - z64sys  : universal (x-platform)
# - z64sysw : windows
# - z64sysu : unix(-like)
#
# //

__z64syscall_fn:
    .quad z64sysread
    .quad z64syswrite
    .quad z64sysfopen
    .quad z64sysclose
    .quad z64sysstdio
    .quad z64syserrno
    .quad z64sysalloc
    .quad z64sysfree
    .quad z64sysexit
    .quad z64sysdup
    .quad z64sysdup2

.text
.globl z64syswcall
# @brief z64syswcall
# @param %rcx (int) - %rax value (fnid)
# @param %rdx, r8, %r9 (quad values)
# @param %xmm1, %xmm3 (float values)
# @param stack (other values)
z64syswcall:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov %rcx, %rax

    add $0x20, %rsp
    pop %rbp
    ret

.globl z64syscall
z64syscall:
    push %rbx
    lea __z64syscall_fn, %rbx
    movq (%rbx, %rax, 8), %rax
    callq *%rax
    pop %rbx
    ret

### z64sys x-platform functions [0 args] ###

.globl z64syserrno
z64syserrno:
    call zgetlasterror
    ret
