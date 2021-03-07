#ifndef ZSYSCALL_H
#define ZSYSCALL_H
#ifdef ZSRLIB_64
// x...xxx = ID
// n...nnn = ARGS_COUNT
// a...aaa = 0=PINT_TYPE, 1=FLOAT_TYPE
//          ZSYS_*****      0brrrrrrrr ----aaaa aaaannnx xxxxxxxx
#define     ZSYS_FOPEN  /*  0b00000000,00000000,00000010,00000001 */\
                        0x00201
#define     ZSYS_FCLOSE 0x00202
#else
#endif
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#ifdef WIN64
    void* z64syswcall(unsigned long long,...);
#elif defined(UNIX64)
    void* z64sysucall(unsigned long long,...);
#endif // operating system
#ifdef __cplusplus
};
#endif // __cplusplus
#endif // ZSYSCALL_H
