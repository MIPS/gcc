/* Utilities to execute a program in a subprocess (possibly linked by pipes
   with other subprocesses), and wait for it.  Generic Unix version
   (also used for UWIN and VMS).
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2003
   Free Software Foundation, Inc.

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "pex-common.h"

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#ifdef NEED_DECLARATION_ERRNO
extern int errno;
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifndef HAVE_WAITPID
#define waitpid(pid, status, flags) wait(status)
#endif

/* These are needed to prevent the build from bombing out on
   platforms (such as embedded targets) where no pex-*.c
   will work at all (but we try to build this one anyway).  */
extern int execvp();
extern int execv();

#define xdup2(a, b) do {				\
  if (a != -1)						\
    if (dup2 (a, b) < 0)				\
      {							\
	fprintf (stderr, "%s: dup2(%d, %d): %s\n",	\
	         this_program, a, b, xstrerror(errno));	\
        exit(127);					\
      }							\
} while (0);

#define xclose(fd) do {					\
  if (fd != -1)						\
    {							\
      close (fd);					\
      fd = -1;						\
    }							\
} while (0)

static const char *this_program = "<unknown>";

/* Execute PROGRAM with argument vector ARGV.  If SEARCH is false,
   PROGRAM is expected to be a usable pathname already; otherwise it
   is looked for in the locations specified by the PATH environment
   variable (as per execvp).

   STDIN_FD, STDOUT_FD, and STDERR_FD are file descriptors which
   should be established as the child process's stdin, stdout, or
   stderr, respectively, or -1 if the child should inherit the
   parent's value of that descriptor.  If any of these is not -1, it
   should be a valid file descriptor marked close-on-exec.  */

int
pexec (program, argv, search, stdin_fd, stdout_fd, stderr_fd)
     const char *program;
     char *const *argv;
     int search;
     int stdin_fd;
     int stdout_fd;
     int stderr_fd;
{
  int pid;
  int retries, sleep_interval;

  /* Canonicalize file descriptor arguments.  */
  stdin_fd  = (stdin_fd == STDIN_FILE_NO)   ? -1 : stdin_fd;
  stdout_fd = (stdout_fd == STDOUT_FILE_NO) ? -1 : stdout_fd;
  stderr_fd = (stderr_fd == STDERR_FILE_NO) ? -1 : stderr_fd;

  /* If exec fails, the child will write a message to stderr, using
     fprintf.  It must therefore use exit(), not _exit().  So we have
     to flush pending output here to avoid duplicate output.  */
  fflush (NULL);

  /* Fork a subprocess; wait and retry if it fails.  */
  sleep_interval = 1;
  pid = -1;
  for (retries = 0; retries < 4; retries++)
    {
      pid = fork ();
      if (__builtin_expect (pid >= 0, 1))
	break;
      sleep (sleep_interval);
      sleep_interval *= 2;
    }

  if (pid != 0)
    {
      /* Parent; fork either succeeded or failed.  In either case we
	 close the file descriptors passed to the child, to make life
	 easier for callers.  Don't clobber errno while doing so. */
      int serrno = errno;
      xclose (stdin_fd);
      xclose (stdout_fd);
      xclose (stderr_fd);
      errno = serrno;
      
      return pid;
    }

  /* We are the child.  Set up I/O redirections and call exec.
     FIXME: Report errors without calling fprintf() in the child
     (so we can use vfork).  */
  xdup2 (stdin_fd, STDIN_FILE_NO);
  xdup2 (stdout_fd, STDOUT_FILE_NO);
  xdup2 (stderr_fd, STDERR_FILE_NO);

  (search ? execvp : execv) (program, argv);

  /* If we got here exec failed.  */
/*fprintf (stderr, install_error_msg,  XXX completely wrong format string */
  fprintf (stderr, "%s tried to exec %s but failed (%s)\n",
	   this_program, program, xstrerror (errno));
  exit (127);
}

/* Create a pipe suitable for use with pexec - in particular, both
   ends of the pipe are set close-on-exec.  Returns 0 if successful,
   -1 otherwise.  Returns the pipe in the array argument, just as
   pipe(2) does.  */

int
pmkpipe (thepipe)
     int thepipe[2];
{
  if (pipe (thepipe))
    return -1;

  if (fcntl (thepipe[0], F_SETFD, FD_CLOEXEC)
      || fcntl (thepipe[1], F_SETFD, FD_CLOEXEC))
    {
      int serrno = errno;
      close (thepipe[0]);
      close (thepipe[1]);
      errno = serrno;
      return -1;
    }
  return 0;
}

int
pexecute (program, argv, this_pname, temp_base, errmsg_fmt, errmsg_arg, flags)
     const char *program;
     char * const *argv;
     const char *this_pname ATTRIBUTE_UNUSED;
     const char *temp_base ATTRIBUTE_UNUSED;
     char **errmsg_fmt, **errmsg_arg;
     int flags;
{
  /* Pipe waiting from last process, to be used as input for the next
     one.  Value is -1 if no pipe is waiting (i.e. the next command is
     the first of a group).  */
  static int last_pipe_input = -1;

  int pid;
  int pdesc[2];
  int serrno;
  int child_stdin = -2, child_stdout = -2;
  /* If this is the first process, last_pipe_input ought to be -1.  */
  if (flags & PEXECUTE_FIRST)
    if (last_pipe_input != -1)
      abort ();

  child_stdin = last_pipe_input;

  /* If this is the last process, don't do anything with its output
     pipe.  */
  if (flags & PEXECUTE_LAST)
    child_stdout = -1;
  else
    {
      /* Create a pipe to go between this process and the next one in
	 the pipeline.  */
      if (pmkpipe (pdesc))
	{
	  *errmsg_fmt = "pipe";
	  *errmsg_arg = NULL;
	  return -1;
	}
      last_pipe_input = pdesc[READ_PORT];
      child_stdout = pdesc[WRITE_PORT];
    }

  pid = pexec (program, argv, (flags & PEXECUTE_SEARCH),
	       child_stdin, child_stdout, -1);

  serrno = errno;
  xclose (child_stdin);
  xclose (child_stdout);

  /* To prevent a file descriptor leak, close last_pipe_input if pexec
     failed.  */
  if (pid == -1)
    xclose (last_pipe_input);

  if (flags & PEXECUTE_LAST)
    last_pipe_input = -1;

  errno = serrno;

  if (pid == -1)
    {
      *errmsg_fmt = "fork";
      *errmsg_arg = NULL;
    }

  return pid;
}

int
pwait (pid, status, flags)
     int pid;
     int *status;
     int flags ATTRIBUTE_UNUSED;
{
  /* ??? Here's an opportunity to canonicalize the values in STATUS.
     Needed?  */
  pid = waitpid (pid, status, 0);
  return pid;
}

void
pexec_set_program_name (name)
     const char *name;
{
  this_program = name;
}
