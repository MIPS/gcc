/* Gimple decl, type, and expression support functions.

   Copyright (C) 2007-2014 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>

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
#include "tm.h"
#include "gimple-tree.h"

#include "pointer-set.h"
#include "basic-block.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "tree-eh.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "fold-const.h"
#include "gimple.h"
#include "stringpool.h"
#include "stor-layout.h"
#include "demangle.h"
#include "gimple-ssa.h"
#include "gimplify.h"

/* ----- Type related -----  */

/* Return true if the conversion from INNER_TYPE to OUTER_TYPE is a
   useless type conversion, otherwise return false.

   This function implicitly defines the middle-end type system.  With
   the notion of 'a < b' meaning that useless_type_conversion_p (a, b)
   holds and 'a > b' meaning that useless_type_conversion_p (b, a) holds,
   the following invariants shall be fulfilled:

     1) useless_type_conversion_p is transitive.
	If a < b and b < c then a < c.

     2) useless_type_conversion_p is not symmetric.
	From a < b does not follow a > b.

     3) Types define the available set of operations applicable to values.
	A type conversion is useless if the operations for the target type
	is a subset of the operations for the source type.  For example
	casts to void* are useless, casts from void* are not (void* can't
	be dereferenced or offsetted, but copied, hence its set of operations
	is a strict subset of that of all other data pointer types).  Casts
	to const T* are useless (can't be written to), casts from const T*
	to T* are not.  */

