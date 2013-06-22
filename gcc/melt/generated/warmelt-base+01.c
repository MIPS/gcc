/* GCC MELT GENERATED FILE warmelt-base+01.c - DO NOT EDIT */
/* secondary MELT generated C file of rank #1 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f1[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-base+01.c declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-base ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2008 - 2013 Free Software Foundation, Inc.
    Contributed by Basile Starynkevitch <basile@starynkevitch.net>
               and Pierre Vittet  <piervit@pvittet.com>

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


/** ordinary MELT module meltbuild-sources/warmelt-base**/
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
meltrout_1_WARMELTmiBASE_plIV (meltclosure_ptr_t meltclosp_,
			       melt_ptr_t meltfirstargp_,
			       const melt_argdescr_cell_t meltxargdescr_[],
			       union meltparam_un *meltxargtab_,
			       const melt_argdescr_cell_t meltxresdescr_[],
			       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_2_WARMELTmiBASE_miIV (meltclosure_ptr_t meltclosp_,
			       melt_ptr_t meltfirstargp_,
			       const melt_argdescr_cell_t meltxargdescr_[],
			       union meltparam_un *meltxargtab_,
			       const melt_argdescr_cell_t meltxresdescr_[],
			       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_WARMELTmiBASE_stIV (meltclosure_ptr_t meltclosp_,
			       melt_ptr_t meltfirstargp_,
			       const melt_argdescr_cell_t meltxargdescr_[],
			       union meltparam_un *meltxargtab_,
			       const melt_argdescr_cell_t meltxresdescr_[],
			       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_4_WARMELTmiBASE_diIV (meltclosure_ptr_t meltclosp_,
			       melt_ptr_t meltfirstargp_,
			       const melt_argdescr_cell_t meltxargdescr_[],
			       union meltparam_un *meltxargtab_,
			       const melt_argdescr_cell_t meltxresdescr_[],
			       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_5_WARMELTmiBASE_pcIV (meltclosure_ptr_t meltclosp_,
			       melt_ptr_t meltfirstargp_,
			       const melt_argdescr_cell_t meltxargdescr_[],
			       union meltparam_un *meltxargtab_,
			       const melt_argdescr_cell_t meltxresdescr_[],
			       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_6_WARMELTmiBASE_COMPARE_NAMED_ALPHA (meltclosure_ptr_t meltclosp_,
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
meltrout_7_WARMELTmiBASE_ADD2OUT (meltclosure_ptr_t meltclosp_,
				  melt_ptr_t meltfirstargp_,
				  const melt_argdescr_cell_t meltxargdescr_[],
				  union meltparam_un *meltxargtab_,
				  const melt_argdescr_cell_t meltxresdescr_[],
				  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_8_WARMELTmiBASE_STRING4OUT (meltclosure_ptr_t meltclosp_,
				     melt_ptr_t meltfirstargp_,
				     const melt_argdescr_cell_t
				     meltxargdescr_[],
				     union meltparam_un *meltxargtab_,
				     const melt_argdescr_cell_t
				     meltxresdescr_[],
				     union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiBASE_ADD2OUT4NULL (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiBASE_ADD2OUT4STRING (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB (meltclosure_ptr_t meltclosp_,
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
meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT (meltclosure_ptr_t meltclosp_,
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
meltrout_16_WARMELTmiBASE_ADD2OUT4ANY (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un *meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST (meltclosure_ptr_t
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
meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un *meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY (meltclosure_ptr_t
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
meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH (meltclosure_ptr_t meltclosp_,
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
meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER (meltclosure_ptr_t
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
meltrout_23_WARMELTmiBASE_SET_REFERENCE (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un *meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR (meltclosure_ptr_t
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
meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST (meltclosure_ptr_t
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
meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT (meltclosure_ptr_t
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
meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE (meltclosure_ptr_t
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
meltrout_28_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST (meltclosure_ptr_t
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
meltrout_29_WARMELTmiBASE_MELT_PREDEFINED_GET (meltclosure_ptr_t meltclosp_,
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
meltrout_30_WARMELTmiBASE_SYMBOL_CNAME (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);



MELT_EXTERN void *melt_start_this_module (void *);
struct melt_initial_frame_st;

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_4 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_5 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_6 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_7 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_8 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_9 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_10 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_11 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_12 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_13 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_14 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_15 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_16 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_17 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_18 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_19 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_20 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_21 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_22 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_23 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_24 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_25 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_26 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_27 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_28 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_29 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_30 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_31 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_32 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_33 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_34 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_35 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_36 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_37 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_38 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_39 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_40 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_41 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_42 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_43 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_44 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_45 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_46 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_47 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_48 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_49 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_50 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_51 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__initialmeltchunk_52 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__WARMELTmiBASE__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking);



/**** warmelt-base+01.c implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_8_WARMELTmiBASE_STRING4OUT (meltclosure_ptr_t meltclosp_,
				     melt_ptr_t meltfirstargp_,
				     const melt_argdescr_cell_t
				     meltxargdescr_[],
				     union meltparam_un *meltxargtab_,
				     const melt_argdescr_cell_t
				     meltxresdescr_[],
				     union meltparam_un *meltxrestab_)
{
  /*variadic */ int variad_STRING4OUT_ix = 0, variad_STRING4OUT_len =
    melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_STRING4OUT_len)
#define melt_variadic_index variad_STRING4OUT_ix

  long current_blocklevel_signals_meltrout_8_WARMELTmiBASE_STRING4OUT_melt =
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

  struct meltframe_meltrout_8_WARMELTmiBASE_STRING4OUT_st
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
#define MELTFRAM_NBVARPTR 41
    melt_ptr_t mcfr_varptr[41];
#define MELTFRAM_NBVARNUM 5
    long mcfr_varnum[5];
/*others*/
    const char *loc_CSTRING__o0;
    tree loc_TREE__o1;
    gimple loc_GIMPLE__o2;
    gimple_seq loc_GIMPLE_SEQ__o3;
    edge loc_EDGE__o4;
    loop_p loc_LOOP__o5;
    const char *loc_CSTRING__o6;
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_8_WARMELTmiBASE_STRING4OUT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_8_WARMELTmiBASE_STRING4OUT_st *)
	meltfirstargp_;
      /* use arguments meltrout_8_WARMELTmiBASE_STRING4OUT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 41; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      if (meltframptr_->loc_TREE__o1)
	gt_ggc_mx_tree_node (meltframptr_->loc_TREE__o1);
      if (meltframptr_->loc_GIMPLE__o2)
	gt_ggc_mx_gimple_statement_d (meltframptr_->loc_GIMPLE__o2);
      if (meltframptr_->loc_GIMPLE_SEQ__o3)
	gt_ggc_mx_gimple_seq_d (meltframptr_->loc_GIMPLE_SEQ__o3);
      if (meltframptr_->loc_EDGE__o4)
	gt_ggc_mx_edge_def (meltframptr_->loc_EDGE__o4);
      if (meltframptr_->loc_LOOP__o5)
	gt_ggc_mx_loop (meltframptr_->loc_LOOP__o5);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_8_WARMELTmiBASE_STRING4OUT nbval 41*/
  meltfram__.mcfr_nbvar = 41 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("STRING4OUT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1352:/ getarg");
 /*_.DIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1355:/ cond");
    /*cond */ if ( /*_.DIS__V2*/ meltfptr[1])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V3*/ meltfptr[2] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-base.melt:1355:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  /*^compute */
	  /*_.DIS__V2*/ meltfptr[1] = /*_.SETQ___V4*/ meltfptr[3] =
	    ( /*!DISCR_STRING */ meltfrout->tabval[0]);;
	  /*^quasiblock */


	  /*_.PROGN___V5*/ meltfptr[4] = /*_.SETQ___V4*/ meltfptr[3];;
	  /*^compute */
	  /*_._IFELSE___V3*/ meltfptr[2] = /*_.PROGN___V5*/ meltfptr[4];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1355:/ clear");
	     /*clear *//*_.SETQ___V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V5*/ meltfptr[4] = 0;
	}
	;
      }
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_A__L1*/ meltfnum[0] =
      melt_is_instance_of ((melt_ptr_t) ( /*_.DIS__V2*/ meltfptr[1]),
			   (melt_ptr_t) (( /*!CLASS_DISCRIMINANT */
					  meltfrout->tabval[1])));;
    MELT_LOCATION ("warmelt-base.melt:1356:/ cond");
    /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V6*/ meltfptr[3] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-base.melt:1356:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-base.melt:1357:/ quasiblock");


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-base.melt:1357:/ locexp");
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
	  MELT_LOCATION ("warmelt-base.melt:1356:/ quasiblock");


	  /*_.PROGN___V8*/ meltfptr[7] = /*_.RETURN___V7*/ meltfptr[4];;
	  /*^compute */
	  /*_._IFELSE___V6*/ meltfptr[3] = /*_.PROGN___V8*/ meltfptr[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1356:/ clear");
	     /*clear *//*_.RETURN___V7*/ meltfptr[4] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V8*/ meltfptr[7] = 0;
	}
	;
      }
    ;
    MELT_LOCATION ("warmelt-base.melt:1358:/ quasiblock");


 /*_#GOODMAG__L2*/ meltfnum[1] = 0;;

    {
      MELT_LOCATION ("warmelt-base.melt:1360:/ locexp");
				   /* string4out GETMAGIDISCHK__1 *//*_#GOODMAG__L2*/ meltfnum[1] =
	((meltobject_ptr_t) /*_.DIS__V2*/ meltfptr[1])->meltobj_magic ==
	MELTOBMAG_STRING;;
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1363:/ cond");
    /*cond */ if ( /*_#GOODMAG__L2*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*_._IFELSE___V10*/ meltfptr[7] = ( /*nil */ NULL);;
      }
    else
      {
	MELT_LOCATION ("warmelt-base.melt:1363:/ cond.else");

	/*^block */
	/*anyblock */
	{

	  /*^quasiblock */


   /*_._RETVAL___V1*/ meltfptr[0] = NULL;;

	  {
	    MELT_LOCATION ("warmelt-base.melt:1363:/ locexp");
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
	  /*^quasiblock */


	  /*_.PROGN___V12*/ meltfptr[11] = /*_.RETURN___V11*/ meltfptr[10];;
	  /*^compute */
	  /*_._IFELSE___V10*/ meltfptr[7] = /*_.PROGN___V12*/ meltfptr[11];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1363:/ clear");
	     /*clear *//*_.RETURN___V11*/ meltfptr[10] = 0;
	  /*^clear */
	     /*clear *//*_.PROGN___V12*/ meltfptr[11] = 0;
	}
	;
      }
    ;
    /*_.LET___V9*/ meltfptr[4] = /*_._IFELSE___V10*/ meltfptr[7];;

    MELT_LOCATION ("warmelt-base.melt:1358:/ clear");
	   /*clear *//*_#GOODMAG__L2*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V10*/ meltfptr[7] = 0;
    MELT_LOCATION ("warmelt-base.melt:1364:/ quasiblock");


 /*_.OUT__V14*/ meltfptr[11] =
      (melt_ptr_t)
      meltgc_new_strbuf ((meltobject_ptr_t)
			 (( /*!DISCR_STRBUF */ meltfrout->tabval[2])),
			 (const char *) 0);;
    MELT_LOCATION ("warmelt-base.melt:1366:/ loop");
    /*loop */
    {
    meltlabloop_ARGLOOP_2:;	/*^loopbody */

      /*^block */
      /*anyblock */
      {


	MELT_CHECK_SIGNAL ();
	;
	MELT_LOCATION ("warmelt-base.melt:1368:/ cond");
	/*cond */ if ( /*ifvariadic nomore */ variad_STRING4OUT_ix == variad_STRING4OUT_len)	/*then */
	  {
	    /*^cond.then */
	    /*^block */
	    /*anyblock */
	    {

	      /*^compute */

	      /*consume variadic  ! */ variad_STRING4OUT_ix += 0;;
	      MELT_LOCATION ("warmelt-base.melt:1370:/ quasiblock");


	      /*^compute */
     /*_.ARGLOOP__V16*/ meltfptr[15] = NULL;;

	      /*^exit */
	      /*exit */
	      {
		goto meltlabexit_ARGLOOP_2;
	      }
	      ;
	      /*epilog */
	    }
	    ;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-base.melt:1368:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {

	      /*^cond */
	      /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_PTR)	/*then */
		{
		  /*^cond.then */
		  /*^block */
		  /*anyblock */
		  {

		    /*^compute */
       /*_.V__V17*/ meltfptr[16] =
		      /*variadic argument value */
		      ((meltxargtab_[variad_STRING4OUT_ix + 0].
			meltbp_aptr)
		       ? (*
			  (meltxargtab_[variad_STRING4OUT_ix + 0].
			   meltbp_aptr)) : NULL);;
		    /*^compute */

		    /*consume variadic Value ! */ variad_STRING4OUT_ix += 1;;

		    MELT_CHECK_SIGNAL ();
		    ;
       /*_#IS_CLOSURE__L3*/ meltfnum[1] =
		      (melt_magic_discr
		       ((melt_ptr_t) ( /*_.V__V17*/ meltfptr[16])) ==
		       MELTOBMAG_CLOSURE);;
		    MELT_LOCATION ("warmelt-base.melt:1372:/ cond");
		    /*cond */ if ( /*_#IS_CLOSURE__L3*/ meltfnum[1])	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  MELT_LOCATION ("warmelt-base.melt:1373:/ cond");
			  /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_PTR)	/*then */
			    {
			      /*^cond.then */
			      /*^block */
			      /*anyblock */
			      {

				/*^compute */
	   /*_.VV__V19*/ meltfptr[18] =
				  /*variadic argument value */
				  ((meltxargtab_[variad_STRING4OUT_ix + 0].
				    meltbp_aptr)
				   ? (*
				      (meltxargtab_[variad_STRING4OUT_ix + 0].
				       meltbp_aptr)) : NULL);;
				/*^compute */

				/*consume variadic Value ! */
				  variad_STRING4OUT_ix += 1;;

				MELT_CHECK_SIGNAL ();
				;
				MELT_LOCATION
				  ("warmelt-base.melt:1375:/ apply");
				/*apply */
				{
				  union meltparam_un argtab[1];
				  memset (&argtab, 0, sizeof (argtab));
				  /*^apply.arg */
				  argtab[0].meltbp_aptr =
				    (melt_ptr_t *) & /*_.VV__V19*/
				    meltfptr[18];
				  /*_.V__V20*/ meltfptr[19] =
				    melt_apply ((meltclosure_ptr_t)
						( /*_.V__V17*/ meltfptr[16]),
						(melt_ptr_t) ( /*_.OUT__V14*/
							      meltfptr[11]),
						(MELTBPARSTR_PTR ""), argtab,
						"", (union meltparam_un *) 0);
				}
				;
				/*_._IFELSE___V18*/ meltfptr[17] =
				  /*_.V__V20*/ meltfptr[19];;
				/*epilog */

				MELT_LOCATION
				  ("warmelt-base.melt:1373:/ clear");
		     /*clear *//*_.VV__V19*/ meltfptr[18] = 0;
				/*^clear */
		     /*clear *//*_.V__V20*/ meltfptr[19] = 0;
			      }
			      ;
			    }
			  else
			    {	/*^cond.else */

			      /*^block */
			      /*anyblock */
			      {

				/*^cond */
				/*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_LONG)	/*then */
				  {
				    /*^cond.then */
				    /*^block */
				    /*anyblock */
				    {

				      /*^compute */
	     /*_#LL__L4*/ meltfnum[3] =
					/*variadic argument stuff */
					meltxargtab_[variad_STRING4OUT_ix +
						     0].meltbp_long;;
				      /*^compute */

				      /*consume variadic LONG ! */
					variad_STRING4OUT_ix += 1;;

				      MELT_CHECK_SIGNAL ();
				      ;
				      MELT_LOCATION
					("warmelt-base.melt:1377:/ apply");
				      /*apply */
				      {
					union meltparam_un argtab[1];
					memset (&argtab, 0, sizeof (argtab));
					/*^apply.arg */
					argtab[0].meltbp_long =
					  /*_#LL__L4*/ meltfnum[3];
					/*_.V__V21*/ meltfptr[18] =
					  melt_apply ((meltclosure_ptr_t)
						      ( /*_.V__V17*/
						       meltfptr[16]),
						      (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un *) 0);
				      }
				      ;
				      /*_._IFELSE___V18*/ meltfptr[17] =
					/*_.V__V21*/ meltfptr[18];;
				      /*epilog */

				      MELT_LOCATION
					("warmelt-base.melt:1373:/ clear");
		       /*clear *//*_#LL__L4*/ meltfnum[3] = 0;
				      /*^clear */
		       /*clear *//*_.V__V21*/ meltfptr[18] = 0;
				    }
				    ;
				  }
				else
				  {	/*^cond.else */

				    /*^block */
				    /*anyblock */
				    {

				      /*^cond */
				      /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_CSTRING)	/*then */
					{
					  /*^cond.then */
					  /*^block */
					  /*anyblock */
					  {

					    /*^compute */
	       /*_?*/ meltfram__.
					      loc_CSTRING__o0 =
					      /*variadic argument stuff */
					      meltxargtab_
					      [variad_STRING4OUT_ix +
					       0].meltbp_cstring;;
					    /*^compute */

					    /*consume variadic CSTRING ! */
					      variad_STRING4OUT_ix += 1;;

					    MELT_CHECK_SIGNAL ();
					    ;
					    MELT_LOCATION
					      ("warmelt-base.melt:1379:/ apply");
					    /*apply */
					    {
					      union meltparam_un argtab[1];
					      memset (&argtab, 0,
						      sizeof (argtab));
					      /*^apply.arg */
					      argtab[0].meltbp_cstring =
						/*_?*/
						meltfram__.loc_CSTRING__o0;
					      /*_.V__V22*/ meltfptr[19] =
						melt_apply ((meltclosure_ptr_t) ( /*_.V__V17*/ meltfptr[16]), (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un *) 0);
					    }
					    ;
					    /*_._IFELSE___V18*/ meltfptr[17] =
					      /*_.V__V22*/ meltfptr[19];;
					    /*epilog */

					    MELT_LOCATION
					      ("warmelt-base.melt:1373:/ clear");
			 /*clear *//*_?*/ meltfram__.
					      loc_CSTRING__o0 = 0;
					    /*^clear */
			 /*clear *//*_.V__V22*/
					      meltfptr[19] = 0;
					  }
					  ;
					}
				      else
					{	/*^cond.else */

					  /*^block */
					  /*anyblock */
					  {

					    /*^cond */
					    /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_TREE)	/*then */
					      {
						/*^cond.then */
						/*^block */
						/*anyblock */
						{

						  /*^compute */
		 /*_?*/ meltfram__.
						    loc_TREE__o1 =
						    /*variadic argument stuff */
						    meltxargtab_
						    [variad_STRING4OUT_ix +
						     0].meltbp_tree;;
						  /*^compute */

						  /*consume variadic TREE ! */
						    variad_STRING4OUT_ix +=
						    1;;

						  MELT_CHECK_SIGNAL ();
						  ;
						  MELT_LOCATION
						    ("warmelt-base.melt:1381:/ apply");
						  /*apply */
						  {
						    union meltparam_un
						      argtab[1];
						    memset (&argtab, 0,
							    sizeof (argtab));
						    /*^apply.arg */
						    argtab[0].meltbp_tree =
						      /*_?*/
						      meltfram__.loc_TREE__o1;
						    /*_.V__V23*/ meltfptr[18]
						      =
						      melt_apply ((meltclosure_ptr_t) ( /*_.V__V17*/ meltfptr[16]), (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un *) 0);
						  }
						  ;
						  /*_._IFELSE___V18*/
						    meltfptr[17] =
						    /*_.V__V23*/
						    meltfptr[18];;
						  /*epilog */

						  MELT_LOCATION
						    ("warmelt-base.melt:1373:/ clear");
			   /*clear *//*_?*/ meltfram__.
						    loc_TREE__o1 = 0;
						  /*^clear */
			   /*clear *//*_.V__V23*/
						    meltfptr[18] = 0;
						}
						;
					      }
					    else
					      {	/*^cond.else */

						/*^block */
						/*anyblock */
						{

						  /*^cond */
						  /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_GIMPLE)	/*then */
						    {
						      /*^cond.then */
						      /*^block */
						      /*anyblock */
						      {

							/*^compute */
		   /*_?*/ meltfram__.
							  loc_GIMPLE__o2 =
							  /*variadic argument stuff */
							  meltxargtab_
							  [variad_STRING4OUT_ix
							   +
							   0].meltbp_gimple;;
							/*^compute */

							/*consume variadic GIMPLE ! */
							  variad_STRING4OUT_ix
							  += 1;;

							MELT_CHECK_SIGNAL ();
							;
							MELT_LOCATION
							  ("warmelt-base.melt:1383:/ apply");
							/*apply */
							{
							  union meltparam_un
							    argtab[1];
							  memset (&argtab, 0,
								  sizeof
								  (argtab));
							  /*^apply.arg */
							  argtab[0].
							    meltbp_gimple =
							    /*_?*/
							    meltfram__.
							    loc_GIMPLE__o2;
							  /*_.V__V24*/
							    meltfptr[19] =
							    melt_apply ((meltclosure_ptr_t) ( /*_.V__V17*/ meltfptr[16]), (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un *) 0);
							}
							;
							/*_._IFELSE___V18*/
							  meltfptr[17] =
							  /*_.V__V24*/
							  meltfptr[19];;
							/*epilog */

							MELT_LOCATION
							  ("warmelt-base.melt:1373:/ clear");
			     /*clear *//*_?*/
							  meltfram__.
							  loc_GIMPLE__o2 = 0;
							/*^clear */
			     /*clear *//*_.V__V24*/
							  meltfptr[19] = 0;
						      }
						      ;
						    }
						  else
						    {	/*^cond.else */

						      /*^block */
						      /*anyblock */
						      {

							/*^cond */
							/*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_GIMPLESEQ)	/*then */
							  {
							    /*^cond.then */
							    /*^block */
							    /*anyblock */
							    {

							      /*^compute */
		     /*_?*/
								meltfram__.
								loc_GIMPLE_SEQ__o3
								=
								/*variadic argument stuff */
								meltxargtab_
								[variad_STRING4OUT_ix
								 +
								 0].
								meltbp_gimpleseq;;
							      /*^compute */

							      /*consume variadic GIMPLE_SEQ ! */
								variad_STRING4OUT_ix
								+= 1;;

							      MELT_CHECK_SIGNAL
								();
							      ;
							      MELT_LOCATION
								("warmelt-base.melt:1385:/ apply");
							      /*apply */
							      {
								union
								  meltparam_un
								  argtab[1];
								memset
								  (&argtab, 0,
								   sizeof
								   (argtab));
								/*^apply.arg */
								argtab[0].
								  meltbp_gimpleseq
								  =
								  /*_?*/
								  meltfram__.
								  loc_GIMPLE_SEQ__o3;
								/*_.V__V25*/
								  meltfptr[18]
								  =
								  melt_apply ((meltclosure_ptr_t) ( /*_.V__V17*/ meltfptr[16]), (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un *) 0);
							      }
							      ;
							      /*_._IFELSE___V18*/
								meltfptr[17] =
								/*_.V__V25*/
								meltfptr[18];;
							      /*epilog */

							      MELT_LOCATION
								("warmelt-base.melt:1373:/ clear");
			       /*clear *//*_?*/
								meltfram__.
								loc_GIMPLE_SEQ__o3
								= 0;
							      /*^clear */
			       /*clear *//*_.V__V25*/
								meltfptr
								[18] = 0;
							    }
							    ;
							  }
							else
							  {	/*^cond.else */

							    /*^block */
							    /*anyblock */
							    {

							      /*^cond */
							      /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_EDGE)	/*then */
								{
								  /*^cond.then */
								  /*^block */
								  /*anyblock */
								  {

								    /*^compute */
		       /*_?*/
								      meltfram__.
								      loc_EDGE__o4
								      =
								      /*variadic argument stuff */
								      meltxargtab_
								      [variad_STRING4OUT_ix
								       +
								       0].
								      meltbp_edge;;
								    /*^compute */

								    /*consume variadic EDGE ! */
								      variad_STRING4OUT_ix
								      += 1;;

								    MELT_CHECK_SIGNAL
								      ();
								    ;
								    MELT_LOCATION
								      ("warmelt-base.melt:1387:/ apply");
								    /*apply */
								    {
								      union
									meltparam_un
									argtab
									[1];
								      memset
									(&argtab,
									 0,
									 sizeof
									 (argtab));
								      /*^apply.arg */
								      argtab
									[0].
									meltbp_edge
									=
									/*_?*/
									meltfram__.
									loc_EDGE__o4;
								      /*_.V__V26*/
									meltfptr
									[19] =
									melt_apply
									((meltclosure_ptr_t) ( /*_.V__V17*/ meltfptr[16]), (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un *) 0);
								    }
								    ;
								    /*_._IFELSE___V18*/
								      meltfptr
								      [17] =
								      /*_.V__V26*/
								      meltfptr
								      [19];;
								    /*epilog */

								    MELT_LOCATION
								      ("warmelt-base.melt:1373:/ clear");
				 /*clear *//*_?*/
								      meltfram__.
								      loc_EDGE__o4
								      = 0;
								    /*^clear */
				 /*clear *//*_.V__V26*/
								      meltfptr
								      [19]
								      = 0;
								  }
								  ;
								}
							      else
								{	/*^cond.else */

								  /*^block */
								  /*anyblock */
								  {

								    /*^cond */
								    /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_LOOP)	/*then */
								      {
									/*^cond.then */
									/*^block */
									/*anyblock */
									{

									  /*^compute */
			 /*_?*/
									    meltfram__.
									    loc_LOOP__o5
									    =
									    /*variadic argument stuff */
									    meltxargtab_
									    [variad_STRING4OUT_ix
									     +
									     0].
									    meltbp_loop;;
									  /*^compute */

									  /*consume variadic LOOP ! */
									    variad_STRING4OUT_ix
									    +=
									    1;;

									  MELT_CHECK_SIGNAL
									    ();
									  ;
									  MELT_LOCATION
									    ("warmelt-base.melt:1389:/ apply");
									  /*apply */
									  {
									    union
									      meltparam_un
									      argtab
									      [1];
									    memset
									      (&argtab,
									       0,
									       sizeof
									       (argtab));
									    /*^apply.arg */
									    argtab
									      [0].
									      meltbp_loop
									      =
									      /*_?*/
									      meltfram__.
									      loc_LOOP__o5;
									    /*_.V__V27*/
									      meltfptr
									      [18]
									      =
									      melt_apply
									      ((meltclosure_ptr_t) ( /*_.V__V17*/ meltfptr[16]), (melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un *) 0);
									  }
									  ;
									  /*_._IFELSE___V18*/
									    meltfptr
									    [17]
									    =
									    /*_.V__V27*/
									    meltfptr
									    [18];;
									  /*epilog */

									  MELT_LOCATION
									    ("warmelt-base.melt:1373:/ clear");
				   /*clear *//*_?*/
									    meltfram__.
									    loc_LOOP__o5
									    =
									    0;
									  /*^clear */
				   /*clear *//*_.V__V27*/
									    meltfptr
									    [18]
									    =
									    0;
									}
									;
								      }
								    else
								      {	/*^cond.else */

									/*^block */
									/*anyblock */
									{

									  MELT_LOCATION
									    ("warmelt-base.melt:1391:/ quasiblock");


			 /*_.VCTY__V29*/
									    meltfptr
									    [18]
									    =
									    /*variadic_type_code */
#ifdef melt_variadic_index
									    (((melt_variadic_index + 0) >= 0 && (melt_variadic_index + 0) < melt_variadic_length) ? melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0] & MELT_ARGDESCR_MAX) : NULL)
#else
									    NULL	/* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index */
									    ;;
									  MELT_LOCATION
									    ("warmelt-base.melt:1394:/ cond");
									  /*cond */ if (
											 /*ifisa */
											 melt_is_instance_of
											 ((melt_ptr_t) ( /*_.VCTY__V29*/ meltfptr[18]),
											  (melt_ptr_t) (( /*!CLASS_NAMED */ meltfrout->tabval[3])))
									    )	/*then */
									    {
									      /*^cond.then */
									      /*^getslot */
									      {
										melt_ptr_t
										  slot
										  =
										  NULL,
										  obj
										  =
										  NULL;
										obj
										  =
										  (melt_ptr_t)
										  ( /*_.VCTY__V29*/ meltfptr[18]) /*=obj*/ ;
										melt_object_get_field
										  (slot,
										   obj,
										   1,
										   "NAMED_NAME");
			   /*_.NAMED_NAME__V30*/
										  meltfptr
										  [29]
										  =
										  slot;
									      };
									      ;
									    }
									  else
									    {	/*^cond.else */

			  /*_.NAMED_NAME__V30*/
										meltfptr
										[29]
										=
										NULL;;
									    }
									  ;

									  {
									    MELT_LOCATION
									      ("warmelt-base.melt:1393:/ locexp");
									    error
									      ("MELT ERROR MSG [#%ld]::: %s - %s",
									       melt_dbgcounter,
									       ("STRING4OUT with manipulator for unsupported ctype"),
									       melt_string_str
									       ((melt_ptr_t) ( /*_.NAMED_NAME__V30*/ meltfptr[29])));
									  }
									  ;

#if MELT_HAVE_DEBUG
									  MELT_LOCATION
									    ("warmelt-base.melt:1395:/ cppif.then");
									  /*^block */
									  /*anyblock */
									  {


									    MELT_CHECK_SIGNAL
									      ();
									    ;
									    /*^cond */
									    /*cond */ if (( /*nil */ NULL))	/*then */
									      {
										/*^cond.then */
										/*_._IFELSE___V32*/
										  meltfptr
										  [31]
										  =
										  ( /*nil */ NULL);;
									      }
									    else
									      {
										MELT_LOCATION
										  ("warmelt-base.melt:1395:/ cond.else");

										/*^block */
										/*anyblock */
										{




										  {
										    /*^locexp */
										    melt_assert_failed
										      (("invalid variadic argument after closure to STRING4OUT"), ("warmelt-base.melt") ? ("warmelt-base.melt") : __FILE__, (1395) ? (1395) : __LINE__, __FUNCTION__);
										    ;
										  }
										  ;
				       /*clear *//*_._IFELSE___V32*/
										    meltfptr
										    [31]
										    =
										    0;
										  /*epilog */
										}
										;
									      }
									    ;
									    /*^compute */
									    /*_.IFCPP___V31*/
									      meltfptr
									      [30]
									      =
									      /*_._IFELSE___V32*/
									      meltfptr
									      [31];;
									    /*epilog */

									    MELT_LOCATION
									      ("warmelt-base.melt:1395:/ clear");
				     /*clear *//*_._IFELSE___V32*/
									      meltfptr
									      [31]
									      =
									      0;
									  }

#else /*MELT_HAVE_DEBUG */
									  /*^cppif.else */
									  /*_.IFCPP___V31*/
									    meltfptr
									    [30]
									    =
									    ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
									  ;
									  MELT_LOCATION
									    ("warmelt-base.melt:1396:/ quasiblock");


			 /*_._RETVAL___V1*/
									    meltfptr
									    [0]
									    =
									    NULL;;

									  {
									    MELT_LOCATION
									      ("warmelt-base.melt:1396:/ locexp");
									    /*ochecknores compilobj_nrep_return */
#if MELT_HAVE_DEBUG
									    if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
									      melt_warn_for_no_expected_secondary_results
										();
/* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG */
									    ;
									  }
									  ;
									  /*^finalreturn */
									  ;
									  /*finalret */
									    goto
									    meltlabend_rout;
									  /*_.LET___V28*/
									    meltfptr
									    [19]
									    =
									    /*_.RETURN___V33*/
									    meltfptr
									    [31];;

									  MELT_LOCATION
									    ("warmelt-base.melt:1391:/ clear");
				   /*clear *//*_.VCTY__V29*/
									    meltfptr
									    [18]
									    =
									    0;
									  /*^clear */
				   /*clear *//*_.NAMED_NAME__V30*/
									    meltfptr
									    [29]
									    =
									    0;
									  /*^clear */
				   /*clear *//*_.IFCPP___V31*/
									    meltfptr
									    [30]
									    =
									    0;
									  /*^clear */
				   /*clear *//*_.RETURN___V33*/
									    meltfptr
									    [31]
									    =
									    0;
									  /*_._IFELSE___V18*/
									    meltfptr
									    [17]
									    =
									    /*_.LET___V28*/
									    meltfptr
									    [19];;
									  /*epilog */

									  MELT_LOCATION
									    ("warmelt-base.melt:1373:/ clear");
				   /*clear *//*_.LET___V28*/
									    meltfptr
									    [19]
									    =
									    0;
									}
									;
								      }
								    ;
								    /*epilog */
								  }
								  ;
								}
							      ;
							      /*epilog */
							    }
							    ;
							  }
							;
							/*epilog */
						      }
						      ;
						    }
						  ;
						  /*epilog */
						}
						;
					      }
					    ;
					    /*epilog */
					  }
					  ;
					}
				      ;
				      /*epilog */
				    }
				    ;
				  }
				;
				/*epilog */
			      }
			      ;
			    }
			  ;
			  /*epilog */
			}
			;
		      }
		    else
		      {
			MELT_LOCATION ("warmelt-base.melt:1372:/ cond.else");

			/*^block */
			/*anyblock */
			{


			  MELT_CHECK_SIGNAL ();
			  ;
			  MELT_LOCATION ("warmelt-base.melt:1398:/ msend");
			  /*msend */
			  {
			    union meltparam_un argtab[1];
			    memset (&argtab, 0, sizeof (argtab));
			    /*^ojbmsend.arg */
			    argtab[0].meltbp_aptr =
			      (melt_ptr_t *) & /*_.OUT__V14*/ meltfptr[11];
			    /*_.ADD_TO_OUT__V34*/ meltfptr[18] =
			      meltgc_send ((melt_ptr_t)
					   ( /*_.V__V17*/ meltfptr[16]),
					   (melt_ptr_t) (( /*!ADD_TO_OUT */
							  meltfrout->
							  tabval[4])),
					   (MELTBPARSTR_PTR ""), argtab, "",
					   (union meltparam_un *) 0);
			  }
			  ;
			  /*_._IFELSE___V18*/ meltfptr[17] =
			    /*_.ADD_TO_OUT__V34*/ meltfptr[18];;
			  /*epilog */

			  MELT_LOCATION ("warmelt-base.melt:1372:/ clear");
		   /*clear *//*_.ADD_TO_OUT__V34*/ meltfptr[18] = 0;
			}
			;
		      }
		    ;
		    /*epilog */

		    MELT_LOCATION ("warmelt-base.melt:1368:/ clear");
		 /*clear *//*_.V__V17*/ meltfptr[16] = 0;
		    /*^clear */
		 /*clear *//*_#IS_CLOSURE__L3*/ meltfnum[1] = 0;
		    /*^clear */
		 /*clear *//*_._IFELSE___V18*/ meltfptr[17] = 0;
		  }
		  ;
		}
	      else
		{		/*^cond.else */

		  /*^block */
		  /*anyblock */
		  {

		    /*^cond */
		    /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_LONG)	/*then */
		      {
			/*^cond.then */
			/*^block */
			/*anyblock */
			{

			  /*^compute */
	 /*_#N__L5*/ meltfnum[3] =
			    /*variadic argument stuff */
			    meltxargtab_[variad_STRING4OUT_ix +
					 0].meltbp_long;;
			  /*^compute */

			  /*consume variadic LONG ! */ variad_STRING4OUT_ix +=
			    1;;

			  {
			    MELT_LOCATION ("warmelt-base.melt:1400:/ locexp");
			    meltgc_add_out_dec ((melt_ptr_t)
						( /*_.OUT__V14*/
						 meltfptr[11]),
						( /*_#N__L5*/ meltfnum[3]));
			  }
			  ;
			  /*epilog */

			  MELT_LOCATION ("warmelt-base.melt:1368:/ clear");
		   /*clear *//*_#N__L5*/ meltfnum[3] = 0;
			}
			;
		      }
		    else
		      {		/*^cond.else */

			/*^block */
			/*anyblock */
			{

			  /*^cond */
			  /*cond */ if ( /*ifvariadic arg#1 */ variad_STRING4OUT_ix >= 0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix] == MELTBPAR_CSTRING)	/*then */
			    {
			      /*^cond.then */
			      /*^block */
			      /*anyblock */
			      {

				/*^compute */
	   /*_?*/ meltfram__.loc_CSTRING__o6 =
				  /*variadic argument stuff */
				  meltxargtab_[variad_STRING4OUT_ix +
					       0].meltbp_cstring;;
				/*^compute */

				/*consume variadic CSTRING ! */
				  variad_STRING4OUT_ix += 1;;

				{
				  MELT_LOCATION
				    ("warmelt-base.melt:1402:/ locexp");
				  meltgc_add_out ((melt_ptr_t)
						  ( /*_.OUT__V14*/
						   meltfptr[11]),
						  ( /*_?*/ meltfram__.
						   loc_CSTRING__o6));
				}
				;
				/*epilog */

				MELT_LOCATION
				  ("warmelt-base.melt:1368:/ clear");
		     /*clear *//*_?*/ meltfram__.loc_CSTRING__o6 =
				  0;
			      }
			      ;
			    }
			  else
			    {	/*^cond.else */

			      /*^block */
			      /*anyblock */
			      {

				MELT_LOCATION
				  ("warmelt-base.melt:1404:/ quasiblock");


	   /*_.VCTY__V35*/ meltfptr[29] =
				  /*variadic_type_code */
#ifdef melt_variadic_index
				  (((melt_variadic_index + 0) >= 0
				    && (melt_variadic_index + 0) <
				    melt_variadic_length) ?
				   melt_code_to_ctype (meltxargdescr_
						       [melt_variadic_index +
							0] &
						       MELT_ARGDESCR_MAX) :
				   NULL)
#else
				  NULL	/* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index */
				  ;;
				MELT_LOCATION
				  ("warmelt-base.melt:1407:/ cond");
				/*cond */ if (
					       /*ifisa */
					       melt_is_instance_of ((melt_ptr_t) ( /*_.VCTY__V35*/ meltfptr[29]),
								    (melt_ptr_t) (( /*!CLASS_NAMED */ meltfrout->tabval[3])))
				  )	/*then */
				  {
				    /*^cond.then */
				    /*^getslot */
				    {
				      melt_ptr_t slot = NULL, obj = NULL;
				      obj =
					(melt_ptr_t) ( /*_.VCTY__V35*/
						      meltfptr[29]) /*=obj*/ ;
				      melt_object_get_field (slot, obj, 1,
							     "NAMED_NAME");
	     /*_.NAMED_NAME__V36*/ meltfptr[30] =
					slot;
				    };
				    ;
				  }
				else
				  {	/*^cond.else */

	    /*_.NAMED_NAME__V36*/ meltfptr[30] =
				      NULL;;
				  }
				;

				{
				  MELT_LOCATION
				    ("warmelt-base.melt:1406:/ locexp");
				  error ("MELT ERROR MSG [#%ld]::: %s - %s",
					 melt_dbgcounter,
					 ("STRING4OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
					 melt_string_str ((melt_ptr_t)
							  ( /*_.NAMED_NAME__V36*/ meltfptr[30])));
				}
				;

				MELT_LOCATION
				  ("warmelt-base.melt:1404:/ clear");
		     /*clear *//*_.VCTY__V35*/ meltfptr[29] = 0;
				/*^clear */
		     /*clear *//*_.NAMED_NAME__V36*/ meltfptr[30] =
				  0;

#if MELT_HAVE_DEBUG
				MELT_LOCATION
				  ("warmelt-base.melt:1409:/ cppif.then");
				/*^block */
				/*anyblock */
				{


				  MELT_CHECK_SIGNAL ();
				  ;
				  /*^cond */
				  /*cond */ if (( /*nil */ NULL))	/*then */
				    {
				      /*^cond.then */
				      /*_._IFELSE___V38*/ meltfptr[19] =
					( /*nil */ NULL);;
				    }
				  else
				    {
				      MELT_LOCATION
					("warmelt-base.melt:1409:/ cond.else");

				      /*^block */
				      /*anyblock */
				      {




					{
					  /*^locexp */
					  melt_assert_failed (("invalid variadic argument to STRING4OUT"), ("warmelt-base.melt") ? ("warmelt-base.melt") : __FILE__, (1409) ? (1409) : __LINE__, __FUNCTION__);
					  ;
					}
					;
			 /*clear *//*_._IFELSE___V38*/
					  meltfptr[19] = 0;
					/*epilog */
				      }
				      ;
				    }
				  ;
				  /*^compute */
				  /*_.IFCPP___V37*/ meltfptr[31] =
				    /*_._IFELSE___V38*/ meltfptr[19];;
				  /*epilog */

				  MELT_LOCATION
				    ("warmelt-base.melt:1409:/ clear");
		       /*clear *//*_._IFELSE___V38*/ meltfptr[19] =
				    0;
				}

#else /*MELT_HAVE_DEBUG */
				/*^cppif.else */
				/*_.IFCPP___V37*/ meltfptr[31] =
				  ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
				;

				MELT_CHECK_SIGNAL ();
				;
				/*epilog */

				MELT_LOCATION
				  ("warmelt-base.melt:1368:/ clear");
		     /*clear *//*_.IFCPP___V37*/ meltfptr[31] = 0;
			      }
			      ;
			    }
			  ;
			  /*epilog */
			}
			;
		      }
		    ;
		    /*epilog */
		  }
		  ;
		}
	      ;
	      /*epilog */
	    }
	    ;
	  }
	;
	/*epilog */
      }
      ;
      ;
      goto meltlabloop_ARGLOOP_2;
    meltlabexit_ARGLOOP_2:;
      MELT_LOCATION ("warmelt-base.melt:1366:/ loopepilog");
      /*loopepilog */
      /*_.FOREVER___V15*/ meltfptr[7] = /*_.ARGLOOP__V16*/ meltfptr[15];;
    }
    ;
    MELT_LOCATION ("warmelt-base.melt:1411:/ quasiblock");


 /*_.STR__V40*/ meltfptr[16] =
      (meltgc_new_stringdup
       ((meltobject_ptr_t) ( /*_.DIS__V2*/ meltfptr[1]),
	melt_strbuf_str ((melt_ptr_t) ( /*_.OUT__V14*/ meltfptr[11]))));;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1413:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.STR__V40*/ meltfptr[16];;

    {
      MELT_LOCATION ("warmelt-base.melt:1413:/ locexp");
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
    /*_.LET___V39*/ meltfptr[18] = /*_.RETURN___V41*/ meltfptr[17];;

    MELT_LOCATION ("warmelt-base.melt:1411:/ clear");
	   /*clear *//*_.STR__V40*/ meltfptr[16] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V41*/ meltfptr[17] = 0;
    /*_.LET___V13*/ meltfptr[10] = /*_.LET___V39*/ meltfptr[18];;

    MELT_LOCATION ("warmelt-base.melt:1364:/ clear");
	   /*clear *//*_.OUT__V14*/ meltfptr[11] = 0;
    /*^clear */
	   /*clear *//*_.FOREVER___V15*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.LET___V39*/ meltfptr[18] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1352:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[10];;

    {
      MELT_LOCATION ("warmelt-base.melt:1352:/ locexp");
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
	   /*clear *//*_._IFELSE___V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IFELSE___V6*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.LET___V9*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_.LET___V13*/ meltfptr[10] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("STRING4OUT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_8_WARMELTmiBASE_STRING4OUT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_8_WARMELTmiBASE_STRING4OUT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiBASE_ADD2OUT4NULL (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un * meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_9_WARMELTmiBASE_ADD2OUT4NULL_melt =
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

  struct meltframe_meltrout_9_WARMELTmiBASE_ADD2OUT4NULL_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_9_WARMELTmiBASE_ADD2OUT4NULL is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_9_WARMELTmiBASE_ADD2OUT4NULL_st *)
	meltfirstargp_;
      /* use arguments meltrout_9_WARMELTmiBASE_ADD2OUT4NULL output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_9_WARMELTmiBASE_ADD2OUT4NULL nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4NULL", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1419:/ getarg");
 /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-base.melt:1420:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#NULL__L1*/ meltfnum[0] =
	(( /*_.V__V2*/ meltfptr[1]) == NULL);;
      MELT_LOCATION ("warmelt-base.melt:1420:/ cond");
      /*cond */ if ( /*_#NULL__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V5*/ meltfptr[4] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1420:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check null v"),
				  ("warmelt-base.melt")
				  ? ("warmelt-base.melt") : __FILE__,
				  (1420) ? (1420) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-base.melt:1420:/ clear");
	     /*clear *//*_#NULL__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1421:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]), ("*nil*"));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1419:/ clear");
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4NULL", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_9_WARMELTmiBASE_ADD2OUT4NULL_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_9_WARMELTmiBASE_ADD2OUT4NULL */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un * meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER_melt
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

  struct meltframe_meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER_st *)
	meltfirstargp_;
      /* use arguments meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4INTEGER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1424:/ getarg");
 /*_.VN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-base.melt:1425:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_INTEGERBOX__L1*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.VN__V2*/ meltfptr[1])) ==
	 MELTOBMAG_INT);;
      MELT_LOCATION ("warmelt-base.melt:1425:/ cond");
      /*cond */ if ( /*_#IS_INTEGERBOX__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V5*/ meltfptr[4] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1425:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check boxed number vn"),
				  ("warmelt-base.melt")
				  ? ("warmelt-base.melt") : __FILE__,
				  (1425) ? (1425) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-base.melt:1425:/ clear");
	     /*clear *//*_#IS_INTEGERBOX__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    /*^compute */
 /*_#GET_INT__L2*/ meltfnum[0] =
      (melt_get_int ((melt_ptr_t) ( /*_.VN__V2*/ meltfptr[1])));;

    {
      MELT_LOCATION ("warmelt-base.melt:1426:/ locexp");
      meltgc_add_out_dec ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
			  ( /*_#GET_INT__L2*/ meltfnum[0]));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1424:/ clear");
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#GET_INT__L2*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4INTEGER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_10_WARMELTmiBASE_ADD2OUT4INTEGER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiBASE_ADD2OUT4STRING (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un * meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_11_WARMELTmiBASE_ADD2OUT4STRING_melt =
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

  struct meltframe_meltrout_11_WARMELTmiBASE_ADD2OUT4STRING_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_11_WARMELTmiBASE_ADD2OUT4STRING is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_11_WARMELTmiBASE_ADD2OUT4STRING_st *)
	meltfirstargp_;
      /* use arguments meltrout_11_WARMELTmiBASE_ADD2OUT4STRING output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_11_WARMELTmiBASE_ADD2OUT4STRING nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4STRING", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1429:/ getarg");
 /*_.VS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-base.melt:1430:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_STRING__L1*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.VS__V2*/ meltfptr[1])) ==
	 MELTOBMAG_STRING);;
      MELT_LOCATION ("warmelt-base.melt:1430:/ cond");
      /*cond */ if ( /*_#IS_STRING__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V5*/ meltfptr[4] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1430:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check boxed string vs"),
				  ("warmelt-base.melt")
				  ? ("warmelt-base.melt") : __FILE__,
				  (1430) ? (1430) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-base.melt:1430:/ clear");
	     /*clear *//*_#IS_STRING__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1431:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
		      melt_string_str ((melt_ptr_t)
				       ( /*_.VS__V2*/ meltfptr[1])));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1429:/ clear");
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4STRING", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_11_WARMELTmiBASE_ADD2OUT4STRING_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_11_WARMELTmiBASE_ADD2OUT4STRING */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un * meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF_melt =
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

  struct meltframe_meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF_st *)
	meltfirstargp_;
      /* use arguments meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4STRBUF", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1434:/ getarg");
 /*_.VSBUF__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-base.melt:1435:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_STRBUF__L1*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.VSBUF__V2*/ meltfptr[1])) ==
	 MELTOBMAG_STRBUF);;
      MELT_LOCATION ("warmelt-base.melt:1435:/ cond");
      /*cond */ if ( /*_#IS_STRBUF__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V5*/ meltfptr[4] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1435:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check strbuf vsbuf"),
				  ("warmelt-base.melt")
				  ? ("warmelt-base.melt") : __FILE__,
				  (1435) ? (1435) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
      /*epilog */

      MELT_LOCATION ("warmelt-base.melt:1435:/ clear");
	     /*clear *//*_#IS_STRBUF__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V5*/ meltfptr[4] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V4*/ meltfptr[3] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1436:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
		      melt_strbuf_str ((melt_ptr_t) /*_.VSBUF__V2*/
				       meltfptr[1]));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1434:/ clear");
	   /*clear *//*_.IFCPP___V4*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4STRBUF", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_12_WARMELTmiBASE_ADD2OUT4STRBUF */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt
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

  struct meltframe_meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_st
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
#define MELTFRAM_NBVARPTR 10
    melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_st *)
	meltfirstargp_;
      /* use arguments meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 10; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB nbval 10*/
  meltfram__.mcfr_nbvar = 10 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4CLONEDSYMB", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1439:/ getarg");
 /*_.VC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1440:/ block");
    /*anyblock */
    {

      /*^objgoto */
      /*objgoto */ goto mtch1_0;
      ;

    /*objlabel */ mtch1_0:;
      MELT_LOCATION ("warmelt-base.melt:1441:/ objlabel");
      /* objlabel */
#if MELTDEBUG_MATCHING
      static long meltlab_count_1;
      meltlab_count_1++;
      debugeprintf
	("objlabel_1 mtch1_*CLASS_NORMTESTER_INSTANCE/2bc6f710. *CLASS_OBJLABELINSTR/d740257. %ld",
	 meltlab_count_1);
#endif

      ;
      /*^clear */
	    /*clear *//*_.NAMED_NAME__V4*/ meltfptr[3] = 0;
      /*^clear */
	    /*clear *//*_.CSYM_URANK__V5*/ meltfptr[4] = 0;
      /*^cond */
      /*cond */ if (
		     /*normtesterinst */
	(melt_is_instance_of ((melt_ptr_t) ( /*_.VC__V2*/ meltfptr[1]),
			      (melt_ptr_t) (( /*!CLASS_CLONED_SYMBOL */ meltfrout->tabval[0])))))	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

	    /*^getslot */
	    {
	      melt_ptr_t slot = NULL, obj = NULL;
	      obj = (melt_ptr_t) ( /*_.VC__V2*/ meltfptr[1]) /*=obj*/ ;
	      melt_object_get_field (slot, obj, 1, "NAMED_NAME");
     /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
	    };
	    ;
	    /*^getslot */
	    {
	      melt_ptr_t slot = NULL, obj = NULL;
	      obj = (melt_ptr_t) ( /*_.VC__V2*/ meltfptr[1]) /*=obj*/ ;
	      melt_object_get_field (slot, obj, 3, "CSYM_URANK");
     /*_.CSYM_URANK__V5*/ meltfptr[4] = slot;
	    };
	    ;
	    MELT_LOCATION ("warmelt-base.melt:1443:/ objgoto");
	    /*objgoto */ goto mtch1_1;
	    ;
	  }
	  ;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1441:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {

	    MELT_LOCATION ("warmelt-base.melt:1448:/ objgoto");
	    /*objgoto */ goto mtch1_3;
	    ;
	  }
	  ;
	}
      ;

    /*objlabel */ mtch1_1:;
      MELT_LOCATION ("warmelt-base.melt:1443:/ objlabel");
      /* objlabel */
