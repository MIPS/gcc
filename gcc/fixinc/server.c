
/*
 *  server.c  Set up and handle communications with a server process.
 *
 *  Server Handling copyright 1992-1999 The Free Software Foundation
 *
 *  Server Handling is free software.
 *  You may redistribute it and/or modify it under the terms of the
 *  GNU General Public License, as published by the Free Software
 *  Foundation; either version 2, or (at your option) any later version.
 *
 *  Server Handling is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Server Handling.  See the file "COPYING".  If not,
 *  write to:  The Free Software Foundation, Inc.,
 *             59 Temple Place - Suite 330,
 *             Boston,  MA  02111-1307, USA.
 *
 * As a special exception, The Free Software Foundation gives
 * permission for additional uses of the text contained in his release
 * of ServerHandler.
 *
 * The exception is that, if you link the ServerHandler library with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the ServerHandler library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by The Free
 * Software Foundation under the name ServerHandler.  If you copy code
 * from other sources under the General Public License into a copy of
 * ServerHandler, as the General Public License permits, the exception
 * does not apply to the code that you add in this way.  To avoid
 * misleading anyone as to the status of such modified files, you must
 * delete this exception notice from them.
 *
 * If you write modifications of your own for ServerHandler, it is your
 * choice whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 */
#include "config.h"

#include <sys/types.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>

#include "server.h"

/* If this particular system's header files define the macro `MAXPATHLEN',
   we happily take advantage of it; otherwise we use a value which ought
   to be large enough.  */
#ifndef MAXPATHLEN
# define MAXPATHLEN     4096
#endif

#ifndef STDIN_FILENO
# define STDIN_FILENO	0
#endif
#ifndef STDOUT_FILENO
# define STDOUT_FILENO	1
#endif

#ifdef DEBUG
#define STATIC
#else
#define STATIC static
#endif
#ifndef tSCC
#define tSCC static const char
#endif
#ifndef NUL
#define NUL '\0'
#endif

STATIC bool read_pipe_timeout;

STATIC tpChar def_args[] =
{ (char *) NULL, (char *) NULL };
STATIC tpfPair server_pair =
{ (FILE *) NULL, (FILE *) NULL };
STATIC pid_t server_id = NULLPROCESS;
/*
 *  Arbitrary text that should not be found in the shell output.
 *  It must be a single line and appear verbatim at the start of
 *  the terminating output line.
 */
tSCC z_done[] = "ShElL-OuTpUt-HaS-bEeN-cOmPlEtEd";
STATIC tpChar p_cur_dir = (char *) NULL;

/*
 *  chainOpen
 *
 *  Given an FD for an inferior process to use as stdin,
 *  start that process and return a NEW FD that that process
 *  will use for its stdout.  Requires the argument vector
 *  for the new process and, optionally, a pointer to a place
 *  to store the child's process id.
 */
int
chainOpen (stdin_fd, pp_args, p_child)
     int stdin_fd;
     tpChar *pp_args;
     pid_t *p_child;
{
  tFdPair stdout_pair = {-1, -1};
  pid_t ch_id;
  char *pz_cmd;

  /*
   *  Create a pipe it will be the child process' stdout,
   *  and the parent will read from it.
   */
  if (pipe ((int *) &stdout_pair) < 0)
    {
      if (p_child != (pid_t *) NULL)
        *p_child = NOPROCESS;
      return -1;
    }

  /*
   *  If we did not get an arg list, use the default
   */
  if (pp_args == (tpChar *) NULL)
    pp_args = def_args;

  /*
   *  If the arg list does not have a program,
   *  assume the "SHELL" from the environment, or, failing
   *  that, then sh.  Set argv[0] to whatever we decided on.
   */
  if (pz_cmd = *pp_args,
      (pz_cmd == (char *) NULL) || (*pz_cmd == '\0'))
    {

      pz_cmd = getenv ("SHELL");
      if (pz_cmd == (char *) NULL)
        pz_cmd = "sh";
    }

#ifdef DEBUG_PRINT
  printf ("START:  %s\n", pz_cmd);
  {
    int idx = 0;
    
    while (pp_args[++idx] != (char *) NULL)
      printf ("  ARG %2d:  %s\n", idx, pp_args[idx]);
  }
#endif

  /*
   *  Call fork() and see which process we become
   */
  ch_id = fork ();
  switch (ch_id)
    {
    case NOPROCESS:             /* parent - error in call */
      close (stdout_pair.readFd);
      close (stdout_pair.writeFd);
      if (p_child != (pid_t *) NULL)
        *p_child = NOPROCESS;
      return -1;

    default:                    /* parent - return opposite FD's */
      if (p_child != (pid_t *) NULL)
        *p_child = ch_id;
#ifdef DEBUG_PRINT
      printf ("for pid %d:  stdin from %d, stdout to %d\n"
              "for parent:  read from %d\n",
              ch_id, stdin_fd, stdout_pair.writeFd, stdout_pair.readFd);
#endif
      close (stdin_fd);
      close (stdout_pair.writeFd);
      return stdout_pair.readFd;

    case NULLPROCESS:           /* child - continue processing */
      break;
    }

  /*
   *  Close the pipe end handed back to the parent process
   */
  close (stdout_pair.readFd);

  /*
   *  Close our current stdin and stdout
   */
  close (STDIN_FILENO);
  close (STDOUT_FILENO);

  /*
   *  Make the fd passed in the stdin, and the write end of
   *  the new pipe become the stdout.
   */
  fcntl (stdout_pair.writeFd, F_DUPFD, STDOUT_FILENO);
  fcntl (stdin_fd, F_DUPFD, STDIN_FILENO);

  if (*pp_args == (char *) NULL)
    *pp_args = pz_cmd;

  execvp (pz_cmd, pp_args);
  fprintf (stderr, "Error %d:  Could not execvp( '%s', ... ):  %s\n",
           errno, pz_cmd, strerror (errno));
  exit (EXIT_PANIC);
}


