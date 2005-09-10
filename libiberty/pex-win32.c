/* Utilities to execute a program in a subprocess (possibly linked by pipes
   with other subprocesses), and wait for it.  Generic Win32 specialization.
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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static const char *this_program;

#define xclose(fd) do {					\
  if (fd != -1)						\
    {							\
      _close (fd);					\
      fd = -1;						\
    }							\
} while (0)

/* Returns a string containing a text error message, after a Windows
   "system call" failed.  Caller is responsible for deallocating it
   (with LocalFree()).  */
static char *
get_last_error_as_text ()
{
  DWORD last_error = GetLastError();
  LPSTR result;

  /* We assume the error message belongs to 'the system' as opposed
     to some module (which we would have to load, and we don't know
     which one it is).  */
  DWORD flags = (FORMAT_MESSAGE_ALLOCATE_BUFFER
		 | FORMAT_MESSAGE_IGNORE_INSERTS
		 | FORMAT_MESSAGE_FROM_SYSTEM);

  /* Default language.  */
  DWORD langid = MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT);

  /* Yes, you are supposed to cast LPSTR* to LPSTR in the fifth
     argument.  This interface is intrinsically type-unsafe.  */
  FormatMessageA(flags, 0, last_error, langid, (LPSTR) &result, 0, 0);

  return result;
}
  
static char *
argv_to_cmdline(argv)
     char *const *argv;
{
  char *cmdline;
  char *p;
  size_t cmdline_len;
  int i, j, k;

  cmdline_len = 0;
  for (i = 0; argv[i]; i++)
    {
      /* We quote every last argument.  This simplifies the problem;
	 we need only escape embedded double-quotes and immediately
	 preceeding backslash characters.  A sequence of backslach characters
	 that is not follwed by a double quote character will not be
	 escaped.  */
      for (j = 0; argv[i][j]; j++)
	{
	  if (argv[i][j] == '"')
	    {
	      /* Escape preceeding backslashes.  */
	      for (k = j - 1; k >= 0 && argv[i][k] == '\\'; k--)
		cmdline_len++;
	      /* Escape the qote character.  */
	      cmdline_len++;
	    }
	}
      /* Trailing backslashes also need to be escaped because they will be
         followed by the terminating quote.  */
      for (k = j - 1; k >= 0 && argv[i][k] == '\\'; k--)
	cmdline_len++;
      cmdline_len += j;
      cmdline_len += 3;  /* for leading and trailing quotes and space */
    }
  cmdline = xmalloc (cmdline_len);
  p = cmdline;
  for (i = 0; argv[i]; i++)
    {
      *p++ = '"';
      for (j = 0; argv[i][j]; j++)
	{
	  if (argv[i][j] == '"')
	    {
	      for (k = j - 1; k >= 0 && argv[i][k] == '\\'; k--)
		*p++ = '\\';
	      *p++ = '\\';
	    }
	  *p++ = argv[i][j];
	}
      for (k = j - 1; k >= 0 && argv[i][k] == '\\'; k--)
	*p++ = '\\';
      *p++ = '"';
      *p++ = ' ';
    }
  p[-1] = '\0';
  return cmdline;
}

static const char *const
std_suffixes[] = {
  ".com",
  ".exe",
  ".bat",
  ".cmd",
  0
};
static const char *const
no_suffixes[] = {
  "",
  0
};

