/* JSON-RPC server implementation.
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
#include "json.h"
#include "http-server.h"
#include "json-rpc.h"
#include "selftest.h"

/* Create a JSON-RPC 2.0 response object, setting "id" to a copy
   of ID if it is non-NULL.  */

json::object *
jsonrpc::make_response (const json::value *id)
{
  json::object *response = new json::object ();
  response->set ("jsonrpc", new json::string ("2.0"));
  if (id)
    response->set ("id", id->clone ());
  return response;
}

/* Create a JSON-RPC 2.0 error object, setting:
   - "id" to a copy of ID if it is non-NULL,
   - error.code to CODE,
   - error.message to MESSAGE.  */

json::object *
jsonrpc::make_failure (int code, const char *message, const json::value *id)
{
  json::object *failure = make_response (id);
  json::object *error = new json::object ();
  failure->set ("error", error);

  error->set ("code", new json::number (code));
  error->set ("message", new json::string (message));
  return failure;
}

/* Create a JSON-RPC 2.0 "parse error" object with error.message as MSG.  */

json::value *
jsonrpc::make_parse_error (const char *msg)
{
  return make_failure (PARSE_ERROR, msg, NULL);
}

/* Create a JSON-RPC 2.0 "invalid request" error object with the given ID.  */

json::value *
jsonrpc::make_invalid_request (const json::value *id)
{
  return make_failure (INVALID_REQUEST, "invalid request", id);
}

/* Create a JSON-RPC 2.0 "method not found" error object for a method named
   METHOD.  */

json::value *
jsonrpc::make_method_not_found (const json::value *id, const char *method)
{
  const char *msg = ACONCAT (("method not found: ", method, NULL));
  return make_failure (METHOD_NOT_FOUND, msg, id);
}

/* Create a JSON-RPC 2.0 "invalid params" error object with error.message as MSG
   if non-NULL, or "invalid params" otherwise.  */

json::value *
jsonrpc::make_invalid_params (const json::value *id, const char *msg)
{
  if (msg == NULL)
    msg = "invalid params";
  return make_failure (INVALID_PARAMS, msg, id);
}

/* Create a JSON-RPC 2.0 response object for request ID, with result
   as RESULT, taking ownership of RESULT.  */

json::value *
jsonrpc::make_success (const json::value *id, json::value *result)
{
  json::object *success = make_response (id);
  success->set ("result", result);
  return success;
}

/* Public interface for handling JSON-RPC requests.
   Takes a borrowed pointer to REQUEST.
   Returns a new pointer to a result object.
   Wrapper around handle_request_1, adding support for dumping.  */

json::value *
jsonrpc::server::handle_request (json::value *request)
{
  if (m_verbose)
    {
      fprintf (stderr, "request: ");
      request->dump (stderr);
      fprintf (stderr, "\n");
    }

  json::value *response = handle_request_1 (request);

  if (m_verbose)
    {
      fprintf (stderr, "response: ");
      if (response)
	response->dump (stderr);
      else
	fprintf (stderr, "NULL");
      fprintf (stderr, "\n");
    }

  return response;
}

/* Private subroutine of handle_request.
   Takes a borrowed pointer to REQUEST.
   Returns a new pointer to a result object.
   Performs various error-checking, then calls the "dispatch" vfunc.  */

json::value *
jsonrpc::server::handle_request_1 (json::value *request)
{
  // FIXME: handle batch requests
  if (request->get_kind () != json::JSON_OBJECT)
    return make_parse_error ("not an object");

  json::object *reqobj = static_cast <json::object *> (request);

  /* Lookup id.  */
  json::value *id = reqobj->get ("id");

  /* If no id, then it's a notification.  */

  /* TODO: validate non-NULL id.  */

  /* Check version.  */
  json::value *version = reqobj->get ("jsonrpc");
  if (!version)
    return make_invalid_request (id);
  const json::string *version_string = version->as_string ();
  if (!version_string)
    return make_failure (INVALID_REQUEST, "jsonrpc was not 2.0", id);
  if (0 != strcmp (version_string->get_string (), "2.0"))
    return make_failure (INVALID_REQUEST, "jsonrpc was not 2.0", id);

  json::value *method = reqobj->get ("method");
  if (!method)
    return make_failure (INVALID_REQUEST, "no method", id);
  const json::string *method_str = method->as_string ();
  if (!method_str)
    return make_failure (INVALID_REQUEST, "method was not a string", id);

  const json::value *params = reqobj->get ("params");
  return dispatch (method_str->get_string (), params, id);
}

/* Parse the given buffer as JSON-RPC and attempt to dispatch the call.
   Returns a new pointer to a result object.  */

