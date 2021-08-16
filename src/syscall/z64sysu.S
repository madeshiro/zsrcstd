# Zsr x86_64 assembly file for unix-x86_64 interface
# @file z64sysu.S - syscall
# @os UNIX/Kernel system
# @os GNU Linux (unix-like system)
# @version 2021/08 0.1.0
.att_syntax # AT&T syntax
.include "zsr_asm_const.S"
.include "z64sys.S"
.text

#### <<< z64sysu / interface <=> implementation >>> ####
# > All functions are aligned with syscall tables [64bit]
#
# >> REMINDER // 'Zsr' Calling convention [amd64] <<
# > Syscall : RAX
# > Integers & Pointers : RDI, RSI, RDX, R8, R9, R10
# > Floatings points : %xmm0-%xmm7
#
# >> REMINDER // `unix-x86_64` Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
#
# PREFIX LIST >>
# - z64sys  : universal (x-platform)  | define in 'zsyscall.S'
# - z64sysw : windows
# - z64sysu : unix(-like)
#
# //

.globl z64sysread
z64sysread:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdi, -0x8(%rbp)
    add $0x10, %rsp
    pop %rbp
    ret

.globl z64syswrite
z64syswrite:
    ret

.globl z64sysfopen
z64sysfopen:

    ret

.globl z64sysclose
z64sysclose:
    call zclosehandler
    ret

.globl z64sysstdio
z64sysstdio:
    ret

.globl z64sysalloc
z64sysalloc:
    cmp $ZSYS_ALLOC_MAP, %esi
    jne 1f
    call _Z_alloc
    ret
1:  cmp $ZSYS_ALLOC_ZERO, %esi
    jne 1f
    call _Z_calloc
    ret
1:  cmp $ZSYS_ALLOC_REMAP, %esi
    jne 1f
    mov %edi, %esi
    mov %rdx, %rdi
    call _Z_realloc
    ret
1:  mov $ZEINVAL, %edi
    call zsetlasterror
    mov $0, %rax
    ret
