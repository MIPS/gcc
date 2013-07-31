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



class Melt_InitialClassyFrame_WARMELTmiBASE_h143582010; // forward declaration fromline 6518
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h143582010 Melt_InitialFrame;
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

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h143582010 /*opaqueinitialclassy*/ meltinitial_frame_t;


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
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_55 (meltinitial_frame_t*, char*);


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

    /* start of frame for routine meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1716 */

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
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ), clos) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ() //the constructor fromline 1606
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ)) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ


    /** end of frame for meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1661**/

    /* end of frame for routine meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1720 */

    /* classy proc frame meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ */
    MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_GIMPLE_SEQ", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:528:/ getarg");
    /*_.C__V2*/
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
            (melt_magic_discr((melt_ptr_t)(/*_.C__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:529:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
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
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1716 */

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
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE), clos) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE)) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE


    /** end of frame for meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1661**/

    /* end of frame for routine meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1720 */

    /* classy proc frame meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE */
    MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_TREE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:532:/ getarg");
    /*_.C__V2*/
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
            (melt_magic_discr((melt_ptr_t)(/*_.C__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:533:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
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
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1716 */

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
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK), clos) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK() //the constructor fromline 1606
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK)) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK


    /** end of frame for meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1661**/

    /* end of frame for routine meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1720 */

    /* classy proc frame meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK */
    MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_BASIC_BLOCK", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:536:/ getarg");
    /*_.C__V2*/
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
            (melt_magic_discr((melt_ptr_t)(/*_.C__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-base.melt:537:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
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
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1716 */

    /** start of frame for meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO// fromline 1531
        : public Melt_CallFrameWithValues<259>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[12];
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
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<259> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO), clos) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO() //the constructor fromline 1606
            : Melt_CallFrameWithValues<259> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<259> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO)) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<259> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO


    /** end of frame for meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1661**/

    /* end of frame for routine meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1720 */

    /* classy proc frame meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO */
    MeltFrame_meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_12_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("DIAGNOSTIC_ARGS_MACRO", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:543:/ getarg");
    /*_.SEXP__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:544:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:544:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:544:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-base.melt:545:/ quasiblock");


        MELT_LOCATION("warmelt-base.melt:546:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                    /*_.SLOC__V9*/
                    meltfptr[5] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SLOC__V9*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:547:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                    /*_.SCONT__V10*/
                    meltfptr[9] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SCONT__V10*/ meltfptr[9] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:548:/ apply");
        /*apply*/
        {
            /*_.TCONT__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.SCONT__V10*/ meltfptr[9]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:550:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L3*/
            meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:550:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L4*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:550:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 550;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro sloc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V9*/ meltfptr[5];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " tcont=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V11*/ meltfptr[10];
                            /*_.MELT_DEBUG_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V12*/
                        meltfptr[11] = /*_.MELT_DEBUG_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:550:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/
                        meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V12*/ meltfptr[11] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:550:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V12*/
            meltfptr[11] = 0 ;
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
        /*_#MULTIPLE_LENGTH__L5*/
        meltfnum[0] =
            (melt_multiple_length((melt_ptr_t)(/*_.TCONT__V11*/ meltfptr[10])));;
        /*^compute*/
        /*_#exeqI__L6*/
        meltfnum[1] =
            ((/*_#MULTIPLE_LENGTH__L5*/ meltfnum[0]) != (3));;
        MELT_LOCATION("warmelt-base.melt:551:/ cond");
        /*cond*/
        if (/*_#exeqI__L6*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:552:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "DIAGNOSTIC_ARGS_MACRO needs 2 args: <nbargs> <vect>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:553:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:553:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:551:/ quasiblock");


                    /*_.PROGN___V16*/
                    meltfptr[15] = /*_.RETURN___V15*/ meltfptr[11];;
                    /*^compute*/
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.PROGN___V16*/ meltfptr[15];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:551:/ clear");
                    /*clear*/ /*_.RETURN___V15*/
                    meltfptr[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V16*/
                    meltfptr[15] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:555:/ quasiblock");


        /*_.MACROVARNBARG__V18*/
        meltfptr[15] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V11*/ meltfptr[10]), (1)));;
        /*^compute*/
        /*_.MACROVARVECT__V19*/
        meltfptr[18] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V11*/ meltfptr[10]), (2)));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L7*/
        meltfnum[6] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MACROVARNBARG__V18*/ meltfptr[15]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-base.melt:558:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:559:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "(DIAGNOSTIC_ARGS_MACRO <nbargs> <vect>) not variable <nbargs>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:561:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:561:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:558:/ quasiblock");


                    /*_.PROGN___V22*/
                    meltfptr[21] = /*_.RETURN___V21*/ meltfptr[20];;
                    /*^compute*/
                    /*_._IF___V20*/
                    meltfptr[19] = /*_.PROGN___V22*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:558:/ clear");
                    /*clear*/ /*_.RETURN___V21*/
                    meltfptr[20] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V22*/
                    meltfptr[21] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V20*/ meltfptr[19] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L8*/
        meltfnum[7] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MACROVARVECT__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-base.melt:562:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:563:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), ( "(DIAGNOSTIC_ARGS_MACRO <nbargs> <vect>) not variable <vect>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:565:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:565:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:562:/ quasiblock");


                    /*_.PROGN___V25*/
                    meltfptr[24] = /*_.RETURN___V24*/ meltfptr[21];;
                    /*^compute*/
                    /*_._IF___V23*/
                    meltfptr[20] = /*_.PROGN___V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:562:/ clear");
                    /*clear*/ /*_.RETURN___V24*/
                    meltfptr[21] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V25*/
                    meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V23*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:566:/ quasiblock");


        /*_.HOOK_MELT_MAKE_LOCATION__V27*/
        meltfptr[24] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 567) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V28*/
        meltfptr[27] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 569) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V29*/
        meltfptr[28] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 570) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V30*/
        meltfptr[29] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 571) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:571:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_10_EXIT*/ meltfrout->tabval[10]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_8_ARGLOOP*/ meltfrout->tabval[8]);
            /*_.MELT_MAKE_SEXPR__V31*/
            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V30*/ meltfptr[29]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:570:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V31*/ meltfptr[30];
            /*_.MELT_MAKE_SEXPR__V32*/
            meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V29*/ meltfptr[28]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V33*/
        meltfptr[32] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 572) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V34*/
        meltfptr[33] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 572) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:572:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_CSTRING*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_13_STR*/ meltfrout->tabval[13]);
            /*_.MELT_MAKE_SEXPR__V35*/
            meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V34*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V36*/
        meltfptr[35] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 573) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V37*/
        meltfptr[36] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 573) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V38*/
        meltfptr[37] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 573) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V39*/
        meltfptr[38] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 573) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:573:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_16_CONSTANT_BOX*/ meltfrout->tabval[16]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_13_STR*/ meltfrout->tabval[13]);
            /*_.MELT_MAKE_SEXPR__V40*/
            meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V39*/ meltfptr[38]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_BS*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V40*/ meltfptr[39];
            /*_.MELT_MAKE_SEXPR__V41*/
            meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V38*/ meltfptr[37]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V41*/ meltfptr[40];
            /*_.MELT_MAKE_SEXPR__V42*/
            meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V37*/ meltfptr[36]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V43*/
        meltfptr[42] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 575) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:575:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_BS*/ meltfrout->tabval[15]);
            /*_.MELT_MAKE_SEXPR__V44*/
            meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V43*/ meltfptr[42]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V45*/
        meltfptr[44] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 576) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V46*/
        meltfptr[45] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 576) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:576:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V47*/
            meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V46*/ meltfptr[45]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V47*/ meltfptr[46];
            /*_.MELT_MAKE_SEXPR__V48*/
            meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V45*/ meltfptr[44]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:573:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V42*/ meltfptr[41];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V44*/ meltfptr[43];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V48*/ meltfptr[47];
            /*_.MELT_MAKE_SEXPR__V49*/
            meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V36*/ meltfptr[35]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V50*/
        meltfptr[49] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 577) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:577:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V51*/
            meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V50*/ meltfptr[49]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:572:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V35*/ meltfptr[34];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V49*/ meltfptr[48];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V51*/ meltfptr[50];
            /*_.MELT_MAKE_SEXPR__V52*/
            meltfptr[51] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V33*/ meltfptr[32]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V53*/
        meltfptr[52] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 579) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V54*/
        meltfptr[53] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 579) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:579:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_LONG*/ meltfrout->tabval[23]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_24_L*/ meltfrout->tabval[24]);
            /*_.MELT_MAKE_SEXPR__V55*/
            meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V54*/ meltfptr[53]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V56*/
        meltfptr[55] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 580) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V57*/
        meltfptr[56] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 580) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V58*/
        meltfptr[57] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 580) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V59*/
        meltfptr[58] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 580) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:580:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_27_STRING4OUT*/ meltfrout->tabval[27]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_28_DISCR_STRING*/ meltfrout->tabval[28]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_24_L*/ meltfrout->tabval[24]);
            /*_.MELT_MAKE_SEXPR__V60*/
            meltfptr[59] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V59*/ meltfptr[58]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_26_SN*/ meltfrout->tabval[26]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V60*/ meltfptr[59];
            /*_.MELT_MAKE_SEXPR__V61*/
            meltfptr[60] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V58*/ meltfptr[57]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V61*/ meltfptr[60];
            /*_.MELT_MAKE_SEXPR__V62*/
            meltfptr[61] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V57*/ meltfptr[56]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V63*/
        meltfptr[62] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 582) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:582:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_26_SN*/ meltfrout->tabval[26]);
            /*_.MELT_MAKE_SEXPR__V64*/
            meltfptr[63] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V65*/
        meltfptr[64] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 583) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V66*/
        meltfptr[65] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 583) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:583:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V67*/
            meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V66*/ meltfptr[65]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66];
            /*_.MELT_MAKE_SEXPR__V68*/
            meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V65*/ meltfptr[64]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V62*/ meltfptr[61];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V64*/ meltfptr[63];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V68*/ meltfptr[67];
            /*_.MELT_MAKE_SEXPR__V69*/
            meltfptr[68] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V56*/ meltfptr[55]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V70*/
        meltfptr[69] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 584) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:584:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V71*/
            meltfptr[70] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V70*/ meltfptr[69]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V55*/ meltfptr[54];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V69*/ meltfptr[68];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V71*/ meltfptr[70];
            /*_.MELT_MAKE_SEXPR__V72*/
            meltfptr[71] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V53*/ meltfptr[52]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V73*/
        meltfptr[72] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 586) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V74*/
        meltfptr[73] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 586) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:586:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_TREE*/ meltfrout->tabval[35]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_36_TR*/ meltfrout->tabval[36]);
            /*_.MELT_MAKE_SEXPR__V75*/
            meltfptr[74] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V74*/ meltfptr[73]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V76*/
        meltfptr[75] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 587) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V77*/
        meltfptr[76] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 587) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:587:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_IS_CLOSURE*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_DIAGV_TREE*/ meltfrout->tabval[39]);
            /*_.MELT_MAKE_SEXPR__V78*/
            meltfptr[77] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V77*/ meltfptr[76]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_ASSERT_MSG*/ meltfrout->tabval[37]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_tree";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V78*/ meltfptr[77];
            /*_.MELT_MAKE_SEXPR__V79*/
            meltfptr[78] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V76*/ meltfptr[75]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V80*/
        meltfptr[79] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 588) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V81*/
        meltfptr[80] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 588) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V82*/
        meltfptr[81] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 588) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V83*/
        meltfptr[82] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 588) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:588:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_DIAGV_TREE*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_36_TR*/ meltfrout->tabval[36]);
            /*_.MELT_MAKE_SEXPR__V84*/
            meltfptr[83] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V83*/ meltfptr[82]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V84*/ meltfptr[83];
            /*_.MELT_MAKE_SEXPR__V85*/
            meltfptr[84] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V82*/ meltfptr[81]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V85*/ meltfptr[84];
            /*_.MELT_MAKE_SEXPR__V86*/
            meltfptr[85] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V81*/ meltfptr[80]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V87*/
        meltfptr[86] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 590) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:590:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*_.MELT_MAKE_SEXPR__V88*/
            meltfptr[87] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V87*/ meltfptr[86]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V89*/
        meltfptr[88] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 591) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V90*/
        meltfptr[89] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 591) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:591:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V91*/
            meltfptr[90] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V90*/ meltfptr[89]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V91*/ meltfptr[90];
            /*_.MELT_MAKE_SEXPR__V92*/
            meltfptr[91] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V89*/ meltfptr[88]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:588:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V86*/ meltfptr[85];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V88*/ meltfptr[87];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V92*/ meltfptr[91];
            /*_.MELT_MAKE_SEXPR__V93*/
            meltfptr[92] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V80*/ meltfptr[79]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V94*/
        meltfptr[93] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 592) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:592:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V95*/
            meltfptr[94] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V94*/ meltfptr[93]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:586:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V75*/ meltfptr[74];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V79*/ meltfptr[78];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V93*/ meltfptr[92];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V95*/ meltfptr[94];
            /*_.MELT_MAKE_SEXPR__V96*/
            meltfptr[95] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V73*/ meltfptr[72]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V97*/
        meltfptr[96] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 594) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V98*/
        meltfptr[97] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 594) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:594:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_49_GIMPLE*/ meltfrout->tabval[49]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_GI*/ meltfrout->tabval[50]);
            /*_.MELT_MAKE_SEXPR__V99*/
            meltfptr[98] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V98*/ meltfptr[97]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V100*/
        meltfptr[99] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 595) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V101*/
        meltfptr[100] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 595) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:595:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_IS_CLOSURE*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_53_DIAGV_GIMPLE*/ meltfrout->tabval[53]);
            /*_.MELT_MAKE_SEXPR__V102*/
            meltfptr[101] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V101*/ meltfptr[100]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_ASSERT_MSG*/ meltfrout->tabval[37]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_gimple";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V102*/ meltfptr[101];
            /*_.MELT_MAKE_SEXPR__V103*/
            meltfptr[102] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V100*/ meltfptr[99]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V104*/
        meltfptr[103] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V105*/
        meltfptr[104] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V106*/
        meltfptr[105] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V107*/
        meltfptr[106] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 596) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:596:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_53_DIAGV_GIMPLE*/ meltfrout->tabval[53]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_GI*/ meltfrout->tabval[50]);
            /*_.MELT_MAKE_SEXPR__V108*/
            meltfptr[107] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V107*/ meltfptr[106]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V108*/ meltfptr[107];
            /*_.MELT_MAKE_SEXPR__V109*/
            meltfptr[108] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V106*/ meltfptr[105]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V109*/ meltfptr[108];
            /*_.MELT_MAKE_SEXPR__V110*/
            meltfptr[109] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V105*/ meltfptr[104]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V111*/
        meltfptr[110] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 598) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:598:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*_.MELT_MAKE_SEXPR__V112*/
            meltfptr[111] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V111*/ meltfptr[110]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V113*/
        meltfptr[112] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 599) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V114*/
        meltfptr[113] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 599) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:599:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V115*/
            meltfptr[114] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V114*/ meltfptr[113]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V115*/ meltfptr[114];
            /*_.MELT_MAKE_SEXPR__V116*/
            meltfptr[115] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V113*/ meltfptr[112]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:596:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V110*/ meltfptr[109];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V112*/ meltfptr[111];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V116*/ meltfptr[115];
            /*_.MELT_MAKE_SEXPR__V117*/
            meltfptr[116] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V104*/ meltfptr[103]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V118*/
        meltfptr[117] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 600) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:600:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V119*/
            meltfptr[118] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V118*/ meltfptr[117]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:594:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V99*/ meltfptr[98];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V103*/ meltfptr[102];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V117*/ meltfptr[116];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V119*/ meltfptr[118];
            /*_.MELT_MAKE_SEXPR__V120*/
            meltfptr[119] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V97*/ meltfptr[96]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V121*/
        meltfptr[120] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 602) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V122*/
        meltfptr[121] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 602) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:602:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_63_GIMPLE_SEQ*/ meltfrout->tabval[63]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_64_GS*/ meltfrout->tabval[64]);
            /*_.MELT_MAKE_SEXPR__V123*/
            meltfptr[122] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V122*/ meltfptr[121]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V124*/
        meltfptr[123] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 603) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V125*/
        meltfptr[124] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 603) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:603:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_IS_CLOSURE*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_67_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[67]);
            /*_.MELT_MAKE_SEXPR__V126*/
            meltfptr[125] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V125*/ meltfptr[124]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_ASSERT_MSG*/ meltfrout->tabval[37]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_gimple_seq";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V126*/ meltfptr[125];
            /*_.MELT_MAKE_SEXPR__V127*/
            meltfptr[126] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V124*/ meltfptr[123]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V128*/
        meltfptr[127] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 604) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V129*/
        meltfptr[128] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 604) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V130*/
        meltfptr[129] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 604) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V131*/
        meltfptr[130] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 604) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:604:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_67_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[67]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_64_GS*/ meltfrout->tabval[64]);
            /*_.MELT_MAKE_SEXPR__V132*/
            meltfptr[131] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V131*/ meltfptr[130]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V132*/ meltfptr[131];
            /*_.MELT_MAKE_SEXPR__V133*/
            meltfptr[132] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V130*/ meltfptr[129]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V133*/ meltfptr[132];
            /*_.MELT_MAKE_SEXPR__V134*/
            meltfptr[133] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V129*/ meltfptr[128]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V135*/
        meltfptr[134] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 606) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:606:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*_.MELT_MAKE_SEXPR__V136*/
            meltfptr[135] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V135*/ meltfptr[134]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V137*/
        meltfptr[136] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 607) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V138*/
        meltfptr[137] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 607) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:607:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V139*/
            meltfptr[138] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V138*/ meltfptr[137]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V139*/ meltfptr[138];
            /*_.MELT_MAKE_SEXPR__V140*/
            meltfptr[139] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V137*/ meltfptr[136]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:604:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V134*/ meltfptr[133];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V136*/ meltfptr[135];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V140*/ meltfptr[139];
            /*_.MELT_MAKE_SEXPR__V141*/
            meltfptr[140] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V128*/ meltfptr[127]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V142*/
        meltfptr[141] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 608) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:608:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V143*/
            meltfptr[142] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V142*/ meltfptr[141]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:602:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V123*/ meltfptr[122];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V127*/ meltfptr[126];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V141*/ meltfptr[140];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V143*/ meltfptr[142];
            /*_.MELT_MAKE_SEXPR__V144*/
            meltfptr[143] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V121*/ meltfptr[120]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V145*/
        meltfptr[144] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 610) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V146*/
        meltfptr[145] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 610) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:610:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_77_BASIC_BLOCK*/ meltfrout->tabval[77]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_78_BB*/ meltfrout->tabval[78]);
            /*_.MELT_MAKE_SEXPR__V147*/
            meltfptr[146] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V146*/ meltfptr[145]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V148*/
        meltfptr[147] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 611) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V149*/
        meltfptr[148] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 611) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:611:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_38_IS_CLOSURE*/ meltfrout->tabval[38]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_81_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[81]);
            /*_.MELT_MAKE_SEXPR__V150*/
            meltfptr[149] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V149*/ meltfptr[148]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_ASSERT_MSG*/ meltfrout->tabval[37]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_basic_block";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V150*/ meltfptr[149];
            /*_.MELT_MAKE_SEXPR__V151*/
            meltfptr[150] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V148*/ meltfptr[147]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V152*/
        meltfptr[151] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 612) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V153*/
        meltfptr[152] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 612) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V154*/
        meltfptr[153] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 612) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V155*/
        meltfptr[154] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 612) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:612:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_81_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[81]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_78_BB*/ meltfrout->tabval[78]);
            /*_.MELT_MAKE_SEXPR__V156*/
            meltfptr[155] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V155*/ meltfptr[154]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V156*/ meltfptr[155];
            /*_.MELT_MAKE_SEXPR__V157*/
            meltfptr[156] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V154*/ meltfptr[153]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V157*/ meltfptr[156];
            /*_.MELT_MAKE_SEXPR__V158*/
            meltfptr[157] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V153*/ meltfptr[152]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V159*/
        meltfptr[158] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 614) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:614:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_S*/ meltfrout->tabval[41]);
            /*_.MELT_MAKE_SEXPR__V160*/
            meltfptr[159] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V159*/ meltfptr[158]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V161*/
        meltfptr[160] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 615) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V162*/
        meltfptr[161] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 615) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:615:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V163*/
            meltfptr[162] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V162*/ meltfptr[161]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V163*/ meltfptr[162];
            /*_.MELT_MAKE_SEXPR__V164*/
            meltfptr[163] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V161*/ meltfptr[160]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:612:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V158*/ meltfptr[157];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V160*/ meltfptr[159];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V164*/ meltfptr[163];
            /*_.MELT_MAKE_SEXPR__V165*/
            meltfptr[164] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V152*/ meltfptr[151]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V166*/
        meltfptr[165] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 616) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:616:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V167*/
            meltfptr[166] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V166*/ meltfptr[165]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:610:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V147*/ meltfptr[146];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V151*/ meltfptr[150];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V165*/ meltfptr[164];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V167*/ meltfptr[166];
            /*_.MELT_MAKE_SEXPR__V168*/
            meltfptr[167] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V145*/ meltfptr[144]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V169*/
        meltfptr[168] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 618) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V170*/
        meltfptr[169] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 618) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:618:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_91_VALUE*/ meltfrout->tabval[91]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*_.MELT_MAKE_SEXPR__V171*/
            meltfptr[170] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V170*/ meltfptr[169]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V172*/
        meltfptr[171] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 619) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V173*/
        meltfptr[172] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 620) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V174*/
        meltfptr[173] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 620) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:620:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_94_IS_INTEGERBOX*/ meltfrout->tabval[94]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*_.MELT_MAKE_SEXPR__V175*/
            meltfptr[174] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V174*/ meltfptr[173]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V176*/
        meltfptr[175] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 621) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V177*/
        meltfptr[176] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 621) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V178*/
        meltfptr[177] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 621) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V179*/
        meltfptr[178] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 621) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V180*/
        meltfptr[179] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 621) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:621:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_100_GET_INT*/ meltfrout->tabval[100]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*_.MELT_MAKE_SEXPR__V181*/
            meltfptr[180] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V180*/ meltfptr[179]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_27_STRING4OUT*/ meltfrout->tabval[27]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_28_DISCR_STRING*/ meltfrout->tabval[28]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V181*/ meltfptr[180];
            /*_.MELT_MAKE_SEXPR__V182*/
            meltfptr[181] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V179*/ meltfptr[178]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_26_SN*/ meltfrout->tabval[26]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V182*/ meltfptr[181];
            /*_.MELT_MAKE_SEXPR__V183*/
            meltfptr[182] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V178*/ meltfptr[177]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V183*/ meltfptr[182];
            /*_.MELT_MAKE_SEXPR__V184*/
            meltfptr[183] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V177*/ meltfptr[176]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V185*/
        meltfptr[184] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 623) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:623:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_26_SN*/ meltfrout->tabval[26]);
            /*_.MELT_MAKE_SEXPR__V186*/
            meltfptr[185] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V185*/ meltfptr[184]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V187*/
        meltfptr[186] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 624) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V188*/
        meltfptr[187] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 624) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:624:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V189*/
            meltfptr[188] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V188*/ meltfptr[187]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V189*/ meltfptr[188];
            /*_.MELT_MAKE_SEXPR__V190*/
            meltfptr[189] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V187*/ meltfptr[186]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:621:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V184*/ meltfptr[183];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V186*/ meltfptr[185];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V190*/ meltfptr[189];
            /*_.MELT_MAKE_SEXPR__V191*/
            meltfptr[190] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V176*/ meltfptr[175]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V192*/
        meltfptr[191] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 625) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:625:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V193*/
            meltfptr[192] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V192*/ meltfptr[191]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:620:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V175*/ meltfptr[174];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V191*/ meltfptr[190];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V193*/ meltfptr[192];
            /*_.MELT_MAKE_SEXPR__V194*/
            meltfptr[193] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V173*/ meltfptr[172]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V195*/
        meltfptr[194] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 627) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V196*/
        meltfptr[195] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 627) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:627:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_107_IS_STRING*/ meltfrout->tabval[107]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*_.MELT_MAKE_SEXPR__V197*/
            meltfptr[196] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V196*/ meltfptr[195]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V198*/
        meltfptr[197] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 628) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:628:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*_.MELT_MAKE_SEXPR__V199*/
            meltfptr[198] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V198*/ meltfptr[197]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V200*/
        meltfptr[199] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 629) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V201*/
        meltfptr[200] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 629) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:629:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V202*/
            meltfptr[201] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V201*/ meltfptr[200]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V202*/ meltfptr[201];
            /*_.MELT_MAKE_SEXPR__V203*/
            meltfptr[202] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V200*/ meltfptr[199]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V204*/
        meltfptr[203] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 630) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:630:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V205*/
            meltfptr[204] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V204*/ meltfptr[203]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:627:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V197*/ meltfptr[196];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V199*/ meltfptr[198];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V203*/ meltfptr[202];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V205*/ meltfptr[204];
            /*_.MELT_MAKE_SEXPR__V206*/
            meltfptr[205] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V195*/ meltfptr[194]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V207*/
        meltfptr[206] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 632) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V208*/
        meltfptr[207] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 632) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:632:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_114_IS_A*/ meltfrout->tabval[114]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_116_CLASS_NAMED*/ meltfrout->tabval[116]);
            /*_.MELT_MAKE_SEXPR__V209*/
            meltfptr[208] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V208*/ meltfptr[207]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V210*/
        meltfptr[209] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 633) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V211*/
        meltfptr[210] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 633) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V212*/
        meltfptr[211] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 633) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V213*/
        meltfptr[212] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 633) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:633:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_119_UNSAFE_GET_FIELD*/ meltfrout->tabval[119]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_120_NAMED_NAME*/ meltfrout->tabval[120]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_92_V*/ meltfrout->tabval[92]);
            /*_.MELT_MAKE_SEXPR__V214*/
            meltfptr[213] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V213*/ meltfptr[212]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_118_NAM*/ meltfrout->tabval[118]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V214*/ meltfptr[213];
            /*_.MELT_MAKE_SEXPR__V215*/
            meltfptr[214] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V212*/ meltfptr[211]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V215*/ meltfptr[214];
            /*_.MELT_MAKE_SEXPR__V216*/
            meltfptr[215] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V211*/ meltfptr[210]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V217*/
        meltfptr[216] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 635) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V218*/
        meltfptr[217] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 635) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:635:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_107_IS_STRING*/ meltfrout->tabval[107]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_118_NAM*/ meltfrout->tabval[118]);
            /*_.MELT_MAKE_SEXPR__V219*/
            meltfptr[218] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V218*/ meltfptr[217]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_ASSERT_MSG*/ meltfrout->tabval[37]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check nam";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V219*/ meltfptr[218];
            /*_.MELT_MAKE_SEXPR__V220*/
            meltfptr[219] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V217*/ meltfptr[216]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V221*/
        meltfptr[220] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 636) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:636:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MULTIPLE_PUT_NTH*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_118_NAM*/ meltfrout->tabval[118]);
            /*_.MELT_MAKE_SEXPR__V222*/
            meltfptr[221] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V221*/ meltfptr[220]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V223*/
        meltfptr[222] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 637) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V224*/
        meltfptr[223] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 637) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:637:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_pl*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V225*/
            meltfptr[224] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V224*/ meltfptr[223]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_SETQ*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V18*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V225*/ meltfptr[224];
            /*_.MELT_MAKE_SEXPR__V226*/
            meltfptr[225] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V223*/ meltfptr[222]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:633:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V216*/ meltfptr[215];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V220*/ meltfptr[219];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V222*/ meltfptr[221];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V226*/ meltfptr[225];
            /*_.MELT_MAKE_SEXPR__V227*/
            meltfptr[226] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V210*/ meltfptr[209]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V228*/
        meltfptr[227] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 638) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:638:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V229*/
            meltfptr[228] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V228*/ meltfptr[227]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:632:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V209*/ meltfptr[208];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V227*/ meltfptr[226];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V229*/ meltfptr[228];
            /*_.MELT_MAKE_SEXPR__V230*/
            meltfptr[229] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V207*/ meltfptr[206]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_93_COND*/ meltfrout->tabval[93]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V194*/ meltfptr[193];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V206*/ meltfptr[205];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V230*/ meltfptr[229];
            /*_.MELT_MAKE_SEXPR__V231*/
            meltfptr[230] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V172*/ meltfptr[171]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:618:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V171*/ meltfptr[170];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V231*/ meltfptr[230];
            /*_.MELT_MAKE_SEXPR__V232*/
            meltfptr[231] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V169*/ meltfptr[168]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V233*/
        meltfptr[232] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 642) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V234*/
        meltfptr[233] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 643) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V235*/
        meltfptr[234] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 643) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V236*/
        meltfptr[235] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 643) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V237*/
        meltfptr[236] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 643) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:643:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_133_VARIADIC_CTYPE*/ meltfrout->tabval[133]);
            /*^apply.arg*/
            argtab[1].meltbp_long = 0;
            /*_.MELT_MAKE_SEXPR__V238*/
            meltfptr[237] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V237*/ meltfptr[236]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_132_VCTY*/ meltfrout->tabval[132]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V238*/ meltfptr[237];
            /*_.MELT_MAKE_SEXPR__V239*/
            meltfptr[238] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V236*/ meltfptr[235]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V239*/ meltfptr[238];
            /*_.MELT_MAKE_SEXPR__V240*/
            meltfptr[239] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V235*/ meltfptr[234]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V241*/
        meltfptr[240] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 645) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V242*/
        meltfptr[241] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 646) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:646:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_135_GET_FIELD*/ meltfrout->tabval[135]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_120_NAMED_NAME*/ meltfrout->tabval[120]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_132_VCTY*/ meltfrout->tabval[132]);
            /*_.MELT_MAKE_SEXPR__V243*/
            meltfptr[242] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V242*/ meltfptr[241]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:645:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_134_ERRORMSG_STRV*/ meltfrout->tabval[134]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "DIAGNOSTIC_ARGS_MACRO for unsupported ctype";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V243*/ meltfptr[242];
            /*_.MELT_MAKE_SEXPR__V244*/
            meltfptr[243] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V241*/ meltfptr[240]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V245*/
        meltfptr[244] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 647) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:647:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_ASSERT_MSG*/ meltfrout->tabval[37]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "invalid variadic argument to DIAGNOSTIC_ARGS_MACRO";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*_.MELT_MAKE_SEXPR__V246*/
            meltfptr[245] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V245*/ meltfptr[244]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:643:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_LET*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V240*/ meltfptr[239];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V244*/ meltfptr[243];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V246*/ meltfptr[245];
            /*_.MELT_MAKE_SEXPR__V247*/
            meltfptr[246] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V234*/ meltfptr[233]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V248*/
        meltfptr[247] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[6]),  "warmelt-base.melt", 648) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:648:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VOID*/ meltfrout->tabval[22]);
            /*_.MELT_MAKE_SEXPR__V249*/
            meltfptr[248] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V248*/ meltfptr[247]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:642:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_130_ELSE*/ meltfrout->tabval[130]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V247*/ meltfptr[246];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V249*/ meltfptr[248];
            /*_.MELT_MAKE_SEXPR__V250*/
            meltfptr[249] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V233*/ meltfptr[232]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:569:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[10];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_9_VARIADIC*/ meltfrout->tabval[9]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V32*/ meltfptr[31];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V52*/ meltfptr[51];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V72*/ meltfptr[71];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V96*/ meltfptr[95];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V120*/ meltfptr[119];
            /*^apply.arg*/
            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V144*/ meltfptr[143];
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V168*/ meltfptr[167];
            /*^apply.arg*/
            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V232*/ meltfptr[231];
            /*^apply.arg*/
            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V250*/ meltfptr[249];
            /*_.MELT_MAKE_SEXPR__V251*/
            meltfptr[250] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V28*/ meltfptr[27]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:567:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_7_FOREVER*/ meltfrout->tabval[7]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_8_ARGLOOP*/ meltfrout->tabval[8]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V251*/ meltfptr[250];
            /*_.RESEXP__V252*/
            meltfptr[251] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V27*/ meltfptr[24]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:653:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L9*/
            meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:653:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L9*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:653:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 653;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro resexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESEXP__V252*/ meltfptr[251];
                            /*_.MELT_DEBUG_FUN__V254*/
                            meltfptr[253] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V253*/
                        meltfptr[252] = /*_.MELT_DEBUG_FUN__V254*/ meltfptr[253];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:653:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L10*/
                        meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V254*/
                        meltfptr[253] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V253*/ meltfptr[252] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:653:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L9*/
            meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V253*/
            meltfptr[252] = 0 ;
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
        MELT_LOCATION("warmelt-base.melt:654:/ quasiblock");



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
            /*_.MEXP__V256*/
            meltfptr[252] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.RESEXP__V252*/ meltfptr[251]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:656:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/
            meltfnum[9] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:656:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L12*/ meltfnum[8] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:656:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 656;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro final mexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXP__V256*/ meltfptr[252];
                            /*_.MELT_DEBUG_FUN__V258*/
                            meltfptr[257] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V257*/
                        meltfptr[256] = /*_.MELT_DEBUG_FUN__V258*/ meltfptr[257];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:656:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V258*/
                        meltfptr[257] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V257*/ meltfptr[256] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:656:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V257*/
            meltfptr[256] = 0 ;
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
        MELT_LOCATION("warmelt-base.melt:657:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.MEXP__V256*/ meltfptr[252]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.MEXP__V256*/ meltfptr[252])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.MEXP__V256*/ meltfptr[252]), (1), (/*_.SLOC__V9*/ meltfptr[5]), "LOCA_LOCATION");
                    ;
                    /*^touch*/
                    meltgc_touch(/*_.MEXP__V256*/ meltfptr[252]);
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.MEXP__V256*/ meltfptr[252], "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:658:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MEXP__V256*/ meltfptr[252];;

        {
            MELT_LOCATION("warmelt-base.melt:658:/ locexp");
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
        /*_.LET___V255*/
        meltfptr[253] = /*_.RETURN___V259*/ meltfptr[257];;

        MELT_LOCATION("warmelt-base.melt:654:/ clear");
        /*clear*/ /*_.MEXP__V256*/
        meltfptr[252] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V259*/
        meltfptr[257] = 0 ;
        /*_.LET___V26*/
        meltfptr[21] = /*_.LET___V255*/ meltfptr[253];;

        MELT_LOCATION("warmelt-base.melt:566:/ clear");
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V27*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V33*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V35*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V36*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V38*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V39*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V40*/
        meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V41*/
        meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V42*/
        meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V43*/
        meltfptr[42] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V44*/
        meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V45*/
        meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V46*/
        meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V47*/
        meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V48*/
        meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V49*/
        meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V50*/
        meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V51*/
        meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V52*/
        meltfptr[51] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V53*/
        meltfptr[52] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V54*/
        meltfptr[53] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V55*/
        meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V56*/
        meltfptr[55] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V57*/
        meltfptr[56] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V58*/
        meltfptr[57] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V59*/
        meltfptr[58] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V60*/
        meltfptr[59] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V61*/
        meltfptr[60] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V62*/
        meltfptr[61] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V63*/
        meltfptr[62] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V64*/
        meltfptr[63] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V65*/
        meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V66*/
        meltfptr[65] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V67*/
        meltfptr[66] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V68*/
        meltfptr[67] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V69*/
        meltfptr[68] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V70*/
        meltfptr[69] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V71*/
        meltfptr[70] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V72*/
        meltfptr[71] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V73*/
        meltfptr[72] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V74*/
        meltfptr[73] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V75*/
        meltfptr[74] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V76*/
        meltfptr[75] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V77*/
        meltfptr[76] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V78*/
        meltfptr[77] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V79*/
        meltfptr[78] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V80*/
        meltfptr[79] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V81*/
        meltfptr[80] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V82*/
        meltfptr[81] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V83*/
        meltfptr[82] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V84*/
        meltfptr[83] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V85*/
        meltfptr[84] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V86*/
        meltfptr[85] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V87*/
        meltfptr[86] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V88*/
        meltfptr[87] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V89*/
        meltfptr[88] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V90*/
        meltfptr[89] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V91*/
        meltfptr[90] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V92*/
        meltfptr[91] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V93*/
        meltfptr[92] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V94*/
        meltfptr[93] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V95*/
        meltfptr[94] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V96*/
        meltfptr[95] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V97*/
        meltfptr[96] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V98*/
        meltfptr[97] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V99*/
        meltfptr[98] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V100*/
        meltfptr[99] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V101*/
        meltfptr[100] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V102*/
        meltfptr[101] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V103*/
        meltfptr[102] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V104*/
        meltfptr[103] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V105*/
        meltfptr[104] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V106*/
        meltfptr[105] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V107*/
        meltfptr[106] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V108*/
        meltfptr[107] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V109*/
        meltfptr[108] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V110*/
        meltfptr[109] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V111*/
        meltfptr[110] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V112*/
        meltfptr[111] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V113*/
        meltfptr[112] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V114*/
        meltfptr[113] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V115*/
        meltfptr[114] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V116*/
        meltfptr[115] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V117*/
        meltfptr[116] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V118*/
        meltfptr[117] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V119*/
        meltfptr[118] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V120*/
        meltfptr[119] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V121*/
        meltfptr[120] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V122*/
        meltfptr[121] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V123*/
        meltfptr[122] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V124*/
        meltfptr[123] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V125*/
        meltfptr[124] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V126*/
        meltfptr[125] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V127*/
        meltfptr[126] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V128*/
        meltfptr[127] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V129*/
        meltfptr[128] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V130*/
        meltfptr[129] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V131*/
        meltfptr[130] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V132*/
        meltfptr[131] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V133*/
        meltfptr[132] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V134*/
        meltfptr[133] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V135*/
        meltfptr[134] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V136*/
        meltfptr[135] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V137*/
        meltfptr[136] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V138*/
        meltfptr[137] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V139*/
        meltfptr[138] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V140*/
        meltfptr[139] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V141*/
        meltfptr[140] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V142*/
        meltfptr[141] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V143*/
        meltfptr[142] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V144*/
        meltfptr[143] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V145*/
        meltfptr[144] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V146*/
        meltfptr[145] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V147*/
        meltfptr[146] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V148*/
        meltfptr[147] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V149*/
        meltfptr[148] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V150*/
        meltfptr[149] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V151*/
        meltfptr[150] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V152*/
        meltfptr[151] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V153*/
        meltfptr[152] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V154*/
        meltfptr[153] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V155*/
        meltfptr[154] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V156*/
        meltfptr[155] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V157*/
        meltfptr[156] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V158*/
        meltfptr[157] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V159*/
        meltfptr[158] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V160*/
        meltfptr[159] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V161*/
        meltfptr[160] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V162*/
        meltfptr[161] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V163*/
        meltfptr[162] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V164*/
        meltfptr[163] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V165*/
        meltfptr[164] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V166*/
        meltfptr[165] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V167*/
        meltfptr[166] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V168*/
        meltfptr[167] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V169*/
        meltfptr[168] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V170*/
        meltfptr[169] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V171*/
        meltfptr[170] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V172*/
        meltfptr[171] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V173*/
        meltfptr[172] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V174*/
        meltfptr[173] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V175*/
        meltfptr[174] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V176*/
        meltfptr[175] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V177*/
        meltfptr[176] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V178*/
        meltfptr[177] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V179*/
        meltfptr[178] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V180*/
        meltfptr[179] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V181*/
        meltfptr[180] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V182*/
        meltfptr[181] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V183*/
        meltfptr[182] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V184*/
        meltfptr[183] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V185*/
        meltfptr[184] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V186*/
        meltfptr[185] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V187*/
        meltfptr[186] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V188*/
        meltfptr[187] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V189*/
        meltfptr[188] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V190*/
        meltfptr[189] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V191*/
        meltfptr[190] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V192*/
        meltfptr[191] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V193*/
        meltfptr[192] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V194*/
        meltfptr[193] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V195*/
        meltfptr[194] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V196*/
        meltfptr[195] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V197*/
        meltfptr[196] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V198*/
        meltfptr[197] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V199*/
        meltfptr[198] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V200*/
        meltfptr[199] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V201*/
        meltfptr[200] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V202*/
        meltfptr[201] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V203*/
        meltfptr[202] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V204*/
        meltfptr[203] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V205*/
        meltfptr[204] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V206*/
        meltfptr[205] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V207*/
        meltfptr[206] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V208*/
        meltfptr[207] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V209*/
        meltfptr[208] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V210*/
        meltfptr[209] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V211*/
        meltfptr[210] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V212*/
        meltfptr[211] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V213*/
        meltfptr[212] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V214*/
        meltfptr[213] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V215*/
        meltfptr[214] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V216*/
        meltfptr[215] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V217*/
        meltfptr[216] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V218*/
        meltfptr[217] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V219*/
        meltfptr[218] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V220*/
        meltfptr[219] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V221*/
        meltfptr[220] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V222*/
        meltfptr[221] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V223*/
        meltfptr[222] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V224*/
        meltfptr[223] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V225*/
        meltfptr[224] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V226*/
        meltfptr[225] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V227*/
        meltfptr[226] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V228*/
        meltfptr[227] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V229*/
        meltfptr[228] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V230*/
        meltfptr[229] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V231*/
        meltfptr[230] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V232*/
        meltfptr[231] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V233*/
        meltfptr[232] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V234*/
        meltfptr[233] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V235*/
        meltfptr[234] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V236*/
        meltfptr[235] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V237*/
        meltfptr[236] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V238*/
        meltfptr[237] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V239*/
        meltfptr[238] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V240*/
        meltfptr[239] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V241*/
        meltfptr[240] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V242*/
        meltfptr[241] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V243*/
        meltfptr[242] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V244*/
        meltfptr[243] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V245*/
        meltfptr[244] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V246*/
        meltfptr[245] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V247*/
        meltfptr[246] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V248*/
        meltfptr[247] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V249*/
        meltfptr[248] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V250*/
        meltfptr[249] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V251*/
        meltfptr[250] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESEXP__V252*/
        meltfptr[251] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V255*/
        meltfptr[253] = 0 ;
        /*_.LET___V17*/
        meltfptr[11] = /*_.LET___V26*/ meltfptr[21];;

        MELT_LOCATION("warmelt-base.melt:555:/ clear");
        /*clear*/ /*_.MACROVARNBARG__V18*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROVARVECT__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V23*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V26*/
        meltfptr[21] = 0 ;
        /*_.LET___V8*/
        meltfptr[6] = /*_.LET___V17*/ meltfptr[11];;

        MELT_LOCATION("warmelt-base.melt:545:/ clear");
        /*clear*/ /*_.SLOC__V9*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SCONT__V10*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TCONT__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MULTIPLE_LENGTH__L5*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#exeqI__L6*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[11] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:543:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V8*/
        meltfptr[6] = 0 ;
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

    /* start of frame for routine meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1716 */

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
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA() //the constructor fromline 1606
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA


    /** end of frame for meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1661**/

    /* end of frame for routine meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1720 */

    /* classy proc frame meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA */
    MeltFrame_meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_13_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPARE_NAMED_ALPHA", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1189:/ getarg");
    /*_.N1__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.N2__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        /*_#eqeq__L1*/
        meltfnum[0] =
            ((/*_.N1__V2*/ meltfptr[1]) == (/*_.N2__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-base.melt:1193:/ cond");
        /*cond*/
        if (/*_#eqeq__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:1194:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*!konst_0*/ meltfrout->tabval[0]);;
                    MELT_LOCATION("warmelt-base.melt:1194:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/
                    goto meltlabend_rout ;
                    /*_._IFELSE___V4*/
                    meltfptr[3] = /*_.RETURN___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1193:/ clear");
                    /*clear*/ /*_.RETURN___V5*/
                    meltfptr[4] = 0 ;
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
                    /*_#IS_NOT_A__L2*/
                    meltfnum[1] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-base.melt:1195:/ cond");
                    /*cond*/
                    if (/*_#IS_NOT_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-base.melt:1196:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*!konst_2*/ meltfrout->tabval[2]);;
                                MELT_LOCATION("warmelt-base.melt:1196:/ putxtraresult");
                                if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                ;
                                /*^finalreturn*/
                                ;
                                /*finalret*/
                                goto meltlabend_rout ;
                                /*_._IFELSE___V6*/
                                meltfptr[4] = /*_.RETURN___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1195:/ clear");
                                /*clear*/ /*_.RETURN___V7*/
                                meltfptr[6] = 0 ;
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
                                /*_#IS_NOT_A__L3*/
                                meltfnum[2] =
                                    !melt_is_instance_of((melt_ptr_t)(/*_.N2__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[1])));;
                                MELT_LOCATION("warmelt-base.melt:1197:/ cond");
                                /*cond*/
                                if (/*_#IS_NOT_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1198:/ quasiblock");


                                            /*_._RETVAL___V1*/
                                            meltfptr[0] = (/*!konst_3*/ meltfrout->tabval[3]);;
                                            MELT_LOCATION("warmelt-base.melt:1198:/ putxtraresult");
                                            if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                            if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                            if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                            ;
                                            /*^finalreturn*/
                                            ;
                                            /*finalret*/
                                            goto meltlabend_rout ;
                                            /*_._IFELSE___V8*/
                                            meltfptr[6] = /*_.RETURN___V9*/ meltfptr[8];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1197:/ clear");
                                            /*clear*/ /*_.RETURN___V9*/
                                            meltfptr[8] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1200:/ quasiblock");


                                            /*^getslot*/
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN1__V11*/
                                                meltfptr[10] = slot;
                                            };
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1201:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N2__V3*/ meltfptr[2]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN2__V12*/
                                                meltfptr[11] = slot;
                                            };
                                            ;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#STRINGlt__L4*/
                                            meltfnum[3] =
                                                melt_string_less((melt_ptr_t)(/*_.SN1__V11*/ meltfptr[10]), (melt_ptr_t)(/*_.SN2__V12*/ meltfptr[11]));;
                                            MELT_LOCATION("warmelt-base.melt:1203:/ cond");
                                            /*cond*/
                                            if (/*_#STRINGlt__L4*/ meltfnum[3]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1204:/ quasiblock");


                                                        /*_._RETVAL___V1*/
                                                        meltfptr[0] = (/*!konst_4*/ meltfrout->tabval[4]);;
                                                        MELT_LOCATION("warmelt-base.melt:1204:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/
                                                        goto meltlabend_rout ;
                                                        /*_._IFELSE___V13*/
                                                        meltfptr[12] = /*_.RETURN___V14*/ meltfptr[13];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1203:/ clear");
                                                        /*clear*/ /*_.RETURN___V14*/
                                                        meltfptr[13] = 0 ;
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
                                                        /*_#STRINGgt__L5*/
                                                        meltfnum[4] =
                                                            melt_string_less((melt_ptr_t)(/*_.SN2__V12*/ meltfptr[11]), (melt_ptr_t)(/*_.SN1__V11*/ meltfptr[10]));;
                                                        MELT_LOCATION("warmelt-base.melt:1205:/ cond");
                                                        /*cond*/
                                                        if (/*_#STRINGgt__L5*/ meltfnum[4]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1206:/ quasiblock");


                                                                    /*_._RETVAL___V1*/
                                                                    meltfptr[0] = (/*!konst_5*/ meltfrout->tabval[5]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1206:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/
                                                                    goto meltlabend_rout ;
                                                                    /*_._IFELSE___V15*/
                                                                    meltfptr[13] = /*_.RETURN___V16*/ meltfptr[15];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1205:/ clear");
                                                                    /*clear*/ /*_.RETURN___V16*/
                                                                    meltfptr[15] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-base.melt:1208:/ quasiblock");


                                                                    /*_._RETVAL___V1*/
                                                                    meltfptr[0] = (/*!konst_6*/ meltfrout->tabval[6]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1208:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/
                                                                    goto meltlabend_rout ;
                                                                    MELT_LOCATION("warmelt-base.melt:1207:/ quasiblock");


                                                                    /*_.PROGN___V18*/
                                                                    meltfptr[17] = /*_.RETURN___V17*/ meltfptr[15];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V15*/
                                                                    meltfptr[13] = /*_.PROGN___V18*/ meltfptr[17];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1205:/ clear");
                                                                    /*clear*/ /*_.RETURN___V17*/
                                                                    meltfptr[15] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V18*/
                                                                    meltfptr[17] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V13*/
                                                        meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[13];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1203:/ clear");
                                                        /*clear*/ /*_#STRINGgt__L5*/
                                                        meltfnum[4] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V15*/
                                                        meltfptr[13] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.LET___V10*/
                                            meltfptr[8] = /*_._IFELSE___V13*/ meltfptr[12];;

                                            MELT_LOCATION("warmelt-base.melt:1200:/ clear");
                                            /*clear*/ /*_.SN1__V11*/
                                            meltfptr[10] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.SN2__V12*/
                                            meltfptr[11] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#STRINGlt__L4*/
                                            meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V13*/
                                            meltfptr[12] = 0 ;
                                            MELT_LOCATION("warmelt-base.melt:1199:/ quasiblock");


                                            /*_.PROGN___V19*/
                                            meltfptr[15] = /*_.LET___V10*/ meltfptr[8];;
                                            /*^compute*/
                                            /*_._IFELSE___V8*/
                                            meltfptr[6] = /*_.PROGN___V19*/ meltfptr[15];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1197:/ clear");
                                            /*clear*/ /*_.LET___V10*/
                                            meltfptr[8] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V19*/
                                            meltfptr[15] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V6*/
                                meltfptr[4] = /*_._IFELSE___V8*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1195:/ clear");
                                /*clear*/ /*_#IS_NOT_A__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V8*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V4*/
                    meltfptr[3] = /*_._IFELSE___V6*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1193:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V6*/
                    meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1189:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:1189:/ locexp");
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
        /*clear*/ /*_#eqeq__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V4*/
        meltfptr[3] = 0 ;
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

    /* start of frame for routine meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1716 */

    /** start of frame for meltrout_14_WARMELTmiBASE_ADD2OUT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT// fromline 1531
        : public Melt_CallFrameWithValues<29>
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
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<29> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT), clos) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<29> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT)) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT


    /** end of frame for meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1661**/

    /* end of frame for routine meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1720 */

    /* classy proc frame meltrout_14_WARMELTmiBASE_ADD2OUT */
    MeltFrame_meltrout_14_WARMELTmiBASE_ADD2OUT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_14_WARMELTmiBASE_ADD2OUT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1504:/ getarg");
    /*_.OUT__V2*/
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
        /*_#IS_OUT__L1*/
        meltfnum[0] =
            (melt_is_out ((melt_ptr_t) /*_.OUT__V2*/ meltfptr[1])) ;;
        /*^compute*/
        /*_#NOT__L2*/
        meltfnum[1] =
            (!(/*_#IS_OUT__L1*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-base.melt:1508:/ cond");
        /*cond*/
        if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1509:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1509:/ locexp");
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

                    MELT_LOCATION("warmelt-base.melt:1508:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:1510:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_1:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1512:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_ADD2OUT_ix == variad_ADD2OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_ADD2OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1514:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V6*/
                            meltfptr[5] =  /*fromline 1341*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:1512:/ cond.else");

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

                                        /*consume variadic Value !*/
                                        variad_ADD2OUT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*_#IS_CLOSURE__L3*/
                                        meltfnum[2] =
                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V7*/ meltfptr[6])) == MELTOBMAG_CLOSURE);;
                                        MELT_LOCATION("warmelt-base.melt:1516:/ cond");
                                        /*cond*/
                                        if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1517:/ cond");
                                                    /*cond*/
                                                    if (/*ifvariadic arg#1*/ variad_ADD2OUT_ix>=0 && variad_ADD2OUT_ix + 1 <= variad_ADD2OUT_len && meltxargdescr_[variad_ADD2OUT_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.VV__V9*/ meltfptr[8] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_ADD2OUT_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/
                                                                variad_ADD2OUT_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:1519:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[1];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VV__V9*/ meltfptr[8];
                                                                    /*_.V__V10*/
                                                                    meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_._IFELSE___V8*/
                                                                meltfptr[7] = /*_.V__V10*/ meltfptr[9];;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                /*clear*/ /*_.VV__V9*/
                                                                meltfptr[8] = 0 ;
                                                                /*^clear*/
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

                                                                            /*consume variadic LONG !*/
                                                                            variad_ADD2OUT_ix += 1;;

                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            MELT_LOCATION("warmelt-base.melt:1521:/ apply");
                                                                            /*apply*/
                                                                            {
                                                                                union meltparam_un argtab[1];
                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                /*^apply.arg*/
                                                                                argtab[0].meltbp_long = /*_#LL__L4*/ meltfnum[3];
                                                                                /*_.V__V11*/
                                                                                meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                            }
                                                                            ;
                                                                            /*_._IFELSE___V8*/
                                                                            meltfptr[7] = /*_.V__V11*/ meltfptr[8];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                            /*clear*/ /*_#LL__L4*/
                                                                            meltfnum[3] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.V__V11*/
                                                                            meltfptr[8] = 0 ;
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

                                                                                        /*consume variadic CSTRING !*/
                                                                                        variad_ADD2OUT_ix += 1;;

                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        MELT_LOCATION("warmelt-base.melt:1523:/ apply");
                                                                                        /*apply*/
                                                                                        {
                                                                                            union meltparam_un argtab[1];
                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                            /*^apply.arg*/
                                                                                            argtab[0].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
                                                                                            /*_.V__V12*/
                                                                                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                        }
                                                                                        ;
                                                                                        /*_._IFELSE___V8*/
                                                                                        meltfptr[7] = /*_.V__V12*/ meltfptr[9];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                        /*clear*/ /*_?*/
                                                                                        meltfram__.loc_CSTRING__o0 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.V__V12*/
                                                                                        meltfptr[9] = 0 ;
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

                                                                                                    /*consume variadic TREE !*/
                                                                                                    variad_ADD2OUT_ix += 1;;

                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    MELT_LOCATION("warmelt-base.melt:1525:/ apply");
                                                                                                    /*apply*/
                                                                                                    {
                                                                                                        union meltparam_un argtab[1];
                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                        /*^apply.arg*/
                                                                                                        argtab[0].meltbp_tree = /*_?*/ meltfram__.loc_TREE__o1;
                                                                                                        /*_.V__V13*/
                                                                                                        meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un*)0);
                                                                                                    }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V8*/
                                                                                                    meltfptr[7] = /*_.V__V13*/ meltfptr[8];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                                    /*clear*/ /*_?*/
                                                                                                    meltfram__.loc_TREE__o1 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.V__V13*/
                                                                                                    meltfptr[8] = 0 ;
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

                                                                                                                /*consume variadic GIMPLE !*/
                                                                                                                variad_ADD2OUT_ix += 1;;

                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                MELT_LOCATION("warmelt-base.melt:1527:/ apply");
                                                                                                                /*apply*/
                                                                                                                {
                                                                                                                    union meltparam_un argtab[1];
                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                    /*^apply.arg*/
                                                                                                                    argtab[0].meltbp_gimple = /*_?*/ meltfram__.loc_GIMPLE__o2;
                                                                                                                    /*_.V__V14*/
                                                                                                                    meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V8*/
                                                                                                                meltfptr[7] = /*_.V__V14*/ meltfptr[9];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                                                /*clear*/ /*_?*/
                                                                                                                meltfram__.loc_GIMPLE__o2 = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_.V__V14*/
                                                                                                                meltfptr[9] = 0 ;
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

                                                                                                                            /*consume variadic GIMPLE_SEQ !*/
                                                                                                                            variad_ADD2OUT_ix += 1;;

                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1529:/ apply");
                                                                                                                            /*apply*/
                                                                                                                            {
                                                                                                                                union meltparam_un argtab[1];
                                                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                                                /*^apply.arg*/
                                                                                                                                argtab[0].meltbp_gimpleseq = /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3;
                                                                                                                                /*_.V__V15*/
                                                                                                                                meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un*)0);
                                                                                                                            }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V8*/
                                                                                                                            meltfptr[7] = /*_.V__V15*/ meltfptr[8];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                                                            /*clear*/ /*_?*/
                                                                                                                            meltfram__.loc_GIMPLE_SEQ__o3 = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_.V__V15*/
                                                                                                                            meltfptr[8] = 0 ;
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

                                                                                                                                        /*consume variadic EDGE !*/
                                                                                                                                        variad_ADD2OUT_ix += 1;;

                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1531:/ apply");
                                                                                                                                        /*apply*/
                                                                                                                                        {
                                                                                                                                            union meltparam_un argtab[1];
                                                                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                                                                            /*^apply.arg*/
                                                                                                                                            argtab[0].meltbp_edge = /*_?*/ meltfram__.loc_EDGE__o4;
                                                                                                                                            /*_.V__V16*/
                                                                                                                                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V8*/
                                                                                                                                        meltfptr[7] = /*_.V__V16*/ meltfptr[9];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                                                                        /*clear*/ /*_?*/
                                                                                                                                        meltfram__.loc_EDGE__o4 = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_.V__V16*/
                                                                                                                                        meltfptr[9] = 0 ;
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

                                                                                                                                                    /*consume variadic LOOP !*/
                                                                                                                                                    variad_ADD2OUT_ix += 1;;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1533:/ apply");
                                                                                                                                                    /*apply*/
                                                                                                                                                    {
                                                                                                                                                        union meltparam_un argtab[1];
                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                        argtab[0].meltbp_loop = /*_?*/ meltfram__.loc_LOOP__o5;
                                                                                                                                                        /*_.V__V17*/
                                                                                                                                                        meltfptr[8] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V8*/
                                                                                                                                                    meltfptr[7] = /*_.V__V17*/ meltfptr[8];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                                                                                    /*clear*/ /*_?*/
                                                                                                                                                    meltfram__.loc_LOOP__o5 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.V__V17*/
                                                                                                                                                    meltfptr[8] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else    /*^cond.else*/
                                                                                                                                            {

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1535:/ quasiblock");


                                                                                                                                                    /*_.VCTY__V19*/
                                                                                                                                                    meltfptr[8] =
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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1538:/ cond");
                                                                                                                                                    /*cond*/
                                                                                                                                                    if (
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
                                                                                                                                                                /*_.NAMED_NAME__V20*/
                                                                                                                                                                meltfptr[19] = slot;
                                                                                                                                                            };
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_.NAMED_NAME__V20*/ meltfptr[19] =  /*fromline 1341*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1537:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V20*/ meltfptr[19])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1539:/ cppif.then");
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
                                                                                                                                                                /*_._IFELSE___V22*/ meltfptr[21] = (/*nil*/NULL);;
                                                                                                                                                            }
                                                                                                                                                        else
                                                                                                                                                            {
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1539:/ cond.else");

                                                                                                                                                                /*^block*/
                                                                                                                                                                /*anyblock*/
                                                                                                                                                                {




                                                                                                                                                                    {
                                                                                                                                                                        /*^locexp*/
                                                                                                                                                                        melt_assert_failed(( "invalid variadic argument after closure to ADD2OUT"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                                                                                                                                                                           (1539)?(1539):__LINE__, __FUNCTION__);
                                                                                                                                                                        ;
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*clear*/ /*_._IFELSE___V22*/
                                                                                                                                                                    meltfptr[21] = 0 ;
                                                                                                                                                                    /*epilog*/
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*^compute*/
                                                                                                                                                        /*_.IFCPP___V21*/
                                                                                                                                                        meltfptr[20] = /*_._IFELSE___V22*/ meltfptr[21];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1539:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V22*/
                                                                                                                                                        meltfptr[21] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V21*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1540:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/
                                                                                                                                                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1540:/ locexp");
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
                                                                                                                                                    /*_.LET___V18*/
                                                                                                                                                    meltfptr[9] = /*_.RETURN___V23*/ meltfptr[21];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1535:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V19*/
                                                                                                                                                    meltfptr[8] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V20*/
                                                                                                                                                    meltfptr[19] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V21*/
                                                                                                                                                    meltfptr[20] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V23*/
                                                                                                                                                    meltfptr[21] = 0 ;
                                                                                                                                                    /*_._IFELSE___V8*/
                                                                                                                                                    meltfptr[7] = /*_.LET___V18*/ meltfptr[9];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1517:/ clear");
                                                                                                                                                    /*clear*/ /*_.LET___V18*/
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
                                                MELT_LOCATION("warmelt-base.melt:1516:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1542:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V2*/ meltfptr[1];
                                                        /*_.ADD_TO_OUT__V24*/
                                                        meltfptr[8] = meltgc_send((melt_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[1])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V8*/
                                                    meltfptr[7] = /*_.ADD_TO_OUT__V24*/ meltfptr[8];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1516:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V24*/
                                                    meltfptr[8] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1512:/ clear");
                                        /*clear*/ /*_.V__V7*/
                                        meltfptr[6] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#IS_CLOSURE__L3*/
                                        meltfnum[2] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_._IFELSE___V8*/
                                        meltfptr[7] = 0 ;
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

                                                    /*consume variadic LONG !*/
                                                    variad_ADD2OUT_ix += 1;;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1544:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1512:/ clear");
                                                    /*clear*/ /*_#N__L5*/
                                                    meltfnum[3] = 0 ;
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

                                                                /*consume variadic CSTRING !*/
                                                                variad_ADD2OUT_ix += 1;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1546:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1512:/ clear");
                                                                /*clear*/ /*_?*/
                                                                meltfram__.loc_CSTRING__o6 = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1548:/ quasiblock");


                                                                /*_.VCTY__V25*/
                                                                meltfptr[19] =
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
                                                                MELT_LOCATION("warmelt-base.melt:1551:/ cond");
                                                                /*cond*/
                                                                if (
                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V25*/ meltfptr[19]),
                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[0])))
                                                                ) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^getslot*/
                                                                        {
                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                            obj = (melt_ptr_t)(/*_.VCTY__V25*/ meltfptr[19]) /*=obj*/;
                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                            /*_.NAMED_NAME__V26*/
                                                                            meltfptr[20] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V26*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1550:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V26*/ meltfptr[20])));
                                                                }
                                                                ;

                                                                MELT_LOCATION("warmelt-base.melt:1548:/ clear");
                                                                /*clear*/ /*_.VCTY__V25*/
                                                                meltfptr[19] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V26*/
                                                                meltfptr[20] = 0 ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1553:/ cppif.then");
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
                                                                            /*_._IFELSE___V28*/ meltfptr[9] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1553:/ cond.else");

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {




                                                                                {
                                                                                    /*^locexp*/
                                                                                    melt_assert_failed(( "invalid variadic argument to ADD2OUT"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                                                                                       (1553)?(1553):__LINE__, __FUNCTION__);
                                                                                    ;
                                                                                }
                                                                                ;
                                                                                /*clear*/ /*_._IFELSE___V28*/
                                                                                meltfptr[9] = 0 ;
                                                                                /*epilog*/
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*^compute*/
                                                                    /*_.IFCPP___V27*/
                                                                    meltfptr[21] = /*_._IFELSE___V28*/ meltfptr[9];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1553:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V28*/
                                                                    meltfptr[9] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V27*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1512:/ clear");
                                                                /*clear*/ /*_.IFCPP___V27*/
                                                                meltfptr[21] = 0 ;
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
            MELT_LOCATION("warmelt-base.melt:1510:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V5*/
            meltfptr[3] = /*_.ARGLOOP__V6*/ meltfptr[5];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1555:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.OUT__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:1555:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1504:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RETURN___V29*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-base.melt:1504:/ locexp");
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
        /*clear*/ /*_#IS_OUT__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V5*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V29*/
        meltfptr[8] = 0 ;
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

    /* start of frame for routine meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1716 */

    /** start of frame for meltrout_15_WARMELTmiBASE_STRING4OUT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT// fromline 1531
        : public Melt_CallFrameWithValues<41>
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
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<41> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT), clos) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<41> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT)) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT


    /** end of frame for meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1661**/

    /* end of frame for routine meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1720 */

    /* classy proc frame meltrout_15_WARMELTmiBASE_STRING4OUT */
    MeltFrame_meltrout_15_WARMELTmiBASE_STRING4OUT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_15_WARMELTmiBASE_STRING4OUT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("STRING4OUT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1559:/ getarg");
    /*_.DIS__V2*/
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
        MELT_LOCATION("warmelt-base.melt:1562:/ cond");
        /*cond*/
        if (/*_.DIS__V2*/ meltfptr[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V3*/ meltfptr[2] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1562:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.DIS__V2*/ meltfptr[1] = /*_.SETQ___V4*/ meltfptr[3] = (/*!DISCR_STRING*/ meltfrout->tabval[0]);;
                    /*^quasiblock*/


                    /*_.PROGN___V5*/
                    meltfptr[4] = /*_.SETQ___V4*/ meltfptr[3];;
                    /*^compute*/
                    /*_._IFELSE___V3*/
                    meltfptr[2] = /*_.PROGN___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1562:/ clear");
                    /*clear*/ /*_.SETQ___V4*/
                    meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V5*/
                    meltfptr[4] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L1*/
        meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_DISCRIMINANT*/ meltfrout->tabval[1])));;
        MELT_LOCATION("warmelt-base.melt:1563:/ cond");
        /*cond*/
        if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V6*/ meltfptr[3] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1563:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1564:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1564:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:1563:/ quasiblock");


                    /*_.PROGN___V8*/
                    meltfptr[7] = /*_.RETURN___V7*/ meltfptr[4];;
                    /*^compute*/
                    /*_._IFELSE___V6*/
                    meltfptr[3] = /*_.PROGN___V8*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1563:/ clear");
                    /*clear*/ /*_.RETURN___V7*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V8*/
                    meltfptr[7] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1565:/ quasiblock");


        /*_#GOODMAG__L2*/
        meltfnum[1] = 0;;

        {
            MELT_LOCATION("warmelt-base.melt:1567:/ locexp");
            /* string4out GETMAGIDISCHK__1 */ /*_#GOODMAG__L2*/
            meltfnum[1] =
                ((meltobject_ptr_t)/*_.DIS__V2*/ meltfptr[1])->meltobj_magic == MELTOBMAG_STRING ;;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1570:/ cond");
        /*cond*/
        if (/*_#GOODMAG__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V10*/ meltfptr[7] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1570:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1570:/ locexp");
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
                    /*^quasiblock*/


                    /*_.PROGN___V12*/
                    meltfptr[11] = /*_.RETURN___V11*/ meltfptr[10];;
                    /*^compute*/
                    /*_._IFELSE___V10*/
                    meltfptr[7] = /*_.PROGN___V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1570:/ clear");
                    /*clear*/ /*_.RETURN___V11*/
                    meltfptr[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V12*/
                    meltfptr[11] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V9*/
        meltfptr[4] = /*_._IFELSE___V10*/ meltfptr[7];;

        MELT_LOCATION("warmelt-base.melt:1565:/ clear");
        /*clear*/ /*_#GOODMAG__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V10*/
        meltfptr[7] = 0 ;
        MELT_LOCATION("warmelt-base.melt:1571:/ quasiblock");


        /*_.OUT__V14*/
        meltfptr[11] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[2])), (const char*)0);;
        MELT_LOCATION("warmelt-base.melt:1573:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_2:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1575:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_STRING4OUT_ix == variad_STRING4OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_STRING4OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1577:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V16*/
                            meltfptr[15] =  /*fromline 1341*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:1575:/ cond.else");

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

                                        /*consume variadic Value !*/
                                        variad_STRING4OUT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*_#IS_CLOSURE__L3*/
                                        meltfnum[1] =
                                            (melt_magic_discr((melt_ptr_t)(/*_.V__V17*/ meltfptr[16])) == MELTOBMAG_CLOSURE);;
                                        MELT_LOCATION("warmelt-base.melt:1579:/ cond");
                                        /*cond*/
                                        if (/*_#IS_CLOSURE__L3*/ meltfnum[1]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1580:/ cond");
                                                    /*cond*/
                                                    if (/*ifvariadic arg#1*/ variad_STRING4OUT_ix>=0 && variad_STRING4OUT_ix + 1 <= variad_STRING4OUT_len && meltxargdescr_[variad_STRING4OUT_ix]== MELTBPAR_PTR) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^compute*/
                                                                /*_.VV__V19*/ meltfptr[18] =
                                                                    /*variadic argument value*/ ((meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_STRING4OUT_ix + 0].meltbp_aptr)) : NULL);;
                                                                /*^compute*/

                                                                /*consume variadic Value !*/
                                                                variad_STRING4OUT_ix += 1;;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-base.melt:1582:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[1];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VV__V19*/ meltfptr[18];
                                                                    /*_.V__V20*/
                                                                    meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_._IFELSE___V18*/
                                                                meltfptr[17] = /*_.V__V20*/ meltfptr[19];;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                /*clear*/ /*_.VV__V19*/
                                                                meltfptr[18] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.V__V20*/
                                                                meltfptr[19] = 0 ;
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

                                                                            /*consume variadic LONG !*/
                                                                            variad_STRING4OUT_ix += 1;;

                                                                            MELT_CHECK_SIGNAL();
                                                                            ;
                                                                            MELT_LOCATION("warmelt-base.melt:1584:/ apply");
                                                                            /*apply*/
                                                                            {
                                                                                union meltparam_un argtab[1];
                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                /*^apply.arg*/
                                                                                argtab[0].meltbp_long = /*_#LL__L4*/ meltfnum[3];
                                                                                /*_.V__V21*/
                                                                                meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                            }
                                                                            ;
                                                                            /*_._IFELSE___V18*/
                                                                            meltfptr[17] = /*_.V__V21*/ meltfptr[18];;
                                                                            /*epilog*/

                                                                            MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                            /*clear*/ /*_#LL__L4*/
                                                                            meltfnum[3] = 0 ;
                                                                            /*^clear*/
                                                                            /*clear*/ /*_.V__V21*/
                                                                            meltfptr[18] = 0 ;
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

                                                                                        /*consume variadic CSTRING !*/
                                                                                        variad_STRING4OUT_ix += 1;;

                                                                                        MELT_CHECK_SIGNAL();
                                                                                        ;
                                                                                        MELT_LOCATION("warmelt-base.melt:1586:/ apply");
                                                                                        /*apply*/
                                                                                        {
                                                                                            union meltparam_un argtab[1];
                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                            /*^apply.arg*/
                                                                                            argtab[0].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
                                                                                            /*_.V__V22*/
                                                                                            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                        }
                                                                                        ;
                                                                                        /*_._IFELSE___V18*/
                                                                                        meltfptr[17] = /*_.V__V22*/ meltfptr[19];;
                                                                                        /*epilog*/

                                                                                        MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                        /*clear*/ /*_?*/
                                                                                        meltfram__.loc_CSTRING__o0 = 0 ;
                                                                                        /*^clear*/
                                                                                        /*clear*/ /*_.V__V22*/
                                                                                        meltfptr[19] = 0 ;
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

                                                                                                    /*consume variadic TREE !*/
                                                                                                    variad_STRING4OUT_ix += 1;;

                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                    ;
                                                                                                    MELT_LOCATION("warmelt-base.melt:1588:/ apply");
                                                                                                    /*apply*/
                                                                                                    {
                                                                                                        union meltparam_un argtab[1];
                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                        /*^apply.arg*/
                                                                                                        argtab[0].meltbp_tree = /*_?*/ meltfram__.loc_TREE__o1;
                                                                                                        /*_.V__V23*/
                                                                                                        meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_TREE ""), argtab, "", (union meltparam_un*)0);
                                                                                                    }
                                                                                                    ;
                                                                                                    /*_._IFELSE___V18*/
                                                                                                    meltfptr[17] = /*_.V__V23*/ meltfptr[18];;
                                                                                                    /*epilog*/

                                                                                                    MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                                    /*clear*/ /*_?*/
                                                                                                    meltfram__.loc_TREE__o1 = 0 ;
                                                                                                    /*^clear*/
                                                                                                    /*clear*/ /*_.V__V23*/
                                                                                                    meltfptr[18] = 0 ;
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

                                                                                                                /*consume variadic GIMPLE !*/
                                                                                                                variad_STRING4OUT_ix += 1;;

                                                                                                                MELT_CHECK_SIGNAL();
                                                                                                                ;
                                                                                                                MELT_LOCATION("warmelt-base.melt:1590:/ apply");
                                                                                                                /*apply*/
                                                                                                                {
                                                                                                                    union meltparam_un argtab[1];
                                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                                    /*^apply.arg*/
                                                                                                                    argtab[0].meltbp_gimple = /*_?*/ meltfram__.loc_GIMPLE__o2;
                                                                                                                    /*_.V__V24*/
                                                                                                                    meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                }
                                                                                                                ;
                                                                                                                /*_._IFELSE___V18*/
                                                                                                                meltfptr[17] = /*_.V__V24*/ meltfptr[19];;
                                                                                                                /*epilog*/

                                                                                                                MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                                                /*clear*/ /*_?*/
                                                                                                                meltfram__.loc_GIMPLE__o2 = 0 ;
                                                                                                                /*^clear*/
                                                                                                                /*clear*/ /*_.V__V24*/
                                                                                                                meltfptr[19] = 0 ;
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

                                                                                                                            /*consume variadic GIMPLE_SEQ !*/
                                                                                                                            variad_STRING4OUT_ix += 1;;

                                                                                                                            MELT_CHECK_SIGNAL();
                                                                                                                            ;
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1592:/ apply");
                                                                                                                            /*apply*/
                                                                                                                            {
                                                                                                                                union meltparam_un argtab[1];
                                                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                                                /*^apply.arg*/
                                                                                                                                argtab[0].meltbp_gimpleseq = /*_?*/ meltfram__.loc_GIMPLE_SEQ__o3;
                                                                                                                                /*_.V__V25*/
                                                                                                                                meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_GIMPLESEQ ""), argtab, "", (union meltparam_un*)0);
                                                                                                                            }
                                                                                                                            ;
                                                                                                                            /*_._IFELSE___V18*/
                                                                                                                            meltfptr[17] = /*_.V__V25*/ meltfptr[18];;
                                                                                                                            /*epilog*/

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                                                            /*clear*/ /*_?*/
                                                                                                                            meltfram__.loc_GIMPLE_SEQ__o3 = 0 ;
                                                                                                                            /*^clear*/
                                                                                                                            /*clear*/ /*_.V__V25*/
                                                                                                                            meltfptr[18] = 0 ;
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

                                                                                                                                        /*consume variadic EDGE !*/
                                                                                                                                        variad_STRING4OUT_ix += 1;;

                                                                                                                                        MELT_CHECK_SIGNAL();
                                                                                                                                        ;
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1594:/ apply");
                                                                                                                                        /*apply*/
                                                                                                                                        {
                                                                                                                                            union meltparam_un argtab[1];
                                                                                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                                                                                            /*^apply.arg*/
                                                                                                                                            argtab[0].meltbp_edge = /*_?*/ meltfram__.loc_EDGE__o4;
                                                                                                                                            /*_.V__V26*/
                                                                                                                                            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_EDGE ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                        }
                                                                                                                                        ;
                                                                                                                                        /*_._IFELSE___V18*/
                                                                                                                                        meltfptr[17] = /*_.V__V26*/ meltfptr[19];;
                                                                                                                                        /*epilog*/

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                                                                        /*clear*/ /*_?*/
                                                                                                                                        meltfram__.loc_EDGE__o4 = 0 ;
                                                                                                                                        /*^clear*/
                                                                                                                                        /*clear*/ /*_.V__V26*/
                                                                                                                                        meltfptr[19] = 0 ;
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

                                                                                                                                                    /*consume variadic LOOP !*/
                                                                                                                                                    variad_STRING4OUT_ix += 1;;

                                                                                                                                                    MELT_CHECK_SIGNAL();
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1596:/ apply");
                                                                                                                                                    /*apply*/
                                                                                                                                                    {
                                                                                                                                                        union meltparam_un argtab[1];
                                                                                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                        argtab[0].meltbp_loop = /*_?*/ meltfram__.loc_LOOP__o5;
                                                                                                                                                        /*_.V__V27*/
                                                                                                                                                        meltfptr[18] =  melt_apply ((meltclosure_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (MELTBPARSTR_LOOP ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                    }
                                                                                                                                                    ;
                                                                                                                                                    /*_._IFELSE___V18*/
                                                                                                                                                    meltfptr[17] = /*_.V__V27*/ meltfptr[18];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                                                                                    /*clear*/ /*_?*/
                                                                                                                                                    meltfram__.loc_LOOP__o5 = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.V__V27*/
                                                                                                                                                    meltfptr[18] = 0 ;
                                                                                                                                                }
                                                                                                                                                ;
                                                                                                                                            }
                                                                                                                                        else    /*^cond.else*/
                                                                                                                                            {

                                                                                                                                                /*^block*/
                                                                                                                                                /*anyblock*/
                                                                                                                                                {

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1598:/ quasiblock");


                                                                                                                                                    /*_.VCTY__V29*/
                                                                                                                                                    meltfptr[18] =
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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1601:/ cond");
                                                                                                                                                    /*cond*/
                                                                                                                                                    if (
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
                                                                                                                                                                /*_.NAMED_NAME__V30*/
                                                                                                                                                                meltfptr[29] = slot;
                                                                                                                                                            };
                                                                                                                                                            ;
                                                                                                                                                        }
                                                                                                                                                    else    /*^cond.else*/
                                                                                                                                                        {

                                                                                                                                                            /*_.NAMED_NAME__V30*/ meltfptr[29] =  /*fromline 1341*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1600:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V30*/ meltfptr[29])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1602:/ cppif.then");
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
                                                                                                                                                                /*_._IFELSE___V32*/ meltfptr[31] = (/*nil*/NULL);;
                                                                                                                                                            }
                                                                                                                                                        else
                                                                                                                                                            {
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1602:/ cond.else");

                                                                                                                                                                /*^block*/
                                                                                                                                                                /*anyblock*/
                                                                                                                                                                {




                                                                                                                                                                    {
                                                                                                                                                                        /*^locexp*/
                                                                                                                                                                        melt_assert_failed(( "invalid variadic argument after closure to STRING4OUT"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                                                                                                                                                                           (1602)?(1602):__LINE__, __FUNCTION__);
                                                                                                                                                                        ;
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*clear*/ /*_._IFELSE___V32*/
                                                                                                                                                                    meltfptr[31] = 0 ;
                                                                                                                                                                    /*epilog*/
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*^compute*/
                                                                                                                                                        /*_.IFCPP___V31*/
                                                                                                                                                        meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1602:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V32*/
                                                                                                                                                        meltfptr[31] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1603:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/
                                                                                                                                                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1603:/ locexp");
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
                                                                                                                                                    /*_.LET___V28*/
                                                                                                                                                    meltfptr[19] = /*_.RETURN___V33*/ meltfptr[31];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1598:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V29*/
                                                                                                                                                    meltfptr[18] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V30*/
                                                                                                                                                    meltfptr[29] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V31*/
                                                                                                                                                    meltfptr[30] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V33*/
                                                                                                                                                    meltfptr[31] = 0 ;
                                                                                                                                                    /*_._IFELSE___V18*/
                                                                                                                                                    meltfptr[17] = /*_.LET___V28*/ meltfptr[19];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1580:/ clear");
                                                                                                                                                    /*clear*/ /*_.LET___V28*/
                                                                                                                                                    meltfptr[19] = 0 ;
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
                                                MELT_LOCATION("warmelt-base.melt:1579:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1605:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V14*/ meltfptr[11];
                                                        /*_.ADD_TO_OUT__V34*/
                                                        meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V18*/
                                                    meltfptr[17] = /*_.ADD_TO_OUT__V34*/ meltfptr[18];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1579:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V34*/
                                                    meltfptr[18] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1575:/ clear");
                                        /*clear*/ /*_.V__V17*/
                                        meltfptr[16] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#IS_CLOSURE__L3*/
                                        meltfnum[1] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_._IFELSE___V18*/
                                        meltfptr[17] = 0 ;
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

                                                    /*consume variadic LONG !*/
                                                    variad_STRING4OUT_ix += 1;;

                                                    {
                                                        MELT_LOCATION("warmelt-base.melt:1607:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1575:/ clear");
                                                    /*clear*/ /*_#N__L5*/
                                                    meltfnum[3] = 0 ;
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

                                                                /*consume variadic CSTRING !*/
                                                                variad_STRING4OUT_ix += 1;;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1609:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1575:/ clear");
                                                                /*clear*/ /*_?*/
                                                                meltfram__.loc_CSTRING__o6 = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    else    /*^cond.else*/
                                                        {

                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                MELT_LOCATION("warmelt-base.melt:1611:/ quasiblock");


                                                                /*_.VCTY__V35*/
                                                                meltfptr[29] =
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
                                                                MELT_LOCATION("warmelt-base.melt:1614:/ cond");
                                                                /*cond*/
                                                                if (
                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.VCTY__V35*/ meltfptr[29]),
                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
                                                                ) /*then*/
                                                                    {
                                                                        /*^cond.then*/
                                                                        /*^getslot*/
                                                                        {
                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                            obj = (melt_ptr_t)(/*_.VCTY__V35*/ meltfptr[29]) /*=obj*/;
                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                            /*_.NAMED_NAME__V36*/
                                                                            meltfptr[30] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V36*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1613:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V36*/ meltfptr[30])));
                                                                }
                                                                ;

                                                                MELT_LOCATION("warmelt-base.melt:1611:/ clear");
                                                                /*clear*/ /*_.VCTY__V35*/
                                                                meltfptr[29] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V36*/
                                                                meltfptr[30] = 0 ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1616:/ cppif.then");
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
                                                                            /*_._IFELSE___V38*/ meltfptr[19] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1616:/ cond.else");

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {




                                                                                {
                                                                                    /*^locexp*/
                                                                                    melt_assert_failed(( "invalid variadic argument to STRING4OUT"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                                                                                       (1616)?(1616):__LINE__, __FUNCTION__);
                                                                                    ;
                                                                                }
                                                                                ;
                                                                                /*clear*/ /*_._IFELSE___V38*/
                                                                                meltfptr[19] = 0 ;
                                                                                /*epilog*/
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*^compute*/
                                                                    /*_.IFCPP___V37*/
                                                                    meltfptr[31] = /*_._IFELSE___V38*/ meltfptr[19];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1616:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V38*/
                                                                    meltfptr[19] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V37*/ meltfptr[31] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1575:/ clear");
                                                                /*clear*/ /*_.IFCPP___V37*/
                                                                meltfptr[31] = 0 ;
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
            MELT_LOCATION("warmelt-base.melt:1573:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V15*/
            meltfptr[7] = /*_.ARGLOOP__V16*/ meltfptr[15];;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:1618:/ quasiblock");


        /*_.STR__V40*/
        meltfptr[16] =
            (meltgc_new_stringdup((meltobject_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), melt_strbuf_str((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]))));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1620:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.STR__V40*/ meltfptr[16];;

        {
            MELT_LOCATION("warmelt-base.melt:1620:/ locexp");
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
        /*_.LET___V39*/
        meltfptr[18] = /*_.RETURN___V41*/ meltfptr[17];;

        MELT_LOCATION("warmelt-base.melt:1618:/ clear");
        /*clear*/ /*_.STR__V40*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V41*/
        meltfptr[17] = 0 ;
        /*_.LET___V13*/
        meltfptr[10] = /*_.LET___V39*/ meltfptr[18];;

        MELT_LOCATION("warmelt-base.melt:1571:/ clear");
        /*clear*/ /*_.OUT__V14*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V15*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V39*/
        meltfptr[18] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1559:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-base.melt:1559:/ locexp");
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
        /*clear*/ /*_._IFELSE___V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V6*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V9*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/
        meltfptr[10] = 0 ;
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

    /* start of frame for routine meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1716 */

    /** start of frame for meltrout_16_WARMELTmiBASE_ADD2OUT4NULL of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL// fromline 1531
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
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL)) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL


    /** end of frame for meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1661**/

    /* end of frame for routine meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1720 */

    /* classy proc frame meltrout_16_WARMELTmiBASE_ADD2OUT4NULL */
    MeltFrame_meltrout_16_WARMELTmiBASE_ADD2OUT4NULL
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_16_WARMELTmiBASE_ADD2OUT4NULL fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4NULL", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1626:/ getarg");
    /*_.V__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-base.melt:1627:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#NULL__L1*/
            meltfnum[0] =
                ((/*_.V__V2*/ meltfptr[1]) == NULL);;
            MELT_LOCATION("warmelt-base.melt:1627:/ cond");
            /*cond*/
            if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1627:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check null v"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                               (1627)?(1627):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V5*/
                        meltfptr[4] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1627:/ clear");
            /*clear*/ /*_#NULL__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1628:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*nil*"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1626:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
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

    /* start of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1716 */

    /** start of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER// fromline 1531
        : public Melt_CallFrameWithValues<5>
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
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER


    /** end of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1661**/

    /* end of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1720 */

    /* classy proc frame meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER */
    MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_17_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4INTEGER", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1631:/ getarg");
    /*_.VN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-base.melt:1632:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_INTEGERBOX__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
            MELT_LOCATION("warmelt-base.melt:1632:/ cond");
            /*cond*/
            if (/*_#IS_INTEGERBOX__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1632:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check boxed number vn"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                               (1632)?(1632):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V5*/
                        meltfptr[4] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1632:/ clear");
            /*clear*/ /*_#IS_INTEGERBOX__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_#GET_INT__L2*/
        meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1633:/ locexp");
            meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#GET_INT__L2*/ meltfnum[0]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1631:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L2*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1716 */

    /** start of frame for meltrout_18_WARMELTmiBASE_ADD2OUT4STRING of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING// fromline 1531
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
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING)) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING


    /** end of frame for meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1661**/

    /* end of frame for routine meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1720 */

    /* classy proc frame meltrout_18_WARMELTmiBASE_ADD2OUT4STRING */
    MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4STRING
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_18_WARMELTmiBASE_ADD2OUT4STRING fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4STRING", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1636:/ getarg");
    /*_.VS__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-base.melt:1637:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-base.melt:1637:/ cond");
            /*cond*/
            if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1637:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check boxed string vs"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                               (1637)?(1637):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V5*/
                        meltfptr[4] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1637:/ clear");
            /*clear*/ /*_#IS_STRING__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1638:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1636:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
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

    /* start of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1716 */

    /** start of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF// fromline 1531
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
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF


    /** end of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1661**/

    /* end of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1720 */

    /* classy proc frame meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF */
    MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_19_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4STRBUF", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1641:/ getarg");
    /*_.VSBUF__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-base.melt:1642:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VSBUF__V2*/ meltfptr[1])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-base.melt:1642:/ cond");
            /*cond*/
            if (/*_#IS_STRBUF__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1642:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check strbuf vsbuf"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                               (1642)?(1642):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V5*/
                        meltfptr[4] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1642:/ clear");
            /*clear*/ /*_#IS_STRBUF__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1643:/ locexp");
            meltgc_add_out ((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_strbuf_str ((melt_ptr_t) /*_.VSBUF__V2*/ meltfptr[1]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1641:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
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

    /* start of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1716 */

    /** start of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB// fromline 1531
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
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB() //the constructor fromline 1606
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB


    /** end of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1661**/

    /* end of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1720 */

    /* classy proc frame meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB */
    MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_20_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4CLONEDSYMB", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1646:/ getarg");
    /*_.VC__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        MELT_LOCATION("warmelt-base.melt:1647:/ block");
        /*anyblock*/
        {

            /*^objgoto*/
            /*objgoto*/ goto mtch1_0;
            ;

            /*objlabel*/
mtch1_0:
            ;
            MELT_LOCATION("warmelt-base.melt:1648:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_1;
            meltlab_count_1++;
            debugeprintf("objlabel_1 mtch1_*CLASS_NORMTESTER_INSTANCE/3aaee1b8. *CLASS_OBJLABELINSTR/8046242. %ld", meltlab_count_1);
#endif

            ;
            /*^clear*/
            /*clear*/ /*_.NAMED_NAME__V4*/
            meltfptr[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CSYM_URANK__V5*/
            meltfptr[4] = 0 ;
            /*^cond*/
            /*cond*/
            if (
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
                            /*_.NAMED_NAME__V4*/
                            meltfptr[3] = slot;
                        };
                        ;
                        /*^getslot*/
                        {
                            melt_ptr_t slot=NULL, obj=NULL;
                            obj = (melt_ptr_t)(/*_.VC__V2*/ meltfptr[1]) /*=obj*/;
                            melt_object_get_field(slot,obj, 3, "CSYM_URANK");
                            /*_.CSYM_URANK__V5*/
                            meltfptr[4] = slot;
                        };
                        ;
                        MELT_LOCATION("warmelt-base.melt:1650:/ objgoto");
                        /*objgoto*/
                        goto mtch1_1;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1648:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1655:/ objgoto");
                        /*objgoto*/
                        goto mtch1_3;
                        ;
                    }
                    ;
                }
            ;

            /*objlabel*/
mtch1_1:
            ;
            MELT_LOCATION("warmelt-base.melt:1650:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_2;
            meltlab_count_2++;
            debugeprintf("objlabel_2 mtch1_*CLASS_NORMTESTER_MATCHER/19a126a4. *CLASS_OBJLABELINSTR/356c83f1. %ld", meltlab_count_2);
#endif

            ;
            /*^clear*/
            /*clear*/ /*_#ICT__L1*/
            meltfnum[0] = 0 ;
            /*^cond*/
            /*cond*/
            if (
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
                        MELT_LOCATION("warmelt-base.melt:1648:/ objgoto");
                        /*objgoto*/
                        goto mtch1_2;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1650:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1655:/ objgoto");
                        /*objgoto*/
                        goto mtch1_3;
                        ;
                    }
                    ;
                }
            ;

            /*objlabel*/
mtch1_2:
            ;
            MELT_LOCATION("warmelt-base.melt:1648:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_3;
            meltlab_count_3++;
            debugeprintf("objlabel_3 mtch1_*CLASS_NORMTESTER_SUCCESS/2f5f1cbb. *CLASS_OBJLABELINSTR/3363474d. %ld", meltlab_count_3);
#endif

            ;
            /*^quasiblock*/


            /*_.NVARNAM__V6*/
            meltfptr[4] = /*_.NAMED_NAME__V4*/ meltfptr[3];;
            /*^compute*/
            /*_#NVARURANK__L2*/
            meltfnum[1] = /*_#ICT__L1*/ meltfnum[0];;

#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1651:/ cppif.then");
            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_STRING__L3*/
                meltfnum[2] =
                    (melt_magic_discr((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])) == MELTOBMAG_STRING);;
                MELT_LOCATION("warmelt-base.melt:1651:/ cond");
                /*cond*/
                if (/*_#IS_STRING__L3*/ meltfnum[2]) /*then*/
                    {
                        /*^cond.then*/
                        /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1651:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {




                            {
                                /*^locexp*/
                                melt_assert_failed(( "check nvarnam"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                                   (1651)?(1651):__LINE__, __FUNCTION__);
                                ;
                            }
                            ;
                            /*clear*/ /*_._IFELSE___V8*/
                            meltfptr[7] = 0 ;
                            /*epilog*/
                        }
                        ;
                    }
                ;
                /*^compute*/
                /*_.IFCPP___V7*/
                meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1651:/ clear");
                /*clear*/ /*_#IS_STRING__L3*/
                meltfnum[2] = 0 ;
                /*^clear*/
                /*clear*/ /*_._IFELSE___V8*/
                meltfptr[7] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V7*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1652:/ locexp");
                meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                      melt_string_str((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1653:/ locexp");
                meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "__"));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1654:/ locexp");
                meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#NVARURANK__L2*/ meltfnum[1]));
            }
            ;

            MELT_CHECK_SIGNAL();
            ;

            MELT_LOCATION("warmelt-base.melt:1648:/ clear");
            /*clear*/ /*_.NVARNAM__V6*/
            meltfptr[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NVARURANK__L2*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V7*/
            meltfptr[6] = 0 ;
            /*^objgoto*/
            /*objgoto*/
            goto mtch1__end /*endmatch*/;
            ;

            /*objlabel*/
mtch1_3:
            ;
            MELT_LOCATION("warmelt-base.melt:1655:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_4;
            meltlab_count_4++;
            debugeprintf("objlabel_4 mtch1_*CLASS_NORMTESTER_SUCCESS/2a0b2d61. *CLASS_OBJLABELINSTR/21182ebe. %ld", meltlab_count_4);
#endif

            ;
            /*^quasiblock*/



#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1656:/ cppif.then");
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
                        /*_._IFELSE___V10*/ meltfptr[4] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1656:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {




                            {
                                /*^locexp*/
                                melt_assert_failed(( "bad cloned symbol"),( "warmelt-base.melt")?( "warmelt-base.melt"):__FILE__,
                                                   (1656)?(1656):__LINE__, __FUNCTION__);
                                ;
                            }
                            ;
                            /*clear*/ /*_._IFELSE___V10*/
                            meltfptr[4] = 0 ;
                            /*epilog*/
                        }
                        ;
                    }
                ;
                /*^compute*/
                /*_.IFCPP___V9*/
                meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[4];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1656:/ clear");
                /*clear*/ /*_._IFELSE___V10*/
                meltfptr[4] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            MELT_LOCATION("warmelt-base.melt:1655:/ clear");
            /*clear*/ /*_.IFCPP___V9*/
            meltfptr[7] = 0 ;
            /*^objgoto*/
            /*objgoto*/
            goto mtch1__end /*endmatch*/;
            ;

            /*objlabel*/
mtch1__end:
            ;
            MELT_LOCATION("warmelt-base.melt:1647:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_5;
            meltlab_count_5++;
            debugeprintf("objlabel_5 mtch1__end_h507519720 *CLASS_OBJLABELINSTR/1e4022e8. %ld", meltlab_count_5);
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

    /* start of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1716 */

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
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED() //the constructor fromline 1606
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED)) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED


    /** end of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1661**/

    /* end of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1720 */

    /* classy proc frame meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED */
    MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_21_WARMELTmiBASE_ADD2OUT4NAMED fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4NAMED", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1659:/ getarg");
    /*_.VN__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:1660:/ cond");
        /*cond*/
        if (
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
                    /*_.NAMED_NAME__V4*/
                    meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V4*/ meltfptr[3] =  /*fromline 1341*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1660:/ locexp");
            meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V4*/ meltfptr[3])));
        }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1659:/ clear");
        /*clear*/ /*_.NAMED_NAME__V4*/
        meltfptr[3] = 0 ;
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

    /* start of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1716 */

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
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT


    /** end of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1661**/

    /* end of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1720 */

    /* classy proc frame meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT */
    MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_22_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4ROOTOBJECT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1663:/ getarg");
    /*_.VO__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
            MELT_LOCATION("warmelt-base.melt:1664:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/
        meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1665:/ cond");
        /*cond*/
        if (
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
                    /*_.NAMED_NAME__V5*/
                    meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1665:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1666:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "/"));
        }
        ;
        /*_#OBJ_HASH__L1*/
        meltfnum[0] =
            (melt_obj_hash((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1667:/ locexp");
            meltgc_add_out_hex((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#OBJ_HASH__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1668:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "."));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1663:/ clear");
        /*clear*/ /*_.DISCRIM__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OBJ_HASH__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1716 */

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
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY)) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY


    /** end of frame for meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1661**/

    /* end of frame for routine meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1720 */

    /* classy proc frame meltrout_23_WARMELTmiBASE_ADD2OUT4ANY */
    MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ANY
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_23_WARMELTmiBASE_ADD2OUT4ANY fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2OUT4ANY", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1671:/ getarg");
    /*_.VA__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.OUT__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
            MELT_LOCATION("warmelt-base.melt:1672:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/
        meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VA__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1673:/ cond");
        /*cond*/
        if (
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
                    /*_.NAMED_NAME__V5*/
                    meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1673:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1671:/ clear");
        /*clear*/ /*_.DISCRIM__V4*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/
        meltfptr[4] = 0 ;
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

    /* start of frame for routine meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1716 */

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
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY() //the constructor fromline 1606
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY)) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY


    /** end of frame for meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1661**/

    /* end of frame for routine meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1720 */

    /* classy proc frame meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY */
    MeltFrame_meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_24_WARMELTmiBASE_MAPSTRING_EVERY fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPSTRING_EVERY", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1732:/ getarg");
    /*_.MAP__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        /*_#IS_MAPSTRING__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MAP__V2*/ meltfptr[1])) == MELTOBMAG_MAPSTRINGS);;
        MELT_LOCATION("warmelt-base.melt:1736:/ cond");
        /*cond*/
        if (/*_#IS_MAPSTRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1737:/ cond");
                    /*cond*/
                    if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
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
                                            /*_.CURAT__V4*/
                                            meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/
                                            meltfptr[4] = NULL;
                                            meltcit1__EACHSTRMAP_str =
                                                ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
                                            if (!meltcit1__EACHSTRMAP_str || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
                                                continue; /*foreach_in_mapstring meltcit1__EACHSTRMAP inside before*/
                                            /*_.CURVAL__V5*/
                                            meltfptr[4] = ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
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
                                                    /*_.CURAT__V4*/
                                                    meltfptr[3] =
                                                        meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
                                                                              meltcit1__EACHSTRMAP_str);
                                                }
                                            meltcit1__EACHSTRMAP_str = (const char*)0;
                                            meltcit1__EACHSTRMAP_nam = (const char*)0;




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1741:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURVAL__V5*/ meltfptr[4];
                                                /*_.F__V6*/
                                                meltfptr[5] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.CURAT__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
                                            /*_.CURAT__V4*/
                                            meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/
                                            meltfptr[4] = NULL;
                                        }

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1738:/ clear");
                                    /*clear*/ /*_.CURAT__V4*/
                                    meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURVAL__V5*/
                                    meltfptr[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V6*/
                                    meltfptr[5] = 0 ;
                                } /*endciterblock FOREACH_IN_MAPSTRING*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1736:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1732:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1716 */

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
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST() //the constructor fromline 1606
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST


    /** end of frame for meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1661**/

    /* end of frame for routine meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1720 */

    /* classy proc frame meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST */
    MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_25_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MAPSTRING_ITERATE_TEST", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1745:/ getarg");
    /*_.MAP__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        /*_#IS_MAPSTRING__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.MAP__V2*/ meltfptr[1])) == MELTOBMAG_MAPSTRINGS);;
        MELT_LOCATION("warmelt-base.melt:1750:/ cond");
        /*cond*/
        if (/*_#IS_MAPSTRING__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1751:/ cond");
                    /*cond*/
                    if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
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
                                            /*_.CURAT__V4*/
                                            meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/
                                            meltfptr[4] = NULL;
                                            meltcit1__EACHSTRMAP_str =
                                                ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_at;
                                            if (!meltcit1__EACHSTRMAP_str || meltcit1__EACHSTRMAP_str == HTAB_DELETED_ENTRY)
                                                continue; /*foreach_in_mapstring meltcit1__EACHSTRMAP inside before*/
                                            /*_.CURVAL__V5*/
                                            meltfptr[4] = ((struct meltmapstrings_st*)/*_.MAP__V2*/ meltfptr[1])->entab[meltcit1__EACHSTRMAP_ix].e_va;
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
                                                    /*_.CURAT__V4*/
                                                    meltfptr[3] =
                                                        meltgc_new_stringdup ((meltobject_ptr_t) MELT_PREDEF (DISCR_STRING),
                                                                              meltcit1__EACHSTRMAP_str);
                                                }
                                            meltcit1__EACHSTRMAP_str = (const char*)0;
                                            meltcit1__EACHSTRMAP_nam = (const char*)0;




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1755:/ quasiblock");


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
                                                /*_.TEST__V7*/
                                                meltfptr[6] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.CURAT__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, (MELTBPARSTR_PTR ""), restab);
                                            }
                                            ;
                                            /*^quasiblock*/



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#NULL__L3*/
                                            meltfnum[2] =
                                                ((/*_.TEST__V7*/ meltfptr[6]) == NULL);;
                                            MELT_LOCATION("warmelt-base.melt:1758:/ cond");
                                            /*cond*/
                                            if (/*_#NULL__L3*/ meltfnum[2]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1759:/ quasiblock");


                                                        /*_._RETVAL___V1*/
                                                        meltfptr[0] = /*_.CURAT__V4*/ meltfptr[3];;
                                                        MELT_LOCATION("warmelt-base.melt:1759:/ putxtraresult");
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
                                                        /*finalret*/
                                                        goto meltlabend_rout ;
                                                        /*_._IF___V9*/
                                                        meltfptr[8] = /*_.RETURN___V10*/ meltfptr[9];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:1758:/ clear");
                                                        /*clear*/ /*_.RETURN___V10*/
                                                        meltfptr[9] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V9*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                                                }
                                            ;
                                            /*^compute*/
                                            /*_.MULTI___V6*/
                                            meltfptr[5] = /*_._IF___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1755:/ clear");
                                            /*clear*/ /*_#NULL__L3*/
                                            meltfnum[2] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IF___V9*/
                                            meltfptr[8] = 0 ;

                                            /*^clear*/
                                            /*clear*/ /*_.OTHER__V8*/
                                            meltfptr[7] = 0 ;
                                            /* end foreach_in_mapstring meltcit1__EACHSTRMAP */
                                            /*_.CURAT__V4*/
                                            meltfptr[3] = NULL;
                                            /*_.CURVAL__V5*/
                                            meltfptr[4] = NULL;
                                        }

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1752:/ clear");
                                    /*clear*/ /*_.CURAT__V4*/
                                    meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURVAL__V5*/
                                    meltfptr[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MULTI___V6*/
                                    meltfptr[5] = 0 ;
                                } /*endciterblock FOREACH_IN_MAPSTRING*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1750:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1745:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1716 */

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
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY)) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY


    /** end of frame for meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1661**/

    /* end of frame for routine meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1720 */

    /* classy proc frame meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY */
    MeltFrame_meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_26_WARMELTmiBASE_MULTIPLE_EVERY fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_EVERY", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1794:/ getarg");
    /*_.TUP__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1797:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1798:/ cond");
                    /*cond*/
                    if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
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
                                            MELT_LOCATION("warmelt-base.melt:1802:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
                                                /*_.F__V5*/
                                                meltfptr[4] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.COMP__V4*/ meltfptr[3]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            if (/*_#IX__L3*/ meltfnum[2]<0) break;
                                        } /* end  foreach_in_multiple meltcit1__EACHTUP */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1799:/ clear");
                                    /*clear*/ /*_.COMP__V4*/
                                    meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L3*/
                                    meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V5*/
                                    meltfptr[4] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1797:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1794:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1716 */

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
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY() //the constructor fromline 1606
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY


    /** end of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1661**/

    /* end of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1720 */

    /* classy proc frame meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY */
    MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_27_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1805:/ getarg");
    /*_.TUP__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
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
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1808:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.F__V3*/ meltfptr[2])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-base.melt:1809:/ cond");
                    /*cond*/
                    if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
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
                                            /*_#IX__L3*/
                                            meltfnum[2]  =  meltcit1__EACHTUPBACK_ix;



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1813:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L3*/ meltfnum[2];
                                                /*_.F__V5*/
                                                meltfptr[4] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.COMP__V4*/ meltfptr[3]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                        } /* end  foreach_in_multiple_backward meltcit1__EACHTUPBACK */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:1810:/ clear");
                                    /*clear*/ /*_.COMP__V4*/
                                    meltfptr[3] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L3*/
                                    meltfnum[2] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.F__V5*/
                                    meltfptr[4] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE_BACKWARD*/
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1808:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1805:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
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

    /* start of frame for routine meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1716 */

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
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH() //the constructor fromline 1606
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH


    /** end of frame for meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1661**/

    /* end of frame for routine meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1720 */

    /* classy proc frame meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH */
    MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MULTIPLE_EVERY_BOTH", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:1817:/ getarg");
    /*_.TUP1__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.TUP2__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.F__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
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
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:1820:/ cond");
        /*cond*/
        if (/*_#IS_MULTIPLE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_MULTIPLE__L2*/
                    meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-base.melt:1821:/ cond");
                    /*cond*/
                    if (/*_#IS_MULTIPLE__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_CLOSURE__L3*/
                                meltfnum[2] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.F__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
                                MELT_LOCATION("warmelt-base.melt:1822:/ cond");
                                /*cond*/
                                if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1823:/ quasiblock");


                                            /*_#LN1__L4*/
                                            meltfnum[3] =
                                                (melt_multiple_length((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1])));;
                                            /*^compute*/
                                            /*_#LN2__L5*/
                                            meltfnum[4] =
                                                (melt_multiple_length((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2])));;
                                            /*^compute*/
                                            /*_#IX__L6*/
                                            meltfnum[5] = 0;;
                                            MELT_LOCATION("warmelt-base.melt:1826:/ loop");
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
                                                    /*_#gteqI__L7*/
                                                    meltfnum[6] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN1__L4*/ meltfnum[3]));;
                                                    MELT_LOCATION("warmelt-base.melt:1827:/ cond");
                                                    /*cond*/
                                                    if (/*_#gteqI__L7*/ meltfnum[6]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^quasiblock*/


                                                                /*^compute*/
                                                                /*_.TUPLOOP__V10*/ meltfptr[9] =  /*fromline 1341*/ NULL ;;

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
                                                    /*_#gteqI__L8*/
                                                    meltfnum[7] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN2__L5*/ meltfnum[4]));;
                                                    MELT_LOCATION("warmelt-base.melt:1828:/ cond");
                                                    /*cond*/
                                                    if (/*_#gteqI__L8*/ meltfnum[7]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {

                                                                /*^quasiblock*/


                                                                /*^compute*/
                                                                /*_.TUPLOOP__V10*/ meltfptr[9] =  /*fromline 1341*/ NULL ;;

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
                                                    /*_.MULTIPLE_NTH__V11*/
                                                    meltfptr[10] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1]), (/*_#IX__L6*/ meltfnum[5])));;
                                                    /*^compute*/
                                                    /*_.MULTIPLE_NTH__V12*/
                                                    meltfptr[11] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2]), (/*_#IX__L6*/ meltfnum[5])));;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1829:/ apply");
                                                    /*apply*/
                                                    {
                                                        union meltparam_un argtab[2];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^apply.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MULTIPLE_NTH__V12*/ meltfptr[11];
                                                        /*^apply.arg*/
                                                        argtab[1].meltbp_long = /*_#IX__L6*/ meltfnum[5];
                                                        /*_.F__V13*/
                                                        meltfptr[12] =  melt_apply ((meltclosure_ptr_t)(/*_.F__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.MULTIPLE_NTH__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_#plI__L9*/
                                                    meltfnum[8] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) + (1));;
                                                    MELT_LOCATION("warmelt-base.melt:1830:/ compute");
                                                    /*_#IX__L6*/
                                                    meltfnum[5] = /*_#SETQ___L10*/ meltfnum[9] = /*_#plI__L9*/ meltfnum[8];;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1826:/ clear");
                                                    /*clear*/ /*_#gteqI__L7*/
                                                    meltfnum[6] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#gteqI__L8*/
                                                    meltfnum[7] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.MULTIPLE_NTH__V11*/
                                                    meltfptr[10] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.MULTIPLE_NTH__V12*/
                                                    meltfptr[11] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.F__V13*/
                                                    meltfptr[12] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#plI__L9*/
                                                    meltfnum[8] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#SETQ___L10*/
                                                    meltfnum[9] = 0 ;
                                                }
                                                ;
                                                ;
                                                goto meltlabloop_TUPLOOP_1;
