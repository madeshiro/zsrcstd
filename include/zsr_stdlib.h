#ifndef ZSR_STDLIB_H
#define ZSR_STDLIB_H

/**
 * ZaeSora Library -=- Standard C
 *
 * @file zsr_stdlib.h
 * @version 2021/01 0.4.0
 * @author Rin Baudelet
 *
 * ----- << OS/SPECIFICATION >> -----
 * > support OS(arch):
 *  - GNU/Linux distribution (64bit)    [kernel]
 *  - Unix Distribution (64bit)         [kernel]
 *  - Microsoft Windows 10 (64bit)      [WINAPI]
 *
 * All functions marked as `zsrcall` will only work on the os specified above.
 *
 * > Project made with CMake (@see CMakeLists.txt)
 * > C Standard : C11 (2011)
 * > ASM Standard : GNU as (default at&t syntax)
 *
 * ----- << Project structure >> -----
 * > All necessaries include files in `/include` folder
 * > All source files in `/src` folder
 *  - `.c` & `.S` (assembly files) available in `/src/{OS-ARCH}` folders according to OS dependency implementations
 *  - `.h` headers made according to OS/ARCH specifications available on `/src/{OS-ARCH}/headers`
 */

#if defined(ZSR_STDLIBRARY)
#   define ZSTD_DIR __declspec(dllexport)
#else
#   define ZSTD_DIR __declspec(dllimport)
#endif

#endif //ZSR_STDLIB_H
