/*
	mini_shell.c
	Reads commands from standard input and executes them
*/

#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 128

// Prints a message and returns to calle
// caller specifies "errnoflag"
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char		buf[MAXLINE];

	vsnprintf(buf, MAXLINE-1, fmt, ap);
	if(errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
			strerror(error));
	strcat(buf, "\n");
	fflush(stdout);					//in case stdout and stderr are the same
	fputs(buf, stderr);
	fflush(NULL);					// flushes all stdio output streams
}


// Nonfatal error related to system call
// prints a message and returns
void err_ret(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

// Fatal error related to system call
// prints a message and terminates
void err_sys(const char *fmt, ...)
{
	va_list		ap;
	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}


int main(void)
{
	char		buf[MAXLINE];
	pid_t		pid;
	int			status;

	printf("%% ");
	while( fgets(buf,MAXLINE, stdin) != NULL)
	{
		if (buf[strlen(buf) -1] == '\n' )
			buf[strlen(buf) -1] = 0;		// replace newline

		if ((pid = fork()) < 0) {			// child
			exit(0);
		} else if (pid == 0) {
			execlp(buf, buf, (char*) 0);
			err_ret("couldnt execute :%s", buf);
			exit(127);
		}

		// parent
		if ((pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");
		printf("%% ");
	}
	exit(0);

}
