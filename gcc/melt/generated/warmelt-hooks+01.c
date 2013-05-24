/* GCC MELT GENERATED FILE warmelt-hooks+01.c - DO NOT EDIT */
/* secondary MELT generated C file of rank #1 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f1[] =
  MELT_RUN_HASHMD5 /* from melt-run.h */ ;


/**** warmelt-hooks+01.c declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-hooks ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2012 - 2013 Free Software Foundation, Inc.
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


/** ordinary MELT module meltbuild-sources/warmelt-hooks**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1	/*usualmodule */


/*** 14 extra MELT c-headers ***/


/** MELT extra c-header 1 : **/


static void
melt_override_gate_callback (void *gccdata, void *userdata ATTRIBUTE_UNUSED)
{
  bool *pgatestatus = (bool *) gccdata;
  bool gatsta = false;
  long oldgatstalng = 0, newgatstalng = 0;
  gcc_assert (pgatestatus != NULL);
  gatsta = *pgatestatus;
  oldgatstalng = newgatstalng = (long) gatsta;
  melthookproc_HOOK_OVERRIDE_GATE (oldgatstalng, &newgatstalng);
  gatsta = (newgatstalng != 0L);
  *pgatestatus = gatsta;
}				/* end of  melt_override_gate_callback */

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 2 : **/

/* flag for PLUGIN_OVERRIDE_GATE */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_override_gate_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 3 : **/

/* flag for PLUGIN_START_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_start_unit_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 4 : **/


void MELT_MODULE_VISIBILITY
melt_startunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			     void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 5 : **/

/* flag for PLUGIN_FINISH_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_finish_unit_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 6 : **/


