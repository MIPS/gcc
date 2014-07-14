#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"
#include "opts.h"
#include "tree.h"
#include "debug.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "cgraph.h"
#include "toplev.h"
#include "tree-iterator.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "timevar.h"
#include "diagnostic-core.h"
#include "dumpfile.h"
#include "tree-cfg.h"
#include "target.h"
#include "convert.h"

#include <pthread.h>

#include "internal-api.h"
#include "jit-builtins.h"

/* gcc::jit::playback::context::build_cast uses the convert.h API,
   which in turn requires the frontend to provide a "convert"
   function, apparently as a fallback.

   Hence we provide this dummy one, with the requirement that any casts
   are handled before reaching this.  */
extern tree convert (tree type, tree expr);

tree
convert (tree dst_type, tree expr)
{
  gcc_assert (gcc::jit::active_playback_ctxt);
  gcc::jit::active_playback_ctxt->add_error (NULL, "unhandled conversion");
  fprintf (stderr, "input expression:\n");
  debug_tree (expr);
  fprintf (stderr, "requested type:\n");
  debug_tree (dst_type);
  return error_mark_node;
}

namespace gcc {
namespace jit {

// class dump

dump::dump (recording::context &ctxt,
	    const char *filename,
	    bool update_locations)
: m_ctxt (ctxt),
  m_filename (filename),
  m_update_locations (update_locations),
  m_line (0),
  m_column (0)
{
  m_file  = fopen (filename, "w");
}

dump::~dump ()
{
  if (m_file)
    fclose (m_file);
}

void
dump::write (const char *fmt, ...)
{
  char buf[4096];
  va_list ap;
  va_start (ap, fmt);
  vsnprintf (buf, sizeof (buf), fmt, ap);
  va_end (ap);

  fwrite (buf, strlen (buf), 1, m_file);

  /* Update line/column: */
  for (const char *ptr = buf; *ptr; ptr++)
    {
      if ('\n' == *ptr)
	{
	  m_line++;
	  m_column = 0;
	}
      else
	m_column++;
    }
}

recording::location *
dump::make_location () const
{
  return m_ctxt.new_location (m_filename, m_line, m_column);
}

/**********************************************************************
 Recording.
 **********************************************************************/

playback::location *
recording::playback_location (replayer *r, recording::location *loc)
{
  if (loc)
    return loc->playback_location (r);
  else
    return NULL;
}

const char *
recording::playback_string (recording::string *str)
{
  if (str)
    return str->c_str ();
  else
    return NULL;
}

playback::block *
recording::playback_block (recording::block *b)
{
  if (b)
    return b->playback_block ();
  else
    return NULL;
}

/* gcc::jit::recording::context:: */

recording::context::context (context *parent_ctxt)
  : m_parent_ctxt (parent_ctxt),
    m_error_count (0),
    m_mementos (),
    m_structs (),
    m_functions (),
    m_FILE_type (NULL),
    m_builtins_manager(NULL)
{
  m_first_error_str[0] = '\0';

  if (parent_ctxt)
    {
      /* Inherit options from parent.
         Note that the first memcpy means copying pointers to strings.  */
      memcpy (m_str_options,
              parent_ctxt->m_str_options,
              sizeof (m_str_options));
      memcpy (m_int_options,
              parent_ctxt->m_int_options,
              sizeof (m_int_options));
      memcpy (m_bool_options,
              parent_ctxt->m_bool_options,
              sizeof (m_bool_options));
    }
  else
    {
      memset (m_str_options, 0, sizeof (m_str_options));
      memset (m_int_options, 0, sizeof (m_int_options));
      memset (m_bool_options, 0, sizeof (m_bool_options));
    }

  memset (m_basic_types, 0, sizeof (m_basic_types));
}

recording::context::~context ()
{
  int i;
  memento *m;
  FOR_EACH_VEC_ELT (m_mementos, i, m)
    {
      delete m;
    }

  if (m_builtins_manager)
    delete m_builtins_manager;
}

void
recording::context::record (memento *m)
{
  gcc_assert (m);

  m_mementos.safe_push (m);
}

void
recording::context::replay_into (replayer *r)
{
  int i;
  memento *m;

  /* If we have a parent context, we must replay it.  This will
     recursively walk backwards up the historical tree, then replay things
     forwards "in historical order", starting with the ultimate parent
     context, until we reach the "this" context.

     Note that we fully replay the parent, then fully replay the child,
     which means that inter-context references can only exist from child
     to parent, not the other way around.

     All of this replaying is suboptimal - it would be better to do the
     work for the parent context *once*, rather than replaying the parent
     every time we replay each child.  However, fixing this requires deep
     surgery to lifetime-management: we'd need every context family tree
     to have its own GC heap, and to initialize the GCC code to use that
     heap (with a mutex on such a heap).  */
  if (m_parent_ctxt)
    m_parent_ctxt->replay_into (r);

  if (r->errors_occurred ())
    return;

  /* Replay this context's saved operations into r.  */
  FOR_EACH_VEC_ELT (m_mementos, i, m)
    {
      /* Disabled low-level debugging, here if we need it: print what
	 we're replaying.
	 Note that the calls to get_debug_string might lead to more
	 mementos being created for the strings.
	 This can also be used to exercise the debug_string
	 machinery.  */
      if (0)
	printf ("context %p replaying (%p): %s\n",
		(void *)this, (void *)m, m->get_debug_string ());

      m->replay_into (r);

      if (r->errors_occurred ())
	return;
    }
}

/* During a playback, we associate objects from the recording with
   their counterparts during this playback.

   For simplicity, we store this within the recording objects.

   The following method cleans away these associations, to ensure that
   we never have out-of-date associations lingering on subsequent
   playbacks (the objects pointed to are GC-managed, but the
   recording objects don't own refs to them).  */

void
recording::context::disassociate_from_playback ()
{
  int i;
  memento *m;

  if (m_parent_ctxt)
    m_parent_ctxt->disassociate_from_playback ();

  FOR_EACH_VEC_ELT (m_mementos, i, m)
    {
      m->set_playback_obj (NULL);
    }
}

recording::string *
recording::context::new_string (const char *text)
{
  if (!text)
    return NULL;

  recording::string *result = new string (this, text);
  record (result);
  return result;
}

recording::location *
recording::context::new_location (const char *filename,
				 int line,
				 int column)
{
  recording::location *result =
    new recording::location (this,
			    new_string (filename),
			    line, column);
  record (result);
  return result;
}

recording::type *
recording::context::get_type (enum gcc_jit_types kind)
{
  /* Cache and reuse the types, so that repeated calls on the context
     give the same object.  */
  if (!m_basic_types[kind])
    {
      /* For basic types, use them from the parent ctxt.  */
      if (m_parent_ctxt)
	m_basic_types[kind] = m_parent_ctxt->get_type (kind);
      else
	{
	  recording::type *result = new memento_of_get_type (this, kind);
	  record (result);
	  m_basic_types[kind] = result;
	}
    }

  return m_basic_types[kind];
}

recording::type *
recording::context::get_int_type (int num_bytes, int is_signed)
{
  /* We can't use a switch here since some of the values are macros affected
     by options; e.g. i386.h has
       #define LONG_TYPE_SIZE (TARGET_X32 ? 32 : BITS_PER_WORD)
     Compare with tree.c's make_or_reuse_type.  Note that the _SIZE macros
     are in bits, rather than bytes.
  */
  const int num_bits = num_bytes * 8;
  if (num_bits == INT_TYPE_SIZE)
    return get_type (is_signed
		     ? GCC_JIT_TYPE_INT
		     : GCC_JIT_TYPE_UNSIGNED_INT);
  if (num_bits == CHAR_TYPE_SIZE)
    return get_type (is_signed
		     ? GCC_JIT_TYPE_SIGNED_CHAR
		     : GCC_JIT_TYPE_UNSIGNED_CHAR);
  if (num_bits == SHORT_TYPE_SIZE)
    return get_type (is_signed
		     ? GCC_JIT_TYPE_SHORT
		     : GCC_JIT_TYPE_UNSIGNED_SHORT);
  if (num_bits == LONG_TYPE_SIZE)
    return get_type (is_signed
		     ? GCC_JIT_TYPE_LONG
		     : GCC_JIT_TYPE_UNSIGNED_LONG);
  if (num_bits == LONG_LONG_TYPE_SIZE)
    return get_type (is_signed
		     ? GCC_JIT_TYPE_LONG_LONG
		     : GCC_JIT_TYPE_UNSIGNED_LONG_LONG);

  /* Some other size, not corresponding to the C int types.  */
  /* To be written: support arbitrary other sizes, sharing by
     memoizing at the recording::context level?  */
  gcc_unreachable ();
}

recording::type *
recording::context::new_array_type (recording::location *loc,
				    recording::type *element_type,
				    int num_elements)
{
  if (struct_ *s = element_type->dyn_cast_struct ())
    if (!s->get_fields ())
      {
	add_error (NULL,
		   "cannot create an array of type %s"
		   " until the fields have been set",
		   s->get_name ()->c_str ());
	return NULL;
      }
  recording::type *result =
    new recording::array_type (this, loc, element_type, num_elements);
  record (result);
  return result;
}

recording::field *
recording::context::new_field (recording::location *loc,
			       recording::type *type,
			       const char *name)
{
  recording::field *result =
    new recording::field (this, loc, type, new_string (name));
  record (result);
  return result;
}

recording::struct_ *
recording::context::new_struct_type (recording::location *loc,
				     const char *name)
{
  recording::struct_ *result = new struct_ (this, loc, new_string (name));
  record (result);
  m_structs.safe_push (result);
  return result;
}


recording::param *
recording::context::new_param (recording::location *loc,
			       recording::type *type,
			       const char *name)
{
  recording::param *result = new recording::param (this, loc, type, new_string (name));
  record (result);
  return result;
}

recording::function *
recording::context::new_function (recording::location *loc,
				  enum gcc_jit_function_kind kind,
				  recording::type *return_type,
				  const char *name,
				  int num_params,
				  recording::param **params,
				  int is_variadic,
				  enum built_in_function builtin_id)
{
  recording::function *result =
    new recording::function (this,
			     loc, kind, return_type,
			     new_string (name),
			     num_params, params, is_variadic,
			     builtin_id);
  record (result);
  m_functions.safe_push (result);

