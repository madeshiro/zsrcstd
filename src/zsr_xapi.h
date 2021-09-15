#ifndef ZSR_PRVT_H
#define ZSR_PRVT_H
#include "zsr_std.h"
#include "zsr_stdio.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    void zsetlasterror(int err);
    int zlasterror(char b, int err);
#ifdef __cplusplus
};
#endif

#ifdef ZSR_SYS_UNIX64
#define __z_defhndl() \
    zint32 hid, __r1align8, it, __r2align8
#elif defined(ZSR_SYS_WIN64)
#define __z_defhndl() \
    zint32 hid, __r1align8; \
    zpvoid it
#endif

struct
{
    int stdin, stdout, stderr;
    int dup_stdin, dup_stdout, dup_stderr;
} __z_os_stdio_fileno = {0,1,2,
                         -1, -1, -1};

#ifndef ZSR_DEFINED_STDFILENO
#define ZSR_DEFINED_STDFILENO
#   define zstdin  __z_xapi_fstdio(ZSR_IO_STDIN)
#   define zstdout __z_xapi_fstdio(ZSR_IO_STDOUT)
#   define zstderr __z_xapi_fstdio(ZSR_IO_STDERR)
#endif

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
    zpvoid __ptrdata;
    zuint32 __sizeof;

    zint64 __64intdata;
    zint32 __32intdata;
    zint16 __16intdata;
    char __cdata;
};

/**
 * @param hndl
 * @param offset
 * @param sizeOf
 * @return
 */
struct __z_hndldata zhndl_data(zphandler hndl, zuint32 offset, zuint32 sizeOf);

/* >>> system & memory x-platform API <<< */
// {
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    zsrcall void*   __z_xapi_alloc(zsize_t, zbool zero);
    zsrcall void*   __z_xapi_realloc(void*, zsize_t, zbool __free);
    zsrcall void*   __z_xapi_free(void*);
    zsrcall zsize_t __z_xapi_memsize(const void*);
    zsrcall void    __z_xapi_exit(int code);

    zsrcall zbool   __z_xapi_mkdir(const char*);
    zsrcall zbool   __z_xapi_rmdir(const char*);

    zsrcall void*   __z_xapi_sys_getinfo(zenum);
#ifdef __cplusplus
};
#endif // __cplusplus

// };
/* >>> stdio/basics x-platform API <<< */
// {

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    zsrcall zfd_t   __z_xapi_stdio(int fd);
    zsrcall int     __z_xapi_dup(int fd);
    zsrcall int     __z_xapi_dup2(int fd1, int fd2);
    zsrcall int     __z_xapi_read(char* _Z_out, zulong, zfd_t);
    zsrcall int     __z_xapi_write(const char*, zulong, zfd_t);
    zsrcall int     __z_xapi_lseek(long offset, int whence, zfd_t);

    /**
     * Return the initial version of a stdio channel (in, out, err)
     * @param fd (int) - the file descriptor of the stdio channel -> {0,1,2}
     * @return The initial fd for the standard input/output channel [console]
     */
    zsrcall int   __z_xapi_stdiosrc(int fd);
    /**
     * Return a stdio channel back to its initial value (in, out, err)
     * @param channel (int) - the channel to return back to normal -> {0,1,2}
     * @return The last fd used for this channel
     */
    zsrcall int   __z_xapi_stdioback(int channel);
#ifdef __cplusplus
};
#endif // __cplusplus
// }
/* >>> stdio/file x-platform API <<< */
// {

typedef struct __z_hndl_file ZRHFILE /* rvalue */, *ZHFILE;
struct __z_hndl_file
{
    __z_defhndl();
    /* [32]64bit */ char* fname;
    /* 32bit */ zflag fchencode;
    /* 32bit */ int   __os_fileno;  // specifically for windows to use
    /* 16bit */ short __cc_newline; // control char use to indicate a new line
                                    // By default, the CC is set according to the OS
                                    // use zfccnl to define the control char
    /* 16bit */ zbool unicode;

};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    zsrcall zfd_t __z_xapi_fopen(const char*, zflag openm, zbool creat);
    zsrcall zbool __z_xapi_fclose(ZHFILE);
    zsrcall int   __z_xapi_fread(char* _Z_out, zulong, ZHFILE);
    zsrcall int   __z_xapi_fwrite(const char*, zulong, ZHFILE);
    zsrcall zlong __z_xapi_fseek(long offset, int whence, ZHFILE);
    zsrcall zlong __z_xapi_ftell(ZHFILE);
    zsrcall zbool __z_xapi_flock(ZHFILE, zflag op);
    zsrcall zbool __z_xapi_frename(const char*, const char*);
    zsrcall zbool __z_xapi_fremove(const char*);

    zsrcall ZHFILE __z_xapi_fstdio(int fd);
    zsrcall int    __z_xapi_dup2stdio(int channel, ZHFILE);
