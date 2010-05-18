/*
   Copyright (C) 2010 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!

   Authors:
     Kevin Williams <kevin.williams@inria.fr>

   Contact information at STMicroelectronics:
     Andrea C. Ornstein <andrea.ornstein@st.com>
     Erven Rohou        <erven.rohou@st.com>
 */

#include <stdio.h>

#include "genvec-vector-defs.h"

unsigned int
genvec_support_VQI_get_vec_size_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VQI_stride_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VQI_VQI_align_System_UInt32()
{
  return 16;
}

v16qi
genvec_support_VQI_VQI_uniform_vec_System_SByte_Mono_Simd_Vector16sb (signed char x)
{
  u16qi vec_result;
  int i;
  for(i=0;i<16;i++)
    vec_result.a[i] = x;
  return vec_result.v;
}

v16qi
genvec_support_VQI_VQI_init_vec_System_SByte_System_SByte_Mono_Simd_Vector16sb (signed char a, signed char b)
{
  u16qi vec_result;
  int i;
  vec_result.a[0] = a;
  for(i=1;i<16;i++)
    vec_result.a[i] = b;
  return vec_result.v;
}

v16qi
genvec_support_VQI_VQI_affine_vec_System_SByte_System_SByte_Mono_Simd_Vector16sb (signed char bas, signed char step)
{
  u16qi vec_result;
  int i;
  for(i=0;i<16;i++)
    vec_result.a[i] = (signed char)(bas + i*step);
  return vec_result.v;
}

v16qi
genvec_support_VQI_ALoad_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb (u16qi* addr)
{
  int addr_val = (int)addr;
  int rounded_addr = addr_val & ~0xf;
  return *(v16qi*)rounded_addr;
}