  return result;
}

recording::function *
recording::context::get_builtin_function (const char *name)
{
  if (!m_builtins_manager)
    m_builtins_manager = new builtins_manager (this);
  return m_builtins_manager->get_builtin_function (name);
}

recording::lvalue *
recording::context::new_global (recording::location *loc,
				recording::type *type,
				const char *name)
{
  recording::lvalue *result =
    new recording::global (this, loc, type, new_string (name));
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_rvalue_from_int (recording::type *type,
					 int value)
{
  recording::rvalue *result =
    new memento_of_new_rvalue_from_int (this, NULL, type, value);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_rvalue_from_double (recording::type *type,
					    double value)
{
  recording::rvalue *result =
    new memento_of_new_rvalue_from_double (this, NULL, type, value);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_rvalue_from_ptr (recording::type *type,
					 void *value)
{
  recording::rvalue *result =
    new memento_of_new_rvalue_from_ptr (this, NULL, type, value);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_string_literal (const char *value)
{
  recording::rvalue *result =
    new memento_of_new_string_literal (this, NULL, new_string (value));
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_unary_op (recording::location *loc,
				  enum gcc_jit_unary_op op,
				  recording::type *result_type,
				  recording::rvalue *a)
{
  recording::rvalue *result =
    new unary_op (this, loc, op, result_type, a);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_binary_op (recording::location *loc,
				   enum gcc_jit_binary_op op,
				   recording::type *result_type,
				   recording::rvalue *a,
				   recording::rvalue *b)
{
  recording::rvalue *result =
    new binary_op (this, loc, op, result_type, a, b);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_comparison (recording::location *loc,
				    enum gcc_jit_comparison op,
				    recording::rvalue *a,
				    recording::rvalue *b)
{
  recording::rvalue *result = new comparison (this, loc, op, a, b);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_cast (recording::location *loc,
			      recording::rvalue *expr,
			      recording::type *type_)
{
  recording::rvalue *result = new cast (this, loc, expr, type_);
  record (result);
  return result;
}

recording::rvalue *
recording::context::new_call (recording::location *loc,
			      function *func,
			      int numargs , recording::rvalue **args)
{
  recording::rvalue *result = new call (this, loc, func, numargs, args);
  record (result);
  return result;
}

recording::lvalue *
recording::context::new_array_access (recording::location *loc,
				      recording::rvalue *ptr,
				      recording::rvalue *index)
{
  recording::lvalue *result = new array_access (this, loc, ptr, index);
  record (result);
  return result;
}

void
recording::context::set_str_option (enum gcc_jit_str_option opt,
				    const char *value)
{
  if (opt < 0 || opt >= GCC_JIT_NUM_STR_OPTIONS)
    {
      add_error (NULL,
		 "unrecognized (enum gcc_jit_str_option) value: %i", opt);
      return;
    }
  m_str_options[opt] = value;
}

void
recording::context::set_int_option (enum gcc_jit_int_option opt,
				    int value)
{
  if (opt < 0 || opt >= GCC_JIT_NUM_INT_OPTIONS)
    {
      add_error (NULL,
		 "unrecognized (enum gcc_jit_int_option) value: %i", opt);
      return;
    }
  m_int_options[opt] = value;
}

void
recording::context::set_bool_option (enum gcc_jit_bool_option opt,
				     int value)
{
  if (opt < 0 || opt >= GCC_JIT_NUM_BOOL_OPTIONS)
    {
      add_error (NULL,
		 "unrecognized (enum gcc_jit_bool_option) value: %i", opt);
      return;
    }
  m_bool_options[opt] = value ? true : false;
}

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

result *
recording::context::compile ()
{
  validate ();

  if (errors_occurred ())
    return NULL;

  /* Acquire the big GCC mutex. */
  pthread_mutex_lock (&mutex);
  gcc_assert (NULL == ::gcc::jit::active_playback_ctxt);

  /* Set up a playback context.  */
  ::gcc::jit::playback::context replayer (this);
  ::gcc::jit::active_playback_ctxt = &replayer;

  result *result_obj = replayer.compile ();

  /* Release the big GCC mutex. */
  ::gcc::jit::active_playback_ctxt = NULL;
  pthread_mutex_unlock (&mutex);

  return result_obj;
}

void
recording::context::add_error (location *loc, const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  add_error_va (loc, fmt, ap);
  va_end (ap);
}

void
recording::context::add_error_va (location *loc, const char *fmt, va_list ap)
{
  char buf[1024];
  vsnprintf (buf, sizeof (buf) - 1, fmt, ap);

  const char *ctxt_progname =
    get_str_option (GCC_JIT_STR_OPTION_PROGNAME);
  if (!ctxt_progname)
    ctxt_progname = "libgccjit.so";

  if (loc)
    fprintf (stderr, "%s: %s: error: %s\n",
	     ctxt_progname,
	     loc->get_debug_string (),
	     buf);
  else
    fprintf (stderr, "%s: error: %s\n",
	     ctxt_progname,
	     buf);

  if (!m_error_count)
    {
      strncpy (m_first_error_str, buf, sizeof(m_first_error_str));
      m_first_error_str[sizeof(m_first_error_str) - 1] = '\0';
    }

  m_error_count++;
}

const char *
recording::context::get_first_error () const
{
  if (m_error_count)
    return m_first_error_str;
  else
    return NULL;
}

recording::type *
recording::context::get_opaque_FILE_type ()
{
  if (!m_FILE_type)
    m_FILE_type = new_struct_type (NULL, "FILE");
  return m_FILE_type;
}

void
recording::context::dump_to_file (const char *path, bool update_locations)
{
  int i;
  dump d (*this, path, update_locations);

  /* Forward declaration of structs.  */
  struct_ *st;
  FOR_EACH_VEC_ELT (m_structs, i, st)
    {
      d.write ("%s;\n\n", st->get_debug_string ());
    }

  /* Content of structs, where set.  */
  FOR_EACH_VEC_ELT (m_structs, i, st)
    if (st->get_fields ())
      {
	st->get_fields ()->write_to_dump (d);
	d.write ("\n");
      }

  function *fn;
  FOR_EACH_VEC_ELT (m_functions, i, fn)
    {
      fn->write_to_dump (d);
    }
}

void
recording::context::validate ()
{
  if (m_parent_ctxt)
    m_parent_ctxt->validate ();

  int i;
  function *fn;
  FOR_EACH_VEC_ELT (m_functions, i, fn)
    fn->validate ();
}

/* gcc::jit::recording::memento:: */

const char *
recording::memento::get_debug_string ()
{
  if (!m_debug_string)
    m_debug_string = make_debug_string ();
  return m_debug_string->c_str ();
}

void
recording::memento::write_to_dump (dump &d)
{
  d.write("  %s\n", get_debug_string ());
}

/* gcc::jit::recording::string:: */
recording::string::string (context *ctxt, const char *text)
  : memento (ctxt)
{
  m_len = strlen (text);
  m_buffer = new char[m_len + 1];
  strcpy (m_buffer, text);
}

recording::string::~string ()
{
  delete[] m_buffer;
}

recording::string *
recording::string::from_printf (context *ctxt, const char *fmt, ...)
{
  char buf[4096];
  va_list ap;
  va_start (ap, fmt);

  vsnprintf (buf, sizeof (buf), fmt, ap);

  va_end (ap);

  return ctxt->new_string (buf);
}

recording::string *
recording::string::make_debug_string ()
{
  /* Hack to avoid infinite recursion into strings when logging all
     mementos: don't re-escape strings:  */
  if (m_buffer[0] == '"')
    return this;

  /* Wrap in quotes and do escaping etc */

  size_t sz = (1 /* opening quote */
	       + (m_len * 2) /* each char might get escaped */
	       + 1 /* closing quote */
	       + 1); /* nil termintator */
  char *tmp = new char[sz];
  size_t len = 0;

#define APPEND(CH)  do { gcc_assert (len < sz); tmp[len++] = (CH); } while (0)
  APPEND('"'); /* opening quote */
  for (size_t i = 0; i < m_len ; i++)
    {
      char ch = m_buffer[i];
      if (ch == '\t' || ch == '\n' || ch == '\\' || ch == '"')
	APPEND('\\');
      APPEND(ch);
    }
  APPEND('"'); /* closing quote */
#undef APPEND
  tmp[len] = '\0'; /* nil termintator */

  string *result = m_ctxt->new_string (tmp);

  delete[] tmp;
  return result;
}

/* gcc::jit::recording::location:: */
void
recording::location::replay_into (replayer *r)
{
  m_playback_obj = r->new_location (this,
				    m_filename->c_str (),
				    m_line,
				    m_column);
}

recording::string *
recording::location::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s:%i:%i",
			      m_filename->c_str (), m_line, m_column);
}

/* gcc::jit::recording::type:: */

recording::type *
recording::type::get_pointer ()
{
  if (!m_pointer_to_this_type)
    {
      m_pointer_to_this_type = new memento_of_get_pointer (this);
      m_ctxt->record (m_pointer_to_this_type);
    }
  return m_pointer_to_this_type;
}

recording::type *
recording::type::get_const ()
{
  recording::type *result = new memento_of_get_const (this);
  m_ctxt->record (result);
  return result;
}

recording::type *
recording::type::get_volatile ()
{
  recording::type *result = new memento_of_get_volatile (this);
  m_ctxt->record (result);
  return result;
}

recording::type *
recording::memento_of_get_type::dereference ()
{
  switch (m_kind)
    {
    default: gcc_unreachable ();

    case GCC_JIT_TYPE_VOID:
      return NULL;

    case GCC_JIT_TYPE_VOID_PTR:
      return m_ctxt->get_type (GCC_JIT_TYPE_VOID);

    case GCC_JIT_TYPE_BOOL:
    case GCC_JIT_TYPE_CHAR:
    case GCC_JIT_TYPE_SIGNED_CHAR:
    case GCC_JIT_TYPE_UNSIGNED_CHAR:
    case GCC_JIT_TYPE_SHORT:
    case GCC_JIT_TYPE_UNSIGNED_SHORT:
    case GCC_JIT_TYPE_INT:
    case GCC_JIT_TYPE_UNSIGNED_INT:
    case GCC_JIT_TYPE_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG:
    case GCC_JIT_TYPE_LONG_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG_LONG:
    case GCC_JIT_TYPE_FLOAT:
    case GCC_JIT_TYPE_DOUBLE:
    case GCC_JIT_TYPE_LONG_DOUBLE:
      /* Not a pointer: */
      return NULL;

    case GCC_JIT_TYPE_CONST_CHAR_PTR:
      return m_ctxt->get_type (GCC_JIT_TYPE_CHAR)->get_const ();

    case GCC_JIT_TYPE_SIZE_T:
      /* Not a pointer: */
      return NULL;

    case GCC_JIT_TYPE_FILE_PTR:
      /* Give the client code back an opaque "struct FILE".  */
      return m_ctxt->get_opaque_FILE_type ();
    }
}

bool
recording::memento_of_get_type::is_int () const
{
  switch (m_kind)
    {
    default: gcc_unreachable ();

    case GCC_JIT_TYPE_VOID:
      return false;

    case GCC_JIT_TYPE_VOID_PTR:
      return false;

    case GCC_JIT_TYPE_BOOL:
      return false;

    case GCC_JIT_TYPE_CHAR:
    case GCC_JIT_TYPE_SIGNED_CHAR:
    case GCC_JIT_TYPE_UNSIGNED_CHAR:
    case GCC_JIT_TYPE_SHORT:
    case GCC_JIT_TYPE_UNSIGNED_SHORT:
    case GCC_JIT_TYPE_INT:
    case GCC_JIT_TYPE_UNSIGNED_INT:
    case GCC_JIT_TYPE_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG:
    case GCC_JIT_TYPE_LONG_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG_LONG:
      return true;

    case GCC_JIT_TYPE_FLOAT:
    case GCC_JIT_TYPE_DOUBLE:
    case GCC_JIT_TYPE_LONG_DOUBLE:
      return false;

    case GCC_JIT_TYPE_CONST_CHAR_PTR:
      return false;

    case GCC_JIT_TYPE_SIZE_T:
      return true;

    case GCC_JIT_TYPE_FILE_PTR:
      return false;
    }
}

bool
recording::memento_of_get_type::is_float () const
{
  switch (m_kind)
    {
    default: gcc_unreachable ();

    case GCC_JIT_TYPE_VOID:
      return false;

    case GCC_JIT_TYPE_VOID_PTR:
      return false;

    case GCC_JIT_TYPE_BOOL:
      return false;

    case GCC_JIT_TYPE_CHAR:
    case GCC_JIT_TYPE_SIGNED_CHAR:
    case GCC_JIT_TYPE_UNSIGNED_CHAR:
    case GCC_JIT_TYPE_SHORT:
    case GCC_JIT_TYPE_UNSIGNED_SHORT:
    case GCC_JIT_TYPE_INT:
    case GCC_JIT_TYPE_UNSIGNED_INT:
    case GCC_JIT_TYPE_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG:
    case GCC_JIT_TYPE_LONG_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG_LONG:
      return false;

    case GCC_JIT_TYPE_FLOAT:
    case GCC_JIT_TYPE_DOUBLE:
    case GCC_JIT_TYPE_LONG_DOUBLE:
      return true;

    case GCC_JIT_TYPE_CONST_CHAR_PTR:
      return false;

    case GCC_JIT_TYPE_SIZE_T:
      return false;

    case GCC_JIT_TYPE_FILE_PTR:
      return false;
    }
}

bool
recording::memento_of_get_type::is_bool () const
{
  switch (m_kind)
    {
    default: gcc_unreachable ();

    case GCC_JIT_TYPE_VOID:
      return false;

    case GCC_JIT_TYPE_VOID_PTR:
      return false;

    case GCC_JIT_TYPE_BOOL:
      return true;

    case GCC_JIT_TYPE_CHAR:
    case GCC_JIT_TYPE_SIGNED_CHAR:
    case GCC_JIT_TYPE_UNSIGNED_CHAR:
    case GCC_JIT_TYPE_SHORT:
    case GCC_JIT_TYPE_UNSIGNED_SHORT:
    case GCC_JIT_TYPE_INT:
    case GCC_JIT_TYPE_UNSIGNED_INT:
    case GCC_JIT_TYPE_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG:
    case GCC_JIT_TYPE_LONG_LONG:
    case GCC_JIT_TYPE_UNSIGNED_LONG_LONG:
      return false;

    case GCC_JIT_TYPE_FLOAT:
    case GCC_JIT_TYPE_DOUBLE:
    case GCC_JIT_TYPE_LONG_DOUBLE:
      return false;

    case GCC_JIT_TYPE_CONST_CHAR_PTR:
      return false;

    case GCC_JIT_TYPE_SIZE_T:
      return false;

    case GCC_JIT_TYPE_FILE_PTR:
      return false;
    }
}

void
recording::memento_of_get_type::replay_into (replayer *r)
{
  set_playback_obj (r->get_type (m_kind));
}

/* gcc::jit::recording::memento_of_get_type:: */
static const char * const get_type_strings[] = {
  "void",    /* GCC_JIT_TYPE_VOID */
  "void *",  /* GCC_JIT_TYPE_VOID_PTR */

  "bool",  /* GCC_JIT_TYPE_BOOL */

  "char",           /* GCC_JIT_TYPE_CHAR */
  "signed char",    /* GCC_JIT_TYPE_SIGNED_CHAR */
  "unsigned char",  /* GCC_JIT_TYPE_UNSIGNED_CHAR */

  "short",           /* GCC_JIT_TYPE_SHORT */
  "unsigned short",  /* GCC_JIT_TYPE_UNSIGNED_SHORT */

  "int",           /* GCC_JIT_TYPE_INT */
  "unsigned int",  /* GCC_JIT_TYPE_UNSIGNED_INT */

  "long",           /* GCC_JIT_TYPE_LONG  */
  "unsigned long",  /* GCC_JIT_TYPE_UNSIGNED_LONG, */

  "long long",           /* GCC_JIT_TYPE_LONG_LONG */
  "unsigned long long",  /* GCC_JIT_TYPE_UNSIGNED_LONG_LONG */

  "float",        /* GCC_JIT_TYPE_FLOAT */
  "double",       /* GCC_JIT_TYPE_DOUBLE */
  "long double",  /* GCC_JIT_TYPE_LONG_DOUBLE */

  "const char *",  /* GCC_JIT_TYPE_CONST_CHAR_PTR */

  "size_t",  /* GCC_JIT_TYPE_SIZE_T */

  "FILE *"  /* GCC_JIT_TYPE_FILE_PTR */

};

recording::string *
recording::memento_of_get_type::make_debug_string ()
{
  return m_ctxt->new_string (get_type_strings[m_kind]);
}

/* gcc::jit::recording::memento_of_get_pointer:: */
bool
recording::memento_of_get_pointer::accepts_writes_from (type *rtype)
{
  /* Must be a pointer type: */
  type *rtype_points_to = rtype->is_pointer ();
  if (!rtype_points_to)
    return false;

  /* It's OK to assign to a (const T *) from a (T *).  */
  return m_other_type->unqualified ()
    ->accepts_writes_from (rtype_points_to);
}

void
recording::memento_of_get_pointer::replay_into (replayer *)
{
  set_playback_obj (m_other_type->playback_type ()->get_pointer ());
}

recording::string *
recording::memento_of_get_pointer::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s *", m_other_type->get_debug_string ());
}

/* gcc::jit::recording::memento_of_get_const:: */

void
recording::memento_of_get_const::replay_into (replayer *)
{
  set_playback_obj (m_other_type->playback_type ()->get_const ());
}

recording::string *
recording::memento_of_get_const::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "const %s", m_other_type->get_debug_string ());
}

/* gcc::jit::recording::memento_of_get_volatile:: */

void
recording::memento_of_get_volatile::replay_into (replayer *)
{
  set_playback_obj (m_other_type->playback_type ()->get_volatile ());
}

recording::string *
recording::memento_of_get_volatile::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "volatile %s", m_other_type->get_debug_string ());
}

/* gcc::jit::recording::array_type */

recording::type *
recording::array_type::dereference ()
{
  return m_element_type;
}

void
recording::array_type::replay_into (replayer *r)
{
  set_playback_obj (r->new_array_type (playback_location (r, m_loc),
				       m_element_type->playback_type (),
				       m_num_elements));
}

recording::string *
recording::array_type::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s[%d]",
			      m_element_type->get_debug_string (),
			      m_num_elements);
}

/* gcc::jit::recording::function_type */
recording::function_type::function_type (context *ctxt,
					 type *return_type,
					 int num_params,
					 type **param_types,
					 int is_variadic)
: type (ctxt),
  m_return_type (return_type),
  m_param_types (),
  m_is_variadic (is_variadic)
{
  for (int i = 0; i< num_params; i++)
    m_param_types.safe_push (param_types[i]);
}

recording::type *
recording::function_type::dereference ()
{
  return NULL;
}

void
recording::function_type::replay_into (replayer *r)
{
  /* Convert m_param_types to a vec of playback type.  */
  vec <playback::type *> param_types;
  int i;
  recording::type *type;
  param_types.create (m_param_types.length ());
  FOR_EACH_VEC_ELT (m_param_types, i, type)
    param_types.safe_push (type->playback_type ());

  set_playback_obj (r->new_function_type (m_return_type->playback_type (),
					  &param_types,
					  m_is_variadic));
}

recording::string *
recording::function_type::make_debug_string ()
{
  /* First, build a buffer for the arguments.  */
  /* Calculate length of said buffer.  */
  size_t sz = 1; /* nil terminator */
  for (unsigned i = 0; i< m_param_types.length (); i++)
    {
      sz += strlen (m_param_types[i]->get_debug_string ());
      sz += 2; /* ", " separator */
    }
  if (m_is_variadic)
    sz += 5; /* ", ..." separator and ellipsis */

  /* Now allocate and populate the buffer.  */
  char *argbuf = new char[sz];
  size_t len = 0;

  for (unsigned i = 0; i< m_param_types.length (); i++)
    {
      strcpy (argbuf + len, m_param_types[i]->get_debug_string ());
      len += strlen (m_param_types[i]->get_debug_string ());
      if (i + 1 < m_param_types.length ())
	{
	  strcpy (argbuf + len, ", ");
	  len += 2;
	}
    }
  if (m_is_variadic)
    {
      if (m_param_types.length ())
	{
	  strcpy (argbuf + len, ", ");
	  len += 2;
	}
      strcpy (argbuf + len, "...");
      len += 3;
    }
  argbuf[len] = '\0';

  /* ...and use it to get the string for the call as a whole.  */
  string *result = string::from_printf (m_ctxt,
					"%s (%s)",
					m_return_type->get_debug_string (),
					argbuf);

  delete[] argbuf;

  return result;
}

/* gcc::jit::recording::field:: */
void
recording::field::replay_into (replayer *r)
{
  set_playback_obj (r->new_field (playback_location (r, m_loc),
				  m_type->playback_type (),
				  playback_string (m_name)));
}

void
recording::field::write_to_dump (dump &d)
{
  d.write ("  %s %s;\n",
	   m_type->get_debug_string (),
	   m_name->c_str ());
}

recording::string *
recording::field::make_debug_string ()
{
  return m_name;
}

/* gcc::jit::recording::struct_:: */
recording::struct_::struct_ (context *ctxt,
			     location *loc,
			     string *name)
: type (ctxt),
  m_loc (loc),
  m_name (name),
  m_fields (NULL)
{
}

void
recording::struct_::set_fields (location *loc,
				int num_fields,
				field **field_array)
{
  m_loc = loc;
  gcc_assert (NULL == m_fields);

  m_fields = new fields (this, num_fields, field_array);
  m_ctxt->record (m_fields);
}

recording::type *
recording::struct_::dereference ()
{
  return NULL; /* not a pointer */
}

void
recording::struct_::replay_into (replayer *r)
{
  set_playback_obj (
    r->new_struct_type (playback_location (r, m_loc),
			m_name->c_str ()));
}

recording::string *
recording::struct_::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "struct %s", m_name->c_str ());
}

