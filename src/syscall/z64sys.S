# Zsr x86_64 assembly files
# @file z64sys.S - syscall
# @os Windows 7, 8.1, 10
# @os UNIX/Kernel system
# @os GNU Linux (unix-like system)
# @version 2021/08 0.1.0
.macro zsyscall
call z64syscall
.endm

.set ZSYS_READ   , 0x00000600
.set ZSYS_WRITE  , 0x00000601
.set ZSYS_FOPEN  , 0x00000202
.set ZSYS_FCLOSE , 0x00000203

.set ZSYS_ALLOC_MAP     , 0x0001
.set ZSYS_ALLOC_ZERO    , 0x0002
.set ZSYS_ALLOC_REMAP   , 0x0004