#ifndef ZSR_STD_H
#define ZSR_STD_H
#include "zsr_stdlib.h"

typedef void* zptr;
typedef void* zpvoid;
typedef void* zhandler; /**< Global handler type for ZsrLib's features */

#define ZSR_STDLIB 202103040L //! 2021/03 0.4.0 (R.M.m) .0004 patch

/*<< ZSR LIB KEYWORD >>*/

#define znull 0
#define zfalse 0
#define ztrue 1

typedef short zbool;
typedef unsigned long zenum;
typedef unsigned long zflag;

#ifndef __cplusplus
typedef __WCHAR_TYPE__  wchar_t;
typedef char            char8_t;
typedef __CHAR16_TYPE__ char16_t;
typedef __CHAR32_TYPE__ char32_t;
#endif  // c++

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

typedef __builtin_va_list zva_list;
#define zva_start(l, arg) __builtin_va_start(l, arg)
#define zva_end(l) __builtin_va_end(l)
#define zva_arg(l, type) __builtin_va_arg(l,type)

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
#endif // ZSRLIB 64bit

#define zerror zgetlasterror()
#define ZEAGAIN     0x00000010
#define ZEINVAL     0x00000020
#define ZENOMEM     0x00000040
#define ZENOSYS     0x00000080
#define ZEPERM      0x00000100
#define ZESRCH      0x00000200
#define ZETIMEDOUT  0x00000400

#define ZSR_HANDLER_FILE   0x00000010
#define ZSR_HANDLER_SOCKET 0x00000020
#define ZSR_HANDLER_PIPE   0x00000040
#define ZSR_HANDLER_THREAD 0x00000080

#ifdef __cplusplus
extern "C" {
#endif // c++

    /**
     * @brief Allocate memory
     * @param [in] _size (zsize_t) - The pointer's size to allocate
     * @return A valid pointer on success, Null otherwise
     */
    zptr zsrcall zalloc(zsize_t _size) __asm__("_Z_alloc");
    /**
     * @brief Allocate memory initialised to zero
     * @param [in] _size (zsize_t) - The pointer's size to allocate
     * @return A valid pointer on success, Null otherwise
     */
    zptr zsrcall zcalloc(zsize_t _size) __asm__("_Z_calloc");
    /**
     * @brief Reallocate memory to a new pointer.
     * @warning Passed _ptr will be free
     * @param [free/in] _ptr (zptr) - Pointer to reallocate
     * @param [in] _size (zsize_t) - The new size to reallocate
     * @return The pointer reallocated to a new memory block
     */
    zptr zsrcall zrealloc(zptr _ptr, zsize_t _size) __asm__("_Z_realloc");
    /**
     * @brief Delete a pointer (free memory).
     * @param [in] _ptr (void*) - Pointer to release
     * @return True on success, False otherwise.
     */
    zbool zsrcall zfree(void* _ptr) __asm__("_Z_free");
    /**
     * @brief Get pointer size.
     * @param _ptr (const void*) - The pointer to get the size of
     * @return The pointer size
     */
    zsize_t zsrcall zallocs(const void* _ptr) __asm__("_Z_allocs");

    /**
     *
     * @param src
     * @param dest
     * @param _size
     */
    void zsrcall zmcopy(const void* src, void* dest, zsize_t _size) __asm__("_Z_mcopy");

    /**
     *
     * @param _ptr1
     * @param _ptr2
     */
    void zsrcall zmswap(zptr _ptr1, zptr _ptr2, zsize_t _size) __asm__("_Z_mswap");

    /**
     * @brief Exit the process
     * @param status (int) - Status' code of how the program ended
     */
    void zsrcall zexit(int status) __asm__("_Z_exit");

    /**
     *
     * @return
     */
    int zgetlasterror();

#ifdef __cplusplus
}
#endif // c++

#endif //ZSR_STD_H
