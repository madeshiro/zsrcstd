#ifndef ZSR_STDSTRING_H
#define ZSR_STDSTRING_H
#include "zsr_std.h"
#include "zsr_format.h"

typedef char* zsr_string;
typedef wchar_t* zsr_wstring;

#ifdef __cplusplus
extern "C" {
#endif // c++

/*<< ZSR STRING (8bit `char`) >>*/

/**
 *
 * @param buf
 * @return
 */
zulong zstrlen(const char* buf);
char*  zstrcopy(const char* buf);
char*  zstrupper(const char* str);
char*  zstrlower(const char* str);
zbool  zstrequals(const char* str1, const char* str2);
zbool  zstrmatch(const char* str, const char* regex);
char*  zstrsubstring(const char* str, zuint beg, zuint end);
char** zstrcut(const char* str, const char* delimiter, zuint32* _Z_out pCount);
void zstrfreecut(char**, zuint32);

/*<< ZSR STRING (32bit `wchar_t`) >>*/

zulong zwstrlen(const wchar_t* buf);
wchar_t* zwstrcopy(const wchar_t* buf);
wchar_t* zwstrcast(const char* buf);
zbool zwstrequals(const wchar_t* str1, const wchar_t* str2);
zbool zwstrmatch(const wchar_t* str, const wchar_t* regex);

// @{ zsr_string functions }@ //

zsr_string zsrcall zString(const char* _str);
zbool zsrcall zDeleteString(zsr_string zstr);
zulong zsrcall zStrlength(const zsr_string zstr);

zsr_string zStrAppend(zsr_string *_this, const char* str);
zsr_string zStrPrepend(zsr_string *_this, const char* str);
zsr_string zStrConcat(zsr_string *_this, zuint _cnt, const char*, ...);
zsr_string zStrConcatf(zsr_string *_this, const char* _f, ...);

zsr_string zStrFormat(const zsr_string _this, ...);
zsr_string zStrReplace(const zsr_string _this, const char* target, const char* replcmt);
zsr_string zStrSubstring(const zsr_string _this, zuint beg, zuint end);
zsr_string zStrToLowercase(const zsr_string _this);
zsr_string zStrToUppercase(const zsr_string _this);

zbool zStrEquals(const zsr_string str1, const char* str2);
zbool zStrMatch(const zsr_string _this, const char* regex);
zbool zStrStartsWith(const zsr_string _this, const char* str);


#ifdef __cplusplus
};
#endif // c++

#endif // ZSR_STDSTRING_H
