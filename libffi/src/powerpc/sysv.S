.LFB1:
.LCFI0:
.LCFI3:
   PowerPC Assembly glue.
.LCFI5:
.LCFI6:
.LFE1:

      .section  ".eh_frame","aw"
__FRAME_BEGIN__:
      .4byte    .LECIE1-.LSCIE1  /*  Length of Common Information Entry */
.LSCIE1:
      .4byte    0x0      /*  CIE Identifier Tag */
      .byte     0x1      /*  CIE Version */
      .ascii "\0"        /*  CIE Augmentation */
      .byte     0x1      /*  uleb128 0x1; CIE Code Alignment Factor */
      .byte     0x7c     /*  sleb128 -4; CIE Data Alignment Factor */
      .byte     0x41     /*  CIE RA Column */
      .byte     0xc      /*  DW_CFA_def_cfa */
      .byte     0x1      /*  uleb128 0x1 */
      .byte     0x0      /*  uleb128 0x0 */
      .align 2
.LECIE1:
.LSFDE1:
      .4byte    .LEFDE1-.LASFDE1         /*  FDE Length */
#define LIBFFI_ASM
#include <fficonfig.h>
      .4byte    .LASFDE1-__FRAME_BEGIN__         /*  FDE CIE offset */
      .4byte    .LFB1    /*  FDE initial location */
      .4byte    .LFE1-.LFB1      /*  FDE address range */
#ifndef __powerpc64__
	.globl ffi_prep_args_SYSV
      .4byte    .LCFI0-.LFB1
      .byte     0xd      /*  DW_CFA_def_cfa_register */
      .byte     0x08     /*  uleb128 0x08 */
      .byte     0x4      /*  DW_CFA_advance_loc4 */
      .4byte    .LCFI5-.LCFI0
      .byte     0x2f     /*  DW_CFA_GNU_negative_offset_extended */
      .byte     0x41     /*  uleb128 0x41 */
      .byte     0x1      /*  uleb128 0x1 */
      .byte     0x9f     /*  DW_CFA_offset, column 0x1f */
      .byte     0x1      /*  uleb128 0x1 */
      .byte     0x9e     /*  DW_CFA_offset, column 0x1e */
      .byte     0x2      /*  uleb128 0x2 */
      .byte     0x9d     /*  DW_CFA_offset, column 0x1d */
      .byte     0x3      /*  uleb128 0x3 */
      .byte     0x9c     /*  DW_CFA_offset, column 0x1c */
      .byte     0x4      /*  uleb128 0x4 */
      .byte     0x4      /*  DW_CFA_advance_loc4 */
      .4byte    .LCFI6-.LCFI5
      .byte     0xd      /*  DW_CFA_def_cfa_register */
      .byte     0x1c     /*  uleb128 0x1c */
      .align 2
.LEFDE1:
	/* Call ffi_prep_args_SYSV.  */
	bl	JUMPTARGET(ffi_prep_args_SYSV)
      .section	".eh_frame",EH_FRAME_FLAGS,@progbits
.Lframe1:
#if defined _RELOCATABLE || defined __PIC__
      .ascii	"zR\0"   /*  CIE Augmentation */
#else
      .ascii	"\0"	 /*  CIE Augmentation */
#endif
      .uleb128  0x1      /*  CIE Code Alignment Factor */
      .sleb128  -4	 /*  CIE Data Alignment Factor */
#if defined _RELOCATABLE || defined __PIC__
      .uleb128  0x1      /*  Augmentation size */
      .byte	0x1b	 /*  FDE Encoding (pcrel sdata4) */
#endif
      .uleb128  0x1
      .uleb128  0x0
      .4byte    .LASFDE1-.Lframe1         /*  FDE CIE offset */
#if defined _RELOCATABLE || defined __PIC__
      .4byte    .LFB1-.  /*  FDE initial location */
#else
#endif
#if defined _RELOCATABLE || defined __PIC__
      .uleb128  0x0	 /*  Augmentation size */
#endif
      .uleb128  0x08
      .uleb128  0x41
      .uleb128  0x1
      .uleb128  0x1
      .uleb128  0x2
      .uleb128  0x3
      .uleb128  0x4
      .uleb128  0x1c
#endif