#if MELTDEBUG_MATCHING
      static long meltlab_count_2;
      meltlab_count_2++;
      debugeprintf
	("objlabel_2 mtch1_*CLASS_NORMTESTER_MATCHER/1301c347. *CLASS_OBJLABELINSTR/29bcc047. %ld",
	 meltlab_count_2);
#endif

      ;
      /*^clear */
	    /*clear *//*_#ICT__L1*/ meltfnum[0] = 0;
      /*^cond */
      /*cond */ if (
					 /* integerbox_of IBOXOF_mtch1__1 ? *//*_.CSYM_URANK__V5*/
		     meltfptr[4]
		     && melt_magic_discr ((melt_ptr_t) /*_.CSYM_URANK__V5*/ meltfptr[4]) == MELTOBMAG_INT)	/*then */
	{
	  /*^cond.then */
	  /*^block */
	  /*anyblock */
	  {

	    /*^clear */
	      /*clear *//*_#ICT__L1*/ meltfnum[0] = 0;

	    {
	      /*^locexp */
	      /* integerbox_of IBOXOF_mtch1__1 ! *//*_#ICT__L1*/ meltfnum[0] =
		((struct meltint_st *) /*_.CSYM_URANK__V5*/ meltfptr[4])->
		val;;
	    }
	    ;
	    MELT_LOCATION ("warmelt-base.melt:1441:/ objgoto");
	    /*objgoto */ goto mtch1_2;
	    ;
	  }
	  ;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1443:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {

	    MELT_LOCATION ("warmelt-base.melt:1448:/ objgoto");
	    /*objgoto */ goto mtch1_3;
	    ;
	  }
	  ;
	}
      ;

    /*objlabel */ mtch1_2:;
      MELT_LOCATION ("warmelt-base.melt:1441:/ objlabel");
      /* objlabel */