bool
useless_type_conversion_p (G::type outer_type, G::type inner_type)
{
  G::array_type inner_array;
  G::array_type outer_array;
  G::function_or_method_type inner_func;
  G::function_or_method_type outer_func;

  /* Do the following before stripping toplevel qualifiers.  */
  if (inner_type->pointer_type_p () && outer_type->pointer_type_p ())
    {
      /* Do not lose casts between pointers to different address spaces.  */
      if (outer_type->type()->addr_space ()
	  != inner_type->type()->addr_space ())
	return false;
    }

  /* From now on qualifiers on value types do not matter.  */
  inner_type = inner_type->main_variant ();
  outer_type = outer_type->main_variant ();

  if (inner_type == outer_type)
    return true;

  /* If we know the canonical types, compare them.  */
  if (inner_type->canonical ()
      && inner_type->canonical ()  == outer_type->canonical ())
    return true;

  /* Changes in machine mode are never useless conversions unless we
     deal with aggregate types in which case we defer to later checks.  */
  if (inner_type->mode () != outer_type->mode ()
      && !inner_type->aggregate_type_p ())
    return false;

  /* If both the inner and outer types are integral types, then the
     conversion is not necessary if they have the same mode and
     signedness and precision, and both or neither are boolean.  */
  if (inner_type->integral_type_p ()
      && outer_type->integral_type_p ())
    {
      /* Preserve changes in signedness or precision.  */
      if (inner_type->type_unsigned () != outer_type->type_unsigned ()
	  || inner_type->precision () != outer_type->precision ())
	return false;

      /* Preserve conversions to/from BOOLEAN_TYPE if types are not
	 of precision one.  */
      if (inner_type->boolean_type_p () != outer_type->boolean_type_p ()
	  && outer_type->precision () != 1)
	return false;

      /* We don't need to preserve changes in the types minimum or
	 maximum value in general as these do not generate code
	 unless the types precisions are different.  */
      return true;
    }

  /* Scalar floating point types with the same mode are compatible.  */
  else if (inner_type->scalar_float_type_p ()
	   && outer_type->scalar_float_type_p ())
    return true;

  /* Fixed point types with the same mode are compatible.  */
  else if (inner_type->fixed_point_type_p ()
	   && outer_type->fixed_point_type_p ())
    return true;

  /* We need to take special care recursing to pointed-to types.  */
  else if (inner_type->pointer_type_p ()
	   && outer_type->pointer_type_p ())
    {
      /* Do not lose casts to function pointer types.  */
      if (outer_type->type()->method_or_function_type_p ()
	  && !inner_type->type()->method_or_function_type_p ())
	return false;

      /* We do not care for const qualification of the pointed-to types
	 as const qualification has no semantic value to the middle-end.  */

      /* Otherwise pointers/references are equivalent.  */
      return true;
    }

  /* Recurse for complex types.  */
  else if (inner_type->complex_type_p () && outer_type->complex_type_p ())
    return useless_type_conversion_p (outer_type->type (),
				      inner_type->type ());

  /* Recurse for vector types with the same number of subparts.  */
  else if (inner_type->vector_type_p () && outer_type->vector_type_p ()
	   && inner_type->precision () == outer_type->precision ())
    return useless_type_conversion_p (outer_type->type (),
				      inner_type->type());

  else if ((inner_array = dyn_cast<G::array_type> (inner_type))
	    && (outer_array = dyn_cast<G::array_type> (outer_type)))
    {
      /* Preserve string attributes.  */
      if (inner_array->string_flag () != outer_array->string_flag ())
	return false;

      G::numerical_type inner_domain = inner_array->domain ();
      G::numerical_type outer_domain = outer_array->domain ();

      /* Conversions from array types with unknown extent to
	 array types with known extent are not useless.  */
      if (!inner_domain && outer_domain)
	return false;

      /* Nor are conversions from array types with non-constant size to
         array types with constant size or to different size.  */
      if (outer_array->size ()
	  && is_a<G::integer_cst> (outer_array->size ()) 
	  && (!inner_array->size ()
	      || !is_a<G::integer_cst> (inner_array->size ())
	      || !gimple_int_cst_equal (outer_array->size (),
					inner_array->size ())))
	return false;

      /* Check conversions between arrays with partially known extents.
	 If the array min/max values are constant they have to match.
	 Otherwise allow conversions to unknown and variable extents.
	 In particular this declares conversions that may change the
	 mode to BLKmode as useless.  */
      if (inner_domain && outer_domain && inner_domain != outer_domain)
	{
	  G::value inner_min = inner_domain->min_value ();
	  G::value outer_min = outer_domain->min_value ();
	  G::value inner_max = inner_domain->max_value ();
	  G::value outer_max = outer_domain->max_value ();

	  /* After gimplification a variable min/max value carries no
	     additional information compared to a NULL value.  All that
	     matters has been lowered to be part of the IL.  */
	  if (inner_min && !is_a<G::integer_cst>(inner_min))
	    inner_min = NULL_GIMPLE;
	  if (outer_min && !is_a<G::integer_cst>(outer_min))
	    outer_min = NULL_GIMPLE;
	  if (inner_max && !is_a<G::integer_cst>(inner_max))
	    inner_max = NULL_GIMPLE;
	  if (outer_max && !is_a<G::integer_cst>(outer_max))
	    outer_max = NULL_GIMPLE;

	  /* Conversions NULL / variable <- cst are useless, but not
	     the other way around.  */
	  if (outer_min && (!inner_min
			    || !gimple_int_cst_equal (inner_min, outer_min)))
	    return false;
	  if (outer_max && (!inner_max
			    || !gimple_int_cst_equal (inner_max, outer_max)))
	    return false;
	}

      /* Recurse on the element check.  */
      return useless_type_conversion_p (outer_array->type (),
					inner_array->type ());
    }

  else if ((inner_func = dyn_cast<G::function_or_method_type> (inner_type))
	    && (outer_func = dyn_cast<G::function_or_method_type> (outer_type))
	   && (inner_type->code () == outer_type->code ()))
    {
      G::type_list outer_parm, inner_parm;

      /* If the return types are not compatible bail out.  */
      if (!useless_type_conversion_p (outer_func->type (),
				      inner_func->type ()))
	return false;

      /* Method types should belong to a compatible base class.  */
      if (is_a<G::method_type>(inner_func) 
	  && !useless_type_conversion_p (
			  as_a<G::method_type>(outer_func)->basetype (),
			  as_a<G::method_type>(inner_func)->basetype ()))
	return false;

      /* A conversion to an unprototyped argument list is ok.  */
      if (!prototype_p (outer_func))
	return true;

      /* If the unqualified argument types are compatible the conversion
	 is useless.  */
      if (outer_func->arg_types () == inner_func->arg_types ())
	return true;

      for (outer_parm = outer_func->arg_types (),
	   inner_parm = inner_func->arg_types ();
	   outer_parm && inner_parm;
	   outer_parm = outer_func->chain (),
	   inner_parm = inner_func->chain ())
	if (!useless_type_conversion_p (outer_parm->value()->main_variant (),
					inner_parm->value()->main_variant ()))
	  return false;

      /* If there is a mismatch in the number of arguments the functions
	 are not compatible.  */
      if (outer_parm || inner_parm)
	return false;

      /* Defer to the target if necessary.  */
      if (inner_func->attributes () || outer_func->attributes ())
	return comp_type_attributes (outer_func, inner_func) != 0;

      return true;
    }

  /* For aggregates we rely on TYPE_CANONICAL exclusively and require
     explicit conversions for types involving to be structurally
     compared types.  */
  else if (inner_type->aggregate_type_p ()
	   && inner_type->code () ==  outer_type->code ())
    return false;

  return false;
}


