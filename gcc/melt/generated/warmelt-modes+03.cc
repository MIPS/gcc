/* GCC MELT GENERATED FILE warmelt-modes+03.c - DO NOT EDIT */
/* secondary MELT generated C file of rank #3 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f3[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-modes+03.c declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-modes ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright (C) 2011 - 2013 Free Software Foundation, Inc.
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


/** ordinary MELT module meltbuild-sources/warmelt-modes**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1	/*usualmodule */


/**** no MELT module variables ****/

/*** 4 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t
melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdata,
						 melt_ptr_t meltinp0_PREVENV,
						 const char
						 *meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_POLL_INPUTS **/
MELT_EXTERN void melthook_HOOK_POLL_INPUTS (melt_ptr_t melthookdata,
					    long meltinp0_DELAYMS);

/*declare MELT called hook #2 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdata,
						      const char
						      *meltinp0_SYMNAMESTR,
						      const char
						      *meltinp1_MODULENAMESTR,
						      melt_ptr_t
						      meltinp2_PARENV);

/*declare MELT called hook #3 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdata,
					       melt_ptr_t meltinp0_SYM,
					       melt_ptr_t meltinp1_VAL,
					       melt_ptr_t meltinp2_CONTENV);

/*** end of 4 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_1_WARMELTmiMODES_INSTALL_MELT_MODE (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_2_WARMELTmiMODES_RUNFILE_DOCMD (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_WARMELTmiMODES_RUNDEBUG_DOCMD (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_4_WARMELTmiMODES_EVAL_DOCMD (meltclosure_ptr_t meltclosp_,
				      melt_ptr_t meltfirstargp_,
				      const melt_argdescr_cell_t
				      meltxargdescr_[],
				      union meltparam_un *meltxargtab_,
				      const melt_argdescr_cell_t
				      meltxresdescr_[],
				      union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_5_WARMELTmiMODES_EVALFILE_DOCMD (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_6_WARMELTmiMODES_REPL_PROCESSOR (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_7_WARMELTmiMODES_REPL_DOCMD (meltclosure_ptr_t meltclosp_,
				      melt_ptr_t meltfirstargp_,
				      const melt_argdescr_cell_t
				      meltxargdescr_[],
				      union meltparam_un *meltxargtab_,
				      const melt_argdescr_cell_t
				      meltxresdescr_[],
				      union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_8_WARMELTmiMODES_LAMBDA_cl1 (meltclosure_ptr_t meltclosp_,
				      melt_ptr_t meltfirstargp_,
				      const melt_argdescr_cell_t
				      meltxargdescr_[],
				      union meltparam_un *meltxargtab_,
				      const melt_argdescr_cell_t
				      meltxresdescr_[],
				      union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiMODES_LAMBDA_cl2 (meltclosure_ptr_t meltclosp_,
				      melt_ptr_t meltfirstargp_,
				      const melt_argdescr_cell_t
				      meltxargdescr_[],
				      union meltparam_un *meltxargtab_,
				      const melt_argdescr_cell_t
				      meltxresdescr_[],
				      union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiMODES_LAMBDA_cl3 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiMODES_LAMBDA_cl4 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiMODES_LAMBDA_cl5 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiMODES_INCREMENT_MKDOC_COUNTER (meltclosure_ptr_t
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
meltrout_14_WARMELTmiMODES_MAKEDOC_SCANINPUT (meltclosure_ptr_t meltclosp_,
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
meltrout_15_WARMELTmiMODES_LAMBDA_cl6 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiMODES_LAMBDA_cl7 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiMODES_LAMBDA_cl8 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiMODES_LAMBDA_cl9 (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiMODES_MAKEDOC_OUTDEFLOC (meltclosure_ptr_t meltclosp_,
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
meltrout_20_WARMELTmiMODES_MAKEDOC_OUTFORMALS (meltclosure_ptr_t meltclosp_,
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
meltrout_21_WARMELTmiMODES_MAKEDOC_OUTDOC (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiMODES_MAKEDOC_OUTCLASSDEF (meltclosure_ptr_t meltclosp_,
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
meltrout_23_WARMELTmiMODES_MAKEDOC_GENMACRO (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiMODES_LAMBDA_cl10 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiMODES_MAKEDOC_GENPATMACRO (meltclosure_ptr_t meltclosp_,
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
meltrout_26_WARMELTmiMODES_LAMBDA_cl11 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiMODES_MAKEDOC_GENCLASS (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiMODES_LAMBDA_cl12 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiMODES_LAMBDA_cl13 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_WARMELTmiMODES_LAMBDA_cl14 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_31_WARMELTmiMODES_LAMBDA_cl15 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiMODES_MAKEDOC_OUTPRIMITIVEDEF (meltclosure_ptr_t
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
meltrout_33_WARMELTmiMODES_MAKEDOC_GENPRIMITIVE (meltclosure_ptr_t meltclosp_,
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
meltrout_34_WARMELTmiMODES_LAMBDA_cl16 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_35_WARMELTmiMODES_MAKEDOC_OUTFUNCTIONDEF (meltclosure_ptr_t
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
meltrout_36_WARMELTmiMODES_MAKEDOC_GENFUNCTION (meltclosure_ptr_t meltclosp_,
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
meltrout_37_WARMELTmiMODES_LAMBDA_cl17 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_WARMELTmiMODES_MAKEDOC_GENCITERATOR (meltclosure_ptr_t meltclosp_,
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
meltrout_39_WARMELTmiMODES_LAMBDA_cl18 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_40_WARMELTmiMODES_MAKEDOC_GENCMATCHER (meltclosure_ptr_t meltclosp_,
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
meltrout_41_WARMELTmiMODES_LAMBDA_cl19 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_42_WARMELTmiMODES_MAKEDOC_GENHOOK (meltclosure_ptr_t meltclosp_,
					    melt_ptr_t meltfirstargp_,
					    const melt_argdescr_cell_t
					    meltxargdescr_[],
					    union meltparam_un *meltxargtab_,
					    const melt_argdescr_cell_t
					    meltxresdescr_[],
					    union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_43_WARMELTmiMODES_LAMBDA_cl20 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_44_WARMELTmiMODES_MAKEDOC_GENOUTPUT (meltclosure_ptr_t meltclosp_,
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
meltrout_45_WARMELTmiMODES_MAKEDOC_DOCMD (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_46_WARMELTmiMODES_LAMBDA_cl21 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_47_WARMELTmiMODES_SHOWVAR_DOCMD (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_48_WARMELTmiMODES_HELP_DOCMD (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_49_WARMELTmiMODES_NOP_DOCMD (meltclosure_ptr_t meltclosp_,
				      melt_ptr_t meltfirstargp_,
				      const melt_argdescr_cell_t
				      meltxargdescr_[],
				      union meltparam_un *meltxargtab_,
				      const melt_argdescr_cell_t
				      meltxresdescr_[],
				      union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_50_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_ENUM_OBJMAGIC
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_51_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_GTY (meltclosure_ptr_t
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
meltrout_52_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_FORWCOPY_FUN
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_53_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_CLONING_FUN
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_54_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_SCANNING (meltclosure_ptr_t
							     meltclosp_,
							     melt_ptr_t
							     meltfirstargp_,
							     const
							     melt_argdescr_cell_t
							     meltxargdescr_[],
							     union
							     meltparam_un
							     *meltxargtab_,
							     const
							     melt_argdescr_cell_t
							     meltxresdescr_[],
							     union
							     meltparam_un
							     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_55_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_PARAM (meltclosure_ptr_t
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
meltrout_56_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_COD2CTYPE
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_57_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_MAG2STR (meltclosure_ptr_t
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
meltrout_58_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_BOXINGFUN
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_59_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_MAPFUN (meltclosure_ptr_t
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
meltrout_60_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_PREDEF_FIELDS
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_61_WARMELTmiMODES_GENERATE_RUNTYPESUPPORT_PREDEFINED_HOOKS
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiMODES_RUNTYPESUPPORT_DOCMD (meltclosure_ptr_t meltclosp_,
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
meltrout_63_WARMELTmiMODES_TRANSLATE_TO_FLAVORED_MODULE (meltclosure_ptr_t
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
meltrout_64_WARMELTmiMODES_TRANSLATETOMODULE_DOCMD (meltclosure_ptr_t
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
meltrout_65_WARMELTmiMODES_TRANSLATEDEBUG_DOCMD (meltclosure_ptr_t meltclosp_,
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
meltrout_66_WARMELTmiMODES_TRANSLATEQUICKLY_DOCMD (meltclosure_ptr_t
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
meltrout_67_WARMELTmiMODES_TRANSLATEOPTIMIZED_DOCMD (meltclosure_ptr_t
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
meltrout_68_WARMELTmiMODES_TRANSLATEFILE_DOCMD (meltclosure_ptr_t meltclosp_,
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
meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD (meltclosure_ptr_t meltclosp_,
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
meltrout_70_WARMELTmiMODES_LAMBDA_cl22 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED (meltclosure_ptr_t
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
meltrout_72_WARMELTmiMODES_LAMBDA_cl23 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD (meltclosure_ptr_t
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
meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);



MELT_EXTERN void *melt_start_this_module (void *);
struct melt_initial_frame_st;

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiMODES__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMODES__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_WARMELTmiMODES__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMODES__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_4 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_5 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_6 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_7 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_8 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_9 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_10 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__initialmeltchunk_11 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiMODES__forward_or_mark_module_start_frame (struct
							     melt_callframe_st
							     *fp,
							     int marking);



/**** warmelt-modes+03.c implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD_melt
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

  struct meltframe_meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD_st
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
#define MELTFRAM_NBVARPTR 37
    melt_ptr_t mcfr_varptr[37];
#define MELTFRAM_NBVARNUM 17
    long mcfr_varnum[17];
/*others*/
    const char *loc_CSTRING__o0;
    const char *loc_CSTRING__o1;
    const char *loc_CSTRING__o2;
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD_st *)
	meltfirstargp_;
      /* use arguments meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 37; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD nbval 37*/
  meltfram__.mcfr_nbvar = 37 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("TRANSLATEINIT_DOCMD", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4373:/ getarg");
 /*_.CMD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODULDATA__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODULDATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4374:/ cppif.then");
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
      MELT_LOCATION ("warmelt-modes.melt:4374:/ cond");
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
	    MELT_LOCATION ("warmelt-modes.melt:4374:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4374;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "start translateinit_mode cmd=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CMD__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " moduldata=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODULDATA__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " initial_environment=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & ( /*!INITIAL_ENVIRONMENT */ meltfrout->
				  tabval[1]);
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
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
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4374:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4374:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-modes.melt:4376:/ quasiblock");


 /*_.RLIST__V7*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[2]))));;
    /*^compute */
 /*_?*/ meltfram__.loc_CSTRING__o0 =
      melt_argument ("arg");;
    /*^compute */
 /*_?*/ meltfram__.loc_CSTRING__o1 =
      melt_argument ("arglist");;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4380:/ cond");
    /*cond */ if ( /*_?*/ meltfram__.loc_CSTRING__o0)	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-modes.melt:4381:/ quasiblock");


   /*_.PROGARGSTR__V10*/ meltfptr[9] =
	    (meltgc_new_stringdup
	     ((meltobject_ptr_t) (( /*!DISCR_STRING */ meltfrout->tabval[3])),
	      ( /*_?*/ meltfram__.loc_CSTRING__o0)));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
   /*_#STRING_SUFFIXED__L3*/ meltfnum[1] =
	    /*string_suffixed: */
	    (melt_string_is_ending
	     ((melt_ptr_t) /*_.PROGARGSTR__V10*/ meltfptr[9], ".melt"));;
	  /*^compute */
   /*_#NOT__L4*/ meltfnum[0] =
	    (!( /*_#STRING_SUFFIXED__L3*/ meltfnum[1]));;
	  MELT_LOCATION ("warmelt-modes.melt:4383:/ cond");
	  /*cond */ if ( /*_#NOT__L4*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-modes.melt:4384:/ locexp");
		  warning (0, "MELT WARNING MSG [#%ld]::: %s - %s",
			   melt_dbgcounter,
			   ("MELT translated initial file without .melt suffix"),
			   melt_string_str ((melt_ptr_t)
					    ( /*_.PROGARGSTR__V10*/
					     meltfptr[9])));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*_.LET___V9*/ meltfptr[8] = /*_.PROGARGSTR__V10*/ meltfptr[9];;

	  MELT_LOCATION ("warmelt-modes.melt:4381:/ clear");
	     /*clear *//*_.PROGARGSTR__V10*/ meltfptr[9] = 0;
	  /*^clear */
	     /*clear *//*_#STRING_SUFFIXED__L3*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_#NOT__L4*/ meltfnum[0] = 0;
	  /*_.INARG__V8*/ meltfptr[7] = /*_.LET___V9*/ meltfptr[8];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4380:/ clear");
	     /*clear *//*_.LET___V9*/ meltfptr[8] = 0;
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
	  MELT_LOCATION ("warmelt-modes.melt:4387:/ cond");
	  /*cond */ if ( /*_?*/ meltfram__.loc_CSTRING__o1)	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_STRINGCONST__V12*/ meltfptr[8] =
		  (meltgc_new_stringdup
		   ((meltobject_ptr_t)
		    (( /*!DISCR_STRING */ meltfrout->tabval[3])),
		    ( /*_?*/ meltfram__.loc_CSTRING__o1)));;
		/*^compute */
     /*_.SPLIT_STRING_COMMA__V13*/ meltfptr[12] =
		  meltgc_new_split_string (melt_string_str
					   ((melt_ptr_t)
					    /*_.MAKE_STRINGCONST__V12*/
					    meltfptr[8]), ',',
					   (melt_ptr_t) ( /*!DISCR_STRING */
							 meltfrout->
							 tabval[3]));;
		/*^compute */
		/*_._IFELSE___V11*/ meltfptr[9] =
		  /*_.SPLIT_STRING_COMMA__V13*/ meltfptr[12];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4387:/ clear");
	       /*clear *//*_.MAKE_STRINGCONST__V12*/ meltfptr[8] = 0;
		/*^clear */
	       /*clear *//*_.SPLIT_STRING_COMMA__V13*/ meltfptr[12] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-modes.melt:4391:/ locexp");
		  error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
			 ("invalid arg or arglist to translateinit mode"));
		}
		;
		MELT_LOCATION ("warmelt-modes.melt:4392:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4392:/ locexp");
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
		MELT_LOCATION ("warmelt-modes.melt:4390:/ quasiblock");


		/*_.PROGN___V15*/ meltfptr[12] =
		  /*_.RETURN___V14*/ meltfptr[8];;
		/*^compute */
		/*_._IFELSE___V11*/ meltfptr[9] =
		  /*_.PROGN___V15*/ meltfptr[12];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4387:/ clear");
	       /*clear *//*_.RETURN___V14*/ meltfptr[8] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V15*/ meltfptr[12] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.INARG__V8*/ meltfptr[7] = /*_._IFELSE___V11*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4380:/ clear");
	     /*clear *//*_._IFELSE___V11*/ meltfptr[9] = 0;
	}
	;
      }
    ;
 /*_?*/ meltfram__.loc_CSTRING__o2 =
      melt_argument ("output");;
    /*^compute */
 /*_.OUTARG__V16*/ meltfptr[8] =
      (meltgc_new_stringdup
       ((meltobject_ptr_t) (( /*!DISCR_STRING */ meltfrout->tabval[3])),
	( /*_?*/ meltfram__.loc_CSTRING__o2)));;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_STRING__L5*/ meltfnum[1] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.OUTARG__V16*/ meltfptr[8])) ==
       MELTOBMAG_STRING);;
    MELT_LOCATION ("warmelt-modes.melt:4395:/ cond");
    /*cond */ if ( /*_#IS_STRING__L5*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*_.BASNAM__V17*/ meltfptr[12] = /*_.OUTARG__V16*/ meltfptr[8];;
      }
    else
      {
	MELT_LOCATION ("warmelt-modes.melt:4395:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_STRING__L6*/ meltfnum[0] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.INARG__V8*/ meltfptr[7])) ==
	     MELTOBMAG_STRING);;
	  MELT_LOCATION ("warmelt-modes.melt:4396:/ cond");
	  /*cond */ if ( /*_#IS_STRING__L6*/ meltfnum[0])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_STRING_NAKEDBASENAME__V19*/ meltfptr[18] =
		  (meltgc_new_string_nakedbasename
		   ((meltobject_ptr_t)
		    (( /*!DISCR_STRING */ meltfrout->tabval[3])),
		    melt_string_str ((melt_ptr_t)
				     ( /*_.INARG__V8*/ meltfptr[7]))));;
		/*^compute */
		/*_._IFELSE___V18*/ meltfptr[9] =
		  /*_.MAKE_STRING_NAKEDBASENAME__V19*/ meltfptr[18];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4396:/ clear");
	       /*clear *//*_.MAKE_STRING_NAKEDBASENAME__V19*/ meltfptr[18] =
		  0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-modes.melt:4398:/ locexp");
		  error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
			 ("invalid translateinit mode"));
		}
		;
		MELT_LOCATION ("warmelt-modes.melt:4399:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4399:/ locexp");
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
		MELT_LOCATION ("warmelt-modes.melt:4397:/ quasiblock");


		/*_.PROGN___V21*/ meltfptr[20] =
		  /*_.RETURN___V20*/ meltfptr[18];;
		/*^compute */
		/*_._IFELSE___V18*/ meltfptr[9] =
		  /*_.PROGN___V21*/ meltfptr[20];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4396:/ clear");
	       /*clear *//*_.RETURN___V20*/ meltfptr[18] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V21*/ meltfptr[20] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.BASNAM__V17*/ meltfptr[12] = /*_._IFELSE___V18*/ meltfptr[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4395:/ clear");
	     /*clear *//*_#IS_STRING__L6*/ meltfnum[0] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V18*/ meltfptr[9] = 0;
	}
	;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4402:/ cppif.then");
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
      MELT_LOCATION ("warmelt-modes.melt:4402:/ cond");
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
	    MELT_LOCATION ("warmelt-modes.melt:4402:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4402;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "translateinit_mode basnam=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.BASNAM__V17*/ meltfptr[12];
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[20] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V22*/ meltfptr[18] =
	      /*_.MELT_DEBUG_FUN__V23*/ meltfptr[20];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4402:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V23*/ meltfptr[20] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V22*/ meltfptr[18] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4402:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L7*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V22*/ meltfptr[18] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#STRING_DYNLOADED_SUFFIXED__L9*/ meltfnum[7] =
      /*string_dynloaded_suffixed: */
      (melt_string_is_ending ((melt_ptr_t) /*_.BASNAM__V17*/ meltfptr[12],
			      MELT_DYNLOADED_SUFFIX));;
    MELT_LOCATION ("warmelt-modes.melt:4403:/ cond");
    /*cond */ if ( /*_#STRING_DYNLOADED_SUFFIXED__L9*/ meltfnum[7])	/*then */
      {
	/*^cond.then */
	/*_#OR___L10*/ meltfnum[0] =
	  /*_#STRING_DYNLOADED_SUFFIXED__L9*/ meltfnum[7];;
      }
    else
      {
	MELT_LOCATION ("warmelt-modes.melt:4403:/ cond.else");

	/*^block */
	/*anyblock */
	{

   /*_#STRING_SUFFIXED__L11*/ meltfnum[10] =
	    /*string_suffixed: */
	    (melt_string_is_ending
	     ((melt_ptr_t) /*_.BASNAM__V17*/ meltfptr[12], ".melt"));;
	  MELT_LOCATION ("warmelt-modes.melt:4403:/ cond");
	  /*cond */ if ( /*_#STRING_SUFFIXED__L11*/ meltfnum[10])	/*then */
	    {
	      /*^cond.then */
	      /*_#OR___L12*/ meltfnum[11] =
		/*_#STRING_SUFFIXED__L11*/ meltfnum[10];;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-modes.melt:4403:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {

     /*_#STRING_SUFFIXED__L13*/ meltfnum[12] =
		  /*string_suffixed: */
		  (melt_string_is_ending
		   ((melt_ptr_t) /*_.BASNAM__V17*/ meltfptr[12], ".c"));;
		/*^compute */
		/*_#OR___L12*/ meltfnum[11] =
		  /*_#STRING_SUFFIXED__L13*/ meltfnum[12];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4403:/ clear");
	       /*clear *//*_#STRING_SUFFIXED__L13*/ meltfnum[12] = 0;
	      }
	      ;
	    }
	  ;
	  /*_#OR___L10*/ meltfnum[0] = /*_#OR___L12*/ meltfnum[11];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4403:/ clear");
	     /*clear *//*_#STRING_SUFFIXED__L11*/ meltfnum[10] = 0;
	  /*^clear */
	     /*clear *//*_#OR___L12*/ meltfnum[11] = 0;
	}
	;
      }
    ;
    /*^cond */
    /*cond */ if ( /*_#OR___L10*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-modes.melt:4407:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter,
		   ("tranlateinit mode needs a base name without suffix"),
		   melt_string_str ((melt_ptr_t)
				    ( /*_.BASNAM__V17*/ meltfptr[12])));
	  }
	  ;
	  MELT_LOCATION ("warmelt-modes.melt:4409:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-modes.melt:4409:/ locexp");
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
	  MELT_LOCATION ("warmelt-modes.melt:4406:/ quasiblock");


	  /*_.PROGN___V26*/ meltfptr[18] = /*_.RETURN___V25*/ meltfptr[20];;
	  /*^compute */
	  /*_._IF___V24*/ meltfptr[9] = /*_.PROGN___V26*/ meltfptr[18];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4403:/ clear");
	     /*clear *//*_.RETURN___V25*/ meltfptr[20] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V26*/ meltfptr[18] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V24*/ meltfptr[9] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_STRING__L14*/ meltfnum[12] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.INARG__V8*/ meltfptr[7])) ==
       MELTOBMAG_STRING);;
    MELT_LOCATION ("warmelt-modes.melt:4412:/ cond");
    /*cond */ if ( /*_#IS_STRING__L14*/ meltfnum[12])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.READ_FILE__V28*/ meltfptr[18] =
	    (meltgc_read_file
	     (melt_string_str ((melt_ptr_t) ( /*_.INARG__V8*/ meltfptr[7])),
	      (char *) 0));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-modes.melt:4413:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.READ_FILE__V28*/ meltfptr[18];
	    /*_.LIST_APPEND2LIST__V29*/ meltfptr[28] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!LIST_APPEND2LIST */ meltfrout->tabval[4])),
			  (melt_ptr_t) ( /*_.RLIST__V7*/ meltfptr[3]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_._IFELSE___V27*/ meltfptr[20] =
	    /*_.LIST_APPEND2LIST__V29*/ meltfptr[28];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4412:/ clear");
	     /*clear *//*_.READ_FILE__V28*/ meltfptr[18] = 0;
	  /*^clear */
	     /*clear *//*_.LIST_APPEND2LIST__V29*/ meltfptr[28] = 0;
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
   /*_#IS_LIST__L15*/ meltfnum[10] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.INARG__V8*/ meltfptr[7])) ==
	     MELTOBMAG_LIST);;
	  MELT_LOCATION ("warmelt-modes.melt:4414:/ cond");
	  /*cond */ if ( /*_#IS_LIST__L15*/ meltfnum[10])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-modes.melt:4416:/ quasiblock");


		/*^newclosure */
		     /*newclosure *//*_.LAMBDA___V32*/ meltfptr[31] =
		  (melt_ptr_t)
		  meltgc_new_closure ((meltobject_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (DISCR_CLOSURE)))),
				      (meltroutine_ptr_t) (( /*!konst_8 */
							    meltfrout->
							    tabval[8])), (1));
		;
		/*^putclosedv */
		/*putclosv */
		melt_assertmsg ("putclosv checkclo",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.LAMBDA___V32*/
						   meltfptr[31])) ==
				MELTOBMAG_CLOSURE);
		melt_assertmsg ("putclosv checkoff", 0 >= 0
				&& 0 <
				melt_closure_size ((melt_ptr_t)
						   ( /*_.LAMBDA___V32*/
						    meltfptr[31])));
		((meltclosure_ptr_t) /*_.LAMBDA___V32*/ meltfptr[31])->
		  tabval[0] = (melt_ptr_t) ( /*_.RLIST__V7*/ meltfptr[3]);
		;
		/*_.LAMBDA___V31*/ meltfptr[28] =
		  /*_.LAMBDA___V32*/ meltfptr[31];;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-modes.melt:4415:/ apply");
		/*apply */
		{
		  union meltparam_un argtab[1];
		  memset (&argtab, 0, sizeof (argtab));
		  /*^apply.arg */
		  argtab[0].meltbp_aptr =
		    (melt_ptr_t *) & /*_.LAMBDA___V31*/ meltfptr[28];
		  /*_.LIST_EVERY__V33*/ meltfptr[32] =
		    melt_apply ((meltclosure_ptr_t)
				(( /*!LIST_EVERY */ meltfrout->tabval[5])),
				(melt_ptr_t) ( /*_.INARG__V8*/ meltfptr[7]),
				(MELTBPARSTR_PTR ""), argtab, "",
				(union meltparam_un *) 0);
		}
		;
		/*_._IFELSE___V30*/ meltfptr[18] =
		  /*_.LIST_EVERY__V33*/ meltfptr[32];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4414:/ clear");
	       /*clear *//*_.LAMBDA___V31*/ meltfptr[28] = 0;
		/*^clear */
	       /*clear *//*_.LIST_EVERY__V33*/ meltfptr[32] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IFELSE___V30*/ meltfptr[18] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_._IFELSE___V27*/ meltfptr[20] =
	    /*_._IFELSE___V30*/ meltfptr[18];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4412:/ clear");
	     /*clear *//*_#IS_LIST__L15*/ meltfnum[10] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V30*/ meltfptr[18] = 0;
	}
	;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4425:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L16*/ meltfnum[11] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-modes.melt:4425:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L16*/ meltfnum[11])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L17*/ meltfnum[10] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-modes.melt:4425:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[10];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4425;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"after read translateinit_mode rlist=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RLIST__V7*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[32] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V34*/ meltfptr[28] =
	      /*_.MELT_DEBUG_FUN__V35*/ meltfptr[32];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4425:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L17*/ meltfnum[10] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V35*/ meltfptr[32] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V34*/ meltfptr[28] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4425:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L16*/ meltfnum[11] = 0;
      /*^clear */
	     /*clear *//*_._IF___V34*/ meltfptr[28] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4431:/ apply");
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = /*nil */ (melt_ptr_t *) NULL;
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.BASNAM__V17*/ meltfptr[12];
      /*_.COMPILE_LIST_SEXPR__V36*/ meltfptr[18] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!COMPILE_LIST_SEXPR */ meltfrout->tabval[9])),
		    (melt_ptr_t) ( /*_.RLIST__V7*/ meltfptr[3]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    {
      MELT_LOCATION ("warmelt-modes.melt:4434:/ locexp");
      melt_garbcoll ((10000), MELT_NEED_FULL);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4435:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] =
      ( /*!konst_10_TRUE */ meltfrout->tabval[10]);;

    {
      MELT_LOCATION ("warmelt-modes.melt:4435:/ locexp");
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
    /*_.LET___V6*/ meltfptr[4] = /*_.RETURN___V37*/ meltfptr[32];;

    MELT_LOCATION ("warmelt-modes.melt:4376:/ clear");
	   /*clear *//*_.RLIST__V7*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
    /*^clear */
	   /*clear *//*_?*/ meltfram__.loc_CSTRING__o1 = 0;
    /*^clear */
	   /*clear *//*_.INARG__V8*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_?*/ meltfram__.loc_CSTRING__o2 = 0;
    /*^clear */
	   /*clear *//*_.OUTARG__V16*/ meltfptr[8] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L5*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_.BASNAM__V17*/ meltfptr[12] = 0;
    /*^clear */
	   /*clear *//*_#STRING_DYNLOADED_SUFFIXED__L9*/ meltfnum[7] = 0;
    /*^clear */
	   /*clear *//*_#OR___L10*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V24*/ meltfptr[9] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L14*/ meltfnum[12] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V27*/ meltfptr[20] = 0;
    /*^clear */
	   /*clear *//*_.COMPILE_LIST_SEXPR__V36*/ meltfptr[18] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V37*/ meltfptr[32] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4373:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V6*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4373:/ locexp");
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
	   /*clear *//*_.LET___V6*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("TRANSLATEINIT_DOCMD", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_69_WARMELTmiMODES_TRANSLATEINIT_DOCMD */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_70_WARMELTmiMODES_LAMBDA_cl22 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_70_WARMELTmiMODES_LAMBDA_cl22_melt
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

  struct meltframe_meltrout_70_WARMELTmiMODES_LAMBDA_cl22_st
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
#define MELTFRAM_NBVARPTR 11
    melt_ptr_t mcfr_varptr[11];
#define MELTFRAM_NBVARNUM 4
    long mcfr_varnum[4];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_70_WARMELTmiMODES_LAMBDA_cl22 is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_70_WARMELTmiMODES_LAMBDA_cl22_st *)
	meltfirstargp_;
      /* use arguments meltrout_70_WARMELTmiMODES_LAMBDA_cl22 output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 11; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_70_WARMELTmiMODES_LAMBDA_cl22 nbval 11*/
  meltfram__.mcfr_nbvar = 11 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_cl22", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4416:/ getarg");
 /*_.CURARG__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-modes.melt:4417:/ locexp");
      inform (UNKNOWN_LOCATION, ("MELT INFORM [#%ld]: %s - %s"),
	      melt_dbgcounter, ("reading from file"),
	      melt_string_str ((melt_ptr_t) ( /*_.CURARG__V2*/ meltfptr[1])));
    }
    ;
    MELT_LOCATION ("warmelt-modes.melt:4418:/ quasiblock");


 /*_.CUREAD__V4*/ meltfptr[3] =
      (meltgc_read_file
       (melt_string_str ((melt_ptr_t) ( /*_.CURARG__V2*/ meltfptr[1])),
	(char *) 0));;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4420:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L1*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) (( /*~RLIST */ meltfclos->tabval[0])))
	 == MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-modes.melt:4420:/ cond");
      /*cond */ if ( /*_#IS_LIST__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V6*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-modes.melt:4420:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check rlist"),
				  ("warmelt-modes.melt")
				  ? ("warmelt-modes.melt") : __FILE__,
				  (4420) ? (4420) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V6*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-modes.melt:4420:/ clear");
	     /*clear *//*_#IS_LIST__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V5*/ meltfptr[4] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4421:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST_OR_NULL__L2*/ meltfnum[0] =
	(( /*_.CUREAD__V4*/ meltfptr[3]) == NULL
	 ||
	 (melt_unsafe_magic_discr
	  ((melt_ptr_t) ( /*_.CUREAD__V4*/ meltfptr[3])) == MELTOBMAG_LIST));;
      MELT_LOCATION ("warmelt-modes.melt:4421:/ cond");
      /*cond */ if ( /*_#IS_LIST_OR_NULL__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V8*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-modes.melt:4421:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check curead"),
				  ("warmelt-modes.melt")
				  ? ("warmelt-modes.melt") : __FILE__,
				  (4421) ? (4421) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V8*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-modes.melt:4421:/ clear");
	     /*clear *//*_#IS_LIST_OR_NULL__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V8*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V7*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4422:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L3*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-modes.melt:4422:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L4*/ meltfnum[3] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-modes.melt:4422:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4422;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring = "translateinit_mode curead=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CUREAD__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V9*/ meltfptr[7] =
	      /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4422:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L4*/ meltfnum[3] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V9*/ meltfptr[7] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4422:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4423:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.CUREAD__V4*/ meltfptr[3];
      /*_.LIST_APPEND2LIST__V11*/ meltfptr[9] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_APPEND2LIST */ meltfrout->tabval[1])),
		    (melt_ptr_t) (( /*~RLIST */ meltfclos->tabval[0])),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    /*_.LET___V3*/ meltfptr[2] = /*_.LIST_APPEND2LIST__V11*/ meltfptr[9];;

    MELT_LOCATION ("warmelt-modes.melt:4418:/ clear");
	   /*clear *//*_.CUREAD__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V7*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.LIST_APPEND2LIST__V11*/ meltfptr[9] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4416:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4416:/ locexp");
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
	   /*clear *//*_.LET___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_cl22", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_70_WARMELTmiMODES_LAMBDA_cl22_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_70_WARMELTmiMODES_LAMBDA_cl22 */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED (meltclosure_ptr_t
						   meltclosp_,
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
    current_blocklevel_signals_meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED_melt
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

  struct meltframe_meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED_st
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
#define MELTFRAM_NBVARNUM 28
    long mcfr_varnum[28];
/*others*/
    const char *loc_CSTRING__o0;
    const char *loc_CSTRING__o1;
    const char *loc_CSTRING__o2;
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED_st
	 *) meltfirstargp_;
      /* use arguments meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED nbval 51*/
  meltfram__.mcfr_nbvar = 51 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("TRANSLATEINIT_FLAVORED", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4452:/ getarg");
 /*_.FLAVOR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-modes.melt:4453:/ quasiblock");


 /*_.RLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[0]))));;
    /*^compute */
 /*_?*/ meltfram__.loc_CSTRING__o0 =
      melt_argument ("arg");;
    /*^compute */
 /*_?*/ meltfram__.loc_CSTRING__o1 =
      melt_argument ("arglist");;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4457:/ cond");
    /*cond */ if ( /*_?*/ meltfram__.loc_CSTRING__o0)	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-modes.melt:4458:/ quasiblock");


   /*_.PROGARGSTR__V7*/ meltfptr[6] =
	    (meltgc_new_stringdup
	     ((meltobject_ptr_t) (( /*!DISCR_STRING */ meltfrout->tabval[1])),
	      ( /*_?*/ meltfram__.loc_CSTRING__o0)));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
   /*_#STRING_SUFFIXED__L1*/ meltfnum[0] =
	    /*string_suffixed: */
	    (melt_string_is_ending
	     ((melt_ptr_t) /*_.PROGARGSTR__V7*/ meltfptr[6], ".melt"));;
	  /*^compute */
   /*_#NOT__L2*/ meltfnum[1] =
	    (!( /*_#STRING_SUFFIXED__L1*/ meltfnum[0]));;
	  MELT_LOCATION ("warmelt-modes.melt:4460:/ cond");
	  /*cond */ if ( /*_#NOT__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-modes.melt:4461:/ locexp");
		  warning (0, "MELT WARNING MSG [#%ld]::: %s - %s",
			   melt_dbgcounter,
			   ("MELT translated initial file without .melt suffix"),
			   melt_string_str ((melt_ptr_t)
					    ( /*_.PROGARGSTR__V7*/
					     meltfptr[6])));
		}
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*_.LET___V6*/ meltfptr[5] = /*_.PROGARGSTR__V7*/ meltfptr[6];;

	  MELT_LOCATION ("warmelt-modes.melt:4458:/ clear");
	     /*clear *//*_.PROGARGSTR__V7*/ meltfptr[6] = 0;
	  /*^clear */
	     /*clear *//*_#STRING_SUFFIXED__L1*/ meltfnum[0] = 0;
	  /*^clear */
	     /*clear *//*_#NOT__L2*/ meltfnum[1] = 0;
	  /*_.INARG__V5*/ meltfptr[4] = /*_.LET___V6*/ meltfptr[5];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4457:/ clear");
	     /*clear *//*_.LET___V6*/ meltfptr[5] = 0;
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
	  MELT_LOCATION ("warmelt-modes.melt:4464:/ cond");
	  /*cond */ if ( /*_?*/ meltfram__.loc_CSTRING__o1)	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_STRINGCONST__V9*/ meltfptr[5] =
		  (meltgc_new_stringdup
		   ((meltobject_ptr_t)
		    (( /*!DISCR_STRING */ meltfrout->tabval[1])),
		    ( /*_?*/ meltfram__.loc_CSTRING__o1)));;
		/*^compute */
     /*_.SPLIT_STRING_COMMA__V10*/ meltfptr[9] =
		  meltgc_new_split_string (melt_string_str
					   ((melt_ptr_t)
					    /*_.MAKE_STRINGCONST__V9*/
					    meltfptr[5]), ',',
					   (melt_ptr_t) ( /*!DISCR_STRING */
							 meltfrout->
							 tabval[1]));;
		/*^compute */
		/*_._IFELSE___V8*/ meltfptr[6] =
		  /*_.SPLIT_STRING_COMMA__V10*/ meltfptr[9];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4464:/ clear");
	       /*clear *//*_.MAKE_STRINGCONST__V9*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.SPLIT_STRING_COMMA__V10*/ meltfptr[9] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-modes.melt:4468:/ locexp");
		  error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
			 ("invalid arg or arglist to translateinitmodule mode"));
		}
		;
		MELT_LOCATION ("warmelt-modes.melt:4469:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4469:/ locexp");
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
		MELT_LOCATION ("warmelt-modes.melt:4467:/ quasiblock");


		/*_.PROGN___V12*/ meltfptr[9] =
		  /*_.RETURN___V11*/ meltfptr[5];;
		/*^compute */
		/*_._IFELSE___V8*/ meltfptr[6] =
		  /*_.PROGN___V12*/ meltfptr[9];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4464:/ clear");
	       /*clear *//*_.RETURN___V11*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V12*/ meltfptr[9] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.INARG__V5*/ meltfptr[4] = /*_._IFELSE___V8*/ meltfptr[6];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4457:/ clear");
	     /*clear *//*_._IFELSE___V8*/ meltfptr[6] = 0;
	}
	;
      }
    ;
 /*_?*/ meltfram__.loc_CSTRING__o2 =
      melt_argument ("output");;
    /*^compute */
 /*_.OUTARG__V13*/ meltfptr[5] =
      (meltgc_new_stringdup
       ((meltobject_ptr_t) (( /*!DISCR_STRING */ meltfrout->tabval[1])),
	( /*_?*/ meltfram__.loc_CSTRING__o2)));;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_STRING__L3*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.OUTARG__V13*/ meltfptr[5])) ==
       MELTOBMAG_STRING);;
    MELT_LOCATION ("warmelt-modes.melt:4472:/ cond");
    /*cond */ if ( /*_#IS_STRING__L3*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*_.BASNAM__V14*/ meltfptr[9] = /*_.OUTARG__V13*/ meltfptr[5];;
      }
    else
      {
	MELT_LOCATION ("warmelt-modes.melt:4472:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_STRING__L4*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.INARG__V5*/ meltfptr[4])) ==
	     MELTOBMAG_STRING);;
	  MELT_LOCATION ("warmelt-modes.melt:4473:/ cond");
	  /*cond */ if ( /*_#IS_STRING__L4*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_STRING_NAKEDBASENAME__V16*/ meltfptr[15] =
		  (meltgc_new_string_nakedbasename
		   ((meltobject_ptr_t)
		    (( /*!DISCR_STRING */ meltfrout->tabval[1])),
		    melt_string_str ((melt_ptr_t)
				     ( /*_.INARG__V5*/ meltfptr[4]))));;
		/*^compute */
		/*_._IFELSE___V15*/ meltfptr[6] =
		  /*_.MAKE_STRING_NAKEDBASENAME__V16*/ meltfptr[15];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4473:/ clear");
	       /*clear *//*_.MAKE_STRING_NAKEDBASENAME__V16*/ meltfptr[15] =
		  0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-modes.melt:4475:/ locexp");
		  error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
			 ("invalid translateinitmodule mode"));
		}
		;
		MELT_LOCATION ("warmelt-modes.melt:4476:/ quasiblock");


     /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4476:/ locexp");
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
		MELT_LOCATION ("warmelt-modes.melt:4474:/ quasiblock");


		/*_.PROGN___V18*/ meltfptr[17] =
		  /*_.RETURN___V17*/ meltfptr[15];;
		/*^compute */
		/*_._IFELSE___V15*/ meltfptr[6] =
		  /*_.PROGN___V18*/ meltfptr[17];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4473:/ clear");
	       /*clear *//*_.RETURN___V17*/ meltfptr[15] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V18*/ meltfptr[17] = 0;
	      }
	      ;
	    }
	  ;
	  /*_.BASNAM__V14*/ meltfptr[9] = /*_._IFELSE___V15*/ meltfptr[6];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4472:/ clear");
	     /*clear *//*_#IS_STRING__L4*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V15*/ meltfptr[6] = 0;
	}
	;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4479:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L5*/ meltfnum[1] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-modes.melt:4479:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L5*/ meltfnum[1])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-modes.melt:4479:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[7];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4479;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"translateinitmodule_flavored basnam=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.BASNAM__V14*/ meltfptr[9];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " flavor=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.FLAVOR__V2*/ meltfptr[1];
	      /*_.MELT_DEBUG_FUN__V20*/ meltfptr[17] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[2])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V19*/ meltfptr[15] =
	      /*_.MELT_DEBUG_FUN__V20*/ meltfptr[17];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4479:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L6*/ meltfnum[5] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V20*/ meltfptr[17] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V19*/ meltfptr[15] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4479:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0;
      /*^clear */
	     /*clear *//*_._IF___V19*/ meltfptr[15] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#STRING_DYNLOADED_SUFFIXED__L7*/ meltfnum[5] =
      /*string_dynloaded_suffixed: */
      (melt_string_is_ending ((melt_ptr_t) /*_.BASNAM__V14*/ meltfptr[9],
			      MELT_DYNLOADED_SUFFIX));;
    MELT_LOCATION ("warmelt-modes.melt:4480:/ cond");
    /*cond */ if ( /*_#STRING_DYNLOADED_SUFFIXED__L7*/ meltfnum[5])	/*then */
      {
	/*^cond.then */
	/*_#OR___L8*/ meltfnum[1] =
	  /*_#STRING_DYNLOADED_SUFFIXED__L7*/ meltfnum[5];;
      }
    else
      {
	MELT_LOCATION ("warmelt-modes.melt:4480:/ cond.else");

	/*^block */
	/*anyblock */
	{

   /*_#STRING_SUFFIXED__L9*/ meltfnum[8] =
	    /*string_suffixed: */
	    (melt_string_is_ending
	     ((melt_ptr_t) /*_.BASNAM__V14*/ meltfptr[9], ".melt"));;
	  MELT_LOCATION ("warmelt-modes.melt:4480:/ cond");
	  /*cond */ if ( /*_#STRING_SUFFIXED__L9*/ meltfnum[8])	/*then */
	    {
	      /*^cond.then */
	      /*_#OR___L10*/ meltfnum[9] =
		/*_#STRING_SUFFIXED__L9*/ meltfnum[8];;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-modes.melt:4480:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {

     /*_#STRING_SUFFIXED__L11*/ meltfnum[10] =
		  /*string_suffixed: */
		  (melt_string_is_ending
		   ((melt_ptr_t) /*_.BASNAM__V14*/ meltfptr[9], ".c"));;
		/*^compute */
		/*_#OR___L10*/ meltfnum[9] =
		  /*_#STRING_SUFFIXED__L11*/ meltfnum[10];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4480:/ clear");
	       /*clear *//*_#STRING_SUFFIXED__L11*/ meltfnum[10] = 0;
	      }
	      ;
	    }
	  ;
	  /*_#OR___L8*/ meltfnum[1] = /*_#OR___L10*/ meltfnum[9];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4480:/ clear");
	     /*clear *//*_#STRING_SUFFIXED__L9*/ meltfnum[8] = 0;
	  /*^clear */
	     /*clear *//*_#OR___L10*/ meltfnum[9] = 0;
	}
	;
      }
    ;
    /*^cond */
    /*cond */ if ( /*_#OR___L8*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-modes.melt:4484:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter,
		   ("tranlateinit mode needs a base name without suffix"),
		   melt_string_str ((melt_ptr_t)
				    ( /*_.BASNAM__V14*/ meltfptr[9])));
	  }
	  ;
	  MELT_LOCATION ("warmelt-modes.melt:4486:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-modes.melt:4486:/ locexp");
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
	  MELT_LOCATION ("warmelt-modes.melt:4483:/ quasiblock");


	  /*_.PROGN___V23*/ meltfptr[15] = /*_.RETURN___V22*/ meltfptr[17];;
	  /*^compute */
	  /*_._IF___V21*/ meltfptr[6] = /*_.PROGN___V23*/ meltfptr[15];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4480:/ clear");
	     /*clear *//*_.RETURN___V22*/ meltfptr[17] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V23*/ meltfptr[15] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V21*/ meltfptr[6] = NULL;;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_STRING__L12*/ meltfnum[10] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.INARG__V5*/ meltfptr[4])) ==
       MELTOBMAG_STRING);;
    MELT_LOCATION ("warmelt-modes.melt:4489:/ cond");
    /*cond */ if ( /*_#IS_STRING__L12*/ meltfnum[10])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

   /*_.READ_FILE__V25*/ meltfptr[15] =
	    (meltgc_read_file
	     (melt_string_str ((melt_ptr_t) ( /*_.INARG__V5*/ meltfptr[4])),
	      (char *) 0));;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-modes.melt:4490:/ apply");
	  /*apply */
	  {
	    union meltparam_un argtab[1];
	    memset (&argtab, 0, sizeof (argtab));
	    /*^apply.arg */
	    argtab[0].meltbp_aptr =
	      (melt_ptr_t *) & /*_.READ_FILE__V25*/ meltfptr[15];
	    /*_.LIST_APPEND2LIST__V26*/ meltfptr[25] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!LIST_APPEND2LIST */ meltfrout->tabval[3])),
			  (melt_ptr_t) ( /*_.RLIST__V4*/ meltfptr[3]),
			  (MELTBPARSTR_PTR ""), argtab, "",
			  (union meltparam_un *) 0);
	  }
	  ;
	  /*_._IFELSE___V24*/ meltfptr[17] =
	    /*_.LIST_APPEND2LIST__V26*/ meltfptr[25];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4489:/ clear");
	     /*clear *//*_.READ_FILE__V25*/ meltfptr[15] = 0;
	  /*^clear */
	     /*clear *//*_.LIST_APPEND2LIST__V26*/ meltfptr[25] = 0;
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
   /*_#IS_LIST__L13*/ meltfnum[8] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.INARG__V5*/ meltfptr[4])) ==
	     MELTOBMAG_LIST);;
	  MELT_LOCATION ("warmelt-modes.melt:4491:/ cond");
	  /*cond */ if ( /*_#IS_LIST__L13*/ meltfnum[8])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-modes.melt:4493:/ quasiblock");


		/*^newclosure */
		     /*newclosure *//*_.LAMBDA___V29*/ meltfptr[28] =
		  (melt_ptr_t)
		  meltgc_new_closure ((meltobject_ptr_t)
				      (((melt_ptr_t)
					(MELT_PREDEF (DISCR_CLOSURE)))),
				      (meltroutine_ptr_t) (( /*!konst_7 */
							    meltfrout->
							    tabval[7])), (1));
		;
		/*^putclosedv */
		/*putclosv */
		melt_assertmsg ("putclosv checkclo",
				melt_magic_discr ((melt_ptr_t)
						  ( /*_.LAMBDA___V29*/
						   meltfptr[28])) ==
				MELTOBMAG_CLOSURE);
		melt_assertmsg ("putclosv checkoff", 0 >= 0
				&& 0 <
				melt_closure_size ((melt_ptr_t)
						   ( /*_.LAMBDA___V29*/
						    meltfptr[28])));
		((meltclosure_ptr_t) /*_.LAMBDA___V29*/ meltfptr[28])->
		  tabval[0] = (melt_ptr_t) ( /*_.RLIST__V4*/ meltfptr[3]);
		;
		/*_.LAMBDA___V28*/ meltfptr[25] =
		  /*_.LAMBDA___V29*/ meltfptr[28];;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-modes.melt:4492:/ apply");
		/*apply */
		{
		  union meltparam_un argtab[1];
		  memset (&argtab, 0, sizeof (argtab));
		  /*^apply.arg */
		  argtab[0].meltbp_aptr =
		    (melt_ptr_t *) & /*_.LAMBDA___V28*/ meltfptr[25];
		  /*_.LIST_EVERY__V30*/ meltfptr[29] =
		    melt_apply ((meltclosure_ptr_t)
				(( /*!LIST_EVERY */ meltfrout->tabval[4])),
				(melt_ptr_t) ( /*_.INARG__V5*/ meltfptr[4]),
				(MELTBPARSTR_PTR ""), argtab, "",
				(union meltparam_un *) 0);
		}
		;
		/*_._IFELSE___V27*/ meltfptr[15] =
		  /*_.LIST_EVERY__V30*/ meltfptr[29];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4491:/ clear");
	       /*clear *//*_.LAMBDA___V28*/ meltfptr[25] = 0;
		/*^clear */
	       /*clear *//*_.LIST_EVERY__V30*/ meltfptr[29] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IFELSE___V27*/ meltfptr[15] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_._IFELSE___V24*/ meltfptr[17] =
	    /*_._IFELSE___V27*/ meltfptr[15];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4489:/ clear");
	     /*clear *//*_#IS_LIST__L13*/ meltfnum[8] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V27*/ meltfptr[15] = 0;
	}
	;
      }
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4502:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L14*/ meltfnum[9] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-modes.melt:4502:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L14*/ meltfnum[9])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L15*/ meltfnum[8] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-modes.melt:4502:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[8];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4502;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"after read translateinitmodule_flavored rlist=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.RLIST__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V32*/ meltfptr[29] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[2])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V31*/ meltfptr[25] =
	      /*_.MELT_DEBUG_FUN__V32*/ meltfptr[29];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4502:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L15*/ meltfnum[8] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V32*/ meltfptr[29] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V31*/ meltfptr[25] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4502:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L14*/ meltfnum[9] = 0;
      /*^clear */
	     /*clear *//*_._IF___V31*/ meltfptr[25] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4505:/ apply");
    /*apply */
    {
      union meltparam_un argtab[2];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = /*nil */ (melt_ptr_t *) NULL;
      /*^apply.arg */
      argtab[1].meltbp_aptr = (melt_ptr_t *) & /*_.BASNAM__V14*/ meltfptr[9];
      /*_.COMPILE_LIST_SEXPR__V33*/ meltfptr[15] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!COMPILE_LIST_SEXPR */ meltfrout->tabval[8])),
		    (melt_ptr_t) ( /*_.RLIST__V4*/ meltfptr[3]),
		    (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    {
      MELT_LOCATION ("warmelt-modes.melt:4508:/ locexp");
      melt_garbcoll ((10000), MELT_NEED_FULL);
    }
    ;
    MELT_LOCATION ("warmelt-modes.melt:4509:/ quasiblock");


    MELT_LOCATION ("warmelt-modes.melt:4510:/ cond");
    /*cond */ if ( /*_.OUTARG__V13*/ meltfptr[5])	/*then */
      {
	/*^cond.then */
	/*_.OUTBASE__V35*/ meltfptr[25] = /*_.OUTARG__V13*/ meltfptr[5];;
      }
    else
      {
	MELT_LOCATION ("warmelt-modes.melt:4510:/ cond.else");

	/*_.OUTBASE__V35*/ meltfptr[25] = /*_.BASNAM__V14*/ meltfptr[9];;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_MULTIPLE__L16*/ meltfnum[8] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FLAVOR__V2*/ meltfptr[1])) ==
       MELTOBMAG_MULTIPLE);;
    MELT_LOCATION ("warmelt-modes.melt:4513:/ cond");
    /*cond */ if ( /*_#IS_MULTIPLE__L16*/ meltfnum[8])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*citerblock FOREACH_IN_MULTIPLE */
	  {
	    /* start foreach_in_multiple meltcit1__EACHTUP */
	    long meltcit1__EACHTUP_ln =
	      melt_multiple_length ((melt_ptr_t) /*_.FLAVOR__V2*/
				    meltfptr[1]);
	    for ( /*_#FLAVIX__L17*/ meltfnum[9] = 0;
		 ( /*_#FLAVIX__L17*/ meltfnum[9] >= 0)
		 && ( /*_#FLAVIX__L17*/ meltfnum[9] < meltcit1__EACHTUP_ln);
	/*_#FLAVIX__L17*/ meltfnum[9]++)
	      {
		/*_.CURFLAVOR__V37*/ meltfptr[36] =
		  melt_multiple_nth ((melt_ptr_t)
				     ( /*_.FLAVOR__V2*/ meltfptr[1]),
				     /*_#FLAVIX__L17*/ meltfnum[9]);




#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-modes.melt:4517:/ cppif.then");
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
      /*_#MELT_NEED_DBG__L18*/ meltfnum[17] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-modes.melt:4517:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L18*/ meltfnum[17])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	/*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-modes.melt:4517:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[9];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-modes.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 4517;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "translateinitmodule_flavored before module generation basnam=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.BASNAM__V14*/ meltfptr[9];
			  /*^apply.arg */
			  argtab[5].meltbp_cstring = " outbase=";
			  /*^apply.arg */
			  argtab[6].meltbp_aptr =
			    (melt_ptr_t *) & /*_.OUTBASE__V35*/ meltfptr[25];
			  /*^apply.arg */
			  argtab[7].meltbp_cstring = " curflavor=";
			  /*^apply.arg */
			  argtab[8].meltbp_aptr =
			    (melt_ptr_t *) & /*_.CURFLAVOR__V37*/
			    meltfptr[36];
			  /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[2])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V38*/ meltfptr[37] =
			  /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38];;
			/*epilog */

			MELT_LOCATION ("warmelt-modes.melt:4517:/ clear");
		  /*clear *//*_#MELT_CALLCOUNT__L19*/ meltfnum[18] = 0;
			/*^clear */
		  /*clear *//*_.MELT_DEBUG_FUN__V39*/ meltfptr[38] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

       /*_._IF___V38*/ meltfptr[37] = NULL;;
		    }
		  ;

		  {
		    MELT_LOCATION ("warmelt-modes.melt:4517:/ locexp");
		    /*void */ (void) 0;
		  }
		  ;
		  /*^quasiblock */


		  /*epilog */

		  /*^clear */
		/*clear *//*_#MELT_NEED_DBG__L18*/ meltfnum[17] = 0;
		  /*^clear */
		/*clear *//*_._IF___V38*/ meltfptr[37] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*^block */
		/*anyblock */
		{


		  {
		    /*^locexp */
		    /*void */ (void) 0;
		  }
		  ;
		  /*epilog */
		}

