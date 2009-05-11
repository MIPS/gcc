/* Utility to handle bit streams

   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

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
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "bit-stream.h"


void
bit_stream_emitter_init (struct bit_stream_emitter *bse, emit_byte fun)
{
  bse->eb_fun = fun;
  bse->num_bits = 0;
  bse->cur_byte = 0;
}

void
bit_stream_emitter_fini (struct bit_stream_emitter *bse)
{
  bse->eb_fun = NULL;
  bse->num_bits = 0;
  bse->cur_byte = 0;
}

void
bit_stream_emitter_push_bits (struct bit_stream_emitter *bse,
                              int num_bits, unsigned int bits)
{
  gcc_assert (bse->num_bits >= 0 && bse->num_bits < 8);
  gcc_assert (num_bits > 0 && num_bits <= 16);
  gcc_assert (bits < (2U << num_bits));

  while (bse->num_bits + num_bits >= 8)
    {
      int emitted_bits = 8 - bse->num_bits;
      unsigned char in_bits = bits >> (num_bits - emitted_bits);

      bse->eb_fun ((bse->cur_byte << emitted_bits) | in_bits);
      bse->num_bits = 0;
      bse->cur_byte = 0;

      num_bits -= emitted_bits;
      bits &= (2U << num_bits) - 1;
    }

  if (num_bits > 0)
    {
      bse->cur_byte = (bse->cur_byte << num_bits) | bits;
      bse->num_bits += num_bits;
    }

  gcc_assert (bse->num_bits >= 0 && bse->num_bits < 8);
}

void
bit_stream_emitter_pad_byte (struct bit_stream_emitter *bse)
{
  gcc_assert (bse->num_bits >= 0 && bse->num_bits < 8);

  if (bse->num_bits > 0)
    {
      bse->eb_fun (bse->cur_byte << (8 - bse->num_bits));
      bse->num_bits = 0;
      bse->cur_byte = 0;
    }
}
