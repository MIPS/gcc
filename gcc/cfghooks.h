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

#ifndef GCC_CFGHOOKS_H
#define GCC_CFGHOOKS_H

struct cfg_hooks
{
  basic_block (*cfgh_split_edge)        PARAMS ((edge));
  void (*cfgh_verify_flow_info)	        PARAMS ((void));
  basic_block (*cfgh_make_forwarder_block) PARAMS ((basic_block, int, int, 
						    edge, int));

};

#define split_edge(e)                        cfg_hooks->cfgh_split_edge (e)
#define verify_flow_info()                   cfg_hooks->cfgh_verify_flow_info ()
#define make_forwarder_block(a, b, c, d, e)  cfg_hooks->cfgh_make_forwarder_block (a, b, c, d, e)

/* Hooks containers.  */
extern struct cfg_hooks tree_cfg_hooks;
extern struct cfg_hooks rtl_cfg_hooks;

/* A pointer to one of the hooks containers.  */
extern struct cfg_hooks *cfg_hooks;

enum cfg_level {
  AT_TREE_LEVEL,
  AT_RTL_LEVEL
};

/* A global variable that keeps track of the state of the cfg.  */
extern enum cfg_level cfg_level;

/* Declarations.  */
extern void tree_register_cfg_hooks    PARAMS ((void));
extern void rtl_register_cfg_hooks     PARAMS ((void));

#endif  /* GCC_CFGHOOKS_H */