#if MELTDEBUG_MATCHING
      static long meltlab_count_3;
      meltlab_count_3++;
      debugeprintf
	("objlabel_3 mtch1_*CLASS_NORMTESTER_SUCCESS/37d3930f. *CLASS_OBJLABELINSTR/1658f101. %ld",
	 meltlab_count_3);
#endif

      ;
      /*^quasiblock */


      /*_.NVARNAM__V6*/ meltfptr[4] = /*_.NAMED_NAME__V4*/ meltfptr[3];;
      /*^compute */
      /*_#NVARURANK__L2*/ meltfnum[1] = /*_#ICT__L1*/ meltfnum[0];;

#if MELT_HAVE_DEBUG
      MELT_LOCATION ("warmelt-base.melt:1444:/ cppif.then");
      /*^block */
      /*anyblock */
      {


	MELT_CHECK_SIGNAL ();
	;
    /*_#IS_STRING__L3*/ meltfnum[2] =
	  (melt_magic_discr ((melt_ptr_t) ( /*_.NVARNAM__V6*/ meltfptr[4])) ==
	   MELTOBMAG_STRING);;
	MELT_LOCATION ("warmelt-base.melt:1444:/ cond");
	/*cond */ if ( /*_#IS_STRING__L3*/ meltfnum[2])	/*then */
	  {
	    /*^cond.then */
	    /*_._IFELSE___V8*/ meltfptr[7] = ( /*nil */ NULL);;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-base.melt:1444:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {




	      {
		/*^locexp */
		melt_assert_failed (("check nvarnam"),
				    ("warmelt-base.melt")
				    ? ("warmelt-base.melt") : __FILE__,
				    (1444) ? (1444) : __LINE__, __FUNCTION__);
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
	/*_.IFCPP___V7*/ meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
	/*epilog */

	MELT_LOCATION ("warmelt-base.melt:1444:/ clear");
	      /*clear *//*_#IS_STRING__L3*/ meltfnum[2] = 0;
	/*^clear */
	      /*clear *//*_._IFELSE___V8*/ meltfptr[7] = 0;
      }

#else /*MELT_HAVE_DEBUG */
      /*^cppif.else */
      /*_.IFCPP___V7*/ meltfptr[6] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
      ;

      {
	MELT_LOCATION ("warmelt-base.melt:1445:/ locexp");
	meltgc_add_out_cident ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
			       melt_string_str ((melt_ptr_t)
						( /*_.NVARNAM__V6*/
						 meltfptr[4])));
      }
      ;

      {
	MELT_LOCATION ("warmelt-base.melt:1446:/ locexp");
	meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]), ("__"));
      }
      ;

      {
	MELT_LOCATION ("warmelt-base.melt:1447:/ locexp");
	meltgc_add_out_dec ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
			    ( /*_#NVARURANK__L2*/ meltfnum[1]));
      }
      ;

      MELT_CHECK_SIGNAL ();
      ;

      MELT_LOCATION ("warmelt-base.melt:1441:/ clear");
	    /*clear *//*_.NVARNAM__V6*/ meltfptr[4] = 0;
      /*^clear */
	    /*clear *//*_#NVARURANK__L2*/ meltfnum[1] = 0;
      /*^clear */
	    /*clear *//*_.IFCPP___V7*/ meltfptr[6] = 0;
      /*^objgoto */
      /*objgoto */ goto mtch1__end /*endmatch */ ;
      ;

    /*objlabel */ mtch1_3:;
      MELT_LOCATION ("warmelt-base.melt:1448:/ objlabel");
      /* objlabel */
