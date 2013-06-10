/* GCC MELT GENERATED FILE warmelt-normal+06.c - DO NOT EDIT */
/* secondary MELT generated C file of rank #6 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f6[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-normal+06.c declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-normal ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2008 - 2013 Free Software Foundation, Inc.
    Contributed by Basile Starynkevitch <basile@starynkevitch.net>

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
    <http://www.gnu.org/licenses/>.
***
----------------------------------------------------------------****/


/** ordinary MELT module meltbuild-sources/warmelt-normal**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1	/*usualmodule */


/**** no MELT module variables ****/

/*** 3 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t
melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdata,
						 melt_ptr_t meltinp0_PREVENV,
						 const char
						 *meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdata,
						      const char
						      *meltinp0_SYMNAMESTR,
						      const char
						      *meltinp1_MODULENAMESTR,
						      melt_ptr_t
						      meltinp2_PARENV);

/*declare MELT called hook #2 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdata,
					       melt_ptr_t meltinp0_SYM,
					       melt_ptr_t meltinp1_VAL,
					       melt_ptr_t meltinp2_CONTENV);

/*** end of 3 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_1_WARMELTmiNORMAL_NORMAL_IMPORT (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_2_WARMELTmiNORMAL_ADD_NCTX_DATA (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_WARMELTmiNORMAL_FILL_INITIAL_PREDEFMAP (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_4_WARMELTmiNORMAL_REGISTER_LITERAL_VALUE (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_5_WARMELTmiNORMAL_CREATE_NORMCONTEXT (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_6_WARMELTmiNORMAL_CREATE_NORMAL_EXTENDING_CONTEXT (meltclosure_ptr_t
							    meltclosp_,
							    melt_ptr_t
							    meltfirstargp_,
							    const
							    melt_argdescr_cell_t
							    meltxargdescr_[],
							    union meltparam_un
							    *meltxargtab_,
							    const
							    melt_argdescr_cell_t
							    meltxresdescr_[],
							    union meltparam_un
							    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_7_WARMELTmiNORMAL_NORMEXP_IDENTICAL (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_8_WARMELTmiNORMAL_NORMEXP_NULL (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiNORMAL_NORMEXP_ANY_OBJECT (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiNORMAL_NORMEXP_ANY_VALUE (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiNORMAL_NORMEXP_SRC_CATCHALL (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiNORMAL_NORMEXP_LAZYMACROEXP (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiNORMAL_GECTYP_ANYRECV (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiNORMAL_GECTYP_ROOT (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiNORMAL_GECTYP_INTEGER (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiNORMAL_GECTYP_STRING (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiNORMAL_NORMALIZE_TUPLE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiNORMAL_LAMBDA_cl1 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiNORMAL_WRAP_NORMAL_LET1 (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiNORMAL_LAMBDA_cl2 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiNORMAL_WRAP_NORMAL_LETSEQ (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiNORMAL_LAMBDA_cl3 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiNORMAL_CHECK_CTYPE_NARGS (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiNORMAL_LAMBDA_cl4 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiNORMAL_NORMBIND_FAILANY (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiNORMAL_NORMBIND_ANYBIND (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiNORMAL_NORMBIND_FORMALBIND (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiNORMAL_NORMBIND_LETBIND (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiNORMAL_NORMBIND_FIXBIND (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_WARMELTmiNORMAL_NORMBIND_DEFINEDVALBIND (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_31_WARMELTmiNORMAL_NORMBIND_CONSTRUCTBIND (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiNORMAL_NORMEXP_SYMBOL (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_33_WARMELTmiNORMAL_GECTYP_SYMOCC (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_WARMELTmiNORMAL_GECTYP_MODVAROCC (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_35_WARMELTmiNORMAL_NORMEXP_CLASS (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_WARMELTmiNORMAL_NORMEXP_PRIMITIVE (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_37_WARMELTmiNORMAL_NORMEXP_HOOK_CALL (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_WARMELTmiNORMAL_NORMEXP_BOX (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_39_WARMELTmiNORMAL_NORMEXP_CONSTBOX (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_40_WARMELTmiNORMAL_NORMEXP_UNBOX (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_41_WARMELTmiNORMAL_NORMEXP_CODE_CHUNK (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_42_WARMELTmiNORMAL_LAMBDA_cl5 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_43_WARMELTmiNORMAL_NORMEXP_EXPR_CHUNK (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_44_WARMELTmiNORMAL_NORMEXP_CMATCHEXPR (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_45_WARMELTmiNORMAL_LAMBDA_cl6 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_46_WARMELTmiNORMAL_LAMBDA_cl7 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_47_WARMELTmiNORMAL_NORMEXP_FUNMATCHEXPR (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_48_WARMELTmiNORMAL_NORMEXP_APPLY (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_49_WARMELTmiNORMAL_NORMEXP_MSEND (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_50_WARMELTmiNORMAL_NORMEXP_RETURN (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_51_WARMELTmiNORMAL_NORMEXP_IF (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_52_WARMELTmiNORMAL_LAMBDA_cl8 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_53_WARMELTmiNORMAL_NORMEXP_IFELSE (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_54_WARMELTmiNORMAL_LAMBDA_cl9 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_55_WARMELTmiNORMAL_LAMBDA_cl10 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_56_WARMELTmiNORMAL_NORMEXP_CPPIF (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_57_WARMELTmiNORMAL_LAMBDA_cl11 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_58_WARMELTmiNORMAL_LAMBDA_cl12 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_59_WARMELTmiNORMAL_NORMEXP_OR (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_60_WARMELTmiNORMAL_LAMBDA_cl13 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_61_WARMELTmiNORMAL_LAMBDA_cl14 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiNORMAL_NORMEXP_PROGN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_63_WARMELTmiNORMAL_LAMBDA_cl15 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_64_WARMELTmiNORMAL_NORMEXP_LET (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_65_WARMELTmiNORMAL_LAMBDA_cl16 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_66_WARMELTmiNORMAL_LAMBDA_cl17 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_67_WARMELTmiNORMAL_NORMEXP_UNSAFE_GET_FIELD (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_68_WARMELTmiNORMAL_NORMEXP_GET_FIELD (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_69_WARMELTmiNORMAL_NORMEXP_UNSAFE_PUT_FIELDS (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un
						       *meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un
						       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_70_WARMELTmiNORMAL_LAMBDA_cl18 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiNORMAL_LAMBDA_cl19 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiNORMAL_NORMEXP_PUT_FIELDS (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiNORMAL_NORMEXP_SETQ (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiNORMAL_LAMBDA_cl20 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiNORMAL_NORMEXP_INSTANCE (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiNORMAL_LAMBDA_cl21 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_77_WARMELTmiNORMAL_LAMBDA_cl22 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_78_WARMELTmiNORMAL_NORMEXP_FOREVER (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_79_WARMELTmiNORMAL_NORMEXP_EXIT (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiNORMAL_NORMEXP_AGAIN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiNORMAL_NORMEXP_IFVARIADIC (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiNORMAL_NORMEXP_COMPILEWARNING (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiNORMAL_LAMBDA_cl23 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiNORMAL_REPLACE_LAST_BY_RETURN (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiNORMAL_LAMBDA_cl24 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiNORMAL_NORMEXP_DEFUN (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_87_WARMELTmiNORMAL_LAMBDA_cl25 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_88_WARMELTmiNORMAL_LAMBDA_cl26 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_89_WARMELTmiNORMAL_NORMEXP_DEFHOOK (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_90_WARMELTmiNORMAL_LAMBDA_cl27 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_91_WARMELTmiNORMAL_NORMALIZE_LAMBDA (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_92_WARMELTmiNORMAL_LAMBDA_cl28 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_93_WARMELTmiNORMAL_NORMEXP_LAMBDA (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_94_WARMELTmiNORMAL_NORMEXP_MULTICALL (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_95_WARMELTmiNORMAL_LAMBDA_cl29 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_96_WARMELTmiNORMAL_LAMBDA_cl30 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_97_WARMELTmiNORMAL_LAMBDA_cl31 (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_98_WARMELTmiNORMAL_NORMEXP_TUPLE (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_99_WARMELTmiNORMAL_NORMEXP_LIST (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_100_WARMELTmiNORMAL_LAMBDA_cl32 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_101_WARMELTmiNORMAL_NORMEXP_ARITHMETIC_VARIADIC_OPERATION
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_102_WARMELTmiNORMAL_BADMETH_PREPARE_CONSTRUCTOR_BINDING
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_103_WARMELTmiNORMAL_BADMETH_NORMAL_LETREC_CONSTRUCTIVE
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_104_WARMELTmiNORMAL_PREPCONS_LAMBDA (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_105_WARMELTmiNORMAL_NORMLETREC_LAMBDA (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_106_WARMELTmiNORMAL_PREPCONS_TUPLE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_107_WARMELTmiNORMAL_NORMLETREC_TUPLE (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_108_WARMELTmiNORMAL_PREPCONS_LIST (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_109_WARMELTmiNORMAL_NRECLIST_FIND_LOCSYM (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_110_WARMELTmiNORMAL_NORMLETREC_LIST (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_111_WARMELTmiNORMAL_PREPCONS_INSTANCE (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_112_WARMELTmiNORMAL_NORMLETREC_INSTANCE (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_113_WARMELTmiNORMAL_LAMBDA_cl33 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_114_WARMELTmiNORMAL_NORMEXP_LETREC (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_115_WARMELTmiNORMAL_NORMAL_PREDEF (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_116_WARMELTmiNORMAL_NORMAL_SYMBOL_DATA (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_117_WARMELTmiNORMAL_NORMAL_KEYWORD_DATA (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_118_WARMELTmiNORMAL_CREATE_DATA_SLOTS (meltclosure_ptr_t meltclosp_,
						melt_ptr_t meltfirstargp_,
						const melt_argdescr_cell_t
						meltxargdescr_[],
						union meltparam_un
						*meltxargtab_,
						const melt_argdescr_cell_t
						meltxresdescr_[],
						union meltparam_un
						*meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_119_WARMELTmiNORMAL_FILL_DATA_SLOT (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_120_WARMELTmiNORMAL_NORMEXP_QUOTE (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_121_WARMELTmiNORMAL_NORMEXP_COMMENT (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_122_WARMELTmiNORMAL_NORMEXP_KEYWORD (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_123_WARMELTmiNORMAL_FILL_NORMAL_FORMALBIND (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_124_WARMELTmiNORMAL_FILL_NORMAL_FORMALS (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_125_WARMELTmiNORMAL_FILL_NORMAL_EXPANSION (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_126_WARMELTmiNORMAL_LAMBDA_cl34 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_127_WARMELTmiNORMAL_NORMEXP_DEFPRIMITIVE (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_128_WARMELTmiNORMAL_NORMEXP_DEFCITERATOR (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_129_WARMELTmiNORMAL_NORMEXP_CITERATION (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_130_WARMELTmiNORMAL_LAMBDA_cl35 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_131_WARMELTmiNORMAL_LAMBDA_cl36 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_132_WARMELTmiNORMAL_LAMBDA_cl37 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_133_WARMELTmiNORMAL_LAMBDA_cl38 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_134_WARMELTmiNORMAL_LAMBDA_cl39 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_135_WARMELTmiNORMAL_LAMBDA_cl40 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_136_WARMELTmiNORMAL_LAMBDA_cl41 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_137_WARMELTmiNORMAL_NORMEXP_DEFCMATCHER (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_138_WARMELTmiNORMAL_NORMEXP_DEFUNMATCHER (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_139_WARMELTmiNORMAL_NORMEXP_DEFCLASS (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_140_WARMELTmiNORMAL_NORMEXP_DEFINSTANCE (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_141_WARMELTmiNORMAL_LAMBDA_cl42 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_142_WARMELTmiNORMAL_NORMEXP_DEFVAR (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_143_WARMELTmiNORMAL_NORMEXP_DEFINE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_144_WARMELTmiNORMAL_LAMBDA_cl43 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_145_WARMELTmiNORMAL_NORMEXP_DEFSELECTOR (meltclosure_ptr_t
						  meltclosp_,
						  melt_ptr_t meltfirstargp_,
						  const melt_argdescr_cell_t
						  meltxargdescr_[],
						  union meltparam_un
						  *meltxargtab_,
						  const melt_argdescr_cell_t
						  meltxresdescr_[],
						  union meltparam_un
						  *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_146_WARMELTmiNORMAL_LAMBDA_cl44 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_147_WARMELTmiNORMAL_NORMAL_EXPORTED_VALUE (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_148_WARMELTmiNORMAL_NORMEXP_EXPORT_VALUES (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_149_WARMELTmiNORMAL_NORMEXP_EXPORT_SYNONYM (meltclosure_ptr_t
						     meltclosp_,
						     melt_ptr_t
						     meltfirstargp_,
						     const
						     melt_argdescr_cell_t
						     meltxargdescr_[],
						     union meltparam_un
						     *meltxargtab_,
						     const
						     melt_argdescr_cell_t
						     meltxresdescr_[],
						     union meltparam_un
						     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_150_WARMELTmiNORMAL_NORMEXP_EXPORT_CLASS (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_151_WARMELTmiNORMAL_LAMBDA_cl45 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_152_WARMELTmiNORMAL_LAMBDA_cl46 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_153_WARMELTmiNORMAL_NORMAL_EXPORTED_MACRO (meltclosure_ptr_t
						    meltclosp_,
						    melt_ptr_t meltfirstargp_,
						    const melt_argdescr_cell_t
						    meltxargdescr_[],
						    union meltparam_un
						    *meltxargtab_,
						    const melt_argdescr_cell_t
						    meltxresdescr_[],
						    union meltparam_un
						    *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un
						       *meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un
						       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un
						      *meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un
						      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un
						       *meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un
						       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un
						       *meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un
						       *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un
					      *meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un
					      *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un
						 *meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un
						 *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);



MELT_EXTERN void *melt_start_this_module (void *);
struct melt_initial_frame_st;

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiNORMAL__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiNORMAL__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_WARMELTmiNORMAL__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiNORMAL__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_4 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_5 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_6 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_7 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_8 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_9 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_10 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_11 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_12 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_13 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_14 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_15 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_16 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_17 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_18 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_19 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_20 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_21 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_22 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_23 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_24 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_25 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_26 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_27 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_28 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_29 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_30 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_31 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_32 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_33 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_34 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_35 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_36 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_37 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_38 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_39 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_40 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_41 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_42 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_43 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_44 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_45 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_46 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_47 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_48 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_49 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_50 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_51 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_52 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_53 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_54 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_55 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__initialmeltchunk_56 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiNORMAL__forward_or_mark_module_start_frame (struct
							      melt_callframe_st
							      *fp,
							      int marking);



/**** warmelt-normal+06.c implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO (meltclosure_ptr_t
						   meltclosp_,
						   melt_ptr_t meltfirstargp_,
						   const melt_argdescr_cell_t
						   meltxargdescr_[],
						   union meltparam_un
						   *meltxargtab_,
						   const melt_argdescr_cell_t
						   meltxresdescr_[],
						   union meltparam_un
						   *meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 39
    melt_ptr_t mcfr_varptr[39];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO_st
	 *) meltfirstargp_;
      /* use arguments meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 39; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO nbval 39*/
  meltfram__.mcfr_nbvar = 39 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_EXPORT_MACRO", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8057:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8058:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8058:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8058:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8058;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp export_macro recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V7*/ meltfptr[6] =
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8058:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V7*/ meltfptr[6] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8058:/ quasiblock");


      /*_.PROGN___V9*/ meltfptr[7] = /*_._IF___V7*/ meltfptr[6];;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_.PROGN___V9*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8058:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8059:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_MACRO */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8059:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8059:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check export_macro recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8059) ? (8059) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8059:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8060:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8060:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8060:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8060) ? (8060) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8060:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8061:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8061:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8061:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8061) ? (8061) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8061:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8062:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8063:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8064:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXPMAC_MNAME");
  /*_.MNAME__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8065:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "SEXPMAC_MVAL");
  /*_.MVALEXP__V19*/ meltfptr[18] = slot;
    };
    ;
 /*_.BINDSLIST__V20*/ meltfptr[19] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[4]))));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8067:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_NIL */ meltfrout->
					     tabval[5])), (1),
			      "CLASS_NREP_NIL");
  /*_.INST__V22*/ meltfptr[21] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V22*/ meltfptr[21])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V22*/ meltfptr[21]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[16]), "NREP_LOC");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V22*/ meltfptr[21],
				  "newly made instance");
    ;
    /*_.NREPNIL__V21*/ meltfptr[20] = /*_.INST__V22*/ meltfptr[21];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8069:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L6*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.MNAME__V18*/ meltfptr[17]),
			     (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					    tabval[6])));;
      MELT_LOCATION ("warmelt-normal.melt:8069:/ cond");
      /*cond */ if ( /*_#IS_A__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V24*/ meltfptr[23] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8069:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mname"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8069) ? (8069) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V24*/ meltfptr[23] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V23*/ meltfptr[22] = /*_._IFELSE___V24*/ meltfptr[23];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8069:/ clear");
	     /*clear *//*_#IS_A__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V24*/ meltfptr[23] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V23*/ meltfptr[22] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8070:/ quasiblock");


    /*^multimsend */
    /*multimsend */
    {
      union meltparam_un argtab[3];
      union meltparam_un restab[1];
      memset (&argtab, 0, sizeof (argtab));
      memset (&restab, 0, sizeof (restab));
      /*^multimsend.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];	/*^multimsend.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];	/*^multimsend.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[16];
      /*^multimsend.xres */
      restab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NBINDS__V27*/ meltfptr[26];	/*^multimsend.send */
      /*_.NEXP__V26*/ meltfptr[25] =
	meltgc_send ((melt_ptr_t) ( /*_.MVALEXP__V19*/ meltfptr[18]),
		     ((melt_ptr_t)
		      (( /*!NORMAL_EXP */ meltfrout->tabval[7]))),
		     (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		     argtab, (MELTBPARSTR_PTR ""), restab);
    }
    ;
    /*^quasiblock */



    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8073:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NBINDS__V27*/ meltfptr[26];
      /*_.LIST_APPEND2LIST__V28*/ meltfptr[27] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_APPEND2LIST */ meltfrout->tabval[8])),
		    (melt_ptr_t) ( /*_.BINDSLIST__V20*/ meltfptr[19]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8074:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8075:/ apply");
    /*apply */
    {
      union meltparam_un argtab[5];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NEXP__V26*/ meltfptr[25];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[3].meltbp_aptr = (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[16];
      /*^apply.arg */
      argtab[4].meltbp_aptr =
	(melt_ptr_t *) & /*_.BINDSLIST__V20*/ meltfptr[19];
      /*_.NEXPM__V30*/ meltfptr[29] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!NORMAL_EXPORTED_MACRO */ meltfrout->tabval[9])),
		    (melt_ptr_t) ( /*_.MNAME__V18*/ meltfptr[17]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR
		     MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8077:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8077:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L8*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8077:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8077;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_export_macro nexpm";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NEXPM__V30*/ meltfptr[29];
	      /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V32*/ meltfptr[31] =
	      /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8077:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V32*/ meltfptr[31] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8077:/ quasiblock");


      /*_.PROGN___V34*/ meltfptr[32] = /*_._IF___V32*/ meltfptr[31];;
      /*^compute */
      /*_.IFCPP___V31*/ meltfptr[30] = /*_.PROGN___V34*/ meltfptr[32];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8077:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V32*/ meltfptr[31] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V34*/ meltfptr[32] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V31*/ meltfptr[30] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    /*^compute */
    /*_.LET___V29*/ meltfptr[28] = /*_.IFCPP___V31*/ meltfptr[30];;

    MELT_LOCATION ("warmelt-normal.melt:8074:/ clear");
	   /*clear *//*_.NEXPM__V30*/ meltfptr[29] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V31*/ meltfptr[30] = 0;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8079:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L9*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8079:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L10*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8079:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8079;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_export_macro final nrepnil=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NREPNIL__V21*/ meltfptr[20];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " bindslist=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.BINDSLIST__V20*/ meltfptr[19];
	      /*_.MELT_DEBUG_FUN__V37*/ meltfptr[29] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V36*/ meltfptr[32] =
	      /*_.MELT_DEBUG_FUN__V37*/ meltfptr[29];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8079:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V37*/ meltfptr[29] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V36*/ meltfptr[32] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8079:/ quasiblock");


      /*_.PROGN___V38*/ meltfptr[30] = /*_._IF___V36*/ meltfptr[32];;
      /*^compute */
      /*_.IFCPP___V35*/ meltfptr[31] = /*_.PROGN___V38*/ meltfptr[30];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8079:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V36*/ meltfptr[32] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V38*/ meltfptr[30] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V35*/ meltfptr[31] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8081:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.NREPNIL__V21*/ meltfptr[20];;
    MELT_LOCATION ("warmelt-normal.melt:8081:/ putxtraresult");
    if (!meltxrestab_ || !meltxresdescr_)
      goto meltlabend_rout;
    if (meltxresdescr_[0] != MELTBPAR_PTR)
      goto meltlabend_rout;
    if (meltxrestab_[0].meltbp_aptr)
      *(meltxrestab_[0].meltbp_aptr) =
	(melt_ptr_t) ( /*_.BINDSLIST__V20*/ meltfptr[19]);
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;

    MELT_CHECK_SIGNAL ();
    ;
    /*_.MULTI___V25*/ meltfptr[23] = /*_.RETURN___V39*/ meltfptr[29];;

    MELT_LOCATION ("warmelt-normal.melt:8070:/ clear");
	   /*clear *//*_.LIST_APPEND2LIST__V28*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.LET___V29*/ meltfptr[28] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V35*/ meltfptr[31] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V39*/ meltfptr[29] = 0;

    /*^clear */
	   /*clear *//*_.NBINDS__V27*/ meltfptr[26] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.MULTI___V25*/ meltfptr[23];;

    MELT_LOCATION ("warmelt-normal.melt:8062:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.MNAME__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.MVALEXP__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.BINDSLIST__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.NREPNIL__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.MULTI___V25*/ meltfptr[23] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8057:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8057:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_EXPORT_MACRO", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_154_WARMELTmiNORMAL_NORMEXP_EXPORT_MACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un *
						       meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un *
						       meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 62
    melt_ptr_t mcfr_varptr[62];
#define MELTFRAM_NBVARNUM 14
    long mcfr_varnum[14];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO_st *)
	meltfirstargp_;
      /* use arguments meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 62; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO nbval 62*/
  meltfram__.mcfr_nbvar = 62 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMAL_EXPORTED_PATMACRO", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8092:/ getarg");
 /*_.SYM__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NMACROXP__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NMACROXP__V3*/ meltfptr[2])) !=
	      NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NPATTXP__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NPATTXP__V4*/ meltfptr[3])) !=
	      NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V5*/ meltfptr[4])) != NULL);


  /*getarg#4 */
  /*^getarg */
  if (meltxargdescr_[3] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V6*/ meltfptr[5] =
    (meltxargtab_[3].meltbp_aptr) ? (*(meltxargtab_[3].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V6*/ meltfptr[5])) != NULL);


  /*getarg#5 */
  /*^getarg */
  if (meltxargdescr_[4] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V7*/ meltfptr[6] =
    (meltxargtab_[4].meltbp_aptr) ? (*(meltxargtab_[4].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V7*/ meltfptr[6])) !=
	      NULL);


  /*getarg#6 */
  /*^getarg */
  if (meltxargdescr_[5] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.BINDSLIST__V8*/ meltfptr[7] =
    (meltxargtab_[5].meltbp_aptr) ? (*(meltxargtab_[5].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.BINDSLIST__V8*/ meltfptr[7])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8093:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8093:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8093:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8093;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normal_exported_patmacro start sym=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SYM__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " nmacroxp=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.NMACROXP__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " npattxp=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & /*_.NPATTXP__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V10*/ meltfptr[9] =
	      /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8093:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V11*/ meltfptr[10] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V10*/ meltfptr[9] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8093:/ quasiblock");


      /*_.PROGN___V12*/ meltfptr[10] = /*_._IF___V10*/ meltfptr[9];;
      /*^compute */
      /*_.IFCPP___V9*/ meltfptr[8] = /*_.PROGN___V12*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8093:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V10*/ meltfptr[9] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V12*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V9*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8095:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.SYM__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					    tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8095:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V14*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8095:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check sym"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8095) ? (8095) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V14*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V13*/ meltfptr[9] = /*_._IFELSE___V14*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8095:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V14*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V13*/ meltfptr[9] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8096:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NMACROXP__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_NREP */ meltfrout->
					    tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8096:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V16*/ meltfptr[15] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8096:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nmacroxp"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8096) ? (8096) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V16*/ meltfptr[15] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V15*/ meltfptr[10] = /*_._IFELSE___V16*/ meltfptr[15];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8096:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V16*/ meltfptr[15] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V15*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8097:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V5*/ meltfptr[4]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8097:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V18*/ meltfptr[17] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8097:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8097) ? (8097) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V18*/ meltfptr[17] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V17*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[17];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8097:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V18*/ meltfptr[17] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V17*/ meltfptr[15] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8098:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L6*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V6*/ meltfptr[5]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[4])));;
      MELT_LOCATION ("warmelt-normal.melt:8098:/ cond");
      /*cond */ if ( /*_#IS_A__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V20*/ meltfptr[19] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8098:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check ncx"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8098) ? (8098) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V20*/ meltfptr[19] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V19*/ meltfptr[17] = /*_._IFELSE___V20*/ meltfptr[19];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8098:/ clear");
	     /*clear *//*_#IS_A__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V20*/ meltfptr[19] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V19*/ meltfptr[17] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8099:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L7*/ meltfnum[1] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.BINDSLIST__V8*/ meltfptr[7])) ==
	 MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-normal.melt:8099:/ cond");
      /*cond */ if ( /*_#IS_LIST__L7*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V22*/ meltfptr[21] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8099:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check bindslist"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8099) ? (8099) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V22*/ meltfptr[21] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V21*/ meltfptr[19] = /*_._IFELSE___V22*/ meltfptr[21];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8099:/ clear");
	     /*clear *//*_#IS_LIST__L7*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V22*/ meltfptr[21] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V21*/ meltfptr[19] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8100:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      /*_.CSYMEXPO__V24*/ meltfptr[23] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!CLONE_SYMBOL */ meltfrout->tabval[5])),
		    (melt_ptr_t) (( /*!konst_6__EXPORTEDPAT_ */ meltfrout->
				   tabval[6])), (""),
		    (union meltparam_un *) 0, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8101:/ apply");
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V6*/ meltfptr[5];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.PSLOC__V7*/ meltfptr[6];
      /*_.SYMDATA__V25*/ meltfptr[24] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!NORMAL_SYMBOL_DATA */ meltfrout->tabval[7])),
		    (melt_ptr_t) ( /*_.SYM__V2*/ meltfptr[1]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8102:/ quasiblock");


 /*_.SB__V27*/ meltfptr[26] =
      (melt_ptr_t)
      meltgc_new_strbuf ((meltobject_ptr_t)
			 (( /*!DISCR_STRBUF */ meltfrout->tabval[8])),
			 (const char *) 0);;

    {
      MELT_LOCATION ("warmelt-normal.melt:8103:/ locexp");
      /*add2sbuf_strconst */
	meltgc_add_strbuf ((melt_ptr_t) ( /*_.SB__V27*/ meltfptr[26]),
			   ("norm.exp.pat : "));
    }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8104:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.SYM__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "NAMED_NAME");
  /*_.NAMED_NAME__V28*/ meltfptr[27] = slot;
    };
    ;

    {
      /*^locexp */
      /*add2sbuf_string */
	meltgc_add_strbuf ((melt_ptr_t) ( /*_.SB__V27*/ meltfptr[26]),
			   melt_string_str ((melt_ptr_t)
					    ( /*_.NAMED_NAME__V28*/
					     meltfptr[27])));
    }
    ;
    /*_.LET___V26*/ meltfptr[25] = /*_.SB__V27*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-normal.melt:8102:/ clear");
	   /*clear *//*_.SB__V27*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.NAMED_NAME__V28*/ meltfptr[27] = 0;
    /*_.CSBUF__V29*/ meltfptr[26] = /*_.LET___V26*/ meltfptr[25];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_.STRBUF2STRING__V30*/ meltfptr[27] =
      (meltgc_new_stringdup
       ((meltobject_ptr_t) (( /*!DISCR_STRING */ meltfrout->tabval[10])),
	melt_strbuf_str ((melt_ptr_t) ( /*_.CSBUF__V29*/ meltfptr[26]))));;
    MELT_LOCATION ("warmelt-normal.melt:8106:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[9])), (3), "CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE");
  /*_.INST__V32*/ meltfptr[31] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @LOCA_LOCATION",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V32*/ meltfptr[31])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V32*/ meltfptr[31]), (1),
			  ( /*_.PSLOC__V7*/ meltfptr[6]), "LOCA_LOCATION");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @CMEC_COMMENT",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V32*/ meltfptr[31])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V32*/ meltfptr[31]), (2),
			  ( /*_.STRBUF2STRING__V30*/ meltfptr[27]),
			  "CMEC_COMMENT");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V32*/ meltfptr[31],
				  "newly made instance");
    ;
    /*_.SCURENVBOX__V31*/ meltfptr[30] = /*_.INST__V32*/ meltfptr[31];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8110:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L8*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8110:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L8*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L9*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8110:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8110;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normal_exported_patmacro sgetcurenvbox";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.SCURENVBOX__V31*/ meltfptr[30];
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V34*/ meltfptr[33] =
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8110:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L9*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V35*/ meltfptr[34] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V34*/ meltfptr[33] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8110:/ quasiblock");


      /*_.PROGN___V36*/ meltfptr[34] = /*_._IF___V34*/ meltfptr[33];;
      /*^compute */
      /*_.IFCPP___V33*/ meltfptr[32] = /*_.PROGN___V36*/ meltfptr[34];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8110:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L8*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V34*/ meltfptr[33] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V36*/ meltfptr[34] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V33*/ meltfptr[32] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8111:/ quasiblock");


    /*^multimsend */
    /*multimsend */
    {
      union meltparam_un argtab[3];
      union meltparam_un restab[1];
      memset (&argtab, 0, sizeof (argtab));
      memset (&restab, 0, sizeof (restab));
      /*^multimsend.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V5*/ meltfptr[4];	/*^multimsend.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V6*/ meltfptr[5];	/*^multimsend.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.PSLOC__V7*/ meltfptr[6];
      /*^multimsend.xres */
      restab[0].meltbp_aptr = (melt_ptr_t *) & /*_.CURENVBINDS__V39*/ meltfptr[38];	/*^multimsend.send */
      /*_.NCURENVBOX__V38*/ meltfptr[34] =
	meltgc_send ((melt_ptr_t) ( /*_.SCURENVBOX__V31*/ meltfptr[30]),
		     ((melt_ptr_t)
		      (( /*!NORMAL_EXP */ meltfrout->tabval[11]))),
		     (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		     argtab, (MELTBPARSTR_PTR ""), restab);
    }
    ;
    /*^quasiblock */



#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8114:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L10*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8114:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8114:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8114;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normal_exported_patmacro ncurenvbox=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NCURENVBOX__V38*/ meltfptr[34];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " curenvbinds=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.CURENVBINDS__V39*/ meltfptr[38];
	      /*_.MELT_DEBUG_FUN__V42*/ meltfptr[41] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V41*/ meltfptr[40] =
	      /*_.MELT_DEBUG_FUN__V42*/ meltfptr[41];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8114:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V42*/ meltfptr[41] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V41*/ meltfptr[40] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8114:/ quasiblock");


      /*_.PROGN___V43*/ meltfptr[41] = /*_._IF___V41*/ meltfptr[40];;
      /*^compute */
      /*_.IFCPP___V40*/ meltfptr[39] = /*_.PROGN___V43*/ meltfptr[41];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8114:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V41*/ meltfptr[40] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V43*/ meltfptr[41] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V40*/ meltfptr[39] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_LIST__L12*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.CURENVBINDS__V39*/ meltfptr[38]))
       == MELTOBMAG_LIST);;
    MELT_LOCATION ("warmelt-normal.melt:8116:/ cond");
    /*cond */ if ( /*_#IS_LIST__L12*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^apply */
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.CURENVBINDS__V39*/ meltfptr[38];
	    /*_.LIST_APPEND2LIST__V45*/ meltfptr[41] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!LIST_APPEND2LIST */ meltfrout->tabval[12])),
			  (melt_ptr_t) ( /*_.BINDSLIST__V8*/ meltfptr[7]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_._IF___V44*/ meltfptr[40] =
	    /*_.LIST_APPEND2LIST__V45*/ meltfptr[41];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8116:/ clear");
	     /*clear *//*_.LIST_APPEND2LIST__V45*/ meltfptr[41] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V44*/ meltfptr[40] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8117:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8118:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_1_st
      {
	struct MELT_MULTIPLE_STRUCT (4) rtup_0__TUPLREC__x20;
	long meltletrec_1_endgap;
      } *meltletrec_1_ptr = 0;
      meltletrec_1_ptr =
	(struct meltletrec_1_st *)
	meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x20 */
 /*_.TUPLREC___V48*/ meltfptr[47] =
	(melt_ptr_t) & meltletrec_1_ptr->rtup_0__TUPLREC__x20;
      meltletrec_1_ptr->rtup_0__TUPLREC__x20.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_1_ptr->rtup_0__TUPLREC__x20.nbval = 4;


      /*^putuple */
      /*putupl#55 */
      melt_assertmsg ("putupl [:8118] #55 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V48*/ meltfptr[47]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:8118] #55 checkoff",
		      (0 >= 0
		       && 0 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V48*/
					      meltfptr[47]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V48*/ meltfptr[47]))->tabval[0] =
	(melt_ptr_t) ( /*_.SYMDATA__V25*/ meltfptr[24]);
      ;
      /*^putuple */
      /*putupl#56 */
      melt_assertmsg ("putupl [:8118] #56 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V48*/ meltfptr[47]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:8118] #56 checkoff",
		      (1 >= 0
		       && 1 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V48*/
					      meltfptr[47]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V48*/ meltfptr[47]))->tabval[1] =
	(melt_ptr_t) ( /*_.NMACROXP__V3*/ meltfptr[2]);
      ;
      /*^putuple */
      /*putupl#57 */
      melt_assertmsg ("putupl [:8118] #57 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V48*/ meltfptr[47]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:8118] #57 checkoff",
		      (2 >= 0
		       && 2 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V48*/
					      meltfptr[47]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V48*/ meltfptr[47]))->tabval[2] =
	(melt_ptr_t) ( /*_.NPATTXP__V4*/ meltfptr[3]);
      ;
      /*^putuple */
      /*putupl#58 */
      melt_assertmsg ("putupl [:8118] #58 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V48*/ meltfptr[47]))
		      == MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:8118] #58 checkoff",
		      (3 >= 0
		       && 3 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V48*/
					      meltfptr[47]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V48*/ meltfptr[47]))->tabval[3] =
	(melt_ptr_t) ( /*_.NCURENVBOX__V38*/ meltfptr[34]);
      ;
      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V48*/ meltfptr[47]);
      ;
      /*_.ARGTUP__V47*/ meltfptr[46] = /*_.TUPLREC___V48*/ meltfptr[47];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8118:/ clear");
	    /*clear *//*_.TUPLREC___V48*/ meltfptr[47] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V48*/ meltfptr[47] = 0;
    }				/*end multiallocblock */
    ;

    MELT_CHECK_SIGNAL ();
    ;

    MELT_CHECK_SIGNAL ();
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8128:/ apply");
    /*apply */
    {
      union meltparam_un argtab[3];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V6*/ meltfptr[5];
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.PSLOC__V7*/ meltfptr[6];
      /*^apply.arg */
      argtab[2].meltbp_cstring = "hook_patmacro_exporter";
      /*_.NORMAL_PREDEF__V49*/ meltfptr[47] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!NORMAL_PREDEF */ meltfrout->tabval[17])),
		    (melt_ptr_t) (( /*!HOOK_PATMACRO_EXPORTER */ meltfrout->
				   tabval[18])),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8130:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_2_st
      {
	struct MELT_MULTIPLE_STRUCT (0) rtup_0__TUPLREC__x21;
	long meltletrec_2_endgap;
      } *meltletrec_2_ptr = 0;
      meltletrec_2_ptr =
	(struct meltletrec_2_st *)
	meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x21 */
 /*_.TUPLREC___V51*/ meltfptr[50] =
	(melt_ptr_t) & meltletrec_2_ptr->rtup_0__TUPLREC__x21;
      meltletrec_2_ptr->rtup_0__TUPLREC__x21.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_2_ptr->rtup_0__TUPLREC__x21.nbval = 0;


      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V51*/ meltfptr[50]);
      ;
      /*_.TUPLE___V50*/ meltfptr[49] = /*_.TUPLREC___V51*/ meltfptr[50];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8130:/ clear");
	    /*clear *//*_.TUPLREC___V51*/ meltfptr[50] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V51*/ meltfptr[50] = 0;
    }				/*end multiallocblock */
    ;
 /*_.HOOK_DATA__V52*/ meltfptr[50] =
      melt_hook_data ((melt_ptr_t)
		      ( /*!HOOK_PATMACRO_EXPORTER */ meltfrout->tabval[18]));;
    MELT_LOCATION ("warmelt-normal.melt:8124:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_HOOK_CALL */
					     meltfrout->tabval[15])), (7),
			      "CLASS_NREP_HOOK_CALL");
  /*_.INST__V54*/ meltfptr[53] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NEXPR_CTYP",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V54*/ meltfptr[53])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V54*/ meltfptr[53]), (1),
			  (( /*!CTYPE_VOID */ meltfrout->tabval[14])),
			  "NEXPR_CTYP");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NHOOK_NAME",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V54*/ meltfptr[53])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V54*/ meltfptr[53]), (3),
			  (( /*!konst_16 */ meltfrout->tabval[16])),
			  "NHOOK_NAME");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NEXPR_ARGS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V54*/ meltfptr[53])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V54*/ meltfptr[53]), (2),
			  ( /*_.ARGTUP__V47*/ meltfptr[46]), "NEXPR_ARGS");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NHOOK_CALLED",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V54*/ meltfptr[53])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V54*/ meltfptr[53]), (4),
			  ( /*_.NORMAL_PREDEF__V49*/ meltfptr[47]),
			  "NHOOK_CALLED");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NHOOK_OUTS",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V54*/ meltfptr[53])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V54*/ meltfptr[53]), (5),
			  ( /*_.TUPLE___V50*/ meltfptr[49]), "NHOOK_OUTS");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NHOOK_DESCR",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V54*/ meltfptr[53])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V54*/ meltfptr[53]), (6),
			  ( /*_.HOOK_DATA__V52*/ meltfptr[50]),
			  "NHOOK_DESCR");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V54*/ meltfptr[53],
				  "newly made instance");
    ;
    /*_.INST___V53*/ meltfptr[52] = /*_.INST__V54*/ meltfptr[53];;
    MELT_LOCATION ("warmelt-normal.melt:8119:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NORMAL_LET_BINDING */
					     meltfrout->tabval[13])), (4),
			      "CLASS_NORMAL_LET_BINDING");
  /*_.INST__V56*/ meltfptr[55] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @LETBIND_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V56*/ meltfptr[55])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V56*/ meltfptr[55]), (3),
			  ( /*_.PSLOC__V7*/ meltfptr[6]), "LETBIND_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @BINDER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V56*/ meltfptr[55])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V56*/ meltfptr[55]), (0),
			  ( /*_.CSYMEXPO__V24*/ meltfptr[23]), "BINDER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @LETBIND_TYPE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V56*/ meltfptr[55])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V56*/ meltfptr[55]), (1),
			  (( /*!CTYPE_VOID */ meltfrout->tabval[14])),
			  "LETBIND_TYPE");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @LETBIND_EXPR",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V56*/ meltfptr[55])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V56*/ meltfptr[55]), (2),
			  ( /*_.INST___V53*/ meltfptr[52]), "LETBIND_EXPR");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V56*/ meltfptr[55],
				  "newly made instance");
    ;
    /*_.CBIND__V55*/ meltfptr[54] = /*_.INST__V56*/ meltfptr[55];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8133:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_LOCSYMOCC */
					     meltfrout->tabval[19])), (4),
			      "CLASS_NREP_LOCSYMOCC");
  /*_.INST__V58*/ meltfptr[57] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V58*/ meltfptr[57])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V58*/ meltfptr[57]), (0),
			  ( /*_.PSLOC__V7*/ meltfptr[6]), "NREP_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NOCC_CTYP",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V58*/ meltfptr[57])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V58*/ meltfptr[57]), (2),
			  (( /*!CTYPE_VOID */ meltfrout->tabval[14])),
			  "NOCC_CTYP");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NOCC_SYMB",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V58*/ meltfptr[57])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V58*/ meltfptr[57]), (1),
			  ( /*_.CSYMEXPO__V24*/ meltfptr[23]), "NOCC_SYMB");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NOCC_BIND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V58*/ meltfptr[57])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V58*/ meltfptr[57]), (3),
			  ( /*_.CBIND__V55*/ meltfptr[54]), "NOCC_BIND");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V58*/ meltfptr[57],
				  "newly made instance");
    ;
    /*_.SYOCC__V57*/ meltfptr[56] = /*_.INST__V58*/ meltfptr[57];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8139:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L13*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8139:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L13*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8139:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8139;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normal_exported_patmacro cbind=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CBIND__V55*/ meltfptr[54];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " return syocc=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.SYOCC__V57*/ meltfptr[56];
	      /*_.MELT_DEBUG_FUN__V61*/ meltfptr[60] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V60*/ meltfptr[59] =
	      /*_.MELT_DEBUG_FUN__V61*/ meltfptr[60];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8139:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L14*/ meltfnum[13] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V61*/ meltfptr[60] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V60*/ meltfptr[59] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8139:/ quasiblock");


      /*_.PROGN___V62*/ meltfptr[60] = /*_._IF___V60*/ meltfptr[59];;
      /*^compute */
      /*_.IFCPP___V59*/ meltfptr[58] = /*_.PROGN___V62*/ meltfptr[60];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8139:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L13*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V60*/ meltfptr[59] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V62*/ meltfptr[60] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V59*/ meltfptr[58] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-normal.melt:8140:/ locexp");
      meltgc_append_list ((melt_ptr_t) ( /*_.BINDSLIST__V8*/ meltfptr[7]),
			  (melt_ptr_t) ( /*_.CBIND__V55*/ meltfptr[54]));
    }
    ;
    /*_.LET___V46*/ meltfptr[41] = /*_.SYOCC__V57*/ meltfptr[56];;

    MELT_LOCATION ("warmelt-normal.melt:8117:/ clear");
	   /*clear *//*_.ARGTUP__V47*/ meltfptr[46] = 0;
    /*^clear */
	   /*clear *//*_.NORMAL_PREDEF__V49*/ meltfptr[47] = 0;
    /*^clear */
	   /*clear *//*_.TUPLE___V50*/ meltfptr[49] = 0;
    /*^clear */
	   /*clear *//*_.HOOK_DATA__V52*/ meltfptr[50] = 0;
    /*^clear */
	   /*clear *//*_.INST___V53*/ meltfptr[52] = 0;
    /*^clear */
	   /*clear *//*_.CBIND__V55*/ meltfptr[54] = 0;
    /*^clear */
	   /*clear *//*_.SYOCC__V57*/ meltfptr[56] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V59*/ meltfptr[58] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*_.MULTI___V37*/ meltfptr[33] = /*_.LET___V46*/ meltfptr[41];;

    MELT_LOCATION ("warmelt-normal.melt:8111:/ clear");
	   /*clear *//*_.IFCPP___V40*/ meltfptr[39] = 0;
    /*^clear */
	   /*clear *//*_#IS_LIST__L12*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V44*/ meltfptr[40] = 0;
    /*^clear */
	   /*clear *//*_.LET___V46*/ meltfptr[41] = 0;

    /*^clear */
	   /*clear *//*_.CURENVBINDS__V39*/ meltfptr[38] = 0;
    /*_.LET___V23*/ meltfptr[21] = /*_.MULTI___V37*/ meltfptr[33];;

    MELT_LOCATION ("warmelt-normal.melt:8100:/ clear");
	   /*clear *//*_.CSYMEXPO__V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.SYMDATA__V25*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.LET___V26*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_.CSBUF__V29*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.STRBUF2STRING__V30*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.SCURENVBOX__V31*/ meltfptr[30] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V33*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_.MULTI___V37*/ meltfptr[33] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8092:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V23*/ meltfptr[21];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8092:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V9*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V13*/ meltfptr[9] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V15*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V17*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V19*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V21*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.LET___V23*/ meltfptr[21] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMAL_EXPORTED_PATMACRO", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_155_WARMELTmiNORMAL_NORMAL_EXPORTED_PATMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO (meltclosure_ptr_t
						      meltclosp_,
						      melt_ptr_t
						      meltfirstargp_,
						      const
						      melt_argdescr_cell_t
						      meltxargdescr_[],
						      union meltparam_un *
						      meltxargtab_,
						      const
						      melt_argdescr_cell_t
						      meltxresdescr_[],
						      union meltparam_un *
						      meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 56
    melt_ptr_t mcfr_varptr[56];
