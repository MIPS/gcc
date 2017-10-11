/* Extra location information.
   Copyright (C) 2017 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "pretty-print.h"
#include "diagnostic.h"
#include "diagnostic-color.h"
#include "blt.h"
#include "selftest.h"

static const char *blt_kind_to_name (enum blt_kind kind);

/* blt_node's ctor.  */

blt_node::blt_node (enum blt_kind kind, location_t start)
: m_kind (kind), m_parent (NULL), m_first_child (NULL), m_last_child (NULL),
  m_prev_sibling (NULL), m_next_sibling (NULL),
  m_start (start), m_finish (UNKNOWN_LOCATION), m_tree (NULL_TREE)
{
}

/* blt_node's dtor.  */

blt_node::~blt_node ()
{
  blt_node *iter = m_first_child;
  while (iter)
    {
      blt_node *next = iter->m_next_sibling;
      delete iter;
      iter = next;
    }
}

/* Add CHILD to this blt_node as its final child.
   CHILD must be an orphan.  */

void
blt_node::add_child (blt_node *child)
{
  gcc_assert (child->m_prev_sibling == NULL);
  gcc_assert (child->m_next_sibling == NULL);
  gcc_assert (child->m_parent == NULL);

  if (m_last_child)
    {
      m_last_child->m_next_sibling = child;
      child->m_prev_sibling = m_last_child;
    }
  else
    {
      gcc_assert (m_first_child == NULL);
      m_first_child = child;
    }

  m_last_child = child;
  child->m_parent = this;
}


/* Convert OLD to an orphan, and take over parenthood of NEW,
   putting NEW in OLD's place.  */
// FIXME: motivated by the fixup in C++ parser of
// block-declaration => function-definition

void
blt_node::replace_child (blt_node *old, blt_node *new_)
{
  gcc_assert (old);
  gcc_assert (old->m_parent == this);
  gcc_assert (new_);
  assert_invariants ();
  old->assert_invariants ();
  new_->assert_invariants ();

  blt_node *old_prev_sibling = old->m_prev_sibling;
  blt_node *old_next_sibling = old->m_next_sibling;

  old->make_orphan ();
  new_->make_orphan ();

  new_->m_prev_sibling = old_prev_sibling;
  new_->m_next_sibling = old_next_sibling;

  if (old_prev_sibling == NULL)
    m_first_child = new_;
  if (old_next_sibling == NULL)
    m_last_child = new_;

  assert_invariants ();
  old->assert_invariants ();
  new_->assert_invariants ();
}

/* Convert this node to an orphan.  */

void
blt_node::make_orphan ()
{
  assert_invariants ();

  if (m_parent)
    {
      if (m_prev_sibling)
	{
	  m_prev_sibling->m_next_sibling = m_next_sibling;
	  m_prev_sibling = NULL;
	}
      else
	m_parent->m_first_child = m_next_sibling;
      if (m_next_sibling)
	{
	  m_next_sibling->m_prev_sibling = m_prev_sibling;
	  m_next_sibling = NULL;
	}
      else
	m_parent->m_last_child = m_prev_sibling;
    }
  m_parent = NULL;

  assert_invariants ();
}

/* Set the tree associated with this blt_node to be T.  */
// FIXME: what if it's called more than once
// FIXME: CTXT

void
blt_node::set_tree (tree t, blt_context *ctxt)
{
  m_tree = t;

  /* Add to mapping.  */
  if (!t)
    return;

  gcc_assert (ctxt);
  gcc_assert (ctxt->m_tree_to_blt_map);

  ctxt->m_tree_to_blt_map->put (t, this);
}

/* Dump this blt_node and its descendants to FILE.  */

void
blt_node::dump (FILE *file) const
{
  pretty_printer pp;
  pp_show_color (&pp) = pp_show_color (global_dc->printer);
  pp_format_decoder (&pp) = pp_format_decoder (global_dc->printer);
  pp.buffer->stream = file;
  dump (&pp, "", NULL, true);
  pp_flush (&pp);
}

/* Dump this blt_node and its descendants to PP.
   PREFIX is used for all lines, providing an ascii-art representation
   of the tree structure; PARENT and IS_LAST_CHILD are used to control
   this representation; PARENT should be NULL if printing this node as
   the top-level node within this dump (for dumping sub-trees).  */

