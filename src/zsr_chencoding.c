/** global `C` files
 * @file zsr_chencoding.c - implementation
 * @version 2021/03 0.4.2
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_chencoding.h"
#include "zsr_string.h"

static zbool big_endian_flag = ztrue;
void zset_boutism(zbool big_endian)
{
    big_endian_flag = big_endian;
}

zflag zboutism()
{
    return big_endian_flag;
}

char* zfromchto(const void* buf, enum zch_encoding from, enum zch_encoding to)
{
    switch (from)
    {
        case UTF_32:
            return zutf32to(buf, to);
        case UTF_16:
            return zutf16to(buf, to);
        case UTF_8:
            return zutf8to(buf, to);
        case LATIN_1:
            return zl1chto(buf, to);
        case ASCII:
            return zaschto(buf, to);
        default:
            return znull;
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
char* zwchto(const wchar_t* buf, enum zch_encoding to)
{
    switch (sizeof(wchar_t))
    {
        case sizeof(utf16_t):
            return zutf16to((const utf16_t *) buf, to);
        case sizeof(utf32_t):
            return zutf32to((const utf32_t *) buf, to);
    }
}
#pragma clang diagnostic pop

char* zaschto(const ascii_t* buf, enum zch_encoding to)
{
    zulong len = zstrlen(buf);
    switch (to)
    {
        case ASCII:
        case UTF_8:
        case LATIN_1:
            return zstrcopy(buf);
        case UTF_32:
        {
            utf32_t * buf32 = zalloc(sizeof(utf32_t)*(len+1));
            buf32[len] = 0x0;
            for (int i = 0; i < len; i++)
                buf32[len] = (utf32_t) buf[i];
            return (char *) buf32;
        }
        case UTF_16:
            if (zboutism())
                goto BE;
        case UTF_16LE:
        {
            utf16_t * buf16 = zalloc(sizeof(utf16_t) * (len + 1));
            buf16[len] = 0x0;
            for (int i = 0; i < len; i++)
            {
                utf16_t c = buf[i];
                buf16[len] = c<<8;
            }
            return (char *) buf16;
        }
    BE: case UTF_16BE:
        {
            utf16_t * buf16 = zalloc(sizeof(utf16_t) * (len + 1));
            buf16[len] = 0x0;
            for (int i = 0; i < len; i++)
            {
                utf16_t c = (utf16_t) buf[i];
                buf16[len] = c;
            }
            return (char *) buf16;
        }
    }
}

zulong zaslen(const ascii_t* buf)
{
    int len;
    for (len = 0; buf[len] != '\0'; len++) ;
    return len;
}

zulong zl1len(const latin1* buf)
{
    int len;
    for (len=0;buf[len] != u'\0';len++) ;
    return len;
}

zulong zutf8len(const utf8_t* buf)
{
    int len=0;
    for (int i = 0; buf[i] != '\0'; i++, len++)
    {
        utf8_t b = buf[i];
        if (b & 0x8)
        {
            for (b<<=1; b & 0x8; b<<=1) i++;
        }
    }
    return len;
}

zulong zutf16len(const utf16_t* buf)
{
    int len;
    for(len=0;buf[len] != 0x0; len++) ;
    return len;
}

zulong zutf32len(const utf32_t* buf)
{
    int len;
    for(len=0;buf[len] != 0x0; len++) ;
    return len;
}

zulong zutf8size(const utf8_t* buf)
{
    int len;
    for (len = 0; buf[len] < '\0'; len++) ;
    return len;
}

// %any% to UTF-8 //

utf8_t * __z_utf32toutf8(const utf8_t* buf)
{
    zulong buflen = zutf32len((const utf32_t *) buf);
}