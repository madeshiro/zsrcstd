#ifndef ZSR_PIPE_H
#define ZSR_PIPE_H
#include "zsr_stdio.h"

/**
 * Handler program in ASM
 * > align(8)
 */
typedef void* zsr_pipe, zsr_npipe;

#ifdef __cplusplus
extern "C" {
#endif // c++

    /**
     * @brief Create anonymous pipe
     * @return a handler to the pipe
     */
    zsr_pipe zCreatePipe();
    /**
     * @brief Create named pipe
     * @param name (char*) - the pipe's name.
     * Should not contains espace or special chars
     * @return a handler to the pipe
     */
    zsr_npipe zCreateFifo(const char* name);
    /**
     * @brief Close pipe or fifo (named pipe)
     * @return True if pipe has been successfuly closed, false otherwise
     */
    zbool zClosePipe(zsr_pipe);

#ifdef __cplusplus
};
#endif // c++

#endif // ZSR_PIPE_H
