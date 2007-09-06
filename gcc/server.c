/* Shared code for GCC server
   Copyright (C) 2007
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "server.h"
#include "errors.h"
#include "dyn-string.h"
#include "opts.h"

#include <sys/socket.h>
#include <sys/un.h>

/* The name of the server socket we're using.  */
static char *server_socket_name;

/* The file descriptor of our connection to the server.  */
static int connection_fd = -1;

/* Return the name of the server socket.  The value is cached in
   'server_socket_name'.  PROGNAME is the path name of the server
   executable and is used to construct the socket name.  */
static char *
get_socket_name (const char *progname)
{
  if (!server_socket_name)
    {
      const char *basename;

      basename = strrchr (progname, '/');
      if (basename == NULL)
	basename = progname;

      /* FIXME: put in subdir and use UID.  */
      server_socket_name = concat ("/tmp/", ".", basename, "-server", NULL);
    }

  return server_socket_name;
}

/* Start a compile server.  PROGRAM is the full path name of the
   server to start.  Returns when the server is ready, or prints a
   message to stderr and exits on failure.  */
void
server_start (char *program)
{
  int fds[2];

  /* Make a pipe so we can tell when the server is ready.  */
  pipe (fds);			/* FIXME: error checking */

  switch (fork ())
    {
    case 0:
      {
	/* Child.  */
	char fdstr[40];
	char *args[5];
	int i = 0;
#ifdef ENABLE_VALGRIND_CHECKING
	args[i++] = VALGRIND_PATH;
	args[i++] = "-q";
#endif
	args[i++] = program;
	sprintf (fdstr, "-fserver=%d", fds[1]);
	args[i++] = fdstr;
	args[i++] = NULL;
	/* Close read end of notification pipe.  */
	close (fds[0]);
	/* Don't need these any more -- but keep stderr while
	   debugging.  */
	close (0);
	close (1);
	execv (args[0], args);
	error ("exec of server failed: %s", xstrerror (errno));
	exit (FATAL_EXIT_CODE);
      }
      break;

    case -1:
      error ("fork failed while starting server: %s", xstrerror (errno));
      exit (FATAL_EXIT_CODE);
      break;

    default:
      {
	/* Parent.  */
	char x;
	close (fds[1]);
	int r = read (fds[0], &x, 1);
	if (r == -1)
	  error ("server failed to start: %s", xstrerror (errno));
	close (fds[0]);
      }
    }
}

/* Open a server socket.  PROGNAME is the path name of the server.
   Returns the new file descriptor, or -1 on error. */
static int
open_socket (const char *progname)
{
  struct sockaddr_un address;
  int sockfd, save_mask;
  char *sname = get_socket_name (progname);

  sockfd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0)
    return -1;

  address.sun_family = AF_UNIX;
  /* FIXME: buffer overflow.  */
  strcpy (address.sun_path, sname);

  save_mask = umask (077);
  if (bind (sockfd, (struct sockaddr *) &address, SUN_LEN (&address)) < 0)
    return -1;

  umask (save_mask);
  return sockfd;
}

/* Free the contents of an ARGV array, then the array itself.  */
static void
free_argv (char **argv)
{
  int i;
  for (i = 0; argv[i]; ++i)
    free (argv[i]);
  free (argv);
}

/* Split a string into elements at spaces.  A backslash in the string
   quotes the following character.  The resulting array should later
   be freed with free_argv.  */
static char **
split_argv (char *buffer)
{
  char **argv = NULL;
  int alloc = 0;
  int used = 0;
  char *p = buffer;
  dyn_string_t str = dyn_string_new (20);

  while (*p)
    {
      if (*p == '\\')
	{
	  ++p;
	  if (!*p)
	    {
	      /* Not actually valid...  */
	      break;
	    }
	  dyn_string_append_char (str, *p);
	}
      else if (*p == ' ')
	{
	  if (used == alloc)
	    {
	      alloc = 2 * alloc + 20;
	      argv = (char **) xrealloc (argv, alloc * sizeof (char *));
	    }
	  argv[used++] = dyn_string_release (str);
	  if (!*p)
	    {
	      str = NULL;
	      break;
	    }
	  str = dyn_string_new (20);
	}
      else
	dyn_string_append_char (str, *p);
      ++p;
    }

  if (used + 2 >= alloc)
    {
      alloc = used + 2;
      argv = (char **) xrealloc (argv, alloc * sizeof (char *));
    }

  if (str)
    argv[used++] = dyn_string_release (str);
  argv[used] = NULL;

  return argv;
}

/* Helper function for server_main_loop.  Read a request from the file
   descriptor REQFD and take action.  */
