/*
 *  Copyright (C) 2013,14 Bluwireless Technology Ltd.
 *   All Rights Reserved.
 *    
 *  This software and its associated documentation contains proprietary,
 *  confidential and trade secret information of Bluwireless Technology Ltd.
 *  and except as provided by written agreement with Bluwireless Technology Ltd.
 *
 *  a)   no part may be disclosed, distributed, reproduced, transmitted,
 *       transcribed, stored in a retrieval system, adapted or translated
 *       in any form or by any means electronic, mechanical, magnetic,
 *       optical, chemical, manual or otherwise,
 *
 *  and
 *
 *  b)   the recipient is not entitled to discover through reverse
 *       engineering or reverse compiling or other such techniques or
 *       processes the trade secrets contained therein or in the
 *       documentation.
 **/

#ifndef BWT_UDIMACROS_INTRINSICS_H
#define BWT_UDIMACROS_INTRINSICS_H

#if defined(__bwt__)
#include <bwt.h>
VERSION("udimacros_h 1.8");
#endif

#if !defined(GCC_VER)
#define GCC_VER 40703 /* our default compiler */
#endif

#if GCC_VER <= 40703
#pragma GCC optimize ("no-dce") // stop eliminating volatile register..
#endif

#define VPU       0
#define VPU_DUMMY 1
#define FFT_I     2
#define FFT_O     3
#define LDPC_I    4
#define LDPC_O    5
#define DFE_I     6
#define DFE_O     7
#define MAC_I     8
#define MAC_O     9

#if defined(__bwt__) && !defined(__bwt_intrinsics__)
register volatile unsigned int _udimacros_temp_reg_ asm ("$1") ;
#define _UDIMACROS_TEMP_REG_ 1
#endif

#define BWT_ASM_STRING(expr) ".word " #expr 
#define __BWT_ASM(expr) BWT_ASM_STRING(expr)

// write_reg
// ---------
// write_reg variants:
//
//            bwt_write_reg_i - pipe, ppu_reg, immediate
//                       bwt_write_reg_r - pipe, ppu_reg, top_bits, gpr
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  i  i  i  i  i  i  i  i  i  i  i  i  d  d  d  d  d  0  1  1  0  1  p  p  p  p
//  0  1  1  1  0  0  r  r  r  r  r  i  i  i  i  i  i  i  d  d  d  d  d  1  1  1  0  1  p  p  p  p
//
//    31-26 = 011100 (mips fixed)
//    25-14 = iiiiiiiiiiii (immediate)
//    25-21 = rrrrr register (op1 variant) (mips rs) *subject to Note*
//    20-14 = iiiiiii (immediate hi)
//    13-9  = ddddd (destination register)
//    8 = 0 (variant op1 not used)
//    8 = 1 (variant op1 supplies data)
//    7-6 = 11 (type = 3)
//    0-3 = vvvv (pipe, literal) 0 - 9
//
// -----------------------------

#define __BWT_WRITE_REG_EXPR(pipe, ppu_reg, opp, opp1, topbits, immediate)  \
  ((0x1c << 26) |                          \
  ((opp1 & 0x1f) << 21) |                  \
  ((immediate & 0xfff) << 14) |            \
  ((topbits & 0x7f) << 14) |               \
  ((ppu_reg & 0x1f) << 9) |                \
  ((opp & 0x1) << 8) | (0x0d << 4) |       \
  (pipe & 0xf))

