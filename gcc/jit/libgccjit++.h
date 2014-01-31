/* A C++ API for libgccjit, purely as inline wrapper functions.  */

#ifndef LIBGCCJIT_PLUS_PLUS_H
#define LIBGCCJIT_PLUS_PLUS_H

#include "libgccjit.h"

#include <vector>

/****************************************************************************
 C++ API
 ****************************************************************************/

namespace gccjit
{
  class context;
  class location;
  class field;
  class type;
  class param;
  class function;
  class label;
  class rvalue;
  class lvalue;

  class context
  {
  public:
    static context acquire ();
    context ();
    context (gcc_jit_context *ctxt);

    location
    new_location (const char *filename,
		  int line,
		  int column);

    type get_type (enum gcc_jit_types kind);

    field new_field (type type_, const char *name);
    field new_field (location loc, type type_, const char *name);

    type new_struct_type (const char *name,
			  std::vector<field> fields);
    type new_struct_type (location loc,
			  const char *name,
			  std::vector<field> fields);

    param new_param (type type_,
		     const char *name);
    param new_param (location loc,
		     type type_,
		     const char *name);

    function new_function (enum gcc_jit_function_kind kind,
			   type return_type,
			   const char *name,
			   std::vector<param> params,
			   int is_variadic);
    function new_function (location loc,
			   enum gcc_jit_function_kind kind,
			   type return_type,
			   const char *name,
			   std::vector<param> params,
			   int is_variadic);

    rvalue new_rvalue (type numeric_type,
		       int value);
    rvalue zero (type numeric_type);
    rvalue one (type numeric_type);
    rvalue new_rvalue (type numeric_type,
		       double value);
    rvalue new_rvalue (type pointer_type,
		       void *value);
    rvalue new_rvalue (const char *value);

    rvalue new_unary_op (enum gcc_jit_unary_op op,
			 type result_type,
			 rvalue a);
    rvalue new_unary_op (location loc,
			 enum gcc_jit_unary_op op,
			 type result_type,
			 rvalue a);

    rvalue new_binary_op (enum gcc_jit_binary_op op,
			  type result_type,
			  rvalue a, rvalue b);
    rvalue new_binary_op (location loc,
			  enum gcc_jit_binary_op op,
			  type result_type,
			  rvalue a, rvalue b);

    rvalue new_comparison (enum gcc_jit_comparison op,
			   rvalue a, rvalue b);
    rvalue new_comparison (location loc,
			   enum gcc_jit_comparison op,
			   rvalue a, rvalue b);

    rvalue new_call (function func,
		     std::vector<rvalue> args);
    rvalue new_call (location loc,
		     function func,
		     std::vector<rvalue> args);

  public:
    gcc_jit_context *m_inner_ctxt;
  };

  class location
  {
  public:
    location ();
    location (gcc_jit_location *loc);

  public:
    gcc_jit_location *m_inner_loc;
   };

  class field
  {
  public:
    field ();
    field (gcc_jit_field *inner);

 public:
    gcc_jit_field *m_inner_field;
  };

  class type
  {
  public:
    type ();
    type (gcc_jit_type *inner);

    type get_pointer ();

  public:
    gcc_jit_type *m_inner_type;
  };

  class function
  {
  public:
    function ();
    function (gcc_jit_function *func);

    label new_forward_label (const char *name);

    lvalue new_local (type type_,
		      const char *name);
    lvalue new_local (location loc,
		      type type_,
		      const char *name);

    void add_eval (rvalue rvalue);
    void add_eval (location loc,
		   rvalue rvalue);

    void add_assignment (lvalue lvalue,
			 rvalue rvalue);
    void add_assignment (location loc,
			 lvalue lvalue,
			 rvalue rvalue);

    void add_assignment_op (lvalue lvalue,
			    enum gcc_jit_binary_op op,
			    rvalue rvalue);
    void add_assignment_op (location loc,
			    lvalue lvalue,
			    enum gcc_jit_binary_op op,
			    rvalue rvalue);

