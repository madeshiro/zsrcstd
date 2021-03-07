/** global `C` files
 * @file zsr_string.c - implementation
 * @version 2020/11 0.3.0.0002
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_string.h"
#include "zsr_container.h"
#include "z-regexlib/regex.h"

zulong zstrlen(const char* buf)
{
    zulong len = 0;
    while (buf[len] != '\0')
    {
        len++;
    }
    return len;
}

char* zstrcopy(const char* buf)
{
    zulong len = zstrlen(buf);
    char* str = zalloc(sizeof(char)*(len+1));
    str[len] = '\0';

    zulong i;
    for (i = 0; i < len; i++) str[i] = buf[i];
    return str;
}

char* zstrupper(const char* str)
{
    zulong strlen = zstrlen(str), i;
    char* upper = zalloc(sizeof(char) * (strlen+1));

    for (i=0; i <= strlen; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            upper[i] = 'A' + (str[i]-'a');
        else upper[i] = str[i];
    }

    return upper;
}

char* zstrlower(const char* str)
{
    zulong strlen = zstrlen(str), i;
    char* upper = zalloc(sizeof(char) * (strlen+1));

    for (i=0; i <= strlen; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            upper[i] = 'a' + (str[i]-'Z');
        else upper[i] = str[i];
    }

    return upper;
}

zbool zstrequals(const char* str1, const char* str2)
{
    zuint str1len = zstrlen(str1), str2len = zstrlen(str2), i;
    if (str1len == str2len)
    {
        for (i=0; i < str1len; i++)
        {
            if (str1[i] != str2[i])
                return zfalse;
        }
        return ztrue;
    }
    return zfalse;
}

zbool zstrmatch(const char* str, const char* regex)
{
    return z_ascii_match(str, regex);
}

char* zstrsubstring(const char* str, zuint beg, zuint end)
{
    zulong strlen = zstrlen(str);
    end = end > strlen ? strlen : end;
    zulong sublen = end - beg + 1;
    if (beg < end && beg >= 0 && beg < strlen)
    {
        char* sub = zalloc(sizeof(char)*sublen);
        sub[sublen-1] = '\0';
        for (int i = beg, j=0; i < end; i++, j++)
        {
            sub[j] = str[i];
        }
        return sub;
    }
    else return znull;
}

char** zstrcut(const char* str, const char* delimiter, zuint32* _Z_out pCount)
{
    *pCount=1;
    zulong strlen = zstrlen(str),
           dlen = zstrlen(delimiter);
    char** cuts = zalloc(sizeof(char*));
    char* field = znull;
    for (int i=0, beg=0, findex=0; i < strlen; i++)
    {
        // test if need to increase array of fields
        if (*pCount == findex)
        {
            (*pCount)++;
            zrealloc(cuts, *pCount);
        }
        if (str[i] == delimiter[0])
        {
            int j;
            for (j=1; j < dlen; j++)
                if (str[j+i] != delimiter[j])
                    break;
            if (j == dlen)
            {
                field = zstrsubstring(str, beg, i);
                cuts[findex] = field;
                i+=dlen;
                beg = i;
            }
        }
    }

    return cuts;
}

void zstrfreecut(char** cuts, zuint32 count)
{
    for (int i = 0; i < count; i++)
    {
        if (cuts[i])
            zfree(cuts[i]);
    }
    zfree(cuts);
}

zulong zwstrlen(const wchar_t* buf)
{
    zulong len = 0;
    while(buf[len] != L'\0')
    {
        len++;
    }
    return len;
}

wchar_t* zwstrcopy(const wchar_t* buf)
{
    zulong len = zwstrlen(buf);
    wchar_t* wstr = zalloc(sizeof(wchar_t)*(len+1));
    wstr[len] = L'\0';

    zulong i;
    for (i=0; i < len; i++) wstr[i] = buf[i];
    return wstr;
}

wchar_t* zwstrcast(const char* buf)
{
    zulong buflen = zstrlen(buf), i;
    wchar_t* wstr = zcalloc(sizeof(wchar_t)*(buflen+1));
    for (i=0; i < buflen; i++) wstr[i] = buf[i];
    return wstr;
}

zbool zwstrequals(const wchar_t* str1, const wchar_t* str2)
{
    zulong str1len = zwstrlen(str1), str2len = zwstrlen(str2);
    if (str1len == str2len)
    {
        for (int i = 0; i < str1len; i++)
        {
            if (str1[i] != str2[i])
                return zfalse;
        }
        return ztrue;
    }
    return zfalse;
}

zbool zwstrmatch(const wchar_t* str, const wchar_t* regex)
{
    return z_unicode_match((utf32_t*) str, regex);
}

// @{ zsr_string functions }@ //

void zsrcall zStrlength_set(zsr_string _this, zuint32 len);
zsr_string zsrcall zStrGrow(zsr_string _this, zuint32 grow);
zsr_string zsrcall _zString(zuint32 capacity);

zsr_string zStrAppend(zsr_string *_this, const char* str)
{
    zulong strlen = zstrlen(str), i, _this_len = zStrlength(*_this);
    *_this = zStrGrow(*_this, strlen);

    for (i=0; i < strlen; i++)
    {
        (*_this)[i+_this_len] = str[i];
    }

    return *_this;
}

zsr_string zStrPrepend(zsr_string* _this, const char* str)
{
    zulong strlen = zstrlen(str), _this_len = zStrlength(*_this);
    zulong i,j;

    zsr_string _new = zString(str);
    _new = zStrGrow(_new, _this_len);

    for (i=0,j=strlen; i < _this_len; i++,j++)
    {
        _new[j] = (*_this)[i];
    }

    zDeleteString(*_this);
    *_this = _new;
    return *_this;
}

zsr_string zStrConcat(zsr_string* _this, zuint _cnt, const char* _str1, ...)
{
    zva_list arglist;
    zva_start(arglist, _str1);

    zulong i, j, _this_i, strlen, grow;
    const char** argv = zalloc(sizeof(char**)*_cnt);
    zulong* argvl = zalloc(sizeof(zulong*)*_cnt);
    argv[0] = _str1;
    argvl[0] = zstrlen(_str1);
    grow = argvl[0];

    for (i=1; i <_cnt; i++)
    {
        const char* strarg = zva_arg(arglist, const char*);
        strlen = zstrlen(strarg);
        grow += strlen;

        argv[i] = strarg;
        argvl[i] = strlen;
    }

    *_this = zStrGrow(*_this, grow);
    for (_this_i=0, i=0; i<_cnt; i++)
    {
        for (j=0; j<argvl[i]; j++, _this_i++)
        {
            (*_this)[_this_i] = argv[i][j];
        }
    }

    zva_end(arglist);
    return *_this;
}

zsr_string zStrConcatf(zsr_string* _this, const char* _f, ...)
{
    zva_list arglist;
    zva_start(arglist, _f);

    char* _format = zformatl(_f, arglist);
    zStrAppend(_this, _format);

    zva_end(arglist);
    zfree(_format);
    return *_this;
}

zsr_string zStrFormat(const zsr_string _this, ...)
{
    zva_list valist;
    zva_start(valist, _this);

    char* _format = zformatl(_this, valist);
    zsr_string _new = zString(_format);
    zfree(_format);

    zva_end(valist);
    return _new;
}

zsr_string zStrReplace(const zsr_string _this, const char* target, const char* replcmt)
{
    zulong find, i, j, k;
    zulong _thislen = zStrlength(_this),
           targetlen = zstrlen(target),
           replcmtlen = zstrlen(replcmt);
    z_list(zulong) find_i = zCreateList(sizeof(zulong));
    for (find=0, i=0; i < _thislen-targetlen+1; i++)
    {
        if (_this[i] == target[0])
        {
            for (j=1; j < targetlen; j++)
            {
                if (_this[i+j] != target[j])
                    break;
            }

            if (j == targetlen)
            {
                find++;
                zListAppend(&find_i, &i);
            }
        }
    }

    zulong strlen = _thislen + (find * replcmtlen) - (find * targetlen);
    zsr_string str = _zString(strlen);

    for (i=0, find=0; i < strlen; i++)
    {
        if (zListGet(zulong, find_i, find) == j)
        {
            for (k=0; k < replcmtlen; i++, k++)
            {
                str[i] = replcmt[k];
            }
            j+=targetlen;
            find++;
        }
        else
        {
            str[i] = _this[j];
        }
    }

    return str;
}

zsr_string zStrSubstring(const zsr_string _this, zuint beg, zuint end)
{
    zulong _this_len = zstrlen(_this), strlen = end-beg, i;
    if (beg > end || end > _this_len)
    {
        return znull;
    }

    zsr_string str = _zString(strlen);
    for (i=0; i < strlen; i++)
    {
        str[i] = _this[i+beg];
    }

    return str;
}

zsr_string zStrToLowercase(const zsr_string _this)
{
    zulong _thislen = zStrlength(_this), i;
    zsr_string lower = _zString(_thislen);

    for (i=0; i < _thislen; i++)
    {
        char b = _this[i];
        if (b >= 'A' && b <= 'Z')
        {
            lower[i] = 'a' + (b-'A');
        }
        else
        {
            lower[i] = b;
        }
    }

    return lower;
}

zsr_string zStrToUppercase(const zsr_string _this)
{
    zulong _thislen = zStrlength(_this), i;
    zsr_string upper = _zString(_thislen);

    for (i=0; i < _thislen; i++)
    {
        char b = _this[i];
        if (b >= 'a' && b <= 'z')
        {
            upper[i] = 'A' + (b-'a');
        }
        else
        {
            upper[i] = b;
        }
    }

    return upper;
}

zbool zStrEquals(const zsr_string str1, const char* str2)
{
    zulong str1len = zStrlength(str1), str2len = zstrlen(str2), i;
    if (str1len == str2len)
    {
        for (i=0; i < str1len; i++)
        {
            if (str1[i] != str2[i])
                return zfalse;
        }
        return ztrue;
    }
    return zfalse;
}

zbool zStrMatch(const zsr_string _this, const char* regex)
{
    return zstrmatch(_this, regex);
}

zbool zStrStartsWith(const zsr_string _this, const char* str)
{
    zulong _thislen = zStrlength(_this), strlen = zstrlen(str);
    if (_thislen >= strlen)
    {
        for (int i = 0; i < strlen; i++)
        {
            if (_this[i] != str[i])
                return zfalse;
        }
        return ztrue;
    }

    return zfalse;
}