json::value *
jsonrpc::server::handle_request (size_t sz, const char *buf)
{
  char *err = NULL;
  json::value *request = json::parse_utf8_string (sz, buf, &err);
  if (!request)
    {
      gcc_assert (err);
      json::value *response = make_parse_error (err);
      free (err);
      return response;
    }
  gcc_assert (err == NULL);
  json::value *response = handle_request (request);
  delete request;
  return response;
}

/* Parse the given UTF-8 encoded 0-terminated string as JSON-RPC and
   attempt to dispatch the call.
   Returns a new pointer to a result object.

   Helper function for writing selftests.  */

json::value *
jsonrpc::server::handle_request_string (const char *utf8)
{
  return handle_request (strlen (utf8), utf8);
}

/* class ::jsonrpc::http_server : public ::http::server.  */

/* jsonrpc::http_server's ctor.  */

jsonrpc::http_server::http_server (jsonrpc::server &json_handler)
: ::http::server (true), m_json_handler (json_handler)
{}

/* Handle HTTP requests by parsing the content as JSON-RPC, and delegating
   them to a ::jsonrpc::server.  */

void
jsonrpc::http_server::on_http_request (const http::request &http_request,
				       http::response &http_response)
{
  size_t length = http_request.get_content_length ();
  const char *buf = http_request.get_content ();
  if (0)
    fprintf (stderr, "got content: '%.*s'\n", (int)length, buf);
  json::value *json_response = m_json_handler.handle_request (length, buf);
  char *response_str = json_response->to_str ();
  delete json_response;
  http_response.set_content (strlen (response_str), response_str);
  free (response_str);
}

#if CHECKING_P

namespace selftest {

/* Selftests.  */

/* Assert that RESPONSE is a non-NULL response for ID.
   Verify that the jsonrpc value is "2.0".
   Return RESPONSE, cast to an object *.  */

static const json::object *
assert_is_response (const json::value *response, int id)
{
  ASSERT_TRUE (response != NULL);
  const json::object *obj = response->as_object ();
  ASSERT_TRUE (obj != NULL);
  json::value *jsonrpc = obj->get ("jsonrpc");
  ASSERT_TRUE (jsonrpc != NULL);
  ASSERT_STREQ ("2.0", jsonrpc->as_string ()->get_string ());
  ASSERT_EQ (id, obj->get ("id")->as_number ()->get ());
  return obj;
}

/* Assert that RESPONSE is a non-NULL response, with no ID.
   Verify that the jsonrpc value is "2.0".
   Return RESPONSE, cast to an object *.  */

static const json::object *
assert_is_response (const json::value *response)
{
  ASSERT_TRUE (response != NULL);
  const json::object *obj = response->as_object ();
  ASSERT_TRUE (obj != NULL);
  json::value *jsonrpc = obj->get ("jsonrpc");
  ASSERT_TRUE (jsonrpc != NULL);
  ASSERT_STREQ ("2.0", jsonrpc->as_string ()->get_string ());
  return obj;
}

/* Assert that RESPONSE is a non-NULL successful response for ID.
   Verify that the jsonrpc value is "2.0".
   Return the "result" value.  */

static const json::value *
assert_is_success (const json::value *response, int id)
{
  const json::object *obj = assert_is_response (response, id);
  ASSERT_EQ (NULL, obj->get ("error"));
  return obj->get ("result");
}

/* Assert that RESPONSE is a non-NULL failure response for ID.
   Verify that the jsonrpc value is "2.0".
   Verify that CODE and MESSAGE match the given values.  */

static void
assert_is_failure (const json::value *response, int code, const char *message,
		   int id)
{
  const json::object *obj = assert_is_response (response, id);
  ASSERT_EQ (NULL, obj->get ("result"));
  const json::object *err = obj->get ("error")->as_object ();
  ASSERT_EQ (code, err->get ("code")->as_number ()->get ());
  ASSERT_STREQ (message, err->get ("message")->as_string ()->get_string ());
}

/* Assert that RESPONSE is a non-NULL failure response, with no ID.
   Verify that the jsonrpc value is "2.0".
   Verify that CODE and MESSAGE match the given values.  */

static void
assert_is_failure (const json::value *response, int code, const char *message)
{
  const json::object *obj = assert_is_response (response);
  ASSERT_EQ (NULL, obj->get ("result"));
  const json::object *err = obj->get ("error")->as_object ();
  ASSERT_EQ (code, err->get ("code")->as_number ()->get ());
  ASSERT_STREQ (message, err->get ("message")->as_string ()->get_string ());
}

using namespace jsonrpc;

namespace {

/* A concrete subclass of jsonrpc::server for use by the selftests.  */

class test_server : public jsonrpc::server
{
 public:
  test_server (bool verbose) : server (verbose) {}