#if MELTDEBUG_MATCHING
      static long meltlab_count_4;
      meltlab_count_4++;
      debugeprintf
	("objlabel_4 mtch1_*CLASS_NORMTESTER_SUCCESS/384bded. *CLASS_OBJLABELINSTR/ce95aef. %ld",
	 meltlab_count_4);
#endif

      ;
      /*^quasiblock */



#if MELT_HAVE_DEBUG
      MELT_LOCATION ("warmelt-base.melt:1449:/ cppif.then");
      /*^block */
      /*anyblock */
      {


	MELT_CHECK_SIGNAL ();
	;
	/*^cond */
	/*cond */ if (( /*nil */ NULL))	/*then */
	  {
	    /*^cond.then */
	    /*_._IFELSE___V10*/ meltfptr[4] = ( /*nil */ NULL);;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-base.melt:1449:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {




	      {
		/*^locexp */
		melt_assert_failed (("bad cloned symbol"),
				    ("warmelt-base.melt")
				    ? ("warmelt-base.melt") : __FILE__,
				    (1449) ? (1449) : __LINE__, __FUNCTION__);
		;
	      }
	      ;
		/*clear *//*_._IFELSE___V10*/ meltfptr[4] = 0;
	      /*epilog */
	    }
	    ;
	  }
	;
	/*^compute */
	/*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[4];;
	/*epilog */

	MELT_LOCATION ("warmelt-base.melt:1449:/ clear");
	      /*clear *//*_._IFELSE___V10*/ meltfptr[4] = 0;
      }

#else /*MELT_HAVE_DEBUG */
      /*^cppif.else */
      /*_.IFCPP___V9*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
      ;

      MELT_LOCATION ("warmelt-base.melt:1448:/ clear");
	    /*clear *//*_.IFCPP___V9*/ meltfptr[7] = 0;
      /*^objgoto */
      /*objgoto */ goto mtch1__end /*endmatch */ ;
      ;

    /*objlabel */ mtch1__end:;
      MELT_LOCATION ("warmelt-base.melt:1440:/ objlabel");
      /* objlabel */
#if MELTDEBUG_MATCHING
      static long meltlab_count_5;
      meltlab_count_5++;
      debugeprintf
	("objlabel_5 mtch1__end_h102968815 *CLASS_OBJLABELINSTR/6232def. %ld",
	 meltlab_count_5);
