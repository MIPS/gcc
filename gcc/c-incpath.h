/* Set up combined include path for the preprocessor.
   Copyright (C) 2003, 2004, 2005, 2006, 2007 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* An instance of this holds all the state for a given set of include
   chains.  This structure is intentionally opaque.  */
struct c_incpath;

extern struct c_incpath *new_c_incpath (void);
extern void delete_c_incpath (struct c_incpath *);
extern void split_quote_chain (struct c_incpath *);
extern void add_path (struct c_incpath *, char *, int, int, bool);
extern void register_include_chains (struct c_incpath *,
				     cpp_reader *, const char *,
				     const char *, const char *,
				     int, int, int);
extern void add_cpp_dir_path (struct c_incpath *, struct cpp_dir *, int);

struct target_c_incpath_s {
  /* Do extra includes processing.  STDINC is false iff -nostdinc was given.  */
  void (*extra_pre_includes) (struct c_incpath *, const char *,
			      const char *, int);
  void (*extra_includes) (struct c_incpath *, const char *, const char *, int);
};

extern struct target_c_incpath_s target_c_incpath;

enum { QUOTE = 0, BRACKET, SYSTEM, AFTER };