#endif /*MELT_HAVE_DEBUG */
		;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4519:/ locexp");
		  /*generate_flavored_melt_module */
		    melt_compile_source (melt_string_str
					 ((melt_ptr_t) /*_.BASNAM__V14*/
					  meltfptr[9]),
					 melt_string_str ((melt_ptr_t)
							  /*_.OUTBASE__V35*/
							  meltfptr[25]),
					 NULL, melt_string_str ((melt_ptr_t) /*_.CURFLAVOR__V37*/ meltfptr[36]));	/*generate_flavored_melt_module */
		  ;
		}
		;

#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-modes.melt:4520:/ cppif.then");
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
      /*_#MELT_NEED_DBG__L20*/ meltfnum[18] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-modes.melt:4520:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L20*/ meltfnum[18])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	/*_#MELT_CALLCOUNT__L21*/ meltfnum[17] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-modes.melt:4520:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[9];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L21*/ meltfnum[17];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-modes.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 4520;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "translateinitmodule_flavored after module generation basnam=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.BASNAM__V14*/ meltfptr[9];
			  /*^apply.arg */
			  argtab[5].meltbp_cstring = " outbase=";
			  /*^apply.arg */
			  argtab[6].meltbp_aptr =
			    (melt_ptr_t *) & /*_.OUTBASE__V35*/ meltfptr[25];
			  /*^apply.arg */
			  argtab[7].meltbp_cstring = " curflavor=";
			  /*^apply.arg */
			  argtab[8].meltbp_aptr =
			    (melt_ptr_t *) & /*_.CURFLAVOR__V37*/
			    meltfptr[36];
			  /*_.MELT_DEBUG_FUN__V41*/ meltfptr[37] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[2])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V40*/ meltfptr[38] =
			  /*_.MELT_DEBUG_FUN__V41*/ meltfptr[37];;
			/*epilog */

			MELT_LOCATION ("warmelt-modes.melt:4520:/ clear");
		  /*clear *//*_#MELT_CALLCOUNT__L21*/ meltfnum[17] = 0;
			/*^clear */
		  /*clear *//*_.MELT_DEBUG_FUN__V41*/ meltfptr[37] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

       /*_._IF___V40*/ meltfptr[38] = NULL;;
		    }
		  ;

		  {
		    MELT_LOCATION ("warmelt-modes.melt:4520:/ locexp");
		    /*void */ (void) 0;
		  }
		  ;
		  /*^quasiblock */


		  /*epilog */

		  /*^clear */
		/*clear *//*_#MELT_NEED_DBG__L20*/ meltfnum[18] = 0;
		  /*^clear */
		/*clear *//*_._IF___V40*/ meltfptr[38] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*^block */
		/*anyblock */
		{


		  {
		    /*^locexp */
		    /*void */ (void) 0;
		  }
		  ;
		  /*epilog */
		}

