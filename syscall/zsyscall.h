/**
 * @file zsyscall.h
 * @author Rin Baudelet
 * @version 2021/03 0.4.1
 */
#ifndef ZSYSCALL_H
#define ZSYSCALL_H
#ifdef ZSRLIB_64
// x...xxx = ID
// n...nnn = ARGS_COUNT
// a...aaa = 0=PINT_TYPE, 1=FLOAT_TYPE
//          ZSYS_*****             // 0brrrr.rrrr ----.aaaa aaaa.nnnx xxxx.xxxx
#define     ZSYS_READ   0x00000600 // 0b0000.0000,0000.0000,0000.0110,0000.0000
#define     ZSYS_WRITE  0x00000601 // 0b0000.0000,0000.0000,0000.0110,0000.0001
#define     ZSYS_FOPEN  0x00000202 // 0b0000.0000,0000.0000,0000.0010,0000.0001
#define     ZSYS_FCLOSE 0x00000203 // etc...
#else
#endif
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#ifdef WIN64
    void* z64syswcall(unsigned long long,...);
#define z64syscall(args...) z64syswcall(args...)
#elif defined(UNIX64)
    void* z64sysucall(unsigned long long,...);
#define z64syscall(args...) z64sysucall(args...)
#endif // operating system
#ifdef __cplusplus
};
#endif // __cplusplus
#endif // ZSYSCALL_H
