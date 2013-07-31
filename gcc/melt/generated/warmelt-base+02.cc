/* GCC MELT GENERATED C++ FILE warmelt-base+02.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #2 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f2[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-base+02.cc declarations ****/
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



class Melt_InitialClassyFrame_WARMELTmiBASE_h791191221; // forward declaration fromline 6518
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h791191221 Melt_InitialFrame;
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




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiBASE_plIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiBASE_miIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiBASE_stIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiBASE_diIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiBASE_pcIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/

/** declaration of hook melthook_HOOK_MELT_MAKE_LOCATION */
MELT_EXTERN
melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION(melt_ptr_t melthookdatap,
        const char* meltinp0_FILENAME,
        long meltinp1_LINENO)
;



/** end of declaration for hook melthook_HOOK_MELT_MAKE_LOCATION**/






melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiBASE_LIST_NTH_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiBASE_REGISTER_DIAG_GIMPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h791191221 /*opaqueinitialclassy*/ meltinitial_frame_t;


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


/**** warmelt-base+02.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1716 */

    /** start of frame for meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER// fromline 1531
        : public Melt_CallFrameWithValues<14>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[14];
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
        MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER), clos) {};
        MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER() //the constructor fromline 1606
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER)) {};
        MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER


    /** end of frame for meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1661**/

    /* end of frame for routine meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1720 */

    /* classy proc frame meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER */
    MeltFrame_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_PRAGMA_HANDLER", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1853:/ getarg");
    /*_.LSTHANDLER__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1858:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.LSTHANDLER__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-base.melt:1858:/ cond");
            /*cond*/
            if (/*_#IS_LIST__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1858:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "register_pragma_handler takes a list as argument.";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1858;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LSTHANDLER__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                            meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V4*/
                        meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1858:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                        meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V3*/
            meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1858:/ clear");
            /*clear*/ /*_#IS_LIST__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V4*/
            meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V3*/ meltfptr[2] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-base.melt:1860:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1])),
                                          (melt_ptr_t)((/*!CLASS_SYSTEM_DATA*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1])) /*=obj*/;
                    melt_object_get_field(slot,obj, 11, "SYSDATA_MELTPRAGMAS");
                    /*_.OLDTUPLE__V6*/
                    meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLDTUPLE__V6*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#OLDSIZE__L2*/
        meltfnum[0] = 0;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1862:/ cond");
        /*cond*/
        if ((/*!NOTNULL*/ meltfrout->tabval[3])) /*then*/
            {
                /*^cond.then*/
                /*_._IF___V7*/ meltfptr[3] = /*_.OLDTUPLE__V6*/ meltfptr[4];;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1862:/ cond.else");

                /*_._IF___V7*/
                meltfptr[3] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#MULTIPLE_LENGTH__L3*/
        meltfnum[2] =
            (melt_multiple_length((melt_ptr_t)(/*_.OLDTUPLE__V6*/ meltfptr[4])));;
        MELT_LOCATION("warmelt-base.melt:1863:/ compute");
        /*_#OLDSIZE__L2*/
        meltfnum[0] = /*_#SETQ___L4*/ meltfnum[3] = /*_#MULTIPLE_LENGTH__L3*/ meltfnum[2];;
        MELT_LOCATION("warmelt-base.melt:1864:/ quasiblock");


        /*_#MULTIPLE_LENGTH__L5*/
        meltfnum[4] =
            (melt_multiple_length((melt_ptr_t)(/*_.OLDTUPLE__V6*/ meltfptr[4])));;
        /*^compute*/
        /*_#LIST_LENGTH__L6*/
        meltfnum[5] =
            (melt_list_length((melt_ptr_t)(/*_.LSTHANDLER__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_#NEWSIZE__L7*/
        meltfnum[6] =
            ((/*_#MULTIPLE_LENGTH__L5*/ meltfnum[4]) + (/*_#LIST_LENGTH__L6*/ meltfnum[5]));;
        /*^compute*/
        /*_.NEWTUPLE__V8*/
        meltfptr[7] =
            (meltgc_new_multiple((meltobject_ptr_t)((/*!DISCR_MULTIPLE*/ meltfrout->tabval[4])), (/*_#NEWSIZE__L7*/ meltfnum[6])));;
        /*^compute*/
        /*_#I__L8*/
        meltfnum[7] = 0;;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OLDTUPLE__V6*/ meltfptr[4]);
            for (/*_#IUNUSED__L9*/ meltfnum[8] = 0;
                                   (/*_#IUNUSED__L9*/ meltfnum[8] >= 0) && (/*_#IUNUSED__L9*/ meltfnum[8] <  meltcit1__EACHTUP_ln);
                                   /*_#IUNUSED__L9*/ meltfnum[8]++)
                {
                    /*_.CURHANDER__V9*/ meltfptr[8] = melt_multiple_nth((melt_ptr_t)(/*_.OLDTUPLE__V6*/ meltfptr[4]),  /*_#IUNUSED__L9*/ meltfnum[8]);




                    {
                        MELT_LOCATION("warmelt-base.melt:1872:/ locexp");
                        meltgc_multiple_put_nth((melt_ptr_t)(/*_.NEWTUPLE__V8*/ meltfptr[7]), (/*_#I__L8*/ meltfnum[7]), (melt_ptr_t)(/*_.CURHANDER__V9*/ meltfptr[8]));
                    }
                    ;
                    /*_#plI__L10*/
                    meltfnum[9] =
                        ((/*_#I__L8*/ meltfnum[7]) + (1));;
                    MELT_LOCATION("warmelt-base.melt:1873:/ compute");
                    /*_#I__L8*/
                    meltfnum[7] = /*_#SETQ___L11*/ meltfnum[10] = /*_#plI__L10*/ meltfnum[9];;
                    if (/*_#IUNUSED__L9*/ meltfnum[8]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:1869:/ clear");
            /*clear*/ /*_.CURHANDER__V9*/
            meltfptr[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IUNUSED__L9*/
            meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_#plI__L10*/
            meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_#SETQ___L11*/
            meltfnum[10] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;
        /*citerblock FOREACH_IN_LIST*/
        {
            /* start foreach_in_list meltcit2__EACHLIST */
            for (/*_.CURPAIR__V10*/ meltfptr[9] = melt_list_first( (melt_ptr_t)/*_.LSTHANDLER__V2*/ meltfptr[1]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V10*/ meltfptr[9] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]))
                {
                    /*_.CURHANDLER__V11*/ meltfptr[10] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V10*/ meltfptr[9]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-base.melt:1879:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_A__L12*/
                        meltfnum[11] =
                            melt_is_instance_of((melt_ptr_t)(/*_.CURHANDLER__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_GCC_PRAGMA*/ meltfrout->tabval[5])));;
                        MELT_LOCATION("warmelt-base.melt:1879:/ cond");
                        /*cond*/
                        if (/*_#IS_A__L12*/ meltfnum[11]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-base.melt:1879:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "register_pragma_handler must be a list of class_gcc_pragma.";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 1879;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURHANDLER__V11*/ meltfptr[10];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                                        meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V13*/
                                    meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:1879:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                                    meltfptr[13] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V12*/
                        meltfptr[11] = /*_._IFELSE___V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1879:/ clear");
                        /*clear*/ /*_#IS_A__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V13*/
                        meltfptr[12] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V12*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:1881:/ locexp");
                        meltgc_multiple_put_nth((melt_ptr_t)(/*_.NEWTUPLE__V8*/ meltfptr[7]), (/*_#I__L8*/ meltfnum[7]), (melt_ptr_t)(/*_.CURHANDLER__V11*/ meltfptr[10]));
                    }
                    ;
                    /*_#plI__L13*/
                    meltfnum[11] =
                        ((/*_#I__L8*/ meltfnum[7]) + (1));;
                    MELT_LOCATION("warmelt-base.melt:1882:/ compute");
                    /*_#I__L8*/
                    meltfnum[7] = /*_#SETQ___L14*/ meltfnum[13] = /*_#plI__L13*/ meltfnum[11];;
                } /* end foreach_in_list meltcit2__EACHLIST */
            /*_.CURPAIR__V10*/ meltfptr[9] = NULL;
            /*_.CURHANDLER__V11*/
            meltfptr[10] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:1876:/ clear");
            /*clear*/ /*_.CURPAIR__V10*/
            meltfptr[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURHANDLER__V11*/
            meltfptr[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V12*/
            meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#plI__L13*/
            meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#SETQ___L14*/
            meltfnum[13] = 0 ;
        } /*endciterblock FOREACH_IN_LIST*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1884:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1])),
                                          (melt_ptr_t)((/*!CLASS_SYSTEM_DATA*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SYSDATA_MELTPRAGMAS", melt_magic_discr((melt_ptr_t)((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1])), (11), (/*_.NEWTUPLE__V8*/ meltfptr[7]), "SYSDATA_MELTPRAGMAS");
                    ;
                    /*^touch*/
                    meltgc_touch((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1]));
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object ((/*!INITIAL_SYSTEM_DATA*/ meltfrout->tabval[1]), "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-base.melt:1864:/ clear");
        /*clear*/ /*_#MULTIPLE_LENGTH__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#LIST_LENGTH__L6*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NEWSIZE__L7*/
        meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWTUPLE__V8*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#I__L8*/
        meltfnum[7] = 0 ;

        MELT_LOCATION("warmelt-base.melt:1860:/ clear");
        /*clear*/ /*_.OLDTUPLE__V6*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OLDSIZE__L2*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V7*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MULTIPLE_LENGTH__L3*/
        meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#SETQ___L4*/
        meltfnum[3] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1853:/ clear");
        /*clear*/ /*_.IFCPP___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PRAGMA_HANDLER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_29_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_30_WARMELTmiBASE_SET_REFERENCE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_30_WARMELTmiBASE_SET_REFERENCE fromline 1716 */

    /** start of frame for meltrout_30_WARMELTmiBASE_SET_REFERENCE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE// fromline 1531
        : public Melt_CallFrameWithValues<3>
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
        MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE), clos) {};
        MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE)) {};
        MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE


    /** end of frame for meltrout_30_WARMELTmiBASE_SET_REFERENCE fromline 1661**/

    /* end of frame for routine meltrout_30_WARMELTmiBASE_SET_REFERENCE fromline 1720 */

    /* classy proc frame meltrout_30_WARMELTmiBASE_SET_REFERENCE */
    MeltFrame_meltrout_30_WARMELTmiBASE_SET_REFERENCE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_30_WARMELTmiBASE_SET_REFERENCE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("SET_REFERENCE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1937:/ getarg");
    /*_.R__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.V__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.V__V3*/ meltfptr[2])) != NULL);

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
        MELT_LOCATION("warmelt-base.melt:1940:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.R__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)(/*_.R__V2*/ meltfptr[1])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.R__V2*/ meltfptr[1]), (0), (/*_.V__V3*/ meltfptr[2]), "REFERENCED_VALUE");
                    ;
                    /*^touch*/
                    meltgc_touch(/*_.R__V2*/ meltfptr[1]);
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.R__V2*/ meltfptr[1], "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("SET_REFERENCE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_30_WARMELTmiBASE_SET_REFERENCE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_30_WARMELTmiBASE_SET_REFERENCE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1716 */

    /** start of frame for meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR// fromline 1531
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
        MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR), clos) {};
        MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR() //the constructor fromline 1606
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR)) {};
        MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR


    /** end of frame for meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1661**/

    /* end of frame for routine meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1720 */

    /* classy proc frame meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR */
    MeltFrame_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("INSTALL_VALUE_DESCRIPTOR", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1949:/ getarg");
    /*_.VD__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:1950:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.VD__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_VALUE_DESCRIPTOR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-base.melt:1950:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1950:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check vd";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 1950;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VD__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                            meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V4*/
                        meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1950:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                        meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V3*/
            meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1950:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V4*/
            meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V3*/ meltfptr[2] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-base.melt:1951:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!VALUE_DESCRIPTOR_LIST_REFERENCE*/ meltfrout->tabval[2])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!VALUE_DESCRIPTOR_LIST_REFERENCE*/ meltfrout->tabval[2])) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V6*/
                    meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V6*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1951:/ locexp");
            meltgc_append_list((melt_ptr_t)(/*_.REFERENCED_VALUE__V6*/ meltfptr[4]), (melt_ptr_t)(/*_.VD__V2*/ meltfptr[1]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1949:/ clear");
        /*clear*/ /*_.IFCPP___V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V6*/
        meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("INSTALL_VALUE_DESCRIPTOR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_31_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1716 */

    /** start of frame for meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST// fromline 1531
        : public Melt_CallFrameWithValues<2>
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
        MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST), clos) {};
        MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST() //the constructor fromline 1606
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST)) {};
        MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST


    /** end of frame for meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1661**/

    /* end of frame for routine meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1720 */

    /* classy proc frame meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST */
    MeltFrame_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("RETRIEVE_VALUE_DESCRIPTOR_LIST", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    MELT_LOCATION("warmelt-base.melt:1954:/ block");
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:1955:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!VALUE_DESCRIPTOR_LIST_REFERENCE*/ meltfrout->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!VALUE_DESCRIPTOR_LIST_REFERENCE*/ meltfrout->tabval[0])) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V2*/
                    meltfptr[1] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V2*/ meltfptr[1] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1954:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.REFERENCED_VALUE__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:1954:/ locexp");
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
        /*clear*/ /*_.REFERENCED_VALUE__V2*/
        meltfptr[1] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("RETRIEVE_VALUE_DESCRIPTOR_LIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_32_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1716 */

    /** start of frame for meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT// fromline 1531
        : public Melt_CallFrameWithValues<20>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[6];
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
        MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT), clos) {};
        MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT)) {};
        MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT


    /** end of frame for meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1661**/

    /* end of frame for routine meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1720 */

    /* classy proc frame meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT */
    MeltFrame_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LIST_REMOVE_LAST_ELEMENT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3048:/ getarg");
    /*_.LIS__V2*/
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
        /*_#IS_LIST__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
        /*^compute*/
        /*_#NOT__L2*/
        meltfnum[1] =
            (!(/*_#IS_LIST__L1*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-base.melt:3052:/ cond");
        /*cond*/
        if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3053:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:3053:/ locexp");
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
                    /*_._IF___V3*/
                    meltfptr[2] = /*_.RETURN___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3052:/ clear");
                    /*clear*/ /*_.RETURN___V4*/
                    meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V3*/ meltfptr[2] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3054:/ quasiblock");


        /*_.LASTPAIR__V5*/
        meltfptr[3] =
            (melt_list_last((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_.PREVPAIR__V6*/
        meltfptr[5] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3057:/ cond");
        /*cond*/
        if (/*_.LASTPAIR__V5*/ meltfptr[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    /*^cppif.then*/
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_PAIR__L3*/
                        meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LASTPAIR__V5*/ meltfptr[3])) == MELTOBMAG_PAIR);;
                        MELT_LOCATION("warmelt-base.melt:3057:/ cond");
                        /*cond*/
                        if (/*_#IS_PAIR__L3*/ meltfnum[2]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                            }
                        else {
                                MELT_LOCATION("warmelt-base.melt:3057:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[3];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check lastpair";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3057;
                                        /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                                        meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V9*/
                                    meltfptr[8] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:3057:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                                    meltfptr[9] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V8*/
                        meltfptr[7] = /*_._IFELSE___V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:3057:/ clear");
                        /*clear*/ /*_#IS_PAIR__L3*/
                        meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V8*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    /*^compute*/
                    /*_._IFELSE___V7*/
                    meltfptr[6] = /*_.IFCPP___V8*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3057:/ clear");
                    /*clear*/ /*_.IFCPP___V8*/
                    meltfptr[7] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V7*/ meltfptr[6] = /*_.LASTPAIR__V5*/ meltfptr[3];;
            }
        ;
        /*citerblock FOREACH_IN_LIST*/
        {
            /* start foreach_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V11*/ meltfptr[9] = melt_list_first( (melt_ptr_t)/*_.LIS__V2*/ meltfptr[1]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V11*/ meltfptr[9]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V11*/ meltfptr[9] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V11*/ meltfptr[9]))
                {
                    /*_.CURELEM__V12*/ meltfptr[8] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V11*/ meltfptr[9]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-base.melt:3061:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_PAIR__L4*/
                        meltfnum[2] =
                            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V11*/ meltfptr[9])) == MELTOBMAG_PAIR);;
                        MELT_LOCATION("warmelt-base.melt:3061:/ cond");
                        /*cond*/
                        if (/*_#IS_PAIR__L4*/ meltfnum[2]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-base.melt:3061:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check curpair";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3061;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURPAIR__V11*/ meltfptr[9];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                                        meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V14*/
                                    meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:3061:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                                    meltfptr[14] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V13*/
                        meltfptr[7] = /*_._IFELSE___V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:3061:/ clear");
                        /*clear*/ /*_#IS_PAIR__L4*/
                        meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V14*/
                        meltfptr[13] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V13*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*^compute*/
                    /*_#eqeq__L5*/
                    meltfnum[2] =
                        ((/*_.CURPAIR__V11*/ meltfptr[9]) == (/*_.LASTPAIR__V5*/ meltfptr[3]));;
                    MELT_LOCATION("warmelt-base.melt:3062:/ cond");
                    /*cond*/
                    if (/*_#eqeq__L5*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3063:/ compute");
                                /*_.CURPAIR__V11*/
                                meltfptr[9] = /*_.SETQ___V17*/ meltfptr[13] = (/*nil*/NULL);;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_PAIR__L6*/
                                meltfnum[5] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.PREVPAIR__V6*/ meltfptr[5])) == MELTOBMAG_PAIR);;
                                MELT_LOCATION("warmelt-base.melt:3064:/ cond");
                                /*cond*/
                                if (/*_#IS_PAIR__L6*/ meltfnum[5]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-base.melt:3065:/ locexp");
                                                /* list_remove_last_element UPDATEPAIR_CHK__1 */
                                                ((struct meltpair_st*)/*_.PREVPAIR__V6*/ meltfptr[5])->tl = NULL;
                                                ((struct meltlist_st*)/*_.LIS__V2*/ meltfptr[1])->last
                                                    = (struct meltpair_st*) /*_.PREVPAIR__V6*/ meltfptr[5];
                                                meltgc_touch_dest (/*_.LIS__V2*/ meltfptr[1], /*_.PREVPAIR__V6*/ meltfptr[5]);
                                                ;
                                            }
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-base.melt:3064:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-base.melt:3072:/ locexp");
                                                /* list_remove_last_element EMPTYLIST_CHK__1 */
                                                ((struct meltlist_st*)/*_.LIS__V2*/ meltfptr[1])->first = NULL;
                                                ((struct meltlist_st*)/*_.LIS__V2*/ meltfptr[1])->last = NULL;
                                                meltgc_touch (/*_.LIS__V2*/ meltfptr[1]);
                                                ;
                                            }
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-base.melt:3078:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = /*_.CURELEM__V12*/ meltfptr[8];;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3078:/ locexp");
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
                                MELT_LOCATION("warmelt-base.melt:3062:/ quasiblock");


                                /*_.PROGN___V19*/
                                meltfptr[18] = /*_.RETURN___V18*/ meltfptr[17];;
                                /*^compute*/
                                /*_._IF___V16*/
                                meltfptr[14] = /*_.PROGN___V19*/ meltfptr[18];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3062:/ clear");
                                /*clear*/ /*_.SETQ___V17*/
                                meltfptr[13] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#IS_PAIR__L6*/
                                meltfnum[5] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V18*/
                                meltfptr[17] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V19*/
                                meltfptr[18] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V16*/ meltfptr[14] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-base.melt:3079:/ compute");
                    /*_.PREVPAIR__V6*/
                    meltfptr[5] = /*_.SETQ___V20*/ meltfptr[13] = /*_.CURPAIR__V11*/ meltfptr[9];;
                } /* end foreach_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V11*/ meltfptr[9] = NULL;
            /*_.CURELEM__V12*/
            meltfptr[8] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:3058:/ clear");
            /*clear*/ /*_.CURPAIR__V11*/
            meltfptr[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURELEM__V12*/
            meltfptr[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V13*/
            meltfptr[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#eqeq__L5*/
            meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V16*/
            meltfptr[14] = 0 ;
            /*^clear*/
            /*clear*/ /*_.SETQ___V20*/
            meltfptr[13] = 0 ;
        } /*endciterblock FOREACH_IN_LIST*/
        ;

        MELT_LOCATION("warmelt-base.melt:3054:/ clear");
        /*clear*/ /*_.LASTPAIR__V5*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PREVPAIR__V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V7*/
        meltfptr[6] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:3048:/ clear");
        /*clear*/ /*_#IS_LIST__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LIST_REMOVE_LAST_ELEMENT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_33_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_COMPLETE_SEQUENCE_AS_TUPLE_ix = 0, variad_COMPLETE_SEQUENCE_AS_TUPLE_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_COMPLETE_SEQUENCE_AS_TUPLE_len)
#define melt_variadic_index variad_COMPLETE_SEQUENCE_AS_TUPLE_ix

    long current_blocklevel_signals_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1716 */

    /** start of frame for meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE// fromline 1531
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE), clos) {};
        MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE)) {};
        MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE


    /** end of frame for meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1661**/

    /* end of frame for routine meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1720 */

    /* classy proc frame meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE */
    MeltFrame_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPLETE_SEQUENCE_AS_TUPLE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3083:/ getarg");
    /*_.SRC__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3087:/ quasiblock");


        /*_.NEWLIST__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[0]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:3089:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                        for (/*_#IX__L2*/ meltfnum[1] = 0;
                                          (/*_#IX__L2*/ meltfnum[1] >= 0) && (/*_#IX__L2*/ meltfnum[1] <  meltcit1__EACHTUP_ln);
                                          /*_#IX__L2*/ meltfnum[1]++)
                            {
                                /*_.COMP__V5*/ meltfptr[4] = melt_multiple_nth((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]),  /*_#IX__L2*/ meltfnum[1]);




                                {
                                    MELT_LOCATION("warmelt-base.melt:3093:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.COMP__V5*/ meltfptr[4]));
                                }
                                ;
                                if (/*_#IX__L2*/ meltfnum[1]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-base.melt:3090:/ clear");
                        /*clear*/ /*_.COMP__V5*/
                        meltfptr[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#IX__L2*/
                        meltfnum[1] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3089:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_LIST__L3*/
                    meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-base.melt:3095:/ cond");
                    /*cond*/
                    if (/*_#IS_LIST__L3*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_LIST*/
                                {
                                    /* start foreach_in_list meltcit2__EACHLIST */
                                    for (/*_.CURPAIR__V6*/ meltfptr[5] = melt_list_first( (melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                                                           melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]) == MELTOBMAG_PAIR;
                                                           /*_.CURPAIR__V6*/ meltfptr[5] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]))
                                        {
                                            /*_.CURCOMP__V7*/ meltfptr[6] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]);



                                            {
                                                MELT_LOCATION("warmelt-base.melt:3099:/ locexp");
                                                meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCOMP__V7*/ meltfptr[6]));
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit2__EACHLIST */
                                    /*_.CURPAIR__V6*/ meltfptr[5] = NULL;
                                    /*_.CURCOMP__V7*/
                                    meltfptr[6] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:3096:/ clear");
                                    /*clear*/ /*_.CURPAIR__V6*/
                                    meltfptr[5] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURCOMP__V7*/
                                    meltfptr[6] = 0 ;
                                } /*endciterblock FOREACH_IN_LIST*/
                                ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-base.melt:3095:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-base.melt:3102:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]));
                                }
                                ;
                                MELT_LOCATION("warmelt-base.melt:3101:/ quasiblock");


                                /*epilog*/
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3089:/ clear");
                    /*clear*/ /*_#IS_LIST__L3*/
                    meltfnum[2] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3103:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_3:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:3105:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix == variad_COMPLETE_SEQUENCE_AS_TUPLE_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:3107:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V9*/
                            meltfptr[8] =  /*fromline 1341*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_3;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:3105:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            /*^cond*/
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix>=0 && variad_COMPLETE_SEQUENCE_AS_TUPLE_ix + 1 <= variad_COMPLETE_SEQUENCE_AS_TUPLE_len && meltxargdescr_[variad_COMPLETE_SEQUENCE_AS_TUPLE_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V10*/ meltfptr[9] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_COMPLETE_SEQUENCE_AS_TUPLE_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_COMPLETE_SEQUENCE_AS_TUPLE_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/
                                        variad_COMPLETE_SEQUENCE_AS_TUPLE_ix += 1;;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3109:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V10*/ meltfptr[9]));
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3105:/ clear");
                                        /*clear*/ /*_.V__V10*/
                                        meltfptr[9] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-base.melt:3111:/ quasiblock");


                                        /*_.VCTY__V12*/
                                        meltfptr[11] =
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
                                        MELT_LOCATION("warmelt-base.melt:3114:/ cond");
                                        /*cond*/
                                        if (
                                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V12*/ meltfptr[11]),
                                                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])))
                                        ) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^getslot*/
                                                {
                                                    melt_ptr_t slot=NULL, obj=NULL;
                                                    obj = (melt_ptr_t)(/*_.VCTY__V12*/ meltfptr[11]) /*=obj*/;
                                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                    /*_.NAMED_NAME__V13*/
                                                    meltfptr[12] = slot;
                                                };
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*_.NAMED_NAME__V13*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
                                            }
                                        ;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3113:/ locexp");
                                            error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "COMPLETE_SEQUENCE_AS_TUPLE with unsupported ctype"),
                                                   melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[12])));
                                        }
                                        ;

