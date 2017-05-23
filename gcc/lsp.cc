/* Language Server Protocol implementation.
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
#include "lsp.h"
#include "selftest.h"

using namespace jsonrpc;
using namespace lsp;

// TODO: autogenerate the interface binding/marshalling/demarshalling code
// from an interface description.

#define SET_VALUE(LHS, VALUE, NAME)				\
  do {								\
    if (!(VALUE)->get_value_by_key ((NAME), (LHS), out_err))	\
      return result;						\
  } while (0)

#define SET_NUMBER(LHS, VALUE, NAME)				\
  do {								\
    if (!(VALUE)->get_int_by_key ((NAME), (LHS), out_err))	\
      return result;						\
  } while (0)

#define SET_STRING(LHS, VALUE, NAME)				\
  do {								\
    if (!(VALUE)->get_string_by_key ((NAME), (LHS), out_err))	\
      return result;						\
  } while (0)

Position
Position::from_json (const json::value *params,
		     char *&out_err)
{
  Position result;
  SET_NUMBER (result.line, params, "line");
  SET_NUMBER (result.character, params, "character");
  return result;
}

TextDocumentIdentifier
TextDocumentIdentifier::from_json (const json::value *params,
				   char *&out_err)
{
  TextDocumentIdentifier result;
  SET_STRING (result.uri, params, "uri");
  return result;
}

TextDocumentItem
TextDocumentItem::from_json (const json::value *params,
			     char *&out_err)
{
  TextDocumentItem result;
  SET_STRING (result.uri, params, "uri");
  SET_STRING (result.languageId, params, "languageId");
  SET_NUMBER (result.version, params, "version");
  SET_STRING (result.text, params, "text");
  return result;
}

DidOpenTextDocumentParams
DidOpenTextDocumentParams::from_json (const json::value *params,
				      char *&out_err)
{
  DidOpenTextDocumentParams result;
  // FIXME: error-handling
  const json::value *text_document;
  SET_VALUE (text_document, params, "textDocument");
  result.textDocument
    = TextDocumentItem::from_json (text_document, out_err);
  return result;
}

DidChangeTextDocumentParams
DidChangeTextDocumentParams::from_json (const json::value */*params*/,
					char *&/*out_err*/)
{
  DidChangeTextDocumentParams result;

  // FIXME
  return result;
}

TextDocumentPositionParams
TextDocumentPositionParams::from_json (const json::value *params,
				       char *&out_err)
{
  TextDocumentPositionParams result;
  const json::value *text_document;
  const json::value *position;
  SET_VALUE (text_document, params, "textDocument");
  SET_VALUE (position, params, "position");
  result.textDocument
    = TextDocumentIdentifier::from_json (text_document, out_err);
  result.position
    = Position::from_json (position, out_err);
  return result;
}

/* struct Position.  */

json::value *
Position::to_json () const
{
  json::object *result = new json::object ();
  result->set ("line", new json::number (line));
  result->set ("character", new json::number (character));
  return result;
}

/* struct Range.  */

json::value *
Range::to_json () const
{
  json::object *result = new json::object ();
  result->set ("start", start.to_json ());
  result->set ("end", end.to_json ());
  return result;
}

/* struct Location.  */

json::value *
Location::to_json () const
{
  json::object *result = new json::object ();
  result->set ("uri", new json::string (uri));
  result->set ("range", range.to_json ());
  return result;
}

/* class lsp::jsonrpc_server : public ::jsonrpc::server.  */

json::value *
lsp::jsonrpc_server::dispatch (const char *method, const json::value *params,
			       const json::value *id)
{
  if (0 == strcmp (method, "initialize"))
    return do_initialize (id, params);
  if (0 == strcmp (method, "textDocument/didOpen"))
    return do_text_document_did_open (params);
  if (0 == strcmp (method, "textDocument/didChange"))
    return do_text_document_did_change (params);
  if (0 == strcmp (method, "textDocument/definition"))
    return do_text_document_definition (params);
  return make_method_not_found (id, method);
}

json::value *
lsp::jsonrpc_server::do_initialize (const json::value *id,
				    const json::value */*params*/)
{
  // FIXME: for now, ignore params

  json::object *server_caps = new json::object ();
  json::object *result = new json::object ();
  result->set ("capabilities", server_caps);
  return make_success (id, result);
}

static json::value *
make_invalid_params_and_free_msg (const json::value *id, char *msg)
{
  json::value *err = make_invalid_params (id, msg);
  free (msg);
  return err;
}

json::value *
lsp::jsonrpc_server::do_text_document_did_open (const json::value *params)
{
  char *err = NULL;
  DidOpenTextDocumentParams p
    = DidOpenTextDocumentParams::from_json (params, err);
  if (err)
    return make_invalid_params_and_free_msg (NULL, err); // though we ought not to return non-NULL for a notification

  m_inner.do_text_document_did_open (p);
  return NULL; // notification, so no response
}

json::value *
lsp::jsonrpc_server::do_text_document_did_change (const json::value *params)
{
  char *err = NULL;
  DidChangeTextDocumentParams p
    = DidChangeTextDocumentParams::from_json (params, err);
  if (err)
    return make_invalid_params_and_free_msg (NULL, err); // though we ought not to return non-NULL for a notification

  m_inner.do_text_document_did_change (p);

  return NULL; // notification, so no response
}

json::value *
lsp::jsonrpc_server::do_text_document_definition (const json::value *params)
{
  char *err = NULL;
  TextDocumentPositionParams p
    = TextDocumentPositionParams::from_json (params, err);
  if (err)
    return make_invalid_params_and_free_msg (NULL, err);

  auto_vec<Location> out;
  m_inner.do_text_document_definition (p, out);

  json::array *result = new json::array ();
  unsigned i;
  Location *loc;
  FOR_EACH_VEC_ELT (out, i, loc)
    result->append (loc->to_json ());
  return result;
}

#if CHECKING_P

namespace selftest {

/* Selftests.  */

static void
test_simple ()
{
  noop_server noop;
  lsp::jsonrpc_server js (false, noop);
  const char *init_request
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"initialize\","
       " \"params\": [42, 23], \"id\": 1}"); // FIXME
  json::value *init_response = js.handle_request_string (init_request);
  //const json::value *result = assert_is_success (response, 1);
  //ASSERT_EQ (19, result->as_number ()->get ());
  delete init_response;

  const char *did_open_note
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"textDocument/didOpen\","
       " \"params\": {"
       " \"textDocument\": "
       " { \"uri\": \"DocumentUri goes here\","
         " \"languageId\": \"c++\","
         " \"version\": 0,"
         " \"text\": \"/* Initial content.  */\"}}}");
  json::value *did_open_response = js.handle_request_string (did_open_note);
  delete did_open_response;

  const char *did_change_note
    = ("{\"jsonrpc\": \"2.0\", \"method\": \"textDocument/didChange\","
       " \"params\": {"
         " \"textDocument\": {\"version\": 1},"
         " \"contentChanges\": [{\"text\": \"/* Hello world.  */\"}]"
       "}}"); // FIXME
  json::value *did_change_response = js.handle_request_string (did_change_note);
  delete did_change_response;
}


/* Run all of the selftests within this file.  */

void
lsp_cc_tests ()
{
  test_simple ();
}

} // namespace selftest

#endif /* #if CHECKING_P */
