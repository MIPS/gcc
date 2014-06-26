/* Generate pattern matching and transform code shared between
   GENERIC and GIMPLE folding code from match-and-simplify description.

   Copyright (C) 2014 Free Software Foundation, Inc.
   Contributed by Richard Biener <rguenther@suse.de>

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

#include "bconfig.h"
#include <new>
#include "system.h"
#include "coretypes.h"
#include <cpplib.h>
#include "errors.h"
#include "hashtab.h"
#include "hash-table.h"
#include "vec.h"

/* libccp helpers.  */

static struct line_maps *line_table;

static bool
#if GCC_VERSION >= 4001
__attribute__((format (printf, 6, 0)))
#endif
error_cb (cpp_reader *, int, int, source_location location,
	  unsigned int, const char *msg, va_list *ap)
{
  const line_map *map;
  linemap_resolve_location (line_table, location, LRK_SPELLING_LOCATION, &map);
  expanded_location loc = linemap_expand_location (line_table, map, location);
  fprintf (stderr, "%s:%d:%d error: ", loc.file, loc.line, loc.column);
  vfprintf (stderr, msg, *ap);
  fprintf (stderr, "\n");
  exit (1);
}

static void
#if GCC_VERSION >= 4001
__attribute__((format (printf, 2, 3)))
#endif
fatal_at (const cpp_token *tk, const char *msg, ...)
{
  va_list ap;
  va_start (ap, msg);
  error_cb (NULL, CPP_DL_FATAL, 0, tk->src_loc, 0, msg, &ap);
  va_end (ap);
}

static void
output_line_directive (FILE *f, source_location location)
{
  const line_map *map;
  linemap_resolve_location (line_table, location, LRK_SPELLING_LOCATION, &map);
  expanded_location loc = linemap_expand_location (line_table, map, location);
  /* Other gen programs really output line directives here, at least for
     development it's right now more convenient to have line information
     from the generated file.  Still keep the directives as comment for now
     to easily back-point to the meta-description.  */
  fprintf (f, "/* #line %d \"%s\" */\n", loc.line, loc.file);
}


/* Grammar

     capture = '@' number
     op = predicate | expr [capture]
     c_expr = '{' ... '}'
     genexpr = '(' code genop... ')'
     genop = capture | genexpr | c_expr
     transform = 'match_and_transform' name expr genop

     Match and simplify (A + B) - B -> A
     (define_match_and_simplify foo
       (PLUS_EXPR (MINUS_EXPR integral_op_p@0 @1) @1)
       @0)

     Match and simplify (CST + A) + CST to CST' + A
     (define_match_and_simplify bar
       (PLUS_EXPR INTEGER_CST_P@0 (PLUS_EXPR @1 INTEGER_CST_P@2))
       (PLUS_EXPR { int_const_binop (PLUS_EXPR, captures[0], captures[2]); } @1))
*/


#define DEFTREECODE(SYM, STRING, TYPE, NARGS)   SYM,
enum tree_code {
#include "tree.def"
MAX_TREE_CODES
};
#undef DEFTREECODE

#define DEF_BUILTIN(ENUM, N, C, T, LT, B, F, NA, AT, IM, COND) ENUM,
enum built_in_function {
#include "builtins.def"
END_BUILTINS
};
#undef DEF_BUILTIN

/* Hashtable of known pattern operators.  This is pre-seeded from
   all known tree codes and all known builtin function ids.  */

struct id_base : typed_free_remove<id_base>
{
  enum id_kind { CODE, FN } kind;

  id_base (id_kind, const char *);

  hashval_t hashval;
  const char *id;

  /* hash_table support.  */
  typedef id_base value_type;
  typedef id_base compare_type;
  static inline hashval_t hash (const value_type *);
  static inline int equal (const value_type *, const compare_type *);
};

inline hashval_t
id_base::hash (const value_type *op)
{
  return op->hashval;
}
inline int
id_base::equal (const value_type *op1,
			const compare_type *op2)
{
  return (op1->hashval == op2->hashval
	  && strcmp (op1->id, op2->id) == 0);
}

static hash_table<id_base> operators;

id_base::id_base (id_kind kind_, const char *id_)
{
  kind = kind_;
  id = id_;
  hashval = htab_hash_string (id);
}

struct operator_id : public id_base
{
  operator_id (enum tree_code code_, const char *id_, unsigned nargs_)
      : id_base (id_base::CODE, id_),
      code (code_), nargs (nargs_) {}
  unsigned get_required_nargs () const { return nargs; }
  enum tree_code code;
  unsigned nargs;
};

struct fn_id : public id_base
{
  fn_id (enum built_in_function fn_, const char *id_)
      : id_base (id_base::FN, id_), fn (fn_) {}
  enum built_in_function fn;
};

static void
add_operator (enum tree_code code, const char *id,
	      const char *tcc, unsigned nargs)
{
  if (strcmp (tcc, "tcc_unary") != 0
      && strcmp (tcc, "tcc_binary") != 0
      && strcmp (tcc, "tcc_comparison") != 0
      && strcmp (tcc, "tcc_expression") != 0
      /* For {REAL,IMAG}PART_EXPR and VIEW_CONVERT_EXPR.  */
      && strcmp (tcc, "tcc_reference") != 0)
    return;
  operator_id *op = new operator_id (code, id, nargs);
  id_base **slot = operators.find_slot_with_hash (op, op->hashval, INSERT);
  if (*slot)
    fatal ("duplicate id definition");
  *slot = op;
}

static void
add_builtin (enum built_in_function code, const char *id)
{
  fn_id *fn = new fn_id (code, id);
  id_base **slot = operators.find_slot_with_hash (fn, fn->hashval, INSERT);
  if (*slot)
    fatal ("duplicate id definition");
  *slot = fn;
}


/* The predicate expression tree structure.  */

struct dt_operand;

struct operand {
  enum op_type { OP_PREDICATE, OP_EXPR, OP_CAPTURE, OP_C_EXPR };
  operand (enum op_type type_) : type (type_) {}
  enum op_type type;
  virtual void gen_transform (FILE *f, const char *, bool) = 0;
};

struct predicate : public operand
{
  predicate (const char *ident_) : operand (OP_PREDICATE), ident (ident_) {}
  const char *ident;
  virtual void gen_transform (FILE *, const char *, bool) { gcc_unreachable (); }
};

struct e_operation {
  e_operation (const char *id, bool is_commutative_ = false);
  id_base *op;
  bool is_commutative;
};


struct expr : public operand
{
  expr (e_operation *operation_)
    : operand (OP_EXPR), operation (operation_), ops (vNULL) {}
  void append_op (operand *op) { ops.safe_push (op); }
  e_operation *operation;
  vec<operand *> ops;
  virtual void gen_transform (FILE *f, const char *, bool);
};

struct c_expr : public operand
{
  c_expr (cpp_reader *r_, vec<cpp_token> code_, unsigned nr_stmts_)
    : operand (OP_C_EXPR), r (r_), code (code_),
      nr_stmts (nr_stmts_), fname (NULL) {}
  cpp_reader *r;
  vec<cpp_token> code;
  unsigned nr_stmts;
  char *fname;
  virtual void gen_transform (FILE *f, const char *, bool);
};

