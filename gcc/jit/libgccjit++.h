/* A C++ API for libgccjit, purely as inline wrapper functions.  */

#ifndef LIBGCCJIT_PLUS_PLUS_H
#define LIBGCCJIT_PLUS_PLUS_H

#include "libgccjit.h"

#include <ostream>
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

  class object
  {
  public:
    std::string get_debug_string () const;

  protected:
    object ();
    object (gcc_jit_object *obj);

    gcc_jit_object *get_inner_object ();

  private:
    gcc_jit_object *m_inner_obj;
  };

  inline std::ostream& operator << (std::ostream& stream, const object &obj);

  /* Some client code will want to supply source code locations, others
     won't.  To avoid doubling the number of entrypoints, everything
     accepting a location also has a default argument.  To do this, the
     other classes need to see that "location" has a default constructor,
     hence we need to declare it first.  */
  class location : public object
  {
  public:
    location ();
    location (gcc_jit_location *loc);

    gcc_jit_location *get_inner_location ();
  };

  class context
  {
  public:
    static context acquire ();
    context ();
    context (gcc_jit_context *ctxt);

    gccjit::context new_child_context ();

    void release ();

    gcc_jit_result *compile ();

    void set_int_option (enum gcc_jit_int_option opt,
			 int value);

    void set_bool_option (enum gcc_jit_bool_option opt,
			  int value);

    location
    new_location (const std::string &filename,
		  int line,
		  int column);

    type get_type (enum gcc_jit_types kind);

    type new_array_type (type element_type, int num_elements,
			 location loc = location ());

    field new_field (type type_, const std::string &name,
		     location loc = location ());

    type new_struct_type (const std::string &name,
			  std::vector<field> &fields,
			  location loc = location ());

    param new_param (type type_,
		     const std::string &name,
		     location loc = location ());

    function new_function (enum gcc_jit_function_kind kind,
			   type return_type,
			   const std::string &name,
			   std::vector<param> &params,
			   int is_variadic,
			   location loc = location ());

    rvalue new_rvalue (type numeric_type,
		       int value);
    rvalue zero (type numeric_type);
    rvalue one (type numeric_type);
    rvalue new_rvalue (type numeric_type,
		       double value);
    rvalue new_rvalue (type pointer_type,
		       void *value);
    rvalue new_rvalue (const std::string &value);

    rvalue new_unary_op (enum gcc_jit_unary_op op,
			 type result_type,
			 rvalue a,
			 location loc = location ());

    rvalue new_binary_op (enum gcc_jit_binary_op op,
			  type result_type,
			  rvalue a, rvalue b,
			  location loc = location ());

    rvalue new_comparison (enum gcc_jit_comparison op,
			   rvalue a, rvalue b,
			   location loc = location ());

    rvalue new_call (function func,
		     std::vector<rvalue> &args,
		     location loc = location ());

    lvalue new_array_access (rvalue ptr,
			     rvalue index,
			     location loc = location ());

  public:
    gcc_jit_context *m_inner_ctxt;
  };

  class field : public object
  {
  public:
    field ();
    field (gcc_jit_field *inner);

    gcc_jit_field *get_inner_field ();
  };

  class type : public object
  {
  public:
    type ();
    type (gcc_jit_type *inner);

    gcc_jit_type *get_inner_type ();

    type get_pointer ();

 };

  class function : public object
  {
  public:
    function ();
    function (gcc_jit_function *func);

    gcc_jit_function *get_inner_function ();

    label new_forward_label ();
    label new_forward_label (const std::string &name);

    lvalue new_local (type type_,
		      const std::string &name,
		      location loc = location ());

    void add_eval (rvalue rvalue,
		   location loc = location ());

    void add_assignment (lvalue lvalue,
			 rvalue rvalue,
			 location loc = location ());

    void add_assignment_op (lvalue lvalue,
			    enum gcc_jit_binary_op op,
			    rvalue rvalue,
			    location loc = location ());

    void add_comment (const std::string &text,
		      location loc = location ());

    void add_conditional (rvalue boolval,
			  label on_true,
			  label on_false,
			  location loc = location ());

    label add_label (const std::string &name,
		     location loc = location ());
    label add_label (location loc = location ());

    void place_forward_label (label lab,
			      location loc = location ());

    void add_jump (label target,
		   location loc = location ());

    void add_return (rvalue rvalue,
		     location loc = location ());
  };

  class label : public object
  {
  public:
    label ();
    label (gcc_jit_label *inner);

    gcc_jit_label *get_inner_label ();
  };

  class rvalue : public object
  {
  public:
    rvalue ();
    rvalue (gcc_jit_rvalue *inner);
    gcc_jit_rvalue *get_inner_rvalue ();

    rvalue access_field (field field,
			 location loc = location ());

    lvalue dereference_field (field field,
			      location loc = location ());

    lvalue dereference (location loc = location ());
 };

  class lvalue : public rvalue
  {
  public:
    lvalue ();
    lvalue (gcc_jit_lvalue *inner);

    gcc_jit_lvalue *get_inner_lvalue ();

    lvalue access_field (field field,
			 location loc = location ());

    rvalue get_address (location loc = location ());
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

inline gccjit::context
context::new_child_context ()
{
  return context (gcc_jit_context_new_child_context (m_inner_ctxt));
}

inline void
context::release ()
{
  gcc_jit_context_release (m_inner_ctxt);
  m_inner_ctxt = NULL;
}

inline gcc_jit_result *
context::compile ()
{
  return gcc_jit_context_compile (m_inner_ctxt);
}

inline void
context::set_int_option (enum gcc_jit_int_option opt,
			 int value)
{
  gcc_jit_context_set_int_option (m_inner_ctxt, opt, value);

}

inline void
context::set_bool_option (enum gcc_jit_bool_option opt,
			  int value)
{
  gcc_jit_context_set_bool_option (m_inner_ctxt, opt, value);

}

inline location
context::new_location (const std::string &filename,
		       int line,
		       int column)
{
  return location (gcc_jit_context_new_location (m_inner_ctxt,
						 filename.c_str (),
						 line,
						 column));
}

inline type
context::get_type (enum gcc_jit_types kind)
{
  return type (gcc_jit_context_get_type (m_inner_ctxt, kind));
}

inline type
context::new_array_type (type element_type, int num_elements, location loc)
{
  return type (gcc_jit_context_new_array_type (
		 m_inner_ctxt,
		 loc.get_inner_location (),
		 element_type.get_inner_type (),
		 num_elements));
}

inline field
context::new_field (type type_, const std::string &name, location loc)
{
  return field (gcc_jit_context_new_field (m_inner_ctxt,
					   loc.get_inner_location (),
					   type_.get_inner_type (),
					   name.c_str ()));
}

inline type
context::new_struct_type (const std::string &name,
			  std::vector<field> &fields,
			  location loc)
{
  /* Treat std::vector as an array, relying on it not being resized: */
  field *as_array_of_wrappers = &fields[0];

  /* Treat the array as being of the underlying pointers, relying on
     the wrapper type being such a pointer internally.	*/
  gcc_jit_field **as_array_of_ptrs =
    reinterpret_cast<gcc_jit_field **> (as_array_of_wrappers);

  return type (gcc_jit_context_new_struct_type (m_inner_ctxt,
						loc.get_inner_location (),
						name.c_str (),
						fields.size (),
						as_array_of_ptrs));
}

inline param
context::new_param (type type_,
		    const std::string &name,
		    location loc)
{
  return param (gcc_jit_context_new_param (m_inner_ctxt,
					   loc.get_inner_location (),
					   type_.get_inner_type (),
					   name.c_str ()));
}

inline function
context::new_function (enum gcc_jit_function_kind kind,
		       type return_type,
		       const std::string &name,
		       std::vector<param> &params,
		       int is_variadic,
		       location loc)
{
  /* Treat std::vector as an array, relying on it not being resized: */
  param *as_array_of_wrappers = &params[0];

  /* Treat the array as being of the underlying pointers, relying on
     the wrapper type being such a pointer internally.	*/
  gcc_jit_param **as_array_of_ptrs =
    reinterpret_cast<gcc_jit_param **> (as_array_of_wrappers);

  return function (gcc_jit_context_new_function (m_inner_ctxt,
						 loc.get_inner_location (),
						 kind,
						 return_type.get_inner_type (),
						 name.c_str (),
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
					 numeric_type.get_inner_type (),
					 value));
}

inline rvalue
context::zero (type numeric_type)
{
  return rvalue (gcc_jit_context_zero (m_inner_ctxt,
				       numeric_type.get_inner_type ()));
}

inline rvalue
context::one (type numeric_type)
{
  return rvalue (gcc_jit_context_one (m_inner_ctxt,
				       numeric_type.get_inner_type ()));
}

inline rvalue
context::new_rvalue (type numeric_type,
		     double value)
{
  return rvalue (
    gcc_jit_context_new_rvalue_from_double (m_inner_ctxt,
					    numeric_type.get_inner_type (),
					    value));
}

inline rvalue
context::new_rvalue (type pointer_type,
		     void *value)
{
  return rvalue (
    gcc_jit_context_new_rvalue_from_ptr (m_inner_ctxt,
					 pointer_type.get_inner_type (),
					 value));
}

inline rvalue
context::new_rvalue (const std::string &value)
{
  return rvalue (
    gcc_jit_context_new_string_literal (m_inner_ctxt, value.c_str ()));
}

inline rvalue
context::new_unary_op (enum gcc_jit_unary_op op,
		       type result_type,
		       rvalue a,
		       location loc)
{
  return rvalue (gcc_jit_context_new_unary_op (m_inner_ctxt,
					       loc.get_inner_location (),
					       op,
					       result_type.get_inner_type (),
					       a.get_inner_rvalue ()));
}

inline rvalue
context::new_binary_op (enum gcc_jit_binary_op op,
			type result_type,
			rvalue a, rvalue b,
			location loc)
{
  return rvalue (gcc_jit_context_new_binary_op (m_inner_ctxt,
						loc.get_inner_location (),
						op,
						result_type.get_inner_type (),
						a.get_inner_rvalue (),
						b.get_inner_rvalue ()));
}

inline rvalue
context::new_comparison (enum gcc_jit_comparison op,
			 rvalue a, rvalue b,
			 location loc)
{
  return rvalue (gcc_jit_context_new_comparison (m_inner_ctxt,
						 loc.get_inner_location (),
						 op,
						 a.get_inner_rvalue (),
						 b.get_inner_rvalue ()));
}

inline rvalue
context::new_call (function func,
		   std::vector<rvalue> &args,
		   location loc)
{
  /* Treat std::vector as an array, relying on it not being resized: */
  rvalue *as_array_of_wrappers = &args[0];

  /* Treat the array as being of the underlying pointers, relying on
     the wrapper type being such a pointer internally.	*/
  gcc_jit_rvalue **as_array_of_ptrs =
    reinterpret_cast<gcc_jit_rvalue **> (as_array_of_wrappers);
  return gcc_jit_context_new_call (m_inner_ctxt,
				   loc.get_inner_location (),
				   func.get_inner_function (),
				   args.size (),
				   as_array_of_ptrs);
}

inline lvalue
context::new_array_access (rvalue ptr,
			   rvalue index,
			   location loc)
{
  return lvalue (gcc_jit_context_new_array_access (m_inner_ctxt,
						   loc.get_inner_location (),
						   ptr.get_inner_rvalue (),
						   index.get_inner_rvalue ()));
}

// class object
inline std::string
object::get_debug_string () const
{
  return gcc_jit_object_get_debug_string (m_inner_obj);
}

inline object::object () : m_inner_obj (NULL) {}
inline object::object (gcc_jit_object *obj) : m_inner_obj (obj) {}

inline gcc_jit_object *
object::get_inner_object ()
{
  return m_inner_obj;
}

inline std::ostream&
operator << (std::ostream& stream, const object &obj)
{
  return stream << obj.get_debug_string ();
}

// class location
inline location::location () : object (NULL) {}
inline location::location (gcc_jit_location *loc)
  : object (gcc_jit_location_as_object (loc))
{}

inline gcc_jit_location *
location::get_inner_location ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_location *> (get_inner_object ());
}