/* gcc::jit::recording::fields:: */
recording::fields::fields (struct_ *struct_,
			   int num_fields,
			   field **fields)
: memento (struct_->m_ctxt),
  m_struct (struct_),
  m_fields ()
{
  for (int i = 0; i < num_fields; i++)
    {
      gcc_assert (fields[i]->get_container () == NULL);
      fields[i]->set_container (m_struct);
      m_fields.safe_push (fields[i]);
    }
}

void
recording::fields::replay_into (replayer *)
{
  vec<playback::field *> playback_fields;
  playback_fields.create (m_fields.length ());
  for (unsigned i = 0; i < m_fields.length (); i++)
    playback_fields.safe_push (m_fields[i]->playback_field ());
  m_struct->playback_struct ()->set_fields (playback_fields);
}

void
recording::fields::write_to_dump (dump &d)
{
  int i;
  field *f;

  d.write ("%s\n{\n", m_struct->get_debug_string ());
  FOR_EACH_VEC_ELT (m_fields, i, f)
    f->write_to_dump (d);
  d.write ("};\n");
}

recording::string *
recording::fields::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "fields");
}

/* gcc::jit::recording::rvalue:: */

recording::rvalue *
recording::rvalue::access_field (recording::location *loc,
				 field *field)
{
  recording::rvalue *result =
    new access_field_rvalue (m_ctxt, loc, this, field);
  m_ctxt->record (result);
  return result;
}

recording::lvalue *
recording::rvalue::dereference_field (recording::location *loc,
				      field *field)
{
  recording::lvalue *result =
    new dereference_field_rvalue (m_ctxt, loc, this, field);
  m_ctxt->record (result);
  return result;
}

recording::lvalue *
recording::rvalue::dereference (recording::location *loc)
{
  recording::lvalue *result =
    new dereference_rvalue (m_ctxt, loc, this);
  m_ctxt->record (result);
  return result;
}

/* gcc::jit::recording::lvalue:: */

recording::lvalue *
recording::lvalue::access_field (recording::location *loc,
				 field *field)
{
  recording::lvalue *result =
    new access_field_of_lvalue (m_ctxt, loc, this, field);
  m_ctxt->record (result);
  return result;
}

recording::rvalue *
recording::lvalue::get_address (recording::location *loc)
{
  recording::rvalue *result =
    new get_address_of_lvalue (m_ctxt, loc, this);
  m_ctxt->record (result);
  return result;
}

/* gcc::jit::recording::param:: */
void
recording::param::replay_into (replayer *r)
{
  set_playback_obj (r->new_param (playback_location (r, m_loc),
				  m_type->playback_type (),
				  m_name->c_str ()));
}


/* gcc::jit::recording::function:: */

recording::function::function (context *ctxt,
			       recording::location *loc,
			       enum gcc_jit_function_kind kind,
			       type *return_type,
			       recording::string *name,
			       int num_params,
			       recording::param **params,
			       int is_variadic,
			       enum built_in_function builtin_id)
: memento (ctxt),
  m_loc (loc),
  m_kind (kind),
  m_return_type (return_type),
  m_name (name),
  m_params (),
  m_is_variadic (is_variadic),
  m_builtin_id (builtin_id),
  m_locals (),
  m_blocks ()
{
  for (int i = 0; i< num_params; i++)
    m_params.safe_push (params[i]);
}

void
recording::function::replay_into (replayer *r)
{
  /* Convert m_params to a vec of playback param.  */
  vec <playback::param *> params;
  int i;
  recording::param *param;
  params.create (m_params.length ());
  FOR_EACH_VEC_ELT (m_params, i, param)
    params.safe_push (param->playback_param ());

  set_playback_obj (r->new_function (playback_location (r, m_loc),
				     m_kind,
				     m_return_type->playback_type (),
				     m_name->c_str (),
				     &params,
				     m_is_variadic,
				     m_builtin_id));
}

recording::lvalue *
recording::function::new_local (recording::location *loc,
		     type *type,
		     const char *name)
{
  local *result = new local (this, loc, type, new_string (name));
  m_ctxt->record (result);
  m_locals.safe_push (result);
  return result;
}

recording::block*
recording::function::new_block (const char *name)
{
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  recording::block *result =
    new recording::block (this, m_blocks.length (), new_string (name));
  m_ctxt->record (result);
  m_blocks.safe_push (result);
  return result;
}

void
recording::function::write_to_dump (dump &d)
{
  switch (m_kind)
    {
    default: gcc_unreachable ();
    case GCC_JIT_FUNCTION_EXPORTED:
    case GCC_JIT_FUNCTION_IMPORTED:
      d.write ("extern ");
      break;
    case GCC_JIT_FUNCTION_INTERNAL:
      d.write ("static ");
      break;
    case GCC_JIT_FUNCTION_ALWAYS_INLINE:
      d.write ("static inline ");
      break;
     }
  d.write ("%s\n", m_return_type->get_debug_string ());

  if (d.update_locations ())
    m_loc = d.make_location ();

  d.write ("%s (", get_debug_string ());

  int i;
  recording::param *param;
  FOR_EACH_VEC_ELT (m_params, i, param)
    {
      if (i > 0)
	d.write (", ");
      d.write ("%s %s",
	       param->get_type ()->get_debug_string (),
	       param->get_debug_string ());
    }
  d.write (")");
  if (m_kind == GCC_JIT_FUNCTION_IMPORTED)
    {
      d.write ("; /* (imported) */\n\n");
    }
  else
    {
      int i;
      local *var = NULL;
      block *b;
      d.write ("\n{\n");

      /* Write locals: */
      FOR_EACH_VEC_ELT (m_locals, i, var)
	var->write_to_dump (d);
      if (m_locals.length ())
	d.write ("\n");

      /* Write each block: */
      FOR_EACH_VEC_ELT (m_blocks, i, b)
	{
	  if (i > 0)
	    d.write ("\n");
	  b->write_to_dump (d);
	}

      d.write ("}\n\n");
    }
}

void
recording::function::validate ()
{
  /* Complain about empty functions with non-void return type.  */
  if (m_kind != GCC_JIT_FUNCTION_IMPORTED
      && m_return_type != m_ctxt->get_type (GCC_JIT_TYPE_VOID))
    if (0 == m_blocks.length ())
      m_ctxt->add_error (m_loc,
			 "function %s returns non-void (type: %s)"
			 " but has no blocks",
			 get_debug_string (),
			 m_return_type->get_debug_string ());

  /* Check that all blocks are terminated.  */
  int num_invalid_blocks = 0;
  {
    int i;
    block *b;

    FOR_EACH_VEC_ELT (m_blocks, i, b)
      if (!b->validate ())
	num_invalid_blocks++;
  }

  /* Check that all blocks are reachable.  */
  if (m_blocks.length () > 0 && 0 == num_invalid_blocks)
    {
      /* Iteratively walk the graph of blocks, marking their "m_is_reachable"
	 flag, starting at the initial block.  */
      vec<block *> worklist;
      worklist.create (m_blocks.length ());
      worklist.safe_push (m_blocks[0]);
      while (worklist.length () > 0)
	{
	  block *b = worklist.pop ();
	  b->m_is_reachable = true;

	  /* Add successor blocks that aren't yet marked to the worklist.  */
	  /* We checked that each block has a terminating statement above .  */
	  block *next1, *next2;
	  int n = b->get_successor_blocks (&next1, &next2);
	  switch (n)
	    {
	    default:
	      gcc_unreachable ();
	    case 2:
	      if (!next2->m_is_reachable)
		worklist.safe_push (next2);
	      /* fallthrough */
	    case 1:
	      if (!next1->m_is_reachable)
		worklist.safe_push (next1);
	      break;
	    case 0:
	      break;
	    }
	}

      /* Now complain about any blocks that haven't been marked.  */
      {
	int i;
	block *b;
	FOR_EACH_VEC_ELT (m_blocks, i, b)
	  if (!b->m_is_reachable)
	    m_ctxt->add_error (b->get_loc (),
			       "unreachable block: %s",
			       b->get_debug_string ());
      }
    }
}