#define MELTFRAM_NBVARNUM 16
    long mcfr_varnum[16];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO_st *)
	meltfirstargp_;
      /* use arguments meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 56; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO nbval 56*/
  meltfram__.mcfr_nbvar = 56 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_EXPORT_PATMACRO", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8147:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8148:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8148:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8148:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8148;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp export_patmacro recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V7*/ meltfptr[6] =
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8148:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V7*/ meltfptr[6] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8148:/ quasiblock");


      /*_.PROGN___V9*/ meltfptr[7] = /*_._IF___V7*/ meltfptr[6];;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_.PROGN___V9*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8148:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8149:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_EXPORT_PATMACRO */ meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8149:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8149:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check export_patmacro recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8149) ? (8149) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8149:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8150:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8150:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8150:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8150) ? (8150) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8150:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8151:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8151:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8151:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8151) ? (8151) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8151:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8152:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8153:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8154:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SEXPMAC_MNAME");
  /*_.MNAME__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8155:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "SEXPMAC_MVAL");
  /*_.MVALEXP__V19*/ meltfptr[18] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8156:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 5, "SEXPPAT_PVAL");
  /*_.MPATEXP__V20*/ meltfptr[19] = slot;
    };
    ;
 /*_.BINDSLIST__V21*/ meltfptr[20] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[4]))));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8158:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_NIL */ meltfrout->
					     tabval[5])), (1),
			      "CLASS_NREP_NIL");
  /*_.INST__V23*/ meltfptr[22] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V23*/ meltfptr[22])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[16]), "NREP_LOC");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V23*/ meltfptr[22],
				  "newly made instance");
    ;
    /*_.NREPNIL__V22*/ meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8160:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L6*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8160:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L7*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8160:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8160;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp export_patmacro mname=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MNAME__V18*/ meltfptr[17];
	      /*_.MELT_DEBUG_FUN__V26*/ meltfptr[25] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V25*/ meltfptr[24] =
	      /*_.MELT_DEBUG_FUN__V26*/ meltfptr[25];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8160:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L7*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V26*/ meltfptr[25] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V25*/ meltfptr[24] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8160:/ quasiblock");


      /*_.PROGN___V27*/ meltfptr[25] = /*_._IF___V25*/ meltfptr[24];;
      /*^compute */
      /*_.IFCPP___V24*/ meltfptr[23] = /*_.PROGN___V27*/ meltfptr[25];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8160:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V25*/ meltfptr[24] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V27*/ meltfptr[25] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V24*/ meltfptr[23] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8161:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L8*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.MNAME__V18*/ meltfptr[17]),
			     (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					    tabval[6])));;
      MELT_LOCATION ("warmelt-normal.melt:8161:/ cond");
      /*cond */ if ( /*_#IS_A__L8*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V29*/ meltfptr[25] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8161:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mname"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8161) ? (8161) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V29*/ meltfptr[25] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V28*/ meltfptr[24] = /*_._IFELSE___V29*/ meltfptr[25];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8161:/ clear");
	     /*clear *//*_#IS_A__L8*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V29*/ meltfptr[25] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V28*/ meltfptr[24] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8162:/ quasiblock");


    /*^multimsend */
    /*multimsend */
    {
      union meltparam_un argtab[3];
      union meltparam_un restab[1];
      memset (&argtab, 0, sizeof (argtab));
      memset (&restab, 0, sizeof (restab));
      /*^multimsend.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];	/*^multimsend.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];	/*^multimsend.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[16];
      /*^multimsend.xres */
      restab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NBINDMS__V32*/ meltfptr[31];	/*^multimsend.send */
      /*_.NEXPMAC__V31*/ meltfptr[30] =
	meltgc_send ((melt_ptr_t) ( /*_.MVALEXP__V19*/ meltfptr[18]),
		     ((melt_ptr_t)
		      (( /*!NORMAL_EXP */ meltfrout->tabval[7]))),
		     (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		     argtab, (MELTBPARSTR_PTR ""), restab);
    }
    ;
    /*^quasiblock */



    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8165:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.NBINDMS__V32*/ meltfptr[31];
      /*_.LIST_APPEND2LIST__V33*/ meltfptr[32] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_APPEND2LIST */ meltfrout->tabval[8])),
		    (melt_ptr_t) ( /*_.BINDSLIST__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8166:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L9*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8166:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L10*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8166:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8166;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_export_patmacro nexpmac";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NEXPMAC__V31*/ meltfptr[30];
	      /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V35*/ meltfptr[34] =
	      /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8166:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V36*/ meltfptr[35] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V35*/ meltfptr[34] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8166:/ quasiblock");


      /*_.PROGN___V37*/ meltfptr[35] = /*_._IF___V35*/ meltfptr[34];;
      /*^compute */
      /*_.IFCPP___V34*/ meltfptr[33] = /*_.PROGN___V37*/ meltfptr[35];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8166:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V35*/ meltfptr[34] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V37*/ meltfptr[35] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V34*/ meltfptr[33] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8167:/ quasiblock");


    /*^multimsend */
    /*multimsend */
    {
      union meltparam_un argtab[3];
      union meltparam_un restab[1];
      memset (&argtab, 0, sizeof (argtab));
      memset (&restab, 0, sizeof (restab));
      /*^multimsend.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];	/*^multimsend.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];	/*^multimsend.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[16];
      /*^multimsend.xres */
      restab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NBINDPS__V40*/ meltfptr[39];	/*^multimsend.send */
      /*_.NEXPPAT__V39*/ meltfptr[35] =
	meltgc_send ((melt_ptr_t) ( /*_.MPATEXP__V20*/ meltfptr[19]),
		     ((melt_ptr_t)
		      (( /*!NORMAL_EXP */ meltfrout->tabval[7]))),
		     (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		     argtab, (MELTBPARSTR_PTR ""), restab);
    }
    ;
    /*^quasiblock */



    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8170:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.NBINDPS__V40*/ meltfptr[39];
      /*_.LIST_APPEND2LIST__V41*/ meltfptr[40] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_APPEND2LIST */ meltfrout->tabval[8])),
		    (melt_ptr_t) ( /*_.BINDSLIST__V21*/ meltfptr[20]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8171:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L11*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8171:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L11*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L12*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8171:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8171;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_export_patmacro nexppat";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NEXPMAC__V31*/ meltfptr[30];
	      /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V43*/ meltfptr[42] =
	      /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8171:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V43*/ meltfptr[42] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8171:/ quasiblock");


      /*_.PROGN___V45*/ meltfptr[43] = /*_._IF___V43*/ meltfptr[42];;
      /*^compute */
      /*_.IFCPP___V42*/ meltfptr[41] = /*_.PROGN___V45*/ meltfptr[43];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8171:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V43*/ meltfptr[42] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V45*/ meltfptr[43] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V42*/ meltfptr[41] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8172:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      union meltparam_un argtab[6];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & /*_.NEXPMAC__V31*/ meltfptr[30];
      /*^apply.arg */
      argtab[1].meltbp_aptr =
	(melt_ptr_t *) & /*_.NEXPPAT__V39*/ meltfptr[35];
      /*^apply.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
      /*^apply.arg */
      argtab[3].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];
      /*^apply.arg */
      argtab[4].meltbp_aptr = (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[16];
      /*^apply.arg */
      argtab[5].meltbp_aptr =
	(melt_ptr_t *) & /*_.BINDSLIST__V21*/ meltfptr[20];
      /*_.NEXPM__V47*/ meltfptr[43] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!NORMAL_EXPORTED_PATMACRO */ meltfrout->tabval[9])),
		    (melt_ptr_t) ( /*_.MNAME__V18*/ meltfptr[17]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR
		     MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		    argtab, "", (union meltparam_un *) 0);
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8173:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L13*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8173:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L13*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L14*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8173:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8173;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_export_patmacro nexpm=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NEXPM__V47*/ meltfptr[43];
	      /*_.MELT_DEBUG_FUN__V50*/ meltfptr[49] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V49*/ meltfptr[48] =
	      /*_.MELT_DEBUG_FUN__V50*/ meltfptr[49];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8173:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L14*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V50*/ meltfptr[49] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V49*/ meltfptr[48] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8173:/ quasiblock");


      /*_.PROGN___V51*/ meltfptr[49] = /*_._IF___V49*/ meltfptr[48];;
      /*^compute */
      /*_.IFCPP___V48*/ meltfptr[47] = /*_.PROGN___V51*/ meltfptr[49];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8173:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L13*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V49*/ meltfptr[48] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V51*/ meltfptr[49] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V48*/ meltfptr[47] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8174:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L15*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8174:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L15*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L16*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8174:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8174;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_export_patmacro final nrepnil=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NREPNIL__V22*/ meltfptr[21];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " bindslist=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.BINDSLIST__V21*/ meltfptr[20];
	      /*_.MELT_DEBUG_FUN__V54*/ meltfptr[53] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V53*/ meltfptr[49] =
	      /*_.MELT_DEBUG_FUN__V54*/ meltfptr[53];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8174:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L16*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V54*/ meltfptr[53] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V53*/ meltfptr[49] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8174:/ quasiblock");


      /*_.PROGN___V55*/ meltfptr[53] = /*_._IF___V53*/ meltfptr[49];;
      /*^compute */
      /*_.IFCPP___V52*/ meltfptr[48] = /*_.PROGN___V55*/ meltfptr[53];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8174:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L15*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V53*/ meltfptr[49] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V55*/ meltfptr[53] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V52*/ meltfptr[48] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8175:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.NREPNIL__V22*/ meltfptr[21];;
    MELT_LOCATION ("warmelt-normal.melt:8175:/ putxtraresult");
    if (!meltxrestab_ || !meltxresdescr_)
      goto meltlabend_rout;
    if (meltxresdescr_[0] != MELTBPAR_PTR)
      goto meltlabend_rout;
    if (meltxrestab_[0].meltbp_aptr)
      *(meltxrestab_[0].meltbp_aptr) =
	(melt_ptr_t) ( /*_.BINDSLIST__V21*/ meltfptr[20]);
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V46*/ meltfptr[42] = /*_.RETURN___V56*/ meltfptr[49];;

    MELT_LOCATION ("warmelt-normal.melt:8172:/ clear");
	   /*clear *//*_.NEXPM__V47*/ meltfptr[43] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V48*/ meltfptr[47] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V52*/ meltfptr[48] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V56*/ meltfptr[49] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*_.MULTI___V38*/ meltfptr[34] = /*_.LET___V46*/ meltfptr[42];;

    MELT_LOCATION ("warmelt-normal.melt:8167:/ clear");
	   /*clear *//*_.LIST_APPEND2LIST__V41*/ meltfptr[40] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V42*/ meltfptr[41] = 0;
    /*^clear */
	   /*clear *//*_.LET___V46*/ meltfptr[42] = 0;

    /*^clear */
	   /*clear *//*_.NBINDPS__V40*/ meltfptr[39] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*_.MULTI___V30*/ meltfptr[25] = /*_.MULTI___V38*/ meltfptr[34];;

    MELT_LOCATION ("warmelt-normal.melt:8162:/ clear");
	   /*clear *//*_.LIST_APPEND2LIST__V33*/ meltfptr[32] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V34*/ meltfptr[33] = 0;
    /*^clear */
	   /*clear *//*_.MULTI___V38*/ meltfptr[34] = 0;

    /*^clear */
	   /*clear *//*_.NBINDMS__V32*/ meltfptr[31] = 0;
    /*_.LET___V16*/ meltfptr[14] = /*_.MULTI___V30*/ meltfptr[25];;

    MELT_LOCATION ("warmelt-normal.melt:8152:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.MNAME__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.MVALEXP__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.MPATEXP__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.BINDSLIST__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.NREPNIL__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V28*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.MULTI___V30*/ meltfptr[25] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8147:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8147:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_EXPORT_PATMACRO", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_156_WARMELTmiNORMAL_NORMEXP_EXPORT_PATMACRO */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 34
    melt_ptr_t mcfr_varptr[34];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
	 *) meltfirstargp_;
      /* use arguments meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 34; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE nbval 34*/
  meltfram__.mcfr_nbvar = 34 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		    meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8183:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8184:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8184:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8184:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8184;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_current_module_environment_reference recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V7*/ meltfptr[6] =
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8184:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V7*/ meltfptr[6] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8184:/ quasiblock");


      /*_.PROGN___V9*/ meltfptr[7] = /*_._IF___V7*/ meltfptr[6];;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_.PROGN___V9*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8184:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8185:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8185:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8185:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check current_module_environment_reference recv"), ("warmelt-normal.melt") ? ("warmelt-normal.melt") : __FILE__, (8185) ? (8185) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8185:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8186:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8186:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8186:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8186) ? (8186) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8186:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8187:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8187:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8187:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8187) ? (8187) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8187:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8188:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8189:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8190:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "CMEC_COMMENT");
  /*_.SCOMM__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8191:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 14, "NCTX_PROCURMODENVLIST");
  /*_.NCURMODENVLIST__V19*/ meltfptr[18] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8192:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 10, "NCTX_CURPROC");
  /*_.CURPROC__V20*/ meltfptr[19] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8193:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 12, "NCTX_QDATCURMODENVBOX");
  /*_.QDATCUR__V21*/ meltfptr[20] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8194:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_QUASICONST_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[4])), (5), "CLASS_NREP_QUASICONST_CURRENT_MODULE_ENVIRONMENT_REFERENCE");
  /*_.INST__V23*/ meltfptr[22] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V23*/ meltfptr[22])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[16]), "NREP_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NCONST_SVAL",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V23*/ meltfptr[22])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (1),
			  ( /*_.RECV__V2*/ meltfptr[1]), "NCONST_SVAL");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NCONST_PROC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V23*/ meltfptr[22])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (2),
			  ( /*_.CURPROC__V20*/ meltfptr[19]), "NCONST_PROC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NCONST_DATA",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V23*/ meltfptr[22])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (3),
			  ( /*_.QDATCUR__V21*/ meltfptr[20]), "NCONST_DATA");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NQCMEC_COMMENT",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V23*/ meltfptr[22])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (4),
			  ( /*_.SCOMM__V18*/ meltfptr[17]), "NQCMEC_COMMENT");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V23*/ meltfptr[22],
				  "newly made instance");
    ;
    /*_.NQUASI__V22*/ meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8202:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L6*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.QDATCUR__V21*/ meltfptr[20]),
			     (melt_ptr_t) (( /*!CLASS_NREP_QUASIDATA_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[5])));;
      MELT_LOCATION ("warmelt-normal.melt:8202:/ cond");
      /*cond */ if ( /*_#IS_A__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V25*/ meltfptr[24] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8202:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check qdatcur"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8202) ? (8202) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V25*/ meltfptr[24] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V24*/ meltfptr[23] = /*_._IFELSE___V25*/ meltfptr[24];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8202:/ clear");
	     /*clear *//*_#IS_A__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V25*/ meltfptr[24] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V24*/ meltfptr[23] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L7*/ meltfnum[1] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.CURPROC__V20*/ meltfptr[19]),
			   (melt_ptr_t) (( /*!CLASS_NREP_HOOKPROC */
					  meltfrout->tabval[6])));;
    MELT_LOCATION ("warmelt-normal.melt:8203:/ cond");
    /*cond */ if ( /*_#IS_A__L7*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-normal.melt:8204:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.SLOC__V17*/ meltfptr[16]),
			      ("(CURRENT_MODULE_ENVIRONMENT_REFERENCE) cannot be used within hooks"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8205:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-normal.melt:8205:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-normal.melt:8203:/ quasiblock");


	  /*_.PROGN___V28*/ meltfptr[27] = /*_.RETURN___V27*/ meltfptr[26];;
	  /*^compute */
	  /*_._IF___V26*/ meltfptr[24] = /*_.PROGN___V28*/ meltfptr[27];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8203:/ clear");
	     /*clear *//*_.RETURN___V27*/ meltfptr[26] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V28*/ meltfptr[27] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V26*/ meltfptr[24] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L8*/ meltfnum[0] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.CURPROC__V20*/ meltfptr[19]),
			   (melt_ptr_t) (( /*!CLASS_NREP_ROUTPROC */
					  meltfrout->tabval[7])));;
    MELT_LOCATION ("warmelt-normal.melt:8206:/ cond");
    /*cond */ if ( /*_#IS_A__L8*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-normal.melt:8207:/ getslot");
	  {
	    melt_ptr_t slot = NULL, obj = NULL;
	    obj = (melt_ptr_t) ( /*_.CURPROC__V20*/ meltfptr[19]) /*=obj*/ ;
	    melt_object_get_field (slot, obj, 5, "NRPRO_CONST");
    /*_.NRPRO_CONST__V29*/ meltfptr[26] = slot;
	  };
	  ;

	  {
	    /*^locexp */
	    meltgc_append_list ((melt_ptr_t)
				( /*_.NRPRO_CONST__V29*/ meltfptr[26]),
				(melt_ptr_t) ( /*_.QDATCUR__V21*/
					      meltfptr[20]));
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-normal.melt:8208:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.NCURMODENVLIST__V19*/ meltfptr[18]),
				(melt_ptr_t) ( /*_.CURPROC__V20*/
					      meltfptr[19]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8206:/ quasiblock");


	  /*epilog */

	  /*^clear */
	     /*clear *//*_.NRPRO_CONST__V29*/ meltfptr[26] = 0;
	}
	;
      }				/*noelse */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8210:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L9*/ meltfnum[8] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8210:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[8])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8210:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8210;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_current_module_environment_reference nquasi";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NQUASI__V22*/ meltfptr[21];
	      /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V31*/ meltfptr[26] =
	      /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8210:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V32*/ meltfptr[31] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V31*/ meltfptr[26] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8210:/ quasiblock");


      /*_.PROGN___V33*/ meltfptr[31] = /*_._IF___V31*/ meltfptr[26];;
      /*^compute */
      /*_.IFCPP___V30*/ meltfptr[27] = /*_.PROGN___V33*/ meltfptr[31];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8210:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[8] = 0;
      /*^clear */
	     /*clear *//*_._IF___V31*/ meltfptr[26] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V33*/ meltfptr[31] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V30*/ meltfptr[27] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8211:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.NQUASI__V22*/ meltfptr[21];;
    MELT_LOCATION ("warmelt-normal.melt:8211:/ putxtraresult");
    if (!meltxrestab_ || !meltxresdescr_)
      goto meltlabend_rout;
    if (meltxresdescr_[0] != MELTBPAR_PTR)
      goto meltlabend_rout;
    if (meltxrestab_[0].meltbp_aptr)
      *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (( /*nil */ NULL));
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V34*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-normal.melt:8188:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.SCOMM__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.NCURMODENVLIST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.CURPROC__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.QDATCUR__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.NQUASI__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V24*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L7*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V26*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L8*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V30*/ meltfptr[27] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V34*/ meltfptr[26] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8183:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8183:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		  meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_157_WARMELTmiNORMAL_NORMEXP_CURRENT_MODULE_ENVIRONMENT_REFERENCE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 32
    melt_ptr_t mcfr_varptr[32];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT_st
	 *) meltfirstargp_;
      /* use arguments meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 32; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT nbval 32*/
  meltfram__.mcfr_nbvar = 32 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_PARENT_MODULE_ENVIRONMENT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8217:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8218:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_PARENT_MODULE_ENVIRONMENT */ meltfrout->tabval[0])));;
      MELT_LOCATION ("warmelt-normal.melt:8218:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8218:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check parent_module_environment recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8218) ? (8218) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8218:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8219:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8219:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8219:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8219) ? (8219) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8219:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8220:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8220:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8220:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8220) ? (8220) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8220:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8221:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8221:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8221:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8221;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_parent_module_environment  recv";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V13*/ meltfptr[12] =
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8221:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L5*/ meltfnum[4] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V13*/ meltfptr[12] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8221:/ quasiblock");


      /*_.PROGN___V15*/ meltfptr[13] = /*_._IF___V13*/ meltfptr[12];;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_.PROGN___V15*/ meltfptr[13];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8221:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V15*/ meltfptr[13] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8222:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8223:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[13] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8224:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 10, "NCTX_CURPROC");
  /*_.CURPROC__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8225:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 13, "NCTX_QDATPARMODENV");
  /*_.QDATPAR__V19*/ meltfptr[18] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8226:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_QUASICONST_PARENT_MODULE_ENVIRONMENT */ meltfrout->tabval[4])), (4), "CLASS_NREP_QUASICONST_PARENT_MODULE_ENVIRONMENT");
  /*_.INST__V21*/ meltfptr[20] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V21*/ meltfptr[20])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V21*/ meltfptr[20]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[13]), "NREP_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NCONST_SVAL",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V21*/ meltfptr[20])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V21*/ meltfptr[20]), (1),
			  ( /*_.RECV__V2*/ meltfptr[1]), "NCONST_SVAL");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NCONST_PROC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V21*/ meltfptr[20])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V21*/ meltfptr[20]), (2),
			  ( /*_.CURPROC__V18*/ meltfptr[17]), "NCONST_PROC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NCONST_DATA",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V21*/ meltfptr[20])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V21*/ meltfptr[20]), (3),
			  ( /*_.QDATPAR__V19*/ meltfptr[18]), "NCONST_DATA");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V21*/ meltfptr[20],
				  "newly made instance");
    ;
    /*_.NQUASI__V20*/ meltfptr[19] = /*_.INST__V21*/ meltfptr[20];;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L6*/ meltfnum[4] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.CURPROC__V18*/ meltfptr[17]),
			   (melt_ptr_t) (( /*!CLASS_NREP_HOOKPROC */
					  meltfrout->tabval[5])));;
    MELT_LOCATION ("warmelt-normal.melt:8233:/ cond");
    /*cond */ if ( /*_#IS_A__L6*/ meltfnum[4])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-normal.melt:8234:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.SLOC__V17*/ meltfptr[13]),
			      ("(PARENT_MODULE_ENVIRONMENT) cannot be used within hooks"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8235:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-normal.melt:8235:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-normal.melt:8233:/ quasiblock");


	  /*_.PROGN___V24*/ meltfptr[23] = /*_.RETURN___V23*/ meltfptr[22];;
	  /*^compute */
	  /*_._IF___V22*/ meltfptr[21] = /*_.PROGN___V24*/ meltfptr[23];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8233:/ clear");
	     /*clear *//*_.RETURN___V23*/ meltfptr[22] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V24*/ meltfptr[23] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V22*/ meltfptr[21] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8236:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L7*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.QDATPAR__V19*/ meltfptr[18]),
			     (melt_ptr_t) (( /*!CLASS_NREP_QUASIDATA_PARENT_MODULE_ENVIRONMENT */ meltfrout->tabval[6])));;
      MELT_LOCATION ("warmelt-normal.melt:8236:/ cond");
      /*cond */ if ( /*_#IS_A__L7*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V26*/ meltfptr[23] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8236:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check qdatpar"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8236) ? (8236) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V26*/ meltfptr[23] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V25*/ meltfptr[22] = /*_._IFELSE___V26*/ meltfptr[23];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8236:/ clear");
	     /*clear *//*_#IS_A__L7*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V26*/ meltfptr[23] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V25*/ meltfptr[22] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L8*/ meltfnum[0] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.CURPROC__V18*/ meltfptr[17]),
			   (melt_ptr_t) (( /*!CLASS_NREP_ROUTPROC */
					  meltfrout->tabval[7])));;
    MELT_LOCATION ("warmelt-normal.melt:8237:/ cond");
    /*cond */ if ( /*_#IS_A__L8*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-normal.melt:8238:/ getslot");
	  {
	    melt_ptr_t slot = NULL, obj = NULL;
	    obj = (melt_ptr_t) ( /*_.CURPROC__V18*/ meltfptr[17]) /*=obj*/ ;
	    melt_object_get_field (slot, obj, 5, "NRPRO_CONST");
    /*_.NRPRO_CONST__V27*/ meltfptr[23] = slot;
	  };
	  ;

	  {
	    /*^locexp */
	    meltgc_append_list ((melt_ptr_t)
				( /*_.NRPRO_CONST__V27*/ meltfptr[23]),
				(melt_ptr_t) ( /*_.QDATPAR__V19*/
					      meltfptr[18]));
	  }
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8237:/ clear");
	     /*clear *//*_.NRPRO_CONST__V27*/ meltfptr[23] = 0;
	}
	;
      }				/*noelse */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8239:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L9*/ meltfnum[8] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8239:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L9*/ meltfnum[8])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8239:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8239;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_parent_module_environment nquasi";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NQUASI__V20*/ meltfptr[19];
	      /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V29*/ meltfptr[28] =
	      /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8239:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V29*/ meltfptr[28] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8239:/ quasiblock");


      /*_.PROGN___V31*/ meltfptr[29] = /*_._IF___V29*/ meltfptr[28];;
      /*^compute */
      /*_.IFCPP___V28*/ meltfptr[23] = /*_.PROGN___V31*/ meltfptr[29];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8239:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L9*/ meltfnum[8] = 0;
      /*^clear */
	     /*clear *//*_._IF___V29*/ meltfptr[28] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V31*/ meltfptr[29] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V28*/ meltfptr[23] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8240:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.NQUASI__V20*/ meltfptr[19];;
    MELT_LOCATION ("warmelt-normal.melt:8240:/ putxtraresult");
    if (!meltxrestab_ || !meltxresdescr_)
      goto meltlabend_rout;
    if (meltxresdescr_[0] != MELTBPAR_PTR)
      goto meltlabend_rout;
    if (meltxrestab_[0].meltbp_aptr)
      *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (( /*nil */ NULL));
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[12] = /*_.RETURN___V32*/ meltfptr[28];;

    MELT_LOCATION ("warmelt-normal.melt:8222:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[13] = 0;
    /*^clear */
	   /*clear *//*_.CURPROC__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.QDATPAR__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.NQUASI__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L6*/ meltfnum[4] = 0;
    /*^clear */
	   /*clear *//*_._IF___V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V25*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L8*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V28*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V32*/ meltfptr[28] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8217:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8217:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_PARENT_MODULE_ENVIRONMENT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_158_WARMELTmiNORMAL_NORMEXP_PARENT_MODULE_ENVIRONMENT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 85
    melt_ptr_t mcfr_varptr[85];
#define MELTFRAM_NBVARNUM 15
    long mcfr_varnum[15];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_st
	 *) meltfirstargp_;
      /* use arguments meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 85; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE nbval 85*/
  meltfram__.mcfr_nbvar = 85 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		    meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8246:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8247:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[0])));;
      MELT_LOCATION ("warmelt-normal.melt:8247:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8247:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check update_current_module_environment_reference recv"), ("warmelt-normal.melt") ? ("warmelt-normal.melt") : __FILE__, (8247) ? (8247) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8247:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8248:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8248:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8248:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8248) ? (8248) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8248:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8249:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8249:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8249:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8249) ? (8249) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8249:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8250:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8250:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8250:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8250;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_update_current_module_environment_reference recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n env=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = "\n ncx=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V13*/ meltfptr[12] =
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8250:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L5*/ meltfnum[4] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V13*/ meltfptr[12] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8250:/ quasiblock");


      /*_.PROGN___V15*/ meltfptr[13] = /*_._IF___V13*/ meltfptr[12];;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_.PROGN___V15*/ meltfptr[13];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8250:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V15*/ meltfptr[13] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-normal.melt:8253:/ locexp");

