/* GCC MELT GENERATED C++ FILE warmelt-hooks+02.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #2 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f2[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-hooks+02.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-hooks ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2012 - 2014  Free Software Foundation, Inc.
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



class Melt_InitialClassyFrame_WARMELTmiHOOKS_h244320376; // forward declaration fromline 6691
typedef Melt_InitialClassyFrame_WARMELTmiHOOKS_h244320376 Melt_InitialFrame;
/*** 8 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiHOOKS_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiHOOKS_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 8)?melt_WARMELTmiHOOKS_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 8)
        {
            melt_WARMELTmiHOOKS_module_var_ptr_tab[ix]= val;
            melt_WARMELTmiHOOKS_module_var_flags[ix/16] = true;
        }
}

/* MELT module variables indexes */
enum
{
    meltmodatix_none,
    meltmodvarix_GCC_ATTRIBUTE_DICT_symb = 1,
    meltmodvarix_GCC_ATTRIBUTE_INSTALLER_LIST_symb = 2,
    meltmodvarix_PRAGMA_PROCESSOR_symb = 3,
    meltmodvarix_PRAGMA_HANDLER_TUPLE_symb = 4,
    meltmodvarix_MELT_INPCHANBUCK_symb = 5,
    meltmodvarix_JSON_NAME_DICT_symb = 6,
    meltmodvarix_MELT_ALARMBUCKET_symb = 7,
    meltmodvarix_MELT_CHILDBUCK_symb = 8,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 4 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #3 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 4 MELT called hook declarations ***/

/*** 29 extra MELT c-headers ***/


/** MELT extra c-header 1 : h25383009 **/

/* cheader for override gate in warmelt-hooks.melt */
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




/** MELT extra c-header 2 : h17996579 **/

/* flag for PLUGIN_OVERRIDE_GATE */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_override_gate_registered_flag;




/** MELT extra c-header 3 : h160333989 **/

/* flag for PLUGIN_START_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_start_unit_registered_flag;




/** MELT extra c-header 4 : h850751999 **/


void MELT_MODULE_VISIBILITY
melt_startunithook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                             void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 5 : h735898614 **/

/* flag for PLUGIN_FINISH_UNIT */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_finish_unit_registered_flag;




/** MELT extra c-header 6 : h484051762 **/


void MELT_MODULE_VISIBILITY
melt_finishunithook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                              void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 7 : h726359283 **/

/* flag for PLUGIN_ALL_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_passes_start_registered_flag;




/** MELT extra c-header 8 : h680852009 **/


void MELT_MODULE_VISIBILITY
melt_all_passes_start_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                     void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 9 : h838302894 **/

/* flag for PLUGIN_ALL_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_passes_end_registered_flag;




/** MELT extra c-header 10 : h474747019 **/


void MELT_MODULE_VISIBILITY
melt_all_passes_end_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                   void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 11 : h976685337 **/

/* flag for PLUGIN_ALL_IPA_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_ipa_passes_start_registered_flag;




/** MELT extra c-header 12 : h308476728 **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_start_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
        void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 13 : h547684143 **/

/* flag for PLUGIN_ALL_IPA_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_all_ipa_passes_end_registered_flag;




/** MELT extra c-header 14 : h219756380 **/


void MELT_MODULE_VISIBILITY
melt_all_ipa_passes_end_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                       void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 15 : h243258254 **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_START declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_early_gimple_passes_start_registered_flag;




/** MELT extra c-header 16 : h15470532 **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_start_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
        void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 17 : h154621044 **/

/* flag for PLUGIN_EARLY_GIMPLE_PASSES_END declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_early_gimple_passes_end_registered_flag;




/** MELT extra c-header 18 : h794934896 **/


void MELT_MODULE_VISIBILITY
melt_early_gimple_passes_end_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
        void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 19 : h329325342 **/

/* flag for PLUGIN_PRE_GENERICIZE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_pre_genericize_registered_flag;




/** MELT extra c-header 20 : h119827664 **/


void MELT_MODULE_VISIBILITY
melt_pre_genericize_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                   void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 21 : h749010620 **/

/* flag for PLUGIN_FINISH_TYPE declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_finish_type_registered_flag;




/** MELT extra c-header 22 : h218546915 **/


void MELT_MODULE_VISIBILITY
melt_finish_type_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 23 : h944663762 **/

/* flag for PLUGIN_FINISH_DECL declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_finish_decl_registered_flag;




/** MELT extra c-header 24 : h519729539 **/


void MELT_MODULE_VISIBILITY
melt_finish_decl_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 25 : h466161867 **/

/* flag for PLUGIN_PASS_EXECUTION declare */
MELT_EXTERN bool MELT_MODULE_VISIBILITY  melthk_pass_execution_registered_flag;




/** MELT extra c-header 26 : h398570221 **/


void MELT_MODULE_VISIBILITY
melt_pass_execution_hook_callback (void* gcc_data ATTRIBUTE_UNUSED,
                                   void* user_data ATTRIBUTE_UNUSED);




/** MELT extra c-header 27 : h182032758 **/

/* cheader for attributes in warmelt-hooks.melt */
#if  MELT_GCC_VERSION >= 4009 /* GCC 4.9 or later */
#include "attribs.h"
#endif /*GCC 4.9*/
MELT_EXTERN tree MELT_MODULE_VISIBILITY melt_attribute_handler_glue
(tree *pnode, tree name, tree args, int flags, bool *no_add_attrs);




/** MELT extra c-header 28 : h130302843 **/

/* header from warmelt-hooks.melt for poll etc... */
#include <poll.h>

#define MELT_BUFSIZE 8192
#define MELT_POLL_DELAY_MILLISEC 500




/** MELT extra c-header 29 : h317017663 **/

/* headers before connect_to_server */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <map>

struct Melt_connect_info
{
    std::string             mci_service;
    int             	   mci_family;
    int             	   mci_socktype;
    int             	   mci_protocol;
    socklen_t       	   mci_addrlen;
    struct sockaddr_storage mci_sockaddr;
};
typedef std::map<std::string,Melt_connect_info> melt_connect_info_map_t;
/* end of headers for connect_to_server */


/*** end of 29 extra MELT c-headers ***/

/** start of declaration for hook melthook_HOOK_LOW_DEBUG_VALUE_AT**/

/** declaration of hook melthook_HOOK_LOW_DEBUG_VALUE_AT */
MELT_EXTERN
void melthook_HOOK_LOW_DEBUG_VALUE_AT(melt_ptr_t melthookdatap,
                                      melt_ptr_t meltinp0_VAL,
                                      const char* meltinp1_FILENAME,
                                      long meltinp2_LINENO,
                                      const char* meltinp3_MSG,
                                      long meltinp4_COUNT)
;



/** end of declaration for hook melthook_HOOK_LOW_DEBUG_VALUE_AT**/



/** start of declaration for hook melthook_HOOK_LOW_STDERR_VALUE_AT**/

/** declaration of hook melthook_HOOK_LOW_STDERR_VALUE_AT */
MELT_EXTERN
void melthook_HOOK_LOW_STDERR_VALUE_AT(melt_ptr_t melthookdatap,
                                       melt_ptr_t meltinp0_VAL,
                                       const char* meltinp1_FILENAME,
                                       long meltinp2_LINENO,
                                       const char* meltinp3_MSG,
                                       long meltinp4_COUNT)
;



/** end of declaration for hook melthook_HOOK_LOW_STDERR_VALUE_AT**/



/** start of declaration for hook melthook_HOOK_OVERRIDE_GATE**/

/** declaration of hook melthook_HOOK_OVERRIDE_GATE */
MELT_EXTERN
void melthook_HOOK_OVERRIDE_GATE(melt_ptr_t melthookdatap,
                                 long meltinp0_BEFOREGATE,
                                 long* meltoutp0_AFTERGATE)
;



/** end of declaration for hook melthook_HOOK_OVERRIDE_GATE**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiHOOKS_ENABLE_OVERRIDE_GATE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiHOOKS_MAYBE_DISABLE_OVERRIDE_GATE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiHOOKS_REGISTER_OVERRIDE_GATE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiHOOKS_REGISTER_OVERRIDE_GATE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiHOOKS_UNREGISTER_OVERRIDE_GATE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiHOOKS_UNREGISTER_OVERRIDE_GATE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiHOOKS_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiHOOKS_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiHOOKS_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_START_UNIT**/

/** declaration of hook melthook_HOOK_START_UNIT */
MELT_EXTERN
void melthook_HOOK_START_UNIT(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_START_UNIT**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiHOOKS_ENABLE_START_UNIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiHOOKS_AT_START_UNIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiHOOKS_AT_START_UNIT_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiHOOKS_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiHOOKS_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiHOOKS_LAMBDA_cl6(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_FINISH_UNIT**/

/** declaration of hook melthook_HOOK_FINISH_UNIT */
MELT_EXTERN
void melthook_HOOK_FINISH_UNIT(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_FINISH_UNIT**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiHOOKS_ENABLE_FINISH_UNIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiHOOKS_AT_FINISH_UNIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiHOOKS_AT_FINISH_UNIT_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_ALL_PASSES_START**/

/** declaration of hook melthook_HOOK_ALL_PASSES_START */
MELT_EXTERN
void melthook_HOOK_ALL_PASSES_START(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_ALL_PASSES_START**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiHOOKS_ENABLE_ALL_PASSES_START(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiHOOKS_REGISTER_ALL_PASSES_START_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiHOOKS_REGISTER_ALL_PASSES_START_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_ALL_PASSES_END**/

/** declaration of hook melthook_HOOK_ALL_PASSES_END */
MELT_EXTERN
void melthook_HOOK_ALL_PASSES_END(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_ALL_PASSES_END**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiHOOKS_ENABLE_ALL_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiHOOKS_REGISTER_ALL_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiHOOKS_REGISTER_ALL_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_ALL_IPA_PASSES_START**/

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_START */
MELT_EXTERN
void melthook_HOOK_ALL_IPA_PASSES_START(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_ALL_IPA_PASSES_START**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiHOOKS_ENABLE_ALL_IPA_PASSES_START(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_START_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_START_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_ALL_IPA_PASSES_END**/

/** declaration of hook melthook_HOOK_ALL_IPA_PASSES_END */
MELT_EXTERN
void melthook_HOOK_ALL_IPA_PASSES_END(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_ALL_IPA_PASSES_END**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiHOOKS_ENABLE_ALL_IPA_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiHOOKS_REGISTER_ALL_IPA_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_EARLY_GIMPLE_PASSES_START**/

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_START */
MELT_EXTERN
void melthook_HOOK_EARLY_GIMPLE_PASSES_START(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_EARLY_GIMPLE_PASSES_START**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_START(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_START_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_START_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_EARLY_GIMPLE_PASSES_END**/

/** declaration of hook melthook_HOOK_EARLY_GIMPLE_PASSES_END */
MELT_EXTERN
void melthook_HOOK_EARLY_GIMPLE_PASSES_END(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_EARLY_GIMPLE_PASSES_END**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_PRE_GENERICIZE**/

/** declaration of hook melthook_HOOK_PRE_GENERICIZE */
MELT_EXTERN
void melthook_HOOK_PRE_GENERICIZE(melt_ptr_t melthookdatap,
                                  melt_treeptr_t meltinp0_TFNDECL)
;



/** end of declaration for hook melthook_HOOK_PRE_GENERICIZE**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_FINISH_TYPE**/

/** declaration of hook melthook_HOOK_FINISH_TYPE */
MELT_EXTERN
void melthook_HOOK_FINISH_TYPE(melt_ptr_t melthookdatap,
                               melt_treeptr_t meltinp0_TFNDECL)
;



/** end of declaration for hook melthook_HOOK_FINISH_TYPE**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_FINISH_DECL**/

/** declaration of hook melthook_HOOK_FINISH_DECL */
MELT_EXTERN
void melthook_HOOK_FINISH_DECL(melt_ptr_t melthookdatap,
                               melt_treeptr_t meltinp0_TFNDECL)
;



/** end of declaration for hook melthook_HOOK_FINISH_DECL**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_PASS_EXECUTION**/

/** declaration of hook melthook_HOOK_PASS_EXECUTION */
MELT_EXTERN
void melthook_HOOK_PASS_EXECUTION(melt_ptr_t melthookdatap,
                                  const char* meltinp0_PASSNAME,
                                  long meltinp1_PASSNUM,
                                  long meltinp2_TYPENUM)
;



/** end of declaration for hook melthook_HOOK_PASS_EXECUTION**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_GIMPLE_GATE**/

/** declaration of hook melthook_HOOK_GIMPLE_GATE */
MELT_EXTERN
long melthook_HOOK_GIMPLE_GATE(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_GIMPLE_GATE**/



/** start of declaration for hook melthook_HOOK_GIMPLE_EXECUTE**/

/** declaration of hook melthook_HOOK_GIMPLE_EXECUTE */
MELT_EXTERN
long melthook_HOOK_GIMPLE_EXECUTE(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_GIMPLE_EXECUTE**/



/** start of declaration for hook melthook_HOOK_RTL_GATE**/

/** declaration of hook melthook_HOOK_RTL_GATE */
MELT_EXTERN
long melthook_HOOK_RTL_GATE(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_RTL_GATE**/



/** start of declaration for hook melthook_HOOK_RTL_EXECUTE**/

/** declaration of hook melthook_HOOK_RTL_EXECUTE */
MELT_EXTERN
long melthook_HOOK_RTL_EXECUTE(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_RTL_EXECUTE**/



/** start of declaration for hook melthook_HOOK_SIMPLE_IPA_EXECUTE**/

/** declaration of hook melthook_HOOK_SIMPLE_IPA_EXECUTE */
MELT_EXTERN
long melthook_HOOK_SIMPLE_IPA_EXECUTE(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_SIMPLE_IPA_EXECUTE**/



/** start of declaration for hook melthook_HOOK_MELT_DO_INITIAL_MODE**/

/** declaration of hook melthook_HOOK_MELT_DO_INITIAL_MODE */
MELT_EXTERN
void melthook_HOOK_MELT_DO_INITIAL_MODE(melt_ptr_t melthookdatap,
                                        melt_ptr_t meltinp0_MODATA,
                                        const char* meltinp1_MODSTR)
;



/** end of declaration for hook melthook_HOOK_MELT_DO_INITIAL_MODE**/



/** start of declaration for hook melthook_HOOK_INSTALL_ATTRIBUTES**/

/** declaration of hook melthook_HOOK_INSTALL_ATTRIBUTES */
MELT_EXTERN
void melthook_HOOK_INSTALL_ATTRIBUTES(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_INSTALL_ATTRIBUTES**/



/** start of declaration for hook melthook_HOOK_HANDLE_ATTRIBUTE**/

/** declaration of hook melthook_HOOK_HANDLE_ATTRIBUTE */
MELT_EXTERN
melt_treeptr_t melthook_HOOK_HANDLE_ATTRIBUTE(melt_ptr_t melthookdatap,
        melt_treeptr_t meltinp0_TR_IN_NODE,
        melt_treeptr_t meltinp1_TR_NAME,
        melt_treeptr_t meltinp2_TR_ARGS,
        long meltinp3_FLAGS,
        melt_treeptr_t* meltoutp0_TR_OUT_NODE,
        long* meltoutp1_OUT_NO_ADD_ATTRS)
;



/** end of declaration for hook melthook_HOOK_HANDLE_ATTRIBUTE**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_EXIT_FINALIZER**/

/** declaration of hook melthook_HOOK_EXIT_FINALIZER */
MELT_EXTERN
void melthook_HOOK_EXIT_FINALIZER(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_EXIT_FINALIZER**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_79_WARMELTmiHOOKS_AT_EXIT_LAST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_80_WARMELTmiHOOKS_REGISTER_PRAGMA_PROCESSOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_PROCESS_PRAGMA**/

/** declaration of hook melthook_HOOK_PROCESS_PRAGMA */
MELT_EXTERN
void melthook_HOOK_PROCESS_PRAGMA(melt_ptr_t melthookdatap,
                                  long meltinp0_LIX)
;



/** end of declaration for hook melthook_HOOK_PROCESS_PRAGMA**/



/** start of declaration for hook melthook_HOOK_REGISTER_PRAGMAS**/

/** declaration of hook melthook_HOOK_REGISTER_PRAGMAS */
MELT_EXTERN
void melthook_HOOK_REGISTER_PRAGMAS(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_REGISTER_PRAGMAS**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_83_WARMELTmiHOOKS_ADD_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_84_WARMELTmiHOOKS_REMOVE_PRAGMA_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_85_WARMELTmiHOOKS_MELT_INVOKE_INPUT_CALLBACKS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_POLL_INPUTS**/

/** declaration of hook melthook_HOOK_POLL_INPUTS */
MELT_EXTERN
void melthook_HOOK_POLL_INPUTS(melt_ptr_t melthookdatap,
                               long meltinp0_DELAYMS)
;



/** end of declaration for hook melthook_HOOK_POLL_INPUTS**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_87_WARMELTmiHOOKS_IS_POLLING_INPUTS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_88_WARMELTmiHOOKS_EVERY_POLLING_INPUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_89_WARMELTmiHOOKS_GET_POLLING_INPUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_90_WARMELTmiHOOKS_REGISTER_PARAGRAPH_INPUT_CHANNEL_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_91_WARMELTmiHOOKS_REGISTER_RAW_INPUT_CHANNEL_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_92_WARMELTmiHOOKS_UNREGISTER_INPUT_CHANNEL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_93_WARMELTmiHOOKS_CONNECT_TO_SERVER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_94_WARMELTmiHOOKS_JSON_LEXER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_95_WARMELTmiHOOKS_JSON_STATE_ACCEPTING_JVALUE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_96_WARMELTmiHOOKS_MAKE_JSON_PARSER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_97_WARMELTmiHOOKS_JSON_PARSER_INPUT_PROCESSOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_98_WARMELTmiHOOKS_DO_BLOCKING_JSONRPC2_CALL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_99_WARMELTmiHOOKS_LAMBDA_cl8(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_100_WARMELTmiHOOKS_LAMBDA_cl9(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_HANDLE_SIGIO**/

/** declaration of hook melthook_HOOK_HANDLE_SIGIO */
MELT_EXTERN
void melthook_HOOK_HANDLE_SIGIO(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_HANDLE_SIGIO**/



/** start of declaration for hook melthook_HOOK_HANDLE_SIGALRM**/

/** declaration of hook melthook_HOOK_HANDLE_SIGALRM */
MELT_EXTERN
void melthook_HOOK_HANDLE_SIGALRM(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_HANDLE_SIGALRM**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_103_WARMELTmiHOOKS_REGISTER_ALARM_TIMER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_104_WARMELTmiHOOKS_UNREGISTER_ALARM_TIMER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_HANDLE_SIGCHLD**/

/** declaration of hook melthook_HOOK_HANDLE_SIGCHLD */
MELT_EXTERN
void melthook_HOOK_HANDLE_SIGCHLD(melt_ptr_t melthookdatap)
;



/** end of declaration for hook melthook_HOOK_HANDLE_SIGCHLD**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_106_WARMELTmiHOOKS_REGISTER_CHILD_PROCESS_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_107_WARMELTmiHOOKS_UNREGISTER_CHILD_PROCESS_HANDLER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN MELT_LOW_OPTIMIZATION void* melt_start_this_module (void*); /*mandatory start of module*/

MELT_EXTERN const char melt_module_is_gpl_compatible[];

/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiHOOKS_h244320376 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiHOOKS__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiHOOKS__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiHOOKS__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiHOOKS__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_0 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_1 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_2 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_3 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_4 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_5 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_6 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_7 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_8 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_9 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_10 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_11 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_12 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_13 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_14 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_15 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiHOOKS__initialmeltchunk_16 (meltinitial_frame_t*, char*); //declare



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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END fromline 1752 */

    /** start of frame for meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END// fromline 1568
        : public Melt_CallFrameWithValues<1>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END), clos) {};
        MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END() //the constructor fromline 1642
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END)) {};
        MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END


    /** end of frame for meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END fromline 1697**/

    /* end of frame for routine meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END fromline 1756 */

    /* classy proc frame meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END */ MeltFrame_meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_45_WARMELTmiHOOKS_ENABLE_EARLY_GIMPLE_PASSES_END fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("ENABLE_EARLY_GIMPLE_PASSES_END", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
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
    MELT_LOCATION("warmelt-hooks.melt:1190:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1191:/ locexp");
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST fromline 1752 */

    /** start of frame for meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<7>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST), clos) {};
        MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST)) {};
        MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST


    /** end of frame for meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST fromline 1697**/

    /* end of frame for routine meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST fromline 1756 */

    /* classy proc frame meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST */ MeltFrame_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_EARLY_GIMPLE_PASSES_END_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1203:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1207:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-hooks.melt:1208:/ quasiblock");


                    /*^cond*/
                    /*cond*/ if (
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
                                /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.FIRSTLIST__V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1210:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_EARLY_GIMPLE_PASSES_END*/ meltfrout->tabval[2])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#NULL__L2*/ meltfnum[1] =
                        (/*null*/(/*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
                    MELT_LOCATION("warmelt-hooks.melt:1211:/ cond");
                    /*cond*/ if (/*_#NULL__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MAKE_LIST__V5*/ meltfptr[4] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1213:/ compute");
                                /*_.FIRSTLIST__V3*/ meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_.MAKE_LIST__V7*/ meltfptr[6] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1214:/ cond");
                                /*cond*/ if (
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
                                MELT_LOCATION("warmelt-hooks.melt:1212:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-hooks.melt:1211:/ clear");
                                /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:1219:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_LOCATION("warmelt-hooks.melt:1208:/ clear");
                    /*clear*/ /*_.FIRSTLIST__V3*/ meltfptr[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NULL__L2*/ meltfnum[1] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1207:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1222:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad function (non closure) passed to register_early_gimple_passes_end_first"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-hooks.melt:1203:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_EARLY_GIMPLE_PASSES_END_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_46_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_FIRST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST fromline 1752 */

    /** start of frame for meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST// fromline 1568
        : public Melt_CallFrameWithValues<7>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST), clos) {};
        MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST)) {};
        MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST


    /** end of frame for meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST fromline 1697**/

    /* end of frame for routine meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST fromline 1756 */

    /* classy proc frame meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST */ MeltFrame_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_EARLY_GIMPLE_PASSES_END_LAST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1226:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1230:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-hooks.melt:1231:/ quasiblock");


                    /*^cond*/
                    /*cond*/ if (
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
                                /*_.LASTLIST__V3*/ meltfptr[2] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.LASTLIST__V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1233:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_EARLY_GIMPLE_PASSES_END*/ meltfrout->tabval[2])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#NULL__L2*/ meltfnum[1] =
                        (/*null*/(/*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
                    MELT_LOCATION("warmelt-hooks.melt:1234:/ cond");
                    /*cond*/ if (/*_#NULL__L2*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_.MAKE_LIST__V5*/ meltfptr[4] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1236:/ compute");
                                /*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_.MAKE_LIST__V7*/ meltfptr[6] =
                                    (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
                                MELT_LOCATION("warmelt-hooks.melt:1237:/ cond");
                                /*cond*/ if (
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
                                MELT_LOCATION("warmelt-hooks.melt:1235:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-hooks.melt:1234:/ clear");
                                /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:1242:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_LOCATION("warmelt-hooks.melt:1231:/ clear");
                    /*clear*/ /*_.LASTLIST__V3*/ meltfptr[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ENABLE_EARLY_GIMPLE_PASSES_END__V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NULL__L2*/ meltfnum[1] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1230:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1245:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad function (non closure) passed to register_early_gimple_passes_end_last"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-hooks.melt:1226:/ clear");
        /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_EARLY_GIMPLE_PASSES_END_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_47_WARMELTmiHOOKS_REGISTER_EARLY_GIMPLE_PASSES_END_LAST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE fromline 1752 */

    /** start of frame for meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE// fromline 1568
        : public Melt_CallFrameWithValues<1>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE), clos) {};
        MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE)) {};
        MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE


    /** end of frame for meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE fromline 1697**/

    /* end of frame for routine meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE fromline 1756 */

    /* classy proc frame meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE */ MeltFrame_meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_48_WARMELTmiHOOKS_ENABLE_PRE_GENERICIZE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("ENABLE_PRE_GENERICIZE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
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
    MELT_LOCATION("warmelt-hooks.melt:1286:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1287:/ locexp");
            /* enable_pre_genericize ENABLEPREGEN_CHK__1 */
            if (!melthk_pre_genericize_registered_flag)
                {
                    melthk_pre_genericize_registered_flag = true;
                    register_callback (melt_plugin_name, PLUGIN_PRE_GENERICIZE,
                                       melt_pre_genericize_hook_callback, NULL);
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST fromline 1752 */

    /** start of frame for meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST), clos) {};
        MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST)) {};
        MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST


    /** end of frame for meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST fromline 1697**/

    /* end of frame for routine meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST fromline 1756 */

    /* classy proc frame meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST */ MeltFrame_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_PRE_GENERICIZE_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1329:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1333:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.FIRSTLIST__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.FIRSTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1335:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1337:/ compute");
                    /*_.FIRSTLIST__V4*/ meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/ meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1338:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1336:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1335:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1343:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1345:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1346:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_PRE_GENERICIZE*/ meltfrout->tabval[3])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1344:/ quasiblock");


                    /*_.PROGN___V10*/ meltfptr[6] = /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/ meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1343:/ clear");
                    /*clear*/ /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/ meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1347:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pre_genericize_hook"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/ meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1333:/ clear");
        /*clear*/ /*_.FIRSTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1329:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1329:/ locexp");
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
        /*clear*/ /*_.LET___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PRE_GENERICIZE_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_50_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_FIRST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST fromline 1752 */

    /** start of frame for meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST), clos) {};
        MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST)) {};
        MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST


    /** end of frame for meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST fromline 1697**/

    /* end of frame for routine meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST fromline 1756 */

    /* classy proc frame meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST */ MeltFrame_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_PRE_GENERICIZE_LAST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1352:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1356:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.LASTLIST__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.LASTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1358:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1360:/ compute");
                    /*_.LASTLIST__V4*/ meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/ meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1361:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1359:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1358:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1366:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1368:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1369:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_PRE_GENERICIZE*/ meltfrout->tabval[3])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1367:/ quasiblock");


                    /*_.PROGN___V10*/ meltfptr[6] = /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/ meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1366:/ clear");
                    /*clear*/ /*_.ENABLE_PRE_GENERICIZE__V9*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/ meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1370:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pre_genericize_hook"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/ meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1356:/ clear");
        /*clear*/ /*_.LASTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1352:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1352:/ locexp");
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
        /*clear*/ /*_.LET___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PRE_GENERICIZE_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_51_WARMELTmiHOOKS_REGISTER_PRE_GENERICIZE_LAST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE fromline 1752 */

    /** start of frame for meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE// fromline 1568
        : public Melt_CallFrameWithValues<1>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE), clos) {};
        MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE)) {};
        MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE


    /** end of frame for meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE fromline 1697**/

    /* end of frame for routine meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE fromline 1756 */

    /* classy proc frame meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE */ MeltFrame_meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_52_WARMELTmiHOOKS_ENABLE_FINISH_TYPE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("ENABLE_FINISH_TYPE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
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
    MELT_LOCATION("warmelt-hooks.melt:1415:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1416:/ locexp");
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST fromline 1752 */

    /** start of frame for meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST), clos) {};
        MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST)) {};
        MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST


    /** end of frame for meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST fromline 1697**/

    /* end of frame for routine meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST fromline 1756 */

    /* classy proc frame meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST */ MeltFrame_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_FINISH_TYPE_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1458:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1462:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.FIRSTLIST__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.FIRSTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1464:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1466:/ compute");
                    /*_.FIRSTLIST__V4*/ meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/ meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1467:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1465:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1464:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1472:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1474:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1475:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_TYPE*/ meltfrout->tabval[3])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1473:/ quasiblock");


                    /*_.PROGN___V10*/ meltfptr[6] = /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/ meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1472:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/ meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1476:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_type_first"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/ meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1462:/ clear");
        /*clear*/ /*_.FIRSTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1458:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1458:/ locexp");
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
        /*clear*/ /*_.LET___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_TYPE_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_54_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_FIRST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST fromline 1752 */

    /** start of frame for meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST), clos) {};
        MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST)) {};
        MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST


    /** end of frame for meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST fromline 1697**/

    /* end of frame for routine meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST fromline 1756 */

    /* classy proc frame meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST */ MeltFrame_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_FINISH_TYPE_LAST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1481:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1485:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.LASTLIST__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.LASTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1487:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1489:/ compute");
                    /*_.LASTLIST__V4*/ meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/ meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1490:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1488:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1487:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1495:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1497:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1498:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_TYPE*/ meltfrout->tabval[3])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1496:/ quasiblock");


                    /*_.PROGN___V10*/ meltfptr[6] = /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/ meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1495:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_TYPE__V9*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/ meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1499:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_type_last"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/ meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1485:/ clear");
        /*clear*/ /*_.LASTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1481:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1481:/ locexp");
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
        /*clear*/ /*_.LET___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_TYPE_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_55_WARMELTmiHOOKS_REGISTER_FINISH_TYPE_LAST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS fromline 1752 */

    /** start of frame for meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS// fromline 1568
        : public Melt_CallFrameWithValues<15>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<15> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS), clos) {};
        MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS() //the constructor fromline 1642
            : Melt_CallFrameWithValues<15> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS)) {};
        MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS


    /** end of frame for meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS fromline 1697**/

    /* end of frame for routine meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS fromline 1756 */

    /* classy proc frame meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS */ MeltFrame_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("RUN_ENDMELTPASS_FUNCTIONS", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
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
    MELT_LOCATION("warmelt-hooks.melt:1515:/ block");
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1516:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.FIRSTLIST__V2*/ meltfptr[1] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V2*/ meltfptr[1] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-hooks.melt:1517:/ cond");
        /*cond*/ if (
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
                    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.REVLASTLIST__V4*/ meltfptr[3] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:1519:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_CONTAINER*/ meltfrout->tabval[3])), (1), "CLASS_REFERENCE");
            /*_.INST__V6*/ meltfptr[5] =
                newobj;
        };
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V6*/ meltfptr[5], "newly made instance");
        ;
        /*_.RES__V5*/ meltfptr[4] = /*_.INST__V6*/ meltfptr[5];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:1521:/ cond");
        /*cond*/ if (
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
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.FIRSTPAIR__V7*/ meltfptr[6] = melt_list_first( (melt_ptr_t)/*_.FIRSTLIST__V2*/ meltfptr[1]);
                                     melt_magic_discr((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]) == MELTOBMAG_PAIR;
                                     /*_.FIRSTPAIR__V7*/ meltfptr[6] = melt_pair_tail((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]))
                {
                    /*_.FIRSTPROC__V8*/ meltfptr[7] = melt_pair_head((melt_ptr_t) /*_.FIRSTPAIR__V7*/ meltfptr[6]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L1*/ meltfnum[0] =
                        (melt_magic_discr((melt_ptr_t)(/*_.FIRSTPROC__V8*/ meltfptr[7])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-hooks.melt:1528:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-hooks.melt:1529:/ apply");
                                /*apply*/
                                {
                                    /*_.FIRSTPROC__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.FIRSTPROC__V8*/ meltfptr[7]), (melt_ptr_t)(/*_.RES__V5*/ meltfptr[4]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V9*/ meltfptr[8] = /*_.FIRSTPROC__V10*/ meltfptr[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-hooks.melt:1528:/ clear");
                                /*clear*/ /*_.FIRSTPROC__V10*/ meltfptr[9] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                        }
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.FIRSTPAIR__V7*/ meltfptr[6] = NULL;
            /*_.FIRSTPROC__V8*/ meltfptr[7] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-hooks.melt:1525:/ clear");
            /*clear*/ /*_.FIRSTPAIR__V7*/ meltfptr[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_.FIRSTPROC__V8*/ meltfptr[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V9*/ meltfptr[8] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit2__EACHLIST */
            for (/*_.LASTPAIR__V11*/ meltfptr[9] = melt_list_first( (melt_ptr_t)/*_.LASTLIST__V3*/ meltfptr[2]);
                                     melt_magic_discr((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[9]) == MELTOBMAG_PAIR;
                                     /*_.LASTPAIR__V11*/ meltfptr[9] = melt_pair_tail((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[9]))
                {
                    /*_.LASTPROC__V12*/ meltfptr[11] = melt_pair_head((melt_ptr_t) /*_.LASTPAIR__V11*/ meltfptr[9]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_CLOSURE__L2*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.LASTPROC__V12*/ meltfptr[11])) == MELTOBMAG_CLOSURE);;
                    MELT_LOCATION("warmelt-hooks.melt:1535:/ cond");
                    /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
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
                } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
            /*_.LASTPAIR__V11*/ meltfptr[9] = NULL;
            /*_.LASTPROC__V12*/ meltfptr[11] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-hooks.melt:1532:/ clear");
            /*clear*/ /*_.LASTPAIR__V11*/ meltfptr[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LASTPROC__V12*/ meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit3__EACHLIST */
            for (/*_.REVLASTPAIR__V13*/ meltfptr[12] = melt_list_first( (melt_ptr_t)/*_.REVLASTLIST__V4*/ meltfptr[3]);
                                        melt_magic_discr((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]) == MELTOBMAG_PAIR;
                                        /*_.REVLASTPAIR__V13*/ meltfptr[12] = melt_pair_tail((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]))
                {
                    /*_.REVLASTPROC__V14*/ meltfptr[13] = melt_pair_head((melt_ptr_t) /*_.REVLASTPAIR__V13*/ meltfptr[12]);



                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1540:/ apply");
                    /*apply*/
                    {
                        /*_.REVLASTPROC__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)(/*_.REVLASTPROC__V14*/ meltfptr[13]), (melt_ptr_t)(/*_.RES__V5*/ meltfptr[4]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit3__EACHLIST */
            /*_.REVLASTPAIR__V13*/ meltfptr[12] = NULL;
            /*_.REVLASTPROC__V14*/ meltfptr[13] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-hooks.melt:1537:/ clear");
            /*clear*/ /*_.REVLASTPAIR__V13*/ meltfptr[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REVLASTPROC__V14*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REVLASTPROC__V15*/ meltfptr[14] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:1516:/ clear");
        /*clear*/ /*_.FIRSTLIST__V2*/ meltfptr[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LASTLIST__V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REVLASTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V5*/ meltfptr[4] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("RUN_ENDMELTPASS_FUNCTIONS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_56_WARMELTmiHOOKS_RUN_ENDMELTPASS_FUNCTIONS_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST fromline 1752 */

    /** start of frame for meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST), clos) {};
        MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST)) {};
        MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST


    /** end of frame for meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST fromline 1697**/

    /* end of frame for routine meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST fromline 1756 */

    /* classy proc frame meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST */ MeltFrame_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("AT_END_OF_THIS_MELT_PASS_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1545:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1547:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
            /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1549:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[1]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1550:/ compute");
                    /*_.FIRSTLIST__V3*/ meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1551:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1549:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/ meltfptr[4] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1552:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1553:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:1547:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_END_OF_THIS_MELT_PASS_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_57_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_FIRST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST fromline 1752 */

    /** start of frame for meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST// fromline 1568
        : public Melt_CallFrameWithValues<5>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST), clos) {};
        MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST)) {};
        MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST


    /** end of frame for meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST fromline 1697**/

    /* end of frame for routine meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST fromline 1756 */

    /* classy proc frame meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST */ MeltFrame_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("AT_END_OF_THIS_MELT_PASS_LAST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1556:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1559:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!ENDMELTPASS_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "DELQU_LAST");
            /*_.LASTLIST__V3*/ meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1561:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[1]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1562:/ compute");
                    /*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1563:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1561:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/ meltfptr[4] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1564:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1565:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-hooks.melt:1559:/ clear");
        /*clear*/ /*_.LASTLIST__V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_END_OF_THIS_MELT_PASS_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_58_WARMELTmiHOOKS_AT_END_OF_THIS_MELT_PASS_LAST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL fromline 1752 */

    /** start of frame for meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL// fromline 1568
        : public Melt_CallFrameWithValues<1>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL), clos) {};
        MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<1> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL)) {};
        MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<1> (fil,lin, sizeof(MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL


    /** end of frame for meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL fromline 1697**/

    /* end of frame for routine meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL fromline 1756 */

    /* classy proc frame meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL */ MeltFrame_meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_59_WARMELTmiHOOKS_ENABLE_FINISH_DECL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("ENABLE_FINISH_DECL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
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
    MELT_LOCATION("warmelt-hooks.melt:1607:/ block");
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-hooks.melt:1608:/ locexp");
            /* enable_finish_decl ENABLEPREGEN_CHK__3 */
            if (!melthk_finish_decl_registered_flag)
                {
                    melthk_finish_decl_registered_flag = true;
                    register_callback (melt_plugin_name, PLUGIN_FINISH_DECL,
                                       melt_finish_decl_hook_callback, NULL);
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST fromline 1752 */

    /** start of frame for meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST), clos) {};
        MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST)) {};
        MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST


    /** end of frame for meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST fromline 1697**/

    /* end of frame for routine meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST fromline 1756 */

    /* classy proc frame meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST */ MeltFrame_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_FINISH_DECL_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1650:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1654:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.FIRSTLIST__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.FIRSTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1656:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1658:/ compute");
                    /*_.FIRSTLIST__V4*/ meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/ meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1659:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1657:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1656:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1664:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1666:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1667:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_DECL*/ meltfrout->tabval[3])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1665:/ quasiblock");


                    /*_.PROGN___V10*/ meltfptr[6] = /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/ meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1664:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/ meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1668:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_decl_first"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/ meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1654:/ clear");
        /*clear*/ /*_.FIRSTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1650:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1650:/ locexp");
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
        /*clear*/ /*_.LET___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_DECL_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_61_WARMELTmiHOOKS_REGISTER_FINISH_DECL_FIRST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST fromline 1752 */

    /** start of frame for meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST// fromline 1568
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST), clos) {};
        MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<10> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST)) {};
        MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST


    /** end of frame for meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST fromline 1697**/

    /* end of frame for routine meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST fromline 1756 */

    /* classy proc frame meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST */ MeltFrame_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_FINISH_DECL_LAST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1673:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1677:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.LASTLIST__V4*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.LASTLIST__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1679:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V5*/ meltfptr[4] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1681:/ compute");
                    /*_.LASTLIST__V4*/ meltfptr[3] = /*_.SETQ___V6*/ meltfptr[5] = /*_.MAKE_LIST__V5*/ meltfptr[4];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V7*/ meltfptr[6] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1682:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1680:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1679:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V6*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V7*/ meltfptr[6] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1687:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1689:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1690:/ apply");
                    /*apply*/
                    {
                        /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!ENABLE_FINISH_DECL*/ meltfrout->tabval[3])), (melt_ptr_t)( /*reallynil*/ NULL ), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:1688:/ quasiblock");


                    /*_.PROGN___V10*/ meltfptr[6] = /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5];;
                    /*^compute*/
                    /*_._IFELSE___V8*/ meltfptr[4] = /*_.PROGN___V10*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1687:/ clear");
                    /*clear*/ /*_.ENABLE_FINISH_DECL__V9*/ meltfptr[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V10*/ meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-hooks.melt:1691:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_finish_decl_last"));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.LET___V3*/ meltfptr[2] = /*_._IFELSE___V8*/ meltfptr[4];;

        MELT_LOCATION("warmelt-hooks.melt:1677:/ clear");
        /*clear*/ /*_.LASTLIST__V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        MELT_LOCATION("warmelt-hooks.melt:1673:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-hooks.melt:1673:/ locexp");
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
        /*clear*/ /*_.LET___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_FINISH_DECL_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_62_WARMELTmiHOOKS_REGISTER_FINISH_DECL_LAST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST fromline 1752 */

    /** start of frame for meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST), clos) {};
        MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST)) {};
        MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST


    /** end of frame for meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST fromline 1697**/

    /* end of frame for routine meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST fromline 1756 */

    /* classy proc frame meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST */ MeltFrame_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_PASS_EXECUTION_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1805:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1809:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FIRSTLIST__V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.FIRSTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1811:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1813:/ compute");
                    /*_.FIRSTLIST__V3*/ meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V6*/ meltfptr[5] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1814:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1812:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1811:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V6*/ meltfptr[5] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1819:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1820:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.FIRSTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1819:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1821:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pass_execution_hook"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        MELT_LOCATION("warmelt-hooks.melt:1809:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PASS_EXECUTION_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_64_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_FIRST_melt;
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
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST fromline 1752 */

    /** start of frame for meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST), clos) {};
        MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST)) {};
        MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST


    /** end of frame for meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST fromline 1697**/

    /* end of frame for routine meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST fromline 1756 */

    /* classy proc frame meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST */ MeltFrame_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_PASS_EXECUTION_LAST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:1826:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:1830:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
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
                    /*_.LASTLIST__V3*/ meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.LASTLIST__V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L1*/ meltfnum[0] =
            (/*null*/(/*_.LASTLIST__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-hooks.melt:1832:/ cond");
        /*cond*/ if (/*_#NULL__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_LIST__V4*/ meltfptr[3] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1834:/ compute");
                    /*_.LASTLIST__V3*/ meltfptr[2] = /*_.SETQ___V5*/ meltfptr[4] = /*_.MAKE_LIST__V4*/ meltfptr[3];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MAKE_LIST__V6*/ meltfptr[5] =
                        (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
                    MELT_LOCATION("warmelt-hooks.melt:1835:/ cond");
                    /*cond*/ if (
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
                    MELT_LOCATION("warmelt-hooks.melt:1833:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:1832:/ clear");
                    /*clear*/ /*_.MAKE_LIST__V4*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V5*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MAKE_LIST__V6*/ meltfptr[5] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:1840:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1841:/ locexp");
                        meltgc_append_list((melt_ptr_t)(/*_.LASTLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1]));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:1840:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-hooks.melt:1842:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s", melt_dbgcounter, ( "Bad hook passed to register_pass_execution_last_hook"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        MELT_LOCATION("warmelt-hooks.melt:1830:/ clear");
        /*clear*/ /*_.LASTLIST__V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L2*/ meltfnum[1] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_PASS_EXECUTION_LAST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_65_WARMELTmiHOOKS_REGISTER_PASS_EXECUTION_LAST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE fromline 1752 */

    /** start of frame for meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE// fromline 1568
        : public Melt_CallFrameWithValues<34>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<34> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE), clos) {};
        MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<34> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE)) {};
        MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE


    /** end of frame for meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE fromline 1697**/

    /* end of frame for routine meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE fromline 1756 */

    /* classy proc frame meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE */ MeltFrame_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("REGISTER_GCC_ATTRIBUTE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:2392:/ getarg");
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
        MELT_LOCATION("warmelt-hooks.melt:2404:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-hooks.melt:2404:/ cond");
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
                        MELT_LOCATION("warmelt-hooks.melt:2404:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2404;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "register_gcc_attribute macro sexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2404:/ clear");
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
                MELT_LOCATION("warmelt-hooks.melt:2404:/ locexp");
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
        MELT_LOCATION("warmelt-hooks.melt:2405:/ quasiblock");


        MELT_LOCATION("warmelt-hooks.melt:2406:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                    /*_.SLOC__V9*/ meltfptr[5] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SLOC__V9*/ meltfptr[5] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-hooks.melt:2407:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.SEXP__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                    /*_.SEXCONT__V10*/ meltfptr[9] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SEXCONT__V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2408:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!konst_5_MELT_REGISTER_GCC_ATTRIBUTE_AT*/ meltfrout->tabval[5]);
            /*_.REGATBND__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2410:/ cond");
        /*cond*/ if (/*_.REGATBND__V11*/ meltfptr[10]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2410:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2411:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "REGISTER_GCC_ATTRIBUTE used when MELT_REGISTER_GCC_ATTRIBUTE_AT is\
 unbound";
                        /*_.ERROR_AT__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2412:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:2412:/ locexp");
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
                    MELT_LOCATION("warmelt-hooks.melt:2410:/ quasiblock");


                    /*_.PROGN___V15*/ meltfptr[14] = /*_.RETURN___V14*/ meltfptr[13];;
                    /*^compute*/
                    /*_._IFELSE___V12*/ meltfptr[11] = /*_.PROGN___V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:2410:/ clear");
                    /*clear*/ /*_.ERROR_AT__V13*/ meltfptr[12] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V14*/ meltfptr[13] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V15*/ meltfptr[14] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-hooks.melt:2413:/ quasiblock");


        /*_.LIST_FIRST__V17*/ meltfptr[13] =
            (melt_list_first((melt_ptr_t)(/*_.SEXCONT__V10*/ meltfptr[9])));;
        /*^compute*/
        /*_.PAIR_TAIL__V18*/ meltfptr[14] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V17*/ meltfptr[13])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2413:/ apply");
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
            /*_.XARGLIST__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_LIST*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.PAIR_TAIL__V18*/ meltfptr[14]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2415:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L3*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2415:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L3*/ meltfnum[1]) /*then*/
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
                        MELT_LOCATION("warmelt-hooks.melt:2415:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2415;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "register_gcc_attribute macro xarglist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XARGLIST__V19*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V20*/ meltfptr[19] = /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2415:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2415:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V20*/ meltfptr[19] = 0 ;
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
        /*_#LIST_LENGTH__L5*/ meltfnum[0] =
            (melt_list_length((melt_ptr_t)(/*_.XARGLIST__V19*/ meltfptr[18])));;
        /*^compute*/
        /*_#ltI__L6*/ meltfnum[1] =
            ((/*_#LIST_LENGTH__L5*/ meltfnum[0]) < (1));;
        MELT_LOCATION("warmelt-hooks.melt:2416:/ cond");
        /*cond*/ if (/*_#ltI__L6*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2417:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "REGISTER_GCC_ATTRIBUTE needs at least one argument";
                        /*_.ERROR_AT__V23*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2418:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:2418:/ locexp");
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
                    MELT_LOCATION("warmelt-hooks.melt:2416:/ quasiblock");


                    /*_.PROGN___V25*/ meltfptr[24] = /*_.RETURN___V24*/ meltfptr[23];;
                    /*^compute*/
                    /*_._IF___V22*/ meltfptr[20] = /*_.PROGN___V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:2416:/ clear");
                    /*clear*/ /*_.ERROR_AT__V23*/ meltfptr[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V24*/ meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V25*/ meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V22*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-hooks.melt:2419:/ locexp");
            meltgc_prepend_list((melt_ptr_t)(/*_.XARGLIST__V19*/ meltfptr[18]), (melt_ptr_t)(/*_.SLOC__V9*/ meltfptr[5]));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2420:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L7*/ meltfnum[6] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2420:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2420:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2420;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "register_gcc_attribute macro prepended xarglist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XARGLIST__V19*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V27*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V26*/ meltfptr[19] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2420:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V27*/ meltfptr[23] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V26*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2420:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V26*/ meltfptr[19] = 0 ;
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
        MELT_LOCATION("warmelt-hooks.melt:2421:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2424:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[11]);
            /*_.LIST_TO_MULTIPLE__V29*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.XARGLIST__V19*/ meltfptr[18]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-hooks.melt:2421:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_APPLY*/ meltfrout->tabval[8])), (4), "CLASS_SOURCE_APPLY");
            /*_.INST__V31*/ meltfptr[30] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (1), (/*_.SLOC__V9*/ meltfptr[5]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SAPP_FUN", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (3), ((/*!konst_5_MELT_REGISTER_GCC_ATTRIBUTE_AT*/ meltfrout->tabval[5])), "SAPP_FUN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (2), (/*_.LIST_TO_MULTIPLE__V29*/ meltfptr[23]), "SARGOP_ARGS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V31*/ meltfptr[30], "newly made instance");
        ;
        /*_.RES__V30*/ meltfptr[19] = /*_.INST__V31*/ meltfptr[30];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2426:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L9*/ meltfnum[7] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2426:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L9*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L10*/ meltfnum[6] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2426:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[6];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2426;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "register_gcc_attribute macro returns res=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V30*/ meltfptr[19];
                            /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V32*/ meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2426:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L10*/ meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2426:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L9*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V32*/ meltfptr[31] = 0 ;
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
        MELT_LOCATION("warmelt-hooks.melt:2427:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RES__V30*/ meltfptr[19];;

        {
            MELT_LOCATION("warmelt-hooks.melt:2427:/ locexp");
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
        /*_.LET___V28*/ meltfptr[24] = /*_.RETURN___V34*/ meltfptr[32];;

        MELT_LOCATION("warmelt-hooks.melt:2421:/ clear");
        /*clear*/ /*_.LIST_TO_MULTIPLE__V29*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V30*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V34*/ meltfptr[32] = 0 ;
        /*_.LET___V16*/ meltfptr[12] = /*_.LET___V28*/ meltfptr[24];;

        MELT_LOCATION("warmelt-hooks.melt:2413:/ clear");
        /*clear*/ /*_.LIST_FIRST__V17*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V18*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XARGLIST__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#LIST_LENGTH__L5*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ltI__L6*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V22*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/ meltfptr[24] = 0 ;
        /*_.LET___V8*/ meltfptr[6] = /*_.LET___V16*/ meltfptr[12];;

        MELT_LOCATION("warmelt-hooks.melt:2405:/ clear");
        /*clear*/ /*_.SLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SEXCONT__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REGATBND__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/ meltfptr[12] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2392:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-hooks.melt:2392:/ locexp");
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
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("REGISTER_GCC_ATTRIBUTE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_74_WARMELTmiHOOKS_REGISTER_GCC_ATTRIBUTE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    /*variadic*/ int variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix = 0, variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len = melt_argdescr_length (meltxargdescr_);
#define melt_variadic_length  (0+variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len)
#define melt_variadic_index variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix

    long current_blocklevel_signals_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT fromline 1752 */

    /** start of frame for meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT// fromline 1568
        : public Melt_CallFrameWithValues<87>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[71];
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
        MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<87> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT), clos) {};
        MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<87> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<87> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT)) {};
        MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<87> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT


    /** end of frame for meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT fromline 1697**/

    /* end of frame for routine meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT fromline 1756 */

    /* classy proc frame meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT */ MeltFrame_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("MELT_REGISTER_GCC_ATTRIBUTE_AT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:2432:/ getarg");
    /*_.LOC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR)   /*getargrest*/
        {
            variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix = variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len;
            goto meltlab_endgetargs;
        }
    variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix = 1;
    /*_.ATTR__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2435:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-hooks.melt:2435:/ cond");
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
                        MELT_LOCATION("warmelt-hooks.melt:2435:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2435;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_register_gcc_attribute_at";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " start loc=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " attr=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ATTR__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2435:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2435:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V4*/ meltfptr[3] = 0 ;
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
        /*_#IS_A__L3*/ meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_GCC_ATTRIBUTE*/ meltfrout->tabval[1])));;
        MELT_LOCATION("warmelt-hooks.melt:2436:/ cond");
        /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V6*/ meltfptr[4] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2436:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2437:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "register_gcc_attribute got invalid attr $1";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ATTR__V3*/ meltfptr[2];
                        /*_.ERROR_AT__V7*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2438:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:2438:/ locexp");
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
                    MELT_LOCATION("warmelt-hooks.melt:2436:/ quasiblock");


                    /*_.PROGN___V9*/ meltfptr[8] = /*_.RETURN___V8*/ meltfptr[7];;
                    /*^compute*/
                    /*_._IFELSE___V6*/ meltfptr[4] = /*_.PROGN___V9*/ meltfptr[8];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:2436:/ clear");
                    /*clear*/ /*_.ERROR_AT__V7*/ meltfptr[3] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V8*/ meltfptr[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V9*/ meltfptr[8] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#GET_INT__L4*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2])));;
        MELT_LOCATION("warmelt-hooks.melt:2440:/ cond");
        /*cond*/ if (/*_#GET_INT__L4*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L5*/ meltfnum[4] =
                        (melt_get_int((melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2441:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "register_gcc_attribute already registered #$1 attribute $2";
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#GET_INT__L5*/ meltfnum[4];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.ATTR__V3*/ meltfptr[2];
                        /*_.ERROR_AT__V11*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2444:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:2444:/ locexp");
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
                    MELT_LOCATION("warmelt-hooks.melt:2440:/ quasiblock");


                    /*_.PROGN___V13*/ meltfptr[12] = /*_.RETURN___V12*/ meltfptr[8];;
                    /*^compute*/
                    /*_._IF___V10*/ meltfptr[3] = /*_.PROGN___V13*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:2440:/ clear");
                    /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ERROR_AT__V11*/ meltfptr[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V12*/ meltfptr[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V13*/ meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.GCC_ATTRIBUTE_DICT__V14*/ meltfptr[7] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;
        /*^compute*/
        /*_#IS_MAPSTRING__L6*/ meltfnum[4] =
            (melt_magic_discr((melt_ptr_t)(/*_.GCC_ATTRIBUTE_DICT__V14*/ meltfptr[7])) == MELTOBMAG_MAPSTRINGS);;
        /*^compute*/
        /*_#NOT__L7*/ meltfnum[6] =
            (!(/*_#IS_MAPSTRING__L6*/ meltfnum[4]));;
        MELT_LOCATION("warmelt-hooks.melt:2446:/ cond");
        /*cond*/ if (/*_#NOT__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-hooks.melt:2447:/ quasiblock");


                    /*_.ATDIC__V15*/ meltfptr[8] =
                        (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[3])), (31)));;
                    MELT_LOCATION("warmelt-hooks.melt:2448:/ putmodvar");
                    melt_module_var_put (meltmodvarix_GCC_ATTRIBUTE_DICT_symb, (melt_ptr_t)(/*_.ATDIC__V15*/ meltfptr[8]));
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-hooks.melt:2449:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L8*/ meltfnum[7] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-hooks.melt:2449:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L8*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-hooks.melt:2449:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[6];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2449;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "melt_register_gcc_attribute_at";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_cstring =  " atdic=";
                                        /*^apply.arg*/
                                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.ATDIC__V15*/ meltfptr[8];
                                        /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V16*/ meltfptr[12] = /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-hooks.melt:2449:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V16*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-hooks.melt:2449:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V16*/ meltfptr[12] = 0 ;
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

                    MELT_LOCATION("warmelt-hooks.melt:2447:/ clear");
                    /*clear*/ /*_.ATDIC__V15*/ meltfptr[8] = 0 ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2451:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L10*/ meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2451:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L10*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L11*/ meltfnum[7] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;
                        /*^compute*/
                        /*_.GCC_ATTRIBUTE_DICT__V19*/ meltfptr[12] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2451:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[6];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2451;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_register_gcc_attribute_at";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " gcc_attribute_dict=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.GCC_ATTRIBUTE_DICT__V19*/ meltfptr[12];
                            /*_.MELT_DEBUG_FUN__V20*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V18*/ meltfptr[16] = /*_.MELT_DEBUG_FUN__V20*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2451:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V19*/ meltfptr[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V20*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V18*/ meltfptr[16] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2451:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L10*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V18*/ meltfptr[16] = 0 ;
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
        MELT_LOCATION("warmelt-hooks.melt:2452:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_.GCC_ATTRIBUTE_DICT__V22*/ meltfptr[8] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;
            /*^compute*/
            /*_#IS_MAPSTRING__L12*/ meltfnum[7] =
                (melt_magic_discr((melt_ptr_t)(/*_.GCC_ATTRIBUTE_DICT__V22*/ meltfptr[8])) == MELTOBMAG_MAPSTRINGS);;
            MELT_LOCATION("warmelt-hooks.melt:2452:/ cond");
            /*cond*/ if (/*_#IS_MAPSTRING__L12*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V23*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-hooks.melt:2452:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {

                        /*_.GCC_ATTRIBUTE_DICT__V24*/ meltfptr[23] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2452:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check gcc_attribute_dict";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2452;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.GCC_ATTRIBUTE_DICT__V24*/ meltfptr[23];
                            /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V23*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2452:/ clear");
                        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V24*/ meltfptr[23] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V21*/ meltfptr[12] = /*_._IFELSE___V23*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-hooks.melt:2452:/ clear");
            /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V22*/ meltfptr[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_MAPSTRING__L12*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V23*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V21*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-hooks.melt:2454:/ quasiblock");


        MELT_LOCATION("warmelt-hooks.melt:2455:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.NAME__V26*/ meltfptr[23] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NAME__V26*/ meltfptr[23] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#MIN_LENGTH__L13*/ meltfnum[8] = 0;;
        /*^compute*/
        /*_#MAX_LENGTH__L14*/ meltfnum[7] = -1;;
        /*^compute*/
        /*_#DECL_REQUIRED_FLAG__L15*/ meltfnum[14] = 0;;
        /*^compute*/
        /*_#TYPE_REQUIRED_FLAG__L16*/ meltfnum[15] = 0;;
        /*^compute*/
        /*_#FUNCTION_TYPE_REQUIRED_FLAG__L17*/ meltfnum[16] = 0;;
        /*^compute*/
        /*_#AFFECTS_TYPE_IDENTITY_FLAG__L18*/ meltfnum[17] = 0;;
        /*^compute*/
        /*_.GCC_ATTRIBUTE_DICT__V27*/ meltfptr[24] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;
        /*^compute*/
        /*_#MAPSTRING_COUNT__L19*/ meltfnum[18] =
            (melt_count_mapstrings((struct meltmapstrings_st*)(/*_.GCC_ATTRIBUTE_DICT__V27*/ meltfptr[24])));;
        /*^compute*/
        /*_#ATTIX__L20*/ meltfnum[19] =
            ((/*_#MAPSTRING_COUNT__L19*/ meltfnum[18]) + (1));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2464:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L21*/ meltfnum[20] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2464:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L21*/ meltfnum[20]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L22*/ meltfnum[21] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;
                        /*^compute*/
                        /*_.GCC_ATTRIBUTE_DICT__V29*/ meltfptr[16] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2464:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[21];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2464;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "register_gcc_attribute ";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  "gcc_attribute_dict=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.GCC_ATTRIBUTE_DICT__V29*/ meltfptr[16];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  ";\n attix=";
                            /*^apply.arg*/
                            argtab[7].meltbp_long = /*_#ATTIX__L20*/ meltfnum[19];
                            /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V28*/ meltfptr[8] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2464:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/ meltfnum[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V29*/ meltfptr[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V28*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2464:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L21*/ meltfnum[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V28*/ meltfptr[8] = 0 ;
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
        /*_#IS_STRING__L23*/ meltfnum[21] =
            (melt_magic_discr((melt_ptr_t)(/*_.NAME__V26*/ meltfptr[23])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-hooks.melt:2465:/ cond");
        /*cond*/ if (/*_#IS_STRING__L23*/ meltfnum[21]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V31*/ meltfptr[16] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2465:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2466:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "register_gcc_attribute got badly named attr $1";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ATTR__V3*/ meltfptr[2];
                        /*_.ERROR_AT__V32*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2467:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:2467:/ locexp");
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
                    MELT_LOCATION("warmelt-hooks.melt:2465:/ quasiblock");


                    /*_.PROGN___V34*/ meltfptr[33] = /*_.RETURN___V33*/ meltfptr[8];;
                    /*^compute*/
                    /*_._IFELSE___V31*/ meltfptr[16] = /*_.PROGN___V34*/ meltfptr[33];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:2465:/ clear");
                    /*clear*/ /*_.ERROR_AT__V32*/ meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V33*/ meltfptr[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V34*/ meltfptr[33] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_.GCC_ATTRIBUTE_DICT__V35*/ meltfptr[29] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;
        /*^compute*/
        /*_.MAPSTRING_GETSTR__V36*/ meltfptr[8] =
            (melt_get_mapstrings((struct meltmapstrings_st*)(/*_.GCC_ATTRIBUTE_DICT__V35*/ meltfptr[29]),
                                 melt_string_str((melt_ptr_t)(/*_.NAME__V26*/ meltfptr[23]))));;
        MELT_LOCATION("warmelt-hooks.melt:2468:/ cond");
        /*cond*/ if (/*_.MAPSTRING_GETSTR__V36*/ meltfptr[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2469:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "register_gcc_attribute got already named attr $1";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                        /*_.ERROR_AT__V38*/ meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-hooks.melt:2470:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-hooks.melt:2470:/ locexp");
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
                    MELT_LOCATION("warmelt-hooks.melt:2468:/ quasiblock");


                    /*_.PROGN___V40*/ meltfptr[39] = /*_.RETURN___V39*/ meltfptr[38];;
                    /*^compute*/
                    /*_._IF___V37*/ meltfptr[33] = /*_.PROGN___V40*/ meltfptr[39];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-hooks.melt:2468:/ clear");
                    /*clear*/ /*_.ERROR_AT__V38*/ meltfptr[37] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V39*/ meltfptr[38] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V40*/ meltfptr[39] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V37*/ meltfptr[33] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-hooks.melt:2472:/ loop");
        /*loop*/
        {
meltlabloop_ARGSLOOP_1:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-hooks.melt:2476:/ cond");
                /*cond*/ if (/*ifvariadic nomore*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix == variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^compute*/

                            /*consume variadic  !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 0;;
                            MELT_LOCATION("warmelt-hooks.melt:2477:/ quasiblock");


                            /*^compute*/
                            /*_.ARGSLOOP__V42*/ meltfptr[38] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_ARGSLOOP_1;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-hooks.melt:2476:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-hooks.melt:2478:/ cond");
                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^compute*/
                                        /*_.KEY__V43*/ meltfptr[39] =
                                            /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                        /*^compute*/

                                        /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        MELT_LOCATION("warmelt-hooks.melt:2479:/ block");
                                        /*anyblock*/
                                        {

                                            /*^objgoto*/
                                            /*objgoto*/ goto mtch1_0;
                                            ;

/*objlabel*/ mtch1_0:
                                            ;
                                            /*^objlabel*/
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_1;
                                            meltlab_count_1++;
                                            debugeprintf("objlabel_1 mtch1_*CLASS_NORMTESTER_SAME/db4ad81. *CLASS_OBJLABELINSTR/179ceeb6. %ld", meltlab_count_1);
#endif

                                            ;
                                            /*^cond*/
                                            /*cond*/ if (
                                                /*testersame*/ (/*_.KEY__V43*/ meltfptr[39] == (/*!konst_6_MIN_LENGTH*/ meltfrout->tabval[6]))) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2482:/ objgoto");
                                                        /*objgoto*/ goto mtch1_1;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-hooks.melt:2479:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^objgoto*/
                                                        /*objgoto*/ goto mtch1_2;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            ;

/*objlabel*/ mtch1_1:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2482:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_2;
                                            meltlab_count_2++;
                                            debugeprintf("objlabel_2 mtch1_*CLASS_NORMTESTER_SUCCESS/373da6b5. *CLASS_OBJLABELINSTR/3569b7e8. %ld", meltlab_count_2);
#endif

                                            ;
                                            /*^quasiblock*/


                                            MELT_LOCATION("warmelt-hooks.melt:2484:/ cond");
                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_LONG) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^compute*/
                                                        /*_#L__L24*/ meltfnum[20] =
                                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_long;;
                                                        /*^compute*/

                                                        /*consume variadic LONG !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;
                                                        MELT_LOCATION("warmelt-hooks.melt:2485:/ compute");
                                                        /*_#MIN_LENGTH__L13*/ meltfnum[8] = /*_#SETQ___L25*/ meltfnum[24] = /*_#L__L24*/ meltfnum[20];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-hooks.melt:2484:/ clear");
                                                        /*clear*/ /*_#L__L24*/ meltfnum[20] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#SETQ___L25*/ meltfnum[24] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2486:/ cond");
                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^compute*/
                                                                    /*_.VL__V44*/ meltfptr[43] =
                                                                        /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                                                    /*^compute*/

                                                                    /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_INTEGERBOX__L26*/ meltfnum[20] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.VL__V44*/ meltfptr[43])) == MELTOBMAG_INT);;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2488:/ cond");
                                                                    /*cond*/ if (/*_#IS_INTEGERBOX__L26*/ meltfnum[20]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#UNBOX__L27*/ meltfnum[24] =
                                                                                    /*unboxing*/ /*unboxing ctype_long*/ melt_get_int ((melt_ptr_t)/*_.VL__V44*/ meltfptr[43]);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2489:/ compute");
                                                                                /*_#MIN_LENGTH__L13*/ meltfnum[8] = /*_#SETQ___L28*/ meltfnum[27] = /*_#UNBOX__L27*/ meltfnum[24];;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2490:/ locexp");
                                                                                    /*void*/(void)0;
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2488:/ quasiblock");


                                                                                /*epilog*/

                                                                                /*^clear*/
                                                                                /*clear*/ /*_#UNBOX__L27*/ meltfnum[24] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L28*/ meltfnum[27] = 0 ;
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
                                                                                MELT_LOCATION("warmelt-hooks.melt:2492:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[2];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :MIN_LENGTH value";
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                                    /*_.ERROR_AT__V46*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2493:/ quasiblock");


                                                                                /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2493:/ locexp");
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
                                                                                MELT_LOCATION("warmelt-hooks.melt:2491:/ quasiblock");


                                                                                /*_.PROGN___V48*/ meltfptr[47] = /*_.RETURN___V47*/ meltfptr[46];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V45*/ meltfptr[44] = /*_.PROGN___V48*/ meltfptr[47];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-hooks.melt:2488:/ clear");
                                                                                /*clear*/ /*_.ERROR_AT__V46*/ meltfptr[45] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.RETURN___V47*/ meltfptr[46] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V48*/ meltfptr[47] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2486:/ clear");
                                                                    /*clear*/ /*_.VL__V44*/ meltfptr[43] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#IS_INTEGERBOX__L26*/ meltfnum[20] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IFELSE___V45*/ meltfptr[44] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-hooks.melt:2495:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :MIN_LENGTH";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                        /*_.ERROR_AT__V49*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2496:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                    {
                                                                        MELT_LOCATION("warmelt-hooks.melt:2496:/ locexp");
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
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2486:/ clear");
                                                                    /*clear*/ /*_.ERROR_AT__V49*/ meltfptr[45] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V50*/ meltfptr[46] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            ;

                                            MELT_LOCATION("warmelt-hooks.melt:2482:/ objgoto");
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1_2:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2479:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_3;
                                            meltlab_count_3++;
                                            debugeprintf("objlabel_3 mtch1_*CLASS_NORMTESTER_SAME/1c68a61. *CLASS_OBJLABELINSTR/2565d63f. %ld", meltlab_count_3);
#endif

                                            ;
                                            /*^cond*/
                                            /*cond*/ if (
                                                /*testersame*/ (/*_.KEY__V43*/ meltfptr[39] == (/*!konst_7_MAX_LENGTH*/ meltfrout->tabval[7]))) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2498:/ objgoto");
                                                        /*objgoto*/ goto mtch1_3;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-hooks.melt:2479:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^objgoto*/
                                                        /*objgoto*/ goto mtch1_4;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            ;

/*objlabel*/ mtch1_3:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2498:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_4;
                                            meltlab_count_4++;
                                            debugeprintf("objlabel_4 mtch1_*CLASS_NORMTESTER_SUCCESS/1648426e. *CLASS_OBJLABELINSTR/34e283be. %ld", meltlab_count_4);
#endif

                                            ;
                                            /*^quasiblock*/


                                            MELT_LOCATION("warmelt-hooks.melt:2500:/ cond");
                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_LONG) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^compute*/
                                                        /*_#L__L29*/ meltfnum[24] =
                                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_long;;
                                                        /*^compute*/

                                                        /*consume variadic LONG !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;
                                                        MELT_LOCATION("warmelt-hooks.melt:2501:/ compute");
                                                        /*_#MAX_LENGTH__L14*/ meltfnum[7] = /*_#SETQ___L30*/ meltfnum[27] = /*_#L__L29*/ meltfnum[24];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-hooks.melt:2500:/ clear");
                                                        /*clear*/ /*_#L__L29*/ meltfnum[24] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#SETQ___L30*/ meltfnum[27] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2502:/ cond");
                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^compute*/
                                                                    /*_.VL__V51*/ meltfptr[47] =
                                                                        /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                                                    /*^compute*/

                                                                    /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_INTEGERBOX__L31*/ meltfnum[20] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.VL__V51*/ meltfptr[47])) == MELTOBMAG_INT);;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2504:/ cond");
                                                                    /*cond*/ if (/*_#IS_INTEGERBOX__L31*/ meltfnum[20]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#UNBOX__L32*/ meltfnum[24] =
                                                                                    /*unboxing*/ /*unboxing ctype_long*/ melt_get_int ((melt_ptr_t)/*_.VL__V51*/ meltfptr[47]);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2505:/ compute");
                                                                                /*_#MAX_LENGTH__L14*/ meltfnum[7] = /*_#SETQ___L33*/ meltfnum[27] = /*_#UNBOX__L32*/ meltfnum[24];;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2506:/ locexp");
                                                                                    /*void*/(void)0;
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2504:/ quasiblock");


                                                                                /*epilog*/

                                                                                /*^clear*/
                                                                                /*clear*/ /*_#UNBOX__L32*/ meltfnum[24] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L33*/ meltfnum[27] = 0 ;
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
                                                                                MELT_LOCATION("warmelt-hooks.melt:2508:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[2];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :MAX_LENGTH value";
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                                    /*_.ERROR_AT__V53*/ meltfptr[44] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2509:/ quasiblock");


                                                                                /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2509:/ locexp");
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
                                                                                MELT_LOCATION("warmelt-hooks.melt:2507:/ quasiblock");


                                                                                /*_.PROGN___V55*/ meltfptr[46] = /*_.RETURN___V54*/ meltfptr[45];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V52*/ meltfptr[43] = /*_.PROGN___V55*/ meltfptr[46];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-hooks.melt:2504:/ clear");
                                                                                /*clear*/ /*_.ERROR_AT__V53*/ meltfptr[44] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.RETURN___V54*/ meltfptr[45] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V55*/ meltfptr[46] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2502:/ clear");
                                                                    /*clear*/ /*_.VL__V51*/ meltfptr[47] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#IS_INTEGERBOX__L31*/ meltfnum[20] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IFELSE___V52*/ meltfptr[43] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-hooks.melt:2511:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :MAX_LENGTH";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                        /*_.ERROR_AT__V56*/ meltfptr[44] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2512:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                    {
                                                                        MELT_LOCATION("warmelt-hooks.melt:2512:/ locexp");
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
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2502:/ clear");
                                                                    /*clear*/ /*_.ERROR_AT__V56*/ meltfptr[44] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V57*/ meltfptr[45] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            ;

                                            MELT_LOCATION("warmelt-hooks.melt:2498:/ objgoto");
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1_4:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2479:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_5;
                                            meltlab_count_5++;
                                            debugeprintf("objlabel_5 mtch1_*CLASS_NORMTESTER_SAME/37bdb73e. *CLASS_OBJLABELINSTR/34b1bac1. %ld", meltlab_count_5);
#endif

                                            ;
                                            /*^cond*/
                                            /*cond*/ if (
                                                /*testersame*/ (/*_.KEY__V43*/ meltfptr[39] == (/*!konst_8_DECL_REQUIRED*/ meltfrout->tabval[8]))) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2514:/ objgoto");
                                                        /*objgoto*/ goto mtch1_5;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-hooks.melt:2479:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^objgoto*/
                                                        /*objgoto*/ goto mtch1_6;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            ;

/*objlabel*/ mtch1_5:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2514:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_6;
                                            meltlab_count_6++;
                                            debugeprintf("objlabel_6 mtch1_*CLASS_NORMTESTER_SUCCESS/21dcdce9. *CLASS_OBJLABELINSTR/b2291f2. %ld", meltlab_count_6);
#endif

                                            ;
                                            /*^quasiblock*/


                                            MELT_LOCATION("warmelt-hooks.melt:2516:/ cond");
                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_LONG) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^compute*/
                                                        /*_#L__L34*/ meltfnum[24] =
                                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_long;;
                                                        /*^compute*/

                                                        /*consume variadic LONG !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;
                                                        MELT_LOCATION("warmelt-hooks.melt:2517:/ compute");
                                                        /*_#DECL_REQUIRED_FLAG__L15*/ meltfnum[14] = /*_#SETQ___L35*/ meltfnum[27] = /*_#L__L34*/ meltfnum[24];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-hooks.melt:2516:/ clear");
                                                        /*clear*/ /*_#L__L34*/ meltfnum[24] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#SETQ___L35*/ meltfnum[27] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2518:/ cond");
                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^compute*/
                                                                    /*_.VL__V58*/ meltfptr[46] =
                                                                        /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                                                    /*^compute*/

                                                                    /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_INTEGERBOX__L36*/ meltfnum[20] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.VL__V58*/ meltfptr[46])) == MELTOBMAG_INT);;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2520:/ cond");
                                                                    /*cond*/ if (/*_#IS_INTEGERBOX__L36*/ meltfnum[20]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#UNBOX__L38*/ meltfnum[27] =
                                                                                    /*unboxing*/ /*unboxing ctype_long*/ melt_get_int ((melt_ptr_t)/*_.VL__V58*/ meltfptr[46]);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2521:/ compute");
                                                                                /*_#DECL_REQUIRED_FLAG__L15*/ meltfnum[14] = /*_#SETQ___L39*/ meltfnum[38] = /*_#UNBOX__L38*/ meltfnum[27];;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2522:/ locexp");
                                                                                    /*void*/(void)0;
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2520:/ quasiblock");


                                                                                /*epilog*/

                                                                                /*^clear*/
                                                                                /*clear*/ /*_#UNBOX__L38*/ meltfnum[27] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L39*/ meltfnum[38] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#NONmiNULL__L40*/ meltfnum[27] =
                                                                                    (/*not-null*/(/*_.VL__V58*/ meltfptr[46]) == NULL);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2524:/ compute");
                                                                                /*_#DECL_REQUIRED_FLAG__L15*/ meltfnum[14] = /*_#SETQ___L41*/ meltfnum[38] = /*_#NONmiNULL__L40*/ meltfnum[27];;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2523:/ quasiblock");


                                                                                /*_#PROGN___L42*/ meltfnum[41] = /*_#SETQ___L41*/ meltfnum[38];;
                                                                                /*^compute*/
                                                                                /*_#_IFELSE___L37*/ meltfnum[24] = /*_#PROGN___L42*/ meltfnum[41];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-hooks.melt:2520:/ clear");
                                                                                /*clear*/ /*_#NONmiNULL__L40*/ meltfnum[27] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L41*/ meltfnum[38] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#PROGN___L42*/ meltfnum[41] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2518:/ clear");
                                                                    /*clear*/ /*_.VL__V58*/ meltfptr[46] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#IS_INTEGERBOX__L36*/ meltfnum[20] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#_IFELSE___L37*/ meltfnum[24] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-hooks.melt:2526:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :DECL_REQUIRED";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                        /*_.ERROR_AT__V59*/ meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2527:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                    {
                                                                        MELT_LOCATION("warmelt-hooks.melt:2527:/ locexp");
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
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2518:/ clear");
                                                                    /*clear*/ /*_.ERROR_AT__V59*/ meltfptr[47] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V60*/ meltfptr[43] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            ;

                                            MELT_LOCATION("warmelt-hooks.melt:2514:/ objgoto");
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1_6:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2479:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_7;
                                            meltlab_count_7++;
                                            debugeprintf("objlabel_7 mtch1_*CLASS_NORMTESTER_SAME/258afe0f. *CLASS_OBJLABELINSTR/32b4295c. %ld", meltlab_count_7);
#endif

                                            ;
                                            /*^cond*/
                                            /*cond*/ if (
                                                /*testersame*/ (/*_.KEY__V43*/ meltfptr[39] == (/*!konst_9_TYPE_REQUIRED*/ meltfrout->tabval[9]))) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2529:/ objgoto");
                                                        /*objgoto*/ goto mtch1_7;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-hooks.melt:2479:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^objgoto*/
                                                        /*objgoto*/ goto mtch1_8;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            ;

/*objlabel*/ mtch1_7:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2529:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_8;
                                            meltlab_count_8++;
                                            debugeprintf("objlabel_8 mtch1_*CLASS_NORMTESTER_SUCCESS/2de482fa. *CLASS_OBJLABELINSTR/2b0faeef. %ld", meltlab_count_8);
#endif

                                            ;
                                            /*^quasiblock*/


                                            MELT_LOCATION("warmelt-hooks.melt:2531:/ cond");
                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_LONG) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^compute*/
                                                        /*_#L__L43*/ meltfnum[27] =
                                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_long;;
                                                        /*^compute*/

                                                        /*consume variadic LONG !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;
                                                        MELT_LOCATION("warmelt-hooks.melt:2532:/ compute");
                                                        /*_#TYPE_REQUIRED_FLAG__L16*/ meltfnum[15] = /*_#SETQ___L44*/ meltfnum[38] = /*_#L__L43*/ meltfnum[27];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-hooks.melt:2531:/ clear");
                                                        /*clear*/ /*_#L__L43*/ meltfnum[27] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#SETQ___L44*/ meltfnum[38] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2533:/ cond");
                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^compute*/
                                                                    /*_.VL__V61*/ meltfptr[44] =
                                                                        /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                                                    /*^compute*/

                                                                    /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_INTEGERBOX__L45*/ meltfnum[41] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.VL__V61*/ meltfptr[44])) == MELTOBMAG_INT);;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2535:/ cond");
                                                                    /*cond*/ if (/*_#IS_INTEGERBOX__L45*/ meltfnum[41]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#UNBOX__L47*/ meltfnum[24] =
                                                                                    /*unboxing*/ /*unboxing ctype_long*/ melt_get_int ((melt_ptr_t)/*_.VL__V61*/ meltfptr[44]);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2536:/ compute");
                                                                                /*_#TYPE_REQUIRED_FLAG__L16*/ meltfnum[15] = /*_#SETQ___L48*/ meltfnum[27] = /*_#UNBOX__L47*/ meltfnum[24];;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2537:/ locexp");
                                                                                    /*void*/(void)0;
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2535:/ quasiblock");


                                                                                /*epilog*/

                                                                                /*^clear*/
                                                                                /*clear*/ /*_#UNBOX__L47*/ meltfnum[24] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L48*/ meltfnum[27] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#NONmiNULL__L49*/ meltfnum[38] =
                                                                                    (/*not-null*/(/*_.VL__V61*/ meltfptr[44]) == NULL);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2539:/ compute");
                                                                                /*_#TYPE_REQUIRED_FLAG__L16*/ meltfnum[15] = /*_#SETQ___L50*/ meltfnum[24] = /*_#NONmiNULL__L49*/ meltfnum[38];;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2538:/ quasiblock");


                                                                                /*_#PROGN___L51*/ meltfnum[27] = /*_#SETQ___L50*/ meltfnum[24];;
                                                                                /*^compute*/
                                                                                /*_#_IFELSE___L46*/ meltfnum[20] = /*_#PROGN___L51*/ meltfnum[27];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-hooks.melt:2535:/ clear");
                                                                                /*clear*/ /*_#NONmiNULL__L49*/ meltfnum[38] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L50*/ meltfnum[24] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#PROGN___L51*/ meltfnum[27] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2533:/ clear");
                                                                    /*clear*/ /*_.VL__V61*/ meltfptr[44] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#IS_INTEGERBOX__L45*/ meltfnum[41] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#_IFELSE___L46*/ meltfnum[20] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-hooks.melt:2541:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :TYPE_REQUIRED";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                        /*_.ERROR_AT__V62*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2542:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                    {
                                                                        MELT_LOCATION("warmelt-hooks.melt:2542:/ locexp");
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
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2533:/ clear");
                                                                    /*clear*/ /*_.ERROR_AT__V62*/ meltfptr[45] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V63*/ meltfptr[46] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            ;

                                            MELT_LOCATION("warmelt-hooks.melt:2529:/ objgoto");
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1_8:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2479:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_9;
                                            meltlab_count_9++;
                                            debugeprintf("objlabel_9 mtch1_*CLASS_NORMTESTER_SAME/d63b263. *CLASS_OBJLABELINSTR/3bbaf5b6. %ld", meltlab_count_9);
#endif

                                            ;
                                            /*^cond*/
                                            /*cond*/ if (
                                                /*testersame*/ (/*_.KEY__V43*/ meltfptr[39] == (/*!konst_10_FUNCTION_TYPE_REQUIRED*/ meltfrout->tabval[10]))) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2544:/ objgoto");
                                                        /*objgoto*/ goto mtch1_9;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-hooks.melt:2479:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^objgoto*/
                                                        /*objgoto*/ goto mtch1_10;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            ;

/*objlabel*/ mtch1_9:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2544:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_10;
                                            meltlab_count_10++;
                                            debugeprintf("objlabel_10 mtch1_*CLASS_NORMTESTER_SUCCESS/5ed6b30. *CLASS_OBJLABELINSTR/339e63c2. %ld", meltlab_count_10);
#endif

                                            ;
                                            /*^quasiblock*/


                                            MELT_LOCATION("warmelt-hooks.melt:2546:/ cond");
                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_LONG) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^compute*/
                                                        /*_#L__L52*/ meltfnum[38] =
                                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_long;;
                                                        /*^compute*/

                                                        /*consume variadic LONG !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;
                                                        MELT_LOCATION("warmelt-hooks.melt:2547:/ compute");
                                                        /*_#FUNCTION_TYPE_REQUIRED_FLAG__L17*/ meltfnum[16] = /*_#SETQ___L53*/ meltfnum[24] = /*_#L__L52*/ meltfnum[38];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-hooks.melt:2546:/ clear");
                                                        /*clear*/ /*_#L__L52*/ meltfnum[38] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#SETQ___L53*/ meltfnum[24] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2548:/ cond");
                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^compute*/
                                                                    /*_.VL__V64*/ meltfptr[47] =
                                                                        /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                                                    /*^compute*/

                                                                    /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_INTEGERBOX__L54*/ meltfnum[27] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.VL__V64*/ meltfptr[47])) == MELTOBMAG_INT);;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2550:/ cond");
                                                                    /*cond*/ if (/*_#IS_INTEGERBOX__L54*/ meltfnum[27]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#UNBOX__L56*/ meltfnum[20] =
                                                                                    /*unboxing*/ /*unboxing ctype_long*/ melt_get_int ((melt_ptr_t)/*_.VL__V64*/ meltfptr[47]);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2551:/ compute");
                                                                                /*_#FUNCTION_TYPE_REQUIRED_FLAG__L17*/ meltfnum[16] = /*_#SETQ___L57*/ meltfnum[38] = /*_#UNBOX__L56*/ meltfnum[20];;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2552:/ locexp");
                                                                                    /*void*/(void)0;
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2550:/ quasiblock");


                                                                                /*epilog*/

                                                                                /*^clear*/
                                                                                /*clear*/ /*_#UNBOX__L56*/ meltfnum[20] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L57*/ meltfnum[38] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#NONmiNULL__L58*/ meltfnum[24] =
                                                                                    (/*not-null*/(/*_.VL__V64*/ meltfptr[47]) == NULL);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2555:/ compute");
                                                                                /*_#FUNCTION_TYPE_REQUIRED_FLAG__L17*/ meltfnum[16] = /*_#SETQ___L59*/ meltfnum[20] = /*_#NONmiNULL__L58*/ meltfnum[24];;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2554:/ quasiblock");


                                                                                /*_#PROGN___L60*/ meltfnum[38] = /*_#SETQ___L59*/ meltfnum[20];;
                                                                                /*^compute*/
                                                                                /*_#_IFELSE___L55*/ meltfnum[41] = /*_#PROGN___L60*/ meltfnum[38];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-hooks.melt:2550:/ clear");
                                                                                /*clear*/ /*_#NONmiNULL__L58*/ meltfnum[24] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L59*/ meltfnum[20] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#PROGN___L60*/ meltfnum[38] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2548:/ clear");
                                                                    /*clear*/ /*_.VL__V64*/ meltfptr[47] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#IS_INTEGERBOX__L54*/ meltfnum[27] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#_IFELSE___L55*/ meltfnum[41] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-hooks.melt:2557:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :FUNCTION_TYPE_REQUIRED";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                        /*_.ERROR_AT__V65*/ meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2558:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                    {
                                                                        MELT_LOCATION("warmelt-hooks.melt:2558:/ locexp");
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
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2548:/ clear");
                                                                    /*clear*/ /*_.ERROR_AT__V65*/ meltfptr[43] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V66*/ meltfptr[44] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            ;

                                            MELT_LOCATION("warmelt-hooks.melt:2544:/ objgoto");
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1_10:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2479:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_11;
                                            meltlab_count_11++;
                                            debugeprintf("objlabel_11 mtch1_*CLASS_NORMTESTER_SAME/1eed5982. *CLASS_OBJLABELINSTR/1ccd05d6. %ld", meltlab_count_11);
#endif

                                            ;
                                            /*^cond*/
                                            /*cond*/ if (
                                                /*testersame*/ (/*_.KEY__V43*/ meltfptr[39] == (/*!konst_11_AFFECTS_TYPE_IDENTITY*/ meltfrout->tabval[11]))) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2560:/ objgoto");
                                                        /*objgoto*/ goto mtch1_11;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-hooks.melt:2479:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2576:/ objgoto");
                                                        /*objgoto*/ goto mtch1_12;
                                                        ;
                                                    }
                                                    ;
                                                }
                                            ;

/*objlabel*/ mtch1_11:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2560:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_12;
                                            meltlab_count_12++;
                                            debugeprintf("objlabel_12 mtch1_*CLASS_NORMTESTER_SUCCESS/5f35ed7. *CLASS_OBJLABELINSTR/f970411. %ld", meltlab_count_12);
#endif

                                            ;
                                            /*^quasiblock*/


                                            MELT_LOCATION("warmelt-hooks.melt:2562:/ cond");
                                            /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_LONG) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^compute*/
                                                        /*_#L__L61*/ meltfnum[24] =
                                                            /*variadic argument stuff*/ meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_long;;
                                                        /*^compute*/

                                                        /*consume variadic LONG !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;
                                                        MELT_LOCATION("warmelt-hooks.melt:2563:/ compute");
                                                        /*_#AFFECTS_TYPE_IDENTITY_FLAG__L18*/ meltfnum[17] = /*_#SETQ___L62*/ meltfnum[20] = /*_#L__L61*/ meltfnum[24];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-hooks.melt:2562:/ clear");
                                                        /*clear*/ /*_#L__L61*/ meltfnum[24] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#SETQ___L62*/ meltfnum[20] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-hooks.melt:2564:/ cond");
                                                        /*cond*/ if (/*ifvariadic arg#1*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix>=0 && variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 1 <= variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_len && meltxargdescr_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix]== MELTBPAR_PTR) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^compute*/
                                                                    /*_.VL__V67*/ meltfptr[45] =
                                                                        /*variadic argument value*/ ((meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr) ? (*(meltxargtab_[variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix + 0].meltbp_aptr)) : NULL);;
                                                                    /*^compute*/

                                                                    /*consume variadic Value !*/ variad_MELT_REGISTER_GCC_ATTRIBUTE_AT_ix += 1;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_INTEGERBOX__L63*/ meltfnum[38] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.VL__V67*/ meltfptr[45])) == MELTOBMAG_INT);;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2566:/ cond");
                                                                    /*cond*/ if (/*_#IS_INTEGERBOX__L63*/ meltfnum[38]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#UNBOX__L65*/ meltfnum[41] =
                                                                                    /*unboxing*/ /*unboxing ctype_long*/ melt_get_int ((melt_ptr_t)/*_.VL__V67*/ meltfptr[45]);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2567:/ compute");
                                                                                /*_#AFFECTS_TYPE_IDENTITY_FLAG__L18*/ meltfnum[17] = /*_#SETQ___L66*/ meltfnum[24] = /*_#UNBOX__L65*/ meltfnum[41];;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-hooks.melt:2568:/ locexp");
                                                                                    /*void*/(void)0;
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2566:/ quasiblock");


                                                                                /*epilog*/

                                                                                /*^clear*/
                                                                                /*clear*/ /*_#UNBOX__L65*/ meltfnum[41] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L66*/ meltfnum[24] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#NONmiNULL__L67*/ meltfnum[20] =
                                                                                    (/*not-null*/(/*_.VL__V67*/ meltfptr[45]) == NULL);;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2571:/ compute");
                                                                                /*_#AFFECTS_TYPE_IDENTITY_FLAG__L18*/ meltfnum[17] = /*_#SETQ___L68*/ meltfnum[41] = /*_#NONmiNULL__L67*/ meltfnum[20];;
                                                                                MELT_LOCATION("warmelt-hooks.melt:2570:/ quasiblock");


                                                                                /*_#PROGN___L69*/ meltfnum[24] = /*_#SETQ___L68*/ meltfnum[41];;
                                                                                /*^compute*/
                                                                                /*_#_IFELSE___L64*/ meltfnum[27] = /*_#PROGN___L69*/ meltfnum[24];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-hooks.melt:2566:/ clear");
                                                                                /*clear*/ /*_#NONmiNULL__L67*/ meltfnum[20] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#SETQ___L68*/ meltfnum[41] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#PROGN___L69*/ meltfnum[24] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2564:/ clear");
                                                                    /*clear*/ /*_.VL__V67*/ meltfptr[45] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#IS_INTEGERBOX__L63*/ meltfnum[38] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#_IFELSE___L64*/ meltfnum[27] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-hooks.melt:2573:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got bad :AFFECTS_TYPE_IDENTITY";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                                        /*_.ERROR_AT__V68*/ meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-hooks.melt:2574:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                    {
                                                                        MELT_LOCATION("warmelt-hooks.melt:2574:/ locexp");
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
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-hooks.melt:2564:/ clear");
                                                                    /*clear*/ /*_.ERROR_AT__V68*/ meltfptr[46] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V69*/ meltfptr[47] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            ;

                                            MELT_LOCATION("warmelt-hooks.melt:2560:/ objgoto");
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1_12:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2576:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_13;
                                            meltlab_count_13++;
                                            debugeprintf("objlabel_13 mtch1_*CLASS_NORMTESTER_SUCCESS/9aa2af9. *CLASS_OBJLABELINSTR/25a9cee. %ld", meltlab_count_13);
#endif

                                            ;
                                            /*^quasiblock*/



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2577:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_cstring =  "register_gcc_attribute for attr $1 got unexpected key $2";
                                                /*^apply.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                                /*^apply.arg*/
                                                argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.KEY__V43*/ meltfptr[39];
                                                /*_.ERROR_AT__V70*/ meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2578:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                            {
                                                MELT_LOCATION("warmelt-hooks.melt:2578:/ locexp");
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

                                            MELT_LOCATION("warmelt-hooks.melt:2576:/ clear");
                                            /*clear*/ /*_.ERROR_AT__V70*/ meltfptr[43] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.RETURN___V71*/ meltfptr[44] = 0 ;
                                            /*^objgoto*/
                                            /*objgoto*/ goto mtch1__end /*endmatch*/;
                                            ;

/*objlabel*/ mtch1__end:
                                            ;
                                            MELT_LOCATION("warmelt-hooks.melt:2479:/ objlabel");
                                            /* objlabel */
#if MELTDEBUG_MATCHING
                                            static long meltlab_count_14;
                                            meltlab_count_14++;
                                            debugeprintf("objlabel_14 mtch1__end_h791142281 *CLASS_OBJLABELINSTR/2f27df89. %ld", meltlab_count_14);
#endif

                                            ;
                                        }
                                        ;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-hooks.melt:2478:/ clear");
                                        /*clear*/ /*_.KEY__V43*/ meltfptr[39] = 0 ;
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
                                        MELT_LOCATION("warmelt-hooks.melt:2582:/ apply");
                                        /*apply*/
                                        {
                                            union meltparam_un argtab[2];
                                            memset(&argtab, 0, sizeof(argtab));
                                            /*^apply.arg*/
                                            argtab[0].meltbp_cstring =  "register_gcc_attribute for $1 got invalid key";
                                            /*^apply.arg*/
                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAME__V26*/ meltfptr[23];
                                            /*_.ERROR_AT__V72*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.LOC__V2*/ meltfptr[1]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                        }
                                        ;
                                        MELT_LOCATION("warmelt-hooks.melt:2583:/ quasiblock");


                                        /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                        {
                                            MELT_LOCATION("warmelt-hooks.melt:2583:/ locexp");
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
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-hooks.melt:2478:/ clear");
                                        /*clear*/ /*_.ERROR_AT__V72*/ meltfptr[45] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.RETURN___V73*/ meltfptr[46] = 0 ;
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
            goto meltlabloop_ARGSLOOP_1;
meltlabexit_ARGSLOOP_1:
            ;
            MELT_LOCATION("warmelt-hooks.melt:2472:/ loopepilog");
            /*loopepilog*/
            /*_.FOREVER___V41*/ meltfptr[37] = /*_.ARGSLOOP__V42*/ meltfptr[38];;
        }
        ;

        {
            MELT_LOCATION("warmelt-hooks.melt:2586:/ locexp");
            melt_put_int((melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2]), (/*_#ATTIX__L20*/ meltfnum[19]));
        }
        ;
        /*_.GCC_ATTRIBUTE_INSTALLER_LIST__V74*/ meltfptr[47] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_INSTALLER_LIST_symb));;
        MELT_LOCATION("warmelt-hooks.melt:2591:/ quasiblock");


        /*_.VMINLENGTH__V76*/ meltfptr[44] =
            /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#MIN_LENGTH__L13*/ meltfnum[8]));;
        /*^compute*/
        /*_.VMAXLENGTH__V77*/ meltfptr[39] =
            /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#MAX_LENGTH__L14*/ meltfnum[7]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2593:/ cond");
        /*cond*/ if (/*_#DECL_REQUIRED_FLAG__L15*/ meltfnum[14]) /*then*/
            {
                /*^cond.then*/
                /*_.VDECLREQUIRED__V78*/ meltfptr[45] = (/*!konst_12_TRUE*/ meltfrout->tabval[12]);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2593:/ cond.else");

                /*_.VDECLREQUIRED__V78*/ meltfptr[45] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2594:/ cond");
        /*cond*/ if (/*_#TYPE_REQUIRED_FLAG__L16*/ meltfnum[15]) /*then*/
            {
                /*^cond.then*/
                /*_.VTYPEREQUIRED__V79*/ meltfptr[46] = (/*!konst_12_TRUE*/ meltfrout->tabval[12]);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2594:/ cond.else");

                /*_.VTYPEREQUIRED__V79*/ meltfptr[46] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2595:/ cond");
        /*cond*/ if (/*_#FUNCTION_TYPE_REQUIRED_FLAG__L17*/ meltfnum[16]) /*then*/
            {
                /*^cond.then*/
                /*_.VFUNCTIONTYPEREQUIRED__V80*/ meltfptr[79] = (/*!konst_12_TRUE*/ meltfrout->tabval[12]);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2595:/ cond.else");

                /*_.VFUNCTIONTYPEREQUIRED__V80*/ meltfptr[79] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-hooks.melt:2596:/ cond");
        /*cond*/ if (/*_#AFFECTS_TYPE_IDENTITY_FLAG__L18*/ meltfnum[17]) /*then*/
            {
                /*^cond.then*/
                /*_.VAFFECTSTYPEIDENTITY__V81*/ meltfptr[80] = (/*!konst_12_TRUE*/ meltfrout->tabval[12]);;
            }
        else
            {
                MELT_LOCATION("warmelt-hooks.melt:2596:/ cond.else");

                /*_.VAFFECTSTYPEIDENTITY__V81*/ meltfptr[80] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-hooks.melt:2598:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V83*/ meltfptr[82] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_14*/ meltfrout->tabval[14])), (8));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[0] = (melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[1] = (melt_ptr_t)(/*_.VMINLENGTH__V76*/ meltfptr[44]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[2] = (melt_ptr_t)(/*_.VMAXLENGTH__V77*/ meltfptr[39]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 3>= 0 && 3< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[3] = (melt_ptr_t)(/*_.NAME__V26*/ meltfptr[23]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 4>= 0 && 4< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[4] = (melt_ptr_t)(/*_.VDECLREQUIRED__V78*/ meltfptr[45]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 5>= 0 && 5< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[5] = (melt_ptr_t)(/*_.VTYPEREQUIRED__V79*/ meltfptr[46]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 6>= 0 && 6< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[6] = (melt_ptr_t)(/*_.VFUNCTIONTYPEREQUIRED__V80*/ meltfptr[79]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V83*/ meltfptr[82])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 7>= 0 && 7< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V83*/ meltfptr[82])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V83*/ meltfptr[82])->tabval[7] = (melt_ptr_t)(/*_.VAFFECTSTYPEIDENTITY__V81*/ meltfptr[80]);
        ;
        /*_.LAMBDA___V82*/ meltfptr[81] = /*_.LAMBDA___V83*/ meltfptr[82];;
        /*^compute*/
        /*_.LET___V75*/ meltfptr[43] = /*_.LAMBDA___V82*/ meltfptr[81];;

        MELT_LOCATION("warmelt-hooks.melt:2591:/ clear");
        /*clear*/ /*_.VMINLENGTH__V76*/ meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VMAXLENGTH__V77*/ meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VDECLREQUIRED__V78*/ meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VTYPEREQUIRED__V79*/ meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VFUNCTIONTYPEREQUIRED__V80*/ meltfptr[79] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VAFFECTSTYPEIDENTITY__V81*/ meltfptr[80] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V82*/ meltfptr[81] = 0 ;

        {
            MELT_LOCATION("warmelt-hooks.melt:2588:/ locexp");
            meltgc_append_list((melt_ptr_t)(/*_.GCC_ATTRIBUTE_INSTALLER_LIST__V74*/ meltfptr[47]), (melt_ptr_t)(/*_.LET___V75*/ meltfptr[43]));
        }
        ;
        /*_.GCC_ATTRIBUTE_DICT__V84*/ meltfptr[44] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;

        {
            MELT_LOCATION("warmelt-hooks.melt:2645:/ locexp");
            meltgc_put_mapstrings((struct meltmapstrings_st *) (/*_.GCC_ATTRIBUTE_DICT__V84*/ meltfptr[44]),
                                  melt_string_str((melt_ptr_t)(/*_.NAME__V26*/ meltfptr[23])),
                                  (melt_ptr_t)(/*_.ATTR__V3*/ meltfptr[2]));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2646:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L70*/ meltfnum[20] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2646:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L70*/ meltfnum[20]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L71*/ meltfnum[41] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;
                        /*^compute*/
                        /*_.GCC_ATTRIBUTE_DICT__V86*/ meltfptr[45] =  (melt_module_var_fetch (meltmodvarix_GCC_ATTRIBUTE_DICT_symb));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2646:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L71*/ meltfnum[41];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2646;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_register_gcc_attribute_at end attr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ATTR__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  ";\n gcc_attribute_dict=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.GCC_ATTRIBUTE_DICT__V86*/ meltfptr[45];
                            /*_.MELT_DEBUG_FUN__V87*/ meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V85*/ meltfptr[39] = /*_.MELT_DEBUG_FUN__V87*/ meltfptr[46];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2646:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L71*/ meltfnum[41] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V86*/ meltfptr[45] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V87*/ meltfptr[46] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V85*/ meltfptr[39] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2646:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L70*/ meltfnum[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V85*/ meltfptr[39] = 0 ;
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

        MELT_LOCATION("warmelt-hooks.melt:2454:/ clear");
        /*clear*/ /*_.NAME__V26*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MIN_LENGTH__L13*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MAX_LENGTH__L14*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DECL_REQUIRED_FLAG__L15*/ meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#TYPE_REQUIRED_FLAG__L16*/ meltfnum[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#FUNCTION_TYPE_REQUIRED_FLAG__L17*/ meltfnum[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#AFFECTS_TYPE_IDENTITY_FLAG__L18*/ meltfnum[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V27*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MAPSTRING_COUNT__L19*/ meltfnum[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ATTIX__L20*/ meltfnum[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L23*/ meltfnum[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V31*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V35*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAPSTRING_GETSTR__V36*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V37*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V41*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GCC_ATTRIBUTE_INSTALLER_LIST__V74*/ meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V75*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V84*/ meltfptr[44] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-hooks.melt:2432:/ clear");
        /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L4*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GCC_ATTRIBUTE_DICT__V14*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MAPSTRING__L6*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V21*/ meltfptr[12] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MELT_REGISTER_GCC_ATTRIBUTE_AT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef melt_variadic_length
#undef melt_variadic_index

#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_75_WARMELTmiHOOKS_MELT_REGISTER_GCC_ATTRIBUTE_AT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7 fromline 1752 */

    /** start of frame for meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7// fromline 1568
        : public Melt_CallFrameWithValues<6>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7), clos) {};
        MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7)) {};
        MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7


    /** end of frame for meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7 fromline 1697**/

    /* end of frame for routine meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7 fromline 1756 */

    /* classy proc frame meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7 */ MeltFrame_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl7", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
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
    MELT_LOCATION("warmelt-hooks.melt:2598:/ block");
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2599:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-hooks.melt:2599:/ cond");
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
                        MELT_LOCATION("warmelt-hooks.melt:2599:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2599;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_register_gcc_attribute_at/lambda attr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*~ATTR*/ meltfclos->tabval[0]);
                            /*_.MELT_DEBUG_FUN__V3*/ meltfptr[2] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V2*/ meltfptr[1] = /*_.MELT_DEBUG_FUN__V3*/ meltfptr[2];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2599:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V3*/ meltfptr[2] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V2*/ meltfptr[1] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2599:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V2*/ meltfptr[1] = 0 ;
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
        MELT_LOCATION("warmelt-hooks.melt:2600:/ quasiblock");


        /*_#IMINLENGTH__L3*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)((/*~VMINLENGTH*/ meltfclos->tabval[1]))));;
        /*^compute*/
        /*_#IMAXLENGTH__L4*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)((/*~VMAXLENGTH*/ meltfclos->tabval[2]))));;

        {
            MELT_LOCATION("warmelt-hooks.melt:2604:/ locexp");
            /* ++ melt_register_gcc_attribute_at/lambda GCCATTR_CHK__1 start */
            struct attribute_spec *atspec
        /* will never be freed! */
            = (struct attribute_spec*)xmalloc(sizeof(struct attribute_spec)) ;
            if (!atspec)
                melt_fatal_error("failed to allocate attribute_spec in melt_register_gcc_attribute_at (%s)",
                                 xstrerror(errno)) ;
            memset (atspec, 0, sizeof(struct attribute_spec)) ;
            atspec->name = melt_intern_cstring (melt_string_str ((/*~NAME*/ meltfclos->tabval[3]))) ;
            atspec->min_length = (int) /*_#IMINLENGTH__L3*/ meltfnum[1] ;
            atspec->max_length = (int) /*_#IMAXLENGTH__L4*/ meltfnum[0] ;
            atspec->decl_required = (/*~VDECLREQUIRED*/ meltfclos->tabval[4]) != NULL ;
            atspec->type_required = (/*~VTYPEREQUIRED*/ meltfclos->tabval[5]) != NULL ;
            atspec->function_type_required = (/*~VFUNCTIONTYPEREQUIRED*/ meltfclos->tabval[6]) != NULL ;
            atspec->affects_type_identity = (/*~VAFFECTSTYPEIDENTITY*/ meltfclos->tabval[7]) != NULL ;
            atspec->handler = melt_attribute_handler_glue ;
            debugeprintf("melt_attribute_handler_glue/lambda atspec@%p "
                         "name %s min_length %d, max_length %d",
                         (void*)atspec,
                         atspec->name, atspec->min_length, atspec->max_length);
            debugeprintf("decl_required %s, type_required %s,\n.. "
                         "function_type_required %s, affects_type_identity %s,\n.. "
                         "handler@%p",
                         (atspec->decl_required)?"yes":"no",
                         (atspec->type_required)?"yes":"no",
                         (atspec->function_type_required)?"yes":"no",
                         (atspec->affects_type_identity)?"yes":"no",
                         (void*)atspec->handler);
            register_attribute (atspec) ;
            debugeprintf ("melt_register_gcc_attribute_at/lambda registered atspec=%p",
                          (void*)atspec);
            /* -- melt_register_gcc_attribute_at/lambda GCCATTR_CHK__1 end */
            ;
        }
        ;

        MELT_LOCATION("warmelt-hooks.melt:2600:/ clear");
        /*clear*/ /*_#IMINLENGTH__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IMAXLENGTH__L4*/ meltfnum[0] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-hooks.melt:2640:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L5*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-hooks.melt:2640:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L6*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-hooks.melt:2640:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[6];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-hooks.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2640;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_register_gcc_attribute_at/lambda";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " end, attr=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &(/*~ATTR*/ meltfclos->tabval[0]);
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[1] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[2] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[1];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-hooks.melt:2640:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V5*/ meltfptr[1] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V4*/ meltfptr[2] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-hooks.melt:2640:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V4*/ meltfptr[2] = 0 ;
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
        MELT_LOCATION("warmelt-hooks.melt:2641:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

        {
            MELT_LOCATION("warmelt-hooks.melt:2641:/ locexp");
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
        MELT_LOCATION("warmelt-hooks.melt:2598:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RETURN___V6*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-hooks.melt:2598:/ locexp");
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
        /*clear*/ /*_.RETURN___V6*/ meltfptr[1] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl7", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_76_WARMELTmiHOOKS_LAMBDA_cl7*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST fromline 1752 */

    /** start of frame for meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST// fromline 1568
        : public Melt_CallFrameWithValues<3>
    {
    public: /* fromline 1572*/
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
        MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<3> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST), clos) {};
        MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<3> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST)) {};
        MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST


    /** end of frame for meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST fromline 1697**/

    /* end of frame for routine meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST fromline 1756 */

    /* classy proc frame meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST */ MeltFrame_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("AT_EXIT_FIRST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-hooks.melt:2689:/ getarg");
    /*_.FUN__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-hooks.melt:2692:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*!FINAL_EXIT_DELAYED_QUEUE*/ meltfrout->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "DELQU_FIRST");
            /*_.FIRSTLIST__V3*/ meltfptr[2] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_CLOSURE__L1*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.FUN__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
        MELT_LOCATION("warmelt-hooks.melt:2694:/ cond");
        /*cond*/ if (/*_#IS_CLOSURE__L1*/ meltfnum[0]) /*then*/
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

        MELT_LOCATION("warmelt-hooks.melt:2692:/ clear");
        /*clear*/ /*_.FIRSTLIST__V3*/ meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_CLOSURE__L1*/ meltfnum[0] = 0 ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("AT_EXIT_FIRST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_78_WARMELTmiHOOKS_AT_EXIT_FIRST*/




/**** end of warmelt-hooks+02.cc ****/

