/* JSON-RPC.
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

#ifndef GCC_JSON_RPC_H
#define GCC_JSON_RPC_H

namespace jsonrpc
{

/* jsonrpc::server is an abstract base class for implementing
   a JSON-RPC 2.0 server.  */

class server
{
 public:
  server (bool verbose) : m_verbose (verbose) {}
  virtual ~server () {}

  json::value *handle_request (json::value *);
  json::value *handle_request (size_t sz, const char *buf);
  json::value *handle_request_string (const char *utf8);

  virtual json::value *dispatch (const char *method,
				 const json::value *params,
				 const json::value *id) = 0;

 private:
  json::value *handle_request_1 (json::value *);

  bool m_verbose;
};

/* The standard JSON-RPC error codes.  */

const int PARSE_ERROR = -32700;
const int INVALID_REQUEST = -32600;
const int METHOD_NOT_FOUND = -32601;
const int INVALID_PARAMS = -32602;
const int INTERNAL_ERROR = -32603;

/* Helper functions for creating responses.  */

extern json::object *make_response (const json::value *id);
extern json::object *make_failure (int code, const char *message,
				   const json::value *id);

extern json::value *make_parse_error (const char *msg);

extern json::value *make_invalid_request (const json::value *id);

extern json::value *make_method_not_found (const json::value *id,
					   const char *method);

extern json::value *make_invalid_params (const json::value *id,
					 const char *msg);

extern json::value *make_success (const json::value *id, json::value *result);


/* A subclass of ::http::server that handles HTTP requests
   by parsing the content as JSON-RPC, and delegating them to
   a ::jsonrpc::server.  */

class http_server : public ::http::server
{
 public:
  http_server (::jsonrpc::server &json_handler);

  void on_http_request (const http::request &http_request,
			http::response &http_response) FINAL OVERRIDE;

 private:
  ::jsonrpc::server &m_json_handler;
};

} // namespace jsonrpc

#endif  /* GCC_JSON_RPC_H  */