#endif

      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4CLONEDSYMB", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_13_WARMELTmiBASE_ADD2OUT4CLONEDSYMB */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un * meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED_melt
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

  struct meltframe_meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED_st
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
#define MELTFRAM_NBVARPTR 4
    melt_ptr_t mcfr_varptr[4];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED_st *)
	meltfirstargp_;
      /* use arguments meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 4; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4NAMED", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1452:/ getarg");
 /*_.VN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-base.melt:1453:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.VN__V2*/ meltfptr[1]),
					(melt_ptr_t) (( /*!CLASS_NAMED */
						       meltfrout->tabval[0])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.VN__V2*/ meltfptr[1]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 1, "NAMED_NAME");
   /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.NAMED_NAME__V4*/ meltfptr[3] = NULL;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1453:/ locexp");
      meltgc_add_out_cident ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
			     melt_string_str ((melt_ptr_t)
					      ( /*_.NAMED_NAME__V4*/
					       meltfptr[3])));
    }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1452:/ clear");
	   /*clear *//*_.NAMED_NAME__V4*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4NAMED", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_14_WARMELTmiBASE_ADD2OUT4NAMED */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt
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

  struct meltframe_meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_st *)
	meltfirstargp_;
      /* use arguments meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4ROOTOBJECT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1456:/ getarg");
 /*_.VO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-base.melt:1457:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]), ("*"));
    }
    ;
 /*_.DISCRIM__V4*/ meltfptr[3] =
      ((melt_ptr_t) (melt_discr ((melt_ptr_t) ( /*_.VO__V2*/ meltfptr[1]))));;
    MELT_LOCATION ("warmelt-base.melt:1458:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.DISCRIM__V4*/ meltfptr[3]),
					(melt_ptr_t) (( /*!CLASS_NAMED */
						       meltfrout->tabval[0])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.DISCRIM__V4*/ meltfptr[3]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 1, "NAMED_NAME");
   /*_.NAMED_NAME__V5*/ meltfptr[4] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.NAMED_NAME__V5*/ meltfptr[4] = NULL;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1458:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
		      melt_string_str ((melt_ptr_t)
				       ( /*_.NAMED_NAME__V5*/ meltfptr[4])));
    }
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1459:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]), ("/"));
    }
    ;
 /*_#OBJ_HASH__L1*/ meltfnum[0] =
      (melt_obj_hash ((melt_ptr_t) ( /*_.VO__V2*/ meltfptr[1])));;

    {
      MELT_LOCATION ("warmelt-base.melt:1460:/ locexp");
      meltgc_add_out_hex ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
			  ( /*_#OBJ_HASH__L1*/ meltfnum[0]));
    }
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1461:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]), ("."));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1456:/ clear");
	   /*clear *//*_.DISCRIM__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.NAMED_NAME__V5*/ meltfptr[4] = 0;
    /*^clear */
	   /*clear *//*_#OBJ_HASH__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4ROOTOBJECT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_15_WARMELTmiBASE_ADD2OUT4ROOTOBJECT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiBASE_ADD2OUT4ANY (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un * meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_16_WARMELTmiBASE_ADD2OUT4ANY_melt =
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

  struct meltframe_meltrout_16_WARMELTmiBASE_ADD2OUT4ANY_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_16_WARMELTmiBASE_ADD2OUT4ANY is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_16_WARMELTmiBASE_ADD2OUT4ANY_st *)
	meltfirstargp_;
      /* use arguments meltrout_16_WARMELTmiBASE_ADD2OUT4ANY output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_16_WARMELTmiBASE_ADD2OUT4ANY nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ADD2OUT4ANY", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1464:/ getarg");
 /*_.VA__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.OUT__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-base.melt:1465:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]), ("*"));
    }
    ;
 /*_.DISCRIM__V4*/ meltfptr[3] =
      ((melt_ptr_t) (melt_discr ((melt_ptr_t) ( /*_.VA__V2*/ meltfptr[1]))));;
    MELT_LOCATION ("warmelt-base.melt:1466:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.DISCRIM__V4*/ meltfptr[3]),
					(melt_ptr_t) (( /*!CLASS_NAMED */
						       meltfrout->tabval[0])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj = (melt_ptr_t) ( /*_.DISCRIM__V4*/ meltfptr[3]) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 1, "NAMED_NAME");
   /*_.NAMED_NAME__V5*/ meltfptr[4] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.NAMED_NAME__V5*/ meltfptr[4] = NULL;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1466:/ locexp");
      meltgc_add_out ((melt_ptr_t) ( /*_.OUT__V3*/ meltfptr[2]),
		      melt_string_str ((melt_ptr_t)
				       ( /*_.NAMED_NAME__V5*/ meltfptr[4])));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1464:/ clear");
	   /*clear *//*_.DISCRIM__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.NAMED_NAME__V5*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ADD2OUT4ANY", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_16_WARMELTmiBASE_ADD2OUT4ANY_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_16_WARMELTmiBASE_ADD2OUT4ANY */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY (meltclosure_ptr_t meltclosp_,
					   melt_ptr_t meltfirstargp_,
					   const melt_argdescr_cell_t
					   meltxargdescr_[],
					   union meltparam_un * meltxargtab_,
					   const melt_argdescr_cell_t
					   meltxresdescr_[],
					   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY_melt
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

  struct meltframe_meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY_st
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
  /*meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY_st *)
	meltfirstargp_;
      /* use arguments meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY nbval 6*/
  meltfram__.mcfr_nbvar = 6 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MAPSTRING_EVERY", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1525:/ getarg");
 /*_.MAP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.F__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MAPSTRING__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.MAP__V2*/ meltfptr[1])) ==
       MELTOBMAG_MAPSTRINGS);;
    MELT_LOCATION ("warmelt-base.melt:1529:/ cond");
    /*cond */ if ( /*_#IS_MAPSTRING__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L2*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-base.melt:1530:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*citerblock FOREACH_IN_MAPSTRING */
		{
		  /*foreach_in_mapstring meltcit1__EACHSTRMAP : */ int
		    meltcit1__EACHSTRMAP_ix = 0, meltcit1__EACHSTRMAP_siz = 0;
		  for (meltcit1__EACHSTRMAP_ix = 0;
		       /* we retrieve in meltcit1__EACHSTRMAP_siz the size at each iteration since it could change. */
		       meltcit1__EACHSTRMAP_ix >= 0
		       && (meltcit1__EACHSTRMAP_siz =
			   melt_size_mapstrings ((struct meltmapstrings_st *)
						 /*_.MAP__V2*/ meltfptr[1])) >
		       0
		       && meltcit1__EACHSTRMAP_ix < meltcit1__EACHSTRMAP_siz;
		       meltcit1__EACHSTRMAP_ix++)
		    {
		      const char *meltcit1__EACHSTRMAP_str = NULL;
		      const char *meltcit1__EACHSTRMAP_nam = NULL;
    /*_.CURAT__V4*/ meltfptr[3] = NULL;
    /*_.CURVAL__V5*/ meltfptr[4] = NULL;
		      meltcit1__EACHSTRMAP_str =
			((struct meltmapstrings_st *) /*_.MAP__V2*/
			 meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
		      if (!meltcit1__EACHSTRMAP_str
			  || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
			continue;	/*foreach_in_mapstring meltcit1__EACHSTRMAP inside before */
		      /*_.CURVAL__V5*/ meltfptr[4] =
			((struct meltmapstrings_st *) /*_.MAP__V2*/
			 meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
		      if (! /*_.CURVAL__V5*/ meltfptr[4])
			continue;
		      if (melt_is_instance_of
			  ((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4],
			   (melt_ptr_t) MELT_PREDEF (CLASS_NAMED))
			  && ( /*_.CURAT__V4*/ meltfptr[3] =
			      melt_object_nth_field ((melt_ptr_t)
						     /*_.CURVAL__V5*/
						     meltfptr[4],
						     MELTFIELD_NAMED_NAME)) !=
			  NULL
			  && (meltcit1__EACHSTRMAP_nam =
			      melt_string_str ((melt_ptr_t) /*_.CURAT__V4*/
					       meltfptr[3])) != (char *) 0
			  && !strcmp (meltcit1__EACHSTRMAP_nam,
				      meltcit1__EACHSTRMAP_str))
			/*_.CURAT__V4*/ meltfptr[3] =
			  /*_.CURAT__V4*/ meltfptr[3];
		      else
			{
      /*_.CURAT__V4*/ meltfptr[3] = NULL;
      /*_.CURAT__V4*/ meltfptr[3] =
			    meltgc_new_stringdup ((meltobject_ptr_t)
						  MELT_PREDEF (DISCR_STRING),
						  meltcit1__EACHSTRMAP_str);
			}
		      meltcit1__EACHSTRMAP_str = (const char *) 0;
		      meltcit1__EACHSTRMAP_nam = (const char *) 0;




		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-base.melt:1534:/ apply");
		      /*apply */
		      {
			union meltparam_un argtab[1];
			memset (&argtab, 0, sizeof (argtab));
			/*^apply.arg */
			argtab[0].meltbp_aptr =
			  (melt_ptr_t *) & /*_.CURVAL__V5*/ meltfptr[4];
			/*_.F__V6*/ meltfptr[5] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.F__V3*/ meltfptr[2]),
				      (melt_ptr_t) ( /*_.CURAT__V4*/
						    meltfptr[3]),
				      (MELTBPARSTR_PTR ""), argtab, "",
				      (union meltparam_un *) 0);
		      }
		      ;
		      /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
    /*_.CURAT__V4*/ meltfptr[3] = NULL;
    /*_.CURVAL__V5*/ meltfptr[4] = NULL;
		    }

		  /*citerepilog */

		  MELT_LOCATION ("warmelt-base.melt:1531:/ clear");
		/*clear *//*_.CURAT__V4*/ meltfptr[3] = 0;
		  /*^clear */
		/*clear *//*_.CURVAL__V5*/ meltfptr[4] = 0;
		  /*^clear */
		/*clear *//*_.F__V6*/ meltfptr[5] = 0;
		}		/*endciterblock FOREACH_IN_MAPSTRING */
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1529:/ clear");
	     /*clear *//*_#IS_CLOSURE__L2*/ meltfnum[1] = 0;
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1525:/ clear");
	   /*clear *//*_#IS_MAPSTRING__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MAPSTRING_EVERY", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_17_WARMELTmiBASE_MAPSTRING_EVERY */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt
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

  struct meltframe_meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_st
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
#define MELTFRAM_NBVARPTR 10
    melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 10; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST nbval 10*/
  meltfram__.mcfr_nbvar = 10 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MAPSTRING_ITERATE_TEST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1538:/ getarg");
 /*_.MAP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.F__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MAPSTRING__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.MAP__V2*/ meltfptr[1])) ==
       MELTOBMAG_MAPSTRINGS);;
    MELT_LOCATION ("warmelt-base.melt:1543:/ cond");
    /*cond */ if ( /*_#IS_MAPSTRING__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L2*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-base.melt:1544:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*citerblock FOREACH_IN_MAPSTRING */
		{
		  /*foreach_in_mapstring meltcit1__EACHSTRMAP : */ int
		    meltcit1__EACHSTRMAP_ix = 0, meltcit1__EACHSTRMAP_siz = 0;
		  for (meltcit1__EACHSTRMAP_ix = 0;
		       /* we retrieve in meltcit1__EACHSTRMAP_siz the size at each iteration since it could change. */
		       meltcit1__EACHSTRMAP_ix >= 0
		       && (meltcit1__EACHSTRMAP_siz =
			   melt_size_mapstrings ((struct meltmapstrings_st *)
						 /*_.MAP__V2*/ meltfptr[1])) >
		       0
		       && meltcit1__EACHSTRMAP_ix < meltcit1__EACHSTRMAP_siz;
		       meltcit1__EACHSTRMAP_ix++)
		    {
		      const char *meltcit1__EACHSTRMAP_str = NULL;
		      const char *meltcit1__EACHSTRMAP_nam = NULL;
    /*_.CURAT__V4*/ meltfptr[3] = NULL;
    /*_.CURVAL__V5*/ meltfptr[4] = NULL;
		      meltcit1__EACHSTRMAP_str =
			((struct meltmapstrings_st *) /*_.MAP__V2*/
			 meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
		      if (!meltcit1__EACHSTRMAP_str
			  || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
			continue;	/*foreach_in_mapstring meltcit1__EACHSTRMAP inside before */
		      /*_.CURVAL__V5*/ meltfptr[4] =
			((struct meltmapstrings_st *) /*_.MAP__V2*/
			 meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
		      if (! /*_.CURVAL__V5*/ meltfptr[4])
			continue;
		      if (melt_is_instance_of
			  ((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4],
			   (melt_ptr_t) MELT_PREDEF (CLASS_NAMED))
			  && ( /*_.CURAT__V4*/ meltfptr[3] =
			      melt_object_nth_field ((melt_ptr_t)
						     /*_.CURVAL__V5*/
						     meltfptr[4],
						     MELTFIELD_NAMED_NAME)) !=
			  NULL
			  && (meltcit1__EACHSTRMAP_nam =
			      melt_string_str ((melt_ptr_t) /*_.CURAT__V4*/
					       meltfptr[3])) != (char *) 0
			  && !strcmp (meltcit1__EACHSTRMAP_nam,
				      meltcit1__EACHSTRMAP_str))
			/*_.CURAT__V4*/ meltfptr[3] =
			  /*_.CURAT__V4*/ meltfptr[3];
		      else
			{
      /*_.CURAT__V4*/ meltfptr[3] = NULL;
      /*_.CURAT__V4*/ meltfptr[3] =
			    meltgc_new_stringdup ((meltobject_ptr_t)
						  MELT_PREDEF (DISCR_STRING),
						  meltcit1__EACHSTRMAP_str);
			}
		      meltcit1__EACHSTRMAP_str = (const char *) 0;
		      meltcit1__EACHSTRMAP_nam = (const char *) 0;




		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-base.melt:1548:/ quasiblock");


		      /*^multiapply */
		      /*multiapply 2args, 1x.res */
		      {
			union meltparam_un argtab[1];

			union meltparam_un restab[1];
			memset (&restab, 0, sizeof (restab));
			memset (&argtab, 0, sizeof (argtab));
			/*^multiapply.arg */
			argtab[0].meltbp_aptr =
			  (melt_ptr_t *) & /*_.CURVAL__V5*/ meltfptr[4];
			/*^multiapply.xres */
			restab[0].meltbp_aptr =
			  (melt_ptr_t *) & /*_.OTHER__V8*/ meltfptr[7];
			/*^multiapply.appl */
			/*_.TEST__V7*/ meltfptr[6] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.F__V3*/ meltfptr[2]),
				      (melt_ptr_t) ( /*_.CURAT__V4*/
						    meltfptr[3]),
				      (MELTBPARSTR_PTR ""), argtab,
				      (MELTBPARSTR_PTR ""), restab);
		      }
		      ;
		      /*^quasiblock */



		      MELT_CHECK_SIGNAL ();
		      ;
      /*_#NULL__L3*/ meltfnum[2] =
			(( /*_.TEST__V7*/ meltfptr[6]) == NULL);;
		      MELT_LOCATION ("warmelt-base.melt:1551:/ cond");
		      /*cond */ if ( /*_#NULL__L3*/ meltfnum[2])	/*then */
			{
			  /*^cond.then */
			  /*^block */
			  /*anyblock */
			  {


			    MELT_CHECK_SIGNAL ();
			    ;
			    MELT_LOCATION
			      ("warmelt-base.melt:1552:/ quasiblock");


			    /*_._RETVAL___V1*/ meltfptr[0] =
			      /*_.CURAT__V4*/ meltfptr[3];;
			    MELT_LOCATION
			      ("warmelt-base.melt:1552:/ putxtraresult");
			    if (!meltxrestab_ || !meltxresdescr_)
			      goto meltlabend_rout;
			    if (meltxresdescr_[0] != MELTBPAR_PTR)
			      goto meltlabend_rout;
			    if (meltxrestab_[0].meltbp_aptr)
			      *(meltxrestab_[0].meltbp_aptr) =
				(melt_ptr_t) ( /*_.CURVAL__V5*/ meltfptr[4]);
			    ;
			    /*^putxtraresult */
			    if (!meltxrestab_ || !meltxresdescr_)
			      goto meltlabend_rout;
			    if (meltxresdescr_[1] != MELTBPAR_PTR)
			      goto meltlabend_rout;
			    if (meltxrestab_[1].meltbp_aptr)
			      *(meltxrestab_[1].meltbp_aptr) =
				(melt_ptr_t) ( /*_.OTHER__V8*/ meltfptr[7]);
			    ;
			    /*^finalreturn */
			    ;
			    /*finalret */ goto meltlabend_rout;
			    /*_._IF___V9*/ meltfptr[8] =
			      /*_.RETURN___V10*/ meltfptr[9];;
			    /*epilog */

			    MELT_LOCATION ("warmelt-base.melt:1551:/ clear");
		  /*clear *//*_.RETURN___V10*/ meltfptr[9] = 0;
			  }
			  ;
			}
		      else
			{	/*^cond.else */

       /*_._IF___V9*/ meltfptr[8] = NULL;;
			}
		      ;
		      /*^compute */
		      /*_.MULTI___V6*/ meltfptr[5] =
			/*_._IF___V9*/ meltfptr[8];;

		      MELT_LOCATION ("warmelt-base.melt:1548:/ clear");
		/*clear *//*_#NULL__L3*/ meltfnum[2] = 0;
		      /*^clear */
		/*clear *//*_._IF___V9*/ meltfptr[8] = 0;

		      /*^clear */
		/*clear *//*_.OTHER__V8*/ meltfptr[7] = 0;
		      /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
    /*_.CURAT__V4*/ meltfptr[3] = NULL;
    /*_.CURVAL__V5*/ meltfptr[4] = NULL;
		    }

		  /*citerepilog */

		  MELT_LOCATION ("warmelt-base.melt:1545:/ clear");
		/*clear *//*_.CURAT__V4*/ meltfptr[3] = 0;
		  /*^clear */
		/*clear *//*_.CURVAL__V5*/ meltfptr[4] = 0;
		  /*^clear */
		/*clear *//*_.MULTI___V6*/ meltfptr[5] = 0;
		}		/*endciterblock FOREACH_IN_MAPSTRING */
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1543:/ clear");
	     /*clear *//*_#IS_CLOSURE__L2*/ meltfnum[1] = 0;
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1538:/ clear");
	   /*clear *//*_#IS_MAPSTRING__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MAPSTRING_ITERATE_TEST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_18_WARMELTmiBASE_MAPSTRING_ITERATE_TEST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY (meltclosure_ptr_t meltclosp_,
					  melt_ptr_t meltfirstargp_,
					  const melt_argdescr_cell_t
					  meltxargdescr_[],
					  union meltparam_un * meltxargtab_,
					  const melt_argdescr_cell_t
					  meltxresdescr_[],
					  union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY_melt =
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

  struct meltframe_meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY_st *)
	meltfirstargp_;
      /* use arguments meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MULTIPLE_EVERY", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1587:/ getarg");
 /*_.TUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.F__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.TUP__V2*/ meltfptr[1])) ==
       MELTOBMAG_MULTIPLE);;
    MELT_LOCATION ("warmelt-base.melt:1590:/ cond");
    /*cond */ if ( /*_#IS_MULTIPLE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L2*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-base.melt:1591:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*citerblock FOREACH_IN_MULTIPLE */
		{
		  /* start foreach_in_multiple meltcit1__EACHTUP */
		  long meltcit1__EACHTUP_ln =
		    melt_multiple_length ((melt_ptr_t) /*_.TUP__V2*/
					  meltfptr[1]);
		  for ( /*_#IX__L3*/ meltfnum[2] = 0;
		       ( /*_#IX__L3*/ meltfnum[2] >= 0)
		       && ( /*_#IX__L3*/ meltfnum[2] < meltcit1__EACHTUP_ln);
	/*_#IX__L3*/ meltfnum[2]++)
		    {
		      /*_.COMP__V4*/ meltfptr[3] =
			melt_multiple_nth ((melt_ptr_t)
					   ( /*_.TUP__V2*/ meltfptr[1]),
					   /*_#IX__L3*/ meltfnum[2]);




		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-base.melt:1595:/ apply");
		      /*apply */
		      {
			union meltparam_un argtab[1];
			memset (&argtab, 0, sizeof (argtab));
			/*^apply.arg */
			argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
			/*_.F__V5*/ meltfptr[4] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.F__V3*/ meltfptr[2]),
				      (melt_ptr_t) ( /*_.COMP__V4*/
						    meltfptr[3]),
				      (MELTBPARSTR_LONG ""), argtab, "",
				      (union meltparam_un *) 0);
		      }
		      ;
		      if ( /*_#IX__L3*/ meltfnum[2] < 0)
			break;
		    }		/* end  foreach_in_multiple meltcit1__EACHTUP */

		  /*citerepilog */

		  MELT_LOCATION ("warmelt-base.melt:1592:/ clear");
		/*clear *//*_.COMP__V4*/ meltfptr[3] = 0;
		  /*^clear */
		/*clear *//*_#IX__L3*/ meltfnum[2] = 0;
		  /*^clear */
		/*clear *//*_.F__V5*/ meltfptr[4] = 0;
		}		/*endciterblock FOREACH_IN_MULTIPLE */
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1590:/ clear");
	     /*clear *//*_#IS_CLOSURE__L2*/ meltfnum[1] = 0;
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1587:/ clear");
	   /*clear *//*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MULTIPLE_EVERY", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_19_WARMELTmiBASE_MULTIPLE_EVERY */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt
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

  struct meltframe_meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_st
	 *) meltfirstargp_;
      /* use arguments meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MULTIPLE_BACKWARD_EVERY", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1598:/ getarg");
 /*_.TUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.F__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.TUP__V2*/ meltfptr[1])) ==
       MELTOBMAG_MULTIPLE);;
    MELT_LOCATION ("warmelt-base.melt:1601:/ cond");
    /*cond */ if ( /*_#IS_MULTIPLE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_CLOSURE__L2*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.F__V3*/ meltfptr[2])) ==
	     MELTOBMAG_CLOSURE);;
	  MELT_LOCATION ("warmelt-base.melt:1602:/ cond");
	  /*cond */ if ( /*_#IS_CLOSURE__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*citerblock FOREACH_IN_MULTIPLE_BACKWARD */
		{
		  /* start foreach_in_multiple_backward meltcit1__EACHTUPBACK */
		  long meltcit1__EACHTUPBACK_ln =
		    melt_multiple_length ((melt_ptr_t) /*_.TUP__V2*/
					  meltfptr[1]);
		  long meltcit1__EACHTUPBACK_ix = 0;
		  for (meltcit1__EACHTUPBACK_ix =
		       meltcit1__EACHTUPBACK_ln - 1;
		       meltcit1__EACHTUPBACK_ix >= 0;
		       meltcit1__EACHTUPBACK_ix--)
		    {
		      /*_.COMP__V4*/ meltfptr[3] =
			melt_multiple_nth ((melt_ptr_t)
					   ( /*_.TUP__V2*/ meltfptr[1]),
					   meltcit1__EACHTUPBACK_ix);
   /*_#IX__L3*/ meltfnum[2] = meltcit1__EACHTUPBACK_ix;



		      MELT_CHECK_SIGNAL ();
		      ;
		      MELT_LOCATION ("warmelt-base.melt:1606:/ apply");
		      /*apply */
		      {
			union meltparam_un argtab[1];
			memset (&argtab, 0, sizeof (argtab));
			/*^apply.arg */
			argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
			/*_.F__V5*/ meltfptr[4] =
			  melt_apply ((meltclosure_ptr_t)
				      ( /*_.F__V3*/ meltfptr[2]),
				      (melt_ptr_t) ( /*_.COMP__V4*/
						    meltfptr[3]),
				      (MELTBPARSTR_LONG ""), argtab, "",
				      (union meltparam_un *) 0);
		      }
		      ;
		    }		/* end  foreach_in_multiple_backward meltcit1__EACHTUPBACK */

		  /*citerepilog */

		  MELT_LOCATION ("warmelt-base.melt:1603:/ clear");
		/*clear *//*_.COMP__V4*/ meltfptr[3] = 0;
		  /*^clear */
		/*clear *//*_#IX__L3*/ meltfnum[2] = 0;
		  /*^clear */
		/*clear *//*_.F__V5*/ meltfptr[4] = 0;
		}		/*endciterblock FOREACH_IN_MULTIPLE_BACKWARD */
		;
		/*epilog */
	      }
	      ;
	    }			/*noelse */
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1601:/ clear");
	     /*clear *//*_#IS_CLOSURE__L2*/ meltfnum[1] = 0;
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1598:/ clear");
	   /*clear *//*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MULTIPLE_BACKWARD_EVERY", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_20_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt
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

  struct meltframe_meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_st
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
#define MELTFRAM_NBVARPTR 13
    melt_ptr_t mcfr_varptr[13];
#define MELTFRAM_NBVARNUM 10
    long mcfr_varnum[10];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_st *)
	meltfirstargp_;
      /* use arguments meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 13; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH nbval 13*/
  meltfram__.mcfr_nbvar = 13 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("MULTIPLE_EVERY_BOTH", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1610:/ getarg");
 /*_.TUP1__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.TUP2__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.TUP2__V3*/ meltfptr[2])) !=
	      NULL);


  /*getarg#2 */
  /*^getarg */
  if (meltxargdescr_[1] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.F__V4*/ meltfptr[3] =
    (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.F__V4*/ meltfptr[3])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.TUP1__V2*/ meltfptr[1])) ==
       MELTOBMAG_MULTIPLE);;
    MELT_LOCATION ("warmelt-base.melt:1613:/ cond");
    /*cond */ if ( /*_#IS_MULTIPLE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_MULTIPLE__L2*/ meltfnum[1] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.TUP2__V3*/ meltfptr[2])) ==
	     MELTOBMAG_MULTIPLE);;
	  MELT_LOCATION ("warmelt-base.melt:1614:/ cond");
	  /*cond */ if ( /*_#IS_MULTIPLE__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {


		MELT_CHECK_SIGNAL ();
		;
     /*_#IS_CLOSURE__L3*/ meltfnum[2] =
		  (melt_magic_discr ((melt_ptr_t) ( /*_.F__V4*/ meltfptr[3]))
		   == MELTOBMAG_CLOSURE);;
		MELT_LOCATION ("warmelt-base.melt:1615:/ cond");
		/*cond */ if ( /*_#IS_CLOSURE__L3*/ meltfnum[2])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      MELT_LOCATION ("warmelt-base.melt:1616:/ quasiblock");


       /*_#LN1__L4*/ meltfnum[3] =
			(melt_multiple_length
			 ((melt_ptr_t) ( /*_.TUP1__V2*/ meltfptr[1])));;
		      /*^compute */
       /*_#LN2__L5*/ meltfnum[4] =
			(melt_multiple_length
			 ((melt_ptr_t) ( /*_.TUP2__V3*/ meltfptr[2])));;
		      /*^compute */
       /*_#IX__L6*/ meltfnum[5] = 0;;
		      MELT_LOCATION ("warmelt-base.melt:1619:/ loop");
		      /*loop */
		      {
		      meltlabloop_TUPLOOP_1:;
				/*^loopbody */

			/*^block */
			/*anyblock */
			{


			  MELT_CHECK_SIGNAL ();
			  ;

			  MELT_CHECK_SIGNAL ();
			  ;
	 /*_#gteqI__L7*/ meltfnum[6] =
			    (( /*_#IX__L6*/ meltfnum[5]) >=
			     ( /*_#LN1__L4*/ meltfnum[3]));;
			  MELT_LOCATION ("warmelt-base.melt:1620:/ cond");
			  /*cond */ if ( /*_#gteqI__L7*/ meltfnum[6])	/*then */
			    {
			      /*^cond.then */
			      /*^block */
			      /*anyblock */
			      {

				/*^quasiblock */


				/*^compute */
	   /*_.TUPLOOP__V10*/ meltfptr[9] = NULL;;

				/*^exit */
				/*exit */
				{
				  goto meltlabexit_TUPLOOP_1;
				}
				;
				/*epilog */
			      }
			      ;
			    }	/*noelse */
			  ;

			  MELT_CHECK_SIGNAL ();
			  ;
	 /*_#gteqI__L8*/ meltfnum[7] =
			    (( /*_#IX__L6*/ meltfnum[5]) >=
			     ( /*_#LN2__L5*/ meltfnum[4]));;
			  MELT_LOCATION ("warmelt-base.melt:1621:/ cond");
			  /*cond */ if ( /*_#gteqI__L8*/ meltfnum[7])	/*then */
			    {
			      /*^cond.then */
			      /*^block */
			      /*anyblock */
			      {

				/*^quasiblock */


				/*^compute */
	   /*_.TUPLOOP__V10*/ meltfptr[9] = NULL;;

				/*^exit */
				/*exit */
				{
				  goto meltlabexit_TUPLOOP_1;
				}
				;
				/*epilog */
			      }
			      ;
			    }	/*noelse */
			  ;
	 /*_.MULTIPLE_NTH__V11*/ meltfptr[10] =
			    (melt_multiple_nth
			     ((melt_ptr_t) ( /*_.TUP1__V2*/ meltfptr[1]),
			      ( /*_#IX__L6*/ meltfnum[5])));;
			  /*^compute */
	 /*_.MULTIPLE_NTH__V12*/ meltfptr[11] =
			    (melt_multiple_nth
			     ((melt_ptr_t) ( /*_.TUP2__V3*/ meltfptr[2]),
			      ( /*_#IX__L6*/ meltfnum[5])));;

			  MELT_CHECK_SIGNAL ();
			  ;
			  MELT_LOCATION ("warmelt-base.melt:1622:/ apply");
			  /*apply */
			  {
			    union meltparam_un argtab[2];
			    memset (&argtab, 0, sizeof (argtab));
			    /*^apply.arg */
			    argtab[0].meltbp_aptr =
			      (melt_ptr_t *) & /*_.MULTIPLE_NTH__V12*/
			      meltfptr[11];
			    /*^apply.arg */
			    argtab[1].meltbp_long = /*_#IX__L6*/ meltfnum[5];
			    /*_.F__V13*/ meltfptr[12] =
			      melt_apply ((meltclosure_ptr_t)
					  ( /*_.F__V4*/ meltfptr[3]),
					  (melt_ptr_t) ( /*_.MULTIPLE_NTH__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un *) 0);
			  }
			  ;
	 /*_#plI__L9*/ meltfnum[8] =
			    (( /*_#IX__L6*/ meltfnum[5]) + (1));;
			  MELT_LOCATION ("warmelt-base.melt:1623:/ compute");
			  /*_#IX__L6*/ meltfnum[5] =
			    /*_#SETQ___L10*/ meltfnum[9] =
			    /*_#plI__L9*/ meltfnum[8];;

			  MELT_CHECK_SIGNAL ();
			  ;
			  /*epilog */

			  MELT_LOCATION ("warmelt-base.melt:1619:/ clear");
		   /*clear *//*_#gteqI__L7*/ meltfnum[6] = 0;
			  /*^clear */
		   /*clear *//*_#gteqI__L8*/ meltfnum[7] = 0;
			  /*^clear */
		   /*clear *//*_.MULTIPLE_NTH__V11*/ meltfptr[10] = 0;
			  /*^clear */
		   /*clear *//*_.MULTIPLE_NTH__V12*/ meltfptr[11] = 0;
			  /*^clear */
		   /*clear *//*_.F__V13*/ meltfptr[12] = 0;
			  /*^clear */
		   /*clear *//*_#plI__L9*/ meltfnum[8] = 0;
			  /*^clear */
		   /*clear *//*_#SETQ___L10*/ meltfnum[9] = 0;
			}
			;
			;
			goto meltlabloop_TUPLOOP_1;
		      meltlabexit_TUPLOOP_1:;
				/*^loopepilog */
			/*loopepilog */
			/*_.FOREVER___V9*/ meltfptr[8] =
			  /*_.TUPLOOP__V10*/ meltfptr[9];;
		      }
		      ;
		      /*^compute */
		      /*_.LET___V8*/ meltfptr[7] =
			/*_.FOREVER___V9*/ meltfptr[8];;

		      MELT_LOCATION ("warmelt-base.melt:1616:/ clear");
		 /*clear *//*_#LN1__L4*/ meltfnum[3] = 0;
		      /*^clear */
		 /*clear *//*_#LN2__L5*/ meltfnum[4] = 0;
		      /*^clear */
		 /*clear *//*_#IX__L6*/ meltfnum[5] = 0;
		      /*^clear */
		 /*clear *//*_.FOREVER___V9*/ meltfptr[8] = 0;
		      /*_._IF___V7*/ meltfptr[6] =
			/*_.LET___V8*/ meltfptr[7];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-base.melt:1615:/ clear");
		 /*clear *//*_.LET___V8*/ meltfptr[7] = 0;
		    }
		    ;
		  }
		else
		  {		/*^cond.else */

      /*_._IF___V7*/ meltfptr[6] = NULL;;
		  }
		;
		/*^compute */
		/*_._IF___V6*/ meltfptr[5] = /*_._IF___V7*/ meltfptr[6];;
		/*epilog */

		MELT_LOCATION ("warmelt-base.melt:1614:/ clear");
	       /*clear *//*_#IS_CLOSURE__L3*/ meltfnum[2] = 0;
		/*^clear */
	       /*clear *//*_._IF___V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_._IF___V6*/ meltfptr[5] = NULL;;
	    }
	  ;
	  /*^compute */
	  /*_._IF___V5*/ meltfptr[4] = /*_._IF___V6*/ meltfptr[5];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:1613:/ clear");
	     /*clear *//*_#IS_MULTIPLE__L2*/ meltfnum[1] = 0;
	  /*^clear */
	     /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V5*/ meltfptr[4] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-base.melt:1610:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_._IF___V5*/ meltfptr[4];;

    {
      MELT_LOCATION ("warmelt-base.melt:1610:/ locexp");
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
	   /*clear *//*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V5*/ meltfptr[4] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("MULTIPLE_EVERY_BOTH", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_21_WARMELTmiBASE_MULTIPLE_EVERY_BOTH */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_melt
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

  struct meltframe_meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_st
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
#define MELTFRAM_NBVARPTR 12
    melt_ptr_t mcfr_varptr[12];
#define MELTFRAM_NBVARNUM 14
    long mcfr_varnum[14];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_st
	 *) meltfirstargp_;
      /* use arguments meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 12; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER nbval 12*/
  meltfram__.mcfr_nbvar = 12 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_PRAGMA_HANDLER", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1646:/ getarg");
 /*_.LSTHANDLER__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-base.melt:1651:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_LIST__L1*/ meltfnum[0] =
	(melt_magic_discr ((melt_ptr_t) ( /*_.LSTHANDLER__V2*/ meltfptr[1]))
	 == MELTOBMAG_LIST);;
      MELT_LOCATION ("warmelt-base.melt:1651:/ cond");
      /*cond */ if ( /*_#IS_LIST__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V4*/ meltfptr[3] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1651:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("register_pragma_handler takes a list as argument."), ("warmelt-base.melt") ? ("warmelt-base.melt") : __FILE__, (1651) ? (1651) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V4*/ meltfptr[3] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
      /*epilog */

      MELT_LOCATION ("warmelt-base.melt:1651:/ clear");
	     /*clear *//*_#IS_LIST__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V3*/ meltfptr[2] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-base.melt:1653:/ quasiblock");


    /*^cond */
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!INITIAL_SYSTEM_DATA */
					  meltfrout->tabval[0])),
					(melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!INITIAL_SYSTEM_DATA */ meltfrout->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 11, "SYSDATA_MELTPRAGMAS");
   /*_.OLDTUPLE__V5*/ meltfptr[3] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.OLDTUPLE__V5*/ meltfptr[3] = NULL;;
      }
    ;
    /*^compute */
 /*_#OLDSIZE__L2*/ meltfnum[0] = 0;;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1655:/ cond");
    /*cond */ if (( /*!NOTNULL */ meltfrout->tabval[2]))	/*then */
      {
	/*^cond.then */
	/*_._IF___V6*/ meltfptr[5] = /*_.OLDTUPLE__V5*/ meltfptr[3];;
      }
    else
      {
	MELT_LOCATION ("warmelt-base.melt:1655:/ cond.else");

  /*_._IF___V6*/ meltfptr[5] = NULL;;
      }
    ;
    /*^compute */
 /*_#MULTIPLE_LENGTH__L3*/ meltfnum[2] =
      (melt_multiple_length
       ((melt_ptr_t) ( /*_.OLDTUPLE__V5*/ meltfptr[3])));;
    MELT_LOCATION ("warmelt-base.melt:1656:/ compute");
    /*_#OLDSIZE__L2*/ meltfnum[0] = /*_#SETQ___L4*/ meltfnum[3] =
      /*_#MULTIPLE_LENGTH__L3*/ meltfnum[2];;
    MELT_LOCATION ("warmelt-base.melt:1657:/ quasiblock");


 /*_#MULTIPLE_LENGTH__L5*/ meltfnum[4] =
      (melt_multiple_length
       ((melt_ptr_t) ( /*_.OLDTUPLE__V5*/ meltfptr[3])));;
    /*^compute */
 /*_#LIST_LENGTH__L6*/ meltfnum[5] =
      (melt_list_length ((melt_ptr_t) ( /*_.LSTHANDLER__V2*/ meltfptr[1])));;
    /*^compute */
 /*_#NEWSIZE__L7*/ meltfnum[6] =
      (( /*_#MULTIPLE_LENGTH__L5*/ meltfnum[4]) +
       ( /*_#LIST_LENGTH__L6*/ meltfnum[5]));;
    /*^compute */
 /*_.NEWTUPLE__V7*/ meltfptr[6] =
      (meltgc_new_multiple
       ((meltobject_ptr_t) (( /*!DISCR_MULTIPLE */ meltfrout->tabval[3])),
	( /*_#NEWSIZE__L7*/ meltfnum[6])));;
    /*^compute */
 /*_#I__L8*/ meltfnum[7] = 0;;
    /*citerblock FOREACH_IN_MULTIPLE */
    {
      /* start foreach_in_multiple meltcit1__EACHTUP */
      long meltcit1__EACHTUP_ln =
	melt_multiple_length ((melt_ptr_t) /*_.OLDTUPLE__V5*/ meltfptr[3]);
      for ( /*_#IUNUSED__L9*/ meltfnum[8] = 0;
	   ( /*_#IUNUSED__L9*/ meltfnum[8] >= 0)
	   && ( /*_#IUNUSED__L9*/ meltfnum[8] < meltcit1__EACHTUP_ln);
	/*_#IUNUSED__L9*/ meltfnum[8]++)
	{
	  /*_.CURHANDER__V8*/ meltfptr[7] =
	    melt_multiple_nth ((melt_ptr_t) ( /*_.OLDTUPLE__V5*/ meltfptr[3]),
			       /*_#IUNUSED__L9*/ meltfnum[8]);




	  {
	    MELT_LOCATION ("warmelt-base.melt:1665:/ locexp");
	    meltgc_multiple_put_nth ((melt_ptr_t)
				     ( /*_.NEWTUPLE__V7*/ meltfptr[6]),
				     ( /*_#I__L8*/ meltfnum[7]),
				     (melt_ptr_t) ( /*_.CURHANDER__V8*/
						   meltfptr[7]));
	  }
	  ;
  /*_#plI__L10*/ meltfnum[9] =
	    (( /*_#I__L8*/ meltfnum[7]) + (1));;
	  MELT_LOCATION ("warmelt-base.melt:1666:/ compute");
	  /*_#I__L8*/ meltfnum[7] = /*_#SETQ___L11*/ meltfnum[10] =
	    /*_#plI__L10*/ meltfnum[9];;
	  if ( /*_#IUNUSED__L9*/ meltfnum[8] < 0)
	    break;
	}			/* end  foreach_in_multiple meltcit1__EACHTUP */

      /*citerepilog */

      MELT_LOCATION ("warmelt-base.melt:1662:/ clear");
	    /*clear *//*_.CURHANDER__V8*/ meltfptr[7] = 0;
      /*^clear */
	    /*clear *//*_#IUNUSED__L9*/ meltfnum[8] = 0;
      /*^clear */
	    /*clear *//*_#plI__L10*/ meltfnum[9] = 0;
      /*^clear */
	    /*clear *//*_#SETQ___L11*/ meltfnum[10] = 0;
    }				/*endciterblock FOREACH_IN_MULTIPLE */
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit2__EACHLIST */
      for ( /*_.CURPAIR__V9*/ meltfptr[8] =
	   melt_list_first ((melt_ptr_t) /*_.LSTHANDLER__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V9*/ meltfptr[8]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURPAIR__V9*/ meltfptr[8] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V9*/ meltfptr[8]))
	{
	  /*_.CURHANDLER__V10*/ meltfptr[9] =
	    melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V9*/ meltfptr[8]);



