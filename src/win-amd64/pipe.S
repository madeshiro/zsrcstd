# Windows AMD64 assembly files
# @file socket.S - system call
# @version 2021/01 0.4.0
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
# :: zsr_pipe / zsr_npipe {
#
#   0x0  > (8bytes) uint64 - fd       ]
#
# };
# > zsr_pipe's size = (0x1c) 28 bytes
#
#
# \/