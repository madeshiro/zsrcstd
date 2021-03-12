# Windows AMD64 assembly files
# @file chainlist.S - zsr_chainlist object implementation
# @version 2020/11 0.3.0
.att_syntax # AT&T syntax
.text

#### <<< zsr_chainlist / win-amd64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_CHAINLIST DESCRIPTION << ##
# :: zsr_chainlist {
#
#   void* reserved = 0x0 ] da\
#   void* first          ]    \ta
#   uint32 length        ]
#   uint32 sizeof(obj)   ] total_size: 24bytes
# };
#
# :: zsr_chainobj {
#   void* prev ] hea\
#   void* next ]     \der
#   [...] data
# };
#
# //

.globl zCreateChlist
.globl zDestroyChlist
.globl zChlistLength
.globl zChlistObjSizeof
.globl zChlistNext
.globl zChlistPrev
.globl zChlistFromObj
.globl __zChlistValue
.globl zChlistPush
.globl zChlistPop

# @brief zCreateChlist
# @param %rcx (int32) - sizeof
# @return %rax (void*) - zsr_chlist
zCreateChlist:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    mov %ecx, -0x4(%rbp)
    mov $0x18, %rcx
    callq _Z_alloc

    # Set chainlist's vars
    movq $0x0, (%rax)
    movq $0x0, 0x8(%rax)
    movl $0x0, 0x10(%rax)
    movl -0x4(%rbp), %ecx
    movl %ecx, 0x14(%rax)

    add $0x10, %rsp
    pop %rbp
    retq

# @brief zDestroyChlist
# @param %rcx (void*) - zsr_chlist
# @param %rdx (bool) - remove elements
# @return %rax (bool)
zDestroyChlist:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    mov %rcx, -0x8(%rbp)
    mov 0x8(%rcx), %rcx
    cmp $0x0, %rdx
    je 2f
 1: cmpq $0x0, %rcx
    je 2f
    mov 0x8(%rcx), %rdx
    push %rdx
    callq _Z_free
    pop %rcx
    jmp 1b
 2: mov -0x8(%rbp), %rcx
    callq _Z_free
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zChlistLength
# @param %rcx (void*) - zsr_chlist
# @return %rax (int32)
zChlistLength:
    mov 0x10(%rcx), %eax
    retq

# @brief zChlistObjSizeof
# @param %rcx (void*) - zsr_chlist
# @return %rax (int32)
zChlistObjSizeof:
    mov 0x14(%rcx), %eax
    retq

# @brief zChlistNext
# @param %rcx (void*) - zsr_chlist/zsr_chobj
# @return %rax (void*)
zChlistNext:
    mov 0x8(%rcx), %rax
    retq

# @brief zChlistPrev
# @param %rcx (void*) - zsr_chobj
# @return %rax (void*)
zChlistPrev:
    movq 0x0(%rcx), %rax
    cmpq $0x0, (%rax)
    jne 1f
    mov $0x0, %rax
 1: retq

# @brief zChlistFromPrev
# @param %rcx (void*) - zsr_chainobj
# @return %rax (void*) - zsr_chainlist
zChlistFromObj:
    cmpq $0x0, (%rcx)
    je 1f
    mov (%rcx), %rcx
    jmp zChlistFromObj
 1: mov %rcx, %rax
    retq

# @brief __zChlistValue
# @param %rcx (void*) - zsr_chobj
__zChlistValue:
    lea 0x10(%rcx), %rax
    retq

# @brief __zChListLast
# @param %rcx (void*) - zsr_chlist
# @note only %rax is modified !
# @return %rax (void*) - last zsr_chobj or zsr_chlist if zsr_chlist is empty
__zChlistLast:
    mov %rcx, %rax
    cmpq $0x0, 0x8(%rax)
    je 2f
    mov 0x8(%rax), %rax
 1: cmpq $0x0, 0x8(%rax)
    je 2f
    mov 0x8(%rax), %rax
    jmp 1b
 2: retq


# @brief zChlistPush
# @param %rcx (void*) - zsr_chainlist
# @param %rdx (void*) - obj
# @return %rax
zChlistPush:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    addl $0x1, 0x10(%rcx)
    callq __zChlistLast

    movq %rax, -0x8(%rbp)   # last chain
    movq %rdx, -0x10(%rbp)  # obj
    movl 0x14(%rcx), %ecx
    movl %ecx, -0x14(%rbp)  # sizeof(obj)
    addl $0x10, %ecx
    callq _Z_alloc     # rax = zsr_chainobj (new)
    movq -0x10(%rbp), %rdx   # obj
    movq -0x8(%rbp), %rcx    # zsr_chainobj (last)
    movl -0x14(%rbp), %r8d   # sizeof(obj)
    movl $0x0, %r9d          # for_loop's index
1:  # for_loop (copy data)
    cmp %r8d, %r9d
    jae 1f
    movb (%rdx, %r9, 1), %bl
    movb %bl, 0x10(%rax, %r9, 1)
    inc %r9d
    jmp 1b
 1: movq %rax, 0x8(%rcx) # set zsr_chainobj.next [last]
    movq %rcx, 0x0(%rax) # set zsr_chainobj.prev [new]
    movq $0x0, 0x8(%rax) # set zsr_chainobj.next [new]
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zChlistPop
# @param %rcx (void*) - zsr_chainlist
# @return %rax (void*) - zsr_chobj pop or null if zsr_chainlist was empty
zChlistPop:
    callq __zChlistLast
    cmp %rcx, %rax
    je 2f
    subl $0x1, 0x10(%rcx)
    movq (%rax), %rdx
    movq $0x0, 0x8(%rdx)
    retq
 2: mov $0x0, %rax
    retq