#if MELT_HAVE_DEBUG
      if (melt_need_debug (0))
	melt_dbgshortbacktrace (("normexp_update_current_module_environment_reference"), (15));
#endif
      ;
    }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8254:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8255:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[13] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8256:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SUCME_COMMENT");
  /*_.SCOMM__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8257:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 10, "NCTX_CURPROC");
  /*_.CURPROC__V19*/ meltfptr[18] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8258:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 0, "NCTX_INITPROC");
  /*_.INIPROC__V20*/ meltfptr[19] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8259:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 11, "NCTX_MODULCONTEXT");
  /*_.MODCTX__V21*/ meltfptr[20] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8260:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.MODCTX__V21*/ meltfptr[20]),
					(melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */ meltfrout->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.MODCTX__V21*/ meltfptr[20]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 0, "MOCX_MODULENAME");
   /*_.MODNAM__V22*/ meltfptr[21] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MODNAM__V22*/ meltfptr[21] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#exeq__L6*/ meltfnum[4] =
      (( /*_.CURPROC__V19*/ meltfptr[18]) !=
       ( /*_.INIPROC__V20*/ meltfptr[19]));;
    MELT_LOCATION ("warmelt-normal.melt:8262:/ cond");
    /*cond */ if ( /*_#exeq__L6*/ meltfnum[4])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-normal.melt:8264:/ locexp");
	    /* error_plain */
	      melt_error_str ((melt_ptr_t) ( /*_.SLOC__V17*/ meltfptr[13]),
			      ("(UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE) not at toplevel"),
			      (melt_ptr_t) 0);
	  }
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8265:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-normal.melt:8265:/ locexp");
	    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
	    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	      melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
	    ;
	  }
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  MELT_LOCATION ("warmelt-normal.melt:8263:/ quasiblock");


	  /*_.PROGN___V25*/ meltfptr[24] = /*_.RETURN___V24*/ meltfptr[23];;
	  /*^compute */
	  /*_._IF___V23*/ meltfptr[22] = /*_.PROGN___V25*/ meltfptr[24];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8262:/ clear");
	     /*clear *//*_.RETURN___V24*/ meltfptr[23] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V25*/ meltfptr[24] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V23*/ meltfptr[22] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8266:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L7*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8266:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L7*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8266:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8266;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_update_current_module_environment_reference modctx=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODCTX__V21*/ meltfptr[20];
	      /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V27*/ meltfptr[24] =
	      /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8266:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V27*/ meltfptr[24] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8266:/ quasiblock");


      /*_.PROGN___V29*/ meltfptr[27] = /*_._IF___V27*/ meltfptr[24];;
      /*^compute */
      /*_.IFCPP___V26*/ meltfptr[23] = /*_.PROGN___V29*/ meltfptr[27];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8266:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V27*/ meltfptr[24] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V29*/ meltfptr[27] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V26*/ meltfptr[23] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_A__L9*/ meltfnum[7] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.MODCTX__V21*/ meltfptr[20]),
			   (melt_ptr_t) (( /*!CLASS_RUNNING_EXTENSION_MODULE_CONTEXT */ meltfrout->tabval[5])));;
    MELT_LOCATION ("warmelt-normal.melt:8267:/ cond");
    /*cond */ if ( /*_#IS_A__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-normal.melt:8268:/ quasiblock");



	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^quasiblock */


	  /*^rawallocobj */
	  /*rawallocobj */
	  {
	    melt_ptr_t newobj = 0;
	    melt_raw_object_create (newobj,
				    (melt_ptr_t) (( /*!CLASS_NREP_CHECK_RUNNING_MODULE_ENVIRONMENT_CONTAINER */ meltfrout->tabval[6])), (2), "CLASS_NREP_CHECK_RUNNING_MODULE_ENVIRONMENT_CONTAINER");
    /*_.INST__V33*/ meltfptr[32] =
	      newobj;
	  };
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @NREP_LOC",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V33*/ meltfptr[32]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V33*/ meltfptr[32]), (0),
				( /*_.SLOC__V17*/ meltfptr[13]), "NREP_LOC");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @NCHRUMOD_COMMENT",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V33*/ meltfptr[32]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V33*/ meltfptr[32]), (1),
				( /*_.SCOMM__V18*/ meltfptr[17]),
				"NCHRUMOD_COMMENT");
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.INST__V33*/ meltfptr[32],
					"newly made instance");
	  ;
	  /*_.NCHK__V32*/ meltfptr[31] = /*_.INST__V33*/ meltfptr[32];;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8272:/ apply");
	  /*apply */
	  {
	    /*_.CSYM__V34*/ meltfptr[33] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!CLONE_SYMBOL */ meltfrout->tabval[7])),
			  (melt_ptr_t) (( /*!konst_8_CHECKRUNMODENVBOX_ */
					 meltfrout->tabval[8])), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8273:/ quasiblock");


	  /*^rawallocobj */
	  /*rawallocobj */
	  {
	    melt_ptr_t newobj = 0;
	    melt_raw_object_create (newobj,
				    (melt_ptr_t) (( /*!CLASS_NORMAL_LET_BINDING */ meltfrout->tabval[9])), (4), "CLASS_NORMAL_LET_BINDING");
    /*_.INST__V36*/ meltfptr[35] =
	      newobj;
	  };
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @LETBIND_LOC",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V36*/ meltfptr[35]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V36*/ meltfptr[35]), (3),
				( /*_.SLOC__V17*/ meltfptr[13]),
				"LETBIND_LOC");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @BINDER",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V36*/ meltfptr[35]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V36*/ meltfptr[35]), (0),
				( /*_.CSYM__V34*/ meltfptr[33]), "BINDER");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @LETBIND_TYPE",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V36*/ meltfptr[35]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V36*/ meltfptr[35]), (1),
				(( /*!CTYPE_VOID */ meltfrout->tabval[10])),
				"LETBIND_TYPE");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @LETBIND_EXPR",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V36*/ meltfptr[35]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V36*/ meltfptr[35]), (2),
				( /*_.NCHK__V32*/ meltfptr[31]),
				"LETBIND_EXPR");
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.INST__V36*/ meltfptr[35],
					"newly made instance");
	  ;
	  /*_.CBIND__V35*/ meltfptr[34] = /*_.INST__V36*/ meltfptr[35];;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8280:/ quasiblock");


	  /*^rawallocobj */
	  /*rawallocobj */
	  {
	    melt_ptr_t newobj = 0;
	    melt_raw_object_create (newobj,
				    (melt_ptr_t) (( /*!CLASS_NREP_LOCSYMOCC */
						   meltfrout->tabval[11])),
				    (4), "CLASS_NREP_LOCSYMOCC");
    /*_.INST__V38*/ meltfptr[37] =
	      newobj;
	  };
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @NREP_LOC",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V38*/ meltfptr[37]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (0),
				( /*_.SLOC__V17*/ meltfptr[13]), "NREP_LOC");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @NOCC_CTYP",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V38*/ meltfptr[37]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (2),
				(( /*!CTYPE_VOID */ meltfrout->tabval[10])),
				"NOCC_CTYP");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @NOCC_SYMB",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V38*/ meltfptr[37]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (1),
				( /*_.CSYM__V34*/ meltfptr[33]), "NOCC_SYMB");
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @NOCC_BIND",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V38*/ meltfptr[37]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V38*/ meltfptr[37]), (3),
				( /*_.CBIND__V35*/ meltfptr[34]),
				"NOCC_BIND");
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.INST__V38*/ meltfptr[37],
					"newly made instance");
	  ;
	  /*_.CLOCC__V37*/ meltfptr[36] = /*_.INST__V38*/ meltfptr[37];;
	  MELT_LOCATION ("warmelt-normal.melt:8286:/ blockmultialloc");
	  /*multiallocblock */
	  {
	    struct meltletrec_1_st
	    {
	      struct meltpair_st rpair_0__CBIND_x4;
	      struct meltlist_st rlist_1__LIST_;
	      long meltletrec_1_endgap;
	    } *meltletrec_1_ptr = 0;
	    meltletrec_1_ptr =
	      (struct meltletrec_1_st *)
	      meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
	    /*^blockmultialloc.initfill */
	    /*inipair rpair_0__CBIND_x4 */
     /*_.CBIND__V40*/ meltfptr[39] =
	      (melt_ptr_t) & meltletrec_1_ptr->rpair_0__CBIND_x4;
	    meltletrec_1_ptr->rpair_0__CBIND_x4.discr =
	      (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_PAIR))));

	    /*inilist rlist_1__LIST_ */
     /*_.LIST___V41*/ meltfptr[40] =
	      (melt_ptr_t) & meltletrec_1_ptr->rlist_1__LIST_;
	    meltletrec_1_ptr->rlist_1__LIST_.discr =
	      (meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_LIST))));



	    /*^putpairhead */
	    /*putpairhead */
	    melt_assertmsg ("putpairhead /f checkpair",
			    melt_magic_discr ((melt_ptr_t)
					      ( /*_.CBIND__V40*/
					       meltfptr[39])) ==
			    MELTOBMAG_PAIR);
	    ((meltpair_ptr_t) ( /*_.CBIND__V40*/ meltfptr[39]))->hd =
	      (melt_ptr_t) ( /*_.CBIND__V35*/ meltfptr[34]);
	    ;
	    /*^touch */
	    meltgc_touch ( /*_.CBIND__V40*/ meltfptr[39]);
	    ;
	    /*^putlist */
	    /*putlist */
	    melt_assertmsg ("putlist checklist",
			    melt_magic_discr ((melt_ptr_t)
					      ( /*_.LIST___V41*/
					       meltfptr[40])) ==
			    MELTOBMAG_LIST);
	    ((meltlist_ptr_t) ( /*_.LIST___V41*/ meltfptr[40]))->first =
	      (meltpair_ptr_t) ( /*_.CBIND__V40*/ meltfptr[39]);
	    ((meltlist_ptr_t) ( /*_.LIST___V41*/ meltfptr[40]))->last =
	      (meltpair_ptr_t) ( /*_.CBIND__V40*/ meltfptr[39]);
	    ;
	    /*^touch */
	    meltgc_touch ( /*_.LIST___V41*/ meltfptr[40]);
	    ;
	    /*_.BINDLIST__V39*/ meltfptr[38] = /*_.LIST___V41*/ meltfptr[40];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8286:/ clear");
	      /*clear *//*_.CBIND__V40*/ meltfptr[39] = 0;
	    /*^clear */
	      /*clear *//*_.LIST___V41*/ meltfptr[40] = 0;
	    /*^clear */
	      /*clear *//*_.CBIND__V40*/ meltfptr[39] = 0;
	    /*^clear */
	      /*clear *//*_.LIST___V41*/ meltfptr[40] = 0;
	  }			/*end multiallocblock */
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-normal.melt:8288:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    {
	      /*^locexp */
	      /*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	      melt_dbgcounter++;
#endif
	      ;
	    }
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
     /*_#MELT_NEED_DBG__L10*/ meltfnum[0] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-normal.melt:8288:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[0])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

       /*_#MELT_CALLCOUNT__L11*/ meltfnum[10] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-normal.melt:8288:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[9];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-normal.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 8288;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "normexp_update_current_module_environment_reference gives nchk=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.NCHK__V32*/ meltfptr[31];
		    /*^apply.arg */
		    argtab[5].meltbp_cstring = " clocc=";
		    /*^apply.arg */
		    argtab[6].meltbp_aptr =
		      (melt_ptr_t *) & /*_.CLOCC__V37*/ meltfptr[36];
		    /*^apply.arg */
		    argtab[7].meltbp_cstring = " bindlist=";
		    /*^apply.arg */
		    argtab[8].meltbp_aptr =
		      (melt_ptr_t *) & /*_.BINDLIST__V39*/ meltfptr[38];
		    /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*!MELT_DEBUG_FUN */ meltfrout->
				    tabval[3])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_._IF___V43*/ meltfptr[40] =
		    /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8288:/ clear");
		 /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[10] = 0;
		  /*^clear */
		 /*clear *//*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

      /*_._IF___V43*/ meltfptr[40] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8288:/ quasiblock");


	    /*_.PROGN___V45*/ meltfptr[43] = /*_._IF___V43*/ meltfptr[40];;
	    /*^compute */
	    /*_.IFCPP___V42*/ meltfptr[39] = /*_.PROGN___V45*/ meltfptr[43];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8288:/ clear");
	       /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_._IF___V43*/ meltfptr[40] = 0;
	    /*^clear */
	       /*clear *//*_.PROGN___V45*/ meltfptr[43] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V42*/ meltfptr[39] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8290:/ quasiblock");


	  /*_._RETVAL___V1*/ meltfptr[0] = /*_.CLOCC__V37*/ meltfptr[36];;
	  MELT_LOCATION ("warmelt-normal.melt:8290:/ putxtraresult");
	  if (!meltxrestab_ || !meltxresdescr_)
	    goto meltlabend_rout;
	  if (meltxresdescr_[0] != MELTBPAR_PTR)
	    goto meltlabend_rout;
	  if (meltxrestab_[0].meltbp_aptr)
	    *(meltxrestab_[0].meltbp_aptr) =
	      (melt_ptr_t) ( /*_.BINDLIST__V39*/ meltfptr[38]);
	  ;
	  /*^finalreturn */
	  ;
	  /*finalret */ goto meltlabend_rout;
	  /*_.LET___V31*/ meltfptr[27] = /*_.RETURN___V46*/ meltfptr[40];;

	  MELT_LOCATION ("warmelt-normal.melt:8268:/ clear");
	     /*clear *//*_.NCHK__V32*/ meltfptr[31] = 0;
	  /*^clear */
	     /*clear *//*_.CSYM__V34*/ meltfptr[33] = 0;
	  /*^clear */
	     /*clear *//*_.CBIND__V35*/ meltfptr[34] = 0;
	  /*^clear */
	     /*clear *//*_.CLOCC__V37*/ meltfptr[36] = 0;
	  /*^clear */
	     /*clear *//*_.BINDLIST__V39*/ meltfptr[38] = 0;
	  /*^clear */
	     /*clear *//*_.IFCPP___V42*/ meltfptr[39] = 0;
	  /*^clear */
	     /*clear *//*_.RETURN___V46*/ meltfptr[40] = 0;
	  /*_._IFELSE___V30*/ meltfptr[24] = /*_.LET___V31*/ meltfptr[27];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8267:/ clear");
	     /*clear *//*_.LET___V31*/ meltfptr[27] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_A__L12*/ meltfnum[10] =
	    melt_is_instance_of ((melt_ptr_t)
				 ( /*_.MODCTX__V21*/ meltfptr[20]),
				 (melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */ meltfrout->tabval[4])));;
	  MELT_LOCATION ("warmelt-normal.melt:8292:/ cond");
	  /*cond */ if ( /*_#IS_A__L12*/ meltfnum[10])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-normal.melt:8293:/ cppif.then");
		/*^block */
		/*anyblock */
		{


		  MELT_CHECK_SIGNAL ();
		  ;
       /*_#IS_NOT_A__L13*/ meltfnum[0] =
		    !melt_is_instance_of ((melt_ptr_t)
					  ( /*_.MODCTX__V21*/ meltfptr[20]),
					  (melt_ptr_t) (( /*!CLASS_RUNNING_EXTENSION_MODULE_CONTEXT */ meltfrout->tabval[5])));;
		  MELT_LOCATION ("warmelt-normal.melt:8293:/ cond");
		  /*cond */ if ( /*_#IS_NOT_A__L13*/ meltfnum[0])	/*then */
		    {
		      /*^cond.then */
		      /*_._IFELSE___V49*/ meltfptr[33] = ( /*nil */ NULL);;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-normal.melt:8293:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {




			{
			  /*^locexp */
			  melt_assert_failed (("check modctx not running"),
					      ("warmelt-normal.melt")
					      ? ("warmelt-normal.melt") :
					      __FILE__,
					      (8293) ? (8293) : __LINE__,
					      __FUNCTION__);
			  ;
			}
			;
		   /*clear *//*_._IFELSE___V49*/ meltfptr[33] = 0;
			/*epilog */
		      }
		      ;
		    }
		  ;
		  /*^compute */
		  /*_.IFCPP___V48*/ meltfptr[31] =
		    /*_._IFELSE___V49*/ meltfptr[33];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8293:/ clear");
		 /*clear *//*_#IS_NOT_A__L13*/ meltfnum[0] = 0;
		  /*^clear */
		 /*clear *//*_._IFELSE___V49*/ meltfptr[33] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V48*/ meltfptr[31] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-normal.melt:8294:/ quasiblock");



		MELT_CHECK_SIGNAL ();
		;
		/*^quasiblock */


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_NREP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[12])), (3), "CLASS_NREP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE");
      /*_.INST__V52*/ meltfptr[38] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NREP_LOC",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V52*/
						   meltfptr[38])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V52*/ meltfptr[38]), (0),
				      ( /*_.SLOC__V17*/ meltfptr[13]),
				      "NREP_LOC");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NCUMEB_COMMENT",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V52*/
						   meltfptr[38])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V52*/ meltfptr[38]), (2),
				      ( /*_.SCOMM__V18*/ meltfptr[17]),
				      "NCUMEB_COMMENT");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V52*/ meltfptr[38],
					      "newly made instance");
		;
		/*_.NUP__V51*/ meltfptr[36] = /*_.INST__V52*/ meltfptr[38];;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8299:/ apply");
		/*apply */
		{
		  /*_.CSYM__V53*/ meltfptr[39] =
		    melt_apply ((meltclosure_ptr_t)
				(( /*!CLONE_SYMBOL */ meltfrout->tabval[7])),
				(melt_ptr_t) (( /*!konst_13_UPDATCURMODENVBOX_ */ meltfrout->tabval[13])), (""), (union meltparam_un *) 0, "", (union meltparam_un *) 0);
		}
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8300:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_NORMAL_LET_BINDING */ meltfrout->tabval[9])), (4), "CLASS_NORMAL_LET_BINDING");
      /*_.INST__V55*/ meltfptr[27] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LETBIND_LOC",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V55*/
						   meltfptr[27])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V55*/ meltfptr[27]), (3),
				      ( /*_.SLOC__V17*/ meltfptr[13]),
				      "LETBIND_LOC");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @BINDER",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V55*/
						   meltfptr[27])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V55*/ meltfptr[27]), (0),
				      ( /*_.CSYM__V53*/ meltfptr[39]),
				      "BINDER");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LETBIND_TYPE",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V55*/
						   meltfptr[27])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V55*/ meltfptr[27]), (1),
				      (( /*!CTYPE_VOID */ meltfrout->
					tabval[10])), "LETBIND_TYPE");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LETBIND_EXPR",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V55*/
						   meltfptr[27])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V55*/ meltfptr[27]), (2),
				      ( /*_.NUP__V51*/ meltfptr[36]),
				      "LETBIND_EXPR");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V55*/ meltfptr[27],
					      "newly made instance");
		;
		/*_.CBIND__V54*/ meltfptr[40] = /*_.INST__V55*/ meltfptr[27];;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8307:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_NREP_LOCSYMOCC */ meltfrout->tabval[11])), (4), "CLASS_NREP_LOCSYMOCC");
      /*_.INST__V57*/ meltfptr[56] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NREP_LOC",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V57*/
						   meltfptr[56])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (0),
				      ( /*_.SLOC__V17*/ meltfptr[13]),
				      "NREP_LOC");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NOCC_CTYP",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V57*/
						   meltfptr[56])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (2),
				      (( /*!CTYPE_VOID */ meltfrout->
					tabval[10])), "NOCC_CTYP");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NOCC_SYMB",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V57*/
						   meltfptr[56])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (1),
				      ( /*_.CSYM__V53*/ meltfptr[39]),
				      "NOCC_SYMB");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NOCC_BIND",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V57*/
						   meltfptr[56])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V57*/ meltfptr[56]), (3),
				      ( /*_.CBIND__V54*/ meltfptr[40]),
				      "NOCC_BIND");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V57*/ meltfptr[56],
					      "newly made instance");
		;
		/*_.CLOCC__V56*/ meltfptr[33] = /*_.INST__V57*/ meltfptr[56];;
		MELT_LOCATION ("warmelt-normal.melt:8313:/ quasiblock");


     /*_.SB__V59*/ meltfptr[58] =
		  (melt_ptr_t)
		  meltgc_new_strbuf ((meltobject_ptr_t)
				     (( /*!DISCR_STRBUF */ meltfrout->
				       tabval[14])), (const char *) 0);;

		{
		  MELT_LOCATION ("warmelt-normal.melt:8314:/ locexp");
		  /*add2sbuf_strconst */
		    meltgc_add_strbuf ((melt_ptr_t)
				       ( /*_.SB__V59*/ meltfptr[58]),
				       ("cur.mod.env.cont : "));
		}
		;

		{
		  MELT_LOCATION ("warmelt-normal.melt:8315:/ locexp");
		  /*add2sbuf_string */
		    meltgc_add_strbuf ((melt_ptr_t)
				       ( /*_.SB__V59*/ meltfptr[58]),
				       melt_string_str ((melt_ptr_t)
							( /*_.SCOMM__V18*/
							 meltfptr[17])));
		}
		;
		/*_.LET___V58*/ meltfptr[57] = /*_.SB__V59*/ meltfptr[58];;

		MELT_LOCATION ("warmelt-normal.melt:8313:/ clear");
	       /*clear *//*_.SB__V59*/ meltfptr[58] = 0;
		/*_.CSBUF__V60*/ meltfptr[58] = /*_.LET___V58*/ meltfptr[57];;

		MELT_CHECK_SIGNAL ();
		;
		/*^compute */
     /*_.STRBUF2STRING__V61*/ meltfptr[60] =
		  (meltgc_new_stringdup
		   ((meltobject_ptr_t)
		    (( /*!DISCR_STRING */ meltfrout->tabval[16])),
		    melt_strbuf_str ((melt_ptr_t)
				     ( /*_.CSBUF__V60*/ meltfptr[58]))));;
		MELT_LOCATION ("warmelt-normal.melt:8317:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */ meltfrout->tabval[15])), (3), "CLASS_SOURCE_CURRENT_MODULE_ENVIRONMENT_REFERENCE");
      /*_.INST__V63*/ meltfptr[62] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LOCA_LOCATION",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V63*/
						   meltfptr[62])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V63*/ meltfptr[62]), (1),
				      ( /*_.SLOC__V17*/ meltfptr[13]),
				      "LOCA_LOCATION");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @CMEC_COMMENT",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V63*/
						   meltfptr[62])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V63*/ meltfptr[62]), (2),
				      ( /*_.STRBUF2STRING__V61*/
				       meltfptr[60]), "CMEC_COMMENT");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V63*/ meltfptr[62],
					      "newly made instance");
		;
		/*_.SCURENVBOX__V62*/ meltfptr[61] =
		  /*_.INST__V63*/ meltfptr[62];;

		MELT_CHECK_SIGNAL ();
		;

		MELT_CHECK_SIGNAL ();
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8332:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_SOURCE_PARENT_MODULE_ENVIRONMENT */ meltfrout->tabval[20])), (2), "CLASS_SOURCE_PARENT_MODULE_ENVIRONMENT");
      /*_.INST__V65*/ meltfptr[64] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LOCA_LOCATION",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V65*/
						   meltfptr[64])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V65*/ meltfptr[64]), (1),
				      ( /*_.PSLOC__V5*/ meltfptr[4]),
				      "LOCA_LOCATION");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V65*/ meltfptr[64],
					      "newly made instance");
		;
		/*_.INST___V64*/ meltfptr[63] = /*_.INST__V65*/ meltfptr[64];;
		MELT_LOCATION ("warmelt-normal.melt:8331:/ blockmultialloc");
		/*multiallocblock */
		{
		  struct meltletrec_2_st
		  {
		    struct MELT_MULTIPLE_STRUCT (2) rtup_0__TUPLREC__x23;
		    long meltletrec_2_endgap;
		  } *meltletrec_2_ptr = 0;
		  meltletrec_2_ptr =
		    (struct meltletrec_2_st *)
		    meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
		  /*^blockmultialloc.initfill */
		  /*inimult rtup_0__TUPLREC__x23 */
 /*_.TUPLREC___V67*/ meltfptr[66] =
		    (melt_ptr_t) & meltletrec_2_ptr->rtup_0__TUPLREC__x23;
		  meltletrec_2_ptr->rtup_0__TUPLREC__x23.discr =
		    (meltobject_ptr_t) (((melt_ptr_t)
					 (MELT_PREDEF (DISCR_MULTIPLE))));
		  meltletrec_2_ptr->rtup_0__TUPLREC__x23.nbval = 2;


		  /*^putuple */
		  /*putupl#59 */
		  melt_assertmsg ("putupl [:8331] #59 checktup",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.TUPLREC___V67*/
						     meltfptr[66])) ==
				  MELTOBMAG_MULTIPLE);
		  melt_assertmsg ("putupl [:8331] #59 checkoff",
				  (0 >= 0
				   && 0 <
				   melt_multiple_length ((melt_ptr_t)
							 ( /*_.TUPLREC___V67*/
							  meltfptr[66]))));
		  ((meltmultiple_ptr_t) ( /*_.TUPLREC___V67*/ meltfptr[66]))->
		    tabval[0] = (melt_ptr_t) ( /*_.INST___V64*/ meltfptr[63]);
		  ;
		  /*^putuple */
		  /*putupl#60 */
		  melt_assertmsg ("putupl [:8331] #60 checktup",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.TUPLREC___V67*/
						     meltfptr[66])) ==
				  MELTOBMAG_MULTIPLE);
		  melt_assertmsg ("putupl [:8331] #60 checkoff",
				  (1 >= 0
				   && 1 <
				   melt_multiple_length ((melt_ptr_t)
							 ( /*_.TUPLREC___V67*/
							  meltfptr[66]))));
		  ((meltmultiple_ptr_t) ( /*_.TUPLREC___V67*/ meltfptr[66]))->
		    tabval[1] =
		    (melt_ptr_t) ( /*_.MODNAM__V22*/ meltfptr[21]);
		  ;
		  /*^touch */
		  meltgc_touch ( /*_.TUPLREC___V67*/ meltfptr[66]);
		  ;
		  /*_.TUPLE___V66*/ meltfptr[65] =
		    /*_.TUPLREC___V67*/ meltfptr[66];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8331:/ clear");
		/*clear *//*_.TUPLREC___V67*/ meltfptr[66] = 0;
		  /*^clear */
		/*clear *//*_.TUPLREC___V67*/ meltfptr[66] = 0;
		}		/*end multiallocblock */
		;
		MELT_LOCATION ("warmelt-normal.melt:8327:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_SOURCE_HOOK_CALL */ meltfrout->tabval[18])), (4), "CLASS_SOURCE_HOOK_CALL");
      /*_.INST__V69*/ meltfptr[68] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LOCA_LOCATION",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V69*/
						   meltfptr[68])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V69*/ meltfptr[68]), (1),
				      ( /*_.PSLOC__V5*/ meltfptr[4]),
				      "LOCA_LOCATION");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @SHOOK_CALLED",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V69*/
						   meltfptr[68])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V69*/ meltfptr[68]), (3),
				      (( /*!HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER */ meltfrout->tabval[19])), "SHOOK_CALLED");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @SARGOP_ARGS",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V69*/
						   meltfptr[68])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V69*/ meltfptr[68]), (2),
				      ( /*_.TUPLE___V66*/ meltfptr[65]),
				      "SARGOP_ARGS");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V69*/ meltfptr[68],
					      "newly made instance");
		;
		/*_.INST___V68*/ meltfptr[66] = /*_.INST__V69*/ meltfptr[68];;
		MELT_LOCATION ("warmelt-normal.melt:8325:/ blockmultialloc");
		/*multiallocblock */
		{
		  struct meltletrec_3_st
		  {
		    struct MELT_MULTIPLE_STRUCT (2) rtup_0__TUPLREC__x22;
		    long meltletrec_3_endgap;
		  } *meltletrec_3_ptr = 0;
		  meltletrec_3_ptr =
		    (struct meltletrec_3_st *)
		    meltgc_allocate (sizeof (struct meltletrec_3_st), 0);
		  /*^blockmultialloc.initfill */
		  /*inimult rtup_0__TUPLREC__x22 */
 /*_.TUPLREC___V71*/ meltfptr[70] =
		    (melt_ptr_t) & meltletrec_3_ptr->rtup_0__TUPLREC__x22;
		  meltletrec_3_ptr->rtup_0__TUPLREC__x22.discr =
		    (meltobject_ptr_t) (((melt_ptr_t)
					 (MELT_PREDEF (DISCR_MULTIPLE))));
		  meltletrec_3_ptr->rtup_0__TUPLREC__x22.nbval = 2;


		  /*^putuple */
		  /*putupl#61 */
		  melt_assertmsg ("putupl [:8325] #61 checktup",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.TUPLREC___V71*/
						     meltfptr[70])) ==
				  MELTOBMAG_MULTIPLE);
		  melt_assertmsg ("putupl [:8325] #61 checkoff",
				  (0 >= 0
				   && 0 <
				   melt_multiple_length ((melt_ptr_t)
							 ( /*_.TUPLREC___V71*/
							  meltfptr[70]))));
		  ((meltmultiple_ptr_t) ( /*_.TUPLREC___V71*/ meltfptr[70]))->
		    tabval[0] =
		    (melt_ptr_t) ( /*_.SCURENVBOX__V62*/ meltfptr[61]);
		  ;
		  /*^putuple */
		  /*putupl#62 */
		  melt_assertmsg ("putupl [:8325] #62 checktup",
				  melt_magic_discr ((melt_ptr_t)
						    ( /*_.TUPLREC___V71*/
						     meltfptr[70])) ==
				  MELTOBMAG_MULTIPLE);
		  melt_assertmsg ("putupl [:8325] #62 checkoff",
				  (1 >= 0
				   && 1 <
				   melt_multiple_length ((melt_ptr_t)
							 ( /*_.TUPLREC___V71*/
							  meltfptr[70]))));
		  ((meltmultiple_ptr_t) ( /*_.TUPLREC___V71*/ meltfptr[70]))->
		    tabval[1] = (melt_ptr_t) ( /*_.INST___V68*/ meltfptr[66]);
		  ;
		  /*^touch */
		  meltgc_touch ( /*_.TUPLREC___V71*/ meltfptr[70]);
		  ;
		  /*_.TUPLE___V70*/ meltfptr[69] =
		    /*_.TUPLREC___V71*/ meltfptr[70];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8325:/ clear");
		/*clear *//*_.TUPLREC___V71*/ meltfptr[70] = 0;
		  /*^clear */
		/*clear *//*_.TUPLREC___V71*/ meltfptr[70] = 0;
		}		/*end multiallocblock */
		;
		MELT_LOCATION ("warmelt-normal.melt:8321:/ quasiblock");


		/*^rawallocobj */
		/*rawallocobj */
		{
		  melt_ptr_t newobj = 0;
		  melt_raw_object_create (newobj,
					  (melt_ptr_t) (( /*!CLASS_SOURCE_OR */ meltfrout->tabval[17])), (3), "CLASS_SOURCE_OR");
      /*_.INST__V73*/ meltfptr[72] =
		    newobj;
		};
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @LOCA_LOCATION",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V73*/
						   meltfptr[72])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V73*/ meltfptr[72]), (1),
				      ( /*_.PSLOC__V5*/ meltfptr[4]),
				      "LOCA_LOCATION");
		;
		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @SOR_DISJ",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.INST__V73*/
						   meltfptr[72])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.INST__V73*/ meltfptr[72]), (2),
				      ( /*_.TUPLE___V70*/ meltfptr[69]),
				      "SOR_DISJ");
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.INST__V73*/ meltfptr[72],
					      "newly made instance");
		;
		/*_.SGETCURENVBOX__V72*/ meltfptr[70] =
		  /*_.INST__V73*/ meltfptr[72];;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8338:/ quasiblock");


		/*^multimsend */
		/*multimsend */
		{
		  union meltparam_un argtab[3];
		  union meltparam_un restab[1];
		  memset (&argtab, 0, sizeof (argtab));
		  memset (&restab, 0, sizeof (restab));
		  /*^multimsend.arg */
		  argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];	/*^multimsend.arg */
		  argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];	/*^multimsend.arg */
		  argtab[2].meltbp_aptr =
		    (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[13];
		  /*^multimsend.xres */
		  restab[0].meltbp_aptr = (melt_ptr_t *) & /*_.BINDLIST__V76*/ meltfptr[75];	/*^multimsend.send */
		  /*_.NCURENVBOX__V75*/ meltfptr[74] =
		    meltgc_send ((melt_ptr_t)
				 ( /*_.SGETCURENVBOX__V72*/ meltfptr[70]),
				 ((melt_ptr_t)
				  (( /*!NORMAL_EXP */ meltfrout->
				    tabval[21]))),
				 (MELTBPARSTR_PTR MELTBPARSTR_PTR
				  MELTBPARSTR_PTR ""), argtab,
				 (MELTBPARSTR_PTR ""), restab);
		}
		;
		/*^quasiblock */



		{
		  MELT_LOCATION ("warmelt-normal.melt:8341:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.BINDLIST__V76*/ meltfptr[75]),
				      (melt_ptr_t) ( /*_.CBIND__V54*/
						    meltfptr[40]));
		}
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8342:/ quasiblock");


		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @NUCMEB_EXPR",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.NUP__V51*/
						   meltfptr[36])) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object (( /*_.NUP__V51*/ meltfptr[36]), (1),
				      ( /*_.NCURENVBOX__V75*/ meltfptr[74]),
				      "NUCMEB_EXPR");
		;
		/*^touch */
		meltgc_touch ( /*_.NUP__V51*/ meltfptr[36]);
		;
		/*^touchobj */

		melt_dbgtrace_written_object ( /*_.NUP__V51*/ meltfptr[36],
					      "put-fields");
		;


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-normal.melt:8343:/ cppif.then");
		/*^block */
		/*anyblock */
		{


		  {
		    /*^locexp */
		    /*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
		    melt_dbgcounter++;
#endif
		    ;
		  }
		  ;

		  MELT_CHECK_SIGNAL ();
		  ;
       /*_#MELT_NEED_DBG__L14*/ meltfnum[0] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-normal.melt:8343:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L14*/ meltfnum[0])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	 /*_#MELT_CALLCOUNT__L15*/ meltfnum[14] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-normal.melt:8343:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[7];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L15*/ meltfnum[14];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-normal.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 8343;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "normexp_update_current_module_environment_reference result bindlist=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.BINDLIST__V76*/ meltfptr[75];
			  /*^apply.arg */
			  argtab[5].meltbp_cstring = " clocc=";
			  /*^apply.arg */
			  argtab[6].meltbp_aptr =
			    (melt_ptr_t *) & /*_.CLOCC__V56*/ meltfptr[33];
			  /*_.MELT_DEBUG_FUN__V79*/ meltfptr[78] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[3])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V78*/ meltfptr[77] =
			  /*_.MELT_DEBUG_FUN__V79*/ meltfptr[78];;
			/*epilog */

			MELT_LOCATION ("warmelt-normal.melt:8343:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[14] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V79*/ meltfptr[78] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_._IF___V78*/ meltfptr[77] = NULL;;
		    }
		  ;
		  MELT_LOCATION ("warmelt-normal.melt:8343:/ quasiblock");


		  /*_.PROGN___V80*/ meltfptr[78] =
		    /*_._IF___V78*/ meltfptr[77];;
		  /*^compute */
		  /*_.IFCPP___V77*/ meltfptr[76] =
		    /*_.PROGN___V80*/ meltfptr[78];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8343:/ clear");
		 /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[0] = 0;
		  /*^clear */
		 /*clear *//*_._IF___V78*/ meltfptr[77] = 0;
		  /*^clear */
		 /*clear *//*_.PROGN___V80*/ meltfptr[78] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V77*/ meltfptr[76] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-normal.melt:8344:/ quasiblock");


		/*_._RETVAL___V1*/ meltfptr[0] =
		  /*_.CLOCC__V56*/ meltfptr[33];;
		MELT_LOCATION ("warmelt-normal.melt:8344:/ putxtraresult");
		if (!meltxrestab_ || !meltxresdescr_)
		  goto meltlabend_rout;
		if (meltxresdescr_[0] != MELTBPAR_PTR)
		  goto meltlabend_rout;
		if (meltxrestab_[0].meltbp_aptr)
		  *(meltxrestab_[0].meltbp_aptr) =
		    (melt_ptr_t) ( /*_.BINDLIST__V76*/ meltfptr[75]);
		;
		/*^finalreturn */
		;
		/*finalret */ goto meltlabend_rout;

		MELT_CHECK_SIGNAL ();
		;
		/*_.MULTI___V74*/ meltfptr[73] =
		  /*_.RETURN___V81*/ meltfptr[77];;

		MELT_LOCATION ("warmelt-normal.melt:8338:/ clear");
	       /*clear *//*_.IFCPP___V77*/ meltfptr[76] = 0;
		/*^clear */
	       /*clear *//*_.RETURN___V81*/ meltfptr[77] = 0;

		/*^clear */
	       /*clear *//*_.BINDLIST__V76*/ meltfptr[75] = 0;
		/*_.LET___V50*/ meltfptr[34] =
		  /*_.MULTI___V74*/ meltfptr[73];;

		MELT_LOCATION ("warmelt-normal.melt:8294:/ clear");
	       /*clear *//*_.NUP__V51*/ meltfptr[36] = 0;
		/*^clear */
	       /*clear *//*_.CSYM__V53*/ meltfptr[39] = 0;
		/*^clear */
	       /*clear *//*_.CBIND__V54*/ meltfptr[40] = 0;
		/*^clear */
	       /*clear *//*_.CLOCC__V56*/ meltfptr[33] = 0;
		/*^clear */
	       /*clear *//*_.LET___V58*/ meltfptr[57] = 0;
		/*^clear */
	       /*clear *//*_.CSBUF__V60*/ meltfptr[58] = 0;
		/*^clear */
	       /*clear *//*_.STRBUF2STRING__V61*/ meltfptr[60] = 0;
		/*^clear */
	       /*clear *//*_.SCURENVBOX__V62*/ meltfptr[61] = 0;
		/*^clear */
	       /*clear *//*_.INST___V64*/ meltfptr[63] = 0;
		/*^clear */
	       /*clear *//*_.TUPLE___V66*/ meltfptr[65] = 0;
		/*^clear */
	       /*clear *//*_.INST___V68*/ meltfptr[66] = 0;
		/*^clear */
	       /*clear *//*_.TUPLE___V70*/ meltfptr[69] = 0;
		/*^clear */
	       /*clear *//*_.SGETCURENVBOX__V72*/ meltfptr[70] = 0;
		/*^clear */
	       /*clear *//*_.MULTI___V74*/ meltfptr[73] = 0;
		MELT_LOCATION ("warmelt-normal.melt:8292:/ quasiblock");


		/*_.PROGN___V82*/ meltfptr[78] =
		  /*_.LET___V50*/ meltfptr[34];;
		/*^compute */
		/*_._IFELSE___V47*/ meltfptr[43] =
		  /*_.PROGN___V82*/ meltfptr[78];;
		/*epilog */

		MELT_LOCATION ("warmelt-normal.melt:8292:/ clear");
	       /*clear *//*_.IFCPP___V48*/ meltfptr[31] = 0;
		/*^clear */
	       /*clear *//*_.LET___V50*/ meltfptr[34] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V82*/ meltfptr[78] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-normal.melt:8348:/ cppif.then");
		/*^block */
		/*anyblock */
		{


		  MELT_CHECK_SIGNAL ();
		  ;
		  /*^cond */
		  /*cond */ if (( /*nil */ NULL))	/*then */
		    {
		      /*^cond.then */
		      /*_._IFELSE___V84*/ meltfptr[77] = ( /*nil */ NULL);;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-normal.melt:8348:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {




			{
			  /*^locexp */
			  melt_assert_failed (("normexp_update_current_module_environment_reference unexpected module\
 context"), ("warmelt-normal.melt") ? ("warmelt-normal.melt") : __FILE__,
					      (8348) ? (8348) : __LINE__, __FUNCTION__);
			  ;
			}
			;
		   /*clear *//*_._IFELSE___V84*/ meltfptr[77] = 0;
			/*epilog */
		      }
		      ;
		    }
		  ;
		  /*^compute */
		  /*_.IFCPP___V83*/ meltfptr[76] =
		    /*_._IFELSE___V84*/ meltfptr[77];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8348:/ clear");
		 /*clear *//*_._IFELSE___V84*/ meltfptr[77] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V83*/ meltfptr[76] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-normal.melt:8347:/ quasiblock");


		/*_.PROGN___V85*/ meltfptr[74] =
		  /*_.IFCPP___V83*/ meltfptr[76];;
		/*^compute */
		/*_._IFELSE___V47*/ meltfptr[43] =
		  /*_.PROGN___V85*/ meltfptr[74];;
		/*epilog */

		MELT_LOCATION ("warmelt-normal.melt:8292:/ clear");
	       /*clear *//*_.IFCPP___V83*/ meltfptr[76] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V85*/ meltfptr[74] = 0;
	      }
	      ;
	    }
	  ;
	  /*_._IFELSE___V30*/ meltfptr[24] =
	    /*_._IFELSE___V47*/ meltfptr[43];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8267:/ clear");
	     /*clear *//*_#IS_A__L12*/ meltfnum[10] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V47*/ meltfptr[43] = 0;
	}
	;
      }
    ;
    /*_.LET___V16*/ meltfptr[12] = /*_._IFELSE___V30*/ meltfptr[24];;

    MELT_LOCATION ("warmelt-normal.melt:8254:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[13] = 0;
    /*^clear */
	   /*clear *//*_.SCOMM__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.CURPROC__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.INIPROC__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.MODCTX__V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.MODNAM__V22*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_#exeq__L6*/ meltfnum[4] = 0;
    /*^clear */
	   /*clear *//*_._IF___V23*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V26*/ meltfptr[23] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L9*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V30*/ meltfptr[24] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8246:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8246:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE",
		  meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_159_WARMELTmiNORMAL_NORMEXP_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un *
						       meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un *
						       meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 39
    melt_ptr_t mcfr_varptr[39];
#define MELTFRAM_NBVARNUM 15
    long mcfr_varnum[15];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED_st *)
	meltfirstargp_;
      /* use arguments meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 39; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED nbval 39*/
  meltfram__.mcfr_nbvar = 39 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_FETCH_PREDEFINED", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8355:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8356:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_FETCH_PREDEFINED */ meltfrout->tabval[0])));;
      MELT_LOCATION ("warmelt-normal.melt:8356:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8356:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check fetch_predefined recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8356) ? (8356) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8356:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8357:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8357:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8357:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8357) ? (8357) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8357:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8358:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8358:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8358:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8358) ? (8358) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8358:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8359:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8359:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8359:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8359;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_fetch_predefined recv";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V13*/ meltfptr[12] =
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8359:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L5*/ meltfnum[4] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V13*/ meltfptr[12] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8359:/ quasiblock");


      /*_.PROGN___V15*/ meltfptr[13] = /*_._IF___V13*/ meltfptr[12];;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_.PROGN___V15*/ meltfptr[13];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8359:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V15*/ meltfptr[13] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8360:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8361:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[13] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8362:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SFEPD_PREDEF");
  /*_.SPRED__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8363:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 7, "NCTX_PREDEFMAP");
  /*_.PREDEFMAP__V19*/ meltfptr[18] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_A__L6*/ meltfnum[4] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]),
			   (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					  tabval[4])));;
    MELT_LOCATION ("warmelt-normal.melt:8365:/ cond");
    /*cond */ if ( /*_#IS_A__L6*/ meltfnum[4])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-normal.melt:8367:/ quasiblock");



	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^quasiblock */


	  /*^rawallocobj */
	  /*rawallocobj */
	  {
	    melt_ptr_t newobj = 0;
	    melt_raw_object_create (newobj,
				    (melt_ptr_t) (( /*!CLASS_REFERENCE */
						   meltfrout->tabval[5])),
				    (1), "CLASS_REFERENCE");
    /*_.INST__V22*/ meltfptr[21] =
	      newobj;
	  };
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V22*/ meltfptr[21]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V22*/ meltfptr[21]), (0),
				(( /*nil */ NULL)), "REFERENCED_VALUE");
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.INST__V22*/ meltfptr[21],
					"newly made instance");
	  ;
	  /*_.CONTK__V21*/ meltfptr[20] = /*_.INST__V22*/ meltfptr[21];;
	  MELT_LOCATION ("warmelt-normal.melt:8370:/ quasiblock");


	  /*^newclosure */
		   /*newclosure *//*_.LAMBDA___V24*/ meltfptr[23] =
	    (melt_ptr_t)
	    meltgc_new_closure ((meltobject_ptr_t)
				(((melt_ptr_t)
				  (MELT_PREDEF (DISCR_CLOSURE)))),
				(meltroutine_ptr_t) (( /*!konst_8 */
						      meltfrout->tabval[8])),
				(2));
	  ;
	  /*^putclosedv */
	  /*putclosv */
	  melt_assertmsg ("putclosv checkclo",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.LAMBDA___V24*/
					     meltfptr[23])) ==
			  MELTOBMAG_CLOSURE);
	  melt_assertmsg ("putclosv checkoff", 0 >= 0
			  && 0 <
			  melt_closure_size ((melt_ptr_t)
					     ( /*_.LAMBDA___V24*/
					      meltfptr[23])));
	  ((meltclosure_ptr_t) /*_.LAMBDA___V24*/ meltfptr[23])->tabval[0] =
	    (melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]);
	  ;
	  /*^putclosedv */
	  /*putclosv */
	  melt_assertmsg ("putclosv checkclo",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.LAMBDA___V24*/
					     meltfptr[23])) ==
			  MELTOBMAG_CLOSURE);
	  melt_assertmsg ("putclosv checkoff", 1 >= 0
			  && 1 <
			  melt_closure_size ((melt_ptr_t)
					     ( /*_.LAMBDA___V24*/
					      meltfptr[23])));
	  ((meltclosure_ptr_t) /*_.LAMBDA___V24*/ meltfptr[23])->tabval[1] =
	    (melt_ptr_t) ( /*_.CONTK__V21*/ meltfptr[20]);
	  ;
	  /*_.LAMBDA___V23*/ meltfptr[22] = /*_.LAMBDA___V24*/ meltfptr[23];;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8369:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.LAMBDA___V23*/ meltfptr[22];
	    /*_.MAPOBJECT_EVERY__V25*/ meltfptr[24] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!MAPOBJECT_EVERY */ meltfrout->tabval[6])),
			  (melt_ptr_t) ( /*_.PREDEFMAP__V19*/ meltfptr[18]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8372:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.CONTK__V21*/
					       meltfptr[20]),
					      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[5])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.CONTK__V21*/ meltfptr[20]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
     /*_.REFERENCED_VALUE__V26*/ meltfptr[25] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.REFERENCED_VALUE__V26*/ meltfptr[25] = NULL;;
	    }
	  ;
	  /*^compute */
   /*_#NULL__L7*/ meltfnum[0] =
	    (( /*_.REFERENCED_VALUE__V26*/ meltfptr[25]) == NULL);;
	  MELT_LOCATION ("warmelt-normal.melt:8372:/ cond");
	  /*cond */ if ( /*_#NULL__L7*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-normal.melt:8374:/ getslot");
		{
		  melt_ptr_t slot = NULL, obj = NULL;
		  obj =
		    (melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]) /*=obj*/ ;
		  melt_object_get_field (slot, obj, 1, "NAMED_NAME");
      /*_.NAMED_NAME__V27*/ meltfptr[26] = slot;
		};
		;

		{
		  MELT_LOCATION ("warmelt-normal.melt:8373:/ locexp");
		  melt_warning_str (0,
				    (melt_ptr_t) ( /*_.SLOC__V17*/
						  meltfptr[13]),
				    ("FETCH_PREDEFINED unknown predef name"),
				    (melt_ptr_t) ( /*_.NAMED_NAME__V27*/
						  meltfptr[26]));
		}
		;
		/*epilog */

		MELT_LOCATION ("warmelt-normal.melt:8372:/ clear");
	       /*clear *//*_.NAMED_NAME__V27*/ meltfptr[26] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*_._IFELSE___V20*/ meltfptr[19] =
	    /*_.MAPOBJECT_EVERY__V25*/ meltfptr[24];;

	  MELT_LOCATION ("warmelt-normal.melt:8367:/ clear");
	     /*clear *//*_.CONTK__V21*/ meltfptr[20] = 0;
	  /*^clear */
	     /*clear *//*_.LAMBDA___V23*/ meltfptr[22] = 0;
	  /*^clear */
	     /*clear *//*_.MAPOBJECT_EVERY__V25*/ meltfptr[24] = 0;
	  /*^clear */
	     /*clear *//*_.REFERENCED_VALUE__V26*/ meltfptr[25] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L7*/ meltfnum[0] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-normal.melt:8365:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_INTEGERBOX__L8*/ meltfnum[0] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]))
	     == MELTOBMAG_INT);;
	  MELT_LOCATION ("warmelt-normal.melt:8377:/ cond");
	  /*cond */ if ( /*_#IS_INTEGERBOX__L8*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-normal.melt:8379:/ quasiblock");


     /*_#PREDRK__L9*/ meltfnum[8] =
		  (melt_get_int
		   ((melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17])));;

		MELT_CHECK_SIGNAL ();
		;
		/*^compute */
     /*_#lteqI__L10*/ meltfnum[9] =
		  (( /*_#PREDRK__L9*/ meltfnum[8]) <= (0));;
		MELT_LOCATION ("warmelt-normal.melt:8380:/ cond");
		/*cond */ if ( /*_#lteqI__L10*/ meltfnum[9])	/*then */
		  {
		    /*^cond.then */
		    /*_#OR___L11*/ meltfnum[10] =
		      /*_#lteqI__L10*/ meltfnum[9];;
		  }
		else
		  {
		    MELT_LOCATION ("warmelt-normal.melt:8380:/ cond.else");

		    /*^block */
		    /*anyblock */
		    {

       /*_#LAST_GLOBPREDEF_INDEX__L12*/ meltfnum[11] =
			BGLOB__LASTGLOB;;
		      /*^compute */
       /*_#gteqI__L13*/ meltfnum[12] =
			(( /*_#PREDRK__L9*/ meltfnum[8]) >=
			 ( /*_#LAST_GLOBPREDEF_INDEX__L12*/ meltfnum[11]));;
		      /*^compute */
		      /*_#OR___L11*/ meltfnum[10] =
			/*_#gteqI__L13*/ meltfnum[12];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-normal.melt:8380:/ clear");
		 /*clear *//*_#LAST_GLOBPREDEF_INDEX__L12*/ meltfnum[11]
			= 0;
		      /*^clear */
		 /*clear *//*_#gteqI__L13*/ meltfnum[12] = 0;
		    }
		    ;
		  }
		;
		/*^cond */
		/*cond */ if ( /*_#OR___L11*/ meltfnum[10])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {




		      {
			MELT_LOCATION ("warmelt-normal.melt:8381:/ locexp");
			melt_warning_str (0,
					  (melt_ptr_t) ( /*_.SLOC__V17*/
							meltfptr[13]),
					  ("FETCH_PREDEFINED invalid predef rank"),
					  (melt_ptr_t) 0);
		      }
		      ;
		 /*clear *//*_._IFELSE___V28*/ meltfptr[26] = 0;
		      /*epilog */
		    }
		    ;
		  }
		else
		  {
		    MELT_LOCATION ("warmelt-normal.melt:8380:/ cond.else");

      /*_._IFELSE___V28*/ meltfptr[26] = NULL;;
		  }
		;

		MELT_LOCATION ("warmelt-normal.melt:8379:/ clear");
	       /*clear *//*_#PREDRK__L9*/ meltfnum[8] = 0;
		/*^clear */
	       /*clear *//*_#lteqI__L10*/ meltfnum[9] = 0;
		/*^clear */
	       /*clear *//*_#OR___L11*/ meltfnum[10] = 0;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-normal.melt:8377:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-normal.melt:8385:/ cppif.then");
		/*^block */
		/*anyblock */
		{


		  MELT_CHECK_SIGNAL ();
		  ;
		  /*^cond */
		  /*cond */ if (( /*nil */ NULL))	/*then */
		    {
		      /*^cond.then */
		      /*_._IFELSE___V30*/ meltfptr[22] = ( /*nil */ NULL);;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-normal.melt:8385:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {




			{
			  /*^locexp */
			  melt_assert_failed (("FETCH_PREDEFINED bad predef"),
					      ("warmelt-normal.melt")
					      ? ("warmelt-normal.melt") :
					      __FILE__,
					      (8385) ? (8385) : __LINE__,
					      __FUNCTION__);
			  ;
			}
			;
		   /*clear *//*_._IFELSE___V30*/ meltfptr[22] = 0;
			/*epilog */
		      }
		      ;
		    }
		  ;
		  /*^compute */
		  /*_.IFCPP___V29*/ meltfptr[20] =
		    /*_._IFELSE___V30*/ meltfptr[22];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8385:/ clear");
		 /*clear *//*_._IFELSE___V30*/ meltfptr[22] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V29*/ meltfptr[20] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-normal.melt:8384:/ quasiblock");


		/*_.PROGN___V31*/ meltfptr[24] =
		  /*_.IFCPP___V29*/ meltfptr[20];;
		/*^compute */
		/*_._IFELSE___V28*/ meltfptr[26] =
		  /*_.PROGN___V31*/ meltfptr[24];;
		/*epilog */

		MELT_LOCATION ("warmelt-normal.melt:8377:/ clear");
	       /*clear *//*_.IFCPP___V29*/ meltfptr[20] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V31*/ meltfptr[24] = 0;
	      }
	      ;
	    }
	  ;
	  /*_._IFELSE___V20*/ meltfptr[19] =
	    /*_._IFELSE___V28*/ meltfptr[26];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8365:/ clear");
	     /*clear *//*_#IS_INTEGERBOX__L8*/ meltfnum[0] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V28*/ meltfptr[26] = 0;
	}
	;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8387:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^quasiblock */


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_PREDEF */
					     meltfrout->tabval[9])), (2),
			      "CLASS_NREP_PREDEF");
  /*_.INST__V34*/ meltfptr[20] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V34*/ meltfptr[20])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V34*/ meltfptr[20]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[13]), "NREP_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NRPREDEF",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V34*/ meltfptr[20])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V34*/ meltfptr[20]), (1),
			  ( /*_.SPRED__V18*/ meltfptr[17]), "NRPREDEF");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V34*/ meltfptr[20],
				  "newly made instance");
    ;
    /*_.NPRE__V33*/ meltfptr[22] = /*_.INST__V34*/ meltfptr[20];;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8392:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L14*/ meltfnum[11] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8392:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L14*/ meltfnum[11])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L15*/ meltfnum[12] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8392:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[12];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8392;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_fetch_predefined result npre";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NPRE__V33*/ meltfptr[22];
	      /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V36*/ meltfptr[26] =
	      /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8392:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[12] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V36*/ meltfptr[26] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8392:/ quasiblock");


      /*_.PROGN___V38*/ meltfptr[36] = /*_._IF___V36*/ meltfptr[26];;
      /*^compute */
      /*_.IFCPP___V35*/ meltfptr[24] = /*_.PROGN___V38*/ meltfptr[36];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8392:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[11] = 0;
      /*^clear */
	     /*clear *//*_._IF___V36*/ meltfptr[26] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V38*/ meltfptr[36] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V35*/ meltfptr[24] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8393:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.NPRE__V33*/ meltfptr[22];;
    MELT_LOCATION ("warmelt-normal.melt:8393:/ putxtraresult");
    if (!meltxrestab_ || !meltxresdescr_)
      goto meltlabend_rout;
    if (meltxresdescr_[0] != MELTBPAR_PTR)
      goto meltlabend_rout;
    if (meltxrestab_[0].meltbp_aptr)
      *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (( /*nil */ NULL));
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V32*/ meltfptr[25] = /*_.RETURN___V39*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-normal.melt:8387:/ clear");
	   /*clear *//*_.NPRE__V33*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V35*/ meltfptr[24] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V39*/ meltfptr[26] = 0;
    /*_.LET___V16*/ meltfptr[12] = /*_.LET___V32*/ meltfptr[25];;

    MELT_LOCATION ("warmelt-normal.melt:8360:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[13] = 0;
    /*^clear */
	   /*clear *//*_.SPRED__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.PREDEFMAP__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L6*/ meltfnum[4] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.LET___V32*/ meltfptr[25] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8355:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8355:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_FETCH_PREDEFINED", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_160_WARMELTmiNORMAL_NORMEXP_FETCH_PREDEFINED */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un * meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 3
    melt_ptr_t mcfr_varptr[3];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47 is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47_st *)
	meltfirstargp_;
      /* use arguments meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47 output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 3; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47 nbval 3*/
  meltfram__.mcfr_nbvar = 3 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_cl47", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8370:/ getarg");
 /*_.KEY__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.VAL__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.VAL__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#eqeq__L1*/ meltfnum[0] =
      (( /*_.VAL__V3*/ meltfptr[2]) ==
       (( /*~SPRED */ meltfclos->tabval[0])));;
    MELT_LOCATION ("warmelt-normal.melt:8371:/ cond");
    /*cond */ if ( /*_#eqeq__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*~CONTK */ meltfclos->
						tabval[1])),
					      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[0])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
				melt_magic_discr ((melt_ptr_t)
						  (( /*~CONTK */ meltfclos->
						    tabval[1]))) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object ((( /*~CONTK */ meltfclos->tabval[1])),
				      (0), ( /*_.KEY__V2*/ meltfptr[1]),
				      "REFERENCED_VALUE");
		;
		/*^touch */
		meltgc_touch (( /*~CONTK */ meltfclos->tabval[1]));
		;
		/*^touchobj */

		melt_dbgtrace_written_object (( /*~CONTK */ meltfclos->
					       tabval[1]), "put-fields");
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-normal.melt:8370:/ clear");
	   /*clear *//*_#eqeq__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_cl47", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_161_WARMELTmiNORMAL_LAMBDA_cl47 */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED (meltclosure_ptr_t
						       meltclosp_,
						       melt_ptr_t
						       meltfirstargp_,
						       const
						       melt_argdescr_cell_t
						       meltxargdescr_[],
						       union meltparam_un *
						       meltxargtab_,
						       const
						       melt_argdescr_cell_t
						       meltxresdescr_[],
						       union meltparam_un *
						       meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 51
    melt_ptr_t mcfr_varptr[51];
#define MELTFRAM_NBVARNUM 16
    long mcfr_varnum[16];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED_st *)
	meltfirstargp_;
      /* use arguments meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 51; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED nbval 51*/
  meltfram__.mcfr_nbvar = 51 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_STORE_PREDEFINED", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8401:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8402:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_STORE_PREDEFINED */ meltfrout->tabval[0])));;
      MELT_LOCATION ("warmelt-normal.melt:8402:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V7*/ meltfptr[6] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8402:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check store_predefined recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8402) ? (8402) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8402:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V7*/ meltfptr[6] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8403:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L2*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8403:/ cond");
      /*cond */ if ( /*_#IS_A__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8403:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8403) ? (8403) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8403:/ clear");
	     /*clear *//*_#IS_A__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V8*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8404:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8404:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[10] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8404:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8404) ? (8404) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[10];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8404:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[10] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[8] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8405:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8405:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8405:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8405;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_store_predefined recv";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V13*/ meltfptr[12] =
	      /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8405:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L5*/ meltfnum[4] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V13*/ meltfptr[12] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8405:/ quasiblock");


      /*_.PROGN___V15*/ meltfptr[13] = /*_._IF___V13*/ meltfptr[12];;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[10] = /*_.PROGN___V15*/ meltfptr[13];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8405:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V13*/ meltfptr[12] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V15*/ meltfptr[13] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8406:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8407:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[13] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8408:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "SSTPD_PREDEF");
  /*_.SPRED__V18*/ meltfptr[17] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8409:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "SSTPD_VALUE");
  /*_.SVAL__V19*/ meltfptr[18] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8410:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 7, "NCTX_PREDEFMAP");
  /*_.PREDEFMAP__V20*/ meltfptr[19] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_A__L6*/ meltfnum[4] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]),
			   (melt_ptr_t) (( /*!CLASS_SYMBOL */ meltfrout->
					  tabval[4])));;
    MELT_LOCATION ("warmelt-normal.melt:8412:/ cond");
    /*cond */ if ( /*_#IS_A__L6*/ meltfnum[4])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-normal.melt:8414:/ quasiblock");



	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^quasiblock */


	  /*^rawallocobj */
	  /*rawallocobj */
	  {
	    melt_ptr_t newobj = 0;
	    melt_raw_object_create (newobj,
				    (melt_ptr_t) (( /*!CLASS_REFERENCE */
						   meltfrout->tabval[5])),
				    (1), "CLASS_REFERENCE");
    /*_.INST__V23*/ meltfptr[22] =
	      newobj;
	  };
	  ;
	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.INST__V23*/ meltfptr[22]))
			  == MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.INST__V23*/ meltfptr[22]), (0),
				(( /*nil */ NULL)), "REFERENCED_VALUE");
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.INST__V23*/ meltfptr[22],
					"newly made instance");
	  ;
	  /*_.CONTK__V22*/ meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;
	  MELT_LOCATION ("warmelt-normal.melt:8417:/ quasiblock");


	  /*^newclosure */
		   /*newclosure *//*_.LAMBDA___V25*/ meltfptr[24] =
	    (melt_ptr_t)
	    meltgc_new_closure ((meltobject_ptr_t)
				(((melt_ptr_t)
				  (MELT_PREDEF (DISCR_CLOSURE)))),
				(meltroutine_ptr_t) (( /*!konst_8 */
						      meltfrout->tabval[8])),
				(2));
	  ;
	  /*^putclosedv */
	  /*putclosv */
	  melt_assertmsg ("putclosv checkclo",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.LAMBDA___V25*/
					     meltfptr[24])) ==
			  MELTOBMAG_CLOSURE);
	  melt_assertmsg ("putclosv checkoff", 0 >= 0
			  && 0 <
			  melt_closure_size ((melt_ptr_t)
					     ( /*_.LAMBDA___V25*/
					      meltfptr[24])));
	  ((meltclosure_ptr_t) /*_.LAMBDA___V25*/ meltfptr[24])->tabval[0] =
	    (melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]);
	  ;
	  /*^putclosedv */
	  /*putclosv */
	  melt_assertmsg ("putclosv checkclo",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.LAMBDA___V25*/
					     meltfptr[24])) ==
			  MELTOBMAG_CLOSURE);
	  melt_assertmsg ("putclosv checkoff", 1 >= 0
			  && 1 <
			  melt_closure_size ((melt_ptr_t)
					     ( /*_.LAMBDA___V25*/
					      meltfptr[24])));
	  ((meltclosure_ptr_t) /*_.LAMBDA___V25*/ meltfptr[24])->tabval[1] =
	    (melt_ptr_t) ( /*_.CONTK__V22*/ meltfptr[21]);
	  ;
	  /*_.LAMBDA___V24*/ meltfptr[23] = /*_.LAMBDA___V25*/ meltfptr[24];;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8416:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.LAMBDA___V24*/ meltfptr[23];
	    /*_.MAPOBJECT_EVERY__V26*/ meltfptr[25] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!MAPOBJECT_EVERY */ meltfrout->tabval[6])),
			  (melt_ptr_t) ( /*_.PREDEFMAP__V20*/ meltfptr[19]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8419:/ cond");
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      ( /*_.CONTK__V22*/
					       meltfptr[21]),
					      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[5])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj = (melt_ptr_t) ( /*_.CONTK__V22*/ meltfptr[21]) /*=obj*/ ;
		melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
     /*_.REFERENCED_VALUE__V27*/ meltfptr[26] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.REFERENCED_VALUE__V27*/ meltfptr[26] = NULL;;
	    }
	  ;
	  /*^compute */
   /*_#NULL__L7*/ meltfnum[0] =
	    (( /*_.REFERENCED_VALUE__V27*/ meltfptr[26]) == NULL);;
	  MELT_LOCATION ("warmelt-normal.melt:8419:/ cond");
	  /*cond */ if ( /*_#NULL__L7*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-normal.melt:8421:/ getslot");
		{
		  melt_ptr_t slot = NULL, obj = NULL;
		  obj =
		    (melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]) /*=obj*/ ;
		  melt_object_get_field (slot, obj, 1, "NAMED_NAME");
      /*_.NAMED_NAME__V28*/ meltfptr[27] = slot;
		};
		;

		{
		  MELT_LOCATION ("warmelt-normal.melt:8420:/ locexp");
		  melt_warning_str (0,
				    (melt_ptr_t) ( /*_.SLOC__V17*/
						  meltfptr[13]),
				    ("STORE_PREDEFINED unknown predef name"),
				    (melt_ptr_t) ( /*_.NAMED_NAME__V28*/
						  meltfptr[27]));
		}
		;
		/*epilog */

		MELT_LOCATION ("warmelt-normal.melt:8419:/ clear");
	       /*clear *//*_.NAMED_NAME__V28*/ meltfptr[27] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*_._IFELSE___V21*/ meltfptr[20] =
	    /*_.MAPOBJECT_EVERY__V26*/ meltfptr[25];;

	  MELT_LOCATION ("warmelt-normal.melt:8414:/ clear");
	     /*clear *//*_.CONTK__V22*/ meltfptr[21] = 0;
	  /*^clear */
	     /*clear *//*_.LAMBDA___V24*/ meltfptr[23] = 0;
	  /*^clear */
	     /*clear *//*_.MAPOBJECT_EVERY__V26*/ meltfptr[25] = 0;
	  /*^clear */
	     /*clear *//*_.REFERENCED_VALUE__V27*/ meltfptr[26] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L7*/ meltfnum[0] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-normal.melt:8412:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_INTEGERBOX__L8*/ meltfnum[0] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17]))
	     == MELTOBMAG_INT);;
	  MELT_LOCATION ("warmelt-normal.melt:8424:/ cond");
	  /*cond */ if ( /*_#IS_INTEGERBOX__L8*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-normal.melt:8426:/ quasiblock");


     /*_#PREDRK__L9*/ meltfnum[8] =
		  (melt_get_int
		   ((melt_ptr_t) ( /*_.SPRED__V18*/ meltfptr[17])));;

		MELT_CHECK_SIGNAL ();
		;
		/*^compute */
     /*_#lteqI__L10*/ meltfnum[9] =
		  (( /*_#PREDRK__L9*/ meltfnum[8]) <= (0));;
		MELT_LOCATION ("warmelt-normal.melt:8427:/ cond");
		/*cond */ if ( /*_#lteqI__L10*/ meltfnum[9])	/*then */
		  {
		    /*^cond.then */
		    /*_#OR___L11*/ meltfnum[10] =
		      /*_#lteqI__L10*/ meltfnum[9];;
		  }
		else
		  {
		    MELT_LOCATION ("warmelt-normal.melt:8427:/ cond.else");

		    /*^block */
		    /*anyblock */
		    {

       /*_#LAST_GLOBPREDEF_INDEX__L12*/ meltfnum[11] =
			BGLOB__LASTGLOB;;
		      /*^compute */
       /*_#gteqI__L13*/ meltfnum[12] =
			(( /*_#PREDRK__L9*/ meltfnum[8]) >=
			 ( /*_#LAST_GLOBPREDEF_INDEX__L12*/ meltfnum[11]));;
		      /*^compute */
		      /*_#OR___L11*/ meltfnum[10] =
			/*_#gteqI__L13*/ meltfnum[12];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-normal.melt:8427:/ clear");
		 /*clear *//*_#LAST_GLOBPREDEF_INDEX__L12*/ meltfnum[11]
			= 0;
		      /*^clear */
		 /*clear *//*_#gteqI__L13*/ meltfnum[12] = 0;
		    }
		    ;
		  }
		;
		/*^cond */
		/*cond */ if ( /*_#OR___L11*/ meltfnum[10])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {




		      {
			MELT_LOCATION ("warmelt-normal.melt:8428:/ locexp");
			melt_warning_str (0,
					  (melt_ptr_t) ( /*_.SLOC__V17*/
							meltfptr[13]),
					  ("STORE_PREDEFINED invalid predef rank"),
					  (melt_ptr_t) 0);
		      }
		      ;
		 /*clear *//*_._IFELSE___V29*/ meltfptr[27] = 0;
		      /*epilog */
		    }
		    ;
		  }
		else
		  {
		    MELT_LOCATION ("warmelt-normal.melt:8427:/ cond.else");

      /*_._IFELSE___V29*/ meltfptr[27] = NULL;;
		  }
		;

		MELT_LOCATION ("warmelt-normal.melt:8426:/ clear");
	       /*clear *//*_#PREDRK__L9*/ meltfnum[8] = 0;
		/*^clear */
	       /*clear *//*_#lteqI__L10*/ meltfnum[9] = 0;
		/*^clear */
	       /*clear *//*_#OR___L11*/ meltfnum[10] = 0;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-normal.melt:8424:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-normal.melt:8432:/ cppif.then");
		/*^block */
		/*anyblock */
		{


		  MELT_CHECK_SIGNAL ();
		  ;
		  /*^cond */
		  /*cond */ if (( /*nil */ NULL))	/*then */
		    {
		      /*^cond.then */
		      /*_._IFELSE___V31*/ meltfptr[23] = ( /*nil */ NULL);;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-normal.melt:8432:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {




			{
			  /*^locexp */
			  melt_assert_failed (("STORE_PREDEFINED bad predef"),
					      ("warmelt-normal.melt")
					      ? ("warmelt-normal.melt") :
					      __FILE__,
					      (8432) ? (8432) : __LINE__,
					      __FUNCTION__);
			  ;
			}
			;
		   /*clear *//*_._IFELSE___V31*/ meltfptr[23] = 0;
			/*epilog */
		      }
		      ;
		    }
		  ;
		  /*^compute */
		  /*_.IFCPP___V30*/ meltfptr[21] =
		    /*_._IFELSE___V31*/ meltfptr[23];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8432:/ clear");
		 /*clear *//*_._IFELSE___V31*/ meltfptr[23] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V30*/ meltfptr[21] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-normal.melt:8431:/ quasiblock");


		/*_.PROGN___V32*/ meltfptr[25] =
		  /*_.IFCPP___V30*/ meltfptr[21];;
		/*^compute */
		/*_._IFELSE___V29*/ meltfptr[27] =
		  /*_.PROGN___V32*/ meltfptr[25];;
		/*epilog */

		MELT_LOCATION ("warmelt-normal.melt:8424:/ clear");
	       /*clear *//*_.IFCPP___V30*/ meltfptr[21] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V32*/ meltfptr[25] = 0;
	      }
	      ;
	    }
	  ;
	  /*_._IFELSE___V21*/ meltfptr[20] =
	    /*_._IFELSE___V29*/ meltfptr[27];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8412:/ clear");
	     /*clear *//*_#IS_INTEGERBOX__L8*/ meltfnum[0] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V29*/ meltfptr[27] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8434:/ quasiblock");


    /*^multimsend */
    /*multimsend */
    {
      union meltparam_un argtab[3];
      union meltparam_un restab[1];
      memset (&argtab, 0, sizeof (argtab));
      memset (&restab, 0, sizeof (restab));
      /*^multimsend.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.ENV__V3*/ meltfptr[2];	/*^multimsend.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];	/*^multimsend.arg */
      argtab[2].meltbp_aptr = (melt_ptr_t *) & /*_.SLOC__V17*/ meltfptr[13];
      /*^multimsend.xres */
      restab[0].meltbp_aptr = (melt_ptr_t *) & /*_.NBINDS__V35*/ meltfptr[21];	/*^multimsend.send */
      /*_.NVAL__V34*/ meltfptr[23] =
	meltgc_send ((melt_ptr_t) ( /*_.SVAL__V19*/ meltfptr[18]),
		     ((melt_ptr_t)
		      (( /*!NORMAL_EXP */ meltfrout->tabval[9]))),
		     (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""),
		     argtab, (MELTBPARSTR_PTR ""), restab);
    }
    ;
    /*^quasiblock */



    MELT_CHECK_SIGNAL ();
    ;
 /*_#NULL__L14*/ meltfnum[11] =
      (( /*_.NBINDS__V35*/ meltfptr[21]) == NULL);;
    MELT_LOCATION ("warmelt-normal.melt:8437:/ cond");
    /*cond */ if ( /*_#NULL__L14*/ meltfnum[11])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.MAKE_LIST__V37*/ meltfptr[27] =
	    (meltgc_new_list
	     ((meltobject_ptr_t)
	      (( /*!DISCR_LIST */ meltfrout->tabval[10]))));;
	  MELT_LOCATION ("warmelt-normal.melt:8438:/ compute");
	  /*_.NBINDS__V35*/ meltfptr[21] = /*_.SETQ___V38*/ meltfptr[37] =
	    /*_.MAKE_LIST__V37*/ meltfptr[27];;
	  /*_._IF___V36*/ meltfptr[25] = /*_.SETQ___V38*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-normal.melt:8437:/ clear");
	     /*clear *//*_.MAKE_LIST__V37*/ meltfptr[27] = 0;
	  /*^clear */
	     /*clear *//*_.SETQ___V38*/ meltfptr[37] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V36*/ meltfptr[25] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8439:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      /*_.CSYM__V40*/ meltfptr[37] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!CLONE_SYMBOL */ meltfrout->tabval[11])),
		    (melt_ptr_t) (( /*!konst_12__STOREPREDEF_ */ meltfrout->
				   tabval[12])), (""),
		    (union meltparam_un *) 0, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8440:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_STORE_PREDEFINED */ meltfrout->tabval[13])), (3), "CLASS_NREP_STORE_PREDEFINED");
  /*_.INST__V42*/ meltfptr[41] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V42*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V42*/ meltfptr[41]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[13]), "NREP_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NSTPD_PREDEF",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V42*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V42*/ meltfptr[41]), (1),
			  ( /*_.SPRED__V18*/ meltfptr[17]), "NSTPD_PREDEF");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NSTPD_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V42*/ meltfptr[41])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V42*/ meltfptr[41]), (2),
			  ( /*_.NVAL__V34*/ meltfptr[23]), "NSTPD_VALUE");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V42*/ meltfptr[41],
				  "newly made instance");
    ;
    /*_.NFPRE__V41*/ meltfptr[40] = /*_.INST__V42*/ meltfptr[41];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8444:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NORMAL_LET_BINDING */
					     meltfrout->tabval[14])), (4),
			      "CLASS_NORMAL_LET_BINDING");
  /*_.INST__V44*/ meltfptr[43] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @BINDER",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V44*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V44*/ meltfptr[43]), (0),
			  ( /*_.CSYM__V40*/ meltfptr[37]), "BINDER");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @LETBIND_TYPE",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V44*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V44*/ meltfptr[43]), (1),
			  (( /*!CTYPE_VALUE */ meltfrout->tabval[15])),
			  "LETBIND_TYPE");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @LETBIND_EXPR",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V44*/ meltfptr[43])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V44*/ meltfptr[43]), (2),
			  ( /*_.NFPRE__V41*/ meltfptr[40]), "LETBIND_EXPR");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V44*/ meltfptr[43],
				  "newly made instance");
    ;
    /*_.CBIND__V43*/ meltfptr[42] = /*_.INST__V44*/ meltfptr[43];;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8448:/ quasiblock");


    /*^rawallocobj */
    /*rawallocobj */
    {
      melt_ptr_t newobj = 0;
      melt_raw_object_create (newobj,
			      (melt_ptr_t) (( /*!CLASS_NREP_LOCSYMOCC */
					     meltfrout->tabval[16])), (4),
			      "CLASS_NREP_LOCSYMOCC");
  /*_.INST__V46*/ meltfptr[45] =
	newobj;
    };
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NREP_LOC",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V46*/ meltfptr[45])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V46*/ meltfptr[45]), (0),
			  ( /*_.SLOC__V17*/ meltfptr[13]), "NREP_LOC");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NOCC_CTYP",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V46*/ meltfptr[45])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V46*/ meltfptr[45]), (2),
			  (( /*!CTYPE_VALUE */ meltfrout->tabval[15])),
			  "NOCC_CTYP");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NOCC_SYMB",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V46*/ meltfptr[45])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V46*/ meltfptr[45]), (1),
			  ( /*_.CSYM__V40*/ meltfptr[37]), "NOCC_SYMB");
    ;
    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @NOCC_BIND",
		    melt_magic_discr ((melt_ptr_t)
				      ( /*_.INST__V46*/ meltfptr[45])) ==
		    MELTOBMAG_OBJECT);
    melt_putfield_object (( /*_.INST__V46*/ meltfptr[45]), (3),
			  ( /*_.CBIND__V43*/ meltfptr[42]), "NOCC_BIND");
    ;
    /*^touchobj */

    melt_dbgtrace_written_object ( /*_.INST__V46*/ meltfptr[45],
				  "newly made instance");
    ;
    /*_.SYOCC__V45*/ meltfptr[44] = /*_.INST__V46*/ meltfptr[45];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8454:/ locexp");
      meltgc_append_list ((melt_ptr_t) ( /*_.NBINDS__V35*/ meltfptr[21]),
			  (melt_ptr_t) ( /*_.CBIND__V43*/ meltfptr[42]));
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8455:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L15*/ meltfnum[12] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8455:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L15*/ meltfnum[12])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L16*/ meltfnum[8] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8455:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[8];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8455;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_store_predefined result nbinds=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.NBINDS__V35*/ meltfptr[21];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " syocc=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.SYOCC__V45*/ meltfptr[44];
	      /*_.MELT_DEBUG_FUN__V49*/ meltfptr[48] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[3])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V48*/ meltfptr[47] =
	      /*_.MELT_DEBUG_FUN__V49*/ meltfptr[48];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8455:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L16*/ meltfnum[8] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V49*/ meltfptr[48] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V48*/ meltfptr[47] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8455:/ quasiblock");


      /*_.PROGN___V50*/ meltfptr[48] = /*_._IF___V48*/ meltfptr[47];;
      /*^compute */
      /*_.IFCPP___V47*/ meltfptr[46] = /*_.PROGN___V50*/ meltfptr[48];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8455:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L15*/ meltfnum[12] = 0;
      /*^clear */
	     /*clear *//*_._IF___V48*/ meltfptr[47] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V50*/ meltfptr[48] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V47*/ meltfptr[46] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8456:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.SYOCC__V45*/ meltfptr[44];;
    MELT_LOCATION ("warmelt-normal.melt:8456:/ putxtraresult");
    if (!meltxrestab_ || !meltxresdescr_)
      goto meltlabend_rout;
    if (meltxresdescr_[0] != MELTBPAR_PTR)
      goto meltlabend_rout;
    if (meltxrestab_[0].meltbp_aptr)
      *(meltxrestab_[0].meltbp_aptr) =
	(melt_ptr_t) ( /*_.NBINDS__V35*/ meltfptr[21]);
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V39*/ meltfptr[27] = /*_.RETURN___V51*/ meltfptr[47];;

    MELT_LOCATION ("warmelt-normal.melt:8439:/ clear");
	   /*clear *//*_.CSYM__V40*/ meltfptr[37] = 0;
    /*^clear */
	   /*clear *//*_.NFPRE__V41*/ meltfptr[40] = 0;
    /*^clear */
	   /*clear *//*_.CBIND__V43*/ meltfptr[42] = 0;
    /*^clear */
	   /*clear *//*_.SYOCC__V45*/ meltfptr[44] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V47*/ meltfptr[46] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V51*/ meltfptr[47] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*_.MULTI___V33*/ meltfptr[26] = /*_.LET___V39*/ meltfptr[27];;

    MELT_LOCATION ("warmelt-normal.melt:8434:/ clear");
	   /*clear *//*_#NULL__L14*/ meltfnum[11] = 0;
    /*^clear */
	   /*clear *//*_._IF___V36*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_.LET___V39*/ meltfptr[27] = 0;

    /*^clear */
	   /*clear *//*_.NBINDS__V35*/ meltfptr[21] = 0;
    /*_.LET___V16*/ meltfptr[12] = /*_.MULTI___V33*/ meltfptr[26];;

    MELT_LOCATION ("warmelt-normal.melt:8406:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[13] = 0;
    /*^clear */
	   /*clear *//*_.SPRED__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.SVAL__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.PREDEFMAP__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L6*/ meltfnum[4] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.MULTI___V33*/ meltfptr[26] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8401:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[12];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8401:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V8*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[10] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[12] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_STORE_PREDEFINED", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_162_WARMELTmiNORMAL_NORMEXP_STORE_PREDEFINED */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48 (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un * meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48_melt =
    melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 3
    melt_ptr_t mcfr_varptr[3];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48 is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48_st *)
	meltfirstargp_;
      /* use arguments meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48 output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 3; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48 nbval 3*/
  meltfram__.mcfr_nbvar = 3 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_cl48", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8417:/ getarg");
 /*_.KEY__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.VAL__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.VAL__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#eqeq__L1*/ meltfnum[0] =
      (( /*_.VAL__V3*/ meltfptr[2]) ==
       (( /*~SPRED */ meltfclos->tabval[0])));;
    MELT_LOCATION ("warmelt-normal.melt:8418:/ cond");
    /*cond */ if ( /*_#eqeq__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*~CONTK */ meltfclos->
						tabval[1])),
					      (melt_ptr_t) (( /*!CLASS_REFERENCE */ meltfrout->tabval[0])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*^putslot */
		/*putslot */
		melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
				melt_magic_discr ((melt_ptr_t)
						  (( /*~CONTK */ meltfclos->
						    tabval[1]))) ==
				MELTOBMAG_OBJECT);
		melt_putfield_object ((( /*~CONTK */ meltfclos->tabval[1])),
				      (0), ( /*_.KEY__V2*/ meltfptr[1]),
				      "REFERENCED_VALUE");
		;
		/*^touch */
		meltgc_touch (( /*~CONTK */ meltfclos->tabval[1]));
		;
		/*^touchobj */

		melt_dbgtrace_written_object (( /*~CONTK */ meltfclos->
					       tabval[1]), "put-fields");
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-normal.melt:8417:/ clear");
	   /*clear *//*_#eqeq__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_cl48", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_163_WARMELTmiNORMAL_LAMBDA_cl48 */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER (meltclosure_ptr_t meltclosp_,
					      melt_ptr_t meltfirstargp_,
					      const melt_argdescr_cell_t
					      meltxargdescr_[],
					      union meltparam_un *
					      meltxargtab_,
					      const melt_argdescr_cell_t
					      meltxresdescr_[],
					      union meltparam_un *
					      meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 32
    melt_ptr_t mcfr_varptr[32];
#define MELTFRAM_NBVARNUM 11
    long mcfr_varnum[11];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER_st *)
	meltfirstargp_;
      /* use arguments meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 32; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER nbval 32*/
  meltfram__.mcfr_nbvar = 32 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_CHEADER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8463:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8464:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8464:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8464:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8464;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_cheader recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n* ncx=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V7*/ meltfptr[6] =
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8464:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V7*/ meltfptr[6] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8464:/ quasiblock");


      /*_.PROGN___V9*/ meltfptr[7] = /*_._IF___V7*/ meltfptr[6];;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_.PROGN___V9*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8464:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8465:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_CHEADER */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8465:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8465:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check cheader recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8465) ? (8465) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8465:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8466:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8466:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8466:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8466) ? (8466) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8466:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8467:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8467:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8467:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8467) ? (8467) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8467:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8468:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8469:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8470:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.NCX__V4*/ meltfptr[3]),
					(melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */ meltfrout->tabval[3])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 11, "NCTX_MODULCONTEXT");
   /*_.MODCTX__V18*/ meltfptr[17] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MODCTX__V18*/ meltfptr[17] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8471:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.MODCTX__V18*/ meltfptr[17]),
					(melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */ meltfrout->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.MODCTX__V18*/ meltfptr[17]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 8, "MOCX_CHEADERLIST");
   /*_.MCHEADLIST__V19*/ meltfptr[18] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MCHEADLIST__V19*/ meltfptr[18] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8473:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L6*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8473:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L7*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8473:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8473;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_cheader modctx=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODCTX__V18*/ meltfptr[17];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n mcheadlist=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MCHEADLIST__V19*/ meltfptr[18];
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V21*/ meltfptr[20] =
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8473:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L7*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V21*/ meltfptr[20] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8473:/ quasiblock");


      /*_.PROGN___V23*/ meltfptr[21] = /*_._IF___V21*/ meltfptr[20];;
      /*^compute */
      /*_.IFCPP___V20*/ meltfptr[19] = /*_.PROGN___V23*/ meltfptr[21];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8473:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V21*/ meltfptr[20] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V23*/ meltfptr[21] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V20*/ meltfptr[19] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8474:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L8*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.MODCTX__V18*/ meltfptr[17]),
			     (melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */
					    meltfrout->tabval[4])));;
      MELT_LOCATION ("warmelt-normal.melt:8474:/ cond");
      /*cond */ if ( /*_#IS_A__L8*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V25*/ meltfptr[21] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8474:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8474) ? (8474) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V25*/ meltfptr[21] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V24*/ meltfptr[20] = /*_._IFELSE___V25*/ meltfptr[21];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8474:/ clear");
	     /*clear *//*_#IS_A__L8*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V25*/ meltfptr[21] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V24*/ meltfptr[20] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8475:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L9*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.MCHEADLIST__V19*/ meltfptr[18]))
	 == MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-normal.melt:8475:/ cond");
      /*cond */ if ( /*_#IS_LIST__L9*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V27*/ meltfptr[26] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8475:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mcheadlist"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8475) ? (8475) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V27*/ meltfptr[26] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V26*/ meltfptr[21] = /*_._IFELSE___V27*/ meltfptr[26];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8475:/ clear");
	     /*clear *//*_#IS_LIST__L9*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V27*/ meltfptr[26] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V26*/ meltfptr[21] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-normal.melt:8476:/ locexp");
      meltgc_append_list ((melt_ptr_t) ( /*_.MCHEADLIST__V19*/ meltfptr[18]),
			  (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]));
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8477:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L10*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8477:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8477:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[8];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8477;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_cheader updated mcheadlist=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MCHEADLIST__V19*/ meltfptr[18];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n modctx=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODCTX__V18*/ meltfptr[17];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = "\n";
	      /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab,
			    "", (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V29*/ meltfptr[28] =
	      /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8477:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V29*/ meltfptr[28] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8477:/ quasiblock");


      /*_.PROGN___V31*/ meltfptr[29] = /*_._IF___V29*/ meltfptr[28];;
      /*^compute */
      /*_.IFCPP___V28*/ meltfptr[26] = /*_.PROGN___V31*/ meltfptr[29];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8477:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V29*/ meltfptr[28] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V31*/ meltfptr[29] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V28*/ meltfptr[26] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8478:/ quasiblock");


 /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

    {
      MELT_LOCATION ("warmelt-normal.melt:8478:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V32*/ meltfptr[28];;

    MELT_LOCATION ("warmelt-normal.melt:8468:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.MODCTX__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.MCHEADLIST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V24*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V26*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V28*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V32*/ meltfptr[28] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8463:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8463:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_CHEADER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_164_WARMELTmiNORMAL_NORMEXP_CHEADER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT (meltclosure_ptr_t meltclosp_,
						 melt_ptr_t meltfirstargp_,
						 const melt_argdescr_cell_t
						 meltxargdescr_[],
						 union meltparam_un *
						 meltxargtab_,
						 const melt_argdescr_cell_t
						 meltxresdescr_[],
						 union meltparam_un *
						 meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 32
    melt_ptr_t mcfr_varptr[32];
#define MELTFRAM_NBVARNUM 11
    long mcfr_varnum[11];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT_st
	 *) meltfirstargp_;
      /* use arguments meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 32; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT nbval 32*/
  meltfram__.mcfr_nbvar = 32 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_CIMPLEMENT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8484:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8485:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8485:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8485:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8485;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_cimplement recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n* ncx=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V7*/ meltfptr[6] =
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8485:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V7*/ meltfptr[6] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8485:/ quasiblock");


      /*_.PROGN___V9*/ meltfptr[7] = /*_._IF___V7*/ meltfptr[6];;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_.PROGN___V9*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8485:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8486:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_CIMPLEMENT */
					    meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8486:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8486:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check cimplement recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8486) ? (8486) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8486:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8487:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8487:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8487:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8487) ? (8487) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8487:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8488:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8488:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8488:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8488) ? (8488) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8488:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8489:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8490:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8491:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.NCX__V4*/ meltfptr[3]),
					(melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */ meltfrout->tabval[3])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 11, "NCTX_MODULCONTEXT");
   /*_.MODCTX__V18*/ meltfptr[17] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MODCTX__V18*/ meltfptr[17] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8492:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.MODCTX__V18*/ meltfptr[17]),
					(melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */ meltfrout->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.MODCTX__V18*/ meltfptr[17]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 9, "MOCX_CIMPLEMENTLIST");
   /*_.MCIMPLEMLIST__V19*/ meltfptr[18] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MCIMPLEMLIST__V19*/ meltfptr[18] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8494:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L6*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8494:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L7*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8494:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8494;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "normexp_cimplement modctx=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODCTX__V18*/ meltfptr[17];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n mcimplemlist=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MCIMPLEMLIST__V19*/ meltfptr[18];
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V21*/ meltfptr[20] =
	      /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8494:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L7*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V21*/ meltfptr[20] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8494:/ quasiblock");


      /*_.PROGN___V23*/ meltfptr[21] = /*_._IF___V21*/ meltfptr[20];;
      /*^compute */
      /*_.IFCPP___V20*/ meltfptr[19] = /*_.PROGN___V23*/ meltfptr[21];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8494:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V21*/ meltfptr[20] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V23*/ meltfptr[21] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V20*/ meltfptr[19] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8495:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L8*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.MODCTX__V18*/ meltfptr[17]),
			     (melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */
					    meltfrout->tabval[4])));;
      MELT_LOCATION ("warmelt-normal.melt:8495:/ cond");
      /*cond */ if ( /*_#IS_A__L8*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V25*/ meltfptr[21] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8495:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8495) ? (8495) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V25*/ meltfptr[21] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V24*/ meltfptr[20] = /*_._IFELSE___V25*/ meltfptr[21];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8495:/ clear");
	     /*clear *//*_#IS_A__L8*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V25*/ meltfptr[21] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V24*/ meltfptr[20] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8496:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L9*/ meltfnum[0] =
	(melt_magic_discr
	 ((melt_ptr_t) ( /*_.MCIMPLEMLIST__V19*/ meltfptr[18])) ==
	 MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-normal.melt:8496:/ cond");
      /*cond */ if ( /*_#IS_LIST__L9*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V27*/ meltfptr[26] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8496:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mcimplemlist"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8496) ? (8496) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V27*/ meltfptr[26] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V26*/ meltfptr[21] = /*_._IFELSE___V27*/ meltfptr[26];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8496:/ clear");
	     /*clear *//*_#IS_LIST__L9*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V27*/ meltfptr[26] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V26*/ meltfptr[21] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-normal.melt:8497:/ locexp");
      meltgc_append_list ((melt_ptr_t)
			  ( /*_.MCIMPLEMLIST__V19*/ meltfptr[18]),
			  (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]));
    }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8498:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L10*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8498:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L10*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L11*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8498:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[8];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8498;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_cimplement updated mcimplemlist=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MCIMPLEMLIST__V19*/ meltfptr[18];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n modctx=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODCTX__V18*/ meltfptr[17];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = "\n";
	      /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab,
			    "", (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V29*/ meltfptr[28] =
	      /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8498:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L11*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V29*/ meltfptr[28] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8498:/ quasiblock");


      /*_.PROGN___V31*/ meltfptr[29] = /*_._IF___V29*/ meltfptr[28];;
      /*^compute */
      /*_.IFCPP___V28*/ meltfptr[26] = /*_.PROGN___V31*/ meltfptr[29];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8498:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L10*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V29*/ meltfptr[28] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V31*/ meltfptr[29] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V28*/ meltfptr[26] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8499:/ quasiblock");


 /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

    {
      MELT_LOCATION ("warmelt-normal.melt:8499:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V32*/ meltfptr[28];;

    MELT_LOCATION ("warmelt-normal.melt:8489:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.MODCTX__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.MCIMPLEMLIST__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V24*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V26*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V28*/ meltfptr[26] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V32*/ meltfptr[28] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8484:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8484:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_CIMPLEMENT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_165_WARMELTmiNORMAL_NORMEXP_CIMPLEMENT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG_melt
    = melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
  static long melt_call_counter__;
  long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

  struct meltframe_meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG_st
  {
    int mcfr_nbvar;
#if MELT_HAVE_DEBUG
    const char *mcfr_flocs;
#else				/*!MELT_HAVE_DEBUG */
    const char *mcfr_unusedflocs;
#endif				/*MELT_HAVE_DEBUG */
    struct meltclosure_st *mcfr_clos;
    struct excepth_melt_st *mcfr_exh;
    struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 46
    melt_ptr_t mcfr_varptr[46];
#define MELTFRAM_NBVARNUM 18
    long mcfr_varnum[18];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG_st
	 *) meltfirstargp_;
      /* use arguments meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 46; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG nbval 46*/
  meltfram__.mcfr_nbvar = 46 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-normal.melt:8507:/ getarg");
 /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.ENV__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2])) != NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.NCX__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3])) != NULL);


  /*getarg#3 */
  /*^getarg */
  if (meltxargdescr_[2] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.PSLOC__V5*/ meltfptr[4] =
    (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.PSLOC__V5*/ meltfptr[4])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8508:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8508:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8508:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8508;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_use_package_from_pkg_config recv=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RECV__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = "\n* ncx=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.NCX__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V7*/ meltfptr[6] =
	      /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8508:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V7*/ meltfptr[6] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8508:/ quasiblock");


      /*_.PROGN___V9*/ meltfptr[7] = /*_._IF___V7*/ meltfptr[6];;
      /*^compute */
      /*_.IFCPP___V6*/ meltfptr[5] = /*_.PROGN___V9*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8508:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V6*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8509:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L3*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_SOURCE_USE_PACKAGE_FROM_PKG_CONFIG */ meltfrout->tabval[1])));;
      MELT_LOCATION ("warmelt-normal.melt:8509:/ cond");
      /*cond */ if ( /*_#IS_A__L3*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V11*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8509:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check cheader recv"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8509) ? (8509) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V10*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8509:/ clear");
	     /*clear *//*_#IS_A__L3*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V11*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V10*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8510:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L4*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.ENV__V3*/ meltfptr[2]),
			     (melt_ptr_t) (( /*!CLASS_ENVIRONMENT */
					    meltfrout->tabval[2])));;
      MELT_LOCATION ("warmelt-normal.melt:8510:/ cond");
      /*cond */ if ( /*_#IS_A__L4*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8510:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check env"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8510) ? (8510) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V12*/ meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8510:/ clear");
	     /*clear *//*_#IS_A__L4*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V12*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8511:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L5*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]),
			     (melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */
					    meltfrout->tabval[3])));;
      MELT_LOCATION ("warmelt-normal.melt:8511:/ cond");
      /*cond */ if ( /*_#IS_A__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V15*/ meltfptr[14] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8511:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check nctxt"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8511) ? (8511) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8511:/ clear");
	     /*clear *//*_#IS_A__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V14*/ meltfptr[12] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-normal.melt:8512:/ quasiblock");


    MELT_LOCATION ("warmelt-normal.melt:8513:/ getslot");
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
      melt_object_get_field (slot, obj, 1, "LOCA_LOCATION");
  /*_.SLOC__V17*/ meltfptr[16] = slot;
    };
    ;
    MELT_LOCATION ("warmelt-normal.melt:8514:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.RECV__V2*/ meltfptr[1]),
					(melt_ptr_t) (( /*!CLASS_SOURCE_USE_PACKAGE_FROM_PKG_CONFIG */ meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.RECV__V2*/ meltfptr[1]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 2, "SUSEPACKAGE_PKGTUPLE");
   /*_.PKGTUP__V18*/ meltfptr[17] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.PKGTUP__V18*/ meltfptr[17] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8515:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.NCX__V4*/ meltfptr[3]),
					(melt_ptr_t) (( /*!CLASS_NORMALIZATION_CONTEXT */ meltfrout->tabval[3])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.NCX__V4*/ meltfptr[3]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 11, "NCTX_MODULCONTEXT");
   /*_.MODCTX__V19*/ meltfptr[18] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MODCTX__V19*/ meltfptr[18] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-normal.melt:8516:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.MODCTX__V19*/ meltfptr[18]),
					(melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */ meltfrout->tabval[4])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.MODCTX__V19*/ meltfptr[18]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 12, "MOCX_PACKAGEPCLIST");
   /*_.MCPACKAGELIST__V20*/ meltfptr[19] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.MCPACKAGELIST__V20*/ meltfptr[19] = NULL;;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8518:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L6*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8518:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L6*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L7*/ meltfnum[1] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8518:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8518;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_use_package_from_pkg_config initial mcpackagelist=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MCPACKAGELIST__V20*/ meltfptr[19];
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V22*/ meltfptr[21] =
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8518:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L7*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V22*/ meltfptr[21] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8518:/ quasiblock");


      /*_.PROGN___V24*/ meltfptr[22] = /*_._IF___V22*/ meltfptr[21];;
      /*^compute */
      /*_.IFCPP___V21*/ meltfptr[20] = /*_.PROGN___V24*/ meltfptr[22];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8518:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L6*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V22*/ meltfptr[21] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V24*/ meltfptr[22] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V21*/ meltfptr[20] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8519:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L8*/ meltfnum[1] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.MODCTX__V19*/ meltfptr[18]),
			     (melt_ptr_t) (( /*!CLASS_ANY_MODULE_CONTEXT */
					    meltfrout->tabval[4])));;
      MELT_LOCATION ("warmelt-normal.melt:8519:/ cond");
      /*cond */ if ( /*_#IS_A__L8*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V26*/ meltfptr[22] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8519:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check modctx"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8519) ? (8519) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V26*/ meltfptr[22] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V25*/ meltfptr[21] = /*_._IFELSE___V26*/ meltfptr[22];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8519:/ clear");
	     /*clear *//*_#IS_A__L8*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V26*/ meltfptr[22] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V25*/ meltfptr[21] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8520:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L9*/ meltfnum[0] =
	(melt_magic_discr
	 ((melt_ptr_t) ( /*_.MCPACKAGELIST__V20*/ meltfptr[19])) ==
	 MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-normal.melt:8520:/ cond");
      /*cond */ if ( /*_#IS_LIST__L9*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V28*/ meltfptr[27] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-normal.melt:8520:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check mcpackagelist"),
				  ("warmelt-normal.melt")
				  ? ("warmelt-normal.melt") : __FILE__,
				  (8520) ? (8520) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V28*/ meltfptr[27] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V27*/ meltfptr[22] = /*_._IFELSE___V28*/ meltfptr[27];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8520:/ clear");
	     /*clear *//*_#IS_LIST__L9*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V28*/ meltfptr[27] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V27*/ meltfptr[22] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    /*citerblock FOREACH_IN_MULTIPLE */
    {
      /* start foreach_in_multiple meltcit1__EACHTUP */
      long meltcit1__EACHTUP_ln =
	melt_multiple_length ((melt_ptr_t) /*_.PKGTUP__V18*/ meltfptr[17]);
      for ( /*_#PKGIX__L10*/ meltfnum[1] = 0;
	   ( /*_#PKGIX__L10*/ meltfnum[1] >= 0)
	   && ( /*_#PKGIX__L10*/ meltfnum[1] < meltcit1__EACHTUP_ln);
	/*_#PKGIX__L10*/ meltfnum[1]++)
	{
	  /*_.CURPKGNAME__V29*/ meltfptr[27] =
	    melt_multiple_nth ((melt_ptr_t) ( /*_.PKGTUP__V18*/ meltfptr[17]),
			       /*_#PKGIX__L10*/ meltfnum[1]);




#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-normal.melt:8525:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    {
	      /*^locexp */
	      /*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	      melt_dbgcounter++;
#endif
	      ;
	    }
	    ;

	    MELT_CHECK_SIGNAL ();
	    ;
    /*_#MELT_NEED_DBG__L11*/ meltfnum[0] =
	      /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	      ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	      0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	      ;;
	    MELT_LOCATION ("warmelt-normal.melt:8525:/ cond");
	    /*cond */ if ( /*_#MELT_NEED_DBG__L11*/ meltfnum[0])	/*then */
	      {
		/*^cond.then */
		/*^block */
		/*anyblock */
		{

      /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
		    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
		    meltcallcount	/* melt_callcount debugging */
#else
		    0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
		    ;;

		  MELT_CHECK_SIGNAL ();
		  ;
		  MELT_LOCATION ("warmelt-normal.melt:8525:/ apply");
		  /*apply */
		  {
		    union meltparam_un argtab[5];
		    memset (&argtab, 0, sizeof (argtab));
		    /*^apply.arg */
		    argtab[0].meltbp_long =
		      /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
		    /*^apply.arg */
		    argtab[1].meltbp_cstring = "warmelt-normal.melt";
		    /*^apply.arg */
		    argtab[2].meltbp_long = 8525;
		    /*^apply.arg */
		    argtab[3].meltbp_cstring =
		      "normexp_use_package_from_pkg_config curpkgname=";
		    /*^apply.arg */
		    argtab[4].meltbp_aptr =
		      (melt_ptr_t *) & /*_.CURPKGNAME__V29*/ meltfptr[27];
		    /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31] =
		      melt_apply ((meltclosure_ptr_t)
				  (( /*!MELT_DEBUG_FUN */ meltfrout->
				    tabval[0])),
				  (melt_ptr_t) (( /*nil */ NULL)),
				  (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_LONG MELTBPARSTR_CSTRING
				   MELTBPARSTR_PTR ""), argtab, "",
				  (union meltparam_un *) 0);
		  }
		  ;
		  /*_._IF___V31*/ meltfptr[30] =
		    /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-normal.melt:8525:/ clear");
		/*clear *//*_#MELT_CALLCOUNT__L12*/ meltfnum[11] = 0;
		  /*^clear */
		/*clear *//*_.MELT_DEBUG_FUN__V32*/ meltfptr[31] = 0;
		}
		;
	      }
	    else
	      {			/*^cond.else */

     /*_._IF___V31*/ meltfptr[30] = NULL;;
	      }
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8525:/ quasiblock");


	    /*_.PROGN___V33*/ meltfptr[31] = /*_._IF___V31*/ meltfptr[30];;
	    /*^compute */
	    /*_.IFCPP___V30*/ meltfptr[29] = /*_.PROGN___V33*/ meltfptr[31];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8525:/ clear");
	      /*clear *//*_#MELT_NEED_DBG__L11*/ meltfnum[0] = 0;
	    /*^clear */
	      /*clear *//*_._IF___V31*/ meltfptr[30] = 0;
	    /*^clear */
	      /*clear *//*_.PROGN___V33*/ meltfptr[31] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V30*/ meltfptr[29] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;

#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-normal.melt:8526:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
    /*_#IS_STRING__L13*/ meltfnum[11] =
	      (melt_magic_discr
	       ((melt_ptr_t) ( /*_.CURPKGNAME__V29*/ meltfptr[27])) ==
	       MELTOBMAG_STRING);;
	    MELT_LOCATION ("warmelt-normal.melt:8526:/ cond");
	    /*cond */ if ( /*_#IS_STRING__L13*/ meltfnum[11])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V35*/ meltfptr[31] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-normal.melt:8526:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("check curpkgname"),
					("warmelt-normal.melt")
					? ("warmelt-normal.melt") : __FILE__,
					(8526) ? (8526) : __LINE__,
					__FUNCTION__);
		    ;
		  }
		  ;
		/*clear *//*_._IFELSE___V35*/ meltfptr[31] = 0;
		  /*epilog */
		}
		;
	      }
	    ;
	    /*^compute */
	    /*_.IFCPP___V34*/ meltfptr[30] =
	      /*_._IFELSE___V35*/ meltfptr[31];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8526:/ clear");
	      /*clear *//*_#IS_STRING__L13*/ meltfnum[11] = 0;
	    /*^clear */
	      /*clear *//*_._IFELSE___V35*/ meltfptr[31] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V34*/ meltfptr[30] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8527:/ quasiblock");


  /*_#FOUND__L14*/ meltfnum[0] = 0;;
	  /*citerblock FOREACH_IN_LIST */
	  {
	    /* start foreach_in_list meltcit2__EACHLIST */
	    for ( /*_.CURPAIR__V37*/ meltfptr[36] =
		 melt_list_first ((melt_ptr_t) /*_.MCPACKAGELIST__V20*/
				  meltfptr[19]);
		 melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V37*/
				   meltfptr[36]) == MELTOBMAG_PAIR;
		 /*_.CURPAIR__V37*/ meltfptr[36] =
		 melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V37*/
				 meltfptr[36]))
	      {
		/*_.OLDPKGNAME__V38*/ meltfptr[37] =
		  melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V37*/
				  meltfptr[36]);



		MELT_CHECK_SIGNAL ();
		;
   /*_#eqeqS__L15*/ meltfnum[11] =
		  melt_string_same ((melt_ptr_t)
				    ( /*_.OLDPKGNAME__V38*/ meltfptr[37]),
				    (melt_ptr_t) ( /*_.CURPKGNAME__V29*/
						  meltfptr[27]));;
		MELT_LOCATION ("warmelt-normal.melt:8532:/ cond");
		/*cond */ if ( /*_#eqeqS__L15*/ meltfnum[11])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      MELT_LOCATION ("warmelt-normal.melt:8533:/ compute");
		      /*_#FOUND__L14*/ meltfnum[0] =
			/*_#SETQ___L16*/ meltfnum[15] = 1;;
		      MELT_LOCATION ("warmelt-normal.melt:8534:/ compute");
		      /*_.CURPAIR__V37*/ meltfptr[36] =
			/*_.SETQ___V40*/ meltfptr[39] = ( /*nil */ NULL);;
		      MELT_LOCATION ("warmelt-normal.melt:8532:/ quasiblock");


		      /*_.PROGN___V41*/ meltfptr[40] =
			/*_.SETQ___V40*/ meltfptr[39];;
		      /*^compute */
		      /*_._IF___V39*/ meltfptr[38] =
			/*_.PROGN___V41*/ meltfptr[40];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-normal.melt:8532:/ clear");
	       /*clear *//*_#SETQ___L16*/ meltfnum[15] = 0;
		      /*^clear */
	       /*clear *//*_.SETQ___V40*/ meltfptr[39] = 0;
		      /*^clear */
	       /*clear *//*_.PROGN___V41*/ meltfptr[40] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

    /*_._IF___V39*/ meltfptr[38] = NULL;;
		  }
		;
	      }			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.CURPAIR__V37*/ meltfptr[36] = NULL;
     /*_.OLDPKGNAME__V38*/ meltfptr[37] = NULL;


	    /*citerepilog */

	    MELT_LOCATION ("warmelt-normal.melt:8529:/ clear");
	     /*clear *//*_.CURPAIR__V37*/ meltfptr[36] = 0;
	    /*^clear */
	     /*clear *//*_.OLDPKGNAME__V38*/ meltfptr[37] = 0;
	    /*^clear */
	     /*clear *//*_#eqeqS__L15*/ meltfnum[11] = 0;
	    /*^clear */
	     /*clear *//*_._IF___V39*/ meltfptr[38] = 0;
	  }			/*endciterblock FOREACH_IN_LIST */
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-normal.melt:8535:/ cond");
	  /*cond */ if ( /*_#FOUND__L14*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*_._IFELSE___V42*/ meltfptr[39] = ( /*nil */ NULL);;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-normal.melt:8535:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-normal.melt:8536:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.MCPACKAGELIST__V20*/
				       meltfptr[19]),
				      (melt_ptr_t) ( /*_.CURPKGNAME__V29*/
						    meltfptr[27]));
		}
		;
		MELT_LOCATION ("warmelt-normal.melt:8535:/ quasiblock");


		/*epilog */
	      }
	      ;
	    }
	  ;
	  /*_.LET___V36*/ meltfptr[31] = /*_._IFELSE___V42*/ meltfptr[39];;

	  MELT_LOCATION ("warmelt-normal.melt:8527:/ clear");
	    /*clear *//*_#FOUND__L14*/ meltfnum[0] = 0;
	  /*^clear */
	    /*clear *//*_._IFELSE___V42*/ meltfptr[39] = 0;
	  if ( /*_#PKGIX__L10*/ meltfnum[1] < 0)
	    break;
	}			/* end  foreach_in_multiple meltcit1__EACHTUP */

      /*citerepilog */

      MELT_LOCATION ("warmelt-normal.melt:8522:/ clear");
	    /*clear *//*_.CURPKGNAME__V29*/ meltfptr[27] = 0;
      /*^clear */
	    /*clear *//*_#PKGIX__L10*/ meltfnum[1] = 0;
      /*^clear */
	    /*clear *//*_.IFCPP___V30*/ meltfptr[29] = 0;
      /*^clear */
	    /*clear *//*_.IFCPP___V34*/ meltfptr[30] = 0;
      /*^clear */
	    /*clear *//*_.LET___V36*/ meltfptr[31] = 0;
    }				/*endciterblock FOREACH_IN_MULTIPLE */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-normal.melt:8540:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*melt_increment_dbgcounter */
