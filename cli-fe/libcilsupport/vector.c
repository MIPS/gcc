/* Run-time support for vector types required by native binaries compiled from CIL code.

   Copyright (C) 2006-2010 Free Software Foundation, Inc.

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
	Kevin Williams <kevin.williams@inria.a[r>
*/

#include "vector.h"

u4qi
gcc4net_V4QI_V4QI_ctor1_System_Byte_System_Byte_System_Byte_System_Byte_gcc4net_V4QI (qi b0, qi b1, qi b2, qi b3)
{
  u4qi result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  return result;
}

u4qi
gcc4net_V4QI_V4QI_ctor1_System_Byte_gcc4net_V4QI (qi b)
{
  u4qi result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  return result;
}

u4qi
gcc4net_V4QI_V4QI_ctor2_System_UInt32_gcc4net_V4QI (unsigned int f)
{
  u4qi result;
  result.a[0] = (qi) (f >> 24);
  result.a[1] = (qi) ((f >> 16) & 0xff);
  result.a[2] = (qi) ((f >> 8) & 0xff);
  result.a[3] = (qi) (f & 0xff);
  return result;
}

unsigned int
gcc4net_V4QI_to_usi_gcc4net_V4QI_System_UInt32 (u4qi v)
{
  return (unsigned int) ((v.a[0] << 24) | (v.a[1] << 16) | (v.a[2] << 8) | v.a[3]);
}

int
gcc4net_V4QI_to_si_gcc4net_V4QI_System_Int32 (u4qi v)
{
  return (int) ((v.a[0] << 24) | (v.a[1] << 16) | (v.a[2] << 8) | v.a[3]);
}


u4qi
gcc4net_V4QI_add_gcc4net_V4QI_gcc4net_V4QI_gcc4net_V4QI (u4qi x, u4qi y)
{
  u4qi result;
  result.a[0] = (qi) (x.a[0] + y.a[0]);
  result.a[1] = (qi) (x.a[1] + y.a[1]);
  result.a[2] = (qi) (x.a[2] + y.a[2]);
  result.a[3] = (qi) (x.a[3] + y.a[3]);
  return result;
}

u4qi
gcc4net_V4QI_sub_gcc4net_V4QI_gcc4net_V4QI_gcc4net_V4QI (u4qi x, u4qi y)
{
  u4qi result;
  result.a[0] = (qi) (x.a[0] - y.a[0]);
  result.a[1] = (qi) (x.a[1] - y.a[1]);
  result.a[2] = (qi) (x.a[2] - y.a[2]);
  result.a[3] = (qi) (x.a[3] - y.a[3]);
  return result;
}

u4qi
gcc4net_V4QI_mul_gcc4net_V4QI_gcc4net_V4QI_gcc4net_V4QI (u4qi x, u4qi y)
{
  u4qi result;
  result.a[0] = (qi) (x.a[0] * y.a[0]);
  result.a[1] = (qi) (x.a[1] * y.a[1]);
  result.a[2] = (qi) (x.a[2] * y.a[2]);
  result.a[3] = (qi) (x.a[3] * y.a[3]);
  return result;
}

u4qi
gcc4net_V4QI_and_gcc4net_V4QI_gcc4net_V4QI_gcc4net_V4QI (u4qi x, u4qi y)
{
  u4qi result;
  result.a[0] = (qi) (x.a[0] & y.a[0]);
  result.a[1] = (qi) (x.a[1] & y.a[1]);
  result.a[2] = (qi) (x.a[2] & y.a[2]);
  result.a[3] = (qi) (x.a[3] & y.a[3]);
  return result;
}

u4qi
gcc4net_V4QI_or_gcc4net_V4QI_gcc4net_V4QI_gcc4net_V4QI (u4qi x, u4qi y)
{
  u4qi result;
  result.a[0] = (qi) (x.a[0] | y.a[0]);
  result.a[1] = (qi) (x.a[1] | y.a[1]);
  result.a[2] = (qi) (x.a[2] | y.a[2]);
  result.a[3] = (qi) (x.a[3] | y.a[3]);
  return result;
}

u4qi
gcc4net_V4QI_xor_gcc4net_V4QI_gcc4net_V4QI_gcc4net_V4QI (u4qi x, u4qi y)
{
  u4qi result;
  result.a[0] = (qi) (x.a[0] ^ y.a[0]);
  result.a[1] = (qi) (x.a[1] ^ y.a[1]);
  result.a[2] = (qi) (x.a[2] ^ y.a[2]);
  result.a[3] = (qi) (x.a[3] ^ y.a[3]);
  return result;
}


u2hi
gcc4net_V2HI_V2HI_ctor1_System_UInt16_System_UInt16_gcc4net_V2HI (unsigned short s0, unsigned short s1)
{
  u2hi result;
  result.a[0] = s0;
  result.a[1] = s1;
  return result;
}

u2hi
gcc4net_V2HI_V2HI_ctor1_System_UInt16_gcc4net_V2HI (unsigned short s)
{
  u2hi result;
  result.a[0] = s;
  result.a[1] = s;
  return result;
}

u2hi
gcc4net_V2HI_V2HI_ctor2_System_UInt32_gcc4net_V2HI (unsigned int f)
{
  u2hi result;
  result.a[0] = (unsigned short) (f >> 16);
  result.a[1] = (unsigned short) (f & 0xffff);
  return result;
}

