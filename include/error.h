#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

#define MAXLINE 256

extern void err_msg(const char *, const char *, int);
extern void err_dump(const char *, const char *, int) __attribute__((noreturn));
extern void err_quit(const char *, const char *, int) __attribute__((noreturn));
extern void err_cont(int, const char *, const char *, int);
extern void err_exit(int, const char *, const char *, int) __attribute__((noreturn));
extern void err_ret(const char *, const char *, int);
extern void err_sys(const char *, const char *, int) __attribute__((noreturn));

#endif
