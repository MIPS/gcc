/* Export of alias information to RTL.  
   Copyright (C) 2009 Free Software Foundation, Inc.

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

#ifndef GCC_ALIAS_EXPORT_H
#define GCC_ALIAS_EXPORT_H

#include "tree-ssa-alias.h"

extern tree unshare_and_record_pta_info (tree);
extern void record_escaped_solution (struct pt_solution *);
extern void record_stack_var_partition_for (tree, tree);

extern bool alias_export_may_alias_p (const_rtx, const_rtx, tree, tree);
extern bool ddg_export_may_alias_p (tree, tree, bool);
extern void replace_var_in_datarefs (tree, tree);
extern void remove_exported_ddg_data (rtx);
extern void ddg_export_set_pipelining_completed (bool);

extern void free_alias_export_info (void);
extern void free_ddg_export_info (void);

#endif /* GCC_ALIAS_EXPORT_H */