void MELT_MODULE_VISIBILITY
melt_finishunithook_callback (void *gcc_data ATTRIBUTE_UNUSED,
			      void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 7 : **/

/* flag for PLUGIN_ALL_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_passes_start_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 8 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				     void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 9 : **/

/* flag for PLUGIN_ALL_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY melthk_all_passes_end_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 10 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				   void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 11 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_start_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 12 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_start_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
					 void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 13 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY
  melthk_all_ipa_passes_end_registered_flag;

/*** end of 14 extra MELT c-headers ***/


/** MELT extra c-header 14 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_end_hook_callback (void *gcc_data ATTRIBUTE_UNUSED,
				       void *user_data ATTRIBUTE_UNUSED);

/*** end of 14 extra MELT c-headers ***/

/** declaration of hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
MELT_EXTERN
  void melthook_HOOK_LOW_DEBUG_VALUE_AT (melt_ptr_t melthookdata,
					 melt_ptr_t meltinp0_VAL,
					 const char *meltinp1_FILENAME,
					 long meltinp2_LINENO,
					 const char *meltinp3_MSG,
					 long meltinp4_COUNT);


/* declare hook frame marking routine for melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout (struct
								       melt_callframe_st
								       *,
								       int);




/** declaration of hook melthook_HOOK_OVERRIDE_GATE */
MELT_EXTERN
  void melthook_HOOK_OVERRIDE_GATE (melt_ptr_t melthookdata,
				    long meltinp0_BEFOREGATE,
				    long *meltoutp0_AFTERGATE);


/* declare hook frame marking routine for melthook_HOOK_OVERRIDE_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_OVERRIDE_GATE_rout (struct
								  melt_callframe_st
								  *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_3_warmelt_hooks_ENABLE_OVERRIDE_GATE (meltclosure_ptr_t meltclosp_,
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
meltrout_4_warmelt_hooks_MAYBE_DISABLE_OVERRIDE_GATE (meltclosure_ptr_t
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
meltrout_5_warmelt_hooks_REGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
meltrout_6_warmelt_hooks_REGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
meltrout_7_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_FIRST (meltclosure_ptr_t
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
meltrout_8_warmelt_hooks_UNREGISTER_OVERRIDE_GATE_LAST (meltclosure_ptr_t
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
meltrout_9_warmelt_hooks_LAMBDA___1__ (meltclosure_ptr_t meltclosp_,
				       melt_ptr_t meltfirstargp_,
				       const melt_argdescr_cell_t
				       meltxargdescr_[],
				       union meltparam_un *meltxargtab_,
				       const melt_argdescr_cell_t
				       meltxresdescr_[],
				       union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_warmelt_hooks_LAMBDA___2__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_warmelt_hooks_LAMBDA___3__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_START_UNIT */
MELT_EXTERN void melthook_HOOK_START_UNIT (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_START_UNIT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_START_UNIT_rout (struct
							       melt_callframe_st
							       *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_warmelt_hooks_ENABLE_START_UNIT (meltclosure_ptr_t meltclosp_,
					     melt_ptr_t meltfirstargp_,
					     const melt_argdescr_cell_t
					     meltxargdescr_[],
					     union meltparam_un *meltxargtab_,
					     const melt_argdescr_cell_t
					     meltxresdescr_[],
					     union meltparam_un
					     *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
meltrout_15_warmelt_hooks_AT_START_UNIT_LAST (meltclosure_ptr_t meltclosp_,
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
meltrout_16_warmelt_hooks_LAMBDA___4__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_warmelt_hooks_LAMBDA___5__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_warmelt_hooks_LAMBDA___6__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un *meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_UNIT */
MELT_EXTERN void melthook_HOOK_FINISH_UNIT (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_FINISH_UNIT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_UNIT_rout (struct
								melt_callframe_st
								*, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT (meltclosure_ptr_t meltclosp_,
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
meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST (meltclosure_ptr_t meltclosp_,
					       melt_ptr_t meltfirstargp_,
					       const melt_argdescr_cell_t
					       meltxargdescr_[],
					       union meltparam_un
					       *meltxargtab_,
					       const melt_argdescr_cell_t
					       meltxresdescr_[],
					       union meltparam_un
					       *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_PASSES_START */
MELT_EXTERN void melthook_HOOK_ALL_PASSES_START (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_PASSES_START hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_PASSES_START_rout (struct
								     melt_callframe_st
								     *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START (meltclosure_ptr_t
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
meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST (meltclosure_ptr_t
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
meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_ALL_PASSES_END */
MELT_EXTERN void melthook_HOOK_ALL_PASSES_END (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_PASSES_END_rout (struct
								   melt_callframe_st
								   *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END (meltclosure_ptr_t meltclosp_,
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
meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST (meltclosure_ptr_t
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
meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST (meltclosure_ptr_t
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

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_START */
MELT_EXTERN void melthook_HOOK_ALL_IPA_PASSES_START (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_IPA_PASSES_START hook */
void MELT_MODULE_VISIBILITY
melthookmark_HOOK_ALL_IPA_PASSES_START_rout (struct melt_callframe_st *, int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START (meltclosure_ptr_t
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
meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST
(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[],
union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[],
union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_END */
MELT_EXTERN void melthook_HOOK_ALL_IPA_PASSES_END (melt_ptr_t melthookdata);


/* declare hook frame marking routine for melthook_HOOK_ALL_IPA_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_IPA_PASSES_END_rout (struct
								       melt_callframe_st
								       *,
								       int);







melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END (meltclosure_ptr_t
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
meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST (meltclosure_ptr_t
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
meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST (meltclosure_ptr_t
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



MELT_EXTERN void *melt_start_this_module (void *);

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_warmelt_hooks__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_warmelt_hooks__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG */
MELT_EXTERN const char meltmodule_warmelt_hooks__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_warmelt_hooks__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG */



void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_0 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_1 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_2 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__initialmeltchunk_3 (meltinitial_frame_t *, char *);
void MELT_MODULE_VISIBILITY
meltmod__warmelt_hooks__forward_or_mark_module_start_frame (struct
							    melt_callframe_st
							    *fp, int marking);



/**** warmelt-hooks+01.c implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST_melt
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

  struct meltframe_meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST_st
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
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST_st *)
	meltfirstargp_;
      /* use arguments meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("AT_START_UNIT_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:322:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:324:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*!START_UNIT_DELAYED_QUEUE */ meltfrout->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
  /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:326:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:327:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_START_UNIT__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_START_UNIT */ meltfrout->tabval[1])),
			  (melt_ptr_t) (NULL), (""), (union meltparam_un *) 0,
			  "", (union meltparam_un *) 0);
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:328:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.FIRSTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:326:/ quasiblock");


	  /*epilog */

	  /*^clear */
	     /*clear *//*_.ENABLE_START_UNIT__V4*/ meltfptr[3] = 0;
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:324:/ clear");
	   /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("AT_START_UNIT_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_14_warmelt_hooks_AT_START_UNIT_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_warmelt_hooks_AT_START_UNIT_LAST (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_15_warmelt_hooks_AT_START_UNIT_LAST_melt
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

  struct meltframe_meltrout_15_warmelt_hooks_AT_START_UNIT_LAST_st
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
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_15_warmelt_hooks_AT_START_UNIT_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_15_warmelt_hooks_AT_START_UNIT_LAST_st *)
	meltfirstargp_;
      /* use arguments meltrout_15_warmelt_hooks_AT_START_UNIT_LAST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_15_warmelt_hooks_AT_START_UNIT_LAST nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("AT_START_UNIT_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:331:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:333:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*!START_UNIT_DELAYED_QUEUE */ meltfrout->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "DELQU_LAST");
  /*_.LASTLIST__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:335:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:336:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_START_UNIT__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_START_UNIT */ meltfrout->tabval[1])),
			  (melt_ptr_t) (NULL), (""), (union meltparam_un *) 0,
			  "", (union meltparam_un *) 0);
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:337:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:335:/ quasiblock");


	  /*epilog */

	  /*^clear */
	     /*clear *//*_.ENABLE_START_UNIT__V4*/ meltfptr[3] = 0;
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:333:/ clear");
	   /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("AT_START_UNIT_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_15_warmelt_hooks_AT_START_UNIT_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_15_warmelt_hooks_AT_START_UNIT_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_warmelt_hooks_LAMBDA___4__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_16_warmelt_hooks_LAMBDA___4___melt
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

  struct meltframe_meltrout_16_warmelt_hooks_LAMBDA___4___st
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
  /*meltrout_16_warmelt_hooks_LAMBDA___4__ is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_16_warmelt_hooks_LAMBDA___4___st *)
	meltfirstargp_;
      /* use arguments meltrout_16_warmelt_hooks_LAMBDA___4__ output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_16_warmelt_hooks_LAMBDA___4__ nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:371:/ getarg");
 /*_.FIRSTPROC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:372:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) (( /*~RESCONT */ meltfclos->tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
  /*_.PREVRES__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:373:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!FINISH_UNIT_DELAYED_QUEUE */ meltfrout->
			  tabval[0]);
      /*_.NEXTRES__V4*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t) ( /*_.FIRSTPROC__V2*/ meltfptr[1]),
		    (melt_ptr_t) ( /*_.PREVRES__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:375:/ quasiblock");


    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      (( /*~RESCONT */ meltfclos->
					tabval[0]))) == MELTOBMAG_OBJECT);
    melt_putfield_object ((( /*~RESCONT */ meltfclos->tabval[0])), (0),
			  ( /*_.NEXTRES__V4*/ meltfptr[3]),
			  "REFERENCED_VALUE");
    ;
    /*^touch */
    meltgc_touch (( /*~RESCONT */ meltfclos->tabval[0]));
    ;
    /*^touchobj */

    melt_dbgtrace_written_object (( /*~RESCONT */ meltfclos->tabval[0]),
				  "put-fields");
    ;


    MELT_LOCATION ("warmelt-hooks.melt:372:/ clear");
	   /*clear *//*_.PREVRES__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.NEXTRES__V4*/ meltfptr[3] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_16_warmelt_hooks_LAMBDA___4___melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_16_warmelt_hooks_LAMBDA___4__ */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_warmelt_hooks_LAMBDA___5__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_17_warmelt_hooks_LAMBDA___5___melt
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

  struct meltframe_meltrout_17_warmelt_hooks_LAMBDA___5___st
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
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_17_warmelt_hooks_LAMBDA___5__ is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_17_warmelt_hooks_LAMBDA___5___st *)
	meltfirstargp_;
      /* use arguments meltrout_17_warmelt_hooks_LAMBDA___5__ output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_17_warmelt_hooks_LAMBDA___5__ nbval 2*/
  meltfram__.mcfr_nbvar = 2 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:380:/ getarg");
 /*_.LASTPROC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.LASTPROC__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:381:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  {
	    /*^locexp */
	    meltgc_prepend_list ((melt_ptr_t)
				 (( /*~REVLASTLIST */ meltfclos->tabval[0])),
				 (melt_ptr_t) ( /*_.LASTPROC__V2*/
					       meltfptr[1]));
	  }
	  ;
	  /*epilog */
	}
	;
      }				/*noelse */
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:380:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_17_warmelt_hooks_LAMBDA___5___melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_17_warmelt_hooks_LAMBDA___5__ */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_warmelt_hooks_LAMBDA___6__ (meltclosure_ptr_t meltclosp_,
					melt_ptr_t meltfirstargp_,
					const melt_argdescr_cell_t
					meltxargdescr_[],
					union meltparam_un * meltxargtab_,
					const melt_argdescr_cell_t
					meltxresdescr_[],
					union meltparam_un * meltxrestab_)
{
  long current_blocklevel_signals_meltrout_18_warmelt_hooks_LAMBDA___6___melt
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

  struct meltframe_meltrout_18_warmelt_hooks_LAMBDA___6___st
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
  /*meltrout_18_warmelt_hooks_LAMBDA___6__ is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_18_warmelt_hooks_LAMBDA___6___st *)
	meltfirstargp_;
      /* use arguments meltrout_18_warmelt_hooks_LAMBDA___6__ output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_18_warmelt_hooks_LAMBDA___6__ nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("LAMBDA_", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:385:/ getarg");
 /*_.LASTPROC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:386:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj = (melt_ptr_t) (( /*~RESCONT */ meltfclos->tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 0, "REFERENCED_VALUE");
  /*_.PREVRES__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:387:/ apply");
    /*apply */
    {
      union meltparam_un argtab[1];
      memset (&argtab, 0, sizeof (argtab));
      /*^apply.arg */
      argtab[0].meltbp_aptr =
	(melt_ptr_t *) & ( /*!FINISH_UNIT_DELAYED_QUEUE */ meltfrout->
			  tabval[0]);
      /*_.NEXTRES__V4*/ meltfptr[3] =
	melt_apply ((meltclosure_ptr_t) ( /*_.LASTPROC__V2*/ meltfptr[1]),
		    (melt_ptr_t) ( /*_.PREVRES__V3*/ meltfptr[2]),
		    (MELTBPARSTR_PTR ""), argtab, "",
		    (union meltparam_un *) 0);
    }
    ;

    MELT_CHECK_SIGNAL ();
    ;
    MELT_LOCATION ("warmelt-hooks.melt:389:/ quasiblock");


    /*^putslot */
    /*putslot */
    melt_assertmsg ("putslot checkobj @REFERENCED_VALUE",
		    melt_magic_discr ((melt_ptr_t)
				      (( /*~RESCONT */ meltfclos->
					tabval[0]))) == MELTOBMAG_OBJECT);
    melt_putfield_object ((( /*~RESCONT */ meltfclos->tabval[0])), (0),
			  ( /*_.NEXTRES__V4*/ meltfptr[3]),
			  "REFERENCED_VALUE");
    ;
    /*^touch */
    meltgc_touch (( /*~RESCONT */ meltfclos->tabval[0]));
    ;
    /*^touchobj */

    melt_dbgtrace_written_object (( /*~RESCONT */ meltfclos->tabval[0]),
				  "put-fields");
    ;


    MELT_LOCATION ("warmelt-hooks.melt:386:/ clear");
	   /*clear *//*_.PREVRES__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_.NEXTRES__V4*/ meltfptr[3] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("LAMBDA_", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_18_warmelt_hooks_LAMBDA___6___melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_18_warmelt_hooks_LAMBDA___6__ */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT_melt
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

  struct meltframe_meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT_st
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
#define MELTFRAM_NBVARPTR 1
    melt_ptr_t mcfr_varptr[1];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT_st *)
	meltfirstargp_;
      /* use arguments meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_FINISH_UNIT", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:413:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:414:/ locexp");
      /* enable_finish_unit ENFINISHUNIT_CHK__1 */
      if (!melthk_finish_unit_registered_flag)
	{
	  melthk_finish_unit_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_FINISH_UNIT,
			     melt_finishunithook_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_FINISH_UNIT", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_20_warmelt_hooks_ENABLE_FINISH_UNIT */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST_melt
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

  struct meltframe_meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST_st
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
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST_st *)
	meltfirstargp_;
      /* use arguments meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("AT_FINISH_UNIT_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:427:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:430:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*!FINISH_UNIT_DELAYED_QUEUE */ meltfrout->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
  /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:432:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:433:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_FINISH_UNIT__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_FINISH_UNIT */ meltfrout->tabval[1])),
			  (melt_ptr_t) (NULL), (""), (union meltparam_un *) 0,
			  "", (union meltparam_un *) 0);
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:434:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.FIRSTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:432:/ quasiblock");


	  /*epilog */

	  /*^clear */
	     /*clear *//*_.ENABLE_FINISH_UNIT__V4*/ meltfptr[3] = 0;
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:430:/ clear");
	   /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("AT_FINISH_UNIT_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_21_warmelt_hooks_AT_FINISH_UNIT_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST_melt
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

  struct meltframe_meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST_st
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
#define MELTFRAM_NBVARNUM 1
    long mcfr_varnum[1];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST_st *)
	meltfirstargp_;
      /* use arguments meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST output_curframe_declstruct_init */
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
/* declstructinit plain routine meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST nbval 4*/
  meltfram__.mcfr_nbvar = 4 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("AT_FINISH_UNIT_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:437:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {

    MELT_LOCATION ("warmelt-hooks.melt:440:/ quasiblock");


    /*^getslot */
    {
      melt_ptr_t slot = NULL, obj = NULL;
      obj =
	(melt_ptr_t) (( /*!FINISH_UNIT_DELAYED_QUEUE */ meltfrout->
		       tabval[0])) /*=obj*/ ;
      melt_object_get_field (slot, obj, 3, "DELQU_LAST");
  /*_.LASTLIST__V3*/ meltfptr[2] = slot;
    };
    ;

    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:442:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{


	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:443:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_FINISH_UNIT__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_FINISH_UNIT */ meltfrout->tabval[1])),
			  (melt_ptr_t) (NULL), (""), (union meltparam_un *) 0,
			  "", (union meltparam_un *) 0);
	  }
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:444:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:442:/ quasiblock");


	  /*epilog */

	  /*^clear */
	     /*clear *//*_.ENABLE_FINISH_UNIT__V4*/ meltfptr[3] = 0;
	}
	;
      }				/*noelse */
    ;

    MELT_LOCATION ("warmelt-hooks.melt:440:/ clear");
	   /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
    /*^clear */
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("AT_FINISH_UNIT_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_22_warmelt_hooks_AT_FINISH_UNIT_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START_melt
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

  struct meltframe_meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START_st
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
#define MELTFRAM_NBVARPTR 1
    melt_ptr_t mcfr_varptr[1];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START_st
	 *) meltfirstargp_;
      /* use arguments meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_ALL_PASSES_START", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:513:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:514:/ locexp");
      /* enable_all_passes_start ENALLPASSESSTART_CHK__1 */
      if (!melthk_all_passes_start_registered_flag)
	{
	  melthk_all_passes_start_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_ALL_PASSES_START,
			     melt_all_passes_start_hook_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_ALL_PASSES_START", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_24_warmelt_hooks_ENABLE_ALL_PASSES_START */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST (meltclosure_ptr_t
							   meltclosp_,
							   melt_ptr_t
							   meltfirstargp_,
							   const
							   melt_argdescr_cell_t
							   meltxargdescr_[],
							   union meltparam_un
							   * meltxargtab_,
							   const
							   melt_argdescr_cell_t
							   meltxresdescr_[],
							   union meltparam_un
							   * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST_melt
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

  struct meltframe_meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_PASSES_START_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:526:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:530:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:531:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_PASSES_START_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
     /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.FIRSTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:533:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_PASSES_START__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_PASSES_START */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:534:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:536:/ compute");
		/*_.FIRSTLIST__V3*/ meltfptr[2] =
		  /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:537:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_PASSES_START_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:535:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:534:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:542:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.FIRSTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:531:/ clear");
	     /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_PASSES_START__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:530:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:545:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_passes_start_hook_first"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:526:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_PASSES_START_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_25_warmelt_hooks_REGISTER_ALL_PASSES_START_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST_melt
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

  struct meltframe_meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_PASSES_START_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:549:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:553:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:554:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_PASSES_START_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "DELQU_LAST");
     /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:556:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_PASSES_START__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_PASSES_START */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:557:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:559:/ compute");
		/*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:560:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_PASSES_START_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:558:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:557:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:565:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:554:/ clear");
	     /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_PASSES_START__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:553:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:568:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_passes_start_hook_last"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:549:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_PASSES_START_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_26_warmelt_hooks_REGISTER_ALL_PASSES_START_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END (meltclosure_ptr_t meltclosp_,
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
    current_blocklevel_signals_meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END_melt
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

  struct meltframe_meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END_st
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
#define MELTFRAM_NBVARPTR 1
    melt_ptr_t mcfr_varptr[1];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct meltframe_meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END_st
	 *) meltfirstargp_;
      /* use arguments meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_ALL_PASSES_END", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:636:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:637:/ locexp");
      /* enable_all_passes_end ENALLPASSESEND_CHK__1 */
      if (!melthk_all_passes_end_registered_flag)
	{
	  melthk_all_passes_end_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_ALL_PASSES_END,
			     melt_all_passes_end_hook_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_ALL_PASSES_END", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_28_warmelt_hooks_ENABLE_ALL_PASSES_END */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST_melt
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

  struct meltframe_meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_PASSES_END_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:649:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:653:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:654:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_PASSES_END_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
     /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.FIRSTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:656:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_PASSES_END__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_PASSES_END */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:657:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:659:/ compute");
		/*_.FIRSTLIST__V3*/ meltfptr[2] =
		  /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:660:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_PASSES_END_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:658:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:657:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:665:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.FIRSTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:654:/ clear");
	     /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_PASSES_END__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:653:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:668:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_passes_end_first"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:649:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_PASSES_END_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_29_warmelt_hooks_REGISTER_ALL_PASSES_END_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST_melt
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

  struct meltframe_meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_PASSES_END_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:672:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:676:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:677:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_PASSES_END_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "DELQU_LAST");
     /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:679:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_PASSES_END__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_PASSES_END */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:680:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:682:/ compute");
		/*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:683:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_PASSES_END_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:681:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:680:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:688:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:677:/ clear");
	     /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_PASSES_END__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:676:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:691:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_passes_end_last"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:672:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_PASSES_END_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_30_warmelt_hooks_REGISTER_ALL_PASSES_END_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START_melt
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

  struct meltframe_meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START_st
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
#define MELTFRAM_NBVARPTR 1
    melt_ptr_t mcfr_varptr[1];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START_st *)
	meltfirstargp_;
      /* use arguments meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_ALL_IPA_PASSES_START", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:758:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:759:/ locexp");
      /* enable_all_ipa_passes_start ENALLPASSESSTART_CHK__2 */
      if (!melthk_all_ipa_passes_start_registered_flag)
	{
	  melthk_all_ipa_passes_start_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_ALL_IPA_PASSES_START,
			     melt_all_ipa_passes_start_hook_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_ALL_IPA_PASSES_START", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_32_warmelt_hooks_ENABLE_ALL_IPA_PASSES_START */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST_melt
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

  struct meltframe_meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_IPA_PASSES_START_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:771:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:775:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:776:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
     /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.FIRSTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:778:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_IPA_PASSES_START__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_IPA_PASSES_START */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:779:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:781:/ compute");
		/*_.FIRSTLIST__V3*/ meltfptr[2] =
		  /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:782:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:780:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:779:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:787:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.FIRSTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:776:/ clear");
	     /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_IPA_PASSES_START__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:775:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:790:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_ipa_passes_start_hook_first"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:771:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_IPA_PASSES_START_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_33_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST
  (meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_,
   const melt_argdescr_cell_t meltxargdescr_[],
   union meltparam_un * meltxargtab_,
   const melt_argdescr_cell_t meltxresdescr_[],
   union meltparam_un * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST_melt
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

  struct meltframe_meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_IPA_PASSES_START_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:794:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:798:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:799:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "DELQU_LAST");
     /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:801:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_IPA_PASSES_START__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_IPA_PASSES_START */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:802:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:804:/ compute");
		/*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:805:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_IPA_PASSES_START_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:803:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:802:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:810:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:799:/ clear");
	     /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_IPA_PASSES_START__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:798:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:813:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_ipa_passes_start_hook_last"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:794:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_IPA_PASSES_START_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_34_warmelt_hooks_REGISTER_ALL_IPA_PASSES_START_LAST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END (meltclosure_ptr_t
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
    current_blocklevel_signals_meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END_melt
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

  struct meltframe_meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END_st
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
#define MELTFRAM_NBVARPTR 1
    melt_ptr_t mcfr_varptr[1];
/*no varnum*/
#define MELTFRAM_NBVARNUM /*none*/0
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END_st *)
	meltfirstargp_;
      /* use arguments meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 1; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END nbval 1*/
  meltfram__.mcfr_nbvar = 1 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("ENABLE_ALL_IPA_PASSES_END", meltcallcount);
