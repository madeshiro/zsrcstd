/** Unix x86_64 `C` files
 * @file stdio.c - implementation
 * @version 2020/11 0.3.0.0002
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_std.h"
#include "zsr_locale.h"
#include "zsr_string.h"
#include "zsr_format.h"
#include "headers/stdio_S.h"
#include <sys/file.h>

zsrcall struct _z_file* _Z_os_filedesc(int fd)
{
    static struct _z_file _Z_stdin = {"stdin", 0};
    static struct _z_file _Z_stdout = {"stdout", 1};
    static struct _z_file _Z_stderr = {"stderr", 2};
    switch (fd)
    {
        case 0:
            return &_Z_stdin;
        case 1:
            return &_Z_stdout;
        case 2:
            return &_Z_stderr;
        default:
            return znull;
    }
}

void zprintc(char _c)
{
    _Z_sys_write(&_c, 1ul, 1);
}

void zprintln(const char* _str)
{
    _Z_sys_write(_str, zstrlen(_str), 1);
    _Z_sys_write("\n", 1ul, 1);
}

void zprint(const char* _str)
{
    _Z_sys_write(_str, zstrlen(_str), 1);
}

void zprintf(const char* _f, ...)
{
    zva_list valist;
    zva_start(valist, _f);
    zprint(zformatl(_f, valist));
    zva_end(valist);
}

int zreadc()
{
    int c = 0; char b;
    do {
        _Z_sys_read(&b, 1ul, 0);
        c <<= 8;
        c |= b;
    } while (b & 0x80);
    return c;
}

int zread(char* _buf,  zuint32 _len)
{
    return _Z_sys_read(_buf, _len, 0);
}

void zwprint(const wchar_t* _wstr)
{
    zulong _size;
    char* _buf = utf32toutf8((const utf32_t *) _wstr, &_size);
    _Z_sys_write(_buf, _size,1);
}

void zwprintc(wchar_t c)
{
    wchar_t wchar[2] = {c, L'\0'};
    zwprint(wchar);
}

void zwprintln(const wchar_t* _wstr)
{
    zwprint(_wstr);
    _Z_sys_write("\n", 1ul, 1);
}

void zwprintf(const wchar_t* _f, ...)
{
    zva_list valist;
    zva_start(valist, _f);
    zwprint(zwformatl(_f, valist));
    zva_end(valist);
}

zfd_t _Z_unix_open(const char* fname, zflag openm, zbool create)
{
    zenum flags = create ? O_CREAT : 0x0;
    flags |= Z_IO_OPENM_TRUNC & openm ? O_TRUNC : 0x0;
    flags |= Z_IO_OPENM_APP & openm ? O_APPEND : 0x0;
    if ((openm & (Z_IO_OPENM_IN | Z_IO_OPENM_OUT)) == (Z_IO_OPENM_IN|Z_IO_OPENM_OUT))
    {
        flags |= O_RDWR;
    }
    else if (openm & Z_IO_OPENM_OUT)
    {
        flags |= O_WRONLY;
    }
    else if (!(openm & Z_IO_OPENM_IN))
    {
        return -1;
    }

    return _Z_sys_open(fname, flags, 0777);
}

zbool zfremove(const char* fname)
{
    return _Z_sys_unlink(fname) == 0;
}

zbool zfrename(const char* oldname, const char* newname)
{
    return _Z_sys_link(oldname, newname);
}

char zfgetch(zsr_file file)
{
    char c;
    _Z_sys_read(&c, 1, file->fd);
    return c;
}

zlong zfread(char* _Z_out buf, zulong len, zsr_file file)
{
    return _Z_sys_read(buf, len, file->fd);
}

zlong zfreadln(char** _Z_out buf, zsr_file file)
{
    zlong size = 16, i;
    *buf = zalloc(sizeof(char)*16);
    (*buf)[15] = '\0';

    char c;
    for (i=0; (c=zfgetch(file)) != '\n' && c != -1; i++)
    {
        if (i == (size-1))
        {
            *buf = zrealloc(*buf, sizeof(char) * (size+=16));
        }

        (*buf)[i] = c;
    }

    return i;
}

wchar_t zwfgetch(zsr_file file)
{
    wchar_t wchar;
    _Z_sys_read((char*) &wchar, 4, file->fd);
    return wchar;
}

zlong zwfread(wchar_t* _Z_out buf, zulong len, zsr_file file)
{
    return _Z_sys_read((char*)buf, len*sizeof(wchar_t), file->fd);
}

zlong zwfreadln(wchar_t** _Z_out buf, zsr_file file)
{
    zlong size = 16, i;
    *buf = zalloc(sizeof(wchar_t)*16);
    (*buf)[15] = '\0';

    char c;
    for (i=0; (c=zwfgetch(file)) != '\n'; i++)
    {
        if (i == (size-1))
        {
            *buf = zrealloc(*buf, sizeof(wchar_t) * (size+=16));
        }

        (*buf)[i] = c;
    }

    return i;
}

zlong zfput(char c, zsr_file file)
{
    return _Z_sys_write(&c, 1, file->fd);
}

zlong zfputs(const char* str, zsr_file file)
{
    return _Z_sys_write(str, zstrlen(str), file->fd);
}

zlong zfprintf(const char* _format, zsr_file file, ...)
{
    zva_list valist;
    zva_start(valist, file);

    char* str = zformatl(_format, valist);
    zulong strlen = zstrlen(str);
    zlong ret = _Z_sys_write(str, strlen, file->fd);
    zfree(str);

    zva_end(valist);
    return ret;
}

zlong zfprintln(const char* str, zsr_file file)
{
    return zfwriteln(str, zstrlen(str), file);
}

zlong zfwrite(const char* buf, zulong len, zsr_file file)
{
    return _Z_sys_write(buf, len, file->fd);
}

zlong zfwriteln(const char* buf, zulong len, zsr_file file)
{
    zlong ret = _Z_sys_write(buf, len, file->fd);
    _Z_sys_write("\r\n", 2, file->fd);
    return ret;
}

zlong zwfput(wchar_t c, zsr_file file)
{
    return _Z_sys_write((char*)&c, 4, file->fd) / 4;
}

zlong zwfputs(const wchar_t* str, zsr_file file)
{
    return _Z_sys_write((const char*) str, zwstrlen(str)*4ul, file->fd) / 4l;
}

zlong zwfprintf(const wchar_t* _format, zsr_file file, ...)
{
    zva_list valist;
    zva_start(valist, file);

    wchar_t* str = zwformatl(_format, valist);
    zulong strlen = zwstrlen(str);
    zlong ret = _Z_sys_write((const char*)str, 4ul*zwstrlen(str), file->fd) / 4;
    zfree(str);

    zva_end(valist);
    return ret;
}

zlong zwfprintln(const wchar_t* str, zsr_file file)
{
    return zwfwriteln(str, zwstrlen(str), file);
}

zlong zwfwrite(const wchar_t* buf, zulong len, zsr_file file)
{
    return _Z_sys_write((const char*)buf, len*4ul, file->fd);
}

zlong zwfwriteln(const wchar_t* buf, zulong len, zsr_file file)
{
    zlong ret = _Z_sys_write((const char*)buf, len*sizeof(wchar_t), file->fd);
    _Z_sys_write((const char*)L"\r\n", 2*sizeof(wchar_t), file->fd);
    return ret;
}

zlong zfseek(zsr_file file, long offset, int whence)
{
    return _Z_sys_lseek(file->fd, offset, whence);
}

zlong zftell(zsr_file file)
{
    return _Z_sys_lseek(file->fd, 0, Z_SEEK_CUR);
}
