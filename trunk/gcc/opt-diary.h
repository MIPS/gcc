/* Optimization Diary.
   Copyright (C) 2006 Free Software Foundation, Inc.

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

#ifndef GCC_OPT_DIARY_H
#define GCC_OPT_DIARY_H


enum opt_diary_strings {
  OD_LOOP_VECTORIZED,
  OD_LOOP_NOT_VECTORIZED
};

struct od_entry_tag_proposal_1
{
  /* Source file name */
  const char *file;

  unsigned first_line;
  unsigned last_line;

  /* Compiler phase name */
  const char *pname;

  /* Funciton name */
  const char *fname;
  
  /* Status flag */
  int flag;

};

extern FILE *opt_diary_file;

extern void open_opt_diary (void);
extern void close_opt_diary (void);
#endif 
