/* Run-time support required by CIL binaries.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

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
  Kevin Williams <kevin.williams@inria.fr>
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

void
gcc4net_StartupHelper_Shutdown_System_Int32_System_Void (int eval)
{
  /* TODO */
}

void
System_Environment_Exit_System_Int32_System_Void(int n)
{
  exit(n);
}

int
gcc4net_Crt___minsi3_System_Int32_System_Int32_System_Int32 (int a, int b)
{
  return (a <= b) ? a : b;
}

long
gcc4net_Crt___mindi3_System_Long_System_Long_System_Long (long a, long b)
{
  return (a <= b) ? a : b;
}

unsigned int
gcc4net_Crt___uminsi3_System_UInt32_System_UInt32_System_UInt32 (unsigned int a, unsigned int b)
{
  return (a <= b) ? a : b;
}

unsigned long
gcc4net_Crt___umindi3_System_ULong_System_ULong_System_ULong (unsigned long a, unsigned long b)
{
  return (a <= b) ? a : b;
}

float
gcc4net_Crt___minsf3_System_Single_System_Single_System_Single (float a, float b)
{
  return (a <= b) ? a : b;
}

double
gcc4net_Crt___mindf3_System_Double_System_Double_System_Double (double a, double b)
{
  return (a <= b) ? a : b;
}

int
gcc4net_Crt___maxsi3_System_Int32_System_Int32_System_Int32 (int a, int b)
{
  return (a >= b) ? a : b;
}

long
gcc4net_Crt___maxdi3_System_Long_System_Long_System_Long (long a, long b)
{
  return (a >= b) ? a : b;
}

unsigned int
gcc4net_Crt___umaxsi3_System_UInt32_System_UInt32_System_UInt32 (unsigned int a, unsigned int b)
{
  return (a >= b) ? a : b;
}

unsigned long
gcc4net_Crt___umaxdi3_System_ULong_System_ULong_System_ULong (unsigned long a, unsigned long b)
{
  return (a >= b) ? a : b;
}

float
gcc4net_Crt___maxsf3_System_Single_System_Single_System_Single (float a, float b)
{
  return (a >= b) ? a : b;
}

double
gcc4net_Crt___maxdf3_System_Double_System_Double_System_Double (double a, double b)
{
  return (a >= b) ? a : b;
}

int
gcc4net_Crt___abssi2_System_Int32_System_Int32 (int a)
{
  return (a >= 0) ? a : -a;
}

long
gcc4net_Crt___absdi2_System_Long_System_Long (long a)
{
  return (a >= 0) ? a : -a;
}

float
gcc4net_Crt___abssf2_System_Single_System_Single (float a)
{
  return (a >= 0) ? a : -a;
}

double
gcc4net_Crt___absdf2_System_Double_System_Double (double a)
{
  return (a >= 0) ? a : -a;
}

int
gcc4net_Crt___selectsi4_System_Int32_System_Int32_System_Int32_System_Int32 (int a, int b, int c)
{
  return (a != 0) ? b : c;
}

long
gcc4net_Crt___selectdi4_System_Int32_System_Long_System_Long_System_Long (int a, long b, long c)
{
  return (a != 0) ? b : c;
}

float
gcc4net_Crt___selectsf4_System_Int32_System_Single_System_Single_System_Single (int a, float b, float c)
{
  return (a != 0) ? b : c;
}

double
gcc4net_Crt___selectdf4_System_Int32_System_Double_System_Double_System_Double (int a, double b, double c)
{
  return (a != 0) ? b : c;
}

// Returns the number of leading 0-bits in a, starting at the most
// significant bit position. If a is 0, the result is undefined.
int
gcc4net_Crt___clzsi2_System_UInt32_System_Int32 (unsigned int a)
{
  if (a == 0)
    return 32;			/* __builtin_clz(0) is undefined, anyway */
  int n = 0;
  if (a <= 0x0000ffff)
    {
      n += 16;
      a = a << 16;
    }
  if (a <= 0x00ffffff)
    {
      n += 8;
      a = a << 8;
    }
  if (a <= 0x0fffffff)
    {
      n += 4;
      a = a << 4;
    }
  if (a <= 0x3fffffff)
    {
      n += 2;
      a = a << 2;
    }
  if (a <= 0x7fffffff)
    {
      ++n;
    }
  return n;
}

int
gcc4net_Crt___clzdi2_System_ULong_System_Int32 (unsigned long a)
{
  if (a == 0)
    return 64;			/* __builtin_clz(0) is undefined, anyway */
  int n = 0;
  if (a <= 0x00000000ffffffff)
    {
      n += 32;
      a = a << 32;
    }
  if (a <= 0x0000ffffffffffff)
    {
      n += 16;
      a = a << 16;
    }
  if (a <= 0x00ffffffffffffff)
    {
      n += 8;
      a = a << 8;
    }
  if (a <= 0x0fffffffffffffff)
    {
      n += 4;
      a = a << 4;
    }
  if (a <= 0x3fffffffffffffff)
    {
      n += 2;
      a = a << 2;
    }
  if (a <= 0x7fffffffffffffff)
    {
      ++n;
    }
  return n;
}

