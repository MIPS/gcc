/* Shared code for GCC server
   Copyright (C) 2007, 2008
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
#include "opts.h"

#include <sys/socket.h>
#include <sys/un.h>

/* From libiberty, but not in any header.  */
extern char *choose_tmpdir (void);

/* Possible server-related states that the compiler can be in.  */
enum server_state
{
  /* Not running as a server.  */
  SERVER_NONE,
  /* Running as a server, in the server itself.  */
  SERVER_SERVER,
  /* Running as a server, in the forked code-generation subprocess.  */
  SERVER_CODEGEN
};

/* The server state.  It is useful to keep this for internal checking,
   but it seems better not to export it as a global to the rest of the
   compiler.  */
static enum server_state current_server_state = SERVER_NONE;

/* The name of the server socket we're using.  */
static char *server_socket_name;

/* The name of the server directory.  Only set in the server.  We
   delete this when the server exits.  */
static char *server_directory;

/* The file descriptor of our connection to the server.  */
static int connection_fd = -1;

/* On the client, the process id of the server.  */
static pid_t server_pid;

/* True if interrupted.  Both the client and the server use this.  */
static volatile sig_atomic_t interrupted;

/* Return the name of the server socket.  The value is cached in
   'server_socket_name'.  PROGNAME is the path name of the server
   executable and is used to construct the socket name.  If SERVER is
   true, this will try to make the server directory.  */
static char *
get_socket_name (const char *progname, bool server)
{
  if (!server_socket_name)
    {
      const char *basename;
      char *dir = choose_tmpdir ();
      char uidstr[20];

      if (dir == NULL)
	{
	  error ("could not find temporary directory");
	  exit (FATAL_EXIT_CODE);
	}

      basename = strrchr (progname, '/');
      if (basename == NULL)
	basename = progname;

      sprintf (uidstr, "%d", (int) getuid ());
      dir = concat (dir, "/", ".gcc-", uidstr, NULL);

      if (server)
	{
	  if (mkdir (dir, S_IRWXU))
	    {
	      error ("could not make directory %s: %s", dir, xstrerror (errno));
	      exit (FATAL_EXIT_CODE);
	    }
	  server_directory = xstrdup (dir);
	}

      server_socket_name = reconcat (dir, dir, "/", basename, "-server", NULL);
    }

  return server_socket_name;
}

/* If we computed a socket name, forget about it now.  */
static void
forget_socket_name (void)
{
  if (server_socket_name)
    {
      free (server_socket_name);
      server_socket_name = NULL;
    }
}

/* If SOCKET is not -1, close the server socket.  Unlink the server
   socket and its directory as well.  */
static void
server_cleanup (int socket)
{
  if (socket != -1)
    close (socket);
  if (server_socket_name)
    unlink (server_socket_name);
  if (server_directory)
    rmdir (server_directory);
}

/* Start a compile server.  PROGRAM is the full path name of the
   server to start.  Returns when the server is ready, or prints a
   message to stderr and exits on failure.  */
