#include "zsr_std.h"
#include "zsr_prvt.h"

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
