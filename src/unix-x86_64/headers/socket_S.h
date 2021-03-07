#ifndef _Z_Socket_S
#define _Z_Socket_S
#include "zsr_socket.h"
#ifdef UNIX64
#include <sys/socket.h>
typedef struct sockaddr     _spec_sockaddr;
typedef struct sockaddr_in  _spec_sockaddr_in;
typedef struct sockaddr*    _P_spec_sockaddr;
typedef struct sockaddr_in* _P_spec_sockaddr_in;
#endif

_P_spec_sockaddr_in zsrcall _Z_build_sockaddr_in
        (zsr_socket sock, const char* inetaddr, short port);
_P_spec_sockaddr_in zsrcall _Z_build_sockaddr_in2
        (zsr_socket sock, const char* hostaddr, short port);

#endif // _Z_Socket_S