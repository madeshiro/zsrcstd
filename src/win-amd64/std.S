# Windows AMD64 assembly files
# @file std.S - Standard call
# @version 2020/11 0.3.0.0002
.att_syntax # AT&T syntax
.text

#### <<< STDLIB / win-amd64 >>> #####
# > functions declared in `win-amd64\zsr_std.c` (win-amd64 decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## > Memory Management --- All related functions < ##
#
# \/

.globl _Z_alloc
.globl _Z_calloc
.globl _Z_realloc
.globl _Z_free
.globl _Z_allocs
.globl _Z_mcopy
.globl _Z_mswap
.globl _Z_exit

# @brief _Z_alloc :
# @param %rcx (size_t) : pointer size
# @ret %rax (ptr)
_Z_alloc:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x8(%rbp)
    call GetProcessHeap
    mov %rax, %rcx
    mov $0x0, %rdx
    mov -0x8(%rbp), %r8
    call HeapAlloc
    add $0x30, %rsp
    pop %rbp
    retq

# @brief _Z_calloc
# @param %rcx (size_t) : pointer size
# @ret %rax (ptr)
_Z_calloc:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x8(%rbp)
    call GetProcessHeap
    mov %rax, %rcx
    mov $0x8, %rdx
    mov -0x8(%rbp), %r8
    call HeapAlloc
    mov -0x8(%rbp), %rcx
    mov $0, %rdx
 1: cmp %rcx, %rdx
    jae 1f
    movb $0x0, (%rax, %rdx, 1)
    inc %rdx
    jmp 1b
 1: add $0x30, %rsp
    pop %rbp
    retq

# @brief _Z_realloc
# @param %rcx (void*) - ptr
# @param %rdx (size_t) - new size
_Z_realloc:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp

    mov %rcx, -0x8(%rbp)
    mov %rdx, -0x10(%rbp)

    call GetProcessHeap
    mov %rax, %rcx
    mov $0x0, %rdx
    mov -0x8(%rbp), %r8
    call HeapSize
    mov %rax, -0x18(%rbp)

    mov -0x10(%rbp), %rcx
    callq _Z_alloc

    mov %rax, %rdx          # new ptr
    mov -0x8(%rbp), %rcx    # old ptr
    mov -0x18(%rbp), %rax   # old_ptr.size

    # min (old_ptr.size, new_ptr.size)
    cmp %rax, -0x10(%rbp)
    jae 1f
    mov -0x10(%rbp), %rax
1:  mov $0, %r8

# for_loop : transfer data from old to new ptr
2:  cmp %r8, %rax
    je 2f
    movb (%rcx, %r8, 1), %bl
    movb %bl, (%rdx, %r8, 1)
    inc %r8
    jmp 2b
2:  cmp -0x10(%rbp), %r8
    jae 3f
    movb $0x0, (%rdx, %r8, 1)
    inc %r8
    jmp 2b
3:  mov %rdx, -0x8(%rbp)
    callq _Z_free

    mov -0x8(%rbp), %rax
    add $0x40, %rsp
    pop %rbp
    retq

# @brief _Z_free
# @param %rcx (const void*) : pointer to free
# @ret %rax (int) : 0 on success (ZSUCCESS), -1 on failure (ZFAILURE)
_Z_free:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    push %rcx
    callq GetProcessHeap
    pop %r8         # lpMem
    mov $0x0, %edx  # dwFlags
    mov %rax, %rcx  # hHeap
    callq HeapFree

    add $0x20, %rsp
    pop %rbp
    retq

# @brief _Z_allocs
# @param %rcx (void*) - ptr
# @return %rax (size_t) - size
_Z_allocs:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp

    mov %rcx, -0x8(%rbp)
    callq GetProcessHeap
    mov %rax, %rcx
    mov $0x0, %rdx
    mov -0x8(%rbp), %r8
    callq HeapSize

    add $0x30, %rsp
    pop %rbp
    retq


# @brief _Z_mcopy
# @param %rcx (const void*) - source
# @param %rdx (void*) - dest
# @param %r8 (size_t) - size
_Z_mcopy:
    mov $0, %r9
1:
    cmp %r8, %r9
    je 1f
    movb (%rcx, %r9, 1), %bl
    movb %bl, (%rdx, %r9, 1)
    inc %r9
    jmp 1b
1:
    mov %rdx, %rax
    retq

# @brief _Z_mswap
# @param %rcx (void*) - ptr1
# @param %rdx (void*) - ptr2
# @param %r8 (size_t) -
_Z_mswap:
    mov $0x0, %rax
1:
    cmp %r8, %rax
    jae 1f
    movb (%rcx, %rax, 1), %r9b
    movb (%rdx, %rax, 1), %bl
    movb %bl, (%rcx, %rax, 1)
    movb %r9b, (%rdx, %rax, 1)
    inc %rax
    jmp 1b
1:
    retq

_Z_exit:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    call ExitProcess

    add $0x20, %rsp
    pop %rbp
    retq