// class field
inline field::field () : object (NULL) {}
inline field::field (gcc_jit_field *inner)
  : object (gcc_jit_field_as_object (inner))
{}

inline gcc_jit_field *
field::get_inner_field ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_field *> (get_inner_object ());
}

// class type
inline type::type () : object (NULL) {}
inline type::type (gcc_jit_type *inner)
  : object (gcc_jit_type_as_object (inner))
{}

inline gcc_jit_type *
type::get_inner_type ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_type *> (get_inner_object ());
}

inline type
type::get_pointer ()
{
  return type (gcc_jit_type_get_pointer (get_inner_type ()));
}

// class function
inline function::function () : object (NULL) {}
inline function::function (gcc_jit_function *inner)
  : object (gcc_jit_function_as_object (inner))
{}

inline gcc_jit_function *
function::get_inner_function ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_function *> (get_inner_object ());
}

inline label
function::new_forward_label ()
{
  return label (gcc_jit_function_new_forward_label (get_inner_function (),
						    NULL));
}

inline label
function::new_forward_label (const std::string &name)
{
  return label (gcc_jit_function_new_forward_label (get_inner_function (),
						    name.c_str ()));
}

inline lvalue
function::new_local (type type_,
		     const std::string &name,
		     location loc)
{
  return lvalue (gcc_jit_function_new_local (get_inner_function (),
					     loc.get_inner_location (),
					     type_.get_inner_type (),
					     name.c_str ()));
}