#if MELT_HAVE_DEBUG
                                        MELT_LOCATION("warmelt-base.melt:3115:/ cppif.then");
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*^cond*/
                                            /*cond*/
                                            if ((/*nil*/NULL)) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-base.melt:3115:/ cond.else");

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
                                                            argtab[0].meltbp_cstring =  "invalid variadic argument to COMPLETE_SEQUENCE_AS_TUPLE";
                                                            /*^apply.arg*/
                                                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                            /*^apply.arg*/
                                                            argtab[2].meltbp_long = 3115;
                                                            /*^apply.arg*/
                                                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V12*/ meltfptr[11];
                                                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V15*/
                                                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:3115:/ clear");
                                                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                        meltfptr[15] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.IFCPP___V14*/
                                            meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3115:/ clear");
                                            /*clear*/ /*_._IFELSE___V15*/
                                            meltfptr[14] = 0 ;
                                        }

#else /*MELT_HAVE_DEBUG*/
                                        /*^cppif.else*/
                                        /*_.IFCPP___V14*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                        ;
                                        /*^compute*/
                                        /*_.LET___V11*/
                                        meltfptr[9] = /*_.IFCPP___V14*/ meltfptr[13];;

                                        MELT_LOCATION("warmelt-base.melt:3111:/ clear");
                                        /*clear*/ /*_.VCTY__V12*/
                                        meltfptr[11] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.NAMED_NAME__V13*/
                                        meltfptr[12] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.IFCPP___V14*/
                                        meltfptr[13] = 0 ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3105:/ clear");
                                        /*clear*/ /*_.LET___V11*/
                                        meltfptr[9] = 0 ;
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
            goto meltlabloop_ARGLOOP_3;
