#ifndef ZSR_PRVT_H
#define ZSR_PRVT_H

void zsetlasterror(int err);
int zlasterror(char b, int err);

typedef struct __z_handler zrhandler, * zphandler;
struct __z_handler
{
    /**
     * @var hid : handler identifier (Zsr lib's API)
     * @var __r1align8 : reserved data for align(8)
     */
    zint32 hid, __r1align8;
#ifdef ZSR_SYS_UNIX64
    /**
     * @var it : File descriptor (unix-like)
     * @var __r2align8 : reserved data for align(8)
     */
    zint32 it, __r2align8;
#elif defined(ZSR_SYS_WIN64)
    zpvoid it;
#endif
    /**
     * Starting point for handler data
     */
    void* starting_point_data;
};

struct __z_hndldata
{
    char* __bufdata;
    zuint64 __buflen;

    zint64 __64intdata;
    zint32 __32intdata;
    zint16 __16intdata;
    char __cdata;

};

struct __z_hndldata zhndl_data(zphandler hndl, zuint32 offset, zuint32 sizeOf);

#endif // ZSR_PRVT_H
