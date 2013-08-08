/* GCC MELT GENERATED C++ FILE warmelt-base+01.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #1 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f1[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-base+01.cc declarations ****/
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
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/



class Melt_InitialClassyFrame_WARMELTmiBASE_h816406822; // forward declaration fromline 6529
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h816406822 Melt_InitialFrame;
/*** 4 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiBASE_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiBASE_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 4)?melt_WARMELTmiBASE_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 4)
        {
            melt_WARMELTmiBASE_module_var_ptr_tab[ix]= val;
            melt_WARMELTmiBASE_module_var_flags[ix/16] = true;
        }
}

/* MELT module variables indexes */
enum
{
    meltmodatix_none,
    meltmodvarix_DIAGV_GIMPLE_symb = 1,
    meltmodvarix_DIAGV_GIMPLE_SEQ_symb = 2,
    meltmodvarix_DIAGV_TREE_symb = 3,
    meltmodvarix_DIAGV_BASIC_BLOCK_symb = 4,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 5 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #3 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #4 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 5 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiBASE_plIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiBASE_miIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiBASE_stIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiBASE_diIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiBASE_pcIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/

/** declaration of hook melthook_HOOK_MELT_MAKE_LOCATION */
MELT_EXTERN
melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION(melt_ptr_t melthookdatap,
        const char* meltinp0_FILENAME,
        long meltinp1_LINENO)
;



/** end of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiBASE_LIST_NTH_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiBASE_REGISTER_DIAG_GIMPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h816406822 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiBASE__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiBASE__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_0 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_1 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_2 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_3 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_4 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_5 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_6 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_7 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_8 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_9 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_10 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_11 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_12 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_13 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_14 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_15 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_16 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_17 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_18 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_19 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_20 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_21 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_22 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_23 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_24 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_25 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_26 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_27 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_28 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_29 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_30 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_31 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_32 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_33 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_34 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_35 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_36 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_37 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_38 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_39 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_40 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_41 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_42 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_43 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_44 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_45 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_46 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_47 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_48 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_49 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_50 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_51 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_52 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_53 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_54 (meltinitial_frame_t*, char*);


/**** warmelt-base+01.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1715 */

    /** start of frame for meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ// fromline 1531
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ), clos) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ() //the constructor fromline 1605
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ)) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ


    /** end of frame for meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1660**/

    /* end of frame for routine meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1719 */

