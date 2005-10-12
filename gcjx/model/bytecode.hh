// Represent a block of bytecode.

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

#ifndef GCJX_MODEL_BYTECODE_HH
#define GCJX_MODEL_BYTECODE_HH

class model_unit_class;

/// This represents the bytecode for a method.  It can be attached to
/// a method as that method's body.  If we're compiling a given .class
/// to .o, put the bytecode for a method here.  If we aren't
/// compiling, just leave this empty, it is convenient to have this so
/// that error checking in model_method doesn't have to have special
/// cases.  Note that a model_bytecode_block is not attached to a
/// method when generating bytecode -- only when reading it.  However,
/// if the "verify after generating bytecode" flag is enabled, a dummy
/// model_bytecode_block will be constructed.  However, this dummy
/// object is transient and will be deleted again after verification.
/// FIXME: model_block has some state we don't need, we should have a
/// new base class.
class model_bytecode_block : public model_block
{
public:

  /// This type represents a single exception handler.
  struct exception
  {
    int handler;
    int start;
    int end;
    int type;
  };

private:

  // Information about the method.
  int max_stack;
  int max_locals;

  // Length in bytes.
  int length;

  // The bytecode.
  const uint8 *bytes;

  // Exception information.
  int exc_length;
  exception *excs;

  // True if this method has been verified.  Multiple users may want
  // to ensure that a method has been verified, but we only want to
  // verify it once.
  bool verified;

  // fixme variable and debug info

  // Flags set during the verification process.  This is allocated
  // with new[] and owned by this object.
  unsigned char *flags;

public:

  model_bytecode_block (const location &w)
    : model_block (w),
      max_stack (-1),
      max_locals (-1),
      length (-1),
      bytes (NULL),
      exc_length (-1),
      excs (NULL),
      verified (false),
      flags (NULL)
  {
  }

  ~model_bytecode_block ()
  {
    if (excs)
      delete[] excs;
    if (flags)
      delete[] flags;
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &)
  {
    normal_completion = false;
  }

  void visit (visitor *);

  /// Set the maximum number of stack slots needed by this bytecode
  /// block.
  void set_max_stack (int ms)
  {
    max_stack = ms;
  }

  /// Get the maximum number of stack slots needed by this bytecode
  /// block.
  int get_max_stack () const
  {
    return max_stack;
  }

  /// Set the maximum number of local variable slots needed by this
  /// bytecode block.
  void set_max_locals (int ml)
  {
    max_locals = ml;
  }

  /// Get the maximum number of local variable slots needed by this
  /// bytecode block.
  int get_max_locals () const
  {
    return max_locals;
  }

  /// Set the bytecode contents of this block.
  /// FIXME: ownership info.
  void set_bytecode (int len, const uint8 *p)
  {
    length = len;
    bytes = p;
  }

  /// Return the bytecode for this block.  The returned memory is
  /// owned elsewhere; the caller of this method must not free or
  /// modify it.
  const uint8 *get_bytecode () const
  {
    return bytes;
  }

  /// Return the number of bytes in the bytecode for this method.
  int get_code_length () const
  {
    return length;
  }

  /// Set the exception table used for this method.
  /// FIXME: document ownership
  void set_exceptions (int len, exception *tab)
  {
    assert (exc_length == -1 && excs == NULL);
    exc_length = len;
    excs = tab;
  }

  /// Return the length of the exception table.
  int get_exception_length () const
  {
    return exc_length;
  }

  /// Return the exception table.  The returned memory is owned
  /// elsewhere; the caller of this method must not free or modify it.
  exception *get_exceptions () const
  {
    return excs;
  }

  /// Verify the bytecode.  The first argument is the method that
  /// contains this block.  If a compilation unit is specified, it is
  /// used.  This lets us verify a method we just created.  If no
  /// compilation unit is specified, the method is assumed to have
  /// come from a .class file, and that class' compilation unit is
  /// used.
  void verify (model_method *, model_unit_class * = NULL);

  /// Get the flags for this block.
  unsigned char *get_flags () const
  {
    assert (flags);
    return flags;
  }

  void set_flags (unsigned char *f)
  {
    assert (! flags);
    flags = f;
  }
};

/// This is a phony block which is used when reading a .class file
/// for which we do not need to keep method bodies around.
class model_phony_block : public model_block
{
public:

  model_phony_block (const location &w)
    : model_block (w)
  {
  }

  void compute_normal_completion (normal_completion_state &)
  {
    normal_completion = false;
  }

  void resolve (resolution_scope *)
  {
    // Nothing.
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_BYTECODE_HH