#define _BWT_ASM_WRITE_REG(pipe, ppu_reg, opp, opp1, topbits, immediate) \
 __BWT_ASM( __BWT_WRITE_REG_EXPR(pipe, ppu_reg, opp, opp1, topbits, immediate) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_WRITE_REG_I(pipe, ppu_reg, immediate) \
      __builtin_bwt_write_reg_i(pipe, ppu_reg, 0, 0, 0, immediate)

#define BWT_WRITE_REG_VAR(pipe, ppu_reg, topbits, var) \
      __builtin_bwt_write_reg_var(pipe, ppu_reg, 1, var, topbits, 0)

#else // i2
#define BWT_WRITE_REG_I(pipe, ppu_reg, immediate) \
      __builtin_bwt_i2_write_reg_i(pipe, ppu_reg, 0, 0, 0, immediate)

#define BWT_WRITE_REG_VAR(pipe, ppu_reg, topbits, var) \
      __builtin_bwt_i2_write_reg_var(pipe, ppu_reg, 1, var, topbits, 0)
#endif // i2
#else

#define BWT_WRITE_REG_I(pipe, ppu_reg, immediate)   \
  asm volatile(_BWT_ASM_WRITE_REG(pipe, ppu_reg, 0, 0, 0, immediate))

#define BWT_WRITE_REG_VAR(pipe, ppu_reg, topbits, var) \
  asm volatile(".set noat");  \
  _udimacros_temp_reg_ = var; \
  asm volatile(               \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i" \
        __BWT_WRITE_REG_EXPR(pipe, ppu_reg, 1, _UDIMACROS_TEMP_REG_,topbits,0))

#endif

// ------------------------------
//  read register to internal fifo
//
//   two variants for read_reg 
//     one take a literal to identify a register (id) whilst the
//     other takes a machine register
//
//     bwt_read_reg_i - pipe, ppu_reg, blocking
//     bwt_read_reg_r - pipe, gpr, blocking
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  ?  i  i  i  i  i  i  i  0  b  1  0  0  0  0  0  1  p  p  p  p
//  0  1  1  1  0  0  r  r  r  r  r  ?  i  i  i  i  i  i  i  1  b  1  0  0  0  0  0  1  p  p  p  p
//
//  31-26 = 011100 (mips fixed)
//  25-21 = rrrrr (=0) register (op1 variant) (mips rs) (op1 unused,r = 0)
//  25-21 = rrrrr register (op1 variant) (mips rs) (op1 unused,r = 0)
//  20    = i
//  20    = i = 0
//  19-13 = iiiiiii (immediate hi)
//  19-13 = iiiiiii = 0 (immediate hi)
//  13-9  = ddddd (destination register)
//  12 = 0 (op1 unused)
//  12 = 1 (op1 used)
//  11 = b (blocking)
//  10-8 = 4 (sub-type= 4)
//  7-6 = 00 (type = 0)
//  5-4 = 01
//  0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_READ_REG_EXPR(pipe, ppu_reg, blocking, opp, opp1)  \
  ((0x1c << 26) | ((opp1 & 0x1f) << 21) |        \
  (0 << 20) | ((ppu_reg & 0x7f) << 13) |         \
  (opp << 12) | ((blocking & 0x1) << 11) |       \
  (0x41 << 4) | (pipe & 0xf))

#define _BWT_ASM_READ_REG(pipe, ppu_reg, blocking, opp, opp1) \
 __BWT_ASM( __BWT_READ_REG_EXPR(pipe, ppu_reg, blocking, 0, 0) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_READ_REG_I(pipe, ppu_reg, blocking) \
      __builtin_bwt_read_reg_i(pipe, ppu_reg, blocking, 0, 0)

#define BWT_READ_REG_VAR(pipe, var, blocking) \
      __builtin_bwt_read_reg_var(pipe, 0, blocking, 1, var)
#else // i2
#define BWT_READ_REG_I(pipe, ppu_reg, blocking) \
      __builtin_bwt_i2_read_reg_i(pipe, ppu_reg, blocking, 0, 0)

#define BWT_READ_REG_VAR(pipe, var, blocking) \
      __builtin_bwt_i2_read_reg_var(pipe, 0, blocking, 1, var)
#endif // i2

#else

#define BWT_READ_REG_I(pipe, ppu_reg, blocking)  \
  asm volatile (_BWT_ASM_READ_REG(pipe, ppu_reg, blocking, 0, 0))

#define BWT_READ_REG_VAR(pipe, var, blocking)                     \
  asm volatile(".set noat");                                      \
  _udimacros_temp_reg_ = var;                                     \
  asm volatile(                                                   \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i" \
       __BWT_READ_REG_EXPR(pipe, 0, blocking, 1, _UDIMACROS_TEMP_REG_))

#endif

// ------------------------------
// 
//  bwt_pop_result - gpr, pipe, ppu_pwr, cpu_pwr, blocking
// 
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  ?  ?  ?  ?  ?  ?  ?  P  C  b  0  1  0  0  0  0  1  p  p  p  p
// 
// 31-26 = 011100 (mips fixed)
// 25-21 = rrrrr register (op1 variant) (mips rs)
// 20-14 = 0 (undefined)
// 13 = P ppu_power_save
// 12 = C cpu_power_save
// 11 = b (blocking)
// 10-8 = 2 (sub-type= 2)
// 7-6 = 00 (type = 0)
// 5-4 = 01
// 0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_POP_RESULT_EXPR(gpr, pipe, blocking, cpu_pwr, ppu_pwr) \
 ((0x1c << 26) | ((gpr & 0x1f) << 21) |                       \
 ((0 & 0x7f) << 14) |                                         \
 ((ppu_pwr & 0x1) << 13) | ((cpu_pwr & 0x1) << 12) |          \
 ((blocking & 0x1) << 11) |                                   \
 (0x21 << 4) | (pipe & 0xf))

#define _BWT_ASM_POP_RESULT(gpr, pipe, blocking, cpu_pwr, ppu_pwr) \
 __BWT_ASM( __BWT_POP_RESULT_EXPR(gpr, pipe, blocking, cpu_pwr, ppu_pwr) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_POP_RESULT_VAR(var, pipe) \
      var = __builtin_bwt_pop_result(pipe, 1, 0, 0)
#define BWT_DROP_RESULT(pipe) __builtin_bwt_drop_result(pipe, 1, 0, 0)
#else // i2
#define BWT_POP_RESULT_VAR(var, pipe) \
      var = __builtin_bwt_i2_pop_result(pipe, 1, 0, 0)
#define BWT_DROP_RESULT(pipe) __builtin_bwt_i2_drop_result(pipe, 1, 0, 0)
#endif // i2
#else

#define BWT_POP_RESULT_VAR(var, pipe) \
  asm volatile(".set noat"); \
  asm volatile(".word %[imm]\n" : "=r" (_udimacros_temp_reg_) :[imm] "i" \
      __BWT_POP_RESULT_EXPR(_UDIMACROS_TEMP_REG_, pipe, 1, 0, 0)); \
  var = _udimacros_temp_reg_;

#define BWT_DROP_RESULT(pipe) \
  asm volatile(".set noat"); \
  asm volatile(".word %[imm]\n" :: [imm] "i" \
      __BWT_POP_RESULT_EXPR(0, pipe, 1, 0, 0)); \

#endif

// ------------------------------
// 
// bwt_mc_addr - pipe, addr, m0, m1, m2, m3
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  a  a  a  a  a  ? m3 m3 m3 m2 m2 m2 m1 m1 m1 m0 m0 m0  1  0  0  1  p  p  p  p
//
//    31-26 = 011100 (mips fixed)
//    25-21 = aaaaa register (mips rs)
//    20 = 0 (undefined)
//    19-17 = m3, memory select addr generator 3
//    16-14 = m2, memory select addr generator 2
//    11-13 = m1, memory select addr generator 1
//    10-8  = m0, memory select addr generator 0
//    7-6 = 10 (type = 2)
//    5-4 = 01
//    0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_MC_ADDR_EXPR( pipe, var, m0, m1, m2, m3) \
 ((0x1c << 26) | ((var & 0x1f) << 21) |                    \
 (0 << 20) |                                               \
 ((m3 & 0x7) << 17) | ((m2 & 0x7) << 14) |                 \
 ((m1 & 0x7) << 11) | ((m0 & 0x7) << 8)  |                 \
 (0x9 << 4) | (pipe & 0xf))

#define _BWT_ASM_MC_ADDR(pipe, var, m0, m1, m2, m3) \
        __BWT_ASM( __BWT_MC_ADDR_EXPR(pipe, var, m0, m1, m2, m3) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_MC_ADDR_VAR(var, m0, m1, m2, m3) \
            __builtin_bwt_mc_addr(VPU, var, m0, m1, m2, m3)
#else // i2
#define BWT_MC_ADDR_VAR(var, m0, m1, m2, m3) \
            __builtin_bwt_i2_mc_addr(VPU, var, m0, m1, m2, m3)
#endif // i2
#else

#define BWT_MC_ADDR_VAR(var, m0, m1, m2, m3)                         \
  asm volatile(".set noat");                                         \
  _udimacros_temp_reg_ = var;                                        \
  asm volatile(                                                      \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i"    \
       __BWT_MC_ADDR_EXPR(VPU, _UDIMACROS_TEMP_REG_, m0, m1, m2, m3))

#endif

// ------------------------------
//  bwt_vn_io_inst - pipe, am, pm, im, am, bits, wen
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  v  v  v  v  v  ?  ?  ?  b  p  p  p  i  i  a  a  a  w  0  1  0  1  p  p  p  p
//
//   31-26 = 011100 (mips fixed)
//   25-21 = addr (mips rs)
//   20-18 = 0 (undefined)
//   17 = bits
//   16-14 = pack_mode
//   13-12 = item_mode
//   11-9 = addr_mode
//   8 = wen
//   7-6 = 01
//   5-4 = 01
//   0-3 = pppp (pipe, literal) 0 - 9
//

#define AM_TARGET   0 /* target over bus not transactions */
#define AM_FIXED    1 /* fixed addressing mode            */
#define AM_INC      2 /* normal incrementing address mode */
#define AM_CT       3 /* item mode controls addressing    */
#define AM_TABLE_1  4 /* single table mode                */
#define AM_TABLE_2  5 /* two streams, two tables          */
#define AM_TABLE_3  6 /* three streams, three tables      */

#define IM_COPY     0 /* default copy mode   */
#define IM_TABLE_1  1 /* table driven mode 1 */
#define IM_TABLE_2  2 /* table driver mode 2 */
#define IM_TABLE_3  3 /* table driver mode 3 */

#define PM_COPY   0   /* simple copy        */
#define PM_PACK   1   /* data packed        */
#define PM_BIT_1  2   /* single bit/VN item */
#define PM_BIT_2  3   /* 2 bits/VN item     */
#define PM_BIT_3  4   /* 3 bits/VN item     */
#define PM_BIT_4  5   /* 4 bits/VN item     */
#define PM_BIT_6  6   /* 6 bits/VN item     */
#define PM_BIT_8  7   /* 8 bits/VN item     */

#define __BWT_VN_IO_EXPR(pipe, addr, am, im, pm, bits, wen) \
        ((0x1c << 26) | ((addr & 0x1f) << 21) |             \
         ((bits & 0x1) << 17) | ((pm & 0x7) << 14) |        \
         ((im & 0x3) << 12) |                               \
         ((am & 0x7) << 9) |                                \
         ((wen & 0x1) << 8) |                               \
         (0x5 << 4) | (pipe & 0xf))

#define _BWT_ASM_VN_IO_VAR(pipe, var, am, im, pm, bits, wen) \
      __BWT_ASM( __BWT_VN_IO_EXPR(pipe, var, am, im, pm, bits, wen) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_VN_READ_INST_VAR(pipe, var, am, im, pm, bits) \
  __builtin_bwt_vn_io_var(pipe,var,am,im,pm,bits,0)

#define BWT_VN_WRITE_INST_VAR(pipe, var, am, im, pm, bits) \
  __builtin_bwt_vn_io_var(pipe,var,am,im,pm,bits,1)
#else // i2
#define BWT_VN_READ_INST_VAR(pipe, var, am, im, pm, bits) \
  __builtin_bwt_i2_vn_io_var(pipe,var,am,im,pm,bits,0)

#define BWT_VN_WRITE_INST_VAR(pipe, var, am, im, pm, bits) \
  __builtin_bwt_i2_vn_io_var(pipe,var,am,im,pm,bits,1)
#endif // i2
#else

#define BWT_VN_READ_INST_VAR(pipe, var, am, im, pm, bits) \
  asm volatile(".set noat");                              \
  _udimacros_temp_reg_ = var;                             \
  asm volatile(                                                      \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i"    \
       __BWT_VN_IO_EXPR(pipe, _UDIMACROS_TEMP_REG_, am, im, pm, bits, 0))

#define BWT_VN_WRITE_INST_VAR(pipe, var, am, im, pm, bits) \
  asm volatile(".set noat");                               \
  _udimacros_temp_reg_ = var;                              \
  asm volatile(                                                      \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i"    \
       __BWT_VN_IO_EXPR(pipe, _UDIMACROS_TEMP_REG_, am, im, pm, bits, 1))

#endif

// ------------------------------
// bwt_transfer_inst - pipe, addr, srambler, coder, recycle, scram, rate, decode
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  ?  ?  ?  s  c  l  h  R  R  d  1  0  1  0  0  0  1  p  p  p  p
//
//      31-26 = 011100 (mips fixed)
//      25-21 = rrrrr register (op1 variant) (mips rs)
//      20-18 = 0 (undefined)
//      17 = bypass scrambler
//      16 = bypass coder
//      15 = recycle llrs
//      14 = scram header
//      13-12 = rate
//      11 = decode
//      10-8 = 101
//      7-6 = 0
//      5-4 = 01
//      0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_TRANSFER_INST_EXPR(pipe, addr, rate,                        \
                          recycle, sheader, bypass_s, bypass_c, decode) \
 ((0x1c << 26) | ((addr & 0x1f) << 21) |                                \
 ((bypass_s & 0x1) << 17) | /* bypass scrambler */                      \
 ((bypass_c & 0x1)<< 16) |  /* bypass encode */                         \
 ((recycle & 0x1) << 15) |                                              \
 ((sheader & 0x1) << 14) | /* scrambler header */                       \
 ((rate & 0x3) << 12) |                                                 \
 ((decode & 0x1) << 11) | /* decode */                                  \
 (0x51 << 4) | (pipe & 0xf))

#define _BWT_ASM_TRANSFER_INST(pipe, addr, rate,                         \
                          recycle, sheader, bypass_s, bypass_c, decode) \
 __BWT_ASM( __BWT_TRANSFER_INST_EXPR(pipe, addr, rate, recycle, sheader,\
                              bypass_s, bypass_c, decode))

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_LDPC_RX(rate)   __builtin_bwt_transfer_inst(LDPC_I,0,rate,0,0,0,0,1) 
#define BWT_LDPC_RX_H(rate) __builtin_bwt_transfer_inst(LDPC_I,0,rate,0,1,0,0,1) 

#define BWT_LDPC_TX(rate)   __builtin_bwt_transfer_inst(LDPC_I,0,rate,0,0,0,0,0) 
#define BWT_LDPC_TX_H(rate) __builtin_bwt_transfer_inst(LDPC_I,0,rate,0,1,0,0,0) 
#else // i2
#define BWT_LDPC_RX(rate)   __builtin_bwt_i2_transfer_inst(LDPC_I,0,rate,0,0,0,0,1) 
#define BWT_LDPC_RX_H(rate) __builtin_bwt_i2_transfer_inst(LDPC_I,0,rate,0,1,0,0,1) 

#define BWT_LDPC_TX(rate)   __builtin_bwt_i2_transfer_inst(LDPC_I,0,rate,0,0,0,0,0) 
#define BWT_LDPC_TX_H(rate) __builtin_bwt_i2_transfer_inst(LDPC_I,0,rate,0,1,0,0,0) 
#endif // i2
#else

#define BWT_LDPC_RX(rate)   asm volatile(\
                        _BWT_ASM_TRANSFER_INST(LDPC_I,0,rate,0,0,0,0,1))
#define BWT_LDPC_RX_H(rate) asm volatile(\
                        _BWT_ASM_TRANSFER_INST(LDPC_I,0,rate,0,1,0,0,1))

#define BWT_LDPC_TX(rate)   asm volatile(\
                        _BWT_ASM_TRANSFER_INST(LDPC_I,0,rate,0,0,0,0,0))
#define BWT_LDPC_TX_H(rate) asm volatile(\
                        _BWT_ASM_TRANSFER_INST(LDPC_I,0,rate,0,1,0,0,0))
#endif

// ------------------------------
//  bwt_load_mc - pipe, data, instr
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  ?  ?  ?  ?  ?  ?  ?  ?  ?  i  0  1  0  0  0  0  1  p  p  p  p
//
//     31-26 = 011100 (mips fixed)
//     25-21 = rrrrr register (op1 variant) (mips rs)
//     20-12 = 0 (undefined)
//     11 = i
//     10-8 = 5 (sub-type= 5)
//     7-6 = 00 (type = 0)
//     5-4 = 01
//     0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_LOAD_MC_EXPR(pipe, data, instr ) \
 ((0x1c << 26) | ((data & 0x1f) << 21) |     \
 (0 << 12) |                                 \
 ((instr & 0x1) << 11) |                     \
 (0x51 << 4) | (pipe & 0xf))

#define _BWT_ASM_LOAD_MC(pipe, data, instr) \
      __BWT_ASM( __BWT_LOAD_MC_EXPR(pipe, data, instr) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_LOAD_MC_VAR(data)   __builtin_bwt_load_mc(VPU, data, 0)
#define BWT_LOAD_MC_I_VAR(data) __builtin_bwt_load_mc(VPU, data, 1)
#else // i2
#define BWT_LOAD_MC_VAR(data)   __builtin_bwt_i2_load_mc(VPU, data, 0)
#define BWT_LOAD_MC_I_VAR(data) __builtin_bwt_i2_load_mc(VPU, data, 1)
#endif // i2
#else

#define BWT_LOAD_MC_VAR(var) \
  asm volatile(".set noat"); \
  _udimacros_temp_reg_ = var; \
  asm volatile(                                                      \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i"    \
       __BWT_LOAD_MC_EXPR(VPU, _UDIMACROS_TEMP_REG_, 0))

#define BWT_LOAD_MC_I_VAR(var) \
  asm volatile(".set noat"); \
  _udimacros_temp_reg_ = var; \
  asm volatile(                                                      \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i"    \
       __BWT_LOAD_MC_EXPR(VPU, _UDIMACROS_TEMP_REG_, 1))

#endif

// ------------------------------
//  bwt_select - 
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  c  c  c  c  c  c  c  v  s  s  0  0  0  0  0  0  1  p  p  p  p
//
//     31-26 = 011100 (mips fixed)
//     25-21 = rrrrr register op1  (mips rs)
//     21-14 = sem_count
//     13 = opp1 valid
//     12-11 = semaphore
//     10-8 = 0 (sub-type= 0)
//     7-6 = 00
//     5-4 = 01
//     0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_SELECT_EXPR(pipe, sem, opp, opp1, count) \
 ((0x1c << 26) | ((opp1 & 0x1f) << 21) |       \
 ((count & 0xff) << 14) |                      \
 ((opp & 0x1) << 13) | ((sem & 0x3) << 11) |   \
 (0x1 << 4) | (pipe & 0xf))

#define _BWT_ASM_SELECT(pipe, sem, opp, opp1, count) \
      __BWT_ASM( __BWT_SELECT_EXPR(pipe, sem, opp, opp1, count) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_SELECT_C(pipe,sem,count) __builtin_bwt_select_c(pipe,sem,0,0,count)
#define BWT_SELECT_VAR(pipe,sem,var) __builtin_bwt_select_var(pipe,sem,1,var,0)
#else // i2
#define BWT_SELECT_C(pipe,sem,count) __builtin_bwt_i2_select_c(pipe,sem,0,0,count)
#define BWT_SELECT_VAR(pipe,sem,var) __builtin_bwt_i2_select_var(pipe,sem,1,var,0)
#endif // i2
#else

#define BWT_SELECT_C(pipe, sem, count) \
      asm volatile(_BWT_ASM_SELECT(pipe, sem, 0, 0, count)) 

#define BWT_SELECT_VAR(pipe, sem, var)                            \
  asm volatile(".set noat");                                      \
  _udimacros_temp_reg_ = var;                                     \
  asm volatile(                                                   \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i" \
        __BWT_SELECT_EXPR(pipe, sem, 1, _UDIMACROS_TEMP_REG_, 0))

#endif

// ---------------------------------
//  bwt_sem_set 
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  ?  ?  ?  ?  ?  ?  ? v  s  s   0  0  1  0  0  0  1  p  p  p  p
//
//     31-26 = 011100 (mips fixed)
//     25-21 = rrrrr register op1  (mips rs)
//     20-14 = 0 (undefined)
//     13 = opp1 valid
//     12-11 = semaphore
//     10-8 = 1 (sub-type= 1)
//     7-6 = 00
//     5-4 = 01
//     0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_SEMSET_EXPR(pipe, sem, opp, opp1, count) \
 ((0x1c << 26) | ((opp1 & 0x1f) << 21) |           \
 ((count & 0xff) << 14) |                          \
 ((opp & 0x1) << 13) | ((sem & 0x3) << 11) |       \
 (0x11 << 4) | (pipe & 0xf))

#define _BWT_ASM_SEMSET(pipe, sem, opp, opp1, count) \
      __BWT_ASM( __BWT_SEMSET_EXPR(pipe, sem, opp, opp1, count) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics__)
#define BWT_SEMSET_C(pipe,sem,count) __builtin_bwt_semset_c(pipe,sem,0,0,count)
#define BWT_SEMSET_VAR(pipe,sem,var) __builtin_bwt_semset_var(pipe,sem,1,var,0)
#else // i2
#define BWT_SEMSET_C(pipe,sem,count) __builtin_bwt_i2_semset_c(pipe,sem,0,0,count)
#define BWT_SEMSET_VAR(pipe,sem,var) __builtin_bwt_i2_semset_var(pipe,sem,1,var,0)
#endif // i2
#else

#define BWT_SEMSET_C(pipe, sem, count) \
  asm volatile(_BWT_ASM_SEMSET(pipe, sem, 0, 0, count))
  
#define BWT_SEMSET_VAR(pipe, sem, var) \
  asm volatile(".set noat"); \
  _udimacros_temp_reg_ = var; \
  asm volatile(                                                   \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i" \
        __BWT_SEMSET_EXPR(pipe, sem, 1, _UDIMACROS_TEMP_REG_, 0))

#endif

// ----------------------------------
//  bwt_port_io_inst - pipe, wait, size, opp1, pro
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  r  r  r  r  r  R  R  R  R  R  w  w  w  w  w  w  o  p  1  0  0  1  p  p  p  p
//
// 31-26 = 011100 (mips fixed)
// 25-21 = rrrrr register size, op1 variant (mips rs)
// 21-16 = unused op1 variant 
// 25-16 = rrrrr register size variant (mips rs)
// 15-10 = wait
// 9 = opp1 selection
// 8 = producer/consumer
// 7-6 = 10 (type = 2)
// 5-4 = 01
// 0-3 = pppp (pipe, literal) 0 - 9


#define __BWT_PORT_IO_EXPR(pipe, wait, size, opp, opp1, pro_en) \
 ((0x1c << 26) |                                \
 ((opp1 & 0x1f) << 21) |                        \
 ((size & 0x3ff) << 16) |                       \
 ((wait & 0x3f) << 10) |                        \
 ((opp & 0x1) << 9) |                           \
 ((pro_en & 0x1) << 8) |                        \
 (0x9 << 4) | (pipe & 0xf))

#define _BWT_ASM_PORT_IO(pipe, wait, size, opp, opp1, pro_en) \
      __BWT_ASM( __BWT_PORT_IO_EXPR(pipe, wait, size, opp, opp1, pro_en))

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_PORT_PRODUCE_S(pipe, wait, size) \
      __builtin_bwt_port_io_s(pipe,wait,size,0,0,1)

#define BWT_PORT_CONSUME_S(pipe, wait, size) \
      __builtin_bwt_port_io_s(pipe,wait,size,0,0,0)

#define BWT_PORT_PRODUCE_VAR(pipe, wait, var) \
      __builtin_bwt_port_io_var(pipe,wait,0,1,var,1)
     
#define BWT_PORT_CONSUME_VAR(pipe, wait, var) \
      __builtin_bwt_port_io_var(pipe,wait,0,1,var,0)

#else // i2
#define BWT_PORT_PRODUCE_S(pipe, wait, size) \
      __builtin_bwt_i2_port_io_s(pipe,wait,size,0,0,1)

#define BWT_PORT_CONSUME_S(pipe, wait, size) \
      __builtin_bwt_i2_port_io_s(pipe,wait,size,0,0,0)

#define BWT_PORT_PRODUCE_VAR(pipe, wait, var) \
      __builtin_bwt_i2_port_io_var(pipe,wait,0,1,var,1)
     
#define BWT_PORT_CONSUME_VAR(pipe, wait, var) \
      __builtin_bwt_i2_port_io_var(pipe,wait,0,1,var,0)
#endif // i2

#else

#define BWT_PORT_PRODUCE_S(pipe, wait, size) \
     asm volatile( _BWT_ASM_PORT_IO(pipe, wait, size, 0, 0, 1))

#define BWT_PORT_CONSUME_S(pipe, wait, size) \
     asm volatile( _BWT_ASM_PORT_IO(pipe, wait, size, 0, 0, 0))

#define BWT_PORT_PRODUCE_VAR(pipe, wait, var) \
  asm volatile(".set noat"); \
  _udimacros_temp_reg_ = var; \
  asm volatile(                                                   \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i" \
        __BWT_PORT_IO_EXPR(pipe, wait, 0, 1, _UDIMACROS_TEMP_REG_, 1))
     
#define BWT_PORT_CONSUME_VAR(pipe, wait, var) \
  asm volatile(".set noat"); \
  _udimacros_temp_reg_ = var; \
  asm volatile(                                                   \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i" \
        __BWT_PORT_IO_EXPR(pipe, wait, 0, 1, _UDIMACROS_TEMP_REG_, 0))

#endif

// -----------------------------------
//   bwt_mc_main - pipe, entry, len, addr

//31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
// 0  1  1  1  0  0  r  r  r  r  r  l  l  l  l  l  e  e  e  e  e  e  e  e  0  1  0  1  p  p  p  p
//
// 31-26 = 011100 (mips fixed)
// 25-21 = rrrrr register, addr, (mips rs)
// 20-16 = vector_len
// 15-8  = entry
// 7-6 = 01 (type = 1)
// 5-4 = 01
// 0-3 = pppp (pipe, literal) 0 - 9
//

#define __BWT_MC_MAIN_EXPR( pipe, entry, len, addr) \
 ((0x1c << 26) | ((addr & 0x1f) << 21) |    \
 ((len & 0x1f) << 16) |                     \
 ((entry & 0xff) << 8) |                    \
 (0x5 << 4) | (pipe & 0xf))

#define _BWT_ASM_MC_MAIN(pipe, entry, len, addr) \
        __BWT_ASM( __BWT_MC_MAIN_EXPR(pipe, entry, len, addr))

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define BWT_MC_MAIN_VAR(entry, len, addr) \
            __builtin_bwt_mc_main(VPU, entry, len, addr)
#else // i2
#define BWT_MC_MAIN_VAR(entry, len, addr) \
            __builtin_bwt_i2_mc_main(VPU, entry, len, addr)
#endif // i2
#else

#define BWT_MC_MAIN_VAR(entry, len, var)                           \
  asm volatile(".set noat");                                       \
  _udimacros_temp_reg_ = var;                                      \
  asm volatile(                                                    \
      ".word %[imm]\n" :: "r" (_udimacros_temp_reg_), [imm] "i"  \
      __BWT_MC_MAIN_EXPR(VPU, entry, len, _UDIMACROS_TEMP_REG_))
#endif

// ----------------------------------
//           bwt_fft_inst - pipe, entry, len, addr
//
// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
//  0  1  1  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  i  1  0  1  0  0  0  1  p  p  p  p
//
// 31-26 = 011100 (mips fixed)
// 12-25 = 0
// 11 = ifft
// 10-8 = 101
// 7-6 = 00 (type = 1)
// 5-4 = 01
// 0-3 = pppp (pipe, literal) 0 - 9
//
#define __BWT_FFT_INST_EXPR(pipe, ifft)   \
 ((0x1c << 26) | ((ifft & 0x1) << 11) |   \
 (0x51 << 4) | (pipe & 0xf))

#define _BWT_ASM_FFT(pipe, ifft) __BWT_ASM( __BWT_FFT_INST_EXPR(pipe, ifft) )

#if defined(__bwt_intrinsics__)

#if !defined(__bwt_intrinsics_i2__)
#define __BWT_FFT_INST(pipe,ifft)  __builtin_bwt_fft(pipe, ifft)
#define BWT_FFT_INST              __builtin_bwt_fft(FFT_I,0)
#define BWT_FFT_INST_I            __builtin_bwt_fft(FFT_I,1)
#else // i2
#define __BWT_FFT_INST(pipe,ifft)  __builtin_bwt_i2_fft(pipe, ifft)
#define BWT_FFT_INST              __builtin_bwt_i2_fft(FFT_I,0)
#define BWT_FFT_INST_I            __builtin_bwt_i2_fft(FFT_I,1)
#endif // i2
#else
#define __BWT_FFT_INST(pipe,ifft) \
                  asm volatile(__BWT_ASM(__BWT_FFT_INST_EXPR(pipe,ifft)))
#define BWT_FFT_INST    asm volatile( __BWT_ASM(__BWT_FFT_INST_EXPR(FFT_I,0)))
#define BWT_FFT_INST_I  asm volatile( __BWT_ASM(__BWT_FFT_INST_EXPR(FFT_I,1)))
#endif

#endif /* BWT_UDIMACROS_INTRINSICS_H */