inline void
function::add_eval (rvalue rvalue,
		    location loc)
{
  gcc_jit_function_add_eval (get_inner_function (),
			     loc.get_inner_location (),
			     rvalue.get_inner_rvalue ());
}

inline void
function::add_assignment (lvalue lvalue,
			  rvalue rvalue,
			  location loc)
{
  gcc_jit_function_add_assignment (get_inner_function (),
				   loc.get_inner_location (),
				   lvalue.get_inner_lvalue (),
				   rvalue.get_inner_rvalue ());
}

inline void
function::add_assignment_op (lvalue lvalue,
			     enum gcc_jit_binary_op op,
			     rvalue rvalue,
			     location loc)
{
  gcc_jit_function_add_assignment_op (get_inner_function (),
				      loc.get_inner_location (),
				      lvalue.get_inner_lvalue (),
				      op,
				      rvalue.get_inner_rvalue ());
}

inline void
function::add_comment (const std::string &text,
		       location loc)
{
  gcc_jit_function_add_comment (get_inner_function (),
				loc.get_inner_location (),
				text.c_str ());
}

inline void
function::add_conditional (rvalue boolval,
			   label on_true,
			   label on_false,
			   location loc)
{
  gcc_jit_function_add_conditional (get_inner_function (),
				    loc.get_inner_location (),
				    boolval.get_inner_rvalue (),
				    on_true.get_inner_label (),
				    on_false.get_inner_label ());
}

