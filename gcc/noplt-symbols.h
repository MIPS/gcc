/* No-plt symbol class for the GNU compiler.
   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef NOPLT_SYMBOLS_H
#define NOPLT_SYMBOLS_H

/* Hasher for "const char *" strings, using string rather than pointer
   equality.  */

struct string_hash : pointer_hash <const char>
{
  static inline hashval_t hash (const char *);
  static inline bool equal (const char *, const char *);
};

inline hashval_t
string_hash::hash (const char *id)
{
  return htab_hash_string (id);
}

inline bool
string_hash::equal (const char *id1, const char *id2)
{
  return strcmp (id1, id2) == 0;
}

/* No-plt symbol hash table type.  */
typedef hash_table<string_hash> noplt_symbol_table_type;

extern void noplt_symbols_initialize (void);
extern void noplt_symbols_finish (void);
extern bool noplt_symbol_p (const char *);
extern bool noplt_decl_p (tree);

#endif /* NOPLT_SYMBOLS_H */
