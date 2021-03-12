#include <src/unix-x86_64/headers/std_S.h>
#include "zsr_std.h"
#include "zsr_prvt.h"
#ifdef ZSR_SYS_WIN64
#include "win-amd64/headers/std_S.h"
#elif defined(ZSR_SYS_UNIX64)
#include "unix-x86_64/headers/std_S.h"
#endif

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
    switch (_Z_hndlid(hndl))
    {
        case ZSR_HANDLER_FILE:
            __status = zfclose(hndl);
            break;
        case ZSR_HANDLER_SOCKET:
            __status = zclosesock(hndl);
            break;
        case ZSR_HANDLER_THREAD:
            __status = zThreadDestroy(hndl);
            break;
        case ZSR_HANDLER_MUTEX:
            __status = zMutexDestroy(hndl);
            break;
        case ZSR_HANDLER_COND:
            __status = zCondDestroy(hndl);
            break;
        default:
            zsetlasterror(ZEINVAL);
            return zfalse;
    }

    if (!__status)
        zsetlasterror(ZEBUSY);
    return __status;
}