unsigned int
gcc4net_V2HI_to_usi_gcc4net_V2HI_System_UInt32 (u2hi v)
{
  return (unsigned int) ((v.a[0] << 16) | v.a[1]);
}

int
gcc4net_V2HI_to_si_gcc4net_V2HI_System_Int32 (u2hi v)
{
  return (int) ((v.a[0] << 16) | v.a[1]);
}

u2hi
gcc4net_V2HI_add_gcc4net_V2HI_gcc4net_V2HI_gcc4net_V2HI (u2hi x, u2hi y)
{
  u2hi result;
  result.a[0] = (unsigned short) (x.a[0] + y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] + y.a[1]);
  return result;
}

u2hi
gcc4net_V2HI_sub_gcc4net_V2HI_gcc4net_V2HI_gcc4net_V2HI (u2hi x, u2hi y)
{
  u2hi result;
  result.a[0] = (unsigned short) (x.a[0] - y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] - y.a[1]);
  return result;
}

u2hi
gcc4net_V2HI_mul_gcc4net_V2HI_gcc4net_V2HI_gcc4net_V2HI (u2hi x, u2hi y)
{
  u2hi result;
  result.a[0] = (unsigned short) (x.a[0] * y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] * y.a[1]);
  return result;
}

u2hi
gcc4net_V2HI_and_gcc4net_V2HI_gcc4net_V2HI_gcc4net_V2HI (u2hi x, u2hi y)
{
  u2hi result;
  result.a[0] = (unsigned short) (x.a[0] & y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] & y.a[1]);
  return result;
}

u2hi
gcc4net_V2HI_or_gcc4net_V2HI_gcc4net_V2HI_gcc4net_V2HI (u2hi x, u2hi y)
{
  u2hi result;
  result.a[0] = (unsigned short) (x.a[0] | y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] | y.a[1]);
  return result;
}

u2hi
gcc4net_V2HI_xor_gcc4net_V2HI_gcc4net_V2HI_gcc4net_V2HI (u2hi x, u2hi y)
{
  u2hi result;
  result.a[0] = (unsigned short) (x.a[0] ^ y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] ^ y.a[1]);
  return result;
}


u8qi
gcc4net_V8QI_V8QI_ctor1_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_gcc4net_V8QI (qi b0, qi b1, qi b2, qi b3, qi b4, qi b5, qi b6, qi b7)
{
  u8qi result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  result.a[4] = b4;
  result.a[5] = b5;
  result.a[6] = b6;
  result.a[7] = b7;
  return result;
}

u8qi
gcc4net_V8QI_V8QI_ctor1_System_Byte_gcc4net_V8QI (qi b)
{
  u8qi result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  result.a[4] = b;
  result.a[5] = b;
  result.a[6] = b;
  result.a[7] = b;
  return result;
}

u8qi
gcc4net_V8QI_V8QI_ctor2_System_ULong_gcc4net_V8QI (unsigned long f)
{
  u8qi result;
  result.a[0] = (qi) (f >> 56);
  result.a[1] = (qi) ((f >> 48) & 0xff);
  result.a[2] = (qi) ((f >> 40) & 0xff);
  result.a[3] = (qi) ((f >> 32) & 0xff);
  result.a[4] = (qi) ((f >> 24) & 0xff);
  result.a[5] = (qi) ((f >> 16) & 0xff);
  result.a[6] = (qi) ((f >> 8) & 0xff);
  result.a[7] = (qi) (f & 0xff);
  return result;
}

unsigned long
gcc4net_V8QI_to_udi_gcc4net_V8QI_System_ULong (u8qi v)
{
  return ((unsigned long) v.a[0] << 56) | ((unsigned long) v.a[1] << 48) |
    ((unsigned long) v.a[2] << 40) | ((unsigned long) v.a[3] << 32) |
    ((unsigned long) v.a[4] << 24) | ((unsigned long) v.a[5] << 16) |
    ((unsigned long) v.a[6] << 8) | (unsigned long) v.a[7];
}

long
gcc4net_V8QI_to_di_gcc4net_V8QI_System_Long (u8qi v)
{
  return (long) (((unsigned long) v.a[0] << 56) | ((unsigned long) v.a[1] << 48) |
		 ((unsigned long) v.a[2] << 40) | ((unsigned long) v.a[3] << 32) |
		 ((unsigned long) v.a[4] << 24) | ((unsigned long) v.a[5] << 16) |
		 ((unsigned long) v.a[6] << 8) | (unsigned long) v.a[7]);
}


u8qi
gcc4net_V8QI_add_gcc4net_V8QI_gcc4net_V8QI_gcc4net_V8QI (u8qi x, u8qi y)
{
  u8qi result;
  result.a[0] = (qi) (x.a[0] + y.a[0]);
  result.a[1] = (qi) (x.a[1] + y.a[1]);
  result.a[2] = (qi) (x.a[2] + y.a[2]);
  result.a[3] = (qi) (x.a[3] + y.a[3]);
  result.a[4] = (qi) (x.a[4] + y.a[4]);
  result.a[5] = (qi) (x.a[5] + y.a[5]);
  result.a[6] = (qi) (x.a[6] + y.a[6]);
  result.a[7] = (qi) (x.a[7] + y.a[7]);
  return result;
}

