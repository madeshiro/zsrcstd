# Windows AMD64 assembly files
# @file list.S - zsr_list object implementation
# @version 2020/11 0.3.0
.att_syntax # AT&T syntax
.text

#### <<< zsr_list / win-amd64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_LIST DESCRIPTION << ##
# :: zsr_list {
#
#   int32 obj_sizeof ] hea\
#   int32 lst_length ]     \der
#   [.../.../...] data
# };
#
# > zsr_list's sizeof === 8Bytes (header) + sizeof(obj)*length(list)
#
# //

.globl zCreateList
.globl zDestroyList
.globl zListLength
.globl zListObjSizeof
.globl __zListGet
.globl zListSet
.globl zListAppend
.globl zListPrepend
.globl zListRemove

# @brief zCreateList
# @param %ecx (int32) - sizeof(obj)
# @return %rax (void*) - zsr_list
zCreateList:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    mov $0x0, %edx
    mov %ecx, -0x4(%rbp)
    mov %edx, -0x8(%rbp)

    mov %ecx, %eax
    mul %edx
    add $0x8, %eax
    mov %eax, %ecx
    callq _Z_alloc

    cmp $0x0, %rax
    je 1f

    mov -0x4(%rbp), %ecx
    mov %ecx, (%rax)
    mov -0x8(%rbp), %ecx
    mov %ecx, 0x4(%rax)
1:
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zDestroyList
# @param %rcx (void*) - zsr_list
# @return %rax (bool)
zDestroyList:
    callq _Z_free
    retq

# @brief zListLength
# @param %rcx (void*) - zsr_list
# @return %rax (int32)
zListLength:
    mov 0x4(%rcx), %eax
    retq

# @brief zListObjSizeof
# @param %rcx (void*) - zsr_list
zListObjSizeof:
    mov (%rcx), %eax
    retq

# @brief __zListGet
# @param %rcx (void*) - zsr_list
# @param %edx (int32) - index
# @return %rax (void*) - obj
__zListGet:
    movl (%rcx), %eax
    mull %edx
    lea 0x8(%rcx, %rax, 1), %rax
    retq

# @brief zListSet
# @param %rcx (void*) - zsr_list
# @param %rdx (void*) - obj's buf
# @param %r8d (int32) - index
zListSet:
    cmp 0x4(%rcx), %r8d
    jae 2f
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    mov %rdx, -0x8(%rbp)
    mov (%rcx), %eax
    mull %r8d
    mov $0, %r8
    mov -0x8(%rbp), %rdx
1:
    cmp %r8d, (%rcx)
    je 1f
    movb (%rdx, %r8, 1), %bl
    movb %bl, 0x8(%rcx,%rax,1)
    inc %rax
    inc %r8d
    jmp 1b
1:
    add $0x10, %rsp
    pop %rbp
2:
    retq

# @brief zListAppend
# @param %rcx (void**) - zsr_list
# @param %rdx (void*) - obj
zListAppend:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    # Set local vars
    mov %rcx, -0x8(%rbp)    # ptr to zsr_list
    mov %rdx, -0x10(%rbp)   # obj buffer

    mov (%rcx), %rcx
    mov (%rcx), %edx
    mov 0x4(%rcx), %eax
    mov %edx, -0x14(%rbp)   # sizeof(obj)
    mov %eax, -0x18(%rbp)   # length(list)

    # Reallocate zsr_list
    mull %edx
    addl $0x8, %eax
    addl -0x14(%rbp), %eax
    mov %rax, %rdx
    push %rax
    mov -0x8(%rbp), %rcx
    mov (%rcx), %rcx
    call _Z_realloc

    # Append
    pop %r8
    mov -0x14(%rbp), %ebx
    sub %rbx, %r8
    movq $0x0, %r9
    mov -0x10(%rbp), %rcx
1:
    cmpl %r9d, -0x14(%rbp)
    je 1f
    movb (%rcx, %r9, 1), %bl
    movb %bl, (%rax, %r8, 1)
    inc %r8
    inc %r9d
    jmp 1b
1:
    mov -0x18(%rbp), %ecx
    inc %ecx
    mov %ecx, 0x4(%rax)
    mov -0x8(%rbp), %rcx
    mov %rax, (%rcx)

    add $0x20, %rsp
    pop %rbp
    retq