#endif /*MELT_HAVE_DEBUG */
		;
		if ( /*_#FLAVIX__L17*/ meltfnum[9] < 0)
		  break;
	      }			/* end  foreach_in_multiple meltcit1__EACHTUP */

	    /*citerepilog */

	    MELT_LOCATION ("warmelt-modes.melt:4514:/ clear");
	      /*clear *//*_.CURFLAVOR__V37*/ meltfptr[36] = 0;
	    /*^clear */
	      /*clear *//*_#FLAVIX__L17*/ meltfnum[9] = 0;
	  }			/*endciterblock FOREACH_IN_MULTIPLE */
	  ;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-modes.melt:4513:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_STRING__L22*/ meltfnum[17] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.FLAVOR__V2*/ meltfptr[1]))
	     == MELTOBMAG_STRING);;
	  MELT_LOCATION ("warmelt-modes.melt:4524:/ cond");
	  /*cond */ if ( /*_#IS_STRING__L22*/ meltfnum[17])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-modes.melt:4525:/ cppif.then");
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
       /*_#MELT_NEED_DBG__L23*/ meltfnum[18] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-modes.melt:4525:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L23*/ meltfnum[18])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	 /*_#MELT_CALLCOUNT__L24*/ meltfnum[23] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-modes.melt:4525:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[9];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L24*/ meltfnum[23];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-modes.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 4525;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "translateinitmodule_flavored before module generation basnam=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.BASNAM__V14*/ meltfptr[9];
			  /*^apply.arg */
			  argtab[5].meltbp_cstring = " outbase=";
			  /*^apply.arg */
			  argtab[6].meltbp_aptr =
			    (melt_ptr_t *) & /*_.OUTBASE__V35*/ meltfptr[25];
			  /*^apply.arg */
			  argtab[7].meltbp_cstring = " flavor=";
			  /*^apply.arg */
			  argtab[8].meltbp_aptr =
			    (melt_ptr_t *) & /*_.FLAVOR__V2*/ meltfptr[1];
			  /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[2])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V43*/ meltfptr[38] =
			  /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43];;
			/*epilog */

			MELT_LOCATION ("warmelt-modes.melt:4525:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L24*/ meltfnum[23] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_._IF___V43*/ meltfptr[38] = NULL;;
		    }
		  ;

		  {
		    MELT_LOCATION ("warmelt-modes.melt:4525:/ locexp");
		    /*void */ (void) 0;
		  }
		  ;
		  /*^quasiblock */


		  /*epilog */

		  /*^clear */
		 /*clear *//*_#MELT_NEED_DBG__L23*/ meltfnum[18] = 0;
		  /*^clear */
		 /*clear *//*_._IF___V43*/ meltfptr[38] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*^block */
		/*anyblock */
		{


		  {
		    /*^locexp */
		    /*void */ (void) 0;
		  }
		  ;
		  /*epilog */
		}