void
recording::function::dump_to_dot (const char *path)
{
  FILE *fp  = fopen (path, "w");
  if (!fp)
    return;

  pretty_printer the_pp;
  the_pp.buffer->stream = fp;

  pretty_printer *pp = &the_pp;

  pp_printf (pp,
	     "digraph %s {\n", get_debug_string ());

  /* Blocks: */
  {
    int i;
    block *b;
    FOR_EACH_VEC_ELT (m_blocks, i, b)
      b->dump_to_dot (pp);
  }

  /* Edges: */
  {
    int i;
    block *b;
    FOR_EACH_VEC_ELT (m_blocks, i, b)
      b->dump_edges_to_dot (pp);
  }

  pp_printf (pp, "}\n");
  pp_flush (pp);
  fclose (fp);
}

recording::string *
recording::function::make_debug_string ()
{
  return m_name;
}

/* gcc::jit::recording::block:: */

void
recording::block::add_eval (recording::location *loc,
			    recording::rvalue *rvalue)
{
  statement *result = new eval (this, loc, rvalue);
  m_ctxt->record (result);
  m_statements.safe_push (result);
}

void
recording::block::add_assignment (recording::location *loc,
				  recording::lvalue *lvalue,
				  recording::rvalue *rvalue)
{
  statement *result = new assignment (this, loc, lvalue, rvalue);
  m_ctxt->record (result);
  m_statements.safe_push (result);
}

void
recording::block::add_assignment_op (recording::location *loc,
				     recording::lvalue *lvalue,
				     enum gcc_jit_binary_op op,
				     recording::rvalue *rvalue)
{
  statement *result = new assignment_op (this, loc, lvalue, op, rvalue);
  m_ctxt->record (result);
  m_statements.safe_push (result);
}

void
recording::block::add_comment (recording::location *loc,
			       const char *text)
{
  statement *result = new comment (this, loc, new_string (text));
  m_ctxt->record (result);
  m_statements.safe_push (result);
}

void
recording::block::end_with_conditional (recording::location *loc,
					recording::rvalue *boolval,
					recording::block *on_true,
					recording::block *on_false)
{
  statement *result = new conditional (this, loc, boolval, on_true, on_false);
  m_ctxt->record (result);
  m_statements.safe_push (result);
  m_has_been_terminated = true;
}

void
recording::block::end_with_jump (recording::location *loc,
				 recording::block *target)
{
  statement *result = new jump (this, loc, target);
  m_ctxt->record (result);
  m_statements.safe_push (result);
  m_has_been_terminated = true;
}

void
recording::block::end_with_return (recording::location *loc,
				   recording::rvalue *rvalue)
{
  /* This is used by both gcc_jit_function_add_return and
     gcc_jit_function_add_void_return; rvalue will be non-NULL for
     the former and NULL for the latter.  */
  statement *result = new return_ (this, loc, rvalue);
  m_ctxt->record (result);
  m_statements.safe_push (result);
  m_has_been_terminated = true;
}

void
recording::block::write_to_dump (dump &d)
{
  d.write ("%s:\n", get_debug_string ());

  int i;
  statement *s;
  FOR_EACH_VEC_ELT (m_statements, i, s)
    s->write_to_dump (d);
}

bool
recording::block::validate ()
{
  if (!has_been_terminated ())
    {
      statement *stmt = get_last_statement ();
      location *loc = stmt ? stmt->get_loc () : NULL;
      m_func->get_context ()->add_error (loc,
					 "unterminated block in %s: %s",
					 m_func->get_debug_string (),
					 get_debug_string ());
      return false;
    }

  return true;
}

recording::location *
recording::block::get_loc () const
{
  recording::statement *stmt = get_first_statement ();
  if (stmt)
    return stmt->get_loc ();
  else
    return NULL;
}

recording::statement *
recording::block::get_first_statement () const
{
  if (m_statements.length ())
    return m_statements[0];
  else
    return NULL;
}

recording::statement *
recording::block::get_last_statement () const
{
  if (m_statements.length ())
    return m_statements[m_statements.length () - 1];
  else
    return NULL;
}

int
recording::block::get_successor_blocks (block **next1, block **next2) const
{
  gcc_assert (m_has_been_terminated);
  statement *last_statement = get_last_statement ();
  gcc_assert (last_statement);
  return last_statement->get_successor_blocks (next1, next2);
}

void
recording::block::replay_into (replayer *)
{
  set_playback_obj (m_func->playback_function ()
		      ->new_block (playback_string (m_name)));
}

recording::string *
recording::block::make_debug_string ()
{
  if (m_name)
    return m_name;
  else
    return string::from_printf (m_ctxt,
				"<UNNAMED BLOCK %p>",
				(void *)this);
}

void
recording::block::dump_to_dot (pretty_printer *pp)
{
  pp_printf (pp,
	     ("\tblock_%d "
	      "[shape=record,style=filled,fillcolor=white,label=\"{"),
	     m_index);
  pp_write_text_to_stream (pp);
  if (m_name)
    {
      pp_string (pp, m_name->c_str ());
      pp_string (pp, ":");
      pp_newline (pp);
      pp_write_text_as_dot_label_to_stream (pp, true /*for_record*/);
    }

  int i;
  statement *s;
  FOR_EACH_VEC_ELT (m_statements, i, s)
    {
      pp_string (pp, s->get_debug_string ());
      pp_newline (pp);
      pp_write_text_as_dot_label_to_stream (pp, true /*for_record*/);
    }

  pp_printf (pp,
	     "}\"];\n\n");
  pp_flush (pp);
}

void
recording::block::dump_edges_to_dot (pretty_printer *pp)
{
  block *next[2];
  int num_succs = get_successor_blocks (&next[0], &next[1]);
  for (int i = 0; i < num_succs; i++)
    pp_printf (pp,
	       "\tblock_%d:s -> block_%d:n;\n",
	       m_index, next[i]->m_index);
}

/* gcc::jit::recording::global:: */
void
recording::global::replay_into (replayer *r)
{
  set_playback_obj (r->new_global (playback_location (r, m_loc),
				   m_type->playback_type (),
				   playback_string (m_name)));
}

/* gcc::jit::recording::memento_of_new_rvalue_from_int:: */
void
recording::memento_of_new_rvalue_from_int::replay_into (replayer *r)
{
  set_playback_obj (r->new_rvalue_from_int (m_type->playback_type (),
					    m_value));
}

recording::string *
recording::memento_of_new_rvalue_from_int::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "(%s)%i",
			      m_type->get_debug_string (),
			      m_value);
}

/* gcc::jit::recording::memento_of_new_rvalue_from_double:: */
void
recording::memento_of_new_rvalue_from_double::replay_into (replayer *r)
{
  set_playback_obj (r->new_rvalue_from_double (m_type->playback_type (),
					       m_value));
}

recording::string *
recording::memento_of_new_rvalue_from_double::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "(%s)%f",
			      m_type->get_debug_string (),
			      m_value);
}

/* gcc::jit::recording::memento_of_new_rvalue_from_ptr:: */
void
recording::memento_of_new_rvalue_from_ptr::replay_into (replayer *r)
{
  set_playback_obj (r->new_rvalue_from_ptr (m_type->playback_type (),
					    m_value));
}

recording::string *
recording::memento_of_new_rvalue_from_ptr::make_debug_string ()
{
  if (m_value != NULL)
    return string::from_printf (m_ctxt,
				"(%s)%p",
				m_type->get_debug_string (), m_value);
  else
    return string::from_printf (m_ctxt,
				"(%s)NULL",
				m_type->get_debug_string ());
}

/* gcc::jit::recording::memento_of_new_string_literal:: */
void
recording::memento_of_new_string_literal::replay_into (replayer *r)
{
  set_playback_obj (r->new_string_literal (m_value->c_str ()));
}

recording::string *
recording::memento_of_new_string_literal::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      m_value->get_debug_string ());
}

/* gcc::jit::recording::unary_op:: */
void
recording::unary_op::replay_into (replayer *r)
{
  set_playback_obj (r->new_unary_op (playback_location (r, m_loc),
				     m_op,
				     get_type ()->playback_type (),
				     m_a->playback_rvalue ()));
}

static const char * const unary_op_strings[] = {
  "-", /* GCC_JIT_UNARY_OP_MINUS */
  "~", /* GCC_JIT_UNARY_OP_BITWISE_NEGATE */
  "!", /* GCC_JIT_UNARY_OP_LOGICAL_NEGATE */
};

recording::string *
recording::unary_op::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s(%s)",
			      unary_op_strings[m_op],
			      m_a->get_debug_string ());
}

/* gcc::jit::recording::binary_op:: */
void
recording::binary_op::replay_into (replayer *r)
{
  set_playback_obj (r->new_binary_op (playback_location (r, m_loc),
				      m_op,
				      get_type ()->playback_type (),
				      m_a->playback_rvalue (),
				      m_b->playback_rvalue ()));
}

static const char * const binary_op_strings[] = {
  "+", /* GCC_JIT_BINARY_OP_PLUS */
  "-", /* GCC_JIT_BINARY_OP_MINUS */
  "*", /* GCC_JIT_BINARY_OP_MULT */
  "/", /* GCC_JIT_BINARY_OP_DIVIDE */
  "%", /* GCC_JIT_BINARY_OP_MODULO */
  "&", /* GCC_JIT_BINARY_OP_BITWISE_AND */
  "^", /* GCC_JIT_BINARY_OP_BITWISE_XOR */
  "|", /* GCC_JIT_BINARY_OP_BITWISE_OR */
  "&&", /* GCC_JIT_BINARY_OP_LOGICAL_AND */
  "||" /* GCC_JIT_BINARY_OP_LOGICAL_OR */
};

recording::string *
recording::binary_op::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s %s %s",
			      m_a->get_debug_string (),
			      binary_op_strings[m_op],
			      m_b->get_debug_string ());
}

/* gcc::jit::recording::comparison:: */
static const char * const comparison_strings[] =
{
  "==", /* GCC_JIT_COMPARISON_EQ */
  "!=", /* GCC_JIT_COMPARISON_NE */
  "<",  /* GCC_JIT_COMPARISON_LT */
  "<=", /* GCC_JIT_COMPARISON_LE */
  ">",  /* GCC_JIT_COMPARISON_GT */
  ">=", /* GCC_JIT_COMPARISON_GE */
};

recording::string *
recording::comparison::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s %s %s",
			      m_a->get_debug_string (),
			      comparison_strings[m_op],
			      m_b->get_debug_string ());
}

void
recording::comparison::replay_into (replayer *r)
{
  set_playback_obj (r->new_comparison (playback_location (r, m_loc),
				       m_op,
				       m_a->playback_rvalue (),
				       m_b->playback_rvalue ()));
}

void
recording::cast::replay_into (replayer *r)
{
  set_playback_obj (r->new_cast (playback_location (r, m_loc),
				 m_rvalue->playback_rvalue (),
				 get_type ()->playback_type ()));
}

recording::string *
recording::cast::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "(%s)%s",
			      get_type ()->get_debug_string (),
			      m_rvalue->get_debug_string ());
}

recording::call::call (recording::context *ctxt,
		       recording::location *loc,
		       recording::function *func,
		       int numargs,
		       rvalue **args)
: rvalue (ctxt, loc, func->get_return_type ()),
  m_func (func),
  m_args ()
{
  for (int i = 0; i< numargs; i++)
    m_args.safe_push (args[i]);
}

void
recording::call::replay_into (replayer *r)
{
  vec<playback::rvalue *> playback_args;
  playback_args.create (m_args.length ());
  for (unsigned i = 0; i< m_args.length (); i++)
    playback_args.safe_push (m_args[i]->playback_rvalue ());

  set_playback_obj (r->new_call (playback_location (r, m_loc),
				 m_func->playback_function (),
				 playback_args));
}

recording::string *
recording::call::make_debug_string ()
{
  /* First, build a buffer for the arguments.  */
  /* Calculate length of said buffer.  */
  size_t sz = 1; /* nil terminator */
  for (unsigned i = 0; i< m_args.length (); i++)
    {
      sz += strlen (m_args[i]->get_debug_string ());
      sz += 2; /* ", " separator */
    }

  /* Now allocate and populate the buffer.  */
  char *argbuf = new char[sz];
  size_t len = 0;

  for (unsigned i = 0; i< m_args.length (); i++)
    {
      strcpy (argbuf + len, m_args[i]->get_debug_string ());
      len += strlen (m_args[i]->get_debug_string ());
      if (i + 1 < m_args.length ())
	{
	  strcpy (argbuf + len, ", ");
	  len += 2;
	}
    }
  argbuf[len] = '\0';

  /* ...and use it to get the string for the call as a whole.  */
  string *result = string::from_printf (m_ctxt,
					"%s (%s)",
					m_func->get_debug_string (),
					argbuf);

  delete[] argbuf;

  return result;
}

void
recording::array_access::replay_into (replayer *r)
{
  set_playback_obj (
    r->new_array_access (playback_location (r, m_loc),
			 m_ptr->playback_rvalue (),
			 m_index->playback_rvalue ()));
}

recording::string *
recording::array_access::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s[%s]",
			      m_ptr->get_debug_string (),
			      m_index->get_debug_string ());
}

void
recording::access_field_of_lvalue::replay_into (replayer *r)
{
  set_playback_obj (
    m_lvalue->playback_lvalue ()
      ->access_field (playback_location (r, m_loc),
		      m_field->playback_field ()));

}

