# Windows AMD64 assembly files
# @file thread.S - system call
# @version 2020/11 0.3.0.0002
.att_syntax
.section ,"r"

__Z_INFINITE: .int 0xffffffff

.text

#### <<< THREAD / win-amd64 >>> #####
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_THREAD DESCRIPTION << ##
# :: zsr_thread {
#
#   0x0  > (4bytes) uint32 - pid                ] hea\
#   0x4  > (8bytes) char*  - name               ]     \der
#   0xc  > (8bytes) int (*)(void*) - function   ]  \d
#   0x14 > (8bytes) HANDLE         - handler    ]   \a
#   0x1c > (4bytes) int32          - ret        ]    \t
#   0x20 > (8bytes) void*          - reserved   ]     \a
#   0x28 > (1byte)  bool           - running    ]      \
#
# };
# > zsr_thread's size = (0x29) 41 bytes
#
#
# \/

# @brief __zsr_thread_saferunning
# @param 0x20(%rcx) (void*) - params
# @param %rcx (zsr_thread) - thread
# @exit
__zsr_thread_saferunning:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rcx, -0x8(%rbp)
    mov 0xc(%rcx), %rax
    mov 0x20(%rcx), %rcx
    callq *%rax
    mov -0x8(%rbp), %rdx
    mov %eax, %ecx
    mov %ecx, 0x1c(%rdx)
    movb $0, 0x28(%rdx)
    callq _Z_exit
    add $0x10, %rsp
    pop %rbp
    retq

.globl zThreadCreate
.globl zThreadDestroy
.globl zThreadStart
.globl zThreadJoin
.globl zThreadIsRunning

# @brief zThreadCreate
# @param %rcx (zthread_f) - thread's function
zThreadCreate:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x8(%rbp)
    mov $0x29, %rcx
    callq _Z_calloc
    mov -0x8(%rbp), %r8
    mov %r8, 0xc(%rax)
    add $0x30, %rsp
    pop %rbp
    retq

# @brief zThreadDestroy
# @param %rcx (zsr_thread) - thread
zThreadDestroy:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x8(%rbp)
    mov 0x14(%rcx), %rcx
    call CloseHandle
    test %rax, %rax
    je 1f
    mov -0x8(%rbp), %rcx
    mov 0x4(%rcx), %r8
    test %r8, %r8
    je 2f
    mov 0x4(%rcx), %rcx
    call _Z_free
    mov -0x8(%rbp), %rcx
2:  call _Z_free
1:  add $0x30, %rsp
    pop %rbp
    retq

# @brief zThreadStart
# @param %rcx (zsr_thread) - thread
# @param %rdx (void*) - params
zThreadStart:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, (%rsp)
    lea (%rcx), %r9
    mov %r9, 0x28(%rsp)
    movq $0x0, 0x28(%rsp)
    mov %rdx, 0x20(%rcx)
    mov %rcx, %r9
    lea __zsr_thread_saferunning, %r8
    mov $0x0, %rdx
    mov $0x0, %rcx
    callq CreateThread
    mov (%rsp), %rcx
    mov %rax, 0x14(%rcx)
    movb $0x1, 0x28(%rcx)
    add $0x30, %rsp
    pop %rbp
    retq

# @brief zThreadJoin
# @param %rcx (zsr_thread) - thread
zThreadJoin:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp
    mov %rcx, -0x8(%rbp)
    mov 0x14(%rcx), %rcx
    mov __Z_INFINITE, %edx
    call WaitForSingleObject
    mov -0x8(%rbp), %rcx
    mov 0x1c(%rcx), %eax
    add $0x40, %rsp
    pop %rbp
    retq

# @brief zThreadIsRunning
# @param %rcx (zsr_thread) - thread
# @return %rax (bool)
zThreadIsRunning:
    movb 0x28(%rcx), %al
    retq

.globl zSleep
.globl zThreadSleep
.globl zThreadNanoSleep
.globl zThreadGetPID

zThreadGetPID:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    callq GetCurrentThreadId
    add $0x20, %rsp
    pop %rbp
    retq

# @brief zSleep, zThreadSleep
# @param %rcx (long) - millis
zSleep: zThreadSleep: zThreadNanoSleep:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    callq Sleep
    add $0x20, %rsp
    pop %rbp
    retq

.globl zThreadSetName
.globl zThreadGetName
.globl zThreadTID

# @brief zThreadSetName
# @param %rcx (zsr_thread) - thread
# @param %rdx (const char*) - name
zThreadSetName:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp
    mov %rcx, -0x8(%rbp)
    mov %rdx, %rcx
    call zstrcopy
    mov -0x8(%rbp), %rcx
    mov %rax, 0x4(%rcx)
    add $0x10, %rsp
    pop %rbp
    retq

# @brief zThreadGetName
# @param %rcx (zsr_thread) - thread
zThreadGetName:
    mov 0x4(%rcx), %rax
    retq

# @brief zThreadTID
# @param %rcx (zsr_thread) - thread
zThreadTID:
    mov (%rcx), %eax
    retq

