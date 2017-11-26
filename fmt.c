//
// Created by chenzheng on 20/06/2017.
//

#include "fmt.h"
#include "assert_v.h"
#include "except_v.h"
#include "mem.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#define T Fmt_T

struct buf
{
    char *buf;
    char *bp;
    int size;
};

#define pad(n, c) do { int nn = (n); \
    while (nn-- > 0) \
        put((c), cl); } while (0)

const Except_T Fmt_Overflow = { "Formatting Overflow" };

void Fmt_fmt(int put(int c, void *cl), void *cl, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    Fmt_vfmt(put, cl, fmt, ap);
    va_end(ap);
}

void Fmt_print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    Fmt_vfmt(outc, stdout, fmt, ap);
    va_end(ap);
}

void Fmt_fprint(FILE *stream, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    Fmt_vfmt(outc, stream, fmt, ap);
    va_end(ap);
}

int Fmt_sfmt(char *buf, int size, const char *fmt, ...)
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = Fmt_vsfmt(buf, size, fmt, ap);
    va_end(ap);

    return len;
}

int Fmt_vsfmt(char *buf, int size, const char *fmt, va_list ap) {
    struct buf cl;

    assert(buf);
    assert(size > 0);
    assert(fmt);
    cl.buf = cl.bp = buf;
    cl.size = size;
    Fmt_vfmt(insert, &cl, fmt, ap);
    insert(0, &cl);

    return (cl.bp - cl.buf - 1);
}

static int insert(int c, void *cl) {
    struct buf *p = cl;

    if (p->bp >= p->buf + p->size)
        RAISE(Fmt_Overflow);
    *p->bp++ = c;

    return c;
}

static int outc(int c, void *cl)
{
    FILE *f = cl;

    return putc(c, f);
}

static void cvt_s(int code, va_list *app, int put(int c, void *cl), void *cl, unsigned char flags[], int width, int precision)
{
    char *str = va_arg(*app, char *);

    assert(str);
    Fmt_puts(str, strlen(str), put, cl, flags, width, precision);
}

void Fmt_puts(const char *str, int len, int put(int c, void *cl), void *cl, unsigned char flags[], int width, int precision)
{
    assert(str);
    assert(len >= 0);
    assert(flags);
    /* normalize width and flags */
    if (width == INT_MIN)
        width = 0;
    if (width < 0) {
        flags['-'] = 1;
        width = -width;
    }
    if (precision >= 0)
        flags['0'] = 0;

    if (precision >= 0 && precision < len)
        len = precision;
    if (!flags['-'])
        pad(width - len, ' ');
    {
        int i;
        for (i = 0; i < len; i++)
            put((unsigned char)*str++, cl);
    }
    if (flags['-'])
        pad(width - len, ' ');
}