#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-base.melt:1672:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
    /*_#IS_A__L12*/ meltfnum[11] =
	      melt_is_instance_of ((melt_ptr_t)
				   ( /*_.CURHANDLER__V10*/ meltfptr[9]),
				   (melt_ptr_t) (( /*!CLASS_GCC_PRAGMA */
						  meltfrout->tabval[4])));;
	    MELT_LOCATION ("warmelt-base.melt:1672:/ cond");
	    /*cond */ if ( /*_#IS_A__L12*/ meltfnum[11])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V12*/ meltfptr[11] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-base.melt:1672:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("register_pragma_handler must be a list of class_gcc_pragma."), ("warmelt-base.melt") ? ("warmelt-base.melt") : __FILE__, (1672) ? (1672) : __LINE__, __FUNCTION__);
		    ;
		  }
		  ;
		/*clear *//*_._IFELSE___V12*/ meltfptr[11] = 0;
		  /*epilog */
		}
		;
	      }
	    ;
	    /*^compute */
	    /*_.IFCPP___V11*/ meltfptr[10] =
	      /*_._IFELSE___V12*/ meltfptr[11];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-base.melt:1672:/ clear");
	      /*clear *//*_#IS_A__L12*/ meltfnum[11] = 0;
	    /*^clear */
	      /*clear *//*_._IFELSE___V12*/ meltfptr[11] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V11*/ meltfptr[10] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;

	  {
	    MELT_LOCATION ("warmelt-base.melt:1674:/ locexp");
	    meltgc_multiple_put_nth ((melt_ptr_t)
				     ( /*_.NEWTUPLE__V7*/ meltfptr[6]),
				     ( /*_#I__L8*/ meltfnum[7]),
				     (melt_ptr_t) ( /*_.CURHANDLER__V10*/
						   meltfptr[9]));
	  }
	  ;
  /*_#plI__L13*/ meltfnum[11] =
	    (( /*_#I__L8*/ meltfnum[7]) + (1));;
	  MELT_LOCATION ("warmelt-base.melt:1675:/ compute");
	  /*_#I__L8*/ meltfnum[7] = /*_#SETQ___L14*/ meltfnum[13] =
	    /*_#plI__L13*/ meltfnum[11];;
	}			/* end foreach_in_list meltcit2__EACHLIST */
     /*_.CURPAIR__V9*/ meltfptr[8] = NULL;
     /*_.CURHANDLER__V10*/ meltfptr[9] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-base.melt:1669:/ clear");
	    /*clear *//*_.CURPAIR__V9*/ meltfptr[8] = 0;
      /*^clear */
	    /*clear *//*_.CURHANDLER__V10*/ meltfptr[9] = 0;
      /*^clear */
	    /*clear *//*_.IFCPP___V11*/ meltfptr[10] = 0;
      /*^clear */
	    /*clear *//*_#plI__L13*/ meltfnum[11] = 0;
      /*^clear */
	    /*clear *//*_#SETQ___L14*/ meltfnum[13] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1677:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!INITIAL_SYSTEM_DATA */
					  meltfrout->tabval[0])),
					(melt_ptr_t) (( /*!CLASS_SYSTEM_DATA */ meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("checkobj putslot  _ @SYSDATA_MELTPRAGMAS",
			  melt_magic_discr ((melt_ptr_t)
					    (( /*!INITIAL_SYSTEM_DATA */
					      meltfrout->tabval[0]))) ==
			  MELTOBMAG_OBJECT);
	  melt_putfield_object ((( /*!INITIAL_SYSTEM_DATA */ meltfrout->
				  tabval[0])), (11),
				( /*_.NEWTUPLE__V7*/ meltfptr[6]),
				"SYSDATA_MELTPRAGMAS");
	  ;
	  /*^touch */
	  meltgc_touch (( /*!INITIAL_SYSTEM_DATA */ meltfrout->tabval[0]));
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object (( /*!INITIAL_SYSTEM_DATA */
					 meltfrout->tabval[0]), "put-fields");
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-base.melt:1657:/ clear");
	   /*clear *//*_#MULTIPLE_LENGTH__L5*/ meltfnum[4] = 0;
    /*^clear */
	   /*clear *//*_#LIST_LENGTH__L6*/ meltfnum[5] = 0;
    /*^clear */
	   /*clear *//*_#NEWSIZE__L7*/ meltfnum[6] = 0;
    /*^clear */
	   /*clear *//*_.NEWTUPLE__V7*/ meltfptr[6] = 0;
    /*^clear */
	   /*clear *//*_#I__L8*/ meltfnum[7] = 0;

    MELT_LOCATION ("warmelt-base.melt:1653:/ clear");
	   /*clear *//*_.OLDTUPLE__V5*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#OLDSIZE__L2*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_._IF___V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_#MULTIPLE_LENGTH__L3*/ meltfnum[2] = 0;
    /*^clear */
	   /*clear *//*_#SETQ___L4*/ meltfnum[3] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1646:/ clear");
	   /*clear *//*_.IFCPP___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_PRAGMA_HANDLER", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_22_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiBASE_SET_REFERENCE (meltclosure_ptr_t meltclosp_,
					 melt_ptr_t meltfirstargp_,
					 const melt_argdescr_cell_t
					 meltxargdescr_[],
					 union meltparam_un * meltxargtab_,
					 const melt_argdescr_cell_t
					 meltxresdescr_[],
					 union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_23_WARMELTmiBASE_SET_REFERENCE_melt
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

  struct meltframe_meltrout_23_WARMELTmiBASE_SET_REFERENCE_st
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
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_23_WARMELTmiBASE_SET_REFERENCE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_23_WARMELTmiBASE_SET_REFERENCE_st *)
	meltfirstargp_;
      /* use arguments meltrout_23_WARMELTmiBASE_SET_REFERENCE output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_23_WARMELTmiBASE_SET_REFERENCE nbval 3*/
  meltfram__.mcfr_nbvar = 3 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("SET_REFERENCE", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1730:/ getarg");
 /*_.R__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

  /*getarg#1 */
  /*^getarg */
  if (meltxargdescr_[0] != MELTBPAR_PTR)
    goto meltlab_endgetargs;
 /*_.V__V3*/ meltfptr[2] =
    (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
  gcc_assert (melt_discr ((melt_ptr_t) ( /*_.V__V3*/ meltfptr[2])) != NULL);

  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:1733:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					( /*_.R__V2*/ meltfptr[1]),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[0])))
      )				/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*^putslot */
	  /*putslot */
	  melt_assertmsg ("checkobj putslot  _ @REFERENCED_VALUE",
			  melt_magic_discr ((melt_ptr_t)
					    ( /*_.R__V2*/ meltfptr[1])) ==
			  MELTOBMAG_OBJECT);
	  melt_putfield_object (( /*_.R__V2*/ meltfptr[1]), (0),
				( /*_.V__V3*/ meltfptr[2]),
				"REFERENCED_VALUE");
	  ;
	  /*^touch */
	  meltgc_touch ( /*_.R__V2*/ meltfptr[1]);
	  ;
	  /*^touchobj */

	  melt_dbgtrace_written_object ( /*_.R__V2*/ meltfptr[1],
					"put-fields");
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("SET_REFERENCE", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_23_WARMELTmiBASE_SET_REFERENCE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_23_WARMELTmiBASE_SET_REFERENCE */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_melt
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

  struct meltframe_meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_st
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
#define MELTFRAM_NBVARPTR 5
    melt_ptr_t mcfr_varptr[5];
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_st *)
	meltfirstargp_;
      /* use arguments meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 5; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR nbval 5*/
  meltfram__.mcfr_nbvar = 5 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("INSTALL_VALUE_DESCRIPTOR", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:1742:/ getarg");
 /*_.VD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


#if MELT_HAVE_DEBUG
    MELT_LOCATION ("warmelt-base.melt:1743:/ cppif.then");
    /*^block */
    /*anyblock */
    {


      MELT_CHECK_SIGNAL ();
      ;
   /*_#IS_A__L1*/ meltfnum[0] =
	melt_is_instance_of ((melt_ptr_t) ( /*_.VD__V2*/ meltfptr[1]),
			     (melt_ptr_t) (( /*!CLASS_VALUE_DESCRIPTOR */
					    meltfrout->tabval[0])));;
      MELT_LOCATION ("warmelt-base.melt:1743:/ cond");
      /*cond */ if ( /*_#IS_A__L1*/ meltfnum[0])	/*then */
	{
	  /*^cond.then */
	  /*_._IFELSE___V4*/ meltfptr[3] = ( /*nil */ NULL);;
	}
      else
	{
	  MELT_LOCATION ("warmelt-base.melt:1743:/ cond.else");

	  /*^block */
	  /*anyblock */
	  {




	    {
	      /*^locexp */
	      melt_assert_failed (("check vd"),
				  ("warmelt-base.melt")
				  ? ("warmelt-base.melt") : __FILE__,
				  (1743) ? (1743) : __LINE__, __FUNCTION__);
	      ;
	    }
	    ;
	       /*clear *//*_._IFELSE___V4*/ meltfptr[3] = 0;
	    /*epilog */
	  }
	  ;
	}
      ;
      /*^compute */
      /*_.IFCPP___V3*/ meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
      /*epilog */

      MELT_LOCATION ("warmelt-base.melt:1743:/ clear");
	     /*clear *//*_#IS_A__L1*/ meltfnum[0] = 0;
      /*^clear */
	     /*clear *//*_._IFELSE___V4*/ meltfptr[3] = 0;
    }

#else /*MELT_HAVE_DEBUG */
    /*^cppif.else */
    /*_.IFCPP___V3*/ meltfptr[2] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
    ;
    MELT_LOCATION ("warmelt-base.melt:1744:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!VALUE_DESCRIPTOR_LIST_REFERENCE */ meltfrout->tabval[1])),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[2])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!VALUE_DESCRIPTOR_LIST_REFERENCE */ meltfrout->
			   tabval[1])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
   /*_.REFERENCED_VALUE__V5*/ meltfptr[3] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.REFERENCED_VALUE__V5*/ meltfptr[3] = NULL;;
      }
    ;

    {
      MELT_LOCATION ("warmelt-base.melt:1744:/ locexp");
      meltgc_append_list ((melt_ptr_t)
			  ( /*_.REFERENCED_VALUE__V5*/ meltfptr[3]),
			  (melt_ptr_t) ( /*_.VD__V2*/ meltfptr[1]));
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:1742:/ clear");
	   /*clear *//*_.IFCPP___V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.REFERENCED_VALUE__V5*/ meltfptr[3] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("INSTALL_VALUE_DESCRIPTOR", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_24_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_melt
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

  struct meltframe_meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_st
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
#define MELTFRAM_NBVARPTR 2
    melt_ptr_t mcfr_varptr[2];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 2; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST nbval 2*/
  meltfram__.mcfr_nbvar = 2 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("RETRIEVE_VALUE_DESCRIPTOR_LIST", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-base.melt:1747:/ block");
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-base.melt:1748:/ cond");
    /*cond */ if (
		   /*ifisa */
		   melt_is_instance_of ((melt_ptr_t)
					(( /*!VALUE_DESCRIPTOR_LIST_REFERENCE */ meltfrout->tabval[0])),
					(melt_ptr_t) (( /*!CLASS_REFERENCE */
						       meltfrout->tabval[1])))
      )				/*then */
      {
	/*^cond.then */
	/*^getslot */
	{
	  melt_ptr_t slot = NULL, obj = NULL;
	  obj =
	    (melt_ptr_t) (( /*!VALUE_DESCRIPTOR_LIST_REFERENCE */ meltfrout->
			   tabval[0])) /*=obj*/ ;
	  melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
   /*_.REFERENCED_VALUE__V2*/ meltfptr[1] = slot;
	};
	;
      }
    else
      {				/*^cond.else */

  /*_.REFERENCED_VALUE__V2*/ meltfptr[1] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-base.melt:1747:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.REFERENCED_VALUE__V2*/ meltfptr[1];;

    {
      MELT_LOCATION ("warmelt-base.melt:1747:/ locexp");
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
	   /*clear *//*_.REFERENCED_VALUE__V2*/ meltfptr[1] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("RETRIEVE_VALUE_DESCRIPTOR_LIST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_25_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_melt
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

  struct meltframe_meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_st
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
#define MELTFRAM_NBVARPTR 18
    melt_ptr_t mcfr_varptr[18];
#define MELTFRAM_NBVARNUM 6
    long mcfr_varnum[6];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_st *)
	meltfirstargp_;
      /* use arguments meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 18; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT nbval 18*/
  meltfram__.mcfr_nbvar = 18 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LIST_REMOVE_LAST_ELEMENT", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:2835:/ getarg");
 /*_.LIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_LIST__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.LIS__V2*/ meltfptr[1])) ==
       MELTOBMAG_LIST);;
    /*^compute */
 /*_#NOT__L2*/ meltfnum[1] =
      (!( /*_#IS_LIST__L1*/ meltfnum[0]));;
    MELT_LOCATION ("warmelt-base.melt:2839:/ cond");
    /*cond */ if ( /*_#NOT__L2*/ meltfnum[1])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-base.melt:2840:/ quasiblock");


	  /*_._RETVAL___V1*/ meltfptr[0] = ( /*nil */ NULL);;

	  {
	    MELT_LOCATION ("warmelt-base.melt:2840:/ locexp");
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
	  /*_._IF___V3*/ meltfptr[2] = /*_.RETURN___V4*/ meltfptr[3];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:2839:/ clear");
	     /*clear *//*_.RETURN___V4*/ meltfptr[3] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V3*/ meltfptr[2] = NULL;;
      }
    ;
    MELT_LOCATION ("warmelt-base.melt:2841:/ quasiblock");


 /*_.LASTPAIR__V5*/ meltfptr[3] =
      (melt_list_last ((melt_ptr_t) ( /*_.LIS__V2*/ meltfptr[1])));;
    /*^compute */
    /*_.PREVPAIR__V6*/ meltfptr[5] = ( /*nil */ NULL);;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:2844:/ cond");
    /*cond */ if ( /*_.LASTPAIR__V5*/ meltfptr[3])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


#if MELT_HAVE_DEBUG
	  /*^cppif.then */
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
     /*_#IS_PAIR__L3*/ meltfnum[2] =
	      (melt_magic_discr
	       ((melt_ptr_t) ( /*_.LASTPAIR__V5*/ meltfptr[3])) ==
	       MELTOBMAG_PAIR);;
	    MELT_LOCATION ("warmelt-base.melt:2844:/ cond");
	    /*cond */ if ( /*_#IS_PAIR__L3*/ meltfnum[2])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V9*/ meltfptr[8] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-base.melt:2844:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("check lastpair"),
					("warmelt-base.melt")
					? ("warmelt-base.melt") : __FILE__,
					(2844) ? (2844) : __LINE__,
					__FUNCTION__);
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
	    /*_.IFCPP___V8*/ meltfptr[7] = /*_._IFELSE___V9*/ meltfptr[8];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-base.melt:2844:/ clear");
	       /*clear *//*_#IS_PAIR__L3*/ meltfnum[2] = 0;
	    /*^clear */
	       /*clear *//*_._IFELSE___V9*/ meltfptr[8] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V8*/ meltfptr[7] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;
	  /*^compute */
	  /*_._IF___V7*/ meltfptr[6] = /*_.IFCPP___V8*/ meltfptr[7];;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:2844:/ clear");
	     /*clear *//*_.IFCPP___V8*/ meltfptr[7] = 0;
	}
	;
      }
    else
      {				/*^cond.else */

  /*_._IF___V7*/ meltfptr[6] = NULL;;
      }
    ;
    /*citerblock FOREACH_IN_LIST */
    {
      /* start foreach_in_list meltcit1__EACHLIST */
      for ( /*_.CURPAIR__V10*/ meltfptr[8] =
	   melt_list_first ((melt_ptr_t) /*_.LIS__V2*/ meltfptr[1]);
	   melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[8]) ==
	   MELTOBMAG_PAIR;
	   /*_.CURPAIR__V10*/ meltfptr[8] =
	   melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[8]))
	{
	  /*_.CURELEM__V11*/ meltfptr[7] =
	    melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[8]);