v16qi
genvec_support_VQI_VQI_reduc_plus_epilogue_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb(u16qi v)
{
  signed char result = (signed char)(v.a[0] + v.a[1] + v.a[2] + v.a[3]
                                                                    + v.a[4] + v.a[5] + v.a[6] + v.a[7]
                                                                                                     + v.a[8] + v.a[9] + v.a[10] + v.a[11]
                                                                                                                                       + v.a[12] + v.a[13] + v.a[14] + v.a[15]);
  u16qi vec_result;
  int i;
  vec_result.a[0] = result;
  for(i=1;i<16;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v16qi
genvec_support_VQI_VQI_reduc_min_epilogue_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb(u16qi v)
{
  signed char tmp0_0 = v.a[0] < v.a[1] ? v.a[0] : v.a[1];
  signed char tmp0_1 = v.a[2] < v.a[3] ? v.a[2] : v.a[3];
  signed char tmp0_2 = v.a[4] < v.a[5] ? v.a[4] : v.a[5];
  signed char tmp0_3 = v.a[6] < v.a[7] ? v.a[6] : v.a[7];
  signed char tmp0_4 = v.a[8] < v.a[9] ? v.a[8] : v.a[9];
  signed char tmp0_5 = v.a[10] < v.a[11] ? v.a[10] : v.a[11];
  signed char tmp0_6 = v.a[12] < v.a[13] ? v.a[12] : v.a[13];
  signed char tmp0_7 = v.a[14] < v.a[15] ? v.a[14] : v.a[15];

  signed char tmp1_0 = tmp0_0 < tmp0_1 ? tmp0_0 : tmp0_1;
  signed char tmp1_1 = tmp0_2 < tmp0_3 ? tmp0_2 : tmp0_3;
  signed char tmp1_2 = tmp0_4 < tmp0_5 ? tmp0_4 : tmp0_5;
  signed char tmp1_3 = tmp0_6 < tmp0_7 ? tmp0_6 : tmp0_7;

  signed char tmp2_0 = tmp1_0 < tmp1_1 ? tmp1_0 : tmp1_1;
  signed char tmp2_1 = tmp1_2 < tmp1_3 ? tmp1_2 : tmp1_3;

  signed char result = tmp2_0 < tmp2_1 ? tmp2_0 : tmp2_1;

  u16qi vec_result;
  int i;
  vec_result.a[0] = result;
  for(i=1;i<16;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v16qi
genvec_support_VQI_VQI_reduc_max_epilogue_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb(u16qi v)
{
  signed char tmp0_0 = v.a[0] > v.a[1] ? v.a[0] : v.a[1];
  signed char tmp0_1 = v.a[2] > v.a[3] ? v.a[2] : v.a[3];
  signed char tmp0_2 = v.a[4] > v.a[5] ? v.a[4] : v.a[5];
  signed char tmp0_3 = v.a[6] > v.a[7] ? v.a[6] : v.a[7];
  signed char tmp0_4 = v.a[8] > v.a[9] ? v.a[8] : v.a[9];
  signed char tmp0_5 = v.a[10] > v.a[11] ? v.a[10] : v.a[11];
  signed char tmp0_6 = v.a[12] > v.a[13] ? v.a[12] : v.a[13];
  signed char tmp0_7 = v.a[14] > v.a[15] ? v.a[14] : v.a[15];

  signed char tmp1_0 = tmp0_0 > tmp0_1 ? tmp0_0 : tmp0_1;
  signed char tmp1_1 = tmp0_2 > tmp0_3 ? tmp0_2 : tmp0_3;
  signed char tmp1_2 = tmp0_4 > tmp0_5 ? tmp0_4 : tmp0_5;
  signed char tmp1_3 = tmp0_6 > tmp0_7 ? tmp0_6 : tmp0_7;

  signed char tmp2_0 = tmp1_0 > tmp1_1 ? tmp1_0 : tmp1_1;
  signed char tmp2_1 = tmp1_2 > tmp1_3 ? tmp1_2 : tmp1_3;

  signed char result = tmp2_0 > tmp2_1 ? tmp2_0 : tmp2_1;

  u16qi vec_result;
  int i;
  vec_result.a[0] = result;
  for(i=1;i<16;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}


v16qi
genvec_support_VQI_VQI_mask_for_load_Mono_Simd_Vector16sb__System_Int32_System_Int32_Mono_Simd_Vector16sb (u16qi* u, int a, int b)
{
  int i;
  u16qi r;
  for(i=0; i < 4; i++)
    r.a[i] = u->a[i];
  return r.v;
}

v16qi
genvec_support_VQI_RealignLoad_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb__System_Int32_System_Int32_Mono_Simd_Vector16sb(u16qi a, u16qi b, u16qi c, u16qi* d, int e, int f)
{
  int i;
  u16qi r;
  for(i=0; i < 16; i++)
    r.a[i] = d->a[i];
  return r.v;
}


unsigned int
genvec_support_VQI_VQI_realign_offset_System_UInt32()
{
  return 0;
}

int
genvec_support_VQI_VI_get_loop_niters_System_Int32_System_Int32_System_Int32(int a, int b)
{
  return b;
}

unsigned int
genvec_support_VHI_get_vec_size_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VHI_stride_System_UInt32()
{
  return 8;
}

unsigned int
genvec_support_VHI_VHI_align_System_UInt32()
{
  return 16;
}

v8hi
genvec_support_VHI_VHI_uniform_vec_System_Int16_Mono_Simd_Vector8s (short int x)
{
  u8hi vec_result;
  int i;
  for(i=0;i<8;i++)
    vec_result.a[i] = x;
  return vec_result.v;
}

v8hi
genvec_support_VHI_VHI_init_vec_System_Int16_System_Int16_Mono_Simd_Vector8s (short int a, short int b)
{
  u8hi vec_result;
  int i;
  vec_result.a[0] = a;
  for(i=1;i<8;i++)
    vec_result.a[i] = b;
  return vec_result.v;
}

v8hi
genvec_support_VHI_VHI_affine_vec_System_Int16_System_Int16_Mono_Simd_Vector8s (short int bas, short int step)
{
  u8hi vec_result;
  int i;
  for(i=0;i<8;i++)
    vec_result.a[i] = (short)(bas + i*step);
  return vec_result.v;
}


v8hi
genvec_support_VHI_ALoad_Mono_Simd_Vector8s_Mono_Simd_Vector8s(u8hi* addr)
{
  int addr_val = (int)addr;
  int rounded_addr = addr_val & ~0xf;
  return *(v8hi*)rounded_addr;
}


v8hi
genvec_support_VHI_VHI_dot_product_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector8s_Mono_Simd_Vector8s (u16qi va, u16qi vb, u8hi acc)
{
  u8hi vec_result;
  int i;
  for(i=0;i<8;i++)
    {
    int j = i*2;
    vec_result.a[i] = (short)(va.a[j] * vb.a[j] + va.a[j+1] * vb.a[j+1]);
    }
  return vec_result.v;
}

v8hi
genvec_support_VHI_VHI_reduc_plus_epilogue_Mono_Simd_Vector8s_Mono_Simd_Vector8s(u8hi v)
{
  short int result = (short)(v.a[0] + v.a[1] + v.a[2] + v.a[3] + v.a[4] + v.a[5] + v.a[6] + v.a[7]);
  u8hi vec_result;
  int i;
  vec_result.a[0] = result;
  for(i=1;i<8;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v8hi
genvec_support_VHI_VHI_reduc_min_epilogue_Mono_Simd_Vector8s_Mono_Simd_Vector8s(u8hi v)
{
  short int tmp0_0 = v.a[0] < v.a[1] ? v.a[0] : v.a[1];
  short int tmp0_1 = v.a[2] < v.a[3] ? v.a[2] : v.a[3];
  short int tmp0_2 = v.a[4] < v.a[5] ? v.a[4] : v.a[5];
  short int tmp0_3 = v.a[6] < v.a[7] ? v.a[6] : v.a[7];

  short int tmp1_0 = tmp0_0 < tmp0_1 ? tmp0_0 : tmp0_1;
  short int tmp1_1 = tmp0_2 < tmp0_3 ? tmp0_2 : tmp0_3;

  short int result = tmp1_0 < tmp1_1 ? tmp1_0 : tmp1_1;

  int i;
  u8hi vec_result;
  vec_result.a[0] = result;
  for(i=1;i<8;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v8hi
genvec_support_VHI_VHI_reduc_max_epilogue_Mono_Simd_Vector8s_Mono_Simd_Vector8s(u8hi v)
{
  short int tmp0_0 = v.a[0] > v.a[1] ? v.a[0] : v.a[1];
  short int tmp0_1 = v.a[2] > v.a[3] ? v.a[2] : v.a[3];
  short int tmp0_2 = v.a[4] > v.a[5] ? v.a[4] : v.a[5];
  short int tmp0_3 = v.a[6] > v.a[7] ? v.a[6] : v.a[7];

  short int tmp1_0 = tmp0_0 > tmp0_1 ? tmp0_0 : tmp0_1;
  short int tmp1_1 = tmp0_2 > tmp0_3 ? tmp0_2 : tmp0_3;

  short int result = tmp1_0 > tmp1_1 ? tmp1_0 : tmp1_1;

  int i;
  u8hi vec_result;
  vec_result.a[0] = result;
  for(i=1;i<8;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}


v8hi
genvec_support_VHI_VHI_mask_for_load_Mono_Simd_Vector8s_System_UInt32_Mono_Simd_Vector8s (unsigned int a, u8hi* u)
{
  int i;
  u8hi r;
  for(i=0; i < 8; i++)
    r.a[i] = u->a[i];
  return r.v;
}


v8hi
genvec_support_VHI_RealignLoad_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector8s__System_Int32_System_Int32_Mono_Simd_Vector8s(u8hi a, u8hi b, u8hi c, u8hi* d, int e, int f)
{
  int i;
  u8hi r;
  for(i=0; i < 8; i++)
    r.a[i] = d->a[i];
  return r.v;
}

unsigned int
genvec_support_VHI_VHI_realign_offset_System_UInt32()
{
  return 0;
}

unsigned int
genvec_support_VSI_get_vec_size_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VSI_stride_System_UInt32()
{
  return 4;
}

unsigned int
genvec_support_VSI_VSI_align_System_UInt32()
{
  return 16;
}

v4si
genvec_support_VSI_VSI_uniform_vec_System_Int32_Mono_Simd_Vector4i (int x)
{
  int i;
  u4si vec_result;
  for(i=0;i<4;i++)
    vec_result.a[i] = x;
  return vec_result.v;
}

v4si
genvec_support_VSI_VSI_init_vec_System_Int32_System_Int32_Mono_Simd_Vector4i (int a, int b)
{
  int i;
  u4si vec_result;
  vec_result.a[0] = a;
  for(i=1;i<4;i++)
    vec_result.a[i] = b;
  return vec_result.v;
}

v4si
genvec_support_VSI_VSI_affine_vec_System_Int32_System_Int32_Mono_Simd_Vector4i (int bas, int step)
{
  int i;
  u4si vec_result;
  for(i=0;i<4;i++)
    vec_result.a[i] = bas + i*step;
  return vec_result.v;
}

v4si
genvec_support_VSI_ALoad_Mono_Simd_Vector4i__Mono_Simd_Vector4i (u4si* addr)
{
  int addr_val = (int)addr;
  int rounded_addr = addr_val & ~0xf;
  return *(v4si*)rounded_addr;
}

v4si
genvec_support_VSI_VSI_dot_product_Mono_Simd_Vector8s_Mono_Simd_Vector8s_Mono_Simd_Vector4i_Mono_Simd_Vector4i(u8hi va, u8hi vb, u4si acc)
{
  u4si vec_result;
  int i;
  for(i=0;i<4;i++)
    {
    int j = i*2;
    vec_result.a[i] = (short)(va.a[j] * vb.a[j] + va.a[j+1] * vb.a[j+1]);
    }
  return vec_result.v;
}

v4si
genvec_support_VSI_VSI_reduc_plus_epilogue_Mono_Simd_Vector4i_Mono_Simd_Vector4i (u4si v)
{
  int result = v.a[1] + v.a[2] + v.a[3] + v.a[0];
  int i;
  u4si vec_result;
  vec_result.a[0] = result;
  for(i=1;i<4;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v4si
genvec_support_VSI_VSI_reduc_min_epilogue_Mono_Simd_Vector4i_Mono_Simd_Vector4i (u4si v)
{
  int tmp0_0 = v.a[1] < v.a[2] ? v.a[1] : v.a[2];
  int tmp0_1 = v.a[3] < v.a[0] ? v.a[3] : v.a[0];

  int result = tmp0_0 < tmp0_1 ? tmp0_0 : tmp0_1;

  int i;
  u4si vec_result;
  vec_result.a[0] = result;
  for(i=1;i<4;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v4si
genvec_support_VSI_VSI_reduc_max_epilogue_Mono_Simd_Vector4i_Mono_Simd_Vector4i (u4si v)
{
  int tmp0_0 = v.a[1] > v.a[2] ? v.a[1] : v.a[2];
  int tmp0_1 = v.a[3] > v.a[0] ? v.a[3] : v.a[0];

  int result = tmp0_0 > tmp0_1 ? tmp0_0 : tmp0_1;

  int i;
  u4si vec_result;
  vec_result.a[0] = result;
  for(i=1;i<4;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}


v4si
genvec_support_VSI_VSI_mask_for_load_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb_Mono_Simd_Vector16sb(unsigned int a, u4si* u)
{
  int i;
  u4si r;
  for(i=0; i < 4; i++)
    r.a[i] = u->a[i];
  return r.v;
}



v4si
genvec_support_VSI_RealignLoad_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i_Mono_Simd_Vector4i__System_Int32_System_Int32_Mono_Simd_Vector4i(u4si a, u4si b, u4si c, u4si* d, int e, int f)
{
  int i;
  u4si r;
  for(i=0; i < 4; i++)
    r.a[i] = d->a[i];
  return r.v;
}

unsigned int
genvec_support_VSI_VSI_realign_offset_System_UInt32()
{
  return 0;
}

/*********************************************************************************************/

unsigned int
genvec_support_VDF_get_vec_size_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VDF_stride_System_UInt32()
{
  return 2;
}

unsigned int
genvec_support_VDF_VDF_align_System_UInt32()
{
  return 16;
}

v2df
genvec_support_VDF_VDF_uniform_vec_System_Int32_Mono_Simd_Vector2d (int x)
{
  int i;
  u2df vec_result;
  for(i=0;i<2;i++)
    vec_result.a[i] = x;
  return vec_result.v;
}

v2df
genvec_support_VDF_VDF_affine_vec_System_Int32_System_Int32_Mono_Simd_Vector2d (int bas, int step)
{
  int i;
  u2df vec_result;
  for(i=0;i<2;i++)
    vec_result.a[i] = bas + i*step;
  return vec_result.v;
}


v2df
genvec_support_VDF_ALoad_Mono_Simd_Vector2d_Mono_Simd_Vector2d(u2df* addr)
{
  int addr_val = (int)addr;
  int rounded_addr = addr_val & ~0xf;
  return *(v2df*)rounded_addr;
}


v2df
genvec_support_VDF_VDF_reduc_plus_epilogue_Mono_Simd_Vector2d_Mono_Simd_Vector2d (u2df v)
{
  long result = v.a[1] + v.a[2];
  int i;
  u2df vec_result;
  vec_result.a[0] = result;
  for(i=1;i<2;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v2df
genvec_support_VDF_VDF_reduc_min_epilogue_Mono_Simd_Vector2d_Mono_Simd_Vector2d (u2df v)
{
  long result = v.a[1] < v.a[2] ? v.a[1] : v.a[2];
  int i;
  u2df vec_result;
  vec_result.a[0] = result;
  for(i=1;i<2;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v2df
genvec_support_VDF_VDF_reduc_max_epilogue_Mono_Simd_Vector2d_Mono_Simd_Vector2d (u2df v)
{
  long result = v.a[1] > v.a[2] ? v.a[1] : v.a[2];
  int i;
  u2df vec_result;
  vec_result.a[0] = result;
  for(i=1;i<2;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}


v2df
genvec_support_VDF_VDF_mask_for_load_System_UInt32_Mono_Simd_Vector2d_Mono_Simd_Vector2d(unsigned int a, u2df* u)
{
  int i;
  u2df r;
  for(i=0; i < 2; i++)
    r.a[i] = u->a[i];
  return r.v;
}


v2df
genvec_support_VDF_RealignLoad_Mono_Simd_Vector2d_Mono_Simd_Vector2d_Mono_Simd_Vector2d_Mono_Simd_Vector2d__System_Int32_System_Int32_Mono_Simd_Vector2d(u2df a, u2df b, u2df c, u2df* d, int e, int f)
{
  int i;
  u2df r;
  for(i=0; i < 2; i++)
    r.a[i] = d->a[i];
  return r.v;
}


unsigned int
genvec_support_VDF_VHI_realign_offset_System_UInt32()
{
  return 0;
}

/*********************************************************************************************/




unsigned int
genvec_support_VDI_get_vec_size_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VDI_stride_System_UInt32()
{
  return 2;
}

unsigned int
genvec_support_VDI_VDI_align_System_UInt32()
{
  return 16;
}

v2di
genvec_support_VDI_VDI_uniform_vec_System_Int32_Mono_Simd_Vector2l (int x)
{
  int i;
  u2di vec_result;
  for(i=0;i<2;i++)
    vec_result.a[i] = x;
  return vec_result.v;
}

v2di
genvec_support_VDI_VDI_affine_vec_System_Int32_System_Int32_Mono_Simd_Vector2l (int bas, int step)
{
  int i;
  u2di vec_result;
  for(i=0;i<2;i++)
    vec_result.a[i] = bas + i*step;
  return vec_result.v;
}


v2di
genvec_support_VDI_ALoad_Mono_Simd_Vector2l_Mono_Simd_Vector2l(u2di* addr)
{
  int addr_val = (int)addr;
  int rounded_addr = addr_val & ~0xf;
  return *(v2di*)rounded_addr;
}


v2di
genvec_support_VDI_VDI_reduc_plus_epilogue_Mono_Simd_Vector2l_Mono_Simd_Vector2l (u2di v)
{
  long result = v.a[1] + v.a[2];
  int i;
  u2di vec_result;
  vec_result.a[0] = result;
  for(i=1;i<2;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v2di
genvec_support_VDI_VDI_reduc_min_epilogue_Mono_Simd_Vector2l_Mono_Simd_Vector2l (u2di v)
{
  long result = v.a[1] < v.a[2] ? v.a[1] : v.a[2];
  int i;
  u2di vec_result;
  vec_result.a[0] = result;
  for(i=1;i<2;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v2di
genvec_support_VDI_VDI_reduc_max_epilogue_Mono_Simd_Vector2l_Mono_Simd_Vector2l (u2di v)
{
  long result = v.a[1] > v.a[2] ? v.a[1] : v.a[2];
  int i;
  u2di vec_result;
  vec_result.a[0] = result;
  for(i=1;i<2;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}


v2di
genvec_support_VDI_VDI_mask_for_load_System_UInt32_Mono_Simd_Vector2l_Mono_Simd_Vector2l(unsigned int a, u2di* u)
{
  int i;
  u2di r;
  for(i=0; i < 2; i++)
    r.a[i] = u->a[i];
  return r.v;
}


v2di
genvec_support_VDI_RealignLoad_Mono_Simd_Vector2l_Mono_Simd_Vector2l_Mono_Simd_Vector2l_Mono_Simd_Vector2l__System_Int32_System_Int32_Mono_Simd_Vector2l(u2di a, u2di b, u2di c, u2di* d, int e, int f)
{
  int i;
  u2di r;
  for(i=0; i < 2; i++)
    r.a[i] = d->a[i];
  return r.v;
}


unsigned int
genvec_support_VDI_VHI_realign_offset_System_UInt32()
{
  return 0;
}


unsigned int
genvec_support_VSF_get_vec_size_System_UInt32()
{
  return 16;
}

unsigned int
genvec_support_VSF_stride_System_UInt32()
{
  return 4;
}

unsigned int
genvec_support_VSF_VSF_align_System_UInt32()
{
  return 16;
}

v4sf
genvec_support_VSF_VSF_uniform_vec_System_Single_Mono_Simd_Vector4f (float x)
{
  int i;
  u4sf vec_result;
  for(i=0;i<4;i++)
    vec_result.a[i] = x;
  return vec_result.v;

}

v4sf
genvec_support_VSF_VSF_init_vec (float a, float b)
{
  int i;
  u4sf vec_result;
  vec_result.a[0] = a;
  for(i=1;i<4;i++)
    vec_result.a[i] = b;
  return vec_result.v;
}

v4sf
genvec_support_VSF_VSI_affine_vec (float bas, float step)
{
  int i;
  u4sf vec_result;
  for(i=0;i<4;i++)
    vec_result.a[i] = bas + i*step;
  return vec_result.v;
}


v4sf
genvec_support_VSF_ALoad_Mono_Simd_Vector4f__Mono_Simd_Vector4f(u4sf* addr)
{
  int addr_val = (int)addr;
  int rounded_addr = addr_val & ~0xf;
  return *(v4sf*)rounded_addr;
}


v4sf
genvec_support_VSF_VSF_reduc_plus_epilogue_Mono_Simd_Vector4f_Mono_Simd_Vector4f(u4sf v)
{
  float result = v.a[0] + v.a[1] + v.a[2] + v.a[3];
  int i;
  u4sf vec_result;
  vec_result.a[0] = result;
  for(i=1;i<4;i++)
    vec_result.a[i] = 0;
  return vec_result.v;

}

v4sf
genvec_support_VSF_VSF_reduc_min_epilogue_Mono_Simd_Vector4f_Mono_Simd_Vector4f(u4sf v)
{
  float tmp0_0 = v.a[1] < v.a[2] ? v.a[1] : v.a[2];
  float tmp0_1 = v.a[3] < v.a[0] ? v.a[3] : v.a[0];

  float result = tmp0_0 < tmp0_1 ? tmp0_0 : tmp0_1;

  int i;
  u4sf vec_result;
  vec_result.a[0] = result;
  for(i=1;i<4;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}

v4sf
genvec_support_VSF_VSF_reduc_max_epilogue_Mono_Simd_Vector4f_Mono_Simd_Vector4f(u4sf v)
{
  float tmp0_0 = v.a[1] > v.a[2] ? v.a[1] : v.a[2];
  float tmp0_1 = v.a[3] > v.a[0] ? v.a[3] : v.a[0];

  float result = tmp0_0 > tmp0_1 ? tmp0_0 : tmp0_1;

  int i;
  u4sf vec_result;
  vec_result.a[0] = result;
  for(i=1;i<4;i++)
    vec_result.a[i] = 0;
  return vec_result.v;
}


v4sf
genvec_support_VSF_VSF_mask_for_load_System_UInt32_Mono_Simd_Vector4f_Mono_Simd_Vector4f(unsigned int a, u4sf* u)
{
  int i;
  u4sf r;
  for(i=0; i < 16; i++)
    r.a[i] = u->a[i];
  return r.v;
}



v4sf
genvec_support_VSF_RealignLoad_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f_Mono_Simd_Vector4f__System_Int32_System_Int32_Mono_Simd_Vector4f(u4sf a, u4sf b, u4sf c, u4sf* d, int e, int f)
{
  int i;
  u4sf r;
  for(i=0; i < 16; i++)
    r.a[i] = d->a[i];
  return r.v;
}


unsigned int
genvec_support_VSF_VSF_realign_offset_System_UInt32()
{
  return 0;
}

