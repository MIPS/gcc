#ifndef INTERNAL_API_H
#define INTERNAL_API_H

#include "libgccjit.h"

#include "tree.h"
#include "tree-iterator.h"

#include <utility> // for std::pair

#ifdef GCC_VERSION
#if GCC_VERSION >= 4001
#define GNU_PRINTF(M, N) __attribute__ ((format (gnu_printf, (M), (N))))
#else
#define GNU_PRINTF(M, N)
#endif
#endif

const int NUM_GCC_JIT_TYPES = GCC_JIT_TYPE_FILE_PTR + 1;

/* In order to allow jit objects to be usable outside of a compile
   whilst working with the existing structure of GCC's code the
   C API is implemented in terms of a gcc::jit::recording::context,
   which records the calls made to it.

   When a gcc_jit_context is compiled, the recording context creates a
   playback context.  The playback context invokes the bulk of the GCC
   code, and within the "frontend" parsing hook, plays back the recorded
   API calls, creating GCC tree objects.

   So there are two parallel families of classes: those relating to
   recording, and those relating to playback:

   * Visibility: recording objects are exposed back to client code,
     whereas playback objects are internal to the library.

   * Lifetime: recording objects have a lifetime equal to that of the
     recording context that created them, whereas playback objects only
     exist within the frontend hook.

   * Memory allocation: recording objects are allocated by the recording
     context, and automatically freed by it when the context is released,
     whereas playback objects are allocated within the GC heap, and
     garbage-collected; they can own GC-references.

   * Integration with rest of GCC: recording objects are unrelated to the
     rest of GCC, whereas playback objects are wrappers around "tree"
     instances.  Hence you can't ask a recording rvalue or lvalue what its
     type is, whereas you can for a playback rvalue of lvalue (since it
     can work with the underlying GCC tree nodes).

   * Instancing: There can be multiple recording contexts "alive" at once
     (albeit it only one compiling at once), whereas there can only be one
     playback context alive at one time (since it interacts with the GC).

   Ultimately if GCC could support multiple GC heaps and contexts, and
   finer-grained initialization, then this recording vs playback
   distinction could be eliminated.

   During a playback, we associate objects from the recording with
   their counterparts during this playback.  For simplicity, we store this
   within the recording objects, as "void *m_playback_obj", casting it to
   the appropriate playback object subclass.  For these casts to make
   sense, the two class hierarchies need to have the same structure.

   Note that the playback objects that "m_playback_obj" points to are
   GC-allocated, but the recording objects don't own references:
   these associations only exist within a part of the code where
   the GC doesn't collect, and are set back to NULL before the GC can
   run.  */

namespace gcc {

namespace jit {

class result;
class dump;

namespace recording {
  class context;
  class builtins_manager; // declared within jit-builtins.h
  class memento;
  class string;
  class location;
  class type;
  class function_type;
  class field;
  class struct_;
  class fields;
  class function;
  class block;
  class rvalue;
  class lvalue;
  class local;
  class param;
  class statement;
}

namespace playback {
  class context;
  class location;
  class type;
  class field;
  class struct_;
  class function;
  class block;
  class rvalue;
  class lvalue;
  class param;
  class source_file;
  class source_line;
}

typedef playback::context replayer;

class dump
{
public:
  dump (recording::context &ctxt,
	const char *filename,
	bool update_locations);
  ~dump ();

  void write (const char *fmt, ...)
    GNU_PRINTF(2, 3);

  bool update_locations () const { return m_update_locations; }

  recording::location *
  make_location () const;

private:
  recording::context &m_ctxt;
  const char *m_filename;
  bool m_update_locations;
  int m_line;
  int m_column;
  FILE *m_file;
};

/**********************************************************************
 Recording.
 **********************************************************************/

namespace recording {

playback::location *
playback_location (replayer *r, location *loc);

const char *
playback_string (string *str);

playback::block *
playback_block (block *b);

/* A JIT-compilation context.  */
class context
{
public:
  context (context *parent_ctxt);
  ~context ();

  void record (memento *m);
  void replay_into (replayer *r);
  void disassociate_from_playback ();

  string *
  new_string (const char *text);

  location *
  new_location (const char *filename,
		int line,
		int column);

  type *
  get_type (enum gcc_jit_types type);

  type *
  get_int_type (int num_bytes, int is_signed);

  type *
  new_array_type (location *loc,
		  type *element_type,
		  int num_elements);

  field *
  new_field (location *loc,
	     type *type,
	     const char *name);

  struct_ *
  new_struct_type (location *loc,
		   const char *name);

  param *
  new_param (location *loc,
	     type *type,
	     const char *name);

  function *
  new_function (location *loc,
		enum gcc_jit_function_kind kind,
		type *return_type,
		const char *name,
		int num_params,
		param **params,
		int is_variadic,
		enum built_in_function builtin_id);

  function *
  get_builtin_function (const char *name);

  lvalue *
  new_global (location *loc,
	      type *type,
	      const char *name);

  rvalue *
  new_rvalue_from_int (type *numeric_type,
		       int value);

  rvalue *
  new_rvalue_from_double (type *numeric_type,
			  double value);

  rvalue *
  new_rvalue_from_ptr (type *pointer_type,
		       void *value);

