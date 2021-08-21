#ifndef ZSR_STD_H
#define ZSR_STD_H
#include "zsr_stdlib.h"

typedef void* zptr;
typedef void* zpvoid;
typedef void* zhandler; /**< Global handler type for ZsrLib's features */

#ifdef ZSR_SYS_UNIX64
typedef int zfd_t;
#elif defined(ZSR_SYS_WIN64)
typedef void* zfd_t;
#endif

#define ZSR_STDLIB 202109050L //>! 2021/09 0.5.0 (R.M.m) .0000 <build-id>
#define ZSR_STDLIB_RELEASE      0    // 0
#define ZSR_STDLIB_MAJOR        5    //  .5
#define ZSR_STDLIB_MINOR        0    //    .0
#define ZSR_STDLIB_DP           0    // Development Phase
                                     // [0: prototype; 1: alpha; 2: beta; 3: RC, 4: Release]
#define ZSR_STDLIB_BUILDID      0000 // 0000 [no build]

struct
{
    int release,
        major,
        minor,
        buildid,
        development_phase;
} zsrcstd = {
        ZSR_STDLIB_RELEASE, ZSR_STDLIB_MAJOR, ZSR_STDLIB_MINOR,
        ZSR_STDLIB_BUILDID,ZSR_STDLIB_DP
}, *pzsrcstd = &zsrcstd;

#ifdef _MSC_BUILD
#define ZSR_COMPILER_MSC

#   define _Z_mscint(inst) inst
#   define _Z_gnuint(inst)
#   define _Z_xapieq(alias)
#elif defined(__GNUC__)
#define ZSR_COMPILER_GNU

#   define _Z_gnuint(inst) inst
#   define _Z_mscint(inst)
/**
 * Uses to call directly a x-platform valid declared function from the X/API;
 * @note Works only with GNU compiler (and alikes). MSC has to call an intermediate function.
 */
#   define _Z_xapieq(alias) _Z_gnuint(__asm__("__z_xapi" alias))
#endif

/*<< ZSR LIB KEYWORD >>*/

#define znull 0
#define zfalse 0
#define ztrue 1

typedef short zbool;
typedef unsigned long zenum;
typedef unsigned long zflag;

#ifndef __cplusplus
#ifdef ZSR_COMPILER_GNU
    typedef __WCHAR_TYPE__  wchar_t;
    typedef char            char8_t;
    typedef __CHAR16_TYPE__ char16_t;
    typedef __CHAR32_TYPE__ char32_t;
#else
    typedef unsigned int    wchar_t;
    typedef char            char8_t;
    typedef signed short    char16_t;
    typedef signed int      char32_t;
#endif  // ZSR_COMPILER_GNU
#endif  // __cplusplus

#define ZSUCCESS 0
#define ZFAILURE -1

typedef int                 zint;  /* format: %i */
typedef unsigned            zuint; /* format: %ui */
typedef long int            zlint; /* format: %li */
typedef unsigned long int   zulint; /* format:%lu */

typedef short          zshort;  /* format: %hi */
typedef unsigned short zushort; /* format: %uh */

typedef float       zfloat ;  /* format: %f */
typedef double      zdouble;  /* format: %lf */
typedef long double zldouble; /* format: %lf */

typedef long                zlong;   /* format: %l */
typedef long long           zllong;  /* format: %ll */
typedef unsigned long       zulong;  /* format: %ul */
typedef unsigned long long  zullong; /* format: %uL */

typedef char      zint8;
typedef short     zint16;
typedef int       zint32;
typedef long long zint64;

typedef char      zbyte;
typedef short     zword;
typedef int       zdword;
typedef long long zqword;

typedef unsigned char      zuint8;
typedef unsigned short     zuint16;
typedef unsigned int       zuint32;
typedef unsigned long long zuint64;

#ifdef ZSR_COMPILER_GNU
typedef __builtin_va_list zva_list;
#define zva_start(l, arg) __builtin_va_start(l, arg)
#define zva_end(l) __builtin_va_end(l)
#define zva_arg(l, type) __builtin_va_arg(l,type)
#elif defined(ZSR_COMPILER_MSC)
typedef char* zva_list;
#define zva_start(l, arg) __crt_va_start(l, arg)
#define zva_end(l) __crt_va_end(l)
#define zva_arg(l, type) __crt_va_arg(l,type)
#endif


#ifdef UNICODE
typedef wchar_t zchar_t;
#else
typedef char zchar_t;
#endif // UNICODE

/**
 * @brief zsrcall (ZSR STD CALL)
 * Simple macro defined as keyword to mention that a function belong
 * to ASM implementation
 * @warning All functions may not be available with all os/system_arch
 * @see `zsr_stdlib.h` description for further informations.
 */