    /* classy proc frame meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ */ MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_GIMPLE_SEQ", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:528:/ getarg");
    /*_.C__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_CLOSURE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.C__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:529:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:530:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_GIMPLE_SEQ_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:528:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_DIAG_GIMPLE_SEQ", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1715 */

    /** start of frame for meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE// fromline 1531
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE), clos) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE() //the constructor fromline 1605
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE)) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE


    /** end of frame for meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1660**/

    /* end of frame for routine meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1719 */

    /* classy proc frame meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE */ MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_TREE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:532:/ getarg");
    /*_.C__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_CLOSURE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.C__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:533:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:534:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_TREE_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:532:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_DIAG_TREE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1715 */

    /** start of frame for meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK// fromline 1531
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK), clos) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK() //the constructor fromline 1605
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK)) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK


    /** end of frame for meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1660**/

    /* end of frame for routine meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1719 */

    /* classy proc frame meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK */ MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_BASIC_BLOCK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:536:/ getarg");
    /*_.C__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_CLOSURE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.C__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:537:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:538:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_BASIC_BLOCK_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:536:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_DIAG_BASIC_BLOCK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1715 */

    /** start of frame for meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO// fromline 1531
        : public Melt_CallFrameWithValues<268>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[15];
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
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<268> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO), clos) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO() //the constructor fromline 1605
            : Melt_CallFrameWithValues<268> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<268> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO)) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<268> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO


    /** end of frame for meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1660**/

    /* end of frame for routine meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1719 */

    /* classy proc frame meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO */ MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("DIAGNOSTIC_ARGS_MACRO", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:543:/ getarg");
    /*_.SEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:544:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:544:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:544:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 544;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro sexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:544:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:544:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:545:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-base.melt:545:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:545:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check sexp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 545;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:545:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/ meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:545:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-base.melt:546:/ quasiblock");


        MELT_LOCATION("warmelt-base.melt:547:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                    /*_.SLOC__V12*/ meltfptr[5] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SLOC__V12*/ meltfptr[5] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:548:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                    /*_.SEXCONT__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SEXCONT__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:549:/ apply");
        /*apply*/
        {
            /*_.TCONT__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.SEXCONT__V13*/ meltfptr[12]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:551:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:551:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L5*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:551:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 551;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro sloc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V12*/ meltfptr[5];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " tcont=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V14*/ meltfptr[13];
                            /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V15*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:551:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:551:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:552:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L6*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.SEXCONT__V13*/ meltfptr[12])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-base.melt:552:/ cond");
            /*cond*/ if (/*_#IS_LIST__L6*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:552:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check sexcont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 552;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V13*/ meltfptr[12];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:552:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:552:/ clear");
            /*clear*/ /*_#IS_LIST__L6*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:553:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE__L7*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.TCONT__V14*/ meltfptr[13])) == MELTOBMAG_MULTIPLE);;
            MELT_LOCATION("warmelt-base.melt:553:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE__L7*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:553:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check tcont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 553;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V14*/ meltfptr[13];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:553:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V20*/ meltfptr[18] = /*_._IFELSE___V21*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:553:/ clear");
            /*clear*/ /*_#IS_MULTIPLE__L7*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V21*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V20*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#MULTIPLE_LENGTH__L8*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.TCONT__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_#exeqI__L9*/ meltfnum[0] =
            ((/*_#MULTIPLE_LENGTH__L8*/ meltfnum[1]) != (3));;
        MELT_LOCATION("warmelt-base.melt:554:/ cond");
        /*cond*/ if (/*_#exeqI__L9*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:555:/ locexp");
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "DIAGNOSTIC_ARGS_MACRO needs 2 args: <nbargs> <vect>"), (melt_ptr_t)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:556:/ locexp");
                        /* diagnostic_args_macro DIAGARGBAD_CHK__1 */
                        melt_low_stderr_value ("diagnostic_args_macro sexp", /*_.SEXP__V2*/ meltfptr[1]);
                        melt_low_stderr_value ("diagnostic_args_macro tcont", /*_.TCONT__V14*/ meltfptr[13]);
                        ;
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:560:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:560:/ locexp");
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
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-base.melt:554:/ quasiblock");


                    /*_.PROGN___V25*/ meltfptr[24] = /*_.RETURN___V24*/ meltfptr[14];;
                    /*^compute*/
                    /*_._IF___V23*/ meltfptr[21] = /*_.PROGN___V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:554:/ clear");
                    /*clear*/ /*_.RETURN___V24*/ meltfptr[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V25*/ meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V23*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:562:/ quasiblock");


        /*_.MACROVARNBARG__V27*/ meltfptr[24] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V14*/ meltfptr[13]), (1)));;
        /*^compute*/
        /*_.MACROVARVECT__V28*/ meltfptr[27] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V14*/ meltfptr[13]), (2)));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L10*/ meltfnum[9] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MACROVARNBARG__V27*/ meltfptr[24]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-base.melt:565:/ cond");
        /*cond*/ if (/*_#IS_NOT_A__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:566:/ locexp");
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "(DIAGNOSTIC_ARGS_MACRO <nbargs> <vect>) not variable <nbargs>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:568:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:568:/ locexp");
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
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-base.melt:565:/ quasiblock");


                    /*_.PROGN___V31*/ meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V29*/ meltfptr[28] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:565:/ clear");
                    /*clear*/ /*_.RETURN___V30*/ meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/ meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L11*/ meltfnum[10] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MACROVARVECT__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-base.melt:569:/ cond");
        /*cond*/ if (/*_#IS_NOT_A__L11*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:570:/ locexp");
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "(DIAGNOSTIC_ARGS_MACRO <nbargs> <vect>) not variable <vect>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:572:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:572:/ locexp");
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
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-base.melt:569:/ quasiblock");


                    /*_.PROGN___V34*/ meltfptr[33] = /*_.RETURN___V33*/ meltfptr[30];;
                    /*^compute*/
                    /*_._IF___V32*/ meltfptr[29] = /*_.PROGN___V34*/ meltfptr[33];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:569:/ clear");
                    /*clear*/ /*_.RETURN___V33*/ meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V34*/ meltfptr[33] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V32*/ meltfptr[29] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:573:/ quasiblock");


        /*_.HOOK_MELT_MAKE_LOCATION__V36*/ meltfptr[33] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 574) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V37*/ meltfptr[36] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 576) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V38*/ meltfptr[37] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 577) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V39*/ meltfptr[38] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 578) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:578:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_EXIT*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_9_ARGLOOP*/ meltfrout->tabval[9]);
            /*_.MELT_MAKE_SEXPR__V40*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V39*/ meltfptr[38]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:577:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V40*/ meltfptr[39];
            /*_.MELT_MAKE_SEXPR__V41*/ meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V38*/ meltfptr[37]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V42*/ meltfptr[41] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 579) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V43*/ meltfptr[42] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 579) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:579:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_13_CSTRING*/ meltfrout->tabval[13]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_STR*/ meltfrout->tabval[14]);
            /*_.MELT_MAKE_SEXPR__V44*/ meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V43*/ meltfptr[42]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V45*/ meltfptr[44] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 580) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V46*/ meltfptr[45] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 580) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V47*/ meltfptr[46] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 580) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V48*/ meltfptr[47] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 580) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:580:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_17_CONSTANT_BOX*/ meltfrout->tabval[17]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_STR*/ meltfrout->tabval[14]);
            /*_.MELT_MAKE_SEXPR__V49*/ meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V48*/ meltfptr[47]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_16_BS*/ meltfrout->tabval[16]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V49*/ meltfptr[48];
            /*_.MELT_MAKE_SEXPR__V50*/ meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V47*/ meltfptr[46]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V50*/ meltfptr[49];
            /*_.MELT_MAKE_SEXPR__V51*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V46*/ meltfptr[45]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V52*/ meltfptr[51] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 582) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:582:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_16_BS*/ meltfrout->tabval[16]);
            /*_.MELT_MAKE_SEXPR__V53*/ meltfptr[52] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V52*/ meltfptr[51]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V54*/ meltfptr[53] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 583) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V55*/ meltfptr[54] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 583) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:583:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V56*/ meltfptr[55] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V55*/ meltfptr[54]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V56*/ meltfptr[55];
            /*_.MELT_MAKE_SEXPR__V57*/ meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V54*/ meltfptr[53]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:580:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V51*/ meltfptr[50];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V53*/ meltfptr[52];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V57*/ meltfptr[56];
            /*_.MELT_MAKE_SEXPR__V58*/ meltfptr[57] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V45*/ meltfptr[44]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V59*/ meltfptr[58] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 584) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:584:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V60*/ meltfptr[59] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V59*/ meltfptr[58]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:579:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V44*/ meltfptr[43];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V58*/ meltfptr[57];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V60*/ meltfptr[59];
            /*_.MELT_MAKE_SEXPR__V61*/ meltfptr[60] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V42*/ meltfptr[41]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V62*/ meltfptr[61] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 586) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 586) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:586:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_24_LONG*/ meltfrout->tabval[24]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_25_L*/ meltfrout->tabval[25]);
            /*_.MELT_MAKE_SEXPR__V64*/ meltfptr[63] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V65*/ meltfptr[64] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 587) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 587) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V67*/ meltfptr[66] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 587) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V68*/ meltfptr[67] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 587) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:587:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_28_STRING4OUT*/ meltfrout->tabval[28]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_29_DISCR_STRING*/ meltfrout->tabval[29]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_25_L*/ meltfrout->tabval[25]);
            /*_.MELT_MAKE_SEXPR__V69*/ meltfptr[68] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V68*/ meltfptr[67]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_27_SN*/ meltfrout->tabval[27]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V69*/ meltfptr[68];
            /*_.MELT_MAKE_SEXPR__V70*/ meltfptr[69] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V67*/ meltfptr[66]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V70*/ meltfptr[69];
            /*_.MELT_MAKE_SEXPR__V71*/ meltfptr[70] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V72*/ meltfptr[71] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 589) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:589:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_27_SN*/ meltfrout->tabval[27]);
            /*_.MELT_MAKE_SEXPR__V73*/ meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V72*/ meltfptr[71]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V74*/ meltfptr[73] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 590) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V75*/ meltfptr[74] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 590) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:590:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V76*/ meltfptr[75] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V75*/ meltfptr[74]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V76*/ meltfptr[75];
            /*_.MELT_MAKE_SEXPR__V77*/ meltfptr[76] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V74*/ meltfptr[73]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:587:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V71*/ meltfptr[70];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V73*/ meltfptr[72];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V77*/ meltfptr[76];
            /*_.MELT_MAKE_SEXPR__V78*/ meltfptr[77] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V65*/ meltfptr[64]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V79*/ meltfptr[78] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 591) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:591:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V80*/ meltfptr[79] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V79*/ meltfptr[78]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:586:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V64*/ meltfptr[63];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V78*/ meltfptr[77];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V80*/ meltfptr[79];
            /*_.MELT_MAKE_SEXPR__V81*/ meltfptr[80] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V62*/ meltfptr[61]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V82*/ meltfptr[81] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 593) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V83*/ meltfptr[82] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 593) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:593:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_36_TREE*/ meltfrout->tabval[36]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_TR*/ meltfrout->tabval[37]);
            /*_.MELT_MAKE_SEXPR__V84*/ meltfptr[83] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V83*/ meltfptr[82]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V85*/ meltfptr[84] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 594) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V86*/ meltfptr[85] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 594) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:594:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_IS_CLOSURE*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_40_DIAGV_TREE*/ meltfrout->tabval[40]);
            /*_.MELT_MAKE_SEXPR__V87*/ meltfptr[86] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V86*/ meltfptr[85]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_ASSERT_MSG*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_tree";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V87*/ meltfptr[86];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_40_DIAGV_TREE*/ meltfrout->tabval[40]);
            /*_.MELT_MAKE_SEXPR__V88*/ meltfptr[87] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V85*/ meltfptr[84]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V89*/ meltfptr[88] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 595) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V90*/ meltfptr[89] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 595) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V91*/ meltfptr[90] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 595) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V92*/ meltfptr[91] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 595) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:595:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_40_DIAGV_TREE*/ meltfrout->tabval[40]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_TR*/ meltfrout->tabval[37]);
            /*_.MELT_MAKE_SEXPR__V93*/ meltfptr[92] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V92*/ meltfptr[91]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V93*/ meltfptr[92];
            /*_.MELT_MAKE_SEXPR__V94*/ meltfptr[93] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V91*/ meltfptr[90]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V94*/ meltfptr[93];
            /*_.MELT_MAKE_SEXPR__V95*/ meltfptr[94] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V90*/ meltfptr[89]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V96*/ meltfptr[95] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 597) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:597:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*_.MELT_MAKE_SEXPR__V97*/ meltfptr[96] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V96*/ meltfptr[95]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V98*/ meltfptr[97] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 598) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V99*/ meltfptr[98] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 598) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:598:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V100*/ meltfptr[99] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V99*/ meltfptr[98]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V100*/ meltfptr[99];
            /*_.MELT_MAKE_SEXPR__V101*/ meltfptr[100] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V98*/ meltfptr[97]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:595:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V95*/ meltfptr[94];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V97*/ meltfptr[96];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V101*/ meltfptr[100];
            /*_.MELT_MAKE_SEXPR__V102*/ meltfptr[101] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V89*/ meltfptr[88]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V103*/ meltfptr[102] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 599) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:599:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V104*/ meltfptr[103] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V103*/ meltfptr[102]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:593:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V84*/ meltfptr[83];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V88*/ meltfptr[87];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V102*/ meltfptr[101];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V104*/ meltfptr[103];
            /*_.MELT_MAKE_SEXPR__V105*/ meltfptr[104] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V82*/ meltfptr[81]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V106*/ meltfptr[105] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 601) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V107*/ meltfptr[106] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 601) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:601:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_51_GIMPLE*/ meltfrout->tabval[51]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_52_GI*/ meltfrout->tabval[52]);
            /*_.MELT_MAKE_SEXPR__V108*/ meltfptr[107] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V107*/ meltfptr[106]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V109*/ meltfptr[108] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 602) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V110*/ meltfptr[109] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 602) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:602:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_IS_CLOSURE*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_DIAGV_GIMPLE*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V111*/ meltfptr[110] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V110*/ meltfptr[109]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_ASSERT_MSG*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_gimple";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V111*/ meltfptr[110];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_DIAGV_GIMPLE*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V112*/ meltfptr[111] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V109*/ meltfptr[108]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V113*/ meltfptr[112] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 603) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V114*/ meltfptr[113] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 603) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V115*/ meltfptr[114] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 603) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V116*/ meltfptr[115] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 603) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:603:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_DIAGV_GIMPLE*/ meltfrout->tabval[55]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_52_GI*/ meltfrout->tabval[52]);
            /*_.MELT_MAKE_SEXPR__V117*/ meltfptr[116] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V116*/ meltfptr[115]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V117*/ meltfptr[116];
            /*_.MELT_MAKE_SEXPR__V118*/ meltfptr[117] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V115*/ meltfptr[114]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V118*/ meltfptr[117];
            /*_.MELT_MAKE_SEXPR__V119*/ meltfptr[118] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V114*/ meltfptr[113]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V120*/ meltfptr[119] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 605) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:605:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*_.MELT_MAKE_SEXPR__V121*/ meltfptr[120] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V120*/ meltfptr[119]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V122*/ meltfptr[121] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 606) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V123*/ meltfptr[122] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 606) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:606:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V124*/ meltfptr[123] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V123*/ meltfptr[122]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V124*/ meltfptr[123];
            /*_.MELT_MAKE_SEXPR__V125*/ meltfptr[124] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V122*/ meltfptr[121]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:603:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V119*/ meltfptr[118];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V121*/ meltfptr[120];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V125*/ meltfptr[124];
            /*_.MELT_MAKE_SEXPR__V126*/ meltfptr[125] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V113*/ meltfptr[112]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V127*/ meltfptr[126] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 607) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:607:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V128*/ meltfptr[127] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V127*/ meltfptr[126]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:601:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V108*/ meltfptr[107];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V112*/ meltfptr[111];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V126*/ meltfptr[125];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V128*/ meltfptr[127];
            /*_.MELT_MAKE_SEXPR__V129*/ meltfptr[128] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V106*/ meltfptr[105]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V130*/ meltfptr[129] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 609) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V131*/ meltfptr[130] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 609) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:609:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_66_GIMPLE_SEQ*/ meltfrout->tabval[66]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_67_GS*/ meltfrout->tabval[67]);
            /*_.MELT_MAKE_SEXPR__V132*/ meltfptr[131] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V131*/ meltfptr[130]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V133*/ meltfptr[132] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 610) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V134*/ meltfptr[133] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 610) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:610:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_IS_CLOSURE*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_70_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[70]);
            /*_.MELT_MAKE_SEXPR__V135*/ meltfptr[134] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V134*/ meltfptr[133]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_ASSERT_MSG*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_gimple_seq";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V135*/ meltfptr[134];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_70_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[70]);
            /*_.MELT_MAKE_SEXPR__V136*/ meltfptr[135] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V133*/ meltfptr[132]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V137*/ meltfptr[136] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 611) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V138*/ meltfptr[137] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 611) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V139*/ meltfptr[138] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 611) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V140*/ meltfptr[139] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 611) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:611:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_70_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[70]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_67_GS*/ meltfrout->tabval[67]);
            /*_.MELT_MAKE_SEXPR__V141*/ meltfptr[140] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V140*/ meltfptr[139]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V141*/ meltfptr[140];
            /*_.MELT_MAKE_SEXPR__V142*/ meltfptr[141] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V139*/ meltfptr[138]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V142*/ meltfptr[141];
            /*_.MELT_MAKE_SEXPR__V143*/ meltfptr[142] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V138*/ meltfptr[137]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V144*/ meltfptr[143] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 613) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:613:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*_.MELT_MAKE_SEXPR__V145*/ meltfptr[144] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V144*/ meltfptr[143]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V146*/ meltfptr[145] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 614) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V147*/ meltfptr[146] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 614) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:614:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V148*/ meltfptr[147] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V147*/ meltfptr[146]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V148*/ meltfptr[147];
            /*_.MELT_MAKE_SEXPR__V149*/ meltfptr[148] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V146*/ meltfptr[145]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:611:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V143*/ meltfptr[142];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V145*/ meltfptr[144];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V149*/ meltfptr[148];
            /*_.MELT_MAKE_SEXPR__V150*/ meltfptr[149] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V137*/ meltfptr[136]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V151*/ meltfptr[150] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 615) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:615:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V152*/ meltfptr[151] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V151*/ meltfptr[150]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:609:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V132*/ meltfptr[131];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V136*/ meltfptr[135];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V150*/ meltfptr[149];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V152*/ meltfptr[151];
            /*_.MELT_MAKE_SEXPR__V153*/ meltfptr[152] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V130*/ meltfptr[129]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V154*/ meltfptr[153] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 617) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V155*/ meltfptr[154] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 617) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:617:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_81_BASIC_BLOCK*/ meltfrout->tabval[81]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_82_BB*/ meltfrout->tabval[82]);
            /*_.MELT_MAKE_SEXPR__V156*/ meltfptr[155] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V155*/ meltfptr[154]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V157*/ meltfptr[156] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 618) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V158*/ meltfptr[157] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 618) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:618:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_IS_CLOSURE*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_85_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[85]);
            /*_.MELT_MAKE_SEXPR__V159*/ meltfptr[158] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V158*/ meltfptr[157]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_ASSERT_MSG*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_basic_block";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V159*/ meltfptr[158];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_85_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[85]);
            /*_.MELT_MAKE_SEXPR__V160*/ meltfptr[159] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V157*/ meltfptr[156]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V161*/ meltfptr[160] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 619) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V162*/ meltfptr[161] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 619) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V163*/ meltfptr[162] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 619) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V164*/ meltfptr[163] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 619) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:619:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_85_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[85]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_82_BB*/ meltfrout->tabval[82]);
            /*_.MELT_MAKE_SEXPR__V165*/ meltfptr[164] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V164*/ meltfptr[163]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V165*/ meltfptr[164];
            /*_.MELT_MAKE_SEXPR__V166*/ meltfptr[165] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V163*/ meltfptr[162]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V166*/ meltfptr[165];
            /*_.MELT_MAKE_SEXPR__V167*/ meltfptr[166] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V162*/ meltfptr[161]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V168*/ meltfptr[167] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 621) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:621:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_43_S*/ meltfrout->tabval[43]);
            /*_.MELT_MAKE_SEXPR__V169*/ meltfptr[168] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V168*/ meltfptr[167]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V170*/ meltfptr[169] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 622) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V171*/ meltfptr[170] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 622) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:622:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V172*/ meltfptr[171] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V171*/ meltfptr[170]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V172*/ meltfptr[171];
            /*_.MELT_MAKE_SEXPR__V173*/ meltfptr[172] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V170*/ meltfptr[169]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:619:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V167*/ meltfptr[166];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V169*/ meltfptr[168];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V173*/ meltfptr[172];
            /*_.MELT_MAKE_SEXPR__V174*/ meltfptr[173] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V161*/ meltfptr[160]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V175*/ meltfptr[174] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 623) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:623:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V176*/ meltfptr[175] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V175*/ meltfptr[174]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:617:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V156*/ meltfptr[155];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V160*/ meltfptr[159];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V174*/ meltfptr[173];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V176*/ meltfptr[175];
            /*_.MELT_MAKE_SEXPR__V177*/ meltfptr[176] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V154*/ meltfptr[153]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V178*/ meltfptr[177] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 625) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V179*/ meltfptr[178] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 625) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:625:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_96_VALUE*/ meltfrout->tabval[96]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V180*/ meltfptr[179] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V179*/ meltfptr[178]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V181*/ meltfptr[180] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 626) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V182*/ meltfptr[181] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 627) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V183*/ meltfptr[182] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 627) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:627:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_99_IS_INTEGERBOX*/ meltfrout->tabval[99]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V184*/ meltfptr[183] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V183*/ meltfptr[182]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V185*/ meltfptr[184] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 628) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V186*/ meltfptr[185] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 628) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V187*/ meltfptr[186] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 628) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V188*/ meltfptr[187] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 628) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V189*/ meltfptr[188] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 628) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:628:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_105_GET_INT*/ meltfrout->tabval[105]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V190*/ meltfptr[189] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V189*/ meltfptr[188]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_28_STRING4OUT*/ meltfrout->tabval[28]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_29_DISCR_STRING*/ meltfrout->tabval[29]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V190*/ meltfptr[189];
            /*_.MELT_MAKE_SEXPR__V191*/ meltfptr[190] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V188*/ meltfptr[187]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_27_SN*/ meltfrout->tabval[27]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V191*/ meltfptr[190];
            /*_.MELT_MAKE_SEXPR__V192*/ meltfptr[191] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V187*/ meltfptr[186]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V192*/ meltfptr[191];
            /*_.MELT_MAKE_SEXPR__V193*/ meltfptr[192] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V186*/ meltfptr[185]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V194*/ meltfptr[193] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 630) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:630:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_27_SN*/ meltfrout->tabval[27]);
            /*_.MELT_MAKE_SEXPR__V195*/ meltfptr[194] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V194*/ meltfptr[193]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V196*/ meltfptr[195] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 631) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V197*/ meltfptr[196] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 631) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:631:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V198*/ meltfptr[197] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V197*/ meltfptr[196]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V198*/ meltfptr[197];
            /*_.MELT_MAKE_SEXPR__V199*/ meltfptr[198] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V196*/ meltfptr[195]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:628:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V193*/ meltfptr[192];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V195*/ meltfptr[194];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V199*/ meltfptr[198];
            /*_.MELT_MAKE_SEXPR__V200*/ meltfptr[199] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V185*/ meltfptr[184]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V201*/ meltfptr[200] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 632) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:632:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V202*/ meltfptr[201] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V201*/ meltfptr[200]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:627:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V184*/ meltfptr[183];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V200*/ meltfptr[199];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V202*/ meltfptr[201];
            /*_.MELT_MAKE_SEXPR__V203*/ meltfptr[202] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V182*/ meltfptr[181]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V204*/ meltfptr[203] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 634) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V205*/ meltfptr[204] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 634) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:634:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_112_IS_STRING*/ meltfrout->tabval[112]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V206*/ meltfptr[205] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V205*/ meltfptr[204]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V207*/ meltfptr[206] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 635) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:635:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V208*/ meltfptr[207] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V207*/ meltfptr[206]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V209*/ meltfptr[208] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 636) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V210*/ meltfptr[209] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 636) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:636:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V211*/ meltfptr[210] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V210*/ meltfptr[209]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V211*/ meltfptr[210];
            /*_.MELT_MAKE_SEXPR__V212*/ meltfptr[211] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V209*/ meltfptr[208]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V213*/ meltfptr[212] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 637) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:637:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V214*/ meltfptr[213] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V213*/ meltfptr[212]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:634:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V206*/ meltfptr[205];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V208*/ meltfptr[207];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V212*/ meltfptr[211];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V214*/ meltfptr[213];
            /*_.MELT_MAKE_SEXPR__V215*/ meltfptr[214] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V204*/ meltfptr[203]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V216*/ meltfptr[215] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 639) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V217*/ meltfptr[216] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 639) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:639:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_119_IS_A*/ meltfrout->tabval[119]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_121_CLASS_NAMED*/ meltfrout->tabval[121]);
            /*_.MELT_MAKE_SEXPR__V218*/ meltfptr[217] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V217*/ meltfptr[216]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V219*/ meltfptr[218] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 640) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V220*/ meltfptr[219] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 640) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V221*/ meltfptr[220] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 640) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V222*/ meltfptr[221] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 640) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:640:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_124_UNSAFE_GET_FIELD*/ meltfrout->tabval[124]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_125_NAMED_NAME*/ meltfrout->tabval[125]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_V*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V223*/ meltfptr[222] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V222*/ meltfptr[221]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_123_NAM*/ meltfrout->tabval[123]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V223*/ meltfptr[222];
            /*_.MELT_MAKE_SEXPR__V224*/ meltfptr[223] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V221*/ meltfptr[220]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V224*/ meltfptr[223];
            /*_.MELT_MAKE_SEXPR__V225*/ meltfptr[224] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V220*/ meltfptr[219]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V226*/ meltfptr[225] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 642) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V227*/ meltfptr[226] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 642) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:642:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_112_IS_STRING*/ meltfrout->tabval[112]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_123_NAM*/ meltfrout->tabval[123]);
            /*_.MELT_MAKE_SEXPR__V228*/ meltfptr[227] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V227*/ meltfptr[226]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_ASSERT_MSG*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check nam";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V228*/ meltfptr[227];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_123_NAM*/ meltfrout->tabval[123]);
            /*_.MELT_MAKE_SEXPR__V229*/ meltfptr[228] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V226*/ meltfptr[225]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V230*/ meltfptr[229] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 643) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:643:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_MULTIPLE_PUT_NTH*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V28*/ meltfptr[27];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_123_NAM*/ meltfrout->tabval[123]);
            /*_.MELT_MAKE_SEXPR__V231*/ meltfptr[230] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V230*/ meltfptr[229]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V232*/ meltfptr[231] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 644) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V233*/ meltfptr[232] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 644) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:644:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_pl*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V234*/ meltfptr[233] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V233*/ meltfptr[232]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_SETQ*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V27*/ meltfptr[24];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V234*/ meltfptr[233];
            /*_.MELT_MAKE_SEXPR__V235*/ meltfptr[234] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V232*/ meltfptr[231]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:640:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V225*/ meltfptr[224];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V229*/ meltfptr[228];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V231*/ meltfptr[230];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V235*/ meltfptr[234];
            /*_.MELT_MAKE_SEXPR__V236*/ meltfptr[235] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V219*/ meltfptr[218]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V237*/ meltfptr[236] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 645) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:645:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V238*/ meltfptr[237] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V237*/ meltfptr[236]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:639:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V218*/ meltfptr[217];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V236*/ meltfptr[235];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V238*/ meltfptr[237];
            /*_.MELT_MAKE_SEXPR__V239*/ meltfptr[238] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V216*/ meltfptr[215]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:626:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_98_COND*/ meltfrout->tabval[98]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V203*/ meltfptr[202];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V215*/ meltfptr[214];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V239*/ meltfptr[238];
            /*_.MELT_MAKE_SEXPR__V240*/ meltfptr[239] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V181*/ meltfptr[180]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:625:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V180*/ meltfptr[179];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V240*/ meltfptr[239];
            /*_.MELT_MAKE_SEXPR__V241*/ meltfptr[240] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V178*/ meltfptr[177]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V242*/ meltfptr[241] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 649) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V243*/ meltfptr[242] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 650) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V244*/ meltfptr[243] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 650) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V245*/ meltfptr[244] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 650) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V246*/ meltfptr[245] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 650) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:650:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_139_VARIADIC_CTYPE*/ meltfrout->tabval[139]);
            /*^apply.arg*/
            argtab[1].meltbp_long = 0;
            /*_.MELT_MAKE_SEXPR__V247*/ meltfptr[246] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V246*/ meltfptr[245]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_138_VCTY*/ meltfrout->tabval[138]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V247*/ meltfptr[246];
            /*_.MELT_MAKE_SEXPR__V248*/ meltfptr[247] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V245*/ meltfptr[244]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V248*/ meltfptr[247];
            /*_.MELT_MAKE_SEXPR__V249*/ meltfptr[248] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V244*/ meltfptr[243]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V250*/ meltfptr[249] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 652) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V251*/ meltfptr[250] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 653) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:653:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_141_GET_FIELD*/ meltfrout->tabval[141]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_125_NAMED_NAME*/ meltfrout->tabval[125]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_138_VCTY*/ meltfrout->tabval[138]);
            /*_.MELT_MAKE_SEXPR__V252*/ meltfptr[251] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V251*/ meltfptr[250]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:652:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_140_ERRORMSG_STRV*/ meltfrout->tabval[140]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "DIAGNOSTIC_ARGS_MACRO for unsupported ctype";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V252*/ meltfptr[251];
            /*_.MELT_MAKE_SEXPR__V253*/ meltfptr[252] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V250*/ meltfptr[249]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V254*/ meltfptr[253] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 654) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:654:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_ASSERT_MSG*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "invalid variadic argument to DIAGNOSTIC_ARGS_MACRO";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_138_VCTY*/ meltfrout->tabval[138]);
            /*_.MELT_MAKE_SEXPR__V255*/ meltfptr[254] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V254*/ meltfptr[253]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:650:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LET*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V249*/ meltfptr[248];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V253*/ meltfptr[252];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V255*/ meltfptr[254];
            /*_.MELT_MAKE_SEXPR__V256*/ meltfptr[255] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V243*/ meltfptr[242]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V257*/ meltfptr[256] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[7]),  "warmelt-base.melt", 655) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:655:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_VOID*/ meltfrout->tabval[23]);
            /*_.MELT_MAKE_SEXPR__V258*/ meltfptr[257] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V257*/ meltfptr[256]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:649:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_136_ELSE*/ meltfrout->tabval[136]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V256*/ meltfptr[255];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V258*/ meltfptr[257];
            /*_.MELT_MAKE_SEXPR__V259*/ meltfptr[258] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V242*/ meltfptr[241]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:576:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[10];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_10_VARIADIC*/ meltfrout->tabval[10]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V41*/ meltfptr[40];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V61*/ meltfptr[60];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V81*/ meltfptr[80];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V105*/ meltfptr[104];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V129*/ meltfptr[128];
            /*^apply.arg*/
            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V153*/ meltfptr[152];
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V177*/ meltfptr[176];
            /*^apply.arg*/
            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V241*/ meltfptr[240];
            /*^apply.arg*/
            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V259*/ meltfptr[258];
            /*_.MELT_MAKE_SEXPR__V260*/ meltfptr[259] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V37*/ meltfptr[36]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:574:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_8_FOREVER*/ meltfrout->tabval[8]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_9_ARGLOOP*/ meltfrout->tabval[9]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V260*/ meltfptr[259];
            /*_.RESEXP__V261*/ meltfptr[260] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V36*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:660:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L12*/ meltfnum[11] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:660:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L12*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:660:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 660;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro resexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESEXP__V261*/ meltfptr[260];
                            /*_.MELT_DEBUG_FUN__V263*/ meltfptr[262] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V262*/ meltfptr[261] = /*_.MELT_DEBUG_FUN__V263*/ meltfptr[262];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:660:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V263*/ meltfptr[262] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V262*/ meltfptr[261] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:660:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L12*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V262*/ meltfptr[261] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-base.melt:661:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.MEXP__V265*/ meltfptr[261] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.RESEXP__V261*/ meltfptr[260]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:663:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L14*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:663:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L14*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L15*/ meltfnum[11] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:663:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 663;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro final mexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXP__V265*/ meltfptr[261];
                            /*_.MELT_DEBUG_FUN__V267*/ meltfptr[266] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V266*/ meltfptr[265] = /*_.MELT_DEBUG_FUN__V267*/ meltfptr[266];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:663:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L15*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V267*/ meltfptr[266] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V266*/ meltfptr[265] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:663:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L14*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V266*/ meltfptr[265] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:664:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.MEXP__V265*/ meltfptr[261]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.MEXP__V265*/ meltfptr[261])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.MEXP__V265*/ meltfptr[261]), (1), (/*_.SLOC__V12*/ meltfptr[5]), "LOCA_LOCATION");
                    ;
                    /*^touch*/
                    meltgc_touch(/*_.MEXP__V265*/ meltfptr[261]);
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.MEXP__V265*/ meltfptr[261], "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:665:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.MEXP__V265*/ meltfptr[261];;

        {
            MELT_LOCATION("warmelt-base.melt:665:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*_.LET___V264*/ meltfptr[262] = /*_.RETURN___V268*/ meltfptr[266];;

        MELT_LOCATION("warmelt-base.melt:661:/ clear");
        /*clear*/ /*_.MEXP__V265*/ meltfptr[261] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V268*/ meltfptr[266] = 0 ;
        /*_.LET___V35*/ meltfptr[30] = /*_.LET___V264*/ meltfptr[262];;

        MELT_LOCATION("warmelt-base.melt:573:/ clear");
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V36*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V37*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V38*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V39*/ meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V40*/ meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V41*/ meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V42*/ meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V43*/ meltfptr[42] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V44*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V45*/ meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V46*/ meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V47*/ meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V48*/ meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V49*/ meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V50*/ meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V51*/ meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V52*/ meltfptr[51] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V53*/ meltfptr[52] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V54*/ meltfptr[53] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V55*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V56*/ meltfptr[55] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V57*/ meltfptr[56] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V58*/ meltfptr[57] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V59*/ meltfptr[58] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V60*/ meltfptr[59] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V61*/ meltfptr[60] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V62*/ meltfptr[61] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V64*/ meltfptr[63] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V65*/ meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V67*/ meltfptr[66] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V68*/ meltfptr[67] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V69*/ meltfptr[68] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V70*/ meltfptr[69] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V71*/ meltfptr[70] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V72*/ meltfptr[71] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V73*/ meltfptr[72] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V74*/ meltfptr[73] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V75*/ meltfptr[74] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V76*/ meltfptr[75] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V77*/ meltfptr[76] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V78*/ meltfptr[77] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V79*/ meltfptr[78] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V80*/ meltfptr[79] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V81*/ meltfptr[80] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V82*/ meltfptr[81] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V83*/ meltfptr[82] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V84*/ meltfptr[83] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V85*/ meltfptr[84] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V86*/ meltfptr[85] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V87*/ meltfptr[86] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V88*/ meltfptr[87] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V89*/ meltfptr[88] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V90*/ meltfptr[89] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V91*/ meltfptr[90] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V92*/ meltfptr[91] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V93*/ meltfptr[92] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V94*/ meltfptr[93] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V95*/ meltfptr[94] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V96*/ meltfptr[95] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V97*/ meltfptr[96] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V98*/ meltfptr[97] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V99*/ meltfptr[98] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V100*/ meltfptr[99] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V101*/ meltfptr[100] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V102*/ meltfptr[101] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V103*/ meltfptr[102] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V104*/ meltfptr[103] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V105*/ meltfptr[104] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V106*/ meltfptr[105] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V107*/ meltfptr[106] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V108*/ meltfptr[107] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V109*/ meltfptr[108] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V110*/ meltfptr[109] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V111*/ meltfptr[110] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V112*/ meltfptr[111] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V113*/ meltfptr[112] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V114*/ meltfptr[113] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V115*/ meltfptr[114] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V116*/ meltfptr[115] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V117*/ meltfptr[116] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V118*/ meltfptr[117] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V119*/ meltfptr[118] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V120*/ meltfptr[119] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V121*/ meltfptr[120] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V122*/ meltfptr[121] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V123*/ meltfptr[122] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V124*/ meltfptr[123] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V125*/ meltfptr[124] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V126*/ meltfptr[125] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V127*/ meltfptr[126] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V128*/ meltfptr[127] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V129*/ meltfptr[128] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V130*/ meltfptr[129] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V131*/ meltfptr[130] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V132*/ meltfptr[131] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V133*/ meltfptr[132] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V134*/ meltfptr[133] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V135*/ meltfptr[134] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V136*/ meltfptr[135] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V137*/ meltfptr[136] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V138*/ meltfptr[137] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V139*/ meltfptr[138] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V140*/ meltfptr[139] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V141*/ meltfptr[140] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V142*/ meltfptr[141] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V143*/ meltfptr[142] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V144*/ meltfptr[143] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V145*/ meltfptr[144] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V146*/ meltfptr[145] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V147*/ meltfptr[146] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V148*/ meltfptr[147] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V149*/ meltfptr[148] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V150*/ meltfptr[149] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V151*/ meltfptr[150] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V152*/ meltfptr[151] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V153*/ meltfptr[152] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V154*/ meltfptr[153] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V155*/ meltfptr[154] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V156*/ meltfptr[155] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V157*/ meltfptr[156] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V158*/ meltfptr[157] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V159*/ meltfptr[158] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V160*/ meltfptr[159] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V161*/ meltfptr[160] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V162*/ meltfptr[161] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V163*/ meltfptr[162] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V164*/ meltfptr[163] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V165*/ meltfptr[164] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V166*/ meltfptr[165] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V167*/ meltfptr[166] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V168*/ meltfptr[167] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V169*/ meltfptr[168] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V170*/ meltfptr[169] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V171*/ meltfptr[170] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V172*/ meltfptr[171] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V173*/ meltfptr[172] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V174*/ meltfptr[173] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V175*/ meltfptr[174] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V176*/ meltfptr[175] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V177*/ meltfptr[176] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V178*/ meltfptr[177] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V179*/ meltfptr[178] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V180*/ meltfptr[179] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V181*/ meltfptr[180] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V182*/ meltfptr[181] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V183*/ meltfptr[182] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V184*/ meltfptr[183] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V185*/ meltfptr[184] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V186*/ meltfptr[185] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V187*/ meltfptr[186] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V188*/ meltfptr[187] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V189*/ meltfptr[188] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V190*/ meltfptr[189] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V191*/ meltfptr[190] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V192*/ meltfptr[191] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V193*/ meltfptr[192] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V194*/ meltfptr[193] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V195*/ meltfptr[194] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V196*/ meltfptr[195] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V197*/ meltfptr[196] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V198*/ meltfptr[197] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V199*/ meltfptr[198] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V200*/ meltfptr[199] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V201*/ meltfptr[200] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V202*/ meltfptr[201] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V203*/ meltfptr[202] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V204*/ meltfptr[203] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V205*/ meltfptr[204] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V206*/ meltfptr[205] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V207*/ meltfptr[206] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V208*/ meltfptr[207] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V209*/ meltfptr[208] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V210*/ meltfptr[209] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V211*/ meltfptr[210] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V212*/ meltfptr[211] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V213*/ meltfptr[212] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V214*/ meltfptr[213] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V215*/ meltfptr[214] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V216*/ meltfptr[215] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V217*/ meltfptr[216] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V218*/ meltfptr[217] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V219*/ meltfptr[218] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V220*/ meltfptr[219] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V221*/ meltfptr[220] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V222*/ meltfptr[221] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V223*/ meltfptr[222] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V224*/ meltfptr[223] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V225*/ meltfptr[224] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V226*/ meltfptr[225] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V227*/ meltfptr[226] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V228*/ meltfptr[227] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V229*/ meltfptr[228] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V230*/ meltfptr[229] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V231*/ meltfptr[230] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V232*/ meltfptr[231] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V233*/ meltfptr[232] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V234*/ meltfptr[233] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V235*/ meltfptr[234] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V236*/ meltfptr[235] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V237*/ meltfptr[236] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V238*/ meltfptr[237] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V239*/ meltfptr[238] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V240*/ meltfptr[239] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V241*/ meltfptr[240] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V242*/ meltfptr[241] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V243*/ meltfptr[242] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V244*/ meltfptr[243] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V245*/ meltfptr[244] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V246*/ meltfptr[245] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V247*/ meltfptr[246] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V248*/ meltfptr[247] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V249*/ meltfptr[248] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V250*/ meltfptr[249] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V251*/ meltfptr[250] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V252*/ meltfptr[251] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V253*/ meltfptr[252] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V254*/ meltfptr[253] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V255*/ meltfptr[254] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V256*/ meltfptr[255] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V257*/ meltfptr[256] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V258*/ meltfptr[257] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V259*/ meltfptr[258] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V260*/ meltfptr[259] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESEXP__V261*/ meltfptr[260] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V264*/ meltfptr[262] = 0 ;
        /*_.LET___V26*/ meltfptr[14] = /*_.LET___V35*/ meltfptr[30];;

        MELT_LOCATION("warmelt-base.melt:562:/ clear");
        /*clear*/ /*_.MACROVARNBARG__V27*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROVARVECT__V28*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L10*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L11*/ meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V32*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V35*/ meltfptr[30] = 0 ;
        /*_.LET___V11*/ meltfptr[9] = /*_.LET___V26*/ meltfptr[14];;

        MELT_LOCATION("warmelt-base.melt:546:/ clear");
        /*clear*/ /*_.SLOC__V12*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXCONT__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TCONT__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V20*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MULTIPLE_LENGTH__L8*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#exeqI__L9*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V26*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:543:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V11*/ meltfptr[9];;

        {
            MELT_LOCATION("warmelt-base.melt:543:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V11*/ meltfptr[9] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("DIAGNOSTIC_ARGS_MACRO", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1715 */

    /** start of frame for meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA// fromline 1531
        : public Melt_CallFrameWithValues<19>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[5];
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
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA() //the constructor fromline 1605
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA


    /** end of frame for meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1660**/

    /* end of frame for routine meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1719 */

    /* classy proc frame meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA */ MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPARE_NAMED_ALPHA", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1196:/ getarg");
    /*_.N1__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.N2__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.N2__V3*/ meltfptr[2])) != NULL);

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
        /*_#eqeq__L1*/ meltfnum[0] =
            ((/*_.N1__V2*/ meltfptr[1]) == (/*_.N2__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-base.melt:1200:/ cond");
        /*cond*/ if (/*_#eqeq__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:1201:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_0*/ meltfrout->tabval[0]);;
                    MELT_LOCATION("warmelt-base.melt:1201:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_.RETURN___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1200:/ clear");
                    /*clear*/ /*_.RETURN___V5*/ meltfptr[4] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_NOT_A__L2*/ meltfnum[1] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-base.melt:1202:/ cond");
                    /*cond*/ if (/*_#IS_NOT_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-base.melt:1203:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_2*/ meltfrout->tabval[2]);;
                                MELT_LOCATION("warmelt-base.melt:1203:/ putxtraresult");
                                if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                ;
                                /*^finalreturn*/
                                ;
                                /*finalret*/ goto meltlabend_rout ;
                                /*_._IFELSE___V6*/ meltfptr[4] = /*_.RETURN___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1202:/ clear");
                                /*clear*/ /*_.RETURN___V7*/ meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_NOT_A__L3*/ meltfnum[2] =
                                    !melt_is_instance_of((melt_ptr_t)(/*_.N2__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])));;
                                MELT_LOCATION("warmelt-base.melt:1204:/ cond");
                                /*cond*/ if (/*_#IS_NOT_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1205:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_3*/ meltfrout->tabval[3]);;
                                            MELT_LOCATION("warmelt-base.melt:1205:/ putxtraresult");
                                            if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                            if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                            if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                            ;
                                            /*^finalreturn*/
                                            ;
                                            /*finalret*/ goto meltlabend_rout ;
                                            /*_._IFELSE___V8*/ meltfptr[6] = /*_.RETURN___V9*/ meltfptr[8];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1204:/ clear");
                                            /*clear*/ /*_.RETURN___V9*/ meltfptr[8] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1207:/ quasiblock");


                                            /*^getslot*/
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN1__V11*/ meltfptr[10] = slot;
                                            };
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1208:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N2__V3*/ meltfptr[2]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN2__V12*/ meltfptr[11] = slot;
                                            };
                                            ;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#STRINGlt__L4*/ meltfnum[3] =
                                                melt_string_less((melt_ptr_t)(/*_.SN1__V11*/ meltfptr[10]), (melt_ptr_t)(/*_.SN2__V12*/ meltfptr[11]));;
                                            MELT_LOCATION("warmelt-base.melt:1210:/ cond");
                                            /*cond*/ if (/*_#STRINGlt__L4*/ meltfnum[3]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1211:/ quasiblock");


                                                        /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_4*/ meltfrout->tabval[4]);;
                                                        MELT_LOCATION("warmelt-base.melt:1211:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/ goto meltlabend_rout ;
                                                        /*_._IFELSE___V13*/ meltfptr[12] = /*_.RETURN___V14*/ meltfptr[13];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1210:/ clear");
                                                        /*clear*/ /*_.RETURN___V14*/ meltfptr[13] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        /*_#STRINGgt__L5*/ meltfnum[4] =
                                                            melt_string_less((melt_ptr_t)(/*_.SN2__V12*/ meltfptr[11]), (melt_ptr_t)(/*_.SN1__V11*/ meltfptr[10]));;
                                                        MELT_LOCATION("warmelt-base.melt:1212:/ cond");
                                                        /*cond*/ if (/*_#STRINGgt__L5*/ meltfnum[4]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1213:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_5*/ meltfrout->tabval[5]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1213:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                    /*_._IFELSE___V15*/ meltfptr[13] = /*_.RETURN___V16*/ meltfptr[15];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1212:/ clear");
                                                                    /*clear*/ /*_.RETURN___V16*/ meltfptr[15] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        else    /*^cond.else*/
                                                            {

                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1215:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] = (/*!konst_6*/ meltfrout->tabval[6]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1215:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                    MELT_LOCATION("warmelt-base.melt:1214:/ quasiblock");


                                                                    /*_.PROGN___V18*/ meltfptr[17] = /*_.RETURN___V17*/ meltfptr[15];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V15*/ meltfptr[13] = /*_.PROGN___V18*/ meltfptr[17];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1212:/ clear");
                                                                    /*clear*/ /*_.RETURN___V17*/ meltfptr[15] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V18*/ meltfptr[17] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V13*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[13];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1210:/ clear");
                                                        /*clear*/ /*_#STRINGgt__L5*/ meltfnum[4] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V15*/ meltfptr[13] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.LET___V10*/ meltfptr[8] = /*_._IFELSE___V13*/ meltfptr[12];;

                                            MELT_LOCATION("warmelt-base.melt:1207:/ clear");
                                            /*clear*/ /*_.SN1__V11*/ meltfptr[10] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.SN2__V12*/ meltfptr[11] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#STRINGlt__L4*/ meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V13*/ meltfptr[12] = 0 ;
                                            MELT_LOCATION("warmelt-base.melt:1206:/ quasiblock");


                                            /*_.PROGN___V19*/ meltfptr[15] = /*_.LET___V10*/ meltfptr[8];;
                                            /*^compute*/
                                            /*_._IFELSE___V8*/ meltfptr[6] = /*_.PROGN___V19*/ meltfptr[15];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1204:/ clear");
                                            /*clear*/ /*_.LET___V10*/ meltfptr[8] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V19*/ meltfptr[15] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V6*/ meltfptr[4] = /*_._IFELSE___V8*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1202:/ clear");
                                /*clear*/ /*_#IS_NOT_A__L3*/ meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V8*/ meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_._IFELSE___V6*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1200:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L2*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V6*/ meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1196:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:1196:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_#eqeq__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("COMPARE_NAMED_ALPHA", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                  const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_ADD2OUT_ix = 0, variad_ADD2OUT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_ADD2OUT_len)
#define melt_variadic_index variad_ADD2OUT_ix

    long current_blocklevel_signals_meltrout_14_WARMELTmiBASE_ADD2OUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1715 */

    /** start of frame for meltrout_14_WARMELTmiBASE_ADD2OUT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT// fromline 1531
        : public Melt_CallFrameWithValues<32>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[5];
        /*classy others*/
        const char* loc_CSTRING__o0;
        tree loc_TREE__o1;
        gimple loc_GIMPLE__o2;
        gimple_seq loc_GIMPLE_SEQ__o3;
        edge loc_EDGE__o4;
        loop_p loc_LOOP__o5;
        const char* loc_CSTRING__o6;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
            /*marking tree*/
            gt_ggc_mx_tree_node (loc_TREE__o1);
            /*marking gimple*/
            gt_ggc_mx_gimple_statement_d (loc_GIMPLE__o2);
            /*marking gimple_seq*/
            gt_ggc_mx_gimple_seq_d (loc_GIMPLE_SEQ__o3);
            /*marking edge*/
            gt_ggc_mx_edge_def (loc_EDGE__o4);
            /*marking loop_p*/
            gt_ggc_mx_loop (loc_LOOP__o5);
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT), clos) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT() //the constructor fromline 1605
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT)) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT


    /** end of frame for meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1660**/

    /* end of frame for routine meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1719 */

    /* classy proc frame meltrout_14_WARMELTmiBASE_ADD2OUT */ MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1511:/ getarg");
    /*_.OUT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_OUT__L1*/ meltfnum[0] =
            (melt_is_out ((melt_ptr_t) /*_.OUT__V2*/ meltfptr[1])) ;;
        /*^compute*/
        /*_#NOT__L2*/ meltfnum[1] =
            (!(/*_#IS_OUT__L1*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-base.melt:1515:/ cond");
        /*cond*/ if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1516:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1516:/ locexp");
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
                    /*finalret*/ goto meltlabend_rout ;
                    /*_._IF___V3*/ meltfptr[2] = /*_.RETURN___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1515:/ clear");
                    /*clear*/ /*_.RETURN___V4*/ meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1517:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_1:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1519:/ cond");
                /*cond*/ if (/*ifvariadic nomore*/ variad_ADD2OUT_ix == variad_ADD2OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_ADD2OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1521:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_1;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1519:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            /*^cond*/
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V7*/ meltfptr[6] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/ variad_ADD2OUT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*_#IS_CLOSURE__L3*/ meltfnum[2] =
                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V7*/ meltfptr[6])) == MELTOBMAG_CLOSURE);;
                                        MELT_LOCATION("warmelt-base.melt:1523:/ cond");
                                        /*cond*/ if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1524:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.VV__V9*/ meltfptr[8] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/ variad_ADD2OUT_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:1526:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[1];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VV__V9*/ meltfptr[8];
                                                                    /*_.V__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V10*/ meltfptr[9];;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                /*clear*/ /*_.VV__V9*/ meltfptr[8] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.V__V10*/ meltfptr[9] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^cond*/
                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_LONG) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^compute*/
                                                                            /*_#LL__L4*/ meltfnum[3] =
                                                                                /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_long;;
                                                                            /*^compute*/

                                                                            /*consume variadic LONG !*/ variad_ADD2OUT_ix += 1;;

                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            MELT_LOCATION("warmelt-base.melt:1528:/ apply");
                                                                            /*apply*/
                                                                            {
                                                                                union meltparam_un argtab[1];
                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                /*^apply.arg*/
                                                                                argtab[0].meltbp_long = /*_#LL__L4*/ meltfnum[3];
                                                                                /*_.V__V11*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                            }
                                                                            ;
                                                                            /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V11*/ meltfptr[8];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                            /*clear*/ /*_#LL__L4*/ meltfnum[3] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.V__V11*/ meltfptr[8] = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^cond*/
                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^compute*/
                                                                                        /*_?*/ meltfram__.loc_CSTRING__o0 =
                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_cstring;;
                                                                                        /*^compute*/

                                                                                        /*consume variadic CSTRING !*/ variad_ADD2OUT_ix += 1;;

                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        MELT_LOCATION("warmelt-base.melt:1530:/ apply");
                                                                                        /*apply*/
                                                                                        {
                                                                                            union meltparam_un argtab[1];
                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                            /*^apply.arg*/
                                                                                            argtab[0].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
                                                                                            /*_.V__V12*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                        }
                                                                                        ;
                                                                                        /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V12*/ meltfptr[9];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                        /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o0 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.V__V12*/ meltfptr[9] = 0 ;
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            else    /*^cond.else*/
                                                                                {

                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^cond*/
                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_TREE) /*then*/
                                                                                            {
                                                                                                /*^cond.then*/
                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^compute*/
                                                                                                    /*_?*/ meltfram__.loc_TREE__o1 =
                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_tree;;
                                                                                                    /*^compute*/

                                                                                                    /*consume variadic TREE !*/ variad_ADD2OUT_ix += 1;;

                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    MELT_LOCATION("warmelt-base.melt:1532:/ apply");
                                                                                                    /*apply*/
                                                                                                    {
                                                                                                        union meltparam_un argtab[1];
                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                        /*^apply.arg*/
                                                                                                        argtab[0].meltbp_tree = /*_?*/ meltfram__.loc_TREE__o1;
                                                                                                        /*_.V__V13*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un*)0);
                                                                                                    }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V13*/ meltfptr[8];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_TREE__o1 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.V__V13*/ meltfptr[8] = 0 ;
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        else    /*^cond.else*/
                                                                                            {

                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^cond*/
                                                                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_GIMPLE) /*then*/
                                                                                                        {
                                                                                                            /*^cond.then*/
                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                /*^compute*/
                                                                                                                /*_?*/ meltfram__.loc_GIMPLE__o2 =
                                                                                                                    /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_gimple;;
                                                                                                                /*^compute*/

                                                                                                                /*consume variadic GIMPLE !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                MELT_LOCATION("warmelt-base.melt:1534:/ apply");
                                                                                                                /*apply*/
                                                                                                                {
                                                                                                                    union meltparam_un argtab[1];
                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                    /*^apply.arg*/
                                                                                                                    argtab[0].meltbp_gimple = /*_?*/ meltfram__.loc_GIMPLE__o2;
                                                                                                                    /*_.V__V14*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V14*/ meltfptr[9];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                                                /*clear*/ /*_?*/ meltfram__.loc_GIMPLE__o2 = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_.V__V14*/ meltfptr[9] = 0 ;
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    else    /*^cond.else*/
                                                                                                        {

                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                /*^cond*/
                                                                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_GIMPLESEQ) /*then*/
                                                                                                                    {
                                                                                                                        /*^cond.then*/
                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*^compute*/
                                                                                                                            /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3 =
                                                                                                                                /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_gimpleseq;;
                                                                                                                            /*^compute*/

                                                                                                                            /*consume variadic GIMPLE_SEQ !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1536:/ apply");
                                                                                                                            /*apply*/
                                                                                                                            {
                                                                                                                                union meltparam_un argtab[1];
                                                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                                                /*^apply.arg*/
                                                                                                                                argtab[0].meltbp_gimpleseq = /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3;
                                                                                                                                /*_.V__V15*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un*)0);
                                                                                                                            }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V15*/ meltfptr[8];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                                                            /*clear*/ /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3 = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_.V__V15*/ meltfptr[8] = 0 ;
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                else    /*^cond.else*/
                                                                                                                    {

                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*^cond*/
                                                                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_EDGE) /*then*/
                                                                                                                                {
                                                                                                                                    /*^cond.then*/
                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*^compute*/
                                                                                                                                        /*_?*/ meltfram__.loc_EDGE__o4 =
                                                                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_edge;;
                                                                                                                                        /*^compute*/

                                                                                                                                        /*consume variadic EDGE !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1538:/ apply");
                                                                                                                                        /*apply*/
                                                                                                                                        {
                                                                                                                                            union meltparam_un argtab[1];
                                                                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                                                                            /*^apply.arg*/
                                                                                                                                            argtab[0].meltbp_edge = /*_?*/ meltfram__.loc_EDGE__o4;
                                                                                                                                            /*_.V__V16*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V16*/ meltfptr[9];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                                                                        /*clear*/ /*_?*/ meltfram__.loc_EDGE__o4 = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_.V__V16*/ meltfptr[9] = 0 ;
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            else    /*^cond.else*/
                                                                                                                                {

                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*^cond*/
                                                                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_LOOP) /*then*/
                                                                                                                                            {
                                                                                                                                                /*^cond.then*/
                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    /*^compute*/
                                                                                                                                                    /*_?*/ meltfram__.loc_LOOP__o5 =
                                                                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_loop;;
                                                                                                                                                    /*^compute*/

                                                                                                                                                    /*consume variadic LOOP !*/ variad_ADD2OUT_ix += 1;;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1540:/ apply");
                                                                                                                                                    /*apply*/
                                                                                                                                                    {
                                                                                                                                                        union meltparam_un argtab[1];
                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                        argtab[0].meltbp_loop = /*_?*/ meltfram__.loc_LOOP__o5;
                                                                                                                                                        /*_.V__V17*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.V__V17*/ meltfptr[8];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_LOOP__o5 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.V__V17*/ meltfptr[8] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else    /*^cond.else*/
                                                                                                                                            {

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1542:/ quasiblock");


                                                                                                                                                    /*_.VCTY__V19*/ meltfptr[8] =
                                                                                                                                                        /*variadic_type_code*/
#ifdef melt_variadic_index
                                                                                                                                                        (((melt_variadic_index + 0) >= 0
                                                                                                                                                          && (melt_variadic_index + 0) < melt_variadic_length)
                                                                                                                                                         ?  melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0]
                                                                                                                                                                                & MELT_ARGDESCR_MAX)
                                                                                                                                                         : NULL)
#else
                                                                                                                                                        NULL /* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index*/
                                                                                                                                                        ;;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1545:/ cond");
                                                                                                                                                    /*cond*/ if (
                                                                                                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V19*/ meltfptr[8]),
                                                                                                                                                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                                                                                                                                    ) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^getslot*/
                                                                                                                                                            {
                                                                                                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                                                                                                obj = (melt_ptr_t)(/*_.VCTY__V19*/ meltfptr[8]) /*=obj*/;
                                                                                                                                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                                                                                /*_.NAMED_NAME__V20*/ meltfptr[19] = slot;
                                                                                                                                                            };
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_.NAMED_NAME__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1544:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V20*/ meltfptr[19])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1546:/ cppif.then");
                                                                                                                                                    /*^block*/
                                                                                                                                                    /*anyblock*/
                                                                                                                                                    {


                                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                                        ;
                                                                                                                                                        /*^cond*/
                                                                                                                                                        /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                                                                                                            {
                                                                                                                                                                /*^cond.then*/
                                                                                                                                                                /*_._IFELSE___V22*/ meltfptr[21] = (/*nil*/NULL);;
                                                                                                                                                            }
                                                                                                                                                        else
                                                                                                                                                            {
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1546:/ cond.else");

                                                                                                                                                                /*^block*/
                                                                                                                                                                /*anyblock*/
                                                                                                                                                                {


                                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                                    ;
                                                                                                                                                                    /*^apply*/
                                                                                                                                                                    /*apply*/
                                                                                                                                                                    {
                                                                                                                                                                        union meltparam_un argtab[4];
                                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[0].meltbp_cstring =  "invalid variadic argument after closure to ADD2OUT";
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[2].meltbp_long = 1546;
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V19*/ meltfptr[8];
                                                                                                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*_._IFELSE___V22*/ meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1546:/ clear");
                                                                                                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22] = 0 ;
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*_.IFCPP___V21*/ meltfptr[20] = /*_._IFELSE___V22*/ meltfptr[21];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1546:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V22*/ meltfptr[21] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V21*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1547:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1547:/ locexp");
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
                                                                                                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                                                                                                    /*_.LET___V18*/ meltfptr[9] = /*_.RETURN___V24*/ meltfptr[22];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1542:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V19*/ meltfptr[8] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V20*/ meltfptr[19] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V21*/ meltfptr[20] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V24*/ meltfptr[22] = 0 ;
                                                                                                                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.LET___V18*/ meltfptr[9];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1524:/ clear");
                                                                                                                                                    /*clear*/ /*_.LET___V18*/ meltfptr[9] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        ;
                                                                                                                                        /*epilog*/
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            ;
                                                                                                                            /*epilog*/
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                ;
                                                                                                                /*epilog*/
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    ;
                                                                                                    /*epilog*/
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        ;
                                                                                        /*epilog*/
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            ;
                                                                            /*epilog*/
                                                                        }
                                                                        ;
                                                                    }
                                                                ;
                                                                /*epilog*/
                                                            }
                                                            ;
                                                        }
                                                    ;
                                                    /*epilog*/
                                                }
                                                ;
                                            }
                                        else
                                            {
                                                MELT_LOCATION("warmelt-base.melt:1523:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1549:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V2*/ meltfptr[1];
                                                        /*_.ADD_TO_OUT__V25*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V8*/ meltfptr[7] = /*_.ADD_TO_OUT__V25*/ meltfptr[21];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1523:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V25*/ meltfptr[21] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1519:/ clear");
                                        /*clear*/ /*_.V__V7*/ meltfptr[6] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#IS_CLOSURE__L3*/ meltfnum[2] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_._IFELSE___V8*/ meltfptr[7] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^cond*/
                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_LONG) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^compute*/
                                                    /*_#N__L5*/ meltfnum[3] =
                                                        /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_long;;
                                                    /*^compute*/

                                                    /*consume variadic LONG !*/ variad_ADD2OUT_ix += 1;;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1551:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1519:/ clear");
                                                    /*clear*/ /*_#N__L5*/ meltfnum[3] = 0 ;
                                                }
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^cond*/
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_?*/ meltfram__.loc_CSTRING__o6 =
                                                                    /*variadic argument stuff*/ meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_cstring;;
                                                                /*^compute*/

                                                                /*consume variadic CSTRING !*/ variad_ADD2OUT_ix += 1;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1553:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1519:/ clear");
                                                                /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o6 = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1555:/ quasiblock");


                                                                /*_.VCTY__V27*/ meltfptr[19] =
                                                                    /*variadic_type_code*/
#ifdef melt_variadic_index
                                                                    (((melt_variadic_index + 0) >= 0
                                                                      && (melt_variadic_index + 0) < melt_variadic_length)
                                                                     ?  melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0]
                                                                                            & MELT_ARGDESCR_MAX)
                                                                     : NULL)
#else
                                                                    NULL /* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index*/
                                                                    ;;
                                                                MELT_LOCATION("warmelt-base.melt:1558:/ cond");
                                                                /*cond*/ if (
                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V27*/ meltfptr[19]),
                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                                                ) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^getslot*/
                                                                        {
                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                            obj = (melt_ptr_t)(/*_.VCTY__V27*/ meltfptr[19]) /*=obj*/;
                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                            /*_.NAMED_NAME__V28*/ meltfptr[20] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V28*/ meltfptr[20] =  /*reallynil*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1557:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V28*/ meltfptr[20])));
                                                                }
                                                                ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1559:/ cppif.then");
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*^cond*/
                                                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*_._IFELSE___V30*/ meltfptr[9] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1559:/ cond.else");

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                /*^apply*/
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[4];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "invalid variadic argument to ADD2OUT";
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                    /*^apply.arg*/
                                                                                    argtab[2].meltbp_long = 1559;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V27*/ meltfptr[19];
                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IFELSE___V30*/ meltfptr[9] = /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1559:/ clear");
                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_.IFCPP___V29*/ meltfptr[22] = /*_._IFELSE___V30*/ meltfptr[9];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1559:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V30*/ meltfptr[9] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V29*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;
                                                                /*^compute*/
                                                                /*_.LET___V26*/ meltfptr[8] = /*_.IFCPP___V29*/ meltfptr[22];;

                                                                MELT_LOCATION("warmelt-base.melt:1555:/ clear");
                                                                /*clear*/ /*_.VCTY__V27*/ meltfptr[19] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V28*/ meltfptr[20] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.IFCPP___V29*/ meltfptr[22] = 0 ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1519:/ clear");
                                                                /*clear*/ /*_.LET___V26*/ meltfptr[8] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    ;
                                                    /*epilog*/
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/
                                    }
                                    ;
                                }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                ;
                /*epilog*/
            }
            ;
            ;
            goto meltlabloop_ARGLOOP_1;