#if MELT_HAVE_DEBUG
	  MELT_LOCATION ("warmelt-base.melt:2848:/ cppif.then");
	  /*^block */
	  /*anyblock */
	  {


	    MELT_CHECK_SIGNAL ();
	    ;
    /*_#IS_PAIR__L4*/ meltfnum[2] =
	      (melt_magic_discr
	       ((melt_ptr_t) ( /*_.CURPAIR__V10*/ meltfptr[8])) ==
	       MELTOBMAG_PAIR);;
	    MELT_LOCATION ("warmelt-base.melt:2848:/ cond");
	    /*cond */ if ( /*_#IS_PAIR__L4*/ meltfnum[2])	/*then */
	      {
		/*^cond.then */
		/*_._IFELSE___V13*/ meltfptr[12] = ( /*nil */ NULL);;
	      }
	    else
	      {
		MELT_LOCATION ("warmelt-base.melt:2848:/ cond.else");

		/*^block */
		/*anyblock */
		{




		  {
		    /*^locexp */
		    melt_assert_failed (("check curpair"),
					("warmelt-base.melt")
					? ("warmelt-base.melt") : __FILE__,
					(2848) ? (2848) : __LINE__,
					__FUNCTION__);
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
	    /*_.IFCPP___V12*/ meltfptr[11] =
	      /*_._IFELSE___V13*/ meltfptr[12];;
	    /*epilog */

	    MELT_LOCATION ("warmelt-base.melt:2848:/ clear");
	      /*clear *//*_#IS_PAIR__L4*/ meltfnum[2] = 0;
	    /*^clear */
	      /*clear *//*_._IFELSE___V13*/ meltfptr[12] = 0;
	  }

#else /*MELT_HAVE_DEBUG */
	  /*^cppif.else */
	  /*_.IFCPP___V12*/ meltfptr[11] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  /*^compute */
  /*_#eqeq__L5*/ meltfnum[2] =
	    (( /*_.CURPAIR__V10*/ meltfptr[8]) ==
	     ( /*_.LASTPAIR__V5*/ meltfptr[3]));;
	  MELT_LOCATION ("warmelt-base.melt:2849:/ cond");
	  /*cond */ if ( /*_#eqeq__L5*/ meltfnum[2])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		MELT_LOCATION ("warmelt-base.melt:2850:/ compute");
		/*_.CURPAIR__V10*/ meltfptr[8] =
		  /*_.SETQ___V15*/ meltfptr[14] = ( /*nil */ NULL);;

		MELT_CHECK_SIGNAL ();
		;
    /*_#IS_PAIR__L6*/ meltfnum[5] =
		  (melt_magic_discr
		   ((melt_ptr_t) ( /*_.PREVPAIR__V6*/ meltfptr[5])) ==
		   MELTOBMAG_PAIR);;
		MELT_LOCATION ("warmelt-base.melt:2851:/ cond");
		/*cond */ if ( /*_#IS_PAIR__L6*/ meltfnum[5])	/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {


		      {
			MELT_LOCATION ("warmelt-base.melt:2852:/ locexp");
			/* list_remove_last_element UPDATEPAIR_CHK__1 */
			((struct meltpair_st *) /*_.PREVPAIR__V6*/
			 meltfptr[5])->tl = NULL;
			((struct meltlist_st *) /*_.LIS__V2*/ meltfptr[1])->last
			  = (struct meltpair_st *) /*_.PREVPAIR__V6*/
			  meltfptr[5];
			meltgc_touch_dest ( /*_.LIS__V2*/ meltfptr[1],
					   /*_.PREVPAIR__V6*/ meltfptr[5]);
			;
		      }
		      ;
		      /*epilog */
		    }
		    ;
		  }
		else
		  {
		    MELT_LOCATION ("warmelt-base.melt:2851:/ cond.else");

		    /*^block */
		    /*anyblock */
		    {


		      {
			MELT_LOCATION ("warmelt-base.melt:2859:/ locexp");
			/* list_remove_last_element EMPTYLIST_CHK__1 */
			((struct meltlist_st *) /*_.LIS__V2*/ meltfptr[1])->
			  first = NULL;
			((struct meltlist_st *) /*_.LIS__V2*/ meltfptr[1])->
			  last = NULL;
			meltgc_touch ( /*_.LIS__V2*/ meltfptr[1]);
			;
		      }
		      ;
		      /*epilog */
		    }
		    ;
		  }
		;

		MELT_CHECK_SIGNAL ();
		;
		MELT_LOCATION ("warmelt-base.melt:2865:/ quasiblock");


		/*_._RETVAL___V1*/ meltfptr[0] =
		  /*_.CURELEM__V11*/ meltfptr[7];;

		{
		  MELT_LOCATION ("warmelt-base.melt:2865:/ locexp");
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
		MELT_LOCATION ("warmelt-base.melt:2849:/ quasiblock");


		/*_.PROGN___V17*/ meltfptr[16] =
		  /*_.RETURN___V16*/ meltfptr[15];;
		/*^compute */
		/*_._IF___V14*/ meltfptr[12] =
		  /*_.PROGN___V17*/ meltfptr[16];;
		/*epilog */

		MELT_LOCATION ("warmelt-base.melt:2849:/ clear");
	      /*clear *//*_.SETQ___V15*/ meltfptr[14] = 0;
		/*^clear */
	      /*clear *//*_#IS_PAIR__L6*/ meltfnum[5] = 0;
		/*^clear */
	      /*clear *//*_.RETURN___V16*/ meltfptr[15] = 0;
		/*^clear */
	      /*clear *//*_.PROGN___V17*/ meltfptr[16] = 0;
	      }
	      ;
	    }
	  else
	    {			/*^cond.else */

   /*_._IF___V14*/ meltfptr[12] = NULL;;
	    }
	  ;
	  MELT_LOCATION ("warmelt-base.melt:2866:/ compute");
	  /*_.PREVPAIR__V6*/ meltfptr[5] = /*_.SETQ___V18*/ meltfptr[14] =
	    /*_.CURPAIR__V10*/ meltfptr[8];;
	}			/* end foreach_in_list meltcit1__EACHLIST */
     /*_.CURPAIR__V10*/ meltfptr[8] = NULL;
     /*_.CURELEM__V11*/ meltfptr[7] = NULL;


      /*citerepilog */

      MELT_LOCATION ("warmelt-base.melt:2845:/ clear");
	    /*clear *//*_.CURPAIR__V10*/ meltfptr[8] = 0;
      /*^clear */
	    /*clear *//*_.CURELEM__V11*/ meltfptr[7] = 0;
      /*^clear */
	    /*clear *//*_.IFCPP___V12*/ meltfptr[11] = 0;
      /*^clear */
	    /*clear *//*_#eqeq__L5*/ meltfnum[2] = 0;
      /*^clear */
	    /*clear *//*_._IF___V14*/ meltfptr[12] = 0;
      /*^clear */
	    /*clear *//*_.SETQ___V18*/ meltfptr[14] = 0;
    }				/*endciterblock FOREACH_IN_LIST */
    ;

    MELT_LOCATION ("warmelt-base.melt:2841:/ clear");
	   /*clear *//*_.LASTPAIR__V5*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_.PREVPAIR__V6*/ meltfptr[5] = 0;
    /*^clear */
	   /*clear *//*_._IF___V7*/ meltfptr[6] = 0;

    MELT_CHECK_SIGNAL ();
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-base.melt:2835:/ clear");
	   /*clear *//*_#IS_LIST__L1*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_#NOT__L2*/ meltfnum[1] = 0;
    /*^clear */
	   /*clear *//*_._IF___V3*/ meltfptr[2] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LIST_REMOVE_LAST_ELEMENT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_26_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE (meltclosure_ptr_t
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
  /*variadic */ int variad_COMPLETE_SEQUENCE_AS_TUPLE_ix =
    0, variad_COMPLETE_SEQUENCE_AS_TUPLE_len =
    melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_COMPLETE_SEQUENCE_AS_TUPLE_len)
