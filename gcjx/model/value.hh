// Values for constant expressions.

// Copyright (C) 2004 Free Software Foundation, Inc.
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

#ifndef GCJX_MODEL_VALUE_HH
#define GCJX_MODEL_VALUE_HH

union jvalue
{
  jboolean z;
  jbyte    b;
  jchar    c;
  jshort   s;
  jint     i;
  jlong    j;
  jfloat   f;
  jdouble  d;

  jvalue (jboolean val) { z = val; }
  jvalue (jbyte val) { b = val; }
  jvalue (jchar val) { c = val; }
  jvalue (jshort val) { s = val; }
  jvalue (jint val) { i = val; }
  jvalue (jlong val) { j = val; }
  jvalue (jfloat val) { f = val; }
  jvalue (jdouble val) { d = val; }

  operator jboolean () const { return z; }
  operator jbyte () const { return b; }
  operator jchar () const { return c; }
  operator jshort () const { return s; }
  operator jint () const { return i; }
  operator jlong () const { return j; }
  operator jfloat () const { return f; }
  operator jdouble () const { return d; }
};

extern std::string primitive_format (const jboolean &val);
extern std::string primitive_format (const jbyte &c);
extern std::string primitive_format (const jchar &c);
extern std::string primitive_format (const jshort &c);
extern std::string primitive_format (const jint &c);
extern std::string primitive_format (const jlong &val);
extern std::string primitive_format (const jfloat &val);
extern std::string primitive_format (const jdouble &val);

template<typename TO, typename FROM>
TO
convert_value (const jvalue &val)
{
  return (TO) (FROM) val;
}

// FIXME: look up c++ rules, can we specialize convert_value?
template<typename TO, typename FROM>
TO
convert_from_float (const jvalue &val, long long min, long long max)
{
  FROM fval = (FROM) val;
  TO ret;
  if (fval >= (FROM) max)
    ret = max;
  else if (fval <= (FROM) min)
    ret = min;
  else if (fval != fval)
    ret = 0;
  else
    ret = (TO) fval;
  return ret;
}

inline jfloat
word_to_float (jint w)
{
  union
  {
    jint word;
    jfloat f;
  } val;

  val.word = w;
  return val.f;
}

inline jint
float_to_word (jfloat f)
{
  union
  {
    jint word;
    jfloat f;
  } val;

  val.f = f;
  return val.word;
}

inline void
double_to_words (jint words[2], jdouble d)
{
  union
  {
    jint words[2];
    jdouble d;
  } val;

  val.d = d;
  words[0] = val.words[0];
  words[1] = val.words[1];
  maybe_swap (words[0], words[1]);
}

inline jdouble
words_to_double (jint hi, jint lo)
{
  union
  {
    jint words[2];
    jdouble d;
  } val;

  val.words[0] = hi;
  val.words[1] = lo;
  maybe_swap (val.words[0], val.words[1]);
  return val.d;
}

inline jlong
words_to_long (jint hi, jint lo)
{
  union
  {
    jint words[2];
    jlong l;
  } val;

  val.words[0] = hi;
  val.words[1] = lo;
  maybe_swap (val.words[0], val.words[1]);
  return val.l;
}

inline void
long_to_words (jint words[2], jlong l)
{
  union
  {
    jint words[2];
    jlong l;
  } val;

  val.l = l;
  words[0] = val.words[0];
  words[1] = val.words[1];
  maybe_swap (words[0], words[1]);
}

/// Convert a string to a double according to Java rules.
/// The string must already be well-formed.  Returns false on
/// overflow or underflow; in this case the result will be Inf on
/// overflow and 0 on underflow.
bool string_to_double (const char *, jdouble &);

/// Convert a string to a float according to Java rules.
/// The string must already be well-formed.  Returns false on
/// overflow or underflow; in this case the result will be Inf on
/// overflow and 0 on underflow.
bool string_to_float (const char *, jfloat &);

#endif // GCJX_MODEL_VALUE_HH
