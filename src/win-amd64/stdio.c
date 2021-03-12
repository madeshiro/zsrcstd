/** Windows amd64 `C` files
 * @file stdio.c - implementation
 * @version 2020/11 0.3.0
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_file.h"
#include "zsr_stdio.h"
#include "zsr_format.h"
#include "zsr_string.h"
#include "headers/stdio_S.h"

#include <windows.h>

zsrcall struct _z_file* _Z_os_filedesc(int fd)
{
    static struct _z_file _Z_stdin = {"stdin", znull, zfalse};
    static struct _z_file _Z_stdout = {"stdout", znull, zfalse};
    static struct _z_file _Z_stderr = {"stderr", znull, zfalse};

    switch (fd)
    {
        case 0:
            _Z_stdin.fd = GetStdHandle(STD_INPUT_HANDLE);
            return &_Z_stdin;
        case 1:
            _Z_stdout.fd = GetStdHandle(STD_OUTPUT_HANDLE);
            return &_Z_stdout;
        case 2:
            _Z_stderr.fd = GetStdHandle(STD_ERROR_HANDLE);
            return &_Z_stderr;
        default:
            return znull;
    }
}

// >>> STD I/O <<< //

void zprintc(char _c)
{
    _Z_winapi_cwrite(&_c, 1ul);
}

void zprint(const char *_str)
{
    _Z_winapi_cwrite(_str,zstrlen(_str));
}

void zprintln(const char* _str)
{
    _Z_winapi_cwrite(_str, zstrlen(_str));
    _Z_winapi_cwrite("\n\r", 2ul);
}

void zprintf(const char* _f, ...)
{
    zva_list valist;
    zva_start(valist, _f);
    zprint(zformatl(_f, valist));
    zva_end(valist);
}

zulong zwrite(const char* buf, zulong len)
{
    return _Z_winapi_cwrite(buf, len);
}

int zreadc()
{
    wchar_t b;
    _Z_winapi_Wcread(&b, 1l);
    return b;
}

int zread(char* buf, zuint32 _len)
{
    return _Z_winapi_cread(buf, _len);
}

void zwprintc(wchar_t c)
{
    _Z_winapi_Wcwrite(&c, 1l);
}

void zwprint(const wchar_t* _wstr)
{
    _Z_winapi_Wcwrite(_wstr, zwstrlen(_wstr));
}

void zwprintln(const wchar_t* _wstr)
{
    _Z_winapi_Wcwrite(_wstr, zwstrlen(_wstr));
    _Z_winapi_Wcwrite(L"\n\r", 2l);
}

void zwprintf(const wchar_t* _wf, ...)
{
    zva_list valist;
    zva_start(valist, _wf);
    zwprint(zwformatl(_wf, valist));
    zva_end(valist);
}

// >>> FILE I/O <<< //

HANDLE _Z_winapi_open(const char* __fname, zflag __openmode, zbool __create)
{
    DWORD dwDesiredAccess = (__openmode & Z_IO_OPENM_IN ? GENERIC_READ : 0) |
                            (__openmode & Z_IO_OPENM_OUT ? GENERIC_WRITE : 0),
          dwShareMode = 0x0,
          dwCreationDisposition,
          dwFlagsAndAttributes = 0x0;

    if (__openmode & Z_IO_OPENM_TRUNC)
    {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }
    else if (__create)
    {
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else dwCreationDisposition = OPEN_EXISTING;

    return CreateFileA(__fname, dwDesiredAccess, dwShareMode,
            znull, dwCreationDisposition, dwFlagsAndAttributes, znull);
}

HANDLE _Z_winapi_Wopen(const wchar_t* __fname, zflag __openmode, zbool __create)
{
    DWORD dwDesiredAccess = (__openmode & Z_IO_OPENM_IN ? GENERIC_READ : 0) |
                            (__openmode & Z_IO_OPENM_OUT ? GENERIC_WRITE : 0),
            dwShareMode = 0x0,
            dwCreationDisposition,
            dwFlagsAndAttributes = 0x0;

    if (__openmode & Z_IO_OPENM_TRUNC)
    {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }
    else if (__create)
    {
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else dwCreationDisposition = OPEN_EXISTING;

    return CreateFileW(__fname, dwDesiredAccess, dwShareMode,
                       znull, dwCreationDisposition, dwFlagsAndAttributes, znull);
}

zbool zmkdir(const char* dirname)
{
    return CreateDirectoryA(dirname, znull);
}

zbool zwmkdir(const wchar_t* dirname)
{
    return CreateDirectoryW(dirname, znull);
}

char zfgetch(zsr_file file)
{
    char c =  _Z_winapi_read(file->fd, &c, 1ul) == 1 ? c : -1;
    return c;
}

zlong zfread(char* buf, zulong len, zsr_file file)
{
    return _Z_winapi_read(file->fd, buf, len);
}

zlong zfreadln(char** buf, zsr_file file)
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
    wchar_t wchar = _Z_winapi_Wread(file->fd, &wchar, 1ul) == 1 ? wchar : -1;
}

zlong zwfread(wchar_t* buf, zulong len, zsr_file file)
{
    return _Z_winapi_Wread(file->fd, buf, len);
}

zlong zwfreadln(wchar_t** buf, zsr_file file)
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
    return _Z_winapi_write(file->fd, &c, 1l);
}

zlong zfputs(const char* str, zsr_file file)
{
    zulong strlen = zstrlen(str);
    return _Z_winapi_write(file->fd, str, strlen);
}

zlong zfprintf(const char* _format, zsr_file file, ...)
{
    zva_list valist;
    zva_start(valist, file);

    char* str = zformatl(_format, valist);
    zulong strlen = zstrlen(str);
    zlong ret = _Z_winapi_write(file->fd, str, strlen);
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
    return _Z_winapi_write(file->fd, buf, len);
}

zlong zfwriteln(const char* buf, zulong len, zsr_file file)
{
    zlong ret = _Z_winapi_write(file->fd, buf, len);
    _Z_winapi_write(file->fd, "\r\n", 2l);
    return ret;
}

zlong zwfput(wchar_t c, zsr_file file)
{
    return _Z_winapi_Wwrite(file->fd, &c, 1l);
}

zlong zwfputs(const wchar_t* str, zsr_file file)
{
    zulong strlen = zwstrlen(str);
    return _Z_winapi_Wwrite(file->fd, str, strlen);
}

zlong zwfprintf(const wchar_t* _format, zsr_file file, ...)
{
    zva_list valist;
    zva_start(valist, file);

    wchar_t* str = zwformatl(_format, valist);
    zulong strlen = zwstrlen(str);
    zlong ret = _Z_winapi_Wwrite(file->fd, str, strlen);
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
    return _Z_winapi_Wwrite(file->fd, buf, len);
}

zlong zwfwriteln(const wchar_t* buf, zulong len, zsr_file file)
{
    zlong ret = _Z_winapi_Wwrite(file->fd, buf, len);
    _Z_winapi_Wwrite(file->fd, L"\r\n", 2l);
    return ret;
}

zlong zfseek(zsr_file file, long offset, int whence)
{
    return _Z_winapi_setfileptr(file->fd, offset, whence);
}

zlong zftell(zsr_file file)
{
    return _Z_winapi_setfileptr(file->fd, 0, Z_SEEK_CUR);
}

zbool zfremove(const char* name)
{
    return _Z_winapi_deletefile(name);
}