recording::string *
recording::access_field_of_lvalue::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s.%s",
			      m_lvalue->get_debug_string (),
			      m_field->get_debug_string ());
}

void
recording::access_field_rvalue::replay_into (replayer *r)
{
  set_playback_obj (
    m_rvalue->playback_rvalue ()
      ->access_field (playback_location (r, m_loc),
		      m_field->playback_field ()));
}

recording::string *
recording::access_field_rvalue::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s.%s",
			      m_rvalue->get_debug_string (),
			      m_field->get_debug_string ());
}

void
recording::dereference_field_rvalue::replay_into (replayer *r)
{
  set_playback_obj (
    m_rvalue->playback_rvalue ()->
      dereference_field (playback_location (r, m_loc),
			 m_field->playback_field ()));
}

recording::string *
recording::dereference_field_rvalue::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s->%s",
			      m_rvalue->get_debug_string (),
			      m_field->get_debug_string ());
}

void
recording::dereference_rvalue::replay_into (replayer *r)
{
  set_playback_obj (
    m_rvalue->playback_rvalue ()->
      dereference (playback_location (r, m_loc)));
}

recording::string *
recording::dereference_rvalue::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "*%s",
			      m_rvalue->get_debug_string ());
}

void
recording::get_address_of_lvalue::replay_into (replayer *r)
{
  set_playback_obj (
    m_lvalue->playback_lvalue ()->
      get_address (playback_location (r, m_loc)));
}

recording::string *
recording::get_address_of_lvalue::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "&%s",
			      m_lvalue->get_debug_string ());
}

void
recording::local::replay_into (replayer *r)
{
  set_playback_obj (
    m_func->playback_function ()
      ->new_local (playback_location (r, m_loc),
		   m_type->playback_type (),
		   playback_string (m_name)));
}

void
recording::local::write_to_dump (dump &d)
{
  if (d.update_locations ())
    m_loc = d.make_location ();
  d.write("  %s %s;\n",
	  m_type->get_debug_string (),
	  get_debug_string ());
}

// gcc::jit::recording::statement

int
recording::statement::get_successor_blocks (block **/*out_next1*/,
					    block **/*out_next2*/) const
{
  /* The base class implementation is for non-terminating statements,
     and thus should never be called.  */
  gcc_unreachable ();
  return 0;
}

void
recording::statement::write_to_dump (dump &d)
{
  memento::write_to_dump (d);
  if (d.update_locations ())
    m_loc = d.make_location ();
}

void
recording::eval::replay_into (replayer *r)
{
  playback_block (get_block ())
    ->add_eval (playback_location (r),
		m_rvalue->playback_rvalue ());
}

recording::string *
recording::eval::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "(void)%s;",
			      m_rvalue->get_debug_string ());
}

void
recording::assignment::replay_into (replayer *r)
{
  playback_block (get_block ())
    ->add_assignment (playback_location (r),
		      m_lvalue->playback_lvalue (),
		      m_rvalue->playback_rvalue ());
}

recording::string *
recording::assignment::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s = %s;",
			      m_lvalue->get_debug_string (),
			      m_rvalue->get_debug_string ());
}

void
recording::assignment_op::replay_into (replayer *r)
{
  playback::type *result_type =
    m_lvalue->playback_lvalue ()->get_type ();

  playback::rvalue *binary_op =
    r->new_binary_op (playback_location (r),
		      m_op,
		      result_type,
		      m_lvalue->playback_rvalue (),
		      m_rvalue->playback_rvalue ());

  playback_block (get_block ())
    ->add_assignment (playback_location (r),
		      m_lvalue->playback_lvalue (),
		      binary_op);
}

recording::string *
recording::assignment_op::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "%s %s= %s;",
			      m_lvalue->get_debug_string (),
			      binary_op_strings[m_op],
			      m_rvalue->get_debug_string ());
}

void
recording::comment::replay_into (replayer *r)
{
  playback_block (get_block ())
    ->add_comment (playback_location (r),
		   m_text->c_str ());
}

recording::string *
recording::comment::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "/* %s */",
			      m_text->c_str ());
}

void
recording::conditional::replay_into (replayer *r)
{
  playback_block (get_block ())
    ->add_conditional (playback_location (r),
		       m_boolval->playback_rvalue (),
		       playback_block (m_on_true),
		       playback_block (m_on_false));
}

int
recording::conditional::get_successor_blocks (block **out_next1,
					      block **out_next2) const
{
  *out_next1 = m_on_true;
  *out_next2 = m_on_false;
  return 2;
}

recording::string *
recording::conditional::make_debug_string ()
{
  if (m_on_false)
    return string::from_printf (m_ctxt,
				"if (%s) goto %s; else goto %s;",
				m_boolval->get_debug_string (),
				m_on_true->get_debug_string (),
				m_on_false->get_debug_string ());
  else
    return string::from_printf (m_ctxt,
				"if (%s) goto %s;",
				m_boolval->get_debug_string (),
				m_on_true->get_debug_string ());
}

void
recording::jump::replay_into (replayer *r)
{
  playback_block (get_block ())
    ->add_jump (playback_location (r),
		m_target->playback_block ());
}

int
recording::jump::get_successor_blocks (block **out_next1,
				       block **/*out_next2*/) const
{
  *out_next1 = m_target;
  return 1;
}

recording::string *
recording::jump::make_debug_string ()
{
  return string::from_printf (m_ctxt,
			      "goto %s;",
			      m_target->get_debug_string ());
}

void
recording::return_::replay_into (replayer *r)
{
  playback_block (get_block ())
    ->add_return (playback_location (r),
		  m_rvalue ? m_rvalue->playback_rvalue () : NULL);
}

int
recording::return_::get_successor_blocks (block **/*out_next1*/,
					  block **/*out_next2*/) const
{
  return 0;
}

recording::string *
recording::return_::make_debug_string ()
{
  if (m_rvalue)
    return string::from_printf (m_ctxt,
				"return %s;",
				m_rvalue->get_debug_string ());
  else
    return string::from_printf (m_ctxt,
				"return;");
}

/**********************************************************************
 Playback.
 **********************************************************************/

playback::context::context (recording::context *ctxt)
  : m_recording_ctxt (ctxt),
    m_char_array_type_node (NULL),
    m_const_char_ptr (NULL)
{
  m_functions.create (0);
  m_source_files.create (0);
  m_cached_locations.create (0);
}

playback::context::~context ()
{
  if (get_bool_option (GCC_JIT_BOOL_OPTION_KEEP_INTERMEDIATES))
    fprintf (stderr, "intermediate files written to %s\n", m_path_tempdir);
  else
    {
      /* Clean up .s/.so and tempdir. */
      if (m_path_s_file)
        unlink (m_path_s_file);
      if (m_path_so_file)
        unlink (m_path_so_file);
      if (m_path_tempdir)
        rmdir (m_path_tempdir);
    }

  free (m_path_template);
  /* m_path_tempdir aliases m_path_template, or is NULL, so don't
     attempt to free it .  */
  free (m_path_c_file);
  free (m_path_s_file);
  free (m_path_so_file);
  m_functions.release ();
}

void
playback::context::
gt_ggc_mx ()
{
  int i;
  function *func;
  FOR_EACH_VEC_ELT (m_functions, i, func)
    {
      if (ggc_test_and_set_mark (func))
	func->gt_ggc_mx ();
    }
}

static tree
get_tree_node_for_type (enum gcc_jit_types type_)
{
  switch (type_)
    {
    case GCC_JIT_TYPE_VOID:
      return void_type_node;

    case GCC_JIT_TYPE_VOID_PTR:
      return ptr_type_node;

    case GCC_JIT_TYPE_BOOL:
      return boolean_type_node;

    case GCC_JIT_TYPE_CHAR:
      return char_type_node;
    case GCC_JIT_TYPE_SIGNED_CHAR:
      return signed_char_type_node;
    case GCC_JIT_TYPE_UNSIGNED_CHAR:
      return unsigned_char_type_node;

    case GCC_JIT_TYPE_SHORT:
      return short_integer_type_node;
    case GCC_JIT_TYPE_UNSIGNED_SHORT:
      return short_unsigned_type_node;

    case GCC_JIT_TYPE_CONST_CHAR_PTR:
      {
	tree const_char = build_qualified_type (char_type_node,
						TYPE_QUAL_CONST);
	return build_pointer_type (const_char);
      }

    case GCC_JIT_TYPE_INT:
      return integer_type_node;
    case GCC_JIT_TYPE_UNSIGNED_INT:
      return unsigned_type_node;

    case GCC_JIT_TYPE_LONG:
      return long_integer_type_node;
    case GCC_JIT_TYPE_UNSIGNED_LONG:
      return long_unsigned_type_node;

    case GCC_JIT_TYPE_LONG_LONG:
      return long_long_integer_type_node;
    case GCC_JIT_TYPE_UNSIGNED_LONG_LONG:
      return long_long_unsigned_type_node;

    case GCC_JIT_TYPE_FLOAT:
      return float_type_node;
    case GCC_JIT_TYPE_DOUBLE:
      return double_type_node;
    case GCC_JIT_TYPE_LONG_DOUBLE:
      return long_double_type_node;

    case GCC_JIT_TYPE_SIZE_T:
      return size_type_node;

    case GCC_JIT_TYPE_FILE_PTR:
      return fileptr_type_node;
    }

  return NULL;
}

playback::type *
playback::context::
get_type (enum gcc_jit_types type_)
{
  tree type_node = get_tree_node_for_type (type_);
  if (NULL == type_node)
    {
      add_error (NULL,
		 "unrecognized (enum gcc_jit_types) value: %i", type_);
      return NULL;
    }

  return new type (type_node);
}

playback::type *
playback::context::
new_array_type (playback::location *loc,
		playback::type *element_type,
		int num_elements)
{
  gcc_assert (element_type);

  tree t = build_array_type_nelts (element_type->as_tree (),
				   num_elements);
  layout_type (t);

  if (loc)
    set_tree_location (t, loc);

  return new type (t);
}

playback::field *
playback::context::
new_field (location *loc,
	   type *type,
	   const char *name)
{
  gcc_assert (type);
  gcc_assert (name);

  /* compare with c/c-decl.c:grokfield and grokdeclarator.  */
  tree decl = build_decl (UNKNOWN_LOCATION, FIELD_DECL,
			  get_identifier (name), type->as_tree ());

  if (loc)
    set_tree_location (decl, loc);

  return new field (decl);
}

playback::struct_ *
playback::context::
new_struct_type (location *loc,
		 const char *name)
{
  gcc_assert (name);

  /* Compare with c/c-decl.c: start_struct. */

  tree t = make_node (RECORD_TYPE);
  TYPE_NAME (t) = get_identifier (name);
  TYPE_SIZE (t) = 0;

  if (loc)
    set_tree_location (t, loc);

  return new struct_ (t);
}

void
playback::struct_::set_fields (const vec<playback::field *> &fields)
{
  /* Compare with c/c-decl.c: finish_struct. */
  tree t = as_tree ();

  tree fieldlist = NULL;
  for (unsigned i = 0; i < fields.length (); i++)
    {
      field *f = fields[i];
      DECL_CONTEXT (f->as_tree ()) = t;
      fieldlist = chainon (f->as_tree (), fieldlist);
    }
  fieldlist = nreverse (fieldlist);
  TYPE_FIELDS (t) = fieldlist;

  layout_type (t);
}

playback::type *
playback::context::
new_function_type (type *return_type,
		   vec<type *> *param_types,
		   int is_variadic)
{
  int i;
  type *param_type;

  tree *arg_types = (tree *)xcalloc(param_types->length (), sizeof(tree*));

  FOR_EACH_VEC_ELT (*param_types, i, param_type)
    arg_types[i] = param_type->as_tree ();

  tree fn_type;
  if (is_variadic)
    fn_type =
      build_varargs_function_type_array (return_type->as_tree (),
					 param_types->length (),
					 arg_types);
  else
    fn_type = build_function_type_array (return_type->as_tree (),
					 param_types->length (),
					 arg_types);
  free (arg_types);

  return new type (fn_type);
}

playback::param *
playback::context::
new_param (location *loc,
	   type *type,
	   const char *name)
{
  gcc_assert (type);
  gcc_assert (name);
  tree inner = build_decl (UNKNOWN_LOCATION, PARM_DECL,
			   get_identifier (name), type->as_tree ());
  if (loc)
    set_tree_location (inner, loc);

  return new param (this, inner);
}

playback::function *
playback::context::
new_function (location *loc,
	      enum gcc_jit_function_kind kind,
	      type *return_type,
	      const char *name,
	      vec<param *> *params,
	      int is_variadic,
	      enum built_in_function builtin_id)
{
  int i;
  param *param;

  //can return_type be NULL?
  gcc_assert (name);

  tree *arg_types = (tree *)xcalloc(params->length (), sizeof(tree*));
  FOR_EACH_VEC_ELT (*params, i, param)
    arg_types[i] = TREE_TYPE (param->as_tree ());

  tree fn_type;
  if (is_variadic)
    fn_type = build_varargs_function_type_array (return_type->as_tree (),
						 params->length (), arg_types);
  else
    fn_type = build_function_type_array (return_type->as_tree (),
					 params->length (), arg_types);
  free (arg_types);

  /* FIXME: this uses input_location: */
  tree fndecl = build_fn_decl (name, fn_type);

  if (loc)
    set_tree_location (fndecl, loc);

