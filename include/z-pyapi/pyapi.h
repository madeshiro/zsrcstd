#ifndef Z_PYAPI_PYAPI_H
#define Z_PYAPI_PYAPI_H
#include "zsr_std.h"
#include "pyfile.h"

typedef zbool           pybool;
typedef char            pychar;
typedef wchar_t         pywchar;
typedef zshort          pyshort;
typedef zushort         pyushort;
typedef zint            pyint;
typedef zuint           pyuint;
typedef zlong           pylong;
typedef zulong          pyulong;
typedef zllong          pyllong;
typedef zullong         pyullong;
typedef zsize_t         pysize_t;
typedef zfloat          pyfloat;
typedef zdouble         pydouble;
typedef zldouble        pyldouble;
typedef char*           pybytes;
typedef wchar_t*        pystring;

/**
 * @brief zsr_pycall (ZSR PYTHON CALLABLE FUNCTION)
 * Simple macro defined as keyword to mention that a function is usable as python extension function
 */
#define zsr_pycall

#ifdef __cplusplus
extern "C" {
#endif // c++

    /**
     *
     * @return
     */
    pybool zsr_pycall zpy_initlib();
    /**
     *
     * @return
     */
    pybool zsr_pycall zpy_unloadlib();
    /**
     *
     * @return
     */
    pystring zpy_pystr(const char*);
    /**
     *
     * @param addr
     * @return
     */
    zptr zpy_getptr(pysize_t addr);

#ifdef __cplusplus
};
#endif // c++
#endif // Z_PYAPI_PYAPI_H
