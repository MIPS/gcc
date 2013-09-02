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



class Melt_InitialClassyFrame_WARMELTmiBASE_h219695137; // forward declaration fromline 6539
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h219695137 Melt_InitialFrame;
/*** 5 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiBASE_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiBASE_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 5)?melt_WARMELTmiBASE_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 5)
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
    meltmodvarix_DIAGV_EDGE_symb = 5,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 6 MELT called hook declarations ***/

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

/*declare MELT called hook #5 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 6 MELT called hook declarations ***/

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




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h219695137 /*opaqueinitialclassy*/ meltinitial_frame_t;


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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1718 */

    /** start of frame for meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ// fromline 1534
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ), clos) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ() //the constructor fromline 1608
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ)) {};
        MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ


    /** end of frame for meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1663**/

    /* end of frame for routine meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1722 */

    /* classy proc frame meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ */
    MeltFrame_meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_9_WARMELTmiBASE_REGISTER_DIAG_GIMPLE_SEQ fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:529:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:530:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:531:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_GIMPLE_SEQ_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:529:/ clear");
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1718 */

    /** start of frame for meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE// fromline 1534
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE), clos) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE() //the constructor fromline 1608
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE)) {};
        MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE


    /** end of frame for meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1663**/

    /* end of frame for routine meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1722 */

    /* classy proc frame meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE */
    MeltFrame_meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_10_WARMELTmiBASE_REGISTER_DIAG_TREE fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:533:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:534:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:535:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_TREE_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:533:/ clear");
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1718 */

    /** start of frame for meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK// fromline 1534
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK), clos) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK() //the constructor fromline 1608
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK)) {};
        MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK


    /** end of frame for meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1663**/

    /* end of frame for routine meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1722 */

    /* classy proc frame meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK */
    MeltFrame_meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_11_WARMELTmiBASE_REGISTER_DIAG_BASIC_BLOCK fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:537:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:538:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:539:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_BASIC_BLOCK_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:537:/ clear");
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
meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE fromline 1718 */

    /** start of frame for meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE// fromline 1534
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE), clos) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE() //the constructor fromline 1608
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE)) {};
        MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE


    /** end of frame for meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE fromline 1663**/

    /* end of frame for routine meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE fromline 1722 */

    /* classy proc frame meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE */
    MeltFrame_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_DIAG_EDGE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-base.melt:541:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:542:/ cond");
        /*cond*/
        if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:543:/ putmodvar");
                    melt_module_var_put (meltmodvarix_DIAGV_EDGE_symb, (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]));
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:541:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_DIAG_EDGE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_12_WARMELTmiBASE_REGISTER_DIAG_EDGE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1718 */

    /** start of frame for meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO// fromline 1534
        : public Melt_CallFrameWithValues<310>
    {
    public: /* fromline 1538*/
        long mcfr_varnum[18];
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
        MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<310> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO), clos) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO() //the constructor fromline 1608
            : Melt_CallFrameWithValues<310> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<310> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO)) {};
        MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<310> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO


    /** end of frame for meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1663**/

    /* end of frame for routine meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1722 */

    /* classy proc frame meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO */
    MeltFrame_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:555:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:556:/ cppif.then");
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
            MELT_LOCATION("warmelt-base.melt:556:/ cond");
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
                        MELT_LOCATION("warmelt-base.melt:556:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 556;
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

                        MELT_LOCATION("warmelt-base.melt:556:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:556:/ locexp");
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

        {
            MELT_LOCATION("warmelt-base.melt:557:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "diagnostic_args_macro"), (12));
#endif
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:558:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-base.melt:558:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:558:/ cond.else");

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
                            argtab[2].meltbp_long = 558;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:558:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:558:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-base.melt:559:/ quasiblock");


        MELT_LOCATION("warmelt-base.melt:560:/ cond");
        /*cond*/
        if (
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
                    /*_.SLOC__V12*/
                    meltfptr[5] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SLOC__V12*/ meltfptr[5] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:561:/ cond");
        /*cond*/
        if (
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
                    /*_.SEXCONT__V13*/
                    meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SEXCONT__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:563:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L4*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:563:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L4*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-base.melt:563:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 563;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro sloc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V12*/ meltfptr[5];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " sexcont=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V13*/ meltfptr[12];
                            /*_.MELT_DEBUG_FUN__V15*/
                            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V14*/
                        meltfptr[13] = /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:563:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V15*/
                        meltfptr[14] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:563:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V14*/
            meltfptr[13] = 0 ;
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
        MELT_LOCATION("warmelt-base.melt:564:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L6*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.SEXCONT__V13*/ meltfptr[12])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-base.melt:564:/ cond");
            /*cond*/
            if (/*_#IS_LIST__L6*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:564:/ cond.else");

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
                            argtab[2].meltbp_long = 564;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V13*/ meltfptr[12];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:564:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:564:/ clear");
            /*clear*/ /*_#IS_LIST__L6*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-base.melt:565:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:566:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[5]);
            /*_.TCONT__V20*/
            meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.SEXCONT__V13*/ meltfptr[12]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#TCONTLEN__L7*/
        meltfnum[0] =
            (melt_multiple_length((melt_ptr_t)(/*_.TCONT__V20*/ meltfptr[13])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:569:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE__L8*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.TCONT__V20*/ meltfptr[13])) == MELTOBMAG_MULTIPLE);;
            MELT_LOCATION("warmelt-base.melt:569:/ cond");
            /*cond*/
            if (/*_#IS_MULTIPLE__L8*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V22*/ meltfptr[21] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:569:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check tcont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 569;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V20*/ meltfptr[13];
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V13*/ meltfptr[12];
                            /*_.MELT_ASSERT_FAILURE_FUN__V23*/
                            meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V22*/
                        meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:569:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V23*/
                        meltfptr[22] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V21*/
            meltfptr[20] = /*_._IFELSE___V22*/ meltfptr[21];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:569:/ clear");
            /*clear*/ /*_#IS_MULTIPLE__L8*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V22*/
            meltfptr[21] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V21*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:570:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#gtI__L9*/
            meltfnum[1] =
                ((/*_#TCONTLEN__L7*/ meltfnum[0]) > (0));;
            MELT_LOCATION("warmelt-base.melt:570:/ cond");
            /*cond*/
            if (/*_#gtI__L9*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V25*/ meltfptr[21] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:570:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check tcontlen";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 570;
                            /*^apply.arg*/
                            argtab[3].meltbp_long = /*_#TCONTLEN__L7*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V20*/ meltfptr[13];
                            /*_.MELT_ASSERT_FAILURE_FUN__V26*/
                            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V25*/
                        meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:570:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V26*/
                        meltfptr[25] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/
            meltfptr[22] = /*_._IFELSE___V25*/ meltfptr[21];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:570:/ clear");
            /*clear*/ /*_#gtI__L9*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V25*/
            meltfptr[21] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#exeqI__L10*/
        meltfnum[1] =
            ((/*_#TCONTLEN__L7*/ meltfnum[0]) != (3));;
        MELT_LOCATION("warmelt-base.melt:571:/ cond");
        /*cond*/
        if (/*_#exeqI__L10*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:572:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "DIAGNOSTIC_ARGS_MACRO needs two args: <nbargs> <vect>"), (melt_ptr_t)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:573:/ locexp");
                        /* diagnostic_args_macro DIAGARGBAD_CHK__1 */
                        melt_low_stderr_value ("diagnostic_args_macro sexp", /*_.SEXP__V2*/ meltfptr[1]) ;
                        melt_low_stderr_value ("diagnostic_args_macro sexcont", /*_.SEXCONT__V13*/ meltfptr[12]) ;
                        melt_low_stderr_value ("diagnostic_args_macro tcont", /*_.TCONT__V20*/ meltfptr[13]) ;
                        ;
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
                        argtab[0].meltbp_long = /*_#TCONTLEN__L7*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[1].meltbp_cstring =  " args == ";
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V20*/ meltfptr[13];
                        /*_.STRING4OUT__V28*/
                        meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!STRING4OUT*/ meltfrout->tabval[6])), (melt_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[7])), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:578:/ locexp");
                        melt_inform_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "DIAGNOSTIC_ARGS_MACRO called with "), (melt_ptr_t)(/*_.STRING4OUT__V28*/ meltfptr[21]));
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-base.melt:580:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_IS_BOOTSTRAPPING__L11*/
                        meltfnum[10] =
                            (melt_flag_bootstrapping);;
                        /*^compute*/
                        /*_#NOT__L12*/
                        meltfnum[11] =
                            (!(/*_#MELT_IS_BOOTSTRAPPING__L11*/ meltfnum[10]));;
                        MELT_LOCATION("warmelt-base.melt:580:/ cond");
                        /*cond*/
                        if (/*_#NOT__L12*/ meltfnum[11]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V30*/ meltfptr[29] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-base.melt:580:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[6];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "diagnostic_args_macro wrongly called when bootstrapping";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-base.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 580;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXCONT__V13*/ meltfptr[12];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.TCONT__V20*/ meltfptr[13];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V31*/
                                        meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V30*/
                                    meltfptr[29] = /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[30];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:580:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V31*/
                                    meltfptr[30] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V29*/
                        meltfptr[28] = /*_._IFELSE___V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:580:/ clear");
                        /*clear*/ /*_#MELT_IS_BOOTSTRAPPING__L11*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#NOT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V30*/
                        meltfptr[29] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V29*/ meltfptr[28] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-base.melt:582:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:582:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:571:/ quasiblock");


                    /*_.PROGN___V33*/
                    meltfptr[29] = /*_.RETURN___V32*/ meltfptr[30];;
                    /*^compute*/
                    /*_._IF___V27*/
                    meltfptr[25] = /*_.PROGN___V33*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:571:/ clear");
                    /*clear*/ /*_.STRING4OUT__V28*/
                    meltfptr[21] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.IFCPP___V29*/
                    meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V32*/
                    meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V33*/
                    meltfptr[29] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V27*/ meltfptr[25] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:584:/ quasiblock");


        /*_.MACROVARNBARG__V35*/
        meltfptr[28] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V20*/ meltfptr[13]), (1)));;
        /*^compute*/
        /*_.MACROVARVECT__V36*/
        meltfptr[30] =
            (melt_multiple_nth((melt_ptr_t)(/*_.TCONT__V20*/ meltfptr[13]), (2)));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L13*/
        meltfnum[10] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MACROVARNBARG__V35*/ meltfptr[28]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[8])));;
        MELT_LOCATION("warmelt-base.melt:587:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L13*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:588:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "(DIAGNOSTIC_ARGS_MACRO <nbargs> <vect>) not variable <nbargs>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:590:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:590:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:587:/ quasiblock");


                    /*_.PROGN___V39*/
                    meltfptr[38] = /*_.RETURN___V38*/ meltfptr[37];;
                    /*^compute*/
                    /*_._IF___V37*/
                    meltfptr[29] = /*_.PROGN___V39*/ meltfptr[38];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:587:/ clear");
                    /*clear*/ /*_.RETURN___V38*/
                    meltfptr[37] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V39*/
                    meltfptr[38] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V37*/ meltfptr[29] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L14*/
        meltfnum[11] =
            !melt_is_instance_of((melt_ptr_t)(/*_.MACROVARVECT__V36*/ meltfptr[30]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[8])));;
        MELT_LOCATION("warmelt-base.melt:591:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L14*/ meltfnum[11]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-base.melt:592:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V12*/ meltfptr[5]), ( "(DIAGNOSTIC_ARGS_MACRO <nbargs> <vect>) not variable <vect>"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:594:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:594:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:591:/ quasiblock");


                    /*_.PROGN___V42*/
                    meltfptr[41] = /*_.RETURN___V41*/ meltfptr[38];;
                    /*^compute*/
                    /*_._IF___V40*/
                    meltfptr[37] = /*_.PROGN___V42*/ meltfptr[41];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:591:/ clear");
                    /*clear*/ /*_.RETURN___V41*/
                    meltfptr[38] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V42*/
                    meltfptr[41] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V40*/ meltfptr[37] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:595:/ quasiblock");


        /*_.HOOK_MELT_MAKE_LOCATION__V44*/
        meltfptr[41] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V45*/
        meltfptr[44] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V46*/
        meltfptr[45] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V47*/
        meltfptr[46] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 596) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V48*/
        meltfptr[47] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 599) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:599:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_18_MAKE_MAPOBJECT*/ meltfrout->tabval[18]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_19_DISCR_MAP_OBJECTS*/ meltfrout->tabval[19]);
            /*^apply.arg*/
            argtab[2].meltbp_long = 19;
            /*_.MELT_MAKE_SEXPR__V49*/
            meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V48*/ meltfptr[47]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:596:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[8];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_13_INSTANCE*/ meltfrout->tabval[13]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_14_CLASS_DIAGNOSTIC_STATE*/ meltfrout->tabval[14]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_15_LOCA_LOCATION*/ meltfrout->tabval[15]);
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V12*/ meltfptr[5];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*!konst_16_DIAG_VECT*/ meltfrout->tabval[16]);
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!konst_17_DIAG_STATE*/ meltfrout->tabval[17]);
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V49*/ meltfptr[48];
            /*_.MELT_MAKE_SEXPR__V50*/
            meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V47*/ meltfptr[46]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_DIAGSTATE*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V50*/ meltfptr[49];
            /*_.MELT_MAKE_SEXPR__V51*/
            meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V46*/ meltfptr[45]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V51*/ meltfptr[50];
            /*_.MELT_MAKE_SEXPR__V52*/
            meltfptr[51] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V45*/ meltfptr[44]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V53*/
        meltfptr[52] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 601) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V54*/
        meltfptr[53] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 603) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V55*/
        meltfptr[54] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 604) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V56*/
        meltfptr[55] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 605) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:605:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_23_EXIT*/ meltfrout->tabval[23]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_ARGLOOP*/ meltfrout->tabval[21]);
            /*_.MELT_MAKE_SEXPR__V57*/
            meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V56*/ meltfptr[55]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:604:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V57*/ meltfptr[56];
            /*_.MELT_MAKE_SEXPR__V58*/
            meltfptr[57] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V55*/ meltfptr[54]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V59*/
        meltfptr[58] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 606) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V60*/
        meltfptr[59] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 606) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:606:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_25_CSTRING*/ meltfrout->tabval[25]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_26_STR*/ meltfrout->tabval[26]);
            /*_.MELT_MAKE_SEXPR__V61*/
            meltfptr[60] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V60*/ meltfptr[59]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V62*/
        meltfptr[61] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 607) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V63*/
        meltfptr[62] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 607) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V64*/
        meltfptr[63] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 607) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V65*/
        meltfptr[64] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 607) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:607:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_29_CONSTANT_BOX*/ meltfrout->tabval[29]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_26_STR*/ meltfrout->tabval[26]);
            /*_.MELT_MAKE_SEXPR__V66*/
            meltfptr[65] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V65*/ meltfptr[64]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_28_BS*/ meltfrout->tabval[28]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V66*/ meltfptr[65];
            /*_.MELT_MAKE_SEXPR__V67*/
            meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V64*/ meltfptr[63]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V67*/ meltfptr[66];
            /*_.MELT_MAKE_SEXPR__V68*/
            meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V63*/ meltfptr[62]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V69*/
        meltfptr[68] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 609) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:609:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_28_BS*/ meltfrout->tabval[28]);
            /*_.MELT_MAKE_SEXPR__V70*/
            meltfptr[69] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V69*/ meltfptr[68]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V71*/
        meltfptr[70] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 610) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V72*/
        meltfptr[71] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 610) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:610:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V73*/
            meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V72*/ meltfptr[71]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V73*/ meltfptr[72];
            /*_.MELT_MAKE_SEXPR__V74*/
            meltfptr[73] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V71*/ meltfptr[70]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:607:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V68*/ meltfptr[67];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V70*/ meltfptr[69];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V74*/ meltfptr[73];
            /*_.MELT_MAKE_SEXPR__V75*/
            meltfptr[74] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V62*/ meltfptr[61]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V76*/
        meltfptr[75] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 611) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:611:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V77*/
            meltfptr[76] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V76*/ meltfptr[75]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:606:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V61*/ meltfptr[60];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V75*/ meltfptr[74];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V77*/ meltfptr[76];
            /*_.MELT_MAKE_SEXPR__V78*/
            meltfptr[77] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V59*/ meltfptr[58]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V79*/
        meltfptr[78] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 613) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V80*/
        meltfptr[79] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 613) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:613:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_36_LONG*/ meltfrout->tabval[36]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_L*/ meltfrout->tabval[37]);
            /*_.MELT_MAKE_SEXPR__V81*/
            meltfptr[80] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V80*/ meltfptr[79]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V82*/
        meltfptr[81] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 614) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V83*/
        meltfptr[82] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 614) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V84*/
        meltfptr[83] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 614) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V85*/
        meltfptr[84] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 614) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:614:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_40_STRING4OUT*/ meltfrout->tabval[40]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_DISCR_STRING*/ meltfrout->tabval[41]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_37_L*/ meltfrout->tabval[37]);
            /*_.MELT_MAKE_SEXPR__V86*/
            meltfptr[85] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V85*/ meltfptr[84]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_SN*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V86*/ meltfptr[85];
            /*_.MELT_MAKE_SEXPR__V87*/
            meltfptr[86] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V84*/ meltfptr[83]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V87*/ meltfptr[86];
            /*_.MELT_MAKE_SEXPR__V88*/
            meltfptr[87] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V83*/ meltfptr[82]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V89*/
        meltfptr[88] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 616) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:616:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_SN*/ meltfrout->tabval[39]);
            /*_.MELT_MAKE_SEXPR__V90*/
            meltfptr[89] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V89*/ meltfptr[88]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V91*/
        meltfptr[90] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 617) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V92*/
        meltfptr[91] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 617) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:617:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V93*/
            meltfptr[92] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V92*/ meltfptr[91]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V93*/ meltfptr[92];
            /*_.MELT_MAKE_SEXPR__V94*/
            meltfptr[93] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V91*/ meltfptr[90]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:614:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V88*/ meltfptr[87];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V90*/ meltfptr[89];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V94*/ meltfptr[93];
            /*_.MELT_MAKE_SEXPR__V95*/
            meltfptr[94] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V82*/ meltfptr[81]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V96*/
        meltfptr[95] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 618) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:618:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V97*/
            meltfptr[96] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V96*/ meltfptr[95]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:613:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V81*/ meltfptr[80];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V95*/ meltfptr[94];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V97*/ meltfptr[96];
            /*_.MELT_MAKE_SEXPR__V98*/
            meltfptr[97] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V79*/ meltfptr[78]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V99*/
        meltfptr[98] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 620) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V100*/
        meltfptr[99] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 620) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:620:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_48_TREE*/ meltfrout->tabval[48]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_49_TR*/ meltfrout->tabval[49]);
            /*_.MELT_MAKE_SEXPR__V101*/
            meltfptr[100] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V100*/ meltfptr[99]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V102*/
        meltfptr[101] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 621) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V103*/
        meltfptr[102] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 621) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:621:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_51_IS_CLOSURE*/ meltfrout->tabval[51]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_52_DIAGV_TREE*/ meltfrout->tabval[52]);
            /*_.MELT_MAKE_SEXPR__V104*/
            meltfptr[103] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V103*/ meltfptr[102]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_tree";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V104*/ meltfptr[103];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_52_DIAGV_TREE*/ meltfrout->tabval[52]);
            /*_.MELT_MAKE_SEXPR__V105*/
            meltfptr[104] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V102*/ meltfptr[101]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V106*/
        meltfptr[105] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 622) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V107*/
        meltfptr[106] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 622) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V108*/
        meltfptr[107] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 622) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V109*/
        meltfptr[108] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 622) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:622:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_52_DIAGV_TREE*/ meltfrout->tabval[52]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_DIAGSTATE*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_49_TR*/ meltfrout->tabval[49]);
            /*_.MELT_MAKE_SEXPR__V110*/
            meltfptr[109] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V109*/ meltfptr[108]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V110*/ meltfptr[109];
            /*_.MELT_MAKE_SEXPR__V111*/
            meltfptr[110] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V108*/ meltfptr[107]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V111*/ meltfptr[110];
            /*_.MELT_MAKE_SEXPR__V112*/
            meltfptr[111] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V107*/ meltfptr[106]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V113*/
        meltfptr[112] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 624) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:624:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V114*/
            meltfptr[113] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V113*/ meltfptr[112]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V115*/
        meltfptr[114] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 625) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V116*/
        meltfptr[115] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 625) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:625:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V117*/
            meltfptr[116] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V116*/ meltfptr[115]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V117*/ meltfptr[116];
            /*_.MELT_MAKE_SEXPR__V118*/
            meltfptr[117] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V115*/ meltfptr[114]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:622:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V112*/ meltfptr[111];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V114*/ meltfptr[113];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V118*/ meltfptr[117];
            /*_.MELT_MAKE_SEXPR__V119*/
            meltfptr[118] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V106*/ meltfptr[105]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V120*/
        meltfptr[119] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 626) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:626:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V121*/
            meltfptr[120] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V120*/ meltfptr[119]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:620:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V101*/ meltfptr[100];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V105*/ meltfptr[104];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V119*/ meltfptr[118];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V121*/ meltfptr[120];
            /*_.MELT_MAKE_SEXPR__V122*/
            meltfptr[121] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V99*/ meltfptr[98]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V123*/
        meltfptr[122] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 628) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V124*/
        meltfptr[123] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 628) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:628:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_64_GIMPLE*/ meltfrout->tabval[64]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_65_GI*/ meltfrout->tabval[65]);
            /*_.MELT_MAKE_SEXPR__V125*/
            meltfptr[124] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V124*/ meltfptr[123]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V126*/
        meltfptr[125] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 629) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V127*/
        meltfptr[126] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 629) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:629:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_51_IS_CLOSURE*/ meltfrout->tabval[51]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_68_DIAGV_GIMPLE*/ meltfrout->tabval[68]);
            /*_.MELT_MAKE_SEXPR__V128*/
            meltfptr[127] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V127*/ meltfptr[126]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_gimple";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V128*/ meltfptr[127];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_68_DIAGV_GIMPLE*/ meltfrout->tabval[68]);
            /*_.MELT_MAKE_SEXPR__V129*/
            meltfptr[128] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V126*/ meltfptr[125]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V130*/
        meltfptr[129] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 630) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V131*/
        meltfptr[130] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 630) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V132*/
        meltfptr[131] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 630) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V133*/
        meltfptr[132] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 630) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:630:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_68_DIAGV_GIMPLE*/ meltfrout->tabval[68]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_DIAGSTATE*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_65_GI*/ meltfrout->tabval[65]);
            /*_.MELT_MAKE_SEXPR__V134*/
            meltfptr[133] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V133*/ meltfptr[132]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V134*/ meltfptr[133];
            /*_.MELT_MAKE_SEXPR__V135*/
            meltfptr[134] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V132*/ meltfptr[131]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V135*/ meltfptr[134];
            /*_.MELT_MAKE_SEXPR__V136*/
            meltfptr[135] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V131*/ meltfptr[130]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V137*/
        meltfptr[136] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 632) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:632:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V138*/
            meltfptr[137] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V137*/ meltfptr[136]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V139*/
        meltfptr[138] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 633) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V140*/
        meltfptr[139] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 633) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:633:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V141*/
            meltfptr[140] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V140*/ meltfptr[139]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V141*/ meltfptr[140];
            /*_.MELT_MAKE_SEXPR__V142*/
            meltfptr[141] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V139*/ meltfptr[138]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:630:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V136*/ meltfptr[135];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V138*/ meltfptr[137];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V142*/ meltfptr[141];
            /*_.MELT_MAKE_SEXPR__V143*/
            meltfptr[142] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V130*/ meltfptr[129]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V144*/
        meltfptr[143] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 634) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:634:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V145*/
            meltfptr[144] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V144*/ meltfptr[143]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V125*/ meltfptr[124];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V129*/ meltfptr[128];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V143*/ meltfptr[142];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V145*/ meltfptr[144];
            /*_.MELT_MAKE_SEXPR__V146*/
            meltfptr[145] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V123*/ meltfptr[122]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V147*/
        meltfptr[146] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 636) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V148*/
        meltfptr[147] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 636) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:636:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_80_GIMPLE_SEQ*/ meltfrout->tabval[80]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_81_GS*/ meltfrout->tabval[81]);
            /*_.MELT_MAKE_SEXPR__V149*/
            meltfptr[148] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V148*/ meltfptr[147]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V150*/
        meltfptr[149] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 637) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V151*/
        meltfptr[150] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 637) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:637:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_51_IS_CLOSURE*/ meltfrout->tabval[51]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_84_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[84]);
            /*_.MELT_MAKE_SEXPR__V152*/
            meltfptr[151] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V151*/ meltfptr[150]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_gimple_seq";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V152*/ meltfptr[151];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_84_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[84]);
            /*_.MELT_MAKE_SEXPR__V153*/
            meltfptr[152] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V150*/ meltfptr[149]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V154*/
        meltfptr[153] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 638) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V155*/
        meltfptr[154] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 638) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V156*/
        meltfptr[155] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 638) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V157*/
        meltfptr[156] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 638) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:638:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_84_DIAGV_GIMPLE_SEQ*/ meltfrout->tabval[84]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_DIAGSTATE*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_81_GS*/ meltfrout->tabval[81]);
            /*_.MELT_MAKE_SEXPR__V158*/
            meltfptr[157] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V157*/ meltfptr[156]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V158*/ meltfptr[157];
            /*_.MELT_MAKE_SEXPR__V159*/
            meltfptr[158] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V156*/ meltfptr[155]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V159*/ meltfptr[158];
            /*_.MELT_MAKE_SEXPR__V160*/
            meltfptr[159] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V155*/ meltfptr[154]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V161*/
        meltfptr[160] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 640) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:640:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V162*/
            meltfptr[161] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V161*/ meltfptr[160]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V163*/
        meltfptr[162] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 641) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V164*/
        meltfptr[163] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 641) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:641:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V165*/
            meltfptr[164] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V164*/ meltfptr[163]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V165*/ meltfptr[164];
            /*_.MELT_MAKE_SEXPR__V166*/
            meltfptr[165] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V163*/ meltfptr[162]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:638:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V160*/ meltfptr[159];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V162*/ meltfptr[161];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V166*/ meltfptr[165];
            /*_.MELT_MAKE_SEXPR__V167*/
            meltfptr[166] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V154*/ meltfptr[153]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V168*/
        meltfptr[167] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 642) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:642:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V169*/
            meltfptr[168] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V168*/ meltfptr[167]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:636:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V149*/ meltfptr[148];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V153*/ meltfptr[152];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V167*/ meltfptr[166];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V169*/ meltfptr[168];
            /*_.MELT_MAKE_SEXPR__V170*/
            meltfptr[169] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V147*/ meltfptr[146]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V171*/
        meltfptr[170] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 644) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V172*/
        meltfptr[171] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 644) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:644:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_96_BASIC_BLOCK*/ meltfrout->tabval[96]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_BB*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V173*/
            meltfptr[172] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V172*/ meltfptr[171]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V174*/
        meltfptr[173] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 645) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V175*/
        meltfptr[174] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 645) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:645:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_51_IS_CLOSURE*/ meltfrout->tabval[51]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_100_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[100]);
            /*_.MELT_MAKE_SEXPR__V176*/
            meltfptr[175] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V175*/ meltfptr[174]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_basic_block";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V176*/ meltfptr[175];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_100_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[100]);
            /*_.MELT_MAKE_SEXPR__V177*/
            meltfptr[176] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V174*/ meltfptr[173]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V178*/
        meltfptr[177] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 646) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V179*/
        meltfptr[178] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 646) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V180*/
        meltfptr[179] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 646) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V181*/
        meltfptr[180] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 646) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:646:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_100_DIAGV_BASIC_BLOCK*/ meltfrout->tabval[100]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_DIAGSTATE*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_97_BB*/ meltfrout->tabval[97]);
            /*_.MELT_MAKE_SEXPR__V182*/
            meltfptr[181] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V181*/ meltfptr[180]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V182*/ meltfptr[181];
            /*_.MELT_MAKE_SEXPR__V183*/
            meltfptr[182] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V180*/ meltfptr[179]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            meltfptr[183] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V179*/ meltfptr[178]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V185*/
        meltfptr[184] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 648) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:648:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V186*/
            meltfptr[185] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V185*/ meltfptr[184]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V187*/
        meltfptr[186] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 649) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V188*/
        meltfptr[187] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 649) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:649:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V189*/
            meltfptr[188] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V188*/ meltfptr[187]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V189*/ meltfptr[188];
            /*_.MELT_MAKE_SEXPR__V190*/
            meltfptr[189] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V187*/ meltfptr[186]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:646:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V184*/ meltfptr[183];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V186*/ meltfptr[185];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V190*/ meltfptr[189];
            /*_.MELT_MAKE_SEXPR__V191*/
            meltfptr[190] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V178*/ meltfptr[177]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V192*/
        meltfptr[191] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 650) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:650:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V193*/
            meltfptr[192] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V192*/ meltfptr[191]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:644:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V173*/ meltfptr[172];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V177*/ meltfptr[176];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V191*/ meltfptr[190];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V193*/ meltfptr[192];
            /*_.MELT_MAKE_SEXPR__V194*/
            meltfptr[193] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V171*/ meltfptr[170]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V195*/
        meltfptr[194] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 652) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V196*/
        meltfptr[195] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 652) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:652:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_112_EDGE*/ meltfrout->tabval[112]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_113_ED*/ meltfrout->tabval[113]);
            /*_.MELT_MAKE_SEXPR__V197*/
            meltfptr[196] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V196*/ meltfptr[195]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V198*/
        meltfptr[197] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 653) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V199*/
        meltfptr[198] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 653) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:653:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_51_IS_CLOSURE*/ meltfrout->tabval[51]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_116_DIAGV_EDGE*/ meltfrout->tabval[116]);
            /*_.MELT_MAKE_SEXPR__V200*/
            meltfptr[199] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V199*/ meltfptr[198]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check diagv_edge";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V200*/ meltfptr[199];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_116_DIAGV_EDGE*/ meltfrout->tabval[116]);
            /*_.MELT_MAKE_SEXPR__V201*/
            meltfptr[200] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V198*/ meltfptr[197]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V202*/
        meltfptr[201] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 654) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V203*/
        meltfptr[202] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 654) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V204*/
        meltfptr[203] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 654) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V205*/
        meltfptr[204] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 654) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:654:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_116_DIAGV_EDGE*/ meltfrout->tabval[116]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_12_DIAGSTATE*/ meltfrout->tabval[12]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_113_ED*/ meltfrout->tabval[113]);
            /*_.MELT_MAKE_SEXPR__V206*/
            meltfptr[205] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V205*/ meltfptr[204]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V206*/ meltfptr[205];
            /*_.MELT_MAKE_SEXPR__V207*/
            meltfptr[206] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V204*/ meltfptr[203]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V207*/ meltfptr[206];
            /*_.MELT_MAKE_SEXPR__V208*/
            meltfptr[207] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V203*/ meltfptr[202]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V209*/
        meltfptr[208] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 656) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:656:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_55_S*/ meltfrout->tabval[55]);
            /*_.MELT_MAKE_SEXPR__V210*/
            meltfptr[209] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V209*/ meltfptr[208]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V211*/
        meltfptr[210] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 657) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V212*/
        meltfptr[211] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 657) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:657:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V213*/
            meltfptr[212] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V212*/ meltfptr[211]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V213*/ meltfptr[212];
            /*_.MELT_MAKE_SEXPR__V214*/
            meltfptr[213] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V211*/ meltfptr[210]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:654:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V208*/ meltfptr[207];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V210*/ meltfptr[209];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V214*/ meltfptr[213];
            /*_.MELT_MAKE_SEXPR__V215*/
            meltfptr[214] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V202*/ meltfptr[201]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V216*/
        meltfptr[215] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 658) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:658:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V217*/
            meltfptr[216] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V216*/ meltfptr[215]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:652:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V197*/ meltfptr[196];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V201*/ meltfptr[200];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V215*/ meltfptr[214];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V217*/ meltfptr[216];
            /*_.MELT_MAKE_SEXPR__V218*/
            meltfptr[217] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V195*/ meltfptr[194]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V219*/
        meltfptr[218] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 660) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V220*/
        meltfptr[219] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 660) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:660:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_128_VALUE*/ meltfrout->tabval[128]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*_.MELT_MAKE_SEXPR__V221*/
            meltfptr[220] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V220*/ meltfptr[219]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V222*/
        meltfptr[221] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 661) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V223*/
        meltfptr[222] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 662) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V224*/
        meltfptr[223] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 662) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:662:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_131_IS_INTEGERBOX*/ meltfrout->tabval[131]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*_.MELT_MAKE_SEXPR__V225*/
            meltfptr[224] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V224*/ meltfptr[223]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V226*/
        meltfptr[225] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 663) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V227*/
        meltfptr[226] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 663) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V228*/
        meltfptr[227] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 663) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V229*/
        meltfptr[228] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 663) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V230*/
        meltfptr[229] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 663) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:663:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_137_GET_INT*/ meltfrout->tabval[137]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*_.MELT_MAKE_SEXPR__V231*/
            meltfptr[230] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V230*/ meltfptr[229]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_40_STRING4OUT*/ meltfrout->tabval[40]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_41_DISCR_STRING*/ meltfrout->tabval[41]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V231*/ meltfptr[230];
            /*_.MELT_MAKE_SEXPR__V232*/
            meltfptr[231] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V229*/ meltfptr[228]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_SN*/ meltfrout->tabval[39]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V232*/ meltfptr[231];
            /*_.MELT_MAKE_SEXPR__V233*/
            meltfptr[232] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V228*/ meltfptr[227]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V233*/ meltfptr[232];
            /*_.MELT_MAKE_SEXPR__V234*/
            meltfptr[233] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V227*/ meltfptr[226]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V235*/
        meltfptr[234] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 665) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:665:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_39_SN*/ meltfrout->tabval[39]);
            /*_.MELT_MAKE_SEXPR__V236*/
            meltfptr[235] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V235*/ meltfptr[234]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V237*/
        meltfptr[236] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 666) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V238*/
        meltfptr[237] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 666) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:666:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V239*/
            meltfptr[238] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V238*/ meltfptr[237]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V239*/ meltfptr[238];
            /*_.MELT_MAKE_SEXPR__V240*/
            meltfptr[239] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V237*/ meltfptr[236]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:663:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V234*/ meltfptr[233];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V236*/ meltfptr[235];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V240*/ meltfptr[239];
            /*_.MELT_MAKE_SEXPR__V241*/
            meltfptr[240] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V226*/ meltfptr[225]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V242*/
        meltfptr[241] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 667) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:667:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V243*/
            meltfptr[242] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V242*/ meltfptr[241]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:662:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V225*/ meltfptr[224];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V241*/ meltfptr[240];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V243*/ meltfptr[242];
            /*_.MELT_MAKE_SEXPR__V244*/
            meltfptr[243] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V223*/ meltfptr[222]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V245*/
        meltfptr[244] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 669) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V246*/
        meltfptr[245] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 669) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:669:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_144_IS_STRING*/ meltfrout->tabval[144]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*_.MELT_MAKE_SEXPR__V247*/
            meltfptr[246] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V246*/ meltfptr[245]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V248*/
        meltfptr[247] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 670) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:670:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*_.MELT_MAKE_SEXPR__V249*/
            meltfptr[248] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V248*/ meltfptr[247]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V250*/
        meltfptr[249] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 671) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V251*/
        meltfptr[250] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 671) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:671:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V252*/
            meltfptr[251] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V251*/ meltfptr[250]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V252*/ meltfptr[251];
            /*_.MELT_MAKE_SEXPR__V253*/
            meltfptr[252] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V250*/ meltfptr[249]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V254*/
        meltfptr[253] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 672) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:672:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V255*/
            meltfptr[254] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V254*/ meltfptr[253]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:669:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V247*/ meltfptr[246];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V249*/ meltfptr[248];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V253*/ meltfptr[252];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V255*/ meltfptr[254];
            /*_.MELT_MAKE_SEXPR__V256*/
            meltfptr[255] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V245*/ meltfptr[244]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V257*/
        meltfptr[256] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 674) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V258*/
        meltfptr[257] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 674) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:674:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_151_IS_A*/ meltfrout->tabval[151]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_153_CLASS_NAMED*/ meltfrout->tabval[153]);
            /*_.MELT_MAKE_SEXPR__V259*/
            meltfptr[258] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V258*/ meltfptr[257]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V260*/
        meltfptr[259] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 675) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V261*/
        meltfptr[260] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 675) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V262*/
        meltfptr[261] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 675) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V263*/
        meltfptr[262] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 675) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:675:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_156_UNSAFE_GET_FIELD*/ meltfrout->tabval[156]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_157_NAMED_NAME*/ meltfrout->tabval[157]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_129_V*/ meltfrout->tabval[129]);
            /*_.MELT_MAKE_SEXPR__V264*/
            meltfptr[263] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V263*/ meltfptr[262]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_155_NAM*/ meltfrout->tabval[155]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V264*/ meltfptr[263];
            /*_.MELT_MAKE_SEXPR__V265*/
            meltfptr[264] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V262*/ meltfptr[261]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V265*/ meltfptr[264];
            /*_.MELT_MAKE_SEXPR__V266*/
            meltfptr[265] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V261*/ meltfptr[260]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V267*/
        meltfptr[266] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 677) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V268*/
        meltfptr[267] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 677) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:677:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_144_IS_STRING*/ meltfrout->tabval[144]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_155_NAM*/ meltfrout->tabval[155]);
            /*_.MELT_MAKE_SEXPR__V269*/
            meltfptr[268] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V268*/ meltfptr[267]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "check nam";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V269*/ meltfptr[268];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_155_NAM*/ meltfrout->tabval[155]);
            /*_.MELT_MAKE_SEXPR__V270*/
            meltfptr[269] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V267*/ meltfptr[266]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V271*/
        meltfptr[270] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 678) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:678:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_31_MULTIPLE_PUT_NTH*/ meltfrout->tabval[31]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARVECT__V36*/ meltfptr[30];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_155_NAM*/ meltfrout->tabval[155]);
            /*_.MELT_MAKE_SEXPR__V272*/
            meltfptr[271] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V271*/ meltfptr[270]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V273*/
        meltfptr[272] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 679) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V274*/
        meltfptr[273] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 679) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:679:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_34_pl*/ meltfrout->tabval[34]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_long = 1;
            /*_.MELT_MAKE_SEXPR__V275*/
            meltfptr[274] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V274*/ meltfptr[273]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_33_SETQ*/ meltfrout->tabval[33]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MACROVARNBARG__V35*/ meltfptr[28];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V275*/ meltfptr[274];
            /*_.MELT_MAKE_SEXPR__V276*/
            meltfptr[275] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V273*/ meltfptr[272]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:675:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V266*/ meltfptr[265];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V270*/ meltfptr[269];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V272*/ meltfptr[271];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V276*/ meltfptr[275];
            /*_.MELT_MAKE_SEXPR__V277*/
            meltfptr[276] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V260*/ meltfptr[259]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V278*/
        meltfptr[277] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 680) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:680:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V279*/
            meltfptr[278] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V278*/ meltfptr[277]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:674:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V259*/ meltfptr[258];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V277*/ meltfptr[276];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V279*/ meltfptr[278];
            /*_.MELT_MAKE_SEXPR__V280*/
            meltfptr[279] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V257*/ meltfptr[256]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:661:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_130_COND*/ meltfrout->tabval[130]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V244*/ meltfptr[243];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V256*/ meltfptr[255];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V280*/ meltfptr[279];
            /*_.MELT_MAKE_SEXPR__V281*/
            meltfptr[280] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V222*/ meltfptr[221]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:660:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V221*/ meltfptr[220];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V281*/ meltfptr[280];
            /*_.MELT_MAKE_SEXPR__V282*/
            meltfptr[281] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V219*/ meltfptr[218]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V283*/
        meltfptr[282] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 684) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V284*/
        meltfptr[283] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 685) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V285*/
        meltfptr[284] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 685) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V286*/
        meltfptr[285] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 685) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V287*/
        meltfptr[286] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 685) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:685:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_171_VARIADIC_CTYPE*/ meltfrout->tabval[171]);
            /*^apply.arg*/
            argtab[1].meltbp_long = 0;
            /*_.MELT_MAKE_SEXPR__V288*/
            meltfptr[287] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V287*/ meltfptr[286]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_170_VCTY*/ meltfrout->tabval[170]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V288*/ meltfptr[287];
            /*_.MELT_MAKE_SEXPR__V289*/
            meltfptr[288] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V286*/ meltfptr[285]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V289*/ meltfptr[288];
            /*_.MELT_MAKE_SEXPR__V290*/
            meltfptr[289] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V285*/ meltfptr[284]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V291*/
        meltfptr[290] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 687) ;;
        /*^compute*/
        /*_.HOOK_MELT_MAKE_LOCATION__V292*/
        meltfptr[291] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 688) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:688:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_173_GET_FIELD*/ meltfrout->tabval[173]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_157_NAMED_NAME*/ meltfrout->tabval[157]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*!konst_170_VCTY*/ meltfrout->tabval[170]);
            /*_.MELT_MAKE_SEXPR__V293*/
            meltfptr[292] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V292*/ meltfptr[291]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:687:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_172_ERRORMSG_STRV*/ meltfrout->tabval[172]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "DIAGNOSTIC_ARGS_MACRO for unsupported ctype";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V293*/ meltfptr[292];
            /*_.MELT_MAKE_SEXPR__V294*/
            meltfptr[293] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V291*/ meltfptr[290]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V295*/
        meltfptr[294] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 689) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:689:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_50_ASSERT_MSG*/ meltfrout->tabval[50]);
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "invalid variadic argument to DIAGNOSTIC_ARGS_MACRO";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!konst_170_VCTY*/ meltfrout->tabval[170]);
            /*_.MELT_MAKE_SEXPR__V296*/
            meltfptr[295] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V295*/ meltfptr[294]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:685:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V290*/ meltfptr[289];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V294*/ meltfptr[293];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V296*/ meltfptr[295];
            /*_.MELT_MAKE_SEXPR__V297*/
            meltfptr[296] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V284*/ meltfptr[283]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.HOOK_MELT_MAKE_LOCATION__V298*/
        meltfptr[297] =
            /*hookcall*/ melthook_HOOK_MELT_MAKE_LOCATION ((melt_ptr_t) (/*!HOOK_MELT_MAKE_LOCATION*/ meltfrout->tabval[10]),  "warmelt-base.melt", 690) ;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:690:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_35_VOID*/ meltfrout->tabval[35]);
            /*_.MELT_MAKE_SEXPR__V299*/
            meltfptr[298] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V298*/ meltfptr[297]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:684:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_168_ELSE*/ meltfrout->tabval[168]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V297*/ meltfptr[296];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V299*/ meltfptr[298];
            /*_.MELT_MAKE_SEXPR__V300*/
            meltfptr[299] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V283*/ meltfptr[282]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:603:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[11];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_22_VARIADIC*/ meltfrout->tabval[22]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V58*/ meltfptr[57];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V78*/ meltfptr[77];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V98*/ meltfptr[97];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V122*/ meltfptr[121];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V146*/ meltfptr[145];
            /*^apply.arg*/
            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V170*/ meltfptr[169];
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V194*/ meltfptr[193];
            /*^apply.arg*/
            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V218*/ meltfptr[217];
            /*^apply.arg*/
            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V282*/ meltfptr[281];
            /*^apply.arg*/
            argtab[10].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V300*/ meltfptr[299];
            /*_.MELT_MAKE_SEXPR__V301*/
            meltfptr[300] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V54*/ meltfptr[53]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:601:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_20_FOREVER*/ meltfrout->tabval[20]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!konst_21_ARGLOOP*/ meltfrout->tabval[21]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V301*/ meltfptr[300];
            /*_.MELT_MAKE_SEXPR__V302*/
            meltfptr[301] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V53*/ meltfptr[52]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:596:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_11_LET*/ meltfrout->tabval[11]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V52*/ meltfptr[51];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MELT_MAKE_SEXPR__V302*/ meltfptr[301];
            /*_.RESEXP__V303*/
            meltfptr[302] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_MAKE_SEXPR*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.HOOK_MELT_MAKE_LOCATION__V44*/ meltfptr[41]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:696:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L15*/
            meltfnum[14] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:696:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L15*/ meltfnum[14]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:696:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 696;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro resexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESEXP__V303*/ meltfptr[302];
                            /*_.MELT_DEBUG_FUN__V305*/
                            meltfptr[304] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V304*/
                        meltfptr[303] = /*_.MELT_DEBUG_FUN__V305*/ meltfptr[304];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:696:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L16*/
                        meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V305*/
                        meltfptr[304] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V304*/ meltfptr[303] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:696:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L15*/
            meltfnum[14] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V304*/
            meltfptr[303] = 0 ;
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
        MELT_LOCATION("warmelt-base.melt:697:/ quasiblock");



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
            /*_.MEXP__V307*/
            meltfptr[303] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.RESEXP__V303*/ meltfptr[302]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-base.melt:699:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L17*/
            meltfnum[15] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-base.melt:699:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L17*/ meltfnum[15]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L18*/ meltfnum[14] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-base.melt:699:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[14];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-base.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 699;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "diagnostic_args_macro final mexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXP__V307*/ meltfptr[303];
                            /*_.MELT_DEBUG_FUN__V309*/
                            meltfptr[308] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V308*/
                        meltfptr[307] = /*_.MELT_DEBUG_FUN__V309*/ meltfptr[308];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:699:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L18*/
                        meltfnum[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V309*/
                        meltfptr[308] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V308*/ meltfptr[307] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:699:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L17*/
            meltfnum[15] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V308*/
            meltfptr[307] = 0 ;
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
        MELT_LOCATION("warmelt-base.melt:700:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.MEXP__V307*/ meltfptr[303]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.MEXP__V307*/ meltfptr[303])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.MEXP__V307*/ meltfptr[303]), (1), (/*_.SLOC__V12*/ meltfptr[5]), "LOCA_LOCATION");
                    ;
                    /*^touch*/
                    meltgc_touch(/*_.MEXP__V307*/ meltfptr[303]);
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.MEXP__V307*/ meltfptr[303], "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:701:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MEXP__V307*/ meltfptr[303];;

        {
            MELT_LOCATION("warmelt-base.melt:701:/ locexp");
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
        /*_.LET___V306*/
        meltfptr[304] = /*_.RETURN___V310*/ meltfptr[308];;

        MELT_LOCATION("warmelt-base.melt:697:/ clear");
        /*clear*/ /*_.MEXP__V307*/
        meltfptr[303] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V310*/
        meltfptr[308] = 0 ;
        /*_.LET___V43*/
        meltfptr[38] = /*_.LET___V306*/ meltfptr[304];;

        MELT_LOCATION("warmelt-base.melt:595:/ clear");
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V44*/
        meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V45*/
        meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V46*/
        meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V47*/
        meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V48*/
        meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V49*/
        meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V50*/
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
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V55*/
        meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V56*/
        meltfptr[55] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V57*/
        meltfptr[56] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V58*/
        meltfptr[57] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V59*/
        meltfptr[58] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V60*/
        meltfptr[59] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V61*/
        meltfptr[60] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V62*/
        meltfptr[61] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V63*/
        meltfptr[62] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V64*/
        meltfptr[63] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V65*/
        meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V66*/
        meltfptr[65] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V67*/
        meltfptr[66] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V68*/
        meltfptr[67] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V69*/
        meltfptr[68] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V70*/
        meltfptr[69] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V71*/
        meltfptr[70] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V72*/
        meltfptr[71] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V73*/
        meltfptr[72] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V74*/
        meltfptr[73] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V75*/
        meltfptr[74] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V76*/
        meltfptr[75] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V77*/
        meltfptr[76] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V78*/
        meltfptr[77] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V79*/
        meltfptr[78] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V80*/
        meltfptr[79] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V81*/
        meltfptr[80] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V82*/
        meltfptr[81] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V83*/
        meltfptr[82] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V84*/
        meltfptr[83] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V85*/
        meltfptr[84] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V86*/
        meltfptr[85] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V87*/
        meltfptr[86] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V88*/
        meltfptr[87] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V89*/
        meltfptr[88] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V90*/
        meltfptr[89] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V91*/
        meltfptr[90] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V92*/
        meltfptr[91] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V93*/
        meltfptr[92] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V94*/
        meltfptr[93] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V95*/
        meltfptr[94] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V96*/
        meltfptr[95] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V97*/
        meltfptr[96] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V98*/
        meltfptr[97] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V99*/
        meltfptr[98] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V100*/
        meltfptr[99] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V101*/
        meltfptr[100] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V102*/
        meltfptr[101] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V103*/
        meltfptr[102] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V104*/
        meltfptr[103] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V105*/
        meltfptr[104] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V106*/
        meltfptr[105] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V107*/
        meltfptr[106] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V108*/
        meltfptr[107] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V109*/
        meltfptr[108] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V110*/
        meltfptr[109] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V111*/
        meltfptr[110] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V112*/
        meltfptr[111] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V113*/
        meltfptr[112] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V114*/
        meltfptr[113] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V115*/
        meltfptr[114] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V116*/
        meltfptr[115] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V117*/
        meltfptr[116] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V118*/
        meltfptr[117] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V119*/
        meltfptr[118] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V120*/
        meltfptr[119] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V121*/
        meltfptr[120] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V122*/
        meltfptr[121] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V123*/
        meltfptr[122] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V124*/
        meltfptr[123] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V125*/
        meltfptr[124] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V126*/
        meltfptr[125] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V127*/
        meltfptr[126] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V128*/
        meltfptr[127] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V129*/
        meltfptr[128] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V130*/
        meltfptr[129] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V131*/
        meltfptr[130] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V132*/
        meltfptr[131] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V133*/
        meltfptr[132] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V134*/
        meltfptr[133] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V135*/
        meltfptr[134] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V136*/
        meltfptr[135] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V137*/
        meltfptr[136] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V138*/
        meltfptr[137] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V139*/
        meltfptr[138] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V140*/
        meltfptr[139] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V141*/
        meltfptr[140] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V142*/
        meltfptr[141] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V143*/
        meltfptr[142] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V144*/
        meltfptr[143] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V145*/
        meltfptr[144] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V146*/
        meltfptr[145] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V147*/
        meltfptr[146] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V148*/
        meltfptr[147] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V149*/
        meltfptr[148] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V150*/
        meltfptr[149] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V151*/
        meltfptr[150] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V152*/
        meltfptr[151] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V153*/
        meltfptr[152] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V154*/
        meltfptr[153] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V155*/
        meltfptr[154] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V156*/
        meltfptr[155] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V157*/
        meltfptr[156] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V158*/
        meltfptr[157] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V159*/
        meltfptr[158] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V160*/
        meltfptr[159] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V161*/
        meltfptr[160] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V162*/
        meltfptr[161] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V163*/
        meltfptr[162] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V164*/
        meltfptr[163] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V165*/
        meltfptr[164] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V166*/
        meltfptr[165] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V167*/
        meltfptr[166] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V168*/
        meltfptr[167] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V169*/
        meltfptr[168] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V170*/
        meltfptr[169] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V171*/
        meltfptr[170] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V172*/
        meltfptr[171] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V173*/
        meltfptr[172] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V174*/
        meltfptr[173] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V175*/
        meltfptr[174] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V176*/
        meltfptr[175] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V177*/
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
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V181*/
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
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V199*/
        meltfptr[198] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V200*/
        meltfptr[199] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V201*/
        meltfptr[200] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V202*/
        meltfptr[201] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V203*/
        meltfptr[202] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V204*/
        meltfptr[203] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V205*/
        meltfptr[204] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V206*/
        meltfptr[205] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V207*/
        meltfptr[206] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V208*/
        meltfptr[207] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V209*/
        meltfptr[208] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V210*/
        meltfptr[209] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V211*/
        meltfptr[210] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V212*/
        meltfptr[211] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V213*/
        meltfptr[212] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V214*/
        meltfptr[213] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V215*/
        meltfptr[214] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V216*/
        meltfptr[215] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V217*/
        meltfptr[216] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V218*/
        meltfptr[217] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V219*/
        meltfptr[218] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V220*/
        meltfptr[219] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V221*/
        meltfptr[220] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V222*/
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
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V226*/
        meltfptr[225] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V227*/
        meltfptr[226] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V228*/
        meltfptr[227] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V229*/
        meltfptr[228] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V230*/
        meltfptr[229] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V231*/
        meltfptr[230] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V232*/
        meltfptr[231] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V233*/
        meltfptr[232] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V234*/
        meltfptr[233] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V235*/
        meltfptr[234] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V236*/
        meltfptr[235] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V237*/
        meltfptr[236] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V238*/
        meltfptr[237] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V239*/
        meltfptr[238] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V240*/
        meltfptr[239] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V241*/
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
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V246*/
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
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V250*/
        meltfptr[249] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V251*/
        meltfptr[250] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V252*/
        meltfptr[251] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V253*/
        meltfptr[252] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V254*/
        meltfptr[253] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V255*/
        meltfptr[254] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V256*/
        meltfptr[255] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V257*/
        meltfptr[256] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V258*/
        meltfptr[257] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V259*/
        meltfptr[258] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V260*/
        meltfptr[259] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V261*/
        meltfptr[260] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V262*/
        meltfptr[261] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V263*/
        meltfptr[262] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V264*/
        meltfptr[263] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V265*/
        meltfptr[264] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V266*/
        meltfptr[265] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V267*/
        meltfptr[266] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V268*/
        meltfptr[267] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V269*/
        meltfptr[268] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V270*/
        meltfptr[269] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V271*/
        meltfptr[270] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V272*/
        meltfptr[271] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V273*/
        meltfptr[272] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V274*/
        meltfptr[273] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V275*/
        meltfptr[274] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V276*/
        meltfptr[275] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V277*/
        meltfptr[276] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V278*/
        meltfptr[277] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V279*/
        meltfptr[278] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V280*/
        meltfptr[279] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V281*/
        meltfptr[280] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V282*/
        meltfptr[281] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V283*/
        meltfptr[282] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V284*/
        meltfptr[283] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V285*/
        meltfptr[284] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V286*/
        meltfptr[285] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V287*/
        meltfptr[286] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V288*/
        meltfptr[287] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V289*/
        meltfptr[288] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V290*/
        meltfptr[289] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V291*/
        meltfptr[290] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V292*/
        meltfptr[291] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V293*/
        meltfptr[292] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V294*/
        meltfptr[293] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V295*/
        meltfptr[294] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V296*/
        meltfptr[295] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V297*/
        meltfptr[296] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOK_MELT_MAKE_LOCATION__V298*/
        meltfptr[297] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V299*/
        meltfptr[298] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V300*/
        meltfptr[299] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V301*/
        meltfptr[300] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MELT_MAKE_SEXPR__V302*/
        meltfptr[301] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESEXP__V303*/
        meltfptr[302] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V306*/
        meltfptr[304] = 0 ;
        /*_.LET___V34*/
        meltfptr[21] = /*_.LET___V43*/ meltfptr[38];;

        MELT_LOCATION("warmelt-base.melt:584:/ clear");
        /*clear*/ /*_.MACROVARNBARG__V35*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROVARVECT__V36*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L13*/
        meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V37*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L14*/
        meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V40*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V43*/
        meltfptr[38] = 0 ;
        /*_.LET___V19*/
        meltfptr[17] = /*_.LET___V34*/ meltfptr[21];;

        MELT_LOCATION("warmelt-base.melt:565:/ clear");
        /*clear*/ /*_.TCONT__V20*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#TCONTLEN__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#exeqI__L10*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V27*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V34*/
        meltfptr[21] = 0 ;
        /*_.LET___V11*/
        meltfptr[9] = /*_.LET___V19*/ meltfptr[17];;

        MELT_LOCATION("warmelt-base.melt:559:/ clear");
        /*clear*/ /*_.SLOC__V12*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXCONT__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:555:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V11*/ meltfptr[9];;

        {
            MELT_LOCATION("warmelt-base.melt:555:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V11*/
        meltfptr[9] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("DIAGNOSTIC_ARGS_MACRO", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_13_WARMELTmiBASE_DIAGNOSTIC_ARGS_MACRO*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1718 */

    /** start of frame for meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA// fromline 1534
        : public Melt_CallFrameWithValues<19>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA() //the constructor fromline 1608
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA)) {};
        MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA


    /** end of frame for meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1663**/

    /* end of frame for routine meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1722 */

    /* classy proc frame meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA */
    MeltFrame_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1233:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1237:/ cond");
        /*cond*/
        if (/*_#eqeq__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:1238:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*!konst_0*/ meltfrout->tabval[0]);;
                    MELT_LOCATION("warmelt-base.melt:1238:/ putxtraresult");
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

                    MELT_LOCATION("warmelt-base.melt:1237:/ clear");
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
                    MELT_LOCATION("warmelt-base.melt:1239:/ cond");
                    /*cond*/
                    if (/*_#IS_NOT_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-base.melt:1240:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*!konst_2*/ meltfrout->tabval[2]);;
                                MELT_LOCATION("warmelt-base.melt:1240:/ putxtraresult");
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

                                MELT_LOCATION("warmelt-base.melt:1239:/ clear");
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
                                MELT_LOCATION("warmelt-base.melt:1241:/ cond");
                                /*cond*/
                                if (/*_#IS_NOT_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1242:/ quasiblock");


                                            /*_._RETVAL___V1*/
                                            meltfptr[0] = (/*!konst_3*/ meltfrout->tabval[3]);;
                                            MELT_LOCATION("warmelt-base.melt:1242:/ putxtraresult");
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

                                            MELT_LOCATION("warmelt-base.melt:1241:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:1244:/ quasiblock");


                                            /*^getslot*/
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.N1__V2*/ meltfptr[1]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.SN1__V11*/
                                                meltfptr[10] = slot;
                                            };
                                            ;
                                            MELT_LOCATION("warmelt-base.melt:1245:/ getslot");
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
                                            MELT_LOCATION("warmelt-base.melt:1247:/ cond");
                                            /*cond*/
                                            if (/*_#STRINGlt__L4*/ meltfnum[3]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1248:/ quasiblock");


                                                        /*_._RETVAL___V1*/
                                                        meltfptr[0] = (/*!konst_4*/ meltfrout->tabval[4]);;
                                                        MELT_LOCATION("warmelt-base.melt:1248:/ putxtraresult");
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

                                                        MELT_LOCATION("warmelt-base.melt:1247:/ clear");
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
                                                        MELT_LOCATION("warmelt-base.melt:1249:/ cond");
                                                        /*cond*/
                                                        if (/*_#STRINGgt__L5*/ meltfnum[4]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-base.melt:1250:/ quasiblock");


                                                                    /*_._RETVAL___V1*/
                                                                    meltfptr[0] = (/*!konst_5*/ meltfrout->tabval[5]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1250:/ putxtraresult");
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

                                                                    MELT_LOCATION("warmelt-base.melt:1249:/ clear");
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
                                                                    MELT_LOCATION("warmelt-base.melt:1252:/ quasiblock");


                                                                    /*_._RETVAL___V1*/
                                                                    meltfptr[0] = (/*!konst_6*/ meltfrout->tabval[6]);;
                                                                    MELT_LOCATION("warmelt-base.melt:1252:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/
                                                                    goto meltlabend_rout ;
                                                                    MELT_LOCATION("warmelt-base.melt:1251:/ quasiblock");


                                                                    /*_.PROGN___V18*/
                                                                    meltfptr[17] = /*_.RETURN___V17*/ meltfptr[15];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V15*/
                                                                    meltfptr[13] = /*_.PROGN___V18*/ meltfptr[17];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1249:/ clear");
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

                                                        MELT_LOCATION("warmelt-base.melt:1247:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:1244:/ clear");
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
                                            MELT_LOCATION("warmelt-base.melt:1243:/ quasiblock");


                                            /*_.PROGN___V19*/
                                            meltfptr[15] = /*_.LET___V10*/ meltfptr[8];;
                                            /*^compute*/
                                            /*_._IFELSE___V8*/
                                            meltfptr[6] = /*_.PROGN___V19*/ meltfptr[15];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:1241:/ clear");
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

                                MELT_LOCATION("warmelt-base.melt:1239:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:1237:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V6*/
                    meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1233:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:1233:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_14_WARMELTmiBASE_COMPARE_NAMED_ALPHA*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                  const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_ADD2OUT_ix = 0, variad_ADD2OUT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_ADD2OUT_len)
#define melt_variadic_index variad_ADD2OUT_ix

    long current_blocklevel_signals_meltrout_15_WARMELTmiBASE_ADD2OUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_15_WARMELTmiBASE_ADD2OUT fromline 1718 */

    /** start of frame for meltrout_15_WARMELTmiBASE_ADD2OUT of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT// fromline 1534
        : public Melt_CallFrameWithValues<32>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT), clos) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT() //the constructor fromline 1608
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT)) {};
        MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT


    /** end of frame for meltrout_15_WARMELTmiBASE_ADD2OUT fromline 1663**/

    /* end of frame for routine meltrout_15_WARMELTmiBASE_ADD2OUT fromline 1722 */

    /* classy proc frame meltrout_15_WARMELTmiBASE_ADD2OUT */
    MeltFrame_meltrout_15_WARMELTmiBASE_ADD2OUT
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_15_WARMELTmiBASE_ADD2OUT fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1548:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1552:/ cond");
        /*cond*/
        if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1553:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1553:/ locexp");
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

                    MELT_LOCATION("warmelt-base.melt:1552:/ clear");
                    /*clear*/ /*_.RETURN___V4*/
                    meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1554:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_1:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1556:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_ADD2OUT_ix == variad_ADD2OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_ADD2OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1558:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V6*/
                            meltfptr[5] =  /*reallynil*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:1556:/ cond.else");

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
                                        MELT_LOCATION("warmelt-base.melt:1560:/ cond");
                                        /*cond*/
                                        if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1561:/ cond");
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
                                                                MELT_LOCATION("warmelt-base.melt:1563:/ apply");
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

                                                                MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                            MELT_LOCATION("warmelt-base.melt:1565:/ apply");
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

                                                                            MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                                        MELT_LOCATION("warmelt-base.melt:1567:/ apply");
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

                                                                                        MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                                                    MELT_LOCATION("warmelt-base.melt:1569:/ apply");
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

                                                                                                    MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                                                                MELT_LOCATION("warmelt-base.melt:1571:/ apply");
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

                                                                                                                MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1573:/ apply");
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

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1575:/ apply");
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

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1577:/ apply");
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

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1579:/ quasiblock");


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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1582:/ cond");
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

                                                                                                                                                            /*_.NAMED_NAME__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1581:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V20*/ meltfptr[19])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1583:/ cppif.then");
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
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1583:/ cond.else");

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
                                                                                                                                                                        argtab[2].meltbp_long = 1583;
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V19*/ meltfptr[8];
                                                                                                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V23*/
                                                                                                                                                                        meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*_._IFELSE___V22*/
                                                                                                                                                                    meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1583:/ clear");
                                                                                                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V23*/
                                                                                                                                                                    meltfptr[22] = 0 ;
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*_.IFCPP___V21*/
                                                                                                                                                        meltfptr[20] = /*_._IFELSE___V22*/ meltfptr[21];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1583:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V22*/
                                                                                                                                                        meltfptr[21] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V21*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1584:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/
                                                                                                                                                    meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1584:/ locexp");
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
                                                                                                                                                    meltfptr[9] = /*_.RETURN___V24*/ meltfptr[22];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1579:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V19*/
                                                                                                                                                    meltfptr[8] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V20*/
                                                                                                                                                    meltfptr[19] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V21*/
                                                                                                                                                    meltfptr[20] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V24*/
                                                                                                                                                    meltfptr[22] = 0 ;
                                                                                                                                                    /*_._IFELSE___V8*/
                                                                                                                                                    meltfptr[7] = /*_.LET___V18*/ meltfptr[9];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1561:/ clear");
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
                                                MELT_LOCATION("warmelt-base.melt:1560:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1586:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V2*/ meltfptr[1];
                                                        /*_.ADD_TO_OUT__V25*/
                                                        meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.V__V7*/ meltfptr[6]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V8*/
                                                    meltfptr[7] = /*_.ADD_TO_OUT__V25*/ meltfptr[21];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1560:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V25*/
                                                    meltfptr[21] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1556:/ clear");
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
                                                        MELT_LOCATION("warmelt-base.melt:1588:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1556:/ clear");
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
                                                                    MELT_LOCATION("warmelt-base.melt:1590:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V2*/ meltfptr[1]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1556:/ clear");
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

                                                                MELT_LOCATION("warmelt-base.melt:1592:/ quasiblock");


                                                                /*_.VCTY__V27*/
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
                                                                MELT_LOCATION("warmelt-base.melt:1595:/ cond");
                                                                /*cond*/
                                                                if (
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
                                                                            /*_.NAMED_NAME__V28*/
                                                                            meltfptr[20] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V28*/ meltfptr[20] =  /*reallynil*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1594:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "ADD2OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V28*/ meltfptr[20])));
                                                                }
                                                                ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1596:/ cppif.then");
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
                                                                            /*_._IFELSE___V30*/ meltfptr[9] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1596:/ cond.else");

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
                                                                                    argtab[2].meltbp_long = 1596;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V27*/ meltfptr[19];
                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V31*/
                                                                                    meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IFELSE___V30*/
                                                                                meltfptr[9] = /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1596:/ clear");
                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V31*/
                                                                                meltfptr[21] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_.IFCPP___V29*/
                                                                    meltfptr[22] = /*_._IFELSE___V30*/ meltfptr[9];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1596:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V30*/
                                                                    meltfptr[9] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V29*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;
                                                                /*^compute*/
                                                                /*_.LET___V26*/
                                                                meltfptr[8] = /*_.IFCPP___V29*/ meltfptr[22];;

                                                                MELT_LOCATION("warmelt-base.melt:1592:/ clear");
                                                                /*clear*/ /*_.VCTY__V27*/
                                                                meltfptr[19] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V28*/
                                                                meltfptr[20] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.IFCPP___V29*/
                                                                meltfptr[22] = 0 ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1556:/ clear");
                                                                /*clear*/ /*_.LET___V26*/
                                                                meltfptr[8] = 0 ;
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
            MELT_LOCATION("warmelt-base.melt:1554:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V5*/
            meltfptr[3] = /*_.ARGLOOP__V6*/ meltfptr[5];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1599:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.OUT__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:1599:/ locexp");
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
        MELT_LOCATION("warmelt-base.melt:1548:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RETURN___V32*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-base.melt:1548:/ locexp");
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
        /*clear*/ /*_.RETURN___V32*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_15_WARMELTmiBASE_ADD2OUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_15_WARMELTmiBASE_ADD2OUT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                     const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_STRING4OUT_ix = 0, variad_STRING4OUT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_STRING4OUT_len)
#define melt_variadic_index variad_STRING4OUT_ix

    long current_blocklevel_signals_meltrout_16_WARMELTmiBASE_STRING4OUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_16_WARMELTmiBASE_STRING4OUT fromline 1718 */

    /** start of frame for meltrout_16_WARMELTmiBASE_STRING4OUT of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT// fromline 1534
        : public Melt_CallFrameWithValues<44>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<44> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT), clos) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT() //the constructor fromline 1608
            : Melt_CallFrameWithValues<44> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<44> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT)) {};
        MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<44> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT


    /** end of frame for meltrout_16_WARMELTmiBASE_STRING4OUT fromline 1663**/

    /* end of frame for routine meltrout_16_WARMELTmiBASE_STRING4OUT fromline 1722 */

    /* classy proc frame meltrout_16_WARMELTmiBASE_STRING4OUT */
    MeltFrame_meltrout_16_WARMELTmiBASE_STRING4OUT
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_16_WARMELTmiBASE_STRING4OUT fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1603:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1606:/ cond");
        /*cond*/
        if (/*_.DIS__V2*/ meltfptr[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V3*/ meltfptr[2] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1606:/ cond.else");

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

                    MELT_LOCATION("warmelt-base.melt:1606:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:1607:/ cond");
        /*cond*/
        if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V6*/ meltfptr[3] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1607:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:1608:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1608:/ locexp");
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
                    MELT_LOCATION("warmelt-base.melt:1607:/ quasiblock");


                    /*_.PROGN___V8*/
                    meltfptr[7] = /*_.RETURN___V7*/ meltfptr[4];;
                    /*^compute*/
                    /*_._IFELSE___V6*/
                    meltfptr[3] = /*_.PROGN___V8*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1607:/ clear");
                    /*clear*/ /*_.RETURN___V7*/
                    meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V8*/
                    meltfptr[7] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1609:/ quasiblock");


        /*_#GOODMAG__L2*/
        meltfnum[1] = 0;;

        {
            MELT_LOCATION("warmelt-base.melt:1611:/ locexp");
            /* string4out GETMAGIDISCHK__1 */ /*_#GOODMAG__L2*/
            meltfnum[1] =
                ((meltobject_ptr_t)/*_.DIS__V2*/ meltfptr[1])->meltobj_magic == MELTOBMAG_STRING ;;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1614:/ cond");
        /*cond*/
        if (/*_#GOODMAG__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V10*/ meltfptr[7] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1614:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-base.melt:1614:/ locexp");
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

                    MELT_LOCATION("warmelt-base.melt:1614:/ clear");
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

        MELT_LOCATION("warmelt-base.melt:1609:/ clear");
        /*clear*/ /*_#GOODMAG__L2*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V10*/
        meltfptr[7] = 0 ;
        MELT_LOCATION("warmelt-base.melt:1615:/ quasiblock");


        /*_.OUT__V14*/
        meltfptr[11] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[2])), (const char*)0);;
        MELT_LOCATION("warmelt-base.melt:1617:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_2:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:1619:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_STRING4OUT_ix == variad_STRING4OUT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_STRING4OUT_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:1621:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V16*/
                            meltfptr[15] =  /*reallynil*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:1619:/ cond.else");

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
                                        MELT_LOCATION("warmelt-base.melt:1623:/ cond");
                                        /*cond*/
                                        if (/*_#IS_CLOSURE__L3*/ meltfnum[1]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {

                                                    MELT_LOCATION("warmelt-base.melt:1624:/ cond");
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
                                                                MELT_LOCATION("warmelt-base.melt:1626:/ apply");
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

                                                                MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                            MELT_LOCATION("warmelt-base.melt:1628:/ apply");
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

                                                                            MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                                        MELT_LOCATION("warmelt-base.melt:1630:/ apply");
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

                                                                                        MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                                                    MELT_LOCATION("warmelt-base.melt:1632:/ apply");
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

                                                                                                    MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                                                                MELT_LOCATION("warmelt-base.melt:1634:/ apply");
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

                                                                                                                MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                                                                            MELT_LOCATION("warmelt-base.melt:1636:/ apply");
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

                                                                                                                            MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1638:/ apply");
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

                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1640:/ apply");
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

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1642:/ quasiblock");


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
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1645:/ cond");
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

                                                                                                                                                            /*_.NAMED_NAME__V30*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                                                                                                                                                        }
                                                                                                                                                    ;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1644:/ locexp");
                                                                                                                                                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT with manipulator for unsupported ctype"),
                                                                                                                                                               melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V30*/ meltfptr[29])));
                                                                                                                                                    }
                                                                                                                                                    ;

#if MELT_HAVE_DEBUG
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1646:/ cppif.then");
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
                                                                                                                                                                MELT_LOCATION("warmelt-base.melt:1646:/ cond.else");

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
                                                                                                                                                                        argtab[2].meltbp_long = 1646;
                                                                                                                                                                        /*^apply.arg*/
                                                                                                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V29*/ meltfptr[18];
                                                                                                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V33*/
                                                                                                                                                                        meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                                                                                    }
                                                                                                                                                                    ;
                                                                                                                                                                    /*_._IFELSE___V32*/
                                                                                                                                                                    meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                                                                                                                                                    /*epilog*/

                                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1646:/ clear");
                                                                                                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/
                                                                                                                                                                    meltfptr[32] = 0 ;
                                                                                                                                                                }
                                                                                                                                                                ;
                                                                                                                                                            }
                                                                                                                                                        ;
                                                                                                                                                        /*_.IFCPP___V31*/
                                                                                                                                                        meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                                                                                                                                                        /*epilog*/

                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1646:/ clear");
                                                                                                                                                        /*clear*/ /*_._IFELSE___V32*/
                                                                                                                                                        meltfptr[31] = 0 ;
                                                                                                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                                                                                                    /*^cppif.else*/
                                                                                                                                                    /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                                                                                    ;
                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1647:/ quasiblock");


                                                                                                                                                    /*_._RETVAL___V1*/
                                                                                                                                                    meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                                                                                    {
                                                                                                                                                        MELT_LOCATION("warmelt-base.melt:1647:/ locexp");
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
                                                                                                                                                    meltfptr[19] = /*_.RETURN___V34*/ meltfptr[32];;

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1642:/ clear");
                                                                                                                                                    /*clear*/ /*_.VCTY__V29*/
                                                                                                                                                    meltfptr[18] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.NAMED_NAME__V30*/
                                                                                                                                                    meltfptr[29] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.IFCPP___V31*/
                                                                                                                                                    meltfptr[30] = 0 ;
                                                                                                                                                    /*^clear*/
                                                                                                                                                    /*clear*/ /*_.RETURN___V34*/
                                                                                                                                                    meltfptr[32] = 0 ;
                                                                                                                                                    /*_._IFELSE___V18*/
                                                                                                                                                    meltfptr[17] = /*_.LET___V28*/ meltfptr[19];;
                                                                                                                                                    /*epilog*/

                                                                                                                                                    MELT_LOCATION("warmelt-base.melt:1624:/ clear");
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
                                                MELT_LOCATION("warmelt-base.melt:1623:/ cond.else");

                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1649:/ msend");
                                                    /*msend*/
                                                    {
                                                        union meltparam_un argtab[1];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^ojbmsend.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OUT__V14*/ meltfptr[11];
                                                        /*_.ADD_TO_OUT__V35*/
                                                        meltfptr[31] = meltgc_send((melt_ptr_t)(/*_.V__V17*/ meltfptr[16]), (melt_ptr_t)((/*!ADD_TO_OUT*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    /*_._IFELSE___V18*/
                                                    meltfptr[17] = /*_.ADD_TO_OUT__V35*/ meltfptr[31];;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1623:/ clear");
                                                    /*clear*/ /*_.ADD_TO_OUT__V35*/
                                                    meltfptr[31] = 0 ;
                                                }
                                                ;
                                            }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:1619:/ clear");
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
                                                        MELT_LOCATION("warmelt-base.melt:1651:/ locexp");
                                                        meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_#N__L5*/ meltfnum[3]));
                                                    }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1619:/ clear");
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
                                                                    MELT_LOCATION("warmelt-base.melt:1653:/ locexp");
                                                                    meltgc_add_out((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]), (/*_?*/ meltfram__.loc_CSTRING__o6));
                                                                }
                                                                ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1619:/ clear");
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

                                                                MELT_LOCATION("warmelt-base.melt:1655:/ quasiblock");


                                                                /*_.VCTY__V37*/
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
                                                                MELT_LOCATION("warmelt-base.melt:1658:/ cond");
                                                                /*cond*/
                                                                if (
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
                                                                            /*_.NAMED_NAME__V38*/
                                                                            meltfptr[30] = slot;
                                                                        };
                                                                        ;
                                                                    }
                                                                else    /*^cond.else*/
                                                                    {

                                                                        /*_.NAMED_NAME__V38*/ meltfptr[30] =  /*reallynil*/ NULL ;;
                                                                    }
                                                                ;

                                                                {
                                                                    MELT_LOCATION("warmelt-base.melt:1657:/ locexp");
                                                                    error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "STRING4OUT for unsupported ctype; use a manipulator like OUTPUT_TREE "),
                                                                           melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V38*/ meltfptr[30])));
                                                                }
                                                                ;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-base.melt:1659:/ cppif.then");
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
                                                                            /*_._IFELSE___V40*/ meltfptr[19] = (/*nil*/NULL);;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-base.melt:1659:/ cond.else");

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
                                                                                    argtab[2].meltbp_long = 1659;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V37*/ meltfptr[29];
                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V41*/
                                                                                    meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IFELSE___V40*/
                                                                                meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-base.melt:1659:/ clear");
                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V41*/
                                                                                meltfptr[31] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_.IFCPP___V39*/
                                                                    meltfptr[32] = /*_._IFELSE___V40*/ meltfptr[19];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-base.melt:1659:/ clear");
                                                                    /*clear*/ /*_._IFELSE___V40*/
                                                                    meltfptr[19] = 0 ;
                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                /*^cppif.else*/
                                                                /*_.IFCPP___V39*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                ;
                                                                /*^compute*/
                                                                /*_.LET___V36*/
                                                                meltfptr[18] = /*_.IFCPP___V39*/ meltfptr[32];;

                                                                MELT_LOCATION("warmelt-base.melt:1655:/ clear");
                                                                /*clear*/ /*_.VCTY__V37*/
                                                                meltfptr[29] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.NAMED_NAME__V38*/
                                                                meltfptr[30] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.IFCPP___V39*/
                                                                meltfptr[32] = 0 ;
                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-base.melt:1619:/ clear");
                                                                /*clear*/ /*_.LET___V36*/
                                                                meltfptr[18] = 0 ;
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
            MELT_LOCATION("warmelt-base.melt:1617:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V15*/
            meltfptr[7] = /*_.ARGLOOP__V16*/ meltfptr[15];;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:1662:/ quasiblock");


        /*_.STR__V43*/
        meltfptr[17] =
            (meltgc_new_stringdup((meltobject_ptr_t)(/*_.DIS__V2*/ meltfptr[1]), melt_strbuf_str((melt_ptr_t)(/*_.OUT__V14*/ meltfptr[11]))));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1664:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.STR__V43*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-base.melt:1664:/ locexp");
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
        /*_.LET___V42*/
        meltfptr[16] = /*_.RETURN___V44*/ meltfptr[31];;

        MELT_LOCATION("warmelt-base.melt:1662:/ clear");
        /*clear*/ /*_.STR__V43*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V44*/
        meltfptr[31] = 0 ;
        /*_.LET___V13*/
        meltfptr[10] = /*_.LET___V42*/ meltfptr[16];;

        MELT_LOCATION("warmelt-base.melt:1615:/ clear");
        /*clear*/ /*_.OUT__V14*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V15*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V42*/
        meltfptr[16] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1603:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[10];;

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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_16_WARMELTmiBASE_STRING4OUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_16_WARMELTmiBASE_STRING4OUT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4NULL fromline 1718 */

    /** start of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4NULL of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL// fromline 1534
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL() //the constructor fromline 1608
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL)) {};
        MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL


    /** end of frame for meltrout_17_WARMELTmiBASE_ADD2OUT4NULL fromline 1663**/

    /* end of frame for routine meltrout_17_WARMELTmiBASE_ADD2OUT4NULL fromline 1722 */

    /* classy proc frame meltrout_17_WARMELTmiBASE_ADD2OUT4NULL */
    MeltFrame_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_17_WARMELTmiBASE_ADD2OUT4NULL fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1670:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1671:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#NULL__L1*/
            meltfnum[0] =
                ((/*_.V__V2*/ meltfptr[1]) == NULL);;
            MELT_LOCATION("warmelt-base.melt:1671:/ cond");
            /*cond*/
            if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1671:/ cond.else");

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
                            argtab[2].meltbp_long = 1671;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1671:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1671:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:1672:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*nil*"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1670:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4NULL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_17_WARMELTmiBASE_ADD2OUT4NULL_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_17_WARMELTmiBASE_ADD2OUT4NULL*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1718 */

    /** start of frame for meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER// fromline 1534
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER() //the constructor fromline 1608
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER)) {};
        MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER


    /** end of frame for meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1663**/

    /* end of frame for routine meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1722 */

    /* classy proc frame meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER */
    MeltFrame_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1675:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1676:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_INTEGERBOX__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VN__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
            MELT_LOCATION("warmelt-base.melt:1676:/ cond");
            /*cond*/
            if (/*_#IS_INTEGERBOX__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1676:/ cond.else");

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
                            argtab[2].meltbp_long = 1676;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VN__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1676:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1676:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:1677:/ locexp");
            meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#GET_INT__L2*/ meltfnum[0]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1675:/ clear");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_18_WARMELTmiBASE_ADD2OUT4INTEGER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4STRING fromline 1718 */

    /** start of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4STRING of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING// fromline 1534
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING() //the constructor fromline 1608
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING)) {};
        MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING


    /** end of frame for meltrout_19_WARMELTmiBASE_ADD2OUT4STRING fromline 1663**/

    /* end of frame for routine meltrout_19_WARMELTmiBASE_ADD2OUT4STRING fromline 1722 */

    /* classy proc frame meltrout_19_WARMELTmiBASE_ADD2OUT4STRING */
    MeltFrame_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_19_WARMELTmiBASE_ADD2OUT4STRING fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1680:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1681:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-base.melt:1681:/ cond");
            /*cond*/
            if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1681:/ cond.else");

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
                            argtab[2].meltbp_long = 1681;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VS__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1681:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1681:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:1682:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.VS__V2*/ meltfptr[1])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1680:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4STRING", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_19_WARMELTmiBASE_ADD2OUT4STRING_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_19_WARMELTmiBASE_ADD2OUT4STRING*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1718 */

    /** start of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF// fromline 1534
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF() //the constructor fromline 1608
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF)) {};
        MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF


    /** end of frame for meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1663**/

    /* end of frame for routine meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1722 */

    /* classy proc frame meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF */
    MeltFrame_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1685:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1686:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VSBUF__V2*/ meltfptr[1])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-base.melt:1686:/ cond");
            /*cond*/
            if (/*_#IS_STRBUF__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1686:/ cond.else");

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
                            argtab[2].meltbp_long = 1686;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VSBUF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1686:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1686:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:1687:/ locexp");
            meltgc_add_out ((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_strbuf_str ((melt_ptr_t) /*_.VSBUF__V2*/ meltfptr[1]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1685:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4STRBUF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_20_WARMELTmiBASE_ADD2OUT4STRBUF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1718 */

    /** start of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB// fromline 1534
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB() //the constructor fromline 1608
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB)) {};
        MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB


    /** end of frame for meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1663**/

    /* end of frame for routine meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1722 */

    /* classy proc frame meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB */
    MeltFrame_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1690:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1691:/ block");
        /*anyblock*/
        {

            /*^objgoto*/
            /*objgoto*/ goto mtch1_0;
            ;

            /*objlabel*/
mtch1_0:
            ;
            MELT_LOCATION("warmelt-base.melt:1692:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_1;
            meltlab_count_1++;
            debugeprintf("objlabel_1 mtch1_*CLASS_NORMTESTER_INSTANCE/2cf980d4. *CLASS_OBJLABELINSTR/2416d225. %ld", meltlab_count_1);
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
                        MELT_LOCATION("warmelt-base.melt:1694:/ objgoto");
                        /*objgoto*/
                        goto mtch1_1;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1692:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1699:/ objgoto");
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
            MELT_LOCATION("warmelt-base.melt:1694:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_2;
            meltlab_count_2++;
            debugeprintf("objlabel_2 mtch1_*CLASS_NORMTESTER_MATCHER/8f75b15. *CLASS_OBJLABELINSTR/711e895. %ld", meltlab_count_2);
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
                        MELT_LOCATION("warmelt-base.melt:1692:/ objgoto");
                        /*objgoto*/
                        goto mtch1_2;
                        ;
                    }
                    ;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1694:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        MELT_LOCATION("warmelt-base.melt:1699:/ objgoto");
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
            MELT_LOCATION("warmelt-base.melt:1692:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_3;
            meltlab_count_3++;
            debugeprintf("objlabel_3 mtch1_*CLASS_NORMTESTER_SUCCESS/fc5c73c. *CLASS_OBJLABELINSTR/13500397. %ld", meltlab_count_3);
#endif

            ;
            /*^quasiblock*/


            /*_.NVARNAM__V6*/
            meltfptr[4] = /*_.NAMED_NAME__V4*/ meltfptr[3];;
            /*^compute*/
            /*_#NVARURANK__L2*/
            meltfnum[1] = /*_#ICT__L1*/ meltfnum[0];;

#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1695:/ cppif.then");
            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_STRING__L3*/
                meltfnum[2] =
                    (melt_magic_discr((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])) == MELTOBMAG_STRING);;
                MELT_LOCATION("warmelt-base.melt:1695:/ cond");
                /*cond*/
                if (/*_#IS_STRING__L3*/ meltfnum[2]) /*then*/
                    {
                        /*^cond.then*/
                        /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1695:/ cond.else");

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
                                argtab[2].meltbp_long = 1695;
                                /*^apply.arg*/
                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.NVARNAM__V6*/ meltfptr[4];
                                /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                                meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IFELSE___V8*/
                            meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-base.melt:1695:/ clear");
                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                            meltfptr[8] = 0 ;
                        }
                        ;
                    }
                ;
                /*_.IFCPP___V7*/
                meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1695:/ clear");
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
                MELT_LOCATION("warmelt-base.melt:1696:/ locexp");
                meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                      melt_string_str((melt_ptr_t)(/*_.NVARNAM__V6*/ meltfptr[4])));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1697:/ locexp");
                meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "__"));
            }
            ;

            {
                MELT_LOCATION("warmelt-base.melt:1698:/ locexp");
                meltgc_add_out_dec((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#NVARURANK__L2*/ meltfnum[1]));
            }
            ;

            MELT_CHECK_SIGNAL();
            ;

            MELT_LOCATION("warmelt-base.melt:1692:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:1699:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_4;
            meltlab_count_4++;
            debugeprintf("objlabel_4 mtch1_*CLASS_NORMTESTER_SUCCESS/36306c19. *CLASS_OBJLABELINSTR/114ace37. %ld", meltlab_count_4);
#endif

            ;
            /*^quasiblock*/



#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-base.melt:1700:/ cppif.then");
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
                        /*_._IFELSE___V11*/ meltfptr[7] = (/*nil*/NULL);;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-base.melt:1700:/ cond.else");

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
                                argtab[2].meltbp_long = 1700;
                                /*^apply.arg*/
                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VC__V2*/ meltfptr[1];
                                /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                                meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IFELSE___V11*/
                            meltfptr[7] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[4];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-base.melt:1700:/ clear");
                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                            meltfptr[4] = 0 ;
                        }
                        ;
                    }
                ;
                /*_.IFCPP___V10*/
                meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[7];;
                /*epilog*/

                MELT_LOCATION("warmelt-base.melt:1700:/ clear");
                /*clear*/ /*_._IFELSE___V11*/
                meltfptr[7] = 0 ;
            }