struct capture : public operand
{
  capture (const char *where_, operand *what_)
      : operand (OP_CAPTURE), where (where_), what (what_) {}
  const char *where;
  operand *what;
  virtual void gen_transform (FILE *f, const char *, bool);
};


e_operation::e_operation (const char *id, bool is_commutative_)
{
  id_base tem (id_base::CODE, id);
  is_commutative = is_commutative_;

  op = operators.find_with_hash (&tem, tem.hashval);
  if (op)
    return;

  /* Try all-uppercase.  */
  char *id2 = xstrdup (id);
  for (unsigned i = 0; i < strlen (id2); ++i)
    id2[i] = TOUPPER (id2[i]);
  new (&tem) id_base (id_base::CODE, id2);
  op = operators.find_with_hash (&tem, tem.hashval);
  if (op)
    {
      free (id2);
      return;
    }

  /* Try _EXPR appended.  */
  id2 = (char *)xrealloc (id2, strlen (id2) + sizeof ("_EXPR") + 1);
  strcat (id2, "_EXPR");
  new (&tem) id_base (id_base::CODE, id2);
  op = operators.find_with_hash (&tem, tem.hashval);
  if (op)
    {
      free (id2);
      return;
    }

  fatal ("expected operator, got %s", id);
}

struct simplify {
  simplify (const char *name_,
	    vec<operand *> matchers_, source_location match_location_,
	    struct operand *ifexpr_, source_location ifexpr_location_,
	    struct operand *result_, source_location result_location_)
      : name (name_), matchers (matchers_), match_location (match_location_),
      ifexpr (ifexpr_), ifexpr_location (ifexpr_location_),
      result (result_), result_location (result_location_) {}
  const char *name;
  vec<operand *> matchers;  // vector to hold commutative expressions
  source_location match_location;
  struct operand *ifexpr;
  source_location ifexpr_location;
  struct operand *result;
  source_location result_location;
};

struct dt_node
{
  enum dt_type { DT_NODE, DT_OPERAND, DT_TRUE, DT_MATCH, DT_SIMPLIFY };

  enum dt_type type;
  unsigned level;
  vec<dt_node *> kids;

  dt_node (enum dt_type type_): type (type_), level (0), kids (vNULL) {} 
  
  dt_node *append_node (dt_node *); 
  dt_node *append_op (operand *, dt_node *parent = 0, unsigned pos = 0); 
  dt_node *append_true_op (dt_node *parent = 0, unsigned pos = 0);
  dt_node *append_match_op (dt_operand *, dt_node *parent = 0, unsigned pos = 0);
  dt_node *append_simplify (simplify *, unsigned, dt_operand **); 

  virtual void gen_gimple (FILE *) {}
  virtual void gen_generic (FILE *) {}
  
  bool get_expr_code (enum tree_code&);
  bool is_gimple_expr ();
  bool is_gimple_fn ();
};

struct dt_operand: public dt_node
{
  operand *op;
  dt_operand *match_dop;
  dt_operand *parent;
  unsigned pos;

  struct kids_type {
    vec<dt_node *> gimple_exprs;
    vec<dt_node *> fns;
    vec<dt_node *> others;
    dt_node *true_operand;
  
    kids_type (): gimple_exprs (vNULL), fns (vNULL), others (vNULL), true_operand (0) {}
  };
 
  dt_operand (enum dt_type type, operand *op_, dt_operand *match_dop_, dt_operand *parent_ = 0, unsigned pos_ = 0)
	: dt_node (type), op (op_), match_dop (match_dop_), parent (parent_), pos (pos_) {} 

  virtual void gen_gimple (FILE *);
  virtual void gen_generic (FILE *);
  unsigned gen_predicate (FILE *, const char *);
  unsigned gen_match_op (FILE *, const char *);

  unsigned gen_gimple_expr_expr (FILE *); 
  unsigned gen_gimple_expr_fn (FILE *);

  unsigned gen_generic_expr (FILE *, const char *, bool);
  void gen_generic_expr_expr (FILE *, expr *, const char *, bool);
  void gen_generic_expr_fn (FILE *, expr *, const char *, bool);

  char *get_name (char *);
  void gen_opname (char *, unsigned);

  void grok_kids(kids_type&);
  void gen_gimple_kids (FILE *);
};


struct dt_simplify: public dt_node
{
  static const unsigned level_max = UINT_MAX;
  static const unsigned capture_max = 4;
  simplify *s; 
  unsigned pattern_no;
  dt_operand *indexes[capture_max]; 
  
  dt_simplify (simplify *s_, unsigned pattern_no_, dt_operand **indexes_)
	: dt_node (DT_SIMPLIFY), s (s_), pattern_no (pattern_no_)
  {
    for (unsigned i = 0; i < capture_max; ++i)
      indexes[i] = indexes_[i];
  }

  virtual void gen_gimple (FILE *f);
  virtual void gen_generic (FILE *f);
};

struct decision_tree
{
  dt_node *root;
  
  void insert (struct simplify *, unsigned); 
  void gen_gimple (FILE *f = stderr);
  void gen_generic (FILE *f = stderr);
  void print (FILE *f = stderr);

  decision_tree () { root = new dt_node (dt_node::DT_NODE); }

  static dt_node *insert_operand (dt_node *, operand *, dt_operand **indexes, unsigned pos = 0, dt_node *parent = 0);
  static dt_node *find_node (vec<dt_node *>&, dt_node *);
  static bool cmp_node (dt_node *, dt_node *);
  static void print_node (dt_node *, FILE *f = stderr, unsigned = 0);
};

void
print_operand (operand *o, FILE *f = stderr, bool flattened = false)
{
  if (o->type == operand::OP_CAPTURE)
    {
      capture *c = static_cast<capture *> (o);
      fprintf (f, "@%s", (static_cast<capture *> (o))->where);
      if (c->what && flattened == false) 
	{
	  putc (':', f);
	  print_operand (c->what, f, flattened);
	  putc (' ', f);
	}
    }

  else if (o->type == operand::OP_PREDICATE)
    fprintf (f, "%s", (static_cast<predicate *> (o))->ident);

  else if (o->type == operand::OP_C_EXPR)
    fprintf (f, "c_expr");

  else if (o->type == operand::OP_EXPR)
    {
      expr *e = static_cast<expr *> (o);
      fprintf (f, "(%s", e->operation->op->id);

      if (flattened == false)
	{
	  putc (' ', f);
	  for (unsigned i = 0; i < e->ops.length (); ++i)
	    {
	      print_operand (e->ops[i], f, flattened);
	      putc (' ', f);
	    }
	}
      putc (')', f);
    }

  else
    gcc_unreachable ();
}

void
print_matches (struct simplify *s, FILE *f = stderr)
{
  if (s->matchers.length () == 1)
    return;

  fprintf (f, "for expression: ");
  print_operand (s->matchers[0], f);  // s->matchers[0] is equivalent to original expression
  putc ('\n', f);

  fprintf (f, "commutative expressions:\n");
  for (unsigned i = 0; i < s->matchers.length (); ++i)
    {
      print_operand (s->matchers[i], f);
      putc ('\n', f);
    }
}

