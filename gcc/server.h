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

#ifndef GCC_SERVER_H
#define GCC_SERVER_H

/* Wrapper aroung "int" for file descriptors.  */

struct file_descriptor
{
  explicit file_descriptor (int fd) : m_fd (fd) {}

  int m_fd;
};

/* Abstract base class for implementing a server.  */

class server
{
 public:
  virtual ~server () {}
  void serve (int port);

  virtual void on_read (file_descriptor fd, size_t length, const char *buf) = 0;

 private:
  int read_from_client (file_descriptor fd);
};

#endif  /* GCC_SERVER_H  */