  tree resdecl = build_decl (UNKNOWN_LOCATION, RESULT_DECL,
			     NULL_TREE, return_type->as_tree ());
  DECL_ARTIFICIAL (resdecl) = 1;
  DECL_IGNORED_P (resdecl) = 1;
  DECL_RESULT (fndecl) = resdecl;

  if (builtin_id)
    {
      DECL_BUILT_IN_CLASS (fndecl) = BUILT_IN_NORMAL;
      DECL_FUNCTION_CODE (fndecl) = builtin_id;
      gcc_assert (loc == NULL);
      DECL_SOURCE_LOCATION (fndecl) = BUILTINS_LOCATION;
    }

  if (kind != GCC_JIT_FUNCTION_IMPORTED)
    {
      tree param_decl_list = NULL;
      FOR_EACH_VEC_ELT (*params, i, param)
	{
	  param_decl_list = chainon (param->as_tree (), param_decl_list);
	}

      /* The param list was created in reverse order; fix it: */
      param_decl_list = nreverse (param_decl_list);

      tree t;
      for (t = param_decl_list; t; t = DECL_CHAIN (t))
	{
	  DECL_CONTEXT (t) = fndecl;
	  DECL_ARG_TYPE (t) = TREE_TYPE (t);
	}

      /* Set it up on DECL_ARGUMENTS */
      DECL_ARGUMENTS(fndecl) = param_decl_list;
    }

  if (kind == GCC_JIT_FUNCTION_ALWAYS_INLINE)
    {
      DECL_DECLARED_INLINE_P (fndecl) = 1;

      /* Add attribute "always_inline": */
      DECL_ATTRIBUTES (fndecl) =
	tree_cons (get_identifier ("always_inline"),
		   NULL,
		   DECL_ATTRIBUTES (fndecl));
    }

  function *func = new function (this, fndecl, kind);
  m_functions.safe_push (func);
  return func;
}

playback::lvalue *
playback::context::
new_global (location *loc,
            type *type,
            const char *name)
{
  gcc_assert (type);
  gcc_assert (name);
  tree inner = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			   get_identifier (name),
			   type->as_tree ());
  TREE_PUBLIC (inner) = 1;
  DECL_COMMON (inner) = 1;
  DECL_EXTERNAL (inner) = 1;

  if (loc)
    set_tree_location (inner, loc);

  return new lvalue (this, inner);
}

playback::rvalue *
playback::context::
new_rvalue_from_int (type *type,
		     int value)
{
  // FIXME: type-checking, or coercion?
  tree inner_type = type->as_tree ();
  if (INTEGRAL_TYPE_P (inner_type))
    {
      tree inner = build_int_cst (inner_type, value);
      return new rvalue (this, inner);
    }
  else
    {
      REAL_VALUE_TYPE real_value;
      real_from_integer (&real_value, VOIDmode, value, 0, 0);
      tree inner = build_real (inner_type, real_value);
      return new rvalue (this, inner);
    }
}

playback::rvalue *
playback::context::
new_rvalue_from_double (type *type,
			double value)
{
  // FIXME: type-checking, or coercion?
  tree inner_type = type->as_tree ();

  /* We have a "double", we want a REAL_VALUE_TYPE.

     real.c:real_from_target appears to require the representation to be
     split into 32-bit values, and then sent as an pair of host long
     ints.  */
  REAL_VALUE_TYPE real_value;
  union
  {
    double as_double;
    uint32_t as_uint32s[2];
  } u;
  u.as_double = value;
  long int as_long_ints[2];
  as_long_ints[0] = u.as_uint32s[0];
  as_long_ints[1] = u.as_uint32s[1];
  real_from_target (&real_value, as_long_ints, DFmode);
  tree inner = build_real (inner_type, real_value);
  return new rvalue (this, inner);
}

playback::rvalue *
playback::context::
new_rvalue_from_ptr (type *type,
		     void *value)
{
  tree inner_type = type->as_tree ();
  /* FIXME: how to ensure we have a wide enough type?  */
  tree inner = build_int_cstu (inner_type, (unsigned HOST_WIDE_INT)value);
  return new rvalue (this, inner);
}

playback::rvalue *
playback::context::
new_string_literal (const char *value)
{
  tree t_str = build_string (strlen (value), value);
  gcc_assert (m_char_array_type_node);
  TREE_TYPE (t_str) = m_char_array_type_node;

  /* Convert to (const char*), loosely based on
     c/c-typeck.c: array_to_pointer_conversion,
     by taking address of start of string.  */
  tree t_addr = build1 (ADDR_EXPR, m_const_char_ptr, t_str);

  return new rvalue (this, t_addr);
}

tree
playback::context::
as_truth_value (tree expr, location *loc)
{
  /* Compare to c-typeck.c:c_objc_common_truthvalue_conversion */
  tree typed_zero = fold_build1 (CONVERT_EXPR,
				 TREE_TYPE (expr),
				 integer_zero_node);
  if (loc)
    set_tree_location (typed_zero, loc);

  expr = build2 (NE_EXPR, integer_type_node, expr, typed_zero);
  if (loc)
    set_tree_location (expr, loc);

  return expr;
}

playback::rvalue *
playback::context::
new_unary_op (location *loc,
	      enum gcc_jit_unary_op op,
	      type *result_type,
	      rvalue *a)
{
  // FIXME: type-checking, or coercion?
  enum tree_code inner_op;

  gcc_assert (result_type);
  gcc_assert (a);

  tree node = a->as_tree ();
  tree inner_result = NULL;

  switch (op)
    {
    default:
      add_error (loc, "unrecognized (enum gcc_jit_unary_op) value: %i", op);
      return NULL;

    case GCC_JIT_UNARY_OP_MINUS:
      inner_op = NEGATE_EXPR;
      break;

    case GCC_JIT_UNARY_OP_BITWISE_NEGATE:
      inner_op = BIT_NOT_EXPR;
      break;

    case GCC_JIT_UNARY_OP_LOGICAL_NEGATE:
      node = as_truth_value (node, loc);
      inner_result = invert_truthvalue (node);
      if (loc)
	set_tree_location (inner_result, loc);
      return new rvalue (this, inner_result);
    }

  inner_result = build1 (inner_op,
			 result_type->as_tree (),
			 node);
  if (loc)
    set_tree_location (inner_result, loc);

  return new rvalue (this, inner_result);
}

playback::rvalue *
playback::context::
new_binary_op (location *loc,
	       enum gcc_jit_binary_op op,
	       type *result_type,
	       rvalue *a, rvalue *b)
{
  // FIXME: type-checking, or coercion?
  enum tree_code inner_op;

  gcc_assert (result_type);
  gcc_assert (a);
  gcc_assert (b);

  tree node_a = a->as_tree ();
  tree node_b = b->as_tree ();

  switch (op)
    {
    default:
      add_error (loc, "unrecognized (enum gcc_jit_binary_op) value: %i", op);
      return NULL;

    case GCC_JIT_BINARY_OP_PLUS:
      inner_op = PLUS_EXPR;
      break;

    case GCC_JIT_BINARY_OP_MINUS:
      inner_op = MINUS_EXPR;
      break;

    case GCC_JIT_BINARY_OP_MULT:
      inner_op = MULT_EXPR;
      break;

    case GCC_JIT_BINARY_OP_DIVIDE:
      if (FLOAT_TYPE_P (result_type->as_tree ()))
	/* Floating-point division: */
	inner_op = RDIV_EXPR;
      else
	/* Truncating to zero: */
	inner_op = TRUNC_DIV_EXPR;
      break;

    case GCC_JIT_BINARY_OP_MODULO:
      inner_op = TRUNC_MOD_EXPR;
      break;

    case GCC_JIT_BINARY_OP_BITWISE_AND:
      inner_op = BIT_AND_EXPR;
      break;

    case GCC_JIT_BINARY_OP_BITWISE_XOR:
      inner_op = BIT_XOR_EXPR;
      break;

    case GCC_JIT_BINARY_OP_BITWISE_OR:
      inner_op = BIT_IOR_EXPR;
      break;

    case GCC_JIT_BINARY_OP_LOGICAL_AND:
      node_a = as_truth_value (node_a, loc);
      node_b = as_truth_value (node_b, loc);
      inner_op = TRUTH_ANDIF_EXPR;
      break;

    case GCC_JIT_BINARY_OP_LOGICAL_OR:
      node_a = as_truth_value (node_a, loc);
      node_b = as_truth_value (node_b, loc);
      inner_op = TRUTH_ORIF_EXPR;
      break;
    }

  tree inner_expr = build2 (inner_op,
			    result_type->as_tree (),
			    node_a,
			    node_b);
  if (loc)
    set_tree_location (inner_expr, loc);

  return new rvalue (this, inner_expr);
}

playback::rvalue *
playback::context::
new_comparison (location *loc,
		enum gcc_jit_comparison op,
		rvalue *a, rvalue *b)
{
  // FIXME: type-checking, or coercion?
  enum tree_code inner_op;

  gcc_assert (a);
  gcc_assert (b);

  switch (op)
    {
    default:
      add_error (loc, "unrecognized (enum gcc_jit_comparison) value: %i", op);
      return NULL;

    case GCC_JIT_COMPARISON_EQ:
      inner_op = EQ_EXPR;
      break;
    case GCC_JIT_COMPARISON_NE:
      inner_op = NE_EXPR;
      break;
    case GCC_JIT_COMPARISON_LT:
      inner_op = LT_EXPR;
      break;
    case GCC_JIT_COMPARISON_LE:
      inner_op = LE_EXPR;
      break;
    case GCC_JIT_COMPARISON_GT:
      inner_op = GT_EXPR;
      break;
    case GCC_JIT_COMPARISON_GE:
      inner_op = GE_EXPR;
      break;
    }

  tree inner_expr = build2 (inner_op,
			    boolean_type_node,
			    a->as_tree (),
			    b->as_tree ());
  if (loc)
    set_tree_location (inner_expr, loc);
  return new rvalue (this, inner_expr);
}

playback::rvalue *
playback::context::
new_call (location *loc,
	  function *func,
	  vec<rvalue *> args)
{
  tree fndecl;
  vec<tree, va_gc> *tree_args;

  gcc_assert (func);

  // FIXME: type checking
  // FIXME: check num args and types

  fndecl = func->as_fndecl ();

  vec_alloc (tree_args, args.length ());
  for (unsigned i = 0; i < args.length (); i++)
    tree_args->quick_push (args[i]->as_tree ());

  tree fntype = TREE_TYPE (fndecl);
  tree fn = build1 (ADDR_EXPR, build_pointer_type (fntype), fndecl);

  if (loc)
    set_tree_location (fn, loc);

  return new rvalue (this,
		     build_call_vec (func->get_return_type_as_tree (),
				     fn, tree_args));

  /* see c-typeck.c: build_function_call
     which calls build_function_call_vec

     which does lots of checking, then:
    result = build_call_array_loc (loc, TREE_TYPE (fntype),
				   function, nargs, argarray);
    which is in tree.c
    (see also build_call_vec)
   */
}

tree
playback::context::build_cast (playback::location *loc,
			       playback::rvalue *expr,
			       playback::type *type_)
{
  /* For comparison, see:
     - c/c-typeck.c:build_c_cast
     - c/c-convert.c: convert
     - convert.h

     Only some kinds of cast are currently supported here.  */
  tree t_expr = expr->as_tree ();
  tree t_dst_type = type_->as_tree ();
  tree t_ret = NULL;
  t_ret = targetm.convert_to_type (t_dst_type, t_expr);
  if (t_ret)
      return t_ret;
  enum tree_code dst_code = TREE_CODE (t_dst_type);
  switch (dst_code)
    {
    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
      t_ret = convert_to_integer (t_dst_type, t_expr);
      goto maybe_fold;

    case BOOLEAN_TYPE:
      /* Compare with c_objc_common_truthvalue_conversion and
	 c_common_truthvalue_conversion. */
      /* For now, convert to: (t_expr != 0)  */
      t_ret = build2 (NE_EXPR, t_dst_type,
		      t_expr, integer_zero_node);
      goto maybe_fold;

    case REAL_TYPE:
      t_ret = convert_to_real (t_dst_type, t_expr);
      goto maybe_fold;

    case POINTER_TYPE:
      t_ret = build1 (NOP_EXPR, t_dst_type, t_expr);
      goto maybe_fold;

    default:
      add_error (loc, "couldn't handle cast during playback");
      fprintf (stderr, "input expression:\n");
      debug_tree (t_expr);
      fprintf (stderr, "requested type:\n");
      debug_tree (t_dst_type);
      return error_mark_node;

    maybe_fold:
      if (TREE_CODE (t_ret) != C_MAYBE_CONST_EXPR)
	t_ret = fold (t_ret);
      return t_ret;
    }
}

playback::rvalue *
playback::context::
new_cast (playback::location *loc,
	  playback::rvalue *expr,
	  playback::type *type_)
{

  tree t_cast = build_cast (loc, expr, type_);
  if (loc)
    set_tree_location (t_cast, loc);
  return new rvalue (this, t_cast);
}

