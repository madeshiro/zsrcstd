/** Unix x86_64 `C` files
 * @file socket.c - implementation
 * @version 2021/02 0.4.0.0003
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_socket.h"
#include "headers/socket_S.h"
#include <arpa/inet.h>
#include <netdb.h>

_P_spec_sockaddr_in _Z_build_sockaddr_in(zsr_socket sock, const char* inetaddr, short port)
{
    _P_spec_sockaddr_in sockaddrIn = zalloc(sizeof(_spec_sockaddr_in));
    sockaddrIn->sin_family = zsockaf(sock);
    sockaddrIn->sin_addr.s_addr = inet_addr(inetaddr);
    sockaddrIn->sin_port = zhtons(port);
    return sockaddrIn;
}

_P_spec_sockaddr_in _Z_build_sockaddr_in2(zsr_socket sock, const char* hostaddr, short port)
{
    if (zsockaf(sock) != AF_INET)
    {
        return 0;
    }

    struct addrinfo* result, *res;
    _P_spec_sockaddr_in sockaddrIn = znull;
    getaddrinfo(hostaddr, znull, znull, &result);
    for (res = result; res != znull; res = res->ai_next)
    {
        if (res->ai_family == zsockaf(sock) &&
            res->ai_socktype == zsocktype(sock) &&
            res->ai_protocol == zsockprotocol(sock))
        {
            sockaddrIn = (_P_spec_sockaddr_in ) res->ai_addr;
            sockaddrIn->sin_port = htons(port);
            break;
        }
    }
    freeaddrinfo(result);
    return sockaddrIn;
}

int _Z_sizeof_sockaddr_in()
{
    return sizeof(_spec_sockaddr_in);
}