//
// Created by chenzheng on 20/06/2017.
//

#include <string.h>
#include <limits.h>
#include "assert_v.h"
#include "str.h"
#include "fmt.h"
#include "mem.h"

/* macros */
#define idx(i, len) ((i) < 0 ? (i) + (len) : (i))
#define convert(s, i, j) do { int len; \
    assert(s); len = strlen(s); \
    i = idx(i, len); j = idx(j, len); \
    assert(i >= 0 && i <= j && j < len); } while (0)

char *Str_sub(const char *s, int i, int j)
{
    char *str, *p;

    convert(s, i, j);
    p = str = ALLOC(j - i + 1 + 1);
    while (i <= j)
        *p++ = s[i++];
    *p = '\0';

    return str;
}

char *Str_dup(const char *s, int i, int j, int n)
{
    int k;
    char *str, *p;

    assert(n >= 0);
    convert(s, i, j);
    if (n == 0)
        return s;
    p = str = ALLOC(n*(j - i + 1) + 1);
    while (n-- > 0) {
        for (k = i; k <= j; k++)
            *p++ = s[k];
    }
    *p = '\0';

    return str;
}

char *Str_reverse(const char *s, int i, int j)
{
    char *str, *p;

    convert(s, i, j);
    p = str = ALLOC(j - i + 1 + 1);
    while (j >= i)
        *p++ = s[j--];
    *p = '\0';

    return str;
}

char *Str_cat(const char *s1, int i1, int j1, const char *s2, int i2, int j2)
{
    char *str, *p;

    convert(s1, i1, j1);
    convert(s2, i2, j2);
    p = str = ALLOC((j1 - i1 + 1) + (j2 - i2 + 1 + 1));
    while (i1 <= j1)
        *p++ = s1[i1++];
    while (i2 <= j2)
        *p++ = s2[i2++];
    *p = '\0';

    return str;
}

/* Need to understand this implementation */
char *Str_catv(const char *s, ...)
{
    char *str, *p;
    const char *save = s;
    int i, j, len = 0;
    va_list ap;

    va_start(ap, s);
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        convert(s, i, j);
        len += j - i;
        s = va_arg(ap, const char *);
    }
    va_end(ap);
    p = str = ALLOC(len + 1);
    s = save;
    va_start(ap, s);
    while (s) {
        i = va_arg(ap, int);
        j = va_arg(ap, int);
        convert(s, i, j);
        while (i < j)
            *p++ = s[i++];
        s = va_arg(ap, const char *);
    }
    va_end(ap);
    *p = '\0';

    return str;
}

char *Str_map(const char *s, int i, int j, const char *from, const char *to)
{

}

int Str_pos(const char *s, int i)
{
    int len;

    assert(s);
    len = strlen(s);
    i = idx(i, len);
    assert(i >= 0 && i < len);

    return i;
}

int Str_len(const char *s, int i, int j)
{
    convert(s, i, j);

    return (j - i + 1);
}

int Str_cmp(const char *s1, int i1, int j1, const char *s2, int i2, int j2)
{
    convert(s1, i1, j1);
    convert(s2, i2, j2);
    s1 += i1;
    s2 += i2;

    if (j1 - i1 < j2 - i2) {
        int cond = strncmp(s1, s2, j1 - i1);
        return (cond == 0 ? -1 : cond);
    } else if (j1 - i1 > j2 - i2) {
        int cond = strncmp(s1, s2, j2 - i2);
        return (cond == 0 ? +1 : cond);
    } else
        return strncmp(s1, s2, j1 - i1);
}

int Str_chr(const char *s, int i, int j, int c)
{
    convert(s, i, j);
    for ( ; i <= j; i++) {
        if (s[i] == c)
            return i;
    }

    return 0;
}

int Str_rchr(const char *s, int i, int j, int c)
{
    convert(s, i, j);
    while (j >= i) {
        if (s[--j] == c)
            return j;
    }

    return 0;
}

int Str_upto(const char *s, int i, int j, const char *set)
{
    assert(set);
    convert(s, i, j);
    for ( ; i <= j; i++) {
        if (strchr(set, s[i]))
            return i;
    }

    return 0;
}

int Str_rupto(const char *s, int i, int j, const char *set)
{
    assert(set);
    convert(s, i, j);
    while (j >= i) {
        if (strchr(set, s[--j]))
            return j;
    }

    return 0;
}

int Str_find(const char *s, int i, int j, const char *set)
{
    int len;

    convert(s, i, j);
    assert(s);
    len = strlen(set);

    if (len == 0)
        return i;
    else if (len == 1) {
        for ( ; i <= j; i++)
            if (s[i] == *set)
                return i;
    } else {
        for ( ; i + len <= j; i++)
            if (strncmp(&s[i], set, len) == 0)
                return i;
    }

    return 0;
}



