// A single compilation.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "typedefs.hh"
#include "codegen.hh"
#include "bytecode/bytegen.hh"
#include "reader/reader.hh"
#include "buffer.hh"
#include "dump.hh"

compiler::compiler (const std::string &name)
  : work_monitor (),
    work_condition (work_monitor),
    multi_threaded (0),
    mt_monitor (),
    mt_condition (mt_monitor),
    pause_monitor (),
    pause_condition (pause_monitor),
    factory (NULL),
    primordial_package (new model_primordial_package ()),
    unnamed_package (new model_unnamed_package ()),
    generating_bytecode (false),
    ok (true),
    encoding ("UTF-8"),
    tab_width (8),
    name (name),
    state (SETTING_OPTIONS),
    can_accept_classes (true),
    need_class_method_bodies (true),
    can_accept_resources (false),
    // Currently we use the same mutex for all caches.
    // This is not as efficient as it could be.
    // FIXME?
    java_lang_Object (mt_monitor,
		      "java.lang.Object"),
    java_io_Serializable (mt_monitor,
			  "java.io.Serializable"),
    java_lang_Cloneable (mt_monitor,
			 "java.lang.Cloneable"),
    java_lang_String (mt_monitor,
		      "java.lang.String"),
    java_lang_Throwable (mt_monitor,
			 "java.lang.Throwable"),
    java_lang_Error (mt_monitor,
		     "java.lang.Error"),
    java_lang_RuntimeException (mt_monitor,
				"java.lang.RuntimeException"),
    java_lang_Iterable (mt_monitor,
			"java.lang.Iterable"),
    java_lang_Boolean (mt_monitor,
		       "java.lang.Boolean"),
    java_lang_Byte (mt_monitor,
		    "java.lang.Byte"),
    java_lang_Short (mt_monitor,
		     "java.lang.Short"),
    java_lang_Character (mt_monitor,
			 "java.lang.Character"),
    java_lang_Integer (mt_monitor,
		       "java.lang.Integer"),
    java_lang_Long (mt_monitor,
		    "java.lang.Long"),
    java_lang_Float (mt_monitor,
		     "java.lang.Float"),
    java_lang_Double (mt_monitor,
		      "java.lang.Double"),
    java_lang_Void (mt_monitor,
                    "java.lang.Void"),
    java_lang_AssertionError (mt_monitor,
			      "java.lang.AssertionError"),
    java_lang_StringBuffer (mt_monitor,
			    "java.lang.StringBuffer"),
    java_lang_StringBuilder (mt_monitor,
			     "java.lang.StringBuilder"),
    java_lang_NullPointerException (mt_monitor,
				    "java.lang.NullPointerException"),
    java_lang_Class (mt_monitor,
		     "java.lang.Class"),
    java_lang_Enum (mt_monitor,
		    "java.lang.Enum"),
    java_util_List (mt_monitor,
		    "java.util.List"),
    java_util_Iterator (mt_monitor,
			"java.util.Iterator"),
    gnu_gcj_runtime_StringBuffer (mt_monitor,
				  "gnu.gcj.runtime.StringBuffer"),
    java_io_ObjectStreamField (mt_monitor,
			       "java.io.ObjectStreamField"),
    java_lang_Exception (mt_monitor,
			 "java.lang.Exception"),
    java_lang_ClassNotFoundException (mt_monitor,
				      "java.lang.ClassNotFoundException"),
    java_lang_NoClassDefFoundError (mt_monitor,
				    "java.lang.NoClassDefFoundError"),
    java_lang_Deprecated (mt_monitor,
			  "java.lang.Deprecated"),
    java_lang_SuppressWarnings (mt_monitor,
				"java.lang.SuppressWarnings"),
    java_lang_Override (mt_monitor,
			"java.lang.Override"),
    java_lang_annotation_Annotation (mt_monitor,
				     "java.lang.annotation.Annotation"),
    java_lang_annotation_Inherited (mt_monitor,
				    "java.lang.annotation.Inherited"),
    java_lang_annotation_Retention (mt_monitor,
				    "java.lang.annotation.Retention"),
    java_lang_annotation_Target (mt_monitor,
				 "java.lang.annotation.Target"),
    java_lang_annotation_RetentionPolicy (mt_monitor,
					  "java.lang.annotation.RetentionPolicy"),
    java_lang_annotation_ElementType (mt_monitor,
				      "java.lang.annotation.ElementType"),
    verbose (false),
    pedantic (false),
    warnings_are_errors (false),
    dump_methods (false),
    dump_tree (false),
    feature_assert (true),
    feature_enum (true),
    feature_static_import (true),
    feature_enhanced_for (true),
    feature_generics (true),
    feature_boxing_conversion (true),
    feature_varargs (true),
    feature_annotations (true),
    target_assert (true),
    target_debug (false),
    target_15 (false),
    // For now this is the default.
    target_14 (true),
    target_verify (false)
{
}

