#ifndef ZSR_CHENCODING_H
#define ZSR_CHENCODING_H
#include "zsr_std.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef char            ascii_t;
typedef unsigned char   latin1;
typedef char            utf8_t;
typedef short           utf16_t;
typedef int             utf32_t;
typedef int             unicode_t;

enum zch_encoding
{
    ASCII,
    LATIN_1,
    UTF_8,
    UTF_16,
    UTF_16BE,
    UTF_16LE,
    UTF_32
};

void zset_boutism(zbool big_endian);
zflag zboutism();

char* zfromchto(const void* buf, enum zch_encoding from, enum zch_encoding to);
char* zwchto(const wchar_t* buf, enum zch_encoding to);
char* zaschto(const ascii_t* buf, enum zch_encoding to);
char* zl1chto(const latin1* buf, enum zch_encoding to);
char* zutf8to(const utf8_t* buf, enum zch_encoding to);
char* zutf16to(const utf16_t *buf, enum zch_encoding to);
char* zutf16leto(const utf16_t* buf, enum zch_encoding to);
char* zutf16beto(const utf16_t* buf, enum zch_encoding to);
char* zutf32to(const utf32_t* buf, enum zch_encoding to);
unicode_t* zanychtouni(const void* buf, enum zch_encoding from);

zulong zaslen(const ascii_t*);
zulong zl1len(const latin1*);
zulong zutf8len(const utf8_t*);
zulong zutf16len(const utf16_t*);
zulong zutf32len(const utf32_t*);

zulong zutf8size(const utf8_t* buf);
zulong zutf16size(const utf16_t* buf);

#ifdef __cplusplus
};
#endif
#endif // ZSR_CHENCODING_H
