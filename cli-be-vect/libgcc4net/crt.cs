/* Run-time support required by CIL binaries.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.
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
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

namespace gcc4net {

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

}
