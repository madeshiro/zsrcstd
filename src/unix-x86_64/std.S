# Unix x86_64 assembly files
# @file std.S - system call
# @version 2020/11 0.3.0
.att_syntax
.text
.include "zsr_asm_const.S"

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

.globl _Z_alloc
.globl _Z_calloc
.globl _Z_realloc
.globl _Z_free
.globl _Z_allocs
.globl _Z_mcopy
.globl _Z_mswap
.globl _Z_hndlid

# @brief _Z_sys_mmap (simplified)
# @param %rdi (size_t) - length
# @return %rax (void*) valid pointer on success, -1 (void*) on failure
_Z_sys_mmap:
    mov $-0x1, %r8  # offset [not used]
    mov $0x00, %r9  # fd     [not used]
    mov $0x22, %r10 # flags
    mov $0x03, %rdx # prot
    mov %rdi, %rsi  # length
    mov $0x0, %rdi  # NULL ptr
    mov $0x9, %rax  # sys_mmap
    syscall
    retq

# @brief _Z_sys_munmap
# @param %rdi (void*) - ptr
# @return %rax (int) - status
_Z_sys_munmap:
    sub $0x8, %rdi
    movq (%rdi), %rsi   # length
    movq $0xb, %rax         # sys_munmap
    syscall
    retq

# @brief _Z_alloc
# @param %rdi (size_t) - size
# @return %rax (void*) - ptr or NULL
_Z_alloc:
    push %rdi
    addq $0x8, %rdi # add 8 bytes
    call _Z_sys_mmap@plt
    cmp $-0x1, %rax
    je 1f
    pop %rdi
    movq %rdi, (%rax)
    addq $0x8, %rax
    retq
1: # on failure
    call _Z_errno
    mov $0x0, %rax
    retq

# @brief _Z_calloc
# @param %rdi (size_t) - size
# @return %rax (void*) - ptr or NULL
_Z_calloc:
    push %rdi
    callq _Z_alloc
    pop %rdi
    mov $0x0, %rsi
1:  cmp %rdi, %rsi
    jae 1f
    movb $0x0, (%rax, %rsi, 1)
    inc %rsi
    jmp 1b
1:  retq

# @brief _Z_realloc
# @param %rdi (void*) - ptr
# @param %rsi (size_t) - size
_Z_realloc:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    # Set local vars
    mov %rdi, -0x8(%rbp)

    # Create new pointer
    mov %rsi, %rdi
    callq _Z_alloc

    mov -0x8(%rbp), %rdi # old_ptr
    mov -0x8(%rax), %rsi # new_size
    cmp %rsi, -0x8(%rdi)
    jae 1f
    mov -0x8(%rdi), %rsi
1:  mov $0, %rcx
2:  cmp %rsi, %rcx
    jae 2f
    movb (%rdi, %rcx, 1), %bl
    movb %bl, (%rax, %rcx, 1)
    inc %rcx
    jmp 2b
2:  mov -0x8(%rax), %rsi
3:  cmp %rsi, %rcx
    jae 3f
    movb $0x0, (%rax, %rcx,1)
    inc %rcx
    jmp 3b
3:  mov %rax, -0x8(%rbp)
    callq _Z_free
    mov -0x8(%rbp), %rax
    add $0x10, %rsp
    pop %rbp
    retq

# @brief _Z_free
# @param %rdi (void*) - ptr
# @return %rax (bool) - status
_Z_free:
    callq _Z_sys_munmap
    inc %rax
    cmp $0x0, %rax
    jne 1f
    mov $ZEINVAL, %edi
    call zsetlasterror
    mov $0x0, %rax
1:  retq


# @brief _Z_allocs
# @param %rdi (void*) - ptr
# @return %rax (int64) - size
_Z_allocs:
    movq -0x8(%rdi), %rax
    retq

# @brief _Z_mcopy
# @param %rdi (const void*) - source
# @param %rsi (void*) - dest
# @param %rdx (size_t) - size
_Z_mcopy:
    mov $0x0, %rcx
1:  cmp %rdx, %rcx
    jae 1f
    movb (%rdi, %rcx, 1), %bl
    movb %bl, (%rsi, %rcx, 1)
    inc %rcx
    jmp 1b
1:  mov %rsi, %rax
    retq

# @brief _Z_mswap
# @param %rdi (void*)
# @param %rsi (void*)
# @param %rdx (size_t)
_Z_mswap:
    mov $0x0, %rax
1:  cmp %rdx, %rax
    jae 1f
    movb (%rdi, %rcx, 1), %cl
    movb (%rsi, %rcx, 1), %bl
    movb %bl, (%rdi, %rcx, 1)
    movb %cl, (%rsi, %rcx, 1)
    inc %rcx
    jmp 1b
1:  retq

# @brief _Z_hndlid
# @param %rdi (zhandler) - handler
# @return %eax (int) - id
_Z_hndlid:
    mov 0x0(%rdi), %eax
    ret
