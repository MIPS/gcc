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

namespace recording {
  class context;
  class memento;
  class string;
  class location;
  class type;
  class field;
  class function;
  class label;
  class rvalue;
  class lvalue;
  class param;
  class loop;
}

namespace playback {
  class context;
  class location;
  class type;
  class field;
  class function;
  class label;
  class rvalue;
  class lvalue;
  class param;
  class loop;
  class source_file;
  class source_line;
}

typedef playback::context replayer;

/**********************************************************************
 Recording.
 **********************************************************************/

namespace recording {

playback::location *
playback_location (location *loc);

const char *
playback_string (string *str);

playback::label *
playback_label (label *lab);

/* A JIT-compilation context.  */
class context
{
public:
  context (context *parent_ctxt);
  ~context ();

  void record (memento *m) { m_mementos.safe_push (m); }
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

  field *
  new_field (location *loc,
	     type *type,
	     const char *name);

  type *
  new_struct_type (location *loc,
		   const char *name,
		   int num_fields,
		   field **fields);


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
		int is_variadic);

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
	    int numargs, rvalue **args);

  rvalue *
  new_array_lookup (location *loc,
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
  add_error (const char *fmt, ...)
      GNU_PRINTF(2, 3);

  void
  add_error_va (const char *fmt, va_list ap)
      GNU_PRINTF(2, 0);

  const char *
  get_first_error () const;

  bool errors_occurred () const
  {
    return m_error_count;
  }

private:
  context *m_parent_ctxt;

  int m_error_count;
  char m_first_error_str[1024];

  const char *m_str_options[GCC_JIT_NUM_STR_OPTIONS];
  int m_int_options[GCC_JIT_NUM_INT_OPTIONS];
  bool m_bool_options[GCC_JIT_NUM_BOOL_OPTIONS];

  /* Recorded API usage.  */
  vec<memento *> m_mementos;

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

protected:
  memento (context *ctxt)
  : m_ctxt (ctxt),
    m_playback_obj (NULL)
  {}

  string *new_string (const char *text) { return m_ctxt->new_string (text); }

public:
  context *m_ctxt;

protected:
  void *m_playback_obj;
};

/* or just use std::string? */
class string : public memento
{
public:
  string (context *ctxt, const char *text);
  ~string ();

  const char *c_str () { return m_copy; }

  void replay_into (replayer *) {}

private:
  size_t m_len;
  char *m_copy;
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
  playback_location () const
  {
    return static_cast <playback::location *> (m_playback_obj);
  }

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

  playback::type *
  playback_type ()
  {
    return static_cast <playback::type *> (m_playback_obj);
  }

protected:
  type (context *ctxt)
    : memento (ctxt) {}

};

/* Result of "gcc_jit_type_get_type".  */
class memento_of_get_type : public type
{
public:
  memento_of_get_type (context *ctxt,
		       enum gcc_jit_types kind)
  : type (ctxt),
    m_kind (kind) {}

public:
  void replay_into (replayer *r);

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

  void replay_into (replayer *r);

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

  void replay_into (replayer *);

private:
  type *m_other_type;
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
    m_name (name)
  {}

  void replay_into (replayer *);

  playback::field *
  playback_field () const
  {
    return static_cast <playback::field *> (m_playback_obj);
  }

private:
  location *m_loc;
  type *m_type;
  string *m_name;
};

class struct_ : public type
{
public:
  struct_ (context *ctxt,
	   location *loc,
	   string *name,
	   vec<field *> fields)
  : type (ctxt),
    m_loc (loc),
    m_name (name),
    m_fields (fields)
  {}

  void replay_into (replayer *r);

private:
  location *m_loc;
  string *m_name;
  vec<field *> m_fields;
};

class rvalue : public memento
{
public:
  rvalue (context *ctxt,
	  location *loc)
  : memento (ctxt),
    m_loc (loc)
  {}

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
};

class lvalue : public rvalue
{
public:
  lvalue (context *ctxt,
	  location *loc)
    : rvalue (ctxt, loc)
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
  : lvalue (ctxt, loc),
    m_type (type),
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
  type *m_type;
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
	    int is_variadic);

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

  label*
  new_forward_label (const char *name);

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
  add_conditional (location *loc,
		   rvalue *boolval,
		   label *on_true,
		   label *on_false);

  label *
  add_label (location *loc,
	     const char *name);

  void
  place_forward_label (location *loc, label *lab);

  void
  add_jump (location *loc,
	    label *target);

  void
  add_return (location *loc,
	      rvalue *rvalue);

  loop *
  new_loop (location *loc,
	    rvalue *boolval);

  string * get_name () const { return m_name; }
  vec<param *> get_params () const { return m_params; }
  bool is_variadic () const { return m_is_variadic; }

private:
  location *m_loc;
  enum gcc_jit_function_kind m_kind;
  type *m_return_type;
  string *m_name;
  vec<param *> m_params;
  int m_is_variadic;
};

class label : public memento
{
public:
  label (function *func, string *name)
  : memento (func->m_ctxt),
    m_func (func),
    m_name (name)
  {
  }