/* ----- Decl related -----  */

/* Set sequence SEQ to be the GIMPLE body for function FN.  */

void
gimple_set_body (G::function_decl fndecl, gimple_seq seq)
{
  struct function *fn = fndecl->function ();
  if (fn == NULL)
    {
      /* If FNDECL still does not have a function structure associated
	 with it, then it does not make sense for it to receive a
	 GIMPLE body.  */
      gcc_assert (seq == NULL);
    }
  else
    fn->gimple_body = seq;
}


/* Return the body of GIMPLE statements for function FN.  After the
   CFG pass, the function body doesn't exist anymore because it has
   been split up into basic blocks.  In this case, it returns
   NULL.  */

gimple_seq
gimple_body (G::function_decl fndecl)
{
  struct function *fn = fndecl->function ();
  return fn ? fn->gimple_body : NULL;
}

/* Return true when FNDECL has Gimple body either in unlowered
   or CFG form.  */
bool
gimple_has_body_p (G::function_decl fndecl)
{
  struct function *fn = fndecl->function ();
  return (gimple_body (fndecl) || (fn && fn->cfg));
}

/* Return a printable name for symbol DECL.  */

const char *
gimple_decl_printable_name (G::decl decl, int verbosity)
{
  if (!decl->name ())
    return NULL;

  if (decl->assembler_name_set_p ())
    {
      const char *str, *mangled_str;
      int dmgl_opts = DMGL_NO_OPTS;

      if (verbosity >= 2)
	{
	  dmgl_opts = DMGL_VERBOSE
		      | DMGL_ANSI
		      | DMGL_GNU_V3
		      | DMGL_RET_POSTFIX;
	  if (is_a<G::function_decl> (decl))
	    dmgl_opts |= DMGL_PARAMS;
	}

      mangled_str = decl->assembler_name()->pointer ();
      str = cplus_demangle_v3 (mangled_str, dmgl_opts);
      return (str) ? str : mangled_str;
    }

  return decl->name()->pointer ();
}


/* Create a new VAR_DECL and copy information from VAR to it.  */

G::var_decl
copy_var_decl (G::decl var, G::identifier name, G::type type)
{
  G::var_decl copy = build_var_decl (var->source_location (), name, type);
  copy->set_addressable (var->addressable ());
  copy->set_this_volatile (var->this_volatile ());
  copy->set_gimple_reg_p (var->gimple_reg_p ());
  copy->set_artificial (var->artificial ());
  copy->set_ignored_p (var->ignored_p ());
  copy->set_context (var->context ());
  copy->set_no_warning (var->no_warning ());
  copy->set_used (true);
  copy->set_seen_in_bind_expr_p (true);
  copy->set_attributes (var->attributes ());

  return copy;
}

