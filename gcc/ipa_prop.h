/* Interprocedural constant propagation
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Razya Ladelsky <RAZYA@il.ibm.com>

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
#include "real.h"
/* The following definitions are used by 
   IPA Constant Propagation algorithm.  */
typedef struct tree_int_cst_lowhi ipcp_int;
enum Jfunc_type {
            UNKNOWN_IPATYPE,
            CONST_IPATYPE_INT,
            CONST_IPATYPE_FLOAT,
            CONST_IPATYPE_INT_REF,
	    CONST_IPATYPE_FLOAT_REF,
            FORMAL_IPATYPE
};
enum Cvalue_type {
            BOTTOM,
            CONST_VALUE_INT,
            CONST_VALUE_FLOAT,
	    CONST_VALUE_INT_REF,
	    CONST_VALUE_FLOAT_REF,
            TOP
};
union info{
  ipcp_int int_value;
  unsigned int formal_id;
  REAL_VALUE_TYPE float_value;
}; 
struct ipa_jump_func
{
  enum Jfunc_type type; 
  union info info_type;
};
struct ipcp_formal
{ 
  enum Cvalue_type cvalue_type;
  union info cvalue;
}; 
struct ipa_tree_map
{
  tree param_tree;
};
struct ipa_modify
{
  bool mod;
};
struct ipa_replace_map
{
  tree old_tree;
  tree new_tree;
  bool replace_p;
  bool ref_p;
};
struct ipa_node
{
  /* Number of formal parameters of this method.  When set to 0,
   this method's parameters would not be analyzed by the different 
   stages of IPA CP.  */
  int ipa_arg_num;
  /* Array of cvals.  */
  struct ipcp_formal* GTY ((skip (""))) ipcp_cval;
  /* Mapping each parameter to its PARM_DECL tree.  */
  struct ipa_tree_map* GTY ((skip (""))) ipa_param_tree;
  /* Indicating which parameter is modified in its method.  */
  struct ipa_modify* GTY ((skip (""))) ipa_mod;
   /* Only for cloned nodes this field would not be NULL, 
  it points to the node that IPA cp cloned from.  */ 
  struct cgraph_node *ipcp_orig_node;
  gcov_type count_scale;
};

struct ipa_edge
{
  /* Number of actual arguments in this callsite.  When set to 0,
   this callsite's parameters would not be analyzed by the different 
   stages of IPA CP.  */
  int ipa_param_num;
  /* Array of the callsite's jump function of each parameter.  */
  struct ipa_jump_func* GTY ((skip (""))) ipcp_param_map; 
};
void ipcp_driver (void);

#endif /* IPA_PROP_H */