  void replay_into (replayer *r);

  playback::label *
  playback_label () const
  {
    return static_cast <playback::label *> (m_playback_obj);
  }

private:
  function *m_func;
  string *m_name;
};

class global : public lvalue
{
public:
  global (context *ctxt,
	  location *loc,
	  type *type,
	  string *name)
  : lvalue (ctxt, loc),
    m_type (type),
    m_name (name)
  {}

  void replay_into (replayer *);

private:
  type *m_type;
  string *m_name;
};

class memento_of_new_rvalue_from_int : public rvalue
{
public:
  memento_of_new_rvalue_from_int (context *ctxt,
				  location *loc,
				  type *type,
				  int value)
  : rvalue (ctxt, loc),
    m_type (type),
    m_value (value) {}

  void replay_into (replayer *r);

private:
  type *m_type;
  int m_value;
};

class memento_of_new_rvalue_from_double : public rvalue
{
public:
  memento_of_new_rvalue_from_double (context *ctxt,
				     location *loc,
				     type *type,
				     double value)
  : rvalue (ctxt, loc),
    m_type (type),
    m_value (value)
  {}

  void replay_into (replayer *);

private:
  type *m_type;
  double m_value;
};

class memento_of_new_rvalue_from_ptr : public rvalue
{
public:
  memento_of_new_rvalue_from_ptr (context *ctxt,
				  location *loc,
				  type *type,
				  void *value)
  : rvalue (ctxt, loc),
    m_type (type),
    m_value (value)
  {}

  void replay_into (replayer *);

private:
  type *m_type;
  void *m_value;
};

class memento_of_new_string_literal : public rvalue
{
public:
  memento_of_new_string_literal (context *ctxt,
				 location *loc,
				 string *value)
  : rvalue (ctxt, loc),
    m_value (value) {}

  void replay_into (replayer *r);

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
  : rvalue (ctxt, loc),
    m_op (op),
    m_result_type (result_type),
    m_a (a)
  {}

  void replay_into (replayer *r);

private:
  enum gcc_jit_unary_op m_op;
  type *m_result_type;
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
  : rvalue (ctxt, loc),
    m_op (op),
    m_result_type (result_type),
    m_a (a),
    m_b (b) {}

  void replay_into (replayer *r);

private:
  enum gcc_jit_binary_op m_op;
  type *m_result_type;
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
  : rvalue (ctxt, loc),
    m_op (op),
    m_a (a),
    m_b (b)
  {}

  void replay_into (replayer *r);

private:
  enum gcc_jit_comparison m_op;
  rvalue *m_a;
  rvalue *m_b;
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
  function *m_func;
  vec<rvalue *> m_args;
};

class array_lookup : public rvalue
{
public:
  array_lookup (context *ctxt,
		location *loc,
		rvalue *ptr,
		rvalue *index)
  : rvalue (ctxt, loc),
    m_ptr (ptr),
    m_index (index)
  {}

  void replay_into (replayer *r);

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
  : lvalue (ctxt, loc),
    m_lvalue (val),
    m_field (field)
  {}

  void replay_into (replayer *r);

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
  : rvalue (ctxt, loc),
    m_rvalue (val),
    m_field (field)
  {}

  void replay_into (replayer *r);

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
  : lvalue (ctxt, loc),
    m_rvalue (val),
    m_field (field)
  {}

  void replay_into (replayer *r);

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
  : lvalue (ctxt, loc),
    m_rvalue (val) {}

  void replay_into (replayer *r);

private:
  rvalue *m_rvalue;
};

class get_address_of_lvalue : public rvalue
{
public:
  get_address_of_lvalue (context *ctxt,
			 location *loc,
			 lvalue *val)
  : rvalue (ctxt, loc),
    m_lvalue (val)
  {}

  void replay_into (replayer *r);

private:
  lvalue *m_lvalue;
};

class local : public lvalue
{
public:
  local (function *func, location *loc, type *type_, string *name)
  : lvalue (func->m_ctxt, loc),
    m_func (func),
    m_type (type_),
    m_name (name) {}

  void replay_into (replayer *r);

private:
  function *m_func;
  type *m_type;
  string *m_name;
};

class statement : public memento
{
protected:
  statement (function *func, location *loc)
  : memento (func->m_ctxt),
    m_func (func),
    m_loc (loc) {}

  playback::function *
  playback_function () const
  {
    return m_func->playback_function ();
  }

  playback::location *
  playback_location () const
  {
    return ::gcc::jit::recording::playback_location (m_loc);
  }

private:
  function *m_func;
  location *m_loc;
};

class eval : public statement
{
public:
  eval (function *func,
	location *loc,
	rvalue *rvalue)
  : statement (func, loc),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  rvalue *m_rvalue;
};

class assignment : public statement
{
public:
  assignment (function *func,
	      location *loc,
	      lvalue *lvalue,
	      rvalue *rvalue)
  : statement (func, loc),
    m_lvalue (lvalue),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  lvalue *m_lvalue;
  rvalue *m_rvalue;
};