  rvalue *
  new_string_literal (const char *value);

  rvalue *
  new_unary_op (location *loc,
		enum gcc_jit_unary_op op,
		type *result_type,
		rvalue *a);

  rvalue *
  new_binary_op (location *loc,
		 enum gcc_jit_binary_op op,
		 type *result_type,
		 rvalue *a, rvalue *b);

  rvalue *
  new_comparison (location *loc,
		  enum gcc_jit_comparison op,
		  rvalue *a, rvalue *b);

  rvalue *
  new_call (location *loc,
	    function *func,
	    int numargs, rvalue **args);

  rvalue *
  new_cast (location *loc,
	    rvalue *expr,
	    type *type_);

  lvalue *
  new_array_access (location *loc,
		    rvalue *ptr,
		    rvalue *index);

  void
  set_str_option (enum gcc_jit_str_option opt,
		  const char *value);

  void
  set_int_option (enum gcc_jit_int_option opt,
		  int value);

  void
  set_bool_option (enum gcc_jit_bool_option opt,
		   int value);

  const char *
  get_str_option (enum gcc_jit_str_option opt) const
  {
    return m_str_options[opt];
  }

  int
  get_int_option (enum gcc_jit_int_option opt) const
  {
    return m_int_options[opt];
  }

  int
  get_bool_option (enum gcc_jit_bool_option opt) const
  {
    return m_bool_options[opt];
  }

  result *
  compile ();

  void
  add_error (location *loc, const char *fmt, ...)
      GNU_PRINTF(3, 4);

  void
  add_error_va (location *loc, const char *fmt, va_list ap)
      GNU_PRINTF(3, 0);

  const char *
  get_first_error () const;

  bool errors_occurred () const
  {
    if (m_parent_ctxt)
      if (m_parent_ctxt->errors_occurred ())
	return true;
    return m_error_count;
  }

  type *get_opaque_FILE_type ();

  void dump_to_file (const char *path, bool update_locations);

private:
  void validate ();

private:
  context *m_parent_ctxt;

  int m_error_count;
  char m_first_error_str[1024];

  const char *m_str_options[GCC_JIT_NUM_STR_OPTIONS];
  int m_int_options[GCC_JIT_NUM_INT_OPTIONS];
  bool m_bool_options[GCC_JIT_NUM_BOOL_OPTIONS];

  /* Recorded API usage.  */
  vec<memento *> m_mementos;

  /* Specific recordings, for use by dump_to_file.  */
  vec<struct_ *> m_structs;
  vec<function *> m_functions;

  type *m_basic_types[NUM_GCC_JIT_TYPES];
  type *m_FILE_type;

  builtins_manager *m_builtins_manager; // lazily created
};


/* An object with lifetime managed by the context i.e.
   it lives until the context is released, at which
   point it itself is cleaned up.  */

class memento
{
public:
  virtual ~memento () {}

  /* Hook for replaying this.  */
  virtual void replay_into (replayer *r) = 0;

  void set_playback_obj (void *obj) { m_playback_obj = obj; }

  context *get_context () { return m_ctxt; }

  memento *
  as_object () { return this; }

  /* Debugging hook, for use in generating error messages etc.  */
  const char *
  get_debug_string ();

  virtual void write_to_dump (dump &d);

protected:
  memento (context *ctxt)
  : m_ctxt (ctxt),
    m_playback_obj (NULL),
    m_debug_string (NULL)
  {
    gcc_assert (ctxt);
  }

  string *new_string (const char *text) { return m_ctxt->new_string (text); }

private:
  virtual string * make_debug_string () = 0;

public:
  context *m_ctxt;

protected:
  void *m_playback_obj;

private:
  string *m_debug_string;
};

/* or just use std::string? */
class string : public memento
{
public:
  string (context *ctxt, const char *text);
  ~string ();

  const char *c_str () { return m_buffer; }

  static string * from_printf (context *ctxt, const char *fmt, ...)
    GNU_PRINTF(2, 3);

  void replay_into (replayer *) {}

private:
  string * make_debug_string ();

private:
  size_t m_len;
  char *m_buffer;
};

class location : public memento
{
public:
  location (context *ctxt, string *filename, int line, int column)
  : memento (ctxt),
    m_filename (filename),
    m_line (line),
    m_column (column)
 {}

  void replay_into (replayer *r);

  playback::location *
  playback_location (replayer *r)
  {
    /* Normally during playback, we can walk forwards through the list of
       recording objects, playing them back.  The ordering of recording
       ensures that everything that a recording object refers to has
       already been played back, so we can simply look up the relevant
       m_playback_obj.

       Locations are an exception, due to the "write_to_dump" method of
       recording::statement.  This method can set a new location on a
       statement after the statement is created, and thus the location
       appears in the context's memento list *after* the statement that
       refers to it.

       In such circumstances, the statement is replayed *before* the location,
       when the latter doesn't yet have a playback object.

       Hence we need to ensure that locations have playback objects.  */
    if (!m_playback_obj)
      {
	replay_into (r);
      }
    gcc_assert (m_playback_obj);
    return static_cast <playback::location *> (m_playback_obj);
  }

private:
  string * make_debug_string ();

private:
  string *m_filename;
  int m_line;
  int m_column;
};

class type : public memento
{
public:
  type *get_pointer ();
  type *get_const ();
  type *get_volatile ();

