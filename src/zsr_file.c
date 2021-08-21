/** global `C` files
 * @file zsr_file.c - implementation
 * @version 2021/08 0.5.0
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_xapi.h"
#include "zsr_file.h"

zsr_file zfopen(const char* fname, zfmode_t mode)
{
    zflag openm = 0;
    zbool creat;
    switch (mode[0])
    {
        case 'r':
            openm |= ZSR_IO_OPENM_IN;
            creat = zfalse;
            switch (mode[1])
            {
                case '+':
                    openm |= ZSR_IO_OPENM_IN;
                    break;
                case 'b':
                    openm |= ZSR_IO_OPENM_BIN;
                    if (mode[2] == '+')
                        openm |= ZSR_IO_OPENM_IN;
                    break;
            }
            break;
        case 'w':
            openm |= ZSR_IO_OPENM_OUT | ZSR_IO_OPENM_TRUNC;
            creat = ztrue;
            switch (mode[1])
            {
                case '+':
                    openm |= ZSR_IO_OPENM_IN;
                    break;
                case 'b':
                    openm |= ZSR_IO_OPENM_BIN;
                    if (mode[2] == '+')
                        openm |= ZSR_IO_OPENM_IN;
                    break;
            }
            break;
        case 'a':
            openm |= ZSR_IO_OPENM_OUT | ZSR_IO_OPENM_APP;
            creat = ztrue;
            switch (mode[1])
            {
                case '+':
                    openm |= ZSR_IO_OPENM_IN;
                    break;
                case 'b':
                    openm |= ZSR_IO_OPENM_BIN;
                    if (mode[2] == '+')
                        openm |= ZSR_IO_OPENM_IN;
                    break;
            }
            break;
    }

    if (openm <= 0)
    {
        zsetlasterror(ZEINVAL);
        return znull;
    }
    ZHFILE hndl = __z_xapi_fopen(fname, openm, creat);
    return hndl;
}

#ifdef ZSR_COMPILER_MSC
zbool zfclose(zsr_file file)
{
    return __z_xapi_fclose(file);
}

zbool zfremove(const char* fname)
{
    return __z_xapi_fremove(fname);
}

zbool zfrename(const char* oldname, const char* newname)
{
    return __z_xapi_frename(oldname, newname);
}

zlong zfread(char* _Z_out buf, zulong len, zsr_file file)
{
    return __z_xapi_fread(buf, len, file);
}
#endif // ZSR_COMPILER_MSC

char zfgetch(zsr_file file)
{
    char c;
    if (__z_xapi_fread(&c, 1ul, file) == 1)
        return c;
    else return -1;
}



zlong zfreadln(char** _Z_out buf, zsr_file file)
{
    zlong len = 0, cc_len;
    if (((ZHFILE)file)->__cc_newline == ZSR_CC_CRLF)
    {
        cc_len = 2;

        char c;
        while (__z_xapi_fread(&c, 1, file) == 1)
        {
            if (c == ZSR_CC_CR)
            {
                if (__z_xapi_fread(&c,1,file) == 1)
                {
                    if (c == ZSR_CC_LF)
                        break;
                    else
                    {
                        len+=2;
                    }
                }
                else break; // EOF
            }
            else len++;
        }
    }
    else
    {
        cc_len = 1;

        char c;
        while (__z_xapi_fread(&c, 1, file) == 1)
        {
            if (c == (char) ZSR_CC_NEWLINE)
                break;
        }
    }

    char* line = zalloc(sizeof(char)*(len+1));
    line[len] = ZSR_CC_EOS;
    *buf = line;
    zfseek(file, -len-cc_len, ZSR_SEEK_CUR);
    return __z_xapi_fread(line, (zulong) len, file);
}

void zfccnl(short cc, zsr_file file)
{
    ((ZHFILE) file)->__cc_newline = cc;
}

short zfgetccnl(zsr_file file)
{
    return ((ZHFILE) file)->__cc_newline;
}

zlong zfseek(zsr_file file, long offset, int whence)
{
    return __z_xapi_lseek(offset, whence, file);
}

zlong zftell(zsr_file file)
{
    return __z_xapi_lseek(0, ZSR_SEEK_CUR, file);
}

zfd_t zfgetfd(zsr_file file)
{
    return ((struct __z_hndl_file*) file)->it;
}

int zfileno(zsr_file file)
{
    return ((struct __z_hndl_file*) file)->__os_fileno;
}

const char* zfgetname(zsr_file file)
{
    return ((struct __z_hndl_file*) file)->fname;
}

enum zch_encoding zfgetchencode(zsr_file file)
{
    return ((struct __z_hndl_file*) file)->fchencode;
}
