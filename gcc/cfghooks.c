/* Hooks for cfg representation specific functions.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "basic-block.h"
#include "tree-flow.h"


/* Containers.  */
struct cfg_hooks tree_cfg_hooks = {
  tree_split_edge,
  tree_verify_flow_info,
  NULL
};

struct cfg_hooks rtl_cfg_hooks = {
  rtl_split_edge,
  rtl_verify_flow_info,
  NULL
};

/* A pointer to one of the hooks containers.  */
struct cfg_hooks *cfg_hooks;

/* A global variable that keeps track of the state of the cfg.  */
enum cfg_level cfg_level;


/* Initialization of functions specific to the tree IR.  */

void 
tree_register_cfg_hooks ()
{
  cfg_level = AT_TREE_LEVEL;
  cfg_hooks = &tree_cfg_hooks;
}

/* Initialization of functions specific to the rtl IR.  */

void 
rtl_register_cfg_hooks ()
{
  cfg_level = AT_RTL_LEVEL;
  cfg_hooks = &rtl_cfg_hooks;
}