#endif /*MELT_HAVE_DEBUG */
		;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4527:/ locexp");
		  /*generate_flavored_melt_module */
		    melt_compile_source (melt_string_str
					 ((melt_ptr_t) /*_.BASNAM__V14*/
					  meltfptr[9]),
					 melt_string_str ((melt_ptr_t)
							  /*_.OUTBASE__V35*/
							  meltfptr[25]),
					 NULL, melt_string_str ((melt_ptr_t) /*_.FLAVOR__V2*/ meltfptr[1]));	/*generate_flavored_melt_module */
		  ;
		}
		;

#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-modes.melt:4528:/ cppif.then");
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
       /*_#MELT_NEED_DBG__L25*/ meltfnum[23] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-modes.melt:4528:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L25*/ meltfnum[23])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	 /*_#MELT_CALLCOUNT__L26*/ meltfnum[18] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-modes.melt:4528:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[9];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L26*/ meltfnum[18];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-modes.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 4528;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "translateinitmodule_flavored after module generation basnam=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.BASNAM__V14*/ meltfptr[9];
			  /*^apply.arg */
			  argtab[5].meltbp_cstring = " outbase=";
			  /*^apply.arg */
			  argtab[6].meltbp_aptr =
			    (melt_ptr_t *) & /*_.OUTBASE__V35*/ meltfptr[25];
			  /*^apply.arg */
			  argtab[7].meltbp_cstring = " flavor=";
			  /*^apply.arg */
			  argtab[8].meltbp_aptr =
			    (melt_ptr_t *) & /*_.FLAVOR__V2*/ meltfptr[1];
			  /*_.MELT_DEBUG_FUN__V46*/ meltfptr[38] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[2])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V45*/ meltfptr[43] =
			  /*_.MELT_DEBUG_FUN__V46*/ meltfptr[38];;
			/*epilog */

			MELT_LOCATION ("warmelt-modes.melt:4528:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L26*/ meltfnum[18] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V46*/ meltfptr[38] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_._IF___V45*/ meltfptr[43] = NULL;;
		    }
		  ;

		  {
		    MELT_LOCATION ("warmelt-modes.melt:4528:/ locexp");
		    /*void */ (void) 0;
		  }
		  ;
		  /*^quasiblock */


		  /*epilog */

		  /*^clear */
		 /*clear *//*_#MELT_NEED_DBG__L25*/ meltfnum[23] = 0;
		  /*^clear */
		 /*clear *//*_._IF___V45*/ meltfptr[43] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*^block */
		/*anyblock */
		{


		  {
		    /*^locexp */
		    /*void */ (void) 0;
		  }
		  ;
		  /*epilog */
		}

