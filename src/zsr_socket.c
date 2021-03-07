/** global `C` files
 * @file zsr_socket.c - implementation
 * @version 2020/12 0.4.0.0003
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_socket.h"

#define Z_IS_LE() \
    int __zreserved_n = 1; \
    if (*(char*)&__zreserved_n == 0x1)

short zhtons(short hs)
{
    Z_IS_LE()
    {
        return ((hs & 0xff00) >> 8) | ((hs & 0x00ff) << 8);
    }

    return hs;
}

short zntohs(short ns)
{
    Z_IS_LE()
    {
        return ((ns & 0xff00) >> 8) | ((ns & 0x00ff) << 8);
    }

    return ns;
}

zint32 zhtonl(zint32 hl)
{
    Z_IS_LE()
    {
        zint32 nl = hl & 0xff000000 >> 24;
            nl |= hl & 0x00ff0000 >> 8;
            nl |= hl & 0x0000ff00 << 8;
            nl |= hl & 0x000000ff << 24;
        return nl;
    }

    return hl;
}

zint32 zntohl(zint32 nl)
{
    Z_IS_LE()
    {
        zint32 hl = nl & 0xff000000 >> 24;
        hl |= nl & 0x00ff0000 >> 8;
        hl |= nl & 0x0000ff00 << 8;
        hl |= nl & 0x000000ff << 24;
        return hl;
    }

    return nl;
}