  /* Get the type obtained when dereferencing this type.

     This will return NULL if it's not valid to dereference this type.
     The caller is responsible for setting an error.  */
  virtual type *dereference () = 0;

  /* Dynamic casts.  */
  virtual function_type *as_a_function_type() { gcc_unreachable (); return NULL; }
  virtual struct_ *dyn_cast_struct () { return NULL; }

  /* Is it typesafe to copy to this type from rtype?  */
  virtual bool accepts_writes_from (type *rtype)
  {
    gcc_assert (rtype);
    return this == rtype->unqualified ();
  }

  /* Strip off "const" etc */
  virtual type *unqualified ()
  {
    return this;
  }

  virtual bool is_int () const = 0;
  virtual bool is_float () const = 0;
  virtual bool is_bool () const = 0;
  virtual type *is_pointer () = 0;
  virtual type *is_array () = 0;

  bool is_numeric () const
  {
    return is_int () || is_float () || is_bool ();
  }

  playback::type *
  playback_type ()
  {
    return static_cast <playback::type *> (m_playback_obj);
  }

protected:
  type (context *ctxt)
    : memento (ctxt),
    m_pointer_to_this_type (NULL)
  {}

private:
  type *m_pointer_to_this_type;
};

/* Result of "gcc_jit_type_get_type".  */
class memento_of_get_type : public type
{
public:
  memento_of_get_type (context *ctxt,
		       enum gcc_jit_types kind)
  : type (ctxt),
    m_kind (kind) {}

  type *dereference ();

  bool accepts_writes_from (type *rtype)
  {
    if (m_kind == GCC_JIT_TYPE_VOID_PTR)
      if (rtype->is_pointer ())
	{
	  /* LHS (this) is type (void *), and the RHS is a pointer:
	     accept it:  */
	  return true;
	}

    return type::accepts_writes_from (rtype);
  }

  bool is_int () const;
  bool is_float () const;
  bool is_bool () const;
  type *is_pointer () { return dereference (); }
  type *is_array () { return NULL; }

public:
  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  enum gcc_jit_types m_kind;
};

/* Result of "gcc_jit_type_get_pointer".  */
class memento_of_get_pointer : public type
{
public:
  memento_of_get_pointer (type *other_type)
  : type (other_type->m_ctxt),
    m_other_type (other_type) {}

  type *dereference () { return m_other_type; }

  bool accepts_writes_from (type *rtype);

  void replay_into (replayer *r);

  bool is_int () const { return false; }
  bool is_float () const { return false; }
  bool is_bool () const { return false; }
  type *is_pointer () { return m_other_type; }
  type *is_array () { return NULL; }

private:
  string * make_debug_string ();

private:
  type *m_other_type;
};

/* Result of "gcc_jit_type_get_const".  */
class memento_of_get_const : public type
{
public:
  memento_of_get_const (type *other_type)
  : type (other_type->m_ctxt),
    m_other_type (other_type) {}

  type *dereference () { return m_other_type->dereference (); }

  bool accepts_writes_from (type */*rtype*/)
  {
    /* Can't write to a "const".  */
    return false;
  }

  /* Strip off the "const", giving the underlying type.  */
  type *unqualified () { return m_other_type; }

  bool is_int () const { return m_other_type->is_int (); }
  bool is_float () const { return m_other_type->is_float (); }
  bool is_bool () const { return m_other_type->is_bool (); }
  type *is_pointer () { return m_other_type->is_pointer (); }
  type *is_array () { return m_other_type->is_array (); }

  void replay_into (replayer *);

private:
  string * make_debug_string ();

private:
  type *m_other_type;
};

/* Result of "gcc_jit_type_get_volatile".  */
class memento_of_get_volatile : public type
{
public:
  memento_of_get_volatile (type *other_type)
  : type (other_type->m_ctxt),
    m_other_type (other_type) {}

  type *dereference () { return m_other_type->dereference (); }

  /* Strip off the "volatile", giving the underlying type.  */
  type *unqualified () { return m_other_type; }

  bool is_int () const { return m_other_type->is_int (); }
  bool is_float () const { return m_other_type->is_float (); }
  bool is_bool () const { return m_other_type->is_bool (); }
  type *is_pointer () { return m_other_type->is_pointer (); }
  type *is_array () { return m_other_type->is_array (); }

  void replay_into (replayer *);

private:
  string * make_debug_string ();

private:
  type *m_other_type;
};

class array_type : public type
{
 public:
  array_type (context *ctxt,
	      location *loc,
	      type *element_type,
	      int num_elements)
  : type (ctxt),
    m_loc (loc),
    m_element_type (element_type),
    m_num_elements (num_elements)
  {}

  type *dereference ();

  bool is_int () const { return false; }
  bool is_float () const { return false; }
  bool is_bool () const { return false; }
  type *is_pointer () { return NULL; }
  type *is_array () { return m_element_type; }

  void replay_into (replayer *);

 private:
  string * make_debug_string ();

 private:
  location *m_loc;
  type *m_element_type;
  int m_num_elements;
};

class function_type : public type
{
public:
  function_type (context *ctxt,
		 type *return_type,
		 int num_params,
		 type **param_types,
		 int is_variadic);

