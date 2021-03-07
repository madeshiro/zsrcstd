#ifndef ZSR_SOCKET_H
#define ZSR_SOCKET_H
#include "zsr_std.h"

typedef void* zsr_socket;
typedef void* zsr_sockaddr;
#if defined(WIN32) || defined(WIN64)
#include "winsock2.h"
#include "windows.h"

#define Z_AF_UNSPEC AF_UNSPEC
#define Z_AF_INET AF_INET
#define Z_AF_IPX AF_IPX
#define Z_AF_APPLETALK AF_APPLETALK
#define Z_AF_NETBIOS AF_NETBIOS
#define Z_AF_INET6 AF_INET6
#define Z_AF_IRDA AF_IRDA
#define Z_AF_BTH AF_BTH

#define Z_SOCK_STREAM SOCK_STREAM
#define Z_SOCK_DGRAM SOCK_DGRAM
#define Z_SOCK_RAW SOCK_RAW
#define Z_SOCK_RDM SOCK_RDM
#define Z_SOCK_SEQPACKET SOCK_SEQPACKET

#define Z_IPPROTO_ICMP IPPROTO_ICMP
#define Z_IPPROTO_IGMP IPPROTO_IGMP
#define Z_BTHPROTO_RFCOMM 3
#define Z_IPPROTO_TCP IPPROTO_TCP
#define Z_IPPROTO_UDP IPPROTO_UDP
#define Z_IPPROTO_ICMPV6 IPPROTO_ICMPV6
#define Z_IPPROTO_RM 113

#elif defined(UNIX64)
#include <netinet/in.h>
#include <sys/socket.h>

#define Z_AF_UNSPEC AF_UNSPEC
#define Z_AF_INET AF_INET
#define Z_AF_IPX AF_IPX
#define Z_AF_APPLETALK AF_APPLETALK
#define Z_AF_NETBIOS AF_NETLINK
#define Z_AF_INET6 AF_INET6
#define Z_AF_IRDA AF_IRDA
#define Z_AF_BTH AF_BLUETOOTH
#define Z_AF_UNIX AF_UNIX

#define Z_SOCK_STREAM SOCK_STREAM
#define Z_SOCK_DGRAM SOCK_DGRAM
#define Z_SOCK_RAW SOCK_RAW
#define Z_SOCK_RDM SOCK_RDM
#define Z_SOCK_SEQPACKET SOCK_SEQPACKET

#define Z_IPPROTO_ICMP IPPROTO_ICMP
#define Z_IPPROTO_IGMP IPPROTO_IGMP
#define Z_IPPROTO_TCP IPPROTO_TCP
#define Z_IPPROTO_UDP IPPROTO_UDP
#define Z_IPPROTO_ICMPV6 IPPROTO_ICMPV6
#endif

#define Z_SOMAXCONN 0x7fffffff
#define Z_MAXPENDING Z_SOMAXCONN
#define ZSD_RECV 0
#define ZSD_SEND 1
#define ZSD_BOTH 2

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
    zsrcall zbool zWSAInit(WSADATA * _Z_out pWsaData);
    zsrcall zint zWSACleanup();
#endif

struct zin_addr {
    union {
        struct { zuint8 s_b1, s_b2, s_b3, s_b4; } S_un_b;
        struct { zushort s_w1, s_w2; } S_un_w;
        zulong S_addr;
    } S_un;
};

#define z_s_addr	S_un.S_addr
#define z_s_host	S_un.S_un_b.s_b2
#define z_s_net	    S_un.S_un_b.s_b1
#define z_s_imp	    S_un.S_un_w.s_w2
#define z_s_impno	S_un.S_un_b.s_b4
#define z_s_lh	    S_un.S_un_b.s_b3

struct zsockaddr {
    zushort sa_family;
    char    sa_data[14];
};

struct zsockaddr_in {
    zshort  sin_family;
    zushort sin_port;
    struct zin_addr sin_addr;
    char    sin_zero[8];
};

zsr_socket zsrcall zsocket(int af, int type, int protocol);
zbool zsrcall zbind(zsr_socket sock, const char* inetaddr, short port);
zbool zsrcall zbindhost(zsr_socket sock, const char* hostaddr, short port);
zbool zsrcall zlisten(zsr_socket sock, int queueLimit);
zsr_socket zsrcall zaccept(zsr_socket sock, struct zsockaddr* clientAddr, int *addrLen);
zbool zsrcall zconnect(zsr_socket sock, const char* inetaddr, short port);
zbool zsrcall zconnecthost(zsr_socket sock, const char* hostaddr, short port);

int zsrcall zsend(zsr_socket sock, const char* buf, int len);
int zsrcall zrecv(zsr_socket sock, char* buf, int len);

zbool zsrcall zshutdown(zsr_socket sock, int action);

zbool zsrcall zclosesock(zsr_socket);

zint zsrcall zsockaf(zsr_socket);
zint zsrcall zsocktype(zsr_socket);
zint zsrcall zsockprotocol(zsr_socket);

short zhtons(short hs);
short zntohs(short ns);
zint32 zhtonl(zint32 hl);
zint32 zntohl(zint32 hs);

#ifdef __cplusplus
};
#endif

#endif // ZSR_SOCKET_H
