/* Copyright (C) 2009 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This file is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* Invalid __ea declarations.  */

/* { dg-do compile } */

void func ()
{
  register __ea int local1; /* { dg-error "'__ea' combined with 'register' qualifier for 'local1'" } */
  auto __ea int local2;     /* { dg-error "'__ea' combined with 'auto' qualifier for 'local2'" } */
  __ea int local3;	    /* { dg-error "'__ea' specified for auto variable 'local3'" } */
  register int *__ea p1;    /* { dg-error "'__ea' combined with 'register' qualifier for 'p1'" } */
  auto char *__ea p2;       /* { dg-error "'__ea' combined with 'auto' qualifier for 'p2'" } */
  void *__ea p3;            /* { dg-error "'__ea' specified for auto variable 'p3'" } */
  register __ea int a1[2];  /* { dg-error "'__ea' combined with 'register' qualifier for 'a1'" } */
  auto __ea char a2[1];     /* { dg-error "'__ea' combined with 'auto' qualifier for 'a2'" } */
  __ea char a3[5];          /* { dg-error "'__ea' specified for auto variable 'a3'" } */
}

void func2 (__ea int x)	    /* { dg-error "'__ea' specified for parameter 'x'" } */
{ }

struct st {
  __ea int x;		    /* { dg-error "'__ea' specified for structure field 'x'" } */
  int *__ea q;		    /* { dg-error "'__ea' specified for structure field 'q'" } */
  int __ea b : 7;	    /* { dg-error "'__ea' specified for structure field 'b'" } */
  int __ea : 1;		    /* { dg-error "'__ea' specified for structure field" } */
} s;

struct A { int a; };

int func3 (int *__ea);	    /* { dg-error "'__ea' specified for unnamed parameter" } */
int func3 (int *__ea x)	    /* { dg-error "'__ea' specified for parameter 'x'" } */
{
  struct A i = (__ea struct A) { 1 };	/* { dg-error "compound literal qualified by address-space qualifier" } */
  return i.a;
}

extern __ea int ea_var;		/* { dg-message "note: previous.*decl" "previous.*decl" } */
int ea_var;			/* { dg-error "conflicting named address spaces" "conflicting named address spaces" } */
