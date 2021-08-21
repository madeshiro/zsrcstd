#ifndef ZSR_THREAD_H
#define ZSR_THREAD_H
#include "zsr_std.h"
#include "zsr_string.h"

typedef zhandler zsr_thread;
typedef zhandler zsr_mutex;
typedef zhandler zsr_cond;
typedef zdword (*zthread_f)(void*);

#define ZSR_MUTEX_LOCKSUCCESS   0x0
#define ZSR_MUTEX_LOCKTIMEOUT   0x1
#define ZSR_MUTEX_LOCKFAILED    0xffffffff
#define ZSR_MUTEX_INVALID       0xe0

#ifdef __cplusplus
extern "C" {
#endif

    zsr_thread zthcreate(zthread_f startAddr)   _Z_xapieq("thcreate");
    zbool      zthclose(zsr_thread th)          _Z_xapieq("thclose");
    zbool      zthrun(zsr_thread th, void*)     _Z_xapieq("thrun");
    int        zthjoin(zsr_thread th)           _Z_xapieq("thjoin");
    zbool      zthstop(zsr_thread th, int code) _Z_xapieq("thabort");

#ifdef __cplusplus
};
#endif

#endif // ZSR_THREAD_H
