#include <src/unix-x86_64/headers/std_S.h>
#include "zsr_std.h"
#include "zsr_xapi.h"
#ifdef ZSR_SYS_WIN64
#include "win-amd64/headers/std_S.h"
#elif defined(ZSR_SYS_UNIX64)
#include "unix-x86_64/headers/std_S.h"
#endif

zptr zalloc(zsize_t _size)
{
    return __z_xapi_alloc(_size, zfalse);
}

zptr zcalloc(zsize_t _size)
{
    return __z_xapi_alloc(_size, ztrue);
}

int zlasterror(char b, int err)
{
    static int lastError = 0;
    if (b)
    {
        lastError = err;
    }

    return lastError;
}

void zsetlasterror(int err)
{
    zlasterror(ztrue, err);
}

int zgetlasterror(void)
{
    return zlasterror(zfalse, 0);
}

#include "zsr_file.h"
#include "zsr_thread.h"
#include "zsr_socket.h"

zbool zclosehandler(zhandler hndl)
{
    zbool __status;
    switch (((struct __z_handler*) hndl)->hid)
    {
        case ZSR_HANDLER_FILE:
            __status = zfclose(hndl);
            break;
        case ZSR_HANDLER_SOCKET:
            __status = zclosesock(hndl);
            break;
        case ZSR_HANDLER_THREAD:
            // __status = zThreadDestroy(hndl);
            break;
        case ZSR_HANDLER_MUTEX:
            // __status = zMutexDestroy(hndl);
            break;
        case ZSR_HANDLER_COND:
            // __status = zCondDestroy(hndl);
            break;
        default:
            zsetlasterror(ZEINVAL);
            return zfalse;
    }

    if (!__status)
        zsetlasterror(ZEBUSY);
    return __status;
}

struct __z_hndldata zhndl_data(zphandler hndl, zuint32 offset, zuint32 sizeOf)
{
    struct __z_hndldata data;
    void* p = (&hndl->starting_point_data) + offset;
    switch (sizeOf)
    {
        case sizeof(char):
            data.__cdata = *((char*)p);
            break;
        case sizeof(zint16):
            data.__16intdata = *((zint16*)p);
            break;
        case sizeof(zint32):
            data.__32intdata = *((zint32*)p);
            break;
        case sizeof(zint64):
            data.__64intdata = *((zint64*)p);
            break;
    }

    data.__ptrdata = p;
    data.__sizeof = sizeOf;

    return data;
}

zbool zisLE()
{
    int __zreserved_n = 1;
    return (*(char*)&__zreserved_n == 0x1);
}