playback::lvalue *
playback::context::
new_array_access (location *loc,
		  rvalue *ptr,
		  rvalue *index)
{
  gcc_assert (ptr);
  gcc_assert (index);

  /* For comparison, see:
       c/c-typeck.c: build_array_ref
       c-family/c-common.c: pointer_int_sum
  */
  tree t_ptr = ptr->as_tree ();
  tree t_index = index->as_tree ();
  tree t_type_ptr = TREE_TYPE (t_ptr);
  tree t_type_star_ptr = TREE_TYPE (t_type_ptr);

  if (TREE_CODE (t_type_ptr) == ARRAY_TYPE)
    {
      tree t_result = build4 (ARRAY_REF, t_type_star_ptr, t_ptr, t_index,
			      NULL_TREE, NULL_TREE);
      if (loc)
        set_tree_location (t_result, loc);
      return new lvalue (this, t_result);
    }
  else
    {
      /* Convert index to an offset in bytes.  */
      tree t_sizeof = size_in_bytes (t_type_star_ptr);
      t_index = fold_build1 (CONVERT_EXPR, sizetype, t_index);
      tree t_offset = build2 (MULT_EXPR, sizetype, t_index, t_sizeof);

      /* Locate (ptr + offset).  */
      tree t_address = build2 (POINTER_PLUS_EXPR, t_type_ptr, t_ptr, t_offset);

      tree t_indirection = build1 (INDIRECT_REF, t_type_star_ptr, t_address);
      if (loc)
        {
          set_tree_location (t_sizeof, loc);
          set_tree_location (t_offset, loc);
          set_tree_location (t_address, loc);
          set_tree_location (t_indirection, loc);
        }

      return new lvalue (this, t_indirection);
    }
}

tree
playback::context::
new_field_access (location *loc,
		  tree datum,
		  field *field)
{
  gcc_assert (datum);
  gcc_assert (field);

  /* Compare with c/c-typeck.c:lookup_field, build_indirect_ref, and
     build_component_ref. */
  tree type = TREE_TYPE (datum);
  gcc_assert (type);
  gcc_assert (TREE_CODE (type) != POINTER_TYPE);

 tree t_field = field->as_tree ();
 tree ref = build3 (COMPONENT_REF, TREE_TYPE (t_field), datum,
		     t_field, NULL_TREE);
  if (loc)
    set_tree_location (ref, loc);
  return ref;
}

tree
playback::context::
new_dereference (tree ptr,
		 location *loc)
{
  gcc_assert (ptr);

  tree type = TREE_TYPE (TREE_TYPE(ptr));
  tree datum = build1 (INDIRECT_REF, type, ptr);
  if (loc)
    set_tree_location (datum, loc);
  return datum;
}

playback::lvalue *
playback::lvalue::
access_field (location *loc,
	      field *field)
{
  tree datum = as_tree ();
  tree ref = get_context ()->new_field_access (loc, datum, field);
  if (!ref)
    return NULL;
  return new lvalue (get_context (), ref);
}

playback::rvalue *
playback::rvalue::
access_field (location *loc,
	      field *field)
{
  tree datum = as_tree ();
  tree ref = get_context ()->new_field_access (loc, datum, field);
  if (!ref)
    return NULL;
  return new rvalue (get_context (), ref);
}

playback::lvalue *
playback::rvalue::
dereference_field (location *loc,
		   field *field)
{
  tree ptr = as_tree ();
  tree datum = get_context ()->new_dereference (ptr, loc);
  if (!datum)
    return NULL;
  tree ref = get_context ()->new_field_access (loc, datum, field);
  if (!ref)
    return NULL;
  return new lvalue (get_context (), ref);
}

playback::lvalue *
playback::rvalue::
dereference (location *loc)
{
  tree ptr = as_tree ();
  tree datum = get_context ()->new_dereference (ptr, loc);
  return new lvalue (get_context (), datum);
}

playback::rvalue *
playback::lvalue::
get_address (location *loc)
{
  tree t_lvalue = as_tree ();
  tree t_thistype = TREE_TYPE (t_lvalue);
  tree t_ptrtype = build_pointer_type (t_thistype);
  tree ptr = build1 (ADDR_EXPR, t_ptrtype, t_lvalue);
  if (loc)
    get_context ()->set_tree_location (ptr, loc);
  return new rvalue (get_context (), ptr);
}

void *
playback::wrapper::
operator new (size_t sz)
{
  return ggc_internal_cleared_alloc_stat (sz MEM_STAT_INFO);
}

playback::function::
function (context *ctxt,
	  tree fndecl,
	  enum gcc_jit_function_kind kind)
: m_ctxt(ctxt),
  m_inner_fndecl (fndecl),
  m_inner_bind_expr (NULL),
  m_kind (kind)
{
  if (m_kind != GCC_JIT_FUNCTION_IMPORTED)
    {
      /* Create a BIND_EXPR, and within it, a statement list.  */
      m_stmt_list = alloc_stmt_list ();
      m_stmt_iter = tsi_start (m_stmt_list);
      m_inner_block = make_node (BLOCK);
      m_inner_bind_expr =
	build3 (BIND_EXPR, void_type_node, NULL, m_stmt_list, m_inner_block);
    }
  else
    {
      m_inner_block = NULL;
      m_stmt_list = NULL;
    }
}

void
playback::function::
gt_ggc_mx ()
{
  gt_ggc_m_9tree_node (m_inner_fndecl);
  gt_ggc_m_9tree_node (m_inner_bind_expr);
  gt_ggc_m_9tree_node (m_stmt_list);
  gt_ggc_m_9tree_node (m_inner_block);
}

tree
playback::function::
get_return_type_as_tree () const
{
  return TREE_TYPE (TREE_TYPE(m_inner_fndecl));
}

playback::lvalue *
playback::function::
new_local (location *loc,
	   type *type,
	   const char *name)
{
  gcc_assert (type);
  gcc_assert (name);
  tree inner = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			   get_identifier (name),
			   type->as_tree ());
  DECL_CONTEXT (inner) = this->m_inner_fndecl;

  /* Prepend to BIND_EXPR_VARS: */
  DECL_CHAIN (inner) = BIND_EXPR_VARS (m_inner_bind_expr);
  BIND_EXPR_VARS (m_inner_bind_expr) = inner;

  if (loc)
    set_tree_location (inner, loc);
  return new lvalue (m_ctxt, inner);
}

playback::block *
playback::function::
new_block (const char *name)
{
  gcc_assert (m_kind != GCC_JIT_FUNCTION_IMPORTED);

  block *result = new playback::block (this, name);
  m_blocks.safe_push (result);
  return result;
}

void
playback::function::
build_stmt_list ()
{
  int i;
  block *b;

  FOR_EACH_VEC_ELT (m_blocks, i, b)
    {
      int j;
      tree stmt;

      b->m_label_expr = build1 (LABEL_EXPR,
				void_type_node,
				b->as_label_decl ());
      tsi_link_after (&m_stmt_iter, b->m_label_expr, TSI_CONTINUE_LINKING);

      FOR_EACH_VEC_ELT (b->m_stmts, j, stmt)
	tsi_link_after (&m_stmt_iter, stmt, TSI_CONTINUE_LINKING);
    }
}

void
playback::function::
postprocess ()
{
  if (m_ctxt->get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_INITIAL_TREE))
    debug_tree (m_stmt_list);

  /* Do we need this to force cgraphunit.c to output the function? */
  if (m_kind == GCC_JIT_FUNCTION_EXPORTED)
    {
      DECL_EXTERNAL (m_inner_fndecl) = 0;
      DECL_PRESERVE_P (m_inner_fndecl) = 1;
    }

  if (m_kind == GCC_JIT_FUNCTION_INTERNAL
      ||m_kind == GCC_JIT_FUNCTION_ALWAYS_INLINE)
    {
      DECL_EXTERNAL (m_inner_fndecl) = 0;
      TREE_PUBLIC (m_inner_fndecl) = 0;
    }

  if (m_kind != GCC_JIT_FUNCTION_IMPORTED)
    {
      /* Seem to need this in gimple-low.c: */
      gcc_assert (m_inner_block);
      DECL_INITIAL (m_inner_fndecl) = m_inner_block;

      /* how to add to function? the following appears to be how to
	 set the body of a m_inner_fndecl: */
      DECL_SAVED_TREE(m_inner_fndecl) = m_inner_bind_expr;

      /* Ensure that locals appear in the debuginfo.  */
      BLOCK_VARS (m_inner_block) = BIND_EXPR_VARS (m_inner_bind_expr);

      //debug_tree (m_inner_fndecl);

      /* Convert to gimple: */
      //printf("about to gimplify_function_tree\n");
      gimplify_function_tree (m_inner_fndecl);
      //printf("finished gimplify_function_tree\n");

      current_function_decl = m_inner_fndecl;
      if (m_ctxt->get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_INITIAL_GIMPLE))
	dump_function_to_file (m_inner_fndecl, stderr, TDF_VOPS|TDF_MEMSYMS|TDF_LINENO);
      //debug_tree (m_inner_fndecl);

      //printf("about to add to cgraph\n");
      /* Add to cgraph: */
      cgraph_finalize_function (m_inner_fndecl, false);
      /* This can trigger a collection, so we need to have all of
	 the funcs as roots.  */

      current_function_decl = NULL;
    }
}

void
playback::block::
add_eval (location *loc,
	  rvalue *rvalue)
{
  gcc_assert (rvalue);

  if (loc)
    set_tree_location (rvalue->as_tree (), loc);

  add_stmt (rvalue->as_tree ());
}

void
playback::block::
add_assignment (location *loc,
		lvalue *lvalue,
		rvalue *rvalue)
{
  gcc_assert (lvalue);
  gcc_assert (rvalue);

  tree t_lvalue = lvalue->as_tree ();
  tree t_rvalue = rvalue->as_tree ();
  if (TREE_TYPE (t_rvalue) != TREE_TYPE (t_lvalue))
    {
      t_rvalue = build1 (CONVERT_EXPR,
		         TREE_TYPE (t_lvalue),
		         t_rvalue);
      if (loc)
	set_tree_location (t_rvalue, loc);
    }

  tree stmt =
    build2 (MODIFY_EXPR, TREE_TYPE (t_lvalue),
	    t_lvalue, t_rvalue);
  if (loc)
    set_tree_location (stmt, loc);
  add_stmt (stmt);
}

void
playback::block::
add_comment (location *loc,
	     const char *text)
{
  /* Wrap the text in C-style comment delimiters.  */
  size_t sz =
    (3 /* opening delim */
     + strlen (text)
     + 3 /* closing delim */
     + 1 /* terminator */);
  char *wrapped = (char *)ggc_internal_alloc_stat (sz);
  snprintf (wrapped, sz, "/* %s */", text);

  /* For now we simply implement this by adding a dummy label with a name
     containing the given text.  */
  tree identifier = get_identifier (wrapped);
  tree label_decl = build_decl (UNKNOWN_LOCATION, LABEL_DECL,
				identifier, void_type_node);
  DECL_CONTEXT (label_decl) = m_func->as_fndecl ();

  tree label_expr = build1 (LABEL_EXPR, void_type_node, label_decl);
  if (loc)
    set_tree_location (label_expr, loc);
  add_stmt (label_expr);
}

void
playback::block::
add_conditional (location *loc,
		 rvalue *boolval,
		 block *on_true,
		 block *on_false)
{
  gcc_assert (boolval);
  gcc_assert (on_true);
  gcc_assert (on_false);

  /* COND_EXPR wants statement lists for the true/false operands, but we
     want labels.
     Shim it by creating jumps to the labels */
  tree true_jump = build1 (GOTO_EXPR, void_type_node,
			   on_true->as_label_decl ());
  if (loc)
    set_tree_location (true_jump, loc);

  tree false_jump = build1 (GOTO_EXPR, void_type_node,
			    on_false->as_label_decl ());
  if (loc)
    set_tree_location (false_jump, loc);

  tree stmt =
    build3 (COND_EXPR, void_type_node, boolval->as_tree (),
	    true_jump, false_jump);
  if (loc)
    set_tree_location (stmt, loc);
  add_stmt (stmt);
}

void
playback::block::
add_jump (location *loc,
	  block *target)
{
  gcc_assert (target);

  // see c_finish_loop
  //tree top = build1 (LABEL_EXPR, void_type_node, NULL_TREE);
  //add_stmt (top);

  //tree stmt = build_and_jump (&LABEL_EXPR_LABEL (target->label_));
  TREE_USED (target->as_label_decl ()) = 1;
  tree stmt = build1 (GOTO_EXPR, void_type_node, target->as_label_decl ());
  if (loc)
    set_tree_location (stmt, loc);
  add_stmt (stmt);

  /*
  from c-typeck.c:
tree
c_finish_goto_label (location_t loc, tree label)
{
  tree decl = lookup_label_for_goto (loc, label);
  if (!decl)
    return NULL_TREE;
  TREE_USED (decl) = 1;
  {
    tree t = build1 (GOTO_EXPR, void_type_node, decl);
    SET_EXPR_LOCATION (t, loc);
    return add_stmt (t);
  }
}
  */

}

void
playback::block::
add_return (location *loc,
	    rvalue *rvalue)
{
  tree modify_retval = NULL;
  tree return_type = m_func->get_return_type_as_tree ();
  if (rvalue)
    {
      tree t_lvalue = DECL_RESULT (m_func->as_fndecl ());
      tree t_rvalue = rvalue->as_tree ();
      if (TREE_TYPE (t_rvalue) != TREE_TYPE (t_lvalue))
	t_rvalue = build1 (CONVERT_EXPR,
			   TREE_TYPE (t_lvalue),
			   t_rvalue);
      modify_retval = build2 (MODIFY_EXPR, return_type,
			      t_lvalue, t_rvalue);
      if (loc)
	set_tree_location (modify_retval, loc);
    }
  tree return_stmt = build1 (RETURN_EXPR, return_type,
			     modify_retval);
  if (loc)
    set_tree_location (return_stmt, loc);

  add_stmt (return_stmt);
}

