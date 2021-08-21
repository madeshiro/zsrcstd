#ifndef ZSR_FILE_H
#define ZSR_FILE_H
#include "zsr_stdio.h"
#include "zsr_chencoding.h"

typedef zhandler zsr_file, zfile;
typedef const char* zfmode_t;

#ifdef __cplusplus
extern "C" {
#endif // c++

    zsr_file zstdfile(int fd);

#ifndef ZSR_DEFINED_STDFILENO
#   define zstdin  zstdfile(ZSR_IO_STDIN)
#   define zstdout zstdfile(ZSR_IO_STDOUT)
#   define zstderr zstdfile(ZSR_IO_STDERR)
#endif // ZSR_DEFINED_STDFILENO

    zbool zmkdir(const char* dirname) _Z_xapieq("mkdir");
    zbool zrmdir(const char* dirname) _Z_xapieq("rmdir");

    /**
     *
     * @param fname (const char*) - file's path (relative or absolute).
     * @param mode (zfmode_t) - mode to use to open the file <br/>
     * <ul>
     * <li><b>"r"</b> = open for reading; If the file does not exist, return NULL</li>
     * <li><b>"rb"</b> = Open for reading in binary mode; If the file does not exist,
     * return NULL</li>
     * <li><b>"w"</b> = Open for writing; If the file exists, contents are overwritten,
     * otherwise, a file will be created</li>
     * <li><b>"wb"</b> = Open for writing in binary mode; If the file exists,
     * contents are overwritten, otherwise, a file will be created</li>
     * <li><b>"a"</b> = Open for append; If the file does not exist, it will be created</li>
     * <li><b>"ab"</b> = Open for append in binary mode; If the file does not exist, it will
     * be created</li>
     * <li><b>"r+"</b> = Open for both reading and writing; If the file does not exist,
     * return NULL</li>
     * <li><b>"rb+"</b> = Open for both reading and writing in binary mode; If the file does
     * not exist, return NULL</li>
     * <li><b>"w+"</b> = Open for both reading and writing; If the file exists, contents are
     * overwritten, otherwise, it will be created</li>
     * <li><b>"wb+"</b> = Open for both reading and writing in binary mode; If the file
     * exists, contents are overwritten, otherwise, it will be created</li>
     * <li><b>"a+"</b> = Open for both reading and appending; If the file does not exist,
     * it will be created</li>
     * <li><b>"ab+"</b> = Open for both reading and appending in binary mode; If the file
     * does not exist, it will be created</li>
     * </ul>
     *
     * @return the file's handler (zsr_file)
     */
     zsr_file zfopen(const char* fname, zfmode_t mode);
     /**
      *
      * @param file
      * @return
      */
     zsrcall zbool zfclose(zsr_file file)                              _Z_xapieq("fclose");
     /**
      *
      * @param fname
      * @return
      */
     zsrcall zbool zfremove(const char* fname)                         _Z_xapieq("fremove");
     zsrcall zbool zfrename(const char* oldname, const char* newname)  _Z_xapieq("frename");

    // FILE INPUT //
     char zfgetch(zsr_file file);
     zlong zfread(char* _Z_out buf, zulong len, zsr_file file)  _Z_xapieq("fread");
     zlong zfreadln(char** _Z_out buf, zsr_file file);

     wchar_t zwfgetch(zsr_file file);
     zlong zwfread(wchar_t* _Z_out buf, zulong len, zsr_file file);
     zlong zwfreadln(wchar_t** _Z_out buf, zsr_file file);

    // FILE OUTPUT //
     zlong zfput(char c, zsr_file file);
     zlong zfputs(const char* str, zsr_file file);
     zlong zfprintf(const char* _format, zsr_file file, ...);
     zlong zfprintln(const char* str, zsr_file file);
     zlong zfscanf(const char* _format, zsr_file file, ...);
     zlong zfwrite(const char* buf, zulong len, zsr_file file);
     zlong zfwriteln(const char* buf, zulong len, zsr_file file);

     zlong zwfput(wchar_t c, zsr_file file);
     zlong zwfputs(const wchar_t* str, zsr_file file);
     zlong zwfprintf(const wchar_t* _format, zsr_file file, ...);
     zlong zwfprintln(const wchar_t* _format, zsr_file file);
     zlong zwfscanf(const wchar_t* _format, zsr_file file, ...);
     zlong zwfwrite(const wchar_t* buf, zulong len, zsr_file file);
     zlong zwfwriteln(const wchar_t* buf, zulong len, zsr_file file);

    //*> FILE MANAGEMENT <*//

    void zfccnl(short cc, zsr_file file);
    short zfgetccnl(zsr_file file);
    /**
     * Looks at the entire file to analyze which charsequence is used to indicate a new line.<br/>
     * The function will put back the cursor at its initial position.
     * @warning Make sure the file is open with read access, otherwise, the function will returned -1 (EOF)
     * @param file (zsr_file) - The file to look at
     * @return The return line's control char if succeed, EOF otherwise.
     */
    enum zsr_special_char zflookccnl(zsr_file file);

    /**
     *
     * @param file
     * @param offset
     * @param whence
     * @return
     */
     zlong zfseek(zsr_file file, long offset, int whence);
    /**
     *
     * @param file
     * @return
     */
     zlong zftell(zsr_file file);

    zfd_t zfgetfd(zsr_file file);
    int   zfileno(zsr_file file);
    const char* zfgetname(zsr_file file);
    enum zch_encoding zfgetchencode(zsr_file file);
    void zfsetchencode(zsr_file file);

    const char* zflistfiles(const char* dir, int* _Z_out c);
    const char* zflistdir(const char* dir, int* _Z_out c);
    const char* zwflistfiles(const wchar_t* dir, int* _Z_out c);
    const char* zwflistdir(const wchar_t * dir, int* _Z_out c);

#ifdef __cplusplus
};
#endif
#endif // ZSR_FILE_H