meltlabexit_ARGLOOP_3:
            ;
            MELT_LOCATION("warmelt-base.melt:3103:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V8*/
            meltfptr[7] = /*_.ARGLOOP__V9*/ meltfptr[8];;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:3117:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            /*_.RES__V18*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3119:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V18*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-base.melt:3119:/ locexp");
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
        /*_.LET___V17*/
        meltfptr[15] = /*_.RETURN___V19*/ meltfptr[11];;

        MELT_LOCATION("warmelt-base.melt:3117:/ clear");
        /*clear*/ /*_.RES__V18*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V19*/
        meltfptr[11] = 0 ;
        /*_.LET___V3*/
        meltfptr[2] = /*_.LET___V17*/ meltfptr[15];;

        MELT_LOCATION("warmelt-base.melt:3087:/ clear");
        /*clear*/ /*_.NEWLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V8*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
        MELT_LOCATION("warmelt-base.melt:3083:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3083:/ locexp");
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
    melt_trace_end("COMPLETE_SEQUENCE_AS_TUPLE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_34_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_COMPLETE_SEQUENCE_AS_LIST_ix = 0, variad_COMPLETE_SEQUENCE_AS_LIST_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_COMPLETE_SEQUENCE_AS_LIST_len)
#define melt_variadic_index variad_COMPLETE_SEQUENCE_AS_LIST_ix

    long current_blocklevel_signals_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1716 */

    /** start of frame for meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST// fromline 1531
        : public Melt_CallFrameWithValues<17>
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
        MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<17> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST), clos) {};
        MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST() //the constructor fromline 1606
            : Melt_CallFrameWithValues<17> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<17> (fil,lin, sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST)) {};
        MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<17> (fil,lin, sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST


    /** end of frame for meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1661**/

    /* end of frame for routine meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1720 */

    /* classy proc frame meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST */
    MeltFrame_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPLETE_SEQUENCE_AS_LIST", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3123:/ getarg");
    /*_.SRC__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:3127:/ quasiblock");


        /*_.NEWLIST__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[0]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:3129:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                        for (/*_#IX__L2*/ meltfnum[1] = 0;
                                          (/*_#IX__L2*/ meltfnum[1] >= 0) && (/*_#IX__L2*/ meltfnum[1] <  meltcit1__EACHTUP_ln);
                                          /*_#IX__L2*/ meltfnum[1]++)
                            {
                                /*_.COMP__V5*/ meltfptr[4] = melt_multiple_nth((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]),  /*_#IX__L2*/ meltfnum[1]);




                                {
                                    MELT_LOCATION("warmelt-base.melt:3133:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.COMP__V5*/ meltfptr[4]));
                                }
                                ;
                                if (/*_#IX__L2*/ meltfnum[1]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-base.melt:3130:/ clear");
                        /*clear*/ /*_.COMP__V5*/
                        meltfptr[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#IX__L2*/
                        meltfnum[1] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:3129:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_LIST__L3*/
                    meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-base.melt:3135:/ cond");
                    /*cond*/
                    if (/*_#IS_LIST__L3*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_LIST*/
                                {
                                    /* start foreach_in_list meltcit2__EACHLIST */
                                    for (/*_.CURPAIR__V6*/ meltfptr[5] = melt_list_first( (melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                                                           melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]) == MELTOBMAG_PAIR;
                                                           /*_.CURPAIR__V6*/ meltfptr[5] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]))
                                        {
                                            /*_.CURCOMP__V7*/ meltfptr[6] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[5]);



                                            {
                                                MELT_LOCATION("warmelt-base.melt:3139:/ locexp");
                                                meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCOMP__V7*/ meltfptr[6]));
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit2__EACHLIST */
                                    /*_.CURPAIR__V6*/ meltfptr[5] = NULL;
                                    /*_.CURCOMP__V7*/
                                    meltfptr[6] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:3136:/ clear");
                                    /*clear*/ /*_.CURPAIR__V6*/
                                    meltfptr[5] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURCOMP__V7*/
                                    meltfptr[6] = 0 ;
                                } /*endciterblock FOREACH_IN_LIST*/
                                ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-base.melt:3135:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-base.melt:3142:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]));
                                }
                                ;
                                MELT_LOCATION("warmelt-base.melt:3141:/ quasiblock");


                                /*epilog*/
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3129:/ clear");
                    /*clear*/ /*_#IS_LIST__L3*/
                    meltfnum[2] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3143:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_4:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:3145:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix == variad_COMPLETE_SEQUENCE_AS_LIST_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:3147:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V9*/
                            meltfptr[8] =  /*fromline 1341*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGLOOP_4;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:3145:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            /*^cond*/
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix>=0 && variad_COMPLETE_SEQUENCE_AS_LIST_ix + 1 <= variad_COMPLETE_SEQUENCE_AS_LIST_len && meltxargdescr_[variad_COMPLETE_SEQUENCE_AS_LIST_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.V__V10*/ meltfptr[9] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_COMPLETE_SEQUENCE_AS_LIST_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_COMPLETE_SEQUENCE_AS_LIST_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/
                                        variad_COMPLETE_SEQUENCE_AS_LIST_ix += 1;;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3149:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V10*/ meltfptr[9]));
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3145:/ clear");
                                        /*clear*/ /*_.V__V10*/
                                        meltfptr[9] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-base.melt:3151:/ quasiblock");


                                        /*_.VCTY__V12*/
                                        meltfptr[11] =
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
                                        MELT_LOCATION("warmelt-base.melt:3154:/ cond");
                                        /*cond*/
                                        if (
                                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V12*/ meltfptr[11]),
                                                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])))
                                        ) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^getslot*/
                                                {
                                                    melt_ptr_t slot=NULL, obj=NULL;
                                                    obj = (melt_ptr_t)(/*_.VCTY__V12*/ meltfptr[11]) /*=obj*/;
                                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                    /*_.NAMED_NAME__V13*/
                                                    meltfptr[12] = slot;
                                                };
                                                ;
                                            }
                                        else    /*^cond.else*/
                                            {

                                                /*_.NAMED_NAME__V13*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
                                            }
                                        ;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3153:/ locexp");
                                            error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "COMPLETE_SEQUENCE_AS_LIST with unsupported ctype"),
                                                   melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[12])));
                                        }
                                        ;

#if MELT_HAVE_DEBUG
                                        MELT_LOCATION("warmelt-base.melt:3155:/ cppif.then");
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*^cond*/
                                            /*cond*/
                                            if ((/*nil*/NULL)) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-base.melt:3155:/ cond.else");

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
                                                            argtab[0].meltbp_cstring =  "invalid variadic argument to COMPLETE_SEQUENCE_AS_LIST";
                                                            /*^apply.arg*/
                                                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                                            /*^apply.arg*/
                                                            argtab[2].meltbp_long = 3155;
                                                            /*^apply.arg*/
                                                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V12*/ meltfptr[11];
                                                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V15*/
                                                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:3155:/ clear");
                                                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                        meltfptr[15] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.IFCPP___V14*/
                                            meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3155:/ clear");
                                            /*clear*/ /*_._IFELSE___V15*/
                                            meltfptr[14] = 0 ;
                                        }

