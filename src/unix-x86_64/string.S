# Unix x86_64 assembly files
# @file string.S - string object implementation
# @version 2020/11 0.3.0
.att_syntax # AT&T syntax
.text

#### <<< STRING / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
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
# /!\ wchar_t's unix size == 4Bytes [int32] /!\
#
# //

.globl _zString
.globl zString
.globl zDeleteString
.globl zStrlength
.globl zStrlength_set
.globl zStrGrow

# @brief _zString
# @param %edi (uint32) - capacity
# @return %rax (zsr_string)
_zString:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rbp
    mov %edi, -0x8(%rbp)
    add $0x5, %edi
    callq _Z_alloc
    mov -0x8(%rbp), %edi
    mov %edi, (%rax)
    add $0x4, %rax
    movb $0x0, (%rax, %rcx, 1)
    retq

# @brief zString
# @param %rdi (char*) - string
# @return %rax (zsr_string)
zString:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rbp
    mov %rdi, -0x8(%rbp)
    callq zstrlen
    mov %eax, -0x10(%rbp)
    mov $0x5, %rdi
    add %rax, %rdi
    callq _Z_alloc
    mov -0x8(%rbp), %rdi
    mov -0x10(%rbp), %esi
    mov %esi, (%rax)
    add $0x4, %rax
    mov $0x0, %rdx
1:  cmp %esi, %edx
    jae 1f
    movb (%rdi, %rdx, 1), %bl
    movb %bl, (%rax, %rdx, 1)
    inc %edx
    jmp 1b
1:  movb $0, (%rax, %rdx, 1)
    add $0x10, %rsp
    pop %rbp
    retq

# @brief zDeleteString
# @param %rdi (zsr_string) - string
zDeleteString:
    sub $0x4, %rdi
    callq _Z_free
    retq

# @brief zStrlength
# @param %rdi (zsr_string) - string
# @return %rax (uint32)
zStrlength:
    mov -0x4(%rdi), %eax
    retq

# @brief zStrlength_set:
# @param %rdi (zsr_string) - string
# @param %rsi (uint32) - length
zStrlength_set:
    mov %esi, -0x4(%rdi)
    retq

# @brief zStrGrow
# @param %rdi (zsr_string) - string
# @param %esi (uint32) - length
zStrGrow:
    mov -0x4(%rdi), %edx
    add %esi, %edx
    mov %edx, -0x4(%rdi)
    sub $0x4, %rdi
    mov %edx, %esi
    add $0x5, %esi
    callq _Z_realloc
    add $0x4, %rax
    retq
