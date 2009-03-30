/* Run-time support required by CIL binaries.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Roberto Costa
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

using System;
using System.Collections;
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

        public unsafe static IntPtr GetEnvVars() {
            IDictionary env = Environment.GetEnvironmentVariables();
            int num_elems = env.Keys.Count;
            int ptr_size = (int)(sizeof(void*));
            int size = (num_elems + 1) * ptr_size;
            IntPtr res = Marshal.AllocHGlobal(size);

            int i = 0;
            foreach (DictionaryEntry de in env) {
                String str = de.Key + "=" + de.Value;
                Marshal.WriteIntPtr(res, ptr_size * i,
                                    Marshal.StringToHGlobalAnsi(str));
                ++i;
            }
            Marshal.WriteIntPtr(res, ptr_size * num_elems, IntPtr.Zero);

            return res;
        }

        public unsafe static void Startup() {
            Assembly assembly;
            MethodInfo initMethod = null;

            // Find the module that contains the "main" function.
            assembly = Assembly.GetEntryAssembly();

            Type type = assembly.GetType("<Module>");
            if (type != null)
                initMethod = type.GetMethod(".init");
            else {
                Module module = assembly.GetModules()[0];
                initMethod = module.GetMethod(".init");
            }

            // Invoke the application's ".init" function, if present.
            if(initMethod != null)
                initMethod.Invoke(null, null);
        }

        public unsafe static void Shutdown(int status) {
            try {
                Assembly assembly = Assembly.Load("libstd");
                object[] userParameters = new object[1];
                userParameters[0] = status;

                Type theType = assembly.GetType("libstd");
                MethodInfo theMethod = theType.GetMethod("exit");
                theMethod.Invoke(null, userParameters);
            } catch (Exception) { }

            Environment.Exit(status);
        }

    }

    public sealed class Crt {
        public static int    __minsi3(int a, int b)        { return (a<=b) ? a : b; }
        public static long   __mindi3(long a, long b)      { return (a<=b) ? a : b; }
        public static uint   __uminsi3(uint a, uint b)     { return (a<=b) ? a : b; }
        public static ulong  __umindi3(ulong a, ulong b)   { return (a<=b) ? a : b; }
        public static float  __minsf3(float a, float b)    { return (a<=b) ? a : b; }
        public static double __mindf3(double a, double b)  { return (a<=b) ? a : b; }

        public static int    __maxsi3(int a, int b)        { return (a>=b) ? a : b; }
        public static long   __maxdi3(long a, long b)      { return (a>=b) ? a : b; }
        public static uint   __umaxsi3(uint a, uint b)     { return (a>=b) ? a : b; }
        public static ulong  __umaxdi3(ulong a, ulong b)   { return (a>=b) ? a : b; }
        public static float  __maxsf3(float a, float b)    { return (a>=b) ? a : b; }
        public static double __maxdf3(double a, double b)  { return (a>=b) ? a : b; }

        public static int    __abssi2(int a)               { return (a>=0) ? a : -a; }
        public static long   __absdi2(long a)              { return (a>=0) ? a : -a; }
        public static float  __abssf2(float a)             { return (a>=0) ? a : -a; }
        public static double __absdf2(double a)            { return (a>=0) ? a : -a; }

        public static int    __selectsi4(int a, int b, int c)       { return (a != 0) ? b : c; }
        public static long   __selectdi4(int a, long b, long c)     { return (a != 0) ? b : c; }
        public static float  __selectsf4(int a, float b, float c)   { return (a != 0) ? b : c; }
        public static double __selectdf4(int a, double b, double c) { return (a != 0) ? b : c; }

        // Returns the number of leading 0-bits in a, starting at the most
        // significant bit position. If a is 0, the result is undefined.
        public static int __clzsi2(uint a)
        {
          if (a == 0)
            return 32;  /* __builtin_clz(0) is undefined, anyway */
          int n = 0;
          if (a <= 0x0000ffff) { n += 16; a = a << 16; }
          if (a <= 0x00ffffff) { n +=  8; a = a <<  8; }
          if (a <= 0x0fffffff) { n +=  4; a = a <<  4; }
          if (a <= 0x3fffffff) { n +=  2; a = a <<  2; }
          if (a <= 0x7fffffff) { ++n; }
          return n;
        }

        public static int __clzdi2(ulong a)
        {
          if (a == 0)
            return 64;  /* __builtin_clz(0) is undefined, anyway */
          int n = 0;
          if (a <= 0x00000000ffffffff) { n += 32; a = a << 32; }
          if (a <= 0x0000ffffffffffff) { n += 16; a = a << 16; }
          if (a <= 0x00ffffffffffffff) { n +=  8; a = a <<  8; }
          if (a <= 0x0fffffffffffffff) { n +=  4; a = a <<  4; }
          if (a <= 0x3fffffffffffffff) { n +=  2; a = a <<  2; }
          if (a <= 0x7fffffffffffffff) { ++n; }
          return n;
        }

        // Returns the number of trailing 0-bits in a, starting at the least
        // significant bit position. If a is 0, the result is undefined.
        public static int __ctzsi2(uint a)
        {
          if (a == 0)
            return 32;  /* __builtin_ctz(0) is undefined, anyway */
          int n = 1;
          if ((a & 0x0000ffff) == 0) { n += 16; a = a >> 16; }
          if ((a & 0x000000ff) == 0) { n +=  8; a = a >>  8; }
          if ((a & 0x0000000f) == 0) { n +=  4; a = a >>  4; }
          if ((a & 0x00000003) == 0) { n +=  2; a = a >>  2; }
          return n - (int)(a & 1);
        }

        public static int __ctzdi2(ulong a)
        {
          if (a == 0)
            return 64;  /* __builtin_ctz(0) is undefined, anyway */
          int n = 1;
          if ((a & 0x00000000ffffffff) == 0) { n += 32; a = a >> 32; }
          if ((a & 0x000000000000ffff) == 0) { n += 16; a = a >> 16; }
          if ((a & 0x00000000000000ff) == 0) { n +=  8; a = a >>  8; }
          if ((a & 0x000000000000000f) == 0) { n +=  4; a = a >>  4; }
          if ((a & 0x0000000000000003) == 0) { n +=  2; a = a >>  2; }
          return (n - (int)(a & 1));
        }

        // Returns one plus the index of the least significant 1-bit of a, or
        // if a is zero, returns zero.
        // Should take uint, but __builtin_ffs defines it as taking int.
        public static int __ffssi2(int a)
        {
          int i;
          if (a == 0)
            return 0;
          for (i = 0; i < 32; i++)
            if ((a & (1  << i)) != 0)
              break;
          return i + 1;
        }

        // Should take ulong, but __builtin_ffs defines it as taking long.
        public static int __ffsdi2(long a)
        {
          int i;
          if (a == 0)
            return 0;
          for (i = 0; i < 64; i++)
            if ((a & ((long)1  << i)) != 0)
              break;
          return i + 1;
        }

        // Returns the parity of a, i.e. the number of 1-bits in a modulo 2.
        public static int __paritysi2(uint a)
        {
          uint n;
          n = a ^ (a >> 1);
          n = n ^ (n >> 2);
          n = n ^ (n >> 4);
          n = n ^ (n >> 8);
          n = n ^ (n >> 16);
          return (int)(n & 1);
        }

        public static int __paritydi2(ulong a)
        {
          ulong n;
          n = a ^ (a >> 1);
          n = n ^ (n >> 2);
          n = n ^ (n >> 4);
          n = n ^ (n >> 8);
          n = n ^ (n >> 16);
          n = n ^ (n >> 32);
          return (int)(n & 1);
        }

        // Returns the number of 1-bits in a.
        public static int __popcountsi2(uint a)
        {
          a = (a & 0x55555555) + ((a >>  1) & 0x55555555);
          a = (a & 0x33333333) + ((a >>  2) & 0x33333333);
          a = (a & 0x0f0f0f0f) + ((a >>  4) & 0x0f0f0f0f);
          a = (a & 0x00ff00ff) + ((a >>  8) & 0x00ff00ff);
          a = (a & 0x0000ffff) + ((a >> 16) & 0x0000ffff);
          return (int)a;
        }

        public static int __popcountdi2(ulong a)
        {
          a = (a & 0x5555555555555555) + ((a >>  1) & 0x5555555555555555);
          a = (a & 0x3333333333333333) + ((a >>  2) & 0x3333333333333333);
          a = (a & 0x0f0f0f0f0f0f0f0f) + ((a >>  4) & 0x0f0f0f0f0f0f0f0f);
          a = (a & 0x00ff00ff00ff00ff) + ((a >>  8) & 0x00ff00ff00ff00ff);
          a = (a & 0x0000ffff0000ffff) + ((a >> 16) & 0x0000ffff0000ffff);
          a = (a & 0x00000000ffffffff) + ((a >> 32) & 0x00000000ffffffff);
          return (int)a;
        }

        public static unsafe bool __isLittleEndian() {
            // big endian:    3f f0 00 00 00 00 00 00
            // little endian: 00 00 00 00 00 00 f0 3f
            double d = 1.0;
            byte*  b = (byte*)&d;
            return b[0]==0;
        }

        public static unsafe void* __EndianSelect(void* a1, void* a2) {
            // big endian:    3f f0 00 00 00 00 00 00
            // little endian: 00 00 00 00 00 00 f0 3f
            double d = 1.0;
            byte*  b = (byte*)&d;
            if (b[0]==0)
                return a1;
            else
                return a2;
        }
    }

    namespace CQualifiers {
        /* Optional modifier class used to mark "const" types */
        public sealed class IsConst {
        }

        /* Optional modifier class used to mark "restrict" types */
        public sealed class IsRestrict {
        }

        /* Optional modifier class used to mark "volatile" types */
        public sealed class IsVolatile {
        }
    }

    namespace C_Attributes {
        [AttributeUsage (AttributeTargets.Assembly)]
        public sealed class CObjectFile : Attribute {
            public CObjectFile () { }
        }
        [AttributeUsage (AttributeTargets.Assembly)]
        public sealed class CExecutable : Attribute {
            public CExecutable () { }
        }
        [AttributeUsage (AttributeTargets.Assembly)]
        public sealed class CStaticLibrary : Attribute {
            public CStaticLibrary () { }
        }
        [AttributeUsage (AttributeTargets.Assembly)]
        public sealed class CDynamicLibrary : Attribute {
            public CDynamicLibrary () { }
        }
        [AttributeUsage (AttributeTargets.Assembly)]
        public sealed class MultiThreaded : Attribute {
            public MultiThreaded () { }
        }
        [AttributeUsage (AttributeTargets.Method)]
        public sealed class Initializer : Attribute {
            public Initializer () { }
        }
        [AttributeUsage (AttributeTargets.Method)]
        public sealed class Finalizer : Attribute {
            public Finalizer () { }
        }
        [AttributeUsage (AttributeTargets.Struct)]
        public sealed class ConstStringType : Attribute {
            public ConstStringType () { }
        }
        [AttributeUsage (AttributeTargets.Struct)]
        public sealed class ArrayType : Attribute {
            public ArrayType () { }
        }
        [AttributeUsage (AttributeTargets.Struct)]
        public sealed class IncompleteType : Attribute {
            public IncompleteType () { }
        }
    }

    namespace JitCompilationHints {

        [Serializable, AttributeUsage (AttributeTargets.Method)]
        public unsafe sealed class JITMethodAttribute : Attribute {
            public byte* frequencies;
            public byte* probabilities;

            public JITMethodAttribute (byte* frequencies, byte* probabilities) {
                this.frequencies   = frequencies;
                this.probabilities = probabilities;
            }
        }

        [Serializable, AttributeUsage (AttributeTargets.Method)]
        public sealed class BasicBlockFrequenciesAttribute : Attribute {
            byte [] frequencies;

            public BasicBlockFrequenciesAttribute (byte[] frequencies) {
                this.frequencies = frequencies;
            }

            public byte [] Frequencies {
                get {
                    return frequencies;
                }
            }
        }

        [Serializable, AttributeUsage (AttributeTargets.Method)]
        public sealed class BranchProbabilitiesAttribute : Attribute {
            byte [] probabilities;

            public BranchProbabilitiesAttribute (byte[] probabilities) {
                this.probabilities = probabilities;
            }

            public byte [] Probabilities {
                get {
                    return probabilities;
                }
            }
        }
      }


    // ------------------------------------------------------------------
    // Complex types
    // ------------------------------------------------------------------

    // signed integers

    public struct complex_char {
        public sbyte re, im;

        public static complex_char complex_char_ctor(sbyte the_re, sbyte the_im)
        {
            complex_char result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_short {
        public short re, im;

        public static complex_short complex_short_ctor(short the_re, short the_im)
        {
            complex_short result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_int {
        public int re, im;

        public static complex_int complex_int_ctor(int the_re, int the_im)
        {
            complex_int result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_long {
        public long re, im;

        public static complex_long complex_long_ctor(long the_re, long the_im)
        {
            complex_long result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    // unsigned signed integers

    public struct complex_uchar {
        public byte re, im;

        public static complex_uchar complex_uchar_ctor(byte the_re, byte the_im)
        {
            complex_uchar result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_ushort {
        public ushort re, im;

        public static complex_ushort complex_ushort_ctor(ushort the_re, ushort the_im)
        {
            complex_ushort result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_uint {
        public uint re, im;

        public static complex_uint complex_uint_ctor(uint the_re, uint the_im)
        {
            complex_uint result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_ulong {
        public ulong re, im;

        public static complex_ulong complex_ulong_ctor(ulong the_re, ulong the_im)
        {
            complex_ulong result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    // floating point

    public struct complex_float {
        public float re, im;

        public static complex_float complex_float_ctor(float the_re,
                                                       float the_im)
        {
            complex_float result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    public struct complex_double {
        public double re, im;

        public static complex_double complex_double_ctor(double the_re,
                                                         double the_im)
        {
            complex_double result;
            result.re = the_re;
            result.im = the_im;
            return result;
        }
    }


    // ------------------------------------------------------------------
    // Vector types
    // ------------------------------------------------------------------

    // 4-byte int vector types: V4QI, V2HI
    public struct V4QI {
        private byte f0, f1, f2, f3;

        public static V4QI V4QI_ctor1(byte b0, byte b1, byte b2, byte b3)
        {
            V4QI result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            return result;
        }

        public static V4QI V4QI_ctor2(uint f)
        {
            V4QI result;
            result.f0 = (byte) (f >> 24);
            result.f1 = (byte)((f >> 16) & 0xff);
            result.f2 = (byte)((f >> 8) & 0xff);
            result.f3 = (byte) (f & 0xff);
            return result;
        }

        public static uint V4QI_to_usi(V4QI v)
        {
            return (uint) ((v.f0 << 24) | (v.f1 << 16) | (v.f2 << 8)  | v.f3);
        }

        public static int V4QI_to_si(V4QI v)
        {
            return (int) ((v.f0 << 24) | (v.f1 << 16) | (v.f2 << 8) | v.f3);
        }
    }


    public struct V2HI {
        private ushort f0, f1;

        public static V2HI V2HI_ctor1(ushort s0, ushort s1)
        {
            V2HI result;
            result.f0 = s0;
            result.f1 = s1;
            return result;
        }

        public static V2HI V2HI_ctor2(uint f)
        {
            V2HI result;
            result.f0 = (ushort)(f >> 16);
            result.f1 = (ushort)(f & 0xffff);
            return result;
        }

        public static uint V2HI_to_usi(V2HI v)
        {
            return (uint) ((v.f0 << 16) | v.f1);
        }

        public static int V2HI_to_si(V2HI v)
        {
            return (int)((v.f0 << 16) | v.f1);
        }
    }

    // 8-byte int vector types: V8QI, V4HI, V2SI
    public struct V8QI {
        private byte f0, f1, f2, f3, f4, f5, f6, f7;

        public static V8QI V8QI_ctor1(byte b0, byte b1, byte b2, byte b3,
                                     byte b4, byte b5, byte b6, byte b7)
        {
            V8QI result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            result.f4 = b4;
            result.f5 = b5;
            result.f6 = b6;
            result.f7 = b7;
            return result;
        }

        public static V8QI V8QI_ctor2(ulong f)
        {
            V8QI result;
            result.f0 = (byte) (f >> 56);
            result.f1 = (byte)((f >> 48) & 0xff);
            result.f2 = (byte)((f >> 40) & 0xff);
            result.f3 = (byte)((f >> 32) & 0xff);
            result.f4 = (byte)((f >> 24) & 0xff);
            result.f5 = (byte)((f >> 16) & 0xff);
            result.f6 = (byte)((f >> 8) & 0xff);
            result.f7 = (byte) (f & 0xff);
            return result;
        }

        public static ulong V8QI_to_udi(V8QI v)
        {
            return ((ulong)v.f0 << 56) | ((ulong)v.f1 << 48) |
                   ((ulong)v.f2 << 40) | ((ulong)v.f3 << 32) |
                   ((ulong)v.f4 << 24) | ((ulong)v.f5 << 16) |
                   ((ulong)v.f6 << 8)  |  (ulong)v.f7;
        }
        public static long V8QI_to_di(V8QI v)
        {
          return (long)(((ulong)v.f0 << 56) | ((ulong)v.f1 << 48) |
                        ((ulong)v.f2 << 40) | ((ulong)v.f3 << 32) |
                        ((ulong)v.f4 << 24) | ((ulong)v.f5 << 16) |
                        ((ulong)v.f6 << 8)  |  (ulong)v.f7);
        }
    }


    public struct V4HI {
        private ushort f0, f1, f2, f3;

        public static V4HI V4HI_ctor1(ushort b0, ushort b1, ushort b2, ushort b3)
        {
            V4HI result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            return result;
        }

        public static V4HI V4HI_ctor2(ulong f)
        {
            V4HI result;
            result.f0 = (ushort) (f >> 48);
            result.f1 = (ushort)((f >> 32) & 0xffff);
            result.f2 = (ushort)((f >> 16) & 0xffff);
            result.f3 = (ushort) (f & 0xffff);
            return result;
        }

        public static ulong V4HI_to_udi(V4HI v)
        {
            return ((ulong)v.f0 << 48) | ((ulong)v.f1 << 32) |
                   ((ulong)v.f2 << 16) |  (ulong)v.f3;
        }
        public static long V4HI_to_di(V4HI v)
        {
            return (long)(((ulong)v.f0 << 48) | ((ulong)v.f1 << 32) |
                          ((ulong)v.f2 << 16) |  (ulong)v.f3);
        }
    }


    public struct V2SI {
        private uint f0, f1;

        public static V2SI V2SI_ctor1(uint b0, uint b1)
        {
            V2SI result;
            result.f0 = b0;
            result.f1 = b1;
            return result;
        }
        public static V2SI V2SI_ctor2(ulong f)
        {
            V2SI result;
            result.f0 = (uint)(f >> 32);
            result.f1 = (uint)(f & 0xffff);
            return result;
        }

        public static ulong V2SI_to_udi(V2SI v)
        {
            return ((ulong)v.f0 << 32) | (ulong)v.f1;
        }

        public static long V2SI_to_di(V2SI v)
        {
            return (long)(((ulong)v.f0 << 32) | (ulong)v.f1);
        }
    }


    // 16-byte int vector types: V4SI
    public struct V4SI {
        private uint f0, f1, f2, f3;

        public static V4SI V4SI_ctor1(uint b0, uint b1, uint b2, uint b3)
        {
            V4SI result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            return result;
        }

        public unsafe static V4SF V4SI_to_V4SF(V4SI v)
        {
            return V4SF.V4SF_ctor1(*(float*)&v.f0, *(float*)&v.f1,
				   *(float*)&v.f2, *(float*)&v.f3);
        }
    }

    public struct V8HI {
        private ushort f0, f1, f2, f3, f4, f5, f6, f7;

        public static V8HI V8HI_ctor1(ushort b0, ushort b1,
                                      ushort b2, ushort b3,
                                      ushort b4, ushort b5,
                                      ushort b6, ushort b7)
        {
            V8HI result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            result.f4 = b4;
            result.f5 = b5;
            result.f6 = b6;
            result.f7 = b7;
            return result;
        }
    }

    public struct V16QI {
        private byte f0, f1, f2, f3, f4, f5, f6, f7,
                     f8, f9, f10, f11, f12, f13, f14, f15;

        public static V16QI V16QI_ctor1(byte b0, byte b1, byte b2, byte b3,
                                       byte b4, byte b5, byte b6, byte b7,
                                       byte b8, byte b9, byte b10, byte b11,
                                       byte b12, byte b13, byte b14, byte b15)
        {
            V16QI result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            result.f4 = b4;
            result.f5 = b5;
            result.f6 = b6;
            result.f7 = b7;
            result.f8 = b8;
            result.f9 = b9;
            result.f10 = b10;
            result.f11 = b11;
            result.f12 = b12;
            result.f13 = b13;
            result.f14 = b14;
            result.f15 = b15;
            return result;
        }
    }

    // 8-byte float vector types: V2SF
    public struct V2SF {
        private float f0, f1;

        public static V2SF V2SF_ctor1(float b0, float b1)
        {
            V2SF result;
            result.f0 = b0;
            result.f1 = b1;
            return result;
        }

        public unsafe static long V2SF_to_di(V2SF v)
        {
            uint a0 = *(uint*)&v.f0;
            uint a1 = *(uint*)&v.f1;
            return (long)(((ulong)a0 << 32) | (ulong)a1);
        }
    }

    // 16-byte float vector types: V4SF
    public struct V4SF {
        private float f0, f1, f2, f3;

        public static V4SF V4SF_ctor1(float b0, float b1, float b2, float b3)
        {
            V4SF result;
            result.f0 = b0;
            result.f1 = b1;
            result.f2 = b2;
            result.f3 = b3;
            return result;
        }
    }
}
