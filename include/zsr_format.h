#ifndef ZSR_FORMAT_H
#define ZSR_FORMAT_H
#include "zsr_std.h"

#ifdef __cplusplus
extern "C" {
#endif // c++

    char* zformat(const char* _format, ...);
    char* zformatl(const char* _format, zva_list valist);
    wchar_t* zwformat(const wchar_t* _format, ...);
    wchar_t* zwformatl(const wchar_t* _format, zva_list valist);

#ifdef __cplusplus
};
#endif // c++


#endif // ZSR_FORMAT_H
