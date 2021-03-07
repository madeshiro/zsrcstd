#ifndef _Z_Stdio_S
#define _Z_Stdio_S
#include "zsr_std.h"

/*<<< define ASM HEADER >>>*/

// Console I/O //

int zsrcall _Z_winapi_cwrite(const char* __buf, long __len);
int zsrcall _Z_winapi_cread(char* __buf, unsigned long __read);
int zsrcall _Z_winapi_Wcwrite(const wchar_t* __buf, long __len);
int zsrcall _Z_winapi_Wcread(wchar_t * __buf, unsigned long __read);

// STD (generic) I/O //

struct _z_file
{
    void* name;
    void* fd;
#ifdef WIN32
    zbool wname;
#endif
};

void* zsrcall _Z_winapi_open(const char* __fname, zflag __openmode, zbool __create);
zbool zsrcall _Z_winapi_deletefile(const char* __fname);
zbool zsrcall _Z_winapi_close(void* fd);
long zsrcall _Z_winapi_read(void* fd, char* __buf, unsigned long __len);
long zsrcall _Z_winapi_write(void* fd, const char* __buf, unsigned long __len);

void* zsrcall _Z_winapi_Wopen(const wchar_t* __fname, zflag __openmode, zbool __create);
int zsrcall _Z_winapi_Wread(void* fd, wchar_t* __buf, unsigned long __len);
int zsrcall _Z_winapi_Wwrite(void* fd, const wchar_t* __buf, unsigned long __len);

long zsrcall _Z_winapi_setfileptr(void* fd, long offset, int whence);

zbool zsrcall _Z_winapi_mkdir(const char* dirname);
zbool zsrcall _Z_winapi_wmkdir(const wchar_t* dirname);

#endif // _Z_Stdio_S