static bool
request_and_response (int reqfd)
{
  char **argvs[2];
  int count = 0;

  while (true)
    {
      char cmd;
      if (read (reqfd, &cmd, 1) != 1)
	break;
      if (cmd == 'X')
	{
	  /* Execute a command.  */
	  int len;
	  char *buffer, **argv;

	  if (read (reqfd, &len, sizeof (len)) != sizeof (len))
	    break;
	  buffer = (char *) xmalloc (len + 1);
	  if (read (reqfd, buffer, len) != len)
	    break;
	  buffer[len] = '\0';

	  argv = split_argv (buffer);
	  free (buffer);
	  /* FIXME: obviously this is pretty lame.  */
	  if (count >= 2)
	    {
	      fprintf (stderr, "DIE!\n");
	      continue;
	    }

	  argvs[count++] = argv;
	}
      else if (cmd == 'D')
	{
	  /* Done with requests, compile away.  */
	  if (count != 2)
	    {
	      fprintf (stderr, "DIE 2!\n");
	      count = 0;	/* And leak memory while we're at it.  */
	      continue;
	    }
	  server_callback (reqfd, argvs[0], argvs[1]);
	  free_argv (argvs[0]);
	  free_argv (argvs[1]);
	  count = 0;
	  break;
	}
      else if (cmd == 'K')
	{
	  /* Kill server.  In the single-threaded server, it is ok to
	     simply tell the main loop to exit.  */
	  return false;
	}
      else
	{
	  /* FIXME: error.  */
	  break;
	}
    }
  return true;
}

/* Main loop of the server.  Creates a server socket and listens to
   it, accepting requests and acting on them.  PROGNAME is the full
   path to the server executable.  FD is the completion file
   descriptor, used to notify the gcc driver when the server is ready
   to accept connections.  */
void
server_main_loop (const char *progname, int fd)
{
  int sockfd = open_socket (progname);
  char reply = 't';
  bool result = true;

  if (sockfd < 0)
    {
      error ("couldn't create server socket: %s", xstrerror (errno));
      /* FIXME: unlink the socket if it exists.  */
      /* Simply exiting is ok -- GCC will detect that the socket has
	 been closed.  */
      exit (FATAL_EXIT_CODE);
    }

  write (fd, &reply, 1);
  close (fd);
  fprintf (stderr, "server is ready\n");

  listen (sockfd, 5);

  while (result)
    {
      int reqfd = accept (sockfd, NULL, 0);
      if (reqfd < 0)
	{
	  error ("error while accepting: %s", xstrerror (errno));
	  break;
	}
      result = request_and_response (reqfd);
      close (reqfd);
    }

  close (sockfd);
  if (server_socket_name)
    unlink (server_socket_name);
}

/* Fork to let the back end do its work.  Returns true in the parent,
   false in the child.  Will print a message and return true if there
   is an error.  The parent waits for the child to exit.  */
bool
server_start_back_end (void)
{
  pid_t child;
  int status;

  child = fork ();
  if (child == -1)
    {
      error ("fork of server failed: %s", xstrerror (errno));
      return true;
    }
  else if (child == 0)
    {
      /* Child process.  */
      return false;
    }

  /* Parent.  */
  waitpid (child, &status, 0);
  /* FIXME: handle STATUS somehow.  */
  return true;
}

/* Make a connection to a running server.  PROGNAME is the name of the
   server to connect to.  Returns true on success, false on
   failure.  */
bool
client_connect (const char *progname)
{
  char *sname = get_socket_name (progname);
  struct sockaddr_un address;

  connection_fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (connection_fd < 0)
    return -1;

  address.sun_family = AF_UNIX;
  /* FIXME: buffer overflow.  */
  strcpy (address.sun_path, sname);

  if (connect (connection_fd, (struct sockaddr *) &address,
	       SUN_LEN (&address)) < 0)
    return false;

  return true;
}

/* Send a command to the server.  'client_connect' must already have
   been called successfully.  ARGV is the command line that the server
   should execute (or "emulate").  Returns true on success, false on
   failure.  */
bool
client_send_command (const char **argv)
{
  dyn_string_t quoted;
  char cmd, *line;
  int i, len;
  bool result;

  gcc_assert (connection_fd >= 0);

  quoted = dyn_string_new (50);

  for (i = 0; argv[i]; ++i)
    {
      int j;
      if (i > 0)
	dyn_string_append_char (quoted, ' ');
      for (j = 0; argv[i][j]; ++j)
	{
	  if (argv[i][j] == ' ' || argv[i][j] == '\\')
	    dyn_string_append_char (quoted, '\\');
	  dyn_string_append_char (quoted, argv[i][j]);
	}
    }

  line = dyn_string_release (quoted);

  len = strlen (line);
  cmd = 'X';

  if (write (connection_fd, &cmd, sizeof (cmd)) != sizeof (cmd)
      || write (connection_fd, &len, sizeof (len)) != sizeof (len))
    result = false;
  else
    result = (write (connection_fd, line, len) == len);
  free (line);
  return result;
}

/* Helper for client_wait and client_kill_server.  Send a
   single-letter command to the server and pipe the server's output to
   our stderr.  */
static void
send_command_and_wait (char cmd)
{
  gcc_assert (connection_fd >= 0);

  if (write (connection_fd, &cmd, 1) != 1)
    return;

  while (true)
    {
      int len;
      char buffer[100];

      len = read (connection_fd, buffer, 100);
      if (len <= 0)
	break;
      fwrite (buffer, 1, len, stderr);
    }
}

/* Called by the client after submitting all its jobs.  This waits for
   the server to complete the tasks.  It reads from the server
   connection and prints errors to stderr.  Both 'client_connect' and
   'client_send_command' must have been successfully called before
   calling this.  */
void
client_wait (void)
{
  send_command_and_wait ('D');
}

/* Request that the server exit.  PROGNAME is the path name of the
   server executable.  */
void
client_kill_server (const char *progname)
{
  if (!client_connect (progname))
    {
      error ("couldn't connect to server: %s", xstrerror (errno));
      return;
    }
  send_command_and_wait ('K');
}