class assignment_op : public statement
{
public:
  assignment_op (function *func,
		 location *loc,
		 lvalue *lvalue,
		 enum gcc_jit_binary_op op,
		 rvalue *rvalue)
  : statement (func, loc),
    m_lvalue (lvalue),
    m_op (op),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  lvalue *m_lvalue;
  enum gcc_jit_binary_op m_op;
  rvalue *m_rvalue;
};

class comment : public statement
{
public:
  comment (function *func,
	   location *loc,
	   string *text)
  : statement (func, loc),
    m_text (text) {}

  void replay_into (replayer *r);

private:
  string *m_text;
};

class conditional : public statement
{
public:
  conditional (function *func,
	       location *loc,
	       rvalue *boolval,
	       label *on_true,
	       label *on_false)
  : statement (func, loc),
    m_boolval (boolval),
    m_on_true (on_true),
    m_on_false (on_false) {}

  void replay_into (replayer *r);

private:
  rvalue *m_boolval;
  label *m_on_true;
  label *m_on_false;
};

class place_label : public statement
{
public:
  place_label (function *func,
	       location *loc,
	       label *lab)
  : statement (func, loc),
    m_label (lab) {}

  void replay_into (replayer *r);

private:
  label *m_label;
};

class jump : public statement
{
public:
  jump (function *func,
	location *loc,
	label *target)
  : statement (func, loc),
    m_target (target) {}

  void replay_into (replayer *r);

private:
  label *m_target;
};

class return_ : public statement
{
public:
  return_ (function *func,
	   location *loc,
	   rvalue *rvalue)
  : statement (func, loc),
    m_rvalue (rvalue) {}

  void replay_into (replayer *r);

private:
  rvalue *m_rvalue;
};

class loop : public memento
{
public:
  loop (function *func,
	location *loc,
	rvalue *boolval)
  : memento (func->m_ctxt),
    m_func (func),
    m_loc (loc),
    m_boolval (boolval) {}

  void replay_into (replayer *r);

  void end (location *loc);

  playback::loop *
  playback_loop ()
  {
    return static_cast <playback::loop *> (m_playback_obj);
  }

private:
  function *m_func;
  location *m_loc;
  rvalue *m_boolval;
};

class loop_end : public memento
{
public:
  loop_end (loop *loop,
	    location *loc)
  : memento (loop->m_ctxt),
    m_loop (loop),
    m_loc (loc)
  {}

  void replay_into (replayer *r);

private:
  loop *m_loop;
  location *m_loc;
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
  new_location (const char *filename,
		int line,
		int column);

  type *
  get_type (enum gcc_jit_types type);

  field *
  new_field (location *loc,
	     type *type,
	     const char *name);

  type *
  new_struct_type (location *loc,
		   const char *name,
		   vec<field *> *fields);

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
		int is_variadic);

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
  new_array_lookup (location *loc,
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
  add_error (const char *fmt, ...)
      GNU_PRINTF(2, 3);

  void
  add_error_va (const char *fmt, va_list ap)
      GNU_PRINTF(2, 0);

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

private:
  void dump_generated_code ();

  source_file *
  get_source_file (const char *filename);

  void handle_locations ();

  bool errors_occurred () const
  {
    return m_recording_ctxt->errors_occurred ();
  }

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

private:
  tree m_inner;
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

  label*
  new_forward_label (const char *name);

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
		   label *on_true,
		   label *on_false);

  label *
  add_label (location *loc,
	     const char *name);

  void
  place_forward_label (location *loc, label *lab);

  void
  add_jump (location *loc,
	    label *target);

  void
  add_return (location *loc,
	      rvalue *rvalue);

  loop *
  new_loop (location *loc,
	    rvalue *boolval);

  void
  postprocess ();

public:
  context *m_ctxt;

private:
  void add_stmt (tree stmt)
  {
    tsi_link_after (&m_stmt_iter, stmt, TSI_CONTINUE_LINKING);
  }

  void
  set_tree_location (tree t, location *loc)
  {
    m_ctxt->set_tree_location (t, loc);
  }

private:
  tree m_inner_fndecl;
  tree m_inner_bind_expr;
  enum gcc_jit_function_kind m_kind;
  tree m_stmt_list;
  tree_stmt_iterator m_stmt_iter;
};

class label : public wrapper
{
public:
  label (function *func,
	 const char *name);

  tree as_label_decl () const { return m_label_decl; }

private:
  tree m_label_decl;

public: // for now
  tree m_label_expr;
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

class loop : public wrapper
{
public:
  loop (function *func, location *loc, rvalue *boolval);

  void end (location *loc);

private:
  function *m_func;
  label *m_label_cond;
  label *m_label_body;
  label *m_label_end;
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
  get_location (int column_num);

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
  location (source_line *line, int column_num);

  int get_column_num () const { return m_column_num; }

  source_location m_srcloc;

private:
  source_line *m_line;
  int m_column_num;
};

} // namespace gcc::jit::playback

extern playback::context *active_playback_ctxt;

} // namespace gcc::jit

} // namespace gcc

