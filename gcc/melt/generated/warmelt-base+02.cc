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



class Melt_InitialClassyFrame_WARMELTmiBASE_h927796949; // forward declaration fromline 6543
typedef Melt_InitialClassyFrame_WARMELTmiBASE_h927796949 Melt_InitialFrame;
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




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiBASE_DIAGNOSTIC_EXPAND_MESSAGE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiBASE_COMPARE_NAMED_ALPHA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiBASE_ADD2OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiBASE_STRING4OUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiBASE_ADD2OUT4NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiBASE_ADD2OUT4INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiBASE_ADD2OUT4STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiBASE_ADD2OUT4STRBUF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiBASE_ADD2OUT4CLONEDSYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiBASE_ADD2OUT4NAMED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiBASE_ADD2OUT4ROOTOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiBASE_ADD2OUT4ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiBASE_MAPSTRING_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiBASE_MAPSTRING_ITERATE_TEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiBASE_MULTIPLE_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiBASE_h927796949 /*opaqueinitialclassy*/ meltinitial_frame_t;


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
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiBASE__initialmeltchunk_56 (meltinitial_frame_t*, char*);


/**** warmelt-base+02.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1718 */

    /** start of frame for meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY// fromline 1534
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
        MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
        MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY() //the constructor fromline 1608
            : Melt_CallFrameWithValues<5> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY)) {};
        MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY


    /** end of frame for meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1663**/

    /* end of frame for routine meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1722 */

    /* classy proc frame meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY */
    MeltFrame_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY fromline 1751*/
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
    MELT_LOCATION("warmelt-base.melt:1928:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1931:/ cond");
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
                    MELT_LOCATION("warmelt-base.melt:1932:/ cond");
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
                                            MELT_LOCATION("warmelt-base.melt:1936:/ apply");
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

                                    MELT_LOCATION("warmelt-base.melt:1933:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:1931:/ clear");
                    /*clear*/ /*_#IS_CLOSURE__L2*/
                    meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:1928:/ clear");
        /*clear*/ /*_#IS_MULTIPLE__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MULTIPLE_BACKWARD_EVERY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_29_WARMELTmiBASE_MULTIPLE_BACKWARD_EVERY*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1718 */

    /** start of frame for meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH// fromline 1534
        : public Melt_CallFrameWithValues<13>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
        MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH() //the constructor fromline 1608
            : Melt_CallFrameWithValues<13> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH)) {};
        MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH


    /** end of frame for meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1663**/

    /* end of frame for routine meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1722 */

    /* classy proc frame meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH */
    MeltFrame_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1940:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1943:/ cond");
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
                    MELT_LOCATION("warmelt-base.melt:1944:/ cond");
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
                                MELT_LOCATION("warmelt-base.melt:1945:/ cond");
                                /*cond*/
                                if (/*_#IS_CLOSURE__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:1946:/ quasiblock");


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
                                            MELT_LOCATION("warmelt-base.melt:1949:/ loop");
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
                                                    MELT_LOCATION("warmelt-base.melt:1950:/ cond");
                                                    /*cond*/
                                                    if (/*_#gteqI__L7*/ meltfnum[6]) /*then*/
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
                                                    /*_#gteqI__L8*/
                                                    meltfnum[7] =
                                                        ((/*_#IX__L6*/ meltfnum[5]) >= (/*_#LN2__L5*/ meltfnum[4]));;
                                                    MELT_LOCATION("warmelt-base.melt:1951:/ cond");
                                                    /*cond*/
                                                    if (/*_#gteqI__L8*/ meltfnum[7]) /*then*/
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
                                                    /*_.MULTIPLE_NTH__V11*/
                                                    meltfptr[10] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP1__V2*/ meltfptr[1]), (/*_#IX__L6*/ meltfnum[5])));;
                                                    /*^compute*/
                                                    /*_.MULTIPLE_NTH__V12*/
                                                    meltfptr[11] =
                                                        (melt_multiple_nth((melt_ptr_t)(/*_.TUP2__V3*/ meltfptr[2]), (/*_#IX__L6*/ meltfnum[5])));;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-base.melt:1952:/ apply");
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
                                                    MELT_LOCATION("warmelt-base.melt:1953:/ compute");
                                                    /*_#IX__L6*/
                                                    meltfnum[5] = /*_#SETQ___L10*/ meltfnum[9] = /*_#plI__L9*/ meltfnum[8];;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-base.melt:1949:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:1946:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:1945:/ clear");
                                            /*clear*/ /*_.LET___V8*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IF___V6*/
                                meltfptr[5] = /*_._IF___V7*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:1944:/ clear");
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

                            /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_._IF___V5*/
                    meltfptr[4] = /*_._IF___V6*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:1943:/ clear");
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

                /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:1940:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IF___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-base.melt:1940:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_30_WARMELTmiBASE_MULTIPLE_EVERY_BOTH*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1718 */

    /** start of frame for meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER// fromline 1534
        : public Melt_CallFrameWithValues<14>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER), clos) {};
        MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER() //the constructor fromline 1608
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER)) {};
        MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER


    /** end of frame for meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1663**/

    /* end of frame for routine meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1722 */

    /* classy proc frame meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER */
    MeltFrame_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:1976:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:1981:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.LSTHANDLER__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-base.melt:1981:/ cond");
            /*cond*/
            if (/*_#IS_LIST__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:1981:/ cond.else");

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
                            argtab[2].meltbp_long = 1981;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LSTHANDLER__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                            meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V4*/
                        meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:1981:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                        meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V3*/
            meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:1981:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:1983:/ quasiblock");


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

                /*_.OLDTUPLE__V6*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#OLDSIZE__L2*/
        meltfnum[0] = 0;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:1985:/ cond");
        /*cond*/
        if ((/*!NOTNULL*/ meltfrout->tabval[3])) /*then*/
            {
                /*^cond.then*/
                /*_._IF___V7*/ meltfptr[3] = /*_.OLDTUPLE__V6*/ meltfptr[4];;
            }
        else
            {
                MELT_LOCATION("warmelt-base.melt:1985:/ cond.else");

                /*_._IF___V7*/
                meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#MULTIPLE_LENGTH__L3*/
        meltfnum[2] =
            (melt_multiple_length((melt_ptr_t)(/*_.OLDTUPLE__V6*/ meltfptr[4])));;
        MELT_LOCATION("warmelt-base.melt:1986:/ compute");
        /*_#OLDSIZE__L2*/
        meltfnum[0] = /*_#SETQ___L4*/ meltfnum[3] = /*_#MULTIPLE_LENGTH__L3*/ meltfnum[2];;
        MELT_LOCATION("warmelt-base.melt:1987:/ quasiblock");


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
                        MELT_LOCATION("warmelt-base.melt:1995:/ locexp");
                        meltgc_multiple_put_nth((melt_ptr_t)(/*_.NEWTUPLE__V8*/ meltfptr[7]), (/*_#I__L8*/ meltfnum[7]), (melt_ptr_t)(/*_.CURHANDER__V9*/ meltfptr[8]));
                    }
                    ;
                    /*_#plI__L10*/
                    meltfnum[9] =
                        ((/*_#I__L8*/ meltfnum[7]) + (1));;
                    MELT_LOCATION("warmelt-base.melt:1996:/ compute");
                    /*_#I__L8*/
                    meltfnum[7] = /*_#SETQ___L11*/ meltfnum[10] = /*_#plI__L10*/ meltfnum[9];;
                    if (/*_#IUNUSED__L9*/ meltfnum[8]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:1992:/ clear");
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
                    MELT_LOCATION("warmelt-base.melt:2002:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_A__L12*/
                        meltfnum[11] =
                            melt_is_instance_of((melt_ptr_t)(/*_.CURHANDLER__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_GCC_PRAGMA*/ meltfrout->tabval[5])));;
                        MELT_LOCATION("warmelt-base.melt:2002:/ cond");
                        /*cond*/
                        if (/*_#IS_A__L12*/ meltfnum[11]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-base.melt:2002:/ cond.else");

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
                                        argtab[2].meltbp_long = 2002;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURHANDLER__V11*/ meltfptr[10];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                                        meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V13*/
                                    meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:2002:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                                    meltfptr[13] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V12*/
                        meltfptr[11] = /*_._IFELSE___V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:2002:/ clear");
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
                        MELT_LOCATION("warmelt-base.melt:2004:/ locexp");
                        meltgc_multiple_put_nth((melt_ptr_t)(/*_.NEWTUPLE__V8*/ meltfptr[7]), (/*_#I__L8*/ meltfnum[7]), (melt_ptr_t)(/*_.CURHANDLER__V11*/ meltfptr[10]));
                    }
                    ;
                    /*_#plI__L13*/
                    meltfnum[11] =
                        ((/*_#I__L8*/ meltfnum[7]) + (1));;
                    MELT_LOCATION("warmelt-base.melt:2005:/ compute");
                    /*_#I__L8*/
                    meltfnum[7] = /*_#SETQ___L14*/ meltfnum[13] = /*_#plI__L13*/ meltfnum[11];;
                } /* end foreach_in_list meltcit2__EACHLIST */
            /*_.CURPAIR__V10*/ meltfptr[9] = NULL;
            /*_.CURHANDLER__V11*/
            meltfptr[10] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:1999:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:2007:/ cond");
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

        MELT_LOCATION("warmelt-base.melt:1987:/ clear");
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

        MELT_LOCATION("warmelt-base.melt:1983:/ clear");
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

        MELT_LOCATION("warmelt-base.melt:1976:/ clear");
        /*clear*/ /*_.IFCPP___V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PRAGMA_HANDLER", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_31_WARMELTmiBASE_REGISTER_PRAGMA_HANDLER*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_32_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_32_WARMELTmiBASE_SET_REFERENCE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_32_WARMELTmiBASE_SET_REFERENCE fromline 1718 */

    /** start of frame for meltrout_32_WARMELTmiBASE_SET_REFERENCE of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE// fromline 1534
        : public Melt_CallFrameWithValues<3>
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
        MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE), clos) {};
        MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE() //the constructor fromline 1608
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE)) {};
        MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE


    /** end of frame for meltrout_32_WARMELTmiBASE_SET_REFERENCE fromline 1663**/

    /* end of frame for routine meltrout_32_WARMELTmiBASE_SET_REFERENCE fromline 1722 */

    /* classy proc frame meltrout_32_WARMELTmiBASE_SET_REFERENCE */
    MeltFrame_meltrout_32_WARMELTmiBASE_SET_REFERENCE
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_32_WARMELTmiBASE_SET_REFERENCE fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:2061:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:2064:/ cond");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_32_WARMELTmiBASE_SET_REFERENCE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_32_WARMELTmiBASE_SET_REFERENCE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1718 */

    /** start of frame for meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR// fromline 1534
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
        MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR), clos) {};
        MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR() //the constructor fromline 1608
            : Melt_CallFrameWithValues<6> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR)) {};
        MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR


    /** end of frame for meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1663**/

    /* end of frame for routine meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1722 */

    /* classy proc frame meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR */
    MeltFrame_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:2073:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:2074:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.VD__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_VALUE_DESCRIPTOR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-base.melt:2074:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-base.melt:2074:/ cond.else");

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
                            argtab[2].meltbp_long = 2074;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VD__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                            meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V4*/
                        meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:2074:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V5*/
                        meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V3*/
            meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-base.melt:2074:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:2075:/ cond");
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

                /*_.REFERENCED_VALUE__V6*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-base.melt:2075:/ locexp");
            meltgc_append_list((melt_ptr_t)(/*_.REFERENCED_VALUE__V6*/ meltfptr[4]), (melt_ptr_t)(/*_.VD__V2*/ meltfptr[1]));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-base.melt:2073:/ clear");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_33_WARMELTmiBASE_INSTALL_VALUE_DESCRIPTOR*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1718 */

    /** start of frame for meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST// fromline 1534
        : public Melt_CallFrameWithValues<2>
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
        MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST), clos) {};
        MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST() //the constructor fromline 1608
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST)) {};
        MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST


    /** end of frame for meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1663**/

    /* end of frame for routine meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1722 */

    /* classy proc frame meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST */
    MeltFrame_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:2078:/ block");
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-base.melt:2079:/ cond");
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

                /*_.REFERENCED_VALUE__V2*/ meltfptr[1] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:2078:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.REFERENCED_VALUE__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-base.melt:2078:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_34_WARMELTmiBASE_RETRIEVE_VALUE_DESCRIPTOR_LIST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1718 */

    /** start of frame for meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT// fromline 1534
        : public Melt_CallFrameWithValues<20>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT), clos) {};
        MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT() //the constructor fromline 1608
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT)) {};
        MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT


    /** end of frame for meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1663**/

    /* end of frame for routine meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1722 */

    /* classy proc frame meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT */
    MeltFrame_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:3172:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:3176:/ cond");
        /*cond*/
        if (/*_#NOT__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3177:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-base.melt:3177:/ locexp");
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

                    MELT_LOCATION("warmelt-base.melt:3176:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:3178:/ quasiblock");


        /*_.LASTPAIR__V5*/
        meltfptr[3] =
            (melt_list_last((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_.PREVPAIR__V6*/
        meltfptr[5] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3181:/ cond");
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
                        MELT_LOCATION("warmelt-base.melt:3181:/ cond");
                        /*cond*/
                        if (/*_#IS_PAIR__L3*/ meltfnum[2]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                            }
                        else {
                                MELT_LOCATION("warmelt-base.melt:3181:/ cond.else");

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
                                        argtab[2].meltbp_long = 3181;
                                        /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                                        meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V9*/
                                    meltfptr[8] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:3181:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                                    meltfptr[9] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V8*/
                        meltfptr[7] = /*_._IFELSE___V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:3181:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:3181:/ clear");
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
                    MELT_LOCATION("warmelt-base.melt:3185:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_PAIR__L4*/
                        meltfnum[2] =
                            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V11*/ meltfptr[9])) == MELTOBMAG_PAIR);;
                        MELT_LOCATION("warmelt-base.melt:3185:/ cond");
                        /*cond*/
                        if (/*_#IS_PAIR__L4*/ meltfnum[2]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-base.melt:3185:/ cond.else");

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
                                        argtab[2].meltbp_long = 3185;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURPAIR__V11*/ meltfptr[9];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                                        meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V14*/
                                    meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-base.melt:3185:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                                    meltfptr[14] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V13*/
                        meltfptr[7] = /*_._IFELSE___V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-base.melt:3185:/ clear");
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
                    MELT_LOCATION("warmelt-base.melt:3186:/ cond");
                    /*cond*/
                    if (/*_#eqeq__L5*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3187:/ compute");
                                /*_.CURPAIR__V11*/
                                meltfptr[9] = /*_.SETQ___V17*/ meltfptr[13] = (/*nil*/NULL);;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_PAIR__L6*/
                                meltfnum[5] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.PREVPAIR__V6*/ meltfptr[5])) == MELTOBMAG_PAIR);;
                                MELT_LOCATION("warmelt-base.melt:3188:/ cond");
                                /*cond*/
                                if (/*_#IS_PAIR__L6*/ meltfnum[5]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-base.melt:3189:/ locexp");
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
                                        MELT_LOCATION("warmelt-base.melt:3188:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-base.melt:3196:/ locexp");
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
                                MELT_LOCATION("warmelt-base.melt:3202:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = /*_.CURELEM__V12*/ meltfptr[8];;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3202:/ locexp");
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
                                MELT_LOCATION("warmelt-base.melt:3186:/ quasiblock");


                                /*_.PROGN___V19*/
                                meltfptr[18] = /*_.RETURN___V18*/ meltfptr[17];;
                                /*^compute*/
                                /*_._IF___V16*/
                                meltfptr[14] = /*_.PROGN___V19*/ meltfptr[18];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3186:/ clear");
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

                            /*_._IF___V16*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-base.melt:3203:/ compute");
                    /*_.PREVPAIR__V6*/
                    meltfptr[5] = /*_.SETQ___V20*/ meltfptr[13] = /*_.CURPAIR__V11*/ meltfptr[9];;
                } /* end foreach_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V11*/ meltfptr[9] = NULL;
            /*_.CURELEM__V12*/
            meltfptr[8] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-base.melt:3182:/ clear");
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

        MELT_LOCATION("warmelt-base.melt:3178:/ clear");
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

        MELT_LOCATION("warmelt-base.melt:3172:/ clear");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_35_WARMELTmiBASE_LIST_REMOVE_LAST_ELEMENT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_COMPLETE_SEQUENCE_AS_TUPLE_ix = 0, variad_COMPLETE_SEQUENCE_AS_TUPLE_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_COMPLETE_SEQUENCE_AS_TUPLE_len)
