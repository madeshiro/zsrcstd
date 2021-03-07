/** global `C` files
 * @file zsr_wformat.c - implementation
 * @version 2020/11 0.3.0.0002
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_format.h"
#include "zsr_type.h"

enum _z_format
{
    _c  /* wchar_t */ = L'c',
    _s  /* string */ = L's',
    _d  /* integer */ = L'd',
    _i  /* int32 */ = L'i',
    _x  /* hex */ = L'x',
    _u  /* uint32 */ = L'u',
    _ui /* uint32 */ = (L'u'<<8) + L'i',
    _li /* lint32 */ = (L'l'<<8) + L'i',
    _lu /* luint32 */ = (L'l'<<8) + L'u',
    _hi /* int16 */ = (L'h'<<8) + L'i',
    _uh /* uint16 */ = (L'u'<<8) + L'h',
    _l  /* lint */ = L'l',
    _ll /* lint64 */ = (L'l'<<8) + L'l',
    _ul /* luint */ = (L'u'<<8) + L'l',
    _uL /* luint64 */ = (L'u'<<8) + L'L',
    _f  /* float simple precision */ = L'f',
    _lf /* long float double precision */ = (L'l'<<8) + L'f',
    _finvalid = 0
};

enum _z_format _z_wgetformat(const wchar_t* _format, zlong* _Z_out _skip);
wchar_t* _z_wgetvalue(enum _z_format _format, zva_list ls, zlong* _Z_out len);

wchar_t* zwformat(const wchar_t* _format, ...)
{
    zva_list  valist;
    zva_start(valist, _format);

    zulong _flen = zwstrlen(_format), strlen = _flen;
    zlong fcur, scur, skip, len, i;
    wchar_t* buf = zcalloc(sizeof(wchar_t) * (strlen+1));

    for (fcur = scur = 0; fcur < _flen; fcur++)
    {
        enum _z_format f;
        if (_format[fcur] == L'%' && (f = _z_wgetformat(&_format[fcur+1],&skip)) != _finvalid)
        {
            wchar_t* arg = _z_wgetvalue(f, valist, &len);
            if (scur + len >= strlen)
            {
                strlen+=len;
                buf = zrealloc(buf, sizeof(wchar_t)*(strlen+1));
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
                buf = zrealloc(buf, sizeof(wchar_t)*(strlen+1));
            }

            buf[scur] = _format[fcur];
            scur++;
        }
    }

    zva_end(valist);
    return buf;
}

wchar_t* zwformatl(const wchar_t* _format, zva_list valist)
{
    zulong _flen = zwstrlen(_format), strlen = _flen;
    zlong fcur, scur, skip, len, i;
    wchar_t* buf = zcalloc(sizeof(wchar_t) * (strlen+1));

    for (fcur = scur = 0; fcur < _flen; fcur++)
    {
        enum _z_format f;
        if (_format[fcur] == L'%' && (f = _z_wgetformat(&_format[fcur+1],&skip)) != _finvalid)
        {
            wchar_t* arg = _z_wgetvalue(f, valist, &len);
            if (scur + len >= strlen)
            {
                strlen+=len;
                buf = zrealloc(buf, sizeof(wchar_t)*(strlen+1));
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
                buf = zrealloc(buf, sizeof(wchar_t)*(strlen+1));
            }

            buf[scur] = _format[fcur];
            scur++;
        }
    }

    return buf;
}

enum _z_format _z_wgetformat(const wchar_t* _format, zlong* _Z_out _skip)
{
    switch (_format[0])
    {
        case L'c': case L's': case L'd': case L'i': case L'f': case L'x':
            break;
        case L'u':
            switch (_format[1])
            {
                case L'i': case L'h': case L'l': case L'L':
                    *_skip = 2l;
                    return (_format[0]<<8)+_format[1];
            }
            break;
        case L'l':
            switch (_format[1])
            {
                case L'l': case L'i': case L'u': case L'f':
                    *_skip = 2l;
                    return (_format[0]<<8)+_format[1];
            }
            break;
        case L'h':
            switch (_format[1])
            {
                case L'i':
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

wchar_t* _z_wgetvalue(enum _z_format _format, zva_list ls, zlong* _Z_out len)
{
    char* str;
    wchar_t* wstr;
    switch (_format)
    {
        case _c:
            *len = 1;
            return zwstrcopy((wchar_t[2]){ zva_arg(ls, int), L'\0'});
        case _s:
            wstr = zwstrcopy(zva_arg(ls, wchar_t*));
            *len = zwstrlen(wstr);
            return wstr;
        case _x:
            str = zinttohex(zva_arg(ls, zbigint));
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
    return zwstrcast(str);
}