void
blt_node::dump (pretty_printer *pp, const char *prefix,
		const blt_node *parent, bool is_last_child) const
{
  if (parent)
    {
      /* Colorized hierachical ASCII art.  */
      const char *tail = is_last_child ? "`-" : "|-";
      pp_printf (pp, "%r%s%s%R", "note", prefix, tail);
      // FIXME: dedicated color code?
    }
  pp_printf (pp, "%r%s%R %p", "error", blt_kind_to_name (m_kind),
	     (const void *)this); // FIXME: dedicated color code?

  location_t parent_start_loc = parent ? parent->m_start : UNKNOWN_LOCATION;
  location_t parent_finish_loc = parent ? parent->m_finish : UNKNOWN_LOCATION;

  /* Dump location information, eliding commonality with parent.  */
  {
    pp_printf (pp, " %s<", colorize_start (pp_show_color (pp), "warning"));
    // FIXME: dedicated color code?

    expanded_location el_start = expand_location (m_start);
    expanded_location el_parent = expand_location (parent_start_loc);

    if (el_start.file != el_parent.file)
      /* We don't use %qs or %< and %> here, to avoid affecting
	 colorization.  */
      pp_printf (pp, "%s:", el_start.file);
    pp_printf (pp, "%i:%i", el_start.line, el_start.column);
    if (m_finish != m_start)
      {
	pp_printf (pp, "-");
	expanded_location el_finish = expand_location (m_finish);
	if (el_finish.file && el_finish.file != el_start.file)
	  pp_printf (pp, "%s:", el_finish.file);
	pp_printf (pp, "%i:%i", el_finish.line, el_finish.column);
      }

    pp_printf (pp, ">%s", colorize_stop (pp_show_color (pp)));
  }

  /* TODO: print any tree associatee with this blt_node.  */
#if 0
  if (m_tree)
    // FIXME: which format code should be used?
    // FIXME: add get_tree_code_name to both
#if 1
    pp_printf (pp, " tree: %p %r<%s>%R", (void *)m_tree,
	       "warning", // FIXME: dedicated color code?
	       get_tree_code_name (TREE_CODE (m_tree)));
#else
    // seems to work for C++:
    pp_printf (pp, " tree: %p %r<%s>%R %qE", (void *)m_tree,
	       "warning", // FIXME: dedicated color code?
	       get_tree_code_name (TREE_CODE (m_tree)),
	       m_tree);
#endif
#endif

  const char *new_prefix;
  if (parent)
    new_prefix = ACONCAT ((prefix, is_last_child ? "  " : "|  ", NULL));
  else
    new_prefix = prefix;

  /* Show source code.  */
  if (m_start != parent_start_loc || m_finish != parent_finish_loc)
    {
      location_t range = get_range ();
      rich_location richloc (line_table, range);
      diagnostic_context dc;
      memset (&dc, 0, sizeof (dc));
      dc.printer = pp;
      dc.show_caret = true;
      dc.caret_chars[0] = '^';
      diagnostic_set_caret_max_width (&dc, pp_line_cutoff (pp));
      const char *saved_prefix = pp->prefix;
      const char *source_prefix;
      const char *begin_color = colorize_start (pp_show_color (pp), "note");
      const char *end_color = colorize_stop (pp_show_color (pp));
      source_prefix
	= ACONCAT ((begin_color, new_prefix, "|:", end_color, NULL));
      pp_set_prefix (pp, source_prefix);
      pp_prefixing_rule (pp) = DIAGNOSTICS_SHOW_PREFIX_EVERY_LINE;
      dc.show_ruler_p = true;
      diagnostic_show_locus (&dc, &richloc, DK_NOTE);
      pp_prefixing_rule (pp) = DIAGNOSTICS_SHOW_PREFIX_NEVER;
      pp_set_prefix (pp, saved_prefix);
      /* diagnostic_show_locus expects to add a newline at the start
	 and ends with a newline.  */
    }
  else
    pp_newline (pp);

  for (blt_node *child = m_first_child; child; child = child->m_next_sibling)
    {
      bool is_last_child = child == m_last_child;
      child->dump (pp, new_prefix, this, is_last_child);
    }
}

/* Get a human-readable name for this node, e.g. "translation-unit".  */

const char *
blt_node::get_name () const
{
  return blt_kind_to_name (m_kind);
}

/* Get the range of source locations covered by this blt_node.  */

