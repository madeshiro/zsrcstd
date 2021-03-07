#ifndef ZSR_PIPE_H
#define ZSR_PIPE_H
#include "zsr_stdio.h"

typedef void* zsr_pipe, zsr_npipe;

#ifdef __cplusplus
extern "C" {
#endif // c++

    zsr_pipe zCreatePipe();
    zsr_npipe zCreateFifo();
    zbool zClosePipe(zsr_pipe); 

#ifdef __cplusplus
};
#endif // c++

#endif // ZSR_PIPE_H