void
cartesian_product (const vec< vec<operand *> >& ops_vector, vec< vec<operand *> >& result, vec<operand *>& v, unsigned n)
{
  if (n == ops_vector.length ())
    {
      vec<operand *> xv = v.copy (); 
      result.safe_push (xv);
      return;
    }

  for (unsigned i = 0; i < ops_vector[n].length (); ++i)
    {
      v[n] = ops_vector[n][i];
      cartesian_product (ops_vector, result, v, n + 1);
    }
}
 
void
cartesian_product (const vec< vec<operand *> >& ops_vector, vec< vec<operand *> >& result, unsigned n_ops)
{
  vec<operand *> v = vNULL;
  v.safe_grow_cleared (n_ops);
  cartesian_product (ops_vector, result, v, 0);
}

vec<operand *>
commutate (operand *op)
{
  vec<operand *> ret = vNULL;

  if (op->type == operand::OP_CAPTURE)
    {
      capture *c = static_cast<capture *> (op);
      if (!c->what)
	{
	  ret.safe_push (op);
	  return ret;
	}
      vec<operand *> v = commutate (c->what);
      for (unsigned i = 0; i < v.length (); ++i)
	{
	  capture *nc = new capture (c->where, v[i]);
	  ret.safe_push (nc);
	}
      return ret;	
    }

  if (op->type != operand::OP_EXPR)
    {
      ret.safe_push (op);
      return ret;
    }

  expr *e = static_cast<expr *> (op);

  vec< vec<operand *> > ops_vector = vNULL;
  for (unsigned i = 0; i < e->ops.length (); ++i)
    ops_vector.safe_push (commutate (e->ops[i]));

  vec< vec<operand *> > result = vNULL;
  cartesian_product (ops_vector, result, e->ops.length ());

  for (unsigned i = 0; i < result.length (); ++i)
    {
      expr *ne = new expr (e->operation);
      for (unsigned j = 0; j < result[i].length (); ++j)
	ne->append_op (result[i][j]);
      ret.safe_push (ne);
    }

  if (!e->operation->is_commutative)
    return ret;

  for (unsigned i = 0; i < result.length (); ++i)
    {
      expr *ne = new expr (e->operation);
      // result[i].length () is 2 since e->operation is binary
      for (unsigned j = result[i].length (); j; --j)
	ne->append_op (result[i][j-1]);
      ret.safe_push (ne);
    }

  return ret;
}

/* Code gen off the AST.  */

void
expr::gen_transform (FILE *f, const char *dest, bool gimple)
{
  fprintf (f, "{\n");
  fprintf (f, "  tree ops[%u], res;\n", ops.length ());
  for (unsigned i = 0; i < ops.length (); ++i)
    {
      char dest[32];
      snprintf (dest, 32, "  ops[%u]", i);
      ops[i]->gen_transform (f, dest, gimple);
    }
  if (gimple)
    {
      /* ???  Have another helper that is like gimple_build but may
	 fail if seq == NULL.  */
      fprintf (f, "  if (!seq)\n"
	       "    {\n"
	       "      res = gimple_match_and_simplify (%s, TREE_TYPE (ops[0])",
	       operation->op->id);
      for (unsigned i = 0; i < ops.length (); ++i)
	fprintf (f, ", ops[%u]", i);
      fprintf (f, ", seq, valueize);\n");
      fprintf (f, "      if (!res) return false;\n");
      fprintf (f, "    }\n");
      fprintf (f, "  else\n");
      fprintf (f, "    res = gimple_build (seq, UNKNOWN_LOCATION, %s, "
	       "TREE_TYPE (ops[0])", operation->op->id);
      for (unsigned i = 0; i < ops.length (); ++i)
	fprintf (f, ", ops[%u]", i);
      fprintf (f, ", valueize);\n");
    }
  else
    {
      if (operation->op->kind == id_base::CODE)
	fprintf (f, "  res = fold_build%d (%s, TREE_TYPE (ops[0])",
		 ops.length(), operation->op->id);
      else
	fprintf (f, "  res = build_call_expr (builtin_decl_implicit (%s), %d",
		 operation->op->id, ops.length());
      for (unsigned i = 0; i < ops.length (); ++i)
	fprintf (f, ", ops[%u]", i);
      fprintf (f, ");\n");
    }
  fprintf (f, "  %s = res;\n", dest);
  fprintf (f, "}");
}

void
c_expr::gen_transform (FILE *f, const char *dest, bool gimple)
{
  /* If this expression has an outlined function variant, call it.  */
  if (fname)
    {
      fprintf (f, "%s = %s (type, captures);\n", dest, fname);
      return;
    }

  /* All multi-stmt expressions should have been outlined.  */
  gcc_assert (nr_stmts <= 1);

  if (nr_stmts == 1)
    fprintf (f, "%s = ", dest);

  for (unsigned i = 0; i < code.length (); ++i)
    {
      const cpp_token *token = &code[i];

      /* Replace captures for code-gen.  */
      if (token->type == CPP_ATSIGN)
	{
	  const cpp_token *n = &code[i+1];
	  if (n->type == CPP_NUMBER
	      && !(n->flags & PREV_WHITE))
	    {
	      if (token->flags & PREV_WHITE)
		fputc (' ', f);
	      fprintf (f, "captures[%s]", n->val.str.text);
	      ++i;
	      continue;
	    }
	}

      if (token->flags & PREV_WHITE)
	fputc (' ', f);

      /* Output the token as string.  */
      char *tk = (char *)cpp_token_as_text (r, token);
      fputs (tk, f);
    }

  if (nr_stmts == 1)
    fprintf (f, "\n");
}

void
capture::gen_transform (FILE *f, const char *dest, bool)
{
  fprintf (f, "%s = captures[%s];\n", dest, where); 
}

bool
cmp_operand (operand *o1, operand *o2)
{
  if (!o1 || !o2 || o1->type != o2->type)
    return false;

  if (o1->type == operand::OP_PREDICATE)
    {
      predicate *p1 = static_cast<predicate *>(o1);
      predicate *p2 = static_cast<predicate *>(o2);
      return strcmp (p1->ident, p2->ident) == 0;
    }
  else if (o1->type == operand::OP_EXPR)
    {
      expr *e1 = static_cast<expr *>(o1);
      expr *e2 = static_cast<expr *>(o2);
      return strcmp (e1->operation->op->id, e2->operation->op->id) == 0;
    }
  else
    return false;
}


bool
decision_tree::cmp_node (dt_node *n1, dt_node *n2)
{
  if (!n1 || !n2 || n1->type != n2->type)
    return false;

  if (n1 == n2 || n1->type == dt_node::DT_TRUE)
    return true;

  if (n1->type == dt_node::DT_OPERAND)
    return cmp_operand ((static_cast<dt_operand *> (n1))->op, (static_cast<dt_operand *> (n2))->op);

  else if (n1->type == dt_node::DT_MATCH)
    return (static_cast<dt_operand *> (n1))->match_dop == (static_cast<dt_operand *> (n2))->match_dop;

  else
    return false;
}

dt_node *
decision_tree::find_node (vec<dt_node *>& ops, dt_node *p)
{
  for (unsigned i = 0; i < ops.length (); ++i)
    if (decision_tree::cmp_node (ops[i], p))
      return ops[i]; 
  
  return 0;
}

