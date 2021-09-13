# Unix x86_64 assembly files
# @file std.S - system call
# @version 2021/09 0.5.0
.att_syntax
.text
.include "zsr_asm_const.S"
.include "syscall64.S"

#### <<< STDLIB / unix-x86_64 >>> #####
# > functions declared in `unix-86_64\zsr_std.c` (unix-x86_64 decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
## > Memory Management --- All related functions < ##
# :: ptr structure {
#
#   int64 ptr_size ] header
#   [...] data
# };
#
# //

.globl __z_xapi_alloc
# @brief __z_xapi_alloc : X/API realated
# @param %rdi (zsize_t) - alloc size
# @param %si (bool) - Zero Memory
# @return %rax (void*) - pointer to new memory block, 0 on error
__z_xapi_alloc:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov %rdi, -0x8(%rbp)
    mov %si, -0xa(%rbp)
    add $0x8, %rdi
    mov $-0x1, %r8  # offset [not used]
    mov $0x00, %r9  # fd     [not used]
    mov $0x22, %r10 # flags
    mov $0x03, %rdx # prot
    mov %rdi, %rsi  # length
    mov $0x0, %rdi  # NULL ptr
    mov $zu64sys_mmap, %rax
    syscall
    cmp $-1, %rax
    je 3f
    mov -0x8(%rbp), %rdi
    mov -0xa(%rbp), %si
    mov %rdi, (%rax)
    add $0x8, %rdi
    cmp $0, %si
    je 2f
    mov $0x0, %rdx
 1: cmp %rdi, %rdx
    jae 2f
    movb $0x0, (%rax,%rdx,1)
    inc %rdx
    jmp 1b
 3: call __zu64_errno
    mov $0x0, %rax
 2: add $0x20, %rsp
    pop %rbp
    ret

.globl __z_xapi_free
# @brief __z_xapi_free : X/API related
# @param %rdi (void*) - ptr to free
# @return %rax (int)
__z_xapi_free:
    sub $0x8, %rdi
    mov (%rdi), %rsi
    mov $zu64sys_munmap, %rax
    syscall
    ret

.globl __z_xapi_realloc
# @brief __z_xapi_realloc
# @param %rdi (void*)   - ptr
# @param %rsi (size_t)  - sizeof
# @return %rax (void*)  - new ptr
__z_xapi_realloc:
    mov %rsi, %rdx
    mov $0x1, %r10
    mov -0x8(%rdi), %rsi
    mov $zusys_mremap, %rax
    syscall
    ret

.globl __z_xapi_memsize
# @brief __z_xapi_memsize
# @param %rdi (void*) - ptr
# @return %rax (size_t) - memsize
__z_xapi_memsize:
    mov -0x8(%rdi), %rax
    ret

.globl __z_xapi_exit
# @brief __z_xapi_exit
# @param %edi (int) - code
__z_xapi_exit:
    mov $zu64sys_exit, %rax
    syscall

.globl __z_xapi_mkdir
# @brief __z_xapi_mkdir
# @param %rdi (char*) - dirname
# @return %rax (bool)
__z_xapi_mkdir:
    mov $0040000, %rsi
    or $00777, %rsi
    mov $zu64sys_mkdir, %rax
    inc %rax
    ret

.globl __z_xapi_rmdir
# @brief __z_xapi_rmdir
# @param %rdi (char*) - dirname
# @return %rax (bool)
__z_xapi_rmdir:
    mov $zu64sys_rmdir, %rax
    syscall
    inc %rax
    test %rax, %rax
    jne 1f
    call __zu64_errno@plt
    mov $0x0, %rax
 1: ret

.globl __z_xapi_sys_getinfo
# @brief __z_xapi_sys_getinfo
# @param %rdi (enum) - which
# @return %rax (void*) - (struct sysinfo)
__z_xapi_sys_getinfo:   # TODO (unix-x86_64) X/API sys_getinfo
    mov $0x0, %rax
    ret
