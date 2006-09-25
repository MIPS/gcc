/* Run-time support required by CIL binaries.

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Roberto Costa <roberto.costa@st.com>   */

using System;
using System.Reflection;
using System.Runtime.InteropServices;

namespace gcc4net {
    public unsafe sealed class StartupHelper {

        public unsafe static IntPtr GetArgv(out int argc) {
            String[] argv = Environment.GetCommandLineArgs();
            argc = argv.Length;
            int ptr_size = (int)(sizeof(void*));
            int size = (argc + 1) * ptr_size;
            IntPtr res = Marshal.AllocHGlobal(size);

            for (int i=0; i < argc; ++i) {
                Marshal.WriteIntPtr(res, ptr_size * i,
                                    Marshal.StringToHGlobalAnsi(argv[i]));
            }
            Marshal.WriteIntPtr(res, ptr_size * argc, IntPtr.Zero);

            return res;
        }

	public unsafe static void Startup() {
            Assembly assembly;
            Type type;

            // Find the module that contains the "main" function.
            assembly = Assembly.GetEntryAssembly();
            type = assembly.GetType("<Module>");
            // Invoke the application's ".init" function, if present.
            MethodInfo initMethod = type.GetMethod(".init");
            if(initMethod != null)
                initMethod.Invoke(null, null);
        }

	public unsafe static void Shutdown(int status) {
        }

    }

    public sealed class Crt {
        public static int    __minsi3(int a, int b)        { return (a<=b) ? a : b; }
        public static long   __minti3(long a, long b)      { return (a<=b) ? a : b; }
        public static uint   __uminsi3(uint a, uint b)     { return (a<=b) ? a : b; }
        public static ulong  __uminti3(ulong a, ulong b)   { return (a<=b) ? a : b; }
        public static float  __minsf3(float a, float b)    { return (a<=b) ? a : b; }
        public static double __mindf3(double a, double b)  { return (a<=b) ? a : b; }

        public static int    __maxsi3(int a, int b)        { return (a>=b) ? a : b; }
        public static long   __maxti3(long a, long b)      { return (a>=b) ? a : b; }
        public static uint   __umaxsi3(uint a, uint b)     { return (a>=b) ? a : b; }
        public static ulong  __umaxti3(ulong a, ulong b)   { return (a>=b) ? a : b; }
        public static float  __maxsf3(float a, float b)    { return (a>=b) ? a : b; }
        public static double __maxdf3(double a, double b)  { return (a>=b) ? a : b; }

        public static int    __abssi2(int a)               { return (a>=0) ? a : -a; }
        public static long   __absti2(long a)              { return (a>=0) ? a : -a; }
        public static float  __abssf2(float a)             { return (a>=0) ? a : -a; }
        public static double __absdf2(double a)            { return (a>=0) ? a : -a; }
    }
}