static char *
find_executable (program, search)
     const char *program;
     int search;
{
  char *full_executable;
  char *e;
  size_t fe_len;
  const char *path = 0;
  const char *const *ext;
  const char *p, *q;
  size_t proglen = strlen (program);
  int has_extension = !!strchr (program, '.');
  int has_slash = (strchr (program, '/') || strchr (program, '\\'));
  HANDLE h;

  if (has_slash)
    search = 0;

  if (search)
    path = getenv ("PATH");
  if (!path)
    path = "";

  fe_len = 0;
  for (p = path; *p; p = q)
    {
      q = p;
      while (*q != ';' && *q != '\0')
	q++;
      if ((size_t)(q - p) > fe_len)
	fe_len = q - p;
      if (*q == ';')
	q++;
    }
  fe_len = fe_len + 1 + proglen + (has_extension ? 1 : 5);
  full_executable = xmalloc (fe_len);

  p = path;
  do
    {
      q = p;
      while (*q != ';' && *q != '\0')
	q++;

      e = full_executable;
      memcpy (e, p, q - p);
      e += (q - p);
      if (q - p)
	*e++ = '\\';
      strcpy (e, program);

      if (*q == ';')
	q++;

      for (e = full_executable; *e; e++)
	if (*e == '/')
	  *e = '\\';

      /* At this point, e points to the terminating NUL character for
         full_executable.  */
      for (ext = has_extension ? no_suffixes : std_suffixes; *ext; ext++)
	{
	  /* Remove any current extension.  */
	  *e = '\0';
	  /* Add the new one.  */
	  strcat (full_executable, *ext);

	  /* Attempt to open this file.  */
	  h = CreateFile (full_executable, GENERIC_READ,
			  FILE_SHARE_READ | FILE_SHARE_WRITE,
			  0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	  if (h != INVALID_HANDLE_VALUE)
	    goto found;
	}
      p = q;
    }
  while (*p);
  free (full_executable);
  return 0;

 found:
  CloseHandle (h);
  return full_executable;
}

/* Tries to duplicate a handle. Returns true for successful duplication, or
   if GetStdHandle returns 0 or INVALID_HANDLE_VALUE. Returns false on
   error.  */

static int
maybe_duplicate_handle (HANDLE *dest, int fd, DWORD type, HANDLE me)
{
  if (fd == -1)
    {
      HANDLE std_handle = GetStdHandle (type);
      if (std_handle != INVALID_HANDLE_VALUE
	  && std_handle != 0)
	{
	  if (!DuplicateHandle (me, std_handle, me, dest, 0, TRUE,
		                DUPLICATE_SAME_ACCESS))
	    return 0;
	}
    }
  else
    {
      if (!DuplicateHandle (me, (HANDLE)_get_osfhandle (fd), me, dest, 0, TRUE,
	                    DUPLICATE_SAME_ACCESS))
	return 0;
    }
  return 1;
}

int
pexec (program, argv, search, stdin_fd, stdout_fd, stderr_fd)
     const char *program;
     char *const *argv;
     int search;
     int stdin_fd;
     int stdout_fd;
     int stderr_fd;
{
  char *cmdline = argv_to_cmdline (argv);
  char *executable = find_executable (program, search);
  STARTUPINFO si;
  PROCESS_INFORMATION pinf;
  HANDLE me;

  /* Canonicalize file descriptor arguments.  */
  stdin_fd  = (stdin_fd == STDIN_FILE_NO)   ? -1 : stdin_fd;
  stdout_fd = (stdout_fd == STDOUT_FILE_NO) ? -1 : stdout_fd;
  stderr_fd = (stderr_fd == STDERR_FILE_NO) ? -1 : stderr_fd;

  memset (&si, 0, sizeof si);
  si.cb = sizeof si;
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput = INVALID_HANDLE_VALUE;
  si.hStdOutput = INVALID_HANDLE_VALUE;
  si.hStdError = INVALID_HANDLE_VALUE;

  me = GetCurrentProcess();

  if (!maybe_duplicate_handle (&si.hStdInput, stdin_fd, STD_INPUT_HANDLE, me))
    goto cleanup;

  if (!maybe_duplicate_handle (&si.hStdOutput, stdout_fd, STD_OUTPUT_HANDLE,
	                       me))
    goto cleanup;

  if (!maybe_duplicate_handle (&si.hStdError, stderr_fd, STD_ERROR_HANDLE, me))
    goto cleanup;

  if (!CreateProcess (executable, cmdline,
		     0, 0, TRUE, 0, 0, 0,
		     &si, &pinf))
    goto cleanup;

  CloseHandle (pinf.hThread);
  
 done:
  /* Close file descriptors passed to the child (whether or not we
     managed to create a child).  */
  xclose (stdin_fd);
  xclose (stdout_fd);
  if (stderr_fd != stdout_fd)
    xclose (stderr_fd);
  if (si.hStdInput != INVALID_HANDLE_VALUE)
    CloseHandle (si.hStdInput);
  if (si.hStdOutput != INVALID_HANDLE_VALUE)
    CloseHandle (si.hStdOutput);
  if (si.hStdError != INVALID_HANDLE_VALUE)
    CloseHandle (si.hStdError);

  free (executable);
  free (cmdline);

  /* Treat the process handle as an integer.  It would be cleaner to
     define a pid_t type that would be used by all libiberty callers;
     that type would be HANDLE under Windows.  However, that would
     require changing a lot of existing code.  */
  if (sizeof (HANDLE) != sizeof (int))
    abort ();
  return (int) pinf.hProcess;

 cleanup:
  {
    /* Error strings on win32 include newlines.  */
    char *errstr = get_last_error_as_text ();
    fprintf (stderr, "%s tried to spawn %s but failed: %s",
	     this_program, program, errstr);
    LocalFree (errstr);
  }
  /* Fortuitously, INVALID_HANDLE_VALUE is -1, which is the
     conventional error return value for pwait.  */
  pinf.hProcess = INVALID_HANDLE_VALUE;
  goto done;
}

/* MSVCRT's _pipe() creates pipes that can be inherited, which is not
   what we want, so we go directly to CreatePipe().  */
int
pmkpipe (thepipe)
     int thepipe[2];
{
  HANDLE read_port;
  HANDLE write_port;

  if (!CreatePipe (&read_port, &write_port, 0, 0))
    return -1;

  thepipe[0] = _open_osfhandle ((long)read_port, _O_RDONLY);
  thepipe[1] = _open_osfhandle ((long)write_port, _O_WRONLY);
  if (thepipe[0] == -1 || thepipe[1] == -1)
    {
      if (thepipe[0] == -1)
	CloseHandle (read_port);
      else
	_close (thepipe[0]);
      if (thepipe[1] == -1)
	CloseHandle (write_port);
      else
	_close (thepipe[1]);
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

  errno = serrno;

  if (pid == -1)
    {
      *errmsg_fmt = "spawn";
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
  /* The return value from pexecute is actually a HANDLE.  */
  HANDLE proch = (HANDLE) pid;
  if (WaitForSingleObject (proch, INFINITE) != WAIT_OBJECT_0)
    {
      CloseHandle (proch);
      return -1;
    }

  GetExitCodeProcess (proch, (DWORD *)status);
  CloseHandle (proch);
  return pid;
}

void
pexec_set_program_name (name)
     const char *name;
{
  this_program = name;
}