#define melt_variadic_index variad_COMPLETE_SEQUENCE_AS_TUPLE_ix

    long current_blocklevel_signals_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1718 */

    /** start of frame for meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE// fromline 1534
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE), clos) {};
        MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE() //the constructor fromline 1608
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE)) {};
        MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE


    /** end of frame for meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1663**/

    /* end of frame for routine meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1722 */

    /* classy proc frame meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE */
    MeltFrame_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:3207:/ getarg");
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

        MELT_LOCATION("warmelt-base.melt:3211:/ quasiblock");


        /*_.NEWLIST__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[0]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:3213:/ cond");
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
                                    MELT_LOCATION("warmelt-base.melt:3217:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.COMP__V5*/ meltfptr[4]));
                                }
                                ;
                                if (/*_#IX__L2*/ meltfnum[1]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-base.melt:3214:/ clear");
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
                MELT_LOCATION("warmelt-base.melt:3213:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_LIST__L3*/
                    meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-base.melt:3219:/ cond");
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
                                                MELT_LOCATION("warmelt-base.melt:3223:/ locexp");
                                                meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCOMP__V7*/ meltfptr[6]));
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit2__EACHLIST */
                                    /*_.CURPAIR__V6*/ meltfptr[5] = NULL;
                                    /*_.CURCOMP__V7*/
                                    meltfptr[6] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:3220:/ clear");
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
                            MELT_LOCATION("warmelt-base.melt:3219:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-base.melt:3226:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]));
                                }
                                ;
                                MELT_LOCATION("warmelt-base.melt:3225:/ quasiblock");


                                /*epilog*/
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3213:/ clear");
                    /*clear*/ /*_#IS_LIST__L3*/
                    meltfnum[2] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3227:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_3:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:3229:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix == variad_COMPLETE_SEQUENCE_AS_TUPLE_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_COMPLETE_SEQUENCE_AS_TUPLE_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:3231:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V9*/
                            meltfptr[8] =  /*reallynil*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:3229:/ cond.else");

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
                                            MELT_LOCATION("warmelt-base.melt:3233:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V10*/ meltfptr[9]));
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3229:/ clear");
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

                                        MELT_LOCATION("warmelt-base.melt:3235:/ quasiblock");


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
                                        MELT_LOCATION("warmelt-base.melt:3238:/ cond");
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

                                                /*_.NAMED_NAME__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                                            }
                                        ;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3237:/ locexp");
                                            error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "COMPLETE_SEQUENCE_AS_TUPLE with unsupported ctype"),
                                                   melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[12])));
                                        }
                                        ;

#if MELT_HAVE_DEBUG
                                        MELT_LOCATION("warmelt-base.melt:3239:/ cppif.then");
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
                                                    MELT_LOCATION("warmelt-base.melt:3239:/ cond.else");

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
                                                            argtab[2].meltbp_long = 3239;
                                                            /*^apply.arg*/
                                                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V12*/ meltfptr[11];
                                                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V15*/
                                                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:3239:/ clear");
                                                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                        meltfptr[15] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.IFCPP___V14*/
                                            meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3239:/ clear");
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

                                        MELT_LOCATION("warmelt-base.melt:3235:/ clear");
                                        /*clear*/ /*_.VCTY__V12*/
                                        meltfptr[11] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.NAMED_NAME__V13*/
                                        meltfptr[12] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.IFCPP___V14*/
                                        meltfptr[13] = 0 ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3229:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:3227:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V8*/
            meltfptr[7] = /*_.ARGLOOP__V9*/ meltfptr[8];;
        }
        ;
        MELT_LOCATION("warmelt-base.melt:3241:/ quasiblock");



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
        MELT_LOCATION("warmelt-base.melt:3243:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V18*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-base.melt:3243:/ locexp");
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

        MELT_LOCATION("warmelt-base.melt:3241:/ clear");
        /*clear*/ /*_.RES__V18*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V19*/
        meltfptr[11] = 0 ;
        /*_.LET___V3*/
        meltfptr[2] = /*_.LET___V17*/ meltfptr[15];;

        MELT_LOCATION("warmelt-base.melt:3211:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:3207:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3207:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_36_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_TUPLE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_COMPLETE_SEQUENCE_AS_LIST_ix = 0, variad_COMPLETE_SEQUENCE_AS_LIST_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_COMPLETE_SEQUENCE_AS_LIST_len)
#define melt_variadic_index variad_COMPLETE_SEQUENCE_AS_LIST_ix

    long current_blocklevel_signals_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1718 */

    /** start of frame for meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST// fromline 1534
        : public Melt_CallFrameWithValues<17>
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
        MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<17> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST), clos) {};
        MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST() //the constructor fromline 1608
            : Melt_CallFrameWithValues<17> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<17> (fil,lin, sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST)) {};
        MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<17> (fil,lin, sizeof(MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST


    /** end of frame for meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1663**/

    /* end of frame for routine meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1722 */

    /* classy proc frame meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST */
    MeltFrame_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:3247:/ getarg");
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

        MELT_LOCATION("warmelt-base.melt:3251:/ quasiblock");


        /*_.NEWLIST__V4*/
        meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[0]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MULTIPLE__L1*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-base.melt:3253:/ cond");
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
                                    MELT_LOCATION("warmelt-base.melt:3257:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.COMP__V5*/ meltfptr[4]));
                                }
                                ;
                                if (/*_#IX__L2*/ meltfnum[1]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-base.melt:3254:/ clear");
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
                MELT_LOCATION("warmelt-base.melt:3253:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_LIST__L3*/
                    meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-base.melt:3259:/ cond");
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
                                                MELT_LOCATION("warmelt-base.melt:3263:/ locexp");
                                                meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCOMP__V7*/ meltfptr[6]));
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit2__EACHLIST */
                                    /*_.CURPAIR__V6*/ meltfptr[5] = NULL;
                                    /*_.CURCOMP__V7*/
                                    meltfptr[6] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-base.melt:3260:/ clear");
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
                            MELT_LOCATION("warmelt-base.melt:3259:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-base.melt:3266:/ locexp");
                                    meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]));
                                }
                                ;
                                MELT_LOCATION("warmelt-base.melt:3265:/ quasiblock");


                                /*epilog*/
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-base.melt:3253:/ clear");
                    /*clear*/ /*_#IS_LIST__L3*/
                    meltfnum[2] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3267:/ loop");
        /*loop*/
        {
meltlabloop_ARGLOOP_4:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-base.melt:3269:/ cond");
                /*cond*/
                if (/*ifvariadic nomore*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix == variad_COMPLETE_SEQUENCE_AS_LIST_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_COMPLETE_SEQUENCE_AS_LIST_ix += 0;;
                            MELT_LOCATION("warmelt-base.melt:3271:/ quasiblock");


                            /*^compute*/
                            /*_.ARGLOOP__V9*/
                            meltfptr[8] =  /*reallynil*/ NULL ;;

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
                        MELT_LOCATION("warmelt-base.melt:3269:/ cond.else");

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
                                            MELT_LOCATION("warmelt-base.melt:3273:/ locexp");
                                            meltgc_append_list((melt_ptr_t)(/*_.NEWLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.V__V10*/ meltfptr[9]));
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3269:/ clear");
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

                                        MELT_LOCATION("warmelt-base.melt:3275:/ quasiblock");


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
                                        MELT_LOCATION("warmelt-base.melt:3278:/ cond");
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

                                                /*_.NAMED_NAME__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                                            }
                                        ;

                                        {
                                            MELT_LOCATION("warmelt-base.melt:3277:/ locexp");
                                            error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "COMPLETE_SEQUENCE_AS_LIST with unsupported ctype"),
                                                   melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[12])));
                                        }
                                        ;

#if MELT_HAVE_DEBUG
                                        MELT_LOCATION("warmelt-base.melt:3279:/ cppif.then");
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
                                                    MELT_LOCATION("warmelt-base.melt:3279:/ cond.else");

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
                                                            argtab[2].meltbp_long = 3279;
                                                            /*^apply.arg*/
                                                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VCTY__V12*/ meltfptr[11];
                                                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V15*/
                                                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-base.melt:3279:/ clear");
                                                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                                                        meltfptr[15] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_.IFCPP___V14*/
                                            meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3279:/ clear");
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

                                        MELT_LOCATION("warmelt-base.melt:3275:/ clear");
                                        /*clear*/ /*_.VCTY__V12*/
                                        meltfptr[11] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.NAMED_NAME__V13*/
                                        meltfptr[12] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.IFCPP___V14*/
                                        meltfptr[13] = 0 ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-base.melt:3269:/ clear");
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
            MELT_LOCATION("warmelt-base.melt:3267:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V8*/
            meltfptr[7] = /*_.ARGLOOP__V9*/ meltfptr[8];;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-base.melt:3281:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.NEWLIST__V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-base.melt:3281:/ locexp");
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

        MELT_LOCATION("warmelt-base.melt:3251:/ clear");
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
        MELT_LOCATION("warmelt-base.melt:3247:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3247:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_37_WARMELTmiBASE_COMPLETE_SEQUENCE_AS_LIST*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1718 */

    /** start of frame for meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET// fromline 1534
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1538*/
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
        MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET), clos) {};
        MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET() //the constructor fromline 1608
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET)) {};
        MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET


    /** end of frame for meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1663**/

    /* end of frame for routine meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1722 */

    /* classy proc frame meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET */
    MeltFrame_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:3308:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:3310:/ cond");
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

                    MELT_LOCATION("warmelt-base.melt:3310:/ clear");
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
                    MELT_LOCATION("warmelt-base.melt:3313:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3314:/ quasiblock");


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

                                        /*_.NAMSTR__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
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

                                MELT_LOCATION("warmelt-base.melt:3314:/ clear");
                                /*clear*/ /*_.NAMSTR__V7*/
                                meltfptr[6] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PREDEFNAM_CHK__V8*/
                                meltfptr[7] = 0 ;
                                /*_._IFELSE___V5*/
                                meltfptr[3] = /*_.LET___V6*/ meltfptr[5];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3313:/ clear");
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
                                MELT_LOCATION("warmelt-base.melt:3319:/ cond");
                                /*cond*/
                                if (/*_#IS_INTEGERBOX__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3320:/ quasiblock");


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

                                            MELT_LOCATION("warmelt-base.melt:3320:/ clear");
                                            /*clear*/ /*_#NUM__L4*/
                                            meltfnum[3] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PREDEFNAM_CHK__V11*/
                                            meltfptr[5] = 0 ;
                                            /*_._IFELSE___V9*/
                                            meltfptr[6] = /*_.LET___V10*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3319:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:3324:/ quasiblock");


                                            /*_.PROGN___V12*/
                                            meltfptr[5] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V9*/
                                            meltfptr[6] = /*_.PROGN___V12*/ meltfptr[5];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3319:/ clear");
                                            /*clear*/ /*_.PROGN___V12*/
                                            meltfptr[5] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V5*/
                                meltfptr[3] = /*_._IFELSE___V9*/ meltfptr[6];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3313:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:3310:/ clear");
                    /*clear*/ /*_#IS_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V5*/
                    meltfptr[3] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3308:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3308:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_38_WARMELTmiBASE_MELT_PREDEFINED_GET*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_39_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_39_WARMELTmiBASE_SYMBOL_CNAME fromline 1718 */

    /** start of frame for meltrout_39_WARMELTmiBASE_SYMBOL_CNAME of CLASS_PROCROUTINEOBJ from 1521**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME// fromline 1534
        : public Melt_CallFrameWithValues<20>
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
        MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME(meltclosure_ptr_t clos) //the closure constructor fromline 1596
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1600*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1604*/
                sizeof(MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME), clos) {};
        MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME() //the constructor fromline 1608
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1612*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1616*/
                sizeof(MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME)) {};