dt_node *
dt_node::append_node (dt_node *n)
{
  dt_node *kid;

  kid = decision_tree::find_node (kids, n);
  if (kid)
    return kid;

  kids.safe_push (n);
  n->level = this->level + 1;

  unsigned len = kids.length ();

  if (len > 1 && kids[len - 2]->type == dt_node::DT_TRUE)
    {
      dt_node *p = kids[len - 2];
      kids[len - 2] = kids[len - 1];
      kids[len - 1] = p;
    }

  return n;
}

dt_node *
dt_node::append_op (operand *op, dt_node *parent, unsigned pos)
{
  dt_operand *parent_ = static_cast<dt_operand *> (parent);
  dt_node *n = new dt_operand (DT_OPERAND, op, 0, parent_, pos);
  dt_node *p = append_node (n);

  if (p != n)
    free (n);

  return p; 
}

dt_node *
dt_node::append_true_op (dt_node *parent, unsigned pos)
{
  dt_operand *parent_ = static_cast<dt_operand *> (parent);
  dt_node *n = new dt_operand (DT_TRUE, 0, 0, parent_, pos);
  dt_node *p = append_node (n);

  if (p != n)
    free (n);

  return p;
}

dt_node *
dt_node::append_match_op (dt_operand *match_dop, dt_node *parent, unsigned pos)
{
  dt_operand *parent_ = static_cast<dt_operand *> (parent);
  dt_node *n = new dt_operand (DT_MATCH, 0, match_dop, parent_, pos);
  dt_node *p = append_node (n);

  if (p != n)
    free (n);

  return p;
}

dt_node *
dt_node::append_simplify (simplify *s, unsigned pattern_no, dt_operand **indexes) 
{
  dt_node *n = new dt_simplify (s, pattern_no, indexes);
  return append_node (n);
}

dt_node *
decision_tree::insert_operand (dt_node *p, operand *o, dt_operand **indexes, unsigned pos, dt_node *parent) 
{
  dt_node *q, *elm;

  if (o->type == operand::OP_CAPTURE)
    {
      capture *c = static_cast<capture *> (o);
      unsigned capt_index = atoi (c->where);

      if (indexes[capt_index] == 0)
	{
	  if (c->what)
	    {
	      q = insert_operand (p, c->what, indexes, pos, parent);
	      dt_operand temp (dt_node::DT_OPERAND, c->what, 0);
	      elm = decision_tree::find_node (p->kids, &temp);
	    }
	  else
	    q = elm = p->append_true_op (parent, pos);

	  gcc_assert (elm->type == dt_node::DT_TRUE || elm->type == dt_node::DT_OPERAND || elm->type == dt_node::DT_MATCH);
	  indexes[capt_index] = static_cast<dt_operand *> (elm);
	  return q;
	}
      else
	{
	  p = p->append_match_op (indexes[capt_index], parent, pos);
	  if (c->what)
	    return insert_operand (p, c->what, indexes, 0, p);
	  else
	    return p;
	}
    }
  p = p->append_op (o, parent, pos);
  q = p;

  if (o->type == operand::OP_EXPR)
    {
      expr *e = static_cast<expr *> (o);
      for (unsigned i = 0; i < e->ops.length (); ++i)
	q = decision_tree::insert_operand (q, e->ops[i], indexes, i, p);		
    }

  return q;
}

void
decision_tree::insert (struct simplify *s, unsigned pattern_no)
{
  dt_operand *indexes[dt_simplify::capture_max];

  for (unsigned i = 0; i < s->matchers.length (); ++i)
    {
      if (s->matchers[i]->type != operand::OP_EXPR)
	continue;

      for (unsigned j = 0; j < dt_simplify::capture_max; ++j)
	indexes[j] = 0; 

      dt_node *p = decision_tree::insert_operand (root, s->matchers[i], indexes);
      p->append_simplify (s, pattern_no, indexes);
    }            
}

void
decision_tree::print_node (dt_node *p, FILE *f, unsigned indent)
{
  if (p->type == dt_node::DT_NODE)
    fprintf (f, "root");
  else
    {
      fprintf (f, "|");
      for (unsigned i = 0; i < indent; i++)
	fprintf (f, "-");

      if (p->type == dt_node::DT_OPERAND)
	{
	  dt_operand *dop = static_cast<dt_operand *>(p);
	  print_operand (dop->op, f, true); 
	} 
      else if (p->type == dt_node::DT_TRUE)
	fprintf (f, "true");
      else if (p->type == dt_node::DT_MATCH)
	fprintf (f, "match (%p)", (void *) ((static_cast<dt_operand *>(p))->match_dop));
      else if (p->type == dt_node::DT_SIMPLIFY)
	{
	  dt_simplify *s = static_cast<dt_simplify *> (p);
	  fprintf (f, "simplify_%u { ", s->pattern_no); 
	  for (unsigned i = 0; i < dt_simplify::capture_max; ++i)
	    fprintf (f, "%p, ", (void *) s->indexes[i]);
	  fprintf (f, " } "); 
	}
    }      

  fprintf (stderr, " (%p), %u, %u\n", (void *) p, p->level, p->kids.length ());

  for (unsigned i = 0; i < p->kids.length (); ++i)
    decision_tree::print_node (p->kids[i], f, indent + 2);
}


void
decision_tree::print (FILE *f)
{
  return decision_tree::print_node (root, f);
}

char *
dt_operand::get_name (char *name)
{
  if (parent->level == 1)
    sprintf (name, "op%u", pos);
  else if (parent->type == dt_node::DT_MATCH)
    return parent->get_name (name); 
  else
    sprintf (name, "o%u%u", parent->level, pos);
  return name;
}

void
dt_operand::gen_opname (char *name, unsigned pos)
{
  sprintf (name, "o%u%u", level, pos);
}

unsigned
dt_operand::gen_predicate (FILE *f, const char *opname)
{
  predicate *p = static_cast<predicate *> (op);

  fprintf (f, "if (%s (%s))\n", p->ident, opname);
  fprintf (f, "{\n");
  return 1;
}

unsigned
dt_operand::gen_match_op (FILE *f, const char *opname)
{
  char match_opname[20];
  match_dop->get_name (match_opname);
  fprintf (f, "if (%s == %s)\n", opname, match_opname);
  fprintf (f, "{\n");
  return 1;
}

unsigned
dt_operand::gen_gimple_expr_fn (FILE *f) 
{
  expr *e = static_cast<expr *> (op);
  unsigned n_ops = e->ops.length ();

  fn_id *op = static_cast <fn_id *> (e->operation->op);
  fprintf (f, "if (gimple_call_builtin_p (def_stmt, %s))\n", op->id);
  fprintf (f, "{\n");

  for (unsigned i = 0; i < n_ops; ++i)
    {
      char child_opname[20];
      gen_opname (child_opname, i); 

      fprintf (f, "tree %s = gimple_call_arg (def_stmt, %u);\n", child_opname, i);
      fprintf (f, "if ((%s = do_valueize (valueize, %s)) != 0)\n", child_opname, child_opname);
      fprintf (f, "{\n");
    } 

  return n_ops + 1;
}

