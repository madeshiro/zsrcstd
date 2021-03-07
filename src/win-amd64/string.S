# Windows AMD64 assembly files
# @file string.S - Standard call
# @version 2020/11 0.3.0.0002
.att_syntax # AT&T syntax
.text

#### <<< STRING / win-amd64 >>> #####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
# >> ZSR_STRING DESCRIPTION << #
# :: zsr_string (cast as [char*]) {
#
#   -0x4 > (4bytes) uint32 length ] header
#    0x0 > (nbytes) char*         ] data    < starting point
# };
#
# >> ZSR_WSTRING DESCRIPTION << #
# :: zsr_string (cast as [char*]) {
#
#   -0x4 > (4bytes) uint32 length ] header
#    0x0 > (nbytes) wchar_t*      ] data    < starting point
# };
#
# > zsr_string's sizeof === 4Bytes (header) + sizeof(char)*length(str)
# > zsr_wstring's sizeof === 4Bytes (header) + sizeof(wchar_t)*length(str)
# /!\ wchar_t's windows size == 2Bytes [int16:utf16] /!\
#
# //

.globl _zString
.globl zString
.globl zDeleteString
.globl zStrlength
.globl zStrlength_set
.globl zStrGrow

# @brief _zString
# @param %rcx (uint32) - capacity
_zString:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %ecx, -0x8(%rbp)
    add $0x5, %ecx
    callq _Z_alloc
    mov -0x8(%rbp), %ecx
    mov %ecx, (%rax)
    add $0x4, %rax
    movb $0x0, (%rax, %rcx, 1)
    add $0x10, %rsp
    pop %rbp
    retq

# @brief zString
# @param %rcx (char*) - string
# @return %rax (zsr_string)
zString:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rcx, -0x8(%rbp)
    callq zstrlen
    mov %eax, -0xc(%rbp)
    mov %eax, %ecx
    add $0x5, %ecx
    callq _Z_alloc
    mov -0x8(%rbp), %rcx
    mov -0xc(%rbp), %edx
    mov %edx, (%rax)
    add $0x4, %rax
    mov $0x0, %r8d
1:  cmp %edx, %r8d
    jae 1f
    movb (%rcx, %r8, 1), %bl
    movb %bl, (%rax, %r8, 1)
    incl %r8d
    jmp 1b
1:  movb $0, (%rax, %r8, 1)
    add $0x10, %rsp
    pop %rbp
    retq

# @brief zDeleteString
# @param %rcx (zsr_string) - string
zDeleteString:
    sub $0x4, %rcx
    callq _Z_free
    retq

# @brief zStrlength
# @param %rcx (zsr_string) - string
# @return %rax (uint32)
zStrlength:
    mov -0x4(%rcx), %eax
    retq

# @brief zStrlength_set
# @param %rcx (zsr_string) - string
# @param %rdx (uint32) - length
zStrlength_set:
    mov %edx, -0x4(%rcx)
    retq

# @brief zStrGrow
# @param %rcx (zsr_string) - string
# @param %edx (uint32) - grow
zStrGrow:
    mov -0x4(%rcx), %r8d
    add %edx, %r8d
    mov %r8d, -0x4(%rcx)
    sub $0x4, %rcx
    mov %r8d, %edx
    add $0x5, %edx
    callq _Z_realloc
    movl (%rax), %ecx
    add $0x4, %rax
    movb $0, (%rax, %rcx, 1)
    retq