// Returns the number of trailing 0-bits in a, starting at the least
// significant bit position. If a is 0, the result is undefined.
int
gcc4net_Crt___ctzsi2_System_UInt32_System_Int32 (unsigned int a)
{
  if (a == 0)
    return 32;			/* __builtin_ctz(0) is undefined, anyway */
  int n = 1;
  if ((a & 0x0000ffff) == 0)
    {
      n += 16;
      a = a >> 16;
    }
  if ((a & 0x000000ff) == 0)
    {
      n += 8;
      a = a >> 8;
    }
  if ((a & 0x0000000f) == 0)
    {
      n += 4;
      a = a >> 4;
    }
  if ((a & 0x00000003) == 0)
    {
      n += 2;
      a = a >> 2;
    }
  return n - (int) (a & 1);
}

int
gcc4net_Crt___ctzdi2_System_ULong_System_Int32 (unsigned long a)
{
  if (a == 0)
    return 64;			/* __builtin_ctz(0) is undefined, anyway */
  int n = 1;
  if ((a & 0x00000000ffffffff) == 0)
    {
      n += 32;
      a = a >> 32;
    }
  if ((a & 0x000000000000ffff) == 0)
    {
      n += 16;
      a = a >> 16;
    }
  if ((a & 0x00000000000000ff) == 0)
    {
      n += 8;
      a = a >> 8;
    }
  if ((a & 0x000000000000000f) == 0)
    {
      n += 4;
      a = a >> 4;
    }
  if ((a & 0x0000000000000003) == 0)
    {
      n += 2;
      a = a >> 2;
    }
  return (n - (int) (a & 1));
}

	// Returns one plus the index of the least significant 1-bit of a, or
	// if a is zero, returns zero.
	// Should take unsigned int, but __builtin_ffs defines it as taking int.
int
gcc4net_Crt___ffssi2_System_Int32_System_Int32 (int a)
{
  int i;
  if (a == 0)
    return 0;
  for (i = 0; i < 32; i++)
    if ((a & (1 << i)) != 0)
      break;
  return i + 1;
}

	// Should take unsigned long, but __builtin_ffs defines it as taking long.
int
gcc4net_Crt___ffsdi2_System_Long_System_Int32 (long a)
{
  int i;
  if (a == 0)
    return 0;
  for (i = 0; i < 64; i++)
    if ((a & ((long) 1 << i)) != 0)
      break;
  return i + 1;
}

	// Returns the parity of a, i.e. the number of 1-bits in a modulo 2.
int
gcc4net_Crt___paritysi2_System_UInt32_System_Int32 (unsigned int a)
{
  unsigned int n;
  n = a ^ (a >> 1);
  n = n ^ (n >> 2);
  n = n ^ (n >> 4);
  n = n ^ (n >> 8);
  n = n ^ (n >> 16);
  return (int) (n & 1);
}

int
gcc4net_Crt___paritydi2_System_ULong_System_Int32 (unsigned long a)
{
  unsigned long n;
  n = a ^ (a >> 1);
  n = n ^ (n >> 2);
  n = n ^ (n >> 4);
  n = n ^ (n >> 8);
  n = n ^ (n >> 16);
  n = n ^ (n >> 32);
  return (int) (n & 1);
}

	// Returns the number of 1-bits in a.
int
gcc4net_Crt___popcountsi2_System_UInt32_System_Int32 (unsigned int a)
{
  a = (a & 0x55555555) + ((a >> 1) & 0x55555555);
  a = (a & 0x33333333) + ((a >> 2) & 0x33333333);
  a = (a & 0x0f0f0f0f) + ((a >> 4) & 0x0f0f0f0f);
  a = (a & 0x00ff00ff) + ((a >> 8) & 0x00ff00ff);
  a = (a & 0x0000ffff) + ((a >> 16) & 0x0000ffff);
  return (int) a;
}

int
gcc4net_Crt___popcountdi2_System_ULong_System_Int32 (unsigned long a)
{
  a = (a & 0x5555555555555555) + ((a >> 1) & 0x5555555555555555);
  a = (a & 0x3333333333333333) + ((a >> 2) & 0x3333333333333333);
  a = (a & 0x0f0f0f0f0f0f0f0f) + ((a >> 4) & 0x0f0f0f0f0f0f0f0f);
  a = (a & 0x00ff00ff00ff00ff) + ((a >> 8) & 0x00ff00ff00ff00ff);
  a = (a & 0x0000ffff0000ffff) + ((a >> 16) & 0x0000ffff0000ffff);
  a = (a & 0x00000000ffffffff) + ((a >> 32) & 0x00000000ffffffff);
  return (int) a;
}

char
gcc4net_Crt___isLittleEndian_System_Boolean ()
{
  // big endian:    3f f0 00 00 00 00 00 00
  // little endian: 00 00 00 00 00 00 f0 3f
  double d = 1.0;
  char *b = (char *) & d;
  return b[0] == 0;
}

void*
gcc4net_Crt___EndianSelect_System_Void__System_Void__System_Void_ (void *a1, void *a2)
{
  // big endian:    3f f0 00 00 00 00 00 00
  // little endian: 00 00 00 00 00 00 f0 3f
  double d = 1.0;
  char *b = (char *) & d;
  if (b[0] == 0)
    return a1;
  else
    return a2;
}
