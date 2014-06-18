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

extern bool useless_type_conversion_p (Gimple::type, Gimple::type);

extern void gimple_set_body (Gimple::function_decl, gimple_seq);
extern gimple_seq gimple_body (Gimple::function_decl);
extern bool gimple_has_body_p (Gimple::function_decl);
extern const char *gimple_decl_printable_name (Gimple::decl, int);
extern Gimple::var_decl copy_var_decl (Gimple::decl, Gimple::identifier,
				       Gimple::type);
extern bool gimple_can_coalesce_p (Gimple::ssa_name, Gimple::ssa_name);
extern Gimple::identifier create_tmp_var_name (const char *);
extern Gimple::var_decl create_tmp_var_raw (Gimple::type, const char *);
extern Gimple::var_decl create_tmp_var (Gimple::type, const char *);
extern Gimple::var_decl create_tmp_reg (Gimple::type, const char *);
extern Gimple::var_decl create_tmp_reg_fn (struct function *,
					   Gimple::type, const char *);

extern void extract_ops_from_tree_1 (Gimple::value, enum tree_code *,
				     Gimple::value_ptr, Gimple::value_ptr,
				     Gimple::value_ptr);
extern void gimple_cond_get_ops_from_tree (Gimple::value , enum tree_code *,
					   Gimple::value_ptr,
					   Gimple::value_ptr);
extern bool is_gimple_lvalue (Gimple::value);
extern bool is_gimple_condexpr (Gimple::value);
extern bool is_gimple_address (Gimple::value);
extern bool is_gimple_invariant_address (Gimple::value);
extern bool is_gimple_ip_invariant_address (Gimple::value);
extern bool is_gimple_min_invariant (Gimple::value);
extern bool is_gimple_ip_invariant (Gimple::value);
extern bool is_gimple_reg (Gimple::value);
extern bool is_gimple_val (Gimple::value);
extern bool is_gimple_asm_val (Gimple::value);
extern bool is_gimple_min_lval (Gimple::value);
extern bool is_gimple_call_addr (Gimple::value);
extern bool is_gimple_mem_ref_addr (Gimple::value);
extern void mark_addressable (Gimple::value);
extern bool is_gimple_reg_rhs (Gimple::value);
  
/* Return true if a conversion from either type of TYPE1 and TYPE2
   to the other is not required.  Otherwise return false.  */

static inline bool
types_compatible_p (Gimple::type type1, Gimple::type type2)
{
  return (type1 == type2
	  || (useless_type_conversion_p (type1, type2)
	      && useless_type_conversion_p (type2, type1)));
}

/* Return true if TYPE is a suitable type for a scalar register variable.  */

static inline bool
is_gimple_reg_type (Gimple::type type)
{
  return !type->aggregate_type_p ();
}

/* Return true if T is a variable.  */

static inline bool
is_gimple_variable (Gimple::value t)
{
  enum tree_code c = t->code();
  return c  == VAR_DECL || c == PARM_DECL || c == RESULT_DECL || c == SSA_NAME;
}

/*  Return true if T is a GIMPLE identifier (something with an address).  */

static inline bool
is_gimple_id (Gimple::value t)
{
  return (is_gimple_variable (t)
	  || t->code () == FUNCTION_DECL
	  || t->code () == LABEL_DECL
	  || t->code () == CONST_DECL
	  /* Allow string constants, since they are addressable.  */
	  || t->code () == STRING_CST);
}

/* Return true if OP, an SSA name or a DECL is a virtual operand.  */

static inline bool
virtual_operand_p (Gimple::value op)
{
  Gimple::ssa_name ssa(op);
  if (ssa)
    {
      op = ssa->var ();
      if (!op)
	return false;
    }

  Gimple::var_decl decl(op);
  if (decl)
    return decl->is_virtual_operand ();

  return false;
}

/*  Return true if T is something whose address can be taken.  */

static inline bool
is_gimple_addressable (Gimple::value t)
{
  return (is_gimple_id (t) || handled_component_p (t)
	  || t->code () == MEM_REF);
}

/* Return true if T is a valid gimple constant.  */

static inline bool
is_gimple_constant (const Gimple::value t) 
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
extract_ops_from_tree (Gimple::value expr, enum tree_code *code, Gimple::value_ptr op0,
		       Gimple::value_ptr op1)
{
  Gimple::value op2;
  extract_ops_from_tree_1 (expr, code, op0, op1, &op2);
  gcc_assert (!op2);
}

/* Given a valid GIMPLE_CALL function address return the FUNCTION_DECL
   associated with the callee if known.  Otherwise return NULL_TREE.  */

static inline Gimple::function_decl
gimple_call_addr_fndecl (const Gimple::value val)
{
  Gimple::function_decl fndecl;
  Gimple::addr_expr addr = val;
  if (addr)
    {
      fndecl = as_a<Gimple::function_decl> (addr->expr ());

      if (!fndecl)
        {
	  Gimple::mem_ref mem = addr->expr ();
	  if (mem)
	    {
	      Gimple::addr_expr addr2 = mem->base ();
	      if (addr2 && integer_zerop (mem->offset ()))
		fndecl = addr2->expr ();
	    }
	}
    }
  return fndecl;
}

 
#endif /* GCC_GIMPLE_EXPR_H */