/*
 *  p2open
 *
 *  Given a pointer to an argument vector, start a process and
 *  place its stdin and stdout file descriptors into an fd pair
 *  structure.  The "writeFd" connects to the inferior process
 *  stdin, and the "readFd" connects to its stdout.  The calling
 *  process should write to "writeFd" and read from "readFd".
 *  The return value is the process id of the created process.
 */
pid_t
p2open (p_pair, pp_args)
     tFdPair *p_pair;
     tpChar *pp_args;
{
  pid_t ch_id;

  /*
   *  Create a bi-directional pipe.  Writes on 0 arrive on 1
   *  and vice versa, so the parent and child processes will
   *  read and write to opposite FD's.
   */
  if (pipe ((int *) p_pair) < 0)
    return NOPROCESS;

  p_pair->readFd = chainOpen (p_pair->readFd, pp_args, &ch_id);
  if (ch_id == NOPROCESS)
    close (p_pair->writeFd);

  return ch_id;
}


/*
 *  p2fopen
 *
 *  Identical to "p2open()", except that the "fd"'s are "fdopen(3)"-ed
 *  into file pointers instead.
 */
pid_t
p2fopen (pf_pair, pp_args)
     tpfPair *pf_pair;
     tpChar *pp_args;
{
  tFdPair fd_pair;
  pid_t ch_id = p2open (&fd_pair, pp_args);

  if (ch_id == NOPROCESS)
    return ch_id;

  pf_pair->pfRead = fdopen (fd_pair.readFd, "r");
  pf_pair->pfWrite = fdopen (fd_pair.writeFd, "w");
  return ch_id;
}


/*
 *  SHELL SERVER PROCESS CODE
 */

/*
 *  load_data
 *
 *  Read data from a file pointer (a pipe to a process in this context)
 *  until we either get EOF or we get a marker line back.
 *  The read data are stored in a malloc-ed string that is truncated
 *  to size at the end.  Input is assumed to be an ASCII string.
 */
static char *
load_data (fp)
     FILE *fp;
{
  char *pz_text;
  size_t text_size;
  char *pz_scan;
  char z_line[1024];

  text_size = sizeof (z_line) * 2;
  pz_scan = pz_text = malloc (text_size);

  if (pz_text == (char *) NULL)
    return (char *) NULL;

  for (;;)
    {
      size_t usedCt;

      alarm (10);
      read_pipe_timeout = BOOL_FALSE;
      if (fgets (z_line, sizeof (z_line), fp) == (char *) NULL)
        break;

      if (strncmp (z_line, z_done, sizeof (z_done) - 1) == 0)
        break;

      strcpy (pz_scan, z_line);
      pz_scan += strlen (z_line);
      usedCt = (size_t) (pz_scan - pz_text);

      if (text_size - usedCt < sizeof (z_line))
        {
          size_t off = (size_t) (pz_scan - pz_text);
          void *p;
	  
          text_size += 4096;
          p = realloc ((void *) pz_text, text_size);
          if (p == (void *) NULL)
            {
              fprintf (stderr, "Failed to get 0x%08X bytes\n", text_size);
              free ((void *) pz_text);
              return (char *) NULL;
            }
          pz_text = (char *) p;
          pz_scan = pz_text + off;
        }
    }

  alarm (0);
  if (read_pipe_timeout)
    {
      free ((void *) pz_text);
      return (char *) NULL;
    }

  while ((pz_scan > pz_text) && isspace (pz_scan[-1]))
    pz_scan--;
  *pz_scan = NUL;
  return realloc ((void *) pz_text, strlen (pz_text) + 1);
}