#define zsrcall // ZSR Standard call
#define _Z_in
#define _Z_out
#define _Z_nonull
#define _Z_nullable
#ifdef ZSRLIB_64
typedef unsigned long long zalloc_s; // alloc size (64bit)
typedef unsigned long long zsize_t;

typedef zuint64 zbiguint;   // biggest unsigned integer
typedef zint64 zbigint;     // biggest integer
typedef zldouble zbigfloat; // biggest float
#else
typedef unsigned long int zalloc_s; // alloc size (32bit)
typedef unsigned long     zsize_t;

typedef zuint32 zbiguint;  // biggest unsigned integer
typedef zint32 zbigint;    // biggest integer
typedef zdouble zbigfloat; // biggest float
#endif // ZSRLIB_64

#define zerror zgetlasterror()
#define ZEAGAIN     0x00000010
#define ZEBUSY      0x00000020
#define ZEINVAL     0x00000040
#define ZENOLCK     0x00001000
#define ZENOMEM     0x00000080
#define ZENOSYS     0x00000100
#define ZEPERM      0x00000200
#define ZESRCH      0x00000400
#define ZETIMEDOUT  0x00000800

#define ZSR_HANDLER_FILE   0x00000010
#define ZSR_HANDLER_SOCKET 0x00000020
#define ZSR_HANDLER_PIPE   0x00000040
#define ZSR_HANDLER_THREAD 0x00000080
#define ZSR_HANDLER_MUTEX  0x00000081
#define ZSR_HANDLER_COND   0x00000082

#define ZSR_SYSINFO_LANG        //!
#define ZSR_SYSINFO_LOCALE      //!
#define ZSR_SYSINFO_OSNAME      //!
#define ZSR_SYSINFO_TOTALRAM    //! Total usable memory size
#define ZSR_SYSINFO_FREERAM     //! Available memory size
#define ZSR_SYSINFO_UPTIME      //! Seconds since boot
#define ZSR_SYSINFO_MEMUSAGE    //! Gets program's memory size usage

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    /**
     * @brief Allocate memory
     * @param [in] _size (zsize_t) - The pointer's size to allocate
     * @return A valid pointer on success, Null otherwise
     */
    zptr zalloc(zsize_t _size);
    /**
     * @brief Allocate memory initialised to zero
     * @param [in] _size (zsize_t) - The pointer's size to allocate
     * @return A valid pointer on success, Null otherwise
     */
    zptr zcalloc(zsize_t _size);
    /**
     * @brief Reallocate memory to a new pointer.
     * @warning Passed _ptr will be free
     * @param [free/in] _ptr (zptr) - Pointer to reallocate
     * @param [in] _size (zsize_t) - The new size to reallocate
     * @return The pointer reallocated to a new memory block
     */
    zptr zrealloc(zptr _ptr, zsize_t _size);
    /**
     * @brief Delete a pointer (free memory).
     * @param [in] _ptr (void*) - Pointer to release
     * @return True on success, False otherwise.
     */
    zbool zsrcall zfree(void* _ptr) _Z_gnuint(__asm__("__z_xapi_free"));
    /**
     * @brief Get pointer size.
     * @param _ptr (const void*) - The pointer to get the size of
     * @return The pointer size
     */
    zsize_t zsrcall zallocs(const void* _ptr) _Z_gnuint(__asm__("__z_xapi_memsize"));

    /**
     *
     * @param src
     * @param dest
     * @param _size
     */
    void zsrcall zmcopy(const void* src, void* dest, zsize_t _size);

    /**
     *
     * @param _ptr1
     * @param _ptr2
     */
    void zsrcall zmswap(zptr _ptr1, zptr _ptr2, zsize_t _size);

    /**
     * @brief Exit the process
     * @param status (int) - Status' code of how the program ended
     */
    void zsrcall zexit(int status) _Z_gnuint(__asm__("__z_xapi_exit"));

    /**
     *
     * @return
     */
    int zgetlasterror();

    /**
     * Close handler. Should be one of this list :
     * <ul>
     *  <li>zsr_file</li>
     *  <li>zsr_thread</li>
     *  <li>zsr_socket</li>
     *  <li>zsr_mutex</li>
     *  <li>zsr_cond</li>
     * </ul>
     * @param hndl (zhandler) - the handler to close
     * @return True if the handler has been successfuly closed. False otherwise. <br/>
     * @errors If false, check <b>zgetlasterror()</b> for more details :
     * <ul>
     *  <li><b>ZEINVAL</b> Invalid handler (not supported)</li>
     *  <li><b>ZEBUSY</b> Handler busy (e.g: thread running, mutex in use...)
     * </ul>
     */
    zbool zclosehandler(zhandler hndl);

    /**
     *
     * @return
     */
    zbool zisLE();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //ZSR_STD_H