# @brief zListPrepend
# @param %rcx (void**) - zsr_list
# @param %rdx (void*) - obj
zListPrepend:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    # Set local vars
    mov %rcx, -0x8(%rbp)    # zsr_list*
    mov %rdx, -0x10(%rbp)   # obj*
    mov (%rcx), %rcx
    mov (%rcx), %eax
    mov 0x4(%rcx), %edx
    mov %eax, -0x14(%rbp)   # sizeof(obj)
    mov %edx, -0x18(%rbp)   # length(list)

    mull %edx
    addl $0x8, %eax
    addl -0x14(%rbp), %eax
    mov %rax, %rcx
    callq _Z_alloc

    # Set zsr_list's header [sizeof(obj), length(list)]
    mov -0x14(%rbp), %ecx
    mov %ecx, (%rax)
    mov -0x18(%rbp), %ecx
    inc %ecx
    mov %ecx, 0x4(%rax)

    # Prepend obj
    mov $0, %r8d
    mov -0x10(%rbp), %rdx
1:
    cmp -0x14(%rbp), %r8d
    je 1f
    movb (%rdx, %r8, 1), %bl
    movb %bl, 0x8(%rax, %r8, 1)
    inc %r8d
    jmp 1b
1:

    # Copy remaining data
    mov -0x8(%rbp), %rcx
    mov (%rcx), %rdx
    mov %rax, (%rcx)
    mov -0x14(%rbp), %ecx
    add %rcx, %rax

    mov $0, %r8     # index
    mov $0, %r9     # cursor
    mov $0, %rcx    # sizeof(obj) cmp
1:
    cmp %r8d, -0x18(%rbp)
    je 1f

    movb 0x8(%rdx, %r9, 1), %bl
    movb %bl, 0x8(%rax, %r9, 1)
    inc %r9
    inc %rcx

    cmp %ecx, -0x14(%rbp)
    jne 1b
    inc %r8
    mov $0, %rcx
    jmp 1b
1:
    mov %rdx, %rcx
    callq _Z_free
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zListRemove
# @param %rcx (void**) - zsr_list
# @param %rdx (int32) - index
# @return %rax (bool)
zListRemove:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    # Set local vars
    mov %rcx, -0x8(%rbp)    # zsr_list*
    mov %edx, -0xc(%rbp)    # index
    mov (%rcx), %rcx
    mov (%rcx), %edx
    mov %edx, -0x10(%rbp)   # sizeof(obj)
    mov 0x4(%rcx), %edx
    dec %edx
    mov %edx, -0x14(%rbp)   # length(list) [refresh]

    # Test if index in bound
    mov 0x4(%rcx), %edx
    cmp %edx, -0xc(%rbp)
    jae 4f

    # Create new zsr_list
    mov -0x10(%rbp), %edx
    mov -0x14(%rbp), %eax
    mull %edx
    mov %edx, -0x18(%rbp)   # length(list) [byte]
    addl $0x8, %eax
    mov %rax, %rcx
    callq _Z_alloc

    # Set zsr_list's header
    mov -0x10(%rbp), %edx
    mov %edx, (%rax)
    mov -0x14(%rbp), %edx
    mov %edx, 0x4(%rax)

    # Copy data except at index
    mov $0x0, %edx          # index      [byte/sizeof]
    mov $0x0, %r8d          # cursor/rax [in byte]
    mov $0x0, %r9d          # cursor/rcx [in byte]
    mov -0x8(%rbp), %rcx
    mov (%rcx), %rcx        # old zsr_list
1:
    cmp %edx, 0x4(%rcx)
    je 1f
    cmp %edx, -0xc(%rbp)
    je 2f

    mov $0x0, %r10
3:
    cmpl -0x10(%rbp), %r10d
    je 3f
    movb 0x8(%rcx, %r9, 1), %bl
    movb %bl, 0x8(%rax, %r8, 1)
    inc %r8d
    inc %r9d
    inc %r10
    jmp 3b
3:
    inc %edx
    jmp 1b
2:
    add -0x10(%rbp), %r9d
    inc %edx
    jmp 1b
1:
    mov -0x8(%rbp), %rcx
    mov %rax, (%rcx)
    mov $0x1, %rax
    jmp 5f
4:
    mov $0x0, %rax
5:
    add $0x20, %rsp
    pop %rbp
    retq