meltlabexit_ARGLOOP_1:
            ;
            MELT_LOCATION("warmelt-base.melt:1517:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V5*/ meltfptr[3] = /*_.ARGLOOP__V6*/ meltfptr[5];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1562:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.OUT__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:1562:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1511:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RETURN___V32*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-base.melt:1511:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_#IS_OUT__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V5*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V32*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_14_WARMELTmiBASE_ADD2OUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_14_WARMELTmiBASE_ADD2OUT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                     const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_STRING4OUT_ix = 0, variad_STRING4OUT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_STRING4OUT_len)
#define melt_variadic_index variad_STRING4OUT_ix

    long current_blocklevel_signals_meltrout_15_WARMELTmiBASE_STRING4OUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1715 */

    /** start of frame for meltrout_15_WARMELTmiBASE_STRING4OUT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT// fromline 1531
        : public Melt_CallFrameWithValues<44>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[5];
        /*classy others*/
        const char* loc_CSTRING__o0;
        tree loc_TREE__o1;
        gimple loc_GIMPLE__o2;
        gimple_seq loc_GIMPLE_SEQ__o3;
        edge loc_EDGE__o4;
        loop_p loc_LOOP__o5;
        const char* loc_CSTRING__o6;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
            /*marking tree*/
            gt_ggc_mx_tree_node (loc_TREE__o1);
            /*marking gimple*/
            gt_ggc_mx_gimple_statement_d (loc_GIMPLE__o2);
            /*marking gimple_seq*/
            gt_ggc_mx_gimple_seq_d (loc_GIMPLE_SEQ__o3);
            /*marking edge*/
            gt_ggc_mx_edge_def (loc_EDGE__o4);
            /*marking loop_p*/
            gt_ggc_mx_loop (loc_LOOP__o5);
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<44> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT), clos) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT() //the constructor fromline 1605
            : Melt_CallFrameWithValues<44> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<44> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT)) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<44> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT


    /** end of frame for meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1660**/

    /* end of frame for routine meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1719 */

    /* classy proc frame meltrout_15_WARMELTmiBASE_STRING4OUT */ MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("STRING4OUT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1566:/ getarg");
    /*_.DIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        MELT_LOCATION("warmelt-base.melt:1569:/ cond");
        /*cond*/ if (/*_.DIS__V2*/ meltfptr[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V3*/ meltfptr[2] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1569:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.DIS__V2*/ meltfptr[1] = /*_.SETQ___V4*/ meltfptr[3] = (/*!DISCR_STRING*/ meltfrout->tabval[0]);;
                    /*^quasiblock*/


                    /*_.PROGN___V5*/ meltfptr[4] = /*_.SETQ___V4*/ meltfptr[3];;
                    /*^compute*/
                    /*_._IFELSE___V3*/ meltfptr[2] = /*_.PROGN___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1569:/ clear");
                    /*clear*/ /*_.SETQ___V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V5*/ meltfptr[4] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L1*/ meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_DISCRIMINANT*/ meltfrout->tabval[1])));;
        MELT_LOCATION("warmelt-base.melt:1570:/ cond");
        /*cond*/ if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V6*/ meltfptr[3] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1570:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1571:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1571:/ locexp");
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
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-base.melt:1570:/ quasiblock");


                    /*_.PROGN___V8*/ meltfptr[7] = /*_.RETURN___V7*/ meltfptr[4];;
                    /*^compute*/
                    /*_._IFELSE___V6*/ meltfptr[3] = /*_.PROGN___V8*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1570:/ clear");
                    /*clear*/ /*_.RETURN___V7*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V8*/ meltfptr[7] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1572:/ quasiblock");


        /*_#GOODMAG__L2*/ meltfnum[1] = 0;;

        {
            MELT_LOCATION("warmelt-base.melt:1574:/ locexp");
            /* string4out GETMAGIDISCHK__1 */ /*_#GOODMAG__L2*/ meltfnum[1] =
                ((meltobject_ptr_t)/*_.DIS__V2*/ meltfptr[1])->meltobj_magic == MELTOBMAG_STRING ;;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1577:/ cond");
        /*cond*/ if (/*_#GOODMAG__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V10*/ meltfptr[7] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1577:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1577:/ locexp");
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
                    /*finalret*/ goto meltlabend_rout ;
                    /*^quasiblock*/


                    /*_.PROGN___V12*/ meltfptr[11] = /*_.RETURN___V11*/ meltfptr[10];;
                    /*^compute*/
                    /*_._IFELSE___V10*/ meltfptr[7] = /*_.PROGN___V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1577:/ clear");
                    /*clear*/ /*_.RETURN___V11*/ meltfptr[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V12*/ meltfptr[11] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V9*/ meltfptr[4] = /*_._IFELSE___V10*/ meltfptr[7];;

        MELT_LOCATION("warmelt-base.melt:1572:/ clear");
        /*clear*/ /*_#GOODMAG__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V10*/ meltfptr[7] = 0 ;
        MELT_LOCATION("warmelt-base.melt:1578:/ quasiblock");


        /*_.OUT__V14*/ meltfptr[11] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[2])), (const char*)0);;
        MELT_LOCATION("warmelt-base.melt:1580:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_2:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1582:/ cond");
                /*cond*/ if (/*ifvariadic nomore*/ variad_STRING4OUT_ix == variad_STRING4OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_STRING4OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1584:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_2;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1582:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            /*^cond*/
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V17*/ meltfptr[16] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/ variad_STRING4OUT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*_#IS_CLOSURE__L3*/ meltfnum[1] =
                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V17*/ meltfptr[16])) == MELTOBMAG_CLOSURE);;
                                        MELT_LOCATION("warmelt-base.melt:1586:/ cond");
                                        /*cond*/ if (/*_#IS_CLOSURE__L3*/ meltfnum[1]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1587:/ cond");
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.VV__V19*/ meltfptr[18] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/ variad_STRING4OUT_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:1589:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[1];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VV__V19*/ meltfptr[18];
                                                                    /*_.V__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V20*/ meltfptr[19];;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                /*clear*/ /*_.VV__V19*/ meltfptr[18] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.V__V20*/ meltfptr[19] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^cond*/
                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_LONG) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^compute*/
                                                                            /*_#LL__L4*/ meltfnum[3] =
                                                                                /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_long;;
                                                                            /*^compute*/

                                                                            /*consume variadic LONG !*/ variad_STRING4OUT_ix += 1;;

                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            MELT_LOCATION("warmelt-base.melt:1591:/ apply");
                                                                            /*apply*/
                                                                            {
                                                                                union meltparam_un argtab[1];
                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                /*^apply.arg*/
                                                                                argtab[0].meltbp_long = /*_#LL__L4*/ meltfnum[3];
                                                                                /*_.V__V21*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                            }
                                                                            ;
                                                                            /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V21*/ meltfptr[18];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                            /*clear*/ /*_#LL__L4*/ meltfnum[3] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.V__V21*/ meltfptr[18] = 0 ;
                                                                        }
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*^block*/
                                                                        /*anyblock*/
                                                                        {

                                                                            /*^cond*/
                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                                                {
                                                                                    /*^cond.then*/
                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^compute*/
                                                                                        /*_?*/ meltfram__.loc_CSTRING__o0 =
                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_cstring;;
                                                                                        /*^compute*/

                                                                                        /*consume variadic CSTRING !*/ variad_STRING4OUT_ix += 1;;

                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        MELT_LOCATION("warmelt-base.melt:1593:/ apply");
                                                                                        /*apply*/
                                                                                        {
                                                                                            union meltparam_un argtab[1];
                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                            /*^apply.arg*/
                                                                                            argtab[0].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
                                                                                            /*_.V__V22*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                        }
                                                                                        ;
                                                                                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V22*/ meltfptr[19];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                        /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o0 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.V__V22*/ meltfptr[19] = 0 ;
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            else    /*^cond.else*/
                                                                                {

                                                                                    /*^block*/
                                                                                    /*anyblock*/
                                                                                    {

                                                                                        /*^cond*/
                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_TREE) /*then*/
                                                                                            {
                                                                                                /*^cond.then*/
                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^compute*/
                                                                                                    /*_?*/ meltfram__.loc_TREE__o1 =
                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_tree;;
                                                                                                    /*^compute*/

                                                                                                    /*consume variadic TREE !*/ variad_STRING4OUT_ix += 1;;

                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    MELT_LOCATION("warmelt-base.melt:1595:/ apply");
                                                                                                    /*apply*/
                                                                                                    {
                                                                                                        union meltparam_un argtab[1];
                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                        /*^apply.arg*/
                                                                                                        argtab[0].meltbp_tree = /*_?*/ meltfram__.loc_TREE__o1;
                                                                                                        /*_.V__V23*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un*)0);
                                                                                                    }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V23*/ meltfptr[18];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_TREE__o1 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.V__V23*/ meltfptr[18] = 0 ;
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        else    /*^cond.else*/
                                                                                            {

                                                                                                /*^block*/
                                                                                                /*anyblock*/
                                                                                                {

                                                                                                    /*^cond*/
                                                                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_GIMPLE) /*then*/
                                                                                                        {
                                                                                                            /*^cond.then*/
                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                /*^compute*/
                                                                                                                /*_?*/ meltfram__.loc_GIMPLE__o2 =
                                                                                                                    /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_gimple;;
                                                                                                                /*^compute*/

                                                                                                                /*consume variadic GIMPLE !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                MELT_LOCATION("warmelt-base.melt:1597:/ apply");
                                                                                                                /*apply*/
                                                                                                                {
                                                                                                                    union meltparam_un argtab[1];
                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                    /*^apply.arg*/
                                                                                                                    argtab[0].meltbp_gimple = /*_?*/ meltfram__.loc_GIMPLE__o2;
                                                                                                                    /*_.V__V24*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V24*/ meltfptr[19];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                                /*clear*/ /*_?*/ meltfram__.loc_GIMPLE__o2 = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_.V__V24*/ meltfptr[19] = 0 ;
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    else    /*^cond.else*/
                                                                                                        {

                                                                                                            /*^block*/
                                                                                                            /*anyblock*/
                                                                                                            {

                                                                                                                /*^cond*/
                                                                                                                /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_GIMPLESEQ) /*then*/
                                                                                                                    {
                                                                                                                        /*^cond.then*/
                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*^compute*/
                                                                                                                            /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3 =
                                                                                                                                /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_gimpleseq;;
                                                                                                                            /*^compute*/

                                                                                                                            /*consume variadic GIMPLE_SEQ !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1599:/ apply");
                                                                                                                            /*apply*/
                                                                                                                            {
                                                                                                                                union meltparam_un argtab[1];
                                                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                                                /*^apply.arg*/
                                                                                                                                argtab[0].meltbp_gimpleseq = /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3;
                                                                                                                                /*_.V__V25*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un*)0);
                                                                                                                            }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V25*/ meltfptr[18];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                                            /*clear*/ /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3 = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_.V__V25*/ meltfptr[18] = 0 ;
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                else    /*^cond.else*/
                                                                                                                    {

                                                                                                                        /*^block*/
                                                                                                                        /*anyblock*/
                                                                                                                        {

                                                                                                                            /*^cond*/
                                                                                                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_EDGE) /*then*/
                                                                                                                                {
                                                                                                                                    /*^cond.then*/
                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*^compute*/
                                                                                                                                        /*_?*/ meltfram__.loc_EDGE__o4 =
                                                                                                                                            /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_edge;;
                                                                                                                                        /*^compute*/

                                                                                                                                        /*consume variadic EDGE !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1601:/ apply");
                                                                                                                                        /*apply*/
                                                                                                                                        {
                                                                                                                                            union meltparam_un argtab[1];
                                                                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                                                                            /*^apply.arg*/
                                                                                                                                            argtab[0].meltbp_edge = /*_?*/ meltfram__.loc_EDGE__o4;
                                                                                                                                            /*_.V__V26*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V26*/ meltfptr[19];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                                                        /*clear*/ /*_?*/ meltfram__.loc_EDGE__o4 = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_.V__V26*/ meltfptr[19] = 0 ;
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            else    /*^cond.else*/
                                                                                                                                {

                                                                                                                                    /*^block*/
                                                                                                                                    /*anyblock*/
                                                                                                                                    {

                                                                                                                                        /*^cond*/
                                                                                                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_LOOP) /*then*/
                                                                                                                                            {
                                                                                                                                                /*^cond.then*/
                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    /*^compute*/
                                                                                                                                                    /*_?*/ meltfram__.loc_LOOP__o5 =
                                                                                                                                                        /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_loop;;
                                                                                                                                                    /*^compute*/

                                                                                                                                                    /*consume variadic LOOP !*/ variad_STRING4OUT_ix += 1;;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1603:/ apply");
                                                                                                                                                    /*apply*/
                                                                                                                                                    {
                                                                                                                                                        union meltparam_un argtab[1];
                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                        argtab[0].meltbp_loop = /*_?*/ meltfram__.loc_LOOP__o5;
                                                                                                                                                        /*_.V__V27*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.V__V27*/ meltfptr[18];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                                                                    /*clear*/ /*_?*/ meltfram__.loc_LOOP__o5 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.V__V27*/ meltfptr[18] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else    /*^cond.else*/
                                                                                                                                            {

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1605:/ quasiblock");


                                                                                                                                                    /*_.VCTY__V29*/ meltfptr[18] =
                                                                                                                                                        /*variadic_type_code*/
#ifdef melt_variadic_index
                                                                                                                                                        (((melt_variadic_index + 0) >= 0
                                                                                                                                                          && (melt_variadic_index + 0) < melt_variadic_length)
                                                                                                                                                         ?  melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0]
                                                                                                                                                                                & MELT_ARGDESCR_MAX)
                                                                                                                                                         : NULL)
#else
                                                                                                                                                        NULL /* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index*/
                                                                                                                                                        ;;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1608:/ cond");
                                                                                                                                                    /*cond*/ if (
                                                                                                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V29*/ meltfptr[18]),
                                                                                                                                                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
                                                                                                                                                    ) /*then*/
                                                                                                                                                        {
                                                                                                                                                            /*^cond.then*/
                                                                                                                                                            /*^getslot*/
                                                                                                                                                            {
                                                                                                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                                                                                                obj = (melt_ptr_t)(/*_.VCTY__V29*/ meltfptr[18]) /*=obj*/;
                                                                                                                                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                                                                                /*_.NAMED_NAME__V30*/ meltfptr[29] = slot;
                                                                                                                                                            };
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_.NAMED_NAME__V30*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1607:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V30*/ meltfptr[29])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1609:/ cppif.then");
                                                                                                                                                    /*^block*/
                                                                                                                                                    /*anyblock*/
                                                                                                                                                    {


                                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                                        ;
                                                                                                                                                        /*^cond*/
                                                                                                                                                        /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                                                                                                            {
                                                                                                                                                                /*^cond.then*/
                                                                                                                                                                /*_._IFELSE___V32*/ meltfptr[31] = (/*nil*/NULL);;
                                                                                                                                                            }
                                                                                                                                                        else
                                                                                                                                                            {
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1609:/ cond.else");

                                                                                                                                                                /*^block*/
                                                                                                                                                                /*anyblock*/
                                                                                                                                                                {


                                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                                    ;
                                                                                                                                                                    /*^apply*/
                                                                                                                                                                    /*apply*/
                                                                                                                                                                    {
                                                                                                                                                                        union meltparam_un argtab[4];
                                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[0].meltbp_cstring =  "invalid variadic argument after closure to STRING4OUT";
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[2].meltbp_long = 1609;
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V29*/ meltfptr[18];
                                                                                                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*_._IFELSE___V32*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1609:/ clear");
                                                                                                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] = 0 ;
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*_.IFCPP___V31*/ meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1609:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V32*/ meltfptr[31] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1610:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1610:/ locexp");
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
                                                                                                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                                                                                                    /*_.LET___V28*/ meltfptr[19] = /*_.RETURN___V34*/ meltfptr[32];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1605:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V29*/ meltfptr[18] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V30*/ meltfptr[29] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V31*/ meltfptr[30] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V34*/ meltfptr[32] = 0 ;
                                                                                                                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.LET___V28*/ meltfptr[19];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1587:/ clear");
                                                                                                                                                    /*clear*/ /*_.LET___V28*/ meltfptr[19] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        ;
                                                                                                                                        /*epilog*/
                                                                                                                                    }
                                                                                                                                    ;
                                                                                                                                }
                                                                                                                            ;
                                                                                                                            /*epilog*/
                                                                                                                        }
                                                                                                                        ;
                                                                                                                    }
                                                                                                                ;
                                                                                                                /*epilog*/
                                                                                                            }
                                                                                                            ;
                                                                                                        }
                                                                                                    ;
                                                                                                    /*epilog*/
                                                                                                }
                                                                                                ;
                                                                                            }
                                                                                        ;
                                                                                        /*epilog*/
                                                                                    }
                                                                                    ;
                                                                                }
                                                                            ;
                                                                            /*epilog*/
                                                                        }
                                                                        ;
                                                                    }
                                                                ;
                                                                /*epilog*/
                                                            }
                                                            ;
                                                        }
                                                    ;
                                                    /*epilog*/
                                                }
                                                ;
                                            }
                                        else
                                            {
                                                MELT_LOCATION("warmelt-base.melt:1586:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1612:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V14*/ meltfptr[11];
                                                        /*_.ADD_TO_OUT__V35*/ meltfptr[31] = meltgc_send((melt_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V18*/ meltfptr[17] = /*_.ADD_TO_OUT__V35*/ meltfptr[31];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1586:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V35*/ meltfptr[31] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1582:/ clear");
                                        /*clear*/ /*_.V__V17*/ meltfptr[16] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#IS_CLOSURE__L3*/ meltfnum[1] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^cond*/
                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_LONG) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^compute*/
                                                    /*_#N__L5*/ meltfnum[3] =
                                                        /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_long;;
                                                    /*^compute*/

                                                    /*consume variadic LONG !*/ variad_STRING4OUT_ix += 1;;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1614:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1582:/ clear");
                                                    /*clear*/ /*_#N__L5*/ meltfnum[3] = 0 ;
                                                }
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    /*^cond*/
                                                    /*cond*/ if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_CSTRING) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_?*/ meltfram__.loc_CSTRING__o6 =
                                                                    /*variadic argument stuff*/ meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_cstring;;
                                                                /*^compute*/

                                                                /*consume variadic CSTRING !*/ variad_STRING4OUT_ix += 1;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1616:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1582:/ clear");
                                                                /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o6 = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1618:/ quasiblock");


                                                                /*_.VCTY__V37*/ meltfptr[29] =
                                                                    /*variadic_type_code*/
#ifdef melt_variadic_index
                                                                    (((melt_variadic_index + 0) >= 0
                                                                      && (melt_variadic_index + 0) < melt_variadic_length)
                                                                     ?  melt_code_to_ctype (meltxargdescr_[melt_variadic_index + 0]
                                                                                            & MELT_ARGDESCR_MAX)
                                                                     : NULL)
#else
                                                                    NULL /* no variadic_ctype outside of variadic functions */
#endif /*melt_variadic_index*/
                                                                    ;;
                                                                MELT_LOCATION("warmelt-base.melt:1621:/ cond");
                                                                /*cond*/ if (
                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V37*/ meltfptr[29]),
                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
                                                                ) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^getslot*/
                                                                        {
                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                            obj = (melt_ptr_t)(/*_.VCTY__V37*/ meltfptr[29]) /*=obj*/;
                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                            /*_.NAMED_NAME__V38*/ meltfptr[30] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V38*/ meltfptr[30] =  /*reallynil*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1620:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V38*/ meltfptr[30])));
                                                                }
                                                                ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1622:/ cppif.then");
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*^cond*/
                                                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*_._IFELSE___V40*/ meltfptr[19] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1622:/ cond.else");

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                /*^apply*/
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[4];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "invalid variadic argument to STRING4OUT";
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                                                    /*^apply.arg*/
                                                                                    argtab[2].meltbp_long = 1622;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V37*/ meltfptr[29];
                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IFELSE___V40*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1622:/ clear");
                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_.IFCPP___V39*/ meltfptr[32] = /*_._IFELSE___V40*/ meltfptr[19];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1622:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V40*/ meltfptr[19] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V39*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;
                                                                /*^compute*/
                                                                /*_.LET___V36*/ meltfptr[18] = /*_.IFCPP___V39*/ meltfptr[32];;

                                                                MELT_LOCATION("warmelt-base.melt:1618:/ clear");
                                                                /*clear*/ /*_.VCTY__V37*/ meltfptr[29] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V38*/ meltfptr[30] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.IFCPP___V39*/ meltfptr[32] = 0 ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1582:/ clear");
                                                                /*clear*/ /*_.LET___V36*/ meltfptr[18] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    ;
                                                    /*epilog*/
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/
                                    }
                                    ;
                                }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                ;
                /*epilog*/
            }
            ;
            ;
            goto meltlabloop_ARGLOOP_2;