unsigned
dt_operand::gen_gimple_expr_expr (FILE *f)
{
  expr *e = static_cast<expr *> (op);
  unsigned n_ops = e->ops.length (); 

  operator_id *op_id = static_cast <operator_id *> (e->operation->op);
  
  if (op_id->code == NOP_EXPR || op_id->code == CONVERT_EXPR)
    fprintf (f, " if (CONVERT_EXPR_CODE_P (gimple_assign_rhs_code (def_stmt)))");
  else
    fprintf (f, "if (gimple_assign_rhs_code (def_stmt) == %s)\n", op_id->id);

  fprintf (f, "{\n");
  
  for (unsigned i = 0; i < n_ops; ++i)
    {
      char child_opname[20];
      gen_opname (child_opname, i); 

      fprintf (f, "tree %s = gimple_assign_rhs%u (def_stmt);\n", child_opname, i + 1);
      fprintf (f, "if ((%s = do_valueize (valueize, %s)) != 0)\n", child_opname, child_opname);
      fprintf (f, "{\n");
    }      

  return n_ops + 1;
} 

void
dt_operand::gen_generic_expr_expr (FILE *f, expr *e, const char *opname,
				   bool valueize)
{
  unsigned n_ops = e->ops.length ();

  fprintf (f, "if (TREE_CODE (%s) == %s)\n", opname, e->operation->op->id);
  fprintf (f, "{\n");

  for (unsigned i = 0; i < n_ops; ++i)
    {
      char child_opname[20];
      gen_opname (child_opname, i);

      fprintf (f, "tree %s = TREE_OPERAND (%s, %u);\n", child_opname, opname, i);
      if (valueize)
	{
	  fprintf (f, "if ((%s = do_valueize (valueize, %s)) != 0)\n", child_opname, child_opname);
	  fprintf (f, "{\n");
	}
    }
}   

void
dt_operand::gen_generic_expr_fn (FILE *f, expr *e, const char *opname, bool valueize)
{
  unsigned n_ops = e->ops.length ();
  fn_id *op = static_cast <fn_id *> (e->operation->op);

  fprintf (f, "if (TREE_CODE (%s) == CALL_EXPR\n"
               "    && TREE_CODE (CALL_EXPR_FN (%s)) == ADDR_EXPR\n"
               "    && TREE_CODE (TREE_OPERAND (CALL_EXPR_FN (%s), 0)) == FUNCTION_DECL\n"
               "    && DECL_BUILT_IN_CLASS (TREE_OPERAND (CALL_EXPR_FN (%s), 0)) == BUILT_IN_NORMAL\n"
               "    && DECL_FUNCTION_CODE (TREE_OPERAND (CALL_EXPR_FN (%s), 0)) == %s)\n",
               opname, opname, opname, opname, opname, op->id);
  fprintf (f, "  {\n");

  for (unsigned i = 0; i < n_ops; ++i)
    {
      char child_opname[20];
      gen_opname (child_opname, i);

      fprintf (f, "tree %s = CALL_EXPR_ARG (%s, %u);\n", child_opname, opname, i);
      if (valueize)
	{
	  fprintf (f, "if ((%s = do_valueize (valueize, %s)) != 0)\n", child_opname, child_opname);
	  fprintf (f, "{\n");
	}
    }
}

unsigned
dt_operand::gen_generic_expr (FILE *f, const char *opname, bool valueize)
{
  expr *e = static_cast<expr *> (op);
  (e->operation->op->kind == id_base::CODE) ? gen_generic_expr_expr (f, e, opname, valueize) : gen_generic_expr_fn (f, e, opname, valueize);
  return valueize ? e->ops.length () + 1 : 1;
}

bool
dt_node::get_expr_code (enum tree_code& code)
{

  if (type != dt_node::DT_OPERAND)
    return false;

  dt_operand *dop = static_cast<dt_operand *> (this);
  
  if (dop->op->type != operand::OP_EXPR)
    return false;

  expr *e = static_cast<expr *> (dop->op);
  if (e->operation->op->kind != id_base::CODE)
    return false;

  operator_id *opr = static_cast<operator_id *> (e->operation->op);
  code = opr->code;
  return true;
}

bool
dt_node::is_gimple_expr ()
{
  enum tree_code code;

  if (!get_expr_code (code))
    return false;

  if (code == REALPART_EXPR || code == IMAGPART_EXPR
      || code == BIT_FIELD_REF || code == VIEW_CONVERT_EXPR)
    return false;

  dt_operand *dop = static_cast<dt_operand *> (this);

  if (dop->parent->type != dt_operand::DT_OPERAND)
    return true;

  if (dop->parent->get_expr_code (code) && code == COND_EXPR && dop->pos == 0)
    return false;

  return true;
}

bool
dt_node::is_gimple_fn ()
{
  if (type != dt_node::DT_OPERAND)
    return false;
  
  dt_operand *dop = static_cast<dt_operand *> (this);
  if (dop->op->type != operand::OP_EXPR)
    return false;

  expr *e = static_cast<expr *> (dop->op);
  return e->operation->op->kind == id_base::FN;
}

void
dt_operand::grok_kids (kids_type& ktype)
{

  for (unsigned i = 0; i < kids.length (); ++i)
    {
      if (kids[i]->is_gimple_expr ())
	ktype.gimple_exprs.safe_push (kids[i]);
      else if (kids[i]->is_gimple_fn ())
	ktype.fns.safe_push (kids[i]);
      else if (kids[i]->type == dt_node::DT_TRUE)
	ktype.true_operand = kids[i];
      else
	ktype.others.safe_push (kids[i]);
    }
}

void
dt_operand::gen_gimple_kids (FILE *f)
{
  kids_type ktype;
  grok_kids (ktype);

  char buf[128];
  char *kid_opname = buf;
 
  unsigned exprs_len = ktype.gimple_exprs.length ();
  unsigned fns_len = ktype.fns.length ();

  if (exprs_len || fns_len)
    {
      if (exprs_len)
	(static_cast<dt_operand *>(ktype.gimple_exprs[0]))->get_name (kid_opname);
      else
	(static_cast<dt_operand *>(ktype.fns[0]))->get_name (kid_opname);

      fprintf (f, "if (TREE_CODE (%s) == SSA_NAME)\n", kid_opname);
      fprintf (f, "{\n");
      fprintf (f, "gimple def_stmt = SSA_NAME_DEF_STMT (%s);\n", kid_opname);

      if (exprs_len)
	{
	  fprintf (f, "if (is_gimple_assign (def_stmt))\n");
	  fprintf (f, "{\n");
	  for (unsigned i = 0; i < exprs_len; ++i)
	    {
	      ktype.gimple_exprs[i]->gen_gimple (f);
	    }
	  fprintf (f, "}\n");
	}
  
      for (unsigned i = 0; i < fns_len; ++i) 
	ktype.fns[i]->gen_gimple (f);

      fprintf (f, "}\n");
    }
  
  for (unsigned i = 0; i < ktype.others.length (); ++i)
    ktype.others[i]->gen_gimple (f);

  if (ktype.true_operand)
    ktype.true_operand->gen_gimple (f);
}

