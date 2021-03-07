# Unix x86_64 assembly files
# @file list.S - zsr_list object implementation
# @version 2020/11 0.3.0.0002
.att_syntax # AT&T syntax
.text

#### <<< zsr_list / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
# >> ZSR_LIST DESCRIPTION << #
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
# @param %edi (int32) - sizeof(obj)
# @return %rax (void*) - zsr_list
zCreateList:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov $0x0, %esi
    mov %edi, -0x4(%rbp)
    mov %esi, -0x8(%rbp)
    mov %edi, %eax
    mul %edx
    add $0x8, %eax
    mov %eax, %edi
    callq _Z_alloc
    cmp $0x0, %rax
    je 1f
    mov -0x4(%rbp), %edi
    mov %edi, (%rax)
    mov -0x8(%rbp), %edi
    mov %edi, 0x4(%rax)
1:  add $0x10, %rsp
    pop %rbp
    retq

# @brief zDestroyList
# @param %rcx (void*) - zsr_list
# @return %rax (bool)
zDestroyList:
    callq _Z_free
    retq

# @brief zListLength
# @param %rdi (void*) - zsr_list
# @return %eax (int32)
zListLength:
    mov 0x4(%rdi), %eax
    retq

# @brief zListObjSizeof
# @param %rdi (void*) - zsr_list
# @return %eax (int32)
zListObjSizeof:
    mov (%rdi), %eax
    retq

# @brief __zListGet
# @param %rdi (void*) - zsr_list
# @param %rsi (int32) - index
# @return %rax (void*) - obj
__zListGet:
    movl (%rdi), %eax
    mull %esi
    lea 0x8(%rdi, %rax, 1), %rax
    retq

# @brief zListSet
# @param %rdi (void*) - zsr_list
# @param %rsi (void*) - obj's buf
# @param %edx (int32) - index
zListSet:
    cmp 0x4(%rdi), %edx
    jae 2f
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rsi, -0x8(%rbp)
    mov (%rdi), %eax
    mull %edx
    mov $0, %edx
    mov -0x8(%rbp), %rsi
1:  cmp %edx, (%rsi)
    je 1f
    movb (%rsi, %rdx, 1), %bl
    movb %bl, 0x8(%rdi, %rax, 1)
    inc %rax
    inc %edx
    jmp 1b
1:  add $0x10, %rsp
    pop %rbp
2:  retq

# @brief zListAppend
# @param %rdi (void**) - zsr_list
# @param %rsi (void*) - obj
zListAppend:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    # Set local vars
    mov %rdi, -0x8(%rbp)    # zsr_list*
    mov %rsi, -0x10(%rbp)   # obj*
    mov (%rdi), %rdi
    mov 0x4(%rdi), %esi
    mov %esi, -0x18(%rbp)   # length(list)
    mov (%rdi), %eax
    mov %eax, -0x14(%rbp)   # sizeof(obj)

    # Reallocate zsr_list
    mull %esi
    addl $0x8, %eax
    addl -0x14(%rbp), %eax
    mov %rax, %rsi
    mov %rax, -0x20(%rbp)
    mov -0x8(%rbp), %rdi
    mov (%rdi), %rdi
    callq _Z_realloc

    mov %rax, %rdi
    mov -0x10(%rbp), %rsi
    mov -0x18(%rbp), %ecx
    movl %ecx, 0x4(%rdi)
    incl 0x4(%rdi)
    mov %ecx, %eax

    # Append
    mull -0x14(%rbp)
    mov $0x0, %rdx
1:  cmp -0x14(%rbp), %edx
    jae 1f
    movb (%rsi, %rdx, 1), %bl
    movb %bl, 0x8(%rdi, %rax, 1)
    incl %edx
    incl %eax
    jmp 1b
