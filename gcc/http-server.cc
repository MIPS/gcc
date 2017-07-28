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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "http-server.h"
#include "pretty-print.h"
#include "selftest.h"
#include "diagnostic.h"

/* class http::message.  */

/* Set the content of this message.  */

void
http::message::set_content (size_t length, const char *content)
{
  m_length = length;
  m_buffer = xstrndup (content, length);
}

/* class http::request : public http::message.  */

/* http::request's constructor.  */

http::request::request ()
: message (), m_parsing_body (false), m_pending_data (),
  m_content_length (-1), m_header_map ()
{
}

/* http::request's destructor.  */

http::request::~request()
{
  for (header_map_t::iterator it = m_header_map.begin ();
       it != m_header_map.end (); ++it)
    {
      free (const_cast <char *>((*it).first));
      free (const_cast <char *>((*it).second));
    }
}

/* Access the given header within this request, or NULL if not present.  */

const char *
http::request::get_header (const char *header) const
{
  char **slot = const_cast <request*> (this)->m_header_map.get (header);
  if (slot)
    return *slot;
  return NULL;
}

/* Consume up to LENGTH bytes from BUF.
   Return the number of bytes consumed.  */

size_t
http::request::parse_buffer (size_t length, const char *buf)
{
  size_t idx;
  for (idx = 0; idx < length; idx++)
    {
      if (consume_octet (buf[idx]))
	return idx + 1;
    }
  return idx;
}

/* Parse CH.  Return true if the request has finished parsing, or false
   if more data is expected.  */

bool
http::request::consume_octet (char ch)
{
  if (m_parsing_body)
    return consume_body_octet (ch);
  else
    return consume_header_octet (ch);
}

/* Parse CH within the headers.  Return true if the request has finished
   parsing, or false if more data is expected.  */

bool
http::request::consume_header_octet (char ch)
{
  /* We're parsing the headers.  */
  m_pending_data.safe_push (ch);
  /* Detect "\r\n".  */
  size_t len = m_pending_data.length ();
  if (len >= 2 && m_pending_data[len - 2] == '\r' && ch == '\n')
    {
      /* Is this a blank line?  If so, then we're done with headers.  */
      if (len == 2)
	{
	  m_pending_data.truncate (0);
	  const char *content_length_str = get_header ("Content-Length");
	  if (content_length_str)
	    {
	      m_content_length = atoi (content_length_str);
	      // FIXME: error-handling for non-int values
	      /* We're not yet finished; we must read the body.  */
	      m_parsing_body = true;
	      return false;
	    }
	  else
	    /* If there was no Content-Length, we have nothing
	       more to read.  */
	    return true;
	}
      else
	{
	  /* Otherwise we have a header (or the initial verb line).  */
	  parse_header (m_pending_data.length () - 2, &m_pending_data[0]);
	  m_pending_data.truncate (0);
	  /* We're not yet finished.  */
	  return false;
	}
    }

  /* Not yet finished.  */
  return false;
}

/* Parse CH within the body.  Return true if the request has finished
   parsing, or false if more data is expected.  */

bool
http::request::consume_body_octet (char ch)
{
  /* Accumulate data until we've seen Content-Length octets.  */
  gcc_assert (m_content_length > 0);
  m_pending_data.safe_push (ch);
  if (m_pending_data.length () == m_content_length)
    {
      set_content (m_content_length, &m_pending_data[0]);
      m_pending_data.truncate (0);
      /* We've finished parsing this request.  */
      return true;
    }
  else
    /* We're not yet finished.  */
    return false;
}

/* FIXME.  */

void
http::request::parse_header (size_t length, const char *buf)
{
  /* header-field   = field-name ":" OWS field-value OWS  */
  for (size_t colon_idx = 0; colon_idx + 1 < length; colon_idx++)
    // FIXME: whitespace after colon is optional
    // FIXME: optional trailing whitespace after header value
    if (buf[colon_idx] == ':' && buf[colon_idx + 1] == ' ')
      {
	char *key = xstrndup (buf, colon_idx);
	char *value = xstrndup (buf + colon_idx + 2,
				length - (colon_idx + 2));
	m_header_map.put (key, value);
	return;
      }
  // FIXME: error-handling
}

/* class http::response : public http::message.  */

/* Generate a string form of this response.
   The caller is responsible for freeing it.  */

char *
http::response::to_str () const
{
  pretty_printer pp;
  pp_string (&pp, "HTTP/1.1 200 OK\r\n");
  pp_printf (&pp, "Content-Length: %i\r\n", (int)get_content_length ());
  pp_string (&pp, "\r\n");
  if (get_content ())
    pp_string (&pp, get_content ());
  return xstrdup (pp_formatted_text (&pp));
}

/* Implementation of ::server::on_read for http::server.
   Read up to LENGTH bytes from BUF, and potentially call
   on_http_request for any requests seen.
   Write any responses back to FD.  */