/*
 *  close_server
 *
 *  Make certain the server process is dead, close the 
 *  pipes to it and from it, finally NULL out the file pointers
 */
static void
close_server ()
{
  kill ((pid_t) server_id, SIGKILL);
  server_id = NULLPROCESS;
  fclose (server_pair.pfRead);
  fclose (server_pair.pfWrite);
  server_pair.pfRead = server_pair.pfWrite = (FILE *) NULL;
}

/*
 *  sig_handler really only handles the timeout and pipe signals.
 *  This ensures that we do not wait forever on a request
 *  to our server, and also that if the server dies, we do not
 *  die from a sigpipe problem.
 */
static void
sig_handler (signo)
     int signo;
{
  close_server ();
  read_pipe_timeout = BOOL_TRUE;
}


/*
 *  server_setup  Establish the signal handler for PIPE and ALARM.
 *  Also establishes the current directory to give to the
 *  server process at the start of every server command.
 */
static void
server_setup ()
{
  static int atexit_done = 0;
  
  if (atexit_done++ == 0)
    atexit (&close_server);

  signal (SIGPIPE, sig_handler);
  signal (SIGALRM, sig_handler);

  fputs ("trap : 1\n", server_pair.pfWrite);
  fflush (server_pair.pfWrite);
  p_cur_dir = getcwd ((char *) NULL, MAXPATHLEN + 1);
}


/*
 *  runShell
 *
 *  Run a shell command on the server.  The command string
 *  passed in is wrapped inside the sequence:
 *
 *     cd <original directory>
 *     <command string>
 *     echo
 *     echo <end-of-command-marker>
 *
 *  This ensures that all commands start at a known place in
 *  the directory structure, that any incomplete output lines
 *  are completed and that our special marker sequence appears on
 *  a line by itself.  We have chosen a marker that is
 *  excessively unlikely to be reproduced in normal output:
 *
 *     "ShElL-OuTpUt-HaS-bEeN-cOmPlEtEd"
 */
char *
runShell (pz_cmd)
     const char *pz_cmd;
{
  /*
   *  IF the shell server process is not running yet,
   *  THEN try to start it.
   */
  if (server_id == NULLPROCESS)
    {
      server_id = p2fopen (&server_pair, def_args);
      if (server_id > 0)
        server_setup ();
    }

  /*
   *  IF it is still not running,
   *  THEN return the nil string.
   */
  if (server_id <= 0)
    {
      char *pz = (char *) malloc (1);
      
      if (pz != (char *) NULL)
        *pz = '\0';
      return pz;
    }

  /*
   *  Make sure the process will pay attention to us,
   *  send the supplied command, and then
   *  have it output a special marker that we can find.
   */
  fprintf (server_pair.pfWrite, "\\cd %s\n%s\n\necho\necho %s\n",
           p_cur_dir, pz_cmd, z_done);
  fflush (server_pair.pfWrite);

  /*
   *  IF the server died and we received a SIGPIPE,
   *  THEN return an empty string.
   */
  if (server_id == NULLPROCESS)
    {
      char *pz = (char *) malloc (1);
      
      if (pz != (char *) NULL)
        *pz = '\0';
      return pz;
    }

  /*
   *  Now try to read back all the data.  If we fail due to either
   *  a sigpipe or sigalrm (timeout), we will return the nil string.
   */
  {
    char *pz = load_data (server_pair.pfRead);
    
    if (pz == (char *) NULL)
      {
        fprintf (stderr, "CLOSING SHELL SERVER - command failure:\n\t%s\n",
                 pz_cmd);
        close_server ();
        pz = (char *) malloc (1);
        if (pz != (char *) NULL)
          *pz = '\0';
      }
    return pz;
  }
}
