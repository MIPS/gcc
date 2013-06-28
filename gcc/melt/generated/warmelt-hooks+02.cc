/* GCC MELT GENERATED C++ FILE warmelt-hooks+02.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #2 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f2[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-hooks+02.cc declarations ****/
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
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/


/*** 3 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiHOOKS_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiHOOKS_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 3)?melt_WARMELTmiHOOKS_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 3)
        {
            melt_WARMELTmiHOOKS_module_var_ptr_tab[ix]= val;
            melt_WARMELTmiHOOKS_module_var_flags[ix/16] = true;
        }
}

/* MELT module variables indexes */
enum
{
    meltmodatix_none,
    meltmodvarix_MELT_INPCHANBUCK_symb = 1,
    meltmodvarix_MELT_ALARMBUCKET_symb = 2,
    meltmodvarix_MELT_CHILDBUCK_symb = 3,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 3 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #2 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 3 MELT called hook declarations ***/

/*** 27 extra MELT c-headers ***/


/** MELT extra c-header 1 : **/


static void
melt_override_gate_callback (void* gccdata,
                             void* userdata ATTRIBUTE_UNUSED)
{
    bool *pgatestatus = (bool*) gccdata;
    bool gatsta = false;
    long oldgatstalng = 0, newgatstalng = 0;
    gcc_assert (pgatestatus != NULL);
    gatsta = *pgatestatus;
    oldgatstalng = newgatstalng = (long)gatsta;
    melthookproc_HOOK_OVERRIDE_GATE (oldgatstalng, &newgatstalng);
    gatsta = (newgatstalng != 0L);
    *pgatestatus = gatsta;
} /* end of  melt_override_gate_callback */

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 2 : **/

/* flag for PLUGIN_OVERRIDE_GATE */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_override_gate_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 3 : **/

/* flag for PLUGIN_START_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_start_unit_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 4 : **/


void MELT_MODULE_VISIBILITY
melt_startunithook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                             void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 5 : **/

/* flag for PLUGIN_FINISH_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_finish_unit_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 6 : **/


void MELT_MODULE_VISIBILITY
melt_finishunithook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                              void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 7 : **/

/* flag for PLUGIN_ALL_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_passes_start_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 8 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_start_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                     void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 9 : **/

/* flag for PLUGIN_ALL_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_passes_end_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 10 : **/


void MELT_MODULE_VISIBILITY
melt_all_passes_end_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                   void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 11 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_ipa_passes_start_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 12 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_start_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
        void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 13 : **/

/* flag for PLUGIN_ALL_IPA_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_ipa_passes_end_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 14 : **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_end_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                       void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 15 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_early_gimple_passes_start_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 16 : **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_start_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
        void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 17 : **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_early_gimple_passes_end_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 18 : **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_end_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
        void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 19 : **/

/* flag for PLUGIN_PRE_GENERICIZE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_pre_genericize_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 20 : **/


void MELT_MODULE_VISIBILITY
melt_pre_genericize_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                   void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 21 : **/

/* flag for PLUGIN_FINISH_TYPE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_finish_type_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 22 : **/


void MELT_MODULE_VISIBILITY
melt_finish_type_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 23 : **/

/* flag for PLUGIN_FINISH_DECL declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_finish_decl_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 24 : **/


void MELT_MODULE_VISIBILITY
melt_finish_decl_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 25 : **/

/* flag for PLUGIN_PASS_EXECUTION declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_pass_execution_registered_flag;

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 26 : **/


void MELT_MODULE_VISIBILITY
melt_pass_execution_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                   void* user_data ATTRIBUTE_UNUSED);

/*** end of 27 extra MELT c-headers ***/


/** MELT extra c-header 27 : **/

/* header from warmelt-hooks.melt for poll etc... */
#include <poll.h>

#define MELT_BUFSIZE 8192
#define MELT_POLL_DELAY_MILLISEC 500

/*** end of 27 extra MELT c-headers ***/

/** declaration of hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
MELT_EXTERN
void melthook_HOOK_LOW_DEBUG_VALUE_AT(melt_ptr_t melthookdatap,
                                      melt_ptr_t meltinp0_VAL,
                                      const char* meltinp1_FILENAME,
                                      long meltinp2_LINENO,
                                      const char* meltinp3_MSG,
                                      long meltinp4_COUNT)
;


/* declare hook frame marking routine for melthook_HOOK_LOW_DEBUG_VALUE_AT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_LOW_DEBUG_VALUE_AT_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_LOW_STDERR_VALUE_AT */
MELT_EXTERN
void melthook_HOOK_LOW_STDERR_VALUE_AT(melt_ptr_t melthookdatap,
                                       melt_ptr_t meltinp0_VAL,
                                       const char* meltinp1_FILENAME,
                                       long meltinp2_LINENO,
                                       const char* meltinp3_MSG,
                                       long meltinp4_COUNT)
;


/* declare hook frame marking routine for melthook_HOOK_LOW_STDERR_VALUE_AT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_LOW_STDERR_VALUE_AT_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_OVERRIDE_GATE */
MELT_EXTERN
void melthook_HOOK_OVERRIDE_GATE(melt_ptr_t melthookdatap,
                                 long meltinp0_BEFOREGATE,
                                 long* meltoutp0_AFTERGATE)
;


