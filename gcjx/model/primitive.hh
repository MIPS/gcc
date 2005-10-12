// Represent primitive types.

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

#ifndef GCJX_MODEL_PRIMITIVE_HH
#define GCJX_MODEL_PRIMITIVE_HH

#include "model/type.hh"

// These have to be declared model_type for the templates in
// literal.hh to work properly.
extern model_type *primitive_boolean_type;
extern model_type *primitive_byte_type;
extern model_type *primitive_char_type;
extern model_type *primitive_short_type;
extern model_type *primitive_int_type;
extern model_type *primitive_long_type;
extern model_type *primitive_float_type;
extern model_type *primitive_double_type;

class model_primitive_base;

/// Some sort of binary operator.  This takes an element argument in
/// case of error; it is the requesting element.
typedef jvalue (model_primitive_base::*binary_function) (model_element *,
							 const jvalue &,
							 const jvalue &);

typedef bool (model_primitive_base::*comparator) (const jvalue &,
						  const jvalue &);

typedef jvalue (model_primitive_base::*unary_function) (const jvalue &);

typedef bool (model_type::*predicate) () const;

class model_primitive_base : public model_type
{
  // Pretty name of this type.
  const char *pretty_name;

protected:

  model_primitive_base (char sig_char, const char *pname)
    : model_type (LOCATION_UNKNOWN),
      pretty_name (pname)
  {
    descriptor = std::string (1, sig_char);
  }

public:

  bool primitive_p () const
  {
    return true;
  }

  bool assignable_from_p (model_type *other);

  std::string get_pretty_name () const
  {
    return std::string (pretty_name);
  }

  // Many operations are invalid on some types.  The other parts of
  // the compiler should never call these methods in these cases, so
  // we just abort if they are called.  This is a bit simpler than
  // having pure virtual functions that we must then override with
  // aborts in all places.

