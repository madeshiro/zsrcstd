# Windows AMD64 assembly files
# @file stdio.S - Standard call
# @version 2020/11 0.3.0
.att_syntax # AT&T syntax
.text

#### <<< STDIO / win-amd64 >>> #####
# > functions declared in `win-amd64/headers/std_S.h` (win-amd64 decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## > I/O --- All related functions < ##
#
# \/

.globl _Z_winapi_cwrite
.globl _Z_winapi_cread
.globl _Z_winapi_Wcwrite
.globl _Z_winapi_Wcread

# @brief _Z_winapi_cwrite
# @param %rcx (const char*) - buffer
# @param %edx (32bit long) - len
# @ret %rax (int) - how many chars printed or -1
_Z_winapi_cwrite:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    lea -0x8(%rbp), %r9 # lpNumberOfCharsWritten (void*)
    mov %edx, %r8d      # nNumberOfCharsToWrite (DWORD)
    mov %rcx, %rdx      # lpBuffer (const char*)

    push %r9
    push %r8
    push %rdx

    mov $-11, %ecx
    call GetStdHandle
    mov %rax, %rcx

    pop %rdx
    pop %r8
    pop %r9
    call WriteConsoleA

    cmp $1, %rax
    je 1f
    mov $-0x1, %rax
    jmp 2f
1:
    mov -0x8(%rbp), %rax
2:
    add $0x10, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_cread
# @param %rcx (char*) - buffer
# @param %edx (unsigned long) - how many chars to read
# @return %rax (long)
_Z_winapi_cread:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x10(%rbp)
    mov %edx, -0x18(%rbp)

    mov $-0xa, %ecx
    call GetStdHandle

    lea -0x8(%rbp), %r9   # lpNumbersOfCharsRead
    mov -0x18(%rbp), %r8d # nNumberOfCharsToRead
    mov -0x10(%rbp), %rdx
    movq $0, 0x20(%rsp)
    mov %rax, %rcx
    call ReadConsoleA

    test %rax, %rax
    jne 1f
    mov $-0x1, %rax
    jmp 2f
1:  mov -0x8(%rbp), %rax
2:  add $0x30, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_wcwrite
# @param %rcx (const char*) - buffer
# @param %edx (32bit long) - len
# @ret %rax (int) - how many chars printed or -1
_Z_winapi_Wcwrite:
    push %rbp
    mov %rsp, %rbp
    sub $0x10, %rsp

    lea -0x8(%rbp), %r9 # lpNumberOfCharsWritten (void*)
    mov %edx, %r8d      # nNumberOfCharsToWrite (DWORD)
    mov %rcx, %rdx      # lpBuffer (const wchar_t*)

    push %r9
    push %r8
    push %rdx

    mov $-11, %ecx
    call GetStdHandle
    mov %rax, %rcx

    pop %rdx
    pop %r8
    pop %r9
    call WriteConsoleW

    cmp $1, %rax
    je 1f
    mov $-0x1, %rax
    jmp 2f
1:
    mov -0x8(%rbp), %rax
2:
    add $0x10, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_cread
# @param %rcx (wchar_t*) - buffer
# @param %edx (unsigned long) - how many chars to read
# @return %rax (long)
_Z_winapi_Wcread:
    push %rbp
    mov %rsp, %rbp
    sub $0x30, %rsp
    mov %rcx, -0x10(%rbp)
    mov %edx, -0x18(%rbp)
    mov $-0xa, %ecx
    call GetStdHandle

    lea -0x8(%rbp), %r9   # lpNumbersOfCharsRead
    mov -0x18(%rbp), %r8d # nNumberOfCharsToRead
    mov -0x10(%rbp), %rdx
    movq $0x0, 0x20(%rsp)
    mov %rax, %rcx
    call ReadConsoleW

    test %rax, %rax
    jne 1f
    mov $-0x1, %rax
    jmp 2f
1:  mov -0x8(%rsp), %rax
2:  add $0x30, %rsp
    pop %rbp
    retq


.globl _Z_winapi_close
.globl _Z_winapi_deletefile
.globl _Z_winapi_read
.globl _Z_winapi_write
.globl _Z_winapi_Wread
.globl _Z_winapi_Wwrite
.globl _Z_winapi_setfileptr
.globl _Z_winapi_mkdir
.globl _Z_winapi_wmkdir

# @brief _Z_winapi_close
# @param %rcx (HANDLE) - object's handler
_Z_winapi_close:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    callq CloseHandle
    add $0x20, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_deletefile
# @param %rcx (const char*) - file's name
# @return %rax (bool)
_Z_winapi_deletefile:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    callq DeleteFileA
    add $0x20, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_read
# @param %rcx (HANDLE) - file's handle
# @param %rdx (char*) - buf
# @param %r8 (ulong) - len
# @return number of bytes read, -1 on error
_Z_winapi_read:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp
    lea -0x8(%rbp), %r9
    movq $0x0, 0x20(%rsp)
    callq ReadFile
    test %rax, %rax
    je 1f
    mov -0x8(%rbp), %rax
    jmp 2f
1:  mov $-1, %rax
2:  add $0x40, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_Wread
# @param %rcx (HANDLE) - file's handle
# @param %rdx (wchar_t*) - buf
# @param %r8 (ulong) - len
# @return number of bytes read, -1 on error
_Z_winapi_Wread:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp
    lea -0x8(%rbp), %r9
    movq $0x0, 0x20(%rsp)
    callq ReadFile
    test %rax, %rax
    je 1f
    mov -0x8(%rbp), %rax
    jmp 2f
1:  mov $-1, %rax
2:  add $0x40, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_write
# @param %rcx (HANDLE) - file's handle
# @param %rdx (char*) - buf
# @param %r8 (ulong) - len
# @return number of bytes write, -1 on error
_Z_winapi_write:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp

    lea -0x8(%rbp), %r9
    movq $0x0, 0x20(%rsp)
    callq WriteFile
    test %rax, %rax
    je 1f
    mov -0x8(%rbp), %rax
    jmp 2f
1:  mov $-1, %rax
2:  add $0x40, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_Wwrite
# @param %rcx (HANDLE) - file's handle
# @param %rdx (wchar_t*) - buf
# @param %r8 (ulong) - len
# @return number of bytes write, -1 on error
_Z_winapi_Wwrite:
    push %rbp
    mov %rsp, %rbp
    sub $0x40, %rsp

    lea -0x8(%rbp), %r9
    movq $0x0, 0x20(%rsp)
    callq WriteFile
    test %rax, %rax
    je 1f
    mov -0x8(%rbp), %rax
    jmp 2f
1:  mov $-1, %rax
2:  add $0x40, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_setfileptr
# @param %rcx (HANDLE)
# @param %rdx (long) - offset
# @param %r8 (int) - whence
_Z_winapi_setfileptr:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp

    mov %r8, %r9
    mov $0, %r8
    callq SetFilePointer

    add $0x20, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_mkdir
# @param %rcx (char*) - dirname
# @return %rax (bool)
_Z_winapi_mkdir:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov $0x0, %rdx
    callq CreateDirectoryA
    add $0x20, %rsp
    pop %rbp
    retq

# @brief _Z_winapi_wmkdir
# @param %rcx (wchar*) - dirname
# @return %rax (bool)
_Z_winapi_wmkdir:
    push %rbp
    mov %rsp, %rbp
    sub $0x20, %rsp
    mov $0x0, %rdx
    callq CreateDirectoryW
    add $0x20, %rsp
    pop %rbp
    retq