void
dt_operand::gen_gimple (FILE *f)
{
  char opname[20];
  get_name (opname); 

  fprintf (f, "{\n");

  unsigned n_braces = 0;
 
  if (type == DT_OPERAND)
    switch (op->type)
      {
	case operand::OP_PREDICATE:
	  n_braces = gen_predicate (f, opname);
	  break;

	case operand::OP_EXPR:
	  if (is_gimple_expr ())
	    n_braces = gen_gimple_expr_expr (f);
	  else if (is_gimple_fn ())
	    n_braces = gen_gimple_expr_fn (f);
	  else
	    n_braces = gen_generic_expr (f, opname, true);
	  break;

	default:
	  gcc_unreachable ();
      }
  else if (type == DT_TRUE)
    ;
  else if (type == DT_MATCH)
    n_braces = gen_match_op (f, opname);
  else
    gcc_unreachable ();

  gen_gimple_kids (f);

  for (unsigned i = 0; i < n_braces; ++i)
    fprintf (f, "}\n");

  fprintf (f, "}\n");
}

void
dt_operand::gen_generic (FILE *f)
{
  char opname[20];
  get_name (opname); 

  fprintf (f, "{\n");

  unsigned n_braces = 0;
 
  if (type == DT_OPERAND)
    switch (op->type)
      {
	case operand::OP_PREDICATE:
	  n_braces = gen_predicate (f, opname);
	  break;

	case operand::OP_EXPR:
	  n_braces = gen_generic_expr (f, opname, false);
	  break;

	default:
	  gcc_unreachable ();
      }
  else if (type == DT_TRUE)
    ;
  else if (type == DT_MATCH)
    n_braces = gen_match_op (f, opname);
  else
    gcc_unreachable ();

  unsigned i;

  for (i = 0; i < kids.length (); ++i)
    kids[i]->gen_generic (f);

  for (i = 0; i < n_braces; ++i)
    fprintf (f, "}\n");
  
  fprintf (f, "}\n");
}

void
dt_simplify::gen_gimple (FILE *f)
{

  fprintf (f, "/* simplify %u */\n", pattern_no);

  fprintf (f, "{\n");
  fprintf (f, "tree captures[4] = {};\n");

  for (unsigned i = 0; i < dt_simplify::capture_max; ++i)
    if (indexes[i])
      {
	char opname[20];
	fprintf (f, "captures[%u] = %s;\n", i, indexes[i]->get_name (opname));
      }

  if (s->ifexpr)
	{
	  output_line_directive (f, s->ifexpr_location);
	  fprintf (f, "if (");
	  s->ifexpr->gen_transform (f, NULL, true);
	  fprintf (f, ")\n");
	  fprintf (f, "{\n");
	}
      output_line_directive (f, s->result_location);

      if (s->result->type == operand::OP_EXPR)
	{
	  expr *e = static_cast <expr *> (s->result);
	  fprintf (f, "*res_code = %s;\n", e->operation->op->id);
	  for (unsigned j = 0; j < e->ops.length (); ++j)
	    {
	      char dest[32];
	      snprintf (dest, 32, "  res_ops[%d]", j);
	      e->ops[j]->gen_transform (f, dest, true);
	    }
	  /* Re-fold the toplevel result.  It's basically an embedded
	     gimple_build w/o actually building the stmt.  */
	  fprintf (f, "gimple_resimplify%d (seq, res_code, type, "
		   "res_ops, valueize);\n", e->ops.length ());
	}
      else if (s->result->type == operand::OP_CAPTURE
	       || s->result->type == operand::OP_C_EXPR)
	{
	  s->result->gen_transform (f, "res_ops[0]", true);
	  fprintf (f, "*res_code = TREE_CODE (res_ops[0]);\n");
	}
      else
	gcc_unreachable ();

      fprintf (f, "return true;\n");
      if (s->ifexpr)
	fprintf (f, "}\n");

  fprintf (f, "}\n");
}


void
dt_simplify::gen_generic (FILE *f)
{

  fprintf (f, "/* simplify %u */\n", pattern_no);

  fprintf (f, "{\n");
  fprintf (f, "tree captures[4] = {};\n");

  for (unsigned i = 0; i < dt_simplify::capture_max; ++i)
    if (indexes[i])
      {
	char opname[20];
	fprintf (f, "captures[%u] = %s;\n", i, indexes[i]->get_name (opname));
      }

  if (s->ifexpr)
	{
	  output_line_directive (f, s->ifexpr_location);
	  fprintf (f, "if (");
	  s->ifexpr->gen_transform (f, NULL, false);
	  fprintf (f, ")\n");
	  fprintf (f, "{\n");
	}
      output_line_directive (f, s->result_location);

      if (s->result->type == operand::OP_EXPR)
	{
	  expr *e = static_cast <expr *> (s->result);
	  for (unsigned j = 0; j < e->ops.length (); ++j)
	    {
	      fprintf (f, "   tree res_op%d;\n", j);
	      char dest[32];
	      snprintf (dest, 32, "  res_op%d", j);
	      e->ops[j]->gen_transform (f, dest, false);
	    }
	  /* Re-fold the toplevel result.  */
	  if (e->operation->op->kind == id_base::CODE)
	    fprintf (f, "  return fold_build%d (%s, TREE_TYPE (res_op0)",
		     e->ops.length (), e->operation->op->id);
	  else
	    fprintf (f, "  return build_call_expr (builtin_decl_implicit (%s), %d",
		     e->operation->op->id, e->ops.length());
	  for (unsigned j = 0; j < e->ops.length (); ++j)
	    fprintf (f, ", res_op%d", j);
	  fprintf (f, ");\n");
	}
      else if (s->result->type == operand::OP_CAPTURE
	       || s->result->type == operand::OP_C_EXPR)
	{
	  fprintf (f, "  tree res;\n");
	  s->result->gen_transform (f, " res", false);
	  fprintf (f, "  return res;\n");
	}
      else
	gcc_unreachable ();

      if (s->ifexpr)
	fprintf (f, "}\n");

  fprintf (f, "}\n");
}



void
decision_tree::gen_gimple (FILE *f)
{
  for (unsigned n = 1; n <= 3; ++n)
    {
      fprintf (f, "\nstatic bool\n"
	       "gimple_match_and_simplify (code_helper *res_code, tree *res_ops,\n"
	       "                           gimple_seq *seq, tree (*valueize)(tree),\n"
	       "                           code_helper code, tree type");
      for (unsigned i = 0; i < n; ++i)
	fprintf (f, ", tree op%d", i);
      fprintf (f, ")\n");
      fprintf (f, "{\n");

      bool first = true;
      for (unsigned i = 0; i < root->kids.length (); i++)
	{
	  dt_operand *dop = static_cast<dt_operand *>(root->kids[i]);
	  expr *e = static_cast<expr *>(dop->op);
	  if (e->ops.length () != n)
	    continue;

	  if (!first)
	    fprintf (f, "else ");
	  fprintf (f, "if (code == %s)\n", e->operation->op->id);
	  fprintf (f, "{\n");
	  dop->gen_gimple_kids (f); 
	  fprintf (f, "}\n");

	  first = false;
	}

      fprintf (f, "return false;\n");
      fprintf (f, "}\n");
    }
}