  json::value *
  dispatch (const char *method, const json::value *params,
	    const json::value *id) FINAL OVERRIDE
  {
    if (0 == strcmp (method, "subtract"))
      return do_subtract (params, id);
    return make_method_not_found (id, method);
  }

  json::value *
  do_subtract (const json::value *params, const json::value *id)
  {
    const json::array *positional = params->as_array ();
    if (!positional)
      return make_invalid_params (id, "params was not an array");
    if (positional->get_length () != 2)
      return make_invalid_params (id, "length of params was not 2");

    const json::number *arg0 = positional->get (0)->as_number ();
    if (!arg0)
      return make_invalid_params (id, "param 0 was not a number");
    const json::number *arg1 = positional->get (1)->as_number ();
    if (!arg1)
      return make_invalid_params (id, "param 1 was not a number");

    double result = arg0->get () - arg1->get ();

    return make_success (id, new json::number (result));
  }
};

} // anonymous namespace

/* Verify that a simple JSON-RPC call (of "subtract") works.  */

static void
test_simple ()
{
  test_server s (false);
  const char *request
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"subtract\","
       " \"params\": [42, 23], \"id\": 1}");
  json::value *response = s.handle_request_string (request);
  const json::value *result = assert_is_success (response, 1);
  ASSERT_EQ (19, result->as_number ()->get ());
  delete response;
}

/* Verify that jsonrpc::server::make_invalid_params works.  */

static void
test_bad_params ()
{
  test_server s (false);
  const char *request
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"subtract\","
       " \"params\": [42, \"0\"], \"id\": 1}");
  json::value *response = s.handle_request_string (request);
  assert_is_failure (response, INVALID_PARAMS, "param 1 was not a number", 1);
  delete response;
}

/* Verify that jsonrpc::server enforces JSON-RPC 2.0.  */

static void
test_bad_version ()
{
  test_server s (false);
  const char *request
    = ("{\"jsonrpc\": \"1.0\", \"method\": \"subtract\","
       " \"params\": [42, 23], \"id\": 1}");
  json::value *response = s.handle_request_string (request);
  assert_is_failure (response, INVALID_REQUEST, "jsonrpc was not 2.0", 1);
  delete response;
}

/* Verify that jsonrpc::server handles non-string methods.  */

static void
test_method_not_a_string ()
{
  test_server s (false);
  const char *request
    = ("{\"jsonrpc\": \"2.0\", \"method\": 1,"
       " \"params\": \"bar\", \"id\": 1}");
  json::value *response = s.handle_request_string (request);
  assert_is_failure (response, INVALID_REQUEST, "method was not a string", 1);
  delete response;
}

/* Verify that jsonrpc::server::make_method_not_found works.  */

static void
test_method_not_found ()
{
  test_server s (false);
  const char *request
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"does_not_exist\","
       " \"params\": [42, 23], \"id\": 1}");
  json::value *response = s.handle_request_string (request);
  assert_is_failure (response, METHOD_NOT_FOUND,
    "method not found: does_not_exist", 1);
  delete response;
}

/* Verify that jsonrpc::server::handle_request_string gracefully
   handles malformed JSON.  */

static void
test_malformed_json ()
{
  test_server s (false);
  const char *request = "{";
  json::value *response = s.handle_request_string (request);
  assert_is_failure (response, PARSE_ERROR,
    "error at index 1: expected string for object key");
  delete response;
}

/* Verify that jsonrpc::http_server can handle JSON-RPC within HTTP,
   delegating to a jsonrpc::server.  */

static void
test_http_server ()
{
  http::request req;
  const char *in = ("POST /jsonrpc HTTP/1.1\r\n"
		    "Content-Length: 69\r\n"
		    "\r\n"
		    "{\"jsonrpc\": \"2.0\", \"method\": \"subtract\","
		    " \"params\": [42, 23], \"id\": 1}");
  size_t consumed = req.parse_buffer (strlen (in), in);
  ASSERT_EQ (strlen (in), consumed);

  test_server s (false);
  http_server http_server (s);
  http::response resp;
  http_server.on_http_request (req, resp);
  ASSERT_STREQ ("{\"jsonrpc\": \"2.0\", \"id\": 1, \"result\": 19}",
		resp.get_content ()); // FIXME: length?
}

/* Run all of the selftests within this file.  */

void
json_rpc_cc_tests ()
{
  test_simple ();
  test_bad_params ();
  test_bad_version ();
  test_method_not_a_string ();
  test_method_not_found ();
  test_malformed_json ();
  test_http_server ();
}

} // namespace selftest

#endif /* #if CHECKING_P */