/* Given SSA_NAMEs NAME1 and NAME2, return true if they are candidates for
   coalescing together, false otherwise.

   This must stay consistent with var_map_base_init in tree-ssa-live.c.  */

bool
gimple_can_coalesce_p (G::ssa_name name1, G::ssa_name name2)
{
  /* First check the SSA_NAME's associated DECL.  We only want to
     coalesce if they have the same DECL or both have no associated DECL.  */
  G::decl var1 = name1->var ();
  G::decl var2 = name2->var ();

  var1 = (var1 && (!is_a<G::var_decl> (var1) || !var1->ignored_p ())) ? var1 :
									NULL_GIMPLE;
  var2 = (var2 && (!is_a<G::var_decl> (var2) || !var2->ignored_p ())) ? var2 :
									NULL_GIMPLE;
  if (var1 != var2)

    return false;

  /* Now check the types.  If the types are the same, then we should
     try to coalesce V1 and V2.  */
  G::type t1 = name1->type ();
  G::type t2 = name2->type ();
  if (t1 == t2)
    return true;

  /* If the types are not the same, check for a canonical type match.  This
     (for example) allows coalescing when the types are fundamentally the
     same, but just have different names. 

     Note pointer types with different address spaces may have the same
     canonical type.  Those are rejected for coalescing by the
     types_compatible_p check.  */
  if (t1->canonical () && t1->canonical () == t2->canonical ()
      && types_compatible_p (t1, t2))
    return true;

  return false;
}

/* Strip off a legitimate source ending from the input string NAME of
   length LEN.  Rather than having to know the names used by all of
   our front ends, we strip off an ending of a period followed by
   up to five characters.  (Java uses ".class".)  */

static inline void
remove_suffix (char *name, int len)
{
  int i;

  for (i = 2;  i < 8 && len > i;  i++)
    {
      if (name[len - i] == '.')
	{
	  name[len - i] = '\0';
	  break;
	}
    }
}

/* Create a new temporary name with PREFIX.  Return an identifier.  */

static GTY(()) unsigned int tmp_var_id_num;

G::identifier
create_tmp_var_name (const char *prefix)
{
  char *tmp_name;

  if (prefix)
    {
      char *preftmp = ASTRDUP (prefix);

      remove_suffix (preftmp, strlen (preftmp));
      clean_symbol_name (preftmp);

      prefix = preftmp;
    }

  ASM_FORMAT_PRIVATE_NAME (tmp_name, prefix ? prefix : "T", tmp_var_id_num++);
  return get_identifier (tmp_name);
}

/* Create a new temporary variable declaration of type TYPE.
   Do NOT push it into the current binding.  */

G::var_decl
create_tmp_var_raw (G::type type, const char *prefix)
{
  G::var_decl tmp_var;

  tmp_var = build_var_decl (input_location,
			    prefix ? create_tmp_var_name (prefix) : NULL_GIMPLE,
			    type);

  /* The variable was declared by the compiler.  */
  tmp_var->set_artificial (true);
  /* And we don't want debug info for it.  */
  tmp_var->set_ignored_p (true);

  /* Make the variable writable.  */
  tmp_var->set_readonly (false);

  tmp_var->set_external (false);
  tmp_var->set_static_p (false);
  tmp_var->set_used (true);

  return tmp_var;
}

/* Create a new temporary variable declaration of type TYPE.  DO push the
   variable into the current binding.  Further, assume that this is called
   only from gimplification or optimization, at which point the creation of
   certain types are bugs.  */

G::var_decl
create_tmp_var (G::type type, const char *prefix)
{
  G::var_decl tmp_var;

  /* We don't allow types that are addressable (meaning we can't make copies),
     or incomplete.  We also used to reject every variable size objects here,
     but now support those for which a constant upper bound can be obtained.
     The processing for variable sizes is performed in gimple_add_tmp_var,
     point at which it really matters and possibly reached via paths not going
     through this function, e.g. after direct calls to create_tmp_var_raw.  */
  gcc_assert (!type->addressable () && type->complete_type_p ());

  tmp_var = create_tmp_var_raw (type, prefix);
  gimple_add_tmp_var (tmp_var);
  return tmp_var;
}