void
decision_tree::gen_generic (FILE *f)
{
  for (unsigned n = 1; n <= 3; ++n)
    {
      fprintf (f, "\ntree\n"
	       "generic_match_and_simplify (enum tree_code code, tree type");
      for (unsigned i = 0; i < n; ++i)
	fprintf (f, ", tree op%d", i);
      fprintf (f, ")\n");
      fprintf (f, "{\n");

      bool first = true;
      for (unsigned i = 0; i < root->kids.length (); i++)
	{
	  dt_operand *dop = static_cast<dt_operand *>(root->kids[i]);
	  expr *e = static_cast<expr *>(dop->op);
	  if (e->ops.length () != n
	      /* Builtin simplifications are somewhat premature on
	         GENERIC.  The following drops patterns with outermost
		 calls.  It's easy to emit overloads for function code
		 though if necessary.  */
	      || e->operation->op->kind != id_base::CODE)
	    continue;

	  if (!first)
	    fprintf (f, "else ");
	  fprintf (f, "if (code == %s)\n", e->operation->op->id);
	  fprintf (f, "{\n");

	  for (unsigned j = 0; j < dop->kids.length (); ++j)
	    dop->kids[j]->gen_generic (f);

	  fprintf (f, "}\n");

	  first = false;
	}

      fprintf (f, "return NULL_TREE;\n");
      fprintf (f, "}\n");
    }
}


static void
outline_c_exprs (FILE *f, struct operand *op)
{
  if (op->type == operand::OP_C_EXPR)
    {
      c_expr *e = static_cast <c_expr *>(op);
      static unsigned fnnr = 1;
      if (e->nr_stmts > 1
	  && !e->fname)
	{
	  e->fname = (char *)xmalloc (sizeof ("cexprfn") + 4);
	  sprintf (e->fname, "cexprfn%d", fnnr);
	  fprintf (f, "\nstatic tree\ncexprfn%d (tree type, tree *captures)\n",
		   fnnr);
	  fprintf (f, "{\n");
	  unsigned stmt_nr = 1;
	  for (unsigned i = 0; i < e->code.length (); ++i)
	    {
	      const cpp_token *token = &e->code[i];

	      /* Replace captures for code-gen.  */
	      if (token->type == CPP_ATSIGN)
		{
		  const cpp_token *n = &e->code[i+1];
		  if (n->type == CPP_NUMBER
		      && !(n->flags & PREV_WHITE))
		    {
		      if (token->flags & PREV_WHITE)
			fputc (' ', f);
		      fprintf (f, "captures[%s]", n->val.str.text);
		      ++i;
		      continue;
		    }
		}

	      if (token->flags & PREV_WHITE)
		fputc (' ', f);

	      /* Output the token as string.  */
	      char *tk = (char *)cpp_token_as_text (e->r, token);
	      fputs (tk, f);

	      if (token->type == CPP_SEMICOLON)
		{
		  stmt_nr++;
		  if (stmt_nr == e->nr_stmts)
		    fputs ("\n return ", f);
		  else
		    fputc ('\n', f);
		}
	    }
	  fprintf (f, "}\n");
	  fnnr++;
	}
    }
  else if (op->type == operand::OP_CAPTURE)
    {
      capture *c = static_cast <capture *>(op);
      if (c->what)
	outline_c_exprs (f, c->what);
    }
  else if (op->type == operand::OP_EXPR)
    {
      expr *e = static_cast <expr *>(op);
      for (unsigned i = 0; i < e->ops.length (); ++i)
	outline_c_exprs (f, e->ops[i]);
    }
}

static void
write_header (FILE *f, vec<simplify *>& simplifiers, const char *head)
{
  fprintf (f, "/* Generated automatically by the program `genmatch' from\n");
  fprintf (f, "   a IL pattern matching and simplification description.  */\n");

  /* Include the header instead of writing it awkwardly quoted here.  */
  fprintf (f, "\n#include \"%s\"\n", head);

  /* Outline complex C expressions to helper functions.  */
  for (unsigned i = 0; i < simplifiers.length (); ++i)
    outline_c_exprs (stdout, simplifiers[i]->result);
}


/* Read the next non-whitespace token from R.  */

static const cpp_token *
next (cpp_reader *r)
{
  const cpp_token *token;
  do
    {
      token = cpp_get_token (r);
    }
  while (token->type == CPP_PADDING
	 && token->type != CPP_EOF);
  return token;
}

/* Peek at the next non-whitespace token from R.  */

static const cpp_token *
peek (cpp_reader *r)
{
  const cpp_token *token;
  unsigned i = 0;
  do
    {
      token = cpp_peek_token (r, i++);
    }
  while (token->type == CPP_PADDING
	 && token->type != CPP_EOF);
  return token;
}

/* Read the next token from R and assert it is of type TK.  */

static const cpp_token *
expect (cpp_reader *r, enum cpp_ttype tk)
{
  const cpp_token *token = next (r);
  if (token->type != tk)
    fatal_at (token, "expected %s, got %s",
	      cpp_type2name (tk, 0), cpp_type2name (token->type, 0));

  return token;
}

/* Consume the next token from R and assert it is of type TK.  */

static void
eat_token (cpp_reader *r, enum cpp_ttype tk)
{
  expect (r, tk);
}

/* Read the next token from R and assert it is of type CPP_STRING and
   return its value.  */

const char *
get_string (cpp_reader *r)
{
  const cpp_token *token = expect (r, CPP_STRING);
  return (const char *)token->val.str.text;
}

/* Read the next token from R and assert it is of type CPP_NAME and
   return its value.  */

const char *
get_ident (cpp_reader *r)
{
  const cpp_token *token = expect (r, CPP_NAME);
  return (const char *)CPP_HASHNODE (token->val.node.node)->ident.str;
}

/* Read the next token from R and assert it is of type CPP_NUMBER and
   return its value.  */

const char *
get_number (cpp_reader *r)
{
  const cpp_token *token = expect (r, CPP_NUMBER);
  return (const char *)token->val.str.text;
}


/* Parsing.  */

static e_operation *
parse_operation (cpp_reader *r)
{
  return new e_operation (get_ident (r));
}

static struct operand * parse_op (cpp_reader *r);

static struct operand *
parse_capture (cpp_reader *r, operand *op)
{
  eat_token (r, CPP_ATSIGN);
  return new capture (get_number (r), op);
}

/* Parse
     expr = (operation[capture] op...)  */
static struct operand *
parse_expr (cpp_reader *r)
{
  expr *e = new expr (parse_operation (r));
  const cpp_token *token = peek (r);
  operand *op;
  bool is_commutative = false;

  if (token->type == CPP_COLON
      && !(token->flags & PREV_WHITE))
    {
      eat_token (r, CPP_COLON);
      token = peek (r);
      if (token->type == CPP_NAME
	  && !(token->flags & PREV_WHITE))
	{
	  const char *s = get_ident (r);
	  if (s[0] == 'c' && !s[1])
	    is_commutative = true;
	  else
	    fatal_at (token, "predicates or flag %s not recognized", s);
	  token = peek (r);
	}
      else
	fatal_at (token, "expected flag or predicate");
    }

  if (token->type == CPP_ATSIGN
      && !(token->flags & PREV_WHITE))
    op = parse_capture (r, e);
  else
    op = e;
  do
    {
      const cpp_token *token = peek (r);
      if (token->type == CPP_CLOSE_PAREN)
	{
	  if (e->operation->op->kind == id_base::CODE)
	    {
	      operator_id *opr = static_cast <operator_id *> (e->operation->op);
	      if (e->ops.length () != opr->get_required_nargs ())
		fatal_at (token, "got %d operands instead of the required %d",
			  e->ops.length (), opr->get_required_nargs ());
	    }
	  if (is_commutative)
	    {
	      if (e->ops.length () == 2)
		e->operation->is_commutative = true;
	      else
		fatal_at (token, "only binary operators or function with two arguments can be marked commutative");
	    }
	  return op;
	}
      e->append_op (parse_op (r));
    }
  while (1);
}