u8qi
gcc4net_V8QI_sub_gcc4net_V8QI_gcc4net_V8QI_gcc4net_V8QI (u8qi x, u8qi y)
{
  u8qi result;
  result.a[0] = (qi) (x.a[0] - y.a[0]);
  result.a[1] = (qi) (x.a[1] - y.a[1]);
  result.a[2] = (qi) (x.a[2] - y.a[2]);
  result.a[3] = (qi) (x.a[3] - y.a[3]);
  result.a[4] = (qi) (x.a[4] - y.a[4]);
  result.a[5] = (qi) (x.a[5] - y.a[5]);
  result.a[6] = (qi) (x.a[6] - y.a[6]);
  result.a[7] = (qi) (x.a[7] - y.a[7]);
  return result;
}

u8qi
gcc4net_V8QI_mul_gcc4net_V8QI_gcc4net_V8QI_gcc4net_V8QI (u8qi x, u8qi y)
{
  u8qi result;
  result.a[0] = (qi) (x.a[0] * y.a[0]);
  result.a[1] = (qi) (x.a[1] * y.a[1]);
  result.a[2] = (qi) (x.a[2] * y.a[2]);
  result.a[3] = (qi) (x.a[3] * y.a[3]);
  result.a[4] = (qi) (x.a[4] * y.a[4]);
  result.a[5] = (qi) (x.a[5] * y.a[5]);
  result.a[6] = (qi) (x.a[6] * y.a[6]);
  result.a[7] = (qi) (x.a[7] * y.a[7]);
  return result;
}

u8qi
gcc4net_V8QI_and_gcc4net_V8QI_gcc4net_V8QI_gcc4net_V8QI (u8qi x, u8qi y)
{
  u8qi result;
  result.a[0] = (qi) (x.a[0] & y.a[0]);
  result.a[1] = (qi) (x.a[1] & y.a[1]);
  result.a[2] = (qi) (x.a[2] & y.a[2]);
  result.a[3] = (qi) (x.a[3] & y.a[3]);
  result.a[4] = (qi) (x.a[4] & y.a[4]);
  result.a[5] = (qi) (x.a[5] & y.a[5]);
  result.a[6] = (qi) (x.a[6] & y.a[6]);
  result.a[7] = (qi) (x.a[7] & y.a[7]);
  return result;
}

u8qi
gcc4net_V8QI_or_gcc4net_V8QI_gcc4net_V8QI_gcc4net_V8QI (u8qi x, u8qi y)
{
  u8qi result;
  result.a[0] = (qi) (x.a[0] | y.a[0]);
  result.a[1] = (qi) (x.a[1] | y.a[1]);
  result.a[2] = (qi) (x.a[2] | y.a[2]);
  result.a[3] = (qi) (x.a[3] | y.a[3]);
  result.a[4] = (qi) (x.a[4] | y.a[4]);
  result.a[5] = (qi) (x.a[5] | y.a[5]);
  result.a[6] = (qi) (x.a[6] | y.a[6]);
  result.a[7] = (qi) (x.a[7] | y.a[7]);
  return result;
}

u8qi
gcc4net_V8QI_xor_gcc4net_V8QI_gcc4net_V8QI_gcc4net_V8QI (u8qi x, u8qi y)
{
  u8qi result;
  result.a[0] = (qi) (x.a[0] ^ y.a[0]);
  result.a[1] = (qi) (x.a[1] ^ y.a[1]);
  result.a[2] = (qi) (x.a[2] ^ y.a[2]);
  result.a[3] = (qi) (x.a[3] ^ y.a[3]);
  result.a[4] = (qi) (x.a[4] ^ y.a[4]);
  result.a[5] = (qi) (x.a[5] ^ y.a[5]);
  result.a[6] = (qi) (x.a[6] ^ y.a[6]);
  result.a[7] = (qi) (x.a[7] ^ y.a[7]);
  return result;
}

u4hi
gcc4net_V4HI_V4HI_ctor1_System_UInt16_System_UInt16_System_UInt16_System_UInt16_gcc4net_V4HI (unsigned short b0, unsigned short b1, unsigned short b2, unsigned short b3)
{
  u4hi result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  return result;
}

u4hi
gcc4net_V4HI_V4HI_ctor1_System_UInt16_gcc4net_V4HI (unsigned short b)
{
  u4hi result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  return result;
}

u4hi
gcc4net_V4HI_V4HI_ctor2 (unsigned long f)
{
  u4hi result;
  result.a[0] = (unsigned short) (f >> 48);
  result.a[1] = (unsigned short) ((f >> 32) & 0xffff);
  result.a[2] = (unsigned short) ((f >> 16) & 0xffff);
  result.a[3] = (unsigned short) (f & 0xffff);
  return result;
}

unsigned long
gcc4net_V4HI_to_udi (u4hi v)
{
  return ((unsigned long) v.a[0] << 48) | ((unsigned long) v.a[1] << 32) |
    ((unsigned long) v.a[2] << 16) | (unsigned long) v.a[3];
}

long
gcc4net_V4HI_to_di (u4hi v)
{
  return (long) (((unsigned long) v.a[0] << 48) | ((unsigned long) v.a[1] << 32) |
		 ((unsigned long) v.a[2] << 16) | (unsigned long) v.a[3]);
}

u4hi
gcc4net_V4HI_add_gcc4net_V4HI_gcc4net_V4HI_gcc4net_V4HI (u4hi x, u4hi y)
{
  u4hi result;
  result.a[0] = (unsigned short) (x.a[0] + y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] + y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] + y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] + y.a[3]);
  return result;
}

