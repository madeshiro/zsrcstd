# Unix x86_64 assembly files
# @file socket.S - zsr_socket object implementation
# @version 2021/03 0.5.0
.att_syntax # AT&T syntax
.text

#### <<< zsr_socket / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
## >> ZSR_SOCKET DESCRIPTION << ##
# :: zsr_socket {
#
#   0x0  > (4bytes) uint32 - handler id ] header
#   0x4  > (4bytes) uint32 - fd         ] da\
#   0x8  > (8bytes) void*  - sockaddr   ]    \ta
#   0x10 > (4bytes) int32  - af         ]  m\
#   0x14 > (4bytes) int32  - type       ]   e\t
#   0x18 > (4bytes) int32  - protocol   ]     \a
#
# };
# > zsr_socket's size = (0x1c) 28 bytes
#
# //

.globl _Z_sys_socket
# @brief _Z_sys_socket
# @param %edi (int) - family
# @param %esi (int) - type
# @param %edx (int) - protocol
_Z_sys_socket:
    mov $0x29, %rax
    syscall
    ret

.globl _Z_sys_bind
# @brief _Z_sys_bind
# @param %edi (int) - fd
# @param %rsi (sockaddr*) - myaddr
# @param %rdx (uint*) - addrlen
_Z_sys_bind:
    mov $0x31, %rax
    syscall
    ret

.globl _Z_sys_listen
# @brief _Z_sys_listen
# @param %edi (int) - fd
# @param %esi (int) - backlog
_Z_sys_listen:
    mov $0x32, %rax
    syscall
    ret

.globl _Z_sys_accept
# @brief _Z_sys_accept
# @param %edi (int) - fd
# @param %rsi (sockaddr*) - upeer_sockaddr
# @param %rdx (uint*) - sizeof(sockaddr)
_Z_sys_accept:
    mov $0x2b, %rax
    syscall
    ret

.globl _Z_sys_connect
# @brief _Z_sys_connect
# @param %edi (int) - fd
# @param %rsi (sockaddr*) - uservaddr
# @param %edx (uint) - sizeof
_Z_sys_connect:
    mov $0x2a, %rax
    syscall
    ret

.globl _Z_sys_shutdown
# @brief _Z_sys_shutdown
# @param %edi (int) - fd
# @param %esi (int) - how [0=RD, 1=WR, 2=RDWR]
_Z_sys_shutdown:
    mov $0x30, %rax
    syscall
    ret

.globl _Z_sys_recvfrom
# @brief _Z_sys_recv
_Z_sys_recvfrom:

    ret

.globl _Z_sys_sendto
# @brief _Z_sys_send
_Z_sys_sendto:

    ret

.globl zsocket
.globl zclosesock

# @brief zsocket
# @param %edi (int) - af
# @param %esi (int) - type
# @param %edx (int) - protocol
zsocket:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov %edi, -0x4(%rbp)
    mov %esi, -0x8(%rbp)
    mov %edx, -0xc(%rbp)
    call _Z_sys_socket
    cmp $-1, %eax
    je 1f
    mov %eax, -0x10(%rbp)
    mov $0x1c, %rsi
    call _Z_alloc
    cmp %rax, %rax
    je 1f
    movl -0x4(%rbp), %edi
    mov %edi, 0x10(%rax)
    mov -0x8(%rbp), %edi
    mov %edi, 0x14(%rax)
    mov -0xc(%rbp), %edi
    mov %edi, 0x18(%rax)
    mov -0x10(%rbp), %edi
    mov %edi, 0x4(%rax)
    movq $0x0, 0x8(%rax)
    jmp 2f
 1: mov $0, %eax
 2: add $0x20, %rsp
    pop %rbp
    ret

zclosesocket:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rbp
    mov %rdi, -0x8(%rbp)
    mov (%rdi), %edi
    call _Z_sys_close

    .globl zbind
    .globl zbindhost
    .globl zlisten
    .globl zaccept
    .globl zconnect
    .globl zconnecthost
    .globl zshutdown

# @brief zbind
# @param %rdi (void*) - socket
# @param %rsi (char*) - inet_addr
# @param %rdx (short) - port
zbind:
    push %rbp
    mov %rsp, %rbp