location_t
blt_node::get_range () const
{
  return make_location (m_start, m_start, m_finish);
}

/* Get the first child of KIND, or NULL.  */

blt_node *
blt_node::get_first_child_of_kind (enum blt_kind kind) const
{
  for (blt_node *child = m_first_child; child; child = child->m_next_sibling)
    if (kind == child->m_kind)
      return child;
  return NULL;
}

/* Populate OUT with all children of KIND.  */

void
blt_node::get_children_of_kind (auto_vec<blt_node *> &out,
				enum blt_kind kind) const
{
  for (blt_node *child = m_first_child; child; child = child->m_next_sibling)
    if (kind == child->m_kind)
      out.safe_push (child);
}

/* Get the index of NEEDLE within this node if is a child, or -1 otherwise.  */

int
blt_node::get_index_of_child (blt_node *needle) const
{
  int idx = 0;
  for (blt_node *child = m_first_child; child; child = child->m_next_sibling)
    {
      if (child == needle)
	return idx;
      idx++;
    }
  return -1;
}

/* Get the most recent ancestor of this node of KIND, or NULL if there
   aren't any.  */

blt_node *
blt_node::get_ancestor_of_kind (enum blt_kind kind) const
{
  for (blt_node *iter = m_parent; iter; iter = iter->m_parent)
    if (kind == iter->m_kind)
      return iter;
  return NULL;
}

/* Find the deepest descendant of this node (or node itself)
   satisfying PRED, or NULL if there aren't any.  */

blt_node *
blt_node::find_descendant_satisfying (blt_predicate &pred)
{
  /* First, try to find within children, so that we get the deepest node
     matching the criterion.  */
  for (blt_node *child = m_first_child; child; child = child->m_next_sibling)
    {
      blt_node *within_child = child->find_descendant_satisfying (pred);
      if (within_child)
	return within_child;
    }

  /* Try within this node.  */
  if (pred.satisfied_by_node_p (*this))
    return this;

  return NULL;
}

/* Subclass of blt_predicate for identifying nodes containing the
   given source location.  */

class contains_location_predicate : public blt_predicate
{
 public:
  contains_location_predicate (const char *filename, int line, int character)
  : m_filename (filename), m_line (line), m_character (character) {}

  bool satisfied_by_node_p (const blt_node &node) FINAL OVERRIDE
  {
    return node.contains_location_p (m_filename, m_line, m_character);
  }

 private:
  const char *m_filename;
  int m_line;
  int m_character;
};

/* Find the deepest descendant of this node (or the node itself) at
   the given source location, or NULL if there aren't any.  */

blt_node *
blt_node::get_descendant_at_location (const char *filename, int line,
				      int character)
{
  contains_location_predicate pred (filename, line, character);
  return find_descendant_satisfying (pred);
}

/* Return true iff this node contains the given source location.  */

bool
blt_node::contains_location_p (const char *filename, int row, int column) const
{
  expanded_location el_start = expand_location (m_start);
  expanded_location el_finish = expand_location (m_finish);

  if (0 != strcmp (filename, el_start.file))
      return false;
  if (0 != strcmp (filename, el_finish.file))
      return false;

  /* FIXME: share this with diagnostic-show-locus.c:layout_range.  */

  gcc_assert (el_start.line <= el_finish.line);
  /* ...but the equivalent isn't true for the columns;
     consider example B in the comment above.  */

  if (row < el_start.line)
    /* Points before the first line of the range are
       outside it (corresponding to line 01 in example A
       and lines 01 and 02 in example B above).  */
    return false;

  if (row == el_start.line)
    /* On same line as start of range (corresponding
       to line 02 in example A and line 03 in example B).  */
    {
      if (column < el_start.column)
	/* Points on the starting line of the range, but
	   before the column in which it begins.  */
	return false;

      if (row < el_finish.line)
	/* This is a multiline range; the point
	   is within it (corresponds to line 03 in example B
	   from column 14 onwards) */
	return true;
      else
	{
	  /* This is a single-line range.  */
	  gcc_assert (row == el_finish.line);
	  return column <= el_finish.column;
	}
    }

  /* The point is in a line beyond that containing the
     start of the range: lines 03 onwards in example A,
     and lines 04 onwards in example B.  */
  gcc_assert (row > el_start.line);

  if (row > el_finish.line)
    /* The point is beyond the final line of the range
       (lines 03 onwards in example A, and lines 06 onwards
       in example B).  */
    return false;

  if (row < el_finish.line)
    {
      /* The point is in a line that's fully within a multiline
	 range (e.g. line 04 in example B).  */
      gcc_assert (el_start.line < el_finish.line);
      return true;
    }

  gcc_assert (row ==  el_finish.line);

  return column <= el_finish.column;
}