/* Create a new temporary variable declaration of type TYPE by calling
   create_tmp_var and if TYPE is a vector or a complex number, mark the new
   temporary as gimple register.  */

G::var_decl
create_tmp_reg (G::type type, const char *prefix)
{
  G::var_decl tmp;

  tmp = create_tmp_var (type, prefix);
  if (type->complex_type_p () || type->vector_type_p ())
    tmp->set_gimple_reg_p (true);

  return tmp;
}

/* Create a new temporary variable declaration of type TYPE by calling
   create_tmp_var and if TYPE is a vector or a complex number, mark the new
   temporary as gimple register.  */

G::var_decl
create_tmp_reg_fn (struct function *fn, G::type type, const char *prefix)
{
  G::var_decl tmp;

  tmp = create_tmp_var_raw (type, prefix);
  gimple_add_tmp_var_fn (fn, tmp);
  if (is_a<G::complex_type> (type) || is_a<G::vector_type> (type))
    tmp->set_gimple_reg_p (true);

  return tmp;
}


/* ----- Expression related -----  */

/* Extract the operands and code for expression EXPR into *SUBCODE_P,
   *OP1_P, *OP2_P and *OP3_P respectively.  */

void
extract_ops_from_tree_1 (G::value expr, enum tree_code *subcode_p,
			 G::value_ptr op1_p,
			 G::value_ptr op2_p, G::value_ptr op3_p)
{
  enum gimple_rhs_class grhs_class = get_gimple_rhs_class (expr);
  *subcode_p = expr->code ();

  if (grhs_class == GIMPLE_TERNARY_RHS)
    {
      *op1_p = expr->op (0);
      *op2_p = expr->op (1);
      *op3_p = expr->op (2);
    }
  else if (grhs_class == GIMPLE_BINARY_RHS)
    {
      *op1_p = expr->op (0);
      *op2_p = expr->op (1);
      *op3_p = NULL_GIMPLE;
    }
  else if (grhs_class == GIMPLE_UNARY_RHS)
    {
      *op1_p = expr->op (0);
      *op2_p = NULL_GIMPLE;
      *op3_p = NULL_GIMPLE;
    }
  else if (grhs_class == GIMPLE_SINGLE_RHS)
    {
      *op1_p = expr;
      *op2_p = NULL_GIMPLE;
      *op3_p = NULL_GIMPLE;
    }
  else
    gcc_unreachable ();
}

/* Extract operands for a GIMPLE_COND statement out of COND_EXPR tree COND.  */

void
gimple_cond_get_ops_from_tree (G::value cond, enum tree_code *code_p,
                               G::value_ptr lhs_p, G::value_ptr rhs_p)
{
  gcc_assert (is_a<G::comparison> (cond) || is_a<G::truth_not_expr> (cond)
	      || is_gimple_min_invariant (cond) || ssa_var_p (cond));


  extract_ops_from_tree (cond, code_p, lhs_p, rhs_p);

  /* Canonicalize conditionals of the form 'if (!VAL)'.  */
  if (is_a<G::truth_not_expr> (cond))
    {
      *code_p = EQ_EXPR;
      gcc_assert (*lhs_p && *rhs_p == NULL_GIMPLE);
      *rhs_p = build_zero_cst ((*lhs_p)->type ());
    }
  /* Canonicalize conditionals of the form 'if (VAL)'  */
  else if (!is_a<G::comparison> (cond))
    {
      *code_p = NE_EXPR;
      gcc_assert (*lhs_p && *rhs_p == NULL_GIMPLE);
      *rhs_p = build_zero_cst ((*lhs_p)->type ());
    }
}

/*  Return true if T is a valid LHS for a GIMPLE assignment expression.  */

bool
is_gimple_lvalue (G::value t)
{
  return (is_gimple_addressable (t)
	  || is_a<G::with_size_expr> (t)
	  /* These are complex lvalues, but don't have addresses, so they
	     go here.  */
	  || is_a<G::bit_field_ref> (t));
}

