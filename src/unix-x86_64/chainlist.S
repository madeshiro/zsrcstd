# Unix x86_64 assembly files
# @file chainlist.S - zsr_chainlist object implementation
# @version 2020/11 0.3.0.0002
.att_syntax # AT&T syntax
.text

#### <<< zsr_chainlist / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
## >> ZSR_CHAINLIST DESCRIPTION << ##
# :: zsr_chainlist {
#
#   (0x00) void* reserved = 0x0 ] da\
#   (0x08) void* first          ]    \ta
#   (0x10) int32 length         ]
#   (0x14) int32 sizeof(obj)    ] total_size: 24bytes
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
# @param %edi (int32) - sizeof
# @return %rax (void*) - zsr_chainlist
zCreateChlist:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    mov %edi, -0x4(%rbp)
    mov $0x18, %rdi
    callq _Z_alloc
    mov -0x4(%rbp), %edi

    # Set chainlist's vars
    movq $0x0, (%rax)
    movq $0x0, 0x8(%rax)
    movl $0x0, 0x10(%rax)
    movl %edi, 0x14(%rax)

    add $0x10, %rsp
    pop %rbp
    retq

# @brief zDestroyChlist
# @param %rdi (void*) - zsr_chlist
# @param %rsi (bool) - remove elements ?
zDestroyChlist:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    mov %rdi, -0x8(%rbp)
    mov 0x8(%rdi), %rdi
    cmp $0x0, %rsi
    je 2f
1:  cmpq $0x0, %rdi
    je 2f
    mov 0x8(%rdi), %rsi
    push %rsi
    callq _Z_free
    pop %rdi
    jmp 1b
2:  mov -0x8(%rbp), %rdi
    callq _Z_free
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zChlistLength
# @param %rdi (void*) - zsr_chainlist
# @return %eax (int32)
zChlistLength:
    movl 0x10(%rdi), %eax
    retq

# @brief zChlistObjSizeof
# @param %rdi (void*) - zsr_chainlist
# @return %eax (int32)
zChlistObjSizeof:
    movl 0x14(%rdi), %eax
    retq

# @brief zChlistNext
# @param %rdi (void*) - zsr_chainlist or zsr_chainobj
# @return %rax (void*) - zsr_chainobj
zChlistNext:
    mov 0x8(%rdi), %rax
    retq

# @brief zChlistPrev
# @param %rdi (void*) - zsr_chainobj
# @return %rdi (void*) - zsr_chainobj
zChlistPrev:
    movq (%rdi), %rax
    cmpq $0x0, (%rax)
    jne 1f
    mov $0x0, %rax
1:  retq

# @brief zChlistFromObj
# @param %rdi (void*) - zsr_chainobj
# @return %rax (void*) - zsr_chainlist
zChlistFromObj:
    cmpq $0x0, (%rdi)
    je 1f
    mov (%rdi), %rdi
    jmp zChlistFromObj
1:  mov %rdi, %rax
    retq

# @brief __zChlistValue
# @param %rdi (void*) - zsr_chainobj
# @return %rax (void*)
__zChlistValue:
    lea 0x10(%rdi), %rax
    retq

# @brief __zChlistLast
# @param %rdi (void*) - zsr_chainlist
# @note only %rax is modified
# @return %rax (void*) - last zsr_chainobj or zsr_chainlist if empty
__zChlistLast:
    mov %rdi, %rax
    cmpq $0x0, 0x8(%rax)
    je 2f
    mov 0x8(%rax), %rax
1:  cmpq $0x0, 0x8(%rax)
    je 2f
    mov 0x8(%rax), %rax
    jmp 1b
2:  retq

# @brief zChlistPush
# @param %rdi (void*) - zsr_chainlist
# @param %rsi (void*) - obj
zChlistPush:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    addl $0x1, 0x10(%rdi)
    callq __zChlistLast

    movq %rax, -0x8(%rbp)   # last chain
    movq %rsi, -0x10(%rbp)  # obj
    movl 0x14(%rdi), %edi
    movl %edi, -0x14(%rbp)  # sizeof(obj)
    addl $0x10, %edi
    callq _Z_alloc    # rax = zsr_chainobj (new)
    movq -0x10(%rbp), %rsi  # obj
    movq -0x8(%rbp), %rdi   # zsr_chainobj (last)
    movl -0x14(%rbp), %ecx  # sizeof(obj)
    movl $0x0, %edx         # for_loop's index
1:  cmp %ecx, %edx
    jae 1f
    movb (%rsi, %rdx, 1), %bl
    movb %bl, 0x10(%rax, %rdx, 1)
    incl %edx
    jmp 1b
1:  movq %rax, 0x8(%rdi) # set zsr_chainobj.next [last]
    movq %rdi, 0x0(%rax) # set zsr_chainobj.prev [new]
    movq $0x0, 0x8(%rax) # set zsr_chainobj.next [new]
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zChlistPop
# @param %rdi (void*) - zsr_chainlist
# @return %rax (void*) - zsr_chainobj pop or null if zsr_chainlist was empty
zChlistPop:
    callq __zChlistLast
    cmp %rdi, %rax
    je 2f
    subl $0x1, 0x10(%rcx)
    movq (%rax), %rdx
    movq $0x0, 0x8(%rsi)
    retq
2:  mov $0x0, %rax
    retq