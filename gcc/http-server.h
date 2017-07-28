/* HTTP server implementation.
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

#ifndef GCC_HTTP_SERVER_H
#define GCC_HTTP_SERVER_H

#include "server.h"

namespace http {

/* http::message is a base class for encapsulating a collection of
   octets received or to be sent.  */

class message
{
 public:
  message () : m_length (0), m_buffer (NULL) {}
  ~message () { free (m_buffer); }

  size_t get_content_length () const { return m_length; }
  const char *get_content () const { return m_buffer; }

  void set_content (size_t length, const char *content);

 private:
  size_t m_length;
  char *m_buffer;
};

/* http::request encapsulates an HTTP request.  */

class request : public message
{
 public:
  request ();
  ~request();

  const char *get_header (const char *header) const;
  size_t parse_buffer (size_t length, const char *buf);

 private:
  bool consume_octet (char ch);
  bool consume_header_octet (char ch);
  bool consume_body_octet (char ch);
  void parse_header (size_t sz, const char *buf);

  bool m_parsing_body;
  auto_vec<char> m_pending_data;
  size_t m_content_length;

  typedef hash_map <char *, char *,
    simple_hashmap_traits<nofree_string_hash, char *> > header_map_t;
  header_map_t m_header_map;
};

/* http::response encapsulates an HTTP response.  */

class response : public message
{
 public:
  char *to_str () const;
};

/* Subclass of ::server than expects an HTTP-like protocol,
   with header lines ended by '\r\n', then a '\r\n' line, then
   the content.  */

class server : public ::server
{
 public:
  server (bool verbose) : m_verbose (verbose) {}

  void on_read (file_descriptor fd, size_t length,
		const char *buf) OVERRIDE FINAL;

  virtual void on_http_request (const http::request &request,
				http::response &response) = 0;

 private:
  bool m_verbose;
};

} // namespace http

#endif  /* GCC_HTTP_SERVER_H  */
