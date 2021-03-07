#ifndef ZSR_TYPE_H
#define ZSR_TYPE_H
#include "zsr_std.h"
#include "zsr_string.h"

#ifdef __cplusplus
extern "C" {
#endif // c++

    /* Integer/Float to String */
    char* zitos(zint i);
    char* zutos(zuint ui);

    char* zhtos(zshort hi);
    char* zuhtos(zushort uh);

    char* zftos(zfloat f);
    char* zdtos(zdouble lf);

    char* zltos(zlong l);
    char* zlltos(zllong ll);
    char* zultos(zulong ul);
    char* zulltos(zullong uL);

    char* zinttohex(zbigint bint);
    char* zi8tohex(zint8 i8);
    char* zi16tohex(zint16 i16);
    char* zi32tohex(zint32 i32);

#ifdef ZSRLIB_64
    char* zi64tohex(zint64 i64);
#endif // x86_64

    /* String to Integer/Float */
     zbigint zstoi(const char* str);
    zbiguint zstou(const char* str);


    zdouble zstod(const char* str);
    zfloat zstof(const char* str);

    zuint8 zfloatp();
    zuint8 zdoublep();

    zbool zsetfloatp(zuint8 p);
    zbool zsetdoublep(zuint8 p);

#define ZFLOATP zfloatp();
#define ZDOUBLEP zdoublep();

#ifdef __cplusplus
};
#endif // c++

#endif // ZSR_TYPE_H
