# Windows AMD64 assembly files
# @file socket.S - system call
# @version 2020/12 0.4.0
.att_syntax
.text

#### <<< SOCKET / win-amd64 >>> #####
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_SOCKET DESCRIPTION << ##
# :: zsr_socket {
#
#   0x0  > (8bytes) uint64 - fd       ] da\
#   0x8  > (8bytes) void*  - sockaddr ]    \ta
#   0x10 > (4bytes) int32  - af       ]  m\
#   0x14 > (4bytes) int32  - type     ]   e\t
#   0x18 > (4bytes) int32  - protocol ]     \a
#
# };
# > zsr_socket's size = (0x1c) 28 bytes
#
#
# \/

.globl zsocket
.globl zclosesock

zsocket:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp
    mov %ecx, -0x4(%rbp)
    mov %edx, -0x8(%rbp)
    mov %r8d, -0xc(%rbp)
    mov $0x1c, %rcx
    call _Z_alloc
    mov -0x4(%rbp), %ecx
    mov %ecx, 0x10(%rax)
    mov -0x8(%rbp), %edx
    mov %edx, 0x14(%rax)
    mov -0xc(%rbp), %r8d
    mov %r8d, 0x18(%rax)
    mov %rax, -0x18(%rbp)
    call socket
    mov -0x18(%rbp), %rcx
    mov %rax, (%rcx)
    mov %rcx, %rax
    add $0x40, %rsp
    pop %rbp
    ret

zclosesock:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp
    mov %rcx, -0x8(%rbp)
    mov (%rcx), %rcx
    call closesocket
    mov -0x8(%rbp), %rcx
    call _Z_free
    add $0x40, %rsp
    pop %rbp
    ret

.globl zbind
.globl zbindhost
.globl zlisten
.globl zaccept
.globl zconnect
.globl zconnecthost
.globl zshutdown

# @brief zbind
# @param %rcx (void*) - socket
# @param %rdx (char*) - inet_addr
# @param %r8w (short) - port
zbind:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x8(%rbp)
    call _Z_build_sockaddr_in
    mov %rax, -0x10(%rbp)
    call _Z_sizeof_sockaddr_in
    mov %eax, %r8d
    mov -0x10(%rbp), %rdx
    mov -0x8(%rbp), %rax
    mov (%rax), %rcx
    call bind
    test %eax, %eax
    je 1f
    movq $0x0, -0x8(%rbp)
    jmp 2f
 1: movq $0x1, -0x8(%rbp)
 2: mov -0x10(%rbp), %rcx
    call _Z_free
    mov -0x8(%rbp), %rax
    add $0x30, %rsp
    pop %rbp
    ret

zlisten:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov (%rcx), %rcx
    call listen
    test %eax, %eax
    je 1f
    mov $0x0, %rax
    jmp 2f
 1: mov $0x1, %rax
 2: add $0x30, %rsp
    pop %rbp
    ret

zaccept:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov (%rcx), %rcx
    call accept
    mov $0, %rcx
    not %rcx
    cmp %rax, %rcx
    je 1f
    mov %rax, -0x8(%rbp)
    mov $0x1c, %rcx
    call _Z_calloc
    mov -0x8(%rbp), %rcx
    mov %rcx, (%rax)
    jmp 2f
 1: mov $0x0, %rax
 2: add $0x30, %rsp
    pop %rbp
    ret

# @brief zconnect
# @param %rcx (void*) - socket
# @param %rdx (char*) - inet_addr
# @param %r8w (short) - port
zconnect:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov (%rcx), %r9
    mov %r9, -0x8(%rbp)
    call _Z_build_sockaddr_in
    mov %rax, -0x10(%rbp)
    call _Z_sizeof_sockaddr_in
    mov %eax, %r8d
    mov -0x10(%rbp), %rdx
    mov -0x8(%rbp), %rcx
    call connect
    test %eax, %eax
    je 1f
    movq $0x0, -0x8(%rbp)
    jmp 2f
 1: movq $0x1, -0x8(%rbp)
 2: mov -0x10(%rbp), %rcx
    call _Z_free
    mov -0x8(%rbp), %rax
    add $0x30, %rsp
    pop %rbp
    ret

.globl zsend
.globl zrecv
.globl zsendto
.globl zrecvfrom

zsend:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov (%rcx), %rcx
    mov %rcx, -0x8(%rbp)
    mov $0x0, %r9
    call send
    add $0x30, %rsp
    pop %rbp
    ret

zrecv:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov (%rcx), %rcx
    mov $0x0, %r9
    call recv
    add $0x30, %rsp
    pop %rbp
    ret

# @brief zshutdown
# @param %rcx (void*) - zsr_socket
# @param %edx (int) - action
# @return %rax (zbool)
zshutdown:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov (%rcx), %rcx
    call shutdown
    test %eax, %eax
    je 1f
    mov $0x0, %rax
    jmp 2f
 1: mov $0x1, %rax
 2: add $0x30, %rsp
    pop %rbp
    ret

.globl zsockaf
.globl zsocktype
.globl zsockprotocol

zsockaf:
    mov 0x10(%rcx), %eax
    ret

zsocktype:
    mov 0x14(%rcx), %eax
    ret

zsockprotocol:
    mov 0x18(%rcx), %eax
    ret