/* In a debug build, assert that basic invariants hold.  */

void
blt_node::assert_invariants () const
{
  if (m_parent)
    {
      if (m_next_sibling)
	gcc_assert (m_parent);
      else
	gcc_assert (m_parent->m_last_child == this);
      if (m_prev_sibling)
	gcc_assert (m_parent);
      else
	gcc_assert (m_parent->m_first_child == this);
    }
  else
    {
      gcc_assert (m_next_sibling == NULL);
      gcc_assert (m_prev_sibling == NULL);
    }
}

/* The table of names for enum blt_kind.  */

static const char * const blt_kind_names[] = {
#define DEF_BLT_NODE(ENUM_NAME, PRETTY_NAME) PRETTY_NAME,
#include "blt.def"
#undef DEF_BLT_NODE
};

/* Get a human-readable name for this blt_kind, e.g. "translation-unit".  */

static const char *
blt_kind_to_name (enum blt_kind kind)
{
  return blt_kind_names[kind];
}

/* Dump NODE to stderr.  */

DEBUG_FUNCTION void
debug (blt_node *node)
{
  node->dump (stderr);
}

/* class blt_context.  */

/* blt_context's ctor.  */

blt_context::blt_context ()
: m_tree_to_blt_map (new tree_to_blt_map_t ()),
  m_root_node (NULL)
{
}

/* blt_context's dtor.  */

blt_context::~blt_context ()
{
  delete m_tree_to_blt_map;
}

/* Given tree node T, get the assocated blt_node, if any, or NULL.  */

blt_node *
blt_context::get_node_for_tree (tree t)
{
  if (!t)
    return NULL;
  blt_node **slot = m_tree_to_blt_map->get (t);
  if (!slot)
    return NULL;
  return *slot;
}

/* Given tree node T, set the assocated blt_node if it has not been
   set already.

   If it has been set, don't change it; multiple tree nodes can
   reference an blt_node *, but an blt_node * references
   at most one tree node (e.g. C++ template instantiations
   can lead to multiple FUNCTION_DECL tree nodes from one blt_node).  */

void
blt_context::set_node_for_tree (tree t, blt_node *node)
{
  if (!t)
    return;
  if (!node)
    return;

  if (node->get_tree () == NULL)
    node->set_tree (t, this);
  else
    {
      /* Don't attempt to change; multiple tree nodes can
	 reference an blt_node *, but an blt_node * references
	 at most one tree node (e.g. template instantiations).  */
      gcc_assert (m_tree_to_blt_map);
      m_tree_to_blt_map->get_or_insert (t) = node;
    }
}

/* FIXME.  */

blt_context *the_blt_ctxt = NULL;

/* FIXME.  */

blt_node *
blt_get_root_node ()
{
  if (!the_blt_ctxt)
    return NULL;
  return the_blt_ctxt->get_root_node ();
}

/* FIXME.  */

blt_node *
blt_get_node_for_tree (tree t)
{
  if (!the_blt_ctxt)
    return NULL;
  return the_blt_ctxt->get_node_for_tree (t);
}

/* FIXME.  */

void
blt_set_node_for_tree (tree t, blt_node *n)
{
  if (!the_blt_ctxt)
    return;
  the_blt_ctxt->set_node_for_tree (t, n);
}


#if CHECKING_P

