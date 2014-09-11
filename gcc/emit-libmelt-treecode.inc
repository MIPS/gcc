#if 0
/** file emit-libmelt-treecode.inc to be preprocessed by cpp
 *   inside MELT ; see http://gcc-melt.org/ for more
 *
 *   Copyright (C) 2014 Free Software Foundation, Inc.
 *   Contributed by Basile Starynkevitch <basile@starynkevitch.net>
 *
 * This file contrib/emit-libmelt-treecode.inc is part of GCC.
 * It generates the MELT file _libmelt-treecode.melt
 * 
 * GCC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GCC; see the file COPYING3.   If not see
 * <http://www.gnu.org/licenses/>.
 *
 * This file process the tree.def header file to transform it into
 * some MELT file.
 *
 **/
#endif
#define DEFTREECODE(A,B,C,D) (gcc_tree_code A B C D)
#include "tree.def"
