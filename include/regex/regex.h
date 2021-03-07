#ifndef Z_REGEXLIB_REGEX_H
#define Z_REGEXLIB_REGEX_H
#include "zsr_std.h"
#include "zsr_locale.h"

#ifdef __cplusplus
extern "C" {
#endif // c++

zbool z_ascii_match(const char* str, const char* regex);
zbool z_unicode_match(const utf32_t* str, const utf32_t* regex);
zbool z_posix_match(const char* str, const char* regex);

#ifdef __cplusplus
};
#endif // c++

#endif // Z_REGEXLIB_REGEX_H