u4hi
gcc4net_V4HI_sub_gcc4net_V4HI_gcc4net_V4HI_gcc4net_V4HI (u4hi x, u4hi y)
{
  u4hi result;
  result.a[0] = (unsigned short) (x.a[0] - y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] - y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] - y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] - y.a[3]);
  return result;
}

u4hi
gcc4net_V4HI_mul_gcc4net_V4HI_gcc4net_V4HI_gcc4net_V4HI (u4hi x, u4hi y)
{
  u4hi result;
  result.a[0] = (unsigned short) (x.a[0] * y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] * y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] * y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] * y.a[3]);
  return result;
}

u4hi
gcc4net_V4HI_and_gcc4net_V4HI_gcc4net_V4HI_gcc4net_V4HI (u4hi x, u4hi y)
{
  u4hi result;
  result.a[0] = (unsigned short) (x.a[0] & y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] & y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] & y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] & y.a[3]);
  return result;
}

u4hi
gcc4net_V4HI_or_gcc4net_V4HI_gcc4net_V4HI_gcc4net_V4HI (u4hi x, u4hi y)
{
  u4hi result;
  result.a[0] = (unsigned short) (x.a[0] | y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] | y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] | y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] | y.a[3]);
  return result;
}

u4hi
gcc4net_V4HI_xor_gcc4net_V4HI_gcc4net_V4HI_gcc4net_V4HI (u4hi x, u4hi y)
{
  u4hi result;
  result.a[0] = (unsigned short) (x.a[0] ^ y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] ^ y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] ^ y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] ^ y.a[3]);
  return result;
}

u2si
gcc4net_V2SI_V2SI_ctor1_System_UInt32_System_UInt32_gcc4net_V2SI (unsigned int b0, unsigned int b1)
{
  u2si result;
  result.a[0] = b0;
  result.a[1] = b1;
  return result;
}

u2si
gcc4net_V2SI_V2SI_ctor1_System_UInt32_gcc4net_V2SI (unsigned int b)
{
  u2si result;
  result.a[0] = b;
  result.a[1] = b;
  return result;
}

u2si
gcc4net_V2SI_V2SI_ctor2 (unsigned long f)
{
  u2si result;
  result.a[0] = (unsigned int) (f >> 32);
  result.a[1] = (unsigned int) (f & 0xffff);
  return result;
}

unsigned long
u2si_to_udi (u2si v)
{
  return ((unsigned long) v.a[0] << 32) | (unsigned long) v.a[1];
}

long
u2si_to_di (u2si v)
{
  return (long) (((unsigned long) v.a[0] << 32) | (unsigned long) v.a[1]);
}


u2si
gcc4net_V2SI_add_gcc4net_V2SI_gcc4net_V2SI_gcc4net_V2SI (u2si x, u2si y)
{
  u2si result;
  result.a[0] = x.a[0] + y.a[0];
  result.a[1] = x.a[1] + y.a[1];
  return result;
}

u2si
gcc4net_V2SI_sub_gcc4net_V2SI_gcc4net_V2SI_gcc4net_V2SI (u2si x, u2si y)
{
  u2si result;
  result.a[0] = x.a[0] - y.a[0];
  result.a[1] = x.a[1] - y.a[1];
  return result;
}

u2si
gcc4net_V2SI_mul_gcc4net_V2SI_gcc4net_V2SI_gcc4net_V2SI (u2si x, u2si y)
{
  u2si result;
  result.a[0] = x.a[0] * y.a[0];
  result.a[1] = x.a[1] * y.a[1];
  return result;
}

u2si
gcc4net_V2SI_and_gcc4net_V2SI_gcc4net_V2SI_gcc4net_V2SI (u2si x, u2si y)
{
  u2si result;
  result.a[0] = x.a[0] & y.a[0];
  result.a[1] = x.a[1] & y.a[1];
  return result;
}

u2si
gcc4net_V2SI_or_gcc4net_V2SI_gcc4net_V2SI_gcc4net_V2SI (u2si x, u2si y)
{
  u2si result;
  result.a[0] = x.a[0] | y.a[0];
  result.a[1] = x.a[1] | y.a[1];
  return result;
}

u2si
gcc4net_V2SI_xor_gcc4net_V2SI_gcc4net_V2SI_gcc4net_V2SI (u2si x, u2si y)
{
  u2si result;
  result.a[0] = x.a[0] ^ y.a[0];
  result.a[1] = x.a[1] ^ y.a[1];
  return result;
}


u4si
gcc4net_V4SI_V4SI_ctor1_System_UInt32_System_UInt32_System_UInt32_System_UInt32_gcc4net_V4SI (unsigned int b0, unsigned int b1, unsigned int b2, unsigned int b3)
{
  u4si result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  return result;
}

u4si
gcc4net_V4SI_V4SI_ctor1_System_UInt32_gcc4net_V4SI (unsigned int b)
{
  u4si result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  return result;
}

u4si
gcc4net_V4SI_add_gcc4net_V4SI_gcc4net_V4SI_gcc4net_V4SI (u4si x, u4si y)
{
  u4si result;
  result.a[0] = x.a[0] + y.a[0];
  result.a[1] = x.a[1] + y.a[1];
  result.a[2] = x.a[2] + y.a[2];
  result.a[3] = x.a[3] + y.a[3];
  return result;
}