#if MELT_HAVE_DEBUG
	melt_dbgcounter++;
#endif
	;
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;
   /*_#MELT_NEED_DBG__L17*/ meltfnum[15] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-normal.melt:8540:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L17*/ meltfnum[15])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L18*/ meltfnum[0] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-normal.melt:8540:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[0];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-normal.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 8540;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"normexp_use_package_from_pkg_config final mcpackagelist=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.MCPACKAGELIST__V20*/ meltfptr[19];
	      /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V44*/ meltfptr[39] =
	      /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-normal.melt:8540:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L18*/ meltfnum[0] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V45*/ meltfptr[44] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V44*/ meltfptr[39] = NULL;;
	}
      ;
      MELT_LOCATION ("warmelt-normal.melt:8540:/ quasiblock");


      /*_.PROGN___V46*/ meltfptr[44] = /*_._IF___V44*/ meltfptr[39];;
      /*^compute */
      /*_.IFCPP___V43*/ meltfptr[40] = /*_.PROGN___V46*/ meltfptr[44];;
      /*epilog */

      MELT_LOCATION ("warmelt-normal.melt:8540:/ clear");
	     /*clear *//*_#MELT_NEED_DBG__L17*/ meltfnum[15] = 0;
      /*^clear */
	     /*clear *//*_._IF___V44*/ meltfptr[39] = 0;
      /*^clear */
	     /*clear *//*_.PROGN___V46*/ meltfptr[44] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V43*/ meltfptr[40] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    /*^compute */
    /*_.LET___V16*/ meltfptr[14] = /*_.IFCPP___V43*/ meltfptr[40];;

    MELT_LOCATION ("warmelt-normal.melt:8512:/ clear");
	   /*clear *//*_.SLOC__V17*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.PKGTUP__V18*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.MODCTX__V19*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.MCPACKAGELIST__V20*/ meltfptr[19] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V21*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V25*/ meltfptr[21] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V27*/ meltfptr[22] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V43*/ meltfptr[40] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-normal.melt:8507:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

    {
      MELT_LOCATION ("warmelt-normal.melt:8507:/ locexp");
      /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
      if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
	melt_warn_for_no_expected_secondary_results ();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
      ;
    }
    ;
    /*^finalreturn */
    ;
    /*finalret */ goto meltlabend_rout;
    /*epilog */

    /*^clear */
	   /*clear *//*_.IFCPP___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V10*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V12*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V14*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_166_WARMELTmiNORMAL_NORMEXP_USE_PACKAGE_FROM_PKG_CONFIG */



/**** end of warmelt-normal+06.c ****/