/* Parse [({] .... [})] literally recording everything as string and only
   replacing captures.  */

static operand *
parse_c_expr (cpp_reader *r, cpp_ttype start)
{
  /* ???  Use an obstack to build the string.  */
  const cpp_token *token;
  cpp_ttype end;
  unsigned opencnt;
  vec<cpp_token> code = vNULL;
  unsigned nr_stmts = 0;
  eat_token (r, start);
  if (start == CPP_OPEN_PAREN)
    end = CPP_CLOSE_PAREN;
  else if (start == CPP_OPEN_BRACE)
    end = CPP_CLOSE_BRACE;
  else
    gcc_unreachable ();
  opencnt = 1;
  do
    {
      token = next (r);

      /* Count brace pairs to find the end of the expr to match.  */
      if (token->type == start)
	opencnt++;
      else if (token->type == end
	       && --opencnt == 0)
	break;

      if (token->type == CPP_SEMICOLON)
	nr_stmts++;

      /* Record the token.  */
      code.safe_push (*token);
    }
  while (1);
  return new c_expr (r, code, nr_stmts);
}

/* Parse
     op = predicate | ( expr ) */

static struct operand *
parse_op (cpp_reader *r)
{
  const cpp_token *token = peek (r);
  struct operand *op = NULL;
  if (token->type == CPP_OPEN_PAREN)
    {
      eat_token (r, CPP_OPEN_PAREN);
      op = parse_expr (r);
      eat_token (r, CPP_CLOSE_PAREN);
    }
  else if (token->type == CPP_OPEN_BRACE)
    {
      op = parse_c_expr (r, CPP_OPEN_BRACE);
    }
  else
    {
      /* Remaining ops are either empty or predicates  */
      if (token->type == CPP_NAME)
	{
	  op = new predicate (get_ident (r));
	  token = peek (r);
	  if (token->flags & PREV_WHITE)
	    return op;
	}
      else if (token->type != CPP_COLON
	       && token->type != CPP_ATSIGN)
	fatal_at (token, "expected expression or predicate");
      /* optionally followed by a capture and a predicate.  */
      if (token->type == CPP_COLON)
	fatal_at (token, "not implemented: predicate on leaf operand");
      if (token->type == CPP_ATSIGN)
	op = parse_capture (r, op);
    }

  return op;
}

/* Parse
     (define_match_and_simplify "<ident>"
        <op> <op>)  */

static simplify *
parse_match_and_simplify (cpp_reader *r, source_location match_location)
{
  const cpp_token *token = peek (r);
  const char *id;
  if (token->type == CPP_NAME)
    id = get_ident (r);
  else
    {
      static int cnt;
      char *tem;
      asprintf (&tem, "anon_%d", ++cnt);
      id = tem;
    }
  const cpp_token *loc = peek (r);
  struct operand *match = parse_op (r);
  if (match->type != operand::OP_EXPR)
    fatal_at (loc, "expected uncaptured expression");
  token = peek (r);
  /* Conditional if (....)  */
  struct operand *ifexpr = NULL;
  source_location ifexpr_location = 0;
  if (token->type == CPP_NAME)
    {
      const char *tem = get_ident (r);
      if (strcmp (tem, "if") != 0)
	fatal_at (token, "expected 'if' or expression");
      ifexpr_location = token->src_loc;
      ifexpr = parse_c_expr (r, CPP_OPEN_PAREN);
    }
  token = peek (r);
  return new simplify (id, commutate (match), match_location,
		       ifexpr, ifexpr_location, parse_op (r), token->src_loc);
}


static size_t
round_alloc_size (size_t s)
{
  return s;
}

int
main(int argc, char **argv)
{
  cpp_reader *r;
  const cpp_token *token;

  progname = "genmatch";

  if (argc != 3)
    return 1;

  bool gimple;
  if (strcmp (argv[1], "-gimple") == 0)
    gimple = true;
  else if (strcmp (argv[1], "-generic") == 0)
    gimple = false;
  else
    return 1;

  line_table = XCNEW (struct line_maps);
  linemap_init (line_table);
  line_table->reallocator = xrealloc;
  line_table->round_alloc_size = round_alloc_size;

  r = cpp_create_reader (CLK_GNUC99, NULL, line_table);
  cpp_callbacks *cb = cpp_get_callbacks (r);
  cb->error = error_cb;

  if (!cpp_read_main_file (r, argv[2]))
    return 1;
  cpp_define (r, gimple ? "GIMPLE=1": "GENERIC=1");

  /* Pre-seed operators.  */
  operators.create (1024);
#define DEFTREECODE(SYM, STRING, TYPE, NARGS) \
  add_operator (SYM, # SYM, # TYPE, NARGS);
#define END_OF_BASE_TREE_CODES
#include "tree.def"
#undef END_OF_BASE_TREE_CODES
#undef DEFTREECODE

  /* Pre-seed builtin functions.
     ???  Cannot use N (name) as that is targetm.emultls.get_address
     for BUILT_IN_EMUTLS_GET_ADDRESS ... */
#define DEF_BUILTIN(ENUM, N, C, T, LT, B, F, NA, AT, IM, COND) \
  add_builtin (ENUM, # ENUM);
#include "builtins.def"
#undef DEF_BUILTIN

  vec<simplify *> simplifiers = vNULL;

  do
    {
      token = peek (r);
      if (token->type == CPP_EOF)
	break;

      /* All clauses start with '('.  */
      eat_token (r, CPP_OPEN_PAREN);

      const char *id = get_ident (r);
      if (strcmp (id, "match_and_simplify") == 0)
	simplifiers.safe_push (parse_match_and_simplify (r, token->src_loc));
      else
	fatal_at (token, "expected 'match_and_simplify'");

      eat_token (r, CPP_CLOSE_PAREN);
    }
  while (1);

  for (unsigned i = 0; i < simplifiers.length (); ++i)
    print_matches (simplifiers[i]);

  decision_tree dt;
  for (unsigned i = 0; i < simplifiers.length (); ++i)
    dt.insert (simplifiers[i], i);

  dt.print (stderr);
 
  if (gimple)
    {
      write_header (stdout, simplifiers, "gimple-match-head.c");
      dt.gen_gimple (stdout);
    }
  else
    {
      write_header (stdout, simplifiers, "generic-match-head.c");
      dt.gen_generic (stdout);
    }

  cpp_finish (r, NULL);
  cpp_destroy (r);

  return 0;
}
