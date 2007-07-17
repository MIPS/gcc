/* Pluggable tree transforms
   Copyright 2005 Free Software Foundation, Inc.
   Contributed by Sean Callanan <sean@fsl.cs.sunysb.edu>

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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef GCC_TREE_PLUGIN_H
#define GCC_TREE_PLUGIN_H

/* Pass declaration. */

extern struct tree_opt_pass pass_plugin;

/* Function prototypes.  */

void register_tree_plugin(const char* path);

void pre_tu_plugins(void);
void post_tu_plugins(void);

/* Interface to retrieve plugin arguments. */

struct plugin_argument {
  char* key;
  char* value;
};

int get_plugin_arguments(int*, struct plugin_argument**);

#endif /* GCC_TREE_PLUGIN_H */