/*  Return true if T is a GIMPLE condition.  */

bool
is_gimple_condexpr (G::value t)
{
  if (is_gimple_val (t))
    return true;

  G::comparison cmp = dyn_cast<G::comparison> (t);
  if (cmp)
    return !tree_could_throw_p (cmp) 
	   && is_gimple_val (cmp->op1 ())
	   && is_gimple_val (cmp->op2 ());

  return false;
}

/* Return true if T is a gimple address.  */

bool
is_gimple_address (G::value t)
{

  G::addr_expr val = dyn_cast<G::addr_expr> (t); 
  if (!val)
    return false;

  G::value op = val->expr ();

  while (handled_component_p (op))
    {
      if ((is_a<G::array_ref> (op) || is_a<G::array_range_ref> (op))
	  && !is_gimple_val (op->op (1)))
	    return false;

      op = op->op(0);
    }

  if (is_a<G::constant> (op) || is_a<G::mem_ref> (op))
    return true;

  switch (op->code ())
    {
    case PARM_DECL:
    case RESULT_DECL:
    case LABEL_DECL:
    case FUNCTION_DECL:
    case VAR_DECL:
    case CONST_DECL:
      return true;

    default:
      return false;
    }
}

/* Return true if T is a gimple invariant address.  */

bool
is_gimple_invariant_address (G::value t)
{

  G::addr_expr val = dyn_cast<G::addr_expr> (t); 
  if (!val)
    return false;

  G::value op;

  op = strip_invariant_refs (val->expr ());
  if (!op)
    return false;

  G::mem_ref mem = dyn_cast<G::mem_ref> (op);
  if (mem)
    {
      G::addr_expr val2 = dyn_cast<G::addr_expr> (mem->base ());
      return (val2 && (is_a<G::constant> (val2->expr ())
		       || decl_address_invariant_p (val2->expr ())));
    }

  return is_a<G::constant> (op) || decl_address_invariant_p (op);
}

/* Return true if T is a gimple invariant address at IPA level
   (so addresses of variables on stack are not allowed).  */

bool
is_gimple_ip_invariant_address (G::value t)
{
  G::addr_expr val = dyn_cast<G::addr_expr> (t); 
  if (!val)
    return false;

  G::value op;

  op = strip_invariant_refs (val->expr ());
  if (!op)
    return false;

  G::mem_ref mem = dyn_cast<G::mem_ref> (op);
  if (mem)
    {
      G::addr_expr val2 = dyn_cast<G::addr_expr> (mem->base ());
      return (val2
	      && (is_a<G::constant> (val2->expr ())
		  || decl_address_ip_invariant_p (val2->expr ())));
    }

  return is_a<G::constant> (op) || decl_address_ip_invariant_p (op);
}

/* Return true if T is a GIMPLE minimal invariant.  It's a restricted
   form of function invariant.  */

bool
is_gimple_min_invariant (G::value t)
{
  if (is_a<G::addr_expr> (t))
    return is_gimple_invariant_address (t);

  return is_gimple_constant (t);
}

/* Return true if T is a GIMPLE interprocedural invariant.  It's a restricted
   form of gimple minimal invariant.  */

bool
is_gimple_ip_invariant (G::value t)
{
  if (is_a<G::addr_expr> (t))
    return is_gimple_ip_invariant_address (t);

  return is_gimple_constant (t);
}

/* Return true if T is a non-aggregate register variable.  */

