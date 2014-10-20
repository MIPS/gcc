/* 
   Copyright (C) 2014 BluWireless Technologies Ltd.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#if defined(BWT)

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-attr.h"
#include "recog.h"
#include "output.h"
#include "udimacros.h"

#if 0
const char *bwt_emit_i(rtx op0) {
      return("imm!\n");
}

const char *bwt_emit_r(rtx op0) {
      return("GPR!\n");
}
#endif

#if 0
const char *bwt_fft_inst(rtx op0, rtx op1) {
 fprintf(stderr,"0 (code %d mode %d) %d\n", 
            op0->code, op0->mode, op0->u.hwint[0]); // code == CONST_INT
 fprintf(stderr,"1 (code %d mode %d) %d\n", 
            op1->code, op1->mode, op1->u.hwint[0]);
  return BWT_ASM_FFT(%0, %1) ;
}
#endif

#endif /* BWT */