void
server_start (char *program)
{
  int fds[2];

  /* Make a pipe so we can tell when the server is ready.  */
  if (pipe (fds) == -1)
    {
      error ("failed to make pipe: %s", xstrerror (errno));
      exit (FATAL_EXIT_CODE);
    }

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
	close (fileno (stdin));
	close (fileno (stdout));
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
	int r;
	char x;
	close (fds[1]);
	r = read (fds[0], &x, 1);
	if (r == -1)
	  {
	    error ("server failed to start: %s", xstrerror (errno));
	    exit (FATAL_EXIT_CODE);
	  }
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
  char *sname = get_socket_name (progname, true);

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

/* Helper function for server_main_loop.  Read a request from the file
   descriptor REQFD and take action.  */
static bool
request_and_response (int reqfd)
{
  char *dir = NULL;
  char **argvs[2];
  int count = 0;
  int err_fd = -1;

  argvs[0] = NULL;
  argvs[1] = NULL;

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

	  argv = buildargv (buffer);
	  free (buffer);
	  if (count >= 2)
	    {
	      /* We do nothing here, but will fail when the eventual
		 'D' command arrives.  */
	      freeargv (argv);
	    }
	  else
	    argvs[count++] = argv;
	}
      else if (cmd == '.')
	{
	  /* Set working directory.  */
	  int len;

	  if (dir)
	    free (dir);
	  if (read (reqfd, &len, sizeof (len)) != sizeof (len))
	    break;
	  dir = (char *) xmalloc (len + 1);
	  if (read (reqfd, dir, len) != len)
	    break;
	  dir[len] = '\0';
	}
      else if (cmd == 'D')
	{
	  /* Done with requests, compile away.  */
	  bool ok;
	  int result;

	  if (count != 2 || !dir || err_fd == -1)
	    {
	      /* Fail.  */
	      ok = false;
	    }
	  else
	    ok = server_callback (err_fd, dir, argvs[0], argvs[1]);

	  free (dir);
	  freeargv (argvs[0]);
	  freeargv (argvs[1]);
	  dir = NULL;
	  argvs[0] = NULL;
	  argvs[1] = NULL;
	  count = 0;
	  if (err_fd != -1)
	    close (err_fd);
	  err_fd = -1;

	  /* Tell the client the result.  */
	  result = ! ok;
	  /* There's nothing sensible to do about an error here.  */
	  write (reqfd, &result, sizeof (result));

	  break;
	}
      else if (cmd == 'K')
	{
	  /* Kill server.  In the single-threaded server, it is ok to
	     simply tell the main loop to exit.  */
	  return false;
	}
      else if (cmd == 'F')
	{
	  /* Receive the client's stderr fd.  */
	  struct msghdr msg;
	  char space[CMSG_SPACE (sizeof (int))];
	  char iovbuffer[1];
	  struct iovec vec;

	  memset (&msg, 0, sizeof (msg));
	  msg.msg_control = space;
	  msg.msg_controllen = sizeof (space);

	  /* We have to receive at least one message byte along with
	     the file descriptor.  */
	  vec.iov_base = iovbuffer;
	  vec.iov_len = 1;
	  msg.msg_iov = &vec;
	  msg.msg_iovlen = 1;

	  if (recvmsg (reqfd, &msg, 0) >= 0)
	    {
	      struct cmsghdr *cmsg = CMSG_FIRSTHDR (&msg);
	      if (cmsg && cmsg->cmsg_len == CMSG_LEN (sizeof (int))
		  && cmsg->cmsg_level == SOL_SOCKET
		  && cmsg->cmsg_type == SCM_RIGHTS)
		err_fd = *(int *) CMSG_DATA (cmsg);
	    }
	}
      else
	{
	  /* FIXME: error.  */
	  break;
	}
    }
  return true;
}

/* On server, the SIGINT handler.  */
static void
server_sigint_handler (int ARG_UNUSED (num))
{
  interrupted = 1;
}

/* Main loop of the server.  Creates a server socket and listens to
   it, accepting requests and acting on them.  PROGNAME is the full
   path to the server executable.  FD is the completion file
   descriptor, used to notify the gcc driver when the server is ready
   to accept connections.  */
int
server_main_loop (const char *progname, int fd)
{
  int sockfd = open_socket (progname);
  char reply = 't';
  bool result = true;
  int code = SUCCESS_EXIT_CODE;
  pid_t self = getpid ();

  setsid ();
  signal (SIGINT, server_sigint_handler);

  current_server_state = SERVER_SERVER;

  if (sockfd < 0)
    {
      error ("couldn't create server socket: %s", xstrerror (errno));
      server_cleanup (sockfd);
      /* Simply exiting is ok -- the client will detect that the
	 socket has been closed.  */
      return FATAL_EXIT_CODE;
    }

  write (fd, &reply, 1);
  close (fd);
  fprintf (stderr, "server is ready; pid = %ld\n", (long) getpid ());

  listen (sockfd, 5);

  while (result)
    {
      int reqfd = accept (sockfd, NULL, 0);
      if (reqfd < 0)
	{
	  error ("error while accepting: %s", xstrerror (errno));
	  code = FATAL_EXIT_CODE;
	  break;
	}

      if (write (reqfd, &self, sizeof (self)) == sizeof (self))
	result = request_and_response (reqfd);
      close (reqfd);

      interrupted = 0;
    }

  server_cleanup (sockfd);
  return code;
}

/* Fork to let the back end do its work.  Returns true in the parent,
   false in the child.  Will print a message and return true if there
   is an error.  The parent waits for the child to exit.  This can
   also return true, with a false *STATUS, if the server was
   interrupted.  */
bool
server_start_back_end (bool *status)
{
  pid_t child;
  int result;
  sigset_t block_int;

  /* Avoid races by blocking SIGINT for a bit.  */
  sigemptyset (&block_int);
  sigaddset (&block_int, SIGINT);
  sigprocmask (SIG_BLOCK, &block_int, NULL);

  if (interrupted)
    {
      *status = false;
      goto done;
    }

  child = fork ();
  if (child == -1)
    {
      error ("fork of server failed: %s", xstrerror (errno));
      *status = false;
      goto done;
    }
  else if (child == 0)
    {
      /* Child process.  */
      sigprocmask (SIG_UNBLOCK, &block_int, NULL);
      signal (SIGINT, SIG_DFL);
      current_server_state = SERVER_CODEGEN;
      return false;
    }

  /* Parent.  */
  waitpid (child, &result, 0);
  *status = WIFEXITED (result) && ! WEXITSTATUS (result);

 done:
  /* It is ok to leave SIGINT blocked for the duration of the
     subprocess, because we will get the correct value in *STATUS
     anyway.  */
  sigprocmask (SIG_UNBLOCK, &block_int, NULL);

  return true;
}