#if ENABLE_CHECKING /*fromline 1620*/
        MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME(const char*fil, int lin) //the constructor fromline 1622
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME)) {};
        MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1627
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME), clos) {};
#endif /* ENABLE_CHECKING fromline 1632*/

    }; // end  class MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME


    /** end of frame for meltrout_39_WARMELTmiBASE_SYMBOL_CNAME fromline 1663**/

    /* end of frame for routine meltrout_39_WARMELTmiBASE_SYMBOL_CNAME fromline 1722 */

    /* classy proc frame meltrout_39_WARMELTmiBASE_SYMBOL_CNAME */
    MeltFrame_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME
    meltfram__ /*declfrastruct fromline 1746*/
    /*classyprocarg meltrout_39_WARMELTmiBASE_SYMBOL_CNAME fromline 1751*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1755*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1759*/
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
    MELT_LOCATION("warmelt-base.melt:3328:/ getarg");
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
        MELT_LOCATION("warmelt-base.melt:3330:/ cond");
        /*cond*/
        if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-base.melt:3331:/ quasiblock");


                    /*_.SBUF__V5*/
                    meltfptr[4] =
                        (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                    MELT_LOCATION("warmelt-base.melt:3332:/ cond");
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

                            /*_.NAMED_NAME__V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-base.melt:3332:/ locexp");
                        meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V5*/ meltfptr[4]),
                                                 melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V6*/ meltfptr[5])));
                    }
                    ;
                    MELT_LOCATION("warmelt-base.melt:3333:/ cond");
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

                            /*_.CSYM_URANK__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-base.melt:3333:/ apply");
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

                    MELT_LOCATION("warmelt-base.melt:3331:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:3330:/ clear");
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
                    MELT_LOCATION("warmelt-base.melt:3336:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-base.melt:3337:/ quasiblock");


                                /*_.SBUF__V12*/
                                meltfptr[6] =
                                    (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                                MELT_LOCATION("warmelt-base.melt:3338:/ cond");
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

                                        /*_.NAMED_NAME__V13*/ meltfptr[7] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3338:/ locexp");
                                    meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V12*/ meltfptr[6]),
                                                             melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[7])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-base.melt:3339:/ locexp");
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

                                MELT_LOCATION("warmelt-base.melt:3337:/ clear");
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

                                MELT_LOCATION("warmelt-base.melt:3336:/ clear");
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
                                MELT_LOCATION("warmelt-base.melt:3342:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L3*/ meltfnum[2]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-base.melt:3343:/ quasiblock");


                                            /*_.SBUF__V17*/
                                            meltfptr[7] =
                                                (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[1])), (const char*)0);;
                                            MELT_LOCATION("warmelt-base.melt:3344:/ cond");
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

                                                    /*_.NAMED_NAME__V18*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                                                }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-base.melt:3344:/ locexp");
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

                                            MELT_LOCATION("warmelt-base.melt:3343:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:3342:/ clear");
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

                                            MELT_LOCATION("warmelt-base.melt:3347:/ quasiblock");


                                            /*_.PROGN___V20*/
                                            meltfptr[7] = (/*nil*/NULL);;
                                            /*^compute*/
                                            /*_._IFELSE___V15*/
                                            meltfptr[3] = /*_.PROGN___V20*/ meltfptr[7];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-base.melt:3342:/ clear");
                                            /*clear*/ /*_.PROGN___V20*/
                                            meltfptr[7] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V10*/
                                meltfptr[4] = /*_._IFELSE___V15*/ meltfptr[3];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-base.melt:3336:/ clear");
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

                    MELT_LOCATION("warmelt-base.melt:3330:/ clear");
                    /*clear*/ /*_#IS_A__L2*/
                    meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V10*/
                    meltfptr[4] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-base.melt:3328:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-base.melt:3328:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_39_WARMELTmiBASE_SYMBOL_CNAME_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_39_WARMELTmiBASE_SYMBOL_CNAME*/



/**** end of warmelt-base+02.cc ****/