/*getargs*/
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
  MELT_LOCATION ("warmelt-hooks.melt:881:/ block");
/*anyblock*/
  {


    {
      MELT_LOCATION ("warmelt-hooks.melt:882:/ locexp");
      /* enable_all_ipa_passes_end ENALLPASSESEND_CHK__2 */
      if (!melthk_all_ipa_passes_end_registered_flag)
	{
	  melthk_all_ipa_passes_end_registered_flag = true;
	  register_callback (melt_plugin_name, PLUGIN_ALL_IPA_PASSES_END,
			     melt_all_ipa_passes_end_hook_callback, NULL);
	}
      ;
    }
    ;
    /*epilog */
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("ENABLE_ALL_IPA_PASSES_END", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_36_warmelt_hooks_ENABLE_ALL_IPA_PASSES_END */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST (meltclosure_ptr_t
							     meltclosp_,
							     melt_ptr_t
							     meltfirstargp_,
							     const
							     melt_argdescr_cell_t
							     meltxargdescr_[],
							     union
							     meltparam_un *
							     meltxargtab_,
							     const
							     melt_argdescr_cell_t
							     meltxresdescr_[],
							     union
							     meltparam_un *
							     meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST_melt
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

  struct meltframe_meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_IPA_PASSES_END_FIRST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:894:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:898:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:899:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 2, "DELQU_FIRST");
     /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.FIRSTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:901:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_IPA_PASSES_END__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_IPA_PASSES_END */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:902:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:904:/ compute");
		/*_.FIRSTLIST__V3*/ meltfptr[2] =
		  /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:905:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:903:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:902:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:910:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.FIRSTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:899:/ clear");
	     /*clear *//*_.FIRSTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_IPA_PASSES_END__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:898:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:913:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_ipa_passes_end_first"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:894:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_IPA_PASSES_END_FIRST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_37_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_FIRST */





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST (meltclosure_ptr_t
							    meltclosp_,
							    melt_ptr_t
							    meltfirstargp_,
							    const
							    melt_argdescr_cell_t
							    meltxargdescr_[],
							    union meltparam_un
							    * meltxargtab_,
							    const
							    melt_argdescr_cell_t
							    meltxresdescr_[],
							    union meltparam_un
							    * meltxrestab_)
{
  long
    current_blocklevel_signals_meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST_melt
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

  struct meltframe_meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST_st
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
#define MELTFRAM_NBVARPTR 7
    melt_ptr_t mcfr_varptr[7];
#define MELTFRAM_NBVARNUM 2
    long mcfr_varnum[2];
/*others*/
    long _spare_;
  }
   *meltframptr_ = 0, meltfram__;	/*declfrastruct */
#define meltframe meltfram__
  /*meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST is not initial declstructinit */
  if (MELT_UNLIKELY (meltxargdescr_ == MELTPAR_MARKGGC))
    {				/*mark for ggc */
      int meltix;
      meltframptr_ =
	(struct
	 meltframe_meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST_st
	 *) meltfirstargp_;
      /* use arguments meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST output_curframe_declstruct_init */
      (void) meltclosp_;
      (void) meltfirstargp_;
      (void) meltxargdescr_;
      (void) meltxargtab_;
      (void) meltxresdescr_;
      (void) meltxrestab_;
      /* marking closure */
      gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
      for (meltix = 0; meltix < 7; meltix++)
	if (meltframptr_->mcfr_varptr[meltix])
	  gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
      return NULL;
    } /*end markggc */ ;
  memset (&meltfram__, 0, sizeof (meltfram__));
/* declstructinit plain routine meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST nbval 7*/
  meltfram__.mcfr_nbvar = 7 /*nbval */ ;
  meltfram__.mcfr_clos = meltclosp_;
  meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe;
  melt_topframe = (struct melt_callframe_st *) &meltfram__;
  melt_trace_start ("REGISTER_ALL_IPA_PASSES_END_LAST", meltcallcount);
/*getargs*/

  /*getarg#0 */
  MELT_LOCATION ("warmelt-hooks.melt:917:/ getarg");
 /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
  ;
  goto meltlab_endgetargs;
meltlab_endgetargs:;
/*body*/
/*^block*/
/*anyblock*/
  {


    MELT_CHECK_SIGNAL ();
    ;
 /*_#IS_CLOSURE__L1*/ meltfnum[0] =
      (melt_magic_discr ((melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1])) ==
       MELTOBMAG_CLOSURE);;
    MELT_LOCATION ("warmelt-hooks.melt:921:/ cond");
    /*cond */ if ( /*_#IS_CLOSURE__L1*/ meltfnum[0])	/*then */
      {
	/*^cond.then */
	/*^block */
	/*anyblock */
	{

	  MELT_LOCATION ("warmelt-hooks.melt:922:/ quasiblock");


	  /*^cond */
	  /*cond */ if (
			 /*ifisa */
			 melt_is_instance_of ((melt_ptr_t)
					      (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
					      (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
	    )			/*then */
	    {
	      /*^cond.then */
	      /*^getslot */
	      {
		melt_ptr_t slot = NULL, obj = NULL;
		obj =
		  (melt_ptr_t) (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */
				 meltfrout->tabval[0])) /*=obj*/ ;
		melt_object_get_field (slot, obj, 3, "DELQU_LAST");
     /*_.LASTLIST__V3*/ meltfptr[2] = slot;
	      };
	      ;
	    }
	  else
	    {			/*^cond.else */

    /*_.LASTLIST__V3*/ meltfptr[2] = NULL;;
	    }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
	  MELT_LOCATION ("warmelt-hooks.melt:924:/ apply");
	  /*apply */
	  {
	    /*_.ENABLE_ALL_IPA_PASSES_END__V4*/ meltfptr[3] =
	      melt_apply ((meltclosure_ptr_t)
			  (( /*!ENABLE_ALL_IPA_PASSES_END */ meltfrout->
			    tabval[2])), (melt_ptr_t) (NULL), (""),
			  (union meltparam_un *) 0, "",
			  (union meltparam_un *) 0);
	  }
	  ;

	  MELT_CHECK_SIGNAL ();
	  ;
   /*_#NULL__L2*/ meltfnum[1] =
	    (( /*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
	  MELT_LOCATION ("warmelt-hooks.melt:925:/ cond");
	  /*cond */ if ( /*_#NULL__L2*/ meltfnum[1])	/*then */
	    {
	      /*^cond.then */
	      /*^block */
	      /*anyblock */
	      {

     /*_.MAKE_LIST__V5*/ meltfptr[4] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:927:/ compute");
		/*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] =
		  /*_.MAKE_LIST__V5*/ meltfptr[4];;

		MELT_CHECK_SIGNAL ();
		;
     /*_.MAKE_LIST__V7*/ meltfptr[6] =
		  (meltgc_new_list
		   ((meltobject_ptr_t)
		    (( /*!DISCR_LIST */ meltfrout->tabval[3]))));;
		MELT_LOCATION ("warmelt-hooks.melt:928:/ cond");
		/*cond */ if (
			       /*ifisa */
			       melt_is_instance_of ((melt_ptr_t)
						    (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])),
						    (melt_ptr_t) (( /*!CLASS_DELAYED_QUEUE */ meltfrout->tabval[1])))
		  )		/*then */
		  {
		    /*^cond.then */
		    /*^block */
		    /*anyblock */
		    {

		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_FIRST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (2), ( /*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_FIRST");
		      ;
		      /*^putslot */
		      /*putslot */
		      melt_assertmsg ("putslot checkobj @DELQU_LAST",
				      melt_magic_discr ((melt_ptr_t)
							(( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
		      melt_putfield_object ((( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0])), (3), ( /*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_LAST");
		      ;
		      /*^touch */
		      meltgc_touch (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */
				     meltfrout->tabval[0]));
		      ;
		      /*^touchobj */

		      melt_dbgtrace_written_object (( /*!ALL_IPA_PASSES_END_DELAYED_QUEUE */ meltfrout->tabval[0]), "put-fields");
		      ;
		      /*epilog */
		    }
		    ;
		  }		/*noelse */
		;
		MELT_LOCATION ("warmelt-hooks.melt:926:/ quasiblock");


		/*epilog */

		MELT_LOCATION ("warmelt-hooks.melt:925:/ clear");
	       /*clear *//*_.MAKE_LIST__V5*/ meltfptr[4] = 0;
		/*^clear */
	       /*clear *//*_.SETQ___V6*/ meltfptr[5] = 0;
		/*^clear */
	       /*clear *//*_.MAKE_LIST__V7*/ meltfptr[6] = 0;
	      }
	      ;
	    }			/*noelse */
	  ;

	  {
	    MELT_LOCATION ("warmelt-hooks.melt:933:/ locexp");
	    meltgc_append_list ((melt_ptr_t)
				( /*_.LASTLIST__V3*/ meltfptr[2]),
				(melt_ptr_t) ( /*_.FUN__V2*/ meltfptr[1]));
	  }
	  ;

	  MELT_LOCATION ("warmelt-hooks.melt:922:/ clear");
	     /*clear *//*_.LASTLIST__V3*/ meltfptr[2] = 0;
	  /*^clear */
	     /*clear *//*_.ENABLE_ALL_IPA_PASSES_END__V4*/ meltfptr[3] = 0;
	  /*^clear */
	     /*clear *//*_#NULL__L2*/ meltfnum[1] = 0;
	  /*epilog */
	}
	;
      }
    else
      {
	MELT_LOCATION ("warmelt-hooks.melt:921:/ cond.else");

	/*^block */
	/*anyblock */
	{


	  {
	    MELT_LOCATION ("warmelt-hooks.melt:936:/ locexp");
	    error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter,
		   ("Bad function (non closure) passed to register_all_ipa_passes_end_last"));
	  }
	  ;
	  /*epilog */
	}
	;
      }
    ;
    /*epilog */

    MELT_LOCATION ("warmelt-hooks.melt:917:/ clear");
	   /*clear *//*_#IS_CLOSURE__L1*/ meltfnum[0] = 0;
  }

  ;
  goto meltlabend_rout;
meltlabend_rout:
  melt_trace_end ("REGISTER_ALL_IPA_PASSES_END_LAST", meltcallcount);
  MELT_TRACE_EXIT_LOCATION ();
  melt_blocklevel_signals =
    current_blocklevel_signals_meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST_melt;
  melt_topframe = (struct melt_callframe_st *) meltfram__.mcfr_prev;
  return (melt_ptr_t) ( /*noretval */ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
}				/*end meltrout_38_warmelt_hooks_REGISTER_ALL_IPA_PASSES_END_LAST */



/**** end of warmelt-hooks+01.c ****/
