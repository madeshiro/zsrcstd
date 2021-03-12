#ifndef ZSR_THREAD_H
#define ZSR_THREAD_H
#include "zsr_std.h"
#include "zsr_string.h"

typedef void* zsr_thread;
typedef void* zsr_mutex;
typedef void* zsr_cond;
typedef zdword (*zthread_f)(void*);

#define Z_MUTEX_LOCKSUCCESS 0x0
#define Z_MUTEX_LOCKTIMEOUT 0x1
#define Z_MUTEX_LOCKFAILED 0xffffffff
#define Z_MUTEX_INVALID 0xe0

#ifdef __cplusplus
extern "C" {
#endif

    zsrcall zsr_thread zThreadCreate(zthread_f startAddr);
    zsrcall zbool zThreadDestroy(zsr_thread thread);

    zsrcall void zThreadStart(zsr_thread thread, void* params);
    zsrcall int  zThreadJoin(zsr_thread thread);
    zsrcall zbool zThreadCancel(zsr_thread thread);
    zsrcall zbool zThreadIsRunning(zsr_thread thread);

    zsrcall zsr_mutex zMutexCreate();
    zsrcall zbool zMutexDestroy(zsr_mutex lock);
    zsrcall zbool zMutexLock(zsr_mutex lock);
    zsrcall zbool zMutexUnlock(zsr_mutex lock);
//  zsrcall zbool zMutexHoldsLock(zsr_mutex lock);

    zsrcall zsr_cond zCondCreate();
    zsrcall zbool zCondDestroy(zsr_cond cond);
    zsrcall void  zThreadCondWait(zsr_cond cond);
    zsrcall zbool zThreadCondSignal(zsr_cond cond);

    zsrcall void   zSleep(zlong millis);
    zsrcall zulong zThreadGetPID();
    zsrcall void   zThreadSleep(zlong millis);
    zsrcall void   zThreadNanoSleep(zlong millis, zlong nano);

    zsrcall void zThreadSetName(zsr_thread thread, const zsr_string name);
    zsrcall const char* zThreadGetName(zsr_thread thread);
    zsrcall zulong zThreadTID(zsr_thread thread);

#ifdef __cplusplus
};
#endif

#endif // ZSR_THREAD_H
