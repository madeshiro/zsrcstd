/** global `C` files
 * @file zsr_math.c - implementation
 * @version 2020/11 0.3.0
 * @author Rin Baudelet (madeshiro)
 */
#include "zsr_math.h"

double zpow(double d, double p)
{
    return __builtin_pow(d,p);
}

double zabs(double v)
{
    return v < 0 ? -v : v;
}

