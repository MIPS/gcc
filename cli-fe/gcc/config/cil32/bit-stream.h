/* Bit-stream emitter definitions for GNU compiler.

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

#ifndef BIT_STREAM_H
#define BIT_STREAM_H

typedef void (*emit_byte)(unsigned char);

struct bit_stream_emitter
{
  emit_byte eb_fun;
  int num_bits;
  unsigned char cur_byte;
};

void
bit_stream_emitter_init (struct bit_stream_emitter *, emit_byte);

void
bit_stream_emitter_fini (struct bit_stream_emitter *);

void
bit_stream_emitter_push_bits (struct bit_stream_emitter *,
                              int, unsigned int);

void
bit_stream_emitter_pad_byte (struct bit_stream_emitter *);

#endif  /* BIT_STREAM_H */