#define melt_variadic_index variad_COMPLETE_SEQUENCE_AS_TUPLE_ix

  long
    current_blocklevel_signals_meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_melt
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

  struct meltframe_meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_st
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
#define MELTFRAM_NBVARPTR 18
    melt_ptr_t mcfr_varptr[18];
#define MELTFRAM_NBVARNUM 3
    long mcfr_varnum[3];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_st *)
	meltfirstargp_;
      /* use arguments meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 18; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE nbval 18*/
  meltfram__.mcfr_nbvar = 18 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("COMPLETE_SEQUENCE_AS_TUPLE", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-base.melt:2921:/ getarg");
 /*_.SRC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-base.melt:2925:/ quasiblock");


 /*_.NEWLIST__V4*/ meltfptr[3] =
      (meltgc_new_list
       ((meltobject_ptr_t) (( /*!DISCR_LIST */ meltfrout->tabval[0]))));;

    MELT_CHECK_SIGNAL ();
    ;
    /*^compute */
 /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.SRC__V2*/ meltfptr[1])) ==
       MELTOBMAG_MULTIPLE);;
    MELT_LOCATION ("warmelt-base.melt:2927:/ cond");
    /*cond */ if ( /*_#IS_MULTIPLE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  /*citerblock FOREACH_IN_MULTIPLE */
	  {
	    /* start foreach_in_multiple meltcit1__EACHTUP */
	    long meltcit1__EACHTUP_ln =
	      melt_multiple_length ((melt_ptr_t) /*_.SRC__V2*/ meltfptr[1]);
	    for ( /*_#IX__L2*/ meltfnum[1] = 0;
		 ( /*_#IX__L2*/ meltfnum[1] >= 0)
		 && ( /*_#IX__L2*/ meltfnum[1] < meltcit1__EACHTUP_ln);
	/*_#IX__L2*/ meltfnum[1]++)
	      {
		/*_.COMP__V5*/ meltfptr[4] =
		  melt_multiple_nth ((melt_ptr_t)
				     ( /*_.SRC__V2*/ meltfptr[1]),
				     /*_#IX__L2*/ meltfnum[1]);




		{
		  MELT_LOCATION ("warmelt-base.melt:2931:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.NEWLIST__V4*/ meltfptr[3]),
				      (melt_ptr_t) ( /*_.COMP__V5*/
						    meltfptr[4]));
		}
		;
		if ( /*_#IX__L2*/ meltfnum[1] < 0)
		  break;
	      }			/* end  foreach_in_multiple meltcit1__EACHTUP */

	    /*citerepilog */

	    MELT_LOCATION ("warmelt-base.melt:2928:/ clear");
	      /*clear *//*_.COMP__V5*/ meltfptr[4] = 0;
	    /*^clear */
	      /*clear *//*_#IX__L2*/ meltfnum[1] = 0;
	  }			/*endciterblock FOREACH_IN_MULTIPLE */
	  ;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-base.melt:2927:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#IS_LIST__L3*/ meltfnum[2] =
	    (melt_magic_discr ((melt_ptr_t) ( /*_.SRC__V2*/ meltfptr[1])) ==
	     MELTOBMAG_LIST);;
	  MELT_LOCATION ("warmelt-base.melt:2933:/ cond");
	  /*cond */ if ( /*_#IS_LIST__L3*/ meltfnum[2])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

		/*citerblock FOREACH_IN_LIST */
		{
		  /* start foreach_in_list meltcit2__EACHLIST */
		  for ( /*_.CURPAIR__V6*/ meltfptr[5] =
		       melt_list_first ((melt_ptr_t) /*_.SRC__V2*/
					meltfptr[1]);
		       melt_magic_discr ((melt_ptr_t) /*_.CURPAIR__V6*/
					 meltfptr[5]) == MELTOBMAG_PAIR;
		       /*_.CURPAIR__V6*/ meltfptr[5] =
		       melt_pair_tail ((melt_ptr_t) /*_.CURPAIR__V6*/
				       meltfptr[5]))
		    {
		      /*_.CURCOMP__V7*/ meltfptr[6] =
			melt_pair_head ((melt_ptr_t) /*_.CURPAIR__V6*/
					meltfptr[5]);



		      {
			MELT_LOCATION ("warmelt-base.melt:2937:/ locexp");
			meltgc_append_list ((melt_ptr_t)
					    ( /*_.NEWLIST__V4*/ meltfptr[3]),
					    (melt_ptr_t) ( /*_.CURCOMP__V7*/
							  meltfptr[6]));
		      }
		      ;
		    }		/* end foreach_in_list meltcit2__EACHLIST */
     /*_.CURPAIR__V6*/ meltfptr[5] = NULL;
     /*_.CURCOMP__V7*/ meltfptr[6] = NULL;


		  /*citerepilog */

		  MELT_LOCATION ("warmelt-base.melt:2934:/ clear");
		/*clear *//*_.CURPAIR__V6*/ meltfptr[5] = 0;
		  /*^clear */
		/*clear *//*_.CURCOMP__V7*/ meltfptr[6] = 0;
		}		/*endciterblock FOREACH_IN_LIST */
		;
		/*epilog */
	      }
	      ;
	    }
	  else
	    {
	      MELT_LOCATION ("warmelt-base.melt:2933:/ cond.else");

	      /*^block */
	      /*anyblock */
	      {


		{
		  MELT_LOCATION ("warmelt-base.melt:2940:/ locexp");
		  meltgc_append_list ((melt_ptr_t)
				      ( /*_.NEWLIST__V4*/ meltfptr[3]),
				      (melt_ptr_t) ( /*_.SRC__V2*/
						    meltfptr[1]));
		}
		;
		MELT_LOCATION ("warmelt-base.melt:2939:/ quasiblock");


		/*epilog */
	      }
	      ;
	    }
	  ;
	  /*epilog */

	  MELT_LOCATION ("warmelt-base.melt:2927:/ clear");
	     /*clear *//*_#IS_LIST__L3*/ meltfnum[2] = 0;
	}
	;
      }
    ;
    MELT_LOCATION ("warmelt-base.melt:2941:/ loop");
    /*loop */
    {
    meltlabloop_ARGLOOP_3:;	/*^loopbody */

      /*^block */
      /*anyblock */
      {


	MELT_CHECK_SIGNAL ();
	;
	MELT_LOCATION ("warmelt-base.melt:2943:/ cond");
	/*cond */ if ( /*ifvariadic nomore */ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix == variad_COMPLETE_SEQUENCE_AS_TUPLE_len)	/*then */
	  {
	    /*^cond.then */
	    /*^block */
	    /*anyblock */
	    {

	      /*^compute */

	      /*consume variadic  ! */ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix +=
		0;;
	      MELT_LOCATION ("warmelt-base.melt:2945:/ quasiblock");


	      /*^compute */
     /*_.ARGLOOP__V9*/ meltfptr[8] = NULL;;

	      /*^exit */
	      /*exit */
	      {
		goto meltlabexit_ARGLOOP_3;
	      }
	      ;
	      /*epilog */
	    }
	    ;
	  }
	else
	  {
	    MELT_LOCATION ("warmelt-base.melt:2943:/ cond.else");

	    /*^block */
	    /*anyblock */
	    {

	      /*^cond */
	      /*cond */ if ( /*ifvariadic arg#1 */ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix >= 0 && variad_COMPLETE_SEQUENCE_AS_TUPLE_ix + 1 <= variad_COMPLETE_SEQUENCE_AS_TUPLE_len && meltxargdescr_[variad_COMPLETE_SEQUENCE_AS_TUPLE_ix] == MELTBPAR_PTR)	/*then */
		{
		  /*^cond.then */
		  /*^block */
		  /*anyblock */
		  {

		    /*^compute */
       /*_.V__V10*/ meltfptr[9] =
		      /*variadic argument value */
		      ((meltxargtab_
			[variad_COMPLETE_SEQUENCE_AS_TUPLE_ix +
			 0].
			meltbp_aptr)
		       ? (*
			  (meltxargtab_
			   [variad_COMPLETE_SEQUENCE_AS_TUPLE_ix +
			    0].meltbp_aptr)) : NULL);;
		    /*^compute */

		    /*consume variadic Value ! */
		      variad_COMPLETE_SEQUENCE_AS_TUPLE_ix += 1;;

		    {
		      MELT_LOCATION ("warmelt-base.melt:2947:/ locexp");
		      meltgc_append_list ((melt_ptr_t)
					  ( /*_.NEWLIST__V4*/ meltfptr[3]),
					  (melt_ptr_t) ( /*_.V__V10*/
							meltfptr[9]));
		    }
		    ;
		    /*epilog */

		    MELT_LOCATION ("warmelt-base.melt:2943:/ clear");
		 /*clear *//*_.V__V10*/ meltfptr[9] = 0;
		  }
		  ;
		}
	      else
		{		/*^cond.else */

		  /*^block */
		  /*anyblock */
		  {

		    MELT_LOCATION ("warmelt-base.melt:2949:/ quasiblock");


       /*_.VCTY__V12*/ meltfptr[11] =
		      /*variadic_type_code */
#ifdef melt_variadic_index
		      (((melt_variadic_index + 0) >= 0
			&& (melt_variadic_index + 0) < melt_variadic_length)
		       ?
		       melt_code_to_ctype (meltxargdescr_
					   [melt_variadic_index +
					    0] & MELT_ARGDESCR_MAX) : NULL)
#else
		      NULL	/* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index */
		      ;;
		    MELT_LOCATION ("warmelt-base.melt:2952:/ cond");
		    /*cond */ if (
				   /*ifisa */
				   melt_is_instance_of ((melt_ptr_t)
							( /*_.VCTY__V12*/
							 meltfptr[11]),
							(melt_ptr_t) (( /*!CLASS_NAMED */ meltfrout->tabval[1])))
		      )		/*then */
		      {
			/*^cond.then */
			/*^getslot */
			{
			  melt_ptr_t slot = NULL, obj = NULL;
			  obj =
			    (melt_ptr_t) ( /*_.VCTY__V12*/ meltfptr[11])
			    /*=obj*/ ;
			  melt_object_get_field (slot, obj, 1, "NAMED_NAME");
	 /*_.NAMED_NAME__V13*/ meltfptr[12] = slot;
			};
			;
		      }
		    else
		      {		/*^cond.else */

	/*_.NAMED_NAME__V13*/ meltfptr[12] = NULL;;
		      }
		    ;

		    {
		      MELT_LOCATION ("warmelt-base.melt:2951:/ locexp");
		      error ("MELT ERROR MSG [#%ld]::: %s - %s",
			     melt_dbgcounter,
			     ("COMPLETE_SEQUENCE_AS_TUPLE with unsupported ctype"),
			     melt_string_str ((melt_ptr_t)
					      ( /*_.NAMED_NAME__V13*/
					       meltfptr[12])));
		    }
		    ;

#if MELT_HAVE_DEBUG
		    MELT_LOCATION ("warmelt-base.melt:2953:/ cppif.then");
		    /*^block */
		    /*anyblock */
		    {


		      MELT_CHECK_SIGNAL ();
		      ;
		      /*^cond */
		      /*cond */ if (( /*nil */ NULL))	/*then */
			{
			  /*^cond.then */
			  /*_._IFELSE___V15*/ meltfptr[14] =
			    ( /*nil */ NULL);;
			}
		      else
			{
			  MELT_LOCATION
			    ("warmelt-base.melt:2953:/ cond.else");

			  /*^block */
			  /*anyblock */
			  {




			    {
			      /*^locexp */
			      melt_assert_failed (("invalid variadic argument to COMPLETE_SEQUENCE_AS_TUPLE"), ("warmelt-base.melt") ? ("warmelt-base.melt") : __FILE__, (2953) ? (2953) : __LINE__, __FUNCTION__);
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
		      /*_.IFCPP___V14*/ meltfptr[13] =
			/*_._IFELSE___V15*/ meltfptr[14];;
		      /*epilog */

		      MELT_LOCATION ("warmelt-base.melt:2953:/ clear");
		   /*clear *//*_._IFELSE___V15*/ meltfptr[14] = 0;
		    }

#else /*MELT_HAVE_DEBUG */
		    /*^cppif.else */
		    /*_.IFCPP___V14*/ meltfptr[13] = ( /*nil */ NULL);
#endif /*MELT_HAVE_DEBUG */
		    ;
		    /*^compute */
		    /*_.LET___V11*/ meltfptr[9] =
		      /*_.IFCPP___V14*/ meltfptr[13];;

		    MELT_LOCATION ("warmelt-base.melt:2949:/ clear");
		 /*clear *//*_.VCTY__V12*/ meltfptr[11] = 0;
		    /*^clear */
		 /*clear *//*_.NAMED_NAME__V13*/ meltfptr[12] = 0;
		    /*^clear */
		 /*clear *//*_.IFCPP___V14*/ meltfptr[13] = 0;
		    /*epilog */

		    MELT_LOCATION ("warmelt-base.melt:2943:/ clear");
		 /*clear *//*_.LET___V11*/ meltfptr[9] = 0;
		  }
		  ;
		}
	      ;
	      /*epilog */
	    }
	    ;
	  }
	;
	/*epilog */
      }
      ;
      ;
      goto meltlabloop_ARGLOOP_3;
    meltlabexit_ARGLOOP_3:;
      MELT_LOCATION ("warmelt-base.melt:2941:/ loopepilog");
      /*loopepilog */
      /*_.FOREVER___V8*/ meltfptr[7] = /*_.ARGLOOP__V9*/ meltfptr[8];;
    }
    ;
    MELT_LOCATION ("warmelt-base.melt:2955:/ quasiblock");



    MELT_CHECK_SIGNAL ();
    ;
    /*^apply */
    /*apply */
    {
      /*_.RES__V17*/ meltfptr[11] =
	melt_apply ((meltclosure_ptr_t)
		    (( /*!LIST_TO_MULTIPLE */ meltfrout->tabval[2])),
		    (melt_ptr_t) ( /*_.NEWLIST__V4*/ meltfptr[3]), (""),
		    (union meltparam_un *) 0, "", (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-base.melt:2957:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V17*/ meltfptr[11];;

    {
      MELT_LOCATION ("warmelt-base.melt:2957:/ locexp");
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
    /*_.LET___V16*/ meltfptr[14] = /*_.RETURN___V18*/ meltfptr[12];;

    MELT_LOCATION ("warmelt-base.melt:2955:/ clear");
	   /*clear *//*_.RES__V17*/ meltfptr[11] = 0;
    /*^clear */
	   /*clear *//*_.RETURN___V18*/ meltfptr[12] = 0;
    /*_.LET___V3*/ meltfptr[2] = /*_.LET___V16*/ meltfptr[14];;

    MELT_LOCATION ("warmelt-base.melt:2925:/ clear");
	   /*clear *//*_.NEWLIST__V4*/ meltfptr[3] = 0;
    /*^clear */
	   /*clear *//*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0;
    /*^clear */
	   /*clear *//*_.FOREVER___V8*/ meltfptr[7] = 0;
    /*^clear */
	   /*clear *//*_.LET___V16*/ meltfptr[14] = 0;
    MELT_LOCATION ("warmelt-base.melt:2921:/ quasiblock");


    /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

    {
      MELT_LOCATION ("warmelt-base.melt:2921:/ locexp");
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
  melt_trace_end ("COMPLETE_SEQUENCE_AS_TUPLE", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_27_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE */



/**** end of warmelt-base+01.c ****/
