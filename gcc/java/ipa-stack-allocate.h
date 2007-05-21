/* Headers for 
   Copyright (C) 1998, 2000, 2003 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

void init_gcj_stack_allocate (void);
void init_gcj_devirt (void);
void init_gcj_offline_devirt (void);
void init_gcj_invariant (void);
void init_gcj_necessary (void);
void init_offline_devirt (void);


#define SUBMETHOD_FILE "/tmp/ea_symlinks/submethods"

void devirt (void);
void gcj_invariants (void);
void register_dump_files (struct tree_opt_pass*, bool, int);

// different types of escape analysis
extern bool ea_flow_sensitive;
extern bool ea_interprocedural;
extern bool ea_field_sensitive;

// we overload the flag, what do we really mean
extern bool ea_stack_allocate;
extern bool ea_instrument;
extern bool ea_necessary;
extern bool ea_offline_devirt;

tree LHS (tree stmt);
tree RHS (tree stmt);

void xprintf (const char* format_string, ...);
bool is_global (tree);

void setup_flag_ipa_stack_allocate (void);

struct _submethod_list
{
  tree method_name; // identifier node
  tree class_name; // identifier
  VEC(tree, heap) *submethod_names;
};

typedef struct _submethod_list* submethod_list;

submethod_list get_submethod_list (tree method_name, tree class_name);