compiler::~compiler ()
{
  delete factory;
  for (std::list<code_generator *>::const_iterator i = back_ends.begin ();
       i != back_ends.end ();
       ++i)
    delete *i;
}

std::string
compiler::get_name ()
{
  return name;
}

void
compiler::add_unit (const ref_unit &unit, bool emit_code)
{
  concurrence::exclusive_mutex::lock_sentinel sync (mt_monitor);
  units.push_back (unit);
  if (emit_code && (state < ANALYZING_CLASSES || generating_bytecode))
    code_generation_units.push_back (unit);
}

void
compiler::set_class_factory (classpath_class_factory *fac)
{
  assert (! factory);
  assert (state == SETTING_OPTIONS);
  factory = fac;
}

model_class *
compiler::find_class (const std::string &name)
{
  return find_class (split (name, '.'));
}

model_class *
compiler::find_class (const std::list<std::string> &name)
{
  // FIXME: this is called by classify_package_name, which is called
  // by the parsers.  Probably they should call some new function to
  // force creation of a package.
  //  assert (state > PARSING_FILES);
  Iname *obj = find_name (name);
  if (obj == NULL)
    {
      factory->load_class (name);
      obj = find_name (name);
      if (! obj)
	return NULL;
    }
  if (obj->package_p ())
    {
      // fixme: what?
      return NULL;
    }
  return assert_cast<model_class *> (obj);
}

Iname *
compiler::find_name (const std::list<std::string> &name)
{
  Iname *result;
  if (simple_name_p (name))
    result = unnamed_package.get ();
  else
    result = primordial_package.get ();
  for (std::list<std::string>::const_iterator i = name.begin ();
       result && i != name.end ();
       ++i)
    {
      result = result->find_name (*i);
      // fixme it would be nice to have a better error result than
      // just null
    }
  return result;
}

void
compiler::load_source_file (const std::string &name)
{
  assert (state < ANALYZING_CLASSES);

  if (state == SETTING_OPTIONS)
    {
      // See if we can accept class files, and, if so, whether reading
      // a class this way should keep a method body around.
      for (std::list<code_generator *>::const_iterator i = back_ends.begin ();
	   i != back_ends.end ();
	   ++i)
	{
	  // Note that the default is to accept class files and to
	  // keep class method bodies.  This ensures that '-o none'
	  // will verify the bytecode handed to the compiler.
	  if (! (*i)->handles_class_p ())
	    can_accept_classes = false;
	  if (! (*i)->needs_class_method_bodies_p ())
	    need_class_method_bodies = false;
	  if ((*i)->handles_resources_p ())
	    can_accept_resources = true;
	}
      // There's no point accepting resources if we can't also accept
      // class files.
      if (! can_accept_classes)
	can_accept_resources = false;
    }
  state = PARSING_FILES;

  if (! can_accept_classes && (jar_file_p (name) || class_file_p (name)))
    {
      std::cerr << get_name () << ": while reading "
		<< name << ": back end can't handle jar, zip, or class file"
		<< std::endl;
      set_failed ();
    }
  else
    {
      work_item item (name);
      add_job (item);
    }
}

void
compiler::compile_resource (const std::string &name, reader *contents)
{
  byte_buffer *bytes = contents->read_all ();
  // FIXME: catch exceptions?
  for (std::list<code_generator *>::const_iterator i = back_ends.begin ();
       i != back_ends.end ();
       ++i)
    (*i)->compile_resource (name, bytes);
  delete bytes;
  delete contents;
}

bool
compiler::semantic_analysis ()
{
  // Note that if no source files were parsed, the state won't be
  // PARSING_FILES.
  assert (state <= PARSING_FILES);
  state = ANALYZING_CLASSES;
  // After we're finished parsing things from the command line, there
  // is no longer a reason for the class reader to keep method bodies
  // attached.
  need_class_method_bodies = false;

  for (std::list<ref_unit>::const_iterator i = code_generation_units.begin ();
       ok && i != code_generation_units.end ();
       ++i)
    // Note that we do this single-threaded, as we don't have support
    // for MT semantic analysis.
    do_analyze_unit ((*i).get ());

  return ok;
}