#else /*MELT_HAVE_DEBUG*/
            /*^cppif.else*/
            /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
            ;

            MELT_LOCATION("warmelt-base.melt:1699:/ clear");
            /*clear*/ /*_.IFCPP___V10*/
            meltfptr[8] = 0 ;
            /*^objgoto*/
            /*objgoto*/
            goto mtch1__end /*endmatch*/;
            ;

            /*objlabel*/
mtch1__end:
            ;
            MELT_LOCATION("warmelt-base.melt:1691:/ objlabel");
            /* objlabel */
#if MELTDEBUG_MATCHING
            static long meltlab_count_5;
            meltlab_count_5++;
            debugeprintf("objlabel_5 mtch1__end_h334677444 *CLASS_OBJLABELINSTR/13f2c5c4. %ld", meltlab_count_5);
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_21_WARMELTmiBASE_ADD2OUT4CLONEDSYMB*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED fromline 1718 */

    /** start of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED// fromline 1534
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED() //the constructor fromline 1608
            : Melt_CallFrameWithValues<4> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED)) {};
        MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED


    /** end of frame for meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED fromline 1663**/

    /* end of frame for routine meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED fromline 1722 */

    /* classy proc frame meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED */
    MeltFrame_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1703:/ getarg");
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

        MELT_LOCATION("warmelt-base.melt:1704:/ cond");
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

                /*_.NAMED_NAME__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1704:/ locexp");
            meltgc_add_out_cident((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]),
                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V4*/ meltfptr[3])));
        }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1703:/ clear");
        /*clear*/ /*_.NAMED_NAME__V4*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2OUT4NAMED", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_22_WARMELTmiBASE_ADD2OUT4NAMED*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1718 */

    /** start of frame for meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT// fromline 1534
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT() //the constructor fromline 1608
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT)) {};
        MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT


    /** end of frame for meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1663**/

    /* end of frame for routine meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1722 */

    /* classy proc frame meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT */
    MeltFrame_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1707:/ getarg");
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
            MELT_LOCATION("warmelt-base.melt:1708:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/
        meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1709:/ cond");
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

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1709:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1710:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "/"));
        }
        ;
        /*_#OBJ_HASH__L1*/
        meltfnum[0] =
            (melt_obj_hash((melt_ptr_t)(/*_.VO__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-base.melt:1711:/ locexp");
            meltgc_add_out_hex((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), (/*_#OBJ_HASH__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1712:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "."));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1707:/ clear");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_23_WARMELTmiBASE_ADD2OUT4ROOTOBJECT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_24_WARMELTmiBASE_ADD2OUT4ANY fromline 1718 */

    /** start of frame for meltrout_24_WARMELTmiBASE_ADD2OUT4ANY of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY// fromline 1534
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY() //the constructor fromline 1608
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY)) {};
        MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY


    /** end of frame for meltrout_24_WARMELTmiBASE_ADD2OUT4ANY fromline 1663**/

    /* end of frame for routine meltrout_24_WARMELTmiBASE_ADD2OUT4ANY fromline 1722 */

    /* classy proc frame meltrout_24_WARMELTmiBASE_ADD2OUT4ANY */
    MeltFrame_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_24_WARMELTmiBASE_ADD2OUT4ANY fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1715:/ getarg");
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
            MELT_LOCATION("warmelt-base.melt:1716:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), ( "*"));
        }
        ;
        /*_.DISCRIM__V4*/
        meltfptr[3] =
            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.VA__V2*/ meltfptr[1]))));;
        MELT_LOCATION("warmelt-base.melt:1717:/ cond");
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

                /*_.NAMED_NAME__V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:1717:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.OUT__V3*/ meltfptr[2]), melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[4])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1715:/ clear");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_24_WARMELTmiBASE_ADD2OUT4ANY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_24_WARMELTmiBASE_ADD2OUT4ANY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY fromline 1718 */

    /** start of frame for meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY// fromline 1534
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY() //the constructor fromline 1608
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY)) {};
        MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY


    /** end of frame for meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY fromline 1663**/

    /* end of frame for routine meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY fromline 1722 */

    /* classy proc frame meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY */
    MeltFrame_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1776:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1780:/ cond");
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
                    MELT_LOCATION("warmelt-base.melt:1781:/ cond");
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
                                            MELT_LOCATION("warmelt-base.melt:1785:/ apply");
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

                                    MELT_LOCATION("warmelt-base.melt:1782:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:1780:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1776:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MAPSTRING_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_25_WARMELTmiBASE_MAPSTRING_EVERY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1718 */

    /** start of frame for meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST// fromline 1534
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST() //the constructor fromline 1608
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST)) {};
        MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST


    /** end of frame for meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1663**/

    /* end of frame for routine meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1722 */

    /* classy proc frame meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST */
    MeltFrame_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1789:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1794:/ cond");
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
                    MELT_LOCATION("warmelt-base.melt:1795:/ cond");
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
                                            MELT_LOCATION("warmelt-base.melt:1799:/ quasiblock");


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
                                            MELT_LOCATION("warmelt-base.melt:1802:/ cond");
                                            /*cond*/
                                            if (/*_#NULL__L3*/ meltfnum[2]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-base.melt:1803:/ quasiblock");


                                                        /*_._RETVAL___V1*/
                                                        meltfptr[0] = /*_.CURAT__V4*/ meltfptr[3];;
                                                        MELT_LOCATION("warmelt-base.melt:1803:/ putxtraresult");
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

                                                        MELT_LOCATION("warmelt-base.melt:1802:/ clear");
                                                        /*clear*/ /*_.RETURN___V10*/
                                                        meltfptr[9] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                                                }
                                            ;
                                            /*^compute*/
                                            /*_.MULTI___V6*/
                                            meltfptr[5] = /*_._IF___V9*/ meltfptr[8];;

                                            MELT_LOCATION("warmelt-base.melt:1799:/ clear");
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

                                    MELT_LOCATION("warmelt-base.melt:1796:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:1794:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1789:/ clear");
        /*clear*/ /*_#IS_MAPSTRING__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MAPSTRING_ITERATE_TEST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_26_WARMELTmiBASE_MAPSTRING_ITERATE_TEST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY fromline 1718 */

    /** start of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY// fromline 1534
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY() //the constructor fromline 1608
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY)) {};
        MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY


    /** end of frame for meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY fromline 1663**/

    /* end of frame for routine meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY fromline 1722 */

    /* classy proc frame meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY */
    MeltFrame_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1838:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1841:/ cond");
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
                    MELT_LOCATION("warmelt-base.melt:1842:/ cond");
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
                                            MELT_LOCATION("warmelt-base.melt:1846:/ apply");
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

                                    MELT_LOCATION("warmelt-base.melt:1843:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:1841:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1838:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_27_WARMELTmiBASE_MULTIPLE_EVERY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1504

    /* start of frame for routine meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1718 */

    /** start of frame for meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY// fromline 1534
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY() //the constructor fromline 1608
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
        MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY


    /** end of frame for meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1663**/

    /* end of frame for routine meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1722 */

    /* classy proc frame meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY */
    MeltFrame_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1849:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1852:/ cond");
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
                    MELT_LOCATION("warmelt-base.melt:1853:/ cond");
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
                                            MELT_LOCATION("warmelt-base.melt:1857:/ apply");
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

                                    MELT_LOCATION("warmelt-base.melt:1854:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:1852:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1849:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_28_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY*/



/**** end of warmelt-base+01.cc ****/