meltlabexit_ARGLOOP_2:
            ;
            MELT_LOCATION("warmelt-base.melt:1580:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V15*/ meltfptr[7] = /*_.ARGLOOP__V16*/ meltfptr[15];;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:1625:/ quasiblock");


        /*_.STR__V43*/ meltfptr[17] =
            (meltgc_new_stringdup((meltobject_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), melt_strbuf_str((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]))));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1627:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.STR__V43*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-base.melt:1627:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*_.LET___V42*/ meltfptr[16] = /*_.RETURN___V44*/ meltfptr[31];;

        MELT_LOCATION("warmelt-base.melt:1625:/ clear");
        /*clear*/ /*_.STR__V43*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V44*/ meltfptr[31] = 0 ;
        /*_.LET___V13*/ meltfptr[10] = /*_.LET___V42*/ meltfptr[16];;

        MELT_LOCATION("warmelt-base.melt:1578:/ clear");
        /*clear*/ /*_.OUT__V14*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V15*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V42*/ meltfptr[16] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1566:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-base.melt:1566:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_._IFELSE___V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V6*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V9*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/ meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("STRING4OUT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_15_WARMELTmiBASE_STRING4OUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_15_WARMELTmiBASE_STRING4OUT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1715 */

    /** start of frame for meltrout_16_WARMELTmiBASE_ADD2OUT4NULL of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL// fromline 1531
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL() //the constructor fromline 1605
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL)) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL


    /** end of frame for meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1660**/

    /* end of frame for routine meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1719 */

    /* classy proc frame meltrout_16_WARMELTmiBASE_ADD2OUT4NULL */ MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4NULL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1633:/ getarg");
    /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1634:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#NULL__L1*/ meltfnum[0] =
                ((/*_.V__V2*/ meltfptr[1]) == NULL);;
            MELT_LOCATION("warmelt-base.melt:1634:/ cond");
            /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1634:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check null v";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1634;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1634:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1634:/ clear");
            /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1635:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*nil*"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1633:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4NULL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_16_WARMELTmiBASE_ADD2OUT4NULL*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1715 */

    /** start of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER// fromline 1531
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[2];
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
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER() //the constructor fromline 1605
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER


    /** end of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1660**/

    /* end of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1719 */

    /* classy proc frame meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER */ MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4INTEGER", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1638:/ getarg");
    /*_.VN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1639:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_INTEGERBOX__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
            MELT_LOCATION("warmelt-base.melt:1639:/ cond");
            /*cond*/ if (/*_#IS_INTEGERBOX__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1639:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check boxed number vn";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1639;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VN__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1639:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1639:/ clear");
            /*clear*/ /*_#IS_INTEGERBOX__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_#GET_INT__L2*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1640:/ locexp");
            meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#GET_INT__L2*/ meltfnum[0]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1638:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L2*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4INTEGER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1715 */

    /** start of frame for meltrout_18_WARMELTmiBASE_ADD2OUT4STRING of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING// fromline 1531
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING() //the constructor fromline 1605
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING)) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING


    /** end of frame for meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1660**/

    /* end of frame for routine meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1719 */

    /* classy proc frame meltrout_18_WARMELTmiBASE_ADD2OUT4STRING */ MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4STRING", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1643:/ getarg");
    /*_.VS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1644:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-base.melt:1644:/ cond");
            /*cond*/ if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1644:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check boxed string vs";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1644;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VS__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1644:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1644:/ clear");
            /*clear*/ /*_#IS_STRING__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1645:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1643:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4STRING", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_18_WARMELTmiBASE_ADD2OUT4STRING*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1715 */

    /** start of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF// fromline 1531
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF() //the constructor fromline 1605
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF


    /** end of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1660**/

    /* end of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1719 */

    /* classy proc frame meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF */ MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4STRBUF", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1648:/ getarg");
    /*_.VSBUF__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1649:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VSBUF__V2*/ meltfptr[1])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-base.melt:1649:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1649:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check strbuf vsbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1649;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VSBUF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1649:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1649:/ clear");
            /*clear*/ /*_#IS_STRBUF__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1650:/ locexp");
            meltgc_add_out ((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_strbuf_str ((melt_ptr_t) /*_.VSBUF__V2*/ meltfptr[1]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1648:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4STRBUF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1715 */

    /** start of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB// fromline 1531
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[3];
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
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB() //the constructor fromline 1605
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB


    /** end of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1660**/

    /* end of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1719 */

    /* classy proc frame meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB */ MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4CLONEDSYMB", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1653:/ getarg");
    /*_.VC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

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
        MELT_LOCATION("warmelt-base.melt:1654:/ block");
        /*anyblock*/
        {

            /*^objgoto*/
            /*objgoto*/ goto mtch1_0;
            ;

/*objlabel*/ mtch1_0:
            ;
            MELT_LOCATION("warmelt-base.melt:1655:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_1;
            meltlab_count_1++;
            debugeprintf("objlabel_1 mtch1_*CLASS_NORMTESTER_INSTANCE/1d3120d3. *CLASS_OBJLABELINSTR/35e27652. %ld", meltlab_count_1);
#endif

            ;
            /*^clear*/
            /*clear*/ /*_.NAMED_NAME__V4*/ meltfptr[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CSYM_URANK__V5*/ meltfptr[4] = 0 ;
            /*^cond*/
            /*cond*/ if (
                /*normtesterinst*/ (melt_is_instance_of((melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]),
                                    (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[0]))))) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*^getslot*/
                        {
                            melt_ptr_t slot=NULL, obj=NULL;
                            obj = (melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]) /*=obj*/;
                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                            /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
                        };
                        ;
                        /*^getslot*/
                        {
                            melt_ptr_t slot=NULL, obj=NULL;
                            obj = (melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]) /*=obj*/;
                            melt_object_get_field(slot,obj, 3, "CSYM_URANK");
                            /*_.CSYM_URANK__V5*/ meltfptr[4] = slot;
                        };
                        ;
                        MELT_LOCATION("warmelt-base.melt:1657:/ objgoto");
                        /*objgoto*/ goto mtch1_1;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1655:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1662:/ objgoto");
                        /*objgoto*/ goto mtch1_3;
                        ;
                    }
                    ;
                }
            ;