bool
compiler::generate_code ()
{
  assert (ok);

  assert (state == ANALYZING_CLASSES);
  state = GENERATING_CODE;

  // Perhaps we aren't generating code at all.  This is perfectly ok.
  if (back_ends.empty ())
    return true;

  for (std::list<ref_unit>::const_iterator i = code_generation_units.begin ();
       i != code_generation_units.end ();
       ++i)
    {
      // Generate code with each requested code generator.
      work_item job (GENERATE_CODE, (*i).get ());
      add_job (job);
    }

  // Insert the appropriate number of death notices.
  work_item job (DIE);
  for (int i = 0; i < multi_threaded; ++i)
    add_job (job);

  // Now wait for all worker threads to die.
  // FIXME: it would make more sense, perhaps, to turn this thread
  // into a worker as well...?
  {
    concurrence::exclusive_mutex::lock_sentinel sync (mt_monitor);
    while (multi_threaded)
      mt_condition.wait ();
  }

  return ok;
}

void
compiler::add_code_generator (code_generator *cgen)
{
  assert (state == SETTING_OPTIONS);
  back_ends.push_back (cgen);
  // We only want the dependency compilation behavior if we are *only*
  // generating bytecode.
  generating_bytecode = (dynamic_cast<bytecode_code_generator *> (cgen) != NULL
			 && back_ends.size () == 1);
}



void
compiler::set_source_1_5 ()
{
  assert (state == SETTING_OPTIONS);
  feature_assert = true;
  feature_enum = true;
  feature_static_import = true;
  feature_enhanced_for = true;
  feature_generics = true;
  feature_boxing_conversion = true;
  feature_varargs = true;
  feature_annotations = true;
}

void
compiler::set_source_1_4 ()
{
  assert (state == SETTING_OPTIONS);
  feature_assert = true;
  feature_enum = false;
  feature_static_import = false;
  feature_enhanced_for = false;
  feature_generics = false;
  feature_boxing_conversion = false;
  feature_varargs = false;
  feature_annotations = false;
}

void
compiler::set_source_1_3 ()
{
  assert (state == SETTING_OPTIONS);
  feature_assert = false;
  feature_enum = false;
  feature_static_import = false;
  feature_enhanced_for = false;
  feature_generics = false;
  feature_boxing_conversion = false;
  feature_varargs = false;
  feature_annotations = false;
}

void
compiler::set_target_1_5 ()
{
  assert (state == SETTING_OPTIONS);
  target_15 = true;
  target_14 = false;
  target_assert = true;
}

void
compiler::set_target_1_4 ()
{
  assert (state == SETTING_OPTIONS);
  target_15 = false;
  target_14 = true;
  target_assert = true;
}

void
compiler::set_target_1_3 ()
{
  assert (state == SETTING_OPTIONS);
  target_15 = false;
  target_14 = false;
  target_assert = false;
}

void
compiler::set_wall (warning_state val)
{
  assert (state == SETTING_OPTIONS);
  set_warning (WARN_CANONICAL_MODIFIER_ORDER, val);
  set_warning (WARN_DEPRECATED, val);
  set_warning (WARN_JAVA_LANG_IMPORT, val);
  set_warning (WARN_UNUSED_IMPORT, val);
  set_warning (WARN_NON_STATIC_CONTEXT, val);
  set_warning (WARN_DOLLAR_IDENTIFIER, val);
  set_warning (WARN_CONSTRUCTOR_NAME, val);
  set_warning (WARN_FINALIZE_SPELLING, val);
  set_warning (WARN_ASSIGNMENT_NO_EFFECT, val);
  set_warning (WARN_STRING_PLUS_CHAR_ARRAY, val);
  set_warning (WARN_UNNEEDED_INSTANCEOF, val);
  set_warning (WARN_UNUSED_PRIVATE_MEMBERS, val);
  set_warning (WARN_BAD_SERIALIZATION_FIELD, val);
  set_warning (WARN_UNUSED_LABEL, val);
  set_warning (WARN_UNNEEDED_EXTENDS, val);
  set_warning (WARN_ARRAY_SIZE, val);
  set_warning (WARN_ENCLOSING_ACCESS, val);
  // zero-shift and negative-shift aren't in -Wall since real code
  // uses these.  zero-shift is used to make text line up.  negative
  // shifts are used to implement rotates.
  set_warning (WARN_LARGE_SHIFT, val);
  set_warning (WARN_SWITCH, val);
  set_warning (WARN_SWITCH_DEFAULT, val);
  set_warning (WARN_UNUSED_VARIABLE, val);
  set_warning (WARN_UNUSED_PARAMETER, val);
}



void
compiler::add_job (const work_item &job)
{
  if (multi_threaded)
    {
      concurrence::exclusive_mutex::lock_sentinel sync (work_monitor);
      work_list.push_back (job);
      work_condition.signal ();
    }
  else
    dispatch_job (job);
}