  type *dereference ();
  function_type *as_a_function_type () { return this; }

  bool is_int () const { return false; }
  bool is_float () const { return false; }
  bool is_bool () const { return false; }
  type *is_pointer () { return NULL; }
  type *is_array () { return NULL; }

  void replay_into (replayer *);

  type * get_return_type () const { return m_return_type; }
  vec<type *> get_param_types () const { return m_param_types; }
  int is_variadic () const { return m_is_variadic; }

 private:
  string * make_debug_string ();

private:
  type *m_return_type;
  vec<type *> m_param_types;
  int m_is_variadic;
};

class field : public memento
{
public:
  field (context *ctxt,
	 location *loc,
	 type *type,
	 string *name)
  : memento (ctxt),
    m_loc (loc),
    m_type (type),
    m_name (name),
    m_container (NULL)
  {}

  type * get_type () const { return m_type; }

  struct_ * get_container () const { return m_container; }
  void set_container (struct_ *c) { m_container = c; }

  void replay_into (replayer *);

  void write_to_dump (dump &d);

  playback::field *
  playback_field () const
  {
    return static_cast <playback::field *> (m_playback_obj);
  }

private:
  string * make_debug_string ();

private:
  location *m_loc;
  type *m_type;
  string *m_name;
  struct_ *m_container;
};

class struct_ : public type
{
public:
  struct_ (context *ctxt,
	   location *loc,
	   string *name);

  struct_ *dyn_cast_struct () { return this; }

  type *
  as_type () { return this; }

  string *get_name () const { return m_name; }
  fields * get_fields () { return m_fields; }

  void
  set_fields (location *loc,
	      int num_fields,
	      field **fields);

  type *dereference ();

  bool is_int () const { return false; }
  bool is_float () const { return false; }
  bool is_bool () const { return false; }
  type *is_pointer () { return NULL; }
  type *is_array () { return NULL; }

  void replay_into (replayer *r);

  playback::struct_ *
  playback_struct ()
  {
    return static_cast <playback::struct_ *> (m_playback_obj);
  }

private:
  string * make_debug_string ();

private:
  location *m_loc;
  string *m_name;
  fields *m_fields;
};

// memento of struct_::set_fields
class fields : public memento
{
public:
  fields (struct_ *struct_,
	  int num_fields,
	  field **fields);

  void replay_into (replayer *r);

  void write_to_dump (dump &d);

private:
  string * make_debug_string ();

private:
  struct_ *m_struct;
  vec<field *> m_fields;
};

class rvalue : public memento
{
public:
  rvalue (context *ctxt,
	  location *loc,
	  type *type_)
  : memento (ctxt),
    m_loc (loc),
    m_type (type_)
  {
    gcc_assert (type_);
  }

  type * get_type () const { return m_type; }

  playback::rvalue *
  playback_rvalue () const
  {
    return static_cast <playback::rvalue *> (m_playback_obj);
  }
  rvalue *
  access_field (location *loc,
		field *field);

  lvalue *
  dereference_field (location *loc,
		     field *field);

  lvalue *
  dereference (location *loc);

protected:
  location *m_loc;
  type *m_type;
};

class lvalue : public rvalue
{
public:
  lvalue (context *ctxt,
	  location *loc,
	  type *type_)
    : rvalue (ctxt, loc, type_)
    {}

  playback::lvalue *
  playback_lvalue () const
  {
    return static_cast <playback::lvalue *> (m_playback_obj);
  }

  lvalue *
  access_field (location *loc,
		field *field);

  rvalue *
  get_address (location *loc);

  rvalue *
  as_rvalue () { return this; }
};

class param : public lvalue
{
public:
  param (context *ctxt,
	 location *loc,
	 type *type,
	 string *name)
    : lvalue (ctxt, loc, type),
    m_name (name) {}

  lvalue *
  as_lvalue () { return this; }

  void replay_into (replayer *r);

  playback::param *
  playback_param () const
  {
    return static_cast <playback::param *> (m_playback_obj);
  }

private:
  string * make_debug_string () { return m_name; }

private:
  string *m_name;
};

class function : public memento
{
public:
  function (context *ctxt,
	    location *loc,
	    enum gcc_jit_function_kind kind,
	    type *return_type,
	    string *name,
	    int num_params,
	    param **params,
	    int is_variadic,
	    enum built_in_function builtin_id);

  void replay_into (replayer *r);

  playback::function *
  playback_function () const
  {
    return static_cast <playback::function *> (m_playback_obj);
  }

  enum gcc_jit_function_kind get_kind () const { return m_kind; }

  lvalue *
  new_local (location *loc,
	     type *type,
	     const char *name);

  block*
  new_block (const char *name);

  type *get_return_type () const { return m_return_type; }
  string * get_name () const { return m_name; }
  vec<param *> get_params () const { return m_params; }
  param *get_param (int i) const { return m_params[i]; }
  bool is_variadic () const { return m_is_variadic; }

  void write_to_dump (dump &d);

  void validate ();