/* declare hook frame marking routine for melthook_HOOK_OVERRIDE_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_OVERRIDE_GATE_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiHOOKS_ENABLE_OVERRIDE_GATE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiHOOKS_MAYBE_DISABLE_OVERRIDE_GATE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiHOOKS_REGISTER_OVERRIDE_GATE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiHOOKS_REGISTER_OVERRIDE_GATE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiHOOKS_UNREGISTER_OVERRIDE_GATE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiHOOKS_UNREGISTER_OVERRIDE_GATE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiHOOKS_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiHOOKS_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiHOOKS_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_START_UNIT */
MELT_EXTERN
void melthook_HOOK_START_UNIT(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_START_UNIT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_START_UNIT_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiHOOKS_ENABLE_START_UNIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiHOOKS_AT_START_UNIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiHOOKS_AT_START_UNIT_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiHOOKS_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiHOOKS_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiHOOKS_LAMBDA_cl6(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_UNIT */
MELT_EXTERN
void melthook_HOOK_FINISH_UNIT(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_FINISH_UNIT hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_UNIT_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiHOOKS_ENABLE_FINISH_UNIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiHOOKS_AT_FINISH_UNIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiHOOKS_AT_FINISH_UNIT_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_PASSES_START */
MELT_EXTERN
void melthook_HOOK_ALL_PASSES_START(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_ALL_PASSES_START hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_PASSES_START_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiHOOKS_ENABLE_ALL_PASSES_START(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiHOOKS_REGISTER_ALL_PASSES_START_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiHOOKS_REGISTER_ALL_PASSES_START_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_PASSES_END */
MELT_EXTERN
void melthook_HOOK_ALL_PASSES_END(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_ALL_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_PASSES_END_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiHOOKS_ENABLE_ALL_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiHOOKS_REGISTER_ALL_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiHOOKS_REGISTER_ALL_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_START */
MELT_EXTERN
void melthook_HOOK_ALL_IPA_PASSES_START(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_ALL_IPA_PASSES_START hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_IPA_PASSES_START_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiHOOKS_ENABLE_ALL_IPA_PASSES_START(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_START_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_START_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_END */
MELT_EXTERN
void melthook_HOOK_ALL_IPA_PASSES_END(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_ALL_IPA_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_ALL_IPA_PASSES_END_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiHOOKS_ENABLE_ALL_IPA_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_START */
MELT_EXTERN
void melthook_HOOK_EARLY_GIMPLE_PASSES_START(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_START hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_EARLY_GIMPLE_PASSES_START_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_START(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_START_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_END */
MELT_EXTERN
void melthook_HOOK_EARLY_GIMPLE_PASSES_END(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_EARLY_GIMPLE_PASSES_END hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_EARLY_GIMPLE_PASSES_END_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_PRE_GENERICIZE */
MELT_EXTERN
void melthook_HOOK_PRE_GENERICIZE(melt_ptr_t melthookdatap,
                                  tree meltinp0_TFNDECL)
;


/* declare hook frame marking routine for melthook_HOOK_PRE_GENERICIZE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_PRE_GENERICIZE_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_TYPE */
MELT_EXTERN
void melthook_HOOK_FINISH_TYPE(melt_ptr_t melthookdatap,
                               tree meltinp0_TFNDECL)
;


/* declare hook frame marking routine for melthook_HOOK_FINISH_TYPE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_TYPE_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_FINISH_DECL */
MELT_EXTERN
void melthook_HOOK_FINISH_DECL(melt_ptr_t melthookdatap,
                               tree meltinp0_TFNDECL)
;


/* declare hook frame marking routine for melthook_HOOK_FINISH_DECL hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_FINISH_DECL_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_PASS_EXECUTION */
MELT_EXTERN
void melthook_HOOK_PASS_EXECUTION(melt_ptr_t melthookdatap,
                                  const char* meltinp0_PASSNAME,
                                  long meltinp1_PASSNUM)
;


/* declare hook frame marking routine for melthook_HOOK_PASS_EXECUTION hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_PASS_EXECUTION_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_GIMPLE_GATE */
MELT_EXTERN
long melthook_HOOK_GIMPLE_GATE(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_GIMPLE_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_GIMPLE_GATE_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_GIMPLE_EXECUTE */
MELT_EXTERN
long melthook_HOOK_GIMPLE_EXECUTE(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_GIMPLE_EXECUTE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_GIMPLE_EXECUTE_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_RTL_GATE */
MELT_EXTERN
long melthook_HOOK_RTL_GATE(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_RTL_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_RTL_GATE_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_RTL_EXECUTE */
MELT_EXTERN
long melthook_HOOK_RTL_EXECUTE(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_RTL_EXECUTE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_RTL_EXECUTE_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_SIMPLE_IPA_GATE */
MELT_EXTERN
long melthook_HOOK_SIMPLE_IPA_GATE(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_SIMPLE_IPA_GATE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_SIMPLE_IPA_GATE_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_SIMPLE_IPA_EXECUTE */
MELT_EXTERN
long melthook_HOOK_SIMPLE_IPA_EXECUTE(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_SIMPLE_IPA_EXECUTE hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_SIMPLE_IPA_EXECUTE_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_MELT_ATTRIBUTE_DEFINER */
MELT_EXTERN
void melthook_HOOK_MELT_ATTRIBUTE_DEFINER(melt_ptr_t melthookdatap,
        tree meltinp0_DECL,
        tree meltinp1_NAME,
        melt_ptr_t meltinp2_ATTRSEQ,
        const char* meltinp3_FILELOC,
        long meltinp4_LINENO)
;


/* declare hook frame marking routine for melthook_HOOK_MELT_ATTRIBUTE_DEFINER hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_MELT_ATTRIBUTE_DEFINER_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_EXIT_FINALIZER */
MELT_EXTERN
void melthook_HOOK_EXIT_FINALIZER(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_EXIT_FINALIZER hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_EXIT_FINALIZER_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_77_WARMELTmiHOOKS_AT_EXIT_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_78_WARMELTmiHOOKS_MELT_INVOKE_INPUT_CALLBACKS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_POLL_INPUTS */
MELT_EXTERN
void melthook_HOOK_POLL_INPUTS(melt_ptr_t melthookdatap,
                               long meltinp0_DELAYMS)
;


/* declare hook frame marking routine for melthook_HOOK_POLL_INPUTS hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_POLL_INPUTS_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_80_WARMELTmiHOOKS_IS_POLLING_INPUTS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_81_WARMELTmiHOOKS_EVERY_POLLING_INPUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_82_WARMELTmiHOOKS_GET_POLLING_INPUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_83_WARMELTmiHOOKS_REGISTER_INPUT_CHANNEL_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_HANDLE_SIGIO */
MELT_EXTERN
void melthook_HOOK_HANDLE_SIGIO(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_HANDLE_SIGIO hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_HANDLE_SIGIO_rout (struct melt_callframe_st*, int);




/** declaration of hook melthook_HOOK_HANDLE_SIGALRM */
MELT_EXTERN
void melthook_HOOK_HANDLE_SIGALRM(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_HANDLE_SIGALRM hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_HANDLE_SIGALRM_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_86_WARMELTmiHOOKS_REGISTER_ALARM_TIMER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_87_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** declaration of hook melthook_HOOK_HANDLE_SIGCHLD */
MELT_EXTERN
void melthook_HOOK_HANDLE_SIGCHLD(melt_ptr_t melthookdatap)
;


/* declare hook frame marking routine for melthook_HOOK_HANDLE_SIGCHLD hook */
void MELT_MODULE_VISIBILITY melthookmark_HOOK_HANDLE_SIGCHLD_rout (struct melt_callframe_st*, int);







melt_ptr_t MELT_MODULE_VISIBILITY meltrout_89_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_90_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/

struct melt_initial_frame_st; /*forward declaration of initial frame*/

/*declare opaque initial frame: */
typedef struct melt_initial_frame_st meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiHOOKS__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiHOOKS__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiHOOKS__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiHOOKS__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_0 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_1 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_2 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_3 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_4 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_5 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_6 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_7 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_8 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_9 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__initialmeltchunk_10 (meltinitial_frame_t*, char*);


void MELT_MODULE_VISIBILITY meltmod__WARMELTmiHOOKS__forward_or_mark_module_start_frame (struct melt_callframe_st* fp, int marking);



/**** warmelt-hooks+02.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END
        : public Melt_CallFrameWithValues<1>
    {
    public: /*inframedeclclassy*/
// no classy longs
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<1> (sizeof(MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END), clos) {};
    } // end  class MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END_st*) meltfirstargp_;
            /* use arguments meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<1; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END nbval 1*/
    meltfram__.mcfr_nbvar = 1/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("ENABLE_EARLY_GIMPLE_PASSES_END", meltcallcount);
    /*getargs*/
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    MELT_LOCATION("warmelt-hooks.melt:1181:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1182:/ locexp");
            /* enable_early_gimple_passes_end ENALLPASSESEND_CHK__3 */
            if (!melthk_early_gimple_passes_end_registered_flag)
                {
                    melthk_early_gimple_passes_end_registered_flag = true;
                    register_callback (melt_plugin_name, PLUGIN_EARLY_GIMPLE_PASSES_END,
                                       melt_early_gimple_passes_end_hook_callback,
                                       NULL);
                }
            ;
        }
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ENABLE_EARLY_GIMPLE_PASSES_END", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST
        : public Melt_CallFrameWithValues<7>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<7> (sizeof(MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<7; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST nbval 7*/
    meltfram__.mcfr_nbvar = 7/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_EARLY_GIMPLE_PASSES_END_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1194:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1198:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-hooks.melt:1199:/ quasiblock");


                    /*^cond*/
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                                melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                                /*_.FIRSTLIST__V3*/
                                meltfptr[2] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.FIRSTLIST__V3*/ meltfptr[2] =  NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1201:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_EARLY_GIMPLE_PASSES_END*/ meltfrout->tabval[2])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#NULL__L2*/
                    meltfnum[1] =
                        ((/*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
                    MELT_LOCATION("warmelt-hooks.melt:1202:/ cond");
                    /*cond*/
                    if (/*_#NULL__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MAKE_LIST__V5*/ meltfptr[4] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1204:/ compute");
                                /*_.FIRSTLIST__V3*/
                                meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_.MAKE_LIST__V7*/
                                meltfptr[6] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1205:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                                  (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                            melt_putfield_object(((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
                                            ;
                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                            melt_putfield_object(((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                            ;
                                            /*^touch*/
                                            meltgc_touch((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                            ;
                                            /*^touchobj*/

                                            melt_dbgtrace_written_object ((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    } /*noelse*/
                                ;
                                MELT_LOCATION("warmelt-hooks.melt:1203:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-hooks.melt:1202:/ clear");
                                /*clear*/ /*_.MAKE_LIST__V5*/
                                meltfptr[4] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V6*/
                                meltfptr[5] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MAKE_LIST__V7*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:1210:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_LOCATION("warmelt-hooks.melt:1199:/ clear");
                    /*clear*/ /*_.FIRSTLIST__V3*/
                    meltfptr[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NULL__L2*/
                    meltfnum[1] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1198:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1213:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad function (non closure) passed to register_early_gimple_passes_end_first"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-hooks.melt:1194:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_EARLY_GIMPLE_PASSES_END_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST
        : public Melt_CallFrameWithValues<7>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<7> (sizeof(MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST), clos) {};
    } // end  class MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<7; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST nbval 7*/
    meltfram__.mcfr_nbvar = 7/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_EARLY_GIMPLE_PASSES_END_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1217:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1221:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-hooks.melt:1222:/ quasiblock");


                    /*^cond*/
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                                melt_object_get_field(slot,obj, 3, "DELQU_LAST");
                                /*_.LASTLIST__V3*/
                                meltfptr[2] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.LASTLIST__V3*/ meltfptr[2] =  NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1224:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_EARLY_GIMPLE_PASSES_END*/ meltfrout->tabval[2])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#NULL__L2*/
                    meltfnum[1] =
                        ((/*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
                    MELT_LOCATION("warmelt-hooks.melt:1225:/ cond");
                    /*cond*/
                    if (/*_#NULL__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MAKE_LIST__V5*/ meltfptr[4] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1227:/ compute");
                                /*_.LASTLIST__V3*/
                                meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_.MAKE_LIST__V7*/
                                meltfptr[6] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1228:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                                  (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                            melt_putfield_object(((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_FIRST");
                                            ;
                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                            melt_putfield_object(((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_LAST");
                                            ;
                                            /*^touch*/
                                            meltgc_touch((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                            ;
                                            /*^touchobj*/

                                            melt_dbgtrace_written_object ((/*!EARLY_GIMPLE_PASSES_END_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    } /*noelse*/
                                ;
                                MELT_LOCATION("warmelt-hooks.melt:1226:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-hooks.melt:1225:/ clear");
                                /*clear*/ /*_.MAKE_LIST__V5*/
                                meltfptr[4] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V6*/
                                meltfptr[5] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MAKE_LIST__V7*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:1233:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_LOCATION("warmelt-hooks.melt:1222:/ clear");
                    /*clear*/ /*_.LASTLIST__V3*/
                    meltfptr[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NULL__L2*/
                    meltfnum[1] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1221:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1236:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad function (non closure) passed to register_early_gimple_passes_end_last"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-hooks.melt:1217:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_EARLY_GIMPLE_PASSES_END_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE
        : public Melt_CallFrameWithValues<1>
    {
    public: /*inframedeclclassy*/
// no classy longs
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<1> (sizeof(MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE), clos) {};
    } // end  class MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE_st*) meltfirstargp_;
            /* use arguments meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<1; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE nbval 1*/
    meltfram__.mcfr_nbvar = 1/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("ENABLE_PRE_GENERICIZE", meltcallcount);
    /*getargs*/
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    MELT_LOCATION("warmelt-hooks.melt:1277:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1278:/ locexp");
            /* enable_pre_genericize ENABLEPREGEN_CHK__1 */
            if (!melthk_pre_genericize_registered_flag)
                {
                    melthk_pre_genericize_registered_flag = true;
                    /*
                      register_callback (melt_plugin_name, PLUGIN_PRE_GENERICIZE,
                                         melt_pre_genericize_hook_callback, NULL);
                    */
                }
            ;
        }
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ENABLE_PRE_GENERICIZE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST
        : public Melt_CallFrameWithValues<10>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<10> (sizeof(MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 10
        melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<10; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST nbval 10*/
    meltfram__.mcfr_nbvar = 10/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_PRE_GENERICIZE_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1322:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1326:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.FIRSTLIST__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V4*/ meltfptr[3] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.FIRSTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1328:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1330:/ compute");
                    /*_.FIRSTLIST__V4*/
                    meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/
                    meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1331:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.FIRSTLIST__V4*/ meltfptr[3]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1329:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1328:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/
                    meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1336:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1338:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1339:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_PRE_GENERICIZE*/ meltfrout->tabval[3])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1337:/ quasiblock");


                    /*_.PROGN___V10*/
                    meltfptr[6] = /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/
                    meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1336:/ clear");
                    /*clear*/ /*_.ENABLE_PRE_GENERICIZE__V9*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1340:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pre_genericize_hook"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/
        meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1326:/ clear");
        /*clear*/ /*_.FIRSTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/
        meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1322:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1322:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PRE_GENERICIZE_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST
        : public Melt_CallFrameWithValues<10>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<10> (sizeof(MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST), clos) {};
    } // end  class MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 10
        melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<10; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST nbval 10*/
    meltfram__.mcfr_nbvar = 10/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_PRE_GENERICIZE_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1345:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1349:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.LASTLIST__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V4*/ meltfptr[3] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.LASTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1351:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1353:/ compute");
                    /*_.LASTLIST__V4*/
                    meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/
                    meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1354:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.LASTLIST__V4*/ meltfptr[3]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!PREGENERICIZE_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1352:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1351:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/
                    meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1359:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1361:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1362:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_PRE_GENERICIZE*/ meltfrout->tabval[3])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1360:/ quasiblock");


                    /*_.PROGN___V10*/
                    meltfptr[6] = /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/
                    meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1359:/ clear");
                    /*clear*/ /*_.ENABLE_PRE_GENERICIZE__V9*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1363:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pre_genericize_hook"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/
        meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1349:/ clear");
        /*clear*/ /*_.LASTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/
        meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1345:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1345:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PRE_GENERICIZE_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE
        : public Melt_CallFrameWithValues<1>
    {
    public: /*inframedeclclassy*/
// no classy longs
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<1> (sizeof(MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE), clos) {};
    } // end  class MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE_st*) meltfirstargp_;
            /* use arguments meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<1; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE nbval 1*/
    meltfram__.mcfr_nbvar = 1/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("ENABLE_FINISH_TYPE", meltcallcount);
    /*getargs*/
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    MELT_LOCATION("warmelt-hooks.melt:1408:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1409:/ locexp");
            /* enable_finish_type ENABLEPREGEN_CHK__2 */
            if (!melthk_finish_type_registered_flag)
                {
                    melthk_finish_type_registered_flag = true;
                    register_callback (melt_plugin_name, PLUGIN_FINISH_TYPE,
                                       melt_finish_type_hook_callback, NULL);
                }
            ;
        }
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ENABLE_FINISH_TYPE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST
        : public Melt_CallFrameWithValues<10>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<10> (sizeof(MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 10
        melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<10; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST nbval 10*/
    meltfram__.mcfr_nbvar = 10/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_FINISH_TYPE_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1451:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1455:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.FIRSTLIST__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V4*/ meltfptr[3] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.FIRSTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1457:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1459:/ compute");
                    /*_.FIRSTLIST__V4*/
                    meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/
                    meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1460:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.FIRSTLIST__V4*/ meltfptr[3]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1458:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1457:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/
                    meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1465:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1467:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1468:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_TYPE*/ meltfrout->tabval[3])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1466:/ quasiblock");


                    /*_.PROGN___V10*/
                    meltfptr[6] = /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/
                    meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1465:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_TYPE__V9*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1469:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_type_first"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/
        meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1455:/ clear");
        /*clear*/ /*_.FIRSTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/
        meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1451:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1451:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_TYPE_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST
        : public Melt_CallFrameWithValues<10>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<10> (sizeof(MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST), clos) {};
    } // end  class MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 10
        melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<10; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST nbval 10*/
    meltfram__.mcfr_nbvar = 10/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_FINISH_TYPE_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1474:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1478:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.LASTLIST__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V4*/ meltfptr[3] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.LASTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1480:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1482:/ compute");
                    /*_.LASTLIST__V4*/
                    meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/
                    meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1483:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.LASTLIST__V4*/ meltfptr[3]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!FINISHTYPE_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1481:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1480:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/
                    meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1488:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1490:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1491:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_TYPE*/ meltfrout->tabval[3])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1489:/ quasiblock");


                    /*_.PROGN___V10*/
                    meltfptr[6] = /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/
                    meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1488:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_TYPE__V9*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1492:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_type_last"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/
        meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1478:/ clear");
        /*clear*/ /*_.LASTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/
        meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1474:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1474:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_TYPE_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS
        : public Melt_CallFrameWithValues<15>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<15> (sizeof(MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS), clos) {};
    } // end  class MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 15
        melt_ptr_t mcfr_varptr[15];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS_st*) meltfirstargp_;
            /* use arguments meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<15; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS nbval 15*/
    meltfram__.mcfr_nbvar = 15/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("RUN_ENDMELTPASS_FUNCTIONS", meltcallcount);
    /*getargs*/
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    MELT_LOCATION("warmelt-hooks.melt:1508:/ block");
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1509:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.FIRSTLIST__V2*/
                    meltfptr[1] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V2*/ meltfptr[1] =  NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-hooks.melt:1510:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "DELQU_LAST");
                    /*_.LASTLIST__V3*/
                    meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V3*/ meltfptr[2] =  NULL ;;
            }
        ;
        /*^compute*/
        /*_.REVLASTLIST__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:1512:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_CONTAINER*/ meltfrout->tabval[3])), (1), "CLASS_REFERENCE");
            /*_.INST__V6*/
            meltfptr[5] =
                newobj;
        };
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V6*/ meltfptr[5], "newly made instance");
        ;
        /*_.RES__V5*/
        meltfptr[4] = /*_.INST__V6*/ meltfptr[5];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:1514:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), ((/*nil*/NULL)), "DELQU_FIRST");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), ((/*nil*/NULL)), "DELQU_LAST");
                    ;
                    /*^touch*/
                    meltgc_touch((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object ((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*citerblock FOREACH_IN_LIST*/
        {
            /* start foreach_in_list meltcit1__EACHLIST */
            for (/*_.FIRSTPAIR__V7*/ meltfptr[6] = melt_list_first( (melt_ptr_t)/*_.FIRSTLIST__V2*/ meltfptr[1]);
                                     melt_magic_discr((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]) == MELTOBMAG_PAIR;
                                     /*_.FIRSTPAIR__V7*/ meltfptr[6] = melt_pair_tail((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]))
                {
                    /*_.FIRSTPROC__V8*/ meltfptr[7] = melt_pair_head((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L1*/
                    meltfnum[0] =
                        (melt_magic_discr((melt_ptr_t)(/*_.FIRSTPROC__V8*/ meltfptr[7])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-hooks.melt:1521:/ cond");
                    /*cond*/
                    if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-hooks.melt:1522:/ apply");
                                /*apply*/
                                {
                                    /*_.FIRSTPROC__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.FIRSTPROC__V8*/ meltfptr[7]), (melt_ptr_t)(/*_.RES__V5*/ meltfptr[4]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V9*/
                                meltfptr[8] = /*_.FIRSTPROC__V10*/ meltfptr[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-hooks.melt:1521:/ clear");
                                /*clear*/ /*_.FIRSTPROC__V10*/
                                meltfptr[9] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V9*/ meltfptr[8] =  NULL ;;
                        }
                    ;
                } /* end foreach_in_list meltcit1__EACHLIST */
            /*_.FIRSTPAIR__V7*/ meltfptr[6] = NULL;
            /*_.FIRSTPROC__V8*/
            meltfptr[7] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-hooks.melt:1518:/ clear");
            /*clear*/ /*_.FIRSTPAIR__V7*/
            meltfptr[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_.FIRSTPROC__V8*/
            meltfptr[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_CLOSURE__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V9*/
            meltfptr[8] = 0 ;
        } /*endciterblock FOREACH_IN_LIST*/
        ;
        /*citerblock FOREACH_IN_LIST*/
        {
            /* start foreach_in_list meltcit2__EACHLIST */
            for (/*_.LASTPAIR__V11*/ meltfptr[9] = melt_list_first( (melt_ptr_t)/*_.LASTLIST__V3*/ meltfptr[2]);
                                     melt_magic_discr((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[9]) == MELTOBMAG_PAIR;
                                     /*_.LASTPAIR__V11*/ meltfptr[9] = melt_pair_tail((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[9]))
                {
                    /*_.LASTPROC__V12*/ meltfptr[11] = melt_pair_head((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[9]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LASTPROC__V12*/ meltfptr[11])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-hooks.melt:1528:/ cond");
                    /*cond*/
                    if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    /*^locexp*/
                                    meltgc_prepend_list((melt_ptr_t)(/*_.REVLASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.LASTPROC__V12*/ meltfptr[11]));
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                } /* end foreach_in_list meltcit2__EACHLIST */
            /*_.LASTPAIR__V11*/ meltfptr[9] = NULL;
            /*_.LASTPROC__V12*/
            meltfptr[11] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-hooks.melt:1525:/ clear");
            /*clear*/ /*_.LASTPAIR__V11*/
            meltfptr[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LASTPROC__V12*/
            meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_CLOSURE__L2*/
            meltfnum[1] = 0 ;
        } /*endciterblock FOREACH_IN_LIST*/
        ;
        /*citerblock FOREACH_IN_LIST*/
        {
            /* start foreach_in_list meltcit3__EACHLIST */
            for (/*_.REVLASTPAIR__V13*/ meltfptr[12] = melt_list_first( (melt_ptr_t)/*_.REVLASTLIST__V4*/ meltfptr[3]);
                                        melt_magic_discr((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]) == MELTOBMAG_PAIR;
                                        /*_.REVLASTPAIR__V13*/ meltfptr[12] = melt_pair_tail((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]))
                {
                    /*_.REVLASTPROC__V14*/ meltfptr[13] = melt_pair_head((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]);



                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1533:/ apply");
                    /*apply*/
                    {
                        /*_.REVLASTPROC__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)(/*_.REVLASTPROC__V14*/ meltfptr[13]), (melt_ptr_t)(/*_.RES__V5*/ meltfptr[4]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                } /* end foreach_in_list meltcit3__EACHLIST */
            /*_.REVLASTPAIR__V13*/ meltfptr[12] = NULL;
            /*_.REVLASTPROC__V14*/
            meltfptr[13] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-hooks.melt:1530:/ clear");
            /*clear*/ /*_.REVLASTPAIR__V13*/
            meltfptr[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REVLASTPROC__V14*/
            meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REVLASTPROC__V15*/
            meltfptr[14] = 0 ;
        } /*endciterblock FOREACH_IN_LIST*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:1509:/ clear");
        /*clear*/ /*_.FIRSTLIST__V2*/
        meltfptr[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LASTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REVLASTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V5*/
        meltfptr[4] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("RUN_ENDMELTPASS_FUNCTIONS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST
        : public Melt_CallFrameWithValues<5>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<5> (sizeof(MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<5; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST nbval 5*/
    meltfram__.mcfr_nbvar = 5/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("AT_END_OF_THIS_MELT_PASS_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1538:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1540:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
            /*_.FIRSTLIST__V3*/
            meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1542:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[1]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1543:/ compute");
                    /*_.FIRSTLIST__V3*/
                    meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1544:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[2])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1542:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/
                    meltfptr[4] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1545:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1546:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:1540:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_END_OF_THIS_MELT_PASS_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST
        : public Melt_CallFrameWithValues<5>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<5> (sizeof(MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST), clos) {};
    } // end  class MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<5; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST nbval 5*/
    meltfram__.mcfr_nbvar = 5/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("AT_END_OF_THIS_MELT_PASS_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1549:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1552:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "DELQU_LAST");
            /*_.LASTLIST__V3*/
            meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1554:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[1]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1555:/ compute");
                    /*_.LASTLIST__V3*/
                    meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1556:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[2])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1554:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/
                    meltfptr[4] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1557:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1558:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:1552:/ clear");
        /*clear*/ /*_.LASTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_END_OF_THIS_MELT_PASS_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL
        : public Melt_CallFrameWithValues<1>
    {
    public: /*inframedeclclassy*/
// no classy longs
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<1> (sizeof(MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL), clos) {};
    } // end  class MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL_st*) meltfirstargp_;
            /* use arguments meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<1; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL nbval 1*/
    meltfram__.mcfr_nbvar = 1/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("ENABLE_FINISH_DECL", meltcallcount);
    /*getargs*/
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    MELT_LOCATION("warmelt-hooks.melt:1600:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1601:/ locexp");
            /* enable_finish_decl ENABLEPREGEN_CHK__3 */
            if (!melthk_finish_decl_registered_flag)
                {
                    melthk_finish_decl_registered_flag = true;
#if MELT_GCC_VERSION < 4007
                    melt_fatal_error ("PLUGIN_FINISH_DECL does not exist in GCC 4.6");
#else
                    register_callback (melt_plugin_name, PLUGIN_FINISH_DECL,
                                       melt_finish_decl_hook_callback, NULL);
#endif
                }
            ;
        }
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ENABLE_FINISH_DECL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST
        : public Melt_CallFrameWithValues<10>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<10> (sizeof(MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 10
        melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<10; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST nbval 10*/
    meltfram__.mcfr_nbvar = 10/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_FINISH_DECL_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1647:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1651:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.FIRSTLIST__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V4*/ meltfptr[3] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.FIRSTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1653:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1655:/ compute");
                    /*_.FIRSTLIST__V4*/
                    meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/
                    meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1656:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.FIRSTLIST__V4*/ meltfptr[3]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1654:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1653:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/
                    meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1661:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1663:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1664:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_DECL*/ meltfrout->tabval[3])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1662:/ quasiblock");


                    /*_.PROGN___V10*/
                    meltfptr[6] = /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/
                    meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1661:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_DECL__V9*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1665:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_decl_first"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/
        meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1651:/ clear");
        /*clear*/ /*_.FIRSTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/
        meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1647:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1647:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_DECL_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST
        : public Melt_CallFrameWithValues<10>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<10> (sizeof(MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST), clos) {};
    } // end  class MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
        struct meltclosure_st *mcfr_clos;
        struct excepth_melt_st *mcfr_exh;
        struct melt_callframe_st *mcfr_prev;
#undef MELTFRAM_NBVARPTR
#undef MELTFRAM_NBVARNUM
#define MELTFRAM_NBVARPTR 10
        melt_ptr_t mcfr_varptr[10];
#define MELTFRAM_NBVARNUM 2
        long mcfr_varnum[2];
        /*others*/
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<10; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST nbval 10*/
    meltfram__.mcfr_nbvar = 10/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_FINISH_DECL_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1670:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1674:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.LASTLIST__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V4*/ meltfptr[3] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.LASTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1676:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1678:/ compute");
                    /*_.LASTLIST__V4*/
                    meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/
                    meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1679:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.LASTLIST__V4*/ meltfptr[3]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V7*/ meltfptr[6]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!FINISHDECL_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1677:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1676:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/
                    meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1684:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1686:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1687:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_DECL*/ meltfrout->tabval[3])), (melt_ptr_t)( NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1685:/ quasiblock");


                    /*_.PROGN___V10*/
                    meltfptr[6] = /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/
                    meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1684:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_DECL__V9*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1688:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_decl_last"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/
                    meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/
        meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1674:/ clear");
        /*clear*/ /*_.LASTLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/
        meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1670:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1670:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_DECL_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST
        : public Melt_CallFrameWithValues<6>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<6> (sizeof(MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<6; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST nbval 6*/
    meltfram__.mcfr_nbvar = 6/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_PASS_EXECUTION_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1782:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1786:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.FIRSTLIST__V3*/
                    meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V3*/ meltfptr[2] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1788:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1790:/ compute");
                    /*_.FIRSTLIST__V3*/
                    meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V6*/
                    meltfptr[5] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1791:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.FIRSTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V6*/ meltfptr[5]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1789:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1788:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V6*/
                    meltfptr[5] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1796:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1797:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1796:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1798:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pass_execution_hook"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        MELT_LOCATION("warmelt-hooks.melt:1786:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PASS_EXECUTION_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST
        : public Melt_CallFrameWithValues<6>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<6> (sizeof(MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST), clos) {};
    } // end  class MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<6; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST nbval 6*/
    meltfram__.mcfr_nbvar = 6/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("REGISTER_PASS_EXECUTION_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1803:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1807:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
                    /*_.LASTLIST__V3*/
                    meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V3*/ meltfptr[2] =  NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/
        meltfnum[0] =
            ((/*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1809:/ cond");
        /*cond*/
        if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1811:/ compute");
                    /*_.LASTLIST__V3*/
                    meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V6*/
                    meltfptr[5] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1812:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])),
                                                      (melt_ptr_t)((/*!CLASS_DELAYED_QUEUE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_FIRST", melt_magic_discr((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])), (2), (/*_.LASTLIST__V3*/ meltfptr[2]), "DELQU_FIRST");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @DELQU_LAST", melt_magic_discr((melt_ptr_t)((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0])), (3), (/*_.MAKE_LIST__V6*/ meltfptr[5]), "DELQU_LAST");
                                ;
                                /*^touch*/
                                meltgc_touch((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*!PASS_EXECUTION_DELAYED_QUEUE*/ meltfrout->tabval[0]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1810:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1809:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V6*/
                    meltfptr[5] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1817:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1818:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1817:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1819:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pass_execution_last_hook"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        MELT_LOCATION("warmelt-hooks.melt:1807:/ clear");
        /*clear*/ /*_.LASTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/
        meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PASS_EXECUTION_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST
        : public Melt_CallFrameWithValues<3>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[1];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<3> (sizeof(MeltFrame_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<3; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST nbval 3*/
    meltfram__.mcfr_nbvar = 3/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("AT_MELT_ATTRIBUTE_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:2235:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:2238:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!ATTRIBUTE_DEFINER_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
            /*_.FIRSTLIST__V3*/
            meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:2240:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:2238:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_MELT_ATTRIBUTE_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_73_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_FIRST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST
        : public Melt_CallFrameWithValues<3>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[1];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<3> (sizeof(MeltFrame_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST), clos) {};
    } // end  class MeltFrame_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST_st*) meltfirstargp_;
            /* use arguments meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<3; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST nbval 3*/
    meltfram__.mcfr_nbvar = 3/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("AT_MELT_ATTRIBUTE_LAST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:2244:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:2247:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!ATTRIBUTE_DEFINER_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "DELQU_LAST");
            /*_.LASTLIST__V3*/
            meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:2249:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:2247:/ clear");
        /*clear*/ /*_.LASTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_MELT_ATTRIBUTE_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_74_WARMELTmiHOOKS_AT_MELT_ATTRIBUTE_LAST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif

#if MELT_HAVE_CLASSY_FRAME
    /*curframdeclclassy*/ class MeltFrame_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST
        : public Melt_CallFrameWithValues<3>
    {
    public: /*inframedeclclassy*/
        long _meltcf_longtab[1];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST(meltclosure_ptr_t clos=NULL) //the constructor
            : Melt_CallFrameWithValues<3> (sizeof(MeltFrame_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST), clos) {};
    } // end  class MeltFrame_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST
#else /*!MELT_HAVE_CLASSY_FRAME*/

    /*curframdeclstruct*/ struct meltframe_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST_st
    {
        int mcfr_nbvar;
        const char* mcfr_flocs;
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
        long _meltspare_;
    }
#endif /*MELT_HAVE_CLASSY_FRAME*/
    *meltframptr_=0,    meltfram__ /*declfrastruct*/
#if MELT_HAVE_CLASSY_FRAME
                  /*classyproc*/ (meltclosp_)
#endif /*MELT_HAVE_CLASSY_FRAME*/
                  ;
#define meltframe meltfram__
#if !MELT_HAVE_CLASSY_FRAME
    /*meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST is not initial declstructinit*/
    if (MELT_UNLIKELY(meltxargdescr_ == MELTPAR_MARKGGC))   /*mark for ggc*/
        {
            int meltix;
            meltframptr_ = (struct meltframe_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST_st*) meltfirstargp_;
            /* use arguments meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST output_curframe_declstruct_init */
            (void) meltclosp_;
            (void) meltfirstargp_;
            (void) meltxargdescr_;
            (void) meltxargtab_;
            (void) meltxresdescr_;
            (void) meltxrestab_;
            /* marking closure */
            gt_ggc_mx_melt_un (meltframptr_->mcfr_clos);
            for(meltix=0; meltix<3; meltix++)
                if (meltframptr_->mcfr_varptr[meltix])
                    gt_ggc_mx_melt_un (meltframptr_->mcfr_varptr[meltix]);
            return NULL;
        }/*end markggc*/;
    memset(&meltfram__, 0, sizeof(meltfram__));
    /* declstructinit plain routine meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST nbval 3*/
    meltfram__.mcfr_nbvar = 3/*nbval*/;
    meltfram__.mcfr_clos = meltclosp_;
    meltfram__.mcfr_prev = (struct melt_callframe_st *) melt_topframe /*declstructinit*/;
    melt_topframe = /* declstructinit */ (struct melt_callframe_st *) &meltfram__;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    melt_trace_start("AT_EXIT_FIRST", meltcallcount);
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:2289:/ getarg");
    /*_.FUN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:2292:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!FINAL_EXIT_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
            /*_.FIRSTLIST__V3*/
            meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:2294:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:2292:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_EXIT_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST_melt;
#if !MELT_HAVE_CLASSY_FRAME
    melt_topframe /*endprocroutine*/ = (struct melt_callframe_st*) meltfram__.mcfr_prev;
#endif /*!MELT_HAVE_CLASSY_FRAME*/
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_76_WARMELTmiHOOKS_AT_EXIT_FIRST*/



/**** end of warmelt-hooks+02.cc ****/