meltlabexit_TUPLOOP_1:;/*^loopepilog*/
                                                /*loopepilog*/
                                                /*_.FOREVER___V9*/
                                                meltfptr[8] = /*_.TUPLOOP__V10*/ meltfptr[9];;
                                            }
                                            ;
                                            /*^compute*/
                                            /*_.LET___V8*/
                                            meltfptr[7] = /*_.FOREVER___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1823:/ clear");
                                            /*clear*/ /*_#LN1__L4*/
                                            meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#LN2__L5*/
                                            meltfnum[4] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#IX__L6*/
                                            meltfnum[5] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.FOREVER___V9*/
                                            meltfptr[8] = 0 ;
                                            /*_._IF___V7*/
                                            meltfptr[6] = /*_.LET___V8*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1822:/ clear");
                                            /*clear*/ /*_.LET___V8*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V7*/ meltfptr[6] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IF___V6*/
                                meltfptr[5] = /*_._IF___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1821:/ clear");
                                /*clear*/ /*_#IS_CLOSURE__L3*/
                                meltfnum[2] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V7*/
                                meltfptr[6] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_._IF___V5*/
                    meltfptr[4] = /*_._IF___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1820:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V6*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V5*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1817:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IF___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-base.melt:1817:/ locexp");
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
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V5*/
        meltfptr[4] = 0 ;
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
