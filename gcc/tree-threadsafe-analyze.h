/* Header file for thread safety analysis.
   Copyright (C) 2007, 2008 Free Software Foundation, Inc.
   Contributed by Le-Chun Wu <lcwu@google.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */


#ifndef TREE_THREADSAFE_ANALYZE_H
#define TREE_THREADSAFE_ANALYZE_H

/* Maximum length (in bytes) of a lock name string */
#define LOCK_NAME_LEN 64

/* Defined in tree-threadsafe-analyze.c */
extern void clean_up_threadsafe_analysis (void);
extern struct pointer_map_t *lock_acquired_after_map;
extern struct pointer_map_t *unbound_lock_map;
extern const char* dump_expr_tree (tree lock, char *out_buf);

#endif /* TREE_THREADSAFE_ANALYZE_H */
