/* Callgraph handling code.
   Copyright (C) 2003, 2004 Free Software Foundation, Inc.
   Contributed by Razya Ladelsky.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef IPA_PROP_H
#define IPA_PROP_H

#include "tree.h"
/* The following definitions are used by 
   IPA Constant Propagation algorithm.  */
typedef struct tree_int_cst_lowhi ipcp_int;
enum Jfunc_type {
            UNKNOWN_IPATYPE,
            CONST_IPATYPE,
            FORMAL_IPATYPE
};
enum Cvalue_type {
            BOTTOM,
            CONST_VALUE,
            TOP
};
struct ipcp_jump_func
{
  enum Jfunc_type type; 
  ipcp_int info_type;
};
struct ipcp_formal
{ 
  enum Cvalue_type cvalue_type;
  ipcp_int cvalue;
}; 
struct ipcp_tree_map
{
  tree param_tree;
};
struct ipcp_modify
{
  bool mod;
};

struct ipa_node
{
  /* Array of cvals.  */
  struct ipcp_formal* GTY ((skip (""))) ipcp_cval;
  int ipcp_arg_num;
  /* Mapping each parameter to its PARM_DECL tree.  */
  struct ipcp_tree_map* GTY ((skip (""))) ipcp_param_tree;
  /* Indicating which parameter is modified in its method.  */
  struct ipcp_modify* GTY ((skip (""))) ipcp_mod;
};

struct ipa_edge
{
  struct ipcp_jump_func* GTY ((skip (""))) ipcp_param_map;
  int ipcp_param_num;
};
void ipcp_driver (void);

#endif /* IPA_PROP_H */
