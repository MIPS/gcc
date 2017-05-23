/* Abstract server implementation.
   Copyright (C) 2017 Free Software Foundation, Inc.

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

/* adapted from GNU libc docs.  */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXMSG  512

static int
make_socket (uint16_t port)
{
  int sock;
  struct sockaddr_in name;

  /* Create the socket. */
  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
      perror ("socket");
      exit (EXIT_FAILURE);
    }

  /* Give the socket a name. */
  name.sin_family = AF_INET;
  name.sin_port = htons (port);
  name.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
      perror ("bind");
      exit (EXIT_FAILURE);
    }

  return sock;
}

/* Serve on PORT.  */

void
server::serve (int port)
{
  int sock;
  fd_set active_fd_set, read_fd_set;
  int i;
  struct sockaddr_in clientname;
  socklen_t size;

  /* Create the socket and set it up to accept connections. */
  sock = make_socket (port);
  if (listen (sock, 1) < 0)
    {
      perror ("listen");
      exit (EXIT_FAILURE);
    }

  /* Initialize the set of active sockets. */
  FD_ZERO (&active_fd_set);
  FD_SET (sock, &active_fd_set);

  while (1)
    {
      /* Block until input arrives on one or more active sockets. */
      read_fd_set = active_fd_set;
      if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
	{
	  perror ("select");
	  exit (EXIT_FAILURE);
	}

      /* Service all the sockets with input pending. */
      for (i = 0; i < FD_SETSIZE; ++i)
	if (FD_ISSET (i, &read_fd_set))
	  {
	    if (i == sock)
	      {
		/* Connection request on original socket. */
		int new_;
		size = sizeof (clientname);
		new_ = accept (sock,
			       (struct sockaddr *) &clientname,
			       &size);
		if (new_ < 0)
		  {
		    perror ("accept");
		    exit (EXIT_FAILURE);
		  }
		fprintf (stderr,
			 "Server: connect from host %s, port %hd.\n",
			 inet_ntoa (clientname.sin_addr),
			 ntohs (clientname.sin_port));
		FD_SET (new_, &active_fd_set);
	      }
	    else
	      {
		/* Data arriving on an already-connected socket. */
		if (read_from_client (file_descriptor (i)) < 0)
		  {
		    close (i);
		    FD_CLR (i, &active_fd_set);
		  }
	      }
	  }
    }
}

int
server::read_from_client (file_descriptor fd)
{
  char buffer[MAXMSG];
  int nbytes;

  nbytes = read (fd.m_fd, buffer, MAXMSG);
  if (nbytes < 0)
    {
      /* Read error. */
      perror ("read");
      exit (EXIT_FAILURE);
    }
  else if (nbytes == 0)
    /* End-of-file. */
    return -1;
  else
    {
      /* Data read. */
      on_read (fd, nbytes, buffer);
      return 0;
    }
}
