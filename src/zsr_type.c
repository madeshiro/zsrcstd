/** global `C` files
 * @file zsr_type.c - implementation
 * @version 2020/11 0.3.0
 * @author Rin Baudelet (madeshiro)
 */
#include <zsr_stdio.h>
#include "zsr_type.h"
#include "zsr_math.h"

static zuint8 doublePrecision = 6;
static zuint8 floatPrecision = 6;

/* Integer/Float to String */

// >>> UTILS <<< //

zlong zitos_len(zbigint _int)
{
    zlong len = _int < 0;
    if (_int < 0) _int=-_int;
    do {
        len++;
        _int/=10;
    } while (_int > 0);
    return len;
}

zlong zutos_len(zbiguint _uint)
{
    zlong len = 0;
    do {
        len++;
        _uint/=10;
    } while (_uint > 0);
    return len;
}

zuint8 zfloatp()
{
    return floatPrecision;
}

zuint8 zdoublep()
{
    return doublePrecision;
}

zbool zsetfloatp(zuint8 p)
{
    if (p <= 0x7)
    {
        floatPrecision = p;
        return ztrue;
    }
    return zfalse;
}

zbool zsetdoublep(zuint8 p)
{
    if (p <= 0x10)
    {
        doublePrecision = p;
        return ztrue;
    }
    return zfalse;
}

// <<< Functions >>> //

char* zitos(zint i)
{
    zlong strlen = zitos_len(i), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    zbool c = i < 0;
    if (c)
    {
        str[0] = '-';
        i=-i;
    }

    for (index=strlen-1; index >= c; index--, i/=10)
    {
        str[index] = '0'+i%10;
    }
    return str;
}

char* zutos(zuint ui)
{
    zlong strlen = zutos_len(ui), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    for (index=strlen-1; index >= 0; index--, ui/=10)
    {
        str[index] = '0'+ui%10;
    }
    return str;
}

char* zhtos(zshort hi)
{
    zlong strlen = zitos_len(hi), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    zbool c = hi<0;
    if(c)
    {
        str[0]='-';
        hi=-hi;
    }

    for (index=strlen-1; index>=c;index--,hi/=10)
    {
        str[index] = '0'+hi%10;
    }

    return str;
}

char* zuhtos(zushort uh)
{
    zlong strlen = zutos_len(uh), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    for(index=strlen-1;index>=0;index--,uh/=10)
    {
        str[index] = '0'+uh%10;
    }
    return str;
}

char* zftos(zfloat f)
{
    zlong strlen = zitos_len((zllong) f) + floatPrecision + 1;
    zlong index,j;
    char* str = zcalloc(sizeof(char)*(strlen+1));
    str[strlen-floatPrecision-1] = '.';

    zllong lf = (zllong) f;
    zbool c = f<.0f;
    if (c)
    {
        str[0]='-';
        f=-f;
    }
    for(index=strlen-floatPrecision-2;index>=c;index--,lf/=10)
    {
        str[index] = '0'+lf%10;
    }
    f *= (float) zpow(10,floatPrecision);
    for(index=strlen-1,j=0;j<floatPrecision;index--,j++,f/=10)
    {
        char ch = ((zllong) f)%10;
        if (ch > 0 || j==floatPrecision-1)
        {
            ch += '0';
        }
        str[index] = ch;
    }

    return str;
}

char* zdtos(zdouble lf)
{
    zulong strlen = zitos_len((zllong) lf) + doublePrecision + 1;
    zlong index,j;
    char* str = zcalloc(sizeof(char)*(strlen+1));
    str[strlen-doublePrecision-1] = '.';

    zllong llf = (zllong) lf;
    zbool c = lf<.0f;
    if (c)
    {
        str[0]='-';
        lf=-lf;
    }
    for(index=strlen-doublePrecision-2;index>=c;index--,llf/=10)
    {
        str[index] = '0'+llf%10;
    }
    lf *= zpow(10,doublePrecision);
    for(index=strlen-1,j=0;j<doublePrecision;index--,j++,lf/=10)
    {
        char ch = ((zllong) lf)%10;
        if (ch > 0 || j==doublePrecision-1)
        {
            ch += '0';
        }
        str[index] = ch;
    }

    return str;
}

char* zltos(zlong l)
{
    zlong strlen = zitos_len(l), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    zbool c = l<0;
    if(c)
    {
        str[0] = '-';
        l=-l;
    }

    for(index=strlen-1;index>=c;index--,l/=10)
    {
        str[index] = '0'+l%10;
    }

    return str;
}

char* zlltos(zllong ll)
{
    zlong strlen = zitos_len(ll), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    zbool c = ll<0;
    if(c)
    {
        str[0] = '-';
        ll=-ll;
    }

    for(index=strlen-1;index>=c;index--,ll/=10)
    {
        str[index] = '0'+ll%10;
    }

    return str;
}