u4si
gcc4net_V4SI_sub_gcc4net_V4SI_gcc4net_V4SI_gcc4net_V4SI (u4si x, u4si y)
{
  u4si result;
  result.a[0] = x.a[0] - y.a[0];
  result.a[1] = x.a[1] - y.a[1];
  result.a[2] = x.a[2] - y.a[2];
  result.a[3] = x.a[3] - y.a[3];
  return result;
}

u4si
gcc4net_V4SI_mul_gcc4net_V4SI_gcc4net_V4SI_gcc4net_V4SI (u4si x, u4si y)
{
  u4si result;
  result.a[0] = x.a[0] * y.a[0];
  result.a[1] = x.a[1] * y.a[1];
  result.a[2] = x.a[2] * y.a[2];
  result.a[3] = x.a[3] * y.a[3];
  return result;
}

u4si
gcc4net_V4SI_and_gcc4net_V4SI_gcc4net_V4SI_gcc4net_V4SI (u4si x, u4si y)
{
  u4si result;
  result.a[0] = x.a[0] & y.a[0];
  result.a[1] = x.a[1] & y.a[1];
  result.a[2] = x.a[2] & y.a[2];
  result.a[3] = x.a[3] & y.a[3];
  return result;
}

u4si
gcc4net_V4SI_or_gcc4net_V4SI_gcc4net_V4SI_gcc4net_V4SI (u4si x, u4si y)
{
  u4si result;
  result.a[0] = x.a[0] | y.a[0];
  result.a[1] = x.a[1] | y.a[1];
  result.a[2] = x.a[2] | y.a[2];
  result.a[3] = x.a[3] | y.a[3];
  return result;
}

u4si
gcc4net_V4SI_xor_gcc4net_V4SI_gcc4net_V4SI_gcc4net_V4SI (u4si x, u4si y)
{
  u4si result;
  result.a[0] = x.a[0] ^ y.a[0];
  result.a[1] = x.a[1] ^ y.a[1];
  result.a[2] = x.a[2] ^ y.a[2];
  result.a[3] = x.a[3] ^ y.a[3];
  return result;
}

u8hi
  gcc4net_V8HI_V8HI_ctor1_System_UInt16_System_UInt16_System_UInt16_System_UInt16_System_UInt16_System_UInt16_System_UInt16_System_UInt16_gcc4net_V8HI
  (unsigned short b0, unsigned short b1,
   unsigned short b2, unsigned short b3, unsigned short b4, unsigned short b5, unsigned short b6, unsigned short b7)
{
  u8hi result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  result.a[4] = b4;
  result.a[5] = b5;
  result.a[6] = b6;
  result.a[7] = b7;
  return result;
}

u8hi
gcc4net_V8HI_V8HI_ctor1_System_UInt16_gcc4net_V8HI (unsigned short b)
{
  u8hi result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  result.a[4] = b;
  result.a[5] = b;
  result.a[6] = b;
  result.a[7] = b;
  return result;
}

u8hi
gcc4net_V8HI_add_gcc4net_V8HI_gcc4net_V8HI_gcc4net_V8HI (u8hi x, u8hi y)
{
  u8hi result;
  result.a[0] = (unsigned short) (x.a[0] + y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] + y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] + y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] + y.a[3]);
  result.a[4] = (unsigned short) (x.a[4] + y.a[4]);
  result.a[5] = (unsigned short) (x.a[5] + y.a[5]);
  result.a[6] = (unsigned short) (x.a[6] + y.a[6]);
  result.a[7] = (unsigned short) (x.a[7] + y.a[7]);
  return result;
}

u8hi
gcc4net_V8HI_sub_gcc4net_V8HI_gcc4net_V8HI_gcc4net_V8HI (u8hi x, u8hi y)
{
  u8hi result;
  result.a[0] = (unsigned short) (x.a[0] - y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] - y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] - y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] - y.a[3]);
  result.a[4] = (unsigned short) (x.a[4] - y.a[4]);
  result.a[5] = (unsigned short) (x.a[5] - y.a[5]);
  result.a[6] = (unsigned short) (x.a[6] - y.a[6]);
  result.a[7] = (unsigned short) (x.a[7] - y.a[7]);
  return result;
}

u8hi
gcc4net_V8HI_mul_gcc4net_V8HI_gcc4net_V8HI_gcc4net_V8HI (u8hi x, u8hi y)
{
  u8hi result;
  result.a[0] = (unsigned short) (x.a[0] * y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] * y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] * y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] * y.a[3]);
  result.a[4] = (unsigned short) (x.a[4] * y.a[4]);
  result.a[5] = (unsigned short) (x.a[5] * y.a[5]);
  result.a[6] = (unsigned short) (x.a[6] * y.a[6]);
  result.a[7] = (unsigned short) (x.a[7] * y.a[7]);
  return result;
}

u8hi
gcc4net_V8HI_and_gcc4net_V8HI_gcc4net_V8HI_gcc4net_V8HI (u8hi x, u8hi y)
{
  u8hi result;
  result.a[0] = (unsigned short) (x.a[0] & y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] & y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] & y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] & y.a[3]);
  result.a[4] = (unsigned short) (x.a[4] & y.a[4]);
  result.a[5] = (unsigned short) (x.a[5] & y.a[5]);
  result.a[6] = (unsigned short) (x.a[6] & y.a[6]);
  result.a[7] = (unsigned short) (x.a[7] & y.a[7]);
  return result;
}

