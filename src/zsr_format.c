/** global `C` files
 * @file zsr_format.c - implementation
 * @version 2020/11 0.3.0.0002
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_format.h"
#include "zsr_type.h"

enum _z_format
{
    _c  /* char */ = 'c',
    _s  /* string */ = 's',
    _d  /* integer */ = 'd',
    _i  /* int32 */ = 'i',
    _x  /* hex */ = 'x',
    _X  /* HEX */ = 'X',
    _u  /* uint32 */ = 'u',
    _p  /* pointer */ = 'p',
    _ui /* uint32 */ = ('u'<<8) + 'i',
    _li /* lint32 */ = ('l'<<8) + 'i',
    _lu /* luint32 */ = ('l'<<8) + 'u',
    _hi /* int16 */ = ('h'<<8) + 'i',
    _uh /* uint16 */ = ('u'<<8) + 'h',
    _l  /* lint */ = 'l',
    _ll /* lint64 */ = ('l'<<8) + 'l',
    _ul /* luint */ = ('u'<<8) + 'l',
    _uL /* luint64 */ = ('u'<<8) + 'L',
    _f  /* float simple precision */ = 'f',
    _lf /* long float double precision */ = ('l'<<8)+'f',
    _fdouble = ('n'<<8) + ('f'),
    _fstring = ('n'<<8) + ('s'),
    _finvalid = 0
};

enum _z_format _z_getformat(const char* _format, zlong* _Z_out _skip);
char* _z_getvalue(enum _z_format _format, zva_list ls, zlong* _Z_out len);

char* zformat(const char* _format, ...)
{
    zva_list  valist;
    zva_start(valist, _format);

    zulong _flen = zstrlen(_format), strlen = _flen;
    zlong fcur, scur, skip, len, i;
    char* buf = zcalloc(sizeof(char) * (strlen+1));

    for (fcur = scur = 0; fcur < _flen; fcur++)
    {
        enum _z_format f;
        if (_format[fcur] == '%' && (f = _z_getformat(&_format[fcur+1],&skip)) != _finvalid)
        {
            char* arg = _z_getvalue(f, valist, &len);
            zva_arg(valist, zbigint);
            if (scur + len >= strlen)
            {
                strlen+=len;
                buf = zrealloc(buf, sizeof(char)*(strlen+1));
            }
            for (i=0;i<len;i++, scur++)
                buf[scur] = arg[i];
            fcur += skip;
            zfree(arg);
        }
        else
        {
            if (scur == strlen)
            {
                strlen+=16;
                buf = zrealloc(buf, sizeof(char)*(strlen+1));
            }

            buf[scur] = _format[fcur];
            scur++;
        }
    }

    zva_end(valist);
    return buf;
}

char* zformatl(const char* _format, zva_list valist)
{
    zulong _flen = zstrlen(_format), strlen = _flen;
    zlong fcur, scur, skip, len, i;
    char* buf = zcalloc(sizeof(char) * (strlen+1));

    for (fcur = scur = 0; fcur < _flen; fcur++)
    {
        enum _z_format f;
        if (_format[fcur] == '%' && (f = _z_getformat(&_format[fcur+1],&skip)) != _finvalid)
        {
            char* arg = _z_getvalue(f, valist, &len);
            zva_arg(valist, zbigint);
            if (scur + len >= strlen)
            {
                strlen+=len;
                buf = zrealloc(buf, sizeof(char)*(strlen+1));
            }
            for (i=0;i<len;i++, scur++)
                buf[scur] = arg[i];
            fcur += skip;
            zfree(arg);
        }
        else
        {
            if (scur == strlen)
            {
                strlen+=16;
                buf = zrealloc(buf, sizeof(char)*(strlen+1));
            }

            buf[scur] = _format[fcur];
            scur++;
        }
    }

    return buf;
}

enum _z_format _z_getformat(const char* _format, zlong* _Z_out _skip)
{
    switch (_format[0])
    {
        case 'c': case 's': case 'd': case 'i': case 'f': case 'x': case 'p': case 'X':
            break;
        case 'u':
            switch (_format[1])
            {
                case 'i': case 'h': case 'l': case 'L':
                    *_skip = 2l;
                    return (_format[0]<<8)+_format[1];
            }
            break;
        case 'l':
            switch (_format[1])
            {
                case 'l': case 'i': case 'u': case 'f':
                    *_skip = 2l;
                    return (_format[0]<<8)+_format[1];
            }
            break;
        case 'h':
            switch (_format[1])
            {
                case 'i':
                    *_skip = 2l;
                    return (_format[0]<<8)+_format[1];
                default:
                    ;
            }
        default:
            *_skip = 0;
            return _finvalid;
    }
    *_skip = 1l;
    return _format[0];
}

char* _z_getvalue(enum _z_format _format, zva_list ls, zlong* _Z_out len)
{
    char* str;
    switch (_format)
    {
        case _c:
            *len = 1;
            return zstrcopy((char[2]){ zva_arg(ls, int), '\0'});
        case _s:
            str = zstrcopy(zva_arg(ls, char*));
            break;
        case _x: case _p:
            str = zinttohex(zva_arg(ls, zbigint));
            break;
        case _X:
            str = zstrupper(zinttohex(zva_arg(ls, zbigint)));
            break;
        case _hi:
            str = zhtos(zva_arg(ls, zbigint));
            break;
        case _d: case _i:
            str = zitos(zva_arg(ls, zbigint));
            break;
        case _li: case _l:
            str = zltos(zva_arg(ls, zbigint));
            break;
        case _ll:
            str = zlltos(zva_arg(ls, zbigint));
            break;
        case _ui: case _lu: case _uh: case _ul: case _uL: case _u:
            str = zulltos(zva_arg(ls, zbiguint));
            break;
        case _f:
            str = zdtos(zva_arg(ls, zdouble));
            break;
        case _lf:
            str = zdtos(zva_arg(ls, zdouble));
            break;
        default:
            *len = 0;
            return znull;
    }

    *len = zstrlen(str);
    return str;
}