  void dump_to_dot (const char *path);

private:
  string * make_debug_string ();

private:
  location *m_loc;
  enum gcc_jit_function_kind m_kind;
  type *m_return_type;
  string *m_name;
  vec<param *> m_params;
  int m_is_variadic;
  enum built_in_function m_builtin_id;
  vec<local *> m_locals;
  vec<block *> m_blocks;
};

class block : public memento
{
public:
  block (function *func, int index, string *name)
  : memento (func->m_ctxt),
    m_func (func),
    m_index (index),
    m_name (name),
    m_statements (),
    m_has_been_terminated (false),
    m_is_reachable (false)
  {
  }

  function *get_function () { return m_func; }

  bool has_been_terminated () { return m_has_been_terminated; }
  bool is_reachable () { return m_is_reachable; }

  void
  add_eval (location *loc,
	    rvalue *rvalue);

  void
  add_assignment (location *loc,
		  lvalue *lvalue,
		  rvalue *rvalue);

  void
  add_assignment_op (location *loc,
		     lvalue *lvalue,
		     enum gcc_jit_binary_op op,
		     rvalue *rvalue);

  void
  add_comment (location *loc,
	       const char *text);

  void
  end_with_conditional (location *loc,
			rvalue *boolval,
			block *on_true,
			block *on_false);

  void
  end_with_jump (location *loc,
		 block *target);

  void
  end_with_return (location *loc,
		   rvalue *rvalue);

  playback::block *
  playback_block () const
  {
    return static_cast <playback::block *> (m_playback_obj);
  }

  void write_to_dump (dump &d);

  bool validate ();

  location *get_loc () const;

  statement *get_first_statement () const;
  statement *get_last_statement () const;

  int get_successor_blocks (block **next1, block **next2) const;

private:
  string * make_debug_string ();

  void replay_into (replayer *r);

  void dump_to_dot (pretty_printer *pp);
  void dump_edges_to_dot (pretty_printer *pp);

private:
  function *m_func;
  int m_index;
  string *m_name;
  vec<statement *> m_statements;
  bool m_has_been_terminated;
  bool m_is_reachable;

  friend class function;
};

class global : public lvalue
{
public:
  global (context *ctxt,
	  location *loc,
	  type *type,
	  string *name)
  : lvalue (ctxt, loc, type),
    m_name (name)
  {}

  void replay_into (replayer *);

private:
  string * make_debug_string () { return m_name; }

private:
  string *m_name;
};

class memento_of_new_rvalue_from_int : public rvalue
{
public:
  memento_of_new_rvalue_from_int (context *ctxt,
				  location *loc,
				  type *numeric_type,
				  int value)
  : rvalue (ctxt, loc, numeric_type),
    m_value (value) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  int m_value;
};

class memento_of_new_rvalue_from_double : public rvalue
{
public:
  memento_of_new_rvalue_from_double (context *ctxt,
				     location *loc,
				     type *numeric_type,
				     double value)
  : rvalue (ctxt, loc, numeric_type),
    m_value (value)
  {}

  void replay_into (replayer *);

private:
  string * make_debug_string ();

private:
  double m_value;
};

class memento_of_new_rvalue_from_ptr : public rvalue
{
public:
  memento_of_new_rvalue_from_ptr (context *ctxt,
				  location *loc,
				  type *pointer_type,
				  void *value)
  : rvalue (ctxt, loc, pointer_type),
    m_value (value)
  {}

