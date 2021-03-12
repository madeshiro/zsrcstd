# Zsr x86_64 assembly files
# @file zsyscall64.S - syscall
# @os Windows 7, 8.1, 10
# @os UNIX/Kernel system
# @os GNU Linux (unix-like system)
# @version 2020/12 0.4.0
.att_syntax # AT&T syntax
.data

#### <<< zsyscall / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // 'Zsr' Calling convention [amd64] <<
# > Syscall : RAX
# > Integers & Pointers : RDI, RSI, RCX, RDX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
# PREFIX LIST >>
# - z64sys  : universal (x-platform)  # define in 'zsyscall.h'
# - z64sysw : windows
# - z64sysu : unix(-like)
#
# //

__z64syscall_fn:
.quad z64sysfopen

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

    add $0x20, %rsp
    pop %rbp
    ret

.globl z64syscall
z64syscall:
    ret

.globl z64sysfopen
z64sysfopen:
    ret