    void add_comment (location loc,
		      const char *text);
    void add_comment (const char *text);

    void add_conditional (location loc,
			  rvalue boolval,
			  label on_true,
			  label on_false);
    void add_conditional (rvalue boolval,
			  label on_true,
			  label on_false);

    label add_label (location loc,
		     const char *name);
    label add_label (const char *name);

    void place_forward_label (location loc,
			      label lab);
    void place_forward_label (label lab);

    void add_jump (location loc,
		   label target);
    void add_jump (label target);

    void add_return (location loc,
		     rvalue rvalue);
    void add_return (rvalue rvalue);

  public:
    gcc_jit_function *m_inner_func;
   };

  class label
  {
  public:
    label ();
    label (gcc_jit_label *inner);

  public:
    gcc_jit_label *m_inner_label;
  };

  class rvalue
  {
  public:
    rvalue ();
    rvalue (gcc_jit_rvalue *inner);

    rvalue access_field (field field);
    rvalue access_field (location loc,
			 field field);

    lvalue dereference_field (field field);
    lvalue dereference_field (location loc,
			      field field);

    lvalue dereference ();
    lvalue dereference (location loc);

  public:
    gcc_jit_rvalue *m_inner_rvalue;
  };

  class lvalue : public rvalue
  {
  public:
    lvalue ();
    lvalue (gcc_jit_lvalue *inner);

    gcc_jit_lvalue *get_inner_lvalue ();

    lvalue access_field (field field);
    lvalue access_field (location loc,
			 field field);

    rvalue get_address ();
    rvalue get_address (location loc);
  };

  class param : public lvalue
  {
  public:
    param ();
    param (gcc_jit_param *inner);

    gcc_jit_param *get_inner_param ();
  };
}

/****************************************************************************
 Implementation of the API
 ****************************************************************************/