playback::block::
block (function *func,
       const char *name)
: m_func (func),
  m_stmts ()
{
  tree identifier;

  gcc_assert (func);
  // name can be NULL
  if (name)
    identifier = get_identifier (name);
  else
    identifier = NULL;
  m_label_decl = build_decl (UNKNOWN_LOCATION, LABEL_DECL,
			    identifier, void_type_node);
  DECL_CONTEXT (m_label_decl) = func->as_fndecl ();
  m_label_expr = NULL;
}

result *
playback::context::
compile ()
{
  void *handle = NULL;
  const char *ctxt_progname;
  result *result_obj = NULL;
  const char *fake_args[20];
  unsigned int num_args;

  m_path_template = xstrdup ("/tmp/libgccjit-XXXXXX");
  if (!m_path_template)
    return NULL;

  /* Create tempdir using mkdtemp.  This is created with 0700 perms and
     is unique.  Hence no other (non-root) users should have access to
     the paths within it.  */
  m_path_tempdir = mkdtemp (m_path_template);
  if (!m_path_tempdir)
    return NULL;
  m_path_c_file = concat (m_path_tempdir, "/fake.c", NULL);
  m_path_s_file = concat (m_path_tempdir, "/fake.s", NULL);
  m_path_so_file = concat (m_path_tempdir, "/fake.so", NULL);

  /* Call into the rest of gcc.
     For now, we have to assemble command-line options to pass into
     toplev::main, so that they can be parsed. */

  /* Pass in user-provided program name as argv0, if any, so that it
     makes it into GCC's "progname" global, used in various diagnostics. */
  ctxt_progname = get_str_option (GCC_JIT_STR_OPTION_PROGNAME);
  fake_args[0] =
    (ctxt_progname ? ctxt_progname : "libgccjit.so");

  fake_args[1] = m_path_c_file;
  num_args = 2;

#define ADD_ARG(arg) \
  do \
    { \
      gcc_assert(num_args < sizeof(fake_args)/sizeof(char*)); \
      fake_args[num_args++] = arg; \
    } \
  while (0)

  ADD_ARG ("-fPIC");

  /* Handle int options: */
  switch (get_int_option (GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL))
    {
    default:
      add_error (NULL,
		 "unrecognized optimization level: %i",
		 get_int_option (GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL));
      return NULL;

    case 0:
      ADD_ARG ("-O0");
      break;

    case 1:
      ADD_ARG ("-O1");
      break;

    case 2:
      ADD_ARG ("-O2");
      break;

    case 3:
      ADD_ARG ("-O3");
      break;
    }
  /* What about -Os? */

  /* Handle bool options: */
  if (get_bool_option (GCC_JIT_BOOL_OPTION_DEBUGINFO))
    ADD_ARG ("-g");

  /* Suppress timing (and other) info.  */
  if (!get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_SUMMARY))
    {
      ADD_ARG ("-quiet");
      quiet_flag = 1;
    }

  /* Aggressively garbage-collect, to shake out bugs: */
  if (get_bool_option (GCC_JIT_BOOL_OPTION_SELFCHECK_GC))
    {
      ADD_ARG ("--param");
      ADD_ARG ("ggc-min-expand=0");
      ADD_ARG ("--param");
      ADD_ARG ("ggc-min-heapsize=0");
    }

  if (get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_EVERYTHING))
    {
      ADD_ARG ("-fdump-tree-all");
      ADD_ARG ("-fdump-rtl-all");
      ADD_ARG ("-fdump-ipa-all");
    }

  toplev toplev (false);

  toplev.main (num_args, const_cast <char **> (fake_args));
  toplev.finalize ();

  active_playback_ctxt = NULL;

  if (errors_occurred ())
    return NULL;

  if (get_bool_option (GCC_JIT_BOOL_OPTION_DUMP_GENERATED_CODE))
   dump_generated_code ();

  /* Gross hacks follow:
     We have a .s file; we want a .so file.
     We could reuse parts of gcc/gcc.c to do this.
     For now, just use the /usr/bin/gcc on the system...
   */
  /* FIXME: totally faking it for now, not even using pex */
  {
    auto_timevar assemble_timevar (TV_ASSEMBLE);

    char cmd[1024];
    snprintf (cmd, 1024, "gcc -shared %s -o %s",
              m_path_s_file, m_path_so_file);
    if (0)
      printf ("cmd: %s\n", cmd);
    int ret = system (cmd);
    if (ret)
      return NULL;
  }

  // TODO: split out assembles vs linker

  /* dlopen the .so file. */
  {
    auto_timevar load_timevar (TV_LOAD);

    const char *error;

    /* Clear any existing error.  */
    dlerror ();

    handle = dlopen (m_path_so_file, RTLD_NOW | RTLD_LOCAL);
    if ((error = dlerror()) != NULL)  {
      add_error (NULL, "%s", error);
    }
    if (handle)
      result_obj = new result (handle);
    else
      result_obj = NULL;
  }

  return result_obj;
}

void
playback::context::
replay ()
{
  /* Adapted from c-common.c:c_common_nodes_and_builtins.  */
  tree array_domain_type = build_index_type (size_int (200));
  m_char_array_type_node
    = build_array_type (char_type_node, array_domain_type);

  m_const_char_ptr
    = build_pointer_type (build_qualified_type (char_type_node,
						TYPE_QUAL_CONST));

  /* Replay the recorded events:  */
  timevar_push (TV_JIT_REPLAY);

  m_recording_ctxt->replay_into (this);

  /* Clean away the temporary references from recording objects
     to playback objects.  We have to do this now since the
     latter are GC-allocated, but the former don't mark these
     refs.  Hence we must stop using them before the GC can run.  */
  m_recording_ctxt->disassociate_from_playback ();

  timevar_pop (TV_JIT_REPLAY);

  if (!errors_occurred ())
    {
      int i;
      function *func;

      /* No GC can happen yet; process the cached source locations.  */
      handle_locations ();

      /* We've now created tree nodes for the stmts in the various blocks
	 in each function, but we haven't built each function's single stmt
	 list yet.  Do so now.  */
      FOR_EACH_VEC_ELT (m_functions, i, func)
	func->build_stmt_list ();

      /* No GC can have happened yet.  */

      /* Postprocess the functions.  This could trigger GC.  */
      FOR_EACH_VEC_ELT (m_functions, i, func)
	{
	  gcc_assert (func);
	  func->postprocess ();
	}
    }
}

void
playback::context::
dump_generated_code ()
{
  char buf[4096];
  size_t sz;
  FILE *f_in = fopen (m_path_s_file, "r");
  if (!f_in)
    return;

  while ( (sz = fread (buf, 1, sizeof (buf), f_in)) )
    fwrite (buf, 1, sz, stderr);

  fclose (f_in);
}

static int
line_comparator (const void *lhs, const void *rhs)
{
  const playback::source_line *line_lhs = \
    *static_cast<const playback::source_line * const*> (lhs);
  const playback::source_line *line_rhs = \
    *static_cast<const playback::source_line * const*> (rhs);
  return line_lhs->get_line_num () - line_rhs->get_line_num ();
}

static int
location_comparator (const void *lhs, const void *rhs)
{
  const playback::location *loc_lhs = \
    *static_cast<const playback::location * const *> (lhs);
  const playback::location *loc_rhs = \
    *static_cast<const playback::location * const *> (rhs);
  return loc_lhs->get_column_num () - loc_rhs->get_column_num ();
}

void
playback::context::
handle_locations ()
{
  /* Create the source code locations, following the ordering rules
     imposed by the linemap API.

     line_table is a global.  */
  int i;
  source_file *file;

  FOR_EACH_VEC_ELT (m_source_files, i, file)
    {
      linemap_add (line_table, LC_ENTER, false, file->get_filename (), 0);

      /* Sort lines by ascending line numbers.  */
      file->m_source_lines.qsort (&line_comparator);

      int j;
      source_line *line;
      FOR_EACH_VEC_ELT (file->m_source_lines, j, line)
	{
	  int k;
	  location *loc;

	  /* Sort locations in line by ascending column numbers.  */
	  line->m_locations.qsort (&location_comparator);

	  /* Determine maximum column within this line.  */
	  gcc_assert (line->m_locations.length () > 0);
	  location *final_column =
	    line->m_locations[line->m_locations.length () - 1];
	  int max_col = final_column->get_column_num ();

	  linemap_line_start (line_table, line->get_line_num (), max_col);
	  FOR_EACH_VEC_ELT (line->m_locations, k, loc)
	    {
	      loc->m_srcloc =					   \
		linemap_position_for_column (line_table, loc->get_column_num ());
	    }
	}

      linemap_add (line_table, LC_LEAVE, false, NULL, 0);
    }

  /* line_table should now be populated; every playback::location should
     now have an m_srcloc.  */

  if (0)
    line_table_dump (stderr,
		     line_table,
		     LINEMAPS_ORDINARY_USED (line_table),
		     LINEMAPS_MACRO_USED (line_table));

  /* Now assign them to tree nodes as appropriate.  */
  std::pair<tree, location *> *cached_location;

  FOR_EACH_VEC_ELT (m_cached_locations, i, cached_location)
    {
      tree t = cached_location->first;
      source_location srcloc = cached_location->second->m_srcloc;
#if 0
      inform (srcloc, "location of ");
      debug_tree (t);
#endif

      /* This covers expressions: */
      if (CAN_HAVE_LOCATION_P (t))
	SET_EXPR_LOCATION (t, srcloc);
      else if (CODE_CONTAINS_STRUCT(TREE_CODE(t), TS_DECL_MINIMAL))
	DECL_SOURCE_LOCATION (t) = srcloc;
      else
	{
	  /* Don't know how to set location on this node.  */
	  if (0)
	    {
	      fprintf (stderr, "can't set location on:");
	      debug_tree (t);
	      fprintf (stderr, "\n");
	    }
	}
    }
}


void
playback::context::
add_error (location *loc, const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  m_recording_ctxt->add_error_va (loc ? loc->get_recording_loc () : NULL,
				  fmt, ap);
  va_end (ap);
}

void
playback::context::
add_error_va (location *loc, const char *fmt, va_list ap)
{
  m_recording_ctxt->add_error_va (loc ? loc->get_recording_loc () : NULL,
				  fmt, ap);
}

result::
result(void *dso_handle)
  : m_dso_handle(dso_handle)
{
}

result::~result()
{
  dlclose (m_dso_handle);
}

void *
result::
get_code (const char *funcname)
{
  void *code;
  const char *error;

  /* Clear any existing error.  */
  dlerror ();

  code = dlsym (m_dso_handle, funcname);

  if ((error = dlerror()) != NULL)  {
    fprintf(stderr, "%s\n", error);
  }

  return code;
}

/* Dealing with the linemap API.  */

playback::location *
playback::context::
new_location (recording::location *rloc,
	      const char *filename,
	      int line,
	      int column)
{
  /* Get the source_file for filename, creating if necessary.  */
  source_file *src_file = get_source_file (filename);
  /* Likewise for the line within the file.  */
  source_line *src_line = src_file->get_source_line (line);
  /* Likewise for the column within the line.  */
  location *loc = src_line->get_location (rloc, column);
  return loc;
}

void
playback::context::
set_tree_location (tree t, location *loc)
{
  gcc_assert (loc);
  m_cached_locations.safe_push (std::make_pair (t, loc));
}


playback::source_file *
playback::context::
get_source_file (const char *filename)
{
  /* Locate the file.
     For simplicitly, this is currently a linear search.
     Replace with a hash if this shows up in the profile.  */
  int i;
  source_file *file;
  tree ident_filename = get_identifier (filename);

  FOR_EACH_VEC_ELT (m_source_files, i, file)
    if (file->filename_as_tree () == ident_filename)
      return file;

  /* Not found.  */
  file = new source_file (ident_filename);
  m_source_files.safe_push (file);
  return file;
}

playback::source_file::source_file (tree filename) :
  m_source_lines (),
  m_filename (filename)
{
}

playback::source_line *
playback::source_file::
get_source_line (int line_num)
{
  /* Locate the line.
     For simplicitly, this is currently a linear search.
     Replace with a hash if this shows up in the profile.  */
  int i;
  source_line *line;

  FOR_EACH_VEC_ELT (m_source_lines, i, line)
    if (line->get_line_num () == line_num)
      return line;

  /* Not found.  */
  line = new source_line (this, line_num);
  m_source_lines.safe_push (line);
  return line;
}

playback::source_line::source_line (source_file *file, int line_num) :
  m_locations (),
  m_source_file (file),
  m_line_num (line_num)
{
}

playback::location *
playback::source_line::
get_location (recording::location *rloc, int column_num)
{
  int i;
  location *loc;

  /* Another linear search that probably should be a hash table.  */
  FOR_EACH_VEC_ELT (m_locations, i, loc)
    if (loc->get_column_num () == column_num)
      return loc;

  /* Not found.  */
  loc = new location (rloc, this, column_num);
  m_locations.safe_push (loc);
  return loc;
}

playback::location::location (recording::location *loc,
			      source_line *line,
			      int column_num) :
  m_srcloc (UNKNOWN_LOCATION),
  m_recording_loc (loc),
  m_line (line),
  m_column_num(column_num)
{
}

playback::context *active_playback_ctxt;

} // namespace gcc::jit

} // namespace gcc
