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
#include "http-server.h"
#include "json.h"
#include "json-rpc.h"
#include "lsp.h"
#include "blt.h"
#include "tree.h"

using namespace lsp;

class is_record_definition : public blt_predicate
{
 public:
  is_record_definition (tree record_type) : m_record_type (record_type) {}

  bool satisfied_by_node_p (const blt_node &node) FINAL OVERRIDE
  {
    if (node.get_kind () == BLT_STRUCT_CONTENTS
	&& node.get_tree () == m_record_type)
      return true;
    return false;
  }

 private:
  tree m_record_type;
};

static bool
Location_from_blt_node (const blt_node *node, Location &out)
{
  if (!node)
    return false;

  location_t start = node->get_start ();
  if (start == UNKNOWN_LOCATION)
    return false;
  location_t finish = node->get_finish ();
  if (finish == UNKNOWN_LOCATION)
    return false;

  expanded_location el_start = expand_location (start);
  expanded_location el_finish = expand_location (finish);
  if (el_start.file != el_finish.file)
    return false;

  out.uri = el_start.file;
  /* Convert from GCC's 1-based lines and columns to LSP's
     0-based lines and columns.
     Offset the end column back by 1 since blt_node finish
     locations are inclusive, whereas in LSP the endpoint of a Range
     is exclusive.  */
  out.range.start.line = el_start.line - 1;
  out.range.start.character = el_start.column - 1;
  out.range.end.line = el_finish.line - 1;
  out.range.end.character = el_finish.column - 1 + 1;
  return true;
}

/* Implementation of lsp::server, wired up to GCC's IR.  */

class gcc_lsp_server : public lsp::noop_server
{
  /* Implementation of "textDocument/definition":
     find the definition of the symbol at a given source location.  */

  void
  do_text_document_definition (const TextDocumentPositionParams &p,
			       vec<Location> &out) OVERRIDE;
};


/* class gcc_lsp_server : public lsp::noop_server.  */

/* Implementation of "textDocument/definition":
   find the definition of the symbol at a given source location.  */

void
gcc_lsp_server::do_text_document_definition (const TextDocumentPositionParams &p,
					     vec<Location> &out)
{
  /* TODO.  */
  /* Convert from LSP's 0-based lines and columns to GCC's
     1-based lines and columns.  */
  blt_node *blt
    = blt_get_root_node ()->get_descendant_at_location
       (p.textDocument.uri,
	p.position.line + 1,
	p.position.character + 1);
  if (!blt)
    /* No results.  */
    return;

  blt->dump (stderr);

  switch (blt->get_kind ())
    {
    case BLT_STRUCT_OR_UNION_SPECIFIER:
      {
	/* Attempt to locate the RECORD_TYPE for the
	   struct-or-union-specifier.  */
	tree record_type = blt->get_tree ();
	if (!record_type)
	  return;

	if (1)
	  the_blt_ctxt->get_root_node ()->dump (stderr);

	/* Find a struct-contents with tree == record_type.  */
	is_record_definition pred (record_type);
	blt_node *blt_struct_contents
	  = blt_get_root_node ()->find_descendant_satisfying (pred);
	if (!blt_struct_contents)
	  return;

	if (1)
	  blt_struct_contents->dump (stderr);
	blt_node *blt_struct_defn = blt_struct_contents->get_parent ();

	Location result;
	if (Location_from_blt_node (blt_struct_defn, result))
	  out.safe_push (result);
      }
      break;

    default:
      {
	/* Just show the blt_node itself.  */
	Location result;
	if (Location_from_blt_node (blt, result))
	  out.safe_push (result);
      }
      break;
    }
}

/* Serve LSP on PORT.  */

void
serve_lsp (int port)
{
  // FIXME
  gcc_lsp_server lsp_server;
  lsp::jsonrpc_server json_lsp_server (true, lsp_server);
  jsonrpc::http_server http_server (json_lsp_server);

  fprintf (stderr, "serving on port %i\n", port);
  http_server.serve (port);
}