#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-modes.melt:4524:/ quasiblock");


		/*epilog */
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

	      /*^block */
	      /*anyblock */
	      {


#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-modes.melt:4532:/ cppif.then");
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
       /*_#MELT_NEED_DBG__L27*/ meltfnum[18] =
		    /*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
		    ( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
		    0		/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
		    ;;
		  MELT_LOCATION ("warmelt-modes.melt:4532:/ cond");
		  /*cond */ if ( /*_#MELT_NEED_DBG__L27*/ meltfnum[18])	/*then */
		    {
		      /*^cond.then */
		      /*^block */
		      /*anyblock */
		      {

	 /*_#MELT_CALLCOUNT__L28*/ meltfnum[23] =
			  /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
			  meltcallcount	/* melt_callcount debugging */
#else
			  0L	/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
			  ;;

			MELT_CHECK_SIGNAL ();
			;
			MELT_LOCATION ("warmelt-modes.melt:4532:/ apply");
			/*apply */
			{
			  union meltparam_un argtab[5];
			  memset (&argtab, 0, sizeof (argtab));
			  /*^apply.arg */
			  argtab[0].meltbp_long =
			    /*_#MELT_CALLCOUNT__L28*/ meltfnum[23];
			  /*^apply.arg */
			  argtab[1].meltbp_cstring = "warmelt-modes.melt";
			  /*^apply.arg */
			  argtab[2].meltbp_long = 4532;
			  /*^apply.arg */
			  argtab[3].meltbp_cstring =
			    "translateinitmodule_flavored bad flavor=";
			  /*^apply.arg */
			  argtab[4].meltbp_aptr =
			    (melt_ptr_t *) & /*_.FLAVOR__V2*/ meltfptr[1];
			  /*_.MELT_DEBUG_FUN__V48*/ meltfptr[43] =
			    melt_apply ((meltclosure_ptr_t)
					(( /*!MELT_DEBUG_FUN */ meltfrout->
					  tabval[2])),
					(melt_ptr_t) (( /*nil */ NULL)),
					(MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_LONG MELTBPARSTR_CSTRING
					 MELTBPARSTR_PTR ""), argtab, "",
					(union meltparam_un *) 0);
			}
			;
			/*_._IF___V47*/ meltfptr[38] =
			  /*_.MELT_DEBUG_FUN__V48*/ meltfptr[43];;
			/*epilog */

			MELT_LOCATION ("warmelt-modes.melt:4532:/ clear");
		   /*clear *//*_#MELT_CALLCOUNT__L28*/ meltfnum[23] = 0;
			/*^clear */
		   /*clear *//*_.MELT_DEBUG_FUN__V48*/ meltfptr[43] = 0;
		      }
		      ;
		    }
		  else
		    {		/*^cond.else */

	/*_._IF___V47*/ meltfptr[38] = NULL;;
		    }
		  ;

		  {
		    MELT_LOCATION ("warmelt-modes.melt:4532:/ locexp");
		    /*void */ (void) 0;
		  }
		  ;
		  /*^quasiblock */


		  /*epilog */

		  /*^clear */
		 /*clear *//*_#MELT_NEED_DBG__L27*/ meltfnum[18] = 0;
		  /*^clear */
		 /*clear *//*_._IF___V47*/ meltfptr[38] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*^block */
		/*anyblock */
		{


		  {
		    /*^locexp */
		    /*void */ (void) 0;
		  }
		  ;
		  /*epilog */
		}

#endif /*MELT_HAVE_DEBUG */
		;

		{
		  MELT_LOCATION ("warmelt-modes.melt:4533:/ locexp");
		  error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
			 ("bad flavor for translating initial module"));
		}
		;