  virtual jvalue subtract (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue multiply (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue divide (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue mod (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue band (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue bor (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue bxor (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue plus (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual bool equal (const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual bool not_equal (const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual bool less_than (const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual bool greater_than (const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual bool less_than_equal (const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual bool greater_than_equal (const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue plus (const jvalue &)
  {
    abort ();
  }

  virtual jvalue minus (const jvalue &)
  {
    abort ();
  }

  virtual jvalue bnot (const jvalue &)
  {
    abort ();
  }

  virtual jvalue lnot (const jvalue &)
  {
    abort ();
  }

  virtual jvalue left_shift (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue right_shift (model_element *, const jvalue &, const jvalue &)
  {
    abort ();
  }

  virtual jvalue unsigned_right_shift (model_element *, const jvalue &,
				       const jvalue &)
  {
    abort ();
  }

  virtual jvalue convert (const model_type *from_type,
			  const jvalue &from_val)
  {
    abort ();
  }

  /// Return true if the integer value is representable in this type.
  virtual bool representable_p (int)
  {
    abort ();
  }

  void visit (visitor *) = 0;
};

class model_primitive_boolean : public model_primitive_base
{
public:

  model_primitive_boolean ()
    : model_primitive_base ('Z', "boolean")
  {
  }

  std::string to_string (const jvalue &val) const
  {
    return primitive_format ((jboolean) val);
  }

  bool boolean_p () const
  {
    return true;
  }

  jvalue band (model_element *, const jvalue &l, const jvalue &r)
  {
    return jboolean (l) && jboolean (r);
  }

  jvalue bor (model_element *, const jvalue &l, const jvalue &r)
  {
    return jboolean (l) || jboolean (r);
  }

  jvalue bxor (model_element *, const jvalue &l, const jvalue &r)
  {
    return jboolean (l) != jboolean (r);
  }

  bool equal (const jvalue &l, const jvalue &r)
  {
    return (jboolean) l == (jboolean) r;
  }

  bool not_equal (const jvalue &l, const jvalue &r)
  {
    return (jboolean) l != (jboolean) r;
  }

  jvalue lnot (const jvalue &v)
  {
    return ! (jboolean) v;
  }

  virtual jvalue convert (const model_type *from_type,
			  const jvalue &from_val)
  {
    assert (from_type == primitive_boolean_type);
    return from_val;
  }

  void visit (visitor *);
};

template<typename T, char sig_char>
class model_primitive : public model_primitive_base
{
protected:

  model_primitive (const char *pname)
    : model_primitive_base (sig_char, pname)
  {
  }

public:

  std::string to_string (const jvalue &val) const
  {
    return primitive_format ((T) val);
  }

  bool numeric_p () const
  {
    return true;
  }

  jvalue subtract (model_element *, const jvalue &l, const jvalue &r)
  {
    return (T) l - (T) r;
  }

  jvalue multiply (model_element *, const jvalue &l, const jvalue &r)
  {
    return (T) l * (T) r;
  }

  jvalue divide (model_element *, const jvalue &l, const jvalue &r)
  {
    // FIXME: special rules?
    return (T) l / (T) r;
  }

  jvalue plus (model_element *, const jvalue &l, const jvalue &r)
  {
    return (T) l + (T) r;
  }

  bool equal (const jvalue &l, const jvalue &r)
  {
    return (T) l == (T) r;
  }

  bool not_equal (const jvalue &l, const jvalue &r)
  {
    return (T) l != (T) r;
  }

  bool less_than (const jvalue &l, const jvalue &r)
  {
    return (T) l < (T) r;
  }

  bool greater_than (const jvalue &l, const jvalue &r)
  {
    return (T) l > (T) r;
  }

  bool less_than_equal (const jvalue &l, const jvalue &r)
  {
    return (T) l <= (T) r;
  }

  bool greater_than_equal (const jvalue &l, const jvalue &r)
  {
    return (T) l >= (T) r;
  }

  virtual jvalue plus (const jvalue &v)
  {
    return v;
  }

  virtual jvalue minus (const jvalue &v)
  {
    return - (T) v;
  }

  virtual jvalue convert (const model_type *from_type,
			  const jvalue &from_val)
  {
    // We allow conversions from boolean to int here, as a special
    // case; at some places we explicitly do this.
    if (from_type == primitive_boolean_type)
      {
	assert (this == primitive_int_type);
	return convert_value<T, jboolean> (from_val);
      }
    if (from_type == primitive_byte_type)
      return convert_value<T, jbyte> (from_val);
    if (from_type == primitive_char_type)
      return convert_value<T, jchar> (from_val);
    if (from_type == primitive_short_type)
      return convert_value<T, jshort> (from_val);
    if (from_type == primitive_int_type)
      return convert_value<T, jint> (from_val);
    if (from_type == primitive_long_type)
      return convert_value<T, jlong> (from_val);
    assert (! integral_p ());
    if (from_type == primitive_float_type)
      return convert_value<T, jfloat> (from_val);
    if (from_type == primitive_double_type)
      return convert_value<T, jdouble> (from_val);
    abort ();
  }

  void visit (visitor *) = 0;
};

template<typename T, char sig_char, long long MIN, long long MAX>
class model_int_primitive : public model_primitive<T, sig_char>
{
public:

  model_int_primitive (const char *pname)
    : model_primitive<T, sig_char> (pname)
  {
  }

  bool integral_p () const
  {
    return true;
  }

  jvalue divide (model_element *, const jvalue &, const jvalue &);

  jvalue mod (model_element *request, const jvalue &l, const jvalue &r)
  {
    assert ((T) r != 0);
    if ((T) l == MIN && (T) r == -1)
      return (T) 0;
    return (T) l % (T) r;
  }

  jvalue band (model_element *, const jvalue &l, const jvalue &r)
  {
    return (T) l & (T) r;
  }

  jvalue bor (model_element *, const jvalue &l, const jvalue &r)
  {
    return (T) l | (T) r;
  }

  jvalue bxor (model_element *, const jvalue &l, const jvalue &r)
  {
    return (T) l ^ (T) r;
  }

  jvalue bnot (const jvalue &v)
  {
    return ~ (T) v;
  }

  jvalue left_shift (model_element *, const jvalue &l, const jvalue &r)
  {
    // Only 'long' and 'int' shifts are supported.
    assert (sig_char == 'I' || sig_char == 'J');
    T lval = (T) l;
    T rval = (T) r;
    // This is a little ugly -- there should be a way to compute the
    // mask.
    if (sig_char == 'I')
      rval &= 0x1f;
    else
      rval &= 0x3f;
    return (T) (lval << rval);
  }

  jvalue right_shift (model_element *, const jvalue &l, const jvalue &r)
  {
    // Only 'long' and 'int' shifts are supported.
    assert (sig_char == 'I' || sig_char == 'J');
    T lval = (T) l;
    T rval = (T) r;
    // This is a little ugly -- there should be a way to compute the
    // mask.
    if (sig_char == 'I')
      rval &= 0x1f;
    else
      rval &= 0x3f;
    return (T) (lval >> rval);
  }

 jvalue unsigned_right_shift (model_element *, const jvalue &l,
			      const jvalue &r)
  {
    // Only 'long' and 'int' shifts are supported.
    assert (sig_char == 'I' || sig_char == 'J');
    unsigned long lval = (unsigned long) (T) l;
    unsigned long rval = (unsigned long) (T) r;
    // This is a little ugly -- there should be a way to compute the
    // mask.
    if (sig_char == 'I')
      rval &= 0x1f;
    else
      rval &= 0x3f;
    return (T) (lval >> rval);
  }

  jvalue convert (const model_type *from_type, const jvalue &from_val)
  {
    if (from_type == primitive_float_type
	|| from_type == primitive_double_type)
      {
	if (this != primitive_int_type && this != primitive_long_type)
	  {
	    // Must use an intermediate cast.
	    model_primitive_base *mpb
	      = assert_cast<model_primitive_base *> (primitive_int_type);
	    jvalue inter = mpb->convert (from_type, from_val);
	    return convert (mpb, inter);
	  }

	if (from_type == primitive_float_type)
	  return convert_from_float<T, jfloat> (from_val, MIN, MAX);
	else
	  return convert_from_float<T, jdouble> (from_val, MIN, MAX);
      }

    return model_primitive<T, sig_char>::convert (from_type, from_val);
  }

  // This is out-of-line so we can specialize for jlong.
  bool representable_p (int val);

  void visit (visitor *v);
};

template<typename T, char sig_char, long long MIN, long long MAX>
inline bool
model_int_primitive<T,sig_char,MIN,MAX>::representable_p (int val)
{
  return val >= MIN && val <= MAX;
}

// There is no narrowing from 'int' to itself, so this case represents
// a programming error elsewhere in the compiler.
template<>
inline bool
model_int_primitive<jint, 'I', -0x80000000LL, 0x7fffffffLL>::representable_p (int val)
{
  abort ();
}

// There is no narrowing conversion from 'long', so this case
// represents a programming error elsewhere in the compiler.
template<>
inline bool
model_int_primitive<jlong,'J',-0x8000000000000000LL,0x7fffffffffffffffLL>::representable_p (int val)
{
  abort ();
}

template<typename T, char sig_char>
class model_fp_primitive : public model_primitive<T, sig_char>
{
public:

  model_fp_primitive (const char *pname)
    : model_primitive<T, sig_char> (pname)
  {
  }

  jvalue mod (model_element *, const jvalue &l, const jvalue &r)
  {
    T lv = (T) l;
    T rv = (T) r;

    // FIXME: this may be the wrong operator (do we want frem?), and
    // in any case we must handle things like strictfp -- probably
    // need fdlibm.

    return T (fmod (lv, rv));
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_PRIMITIVE_HH