bool
compiler::dispatch_job (const work_item &job)
{
  bool result = false;
  switch (job.kind)
    {
    case PARSE:
      do_load_source_file (job.filename);
      break;
    case ANALYZE:
      do_analyze_unit (job.unit);
      break;
    case GENERATE_CODE:
      do_generate_code (job.unit);
      break;
    case PAUSE:
      {
	{
	  concurrence::exclusive_mutex::lock_sentinel hold (pause_monitor);
	  pause_condition.signal ();
	}
	// Now wait on the pause mutex.  Since we release it as soon
	// as we acquire it, this lets all the workers proceed.
	concurrence::exclusive_mutex::lock_sentinel hold (pause_waiter);
      }
      break;
    case DIE:
      // We're done.
      result = true;
      break;
    }
  return result;
}

void
compiler::work ()
{
  {
    concurrence::exclusive_mutex::lock_sentinel sync (mt_monitor);
    ++multi_threaded;
  }

  global->set_compiler (this);
  try
    {
      while (true)
	{
	  work_item job;

	  {
	    concurrence::exclusive_mutex::lock_sentinel sync (work_monitor);
	    while (work_list.empty ())
	      work_condition.wait ();
	    // Pop the work item while we still hold the mutex.
	    job = work_list.front ();
	    work_list.pop_front ();
	  }

	  if (dispatch_job (job))
	    break;
	}
    }
  catch (...)
    {
      concurrence::exclusive_mutex::lock_sentinel sync (mt_monitor);
      --multi_threaded;
      mt_condition.signal ();

      throw;
    }

  concurrence::exclusive_mutex::lock_sentinel sync (mt_monitor);
  --multi_threaded;
  mt_condition.signal ();
}

void
compiler::pause_workers ()
{
  if (! multi_threaded)
    return;

  // Acquire the lock used for pausing.
  concurrence::exclusive_mutex::lock_sentinel outer (pause_waiter);

  // Acquire the lock that controls the counter.
  concurrence::exclusive_mutex::lock_sentinel inner (pause_monitor);

  // Tell each thread to pause.
  for (int i = 0; i < multi_threaded; ++i)
    {
      work_item job (PAUSE);
      add_job (job);
    }

  // Wait for each thread to report back.
  for (int i = 0; i < multi_threaded; ++i)
    pause_condition.wait ();
}

void
compiler::do_load_source_file (const std::string &filename)
{
  if (! factory->load_source_file (filename))
    {
      std::cerr << get_name () << ": couldn't find file "
		<< filename << std::endl;
      set_failed ();
    }
}

void
compiler::do_generate_code (model_unit *unit)
{
  std::list<ref_class> types = unit->get_types ();
  for (std::list<ref_class>::const_iterator j = types.begin ();
       j != types.end ();
       ++j)
    {
      // Generate code with each requested code generator.  If any
      // code generator throws an error for this class, we just skip
      // this class and its children.  It isn't clear that this it the
      // best approach.
      try
	{
	  for (std::list<code_generator *>::const_iterator k
		 = back_ends.begin ();
	       k != back_ends.end ();
	       ++k)
	    (*k)->generate ((*j).get ());

	  (*j)->add_classes_to_list (types);
	}
      catch (exception_base &exc)
	{
	  std::cerr << exc;
	  ok = false;
	}

      // See if we can release some memory.
      (*j)->clean_up ();
    }
}

void
compiler::do_analyze_unit (model_unit *unit)
{
  if (verbose ())
    std::cout << " [analyzing " << unit->get_file_name () << "]"
	      << std::endl;

  // fixme is this inefficient?  look up the idiom
  std::list<ref_class> types = unit->get_types ();
  for (std::list<ref_class>::const_iterator j = types.begin ();
       ok && j != types.end ();
       ++j)
    {
      try
	{
	  if (verbose ())
	    std::cout << "  class " << (*j)->get_name () << std::endl;
	  (*j)->resolve_classes ();
	  // fixme this is really sort of bogus
	  resolution_scope scope;
	  model_unit *cu = (*j)->get_compilation_unit ();
	  if (cu)
	    cu->push_on_scope (&scope);
	  (*j)->resolve (&scope);
	}
      catch (exception_base &exc)
	{
          exc.set_lexer (unit->get_lexer ());
	  std::cerr << exc;
	  ok = false;
	}
    }

  unit->check_imports ();

  // Dump the AST to stdout now if it was requested.
  if (dump_tree ())
    ::dump_tree (unit);
}
