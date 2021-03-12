/** global `C` files
 * @file zsr_locale.c - implementation
 * @version 2020/11 0.3.0
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_locale.h"
#include "zsr_string.h"

// >>> ASCII CONVERT TO ... <<< //

utf32_t* asciitoutf32(const utf8_t* _str)
{
    zulong _strlen = zstrlen(_str);
    utf32_t* uni = zalloc(sizeof(utf32_t) * (_strlen+1));
    uni[_strlen] = L'\0';

    zulong i;
    for (i=0; i < _strlen; i++)
        uni[i] = (utf32_t) _str[i];
    return uni;
}

// >>> UNICODE (UTF32) CONVERT TO ... <<< //

/**
 *
 * @param buf
 * @param buflen
 * @return
 */
zulong _Z_unitoutf8size(const utf32_t* buf, zulong buflen);
/**
 *
 * @param wchar
 * @param buf
 * @return
 */
zuint32 _Z_unitoutf8(utf32_t wchar, utf8_t _Z_out _Z_nonull buf[4]);

ascii_t * utf32toascii(const utf32_t* _str)
{
    zulong i, len = utf32len(_str);
    utf32_t * ascii = zalloc(sizeof(utf32_t)*(len+1));
    ascii[len] = '\0';
    for(i=0; i < len; i++)
        ascii[i] = _str[i];
    return (ascii_t*) ascii;
}

utf8_t* utf32toutf8(const utf32_t* _str, zulong* _Z_out size)
{
    zulong _strlen = utf32len(_str), 
           utf8size = _Z_unitoutf8size(_str, _strlen);
    utf8_t* utf8 = zalloc(sizeof(utf8_t) * (utf8size+1));
          utf8[utf8size] = '\0';

    utf8_t c[4];
    zulong i, utf8cursor = 0, buflen;
    for (i=0; i < _strlen; i++)
    {
        buflen = _Z_unitoutf8(_str[i], c);
        zulong j;
        for (j=0; j < buflen; j++, utf8cursor++)
            utf8[utf8cursor] = c[j];
    }

    if (size) *size = utf8size;
    return utf8;
}

utf16_t * utf32toutf16(const utf32_t* _str)
{
    zulong strlen = utf32len(_str), utf16len, i,j;
    utf16_t* utf16 = zalloc(sizeof(utf16_t));

    for (utf16len=0,i=0;i<strlen;i++)
    {
        if (_str[i] <= 0xd7ff || (_str[i] >= 0xe000 && _str[i] <= 0xffff))
        {
            utf16len++;
        }
        else if(_str[i] >= 0x100000)
        {
            utf16len+=2;
        }
    }

    for (i=0,j=0; i < strlen; i++,j++)
    {
        if (_str[i] <= 0xd7ff || (_str[i] >= 0xe000 && _str[i] <= 0xffff))
        {
            utf16[j] = _str[i];
        }
        else if(_str[i] >= 0X100000)
        {
            utf32_t b32 = _str[i] - 0x10000;
            utf16_t b0 = 0xd800 + (0x3ff | (b32 >> 10)),
                    b1 = 0xdc00 + (0x3ff | b32);

            utf16[j] = b0;
            utf16[j++] = b1;
        }
    }

    return utf16;
}

zulong utf32len(const utf32_t* _str)
{
    zulong len;
    for (len=0;_str[len]!=U'\0'; len++){}
    return len;
}

// >>> LATIN-1 CONVERT TO ... <<< //

utf8_t* latin1toutf8(const latin1* _str)
{
    zulong buflen=0, strlen=zstrlen((const char*)_str), i,j;
    for(i=0;i<strlen;i++)
    {
        buflen += (_str[i] > 0x7f ? 2 : 1);
    }

    utf8_t* utf8 = zalloc(sizeof(utf8_t)*(buflen+1));
    utf8[buflen] = '\0';

    for(i=0,j=0;i<strlen;i++,j++)
    {
        latin1 b = _str[i];
        if (b > 0x7f)
        {
            char b1 = 0x80 & (b & 0x3f),
                 b0 = 0xc0 & ((b>>6) & 0x1f);

            utf8[j] = b0;
            utf8[j++] = b1;
        }
        else utf8[j] = (char) b;
    }

    return utf8;
}

utf16_t * latin1toutf16(const latin1* _str)
{
    zulong strlen = zstrlen((const char*)_str),i;
    utf16_t * utf16 = zalloc(sizeof(utf16_t)*(strlen+1));
    utf16[strlen] = u'\0';

    for(i=0;i<strlen;i++)
        utf16[i]=_str[i];
    return utf16;
}

utf32_t * latin1toutf32(const latin1* _str)
{
    zulong strlen=zstrlen((const char*) _str), i;
    utf32_t *utf32 = zalloc(sizeof(utf32_t)*(strlen+1));
    utf32[strlen] = U'\0';

    for(i=0;i<strlen;i++)
    {
        utf32[i] = (utf32_t)_str[i];
    }

    return utf32;
}

/* <<< UTF8 - UTILS >>> */

