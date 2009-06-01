/* High-level pass management interface for plugins.
   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

   Authors: Grigori Fursin <grigori.fursin@inria.fr>, Cupertino Miranda
   <cupertinomiranda@gmail.com>, Zbigniew Chamski <zbigniew.chamski@gmail.com>.

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

#ifndef PASS_MANAGER_H
#define PASS_MANAGER_H

/* declare the struct opt_pass type without defining it - needed to make
   the "pointer to struct opt_pass" type unambiguous withouth pulling in
   the complete "tree-pass.h" with its dependencies.  */
typedef struct opt_pass opt_pass;

extern const char **list_passes (void);
extern void register_pass (struct opt_pass *pass);
extern int unregister_pass (char *pass_name);
extern void run_pass (char *pass_name);

#endif /* PASS_MANAGER_H */