#if defined(ZSR_SYS_WIN64) && defined(UNICODE)
    zfd_t __z_xapi_Wfopen(const wchar_t*, zflag openm);
    zbool __z_xapi_Wfclose(ZHFILE);
    int   __z_xapi_Wfread(wchar* _Z_out, zulong, ZHFILE);
    int   __z_xapi_Wfwrite(const wchar_t*, zulong, ZHFILE);
    zbool __z_xapi_Wfrename(const wchar_t*, const char*);
    zbool __z_xapi_Wfremove(const wchar_t*);
#endif // WIN64 && UNICODE
#ifdef __cplusplus
};
#endif // __cplusplus
// } # stdio/file
/* >>> thread x-platform API <<< */
// {

#ifdef ZSRLIB_64
#define ZSR_THREAD_STACKSIZE  0x800000  // 8Mio
#elif defined(ZSRLIB_32)
#define ZSR_THREAD_STACKSIZE  0x400000  // 4Mio
#endif // ZSRLIB_64

typedef struct __z_hndl_thread ZRHTHREAD /* rvalue */, *ZHTHREAD;
struct __z_hndl_thread
{
    __z_defhndl();
    char* thname;                   /**< (32/64bits) thread's name */
    /**
     *
     * @param args
     * @return
     */
    int (*entrypoint)(void* args);  /**< (32/64bits) thread's entry point   */
    void* stacks;                   /**< (64bits) thread's memory stack     */
    int exitcode;                   /**< (32bits) thread's exit code        */
    zbool running;                  /**< (16bits) running status            */
};

typedef struct __z_hndl_futex ZRHFUTEX /* rvalue */, *ZHFUTEX;
struct __z_hndl_futex
{
    __z_defhndl();
    zbool __lock;
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    zsrcall ZHTHREAD __z_xapi_thcreate(int (*startAddr)(void*));
    zsrcall zbool    __z_xapi_thclose(ZHTHREAD);
    zsrcall zbool    __z_xapi_thrun(ZHTHREAD, void*);
    zsrcall int      __z_xapi_thjoin(ZHTHREAD);
    zsrcall zbool    __z_xapi_thabort(ZHTHREAD, int _exit);

    zsrcall void __z_xapi_sleep(long millis);

    zsrcall ZHFUTEX __z_xapi_thmutex();
    zsrcall zbool   __z_xapi_thclosemutex(ZHFUTEX);
    zsrcall long    __z_xapi_thlock(ZHFUTEX);
    zsrcall zbool   __z_xapi_thunlock(ZHFUTEX);
#ifdef __cplusplus
};
#endif // __cplusplus
// } # thread
/* >>> stdio/file x-platform API <<< */
// {

typedef struct __z_fdt_pipe *ZHPIPEFD_T;
struct __z_fdt_pipe
{
    zfd_t in, out;
};

typedef struct __z_hndl_pipe ZRHPIPE /* rvalue */, *ZHPIPE;
struct __z_hndl_pipe
{
    __z_defhndl();
    char* name; /**< (32/64bits) Pipe's name; Null if it's an anonymous pipe */
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    zsrcall ZHPIPE __z_xapi_pipe(zflag openm);
    zsrcall ZHPIPE __z_xapi_fifo(const char*, zflag openm);
    zsrcall zbool  __z_xapi_closepipe(ZHPIPE);
    zsrcall int    __z_xapi_pread(char* _Z_out, zulong, ZHPIPE);
    zsrcall int    __z_xapi_pwrite(const char*, zulong, ZHPIPE);
    zsrcall zfd_t  __z_xapi_pfd(zflag which);
    zsrcall int    __z_xapi_pfileno(zflag which);

#ifdef __cplusplus
};
#endif // __cplusplus
// } # stdio/file
#endif // ZSR_PRVT_H
