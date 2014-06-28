/* Header file for gimple decl, type and expressions.
   Copyright (C) 2013-2014 Free Software Foundation, Inc.

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

#ifndef GCC_GIMPLE_EXPR_H
#define GCC_GIMPLE_EXPR_H

#include "gimple-value.h"

extern bool useless_type_conversion_p (G::type, G::type);

extern void gimple_set_body (G::function_decl, gimple_seq);
extern gimple_seq gimple_body (G::function_decl);
extern bool gimple_has_body_p (G::function_decl);
extern const char *gimple_decl_printable_name (G::decl, int);
extern G::var_decl copy_var_decl (G::decl, G::identifier,
				  G::type);
extern bool gimple_can_coalesce_p (G::ssa_name, G::ssa_name);
extern G::identifier create_tmp_var_name (const char *);
extern G::var_decl create_tmp_var_raw (G::type, const char *);
extern G::var_decl create_tmp_var (G::type, const char *);
extern G::var_decl create_tmp_reg (G::type, const char *);
extern G::var_decl create_tmp_reg_fn (struct function *,
				      G::type, const char *);

extern void extract_ops_from_tree_1 (G::value, enum tree_code *,
				     G::value_ptr, G::value_ptr,
				     G::value_ptr);
extern void gimple_cond_get_ops_from_tree (G::value , enum tree_code *,
					   G::value_ptr,
					   G::value_ptr);
extern bool is_gimple_lvalue (G::value);
extern bool is_gimple_condexpr (G::value);
extern bool is_gimple_address (G::value);
extern bool is_gimple_invariant_address (G::value);
extern bool is_gimple_ip_invariant_address (G::value);
extern bool is_gimple_min_invariant (G::value);
extern bool is_gimple_ip_invariant (G::value);
extern bool is_gimple_reg (G::value);
extern bool is_gimple_val (G::value);
extern bool is_gimple_asm_val (G::value);
extern bool is_gimple_min_lval (G::value);
extern bool is_gimple_call_addr (G::value);
extern bool is_gimple_mem_ref_addr (G::value);
extern void mark_addressable (G::value);
extern bool is_gimple_reg_rhs (G::value);
  
/* Return true if a conversion from either type of TYPE1 and TYPE2
   to the other is not required.  Otherwise return false.  */

static inline bool
types_compatible_p (G::type type1, G::type type2)
{
  return (type1 == type2
	  || (useless_type_conversion_p (type1, type2)
	      && useless_type_conversion_p (type2, type1)));
}

/* Return true if TYPE is a suitable type for a scalar register variable.  */

static inline bool
is_gimple_reg_type (G::type type)
{
  return !type->aggregate_type_p ();
}

/* Return true if T is a variable.  */

static inline bool
is_gimple_variable (G::value t)
{
  return is_a<G::var_decl> (t) || is_a<G::ssa_name> (t) ||
	  is_a<G::parm_decl> (t) || is_a<G::result_decl> (t);
}

/*  Return true if T is a GIMPLE identifier (something with an address).  */

static inline bool
is_gimple_id (G::value t)
{
  return (is_gimple_variable (t)
	  || is_a<G::function_decl> (t)
	  || is_a<G::label_decl> (t)
	  || is_a<G::const_decl> (t)
	  /* Allow string constants, since they are addressable.  */
	  || is_a<G::string_cst> (t));
}

/* Return true if OP, an SSA name or a DECL is a virtual operand.  */

static inline bool
virtual_operand_p (G::value op)
{
  G::ssa_name ssa = dyn_cast<G::ssa_name> (op);
  if (ssa)
    {
      op = ssa->var ();
      if (!op)
	return false;
    }

  G::var_decl decl = dyn_cast<G::var_decl> (op);
  if (decl)
    return decl->is_virtual_operand ();

  return false;
}

/*  Return true if T is something whose address can be taken.  */

static inline bool
is_gimple_addressable (G::value t)
{
  return (is_gimple_id (t) || handled_component_p (t)
	  || is_a<G::mem_ref> (t));
}

/* Return true if T is a valid gimple constant.  */

static inline bool
is_gimple_constant (const G::value t) 
{
  switch (t->code ())
    {
    case INTEGER_CST:
    case REAL_CST:
    case FIXED_CST:
    case STRING_CST:
    case COMPLEX_CST:
    case VECTOR_CST:
      return true;

    default:
      return false;
    }
}

/* A wrapper around extract_ops_from_tree_1, for callers which expect
   to see only a maximum of two operands.  */

static inline void
extract_ops_from_tree (G::value expr, enum tree_code *code, G::value_ptr op0,
		       G::value_ptr op1)
{
  G::value op2;
  extract_ops_from_tree_1 (expr, code, op0, op1, &op2);
  gcc_assert (!op2);
}

/* Given a valid GIMPLE_CALL function address return the FUNCTION_DECL
   associated with the callee if known.  Otherwise return NULL_TREE.  */

static inline G::function_decl
gimple_call_addr_fndecl (const G::value val)
{
  G::function_decl fndecl;
  G::addr_expr addr = val ? dyn_cast<G::addr_expr> (val) : NULL_GIMPLE;
  if (addr)
    {
      fndecl = dyn_cast<G::function_decl> (addr->expr ());
      if (!fndecl)
        {
	  G::mem_ref mem = dyn_cast<G::mem_ref> (addr->expr ());
	  if (mem)
	    {
	      G::addr_expr addr2 = dyn_cast<G::addr_expr> (mem->base ());
	      if (addr2 && integer_zerop (mem->offset ()))
		fndecl = dyn_cast<G::function_decl> (addr2->expr ());
	    }
	}
    }
  return fndecl;
}
 
#endif /* GCC_GIMPLE_EXPR_H */