1:  mov -0x8(%rbp), %rax
    mov %rdi, (%rax)
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zListPrepend
# @param %rdi (void**) - zsr_list
# @param %rsi (void*) - obj
zListPrepend:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    # Set local vars
    mov %rdi, -0x8(%rbp)    # zsr_list*
    mov %rsi, -0x10(%rbp)   # obj*

    # Alloc new zsr_list
    mov (%rdi), %rdi
    movl (%rdi), %eax
    movl 0x4(%rdi), %edx
    mull %edx
    add $0x8, %eax
    add (%rdi), %eax
    mov %rax, %rdi
    callq _Z_alloc

    # Set zsr_list vars
    mov -0x8(%rbp), %rdi
    mov (%rdi), %rdi
    mov 0x4(%rdi), %ecx
    mov %ecx, 0x4(%rax)
    incl 0x4(%rax)
    mov (%rdi), %ecx
    mov %ecx, (%rax)

    # Prepend elements
    # %rax : zsr_list [new]
    # %rsi : obj*
    # %rcx : sizeof(obj)
    mov -0x10(%rbp), %rsi
    mov $0x0, %rdx
1:  cmp %ecx, %edx
    jae 1f
    movb (%rsi, %rdx, 1), %bl
    movb %bl, 0x8(%rax, %rdx, 1)
    incl %edx
    jmp 1b

1:  # Copy the rest
    mov %rax, %rdi
    mov %rdi, -0x18(%rbp)
    add %rcx, %rdi
    mov -0x8(%rbp), %rsi
    mov (%rsi), %rsi
    mov (%rsi), %rax
    mull 0x4(%rsi)
    # %rdi : zsr_list [new]
    # %rsi : zsr_list [old]
    # %ecx : sizeof(obj)
    # %rax : zsr_list list buffer size
    # %rdx : index
    mov $0x0, %rdx
1:  cmp %eax, %edx
    jae 1f
    movb 0x8(%rsi, %rdx, 1), %bl
    movb %bl, 0x8(%rdi, %rdx, 1)
    incl %edx
    jmp 1b
1:  mov %rsi, %rdi
    callq _Z_free
    mov -0x8(%rbp), %rdi
    mov -0x18(%rbp), %rsi
    mov %rsi, (%rdi)
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zListRemove
# @param %rdi (void**) - zsr_list
# @param %esi (int32) - index
# @return %rax (bool)
zListRemove:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    # Set local vars
    mov %rdi, -0x8(%rbp)    # zsr_list*
    mov (%rdi), %rdi
    mov %esi, -0xc(%rbp)    # index
    cmp 0x4(%rdi), %esi
    jae 3f

    movl 0x0(%rdi), %eax
    movl 0x4(%rdi), %edx
    mull %edx
    addl $0x8, %eax
    subl (%rdi), %eax
    mov %rax, %rdi
    callq _Z_alloc

    # Set zsr_list's vars
    mov %rax, %rdi
    mov -0x8(%rbp), %rsi
    mov (%rsi), %rsi
    mov (%rsi), %eax
    mov %eax, (%rdi)
    mov 0x4(%rsi), %edx
    mov %edx, 0x4(%rdi)
    decl 0x4(%rdi)

    # Fill zsr_list
    # %eax : last cursor
    # %ecx : %rdi buf cursor
    # %edx : %rsi buf cursor
    # %rdi : new zsr_list
    # %rsi : last zsr_list
    # %r8 : index
    # %r9 : reserved
    mull %edx
    mov $0x0, %rcx
    mov $0x0, %rdx
    mov $0x0, %r8
    mov $0x0, %r9d
1:  cmp %eax, %edx
    jae 1f
    cmpl %r8d, -0xc(%rbp)
    je 2f
    movb 0x8(%rsi, %rdx, 1), %bl
    movb %bl, 0x8(%rdi, %rcx, 1)
    incl %edx
    incl %ecx
    incl %r9d
    cmpl %r9d, (%rsi)
    jne 1b
    mov $0x0, %r9d
    incl %r8d
    jmp 1b
2:  addl -0xc(%rbp), %edx
    incl %r8d
    jmp 1b
1:  mov -0x8(%rbp), %rcx
    mov %rdi, (%rcx)
    mov %rsi, %rdi
    callq _Z_free
    mov $0x1, %rax
    jmp 4f
3:  mov $0x0, %rax
4:  add $0x10, %rsp
    pop %rbp
    retq