  void replay_into (replayer *);

private:
  string * make_debug_string ();

private:
  void *m_value;
};

class memento_of_new_string_literal : public rvalue
{
public:
  memento_of_new_string_literal (context *ctxt,
				 location *loc,
				 string *value)
  : rvalue (ctxt, loc, ctxt->get_type (GCC_JIT_TYPE_CONST_CHAR_PTR)),
    m_value (value) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  string *m_value;
};

class unary_op : public rvalue
{
public:
  unary_op (context *ctxt,
	    location *loc,
	    enum gcc_jit_unary_op op,
	    type *result_type,
	    rvalue *a)
  : rvalue (ctxt, loc, result_type),
    m_op (op),
    m_a (a)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  enum gcc_jit_unary_op m_op;
  rvalue *m_a;
};

class binary_op : public rvalue
{
public:
  binary_op (context *ctxt,
	     location *loc,
	     enum gcc_jit_binary_op op,
	     type *result_type,
	     rvalue *a, rvalue *b)
  : rvalue (ctxt, loc, result_type),
    m_op (op),
    m_a (a),
    m_b (b) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  enum gcc_jit_binary_op m_op;
  rvalue *m_a;
  rvalue *m_b;
};

class comparison : public rvalue
{
public:
  comparison (context *ctxt,
	      location *loc,
	      enum gcc_jit_comparison op,
	      rvalue *a, rvalue *b)
  : rvalue (ctxt, loc, ctxt->get_type (GCC_JIT_TYPE_BOOL)),
    m_op (op),
    m_a (a),
    m_b (b)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  enum gcc_jit_comparison m_op;
  rvalue *m_a;
  rvalue *m_b;
};

class cast : public rvalue
{
public:
  cast (context *ctxt,
	location *loc,
	rvalue *a,
	type *type_)
  : rvalue (ctxt, loc, type_),
    m_rvalue (a)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  rvalue *m_rvalue;
};

class call : public rvalue
{
public:
  call (context *ctxt,
	location *loc,
	function *func,
	int numargs,
	rvalue **args);

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  function *m_func;
  vec<rvalue *> m_args;
};

class array_access : public lvalue
{
public:
  array_access (context *ctxt,
		location *loc,
		rvalue *ptr,
		rvalue *index)
  : lvalue (ctxt, loc, ptr->get_type ()->dereference ()),
    m_ptr (ptr),
    m_index (index)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  rvalue *m_ptr;
  rvalue *m_index;
};

class access_field_of_lvalue : public lvalue
{
public:
  access_field_of_lvalue (context *ctxt,
			  location *loc,
			  lvalue *val,
			  field *field)
  : lvalue (ctxt, loc, field->get_type ()),
    m_lvalue (val),
    m_field (field)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  lvalue *m_lvalue;
  field *m_field;
};

class access_field_rvalue : public rvalue
{
public:
  access_field_rvalue (context *ctxt,
		       location *loc,
		       rvalue *val,
		       field *field)
  : rvalue (ctxt, loc, field->get_type ()),
    m_rvalue (val),
    m_field (field)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  rvalue *m_rvalue;
  field *m_field;
};

class dereference_field_rvalue : public lvalue
{
public:
  dereference_field_rvalue (context *ctxt,
			    location *loc,
			    rvalue *val,
			    field *field)
  : lvalue (ctxt, loc, field->get_type ()),
    m_rvalue (val),
    m_field (field)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  rvalue *m_rvalue;
  field *m_field;
};

class dereference_rvalue : public lvalue
{
public:
  dereference_rvalue (context *ctxt,
		      location *loc,
		      rvalue *val)
  : lvalue (ctxt, loc, val->get_type ()->dereference ()),
    m_rvalue (val) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  rvalue *m_rvalue;
};

class get_address_of_lvalue : public rvalue
{
public:
  get_address_of_lvalue (context *ctxt,
			 location *loc,
			 lvalue *val)
  : rvalue (ctxt, loc, val->get_type ()->get_pointer ()),
    m_lvalue (val)
  {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  lvalue *m_lvalue;
};

class local : public lvalue
{
public:
  local (function *func, location *loc, type *type_, string *name)
    : lvalue (func->m_ctxt, loc, type_),
    m_func (func),
    m_name (name) {}

  void replay_into (replayer *r);

  void write_to_dump (dump &d);

private:
  string * make_debug_string () { return m_name; }

private:
  function *m_func;
  string *m_name;
};

class statement : public memento
{
public:
  virtual int get_successor_blocks (block **out_next1,
				    block **out_next2) const;

  void write_to_dump (dump &d);

  location *get_loc () const { return m_loc; }

protected:
  statement (block *b, location *loc)
  : memento (b->m_ctxt),
    m_block (b),
    m_loc (loc) {}

  block *get_block () const { return m_block; }

  playback::location *
  playback_location (replayer *r) const
  {
    return ::gcc::jit::recording::playback_location (r, m_loc);
  }

private:
  block *m_block;
  location *m_loc;
};

class eval : public statement
{
public:
  eval (block *b,
	location *loc,
	rvalue *rvalue)
  : statement (b, loc),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  rvalue *m_rvalue;
};

class assignment : public statement
{
public:
  assignment (block *b,
	      location *loc,
	      lvalue *lvalue,
	      rvalue *rvalue)
  : statement (b, loc),
    m_lvalue (lvalue),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  lvalue *m_lvalue;
  rvalue *m_rvalue;
};

class assignment_op : public statement
{
public:
  assignment_op (block *b,
		 location *loc,
		 lvalue *lvalue,
		 enum gcc_jit_binary_op op,
		 rvalue *rvalue)
  : statement (b, loc),
    m_lvalue (lvalue),
    m_op (op),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  lvalue *m_lvalue;
  enum gcc_jit_binary_op m_op;
  rvalue *m_rvalue;
};

class comment : public statement
{
public:
  comment (block *b,
	   location *loc,
	   string *text)
  : statement (b, loc),
    m_text (text) {}

  void replay_into (replayer *r);

private:
  string * make_debug_string ();

private:
  string *m_text;
};

class conditional : public statement
{
public:
  conditional (block *b,
	       location *loc,
	       rvalue *boolval,
	       block *on_true,
	       block *on_false)
  : statement (b, loc),
    m_boolval (boolval),
    m_on_true (on_true),
    m_on_false (on_false) {}

  void replay_into (replayer *r);

  int get_successor_blocks (block **out_next1,
			    block **out_next2) const;

private:
  string * make_debug_string ();

private:
  rvalue *m_boolval;
  block *m_on_true;
  block *m_on_false;
};

class jump : public statement
{
public:
  jump (block *b,
	location *loc,
	block *target)
  : statement (b, loc),
    m_target (target) {}

  void replay_into (replayer *r);

  int get_successor_blocks (block **out_next1,
			    block **out_next2) const;

private:
  string * make_debug_string ();

private:
  block *m_target;
};

class return_ : public statement
{
public:
  return_ (block *b,
	   location *loc,
	   rvalue *rvalue)
  : statement (b, loc),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

  int get_successor_blocks (block **out_next1,
			    block **out_next2) const;

private:
  string * make_debug_string ();

private:
  rvalue *m_rvalue;
};

} // namespace gcc::jit::recording

/* The result of JIT-compilation.  */
class result
{
public:
  result(void *dso_handle);