/* Return true if the server has received a SIGINT, false otherwise.  */
bool
server_interrupted_p (void)
{
  return interrupted;
}

/* Called in response to a SIGINT, while connected to the server.  */
static void
client_sigint_handler (int num)
{
  gcc_assert (server_pid);
  interrupted = 1;
  /* Probably not ok to do this here.  */
  kill (- server_pid, num);
}

/* Make a connection to a running server.  PROGNAME is the name of the
   server to connect to.  Returns true on success, false on
   failure.  */
bool
client_connect (const char *progname)
{
  char *sname = get_socket_name (progname, false);
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

  if (read (connection_fd, &server_pid, sizeof (server_pid))
      != sizeof (server_pid))
    return false;

  signal (SIGINT, client_sigint_handler);

  return true;
}

/* Send the current working directory to the server.  'client_connect'
   must already have been called successfully.  Returns true on
   success, false on failure.  */
bool
client_send_directory (void)
{
  char *dir = getcwd (NULL, 0);
  char cmd = '.';
  int len = strlen (dir);
  bool result;

  gcc_assert (connection_fd >= 0);

  if (write (connection_fd, &cmd, sizeof (cmd)) != sizeof (cmd)
      || write (connection_fd, &len, sizeof (len)) != sizeof (len))
    result = false;
  else
    result = (write (connection_fd, dir, len) == len);
  free (dir);
  return result;
}

/* Send a command to the server.  'client_connect' must already have
   been called successfully.  ARGV is the command line that the server
   should execute (or "emulate").  Returns true on success, false on
   failure.  */
bool
client_send_command (const char **argv)
{
  char cmd, *line;
  int len;
  bool result;

  gcc_assert (connection_fd >= 0);

  line = argvtostr (argv);
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

/* Send a file descriptor to the server.  Return false on error, true
   on success.  */
static bool
send_fd (int fd)
{
  gcc_assert (connection_fd >= 0);
  char cmd = 'F';
  struct msghdr msg;
  struct cmsghdr *cmsg;
  char space[CMSG_SPACE (sizeof (int))];
  int *fdptr;
  char iovbuffer[1];
  struct iovec vec;

  if (write (connection_fd, &cmd, 1) != 1)
    return false;
  
  memset (&msg, 0, sizeof (msg));
  msg.msg_control = space;
  msg.msg_controllen = sizeof (space);
  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (sizeof (int));

  /* We have to send at least one message byte along with the file
     descriptor.  */
  iovbuffer[0] = '!';
  vec.iov_base = iovbuffer;
  vec.iov_len = 1;
  msg.msg_iov = &vec;
  msg.msg_iovlen = 1;

  fdptr = (int *) CMSG_DATA (cmsg);
  fdptr[0] = fd;
  msg.msg_controllen = cmsg->cmsg_len;

  return sendmsg (connection_fd, &msg, 0) >= 0;
}

/* Helper for client_wait and client_kill_server.  Send a
   single-letter command to the server and arrange for the server's
   output to go to our stderr.  Returns true on success, false on
   failure.  */
static bool
send_command_and_wait (char cmd)
{
  int status;
  gcc_assert (connection_fd >= 0);

  if (! send_fd (fileno (stderr)))
    return false;

  if (write (connection_fd, &cmd, 1) != 1)
    return false;

  if (read (connection_fd, &status, sizeof (status)) != sizeof (status))
    return false;

  /* Make sure we exit with the correct status when interrupted.  */
  signal (SIGINT, SIG_DFL);
  if (interrupted)
    raise (SIGINT);

  close (connection_fd);
  connection_fd = -1;
  forget_socket_name ();

  return status ? false : true;
}

/* Called by the client after submitting all its jobs.  This waits for
   the server to complete the tasks.  Both 'client_connect' and
   'client_send_command' must have been successfully called before
   calling this.  Returns true if everything went successfully, false
   on failure.  */
bool
client_wait (void)
{
  return send_command_and_wait ('D');
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

/* Assert that either this compiler is running standalone, or that the
   compiler has forked and we are in the code generation subprocess.
   This can be used to decide when it is ok to destructively rewrite
   front end trees.  */
void
server_assert_code_generation (void)
{
  gcc_assert (current_server_state == SERVER_NONE
	      || current_server_state == SERVER_CODEGEN);
}
