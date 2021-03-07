# Unix x86_64 assembly files
# @file socket.S - zsr_list object implementation
# @version 2021/02 0.4.0.0004
.att_syntax # AT&T syntax
.text

#### <<< zsr_list / unix-x86_64 >>> ####
# > all functions are aligned with standard `.h` declarations (x-platform decl)
#
# >> REMINDER // Calling convention [amd64] <<
# > Integers & Pointers : RDI, RSI, RDX, RCX, R8, R9
# > Floatings points : %xmm0-%xmm7
#
## >> ZSR_PIPE DESCRIPTION << ##
# :: zsr_pipe {
#
#   0x0  > (4bytes) uint32 - handler id ] header
#   0x4  > (4bytes) uint32 - fd         ] da\
#   0x8  > (8bytes) void*  - sockaddr   ]    \ta
#   0x10 > (4bytes) int32  - af         ]  m\
#   0x14 > (4bytes) int32  - type       ]   e\t
#   0x18 > (4bytes) int32  - protocol   ]     \a
#
# };
# > zsr_pipe's size = (0x1c) 28 bytes
#
# //