  virtual ~result();

  void *
  get_code (const char *funcname);

private:
  void *m_dso_handle;
};

/**********************************************************************
 Playback.
 **********************************************************************/

namespace playback {

class context
{
public:
  context (::gcc::jit::recording::context *ctxt);
  ~context ();

  void gt_ggc_mx ();

  void replay ();

  location *
  new_location (recording::location *rloc,
		const char *filename,
		int line,
		int column);

  type *
  get_type (enum gcc_jit_types type);

  type *
  new_array_type (location *loc,
		  type *element_type,
		  int num_elements);

  field *
  new_field (location *loc,
	     type *type,
	     const char *name);

  struct_ *
  new_struct_type (location *loc,
		   const char *name);

  type *
  new_function_type (type *return_type,
		     vec<type *> *param_types,
		     int is_variadic);

  param *
  new_param (location *loc,
	     type *type,
	     const char *name);

  function *
  new_function (location *loc,
		enum gcc_jit_function_kind kind,
		type *return_type,
		const char *name,
		vec<param *> *params,
		int is_variadic,
		enum built_in_function builtin_id);

  lvalue *
  new_global (location *loc,
	      type *type,
	      const char *name);

  rvalue *
  new_rvalue_from_int (type *type,
		       int value);

  rvalue *
  new_rvalue_from_double (type *type,
			  double value);

  rvalue *
  new_rvalue_from_ptr (type *type,
		       void *value);

  rvalue *
  new_string_literal (const char *value);

  rvalue *
  new_unary_op (location *loc,
		enum gcc_jit_unary_op op,
		type *result_type,
		rvalue *a);

  rvalue *
  new_binary_op (location *loc,
		 enum gcc_jit_binary_op op,
		 type *result_type,
		 rvalue *a, rvalue *b);

  rvalue *
  new_comparison (location *loc,
		  enum gcc_jit_comparison op,
		  rvalue *a, rvalue *b);

  rvalue *
  new_call (location *loc,
	    function *func,
	    vec<rvalue *> args);

  rvalue *
  new_cast (location *loc,
	    rvalue *expr,
	    type *type_);

  lvalue *
  new_array_access (location *loc,
		    rvalue *ptr,
		    rvalue *index);

  void
  set_str_option (enum gcc_jit_str_option opt,
		  const char *value);

  void
  set_int_option (enum gcc_jit_int_option opt,
		  int value);

  void
  set_bool_option (enum gcc_jit_bool_option opt,
		   int value);

  const char *
  get_str_option (enum gcc_jit_str_option opt) const
  {
    return m_recording_ctxt->get_str_option (opt);
  }

  int
  get_int_option (enum gcc_jit_int_option opt) const
  {
    return m_recording_ctxt->get_int_option (opt);
  }

  int
  get_bool_option (enum gcc_jit_bool_option opt) const
  {
    return m_recording_ctxt->get_bool_option (opt);
  }

  result *
  compile ();

  void
  add_error (location *loc, const char *fmt, ...)
      GNU_PRINTF(3, 4);

  void
  add_error_va (location *loc, const char *fmt, va_list ap)
      GNU_PRINTF(3, 0);

  const char *
  get_first_error () const;

  void
  set_tree_location (tree t, location *loc);

  tree
  new_field_access (location *loc,
		    tree datum,
		    field *field);

  tree
  new_dereference (tree ptr, location *loc);

  tree
  as_truth_value (tree expr, location *loc);

  bool errors_occurred () const
  {
    return m_recording_ctxt->errors_occurred ();
  }

private:
  void dump_generated_code ();

  tree
  build_cast (location *loc,
	      rvalue *expr,
	      type *type_);

  source_file *
  get_source_file (const char *filename);

  void handle_locations ();

private:
  ::gcc::jit::recording::context *m_recording_ctxt;

  /* Allocated using xmalloc (by xstrdup).  */
  char *m_path_template;

  /* This either aliases m_path_template, or is NULL.  */
  char *m_path_tempdir;

  /* The following are allocated using xmalloc.  */
  char *m_path_c_file;
  char *m_path_s_file;
  char *m_path_so_file;

  vec<function *> m_functions;
  tree m_char_array_type_node;
  tree m_const_char_ptr;

  /* Source location handling.  */
  vec<source_file *> m_source_files;

  vec<std::pair<tree, location *> > m_cached_locations;
};

/* A temporary wrapper object.
   These objects are (mostly) only valid during replay.
   We allocate them on the GC heap, so that they will be cleaned
   the next time the GC collects.
   The exception is the "function" class, which is tracked and marked by
   the jit::context, since it needs to stay alive during post-processing
   (when the GC could run). */
class wrapper
{
public:
  /* Allocate in the GC heap.  */
  void *operator new (size_t sz);

};

class type : public wrapper
{
public:
  type (tree inner)
    : m_inner(inner)
  {}

  tree as_tree () const { return m_inner; }

  type *get_pointer () const { return new type (build_pointer_type (m_inner)); }

  type *get_const () const
  {
    return new type (build_qualified_type (m_inner, TYPE_QUAL_CONST));
  }

