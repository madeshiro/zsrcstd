/** pyapi `C` files
 * @file pyapi.c - implementation
 * @version 2020/11 0.3.0.0002
 * @author Rin Baudelet (madeshiro)
 */
#include "z-pyapi/pyapi.h"
#include "zsr_stdio.h"
#include "zsr_string.h"

pybool zsr_pycall zpy_initlib()
{
    zprintln("Hello World !");
    return ztrue;
}

pybool zsr_pycall zpy_unloadlib()
{
    return ztrue;
}

pystring zpy_pystr(const char* str)
{
    zulong strlen = zstrlen(str), i;
    pystring pystr = zalloc(sizeof(pywchar)*(strlen+1));
    pystr[strlen] = L'\0';

    for (i=0; i < strlen; i++)
    {
        pystr[i] = (pywchar) str[i];
    }

    return pystr;
}

zptr zpy_getptr(pysize_t addr)
{
    return (zptr) addr;
}
