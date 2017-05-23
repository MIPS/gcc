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

#ifndef GCC_LSP_H
#define GCC_LSP_H

namespace lsp {

typedef const char *DocumentUri;

/* Interfaces from the protocol specification (which uses camel case).  */

/* Note that LSP uses 0-based lines and characters, whereas GCC uses
   1-based lines and columns.  */

struct Position
{
  Position ()
  : line (0), character (0) {}

  static Position from_json (const json::value *params,
			     char *&out_err);
  json::value *to_json () const;

  int line;
  int character;
};

struct Range
{
  Range ()
  : start (), end () {}

  json::value *to_json () const;

  Position start;
  Position end;
};

struct Location
{
  Location ()
  : uri (NULL), range () {}

  json::value *to_json () const;

  DocumentUri uri;
  Range range;
};

// Exceptions would be nicer than passing around the out_err

// TODO: autogenerate the interface binding/marshalling/demarshalling code
// from an interface description.

struct TextDocumentIdentifier
{
  TextDocumentIdentifier ()
  : uri (NULL) {}

  static TextDocumentIdentifier from_json (const json::value *params,
					   char *&out_err);

  DocumentUri uri;
};

struct TextDocumentItem
{
  TextDocumentItem ()
  : uri (NULL), languageId (NULL), version (0), text (NULL)
  {}

  static TextDocumentItem from_json (const json::value *params,
				     char *&out_err);

  DocumentUri uri;
  const char *languageId;
  int version;
  const char *text;
};

struct DidOpenTextDocumentParams
{
  DidOpenTextDocumentParams ()
  : textDocument () {}

  static DidOpenTextDocumentParams from_json (const json::value *params,
					      char *&out_err);

  TextDocumentItem textDocument;
};

struct DidChangeTextDocumentParams
{
 public:
  static DidChangeTextDocumentParams from_json (const json::value *params,
						char *&out_err);

 private:
#if 0
  VersionedTextDocumentIdentifier textDocument;
  auto_vec<TextDocumentContentChangeEvent> contentChanges;
#endif
};

struct TextDocumentPositionParams
{
  TextDocumentPositionParams ()
  : textDocument (), position () {}

  static TextDocumentPositionParams from_json (const json::value *params,
					       char *&out_err);

  TextDocumentIdentifier textDocument;
  Position position;
};

/* An abstract base class for implementing the LSP as vfunc calls,
   avoiding dealing with JSON.  */

class server
{
 public:
  virtual ~server () {}

  virtual void
  do_text_document_did_open (const DidOpenTextDocumentParams &p) = 0;

  virtual void
  do_text_document_did_change (const DidChangeTextDocumentParams &p) = 0;

  virtual void
  do_text_document_definition (const TextDocumentPositionParams &p,
			       vec<Location> &out) = 0;
};

/* A concrete subclass of lsp::server that implements everything as a no-op.  */

class noop_server : public server
{
  void
  do_text_document_did_open (const DidOpenTextDocumentParams &) OVERRIDE
  {
    // no-op
  }

  void
  do_text_document_did_change (const DidChangeTextDocumentParams &) OVERRIDE
  {
    // no-op
  }

  void
  do_text_document_definition (const TextDocumentPositionParams &,
			       vec<Location> &) OVERRIDE
  {
    // no-op
  }
};

/* A jsonrpc::server subclass that decodes incoming JSON-RPC requests
   and dispatches them to an lsp::server instance as vfunc calls,
   marshalling the inputs/outputs to/from JSON objects.  */

class jsonrpc_server : public ::jsonrpc::server
{
 public:
  jsonrpc_server (bool verbose, ::lsp::server &inner)
  : server (verbose), m_inner (inner) {}

  json::value *
  dispatch (const char *method, const json::value *params,
	    const json::value *id) FINAL OVERRIDE;

 private:
  json::value *
  do_initialize (const json::value *id, const json::value *params);

  json::value *
  do_text_document_did_open (const json::value *params);

  json::value *
  do_text_document_did_change (const json::value *params);

  json::value *
  do_text_document_definition (const json::value *params);

 private:
  ::lsp::server &m_inner;
};

} // namespace lsp

#endif  /* GCC_LSP_H  */
