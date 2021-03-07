#ifndef ZSR_FILE_H
#define ZSR_FILE_H
#include "zsr_stdio.h"

typedef struct _z_file* zsr_file;
typedef const char* zfmode_t;
#ifdef ZSR_SYS_UNIX64
    typedef int zfd_t;
#elif defined(ZSR_SYS_WIN64)
    typedef void* zfd_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif // c++

zsrcall struct _z_file* _Z_os_filedesc(int fd);

#define zstdin  _Z_os_filedesc(0)
#define zstdout _Z_os_filedesc(1)
#define zstderr _Z_os_filedesc(2)

    zbool zmkdir(const char* dirname);
#ifdef WIN32
    zbool zwmkdir(const wchar_t* dirname);
#endif
    zbool zrmdir(const char* dirname);

    /**
     *
     * @param fname (const char*) - file's path (relative or absolute)
     * @param mode (zfmode_t) - mode to use to open the file <br/>
     * -> <b>"r"</b> = open for reading; If the file does not exist, return NULL <br/>
     * -> <b>"rb"</b> = Open for reading in binary mode; If the file does not exist,
     * return NULL <br/>
     * -> <b>"w"</b> = Open for writing; If the file exists, contents are overwritten,
     * otherwise, a file will be created <br/>
     * -> <b>"wb"</b> = Open for writing in binary mode; If the file exists,
     * contents are overwritten, otherwise, a file will be created <br/>
     * -> <b>"a"</b> = Open for append; If the file does not exist, it will be created <br/>
     * -> <b>"ab"</b> = Open for append in binary mode; If the file does not exist, it will
     * be created <br/>
     * -> <b>"r+"</b> = Open for both reading and writing; If the file does not exist,
     * return NULL <br/>
     * -> <b>"rb+"</b> = Open for both reading and writing in binary mode; If the file does
     * not exist, return NULL <br/>
     * -> <b>"w+"</b> = Open for both reading and writing; If the file exists, contents are
     * overwritten, otherwise, it will be created <br/>
     * -> <b>"wb+"</b> = Open for both reading and writing in binary mode; If the file
     * exists, contents are overwritten, otherwise, it will be created <br/>
     * -> <b>"a+"</b> = Open for both reading and appending; If the file does not exist,
     * it will be created <br/>
     * -> <b>"ab+"</b> = Open for both reading and appending in binary mode; If the file
     * does not exist, it will be created
     *
     * @return the file's handler (zsr_file)
     */
    zsr_file zfopen(const char* fname, zfmode_t mode);
    zsr_file zfwopen(const wchar_t* fname, zfmode_t mode);
    zbool zfclose(zsr_file file);
    zsrcall zbool zfremove(const char* fname);
    zsrcall zbool zfrename(const char* oldname, const char* newname);

    // FILE INPUT //
    zsrcall char zfgetch(zsr_file file);
    zsrcall zlong zfread(char* _Z_out buf, zulong len, zsr_file file);
    zsrcall zlong zfreadln(char** _Z_out buf, zsr_file file);

    zsrcall wchar_t zwfgetch(zsr_file file);
    zsrcall zlong zwfread(wchar_t* _Z_out buf, zulong len, zsr_file file);
    zsrcall zlong zwfreadln(wchar_t** _Z_out buf, zsr_file file);

    // FILE OUTPUT //
    zsrcall zlong zfput(char c, zsr_file file);
    zsrcall zlong zfputs(const char* str, zsr_file file);
    zsrcall zlong zfprintf(const char* _format, zsr_file file, ...);
    zsrcall zlong zfprintln(const char* str, zsr_file file);
    // TODO zfscanf (windows&unix)
    // zsrcall zlong zfscanf(const char* _format, zsr_file file, ...);
    zsrcall zlong zfwrite(const char* buf, zulong len, zsr_file file);
    zsrcall zlong zfwriteln(const char* buf, zulong len, zsr_file file);

    zsrcall zlong zwfput(wchar_t c, zsr_file file);
    zsrcall zlong zwfputs(const wchar_t* str, zsr_file file);
    zsrcall zlong zwfprintf(const wchar_t* _format, zsr_file file, ...);
    zsrcall zlong zwfprintln(const wchar_t* _format, zsr_file file);
    // zsrcall zlong zwfscanf(const wchar_t* _format, zsr_file file, ...);
    zsrcall zlong zwfwrite(const wchar_t* buf, zulong len, zsr_file file);
    zsrcall zlong zwfwriteln(const wchar_t* buf, zulong len, zsr_file file);

    //*> FILE MANAGEMENT <*//

    /**
     *
     * @param file
     * @param offset
     * @param whence
     * @return
     */
    zsrcall zlong zfseek(zsr_file file, long offset, int whence);
    /**
     *
     * @param file
     * @return
     */
    zsrcall zlong zftell(zsr_file file);

    zfd_t zfgetfd(zsr_file file);
    const void* zfgetname(zsr_file file);

    //> OS SPECIFICITY <//
#if defined(ZSR_SYS_WIN64) || defined(ZSR_SYS_WIN32)

#elif defined(ZSR_SYS_UNIX64)

#endif // ZSR_SYS_%os%


#ifdef __cplusplus
};
#endif
#endif // ZSR_FILE_H