char* zultos(zulong ul)
{
    zlong strlen = zutos_len(ul), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    for (index=strlen-1;index>=0;index--, ul/=10)
    {
        str[index] = '0'+ul%10;
    }
    return str;
}

char* zulltos(zullong uL)
{
    zlong strlen = zutos_len(uL), index;
    char* str = zcalloc(sizeof(char)*(strlen+1));

    for (index=strlen-1; index>=0;index--, uL/=10)
    {
        str[index] = '0'+uL%10;
    }
    return str;
}

static char _z_hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

char * zinttohex(zbigint bint)
{
    zbool c = bint < 0;
    char* str = zcalloc(sizeof(char) * (sizeof(zbigint)+1+c));
    str[0] = '-';
    zlong i = c, bitwise = (sizeof(zbigint)*8)-4;

    str[i] = '0';
    str[i+1] = 'x';
    bint = c ? -bint : bint;

    for(c = zfalse, i+=2;bitwise >= 0;bitwise-=4)
    {
        char b = _z_hex[(bint >> bitwise) & 0xf];
        if (b != '0' || c)
        {
            str[i] = b;
            i++;
            c=ztrue;
        }
    }

    return str;
}

char* zi8tohex(zint8 i8)
{
    char* str = zcalloc(sizeof(char)*0x5);
    str[0] = '0';
    str[1] = 'x';

    zlong i;
    for (i=3; i>1; i--)
    {
        str[i] = _z_hex[i8&0xf];
        i8 >>= 4;
    }

    return str;
}

char* zi16tohex(zint16 i16)
{
    char* str = zcalloc(sizeof(char)*0x7);
    str[0] = '0';
    str[1] = 'x';

    zlong i;
    for (i=5; i>1; i--)
    {
        str[i] = _z_hex[i16&0xf];
        i16 >>= 4;
    }

    return str;
}

char* zi32tohex(zint32 i32)
{
    char* str = zcalloc(sizeof(char)*0xb);
    str[0] = '0';
    str[1] = 'x';

    zlong i;
    for (i=9; i>1; i--)
    {
        str[i] = _z_hex[i32&0xf];
        i32 >>= 4;
    }

    return str;
}

#ifdef ZSRLIB_64
char* zi64tohex(zint64 i64)
{
    char* str = zcalloc(sizeof(char)*0x13);
    str[0] = '0';
    str[1] = 'x';

    zlong i;
    for (i=17; i>1; i--)
    {
        str[i] = _z_hex[i64&0xf];
        i64 >>= 4;
    }

    return str;
}
#endif

zbigint zstoi(const char* str)
{
    zbigint _int=0;
    zlong strlen = zstrlen(str), i,j;
    zbool neg = str[0] == '-';

    for(i=strlen-1,j=0;i>=neg;i--,j++)
    {
        _int += (str[i]-'0')*zpow(10,j);
    }
    return neg ? -_int : _int;
}

zbiguint zstou(const char* str)
{
    zbiguint _int=0;
    zlong strlen = zstrlen(str), i,j;

    for(i=strlen-1,j=0;i>=0;i--,j++)
    {
        _int += (str[i]-'0')*zpow(10,j);
    }
    return _int;
}


zdouble zstod(const char* str)
{
    zdouble lf;
    zlong _i=0,_f=0, i, p;
    zlong lfi=0, lff=0;

    zbool c = str[0] == '-';

    for (i=c;str[i] != '.' && str[i] != ','; i++, _i++){}
    for (i++;str[i] != '\0'; _f++,i++){}

    for (i=c,p=_i-1;str[i] != '.' && str[i] != ','; i++, p--)
    {
        lfi += (str[i]-'0')*zpow(10,p);
    }
    for (i++,p=_f-1;str[i] != '\0'; i++,p--)
    {
        lff += (str[i]-'0')*zpow(10,p);
    }

    lf = ((zdouble) lff)/zpow(10,_f);
    lf += (zdouble) lfi;

    return c ? -lf : lf;
}

zfloat zstof(const char* str)
{
    zfloat lf;
    zlong _i=0,_f=0, i, p;
    zlong lfi=0, lff=0;

    zbool c = str[0] == '-';

    for (i=c;str[i] != '.' && str[i] != ','; i++, _i++){}
    for (i++;str[i] != '\0'; _f++,i++){}

    for (i=c,p=_i-1;str[i] != '.' && str[i] != ','; i++, p--)
    {
        lfi += (str[i]-'0')*zpow(10,p);
    }
    for (i++,p=_f-1;str[i] != '\0'; i++,p--)
    {
        lff += (str[i]-'0')*zpow(10,p);
    }

    lf = ((zfloat) lff)/zpow(10,_f);
    lf += (zfloat) lfi;

    return c ? -lf : lf;
}
