# Unix x86_64 assembly files
# @file stdio.S - system call
# @version 2020/11 0.3.0
.att_syntax
.text

#### <<< stdio.S / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
# //

.globl _Z_sys_read
.globl _Z_sys_write
.globl _Z_sys_open
.globl _Z_sys_close
.globl _Z_sys_link
.globl _Z_sys_unlink
.globl _Z_sys_lseek
.globl _Z_sys_mkdir
.globl _Z_sys_rmdir


# @brief _Z_sys_read
# @param %rdi (char*) - buffer
# @param %rsi (zsize_t) - how many chars to read
# @param %rdx (unsigned int) - fd
# @return %rax (ssize_t) - numbers of char reads
_Z_sys_read:
    push %rdx

    mov %rsi, %rdx # (size_t) count
    mov %rdi, %rsi # (char*) buf
    pop %rdi       # (unsigned int) fd
    mov $0x0, %rax # sys_read
    syscall
    nop
    retq

# @brief _Z_sys_write
# @param %rdi (const char*) - buffer
# @param %rsi (zsize_t) - count
# @param %rdx (unsigned int) - fd
_Z_sys_write:
    push %rdx
    mov %rsi, %rdx  # count
    mov %rdi, %rsi  # buf
    pop %rdi        # fd
    mov $0x1, %rax  # sys_write
    syscall
    nop
    retq

# @brief _Z_sys_open
# @param %rdi (const char*) - name
# @param %rsi (int) - flags
# @param %rdx (int) - mode
# @return %rax (int) - fd
_Z_sys_open:
    mov $0x2, %rax
    syscall
    retq

# @brief _Z_sys_close
# @param %edi (uint) - fd
# @return %rax (int) - status
_Z_sys_close:
    mov $0x3, %rax
    syscall
    retq

# @brief _Z_sys_link
# @param %rdi (const char*) - oldname
# @param %rsi (const char*) - newname
# @return %rax (int) - status
_Z_sys_link:
    mov $0x56, %rax
    syscall
    retq

# @brief _Z_sys_unlink
# @param %rdi (const char*) - fname
# @return %rax (int) - status
_Z_sys_unlink:
    mov $0x57, %rax
    syscall
    retq

# @brief _Z_sys_lseek
# @param %edi (int) - fd
# @param %rsi (long) - offset
# @param %rdx (int) - whence
_Z_sys_lseek:
    mov $0x8, %rax
    syscall
    retq


.globl zmkdir
# @brief _Z_sys_mkdir
# @param %rdi (char*) - path
# @return %rax (bool)
_Z_sys_mkdir:
zmkdir:
    mov $0777, %rsi
    mov $0x53, %rax
    syscall
    ret

.globl zrmdir
# @brief _Z_sys_rmdir
# @param %rdi (char*) - path
# @return %rax (bool)
_Z_sys_rmdir:
zrmdir:
    mov $0x54, %rax
    syscall
    ret