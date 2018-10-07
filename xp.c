//
// Created by chenzheng on 19/06/2017.
//

#include <ctype.h>
#include <string.h>
#include "assert_v.h"
#include "xp.h"

#define T XP_T
#define BASE (1 << 8) /* base = 256 */

unsigned long XP_fromint(int n, T z, unsigned long u)
{
    int i = 0;

    do {
        z[i++] = u % BASE;
    } while ((u /= BASE) > 0 && i < n);
    for ( ; i < n; i++)
        z[i] = 0;
    return u;
}

unsigned long XP_toint(int n, T x)
{
    unsigned long u = 0;
    int i = (int)sizeof(u);

    if (i > n)
        i = n;
    while (--i >= 0)
        u = BASE*u + x[i];
    return u;
}

int XP_length(int n, T x)
{
    while (n > 1 && x[n-1] == 0)
        n--;
    return n;
}

int XP_add(int n, T z, T x, T y, int carry)
{
    int i;

    for (i = 0; i < n; i++) {
        carry += x[i] + y[i];
        z[i] = carry % BASE;
        carry /= BASE;
    }
    return carry;
}

int XP_sub(int n, T z, T x, T y, int borrow)
{
    int i;

    for (i = 0; i < n; i++) {
        int d = (x[i] + BASE) - borrow - y[i];
        z[i] = d % BASE;
        borrow = 1 - d/BASE;
    }
    return borrow;
}

int XP_sum(int n, T z, T x, int y)
{
    int i;

    for (i = 0; i < n; i++) {
        y += x[i];
        z[i] = y % BASE;
        y /= BASE;
    }
    return y;
}

int XP_diff(int n, T z, T x, int y)
{
    int i;

    for (i = 0; i < n; i++) {
        int d = (x[i] + BASE) - y;
        z[i] = d % BASE;
        y = 1 - d/BASE;
    }
    return y;
}

int XP_neg(int n, T z, T x, int carry)
{
    int i;

    for (i = 0; i < n; i++) {
        carry += (unsigned char)~x[i];
        z[i] = carry % BASE;
        carry /= BASE;
    }
    return carry;
}

int XP_mul(T z, int n, T x, int m, T y)
{
    int i, j, carryout = 0;

    for (i = 0; i < n; i++) {
        unsigned carry = 0;
        for (j = 0; j < m; j++) {
            carry += x[i] * y[j] + z[i + j];
            z[i + j] = carry % BASE;
            carry /= BASE;
        }
        for ( ; j < n + m - i; j++) {
            carry += z[i + j];
            z[i + j] = carry % BASE;
            carry /= BASE;
        }
        carryout |= carry;
    }
    return carryout;
}

int XP_product(int n, T z, T x, int y)
{
    int i;
    unsigned carry = 0;

    for (i = 0; i < n; i++) {
        carry += x[i] * y;
        z[i] = carry % BASE;
        carry /= BASE;
    }
    return carry;
}