#else /*MELT_HAVE_DEBUG*/
                                        /*^cppif.else*/
                                        /*_.IFCPP___V14*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                        ;
                                        /*^compute*/
                                        /*_.LET___V11*/
                                        meltfptr[9] = /*_.IFCPP___V14*/ meltfptr[13];;

                                        MELT_LOCATION("warmelt-base.melt:3151:/ clear");
                                        /*clear*/ /*_.VCTY__V12*/
                                        meltfptr[11] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.NAMED_NAME__V13*/
                                        meltfptr[12] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.IFCPP___V14*/
                                        meltfptr[13] = 0 ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3145:/ clear");
                                        /*clear*/ /*_.LET___V11*/
                                        meltfptr[9] = 0 ;
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
            goto meltlabloop_ARGLOOP_4;
meltlabexit_ARGLOOP_4:
            ;
            MELT_LOCATION("warmelt-base.melt:3143:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V8*/
            meltfptr[7] = /*_.ARGLOOP__V9*/ meltfptr[8];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3157:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.NEWLIST__V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:3157:/ locexp");
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
        /*_.LET___V3*/
        meltfptr[2] = /*_.RETURN___V17*/ meltfptr[15];;

        MELT_LOCATION("warmelt-base.melt:3127:/ clear");
        /*clear*/ /*_.NEWLIST__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V8*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V17*/
        meltfptr[15] = 0 ;
        MELT_LOCATION("warmelt-base.melt:3123:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3123:/ locexp");
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
    melt_trace_end("COMPLETE_SEQUENCE_AS_LIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1716 */

    /** start of frame for meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET// fromline 1531
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[4];
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
        MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET), clos) {};
        MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET() //the constructor fromline 1606
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET)) {};
        MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET


    /** end of frame for meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1661**/

    /* end of frame for routine meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1720 */

    /* classy proc frame meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET */
    MeltFrame_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MELT_PREDEFINED_GET", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3184:/ getarg");
    /*_.NAMV__V2*/
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
        /*_#IS_STRING__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-base.melt:3186:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.PREDEFSTR_CHK__V4*/ meltfptr[3] =
                        /*melt_predefined_get PREDEFSTR_CHK__1 */
                        melt_fetch_predefined (melt_predefined_index_by_name (melt_string_str ((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])))) ;;
                    /*^compute*/
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_.PREDEFSTR_CHK__V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3186:/ clear");
                    /*clear*/ /*_.PREDEFSTR_CHK__V4*/
                    meltfptr[3] = 0 ;
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
                    /*_#IS_A__L2*/
                    meltfnum[1] =
                        melt_is_instance_of((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])));;
                    MELT_LOCATION("warmelt-base.melt:3189:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3190:/ quasiblock");


                                /*^cond*/
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMSTR__V7*/
                                            meltfptr[6] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMSTR__V7*/ meltfptr[6] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_.PREDEFNAM_CHK__V8*/
                                meltfptr[7] =
                                    /*melt_predefined_get PREDEFNAM_CHK__1 */
                                    melt_fetch_predefined (melt_predefined_index_by_name (melt_string_str ((melt_ptr_t)(/*_.NAMSTR__V7*/ meltfptr[6])))) ;;
                                /*^compute*/
                                /*_.LET___V6*/
                                meltfptr[5] = /*_.PREDEFNAM_CHK__V8*/ meltfptr[7];;

                                MELT_LOCATION("warmelt-base.melt:3190:/ clear");
                                /*clear*/ /*_.NAMSTR__V7*/
                                meltfptr[6] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PREDEFNAM_CHK__V8*/
                                meltfptr[7] = 0 ;
                                /*_._IFELSE___V5*/
                                meltfptr[3] = /*_.LET___V6*/ meltfptr[5];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3189:/ clear");
                                /*clear*/ /*_.LET___V6*/
                                meltfptr[5] = 0 ;
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
                                /*_#IS_INTEGERBOX__L3*/
                                meltfnum[2] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
                                MELT_LOCATION("warmelt-base.melt:3195:/ cond");
                                /*cond*/
                                if (/*_#IS_INTEGERBOX__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3196:/ quasiblock");


                                            /*_#NUM__L4*/
                                            meltfnum[3] =
                                                (melt_get_int((melt_ptr_t)(/*_.NAMV__V2*/ meltfptr[1])));;
                                            /*^compute*/
                                            /*_.PREDEFNAM_CHK__V11*/
                                            meltfptr[5] =
                                                /*melt_predefined_get PREDEFNAM_CHK__2 */
                                                melt_fetch_predefined (/*_#NUM__L4*/ meltfnum[3]) ;;
                                            /*^compute*/
                                            /*_.LET___V10*/
                                            meltfptr[7] = /*_.PREDEFNAM_CHK__V11*/ meltfptr[5];;

                                            MELT_LOCATION("warmelt-base.melt:3196:/ clear");
                                            /*clear*/ /*_#NUM__L4*/
                                            meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PREDEFNAM_CHK__V11*/
                                            meltfptr[5] = 0 ;
                                            /*_._IFELSE___V9*/
                                            meltfptr[6] = /*_.LET___V10*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3195:/ clear");
                                            /*clear*/ /*_.LET___V10*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3200:/ quasiblock");


                                            /*_.PROGN___V12*/
                                            meltfptr[5] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V9*/
                                            meltfptr[6] = /*_.PROGN___V12*/ meltfptr[5];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3195:/ clear");
                                            /*clear*/ /*_.PROGN___V12*/
                                            meltfptr[5] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V5*/
                                meltfptr[3] = /*_._IFELSE___V9*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3189:/ clear");
                                /*clear*/ /*_#IS_INTEGERBOX__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V9*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_._IFELSE___V5*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3186:/ clear");
                    /*clear*/ /*_#IS_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V5*/
                    meltfptr[3] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3184:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3184:/ locexp");
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
        /*clear*/ /*_#IS_STRING__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MELT_PREDEFINED_GET", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_36_WARMELTmiBASE_MELT_PREDEFINED_GET*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_37_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_37_WARMELTmiBASE_SYMBOL_CNAME fromline 1716 */

    /** start of frame for meltrout_37_WARMELTmiBASE_SYMBOL_CNAME of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME// fromline 1531
        : public Melt_CallFrameWithValues<20>
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
        MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME), clos) {};
        MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME() //the constructor fromline 1606
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME)) {};
        MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME


    /** end of frame for meltrout_37_WARMELTmiBASE_SYMBOL_CNAME fromline 1661**/

    /* end of frame for routine meltrout_37_WARMELTmiBASE_SYMBOL_CNAME fromline 1720 */

    /* classy proc frame meltrout_37_WARMELTmiBASE_SYMBOL_CNAME */
    MeltFrame_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_37_WARMELTmiBASE_SYMBOL_CNAME fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("SYMBOL_CNAME", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:3204:/ getarg");
    /*_.SY__V2*/
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
        /*_#IS_A__L1*/
        meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-base.melt:3206:/ cond");
        /*cond*/
        if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:3207:/ quasiblock");


                    /*_.SBUF__V5*/
                    meltfptr[4] =
                        (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                    MELT_LOCATION("warmelt-base.melt:3208:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V6*/
                                meltfptr[5] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.NAMED_NAME__V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:3208:/ locexp");
                        meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]),
                                                 melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V6*/ meltfptr[5])));
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:3209:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                      (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[0])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                melt_object_get_field(slot,obj, 3, "CSYM_URANK");
                                /*_.CSYM_URANK__V7*/
                                meltfptr[6] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CSYM_URANK__V7*/ meltfptr[6] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3209:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "cl";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CSYM_URANK__V7*/ meltfptr[6];
                        /*_.ADD2OUT__V8*/
                        meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.STRBUF2STRING__V9*/
                    meltfptr[8] =
                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]))));;
                    /*^compute*/
                    /*_.LET___V4*/
                    meltfptr[3] = /*_.STRBUF2STRING__V9*/ meltfptr[8];;

                    MELT_LOCATION("warmelt-base.melt:3207:/ clear");
                    /*clear*/ /*_.SBUF__V5*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V6*/
                    meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CSYM_URANK__V7*/
                    meltfptr[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V8*/
                    meltfptr[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.STRBUF2STRING__V9*/
                    meltfptr[8] = 0 ;
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_.LET___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3206:/ clear");
                    /*clear*/ /*_.LET___V4*/
                    meltfptr[3] = 0 ;
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
                    /*_#IS_A__L2*/
                    meltfnum[1] =
                        melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[5])));;
                    MELT_LOCATION("warmelt-base.melt:3212:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3213:/ quasiblock");


                                /*_.SBUF__V12*/
                                meltfptr[6] =
                                    (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                                MELT_LOCATION("warmelt-base.melt:3214:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V13*/
                                            meltfptr[7] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V13*/ meltfptr[7] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3214:/ locexp");
                                    meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]),
                                                             melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[7])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3215:/ locexp");
                                    /*add2sbuf_strconst*/
                                    meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]), ( "kw"));
                                }
                                ;
                                /*_.STRBUF2STRING__V14*/
                                meltfptr[8] =
                                    (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]))));;
                                /*^compute*/
                                /*_.LET___V11*/
                                meltfptr[5] = /*_.STRBUF2STRING__V14*/ meltfptr[8];;

                                MELT_LOCATION("warmelt-base.melt:3213:/ clear");
                                /*clear*/ /*_.SBUF__V12*/
                                meltfptr[6] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NAMED_NAME__V13*/
                                meltfptr[7] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.STRBUF2STRING__V14*/
                                meltfptr[8] = 0 ;
                                /*_._IFELSE___V10*/
                                meltfptr[4] = /*_.LET___V11*/ meltfptr[5];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3212:/ clear");
                                /*clear*/ /*_.LET___V11*/
                                meltfptr[5] = 0 ;
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
                                /*_#IS_A__L3*/
                                meltfnum[2] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[6])));;
                                MELT_LOCATION("warmelt-base.melt:3218:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3219:/ quasiblock");


                                            /*_.SBUF__V17*/
                                            meltfptr[7] =
                                                (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                                            MELT_LOCATION("warmelt-base.melt:3220:/ cond");
                                            /*cond*/
                                            if (
                                                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]),
                                                                              (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                                            ) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^getslot*/
                                                    {
                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                        obj = (melt_ptr_t)(/*_.SY__V2*/ meltfptr[1]) /*=obj*/;
                                                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                        /*_.NAMED_NAME__V18*/
                                                        meltfptr[8] = slot;
                                                    };
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_.NAMED_NAME__V18*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                                                }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-base.melt:3220:/ locexp");
                                                meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V17*/ meltfptr[7]),
                                                                         melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V18*/ meltfptr[8])));
                                            }
                                            ;
                                            /*_.STRBUF2STRING__V19*/
                                            meltfptr[5] =
                                                (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V17*/ meltfptr[7]))));;
                                            /*^compute*/
                                            /*_.LET___V16*/
                                            meltfptr[6] = /*_.STRBUF2STRING__V19*/ meltfptr[5];;

                                            MELT_LOCATION("warmelt-base.melt:3219:/ clear");
                                            /*clear*/ /*_.SBUF__V17*/
                                            meltfptr[7] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.NAMED_NAME__V18*/
                                            meltfptr[8] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.STRBUF2STRING__V19*/
                                            meltfptr[5] = 0 ;
                                            /*_._IFELSE___V15*/
                                            meltfptr[3] = /*_.LET___V16*/ meltfptr[6];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3218:/ clear");
                                            /*clear*/ /*_.LET___V16*/
                                            meltfptr[6] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3223:/ quasiblock");


                                            /*_.PROGN___V20*/
                                            meltfptr[7] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V15*/
                                            meltfptr[3] = /*_.PROGN___V20*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3218:/ clear");
                                            /*clear*/ /*_.PROGN___V20*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V10*/
                                meltfptr[4] = /*_._IFELSE___V15*/ meltfptr[3];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3212:/ clear");
                                /*clear*/ /*_#IS_A__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V15*/
                                meltfptr[3] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_._IFELSE___V10*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3206:/ clear");
                    /*clear*/ /*_#IS_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V10*/
                    meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3204:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3204:/ locexp");
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
        /*clear*/ /*_#IS_A__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("SYMBOL_CNAME", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_37_WARMELTmiBASE_SYMBOL_CNAME_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_37_WARMELTmiBASE_SYMBOL_CNAME*/



/**** end of warmelt-base+02.cc ****/
