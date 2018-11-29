#include "error.h"
#include <string.h>

static void err_doit(int, int, const char *, const char *, int);

/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void err_ret(const char *msg, const char* file, int line)
{
	err_doit(1, errno, msg, file, line);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *msg, const char* file, int line)
{
	err_doit(1, errno, msg, file, line);
	exit(EXIT_FAILURE);
}

/*
 * Nonfatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and return.
 */
void err_cont(int error, const char *msg, const char* file, int line)
{
	err_doit(1, error, msg, file, line);
}

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and terminate.
 */
void err_exit(int error, const char *msg, const char* file, int line)
{
	err_doit(1, error, msg, file, line);
	exit(EXIT_FAILURE);
}

/*
 * Fatal error related to a system call.
 * Print a message, dump code, and terminate.
 */
void err_dump(const char *msg, const char* file, int line)
{
	err_doit(1, errno, msg, file, line);
	abort(); /* dump core and terminate */
	exit(EXIT_FAILURE); /* shouldn't get here */
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void err_msg(const char *msg, const char* file, int line)
{
	err_doit(0, 0, msg, file, line);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void err_quit(const char *msg, const char* file, int line)
{
	err_doit(0, 0, msg, file, line);
	exit(EXIT_FAILURE);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *msg, const char* file, int line)
{
	char buf[MAXLINE];

	snprintf(buf, MAXLINE - 1, "%s", msg);
	if (errnoflag && error != 0) {
		snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s: %s: %d", strerror(error), file, line);
    } else {
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s: %d", file, line);
    }
	strcat(buf, "\n");
	fflush(stdout); /* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL); /* flushes all stdio output streams */ 
}
