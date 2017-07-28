/* JSON parsing
   Copyright (C) 2017 Free Software Foundation, Inc.

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

#ifndef GCC_JSON_H
#define GCC_JSON_H

/* Implementation of JSON, a lightweight data-interchange format.

   See http://www.json.org/
   and http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf
   and https://tools.ietf.org/html/rfc7159

   Supports parsing text into a DOM-like tree of json::value *, dumping
   json::value * to text.  */

namespace json
{

/* Forward decls of json::value and its subclasses (using indentation
   to denote inheritance.  */

class value;
  class object;
  class array;
  class number;
  class string;
  class literal;

/* An enum for discriminating the subclasses of json::value.  */

enum kind
{
  /* class json::object.  */
  JSON_OBJECT,

  /* class json::array.  */
  JSON_ARRAY,

  /* class json::number.  */
  JSON_NUMBER,

  /* class json::string.  */
  JSON_STRING,

  /* class json::literal uses these three values to identify the
     particular literal.  */
  JSON_TRUE,
  JSON_FALSE,
  JSON_NULL
};

/* Base class of JSON value.  */

class value
{
 public:
  virtual ~value () {}
  virtual enum kind get_kind () const = 0;
  virtual void print (pretty_printer *pp) const = 0;

  /* Create a deep copy of the value, returning a value which must be
     deleted by the caller.  */
  virtual value *clone () const = 0;

  char *to_str () const;
  void dump (FILE *) const;

  /* Methods for dynamically casting a value to one of the subclasses,
     returning NULL if the value is of the wrong kind.  */
  const object *as_object () const;
  const array *as_array () const;
  const number *as_number () const;
  const string *as_string () const;

  /* Convenience accessors for attempting to perform key/value lookups
     on this value as if it were an json::object.

     On success, return true and write the value to OUT_VALUE.
     On failure, return false and write an error message to OUT_ERR
     (which must be freed by the caller).  */
  bool get_value_by_key (const char *name, const value *&out_value,
			 char *&out_err) const;
  bool get_int_by_key (const char *name, int &out_value, char *&out_err) const;
  bool get_string_by_key (const char *name, const char *&out_value,
			  char *&out_err) const;
  bool get_array_by_key (const char *name, const array *&out_value,
			 char *&out_err) const;

  /* As above, but the key is optional.  THIS must still be an object,
     though.  */
  bool get_optional_value_by_key (const char *name, const value *&out_value,
				  char *&out_err) const;
  bool get_optional_string_by_key (const char *name, const char *&out_value,
				   char *&out_err) const;
};

/* Subclass of value for objects: an unordered collection of
   key/value pairs.  */

class object : public value
{
 public:
  ~object ();

  enum kind get_kind () const FINAL OVERRIDE { return JSON_OBJECT; }
  void print (pretty_printer *pp) const FINAL OVERRIDE;
  value *clone () const FINAL OVERRIDE;

  value *get (const char *key) const;
  value *get_if_nonnull (const char *key) const;

  void set (const char *key, value *v);

 private:
  typedef hash_map <char *, value *,
    simple_hashmap_traits<nofree_string_hash, value *> > map_t;
  map_t m_map;
};

/* Subclass of value for arrays.  */

class array : public value
{
 public:
  ~array ();

  enum kind get_kind () const FINAL OVERRIDE { return JSON_ARRAY; }
  void print (pretty_printer *pp) const FINAL OVERRIDE;
  value *clone () const FINAL OVERRIDE;

  unsigned get_length () const { return m_elements.length (); }
  value *get (int idx) const { return m_elements[idx]; }
  void append (value *v) { m_elements.safe_push (v); }

 private:
  auto_vec<value *> m_elements;
};

/* Subclass of value for numbers.  */

class number : public value
{
 public:
  number (double value) : m_value (value) {}

  enum kind get_kind () const FINAL OVERRIDE { return JSON_NUMBER; }
  void print (pretty_printer *pp) const FINAL OVERRIDE;
  value *clone () const FINAL OVERRIDE;

  double get () const { return m_value; }

 private:
  double m_value;
};

/* Subclass of value for strings.  */

class string : public value
{
 public:
  string (const char *utf8) : m_utf8 (xstrdup (utf8)) {}
  ~string () { free (m_utf8); }

  enum kind get_kind () const FINAL OVERRIDE { return JSON_STRING; }
  void print (pretty_printer *pp) const FINAL OVERRIDE;
  value *clone () const FINAL OVERRIDE;

  const char *get_string () const { return m_utf8; }

 private:
  char *m_utf8;
};

/* Subclass of value for the three JSON literals "true", "false",
   and "null".  */

class literal : public value
{
 public:
  literal (enum kind kind) : m_kind (kind) {}

  enum kind get_kind () const FINAL OVERRIDE { return m_kind; }
  void print (pretty_printer *pp) const FINAL OVERRIDE;
  value *clone () const FINAL OVERRIDE;

 private:
  enum kind m_kind;
};

/* Declarations for parsing JSON to a json::value * tree.  */

extern value *parse_utf8_string (size_t length, const char *utf8_buf,
				 char **err_out);
extern value *parse_utf8_string (const char *utf8, char **err_out);

} // namespace json

#endif  /* GCC_JSON_H  */