namespace gccjit {

// class context
inline context context::acquire ()
{
  return context (gcc_jit_context_acquire ());
}
inline context::context () : m_inner_ctxt (NULL) {}
inline context::context (gcc_jit_context *inner) : m_inner_ctxt (inner) {}

inline location
context::new_location (const char *filename,
		       int line,
		       int column)
{
  return location (gcc_jit_context_new_location (m_inner_ctxt,
						 filename,
						 line,
						 column));
}

inline type
context::get_type (enum gcc_jit_types kind)
{
  return type (gcc_jit_context_get_type (m_inner_ctxt, kind));
}

inline field
context::new_field (type type_, const char *name)
{
  return new_field (location (), type_, name);
}
inline field
context::new_field (location loc, type type_, const char *name)
{
  return field (gcc_jit_context_new_field (m_inner_ctxt,
					   loc.m_inner_loc,
					   type_.m_inner_type,
					   name));
}

inline type
context::new_struct_type (const char *name,
			  std::vector<field> fields)
{
  return new_struct_type (location (),
			  name,
			  fields);
}
inline type
context::new_struct_type (location loc,
			  const char *name,
			  std::vector<field> fields)
{
  /* Treat std::vector as an array, relying on it not being resized: */
  field *as_array_of_wrappers = &fields[0];

  /* Treat the array as being of the underlying pointers, relying on
     the wrapper type being such a pointer internally.	*/
  gcc_jit_field **as_array_of_ptrs =
    reinterpret_cast<gcc_jit_field **> (as_array_of_wrappers);

  return type (gcc_jit_context_new_struct_type (m_inner_ctxt,
						loc.m_inner_loc,
						name,
						fields.size (),
						as_array_of_ptrs));
}

inline param
context::new_param (type type_,
		    const char *name)
{
  return new_param (location (),
		    type_,
		    name);
}
inline param
context::new_param (location loc,
		    type type_,
		    const char *name)
{
  return param (gcc_jit_context_new_param (m_inner_ctxt,
					   loc.m_inner_loc,
					   type_.m_inner_type,
					   name));
}

inline function
context::new_function (enum gcc_jit_function_kind kind,
		       type return_type,
		       const char *name,
		       std::vector<param> params,
		       int is_variadic)
{
  return new_function (location (),
		       kind,
		       return_type,
		       name,
		       params,
		       is_variadic);
}
inline function
context::new_function (location loc,
		       enum gcc_jit_function_kind kind,
		       type return_type,
		       const char *name,
		       std::vector<param> params,
		       int is_variadic)
{
  /* Treat std::vector as an array, relying on it not being resized: */
  param *as_array_of_wrappers = &params[0];

  /* Treat the array as being of the underlying pointers, relying on
     the wrapper type being such a pointer internally.	*/
  gcc_jit_param **as_array_of_ptrs =
    reinterpret_cast<gcc_jit_param **> (as_array_of_wrappers);

  return function (gcc_jit_context_new_function (m_inner_ctxt,
						 loc.m_inner_loc,
						 kind,
						 return_type.m_inner_type,
						 name,
						 params.size (),
						 as_array_of_ptrs,
						 is_variadic));
}

inline rvalue
context::new_rvalue (type numeric_type,
		     int value)
{
  return rvalue (
    gcc_jit_context_new_rvalue_from_int (m_inner_ctxt,
					 numeric_type.m_inner_type,
					 value));
}

inline rvalue
context::zero (type numeric_type)
{
  return rvalue (gcc_jit_context_zero (m_inner_ctxt,
				       numeric_type.m_inner_type));
}

inline rvalue
context::one (type numeric_type)
{
  return rvalue (gcc_jit_context_one (m_inner_ctxt,
				       numeric_type.m_inner_type));
}

inline rvalue
context::new_rvalue (type numeric_type,
		     double value)
{
  return rvalue (
    gcc_jit_context_new_rvalue_from_double (m_inner_ctxt,
					    numeric_type.m_inner_type,
					    value));
}

inline rvalue
context::new_rvalue (type pointer_type,
		     void *value)
{
  return rvalue (
    gcc_jit_context_new_rvalue_from_ptr (m_inner_ctxt,
					 pointer_type.m_inner_type,
					 value));
}

inline rvalue
context::new_rvalue (const char *value)
{
  return rvalue (
    gcc_jit_context_new_string_literal (m_inner_ctxt, value));
}

inline rvalue
context::new_unary_op (enum gcc_jit_unary_op op,
		       type result_type,
		       rvalue a)
{
  return new_unary_op (location (),
		       op,
		       result_type,
		       a);
}
inline rvalue
context::new_unary_op (location loc,
		       enum gcc_jit_unary_op op,
		       type result_type,
		       rvalue a)
{
  return rvalue (gcc_jit_context_new_unary_op (m_inner_ctxt,
					       loc.m_inner_loc,
					       op,
					       result_type.m_inner_type,
					       a.m_inner_rvalue));
}

inline rvalue
context::new_binary_op (enum gcc_jit_binary_op op,
			type result_type,
			rvalue a, rvalue b)
{
  return new_binary_op (location (),
			op,
			result_type,
			a, b);
}
inline rvalue
context::new_binary_op (location loc,
			enum gcc_jit_binary_op op,
			type result_type,
			rvalue a, rvalue b)
{
  return rvalue (gcc_jit_context_new_binary_op (m_inner_ctxt,
						loc.m_inner_loc,
						op,
						result_type.m_inner_type,
						a.m_inner_rvalue,
						b.m_inner_rvalue));
}

inline rvalue
context::new_comparison (enum gcc_jit_comparison op,
			 rvalue a, rvalue b)
{
  return new_comparison (location (),
			 op,
			 a, b);
}
inline rvalue
context::new_comparison (location loc,
			 enum gcc_jit_comparison op,
			 rvalue a, rvalue b)
{
  return rvalue (gcc_jit_context_new_comparison (m_inner_ctxt,
						 loc.m_inner_loc,
						 op,
						 a.m_inner_rvalue,
						 b.m_inner_rvalue));
}

inline rvalue
context::new_call (function func,
		   std::vector<rvalue> args)
{
  return new_call (location (),
		   func,
		   args);
}

inline rvalue
context::new_call (location loc,
		   function func,
		   std::vector<rvalue> args)
{
  /* Treat std::vector as an array, relying on it not being resized: */
  rvalue *as_array_of_wrappers = &args[0];

  /* Treat the array as being of the underlying pointers, relying on
     the wrapper type being such a pointer internally.	*/
  gcc_jit_rvalue **as_array_of_ptrs =
    reinterpret_cast<gcc_jit_rvalue **> (as_array_of_wrappers);
  return gcc_jit_context_new_call (m_inner_ctxt,
				   loc.m_inner_loc,
				   func.m_inner_func,
				   args.size (),
				   as_array_of_ptrs);
}

// class location
inline location::location () : m_inner_loc (NULL) {}
inline location::location (gcc_jit_location *loc) : m_inner_loc (loc) {}

// class field
inline field::field () : m_inner_field (NULL) {}
inline field::field (gcc_jit_field *inner) : m_inner_field (inner) {}

// class type
inline type::type () : m_inner_type (NULL) {}
inline type::type (gcc_jit_type *inner) : m_inner_type (inner) {}

inline type
type::get_pointer ()
{
  return type (gcc_jit_type_get_pointer (m_inner_type));
}

// class function
inline function::function () : m_inner_func (NULL) {}
inline function::function (gcc_jit_function *inner) : m_inner_func (inner) {}

inline label
function::new_forward_label (const char *name)
{
  return label (gcc_jit_function_new_forward_label (m_inner_func, name));
}

inline lvalue
function::new_local (type type_,
		     const char *name)
{
  return new_local (location (), type_, name);
}

inline lvalue
function::new_local (location loc,
		     type type_,
		     const char *name)
{
  return lvalue (gcc_jit_function_new_local (m_inner_func,
					     loc.m_inner_loc,
					     type_.m_inner_type,
					     name));
}

inline void
function::add_eval (rvalue rvalue)
{
  add_eval (location (),
	    rvalue);
}
inline void
function::add_eval (location loc,
		    rvalue rvalue)
{
  gcc_jit_function_add_eval (m_inner_func,
			     loc.m_inner_loc,
			     rvalue.m_inner_rvalue);
}

inline void
function::add_assignment (lvalue lvalue,
			  rvalue rvalue)
{
  add_assignment (location (),
		  lvalue,
		  rvalue);
}
inline void
function::add_assignment (location loc,
			  lvalue lvalue,
			  rvalue rvalue)
{
  gcc_jit_function_add_assignment (m_inner_func,
				   loc.m_inner_loc,
				   lvalue.get_inner_lvalue (),
				   rvalue.m_inner_rvalue);
}

inline void
function::add_assignment_op (lvalue lvalue,
			     enum gcc_jit_binary_op op,
			     rvalue rvalue)
{
  add_assignment_op (location (),
		     lvalue,
		     op,
		     rvalue);
}
inline void
function::add_assignment_op (location loc,
			     lvalue lvalue,
			     enum gcc_jit_binary_op op,
			     rvalue rvalue)
{
  gcc_jit_function_add_assignment_op (m_inner_func,
				      loc.m_inner_loc,
				      lvalue.get_inner_lvalue (),
				      op,
				      rvalue.m_inner_rvalue);
}

inline void
function::add_comment (const char *text)
{
  add_comment (location (), text);
}
inline void
function::add_comment (location loc,
		       const char *text)
{
  gcc_jit_function_add_comment (m_inner_func,
				loc.m_inner_loc,
				text);
}

inline void
function::add_conditional (rvalue boolval,
			   label on_true,
			   label on_false)
{
  return add_conditional (location (), boolval, on_true, on_false);
}
inline void
function::add_conditional (location loc,
			   rvalue boolval,
			   label on_true,
			   label on_false)
{
  gcc_jit_function_add_conditional (m_inner_func,
				    loc.m_inner_loc,
				    boolval.m_inner_rvalue,
				    on_true.m_inner_label,
				    on_false.m_inner_label);
}

inline label
function::add_label (const char *name)
{
  return add_label (location (), name);
}
inline label
function::add_label (location loc,
		     const char *name)
{
  return label (gcc_jit_function_add_label (m_inner_func,
					    loc.m_inner_loc,
					    name));
}

inline void
function::place_forward_label (label lab)
{
  return place_forward_label (location (), lab);
}
inline void
function::place_forward_label (location loc,
			       label lab)
{
  gcc_jit_function_place_forward_label (m_inner_func,
					loc.m_inner_loc,
					lab.m_inner_label);
}

inline void
function::add_jump (label target)
{
  add_jump (location (), target);
}
inline void
function::add_jump (location loc,
		    label target)
{
  gcc_jit_function_add_jump (m_inner_func,
			     loc.m_inner_loc,
			     target.m_inner_label);
}

inline void
function::add_return (rvalue rvalue)
{
  add_return (location (), rvalue);
}
inline void
function::add_return (location loc,
		      rvalue rvalue)
{
  gcc_jit_function_add_return (m_inner_func,
			       loc.m_inner_loc,
			       rvalue.m_inner_rvalue);
}

// class label
inline label::label () : m_inner_label (NULL) {}
inline label::label (gcc_jit_label *inner) : m_inner_label (inner) {}

//  class rvalue
inline rvalue::rvalue () : m_inner_rvalue (NULL) {}
inline rvalue::rvalue (gcc_jit_rvalue *inner) : m_inner_rvalue (inner) {}

inline rvalue
rvalue::access_field (field field)
{
  return access_field (location (), field);
}
inline rvalue
rvalue::access_field (location loc,
		      field field)
{
  return rvalue (gcc_jit_rvalue_access_field (m_inner_rvalue,
					      loc.m_inner_loc,
					      field.m_inner_field));
}

inline lvalue
rvalue::dereference_field (field field)
{
  return dereference_field (location (),
			    field);
}

inline lvalue
rvalue::dereference_field (location loc,
			   field field)
{
  return lvalue (gcc_jit_rvalue_dereference_field (m_inner_rvalue,
						   loc.m_inner_loc,
						   field.m_inner_field));
}

inline lvalue
rvalue::dereference ()
{
  return dereference (location ());
}
inline lvalue
rvalue::dereference (location loc)
{
  return lvalue (gcc_jit_rvalue_dereference (m_inner_rvalue,
					     loc.m_inner_loc));
}

// class lvalue : public rvalue
inline lvalue::lvalue () : rvalue () {}
inline lvalue::lvalue (gcc_jit_lvalue *inner)
  : rvalue (gcc_jit_lvalue_as_rvalue (inner))
{}

inline gcc_jit_lvalue *
lvalue::get_inner_lvalue ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_lvalue *> (m_inner_rvalue);
}

inline lvalue
lvalue::access_field (field field)
{
  return access_field (location (), field);
}
inline lvalue
lvalue::access_field (location loc,
		      field field)
{
  return lvalue (gcc_jit_lvalue_access_field (get_inner_lvalue (),
					      loc.m_inner_loc,
					      field.m_inner_field));
}

inline rvalue
lvalue::get_address ()
{
  return get_address (location ());
}
inline rvalue
lvalue::get_address (location loc)
{
  return rvalue (gcc_jit_lvalue_get_address (get_inner_lvalue (),
					     loc.m_inner_loc));
}

// class param : public lvalue
inline param::param () : lvalue () {}
inline param::param (gcc_jit_param *inner)
  : lvalue (gcc_jit_param_as_lvalue (inner))
{}

} // namespace gccjit

#endif /* #ifndef LIBGCCJIT_PLUS_PLUS_H */