bool
is_gimple_reg (G::value t)
{
  if (virtual_operand_p (t))
    return false;

  if (is_a<G::ssa_name> (t))
    return true;

  if (!is_gimple_variable (t))
    return false;

  if (!is_gimple_reg_type (t->type ()))
    return false;

  /* A volatile decl is not acceptable because we can't reuse it as
     needed.  We need to copy it into a temp first.  */
  if (t->this_volatile ())
    return false;

  /* We define "registers" as things that can be renamed as needed,
     which with our infrastructure does not apply to memory.  */
  if (needs_to_live_in_memory (t))
    return false;

  /* Hard register variables are an interesting case.  For those that
     are call-clobbered, we don't know where all the calls are, since
     we don't (want to) take into account which operations will turn
     into libcalls at the rtl level.  For those that are call-saved,
     we don't currently model the fact that calls may in fact change
     global hard registers, nor do we examine ASM_CLOBBERS at the tree
     level, and so miss variable changes that might imply.  All around,
     it seems safest to not do too much optimization with these at the
     tree level at all.  We'll have to rely on the rtl optimizers to
     clean this up, as there we've got all the appropriate bits exposed.  */
  if (is_a<G::var_decl> (t) && as_a<G::var_decl> (t)->hard_register ())
    return false;

  /* Complex and vector values must have been put into SSA-like form.
     That is, no assignments to the individual components.  */
  if (is_a<G::complex_type> (t->type ())
      || is_a<G::vector_type> (t->type ()))
    return as_a<G::decl> (t)->gimple_reg_p ();

  return true;
}


/* Return true if T is a GIMPLE rvalue, i.e. an identifier or a constant.  */

bool
is_gimple_val (G::value t)
{
  /* Make loads from volatiles and memory vars explicit.  */
  if (is_gimple_variable (t)
      && is_gimple_reg_type (t->type ())
      && !is_gimple_reg (t))
    return false;

  return (is_gimple_variable (t) || is_gimple_min_invariant (t));
}

/* Similarly, but accept hard registers as inputs to asm statements.  */

bool
is_gimple_asm_val (G::value t)
{
  if (is_a<G::var_decl> (t) && as_a<G::var_decl> (t)->hard_register ())
    return true;

  return is_gimple_val (t);
}

/* Return true if T is a GIMPLE minimal lvalue.  */

bool
is_gimple_min_lval (G::value t)
{
  if (!(t = strip_invariant_refs (t)))
    return false;
  return (is_gimple_id (t) || is_a<G::mem_ref> (t));
}

/* Return true if T is a valid function operand of a CALL_EXPR.  */

bool
is_gimple_call_addr (G::value t)
{
  return (is_a<G::obj_type_ref> (t) || is_gimple_val (t));
}

/* Return true if T is a valid address operand of a MEM_REF.  */

bool
is_gimple_mem_ref_addr (G::value t)
{
  if (is_gimple_reg (t) || is_a<G::integer_cst> (t))
    return true;

  G::addr_expr addr = dyn_cast<G::addr_expr> (t);

  return (addr && (is_a<G::constant> (addr->expr ())
		   || decl_address_invariant_p (addr->expr ())));
}

/* Mark X addressable.  Unlike the langhook we expect X to be in gimple
   form and we don't do any syntax checking.  */

void
mark_addressable (G::value x)
{
  while (handled_component_p (x))
    x = x->op(0);

  G::mem_ref mem = dyn_cast<G::mem_ref> (x);
  if (mem && is_a<G::addr_expr> (mem->base ()))
    x = as_a<G::addr_expr> (mem->base ())-> expr ();
  if (!is_a<G::var_decl> (x) && !is_a<G::parm_decl> (x)
      && !is_a<G::result_decl> (x))
    return;
  x->set_addressable (true);

  /* Also mark the artificial SSA_NAME that points to the partition of X.  */
  G::var_decl var = dyn_cast<G::var_decl> (x);
  if (var && !var->external () && !var->static_p ()
      && cfun->gimple_df != NULL
      && cfun->gimple_df->decls_to_pointers != NULL)
    {
      G::value_ptr namep =
	    (tree *) (pointer_map_contains (cfun->gimple_df->decls_to_pointers,
					    var)); 
      if (namep)
        (*namep)->set_addressable (true);
    }
}

/* Returns true iff T is a valid RHS for an assignment to a renamed
   user -- or front-end generated artificial -- variable.  */

bool
is_gimple_reg_rhs (G::value t)
{
  return get_gimple_rhs_class (t) != GIMPLE_INVALID_RHS;
}

#include "gt-gimple-expr.h"