u8hi
gcc4net_V8HI_or_gcc4net_V8HI_gcc4net_V8HI_gcc4net_V8HI (u8hi x, u8hi y)
{
  u8hi result;
  result.a[0] = (unsigned short) (x.a[0] | y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] | y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] | y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] | y.a[3]);
  result.a[4] = (unsigned short) (x.a[4] | y.a[4]);
  result.a[5] = (unsigned short) (x.a[5] | y.a[5]);
  result.a[6] = (unsigned short) (x.a[6] | y.a[6]);
  result.a[7] = (unsigned short) (x.a[7] | y.a[7]);
  return result;
}

u8hi
gcc4net_V8HI_xor_gcc4net_V8HI_gcc4net_V8HI_gcc4net_V8HI (u8hi x, u8hi y)
{
  u8hi result;
  result.a[0] = (unsigned short) (x.a[0] ^ y.a[0]);
  result.a[1] = (unsigned short) (x.a[1] ^ y.a[1]);
  result.a[2] = (unsigned short) (x.a[2] ^ y.a[2]);
  result.a[3] = (unsigned short) (x.a[3] ^ y.a[3]);
  result.a[4] = (unsigned short) (x.a[4] ^ y.a[4]);
  result.a[5] = (unsigned short) (x.a[5] ^ y.a[5]);
  result.a[6] = (unsigned short) (x.a[6] ^ y.a[6]);
  result.a[7] = (unsigned short) (x.a[7] ^ y.a[7]);
  return result;
}

u16qi
  gcc4net_V16QI_V16QI_ctor1_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_System_Byte_gcc4net_V16QI
  (qi b0, qi b1, qi b2, qi b3,
   qi b4, qi b5, qi b6, qi b7,
   qi b8, qi b9, qi b10, qi b11, qi b12, qi b13, qi b14, qi b15)
{
  u16qi result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  result.a[4] = b4;
  result.a[5] = b5;
  result.a[6] = b6;
  result.a[7] = b7;
  result.a[8] = b8;
  result.a[9] = b9;
  result.a[10] = b10;
  result.a[11] = b11;
  result.a[12] = b12;
  result.a[13] = b13;
  result.a[14] = b14;
  result.a[15] = b15;
  return result;
}

u16qi
gcc4net_V16QI_V16QI_ctor1_System_Byte_gcc4net_V16QI (qi b)
{
  u16qi result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  result.a[4] = b;
  result.a[5] = b;
  result.a[6] = b;
  result.a[7] = b;
  result.a[8] = b;
  result.a[9] = b;
  result.a[10] = b;
  result.a[11] = b;
  result.a[12] = b;
  result.a[13] = b;
  result.a[14] = b;
  result.a[15] = b;
  return result;
}

u16qi
gcc4net_V16QI_add_gcc4net_V16QI_gcc4net_V16QI_gcc4net_V16QI (u16qi x, u16qi y)
{
  u16qi result;
  result.a[0] = (qi) (x.a[0] + y.a[0]);
  result.a[1] = (qi) (x.a[1] + y.a[1]);
  result.a[2] = (qi) (x.a[2] + y.a[2]);
  result.a[3] = (qi) (x.a[3] + y.a[3]);
  result.a[4] = (qi) (x.a[4] + y.a[4]);
  result.a[5] = (qi) (x.a[5] + y.a[5]);
  result.a[6] = (qi) (x.a[6] + y.a[6]);
  result.a[7] = (qi) (x.a[7] + y.a[7]);
  result.a[8] = (qi) (x.a[8] + y.a[8]);
  result.a[9] = (qi) (x.a[9] + y.a[9]);
  result.a[10] = (qi) (x.a[10] + y.a[10]);
  result.a[11] = (qi) (x.a[11] + y.a[11]);
  result.a[12] = (qi) (x.a[12] + y.a[12]);
  result.a[13] = (qi) (x.a[13] + y.a[13]);
  result.a[14] = (qi) (x.a[14] + y.a[14]);
  result.a[15] = (qi) (x.a[15] + y.a[15]);
  return result;
}

u16qi
gcc4net_V16QI_sub_gcc4net_V16QI_gcc4net_V16QI_gcc4net_V16QI (u16qi x, u16qi y)
{
  u16qi result;
  result.a[0] = (qi) (x.a[0] - y.a[0]);
  result.a[1] = (qi) (x.a[1] - y.a[1]);
  result.a[2] = (qi) (x.a[2] - y.a[2]);
  result.a[3] = (qi) (x.a[3] - y.a[3]);
  result.a[4] = (qi) (x.a[4] - y.a[4]);
  result.a[5] = (qi) (x.a[5] - y.a[5]);
  result.a[6] = (qi) (x.a[6] - y.a[6]);
  result.a[7] = (qi) (x.a[7] - y.a[7]);
  result.a[8] = (qi) (x.a[8] - y.a[8]);
  result.a[9] = (qi) (x.a[9] - y.a[9]);
  result.a[10] = (qi) (x.a[10] - y.a[10]);
  result.a[11] = (qi) (x.a[11] - y.a[11]);
  result.a[12] = (qi) (x.a[12] - y.a[12]);
  result.a[13] = (qi) (x.a[13] - y.a[13]);
  result.a[14] = (qi) (x.a[14] - y.a[14]);
  result.a[15] = (qi) (x.a[15] - y.a[15]);
  return result;
}