#if MELT_HAVE_DEBUG
		MELT_LOCATION ("warmelt-modes.melt:4534:/ cppif.then");
		/*^block */
		/*anyblock */
		{


		  MELT_CHECK_SIGNAL ();
		  ;
		  /*^cond */
		  /*cond */ if (( /*nil */ NULL))	/*then */
		    {
		      /*^cond.then */
		      /*_._IFELSE___V50*/ meltfptr[38] = ( /*nil */ NULL);;
		    }
		  else
		    {
		      MELT_LOCATION ("warmelt-modes.melt:4534:/ cond.else");

		      /*^block */
		      /*anyblock */
		      {




			{
			  /*^locexp */
			  melt_assert_failed (("translateinitmodule_flavored bad flavor"), ("warmelt-modes.melt") ? ("warmelt-modes.melt") : __FILE__, (4534) ? (4534) : __LINE__, __FUNCTION__);
			  ;
			}
			;
		   /*clear *//*_._IFELSE___V50*/ meltfptr[38] = 0;
			/*epilog */
		      }
		      ;
		    }
		  ;
		  /*^compute */
		  /*_.IFCPP___V49*/ meltfptr[43] =
		    /*_._IFELSE___V50*/ meltfptr[38];;
		  /*epilog */

		  MELT_LOCATION ("warmelt-modes.melt:4534:/ clear");
		 /*clear *//*_._IFELSE___V50*/ meltfptr[38] = 0;
		}

