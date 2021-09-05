/** Unix x86_64 `C` files
 * @file std.c - implementation
 * @version 2021/09 0.5.0
 * @author Rin Baudelet (madeshiro)
 */
#include <errno.h>
#include "zsr_std.h"
#include "headers/std_S.h"
#include "src/zsr_xapi.h"

/**
 * @brief Translate unix/linux errno into ZLIB errno<br/>
 * This function is necessary to etablish synchronous errno value between all
 * supported Operating System.
 * @since 2021/03 0.4.0
 * @version 2021/09 0.5.0 (unix-x86_64 branch)
 */
void __zu64_errno()
{
    int ecode;
    switch (errno) {
        case EBUSY:
            ecode = ZEBUSY;
            break;
        case EINVAL:
            ecode = ZEINVAL;
            break;
        case ENOLCK:
            ecode = ZENOLCK;
            break;
        case ENOMEM:
            ecode = ZENOMEM;
            break;
        case ENOSYS:
            ecode = ZENOSYS;
            break;
        case EPERM:
            ecode = ZEPERM;
            break;
        case ESRCH:
            ecode = ZESRCH;
            break;
        case ETIMEDOUT:
            ecode = ZETIMEDOUT;
            break;
        case EAGAIN: default:
            ecode = ZEAGAIN;
            break;
    }

    zsetlasterror(ecode);
}