/*objlabel*/ mtch1_1:
            ;
            MELT_LOCATION("warmelt-base.melt:1657:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_2;
            meltlab_count_2++;
            debugeprintf("objlabel_2 mtch1_*CLASS_NORMTESTER_MATCHER/3a33b272. *CLASS_OBJLABELINSTR/39d38924. %ld", meltlab_count_2);
#endif

            ;
            /*^clear*/
            /*clear*/ /*_#ICT__L1*/ meltfnum[0] = 0 ;
            /*^cond*/
            /*cond*/ if (
                /* integerbox_of IBOXOF_mtch1__1 ?*/ /*_.CSYM_URANK__V5*/ meltfptr[4]
                && melt_magic_discr((melt_ptr_t) /*_.CSYM_URANK__V5*/ meltfptr[4]) == MELTOBMAG_INT ) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*^clear*/
                        /*clear*/ /*_#ICT__L1*/ meltfnum[0] = 0 ;

                        {
                            /*^locexp*/
                            /* integerbox_of IBOXOF_mtch1__1 !*/ /*_#ICT__L1*/ meltfnum[0] = ((struct meltint_st*)/*_.CSYM_URANK__V5*/ meltfptr[4])->val; ;
                        }
                        ;
                        MELT_LOCATION("warmelt-base.melt:1655:/ objgoto");
                        /*objgoto*/ goto mtch1_2;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1657:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1662:/ objgoto");
                        /*objgoto*/ goto mtch1_3;
                        ;
                    }
                    ;
                }
            ;

