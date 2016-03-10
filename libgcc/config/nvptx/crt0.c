/* Startup routine for standalone execution.

   Copyright (C) 2015-2016 Free Software Foundation, Inc.

   This file is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

void exit (int);
void abort (void);
void __attribute__((kernel)) __main (int *, int, char *[]);

static int *__exitval;

void
exit (int arg)
{
  *__exitval = arg;
  asm volatile ("exit;");
  __builtin_unreachable ();
}

void
abort (void)
{
  exit (255);
}

extern char *__nvptx_stacks[32] __attribute__((shared));
extern unsigned __nvptx_uni[32] __attribute__((shared));

extern int main (int argc, char *argv[]);

void __attribute__((kernel))
__main (int *__retval, int __argc, char *__argv[])
{
  __exitval = __retval;

  static char gstack[131072] __attribute__((aligned(8)));
  __nvptx_stacks[0] = gstack + sizeof gstack;
  __nvptx_uni[0] = 0;

  exit (main (__argc, __argv));
}
