/* Run-time support required by CIL binaries to handle vector types.

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

        public static V4QI V4QI_ctor1(byte b)
        {
            V4QI result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
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

        public static V4QI add(V4QI x, V4QI y)
        {
	  V4QI result;
	  result.f0 = (byte)(x.f0 + y.f0);
	  result.f1 = (byte)(x.f1 + y.f1);
	  result.f2 = (byte)(x.f2 + y.f2);
	  result.f3 = (byte)(x.f3 + y.f3);
	  return result;
	}

        public static V4QI sub(V4QI x, V4QI y)
        {
	  V4QI result;
	  result.f0 = (byte)(x.f0 - y.f0);
	  result.f1 = (byte)(x.f1 - y.f1);
	  result.f2 = (byte)(x.f2 - y.f2);
	  result.f3 = (byte)(x.f3 - y.f3);
	  return result;
	}

        public static V4QI and(V4QI x, V4QI y)
        {
	  V4QI result;
	  result.f0 = (byte)(x.f0 & y.f0);
	  result.f1 = (byte)(x.f1 & y.f1);
	  result.f2 = (byte)(x.f2 & y.f2);
	  result.f3 = (byte)(x.f3 & y.f3);
	  return result;
	}

        public static V4QI or(V4QI x, V4QI y)
        {
	  V4QI result;
	  result.f0 = (byte)(x.f0 | y.f0);
	  result.f1 = (byte)(x.f1 | y.f1);
	  result.f2 = (byte)(x.f2 | y.f2);
	  result.f3 = (byte)(x.f3 | y.f3);
	  return result;
	}

        public static V4QI xor(V4QI x, V4QI y)
        {
	  V4QI result;
	  result.f0 = (byte)(x.f0 ^ y.f0);
	  result.f1 = (byte)(x.f1 ^ y.f1);
	  result.f2 = (byte)(x.f2 ^ y.f2);
	  result.f3 = (byte)(x.f3 ^ y.f3);
	  return result;
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

        public static V2HI V2HI_ctor1(ushort s)
        {
            V2HI result;
            result.f0 = s;
            result.f1 = s;
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

        public static V2HI add(V2HI x, V2HI y)
        {
	  V2HI result;
	  result.f0 = (ushort)(x.f0 + y.f0);
	  result.f1 = (ushort)(x.f1 + y.f1);
	  return result;
	}

        public static V2HI sub(V2HI x, V2HI y)
        {
	  V2HI result;
	  result.f0 = (ushort)(x.f0 - y.f0);
	  result.f1 = (ushort)(x.f1 - y.f1);
	  return result;
	}

        public static V2HI and(V2HI x, V2HI y)
        {
	  V2HI result;
	  result.f0 = (ushort)(x.f0 & y.f0);
	  result.f1 = (ushort)(x.f1 & y.f1);
	  return result;
	}

        public static V2HI or(V2HI x, V2HI y)
        {
	  V2HI result;
	  result.f0 = (ushort)(x.f0 | y.f0);
	  result.f1 = (ushort)(x.f1 | y.f1);
	  return result;
	}

        public static V2HI xor(V2HI x, V2HI y)
        {
	  V2HI result;
	  result.f0 = (ushort)(x.f0 ^ y.f0);
	  result.f1 = (ushort)(x.f1 ^ y.f1);
	  return result;
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

        public static V8QI V8QI_ctor1(byte b)
        {
            V8QI result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
            result.f4 = b;
            result.f5 = b;
            result.f6 = b;
            result.f7 = b;
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

        public static V8QI add(V8QI x, V8QI y)
        {
	  V8QI result;
	  result.f0 = (byte)(x.f0 + y.f0);
	  result.f1 = (byte)(x.f1 + y.f1);
	  result.f2 = (byte)(x.f2 + y.f2);
	  result.f3 = (byte)(x.f3 + y.f3);
	  result.f4 = (byte)(x.f4 + y.f4);
	  result.f5 = (byte)(x.f5 + y.f5);
	  result.f6 = (byte)(x.f6 + y.f6);
	  result.f7 = (byte)(x.f7 + y.f7);
	  return result;
	}

        public static V8QI sub(V8QI x, V8QI y)
        {
	  V8QI result;
	  result.f0 = (byte)(x.f0 - y.f0);
	  result.f1 = (byte)(x.f1 - y.f1);
	  result.f2 = (byte)(x.f2 - y.f2);
	  result.f3 = (byte)(x.f3 - y.f3);
	  result.f4 = (byte)(x.f4 - y.f4);
	  result.f5 = (byte)(x.f5 - y.f5);
	  result.f6 = (byte)(x.f6 - y.f6);
	  result.f7 = (byte)(x.f7 - y.f7);
	  return result;
	}

        public static V8QI and(V8QI x, V8QI y)
        {
	  V8QI result;
	  result.f0 = (byte)(x.f0 & y.f0);
	  result.f1 = (byte)(x.f1 & y.f1);
	  result.f2 = (byte)(x.f2 & y.f2);
	  result.f3 = (byte)(x.f3 & y.f3);
	  result.f4 = (byte)(x.f4 & y.f4);
	  result.f5 = (byte)(x.f5 & y.f5);
	  result.f6 = (byte)(x.f6 & y.f6);
	  result.f7 = (byte)(x.f7 & y.f7);
	  return result;
	}

        public static V8QI or(V8QI x, V8QI y)
        {
	  V8QI result;
	  result.f0 = (byte)(x.f0 | y.f0);
	  result.f1 = (byte)(x.f1 | y.f1);
	  result.f2 = (byte)(x.f2 | y.f2);
	  result.f3 = (byte)(x.f3 | y.f3);
	  result.f4 = (byte)(x.f4 | y.f4);
	  result.f5 = (byte)(x.f5 | y.f5);
	  result.f6 = (byte)(x.f6 | y.f6);
	  result.f7 = (byte)(x.f7 | y.f7);
	  return result;
	}

        public static V8QI xor(V8QI x, V8QI y)
        {
	  V8QI result;
	  result.f0 = (byte)(x.f0 ^ y.f0);
	  result.f1 = (byte)(x.f1 ^ y.f1);
	  result.f2 = (byte)(x.f2 ^ y.f2);
	  result.f3 = (byte)(x.f3 ^ y.f3);
	  result.f4 = (byte)(x.f4 ^ y.f4);
	  result.f5 = (byte)(x.f5 ^ y.f5);
	  result.f6 = (byte)(x.f6 ^ y.f6);
	  result.f7 = (byte)(x.f7 ^ y.f7);
	  return result;
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

        public static V4HI V4HI_ctor1(ushort b)
        {
            V4HI result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
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

        public static V4HI add(V4HI x, V4HI y)
        {
	  V4HI result;
	  result.f0 = (ushort)(x.f0 + y.f0);
	  result.f1 = (ushort)(x.f1 + y.f1);
	  result.f2 = (ushort)(x.f2 + y.f2);
	  result.f3 = (ushort)(x.f3 + y.f3);
	  return result;
	}

        public static V4HI sub(V4HI x, V4HI y)
        {
	  V4HI result;
	  result.f0 = (ushort)(x.f0 - y.f0);
	  result.f1 = (ushort)(x.f1 - y.f1);
	  result.f2 = (ushort)(x.f2 - y.f2);
	  result.f3 = (ushort)(x.f3 - y.f3);
	  return result;
	}

        public static V4HI and(V4HI x, V4HI y)
        {
	  V4HI result;
	  result.f0 = (ushort)(x.f0 & y.f0);
	  result.f1 = (ushort)(x.f1 & y.f1);
	  result.f2 = (ushort)(x.f2 & y.f2);
	  result.f3 = (ushort)(x.f3 & y.f3);
	  return result;
	}

        public static V4HI or(V4HI x, V4HI y)
        {
	  V4HI result;
	  result.f0 = (ushort)(x.f0 | y.f0);
	  result.f1 = (ushort)(x.f1 | y.f1);
	  result.f2 = (ushort)(x.f2 | y.f2);
	  result.f3 = (ushort)(x.f3 | y.f3);
	  return result;
	}

        public static V4HI xor(V4HI x, V4HI y)
        {
	  V4HI result;
	  result.f0 = (ushort)(x.f0 ^ y.f0);
	  result.f1 = (ushort)(x.f1 ^ y.f1);
	  result.f2 = (ushort)(x.f2 ^ y.f2);
	  result.f3 = (ushort)(x.f3 ^ y.f3);
	  return result;
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

        public static V2SI V2SI_ctor1(uint b)
        {
            V2SI result;
            result.f0 = b;
            result.f1 = b;
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

        public static V2SI add(V2SI x, V2SI y)
        {
	  V2SI result;
	  result.f0 = x.f0 + y.f0;
	  result.f1 = x.f1 + y.f1;
	  return result;
	}

        public static V2SI sub(V2SI x, V2SI y)
        {
	  V2SI result;
	  result.f0 = x.f0 - y.f0;
	  result.f1 = x.f1 - y.f1;
	  return result;
	}

        public static V2SI and(V2SI x, V2SI y)
        {
	  V2SI result;
	  result.f0 = x.f0 & y.f0;
	  result.f1 = x.f1 & y.f1;
	  return result;
	}

        public static V2SI or(V2SI x, V2SI y)
        {
	  V2SI result;
	  result.f0 = x.f0 | y.f0;
	  result.f1 = x.f1 | y.f1;
	  return result;
	}

        public static V2SI xor(V2SI x, V2SI y)
        {
	  V2SI result;
	  result.f0 = x.f0 ^ y.f0;
	  result.f1 = x.f1 ^ y.f1;
	  return result;
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

        public static V4SI V4SI_ctor1(uint b)
        {
            V4SI result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
            return result;
        }

        public static V4SI add(V4SI x, V4SI y)
        {
	  V4SI result;
	  result.f0 = x.f0 + y.f0;
	  result.f1 = x.f1 + y.f1;
	  result.f2 = x.f2 + y.f2;
	  result.f3 = x.f3 + y.f3;
	  return result;
	}

        public static V4SI sub(V4SI x, V4SI y)
        {
	  V4SI result;
	  result.f0 = x.f0 - y.f0;
	  result.f1 = x.f1 - y.f1;
	  result.f2 = x.f2 - y.f2;
	  result.f3 = x.f3 - y.f3;
	  return result;
	}

        public static V4SI and(V4SI x, V4SI y)
        {
	  V4SI result;
	  result.f0 = x.f0 & y.f0;
	  result.f1 = x.f1 & y.f1;
	  result.f2 = x.f2 & y.f2;
	  result.f3 = x.f3 & y.f3;
	  return result;
	}

        public static V4SI or(V4SI x, V4SI y)
        {
	  V4SI result;
	  result.f0 = x.f0 | y.f0;
	  result.f1 = x.f1 | y.f1;
	  result.f2 = x.f2 | y.f2;
	  result.f3 = x.f3 | y.f3;
	  return result;
	}

        public static V4SI xor(V4SI x, V4SI y)
        {
	  V4SI result;
	  result.f0 = x.f0 ^ y.f0;
	  result.f1 = x.f1 ^ y.f1;
	  result.f2 = x.f2 ^ y.f2;
	  result.f3 = x.f3 ^ y.f3;
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

        public static V8HI V8HI_ctor1(ushort b)
        {
            V8HI result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
            result.f4 = b;
            result.f5 = b;
            result.f6 = b;
            result.f7 = b;
            return result;
        }

        public static V8HI add(V8HI x, V8HI y)
        {
	  V8HI result;
	  result.f0 = (ushort)(x.f0 + y.f0);
	  result.f1 = (ushort)(x.f1 + y.f1);
	  result.f2 = (ushort)(x.f2 + y.f2);
	  result.f3 = (ushort)(x.f3 + y.f3);
	  result.f4 = (ushort)(x.f4 + y.f4);
	  result.f5 = (ushort)(x.f5 + y.f5);
	  result.f6 = (ushort)(x.f6 + y.f6);
	  result.f7 = (ushort)(x.f7 + y.f7);
	  return result;
	}

        public static V8HI sub(V8HI x, V8HI y)
        {
	  V8HI result;
	  result.f0 = (ushort)(x.f0 - y.f0);
	  result.f1 = (ushort)(x.f1 - y.f1);
	  result.f2 = (ushort)(x.f2 - y.f2);
	  result.f3 = (ushort)(x.f3 - y.f3);
	  result.f4 = (ushort)(x.f4 - y.f4);
	  result.f5 = (ushort)(x.f5 - y.f5);
	  result.f6 = (ushort)(x.f6 - y.f6);
	  result.f7 = (ushort)(x.f7 - y.f7);
	  return result;
	}

        public static V8HI and(V8HI x, V8HI y)
        {
	  V8HI result;
	  result.f0 = (ushort)(x.f0 & y.f0);
	  result.f1 = (ushort)(x.f1 & y.f1);
	  result.f2 = (ushort)(x.f2 & y.f2);
	  result.f3 = (ushort)(x.f3 & y.f3);
	  result.f4 = (ushort)(x.f4 & y.f4);
	  result.f5 = (ushort)(x.f5 & y.f5);
	  result.f6 = (ushort)(x.f6 & y.f6);
	  result.f7 = (ushort)(x.f7 & y.f7);
	  return result;
	}

        public static V8HI or(V8HI x, V8HI y)
        {
	  V8HI result;
	  result.f0 = (ushort)(x.f0 | y.f0);
	  result.f1 = (ushort)(x.f1 | y.f1);
	  result.f2 = (ushort)(x.f2 | y.f2);
	  result.f3 = (ushort)(x.f3 | y.f3);
	  result.f4 = (ushort)(x.f4 | y.f4);
	  result.f5 = (ushort)(x.f5 | y.f5);
	  result.f6 = (ushort)(x.f6 | y.f6);
	  result.f7 = (ushort)(x.f7 | y.f7);
	  return result;
	}

        public static V8HI xor(V8HI x, V8HI y)
        {
	  V8HI result;
	  result.f0 = (ushort)(x.f0 ^ y.f0);
	  result.f1 = (ushort)(x.f1 ^ y.f1);
	  result.f2 = (ushort)(x.f2 ^ y.f2);
	  result.f3 = (ushort)(x.f3 ^ y.f3);
	  result.f4 = (ushort)(x.f4 ^ y.f4);
	  result.f5 = (ushort)(x.f5 ^ y.f5);
	  result.f6 = (ushort)(x.f6 ^ y.f6);
	  result.f7 = (ushort)(x.f7 ^ y.f7);
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

        public static V16QI V16QI_ctor1(byte b)
        {
            V16QI result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
            result.f4 = b;
            result.f5 = b;
            result.f6 = b;
            result.f7 = b;
            result.f8 = b;
            result.f9 = b;
            result.f10 = b;
            result.f11 = b;
            result.f12 = b;
            result.f13 = b;
            result.f14 = b;
            result.f15 = b;
            return result;
        }

        public static V16QI add(V16QI x, V16QI y)
        {
	  V16QI result;
	  result.f0 = (byte)(x.f0 + y.f0);
	  result.f1 = (byte)(x.f1 + y.f1);
	  result.f2 = (byte)(x.f2 + y.f2);
	  result.f3 = (byte)(x.f3 + y.f3);
	  result.f4 = (byte)(x.f4 + y.f4);
	  result.f5 = (byte)(x.f5 + y.f5);
	  result.f6 = (byte)(x.f6 + y.f6);
	  result.f7 = (byte)(x.f7 + y.f7);
	  result.f8 = (byte)(x.f8 + y.f8);
	  result.f9 = (byte)(x.f9 + y.f9);
	  result.f10 = (byte)(x.f10 + y.f10);
	  result.f11 = (byte)(x.f11 + y.f11);
	  result.f12 = (byte)(x.f12 + y.f12);
	  result.f13 = (byte)(x.f13 + y.f13);
	  result.f14 = (byte)(x.f14 + y.f14);
	  result.f15 = (byte)(x.f15 + y.f15);
	  return result;
	}

        public static V16QI sub(V16QI x, V16QI y)
        {
	  V16QI result;
	  result.f0 = (byte)(x.f0 - y.f0);
	  result.f1 = (byte)(x.f1 - y.f1);
	  result.f2 = (byte)(x.f2 - y.f2);
	  result.f3 = (byte)(x.f3 - y.f3);
	  result.f4 = (byte)(x.f4 - y.f4);
	  result.f5 = (byte)(x.f5 - y.f5);
	  result.f6 = (byte)(x.f6 - y.f6);
	  result.f7 = (byte)(x.f7 - y.f7);
	  result.f8 = (byte)(x.f8 - y.f8);
	  result.f9 = (byte)(x.f9 - y.f9);
	  result.f10 = (byte)(x.f10 - y.f10);
	  result.f11 = (byte)(x.f11 - y.f11);
	  result.f12 = (byte)(x.f12 - y.f12);
	  result.f13 = (byte)(x.f13 - y.f13);
	  result.f14 = (byte)(x.f14 - y.f14);
	  result.f15 = (byte)(x.f15 - y.f15);
	  return result;
	}

        public static V16QI and(V16QI x, V16QI y)
        {
	  V16QI result;
	  result.f0 = (byte)(x.f0 & y.f0);
	  result.f1 = (byte)(x.f1 & y.f1);
	  result.f2 = (byte)(x.f2 & y.f2);
	  result.f3 = (byte)(x.f3 & y.f3);
	  result.f4 = (byte)(x.f4 & y.f4);
	  result.f5 = (byte)(x.f5 & y.f5);
	  result.f6 = (byte)(x.f6 & y.f6);
	  result.f7 = (byte)(x.f7 & y.f7);
	  result.f8 = (byte)(x.f8 & y.f8);
	  result.f9 = (byte)(x.f9 & y.f9);
	  result.f10 = (byte)(x.f10 & y.f10);
	  result.f11 = (byte)(x.f11 & y.f11);
	  result.f12 = (byte)(x.f12 & y.f12);
	  result.f13 = (byte)(x.f13 & y.f13);
	  result.f14 = (byte)(x.f14 & y.f14);
	  result.f15 = (byte)(x.f15 & y.f15);
	  return result;
	}

        public static V16QI or(V16QI x, V16QI y)
        {
	  V16QI result;
	  result.f0 = (byte)(x.f0 | y.f0);
	  result.f1 = (byte)(x.f1 | y.f1);
	  result.f2 = (byte)(x.f2 | y.f2);
	  result.f3 = (byte)(x.f3 | y.f3);
	  result.f4 = (byte)(x.f4 | y.f4);
	  result.f5 = (byte)(x.f5 | y.f5);
	  result.f6 = (byte)(x.f6 | y.f6);
	  result.f7 = (byte)(x.f7 | y.f7);
	  result.f8 = (byte)(x.f8 | y.f8);
	  result.f9 = (byte)(x.f9 | y.f9);
	  result.f10 = (byte)(x.f10 | y.f10);
	  result.f11 = (byte)(x.f11 | y.f11);
	  result.f12 = (byte)(x.f12 | y.f12);
	  result.f13 = (byte)(x.f13 | y.f13);
	  result.f14 = (byte)(x.f14 | y.f14);
	  result.f15 = (byte)(x.f15 | y.f15);
	  return result;
	}

        public static V16QI xor(V16QI x, V16QI y)
        {
	  V16QI result;
	  result.f0 = (byte)(x.f0 ^ y.f0);
	  result.f1 = (byte)(x.f1 ^ y.f1);
	  result.f2 = (byte)(x.f2 ^ y.f2);
	  result.f3 = (byte)(x.f3 ^ y.f3);
	  result.f4 = (byte)(x.f4 ^ y.f4);
	  result.f5 = (byte)(x.f5 ^ y.f5);
	  result.f6 = (byte)(x.f6 ^ y.f6);
	  result.f7 = (byte)(x.f7 ^ y.f7);
	  result.f8 = (byte)(x.f8 ^ y.f8);
	  result.f9 = (byte)(x.f9 ^ y.f9);
	  result.f10 = (byte)(x.f10 ^ y.f10);
	  result.f11 = (byte)(x.f11 ^ y.f11);
	  result.f12 = (byte)(x.f12 ^ y.f12);
	  result.f13 = (byte)(x.f13 ^ y.f13);
	  result.f14 = (byte)(x.f14 ^ y.f14);
	  result.f15 = (byte)(x.f15 ^ y.f15);
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

        public static V2SF V2SF_ctor1(float b)
        {
            V2SF result;
            result.f0 = b;
            result.f1 = b;
            return result;
        }

        public unsafe static long V2SF_to_di(V2SF v)
        {
            uint a0 = *(uint*)&v.f0;
            uint a1 = *(uint*)&v.f1;
            return (long)(((ulong)a0 << 32) | (ulong)a1);
        }

        public static V2SF add(V2SF x, V2SF y)
        {
	  V2SF result;
	  result.f0 = x.f0 + y.f0;
	  result.f1 = x.f1 + y.f1;
	  return result;
	}

        public static V2SF sub(V2SF x, V2SF y)
        {
	  V2SF result;
	  result.f0 = x.f0 - y.f0;
	  result.f1 = x.f1 - y.f1;
	  return result;
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

        public static V4SF V4SF_ctor1(float b)
        {
            V4SF result;
            result.f0 = b;
            result.f1 = b;
            result.f2 = b;
            result.f3 = b;
            return result;
        }

        public static V4SF add(V4SF x, V4SF y)
        {
	  V4SF result;
	  result.f0 = x.f0 + y.f0;
	  result.f1 = x.f1 + y.f1;
	  result.f2 = x.f2 + y.f2;
	  result.f3 = x.f3 + y.f3;
	  return result;
	}

        public static V4SF sub(V4SF x, V4SF y)
        {
	  V4SF result;
	  result.f0 = x.f0 - y.f0;
	  result.f1 = x.f1 - y.f1;
	  result.f2 = x.f2 - y.f2;
	  result.f3 = x.f3 - y.f3;
	  return result;
	}
    }

    // 16-byte float vector types: V2DF
    public struct V2DF {
        private double f0, f1;

        public static V2DF V2DF_ctor1(double b0, double b1)
        {
            V2DF result;
            result.f0 = b0;
            result.f1 = b1;
            return result;
        }

        public static V2DF V2DF_ctor1(double b)
        {
            V2DF result;
            result.f0 = b;
            result.f1 = b;
            return result;
        }

        public static V2DF add(V2DF x, V2DF y)
        {
	  V2DF result;
	  result.f0 = x.f0 + y.f0;
	  result.f1 = x.f1 + y.f1;
	  return result;
	}

        public static V2DF sub(V2DF x, V2DF y)
        {
	  V2DF result;
	  result.f0 = x.f0 - y.f0;
	  result.f1 = x.f1 - y.f1;
	  return result;
	}
    }
}