  type *get_volatile () const
  {
    return new type (build_qualified_type (m_inner, TYPE_QUAL_VOLATILE));
  }

private:
  tree m_inner;
};

class struct_ : public type
{
public:
  struct_ (tree inner)
    : type (inner)
  {}

  void set_fields (const vec<field *> &fields);
};

class field : public wrapper
{
public:
  field (tree inner)
    : m_inner(inner)
  {}

  tree as_tree () const { return m_inner; }

private:
  tree m_inner;
};

class function : public wrapper
{
public:
  function(context *ctxt, tree fndecl, enum gcc_jit_function_kind kind);

  void gt_ggc_mx ();

  tree get_return_type_as_tree () const;

  tree as_fndecl () const { return m_inner_fndecl; }

  enum gcc_jit_function_kind get_kind () const { return m_kind; }

  lvalue *
  new_local (location *loc,
	     type *type,
	     const char *name);

  block*
  new_block (const char *name);

  void
  build_stmt_list ();

  void
  postprocess ();

public:
  context *m_ctxt;

public:
  void
  set_tree_location (tree t, location *loc)
  {
    m_ctxt->set_tree_location (t, loc);
  }

private:
  tree m_inner_fndecl;
  tree m_inner_block;
  tree m_inner_bind_expr;
  enum gcc_jit_function_kind m_kind;
  tree m_stmt_list;
  tree_stmt_iterator m_stmt_iter;
  vec<block *> m_blocks;
};

class block : public wrapper
{
public:
  block (function *func,
	 const char *name);

  tree as_label_decl () const { return m_label_decl; }

  void
  add_eval (location *loc,
	    rvalue *rvalue);

  void
  add_assignment (location *loc,
		  lvalue *lvalue,
		  rvalue *rvalue);

  void
  add_comment (location *loc,
	       const char *text);

  void
  add_conditional (location *loc,
		   rvalue *boolval,
		   block *on_true,
		   block *on_false);

  block *
  add_block (location *loc,
	     const char *name);

  void
  add_jump (location *loc,
	    block *target);

  void
  add_return (location *loc,
	      rvalue *rvalue);

private:
  void
  set_tree_location (tree t, location *loc)
  {
    m_func->set_tree_location (t, loc);
  }

  void add_stmt (tree stmt)
  {
    /* TODO: use one stmt_list per block.  */
    m_stmts.safe_push (stmt);
  }

private:
  function *m_func;
  tree m_label_decl;
  vec<tree> m_stmts;

public: // for now
  tree m_label_expr;

  friend class function;
};

class rvalue : public wrapper
{
public:
  rvalue (context *ctxt, tree inner)
    : m_ctxt (ctxt),
      m_inner (inner)
  {}

  rvalue *
  as_rvalue () { return this; }

  tree as_tree () const { return m_inner; }

  context *get_context () const { return m_ctxt; }

  type *
  get_type () { return new type (TREE_TYPE (m_inner)); }

  rvalue *
  access_field (location *loc,
		field *field);

  lvalue *
  dereference_field (location *loc,
		     field *field);

  lvalue *
  dereference (location *loc);

private:
  context *m_ctxt;
  tree m_inner;
};

class lvalue : public rvalue
{
public:
  lvalue (context *ctxt, tree inner)
    : rvalue(ctxt, inner)
  {}

  lvalue *
  as_lvalue () { return this; }

  lvalue *
  access_field (location *loc,
		field *field);

  rvalue *
  get_address (location *loc);

};

class param : public lvalue
{
public:
  param (context *ctxt, tree inner)
    : lvalue(ctxt, inner)
  {}
};

/* Dealing with the linemap API.

   It appears that libcpp requires locations to be created as if by
   a tokenizer, creating them by filename, in ascending order of
   line/column, whereas our API doesn't impose any such constraints:
   we allow client code to create locations in arbitrary orders.

   To square this circle, we need to cache all location creation,
   grouping things up by filename/line, and then creating the linemap
   entries in a post-processing phase.  */

/* A set of locations, all sharing a filename */
class source_file : public wrapper
{
public:
  source_file (tree filename);

  source_line *
  get_source_line (int line_num);

  tree filename_as_tree () const { return m_filename; }

  const char*
  get_filename () const { return IDENTIFIER_POINTER (m_filename); }

  vec<source_line *> m_source_lines;

private:
  tree m_filename;
};

/* A source line, with one or more locations of interest.  */
class source_line : public wrapper
{
public:
  source_line (source_file *file, int line_num);

  location *
  get_location (recording::location *rloc, int column_num);

  int get_line_num () const { return m_line_num; }

  vec<location *> m_locations;

private:
  source_file *m_source_file;
  int m_line_num;
};

/* A specific location on a source line.  This is what we expose
   to the client API.  */
class location : public wrapper
{
public:
  location (recording::location *loc, source_line *line, int column_num);

  int get_column_num () const { return m_column_num; }

  recording::location *get_recording_loc () const { return m_recording_loc; }

  source_location m_srcloc;

private:
  recording::location *m_recording_loc;
  source_line *m_line;
  int m_column_num;
};

} // namespace gcc::jit::playback

extern playback::context *active_playback_ctxt;

} // namespace gcc::jit

} // namespace gcc

#endif /* INTERNAL_API_H */