/*objlabel*/ mtch1_2:
            ;
            MELT_LOCATION("warmelt-base.melt:1655:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_3;
            meltlab_count_3++;
            debugeprintf("objlabel_3 mtch1_*CLASS_NORMTESTER_SUCCESS/102f3e. *CLASS_OBJLABELINSTR/31c2629c. %ld", meltlab_count_3);
#endif

            ;
            /*^quasiblock*/


            /*_.NVARNAM__V6*/ meltfptr[4] = /*_.NAMED_NAME__V4*/ meltfptr[3];;
            /*^compute*/
            /*_#NVARURANK__L2*/ meltfnum[1] = /*_#ICT__L1*/ meltfnum[0];;

#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1658:/ cppif.then");
            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_STRING__L3*/ meltfnum[2] =
                    (melt_magic_discr((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])) == MELTOBMAG_STRING);;
                MELT_LOCATION("warmelt-base.melt:1658:/ cond");
                /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[2]) /*then*/
                    {
                        /*^cond.then*/
                        /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1658:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {


                            MELT_CHECK_SIGNAL();
                            ;
                            /*^apply*/
                            /*apply*/
                            {
                                union meltparam_un argtab[4];
                                memset(&argtab, 0, sizeof(argtab));
                                /*^apply.arg*/
                                argtab[0].meltbp_cstring =  "check nvarnam";
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 1658;
                                /*^apply.arg*/
                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.NVARNAM__V6*/ meltfptr[4];
                                /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IFELSE___V8*/ meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-base.melt:1658:/ clear");
                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                        }
                        ;
                    }
                ;
                /*_.IFCPP___V7*/ meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1658:/ clear");
                /*clear*/ /*_#IS_STRING__L3*/ meltfnum[2] = 0 ;
                /*^clear*/
                /*clear*/ /*_._IFELSE___V8*/ meltfptr[7] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V7*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1659:/ locexp");
                meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                      melt_string_str((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1660:/ locexp");
                meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "__"));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1661:/ locexp");
                meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#NVARURANK__L2*/ meltfnum[1]));
            }
            ;

            MELT_CHECK_SIGNAL();
            ;

            MELT_LOCATION("warmelt-base.melt:1655:/ clear");
            /*clear*/ /*_.NVARNAM__V6*/ meltfptr[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NVARURANK__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V7*/ meltfptr[6] = 0 ;
            /*^objgoto*/
            /*objgoto*/ goto mtch1__end /*endmatch*/;
            ;

/*objlabel*/ mtch1_3:
            ;
            MELT_LOCATION("warmelt-base.melt:1662:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_4;
            meltlab_count_4++;
            debugeprintf("objlabel_4 mtch1_*CLASS_NORMTESTER_SUCCESS/15cd9343. *CLASS_OBJLABELINSTR/2836245b. %ld", meltlab_count_4);
#endif

            ;
            /*^quasiblock*/



#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1663:/ cppif.then");
            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                /*^cond*/
                /*cond*/ if ((/*nil*/NULL)) /*then*/
                    {
                        /*^cond.then*/
                        /*_._IFELSE___V11*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1663:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {


                            MELT_CHECK_SIGNAL();
                            ;
                            /*^apply*/
                            /*apply*/
                            {
                                union meltparam_un argtab[4];
                                memset(&argtab, 0, sizeof(argtab));
                                /*^apply.arg*/
                                argtab[0].meltbp_cstring =  "bad cloned symbol";
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 1663;
                                /*^apply.arg*/
                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VC__V2*/ meltfptr[1];
                                /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IFELSE___V11*/ meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-base.melt:1663:/ clear");
                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4] = 0 ;
                        }
                        ;
                    }
                ;
                /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1663:/ clear");
                /*clear*/ /*_._IFELSE___V11*/ meltfptr[7] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            MELT_LOCATION("warmelt-base.melt:1662:/ clear");
            /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
            /*^objgoto*/
            /*objgoto*/ goto mtch1__end /*endmatch*/;
            ;

/*objlabel*/ mtch1__end:
            ;
            MELT_LOCATION("warmelt-base.melt:1654:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_5;
            meltlab_count_5++;
            debugeprintf("objlabel_5 mtch1__end_h667464150 *CLASS_OBJLABELINSTR/27c8b1d6. %ld", meltlab_count_5);
#endif

            ;
        }
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4CLONEDSYMB", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1715 */

    /** start of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED// fromline 1531
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED() //the constructor fromline 1605
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED)) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED


    /** end of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1660**/

    /* end of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1719 */

    /* classy proc frame meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED */ MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4NAMED", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1666:/ getarg");
    /*_.VN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:1667:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.VN__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1667:/ locexp");
            meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V4*/ meltfptr[3])));
        }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1666:/ clear");
        /*clear*/ /*_.NAMED_NAME__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4NAMED", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1715 */

    /** start of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT// fromline 1531
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT() //the constructor fromline 1605
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT


    /** end of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1660**/

    /* end of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1719 */

    /* classy proc frame meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT */ MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4ROOTOBJECT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1670:/ getarg");
    /*_.VO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-base.melt:1671:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/ meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1672:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAMED_NAME__V5*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1672:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1673:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "/"));
        }
        ;
        /*_#OBJ_HASH__L1*/ meltfnum[0] =
            (melt_obj_hash((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1674:/ locexp");
            meltgc_add_out_hex((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#OBJ_HASH__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1675:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "."));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1670:/ clear");
        /*clear*/ /*_.DISCRIM__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OBJ_HASH__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4ROOTOBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1715 */

    /** start of frame for meltrout_23_WARMELTmiBASE_ADD2OUT4ANY of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY// fromline 1531
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY() //the constructor fromline 1605
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY)) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY


    /** end of frame for meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1660**/

    /* end of frame for routine meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1719 */

    /* classy proc frame meltrout_23_WARMELTmiBASE_ADD2OUT4ANY */ MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4ANY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1678:/ getarg");
    /*_.VA__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-base.melt:1679:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/ meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VA__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1680:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.DISCRIM__V4*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAMED_NAME__V5*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1680:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1678:/ clear");
        /*clear*/ /*_.DISCRIM__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4ANY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_23_WARMELTmiBASE_ADD2OUT4ANY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1715 */

    /** start of frame for meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY// fromline 1531
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[2];
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
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY() //the constructor fromline 1605
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY)) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY


    /** end of frame for meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1660**/

    /* end of frame for routine meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1719 */

    /* classy proc frame meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY */ MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPSTRING_EVERY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1739:/ getarg");
    /*_.MAP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MAPSTRING__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MAP__V2*/ meltfptr[1])) == MELTOBMAG_MAPSTRINGS);;
        MELT_LOCATION("warmelt-base.melt:1743:/ cond");
        /*cond*/ if (/*_#IS_MAPSTRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1744:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MAPSTRING*/
                                {
                                    /*foreach_in_mapstring meltcit1__EACHSTRMAP :*/ int meltcit1__EACHSTRMAP_ix=0, meltcit1__EACHSTRMAP_siz=0;
                                    for (meltcit1__EACHSTRMAP_ix=0;
                                            /* we retrieve in meltcit1__EACHSTRMAP_siz the size at each iteration since it could change. */
                                            meltcit1__EACHSTRMAP_ix>=0
                                            && (meltcit1__EACHSTRMAP_siz =
                                                    melt_size_mapstrings((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])) > 0
                                            && meltcit1__EACHSTRMAP_ix < meltcit1__EACHSTRMAP_siz;
                                            meltcit1__EACHSTRMAP_ix++)
                                        {
                                            const char* meltcit1__EACHSTRMAP_str = NULL;
                                            const char* meltcit1__EACHSTRMAP_nam = NULL;
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                            meltcit1__EACHSTRMAP_str =
                                                ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
                                            if (!meltcit1__EACHSTRMAP_str || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
                                                continue; /*foreach_in_mapstring meltcit1__EACHSTRMAP inside before*/
                                            /*_.CURVAL__V5*/ meltfptr[4] = ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
                                            if (!/*_.CURVAL__V5*/ meltfptr[4])
                                                continue;
                                            if (melt_is_instance_of((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4], (melt_ptr_t) MELT_PREDEF (CLASS_NAMED))
                                                    && (/*_.CURAT__V4*/ meltfptr[3] = melt_object_nth_field ((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4],
                                                                        MELTFIELD_NAMED_NAME)) != NULL
                                                    && (meltcit1__EACHSTRMAP_nam =
                                                            melt_string_str ((melt_ptr_t) /*_.CURAT__V4*/ meltfptr[3])) != (char*)0
                                                    && !strcmp (meltcit1__EACHSTRMAP_nam, meltcit1__EACHSTRMAP_str))
                                                /*_.CURAT__V4*/ meltfptr[3] = /*_.CURAT__V4*/ meltfptr[3];
                                            else
                                                {
                                                    /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                                    /*_.CURAT__V4*/ meltfptr[3] =
                                                        meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
                                                                              meltcit1__EACHSTRMAP_str);
                                                }
                                            meltcit1__EACHSTRMAP_str = (const char*)0;
                                            meltcit1__EACHSTRMAP_nam = (const char*)0;




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1748:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURVAL__V5*/ meltfptr[4];
                                                /*_.F__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.CURAT__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                        }

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1745:/ clear");
                                    /*clear*/ /*_.CURAT__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURVAL__V5*/ meltfptr[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V6*/ meltfptr[5] = 0 ;
                                } /*endciterblock FOREACH_IN_MAPSTRING*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1743:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1739:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MAPSTRING_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1715 */

    /** start of frame for meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST// fromline 1531
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[3];
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
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST() //the constructor fromline 1605
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST


    /** end of frame for meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1660**/

    /* end of frame for routine meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1719 */

    /* classy proc frame meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST */ MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPSTRING_ITERATE_TEST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1752:/ getarg");
    /*_.MAP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MAPSTRING__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MAP__V2*/ meltfptr[1])) == MELTOBMAG_MAPSTRINGS);;
        MELT_LOCATION("warmelt-base.melt:1757:/ cond");
        /*cond*/ if (/*_#IS_MAPSTRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1758:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MAPSTRING*/
                                {
                                    /*foreach_in_mapstring meltcit1__EACHSTRMAP :*/ int meltcit1__EACHSTRMAP_ix=0, meltcit1__EACHSTRMAP_siz=0;
                                    for (meltcit1__EACHSTRMAP_ix=0;
                                            /* we retrieve in meltcit1__EACHSTRMAP_siz the size at each iteration since it could change. */
                                            meltcit1__EACHSTRMAP_ix>=0
                                            && (meltcit1__EACHSTRMAP_siz =
                                                    melt_size_mapstrings((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])) > 0
                                            && meltcit1__EACHSTRMAP_ix < meltcit1__EACHSTRMAP_siz;
                                            meltcit1__EACHSTRMAP_ix++)
                                        {
                                            const char* meltcit1__EACHSTRMAP_str = NULL;
                                            const char* meltcit1__EACHSTRMAP_nam = NULL;
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                            meltcit1__EACHSTRMAP_str =
                                                ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
                                            if (!meltcit1__EACHSTRMAP_str || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
                                                continue; /*foreach_in_mapstring meltcit1__EACHSTRMAP inside before*/
                                            /*_.CURVAL__V5*/ meltfptr[4] = ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
                                            if (!/*_.CURVAL__V5*/ meltfptr[4])
                                                continue;
                                            if (melt_is_instance_of((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4], (melt_ptr_t) MELT_PREDEF (CLASS_NAMED))
                                                    && (/*_.CURAT__V4*/ meltfptr[3] = melt_object_nth_field ((melt_ptr_t) /*_.CURVAL__V5*/ meltfptr[4],
                                                                        MELTFIELD_NAMED_NAME)) != NULL
                                                    && (meltcit1__EACHSTRMAP_nam =
                                                            melt_string_str ((melt_ptr_t) /*_.CURAT__V4*/ meltfptr[3])) != (char*)0
                                                    && !strcmp (meltcit1__EACHSTRMAP_nam, meltcit1__EACHSTRMAP_str))
                                                /*_.CURAT__V4*/ meltfptr[3] = /*_.CURAT__V4*/ meltfptr[3];
                                            else
                                                {
                                                    /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                                    /*_.CURAT__V4*/ meltfptr[3] =
                                                        meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
                                                                              meltcit1__EACHSTRMAP_str);
                                                }
                                            meltcit1__EACHSTRMAP_str = (const char*)0;
                                            meltcit1__EACHSTRMAP_nam = (const char*)0;




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1762:/ quasiblock");


                                            /*^multiapply*/
                                            /*multiapply 2args, 1x.res*/
                                            {
                                                union meltparam_un argtab[1];

                                                union meltparam_un restab[1];
                                                memset(&restab, 0, sizeof(restab));
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^multiapply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURVAL__V5*/ meltfptr[4];
                                                /*^multiapply.xres*/
                                                restab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OTHER__V8*/ meltfptr[7];
                                                /*^multiapply.appl*/
                                                /*_.TEST__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.CURAT__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, (MELTBPARSTR_PTR ""), restab);
                                            }
                                            ;
                                            /*^quasiblock*/



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#NULL__L3*/ meltfnum[2] =
                                                ((/*_.TEST__V7*/ meltfptr[6]) == NULL);;
                                            MELT_LOCATION("warmelt-base.melt:1765:/ cond");
                                            /*cond*/ if (/*_#NULL__L3*/ meltfnum[2]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1766:/ quasiblock");


                                                        /*_._RETVAL___V1*/ meltfptr[0] = /*_.CURAT__V4*/ meltfptr[3];;
                                                        MELT_LOCATION("warmelt-base.melt:1766:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.CURVAL__V5*/ meltfptr[4]);
                                                        ;
                                                        /*^putxtraresult*/
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[1] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[1].meltbp_aptr) *(meltxrestab_[1].meltbp_aptr) = (melt_ptr_t) (/*_.OTHER__V8*/ meltfptr[7]);
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/ goto meltlabend_rout ;
                                                        /*_._IF___V9*/ meltfptr[8] = /*_.RETURN___V10*/ meltfptr[9];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1765:/ clear");
                                                        /*clear*/ /*_.RETURN___V10*/ meltfptr[9] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                                                }
                                            ;
                                            /*^compute*/
                                            /*_.MULTI___V6*/ meltfptr[5] = /*_._IF___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1762:/ clear");
                                            /*clear*/ /*_#NULL__L3*/ meltfnum[2] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IF___V9*/ meltfptr[8] = 0 ;

                                            /*^clear*/
                                            /*clear*/ /*_.OTHER__V8*/ meltfptr[7] = 0 ;
                                            /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
                                            /*_.CURAT__V4*/ meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/ meltfptr[4] = NULL;
                                        }

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1759:/ clear");
                                    /*clear*/ /*_.CURAT__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURVAL__V5*/ meltfptr[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MULTI___V6*/ meltfptr[5] = 0 ;
                                } /*endciterblock FOREACH_IN_MAPSTRING*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1757:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1752:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MAPSTRING_ITERATE_TEST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1715 */

    /** start of frame for meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY// fromline 1531
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[3];
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
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY() //the constructor fromline 1605
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY)) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY


    /** end of frame for meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1660**/

    /* end of frame for routine meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1719 */

    /* classy proc frame meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY */ MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_EVERY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1801:/ getarg");
    /*_.TUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1804:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1805:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MULTIPLE*/
                                {
                                    /* start foreach_in_multiple meltcit1__EACHTUP */
                                    long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.TUP__V2*/ meltfptr[1]);
                                    for (/*_#IX__L3*/ meltfnum[2] = 0;
                                                      (/*_#IX__L3*/ meltfnum[2] >= 0) && (/*_#IX__L3*/ meltfnum[2] <  meltcit1__EACHTUP_ln);
                                                      /*_#IX__L3*/ meltfnum[2]++)
                                        {
                                            /*_.COMP__V4*/ meltfptr[3] = melt_multiple_nth((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1]),  /*_#IX__L3*/ meltfnum[2]);




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1809:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
                                                /*_.F__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.COMP__V4*/ meltfptr[3]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            if (/*_#IX__L3*/ meltfnum[2]<0) break;
                                        } /* end  foreach_in_multiple meltcit1__EACHTUP */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1806:/ clear");
                                    /*clear*/ /*_.COMP__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L3*/ meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V5*/ meltfptr[4] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1804:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1801:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1715 */

    /** start of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY// fromline 1531
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[3];
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
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY() //the constructor fromline 1605
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY


    /** end of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1660**/

    /* end of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1719 */

    /* classy proc frame meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY */ MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1812:/ getarg");
    /*_.TUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) != NULL);

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
        /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1815:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1816:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MULTIPLE_BACKWARD*/
                                {
                                    /* start foreach_in_multiple_backward meltcit1__EACHTUPBACK */
                                    long  meltcit1__EACHTUPBACK_ln = melt_multiple_length((melt_ptr_t)/*_.TUP__V2*/ meltfptr[1]);
                                    long  meltcit1__EACHTUPBACK_ix = 0;
                                    for (meltcit1__EACHTUPBACK_ix = meltcit1__EACHTUPBACK_ln - 1;
                                            meltcit1__EACHTUPBACK_ix >= 0;
                                            meltcit1__EACHTUPBACK_ix--)
                                        {
                                            /*_.COMP__V4*/ meltfptr[3] = melt_multiple_nth((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1]),  meltcit1__EACHTUPBACK_ix);
                                            /*_#IX__L3*/ meltfnum[2]  =  meltcit1__EACHTUPBACK_ix;



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1820:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
                                                /*_.F__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.COMP__V4*/ meltfptr[3]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                        } /* end  foreach_in_multiple_backward meltcit1__EACHTUPBACK */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1817:/ clear");
                                    /*clear*/ /*_.COMP__V4*/ meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L3*/ meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V5*/ meltfptr[4] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE_BACKWARD*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1815:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1812:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1715 */

    /** start of frame for meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH// fromline 1531
        : public Melt_CallFrameWithValues<13>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[10];
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
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH() //the constructor fromline 1605
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH


    /** end of frame for meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1660**/

    /* end of frame for routine meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1719 */

    /* classy proc frame meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH */ MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_EVERY_BOTH", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1824:/ getarg");
    /*_.TUP1__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.TUP2__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.F__V4*/ meltfptr[3])) != NULL);

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
        /*_#IS_MULTIPLE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1827:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_MULTIPLE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-base.melt:1828:/ cond");
                    /*cond*/ if (/*_#IS_MULTIPLE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_CLOSURE__L3*/ meltfnum[2] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.F__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
                                MELT_LOCATION("warmelt-base.melt:1829:/ cond");
                                /*cond*/ if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1830:/ quasiblock");


                                            /*_#LN1__L4*/ meltfnum[3] =
                                                (melt_multiple_length((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1])));;
                                            /*^compute*/
                                            /*_#LN2__L5*/ meltfnum[4] =
                                                (melt_multiple_length((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])));;
                                            /*^compute*/
                                            /*_#IX__L6*/ meltfnum[5] = 0;;
                                            MELT_LOCATION("warmelt-base.melt:1833:/ loop");
                                            /*loop*/
                                            {
meltlabloop_TUPLOOP_1:;/*^loopbody*/

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*_#gteqI__L7*/ meltfnum[6] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN1__L4*/ meltfnum[3]));;
                                                    MELT_LOCATION("warmelt-base.melt:1834:/ cond");
                                                    /*cond*/ if (/*_#gteqI__L7*/ meltfnum[6]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^quasiblock*/


                                                                /*^compute*/
                                                                /*_.TUPLOOP__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;

                                                                /*^exit*/
                                                                /*exit*/
                                                                {
                                                                    goto meltlabexit_TUPLOOP_1;
                                                                }
                                                                ;
                                                                /*epilog*/
                                                            }
                                                            ;
                                                        } /*noelse*/
                                                    ;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*_#gteqI__L8*/ meltfnum[7] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN2__L5*/ meltfnum[4]));;
                                                    MELT_LOCATION("warmelt-base.melt:1835:/ cond");
                                                    /*cond*/ if (/*_#gteqI__L8*/ meltfnum[7]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^quasiblock*/


                                                                /*^compute*/
                                                                /*_.TUPLOOP__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;

                                                                /*^exit*/
                                                                /*exit*/
                                                                {
                                                                    goto meltlabexit_TUPLOOP_1;
                                                                }
                                                                ;
                                                                /*epilog*/
                                                            }
                                                            ;
                                                        } /*noelse*/
                                                    ;
                                                    /*_.MULTIPLE_NTH__V11*/ meltfptr[10] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1]), (/*_#IX__L6*/ meltfnum[5])));;
                                                    /*^compute*/
                                                    /*_.MULTIPLE_NTH__V12*/ meltfptr[11] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2]), (/*_#IX__L6*/ meltfnum[5])));;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1836:/ apply");
                                                    /*apply*/
                                                    {
                                                        union meltparam_un argtab[2];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^apply.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MULTIPLE_NTH__V12*/ meltfptr[11];
                                                        /*^apply.arg*/
                                                        argtab[1].meltbp_long = /*_#IX__L6*/ meltfnum[5];
                                                        /*_.F__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.MULTIPLE_NTH__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_#plI__L9*/ meltfnum[8] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) + (1));;
                                                    MELT_LOCATION("warmelt-base.melt:1837:/ compute");
                                                    /*_#IX__L6*/ meltfnum[5] = /*_#SETQ___L10*/ meltfnum[9] = /*_#plI__L9*/ meltfnum[8];;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1833:/ clear");
                                                    /*clear*/ /*_#gteqI__L7*/ meltfnum[6] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#gteqI__L8*/ meltfnum[7] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.MULTIPLE_NTH__V11*/ meltfptr[10] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.MULTIPLE_NTH__V12*/ meltfptr[11] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.F__V13*/ meltfptr[12] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#plI__L9*/ meltfnum[8] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#SETQ___L10*/ meltfnum[9] = 0 ;
                                                }
                                                ;
                                                ;
                                                goto meltlabloop_TUPLOOP_1;
meltlabexit_TUPLOOP_1:;/*^loopepilog*/
                                                /*loopepilog*/
                                                /*_.FOREVER___V9*/ meltfptr[8] = /*_.TUPLOOP__V10*/ meltfptr[9];;
                                            }
                                            ;
                                            /*^compute*/
                                            /*_.LET___V8*/ meltfptr[7] = /*_.FOREVER___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1830:/ clear");
                                            /*clear*/ /*_#LN1__L4*/ meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#LN2__L5*/ meltfnum[4] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#IX__L6*/ meltfnum[5] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.FOREVER___V9*/ meltfptr[8] = 0 ;
                                            /*_._IF___V7*/ meltfptr[6] = /*_.LET___V8*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1829:/ clear");
                                            /*clear*/ /*_.LET___V8*/ meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IF___V6*/ meltfptr[5] = /*_._IF___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1828:/ clear");
                                /*clear*/ /*_#IS_CLOSURE__L3*/ meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V7*/ meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_._IF___V5*/ meltfptr[4] = /*_._IF___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1827:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L2*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1824:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IF___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-base.melt:1824:/ locexp");
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
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_#IS_MULTIPLE__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_EVERY_BOTH", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH*/



/**** end of warmelt-base+01.cc ****/
