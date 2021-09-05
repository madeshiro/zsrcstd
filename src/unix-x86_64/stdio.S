# Unix x86_64 assembly files
# @file stdio.S - system call
# @version 2021/09 0.5.0
.att_syntax
.text
.include "zsr_asm_const.S"
.include "syscall64.S"

#### <<< stdio.S / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
# //

.globl __z_xapi_dup
# @brief __z_xapi_dup
# @param %rdi (int) - fd
# @return %rax (int)
__z_xapi_dup:
    mov $zu64sys_dup, %rax
    syscall
    ret

.globl __z_xapi_dup2
# @brief __z_xapi_dup2
# @param %edi (int) - oldfd
# @param %esi (int) - newfd
# @return %rax (int) - the new fd
__z_xapi_dup2:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov %edi, -0x4(%rbp)
    mov %esi, -0x8(%rbp)
    lea -0xa(%rbp), %rdx
    call __zu64_dup2_@plt
    cmpw $0x1, -0xa(%rbp)
    je 1f
    mov -0x4(%rbp), %edi
    mov -0x8(%rbp), %esi
    mov $zu64sys_dup2, %rax
    syscall
 1: add $0x20, %rsp
    pop %rbp
    ret

.globl zu64_dup2
zu64_dup2:
    mov $zu64sys_dup2, %rax
    syscall
    ret

.globl __z_xapi_read
# @brief __z_xapi_read
# @param %rdi (char*) # _Z_out - buf
# @param %rsi (ulong) - len
# @param %rdx (fd_t) - fd
# @return %eax (int) - read len
__z_xapi_read:
    xchg %rdi, %rdx
    xchg %rdx, %rsi
    mov $zu64sys_read, %rax
    syscall
    ret

.globl __z_xapi_write
# @brief __z_xapi_write
# @param %rdi (const char*) - buf
# @param %rsi (ulong) - len
# @param %rdx (fd_t) - fd
# @return %eax (int) - wlen
__z_xapi_write:
    xchg %rdi, %rdx
    xchg %rdx, %rsi
    mov $zu64sys_write, %rax
    syscall
    ret

.globl __z_xapi_lseek
# @brief __z_xapi_lseek
# @param %edi (long) - offset
# @param %esi (int) - whence
# @param %rdx (fd_t) - fd
__z_xapi_lseek:
    xchg %rdi, %rdx
    xchg %rdx, %rsi
    mov $zu64sys_lseek, %rax
    syscall
    ret

.globl __z_xapi_fclose
# @brief __z_xapi_fclose
# @param %rdi (ZHFILE)
# @note (it -> fd) .size 4 @ 0x8 (%zhfile)
# @return %rax (zbool)
__z_xapi_fclose:
    movl 0x8(%rdi), %edi
    mov $zu64sys_close, %rax
    syscall
    inc %eax
    test %eax, %eax
    jne 1f
    call __zu64_errno@plt
    mov $0x0, %rax
 1: ret

.globl  __z_xapi_fread
# @brief __z_xapi_fread
# @param %rdi (char*) - buf
# @param %rsi (ulong) - len
# @param %rdx (ZHFILE)
# @return %rax (int) - read
__z_xapi_fread:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rdx, -0x8(%rbp)
    xchg %rdx, %rdi
    xchg %rdx, %rsi
    mov 0x8(%rdi), %edi
    mov $zu64sys_read, %rax
    syscall