void
http::server::on_read (file_descriptor fd, size_t length, const char *buf)
{
  if (m_verbose)
    inform (UNKNOWN_LOCATION, "received http request: %qs",
	    buf); // FIXME respect length

  /* FIXME: this assumes we have a full request i.e. two "\r\n\r\n"
     If we don't we should read more until we do.  */
  size_t req_start = 0;
  while (req_start < length)
    {
      request req;
      req_start += req.parse_buffer (length, buf);
      http::response resp;
      on_http_request (req, resp);
      char *resp_str = resp.to_str ();
      if (1)
	inform (UNKNOWN_LOCATION, "sending http response: %qs",
		resp_str);
      write (fd.m_fd, resp_str, strlen (resp_str));
      free (resp_str);
    }
}

#if CHECKING_P

namespace selftest {

/* Selftests.  */

/* Verify that we can parse an HTTP request.  */

static void
test_parse_request ()
{
  const char *in = ("POST /jsonrpc HTTP/1.1\r\n"
		    "Host: localhost:4000\r\n"
		    "Content-Length: 12\r\n"
		    "content-type: application/json\r\n"
		    "Accept-Encoding: gzip, deflate, compress\r\n"
		    "Accept: */*\r\n"
		    "User-Agent: test-user-agent\r\n"
		    "\r\n"
		    "test-content");
  http::request r;
  size_t consumed = r.parse_buffer (strlen (in), in);
  ASSERT_EQ (consumed, strlen (in));
  ASSERT_STREQ ("test-user-agent", r.get_header ("User-Agent"));
  ASSERT_STREQ ("12", r.get_header ("Content-Length"));
  ASSERT_EQ (NULL, r.get_header ("Not-A-Header"));
  ASSERT_EQ (12, r.get_content_length ());
  ASSERT_TRUE (0 == strncmp ("test-content", r.get_content (), 12));
}

/* Verify that we can split up the parsing of a request at arbitrary
   places.  */

static void
test_parse_split_request ()
{
  const char *in = ("POST /jsonrpc HTTP/1.1\r\n"
		    "Host: localhost:4000\r\n"
		    "Content-Length: 12\r\n"
		    "content-type: application/json\r\n"
		    "Accept-Encoding: gzip, deflate, compress\r\n"
		    "Accept: */*\r\n"
		    "User-Agent: test-user-agent\r\n"
		    "\r\n"
		    "test-content");
  for (size_t split = 0; split < strlen (in); split++)
    {
      http::request r;

      size_t consumed_1 = r.parse_buffer (split, in);
      ASSERT_EQ (consumed_1, split);

      size_t consumed_2 = r.parse_buffer (strlen (in) - split, in + split);
      ASSERT_EQ (consumed_2, strlen (in) - split);

      ASSERT_STREQ ("test-user-agent", r.get_header ("User-Agent"));
      ASSERT_STREQ ("12", r.get_header ("Content-Length"));
      ASSERT_EQ (NULL, r.get_header ("Not-A-Header"));
      ASSERT_EQ (12, r.get_content_length ());
      ASSERT_TRUE (0 == strncmp ("test-content", r.get_content (), 12));
    }
}

/* Verify that we can parse multiple requests out of one buffer,
   honoring the Content-Length headers.  */

static void
test_parse_multiple_requests ()
{
  const char *in = ("POST /test HTTP/1.1\r\n"
		    "Content-Length: 25\r\n"
		    "\r\n"
		    "This is the first request"
		    "POST /test HTTP/1.1\r\n"
		    "Content-Length: 26\r\n"
		    "\r\n"
		    "This is the second request");
  http::request r1;
  size_t consumed_1 = r1.parse_buffer (strlen (in), in);
  ASSERT_EQ (68, consumed_1);
  ASSERT_STREQ ("25", r1.get_header ("Content-Length"));
  ASSERT_EQ (25, r1.get_content_length ());
  ASSERT_TRUE (0 == strncmp ("This is the first request",
			     r1.get_content (), 25));

  http::request r2;
  size_t consumed_2 = r2.parse_buffer (strlen (in) - consumed_1,
				       in + consumed_1);
  ASSERT_EQ (69, consumed_2);
  ASSERT_STREQ ("26", r2.get_header ("Content-Length"));
  ASSERT_EQ (26, r2.get_content_length ());
  ASSERT_TRUE (0 == strncmp ("This is the second request",
			     r2.get_content (), 26));

  ASSERT_EQ (strlen (in), consumed_1 + consumed_2);
}

/* Verify http::response::to_str.  */

static void
test_emit_response ()
{
  http::response r;
  const char *msg = "hello world";
  r.set_content (strlen (msg), msg);

  char *str = r.to_str ();
  ASSERT_STREQ ("HTTP/1.1 200 OK\r\n"
		"Content-Length: 11\r\n"
		"\r\n"
		"hello world", str);
  free (str);
}

/* Run all of the selftests within this file.  */

void
http_server_cc_tests ()
{
  test_parse_request ();
  test_parse_split_request ();
  test_parse_multiple_requests ();
  test_emit_response ();
}

} // namespace selftest

#endif /* #if CHECKING_P */
