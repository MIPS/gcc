/*
   Copyright (C) 2006-2007 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!

   Authors:
     Ricardo Fernandez Pascual <ricardof@um.es>
     Andrea C. Ornstein <andrea.ornstein@st.com>

   Contact information at STMicroelectronics:
     Andrea C. Ornstein <andrea.ornstein@st.com>
     Erven Rohou        <erven.rohou@st.com>
*/

int *
gcc4net_StartupHelper_GetArgv_System_Int32__System_IntPtr (int *out_argc)
{
  extern int __gcc4net_argc;
  extern int *__gcc4net_argv;
  *out_argc = __gcc4net_argc;
  return __gcc4net_argv;
}

void
gcc4net_StartupHelper_Startup_System_Void (void)
{
  /* TODO */
}

int
gcc4net_Crt___isLittleEndian_System_Boolean (void)
{
  // big endian:    3f f0 00 00 00 00 00 00
  // little endian: 00 00 00 00 00 00 f0 3f
  double d = 1.0;
  char *b = (char *) &d;
  return b[0] == 0;
}
