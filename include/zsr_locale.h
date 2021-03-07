#ifndef ZSR_LOCALE_H
#define ZSR_LOCALE_H
#include "zsr_std.h"

#ifndef ZSR_LC_CTYPE
#define ZSR_LC_CTYPE

typedef char          ascii_t;
typedef unsigned char latin1;
typedef char          utf8_t;
typedef int           utf32_t;

#if defined(ZSR_SYS_UNIX64) || defined(ZSR_SYS_UNIX32)
typedef short         utf16_t;
#elif defined(ZSR_SYS_WIN32) || defined(ZSR_SYS_WIN64)
typedef wchar_t     utf16_t;
#endif // utf16_t

#endif // ZSR_LC_CTYPE

typedef struct zsr_locale zsr_locale;
struct zsr_locale
{
    const char*
        LANG,
        LC_CTYPE,
        LC_NUMERIC,
        LC_TIME,
        LC_COLLATE,
        LC_MONETARY,
        LC_MESSAGES,
        LC_PAPER,
        LC_NAME,
        LC_ADDRESS,
        LC_TELEPHONE,
        LC_MEASUREMENT,
        LC_IDENTIFICATION,
        LC_ALL;
};

#ifdef __cplusplus
extern "C" {
#endif // c++

    zsr_locale getlocale();
    void setlocale(zsr_locale locale);

    utf32_t * asciitoutf32(const ascii_t * _str);

    ascii_t * utf32toascii(const utf32_t* _str);
    utf8_t * utf32toutf8(const utf32_t* _str, zulong* _Z_out size);
    utf16_t * utf32toutf16(const utf32_t* _str);
    zulong utf32len(const utf32_t* _str);

    utf8_t* latin1toutf8(const latin1* _str);
    utf16_t* latin1toutf16(const latin1* _str);
    utf32_t * latin1toutf32(const latin1* _str);

    utf32_t * utf8toutf32(const utf8_t * _str);
    zulong utf8len(const utf8_t * utf8);
    zulong utf8size(const utf8_t * utf8);

    utf32_t * utf16toutf32(const utf16_t * _str);
    zulong utf16len(const utf16_t* utf16);
    zulong utf16size(const utf16_t* utf16);
    utf16_t * utf16le(utf16_t* utf16);

#ifdef __cplusplus
};
#endif // c++

#endif // ZSR_LOCALE_H
