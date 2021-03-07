# Windows AMD64 assembly files
# @file socket.S - system call
# @version 2021/01 0.4.0.0003
.att_syntax
.text

#### <<< SOCKET / win-amd64 >>> #####
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RCX, RDX, R8, R9 (then stack in %rsp)
# > Floatings points : %xmm0-%xmm4 (then stack in %rsp)
# > /!\ exclusive orders (1: RCX or XMM0, 2: RDX or XMM1... not e.g: RCX, XMM0, RDX, ...) /!\
#
## >> ZSR_PIPE DESCRIPTION << ##
# :: zsr_pipe {
#
#   0x0  > (8bytes) uint32 - handler id ] header
#   0x8  > (4bytes) uint32 - fd         ] da\
#   0x10 > (8bytes) void*  - sockaddr   ]    \ta
#   0x18 > (4bytes) int32  - af         ]  m\
#   0x1c > (4bytes) int32  - type       ]   e\t
#   0x20 > (4bytes) int32  - protocol   ]     \a
#
# };
# > zsr_pipe's size = (0x24) 36 bytes
#
#
# \/