#else /*MELT_HAVE_DEBUG */
		/*^cppif.else */
		/*_.IFCPP___V49*/ meltfptr[43] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		;
		MELT_LOCATION ("warmelt-modes.melt:4531:/ quasiblock");


		/*_.PROGN___V51*/ meltfptr[38] =
		  /*_.IFCPP___V49*/ meltfptr[43];;
		/*^compute */
		/*_._IFELSE___V42*/ meltfptr[37] =
		  /*_.PROGN___V51*/ meltfptr[38];;
		/*epilog */

		MELT_LOCATION ("warmelt-modes.melt:4524:/ clear");
	       /*clear *//*_.IFCPP___V49*/ meltfptr[43] = 0;
		/*^clear */
	       /*clear *//*_.PROGN___V51*/ meltfptr[38] = 0;
	      }
	      ;
	    }
	  ;
	  /*_._IFELSE___V36*/ meltfptr[35] =
	    /*_._IFELSE___V42*/ meltfptr[37];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-modes.melt:4513:/ clear");
	     /*clear *//*_#IS_STRING__L22*/ meltfnum[17] = 0;
	  /*^clear */
	     /*clear *//*_._IFELSE___V42*/ meltfptr[37] = 0;
	}
	;
      }
    ;
    /*_.LET___V34*/ meltfptr[29] = /*_._IFELSE___V36*/ meltfptr[35];;

    MELT_LOCATION ("warmelt-modes.melt:4509:/ clear");
	   /*clear *//*_.OUTBASE__V35*/ meltfptr[25] = 0;
    /*^clear */
	   /*clear *//*_#IS_MULTIPLE__L16*/ meltfnum[8] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V36*/ meltfptr[35] = 0;
    /*_.LET___V3*/ meltfptr[2] = /*_.LET___V34*/ meltfptr[29];;

    MELT_LOCATION ("warmelt-modes.melt:4453:/ clear");
	   /*clear *//*_.RLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_?*/ meltfram__.loc_CSTRING__o0 = 0;
    /*^clear */
	   /*clear *//*_?*/ meltfram__.loc_CSTRING__o1 = 0;
    /*^clear */
	   /*clear *//*_.INARG__V5*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_?*/ meltfram__.loc_CSTRING__o2 = 0;
    /*^clear */
	   /*clear *//*_.OUTARG__V13*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L3*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.BASNAM__V14*/ meltfptr[9] = 0;
    /*^clear */
	   /*clear *//*_#STRING_DYNLOADED_SUFFIXED__L7*/ meltfnum[5] = 0;
    /*^clear */
	   /*clear *//*_#OR___L8*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V21*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_#IS_STRING__L12*/ meltfnum[10] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V24*/ meltfptr[17] = 0;
    /*^clear */
	   /*clear *//*_.COMPILE_LIST_SEXPR__V33*/ meltfptr[15] = 0;
    /*^clear */
	   /*clear *//*_.LET___V34*/ meltfptr[29] = 0;
    MELT_LOCATION ("warmelt-modes.melt:4452:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4452:/ locexp");
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
	   /*clear *//*_.LET___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("TRANSLATEINIT_FLAVORED", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_71_WARMELTmiMODES_TRANSLATEINIT_FLAVORED */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiMODES_LAMBDA_cl23 (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_72_WARMELTmiMODES_LAMBDA_cl23_melt
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

  struct meltframe_meltrout_72_WARMELTmiMODES_LAMBDA_cl23_st
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
#define MELTFRAM_NBVARPTR 11
    melt_ptr_t mcfr_varptr[11];
#define MELTFRAM_NBVARNUM 4
    long mcfr_varnum[4];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_72_WARMELTmiMODES_LAMBDA_cl23 is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_72_WARMELTmiMODES_LAMBDA_cl23_st *)
	meltfirstargp_;
      /* use arguments meltrout_72_WARMELTmiMODES_LAMBDA_cl23 output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 11; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_72_WARMELTmiMODES_LAMBDA_cl23 nbval 11*/
  meltfram__.mcfr_nbvar = 11 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_cl23", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4493:/ getarg");
 /*_.CURARG__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-modes.melt:4494:/ locexp");
      inform (UNKNOWN_LOCATION, ("MELT INFORM [#%ld]: %s - %s"),
	      melt_dbgcounter, ("reading from file"),
	      melt_string_str ((melt_ptr_t) ( /*_.CURARG__V2*/ meltfptr[1])));
    }
    ;
    MELT_LOCATION ("warmelt-modes.melt:4495:/ quasiblock");


 /*_.CUREAD__V4*/ meltfptr[3] =
      (meltgc_read_file
       (melt_string_str ((melt_ptr_t) ( /*_.CURARG__V2*/ meltfptr[1])),
	(char *) 0));;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4497:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L1*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) (( /*~RLIST */ meltfclos->tabval[0])))
	 == MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-modes.melt:4497:/ cond");
      /*cond */ if ( /*_#IS_LIST__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V6*/ meltfptr[5] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-modes.melt:4497:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check rlist"),
				  ("warmelt-modes.melt")
				  ? ("warmelt-modes.melt") : __FILE__,
				  (4497) ? (4497) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V6*/ meltfptr[5] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
      /*epilog */

      MELT_LOCATION ("warmelt-modes.melt:4497:/ clear");
	     /*clear *//*_#IS_LIST__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V6*/ meltfptr[5] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V5*/ meltfptr[4] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4498:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST_OR_NULL__L2*/ meltfnum[0] =
	(( /*_.CUREAD__V4*/ meltfptr[3]) == NULL
	 ||
	 (melt_unsafe_magic_discr
	  ((melt_ptr_t) ( /*_.CUREAD__V4*/ meltfptr[3])) == MELTOBMAG_LIST));;
      MELT_LOCATION ("warmelt-modes.melt:4498:/ cond");
      /*cond */ if ( /*_#IS_LIST_OR_NULL__L2*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V8*/ meltfptr[7] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-modes.melt:4498:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check curead"),
				  ("warmelt-modes.melt")
				  ? ("warmelt-modes.melt") : __FILE__,
				  (4498) ? (4498) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V8*/ meltfptr[7] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[7];;
      /*epilog */

      MELT_LOCATION ("warmelt-modes.melt:4498:/ clear");
	     /*clear *//*_#IS_LIST_OR_NULL__L2*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V8*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V7*/ meltfptr[5] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4499:/ cppif.then");
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
   /*_#MELT_NEED_DBG__L3*/ meltfnum[0] =
	/*MELT_NEED_DBG */
#if MELT_HAVE_DEBUG
	( /*melt_need_dbg */ melt_need_debug ((int) 0))
#else
	0			/* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG */
	;;
      MELT_LOCATION ("warmelt-modes.melt:4499:/ cond");
      /*cond */ if ( /*_#MELT_NEED_DBG__L3*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

     /*_#MELT_CALLCOUNT__L4*/ meltfnum[3] =
	      /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
	      meltcallcount	/* melt_callcount debugging */
#else
	      0L		/* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
	      ;;

	    MELT_CHECK_SIGNAL ();
	    ;
	    MELT_LOCATION ("warmelt-modes.melt:4499:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[5];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4499;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"translateinitmodule_flavored curead=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CUREAD__V4*/ meltfptr[3];
	      /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] =
		melt_apply ((meltclosure_ptr_t)
			    (( /*!MELT_DEBUG_FUN */ meltfrout->tabval[0])),
			    (melt_ptr_t) (( /*nil */ NULL)),
			    (MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_LONG MELTBPARSTR_CSTRING
			     MELTBPARSTR_PTR ""), argtab, "",
			    (union meltparam_un *) 0);
	    }
	    ;
	    /*_._IF___V9*/ meltfptr[7] =
	      /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4499:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L4*/ meltfnum[3] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V9*/ meltfptr[7] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4499:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L3*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V9*/ meltfptr[7] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4500:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr = (melt_ptr_t *) & /*_.CUREAD__V4*/ meltfptr[3];
      /*_.LIST_APPEND2LIST__V11*/ meltfptr[9] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_APPEND2LIST */ meltfrout->tabval[1])),
		    (melt_ptr_t) (( /*~RLIST */ meltfclos->tabval[0])),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;
    /*_.LET___V3*/ meltfptr[2] = /*_.LIST_APPEND2LIST__V11*/ meltfptr[9];;

    MELT_LOCATION ("warmelt-modes.melt:4495:/ clear");
	   /*clear *//*_.CUREAD__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V5*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_.IFCPP___V7*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_.LIST_APPEND2LIST__V11*/ meltfptr[9] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4493:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4493:/ locexp");
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
	   /*clear *//*_.LET___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_cl23", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_72_WARMELTmiMODES_LAMBDA_cl23_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_72_WARMELTmiMODES_LAMBDA_cl23 */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD_melt
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

  struct meltframe_meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD_st
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
#define MELTFRAM_NBVARPTR 8
    melt_ptr_t mcfr_varptr[8];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD_st *)
	meltfirstargp_;
      /* use arguments meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 8; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD nbval 8*/
  meltfram__.mcfr_nbvar = 8 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("TRANSLATEINITMODULE_DOCMD", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4541:/ getarg");
 /*_.CMD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODULDATA__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODULDATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4542:/ cppif.then");
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
      MELT_LOCATION ("warmelt-modes.melt:4542:/ cond");
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
	    MELT_LOCATION ("warmelt-modes.melt:4542:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4542;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"start translateinitmodule_mode cmd=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CMD__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " moduldata=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODULDATA__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " initial_environment=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & ( /*!INITIAL_ENVIRONMENT */ meltfrout->
				  tabval[1]);
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
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
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4542:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4542:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-modes.melt:4544:/ blockmultialloc");
    /*multiallocblock */
    {
      struct meltletrec_1_st
      {
	struct MELT_MULTIPLE_STRUCT (3) rtup_0__TUPLREC__x2;
	long meltletrec_1_endgap;
      } *meltletrec_1_ptr = 0;
      meltletrec_1_ptr =
	(struct meltletrec_1_st *)
	meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
      /*^blockmultialloc.initfill */
      /*inimult rtup_0__TUPLREC__x2 */
 /*_.TUPLREC___V7*/ meltfptr[3] =
	(melt_ptr_t) & meltletrec_1_ptr->rtup_0__TUPLREC__x2;
      meltletrec_1_ptr->rtup_0__TUPLREC__x2.discr =
	(meltobject_ptr_t) (((melt_ptr_t) (MELT_PREDEF (DISCR_MULTIPLE))));
      meltletrec_1_ptr->rtup_0__TUPLREC__x2.nbval = 3;


      /*^putuple */
      /*putupl#4 */
      melt_assertmsg ("putupl [:4544] #4 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V7*/ meltfptr[3])) ==
		      MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:4544] #4 checkoff",
		      (0 >= 0
		       && 0 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V7*/
					      meltfptr[3]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V7*/ meltfptr[3]))->tabval[0] =
	(melt_ptr_t) (( /*!konst_3 */ meltfrout->tabval[3]));
      ;
      /*^putuple */
      /*putupl#5 */
      melt_assertmsg ("putupl [:4544] #5 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V7*/ meltfptr[3])) ==
		      MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:4544] #5 checkoff",
		      (1 >= 0
		       && 1 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V7*/
					      meltfptr[3]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V7*/ meltfptr[3]))->tabval[1] =
	(melt_ptr_t) (( /*!konst_4 */ meltfrout->tabval[4]));
      ;
      /*^putuple */
      /*putupl#6 */
      melt_assertmsg ("putupl [:4544] #6 checktup",
		      melt_magic_discr ((melt_ptr_t)
					( /*_.TUPLREC___V7*/ meltfptr[3])) ==
		      MELTOBMAG_MULTIPLE);
      melt_assertmsg ("putupl [:4544] #6 checkoff",
		      (2 >= 0
		       && 2 <
		       melt_multiple_length ((melt_ptr_t)
					     ( /*_.TUPLREC___V7*/
					      meltfptr[3]))));
      ((meltmultiple_ptr_t) ( /*_.TUPLREC___V7*/ meltfptr[3]))->tabval[2] =
	(melt_ptr_t) (( /*!konst_5 */ meltfrout->tabval[5]));
      ;
      /*^touch */
      meltgc_touch ( /*_.TUPLREC___V7*/ meltfptr[3]);
      ;
      /*_.TUPLE___V6*/ meltfptr[4] = /*_.TUPLREC___V7*/ meltfptr[3];;
      /*epilog */

      MELT_LOCATION ("warmelt-modes.melt:4544:/ clear");
	    /*clear *//*_.TUPLREC___V7*/ meltfptr[3] = 0;
      /*^clear */
	    /*clear *//*_.TUPLREC___V7*/ meltfptr[3] = 0;
    }				/*end multiallocblock */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      /*_.TRANSLATEINIT_FLAVORED__V8*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!TRANSLATEINIT_FLAVORED */ meltfrout->tabval[2])),
		    (melt_ptr_t) ( /*_.TUPLE___V6*/ meltfptr[4]), (""),
		    (union meltparam_un *) 0, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4541:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] =
      /*_.TRANSLATEINIT_FLAVORED__V8*/ meltfptr[3];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4541:/ locexp");
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
	   /*clear *//*_.TUPLE___V6*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_.TRANSLATEINIT_FLAVORED__V8*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("TRANSLATEINITMODULE_DOCMD", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_73_WARMELTmiMODES_TRANSLATEINITMODULE_DOCMD */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD_melt
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

  struct meltframe_meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD_st
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
#define MELTFRAM_NBVARPTR 6
    melt_ptr_t mcfr_varptr[6];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD_st
	 *) meltfirstargp_;
      /* use arguments meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 6; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD nbval 6*/
  meltfram__.mcfr_nbvar = 6 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("TRANSLATEINITMODULEQUICKLYBUILT_DOCMD", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4561:/ getarg");
 /*_.CMD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODULDATA__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODULDATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4562:/ cppif.then");
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
      MELT_LOCATION ("warmelt-modes.melt:4562:/ cond");
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
	    MELT_LOCATION ("warmelt-modes.melt:4562:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4562;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"start translateinitmodulequicklybuild_mode cmd=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CMD__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " moduldata=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODULDATA__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " initial_environment=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & ( /*!INITIAL_ENVIRONMENT */ meltfrout->
				  tabval[1]);
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
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
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4562:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4562:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4564:/ apply");
    /*apply */
    {
      /*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!TRANSLATEINIT_FLAVORED */ meltfrout->tabval[2])),
		    (melt_ptr_t) (( /*!konst_3 */ meltfrout->tabval[3])),
		    (""), (union meltparam_un *) 0, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4561:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] =
      /*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4561:/ locexp");
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
	   /*clear *//*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("TRANSLATEINITMODULEQUICKLYBUILT_DOCMD", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_74_WARMELTmiMODES_TRANSLATEINITMODULEQUICKLYBUILT_DOCMD */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD_melt
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

  struct meltframe_meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD_st
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
#define MELTFRAM_NBVARPTR 6
    melt_ptr_t mcfr_varptr[6];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD_st
	 *) meltfirstargp_;
      /* use arguments meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 6; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD nbval 6*/
  meltfram__.mcfr_nbvar = 6 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("TRANSLATEINITMODULEDEBUGNOLINE_DOCMD", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4579:/ getarg");
 /*_.CMD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODULDATA__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODULDATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4580:/ cppif.then");
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
      MELT_LOCATION ("warmelt-modes.melt:4580:/ cond");
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
	    MELT_LOCATION ("warmelt-modes.melt:4580:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4580;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"start translateinitmodulequicklybuild_mode cmd=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CMD__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " moduldata=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODULDATA__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " initial_environment=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & ( /*!INITIAL_ENVIRONMENT */ meltfrout->
				  tabval[1]);
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
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
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4580:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4580:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4582:/ apply");
    /*apply */
    {
      /*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!TRANSLATEINIT_FLAVORED */ meltfrout->tabval[2])),
		    (melt_ptr_t) (( /*!konst_3 */ meltfrout->tabval[3])),
		    (""), (union meltparam_un *) 0, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4579:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] =
      /*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4579:/ locexp");
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
	   /*clear *//*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("TRANSLATEINITMODULEDEBUGNOLINE_DOCMD", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_75_WARMELTmiMODES_TRANSLATEINITMODULEDEBUGNOLINE_DOCMD */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD_melt
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

  struct meltframe_meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD_st
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
#define MELTFRAM_NBVARPTR 6
    melt_ptr_t mcfr_varptr[6];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD_st
	 *) meltfirstargp_;
      /* use arguments meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 6; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD nbval 6*/
  meltfram__.mcfr_nbvar = 6 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("TRANSLATEINITMODULEOPTIMIZED_DOCMD", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-modes.melt:4597:/ getarg");
 /*_.CMD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.MODULDATA__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.MODULDATA__V3*/ meltfptr[2])) !=
	      NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-modes.melt:4598:/ cppif.then");
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
      MELT_LOCATION ("warmelt-modes.melt:4598:/ cond");
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
	    MELT_LOCATION ("warmelt-modes.melt:4598:/ apply");
	    /*apply */
	    {
	      union meltparam_un argtab[9];
	      memset (&argtab, 0, sizeof (argtab));
	      /*^apply.arg */
	      argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
	      /*^apply.arg */
	      argtab[1].meltbp_cstring = "warmelt-modes.melt";
	      /*^apply.arg */
	      argtab[2].meltbp_long = 4598;
	      /*^apply.arg */
	      argtab[3].meltbp_cstring =
		"start translateinitmodulequicklybuild_mode cmd=";
	      /*^apply.arg */
	      argtab[4].meltbp_aptr =
		(melt_ptr_t *) & /*_.CMD__V2*/ meltfptr[1];
	      /*^apply.arg */
	      argtab[5].meltbp_cstring = " moduldata=";
	      /*^apply.arg */
	      argtab[6].meltbp_aptr =
		(melt_ptr_t *) & /*_.MODULDATA__V3*/ meltfptr[2];
	      /*^apply.arg */
	      argtab[7].meltbp_cstring = " initial_environment=";
	      /*^apply.arg */
	      argtab[8].meltbp_aptr =
		(melt_ptr_t *) & ( /*!INITIAL_ENVIRONMENT */ meltfrout->
				  tabval[1]);
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =
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
	    /*_._IF___V4*/ meltfptr[3] =
	      /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-modes.melt:4598:/ clear");
	       /*clear *//*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0;
	    /*^clear */
	       /*clear *//*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0;
	  }
	  ;
	}
      else
	{			/*^cond.else */

    /*_._IF___V4*/ meltfptr[3] = NULL;;
	}
      ;

      {
	MELT_LOCATION ("warmelt-modes.melt:4598:/ locexp");
	/*void */ (void) 0;
      }
      ;
      /*^quasiblock */


      /*epilog */

      /*^clear */
	     /*clear *//*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IF___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*^block */
    /*anyblock */
    {


      {
	/*^locexp */
	/*void */ (void) 0;
      }
      ;
      /*epilog */
    }

#endif /*MELT_HAVE_DEBUG */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4600:/ apply");
    /*apply */
    {
      /*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!TRANSLATEINIT_FLAVORED */ meltfrout->tabval[2])),
		    (melt_ptr_t) (( /*!konst_3 */ meltfrout->tabval[3])),
		    (""), (union meltparam_un *) 0, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-modes.melt:4597:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] =
      /*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-modes.melt:4597:/ locexp");
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
	   /*clear *//*_.TRANSLATEINIT_FLAVORED__V6*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("TRANSLATEINITMODULEOPTIMIZED_DOCMD", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_76_WARMELTmiMODES_TRANSLATEINITMODULEOPTIMIZED_DOCMD */



/**** end of warmelt-modes+03.c ****/
