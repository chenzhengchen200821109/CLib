//
// Created by chenzheng on 28/09/2017.
//

#include "arith.h"

int Arith_max(int x, int y)
{
    return x > y ? x : y;
}

int Arith_min(int x, int y)
{
    return x > y ? y : x;
}

int Arith_div(int x, int y)
{
    /* x and y have different signs and division truncates 0 */
    if ((-13 / 5 == -2) && (x < 0) != (y < 0) && x%y != 0)
        return x/y -1;
    else
        return x/y;
}

int Arith_mod(int x, int y)
{
    return x - y*Arith_div(x, y);
}

int Arith_floor(int x, int y)
{
    return Arith_div(x, y);
}

int Arith_ceilling(int x, int y)
{
    return Arith_div(x, y) + (x%y != 0);
}