u16qi
gcc4net_V16QI_mul_gcc4net_V16QI_gcc4net_V16QI_gcc4net_V16QI (u16qi x, u16qi y)
{
  u16qi result;
  result.a[0] = (qi) (x.a[0] * y.a[0]);
  result.a[1] = (qi) (x.a[1] * y.a[1]);
  result.a[2] = (qi) (x.a[2] * y.a[2]);
  result.a[3] = (qi) (x.a[3] * y.a[3]);
  result.a[4] = (qi) (x.a[4] * y.a[4]);
  result.a[5] = (qi) (x.a[5] * y.a[5]);
  result.a[6] = (qi) (x.a[6] * y.a[6]);
  result.a[7] = (qi) (x.a[7] * y.a[7]);
  result.a[8] = (qi) (x.a[8] * y.a[8]);
  result.a[9] = (qi) (x.a[9] * y.a[9]);
  result.a[10] = (qi) (x.a[10] * y.a[10]);
  result.a[11] = (qi) (x.a[11] * y.a[11]);
  result.a[12] = (qi) (x.a[12] * y.a[12]);
  result.a[13] = (qi) (x.a[13] * y.a[13]);
  result.a[14] = (qi) (x.a[14] * y.a[14]);
  result.a[15] = (qi) (x.a[15] * y.a[15]);
  return result;
}

u16qi
gcc4net_V16QI_and_gcc4net_V16QI_gcc4net_V16QI_gcc4net_V16QI (u16qi x, u16qi y)
{
  u16qi result;
  result.a[0] = (qi) (x.a[0] & y.a[0]);
  result.a[1] = (qi) (x.a[1] & y.a[1]);
  result.a[2] = (qi) (x.a[2] & y.a[2]);
  result.a[3] = (qi) (x.a[3] & y.a[3]);
  result.a[4] = (qi) (x.a[4] & y.a[4]);
  result.a[5] = (qi) (x.a[5] & y.a[5]);
  result.a[6] = (qi) (x.a[6] & y.a[6]);
  result.a[7] = (qi) (x.a[7] & y.a[7]);
  result.a[8] = (qi) (x.a[8] & y.a[8]);
  result.a[9] = (qi) (x.a[9] & y.a[9]);
  result.a[10] = (qi) (x.a[10] & y.a[10]);
  result.a[11] = (qi) (x.a[11] & y.a[11]);
  result.a[12] = (qi) (x.a[12] & y.a[12]);
  result.a[13] = (qi) (x.a[13] & y.a[13]);
  result.a[14] = (qi) (x.a[14] & y.a[14]);
  result.a[15] = (qi) (x.a[15] & y.a[15]);
  return result;
}

u16qi
gcc4net_V16QI_or_gcc4net_V16QI_gcc4net_V16QI_gcc4net_V16QI (u16qi x, u16qi y)
{
  u16qi result;
  result.a[0] = (qi) (x.a[0] | y.a[0]);
  result.a[1] = (qi) (x.a[1] | y.a[1]);
  result.a[2] = (qi) (x.a[2] | y.a[2]);
  result.a[3] = (qi) (x.a[3] | y.a[3]);
  result.a[4] = (qi) (x.a[4] | y.a[4]);
  result.a[5] = (qi) (x.a[5] | y.a[5]);
  result.a[6] = (qi) (x.a[6] | y.a[6]);
  result.a[7] = (qi) (x.a[7] | y.a[7]);
  result.a[8] = (qi) (x.a[8] | y.a[8]);
  result.a[9] = (qi) (x.a[9] | y.a[9]);
  result.a[10] = (qi) (x.a[10] | y.a[10]);
  result.a[11] = (qi) (x.a[11] | y.a[11]);
  result.a[12] = (qi) (x.a[12] | y.a[12]);
  result.a[13] = (qi) (x.a[13] | y.a[13]);
  result.a[14] = (qi) (x.a[14] | y.a[14]);
  result.a[15] = (qi) (x.a[15] | y.a[15]);
  return result;
}

u16qi
gcc4net_V16QI_xor_gcc4net_V16QI_gcc4net_V16QI_gcc4net_V16QI (u16qi x, u16qi y)
{
  u16qi result;
  result.a[0] = (qi) (x.a[0] ^ y.a[0]);
  result.a[1] = (qi) (x.a[1] ^ y.a[1]);
  result.a[2] = (qi) (x.a[2] ^ y.a[2]);
  result.a[3] = (qi) (x.a[3] ^ y.a[3]);
  result.a[4] = (qi) (x.a[4] ^ y.a[4]);
  result.a[5] = (qi) (x.a[5] ^ y.a[5]);
  result.a[6] = (qi) (x.a[6] ^ y.a[6]);
  result.a[7] = (qi) (x.a[7] ^ y.a[7]);
  result.a[8] = (qi) (x.a[8] ^ y.a[8]);
  result.a[9] = (qi) (x.a[9] ^ y.a[9]);
  result.a[10] = (qi) (x.a[10] ^ y.a[10]);
  result.a[11] = (qi) (x.a[11] ^ y.a[11]);
  result.a[12] = (qi) (x.a[12] ^ y.a[12]);
  result.a[13] = (qi) (x.a[13] ^ y.a[13]);
  result.a[14] = (qi) (x.a[14] ^ y.a[14]);
  result.a[15] = (qi) (x.a[15] ^ y.a[15]);
  return result;
}


