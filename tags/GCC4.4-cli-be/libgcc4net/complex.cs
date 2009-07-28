/* Run-time support required by CIL binaries to handle complex types.

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

}
