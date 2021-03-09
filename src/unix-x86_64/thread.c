#include "zsr_thread.h"
#include "headers/thread_S.h"

void zSleep(zlong millis)
{
    zThreadSleep(millis);
}

void zThreadSleep(zlong millis)
{
    zThreadNanoSleep(millis, 0l);
}

void zThreadNanoSleep(zlong millis, zlong nano)
{
    in_timespec.millis = millis;
    in_timespec.nano = nano;
    _Z_sys_nanosleep(&in_timespec, &out_timespec);
}