u2sf
gcc4net_V2SF_V2SF_ctor1_System_Single_System_Single_gcc4net_V2SF (float b0,
								float b1)
{
  u2sf result;
  result.a[0] = b0;
  result.a[1] = b1;
  return result;
}

u2sf
gcc4net_V2SF_V2SF_ctor1_System_Single_gcc4net_V2SF (float b)
{
  u2sf result;
  result.a[0] = b;
  result.a[1] = b;
  return result;
}

long
gcc4net_V2SF_to_di_gcc4net_V2SF_System_Long (u2sf v)
{
  unsigned int a0 = *(unsigned int *) & v.a[0];
  unsigned int a1 = *(unsigned int *) & v.a[1];
  return (long) (((unsigned long) a0 << 32) | (unsigned long) a1);
}

u2sf
gcc4net_V2SF_add_gcc4net_V2SF_gcc4net_V2SF_gcc4net_V2SF (u2sf x, u2sf y)
{
  u2sf result;
  result.a[0] = x.a[0] + y.a[0];
  result.a[1] = x.a[1] + y.a[1];
  return result;
}

u2sf
gcc4net_V2SF_sub_gcc4net_V2SF_gcc4net_V2SF_gcc4net_V2SF (u2sf x, u2sf y)
{
  u2sf result;
  result.a[0] = x.a[0] - y.a[0];
  result.a[1] = x.a[1] - y.a[1];
  return result;
}

u2sf
gcc4net_V2SF_mul_gcc4net_V2SF_gcc4net_V2SF_gcc4net_V2SF (u2sf x, u2sf y)
{
  u2sf result;
  result.a[0] = x.a[0] * y.a[0];
  result.a[1] = x.a[1] * y.a[1];
  return result;
}


u4sf
gcc4net_V4SF_V4SF_ctor1_System_Single_System_Single_System_Single_System_Single_gcc4net_V4SF
  (float b0, float b1, float b2, float b3)
{
  u4sf result;
  result.a[0] = b0;
  result.a[1] = b1;
  result.a[2] = b2;
  result.a[3] = b3;
  return result;
}

u4sf
gcc4net_V4SF_V4SF_ctor1_System_Single_gcc4net_V4SF (float b)
{
  u4sf result;
  result.a[0] = b;
  result.a[1] = b;
  result.a[2] = b;
  result.a[3] = b;
  return result;
}

u4sf
gcc4net_V4SF_add_gcc4net_V4SF_gcc4net_V4SF_gcc4net_V4SF (u4sf x, u4sf y)
{
  u4sf result;
  result.a[0] = x.a[0] + y.a[0];
  result.a[1] = x.a[1] + y.a[1];
  result.a[2] = x.a[2] + y.a[2];
  result.a[3] = x.a[3] + y.a[3];
  return result;
}

u4sf
gcc4net_V4SF_sub_gcc4net_V4SF_gcc4net_V4SF_gcc4net_V4SF (u4sf x, u4sf y)
{
  u4sf result;
  result.a[0] = x.a[0] - y.a[0];
  result.a[1] = x.a[1] - y.a[1];
  result.a[2] = x.a[2] - y.a[2];
  result.a[3] = x.a[3] - y.a[3];
  return result;
}

u4sf
gcc4net_V4SF_mul_gcc4net_V4SF_gcc4net_V4SF_gcc4net_V4SF (u4sf x, u4sf y)
{
  u4sf result;
  result.a[0] = x.a[0] * y.a[0];
  result.a[1] = x.a[1] * y.a[1];
  result.a[2] = x.a[2] * y.a[2];
  result.a[3] = x.a[3] * y.a[3];
  return result;
}

u4sf
gcc4net_V4SI_u4si_to_u4sf_gcc4net_V4SI_gcc4net_V4SI (u4si v)
{
  return gcc4net_V4SF_V4SF_ctor1_System_Single_System_Single_System_Single_System_Single_gcc4net_V4SF (*(float *) &v.a[0], *(float *) &v.a[1],
			  *(float *) &v.a[2], *(float *) &v.a[3]);
}

u2df
gcc4net_V2DF_V2DF_ctor1_System_Double_System_Double_gcc4net_V2DF (double b0,
								  double b1)
{
  u2df result;
  result.a[0] = b0;
  result.a[1] = b1;
  return result;
}

u2df
gcc4net_V2DF_V2DF_ctor1_System_Double_gcc4net_V2DF (double b)
{
  u2df result;
  result.a[0] = b;
  result.a[1] = b;
  return result;
}


u2df
gcc4net_V2DF_add_gcc4net_V2DF_gcc4net_V2DF_gcc4net_V2DF (u2df x, u2df y)
{
  u2df result;
  result.a[0] = x.a[0] + y.a[0];
  result.a[1] = x.a[1] + y.a[1];
  return result;
}

u2df
gcc4net_V2DF_sub_gcc4net_V2DF_gcc4net_V2DF_gcc4net_V2DF (u2df x, u2df y)
{
  u2df result;
  result.a[0] = x.a[0] - y.a[0];
  result.a[1] = x.a[1] - y.a[1];
  return result;
}

u2df
gcc4net_V2DF_mul_gcc4net_V2DF_gcc4net_V2DF_gcc4net_V2DF (u2df x, u2df y)
{
  u2df result;
  result.a[0] = x.a[0] * y.a[0];
  result.a[1] = x.a[1] * y.a[1];
  return result;
}