utf32_t * utf8toutf32(const utf8_t* _str)
{
    zulong strlen = utf8len(_str);
    utf32_t * utf32 = zalloc(strlen+1);
    utf32[strlen] = U'\0';

    zulong utf8_i, utf32_i;
    for (utf8_i=0, utf32_i=0; _str[utf8_i] != '\0'; utf8_i++, utf32_i++)
    {
        zenum count = 0;
        char b = _str[utf8_i];
        while ((b << count) & 0x80) {
            count++;
        }
        utf32_t b32;
        switch (count) {
            case 0:
                b32 = b;
                break;
            case 2:
                b32 = b & 0x1f;
                break;
            case 3:
                b32 = b & 0x0f;
                break;
            case 4:
                b32 = b & 0x07;
                break;
        }

        for(;count > 1; count--)
        {
            b32 <<= 6;
            utf8_i++;
            b32 |= (_str[utf8_i]&0x3f);
        }

        utf32[utf32_i] = b32;
    }

    return utf32;
}

zulong utf8len(const utf8_t* utf8)
{
    zulong len=0, i;
    for (i=0; utf8[i] != '\0'; i++, len++)
    {
        utf8_t c = utf8[i];
        if (c > 0x7f)
        {
            while (c & 0x80) {
                i++;
                c <<= 1;
            }
        }
    }

    return len;
}

zulong utf8size(const utf8_t* utf8)
{
    zulong len;
    for (len=0; utf8[len] != '\0'; len++)
    {/**/}
    return len;
}

/**
 * @brief
 * @param buf
 * @param size
 * @return
 */
zulong _Z_unitoutf8size(const utf32_t* buf, zulong buflen)
{
    zulong size = 0, i;
    for (i = 0; i < buflen; i++)
    {
        if (buf[i] <= 0x7f) size++;
        else if (buf[i] <= 0x7ff) size+=0x2;
        else if (buf[i] <= 0xffff) size+=0x3;
        else if (buf[i] <= 0x10ffff) size+=0x4;
    }
    return size;
}

zuint32 _Z_unitoutf8(utf32_t wchar, utf8_t _Z_out _Z_nonull buf[4])
{
    if (wchar <= 0x7f)
    {
        buf[0] = (utf8_t) wchar;
        return 1ul;
    }
    else if(wchar <= 0x7ff)
    {
        buf[1] = 0x80 | (wchar & 0x3f);
        buf[0] = 0xc0 | ((wchar>>6) & 0x1f);
        return 2ul;
    }
    else if(wchar <= 0xffff)
    {
        buf[2] = 0x80 | (wchar & 0x3f);
        buf[1] = 0x80 | ((wchar>>=6) & 0x3f);
        buf[0] = 0xe0 | ((wchar>>6) & 0x0f);
        return 3ul;
    }
    else if(wchar <= 0x10ffff)
    {
        buf[3] = 0x80 | (wchar & 0x3f);
        buf[2] = 0x80 | ((wchar >>= 6) & 0x3f);
        buf[1] = 0x80 | ((wchar >>= 6) & 0x3f);
        buf[0] = 0xf0 | ((wchar >> 6) & 0x07);
        return 4ul;
    }
    else
    {
        buf[3] = wchar & 0xff;
        buf[2] = (wchar >>= 8) & 0xff;
        buf[1] = (wchar >>= 8) & 0xff;
        buf[0] = (wchar >> 8) & 0xff;
        return 4ul;
    }
}

/* <<< UTF16 - UTILS >>> */

utf32_t * utf16toutf32(const utf16_t* _str)
{
    zulong utf32s = utf16len(_str),i,j;
    utf32_t * utf32 = zalloc(sizeof(utf32_t)*(utf32s+1));
    utf32[utf32s] = U'\0';

    for (j=0,i=0;i<utf32s;i++,j++)
    {
        utf16_t b0 = _str[j], b1;
        if (b0 <= 0xd7ff || (b0 >= 0xe000 && b0 <= 0xffff))
        {
            utf32[i] = b0;
        }
        else if (b0 >= 0xD800 && b0 <= 0xDBFF)
        {
            b1 = _str[j++];
            utf32[i] = 0x10000 + ((((b0 - 0xD800) * 0x400)<<16) | (b1-0xDC00));
        }
    }
    return utf32;
}

zulong utf16len(const utf16_t* utf16)
{
    zulong len = 0, i;
    for (i=0; utf16[i] != '\0'; i++,len++)
    {
        utf16_t b = utf16[i];
        if (b >= 0xD800 && b <= 0xDBFF)
        {
            i++;
        }
    }
    return len;
}

zulong utf16size(const utf16_t* utf16)
{
    zulong size;
    for (size=0; utf16[size] != '\0'; size++){}
    return size;
}

utf16_t* utf16le(utf16_t* utf16)
{
    zulong size = utf16size(utf16), i;
    utf16_t * utf16le = zalloc(sizeof(utf16_t)*(size+1));
    for(i=0; i < size; i++)
    {
        utf16_t b = utf16[i] >> 8;
        utf16le[i] = (utf16[i] << 8) | b;
    }
    return utf16le;
}