/* Implementation of the RANDOM intrinsics
   Copyright 2002 Free Software Foundation, Inc.
   Contributed by Lars Segerlund <seger@linuxmail.org>

This file is part of the GNU Fortran 95 runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

Ligbfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with libgfor; see the file COPYING.LIB.  If not,
write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include "libgfortran.h"

/*Use the 'big' generator by default ( period -> 2**19937 ).  */

#define MT19937

/* Define the necessary constants for the algorithm.  */

#ifdef  MT19937
enum constants
{
  N = 624, M = 397, R = 19, TU = 11, TS = 7, TT = 15, TL = 17
};
#define M_A	0x9908B0DF
#define T_B	0x9D2C5680
#define T_C	0xEFC60000
#else
enum constants
{
  N = 351, M = 175, R = 19, TU = 11, TS = 7, TT = 15, TL = 17
};
#define M_A	0xE4BD75F5
#define T_B	0x655E5280
#define T_C	0xFFD58000
#endif

static int i = N;
static unsigned int seed[N];

/* This is the routine which handles the seeding of the generator,
   and also reading and writing of the seed.  */

#define random_seed prefix(random_seed)
void
random_seed (GFC_INTEGER_4 * size, const gfc_array_i4 * put,
	     const gfc_array_i4 * get)
{
  /* Return the size of the seed.  */
  if (size != NULL)
    *size = N;

  /* Set the seed to PUT data.  */
  if (put != NULL)
    {
      abort ();
      for (i = 0; i < N; i++)
	seed[i] = put->data[i];
    }

  /* Return the seed to GET data.  */
  if (get != NULL)
    {
      abort ();
      for (i = 0; i < N; i++)
	get->data[i] = seed[i];
    }

  /* Initialize the seed in system dependent manner.  */
  if (get == NULL && put == NULL && size == NULL)
    {
      int fd;
      fd = open ("/dev/urandom", O_RDONLY);
      if (fd == 0)
	{
	  /* We dont have urandom.  */
	  GFC_UINTEGER_4 s = (GFC_UINTEGER_4) seed;
	  for (i = 0; i < N; i++)
	    {
	      s = s * 29943829 - 1;
	      seed[i] = s;
	    }
	}
      else
	{
	  /* Using urandom, might have a length issue.  */
	  read (fd, &seed[0], sizeof (GFC_UINTEGER_4) * N);
	  close (fd);
	}
    }
}

/* Here is the internal routine which generates the random numbers
   in 'batches' based upon the need for a new batch.
   It's an integer based routine known as 'Mersenne Twister'.
   This implementation still lacks 'tempering' and a good verification,
   but gives very good metrics.  */

static void
random_generate (void)
{
  /* 32 bit's */
  GFC_UINTEGER_4 y;

  /* Generate batch of N.  */
  int k, m;
  for (k = 0, m = M; k < N - 1; k++)
    {
      y = (seed[k] & (-1 << R)) | (seed[k + 1] & ((1u << R) - 1));
      seed[k] = seed[m] ^ (y >> 1) ^ (-(GFC_INTEGER_4) (y & 1) & M_A);
      if (++m >= N)
	m = 0;
    }

  y = (seed[N - 1] & (-1 << R)) | (seed[0] & ((1u << R) - 1));
  seed[N - 1] = seed[M - 1] ^ (y >> 1) ^ (-(GFC_INTEGER_4) (y & 1) & M_A);
  i = 0;
}

/* A routine to return a REAL(KIND=4).  */

#define random_r4 prefix(random_r4)
void
random_r4 (GFC_REAL_4 * harv)
{
  /* Regenerate if we need to.  */
  if (i >= N)
    random_generate ();

  /* Convert uint32 to REAL(KIND=4).  */
  *harv = (GFC_REAL_4) ((GFC_REAL_4) (GFC_UINTEGER_4) seed[i++] /
			(GFC_REAL_4) (~(GFC_UINTEGER_4) 0));
}

/* A routine to return a REAL(KIND=8).  */

#define random_r8 prefix(random_r8)
void
random_r8 (GFC_REAL_8 * harv)
{
  /* Regenerate if we need to, may waste one value (32 bit).  */
  if ((i + 1) >= N)
    random_generate ();

  /* Convert two uint32 to a REAL(KIND=8).  */
  *harv = ((GFC_REAL_8) ((((GFC_UINTEGER_8) seed[i+1]) << 32) + seed[i])) /
	  (GFC_REAL_8) (~(GFC_UINTEGER_8) 0);
  i += 2;
}

/* Code to handle arrays will follow here.  */

/* REAL(KIND=4) REAL array.  */

#define arandom_r4 prefix(arandom_r4)
void
arandom_r4 (gfc_array_r4 * harv)
{
  abort ();
}

/* REAL(KIND=8) array.  */

#define arandom_r8 prefix(arandom_r8)
void
arandom_r8 (gfc_array_r8 * harv)
{
  abort ();
}