inline label
function::add_label (const std::string &name,
		     location loc)
{
  return label (gcc_jit_function_add_label (get_inner_function (),
					    loc.get_inner_location (),
					    name.c_str ()));
}

inline label
function::add_label (location loc)
{
  return label (gcc_jit_function_add_label (get_inner_function (),
					    loc.get_inner_location (),
					    NULL));
}

inline void
function::place_forward_label (label lab,
			       location loc)
{
  gcc_jit_function_place_forward_label (get_inner_function (),
					loc.get_inner_location (),
					lab.get_inner_label ());
}

inline void
function::add_jump (label target,
		    location loc)
{
  gcc_jit_function_add_jump (get_inner_function (),
			     loc.get_inner_location (),
			     target.get_inner_label ());
}

inline void
function::add_return (rvalue rvalue,
		      location loc)
{
  gcc_jit_function_add_return (get_inner_function (),
			       loc.get_inner_location (),
			       rvalue.get_inner_rvalue ());
}

// class label
inline label::label () : object (NULL) {}
inline label::label (gcc_jit_label *inner)
  : object (gcc_jit_label_as_object (inner))
{}

inline gcc_jit_label *
label::get_inner_label ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_label *> (get_inner_object ());
}

//  class rvalue
inline rvalue::rvalue () : object (NULL) {}
inline rvalue::rvalue (gcc_jit_rvalue *inner)
  : object (gcc_jit_rvalue_as_object (inner))
{}

inline gcc_jit_rvalue *
rvalue::get_inner_rvalue ()
{
  /* Manual downcast: */
  return reinterpret_cast<gcc_jit_rvalue *> (get_inner_object ());
}

inline rvalue
rvalue::access_field (field field,
		      location loc)
{
  return rvalue (gcc_jit_rvalue_access_field (get_inner_rvalue (),
					      loc.get_inner_location (),
					      field.get_inner_field ()));
}

inline lvalue
rvalue::dereference_field (field field,
			   location loc)
{
  return lvalue (gcc_jit_rvalue_dereference_field (get_inner_rvalue (),
						   loc.get_inner_location (),
						   field.get_inner_field ()));
}

inline lvalue
rvalue::dereference (location loc)
{
  return lvalue (gcc_jit_rvalue_dereference (get_inner_rvalue (),
					     loc.get_inner_location ()));
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
  return reinterpret_cast<gcc_jit_lvalue *> (get_inner_object ());
}

inline lvalue
lvalue::access_field (field field, location loc)
{
  return lvalue (gcc_jit_lvalue_access_field (get_inner_lvalue (),
					      loc.get_inner_location (),
					      field.get_inner_field ()));
}

inline rvalue
lvalue::get_address (location loc)
{
  return rvalue (gcc_jit_lvalue_get_address (get_inner_lvalue (),
					     loc.get_inner_location ()));
}

// class param : public lvalue
inline param::param () : lvalue () {}
inline param::param (gcc_jit_param *inner)
  : lvalue (gcc_jit_param_as_lvalue (inner))
{}

} // namespace gccjit

#endif /* #ifndef LIBGCCJIT_PLUS_PLUS_H */
