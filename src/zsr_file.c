/** global `C` files
 * @file zsr_file.c - implementation
 * @version 2020/11 0.3.0
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_file.h"
#include "zsr_string.h"
#if defined(ZSR_SYS_WIN64) || defined (ZSR_SYS_WIN32)
#include "win-amd64/headers/stdio_S.h"
#define _zopen(x...) _Z_winapi_open(x)
#define _zclose(x...) _Z_winapi_close(x)
#define _zwopen(fd, x...) zfd_t fd = _Z_winapi_Wopen(x);
#elif defined (ZSR_SYS_UNIX64)
#include "unix-x86_64/headers/stdio_S.h"
#include "zsr_locale.h"
#define _zopen(x...) _Z_unix_open(x)
#define _zwopen(fd, name, openm, c) zfd_t fd; { utf8_t* __Z_utf8 = utf32toutf8(name, znull); \
                                fd = _Z_unix_open(__Z_utf8, openm, c); zfree(__Z_utf8); }
#define _zclose(x...) _Z_sys_close(x)
#endif

zsr_file zfopen(const char* name, zfmode_t mode)
{
    zflag openm = 0;
    zbool create = zfalse;
    {
        zulong i;
        for(i=0; i <= 2; i++)
        {
            switch (i) {
                case 0:
                    switch(mode[0])
                    {
                        case 'a':
                            openm |= Z_IO_OPENM_APP;
                        case 'w':
                            openm |= Z_IO_OPENM_OUT;
                            create = ztrue;
                            break;
                        case 'r':
                            openm |= Z_IO_OPENM_IN;
                            break;
                        default:
                            return znull;
                    }
                    break;
                case 1:
                    switch(mode[1])
                    {
                        case '+':
                            openm |= (Z_IO_OPENM_OUT | Z_IO_OPENM_IN);
                        case '\0':
                            i=2;
                            break;
                        case 'b':
                            openm |= Z_IO_OPENM_BIN;
                            break;
                    }
                    break;
                case 2:
                    if (mode[2] == '+')
                    {
                        openm |= (Z_IO_OPENM_OUT | Z_IO_OPENM_IN);
                    }
                    else if (mode[2] != '\0')
                    {
                        return znull;
                    }
                    break;
            }
        }
    }

    struct _z_file* file = znull;
    zfd_t fd = _zopen(name, openm, create);
    if (fd)
    {
        file = zalloc(sizeof(struct _z_file));
        file->fd = fd;
        file->name = zstrcopy(name);
#ifdef WIN32
        file->wname = zfalse;
#endif
        if (openm & Z_IO_OPENM_APP)
        {
            zfseek(file, 0, Z_SEEK_END);
        }
    }

    return file;
}

zsr_file zfwopen(const wchar_t* name, zfmode_t mode)
{
    zflag openm = 0;
    zbool create = zfalse;
    {
        zulong i;
        for(i=0; i < 2; i++)
        {
            switch (i) {
                case 0:
                    switch(mode[0])
                    {
                        case 'a':
                            openm |= Z_IO_OPENM_APP;
                        case 'w':
                            openm |= Z_IO_OPENM_OUT;
                            create = ztrue;
                            break;
                        case 'r':
                            openm |= Z_IO_OPENM_IN;
                        default:
                            return znull;
                    }
                    break;
                case 1:
                    switch(mode[1])
                    {
                        case '+':
                            openm |= Z_IO_OPENM_OUT | Z_IO_OPENM_IN;
                        case '\0':
                            i=2;
                            break;
                        case 'b':
                            openm |= Z_IO_OPENM_BIN;
                            break;
                    }
                    break;
                case 2:
                    if (mode[2] == '+')
                    {
                        openm |= (Z_IO_OPENM_OUT | Z_IO_OPENM_IN);
                    }
                    else if (mode[2] != '\0')
                    {
                        return znull;
                    }
                    break;
                default: break;
            }
        }
    }

    struct _z_file* file = znull;
    _zwopen(fd, name, openm, create)
    if (fd)
    {
        file = zalloc(sizeof(struct _z_file));
        file->fd = fd;
        file->name = (char*) zwstrcopy(name);
#ifdef WIN32
        file->wname = ztrue;
#endif

        if (openm & Z_IO_OPENM_APP)
        {
            zfseek(file, 0, Z_SEEK_END);
        }
    }

    return file;
}

zbool zfclose(zsr_file file)
{
    struct _z_file* zfile = file;
    zbool status = _zclose(zfile->fd);

    if (status)
    {
        zfree(zfile->name);
        zfree(zfile);
    }
    return status;
}

zfd_t zfgetfd(zsr_file file)
{
    return ((struct _z_file*) file)->fd;
}

const void* zfgetname(zsr_file file)
{
    return ((struct _z_file*) file)->name;
}