namespace selftest {

/* Selftests.  */

/* Helper function for making a blt_node.  */

static blt_node *
make_blt_node (blt_node *parent, enum blt_kind kind,
	       location_t start, location_t finish)
{
  blt_node *new_node = new blt_node (kind, start);
  new_node->set_finish (finish);
  if (parent)
    parent->add_child (new_node);
  return new_node;
}

/* Verify that blt_node basic operations work as expected.  */

static void
test_basic_ops (const line_table_case &case_)
{
  /* The primary goal of blt_node is location-tracking, so let's track
     some locations.  */

  /* Create a tempfile and write some text to it.
     ...0000000001111111111222222222233333333334444444444.
     ...1234567890123456789012345678901234567890123456789.  */
  const char *content
    = ("/* Some comment.  */\n" /* line 1.  */
       "struct point {\n"       /* line 2.  */
       " double x;\n"           /* line 3.  */
       " double y;\n"           /* line 4.  */
       " double z;\n"           /* line 5.  */
       "};\n");                 /* line 6.  */
  temp_source_file tmp (SELFTEST_LOCATION, ".c", content);
  line_table_test ltt (case_);

  const line_map_ordinary *ord_map
    = linemap_check_ordinary (linemap_add (line_table, LC_ENTER, false,
					   tmp.get_filename (), 0));

  linemap_line_start (line_table, 1, 100);

#define LOC(ROW, COL) \
  (linemap_position_for_line_and_column (line_table, ord_map, (ROW), (COL)))

  const location_t final_line_end = LOC (6, 2);

  /* Don't attempt to run the tests if column data might be unavailable.  */
  if (final_line_end > LINE_MAP_MAX_LOCATION_WITH_COLS)
    return;

  blt_node *tu, *ext_decl, *decl_specs, *type_spec, *s_or_u_spec,
    *s_contents,
    *s_decl_x, /* *decl_specs_x, */ /* *decl_x, */
    *s_decl_y, *decl_specs_y, *decl_y,
    *s_decl_z, /* *decl_specs_z, */ *decl_z;

  /* The block structure here shows the intended hierarchy.  */
  {
    tu = make_blt_node (NULL, BLT_TRANSLATION_UNIT, LOC (1, 1), LOC (6, 2));
    {
      ext_decl = make_blt_node (tu, BLT_EXTERNAL_DECLARATION,
				LOC (2, 1), LOC (6, 2));
      {
	decl_specs = make_blt_node (ext_decl, BLT_DECLARATION_SPECIFIERS,
				    LOC (2, 1), LOC (6, 1));
	{
	  type_spec = make_blt_node (decl_specs, BLT_TYPE_SPECIFIER,
				     LOC (2, 1), LOC (6, 1));
	  {
	    s_or_u_spec
	      = make_blt_node (type_spec, BLT_STRUCT_OR_UNION_SPECIFIER,
			       LOC (2, 1), LOC (6, 1));
	    {
	      s_contents = make_blt_node (s_or_u_spec, BLT_STRUCT_CONTENTS,
					  LOC (2, 14), LOC (6, 1));
	      {
		s_decl_x = make_blt_node (s_contents, BLT_STRUCT_DECLARATION,
					  LOC (3, 2), LOC (3, 9));
		{
		  /*decl_specs_x = */
		  make_blt_node (s_decl_x, BLT_DECLARATION_SPECIFIERS,
				 LOC (3, 2), LOC (3, 7));
		  /* decl_x = */ make_blt_node (s_decl_x, BLT_DECLARATOR,
						LOC (3, 9), LOC (3, 9));
		}
	      }
	      {
		s_decl_y = make_blt_node (s_contents, BLT_STRUCT_DECLARATION,
					  LOC (4, 2), LOC (4, 9));
		{
		  decl_specs_y
		    = make_blt_node (s_decl_y, BLT_DECLARATION_SPECIFIERS,
				     LOC (4, 2), LOC (4, 7));
		  decl_y = make_blt_node (s_decl_y, BLT_DECLARATOR,
					  LOC (4, 9), LOC (4, 9));
		}
	      }
	      {
		s_decl_z = make_blt_node (s_contents, BLT_STRUCT_DECLARATION,
					  LOC (5, 2), LOC (5, 9));
		{
		  /* decl_specs_z = */
		  make_blt_node (s_decl_z, BLT_DECLARATION_SPECIFIERS,
				 LOC (5, 2), LOC (5, 7));
		  decl_z = make_blt_node (s_decl_z, BLT_DECLARATOR,
					  LOC (5, 9), LOC (5, 9));
		}
	      }
	    }
	  }
	}
      }
    }
  }

  if (0)
    tu->dump (stderr);

  ASSERT_EQ (BLT_TRANSLATION_UNIT, tu->get_kind ());
  ASSERT_EQ (BLT_STRUCT_DECLARATION, s_decl_z->get_kind ());

  ASSERT_STREQ ("translation-unit", tu->get_name ());
  ASSERT_STREQ ("struct-declaration", s_decl_z->get_name ());

  ASSERT_EQ (NULL, tu->get_parent ());
  ASSERT_EQ (s_contents, s_decl_z->get_parent ());

  /* Location access.  */
  ASSERT_EQ (LOC (2, 1), ext_decl->get_start ());
  ASSERT_EQ (LOC (6, 2), ext_decl->get_finish ());
  location_t range = ext_decl->get_range ();
  ASSERT_EQ (LOC (2, 1), get_start (range));
  ASSERT_EQ (LOC (6, 2), get_finish (range));

  /* blt_node::get_tree.  */
  // TODO

  /* blt_node::get_first_child_of_kind.  */
  ASSERT_EQ (ext_decl, tu->get_first_child_of_kind (BLT_EXTERNAL_DECLARATION));
  ASSERT_EQ (NULL, tu->get_first_child_of_kind (BLT_TRANSLATION_UNIT));
  ASSERT_EQ (decl_z,  s_decl_z->get_first_child_of_kind (BLT_DECLARATOR));

  /* blt_node::get_children_of_kind.  */
  auto_vec<blt_node *> fields;
  s_contents->get_children_of_kind (fields, BLT_STRUCT_DECLARATION);
  ASSERT_EQ (3, fields.length ());
  ASSERT_EQ (s_decl_x, fields[0]);
  ASSERT_EQ (s_decl_y, fields[1]);
  ASSERT_EQ (s_decl_z, fields[2]);

  /* blt_node::get_index_of_child.  */
  ASSERT_EQ (0, s_contents->get_index_of_child (s_decl_x));
  ASSERT_EQ (1, s_contents->get_index_of_child (s_decl_y));
  ASSERT_EQ (2, s_contents->get_index_of_child (s_decl_z));
  ASSERT_EQ (-1, s_contents->get_index_of_child (tu));

  /* blt_node::get_ancestor_of_kind.  */
  ASSERT_EQ (tu, s_decl_z->get_ancestor_of_kind (BLT_TRANSLATION_UNIT));
  ASSERT_EQ (NULL, tu->get_ancestor_of_kind (BLT_TRANSLATION_UNIT));

  /* blt_node::get_descendant_at_location.  */
  ASSERT_EQ (decl_specs_y,
	     tu->get_descendant_at_location (tmp.get_filename (), 4, 4));
  ASSERT_EQ (decl_y,
	     tu->get_descendant_at_location (tmp.get_filename (), 4, 9));
  ASSERT_EQ (tu,
	     tu->get_descendant_at_location (tmp.get_filename (), 1, 1));
  ASSERT_EQ (NULL,
	     tu->get_descendant_at_location (tmp.get_filename (), 7, 1));

  /* blt_node::contains_location_p.  */
  /* s_contents ought to range from LOC (2, 14) to LOC (6, 1).  */
  ASSERT_FALSE (s_contents->contains_location_p (tmp.get_filename (), 1, 14));
  ASSERT_FALSE (s_contents->contains_location_p (tmp.get_filename (), 2, 13));
  ASSERT_TRUE (s_contents->contains_location_p (tmp.get_filename (), 2, 14));
  ASSERT_TRUE (s_contents->contains_location_p (tmp.get_filename (), 3, 1));
  ASSERT_FALSE (s_contents->contains_location_p ("not-the-filename", 3, 1));
  ASSERT_TRUE (s_contents->contains_location_p (tmp.get_filename (), 6, 1));
  ASSERT_FALSE (s_contents->contains_location_p (tmp.get_filename (), 6, 2));

#undef LOC

  delete tu;
}

/* Verify that we can wrap cpp tokens.  */

static void
test_cpp_tokens ()
{
  blt_node plus_node (BLT_TOKEN_OP_PLUS, UNKNOWN_LOCATION);
  ASSERT_STREQ ("+-token", plus_node.get_name ());

  blt_node name_node (BLT_TOKEN_TK_NAME, UNKNOWN_LOCATION);
  ASSERT_STREQ ("NAME-token", name_node.get_name ());
}

/* Run all of the selftests within this file.  */

void
blt_cc_tests ()
{
  for_each_line_table_case (test_basic_ops);

  test_cpp_tokens ();
}

} // namespace selftest

#endif /* #if CHECKING_P */
