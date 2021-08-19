#ifndef ZSR_STDIO_H
#define ZSR_STDIO_H
#include "zsr_std.h"

#define ZSR_IO_OPENM_IN     0x1
#define ZSR_IO_OPENM_OUT    0x2
#define ZSR_IO_OPENM_APP    0x4
#define ZSR_IO_OPENM_ATE    0x8
#define ZSR_IO_OPENM_BIN    0x10
#define ZSR_IO_OPENM_TRUNC  0x20

#define ZSR_IO_STDIN    0x0
#define ZSR_IO_STDOUT   0x1
#define ZSR_IO_STDERR   0x2

#define ZSR_SEEK_SET 0
#define ZSR_SEEK_CUR 1
#define ZSR_SEEK_END 2

#ifdef __cplusplus
extern "C" {
#endif // c++

    void zprintc(char _c);
    void zprint(const char* _str);
    void zprintln(const char* _str);
    void zprintf(const char* _f, ...);
    zulong zwrite(const char* buf, zulong len);

    int zreadc();
    int zread(char* _buf, zuint32 _len);
    char* zreadln(zint32 * _Z_out _len);

    void zwprintc(wchar_t c);
    void zwprint(const wchar_t* _wstr);
    void zwprintln(const wchar_t* _wstr);
    void zwprintf(const wchar_t* _wf, ...);

#ifdef __cplusplus
};
#endif // c++

#endif // ZSR_STDIO_H
