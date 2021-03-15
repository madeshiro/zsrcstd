# Define CMake project's properties
cmake_minimum_required(VERSION 3.17)
project(ZSRStdLibrary C ASM)
set(CMAKE_C_STANDARD 11)
enable_language(ASM)

message("--- OS/SYSTEM CONFIGURATION --- \n> PROCESSOR-TYPE: " ${CMAKE_SYSTEM_PROCESSOR}
        "\n> SYSTEM-NAME: " ${CMAKE_SYSTEM_NAME})
if((${ARCH} EQUAL 64) OR ((NOT DEFINED ARCH) AND ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "(64)+")))
    set(ZSR_SYS_ARCH 64)
else()
    set(ZSR_SYS_ARCH 32)
endif()
message("> (build) SYSTEM-ARCH: " ${ZSR_SYS_ARCH} "bit")


# Set project's properties
include_directories("include")
include_directories(".")
add_definitions(
        -DZSRSTD
        -DZSRLIB_${ZSR_SYS_ARCH}
        -DZSR_STDLIBRARY
)

### > Source, Include & Assembly's files < ###

# ZSR's Python API SOURCES
set(pyapi_src
        src/pyapi/pyapi.c
        )

# ZSR's Python API HEADERS
set(pyapi_headers
        pyapi/pyapi.h
        pyapi/pyfile.h
        )

# ZSR's regex lib SOURCES
set(regexlib_src
        src/regex/regex.c
        src/regex/regex_cond.c
        )

# ZSR's regex lib HEADERS
set(regexlib_headers
        include/regex/regex.h
        include/regex/regexlib.h
        )

# GLOBAL SOURCES
set(SOURCES
        ${pyapi_src}
        ${regexlib_src}
        src/zsr_string.c
        src/zsr_locale.c
        src/zsr_type.c
        src/zsr_math.c
        src/zsr_format.c
        src/zsr_wformat.c
        src/zsr_file.c
        src/zsr_socket.c
        src/zsr_std.c
        )

# GLOBAL HEADERS
set(HEADERS
        ${pyapi_headers}
        ${regexlib_headers}
        include/zsr_stdlib.h
        include/zsr_std.h
        include/zsr_container.h
        include/zsr_stdio.h
        include/zsr_string.h
        include/zsr_locale.h
        include/zsr_file.h
        include/zsr_pipe.h
        include/zsr_socket.h
        include/zsr_thread.h
        include/zsr_format.h
        include/zsr_type.h
        include/zsr_math.h
        src/zsr_prvt.h
        )

# GLOBAL ASSEMBLY FILES
set(ASM
        include/zsr_asm_const.S
        )

# ZSR's syscall static-lib SOURCES
set(SYSSOURCES
        syscall/zsyscall.c
        )

# ZSR's syscall static-lib HEADERS
set(SYSHEADERS
        syscall/zsyscall.h
        )

# ZSR's syscall static-lib ASSEMBLY FILES
set(SYSASM
        syscall/z64sys.S
        syscall/zsyscall64.S
        )

message("\n--- PROJECTS PROPERTIES ---")

set(ZSR_LIB_TARGET zsrcstd)
set(ZSR_SYSLIB_TARGET zsrsyscall)

### { OS DEFINITIONS } ###
if (WIN32)
    # RC FILE
    set(RC_FILE resources.rc)
    # DEFINITIONS
    message("> Enable GAS/ASM language ${ZSR_SYS_ARCH}bit")
    message("> MinGW ${ZSR_SYS_ARCH}bit configuration enabled")
    add_definitions(
            -DZSR_SYS_WIN${ZSR_SYS_ARCH}
    )

    if (${ZSR_SYS_ARCH} EQUAL 64)

        set(win_amd64_asm
                src/win-amd64/std.S
                src/win-amd64/stdio.S
                src/win-amd64/list.S
                src/win-amd64/chainlist.S
                src/win-amd64/string.S
                src/win-amd64/thread.s
                src/win-amd64/mutex.S
                src/win-amd64/socket.S
        )

        set(win_amd64_src
                src/win-amd64/std.c
                src/win-amd64/stdio.c
                src/win-amd64/thread.c
                src/win-amd64/socket.c
                src/win-amd64/pipe.c)

        set(win_amd64_headers
                src/win-amd64/headers/stdio_S.h
                src/win-amd64/headers/thread_S.h
                src/win-amd64/headers/std_S.h
                src/win-amd64/headers/socket_S.h
                src/win-amd64/headers/pipe_S.h)

        set(ASM ${ASM} ${win_amd64_asm})
        set(SOURCES ${SOURCES} ${win_amd64_src})
        set(HEADERS ${HEADERS} ${win_amd64_headers})
    else()
        message(FATAL_ERROR "Windows 32bit not supported yet !")
    endif()
elseif (UNIX)
    # DEFINITIONS
    message("> Enable GAS/ASM language ${ZSR_SYS_ARCH}bit")
    message("> MinGW ${ZSR_SYS_ARCH}bit configuration enabled")
    add_definitions(
            -DZSR_SYS_UNIX${ZSR_SYS_ARCH}
            -DUNIX64
    )

    if (${ZSR_SYS_ARCH} EQUAL 64)
        set(unix_x64_asm
                src/unix-x86_64/std.S
                src/unix-x86_64/stdio.S
                src/unix-x86_64/list.S
                src/unix-x86_64/chainlist.S
                src/unix-x86_64/futex.S
                src/unix-x86_64/thread.S
                src/unix-x86_64/string.S
                src/unix-x86_64/socket.S
                src/unix-x86_64/pipe.S
                )

        set(unix_x64_src
                src/unix-x86_64/stdio.c
                src/unix-x86_64/std.c
                src/unix-x86_64/thread.c
                src/unix-x86_64/socket.c
                src/unix-x86_64/pipe.c
                )

        set(unix_x64_headers
                src/unix-x86_64/headers/stdio_S.h
                src/unix-x86_64/headers/thread_S.h
                src/unix-x86_64/headers/pipe_S.h
                src/unix-x86_64/headers/std_S.h
                src/unix-x86_64/headers/socket_S.h)

        set(ASM ${ASM} ${unix_x64_asm})
        set(SOURCES ${SOURCES} ${unix_x64_src})
        set(HEADERS ${HEADERS} ${unix_x64_headers})
    else()
        message(FATAL_ERROR "Unix 32bit not supported yet !")
    endif()
endif(WIN32)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message("> Debug mode enabled")
    add_definitions(-DZSR_LIB_DEBUG)
    add_definitions(-DRCF_DEBUG)
else()
    add_definitions(-DZSR_LIB_RELEASE)
endif()

add_link_options(-no-pie)

add_library(${ZSR_LIB_TARGET} SHARED ${RC_FILE} ${SOURCES} ${ASM} ${HEADERS})
add_library(${ZSR_SYSLIB_TARGET} STATIC ${SYSSOURCES} ${SYSASM} ${SYSHEADERS})

target_link_libraries(${ZSR_SYSLIB_TARGET} -lzsrcstd)
if (UNIX)
    target_link_libraries(${ZSR_LIB_TARGET} m)
elseif(WIN32)
    target_link_libraries(${ZSR_LIB_TARGET} -lWs2_32)
endif()