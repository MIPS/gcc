/** -*- C++ -*-
   MELT header melt-runtime.h
   [[middle end lisp translator, see http://gcc-melt.org/ for more.]]
   Copyright (C)  2008 - 2013 Free Software Foundation, Inc.
   Contributed by Basile Starynkevitch <basile@starynkevitch.net>
   and Pierre Vittet <piervit@pvittet.com>

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
along with GCC; see the file COPYING3.   If not see
<http://www.gnu.org/licenses/>.
**/

#ifndef MELT_INCLUDED_
#define MELT_INCLUDED_

#ifndef __cplusplus
#error MELT runtime requires a C++ compiler
#endif	/*__cplusplus*/

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif /*HAVE_STDINT_H*/

#define MELT_HAVE_CLASSY_FRAME 2

/* In the generated gtype-desc.c, file diagnostic.h is not included,
   so we declare these functions explicitly! */

extern void error (const char *, ...);
extern void fatal_error (const char *, ...);

#if __GNUC__ >= 4
#define MELT_MODULE_VISIBILITY  __attribute__ ((visibility ("hidden")))
#define MELT_PUBLIC_VISIBILITY  __attribute__ ((visibility ("default")))
#else /* not GCC 4*/
#define MELT_MODULE_VISIBILITY
#define MELT_PUBLIC_VISIBILITY
#endif /*GCC 4*/

#define MELT_EXTERN extern "C"

// A function attribute for weak names on the host computer
// (i.e. inside cc1plus). Ideally it should be configurable....  Don't
// confuse with TARGET_ATTRIBUTE_WEAK from defaults.h which applies to
// the target system, not the host one!
#define MELT_WEAK_ON_HOST __attribute__((weak))

/* MELTGCC_DYNAMIC_OBJSTRUCT is a cute hack to "dynamically" compute field
   positions; this is only used to compile warmelt-*-0.c files notably
   when new fields have been added in warmelt-first.melt. When
   enabled, slows down significantly MELT. */
#ifndef MELTGCC_DYNAMIC_OBJSTRUCT
#define MELTGCC_DYNAMIC_OBJSTRUCT 0
#endif /*MELTGCC_DYNAMIC_OBJSTRUCT*/

/*****
       if GGC-collected data, e.g. tree-s, edge-s, ... is computed by
       melt/MELT routines and is referenced only by the
       melt/MELT call frames, it should be carefully handled by
       full GC.

       This is done by specially generated code chunk in each MELT
       generated routines which mark such GGC data outside on full
       garbage collections. This code is invoked by calling the
       closure routine with a magic incantation, i.e. with the
       xargdescr_ set to (char*)-1

*****/

#include <vector>
#include <string>

#include "gcc-plugin.h"

/* We include toplev.h for the error routines. */
#include "toplev.h"

/* We need hwint.h for  HOST_WIDE_INT */
#include "hwint.h"

#include "tree-pass.h"

/* the MELT plugin name, useful for register_callback. */
MELT_EXTERN const char* melt_plugin_name;

MELT_EXTERN const char melt_runtime_build_date[];

/* The vector of successfully done modes names */
MELT_EXTERN std::vector<std::string> melt_done_modes_vector;

MELT_EXTERN void melt_fatal_info (const char*filename, int lineno);

#define melt_fatal_error(Fmt,...) do{ melt_fatal_info (__FILE__,__LINE__); \
    fatal_error ((Fmt),##__VA_ARGS__); }while(0)
#define melt_fatal_error_at_line(Lin,Fmt,...) do{ melt_fatal_info (__FILE__,(Lin)); \
    fatal_error ((Fmt),##__VA_ARGS__); }while(0)

#define dbgprintf_raw(Fmt,...) do{if (dump_file) \
      {fprintf(dump_file, Fmt, ##__VA_ARGS__); fflush(dump_file);}}while(0)
#define dbgprintf(Fmt,...) dbgprintf_raw("@%s:%d: " Fmt "\n", \
 lbasename(__FILE__), __LINE__, ##__VA_ARGS__)


/* the version string of GCC when MELT was initialized */
MELT_EXTERN char* melt_gccversionstr;


/* The version number of GCC, at MELT build time. So 4006 is for 4.6,
   4007 is for 4.7.  Same as MELT_GCC_VERSION constant macro.  */
MELT_EXTERN const int melt_gcc_version;
/* Points to the gcc_version from plugin-version.h */
MELT_EXTERN struct plugin_gcc_version* melt_plugin_gcc_version;

/* The GCCPLUGIN_VERSION at MELT build time, e.g. 4008 for GCC 4.8  */
MELT_EXTERN const int melt_gccplugin_version;

MELT_EXTERN int melt_count_runtime_extensions;
#define MELT_MAX_RUNTIME_EXTENSIONS 3000000

/* The version string of MELT; this is parsed by make, so spaces are
   important!  That version string is extracted by scripts or
   makefiles... */
#define MELT_VERSION_STRING "1.1-rc0"

/* return a read only version string */
extern const char* melt_version_str(void);


/* Built-in defaults */
extern const char melt_source_dir[];
extern const char melt_module_dir[];
extern const char melt_module_make_command[];
extern const char melt_module_makefile[];
extern const char melt_module_cflags[];
extern const char melt_default_modlis[];
extern const char melt_default_probe[];

/* Set to 1 iff MELT is a plugin, otherwise 0 */
extern const int melt_is_plugin;


/* the system dynamically loaded [thru dlopen] library suffix, often
   .so for ELF shared objects; it should be a constant string (so its
   sizeof is its length +1).  MELT_DYNLOADED_SUFFIX is the
   system-defined suffix.  */
#ifndef MELT_DYNLOADED_SUFFIX
#define MELT_DYNLOADED_SUFFIX ".so"
#endif /*MELT_DYNLOADED_SUFFIX */

#ifdef MELT_IS_PLUGIN

#ifndef MELT_HAVE_DEBUG
#define MELT_HAVE_DEBUG 1
#endif /*MELT_HAVE_DEBUG*/

#endif /*MELT_IS_PLUGIN*/

#if defined(ENABLE_CHECKING) && !defined(MELT_HAVE_DEBUG)
#define MELT_HAVE_DEBUG 1
#else
#ifndef MELT_HAVE_DEBUG
#define MELT_HAVE_DEBUG 0
#endif /* undef MELT_HAVE_DEBUG */
#endif /*ENABLE_CHECKING */

#ifndef MELT_HAVE_DEBUG
#define MELT_HAVE_DEBUG 0
#endif /*MELT_HAVE_DEBUG*/

extern long melt_dbgcounter;
extern long melt_debugskipcount;
extern long melt_error_counter;

extern volatile sig_atomic_t melt_signaled;
extern volatile sig_atomic_t melt_got_sigio;
extern volatile sig_atomic_t melt_got_sigalrm;
extern volatile sig_atomic_t melt_got_sigchld;

extern long melt_blocklevel_signals;

/* Returns an strdup-ed, never free-d, unique canonical string; in
   other words, if you call it twice with two equal [strcmp returning
   0] but non identical strings, you get the same identical pointer
   result. */
extern const char* melt_intern_cstring (const char* s);

static inline const char*
melt_basename(const char* path)
{
  return (path && path[0])?(lbasename (path)):path;
}

void melt_handle_signal (void);

/* the MELT translator should generate calls to melt_check_signal at safe places.  */
#define MELT_CHECK_SIGNAL() do { if (MELT_UNLIKELY(melt_signaled && melt_blocklevel_signals <= 0)) \
      melt_handle_signal(); } while(0)


/* Gives the relative real time in milliseconds since MELT started. */
long melt_relative_time_millisec (void);

/* Set the real timer alarm in milliseconds, or remove alarm if negative */
void melt_set_real_timer_millisec (long millisec);


#if ENABLE_GC_CHECKING
/* memory is poisoned by an 0xa5a5a5a5a5a5a5a5... pointer in ggc-zone.c or ggc-page.c */
#if SIZEOF_VOID_P == 8
#define MELT_POISON_POINTER (void*)0xa5a5a5a5a5a5a5a5
#elif SIZEOF_VOID_P == 4
#define MELT_POISON_POINTER (void*)0xa5a5a5a5
#else
#error cannot set MELT_POISON_POINTER
#endif
#endif /*ENABLE_GC_CHECKING*/

/* the MELT debug depth for debug_msg ... can be set with -fmelt-debug-depth= */
MELT_EXTERN int melt_debug_depth(void);

extern "C" int melt_flag_debug;
extern "C" int melt_flag_bootstrapping;




#if MELT_HAVE_DEBUG

#define debugeprintf_raw(Fmt,...) do{if (melt_flag_debug) \
      {fprintf(stderr, Fmt, ##__VA_ARGS__); fflush(stderr);}}while(0)
/* Sometimes we need to pass an explicit line number.  */
#define debugeprintfline(Lin,Fmt,...) \
   debugeprintf_raw("!@%s:%d:\n@! " Fmt "\n", \
                    melt_basename(__FILE__), Lin, ##__VA_ARGS__)
/* The usual debugging macro.  */
#define debugeprintf(Fmt,...) debugeprintfline(__LINE__,Fmt,##__VA_ARGS__)

#define debugeprintflinenonl(Lin,Fmt,...)                       \
  debugeprintf_raw("!@%s:%d:\n@! " Fmt,                         \
                   melt_basename(__FILE__), Lin, ##__VA_ARGS__)
#define debugeprintfnonl(Fmt,...) \
  debugeprintflinenonl(__LINE__, Fmt, ##__VA_ARGS__)

#define debugeprintvalue(Msg,Val) do{if (melt_flag_debug){	\
      void* __val = (Val);					\
      fprintf(stderr,"!@%s:%d:\n@! %s @%p= ",			\
              melt_basename(__FILE__), __LINE__, (Msg), __val);	\
      melt_dbgeprint(__val); }} while(0)
#define debugebacktrace(Msg,Depth)  do{if (melt_flag_debug){	\
      void* __val = (Val);					\
      fprintf(stderr,"!@%s:%d: %s **backtrace** ",		\
              melt_basename(__FILE__), __LINE__, (Msg));		\
      melt_dbgbacktrace((Depth)); }} while(0)

/* Low level debug, using the melt_debug_fun; see the
   hook_low_debug_value_at in warmelt-hooks.melt file. */
#define melt_low_debug_value(Msg,Val)\
  melt_low_debug_value_at(__FILE__,__LINE__,(Msg),(Val))

#define melt_low_debug_value_at(Fil,Lin,Msg,Val) do {   \
  static long _meltlowdebugcount_##Lin;                 \
  _meltlowdebugcount_##Lin++;                           \
    if (melt_need_debug(0))                             \
      {                                                 \
        melthookproc_HOOK_LOW_DEBUG_VALUE_AT            \
          ((Val),                                       \
           melt_basename((Fil)),  (Lin),                \
           (Msg),                                       \
           _meltlowdebugcount_##Lin);                   \
      };                                                \
  } while(0)

#else /* !MELT_HAVE_DEBUG*/

#define debugeprintf_raw(Fmt,...) do{if (0) \
      {fprintf(stderr, Fmt, ##__VA_ARGS__); fflush(stderr);}}while(0)
/* The usual debugging macro.  */
#define debugeprintf(Fmt,...) debugeprintfline(__LINE__,Fmt,##__VA_ARGS__)

#define debugeprintflinenonl(Lin,Fmt,...)                       \
  debugeprintf_raw("!@%s:%d:\n@! " Fmt,                         \
                   melt_basename(__FILE__), Lin, ##__VA_ARGS__)

#define debugeprintfline(Lin,Fmt,...) \
   debugeprintf_raw("!@%s:%d:\n@! " Fmt "\n", \
                    melt_basename(__FILE__), Lin, ##__VA_ARGS__)

#define debugeprintfnonl(Fmt,...) \
  debugeprintflinenonl(__LINE__, Fmt, ##__VA_ARGS__)

#define debugeprintvalue(Msg,Val) do{if (0){			\
      void* __val = (Val);					\
      fprintf(stderr,"!@%s:%d:\n@! %s @%p= ",			\
              melt_basename(__FILE__), __LINE__, (Msg), __val);	\
      melt_dbgeprint(__val); }} while(0)

#define debugebacktrace(Msg,Depth)  do{if (0){			\
      void* __val = (Val);					\
      fprintf(stderr,"!@%s:%d: %s **backtrace** ",		\
              melt_basename(__FILE__), __LINE__, (Msg));	\
      melt_dbgbacktrace((Depth)); }} while(0)

#define melt_low_debug_value(Msg,Val)\
  melt_low_debug_value_at(__FILE__,__LINE__,(Msg),(Val))

#define melt_low_debug_value_at(Fil,Lin,Msg,Val) \
  do {if(0) (void)(Val);}while(0)

#endif /*MELT_HAVE_DEBUG*/

/* Low level stderr output of value, using the melt_debug_fun; see the
   hook_low_stderr_value_at in warmelt-hooks.melt file. */
#define melt_low_stderr_value(Msg,Val)\
  melt_low_stderr_value_at(__FILE__,__LINE__,(Msg),(Val))

#define melt_low_stderr_value_at(Fil,Lin,Msg,Val) do {  \
  static long _meltlowstderrcount_##Lin;                \
  _meltlowstderrcount_##Lin++;                          \
  melthookproc_HOOK_LOW_STDERR_VALUE_AT                 \
    ((Val),                                             \
     melt_basename((Fil)),  (Lin),                      \
     (Msg),                                             \
     _meltlowstderrcount_##Lin);                        \
  } while(0)




/* We need melt_need_debug and melt_need_debug_limit to work even
   without MELT_HAVE_DEBUG. Otherwise dbg_out don't work as expected,
   then a user module with debuggable flavor can't use the debug
   macro from the warmelt-debug module in optimized flavor. */

/* the maximal debug depth - should be a parameter */
#define MELTDBG_MAXDEPTH 7

/* Useful, notably to be called from GDB, since melt_flag_debug might
   be a macro ... */
extern void melt_set_flag_debug (void);
extern void melt_clear_flag_debug (void);

static inline int
melt_need_debug (int depth)
{
  return
    melt_flag_debug && melt_dbgcounter>=melt_debugskipcount
    && depth >= 0 && depth < MELTDBG_MAXDEPTH;
}

static inline int
melt_need_debug_limit (int depth, int lim)
{
  return
    melt_flag_debug && melt_dbgcounter>=melt_debugskipcount
    && depth >= 0 && depth < lim;
}

/* unspecified flexible dimension in structure, we use 1 not 0 for standard compliance... */
#if (__clang__ || __GNUC__ || MELT_FORCE_FLEXIBLE_DIM) && !MELT_FORCE_INFLEXIBLE_DIM
#define MELT_FLEXIBLE_DIM /*Clang/GCC flexible*/
#define MELT_HAVE_FLEXIBLE_DIM 1
#else
#define MELT_FLEXIBLE_DIM /*C++ flexible*/1
#define MELT_HAVE_FLEXIBLE_DIM 0
#endif

/* array of (at least 100, increasing order but non consecutive)
   primes, zero terminated. Each prime is at least 1/8-th bigger than
   previous */
MELT_EXTERN const long melt_primtab[256];

/* this is generated by gengtype */
extern void gt_ggc_mx_melt_un (void*);



/* function to retrieve a MELT program -or plugin- argument; return
   NULL if not found */
const char* melt_argument(const char* argname);

/* naming convention: all struct melt*_st are inside the
   melt_un */


union meltparam_un; /* declared in file meltrunsup.h */

/* used in file meltrunsup.h */

#ifndef melt_ptr_t_TYPEDEFINED
typedef union melt_un* melt_ptr_t ;
#define melt_ptr_t_TYPEDEFINED
#endif /*melt_ptr_t_TYPEDEFINED*/

/* forward declaration, return the magic of the discriminant or 0 */
static inline int melt_magic_discr (melt_ptr_t p);


/* the maximal length of name of melt routines descriptions */
#define MELT_ROUTDESCR_LEN 100

/* the maximal length of melt hooks names */
#define MELT_HOOKNAME_LEN 80

#ifndef meltobject_ptr_t_TYPEDEFINED
typedef struct meltobject_st* meltobject_ptr_t ;
#define meltobject_ptr_t_TYPEDEFINED
#endif /*meltobject_ptr_t_TYPEDEFINED*/



struct				/* entry of string maps */
GTY (())
entrystringsmelt_st
{
  const char *GTY (()) e_at;
  melt_ptr_t e_va;
};




struct 				/* entry of object maps*/
GTY (())
entryobjectsmelt_st
{
  meltobject_ptr_t e_at;
  melt_ptr_t e_va;
};



struct
GTY (())			/* entry of long buckets */
melt_bucketlongentry_st
{
  long ebl_at;
  melt_ptr_t ebl_va;
};


struct
GTY (())			// entry of JSONobjects 
  melt_jsonobentry_st {
  melt_ptr_t jsonob_name;
  melt_ptr_t jsonob_val;
};

/* We don't want to use GCC vectors from vec.h because GCC 4.7 & 4.8
   are source incompatible....  */
struct
GTY ((variable_size))
melt_valuevector_st
{
  unsigned long vv_size;		/* allocated size */
  unsigned long vv_ulen;		/* used length, should be <= vv_size */
  melt_ptr_t GTY ((length("%h.vv_ulen"))) vv_tab[MELT_FLEXIBLE_DIM];
};

/* a union of special pointers which have to be explicitly deleted */
union melt_special_un
{
  /* all the pointers here have to be pointers to struct or to void,
     because the generated gtype-desc.c don't include all the files
     which define mpfr_ptr etc... */
  /* generic pointer */ void *sp_pointer;
  /* stdio file */
  FILE *sp_file;
  /*mpfr_ptr= */
  void *sp_mpfr;
  /* malloced pointer to mpfr_t */
};
/* make a special value; return NULL if the discriminant is not special */

struct meltspecialdata_st* meltgc_make_specialdata (melt_ptr_t discr_p);

struct meltspecialpayload_st
{
  union meltpayload_un
  {
    void* meltpayload__ptr1;
    FILE* meltpayload__file1;
  } meltpayload__un1;
#define meltpayload_ptr1 meltpayload__un1.meltpayload__ptr1
#define meltpayload_file1 meltpayload__un1.meltpayload__file1
  void* meltpayload_ptr2;
};

#define MELT_PAYLOAD_DESCRIPTOR_MAGIC 0x6795688b /* 1737844875 */
struct melt_payload_descriptor_st;
/* signature of payload destruction */
typedef void meltpayload_destroy_t (struct meltspecialdata_st*, const struct melt_payload_descriptor_st*);
/* signature of payload short printing; return a malloc-ed buffer to
   be freed by the caller. */
typedef char* meltpayload_sprint_t (struct meltspecialdata_st*, const struct melt_payload_descriptor_st*);

/* Payload descriptors should be static or global.  */
struct melt_payload_descriptor_st
{
  unsigned meltpyd_magic;	/* always
				   MELT_PAYLOAD_DESCRIPTOR_MAGIC, even
				   at registration time */
  unsigned meltpyd_rank;	/* either the rank, or 0 */
  const char* meltpyd_name;	/* unique name */
  void* meltpyd_data;	/* data for the client */
  meltpayload_destroy_t* meltpyd_destroy_rout; /* the destroying routine */
  meltpayload_sprint_t* meltpyd_sprint_rout;     /* the short printing routine */
  /* Unused, always 0 fields, for "upward" binary compatibility.  */
  void* meltpyd_spare1;
  void* meltpyd_spare2;
  void* meltpyd_spare3;
};

/* return a positive rank, or -1 on failure. May also fail with
   melt_fatal_error. */
int melt_payload_register_descriptor (struct melt_payload_descriptor_st*);

/* forwarded pointers; nobody see them except the melt copying
   garbage collector */
struct
GTY (())
meltforward_st
{
  meltobject_ptr_t discr;	/* actually always (void*)1 for forwarded */
  melt_ptr_t forward;
};


struct debugprint_melt_st
{
  FILE *dfil;
  int dmaxdepth;
  int dcount;
};


void melt_debug_out (struct debugprint_melt_st *dp, melt_ptr_t ptr,
                     int depth);
void melt_dbgeprint (void *p);
void melt_dbgbacktrace (int depth);

/* A pretty printing FILE, possibly in memory, or sort-of!  */
extern FILE* meltppfile;
extern char* meltppbuffer;
extern size_t meltppbufsiz;

/* Open a new pretty printing file into meltppfile, return the old one.  */
FILE* melt_open_ppfile (void);

/* Close the meltppfile and restore the previous one.  After than, the
   meltppbuffer & meltppbufsize contains the FILE* content. */
void melt_close_ppfile (FILE* oldfile);

static inline void
melt_release_ppbuf (void)
{
  free (meltppbuffer), meltppbuffer = NULL;
  meltppbufsiz = 0;
}

#ifdef ENABLE_GC_CHECKING
extern int melt_debug_garbcoll;
#define melt_debuggc_eprintf(Fmt,...) do {if (melt_debug_garbcoll > 0) \
      fprintf (stderr, "%s:%d:@$*" Fmt "\n",			       \
	       melt_basename(__FILE__), __LINE__, ##__VA_ARGS__);} while(0)
#else /*!ENABLE_GC_CHECKING*/
#define melt_debuggc_eprintf(Fmt,...) do{}while(0)
#endif /*ENABLE_GC_CHECKING*/

/* also in generated meltrunsup.h */
#ifndef meltobject_ptr_t_TYPEDEFINED
typedef struct meltobject_st* meltobject_ptr_t;
#define meltobject_ptr_t_TYPEDEFINED
#endif /*meltobject_ptr_t_TYPEDEFINED*/

#ifndef melt_ptr_t_TYPEDEFINED
typedef union melt_un* melt_ptr_t;
#define melt_ptr_t_TYPEDEFINED
#endif /*melt_ptr_t_TYPEDEFINED*/

#ifndef meltroutine_ptr_t_TYPEDEFINED
typedef struct meltroutine_st *meltroutine_ptr_t;
#define  meltroutine_ptr_t_TYPEDEFINED
#endif /*meltroutine_ptr_t_TYPEDEFINED*/


/******************* closures, routines ************************/

/* when the argdescr string of a closure routine is MELTPAR_MARKGGC
   the routine just marks the frame passed as first argument */
#define MELTPAR_MARKGGC ((char*)(-1L))

/* functions needed to support the generated map accessing routines
   inside meltrunsup.h */
void *meltgc_raw_new_mappointers (meltobject_ptr_t discr_p,
                                  unsigned len);

void
meltgc_raw_put_mappointers (void *mappointer_p,
                            const void *attr, melt_ptr_t valu_p);

melt_ptr_t
melt_raw_get_mappointers (void *mappointer_p, const void *attr);

melt_ptr_t
meltgc_raw_remove_mappointers (void *mappointer_p, const void *attr);



/*** the closures contain routines which are called by applying
     closures; each routine is called with:

     + the called closure
     + this first pointer argument
     + a (non null, can be empty) constant argument descriptor
       string describing the extra arguments
     + the array of union meltparam_un for extra arguments
     + a (non null, can be empty) constant argument descriptor string
       describing the extra results
     + the array of union meltparam_un for extra results

     and the result of the call is a pointer (the main result)

BTW, on AMD64 or x86_64 processors [a very common host at time of
writing], http://www.x86-64.org/documentation/abi.pdf the first six
arguments are passed thru registers; on POWERPC eight arguments are
passed thru registers
*/

/* Changing the argument descriptor format should be done with extreme
   care, and should be delicately kept in sync with
   warmelt-outobj.melt code generation, in particular with the
   generate of runtime support in meltrunsup.h generated file. The
   Argument descriptor cell type is those of elements in argument
   descriptor string. It should be char or wchar_t; changing it
   require also delicate changes inside warmelt-outobj.melt, in
   particular of the generate_runtypesupport_param function there. It
   should be kept in sync! */
typedef char melt_argdescr_cell_t;
/* The maximal value of above scalar types */
#define MELT_ARGDESCR_MAX CHAR_MAX



/* enumeration of predefined global object indexes inside
   melt_globvec; Most are wired predefined, in the sense that
   they are automagically allocated and partly filled before loading
   the melt file. Others are named, and are expected to be
   created by loading the melt files.  */
enum melt_globalix_en
{
  MELTGLOB__NONE,
  /************************* wired predefined */
#define MELT_HAS_PREDEFINED(Pred,Index) MELTGLOB_##Pred = Index,
#include "melt-predef.h"
  /**************************** placeholder for last wired */
  MELTGLOB__LASTWIRED,
  MELTGLOB___SPARE1,
  MELTGLOB___SPARE2,
  MELTGLOB___SPARE3,
  MELTGLOB___SPARE4,
  MELTGLOB___SPARE5,
  /*****/
  MELTGLOB__LASTGLOB
};
#define BGLOB__LASTGLOB MELTGLOB__LASTGLOB

/* get the predefined index by its name */
MELT_EXTERN int melt_predefined_index_by_name(const char*);

/* access or set a predefined */
static inline melt_ptr_t melt_fetch_predefined(int);
static inline void melt_store_predefined(int, melt_ptr_t);
#define MELT_PREDEF(Glob)  melt_fetch_predefined(MELTGLOB_##Glob)
#define MELT_STORE_PREDEF(Glob,P) melt_store_predefined(MELTGLOB_##Glob, (P))



/* forward declarations of touching functions; they can appear in
   meltrunsup.h generated file.  */
static inline void meltgc_touch_dest (void *touchedptr, void *destptr);
static inline void meltgc_touch (void* touchedptr);


/* File meltrunsup.h is inside melt/generated/ */
#include "meltrunsup.h"


/* the application routine does not call the GC; of course, the
   applied closure can call the GC! */
melt_ptr_t melt_apply (meltclosure_ptr_t clos_p,
                       melt_ptr_t firstarg,
                       const melt_argdescr_cell_t xargdescr_[],
                       union meltparam_un *xargtab_,
                       const melt_argdescr_cell_t xresdescr_[],
                       union meltparam_un *xrestab_);

/* Depth and counter of MELT applications are only significant when
   checking is enabled by ENABLE_CHECKING.  */
extern long melt_application_count (void);
extern long melt_application_depth (void);

/* The number of arguments given in an argument description "string";
   useful to implement variadic functions. */
static inline int
melt_argdescr_length (const melt_argdescr_cell_t* argdesc)
{
  if (!argdesc || (const void*)argdesc == (const void*)MELTPAR_MARKGGC)
    return 0;
#if MELT_ARGDESCR_MAX == CHAR_MAX
  return strlen ((const char*)argdesc);
#else /*MELT_ARGDESCR_MAX != CHAR_MAX*/
  {
    int ln = 0;
    while (*argdesc != (melt_argdescr_cell_t)0)
      ln++, argdesc++;
    return ln;
  }
#endif /* MELT_ARGDESCR_MAX == CHAR_MAX */
}

/* gnu indent has some trouble with GTY hence */
/* *INDENT-OFF* */

/* Since november 2012, MELT svn rev 193746, we don't use any more the
   vec.h GCC vectors, because they have changed significantly between
   GCC 4.7 and GCC 4.8 to use C++ templates. */




/* Gives the constant string corresponding to a given object magic
   above.  */
const char* melt_obmag_string(int i);

/* maxhash can also be used as a bit mask */
#define MELT_MAXHASH 0x3fffffff

/* maxlen can also be used as a bit mask */
#define MELT_MAXLEN 0x1fffffff

/* big length might be used to warn about really big stuff.. and is
   also a bit mask. */
#define MELT_BIGLEN 0x3ffffff


/***
    objects are a la ObjVlisp, single-inheritance with a root class,
    the discr of an object is its class
    each object has its hashcode, its magic (used to discriminate non-object values),
    its number of slots or instance variables object_arity, and an array of slots called vartab[]

    objects should be allocated in young region, hence discr should be
    forwarded in the garbage collector



*/



/* some types, including objects, strbuf, stringmaps, objectmaps, all
   the other *maps, contain a pointer to a non value; this pointer
   should be carefully updated in the forwarding step (and checked if
   young) */


/* return the magic of the discriminant or 0 */
static inline int
melt_magic_discr (melt_ptr_t p)
{
  if (!p)
    return 0;
#if ENABLE_GC_CHECKING
  if ((void*) p == MELT_POISON_POINTER)
    {
      /* This should never happen, and if it happens it means that p
      was insided a poisoned freed data zone, so the memory is
      corrupted; a data zone has been freed and then dereferenced. */
      melt_fatal_error
      ("corrupted memory retrieving magic discriminant of %p,"
       " (= the poison pointer)",
       (void*) p);
    }
#endif /*ENABLE_GC_CHECKING */
#if MELT_HAVE_DEBUG
  if (!p->u_discr)
    {
      /* This should never happen, we are asking the discriminant of a
      not yet filled, since cleared, memory zone. */
      melt_fatal_error
      ("corrupted memory heap retrieving magic discriminant of %p,"
       "(= a cleeared memory zone)",
       (void*) p);
    }
#endif /*MELT_HAVE_DEBUG*/
#if ENABLE_GC_CHECKING
  if ((void*) (p->u_discr) == MELT_POISON_POINTER)
    {
      /* This should never happen, we are asking the discriminant of a
      zone which has been poisoned, that is has been freed! */
      melt_fatal_error
      ("corrupted memory heap retrieving magic discriminant of %p,"
       "(= a freed and poisoned memory zone)",
       (void*) p);
    }
#endif /*ENABLE_GC_CHECKING*/
  return p->u_discr->meltobj_magic;
}


/* likewise, but without testing for null */
static inline int
melt_unsafe_magic_discr (melt_ptr_t p)
{
  return p->u_discr->meltobj_magic;
}

enum
{
  meltpydkind__none=0,
  meltpydkind_file,
  meltpydkind_rawfile,
  meltpydkind_reserve1,
  meltpydkind_reserve2,
  meltpydkind_reserve3,
  meltpydkind_reserve4,
  meltpydkind_reserve5,
  meltpydkind_reserve6,
  meltpydkind_reserve7,
  meltpydkind_reserve8,
  meltpydkind_reserve9,
  meltpydkind__last
};

/* Gives the special kind of some special data. */
static inline unsigned
melt_special_kind (melt_ptr_t p)
{
  if (p && melt_magic_discr (p) == MELTOBMAG_SPECIAL_DATA)
    return ((struct meltspecialdata_st*)p)->meltspec_kind;
  else
    return 0;
}

/* Gives a malloc-ed string -to be freed by the caller routine- for
   short printing of a special data, or else NULL.  */
char* meltgc_specialdata_sprint (melt_ptr_t ptr);

/* test if a pointer is an output - either a string buffer or a file */
static inline bool
melt_is_out (melt_ptr_t p)
{
  int d = melt_magic_discr(p);
  unsigned k = melt_special_kind (p);
  return d == MELTOBMAG_STRBUF
         || k == meltpydkind_file || k == meltpydkind_rawfile;
}

/* test if a pointer is a file */
static inline bool
melt_is_file (melt_ptr_t p)
{
  int d = melt_magic_discr (p);
  unsigned k = melt_special_kind (p);
  return
    (d == MELTOBMAG_SPECIAL_DATA
    ) &&
    (k == meltpydkind_file || k == meltpydkind_rawfile);
}


/* return the nth of a multiple (starting from 0) */
static inline melt_ptr_t
melt_multiple_nth (melt_ptr_t mul, int n)
{
  if (!mul || ((meltmultiple_ptr_t)mul)->discr->meltobj_magic != MELTOBMAG_MULTIPLE)
    return NULL;
  if (n >= 0 && n < (int) ((meltmultiple_ptr_t)mul)->nbval)
    return ((meltmultiple_ptr_t)mul)->tabval[n];
  else if (n < 0 && n + (int) ((meltmultiple_ptr_t)mul)->nbval >= 0)
    return ((meltmultiple_ptr_t)mul)->tabval[n + ((meltmultiple_ptr_t)mul)->nbval];
  return NULL;
}

/* set the nth of a multiple (but beware of circularities!) */
void meltgc_multiple_put_nth (melt_ptr_t mul, int n,
                              melt_ptr_t val);

/* return the length of a multiple */
static inline int
melt_multiple_length (melt_ptr_t mul)
{
  if (!mul || ((meltmultiple_ptr_t)mul)->discr->meltobj_magic != MELTOBMAG_MULTIPLE)
    return 0;
  return ((meltmultiple_ptr_t)mul)->nbval;
}

/* test if something is a tuple of a given length or bigger */
static inline bool
melt_is_multiple_at_least(melt_ptr_t mul, int ln)
{
  if (!mul || ln<0 || ((meltmultiple_ptr_t)mul)->discr->meltobj_magic != MELTOBMAG_MULTIPLE) return 0;
  return  (int)((meltmultiple_ptr_t)mul)->nbval >= (int)ln;
}

/* test if something is a tuple of an exactly given length */
static inline bool
melt_is_multiple_of_length(melt_ptr_t mul, int ln)
{
  if (!mul || ln<0 || ((meltmultiple_ptr_t)mul)->discr->meltobj_magic != MELTOBMAG_MULTIPLE) return 0;
  return  (int)((meltmultiple_ptr_t)mul)->nbval == (int)ln;
}




/* safely return the value inside a reference - instance of CLASS_REFERENCE */
melt_ptr_t
melt_reference_value  (melt_ptr_t cont);

/* make a new reference, instance of CLASS_REFERENCE */
melt_ptr_t meltgc_new_reference (melt_ptr_t val_p);

/* put inside a reference */
void meltgc_reference_put (melt_ptr_t cont, melt_ptr_t val);

/* return the phinodes of a boxed basicblock */
static inline gimple_seq
melt_basicblock_phinodes (melt_ptr_t box)
{
  struct meltbasicblock_st* b = (struct meltbasicblock_st*)box;
  if (!b || b->discr->meltobj_magic != MELTOBMAG_BASICBLOCK || !b->val)
    return NULL;
  return phi_nodes(b->val);
}


/* return the gimpleseq of a boxed basicblock */
static inline gimple_seq
melt_basicblock_gimpleseq (melt_ptr_t box)
{
  struct meltbasicblock_st* b = (struct meltbasicblock_st*)box;
  if (!b || b->discr->meltobj_magic != MELTOBMAG_BASICBLOCK || !b->val)
    return NULL;
  return bb_seq(b->val);
}



/*************************************************************
 * young generation copying garbage collector
 *
 * the young generation is managed specifically by an additional
 * copying garbage collector, which copies melt_ptr_t data into the
 * GGC heap from a young region.  This requires that every local
 * variable is known to our copying melt GC. For that purpose,
 * locals are copied (or used) inside a chain of melt_callframe_st
 * structures. Since our copying GC change pointers, every allocation
 * or call may change all the frames. Also stores inside data should
 * be explicitly managed in a store list
 *
 * the young allocation zone is typically of a few megabytes when it
 * is full, a minor garbage collection occur (and possibly a full GGC
 * collection afterwards) which changes all the locals
 *************************************************************/

/* start and end of young allocation zone */
extern void *melt_startalz;
extern void *melt_endalz;
/* current allocation pointer aligned */
extern char *melt_curalz;
/* the store vector grows downward */
extern void **melt_storalz;


/* special data in the allocation zone and in the old Ggc heap */
MELT_EXTERN struct meltspecialdata_st* melt_newspecdatalist;
MELT_EXTERN struct meltspecialdata_st* melt_oldspecdatalist;


/* kilowords allocated in young zone since last full collection */
extern unsigned long melt_kilowords_sincefull;
/* cumulated kilowords forwarded & copied to old Ggc heap */
extern unsigned long melt_kilowords_forwarded;
/* number of full & any melt garbage collections */
extern unsigned long melt_nb_full_garbcoll;
extern unsigned long melt_nb_garbcoll;

extern bool melt_prohibit_garbcoll;

extern bool melt_is_forwarding;

#define MELT_FORWARDED_DISCR (meltobject_ptr_t)1
melt_ptr_t melt_forwarded_copy (melt_ptr_t);


static inline bool
melt_is_young (const void *const p)
{
  return (const char * const) p >= (const char * const) melt_startalz
         && (const char * const) p < (const char * const) melt_endalz;
}

static inline void *
melt_forwarded (void *ptr)
{
  melt_ptr_t p = (melt_ptr_t) ptr;
  if (p && melt_is_young (p))
    {
      if (p->u_discr == MELT_FORWARDED_DISCR)
        p = ((struct meltforward_st *) p)->forward;
      else
        p = melt_forwarded_copy (p);
    }
  return p;
}

#if defined(__GNUC__) && GCC_VERSION > 4000
#define MELT_FORWARDED(P) do {if (P) { \
  (P) = (__typeof__(P))melt_forwarded((void*)(P));} } while(0)
#else
#define MELT_FORWARDED(P) do {if (P) { 		       		\
  (P) = (melt_ptr_t)melt_forwarded((melt_ptr_t)(P));} }  while(0)
#endif /*GCC_VERSION*/

/* the MELT copying garbage collector routine - moves all locals on
   the stack!  Minor GC is only moving, Minor or Full chooses either
   minor or full appropriately, and Full GC is the minor one followed by
   GCC garbage collector Ggc. */
enum melt_gckind_en
{ MELT_ONLY_MINOR= 0, MELT_MINOR_OR_FULL = 1, MELT_NEED_FULL = 2};
void melt_garbcoll (size_t wanted, enum melt_gckind_en gckd);

/* the alignment */
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define MELT_ALIGN (__alignof__(union melt_un))
#define MELT_LIKELY(P) __builtin_expect((P),1)
#define MELT_UNLIKELY(P) __builtin_expect((P),0)
#else
#define MELT_ALIGN (2*sizeof(void*))
#define MELT_LIKELY(P) (P)
#define MELT_UNLIKELY(P) (P)
#endif



#if ENABLE_CHECKING
/* to ease debugging we sometimes want to know when some pointer is
   allocated: set these variables in the debugger */
MELT_EXTERN void *melt_alptr_1;
MELT_EXTERN void *melt_alptr_2;
MELT_EXTERN unsigned melt_objhash_1;
MELT_EXTERN unsigned melt_objhash_2;
void melt_break_alptr_1_at (const char *msg, const char *fil, int line);
void melt_break_alptr_2_at (const char *msg, const char *fil, int line);
void melt_break_objhash_1_at (const char *msg, const char *fil, int line);
void melt_break_objhash_2_at (const char *msg, const char *fil, int line);
#define melt_break_alptr_1(Msg) melt_break_alptr_1_at((Msg),__FILE__,__LINE__)
#define melt_break_alptr_2(Msg) melt_break_alptr_2_at((Msg),__FILE__,__LINE__)
#define melt_break_objhash_1(Msg) melt_break_objhash_1_at((Msg),__FILE__,__LINE__)
#define melt_break_objhash_2(Msg) melt_break_objhash_2_at((Msg),__FILE__,__LINE__)

static void inline
melt_dbgtrace_written_object_at(meltobject_ptr_t ob, const char*msg, const char*fil, int lin)
{
  unsigned obh = 0;
  if (!ob || !msg)
    return;
  if (ob == melt_alptr_1)
    melt_break_alptr_1_at(msg, fil, lin);
  if (ob == melt_alptr_2)
    melt_break_alptr_2_at(msg, fil, lin);
  obh = ob->obj_hash;
  if (!obh)
    return;
  if (obh == melt_objhash_1)
    melt_break_objhash_1_at (msg, fil, lin);
  if (obh == melt_objhash_2)
    melt_break_objhash_2_at (msg, fil, lin);
}
#define melt_dbgtrace_written_object(Obj,Msg) do { \
  melt_dbgtrace_written_object_at ((meltobject_ptr_t)(Obj),(Msg),\
				  __FILE__,__LINE__); }while(0)
#else  /*!ENABLE_CHECKING*/
#define melt_dbgtrace_written_object(Obj,Msg) do{}while(0)
#endif /*ENABLE_CHECKING*/

/* the allocator routine allocates a zone of BASESZ with extra GAP */
static inline void *
meltgc_allocate (size_t basesz, size_t gap)
{
  size_t wanted;
  void *ptr;
  if (basesz < sizeof (struct meltforward_st))
    basesz = sizeof (struct meltforward_st);
  if ((basesz % MELT_ALIGN) != 0)
    basesz += (MELT_ALIGN - (basesz % MELT_ALIGN));
  if ((gap % MELT_ALIGN) != 0)
    gap += (MELT_ALIGN - (gap % MELT_ALIGN));
  wanted = basesz + gap;
  gcc_assert (wanted >= sizeof (struct meltforward_st));
  if (MELT_UNLIKELY (melt_curalz + wanted + 2 * MELT_ALIGN
                     >= (char *) melt_storalz))
    melt_garbcoll (wanted, MELT_MINOR_OR_FULL);
  ptr = melt_curalz;
#if ENABLE_CHECKING
  if (ptr == melt_alptr_1)
    melt_break_alptr_1("allocated alptr1");
  else if (ptr == melt_alptr_2)
    melt_break_alptr_2("allocated alptr2");
#endif /*ENABLE_CHECKING*/
  melt_curalz += wanted;
  return ptr;
}

/* we need sometimes to reserve some wanted size in the allocation
   zone without actaully using it now; this is needed for the few
   melt data structures, e.g. meltstrbuf_st, which have some
   content (e.g. the buffer zone itself bufzn) which should be kept
   young if the datastructure is young, and should become old (ie. GGC
   allocated) when it becomes old */
static inline void
meltgc_reserve(size_t wanted)
{
  if (wanted < 100*sizeof(void*) + sizeof(struct meltforward_st))
    wanted = 100*sizeof(void*) + sizeof(struct meltforward_st);
  if ((wanted  % MELT_ALIGN) != 0)
    wanted += (MELT_ALIGN - (wanted % MELT_ALIGN));
  if (MELT_UNLIKELY (melt_curalz + wanted + 2 * MELT_ALIGN
                     >= (char *) melt_storalz))
    melt_garbcoll (wanted, MELT_MINOR_OR_FULL);
}

/* we need a function to detect failure in reserved allocation; this
   melt_reserved_allocation_failure function should never be
   called; we do not want to use fatal_error which requires toplev.h
   inclusion; never call this function outside of
   melt_allocatereserved */
void melt_reserved_allocation_failure(long siz);

/*  allocates a previously reserved zone of BASESZ with extra GAP;
    this should never trigger the GC, because space was reserved
    earlier */
static inline void *
melt_allocatereserved (size_t basesz, size_t gap)
{
  size_t wanted;
  void *ptr;
  if (basesz < sizeof (struct meltforward_st))
    basesz = sizeof (struct meltforward_st);
  if ((basesz % MELT_ALIGN) != 0)
    basesz += (MELT_ALIGN - (basesz % MELT_ALIGN));
  if ((gap % MELT_ALIGN) != 0)
    gap += (MELT_ALIGN - (gap % MELT_ALIGN));
  wanted = basesz + gap;
  gcc_assert (wanted >= sizeof (struct meltforward_st));
  if (MELT_UNLIKELY (melt_curalz + wanted + 2 * MELT_ALIGN
                     >= (char *) melt_storalz))
    /* this should never happen */
    melt_reserved_allocation_failure((long) wanted);
  ptr = melt_curalz;
#if ENABLE_CHECKING
  if (ptr == melt_alptr_1)
    melt_break_alptr_1("allocatedreserved alptr1");
  else if (ptr == melt_alptr_2)
    melt_break_alptr_1("allocatedreserved alptr2");
#endif /*ENABLE_CHECKING*/
  melt_curalz += wanted;
  return ptr;
}


/* we maintain a small cache hasharray of touched values - the touched
   cache size should be a small prime */
#define MELT_TOUCHED_CACHE_SIZE 19
extern void *melt_touched_cache[MELT_TOUCHED_CACHE_SIZE];
/* the touching routine should be called on every melt value which
   has been touched (by mutating one of its internal pointers) - it
   may add the touched value to the store "array" and may trigger our
   melt copying garbage collector */
static inline void
meltgc_touch (void *touchedptr)
{
  /* we know that this may loose -eg on some 64bits hosts- some
     highend bits of the pointer but we don't care, since the 32
     lowest bits are enough (as hash); we need a double cast to avoid
     a warning */

#ifdef HAVE_STDINT_H
  unsigned pad = (unsigned) ((intptr_t) touchedptr);
#else /*HAVE_STDINT_H*/
  unsigned pad = (unsigned) ((long) touchedptr);
#endif /*HAVE_STDINT_H*/
  if ((char *) touchedptr >= (char *) melt_startalz
      && (char *) touchedptr <= (char *) melt_endalz)
    return;
  pad = pad % (unsigned) MELT_TOUCHED_CACHE_SIZE;
  if (melt_touched_cache[pad] == touchedptr)
    return;
  *melt_storalz = touchedptr;
  melt_storalz--;
  melt_touched_cache[pad] = touchedptr;
  if (MELT_UNLIKELY
      ((char *) ((void **) melt_storalz - 3) <= (char *) melt_curalz))
    melt_garbcoll (1024 * sizeof (void *) +
                   ((char *) melt_endalz - (char *) melt_storalz),
                   MELT_MINOR_OR_FULL);
}

/* we can avoid the hassle of adding a touched pointer to the store
   list if we know that the newly added pointer inside does not point
   into the new allocation zone; TOUCHEDPTR is the mutated value and
   DSTPTR is the newly added pointer insided */
static inline void
meltgc_touch_dest (void *touchedptr, void *destptr)
{
  /* if we add an old pointer we don't care */
  if (!melt_is_young(destptr))
    return;
  meltgc_touch (touchedptr);
}




static inline void
melt_mark_specialdata (struct meltspecialdata_st* p)
{
  p->meltspec_mark = 1;
  melt_debuggc_eprintf ("marked specialdata %p of magic %d = %s",
                        (void*)p, p->discr->meltobj_magic,
                        melt_obmag_string (p->discr->meltobj_magic));
}
#define melt_mark_special_data(P) melt_mark_specialdata(P)


static inline void
melt_mark_decay (struct meltdecay_st *p)
{
  /* FIXME: gengtype don't generate call to this! */
  /* this is tricky since it actually changes the marked data; however,
     changing pointers to NULL is ok! */
  if (p->remain <= 0)
    p->val = NULL;
  else
    p->remain--;
}




/* low level map routines */


/***
 * allocation routines that may trigger a garbage collection
 * (their name starts with meltgc)
 ***/

/* allocate a boxed long integer (or null if bad DISCR) fillen with NUM */
melt_ptr_t meltgc_new_int (meltobject_ptr_t discr, long num);

/* Retrieve an integer from a boxed integer or mixnumbers.  */
static inline long
melt_get_int (melt_ptr_t v)
{
  switch (melt_magic_discr (v))
    {
    case MELTOBMAG_INT:
      return ((struct meltint_st *) (v))->val;
    case MELTOBMAG_MIXINT:
      return ((struct meltmixint_st *) (v))->intval;
    case MELTOBMAG_MIXLOC:
      return ((struct meltmixloc_st *) (v))->intval;
    case MELTOBMAG_OBJECT:
      return ((meltobject_ptr_t) (v))->obj_num;
    default:
      return 0;
    }
}

static inline long
melt_unbox_int (melt_ptr_t v)
{
  if (!v) return 0L;
  if (melt_magic_discr(v) == MELTOBMAG_INT)
    return  ((struct meltint_st *) (v))->val;
  return 0L;
}

#define melt_unbox_long(V) melt_unbox_int(V)

/* Make a boxed real from a real value.  If discr is NULL, use DISCR_REAL.  */
melt_ptr_t meltgc_new_real(meltobject_ptr_t discr, REAL_VALUE_TYPE r);

/* Unbox real value. It returns 0 if not a boxed real.  */
static inline REAL_VALUE_TYPE
melt_get_real (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_REAL)
    return ((struct meltreal_st*) v)->val;
  return dconst0;
}


static inline long
melt_obj_hash (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_OBJECT)
    return ((meltobject_ptr_t) (v))->obj_hash;
  return 0;
}

/* obsolete function */
static inline unsigned long
melt_obj_serial (melt_ptr_t v ATTRIBUTE_UNUSED)
{
  return 0;
}


static inline long
melt_obj_len (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_OBJECT)
    return ((meltobject_ptr_t) (v))->obj_len;
  return 0;
}

static inline long
melt_obj_num (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_OBJECT)
    return ((meltobject_ptr_t) (v))->obj_num;
  return 0;
}

/* safe integer div & mod */
static inline long
melt_idiv (long i, long j)
{
  return (j != 0) ? (i / j) : 0;
}
static inline long
melt_imod (long i, long j)
{
  return (j != 0) ? (i % j) : 0;
}



/* allocate a boxed mixed integer & value) */
melt_ptr_t
meltgc_new_mixint (meltobject_ptr_t discr_p, melt_ptr_t val_p,
                   long num);
/* allocate a boxed mixed location */
melt_ptr_t
meltgc_new_mixloc (meltobject_ptr_t discr_p, melt_ptr_t val_p,
                   long num, location_t loc);

/* get the boxed value of a mixint */
static inline melt_ptr_t
melt_val_mixint (melt_ptr_t mix)
{
  struct meltmixint_st *smix = (struct meltmixint_st *) mix;
  if (melt_magic_discr (mix) == MELTOBMAG_MIXINT)
    return smix->ptrval;
  return NULL;
}
/* get the boxed value of a mixbigint */
static inline melt_ptr_t
melt_val_mixbigint (melt_ptr_t mix)
{
  struct meltmixbigint_st *smix = (struct meltmixbigint_st *) mix;
  if (melt_magic_discr (mix) == MELTOBMAG_MIXBIGINT)
    return smix->ptrval;
  return NULL;
}

static inline long
melt_num_mixint (melt_ptr_t mix)
{
  struct meltmixint_st *smix = (struct meltmixint_st *) mix;
  if (melt_magic_discr (mix) == MELTOBMAG_MIXINT)
    return smix->intval;
  return 0;
}

static inline long
melt_num_mixloc (melt_ptr_t mix)
{
  struct meltmixloc_st *smix = (struct meltmixloc_st *) mix;
  if (melt_magic_discr (mix) == MELTOBMAG_MIXLOC)
    return smix->intval;
  return 0;
}

static inline melt_ptr_t
melt_val_mixloc (melt_ptr_t mix)
{
  struct meltmixloc_st *smix = (struct meltmixloc_st *) mix;
  if (melt_magic_discr (mix) == MELTOBMAG_MIXLOC)
    return smix->ptrval;
  return NULL;
}


static inline location_t
melt_location_mixloc (melt_ptr_t mix)
{
  struct meltmixloc_st *smix = (struct meltmixloc_st *) mix;
  if (melt_magic_discr (mix) == MELTOBMAG_MIXLOC)
    return smix->locval;
  return (location_t)UNKNOWN_LOCATION;
}

/* allocate a mixbigint from a GMP biginteger */
melt_ptr_t meltgc_new_mixbigint_mpz (meltobject_ptr_t discr_p,
                                     melt_ptr_t val_p, mpz_t mp);

/* fill an mpz from a mixbigint and return true iff ok */
static inline bool
melt_fill_mpz_from_mixbigint(melt_ptr_t mix, mpz_t mp)
{
  struct meltmixbigint_st *bmix = (struct meltmixbigint_st *) mix;
  if (!bmix || !mp || melt_magic_discr (mix) != MELTOBMAG_MIXBIGINT)
    return false;
  mpz_import (mp, bmix->biglen,
              /*most significant word first*/ 1,
              sizeof(bmix->tabig[0]),
              /*native endian*/ 0,
              /*no nails bits*/0,
              bmix->tabig);
  return true;
}

/* get (safely) the nth (counting from 0) field of an object */
static inline melt_ptr_t
melt_field_object (melt_ptr_t ob, unsigned off)
{
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      if (off < pob->obj_len)
        return pob->obj_vartab[off];
    };
  return NULL;
}

/* allocate a new raw object of given KLASS (unchecked) with LEN slots */
meltobject_ptr_t meltgc_new_raw_object (meltobject_ptr_t klass_p,
                                        unsigned len);


/* melt diagnostic routine */
void melt_error_str(melt_ptr_t mixloc_p, const char* msg, melt_ptr_t str_p);
void melt_warning_str(int opt, melt_ptr_t mixloc_p, const char* msg, melt_ptr_t str_p);
void melt_inform_str(melt_ptr_t mixloc_p, const char* msg, melt_ptr_t str_p);

int* melt_dynobjstruct_fieldoffset_at(const char*fldnam, const char*fil, int lin);
int* melt_dynobjstruct_classlength_at(const char*clanam, const char* fil, int lin);

/* Print a warning at location loc with message $msg (a strbuf value). */
void melt_warning_at_strbuf (location_t loc, melt_ptr_t msgbuf);

#if MELTGCC_DYNAMIC_OBJSTRUCT

static inline melt_ptr_t
melt_dynobjstruct_getfield_object_at (melt_ptr_t ob, unsigned off, const char*fldnam, const char*fil, int lin, int**poff)
{
  unsigned origoff = off;
  if (poff && !*poff)
    *poff = melt_dynobjstruct_fieldoffset_at(fldnam, fil, lin);
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      if (poff && *poff) off = **poff;
      if (off < pob->obj_len)
        return pob->obj_vartab[off];
      error ("MELT checked dynamic field access failed (bad offset %d/%d/%d [%s:%d]) - %s",
             (int)off, (int)pob->obj_len, (int)origoff, fil, lin, fldnam?fldnam:"...");
      return NULL;
    }
  error ("MELT checked dynamic field access failed (not object [%s:%d]) - %s",
         fil, lin, fldnam?fldnam:"...");
  return NULL;
}

#define melt_object_get_field_at(Slot,Obj,Off,Fldnam,Fil,Lin) do {      \
  static int *offptr_##Lin;                                             \
  Slot =                                                                \
    melt_dynobjstruct_getfield_object_at((melt_ptr_t)(Obj),             \
                                            (Off),Fldnam,Fil,Lin,       \
                                            &offptr_##Lin);             \
} while(0)

#define melt_object_get_field(Slot,Obj,Off,Fldnam) \
  melt_object_get_field_at(Slot,Obj,Off,Fldnam,__FILE__,__LINE__)

#define melt_getfield_object(Obj,Off,Fldnam)				\
    melt_dynobjstruct_getfield_object_at((melt_ptr_t)(Obj),		\
					    (Off),Fldnam,__FILE__,	\
                                            __LINE__,			\
					    (int**)0)

static inline void
melt_dynobjstruct_putfield_object_at(melt_ptr_t ob, unsigned off, melt_ptr_t val, const char*fldnam, const char*fil, int lin, int**poff)
{
  unsigned origoff = off;
  if (poff && !*poff)
    *poff = melt_dynobjstruct_fieldoffset_at(fldnam, fil, lin);
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      if (poff && *poff) off = **poff;
      if (off < pob->obj_len)
        {
          pob->obj_vartab[off] = val;
          return;
        }
      error ("MELT checked dynamic field put failed (bad offset %d/%d/%d [%s:%d]) - %s",
             (int)off, (int)pob->obj_len, (int)origoff, fil, lin, fldnam?fldnam:"...");
      return;
    }
  error ("MELT checked dynamic field put failed (not object [%s:%d]) - %s",
         fil, lin, fldnam?fldnam:"...");
}

#define melt_putfield_object_at(Obj,Off,Val,Fldnam,Fil,Lin) do {	\
  static int* ptroff_##Lin;						\
  melt_dynobjstruct_putfield_object_at((melt_ptr_t)(Obj),		\
					  (Off),			\
					  (melt_ptr_t)(Val),Fldnam,	\
					  Fil,Lin,			\
					  &ptroff_##Lin); } while(0)

#define melt_putfield_object(Obj,Off,Val,Fldnam) \
  melt_putfield_object_at(Obj,Off,Val,Fldnam,__FILE__,__LINE__)


static inline melt_ptr_t
melt_dynobjstruct_make_raw_object (melt_ptr_t klas, int len,
                                   const char*clanam, const char*fil, int lin,
                                   int**pptr,
                                   int*deflenptr)
{
  if (pptr && !*pptr)
    *pptr = melt_dynobjstruct_classlength_at(clanam,fil,lin);
  if (pptr && *pptr)
    len = **pptr;
  if (pptr && !*pptr && deflenptr)
    {
      *deflenptr = len;
      *pptr = deflenptr;
    }
  return (melt_ptr_t)meltgc_new_raw_object((meltobject_ptr_t)klas,len);
}

#define melt_raw_object_create_at(Newobj,Klas,Len,Clanam,Fil,Lin) do {	\
  static int* ptrlen_##Lin;						\
  static int deflen_##Lin;						\
  Newobj =								\
    melt_dynobjstruct_make_raw_object					\
    ( (Klas),(Len),							\
      Clanam,Fil,Lin,							\
      &ptrlen_##Lin,							\
      &deflen_##Lin); } while(0)

#define melt_raw_object_create(Newobj,Klas,Len,Clanam) \
  melt_raw_object_create_at(Newobj,Klas,Len,Clanam,__FILE__,__LINE__)

#define melt_make_raw_object(Klas,Len,Clanam)			\
    melt_dynobjstruct_make_raw_object((Klas),(Len),			\
					 Clanam, __FILE__, __LINE__,	\
				      (int**)0, (int*)0)

#elif ENABLE_CHECKING
static inline melt_ptr_t
melt_getfield_object_at (melt_ptr_t ob, unsigned off, const char*msg, const char*fil, int lin)
{
  unsigned origoff = off;
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      if (off < pob->obj_len)
        return pob->obj_vartab[off];
      error ("MELT checked field access failed (bad offset %d/len %d/origoff %d [%s:%d]) - %s",
             (int)off, (int)pob->obj_len, (int)origoff, fil, lin, msg?msg:"...");
      return NULL;
    }
  error ("MELT checked field access failed (not object [%s:%d]) - %s",
         fil, lin, msg?msg:"...");
  return NULL;
}

static inline void
melt_putfield_object_at(melt_ptr_t ob, unsigned off, melt_ptr_t val, const char*msg, const char*fil, int lin)
{
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      if (off < pob->obj_len)
        {
          pob->obj_vartab[off] = val;
          return;
        }
      melt_fatal_error("MELT checked field put failed (bad offset %d/%d [%s:%d]) - %s",
                       (int)off, (int)pob->obj_len, fil, lin, msg?msg:"...");
    }
  melt_fatal_error("MELT checked field put failed (not object [%s:%d]) - %s",
                   fil, lin, msg?msg:"...");
}

static inline melt_ptr_t
melt_make_raw_object(melt_ptr_t klas, int len, const char*clanam)
{
  gcc_assert(clanam != NULL);
  return (melt_ptr_t)meltgc_new_raw_object((meltobject_ptr_t)klas,len);
}

#define melt_raw_object_create(Newobj,Klas,Len,Clanam) do {	\
  Newobj = melt_make_raw_object(Klas,Len,Clanam); } while(0)
#define melt_getfield_object(Obj,Off,Fldnam) melt_getfield_object_at((melt_ptr_t)(Obj),(Off),(Fldnam),__FILE__,__LINE__)
#define melt_object_get_field(Slot,Obj,Off,Fldnam) do {	\
  Slot = melt_getfield_object(Obj,Off,Fldnam);} while(0)
#define melt_putfield_object(Obj,Off,Val,Fldnam) melt_putfield_object_at((melt_ptr_t)(Obj),(Off),(melt_ptr_t)(Val),(Fldnam),__FILE__,__LINE__)
#else
#define melt_getfield_object(Obj,Off,Fldnam) (((meltobject_ptr_t)(Obj))->obj_vartab[Off])
#define melt_object_get_field(Slot,Obj,Off,Fldnam) do {	\
  Slot = melt_getfield_object(Obj,Off,Fldnam);} while(0)
#define melt_putfield_object(Obj,Off,Val,Fldnam) do {		\
((meltobject_ptr_t)(Obj))->obj_vartab[Off] = (melt_ptr_t)(Val);	\
}while(0)
#define melt_make_raw_object(Klas,Len,Clanam) \
  ((melt_ptr_t)meltgc_new_raw_object((meltobject_ptr_t)(Klas),Len))
#define melt_raw_object_create(Newobj,Klas,Len,Clanam) do { \
  Newobj = melt_make_raw_object(Klas,Len,Clanam); } while(0)
#endif



/* get (safely) the length of an object */
static inline int
melt_object_length (melt_ptr_t ob)
{
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      return pob->obj_len;
    }
  return 0;
}

/* get safely the nth field of an object or NULL */
static inline melt_ptr_t
melt_object_nth_field(melt_ptr_t ob, int rk)
{
  if (melt_magic_discr (ob) == MELTOBMAG_OBJECT)
    {
      meltobject_ptr_t pob = (meltobject_ptr_t) ob;
      if (rk<0)
        rk += pob->obj_len;
      if (rk>=0 && rk<pob->obj_len)
        return (melt_ptr_t)(pob->obj_vartab[rk]);
    }
  return NULL;
}

/* allocate a new string (or null if bad DISCR or null STR) initialized from
   _static_ (non gc-ed) memory STR with len SLEN or strlen(STR) if <0  */
melt_ptr_t meltgc_new_string_raw_len (meltobject_ptr_t discr,
                                      const char *str, int slen);

/* allocate a new string (or null if bad DISCR or null STR) initialized from
   _static_ (non gc-ed) memory STR  */
melt_ptr_t meltgc_new_string (meltobject_ptr_t discr,
                              const char *str);

/* Allocate a new string (or null if bad DISCR or null STR) initialized from
   a memory STR which is temporarily duplicated (so can be in gc-ed) */
melt_ptr_t meltgc_new_stringdup (meltobject_ptr_t discr,
                                 const char *str);

/* Get the naked basename of a path, ie from "/foo/bar.gyz" return
   "bar"; argument is duplicated */
melt_ptr_t meltgc_new_string_nakedbasename (meltobject_ptr_t discr,
    const char *str);
/* Get the basename of a path inside the temporary directory with an optional suffix */
melt_ptr_t meltgc_new_string_tempname_suffixed (meltobject_ptr_t discr,
    const char *namstr, const char*suffix);

/* Return a new string of given discriminant, with the original STR
   amputed of a given SUFFIX if appropriate, or else a copy of STR.  */
melt_ptr_t
meltgc_new_string_without_suffix (meltobject_ptr_t discr_p,
                                  const char* str,
                                  const char* suffix);

/* Return a string of a given discriminant (default DISCR_STRING), for
   the real path of a filepath which is an accessible file [perhaps a
   directory, etc...], or else NULL */
melt_ptr_t
meltgc_new_real_accessible_path_string (meltobject_ptr_t discr_p,
                                        const char *str);

/* Compute the hexadecimal encoded md5sum string of a file, or NULL on
   failure.  */
melt_ptr_t
meltgc_string_hex_md5sum_file (const char* path);

/* Compute the hexadecimal encoded md5sum string of a tuple of file
   paths, or NULL on failure */
melt_ptr_t
meltgc_string_hex_md5sum_file_sequence (melt_ptr_t pathtup_p);


/* Return as string value the name of a generated C++ file; if dirname
   is given and non-empty, it is used as the directory name using the
   basename of basepath, otherwise basepath is used. Any .melt or .so
   or .c suffix is removed, and if the num is positive it is
   appended. The result string is dirpath/basename+num.cc, e.g.
   /foo/dir/mybase+3.cc if dirpath is /foo/dir and basepath is
   /bar/mybase.c or mybase.melt etc...  and num is 3. If num is
   non-positive it is ignored. */
melt_ptr_t meltgc_new_string_generated_cc_filename  (meltobject_ptr_t discr_p,
    const char* basepath,
    const char* dirpath,
    int num);
/* for old compatibility */
#define meltgc_new_string_generated_c_filename meltgc_new_string_generated_cc_filename


/* Return true if we don't want to generate several C files for a
   given MELT module */
static inline bool melt_wants_single_c_file (void)
{
  return false;
}

/* split a [raw] string into a list of strings using a seperator. */
melt_ptr_t meltgc_new_split_string (const char*str, int sep, melt_ptr_t discr);


/* Return as a cached MELT string, using the
   :sysdata_src_loc_file_dict dictonnary for memoization, the file
   path of a location, or else NULL. */
melt_ptr_t meltgc_cached_string_path_of_source_location (source_location loc);

static inline const char *
melt_string_str (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_STRING)
    return ((struct meltstring_st *) v)->val;
  return 0;
}

static inline int
melt_string_length (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_STRING)
    {
      unsigned slen = ((struct meltstring_st *) v)->slen;
      gcc_assert (((struct meltstring_st *) v)->val[slen] == 0);
      return slen;
    }
  return 0;
}

static inline int
melt_string_nth (melt_ptr_t v, int rk)
{

  if (melt_magic_discr (v) == MELTOBMAG_STRING)
    {
      unsigned slen = ((struct meltstring_st *) v)->slen;
      if (rk < 0)
        rk += (int) slen;
      if (rk >= 0 && rk < (int)slen)
        return ((struct meltstring_st *) v)->val[rk];
    };
  return 0;
}

static inline int
melt_string_is_ending (melt_ptr_t v, const char *s)
{
  if (s && melt_magic_discr (v) == MELTOBMAG_STRING)
    {
      const char* str = ((struct meltstring_st *) v)->val;
      int lenstr = ((struct meltstring_st *) v)->slen;
      int lens = strlen (s);
      return (lens <= lenstr && !strncmp(str+lenstr-lens, s, lens));
    }
  return 0;
}

static inline int
melt_string_is_starting  (melt_ptr_t v, const char *s)
{
  if (s && melt_magic_discr (v) == MELTOBMAG_STRING)
    {
      const char* str = ((struct meltstring_st *) v)->val;
      int lenstr = strlen (str);
      int lens = strlen (s);
      return (lens <= lenstr && !strncmp(str, s, lens));
    }
  return 0;
}


static inline bool
melt_string_same (melt_ptr_t v1, melt_ptr_t v2)
{
  if (melt_magic_discr (v1) == MELTOBMAG_STRING
      && melt_magic_discr (v2) == MELTOBMAG_STRING)
    {
      return 0 == strcmp (((struct meltstring_st *) v1)->val,
                          ((struct meltstring_st *) v2)->val);
    }
  return 0;
}

static inline bool
melt_string_less (melt_ptr_t v1, melt_ptr_t v2)
{
  if (melt_magic_discr (v1) == MELTOBMAG_STRING
      && melt_magic_discr (v2) == MELTOBMAG_STRING)
    {
      return strcmp (((struct meltstring_st *) v1)->val,
                     ((struct meltstring_st *) v2)->val) < 0;
    }
  return 0;
}

static inline bool
melt_is_string_const (melt_ptr_t v, const char *s)
{
  if (s && melt_magic_discr (v) == MELTOBMAG_STRING)
    return 0 == strcmp (((struct meltstring_st *) v)->val, s);
  return 0;
}

static inline const char *
melt_strbuf_str (melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_STRBUF)
    {
      struct meltstrbuf_st *sb = (struct meltstrbuf_st*) v;
      if (sb->bufend >= sb->bufstart)
        return sb->bufzn + sb->bufstart;
    }
  return 0;
}

static inline int
melt_strbuf_usedlength(melt_ptr_t v)
{
  if (melt_magic_discr (v) == MELTOBMAG_STRBUF)
    {
      struct meltstrbuf_st *sb = (struct meltstrbuf_st *) v;
      if (sb->bufend >= sb->bufstart)
        return sb->bufend - sb->bufstart;
    }
  return 0;
}


/* peek the character in a stringbuffer at given offset or else -1 (which is EOF) */
static inline int
melt_strbuf_peek (melt_ptr_t v, int ioff)
{
  if (melt_magic_discr (v) == MELTOBMAG_STRBUF)
    {
      struct meltstrbuf_st *sbu = (struct meltstrbuf_st *) v;
      int slen = (sbu->bufend) - (sbu->bufstart);
      if (ioff < 0)
	ioff += slen;
      if (ioff >= 0 && ioff < slen)
	return (int)(sbu->bufzn[sbu->bufstart+ioff]);
    }
  return -1;
}


static inline void
melt_strbuf_consume (melt_ptr_t v, int cnt)
{
  if (melt_magic_discr (v) == MELTOBMAG_STRBUF && cnt>0)
    {
      struct meltstrbuf_st *sb = (struct meltstrbuf_st *) v;
      if (sb->bufstart + cnt < sb->bufend)
        sb->bufstart = sb->bufstart + cnt;
      else
        sb->bufstart = sb->bufend;
    }
}

/* return the length of an output, i.e. the used length of strbuf or
   the current file position of a file */
long melt_output_length (melt_ptr_t out_p);

/* output an strbuf into a file */
void melt_output_strbuf_to_file (melt_ptr_t sbuf, const char*filnam);

/* Write a buffer to a file, but take care to not overwrite the file
   if it does not change. */
void melt_output_strbuf_to_file_no_overwrite (melt_ptr_t sbufv, const char*filnam);


/* allocate a pair of given head and tail */
melt_ptr_t meltgc_new_pair (meltobject_ptr_t discr,
                            void *head, void *tail);

/* change the head of a pair */
void meltgc_pair_set_head(melt_ptr_t pair, void* head);

/* allocate a new multiple of given DISCR & length LEN */
melt_ptr_t meltgc_new_multiple (meltobject_ptr_t discr_p,
                                unsigned len);

/* make a subsequence of a given multiple OLDMUL_P from STARTIX to
   ENDIX; if either index is negative, take it from last.  return null
   if arguments are incorrect, or a fresh subsequence of same
   discriminant as source otherwise */
melt_ptr_t meltgc_new_subseq_multiple (melt_ptr_t oldmul_p,
                                       int startix, int endix);


/* allocate a new (empty) list */
melt_ptr_t meltgc_new_list (meltobject_ptr_t discr_p);
/* append to the tail of a list */
void meltgc_append_list (melt_ptr_t list_p, melt_ptr_t val_p);
/* prepend to the head of a list */
void meltgc_prepend_list (melt_ptr_t list_p, melt_ptr_t val_p);
/* pop from head of list (and remove) */
melt_ptr_t meltgc_popfirst_list (melt_ptr_t list_p);

/* return the length of a list, 0 for nil, or -1 iff non list */
int melt_list_length (melt_ptr_t list_p);




/* allocate e new empty mapobjects */
melt_ptr_t meltgc_new_mapobjects (meltobject_ptr_t discr_p,
                                  unsigned len);

/* put into a mapobjects */
void meltgc_put_mapobjects (meltmapobjects_ptr_t mapobject_p,
                            meltobject_ptr_t attrobject_p,
                            melt_ptr_t valu_p);

/* get from a mapobject */
melt_ptr_t melt_get_mapobjects (meltmapobjects_ptr_t mapobject_p,
                                meltobject_ptr_t attrobject_p);

/* remove from a mapobject (return the removed value) */
melt_ptr_t meltgc_remove_mapobjects (meltmapobjects_ptr_t
                                     mapobject_p,
                                     meltobject_ptr_t attrobject_p);

static inline int
melt_size_mapobjects (meltmapobjects_ptr_t mapobject_p)
{
  if (!mapobject_p || mapobject_p->discr->obj_num != MELTOBMAG_MAPOBJECTS)
    return 0;
  return melt_primtab[mapobject_p->lenix];
}

static inline unsigned
melt_count_mapobjects (meltmapobjects_ptr_t mapobject_p)
{
  if (!mapobject_p || mapobject_p->discr->obj_num != MELTOBMAG_MAPOBJECTS)
    return 0;
  return mapobject_p->count;
}

static inline meltobject_ptr_t
melt_nthattr_mapobjects (meltmapobjects_ptr_t mapobject_p, int ix)
{
  meltobject_ptr_t at = 0;
  if (!mapobject_p || mapobject_p->discr->obj_num != MELTOBMAG_MAPOBJECTS)
    return 0;
  if (ix < 0 || ix >= melt_primtab[mapobject_p->lenix])
    return 0;
  at = mapobject_p->entab[ix].e_at;
  if ((void *) at == (void *) HTAB_DELETED_ENTRY)
    return 0;
  return at;
}

static inline melt_ptr_t
melt_nthval_mapobjects (meltmapobjects_ptr_t mapobject_p, int ix)
{
  meltobject_ptr_t at = 0;
  if (!mapobject_p || mapobject_p->discr->obj_num != MELTOBMAG_MAPOBJECTS)
    return 0;
  if (ix < 0 || ix >= melt_primtab[mapobject_p->lenix])
    return 0;
  at = mapobject_p->entab[ix].e_at;
  if ((void *) at == (void *) HTAB_DELETED_ENTRY)
    return 0;
  return mapobject_p->entab[ix].e_va;
}

/* auxiliary data for mapobjects */

static inline melt_ptr_t
melt_auxdata_mapobjects (melt_ptr_t map_p)
{
  if (melt_magic_discr(map_p) == MELTOBMAG_MAPOBJECTS)
    return ((struct meltmapobjects_st *)map_p)->meltmap_aux;
  return NULL;
}

static inline void
melt_auxput_mapobjects (melt_ptr_t map_p, melt_ptr_t val_p)
{
  if (melt_magic_discr(map_p) == MELTOBMAG_MAPOBJECTS)
    {
      ((struct meltmapobjects_st *)map_p)->meltmap_aux = val_p;
      meltgc_touch_dest (map_p, val_p);
    }
}




/* allocate a new empty mapstrings */
melt_ptr_t meltgc_new_mapstrings (meltobject_ptr_t discr_p,
                                  unsigned len);

/* put into a mapstrings, the string is copied so can be in the gc-ed heap */
void meltgc_put_mapstrings (struct meltmapstrings_st *mapstring_p,
                            const char *str, melt_ptr_t valu_p);

/* get from a mapstring */
melt_ptr_t melt_get_mapstrings (struct meltmapstrings_st
                                *mapstring_p, const char *attr);


/* remove from a mapstring (return the removed value) */
melt_ptr_t meltgc_remove_mapstrings (struct meltmapstrings_st
                                     *mapstring_p, const char *str);

static inline int
melt_size_mapstrings (struct meltmapstrings_st *mapstring_p)
{
  if (!mapstring_p || mapstring_p->discr->obj_num != MELTOBMAG_MAPSTRINGS)
    return 0;
  return melt_primtab[mapstring_p->lenix];
}

static inline unsigned
melt_count_mapstrings (struct meltmapstrings_st *mapstring_p)
{
  if (!mapstring_p || mapstring_p->discr->obj_num != MELTOBMAG_MAPSTRINGS)
    return 0;
  return mapstring_p->count;
}

static inline const char *
melt_nthattrraw_mapstrings (struct meltmapstrings_st *mapstring_p,
                            int ix)
{
  const char *at = 0;
  if (!mapstring_p || mapstring_p->discr->obj_num != MELTOBMAG_MAPSTRINGS)
    return 0;
  if (ix < 0 || ix >= melt_primtab[mapstring_p->lenix])
    return 0;
  at = mapstring_p->entab[ix].e_at;
  if ((const void *) at == (const void *) HTAB_DELETED_ENTRY)
    return 0;
  return at;
}

static inline melt_ptr_t
melt_nthval_mapstrings (struct meltmapstrings_st *mapstring_p, int ix)
{
  const char *at = 0;
  if (!mapstring_p || mapstring_p->discr->obj_num != MELTOBMAG_MAPSTRINGS)
    return 0;
  if (ix < 0 || ix >= melt_primtab[mapstring_p->lenix])
    return 0;
  at = mapstring_p->entab[ix].e_at;
  if ((const void *) at == (const void *) HTAB_DELETED_ENTRY)
    return 0;
  return mapstring_p->entab[ix].e_va;
}

/* auxiliary data for mapstrings */

static inline melt_ptr_t
melt_auxdata_mapstrings (melt_ptr_t map_p)
{
  if (melt_magic_discr(map_p) == MELTOBMAG_MAPSTRINGS)
    return ((struct meltmapstrings_st *)map_p)->meltmap_aux;
  return NULL;
}

static inline void
melt_auxput_mapstrings (melt_ptr_t map_p, melt_ptr_t val_p)
{
  if (melt_magic_discr(map_p) == MELTOBMAG_MAPSTRINGS)
    {
      ((struct meltmapstrings_st *)map_p)->meltmap_aux = val_p;
      meltgc_touch_dest (map_p, val_p);
    }
}

/* allocate a new routine object of given DISCR and of length LEN,
   with a DESCR-iptive string a a PROC-edure */
meltroutine_ptr_t meltgc_new_routine (meltobject_ptr_t discr_p,
                                      unsigned len, const char *descr,
                                      meltroutfun_t * proc);


void meltgc_set_routine_data(melt_ptr_t rout_p, melt_ptr_t data_p);

static inline melt_ptr_t
melt_routine_data(melt_ptr_t rout)
{
  if (rout && ((meltroutine_ptr_t) rout)->discr->obj_num == MELTOBMAG_ROUTINE)
    return ((meltroutine_ptr_t) rout)->routdata;
  return NULL;
}

static inline char *
melt_routine_descrstr (melt_ptr_t rout)
{
  if (rout && ((meltroutine_ptr_t) rout)->discr->obj_num == MELTOBMAG_ROUTINE)
    return ((meltroutine_ptr_t) rout)->routdescr;
  return (char *) 0;
}

static inline int
melt_routine_size (melt_ptr_t rout)
{
  if (rout && ((meltroutine_ptr_t) rout)->discr->obj_num == MELTOBMAG_ROUTINE)
    return ((meltroutine_ptr_t) rout)->nbval;
  return 0;
}

static inline melt_ptr_t
melt_routine_nth (melt_ptr_t rout, int ix)
{
  if (rout && ((meltroutine_ptr_t) rout)->discr->obj_num == MELTOBMAG_ROUTINE)
    if (ix >= 0 && ix < (int) ((meltroutine_ptr_t) rout)->nbval)
      return ((meltroutine_ptr_t) rout)->tabval[ix];
  return 0;
}

/*********/
/* allocate a new closure of given DISCR with a given ROUT,  and of length LEN
 */
meltclosure_ptr_t meltgc_new_closure (meltobject_ptr_t discr_p,
                                      meltroutine_ptr_t rout_p,
                                      unsigned len);

static inline int
melt_closure_size (melt_ptr_t clo)
{
  if (clo && ((meltclosure_ptr_t) clo)->discr->obj_num == MELTOBMAG_CLOSURE)
    return ((meltclosure_ptr_t) clo)->nbval;
  return 0;
}

static inline melt_ptr_t
melt_closure_routine (melt_ptr_t clo)
{
  if (clo && ((meltclosure_ptr_t) clo)->discr->obj_num == MELTOBMAG_CLOSURE)
    return (melt_ptr_t) (((meltclosure_ptr_t) clo)->rout);
  return 0;
}

static inline melt_ptr_t
melt_closure_nth (melt_ptr_t clo, int ix)
{
  if (clo && ((meltclosure_ptr_t) clo)->discr->obj_num == MELTOBMAG_CLOSURE
      && ix >= 0 && ix < (int) (((meltclosure_ptr_t) clo)->nbval))
    return (melt_ptr_t) (((meltclosure_ptr_t) clo)->tabval[ix]);
  return NULL;
}


/*********** hook functions *******/
static inline int
melt_hook_size (melt_ptr_t hk)
{
  if (hk && ((melthook_ptr_t) hk)->discr->obj_num == MELTOBMAG_HOOK)
    return (int) (((melthook_ptr_t) hk)->nbval);
  return 0;
}

static inline melt_ptr_t
melt_hook_nth  (melt_ptr_t hk, int ix)
{
  if (hk && ((melthook_ptr_t) hk)->discr->obj_num == MELTOBMAG_HOOK
      && ix >= 0 && ix < (int) (((melthook_ptr_t) hk)->nbval))
    return (melt_ptr_t)  ((melthook_ptr_t) hk)->tabval[ix];
  return NULL;
}

#define MELT_HOOK_NTH_SLOT(Msg,Hk,Rk)					\
   (melt_ensuremsg("MELT hook nth check" Msg,				\
		   ((Rk) >= 0						\
		    && (Rk) < melt_hook_size((melt_ptr_t)(Hk)))),	\
    ((melthook_ptr_t)(Hk))->tabval[(Rk)])

static inline melt_ptr_t
melt_hook_data  (melt_ptr_t hk)
{
  if (hk && ((melthook_ptr_t) hk)->discr->obj_num == MELTOBMAG_HOOK)
    return  (melt_ptr_t) (((melthook_ptr_t) hk)->hookdata);
  return NULL;
}

MELT_EXTERN void meltgc_set_hook_data (melt_ptr_t hk, melt_ptr_t data);
MELT_EXTERN const char* melt_hook_interned_name (melt_ptr_t hk);
MELT_EXTERN melt_ptr_t meltgc_hook_name_string (melt_ptr_t hook_p);


/***** list and pairs accessors ****/
/* safe pair head & tail accessors */
static inline melt_ptr_t
melt_pair_head (melt_ptr_t pair)
{
  if (pair && ((struct meltpair_st *) pair)->discr->obj_num == MELTOBMAG_PAIR)
    return ((struct meltpair_st *) pair)->hd;
  return 0;
}

static inline melt_ptr_t
melt_pair_tail (melt_ptr_t pair)
{
  if (pair && ((struct meltpair_st *) pair)->discr->obj_num == MELTOBMAG_PAIR)
    return (melt_ptr_t) (((struct meltpair_st *) pair)->tl);
  return 0;
}

/* compute the length of a pairlist */
static inline long
melt_pair_listlength (melt_ptr_t pair)
{
  long l = 0;
  while (pair
         && ((struct meltpair_st *) pair)->discr->obj_num == MELTOBMAG_PAIR)
    {
      l++;
      pair = (melt_ptr_t) (((struct meltpair_st *) pair)->tl);
    };
  return l;
}

static inline melt_ptr_t
melt_list_first (melt_ptr_t lis)
{
  if (lis && ((struct meltlist_st *) lis)->discr->obj_num == MELTOBMAG_LIST)
    return (melt_ptr_t) (((struct meltlist_st *) lis)->first);
  return NULL;
}

static inline melt_ptr_t
melt_list_last (melt_ptr_t lis)
{
  if (lis && ((struct meltlist_st *) lis)->discr->obj_num == MELTOBMAG_LIST)
    return (melt_ptr_t) (((struct meltlist_st *) lis)->last);
  return NULL;
}


/* allocate e new empty longsbucket */
melt_ptr_t meltgc_new_longsbucket (meltobject_ptr_t discr_p,
                                   unsigned len);


/* retrieve value associated in a bucket of longs with dichotomical search or else NULL */
static inline melt_ptr_t
melt_longsbucket_get (melt_ptr_t bucketp, long key)
{
  struct meltbucketlongs_st*buck = NULL;
  unsigned lo=0, hi=0, md=0, ucnt=0;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return NULL;
  buck = (struct meltbucketlongs_st*)bucketp;
  ucnt = buck->buckl_ucount;
  if (ucnt == 0)
    return NULL;
  gcc_assert (melt_primtab[buck->buckl_lenix] >= ucnt);
  lo = 0;
  hi = ucnt - 1;
  while (lo + 2 < hi)
    {
      long curk = 0;
      md = (lo + hi) / 2;
      curk = buck->buckl_entab[md].ebl_at;
      if (curk < key)
        lo = md;
      else if (curk > key)
        hi = md;
      else return buck->buckl_entab[md].ebl_va;
    };
  for (md = lo; md <= hi; md++)
    if (buck->buckl_entab[md].ebl_at == key)
      return buck->buckl_entab[md].ebl_va;
  return NULL;
}

/* retrieve the n-th long key in a bucketlong or else 0 */
static inline long
melt_longsbucket_nth_key (melt_ptr_t bucketp, int rk)
{
  struct meltbucketlongs_st*buck = NULL;
  unsigned ucnt=0;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return 0L;
  buck = (struct meltbucketlongs_st*)bucketp;
  ucnt = buck->buckl_ucount;
  if (ucnt == 0)
    return 0L;
  if (rk<0)
    rk += (int) ucnt;
  if (rk >= 0 && rk < (int) ucnt)
    return buck->buckl_entab[rk].ebl_at;
  return 0L;
}

static inline melt_ptr_t
melt_longsbucket_nth_val (melt_ptr_t bucketp, int rk)
{
  struct meltbucketlongs_st*buck = NULL;
  unsigned  ucnt=0;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return NULL;
  buck = (struct meltbucketlongs_st*)bucketp;
  ucnt = buck->buckl_ucount;
  if (ucnt == 0)
    return NULL;
  if (rk<0)
    rk += (int) ucnt;
  if (rk >= 0 && rk < (int) ucnt)
    return buck->buckl_entab[rk].ebl_va;
  return NULL;
}

/* retrieve the auxiliary data inside a bucket of longs */
static inline melt_ptr_t
melt_longsbucket_aux (melt_ptr_t bucketp)
{
  struct meltbucketlongs_st*buck = NULL;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return NULL;
  buck = (struct meltbucketlongs_st*)bucketp;
  return buck->buckl_aux;
}

/* retrieve the extra number inside a bucket of longs */
static inline int
melt_longsbucket_xnum (melt_ptr_t bucketp)
{
  struct meltbucketlongs_st*buck = NULL;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return 0;
  buck = (struct meltbucketlongs_st*)bucketp;
  return buck->buckl_xnum;
}

/* Set the extra number in a longsbucket - no GC possible! */
static inline void
melt_longsbucket_set_xnum (melt_ptr_t bucketp, int xnum)
{
  struct meltbucketlongs_st*buck = NULL;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return;
  buck = (struct meltbucketlongs_st*)bucketp;
  buck->buckl_xnum = xnum;
}


/* get the used count of a bucket of longs */
static inline int
melt_longsbucket_count (melt_ptr_t bucketp)
{
  struct meltbucketlongs_st*buck = NULL;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return 0;
  buck = (struct meltbucketlongs_st*)bucketp;
  gcc_assert (buck->buckl_ucount <= melt_primtab[buck->buckl_lenix]);
  return buck->buckl_ucount;
}


/* get the allocated size of a bucket of longs */
static inline int
melt_longsbucket_size (melt_ptr_t bucketp)
{
  struct meltbucketlongs_st*buck = NULL;
  if (melt_magic_discr (bucketp) != MELTOBMAG_BUCKETLONGS)
    return 0;
  buck = (struct meltbucketlongs_st*)bucketp;
  return melt_primtab[buck->buckl_lenix];
}


/* replace the value associated in a bucket of longs to a long key;
   don't do anything if the key was absent; return the old value
   associated to that key, or else NULL. */
melt_ptr_t meltgc_longsbucket_replace (melt_ptr_t bucketp, long key, melt_ptr_t valp);

/* put or replace the value associated in a bucket of longs; return
   the re-allocated bucket or the same one, or else NULL */
melt_ptr_t meltgc_longsbucket_put (melt_ptr_t bucketp, long key, melt_ptr_t valp);

/* Remove the value associated in a bucket of longs; return the
   re-allocated bucket or the same one, or else NULL */
melt_ptr_t meltgc_longsbucket_remove (melt_ptr_t bucketp, long key);

/* Set the auxiliary data in a longsbucket */
void meltgc_longsbucket_set_aux (melt_ptr_t bucketp, melt_ptr_t auxp);

/***** STRBUF ie string buffers *****/

/* allocate a new strbuf of given DISCR with initial content STR */
struct meltstrbuf_st *meltgc_new_strbuf (meltobject_ptr_t discr_p,
    const char *str);


/****
      Output routines can go into a boxed strbuf or a boxed file
****/
/* reserve some space in the buffer, for performance reasons. */
void meltgc_strbuf_reserve (melt_ptr_t outbuf_p, unsigned len);

/* add into OUT (a boxed STRBUF or a boxed FILE) the static string STR
   (which is not in the melt heap) */
void meltgc_add_out_raw (melt_ptr_t outbuf_p,
                         const char *str);

#define meltgc_add_strbuf_raw(Out,Str) meltgc_add_out_raw((Out),(Str))

/*  add into OUT (a boxed STRBUF or a boxed FILE) the static string
   STR (which is not in the melt heap) of length SLEN or
   strlen(STR) if SLEN<0 */
void meltgc_add_out_raw_len (melt_ptr_t outbuf_p,
                             const char *str, int slen);
#define meltgc_add_strbuf_raw_len(Out,Str,Len) meltgc_add_out_raw_len((Out),(Str),(Len))

/* add safely into OUTBUF the string STR (which is first copied, so
   can be in the melt heap) */
void meltgc_add_out (melt_ptr_t outbuf_p,
                     const char *str);
#define meltgc_add_strbuf(Out,Str) meltgc_add_out((Out),(Str))

/* add safely into OUTBUF the string STR encoded as a C string with
   backslash escapes */
void meltgc_add_out_cstr (melt_ptr_t outbuf_p,
                          const char *str);
#define meltgc_add_strbuf_cstr(Out,Str) meltgc_add_out_cstr(Out,Str)

/* add safely into OUTBUF the string STR of length SLEN encoded as a C
   string with backslash escapes */
void meltgc_add_out_cstr_len (melt_ptr_t outbuf_p,
                              const char *str, int slen);
#define meltgc_add_strbuf_cstr_len(Out,Str,Slen) \
  meltgc_add_out_cstr_len(Out,Str,Slen)

/* add safely into OUTBUF the substring of STR starting at offset OFF
   of length SLEN encoded as a C string with backslash escapes */
void meltgc_add_out_csubstr_len (melt_ptr_t outbuf_p,
                                 const char *str, int off, int slen);
#define meltgc_add_strbuf_csubstr_len(Out,Str,Slen) \
  meltgc_add_out_csubstr_len(Out,Str,Slen)

/* add safely into OUTBUF the string STR encoded as the interior of a
   C comment with slash star and star slash replaced by slash plus and
   plus slash */
void meltgc_add_out_ccomment (melt_ptr_t outbuf_p,
                              const char *str);
#define meltgc_add_strbuf_ccomment(Out,Str) meltgc_add_out_ccomment((Out),(Str))

/* add safely into OUTBUF the string STR (which is copied at first)
   encoded as a C identifier; ie non-alphanum begine encoded as an
   underscore */
void meltgc_add_out_cident (melt_ptr_t outbuf_p,
                            const char *str);
#define meltgc_add_strbuf_cident(Out,Str) meltgc_add_out_cident((Out),(Str))

/* add safely into OUTBUF the initial prefix of string STR (which is
   copied at first), with a length of at most PREFLEN encoded as a C
   identifier; ie non-alphanum begine encoded as an underscore */
void
meltgc_add_out_cidentprefix (melt_ptr_t strbuf_p, const char *str, int preflen);
#define meltgc_add_strbuf_cidentprefix(Out,Str,Pln) meltgc_add_out_cidentprefix((Out),(Str),(Pln))

/* add safely into OUTBUF the hex encoded number L */
void meltgc_add_out_hex (melt_ptr_t outbuf_p,
                         unsigned long l);
#define meltgc_add_strbuf_hex(Out,L) meltgc_add_out_hex((Out),(L))

/* add safely into OUTBUF the decimal encoded number L */
void meltgc_add_out_dec (melt_ptr_t outbuf_p, long l);
#define meltgc_add_strbuf_dec(Out,L) meltgc_add_out_dec((Out),(L))

/* add safely into OUTBUF a printf like stuff with FMT */
void
meltgc_out_printf (melt_ptr_t outbuf_p, const char *fmt,
                   ...) ATTRIBUTE_PRINTF (2, 3);
/* don't bother using CPP varargs */
#define meltgc_strbuf_printf meltgc_out_printf

/* add safely into OUTBUF either a space or an indented newline if the current line is bigger than the threshold */
void meltgc_out_add_indent (melt_ptr_t strbuf_p,
                            int indeptn, int linethresh);
#define meltgc_strbuf_add_indent(Out,I,L) meltgc_out_add_indent ((Out),(I),(L))

/* pretty print into OUTBUF a gimple */
void meltgc_ppout_gimple(melt_ptr_t outbuf_p, int indentsp, gimple gstmt);
#define meltgc_ppstrbuf_gimple(Out,I,G) meltgc_ppout_gimple ((Out), (I), (G))

/* pretty print into an OUTBUF a gimple seq */
void meltgc_ppout_gimple_seq(melt_ptr_t outbuf_p, int indentsp, gimple_seq gseq);
#define meltgc_ppstrbuf_gimple_seq(Out,I,G) meltgc_ppout_gimple_seq ((Out), (I), (G))

/* pretty print into an OUTBUF a tree */
void meltgc_ppout_tree_perhaps_briefly(melt_ptr_t outbuf_p, int indentsp, tree tr, bool briefly);
#define meltgc_ppout_tree(Out,I,T) meltgc_ppout_tree_perhaps_briefly((Out),(I),(T),FALSE)
#define meltgc_ppout_tree_briefly(Out,I,T) meltgc_ppout_tree_perhaps_briefly((Out),(I),(T),TRUE)
#define meltgc_ppstrbuf_tree(Out,I,T) meltgc_ppout_tree  ((Out), (I), (T))

/* pretty print into an outbuf a basic_block */
void meltgc_ppout_basicblock(melt_ptr_t out_p, int indentsp, basic_block bb);
#define meltgc_ppstrbuf_basicblock(Out,I,BB) meltgc_ppout_basicblock ((Out),(I),(BB))

/* pretty print into an outbuf a multiprecision integer */
void meltgc_ppout_mpz(melt_ptr_t out_p, int indentsp, mpz_t mp);
#define meltgc_ppstrbuf_mpz(O,I,M) meltgc_ppout_mpz((O), (I), (M))

/* pretty print into an outbuf the mpz of a MELT bigint; do nothing if
   big_p is not a MELT bigint */
void meltgc_ppout_mixbigint(melt_ptr_t out_p, int indentsp, melt_ptr_t big_p);
#define meltgc_ppstrbuf_mixbigint(O,I,B) meltgc_ppout_mixbigint ((O), (I), (B))

/* output print into an outbuf an edge */
void meltgc_out_edge(melt_ptr_t out_p, edge edg);

/* output print into an outbuf a loop */
void meltgc_out_loop(melt_ptr_t out_p, loop_p loo);



/*** allocate a boxed file ***/
melt_ptr_t meltgc_new_file(melt_ptr_t discr_p, FILE* fil);




/**************************** misc *****************************/
/* a random generator */
static inline long melt_lrand (void)
{
  /* we used to call lrand48_r using some randata filled at init time,
     but lrand48_r is less portable than lrand48 */
  return lrand48();
}


static inline unsigned melt_nonzerohash (void)
{
  unsigned h;
  do
    {
      h = melt_lrand () & MELT_MAXHASH;
    }
  while (h == 0);
  return h;
}

/* initialize all - don't do anything when called more than once */
void melt_initialize (void);
/* finalize all */
void melt_finalize (void);

/* find a symbol in all the loaded modules */
void* melt_dlsym_all (const char*nam);

/* Returns malloc-ed path inside a temporary directory, with a given
   basename & suffix; may create using mkdir the temporary directory,
   unless basnam is NULL.  */
char* melt_tempdir_path (const char* basnam, const char* suffix);

/* Clone a source value with a new discriminant. When the cloning can't be
   done, return the original source value. Generated code in meltrunsup-inc.cc */
melt_ptr_t
meltgc_clone_with_discriminant (melt_ptr_t srcval_p, melt_ptr_t newdiscr_p);

/* Load a module list, but don't start the modules yet.  DEPTH is
   the recursion depth, MODLISTBASE is the module list's name.  */
void meltgc_load_module_list (int depth, const char *modlistbase);

/* Load a single module, but don't start it. The MODUL can be a
   MODULBASE.FLAVOR string.  Give the module index.  */
int meltgc_load_one_module (const char*modul);

/* Load a single module of base MODULBASE and given FLAVOR but don't
   start it.  Give the module index.  */
int meltgc_load_flavored_module (const char*modulbase, const char*flavor);

/* Load a single module MODULBASE of given FLAVOR and start it with
   all new modules in environment ENV.  Return the resulting new environment. */
melt_ptr_t
meltgc_start_flavored_module (melt_ptr_t env_p, const char*modulbase, const char*flavor);

/* Run the start routine of a module already loaded. ENV_P is the
   parent environment, MODIX is the index of the module. Result is the
   new environment produced by the module.  */
melt_ptr_t meltgc_start_module_by_index (melt_ptr_t env_p, int modix);

/* Run the start routines of all modules which have not been
   started. ENV_P is the parent environment. Gives an environment. */
melt_ptr_t meltgc_start_all_new_modules (melt_ptr_t env_p);


/* Run a C extension module of BASENAME_P in environment ENV_P with
   literal tuple value LITVALTUP_P. Only called from
   translate_run_melt_expressions MELT function. */
melt_ptr_t
meltgc_run_cc_extension (melt_ptr_t basename_p, melt_ptr_t env_p, melt_ptr_t litvaltup_p);
/* for compatibility */
#define meltgc_run_c_extension meltgc_run_cc_extension

/* Compile a SRCBASE file (without .c) into a BINBASE file (without
   .so) in a WORKDIR (or the tempdir) with given FLAVOR. See
   melt-module.mk file.  Actually compiles several files, that is
   SRCBASE.c SRCBASE+01.c ... SRCBASE+meltdesc.c SRCBASE+melttime.h
   into BINBASE.SRCMD5SUM.FLAVOR.so dynamically loadable shared object
   file in the WORKDIR (which also keeps the intermediate *.pic.o
   files).  */
void melt_compile_source (const char *srcbase, const char *binbase, const char*workdir, const char*flavor);


/* get (or create) the symbol of a given name, using the INITIAL_SYSTEM_DATA
   global; the NAM string can be in the GC-allocated heap since it is
   copied */
enum
{
  /* a meningful enum for the create flag below */
  MELT_GET = 0,
  MELT_CREATE
};

/* Nota: use the melthookproc_HOOK_NAMED_SYMBOL to get or created
   named symbols. */

/* Note: use the melthookproc_HOOK_NAMED_KEYWORD to get or created
   named keywords. */

/* Note: use the melthookproc_HOOK_INTERN_SYMBOL to intern symbols,
   and melthookproc_HOOK_INTERN_KEYWORD to intern keywords.  */

/* Read a list of sexpressions from a file; if the second argument is
   non-empty and non-null, it is used for locations; otherwise the
   basename of the filnam is used: if the filnam is - read from stdin
   till eof.  */
melt_ptr_t meltgc_read_file (const char *filnam, const char* locnam);

/* read a list of sexpressions from a raw string [which should not be
   in the melt heap] using a raw location name and a location in source */
melt_ptr_t meltgc_read_from_rawstring(const char* rawstr, const char* rawlocnam, location_t loch);

/* read a list of sexpressions from a string or strbuf value or named
   object; if the second argument is non-empty and non-null, it is
   used for locations */
melt_ptr_t meltgc_read_from_val(melt_ptr_t strv_p, melt_ptr_t locnam_p);

/* called from c-common.c in handle_melt_attribute */
void melt_handle_melt_attribute(tree decl, tree name, const char* attrstr, location_t loch);

/* Use melt_assertmsg(MESSAGE,EXPR) to test invariants.  The MESSAGE
   should be a constant string displayed when asserted EXPR is
   false */
void
melt_assert_failed (const char *msg, const char *filnam, int lineno,
                    const char *fun)
ATTRIBUTE_NORETURN;
void
melt_check_failed (const char *msg, const char *filnam, int lineno,
                   const char *fun);

#if ENABLE_ASSERT_CHECKING
#define melt_assertmsg(MSG,EXPR)  do { if (MELT_UNLIKELY(!(EXPR))) \
      melt_assert_failed ((MSG),__FILE__,__LINE__,__FUNCTION__);   \
  } while(0)
#define melt_checkmsg(MSG,EXPR)   do { if (MELT_UNLIKELY(!(EXPR))) \
      melt_check_failed ((MSG),__FILE__,__LINE__,__FUNCTION__);	   \
  } while(0)
#define melt_ensuremsg(MSG,EXPR) (void) ((EXPR) ||  \
					 (melt_assert_failed ((MSG),__FILE__,__LINE__,__FUNCTION__),0))
#else
/* Include EXPR, so that unused variable warnings do not occur.  */
#define melt_assertmsg(MSG,EXPR) ((void)(0 && (MSG) && (EXPR)))
#define melt_assert_failed(MSG,FIL,LIN,FUN) ((void)(0 && (MSG)))
#define melt_checkmsg(MSG,EXPR) ((void)(0 && (MSG) && (EXPR)))
#define melt_check_failed(MSG,FIL,LIN,FUN) ((void)(0 && (MSG)))
#define melt_ensuremsg(MSG,EXPR) ((void)(0 && (MSG) && (EXPR)))
#endif

enum { MELT_ANYWHERE=0, MELT_NOYOUNG };
// no more check of call frames
#define melt_check_call_frames(YNG,MSG) (void)(0)


/******************* method sending ************************/
melt_ptr_t meltgc_send (melt_ptr_t recv,
                        melt_ptr_t sel,
                        const melt_argdescr_cell_t *xargdescr_,
                        union meltparam_un *xargtab_,
                        const melt_argdescr_cell_t *xresdescr_,
                        union meltparam_un *xrestab_);

/**************************** globals **************************/

/* *INDENT-OFF* */
/* We group the globals in small chunks, to ease forwarding them in
   the GC.  Notice that MELT_GLOBAL_ENTRY_CHUNK should be a small power
   of two. */
#define MELT_GLOBAL_ENTRY_CHUNK 8
#define MELT_NB_GLOBAL_CHUNKS (2+MELTGLOB__LASTGLOB/MELT_GLOBAL_ENTRY_CHUNK)
#define MELT_NB_GLOBALS (MELT_GLOBAL_ENTRY_CHUNK*MELT_NB_GLOBAL_CHUNKS)
extern GTY(()) melt_ptr_t melt_globalptrs[MELT_NB_GLOBALS];
extern bool melt_touchedglobalchunk[MELT_NB_GLOBAL_CHUNKS];

/* *INDENT-ON* */


static inline melt_ptr_t
melt_fetch_predefined(int ix)
{
  if (ix>0 && ix<MELTGLOB__LASTWIRED)
    return (melt_ptr_t) melt_globalptrs[ix];
  return NULL;
}

static inline void melt_store_predefined(int ix, melt_ptr_t p)
{
  if (ix < 0)
    return;
  gcc_assert (ix>0 && ix<MELTGLOB__LASTWIRED);
  melt_globalptrs[ix] = p;
  melt_touchedglobalchunk[ix / MELT_GLOBAL_ENTRY_CHUNK] = true;
}

#define melt_globpredef(Ix) ((void*)melt_fetch_predefined((Ix)))


/* this is useful in generated MELT code */
#define MELTPREDEFIX(Tab,Glob) Tab[MELTGLOB_##Glob]


/* return the discriminant or class itself */
static inline meltobject_ptr_t
melt_discr (melt_ptr_t p)
{
  if (!p)
    return (meltobject_ptr_t) MELT_PREDEF (DISCR_NULL_RECEIVER);
  return p->u_discr;
}

bool melt_is_subclass_of (meltobject_ptr_t subclass_p,
                          meltobject_ptr_t superclass_p);

static inline bool
melt_is_instance_of (melt_ptr_t inst_p, melt_ptr_t class_p)
{
  unsigned mag_class = 0;
  unsigned mag_inst = 0;
  meltobject_ptr_t curdiscr_p = NULL;
  if (!class_p)
    return FALSE;
  if (class_p == MELT_PREDEF (DISCR_ANY_RECEIVER))
    return TRUE;
  gcc_assert (class_p->u_discr != NULL);
  if (!inst_p)
    {
      /* The null value is conceptually an instance of DISCR_NULL_RECEIVER, which is a subtype of DISCR_ANY_RECEIVER. */
      if (class_p == MELT_PREDEF (DISCR_NULL_RECEIVER))
        return TRUE;
      return FALSE;
    }
  mag_class = class_p->u_discr->obj_num;
  if (mag_class != MELTOBMAG_OBJECT || ((meltobject_ptr_t) class_p)->obj_len < MELTLENGTH_CLASS_DISCRIMINANT)
    return FALSE;
  curdiscr_p = inst_p->u_discr;
  /* We need to loop to handle the case of non-object discriminant
     type hierarchies; for instance DISCR_FIELD_SEQUENCE is a sub-type
     of DISCR_MULTIPLE.  This while loop is often run once.  */
  while (curdiscr_p)
    {
      if (curdiscr_p == (meltobject_ptr_t) class_p)
        return TRUE;
      mag_inst = curdiscr_p->obj_num;
      if (mag_class != MELTOBMAG_OBJECT || !mag_inst)
        return FALSE;
      if (mag_inst != ((meltobject_ptr_t) class_p)->meltobj_magic)
        return FALSE;
      if (mag_inst == MELTOBMAG_OBJECT)
        return melt_is_subclass_of (((meltobject_ptr_t) curdiscr_p),
                                    ((meltobject_ptr_t) class_p));
      /* the instance is not an object, but the current discriminant
         might have a super discriminant. */
      gcc_assert (curdiscr_p->obj_len >= MELTLENGTH_CLASS_DISCRIMINANT);
      curdiscr_p = (meltobject_ptr_t) (curdiscr_p->obj_vartab[MELTFIELD_DISC_SUPER]);
    }
  return FALSE;
}



/* since melt_put_int uses DISCR_CONSTANT_INTEGER it should be here */
static inline bool
melt_put_int (melt_ptr_t v, long x)
{
  if (!v)
    return FALSE;
  switch (melt_magic_discr (v))
    {
    case MELTOBMAG_INT:
      if (v->u_discr == (meltobject_ptr_t)MELT_PREDEF(DISCR_CONSTANT_INTEGER))
        return FALSE;
      ((struct meltint_st *) (v))->val = x;
      return TRUE;
    case MELTOBMAG_MIXINT:
      ((struct meltmixint_st *) (v))->intval = x;
      return TRUE;
    case MELTOBMAG_MIXLOC:
      ((struct meltmixloc_st *) (v))->intval = x;
      return TRUE;
    case MELTOBMAG_OBJECT:
      if (((meltobject_ptr_t) (v))->obj_num != 0)
        return FALSE;
      ((meltobject_ptr_t) (v))->obj_num = (unsigned short) x;
      return TRUE;
    default:
      return FALSE;
    }
}





////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/***
 * CALL FRAMES
 ***/


class Melt_CallFrame;
class Melt_CallProtoFrame;
MELT_EXTERN Melt_CallProtoFrame* melt_top_call_frame;

class Melt_CallProtoFrame
{
  friend void melt_minor_copying_garbage_collector (size_t wanted);
  friend void melt_garbcoll (size_t wanted, enum melt_gckind_en gckd);
  friend void melt_marking_callback (void *, void*);
  virtual void melt_forward_values (void) =0;
  virtual void melt_mark_ggc_data (void) =0;
protected:
  // this is tricky but essential; we need to explicitly zero the rest of the frame.
  void melt_clear_rest_of_frame (size_t sz)
  {
    if (sz > sizeof(Melt_CallProtoFrame))
      memset ((char*)this+sizeof(Melt_CallProtoFrame), 0,
              sz - sizeof(Melt_CallProtoFrame));
  };
  Melt_CallProtoFrame* _meltcf_prev;
public:
  const char* mcfr_flocs;
protected:
#if ENABLE_CHECKING
  const char* _meltcf_dbgfile;
  const long _meltcf_dbgline;
  const long _meltcf_dbgserial;
  static FILE* _dbgcall_file_;
  static long _dbgcall_count_;
#endif /*ENABLE_CHECKING*/
public:
  static Melt_CallProtoFrame* top_call_frame()
  {
    return melt_top_call_frame;
  };
  inline static int top_call_depth(void);
  inline static bool top_call_deeper_than (int d);
  Melt_CallProtoFrame* previous_frame() const
  {
    return _meltcf_prev;
  };
  int call_depth() const
  {
    int d=0;
    for (const Melt_CallProtoFrame* fr = this;
         fr != NULL;
         fr = fr->previous_frame())
      d++;
    return d;
  };
  bool call_deeper_than (int d) const
  {
    for (const Melt_CallProtoFrame* fr = this;
         fr != NULL;
         fr = fr->previous_frame())
      if (d-- <= 0)
        return true;
    return false;
  }
  const char* srcloc() const
  {
    return mcfr_flocs;
  };
  void set_srcloc(const char*s)
  {
    mcfr_flocs = s;
  };
#if ENABLE_CHECKING
  const char* dbg_file() const
  {
    return _meltcf_dbgfile;
  };
  long dbg_line() const
  {
    return  _meltcf_dbgline;
  };
  long dbg_serial() const
  {
    return _meltcf_dbgserial;
  };
  Melt_CallProtoFrame(const char*file, int lin, size_t sz)
    : _meltcf_prev(melt_top_call_frame), mcfr_flocs(NULL),
      _meltcf_dbgfile (melt_basename (file)), _meltcf_dbgline(lin),
      _meltcf_dbgserial(++_dbgcall_count_)
  {
    if (MELT_UNLIKELY( _dbgcall_file_ != NULL))
      {
        fprintf (_dbgcall_file_, "+ %s:%d #%ld S%d\n", _meltcf_dbgfile, lin, _meltcf_dbgserial, (int)sz);
        fflush (_dbgcall_file_);
      }
    melt_clear_rest_of_frame (sz);
    melt_top_call_frame = this;
  };
  Melt_CallProtoFrame(size_t sz)
    : _meltcf_prev(melt_top_call_frame), mcfr_flocs(NULL),
      _meltcf_dbgfile(NULL), _meltcf_dbgline(0), _meltcf_dbgserial(++_dbgcall_count_)
  {
    melt_clear_rest_of_frame (sz);
    if (MELT_UNLIKELY( _dbgcall_file_ != NULL))
      {
        fprintf (_dbgcall_file_, "+ * #%ld S%d\n", _meltcf_dbgserial, (int)sz);
        fflush (_dbgcall_file_);
      }
    melt_top_call_frame = this;
  };
  void debug_hook (melthook_ptr_t hk)
  {
    if (MELT_UNLIKELY( _dbgcall_file_ != NULL))
      {
        if (hk)
          {
            fprintf(_dbgcall_file_, "!Hook @%p %s\n", (void*)hk, hk->hookname);
            fflush (_dbgcall_file_);
          }
      }
  }
  void debug_closure (meltclosure_ptr_t clo)
  {
    if (MELT_UNLIKELY( _dbgcall_file_ != NULL))
      {
        if (clo && clo->rout)
          {
            fprintf(_dbgcall_file_, "!Closure @%p %s\n", (void*) clo, clo->rout->routdescr);
            fflush (_dbgcall_file_);
          }
      }
  }
#else /*!ENABLE_CHECKING*/
  Melt_CallProtoFrame(const char*, int, size_t sz)
    : _meltcf_prev(melt_top_call_frame), mcfr_flocs(NULL)
  {
    melt_clear_rest_of_frame (sz);
    melt_top_call_frame = this;
  };
  Melt_CallProtoFrame(size_t sz)
    : _meltcf_prev(melt_top_call_frame), mcfr_flocs(NULL)
  {
    melt_clear_rest_of_frame (sz);
    melt_top_call_frame = this;
  };
  void debug_hook (melthook_ptr_t) {};
  void debug_closure (meltclosure_ptr_t) {};
  const char* dbg_file() const { return NULL; };
  long dbg_line() const { return  0; };
#endif  /*ENABLE_CHECKING*/
  ~Melt_CallProtoFrame()
  {
#if ENABLE_CHECKING
    if (MELT_UNLIKELY( _dbgcall_file_ != NULL))
      {
        fprintf (_dbgcall_file_, "- %s:%d\n",  _meltcf_dbgfile, (int)_meltcf_dbgline);
        fflush (_dbgcall_file_);
      }
#endif /*ENABLE_CHECKING*/
    mcfr_flocs = NULL;
    melt_top_call_frame = _meltcf_prev;
  }
#if ENABLE_CHECKING
public:
  static void set_debug_file(FILE*f)
  {
    if (MELT_UNLIKELY( _dbgcall_file_ != NULL))
      {
        fflush (_dbgcall_file_);
        if (f == NULL)
          {
            fprintf (_dbgcall_file_, "#eof #%ld\n", _dbgcall_count_);
            fclose (_dbgcall_file_);
          }
      }
    _dbgcall_file_ = f;
  };
#endif /*ENABLE_CHECKING*/
};				// end class Melt_CallProtoFrame

int Melt_CallProtoFrame::top_call_depth()
{
  return melt_top_call_frame->call_depth();
}
bool Melt_CallProtoFrame::top_call_deeper_than (int d)
{
  return melt_top_call_frame->call_deeper_than(d);
}

class Melt_CallFrame : public Melt_CallProtoFrame
{
  friend void melt_minor_copying_garbage_collector (size_t wanted);
  friend void melt_garbcoll (size_t wanted, enum melt_gckind_en gckd);
  friend void melt_marking_callback (void *, void*);
public:
  union
  {
    meltclosure_ptr_t mcfr_clos;
    melthook_ptr_t mcfr_hook;
    melt_ptr_t mcfr_current;
  };
  melt_ptr_t current() const
  {
    return mcfr_current;
  };
  meltclosure_ptr_t current_closure() const
  {
    return meltcast_meltclosure_st (mcfr_current);
  };
  melthook_ptr_t current_hook() const
  {
    return meltcast_melthook_st (mcfr_current);
  };
protected:
#if ENABLE_CHECKING
  Melt_CallFrame(const char*file, int lin, size_t sz, meltclosure_ptr_t clos=NULL)
    : Melt_CallProtoFrame(file,lin,sz), mcfr_clos(clos)
  {
    debug_closure (clos);
  };
  Melt_CallFrame(const char*file, int lin, size_t sz, melthook_ptr_t hook)
    : Melt_CallProtoFrame(file,lin,sz), mcfr_hook(hook)
  {
    debug_hook (hook);
  };
  Melt_CallFrame(const char*file, int lin, size_t sz)
    : Melt_CallProtoFrame(file,lin,sz), mcfr_current(NULL)
  {
  };
#else /*!ENABLE_CHECKING*/
  Melt_CallFrame(const char*, int, size_t sz, meltclosure_ptr_t clos=NULL)
    : Melt_CallProtoFrame(sz), mcfr_clos(clos)
  {
    debug_closure (clos);
  };
  Melt_CallFrame(const char*, int, size_t sz, melthook_ptr_t hook)
    : Melt_CallProtoFrame(sz), mcfr_hook(hook)
  {
    debug_hook (hook);
  };
  Melt_CallFrame(const char*, int , size_t sz)
    : Melt_CallProtoFrame(sz), mcfr_current(NULL)
  {
  };
#endif /*ENABLE_CHECKING*/
  Melt_CallFrame(size_t sz, meltclosure_ptr_t clos)
    : Melt_CallProtoFrame(sz), mcfr_clos(clos)
  {
  };
  Melt_CallFrame(size_t sz, melthook_ptr_t hook)
    : Melt_CallProtoFrame(sz), mcfr_hook(hook)
  {
  };
  Melt_CallFrame(size_t sz)
    : Melt_CallProtoFrame(sz), mcfr_current(NULL)
  {
  };
  ~Melt_CallFrame()
  {
  };
};				// end class Melt_CallFrame.

template<unsigned NbVal> class Melt_CallFrameWithValues
  : public Melt_CallFrame
{
public:
  melt_ptr_t mcfr_varptr[(NbVal>0)?NbVal:1];
  virtual void melt_forward_values (void)
  {
#if MELT_HAVE_DEBUG
    if (dbg_file())
      melt_debuggc_eprintf("forwarding %d values call frame @%p from %s:%ld #%ld",
                           NbVal, (void*) this, dbg_file(), dbg_line(), dbg_serial());
    else
      melt_debuggc_eprintf("forwarding %d values call frame @%p #%ld",
                           NbVal, (void*) this, dbg_serial());
#endif /*MELT_HAVE_DEBUG*/
    MELT_FORWARDED (mcfr_current);
    for (unsigned ix=0; ix<NbVal; ix++)
      MELT_FORWARDED (mcfr_varptr[ix]);
  };
  void melt_mark_values (void)
  {
    if (mcfr_current)
      gt_ggc_mx_melt_un (mcfr_current);
    for (unsigned ix=0; ix<NbVal; ix++)
      if (mcfr_varptr[ix] != NULL)
        gt_ggc_mx_melt_un (mcfr_varptr[ix]);
  };
  virtual void melt_mark_ggc_data (void)
  {
    melt_mark_values ();
  };
#if ENABLE_CHECKING
  Melt_CallFrameWithValues(const char*fil, int lin,
                           size_t sz, meltclosure_ptr_t clos=NULL)
    : Melt_CallFrame(fil, lin, sz, clos) {};
  Melt_CallFrameWithValues(const char*fil, int lin, size_t sz, melthook_ptr_t hook)
    : Melt_CallFrame(fil, lin, sz, hook) {};
#endif /*ENABLE_CHECKING*/
  Melt_CallFrameWithValues(size_t sz, meltclosure_ptr_t clos=NULL)
    : Melt_CallFrame(sz,clos) {};
  Melt_CallFrameWithValues(size_t sz, melthook_ptr_t hook)
    : Melt_CallFrame(sz,hook) {};
  ~Melt_CallFrameWithValues() {};
};				// end template class Melt_CallFrameWithValues


#if ENABLE_CHECKING

#define MELT_ENTERFRAME_AT(NbVar,Clos,Lin)			\
  /* classy enter frame */					\
  Melt_CallFrameWithValues<NbVar> meltfram__			\
     (__FILE__, Lin, sizeof(Melt_CallFrameWithValues<NbVar>),	\
     meltcast_meltclosure_st((melt_ptr_t)(Clos)));		\
  if (MELT_HAVE_DEBUG) {					\
    static char meltlocbuf_##Lin [92];				\
    if (MELT_UNLIKELY(!meltlocbuf_##Lin [0]))			\
      snprintf (meltlocbuf_##Lin, sizeof(meltlocbuf_##Lin),	\
		"%s:%d ~%s", melt_basename (__FILE__),		\
		Lin, __func__);					\
    meltfram__.mcfr_flocs = meltlocbuf_##Lin; }

#else /*!ENABLE_CHECKING*/

#define MELT_ENTERFRAME_AT(NbVar,Clos,Lin)			\
  /* classy enter frame */					\
  Melt_CallFrameWithValues<NbVar> meltfram__			\
    (sizeof(Melt_CallFrameWithValues<NbVar>),			\
     meltcast_meltclosure_st((melt_ptr_t)(Clos)));		\
  if (MELT_HAVE_DEBUG) {					\
    static char meltlocbuf_##Lin [92];				\
    if (MELT_UNLIKELY(!meltlocbuf_##Lin [0]))			\
      snprintf (meltlocbuf_##Lin, sizeof(meltlocbuf_##Lin),	\
		"%s:%d ~%s", melt_basename (__FILE__),		\
		Lin, __func__);					\
    meltfram__.mcfr_flocs = meltlocbuf_##Lin; }

#endif /*ENABLE_CHECKING*/

#define MELT_ENTERFRAME(NBVAR,CLOS) MELT_ENTERFRAME_AT(NBVAR,CLOS,__LINE__)
#define MELT_ENTEREMPTYFRAME(CLOS) MELT_ENTERFRAME_AT(0,CLOS,__LINE__)
#define MELT_EXITFRAME() do {meltfram__.mcfr_flocs = NULL;}while(0)

static inline int
melt_curframdepth (void)
{
  int cnt = 0;
  for (Melt_CallProtoFrame* cfr = Melt_CallFrame::top_call_frame();
       cfr != NULL;
       cfr=cfr->previous_frame())
    cnt++;
  return cnt;
}

////================================================================


/* MELT location macros should work with both oldstyle and classy
   frames. They use "if (MELT_HAVE_DEBUG)" not "#if MELT_HAVE_DEBUG"
   so the optimizer compiling them would remove the dead code when not
   debugging. */

#define MELT_LOCATION(LOCS) do{			\
    if (MELT_HAVE_DEBUG)			\
       meltfram__.mcfr_flocs = LOCS;		\
}while(0)

#define MELT_LOCATION_HERE_AT(FIL,LIN,MSG) do {		\
  if (MELT_HAVE_DEBUG) {				\
    static char locbuf_##LIN[92];			\
    if (!MELT_UNLIKELY(locbuf_##LIN[0]))		\
      snprintf(locbuf_##LIN, sizeof(locbuf_##LIN),	\
	       "%s:%d <%s>",				\
	       melt_basename (FIL), (int)LIN, MSG);	\
    meltfram__.mcfr_flocs = locbuf_##LIN;		\
  }							\
} while(0)

/* We need several indirections of macro to have the ##LIN trick above
   working!  */
#define MELT_LOCATION_HERE_AT_MACRO(FIL,LIN,MSG) \
  MELT_LOCATION_HERE_AT(FIL,LIN,MSG)
#define MELT_LOCATION_HERE_MACRO(MSG)  \
  MELT_LOCATION_HERE_AT_MACRO(__FILE__,__LINE__,MSG)
#define MELT_LOCATION_HERE(MSG)  MELT_LOCATION_HERE_MACRO(MSG)

/* SBUF should be a local array of char */
#define MELT_LOCATION_HERE_PRINTF_AT(SBUF,FIL,LIN,FMT,...) do {	\
  if (MELT_HAVE_DEBUG) {					\
    memset (SBUF, 0, sizeof(SBUF));				\
    snprintf (SBUF, sizeof(SBUF),				\
	      "%s:%d:: " FMT,					\
	      melt_basename (FIL),				\
	      (int)LIN, __VA_ARGS__);				\
    meltfram__.mcfr_flocs = SBUF;				\
  }} while(0)
/* We need several indirections of macro to have the ##LIN trick above
   working!  */
#define MELT_LOCATION_HERE_PRINTF_AT_MACRO(SBUF,FIL,LIN,FMT,...)	\
  MELT_LOCATION_HERE_PRINTF_AT(SBUF,FIL,LIN,FMT,__VA_ARGS__)
#define MELT_LOCATION_HERE_PRINTF_MACRO(SBUF,FMT,...)			\
  MELT_LOCATION_HERE_PRINTF_AT_MACRO(SBUF,__FILE__,__LINE__,FMT,__VA_ARGS__)
#define MELT_LOCATION_HERE_PRINTF(SBUF,FMT,...)		\
  MELT_LOCATION_HERE_PRINTF_MACRO(SBUF,FMT, __VA_ARGS__)


////////////////////////////////////////////////////////////////


/* Internal unction to be called by MELT code when the
   :sysdata_inchannel_data is changed.  Called by code_chunk-s inside
   MELT file melt/warmelt-base.melt.  */
void meltgc_notify_inchannel_data (void);


/* convert a MELT value to a plugin flag or option */
unsigned long melt_val2passflag(melt_ptr_t val_p);

MELT_EXTERN long melt_pass_instance_counter;
MELT_EXTERN long melt_current_pass_index_var;
// current_pass is not always enough. See http://gcc.gnu.org/ml/gcc/2014-03/msg00141.html
MELT_EXTERN opt_pass *melt_current_pass_ptr;

/* ====== safer output routines ===== */

/* output a string */
static inline void
melt_puts (FILE * f, const char *str)
{
  if (f && str)
    fputs (str, f);
}

/* output a number with a prefix & suffix message */
static inline void
melt_putnum(FILE* f, const char*pref, long l, const char*suff)
{
  if (f)
    fprintf(f, "%s%ld%s", pref?pref:"", l, suff?suff:"");
}

/* safe flush */
static inline void
melt_flush (FILE * f)
{
  if (f)
    fflush (f);
}

/* safe newline and flush */
static inline void
melt_newlineflush (FILE * f)
{
  if (f)
    {
      putc ('\n', f);
      fflush (f);
    }
}

/* output a string value */
static inline void
melt_putstr (FILE * f, melt_ptr_t sv)
{
  if (f && sv && melt_magic_discr (sv) == MELTOBMAG_STRING)
    fputs (((struct meltstring_st *) sv)->val, f);
}

/* output a string buffer */
static inline void
melt_putstrbuf (FILE * f, melt_ptr_t sb)
{
  struct meltstrbuf_st *sbuf = (struct meltstrbuf_st *) sb;
  if (f && sbuf && melt_magic_discr ((melt_ptr_t) sbuf) == MELTOBMAG_STRBUF)
    {
      gcc_assert (sbuf->bufzn);
      if (!sbuf->bufzn || sbuf->bufend <= sbuf->bufstart)
        return;
      fwrite (sbuf->bufzn + sbuf->bufstart, sbuf->bufend - sbuf->bufstart, 1,
              f);
    }
}

/* wait for the probe. waitopt is often WNOHANG; see waitpid(2);
   return 0 if the wait was successful. */
int melt_wait_for_probe (int waitopt);

/* Stop the MELT probe process */
void melt_probe_stop (void);


/* Start the MELT probe process, and returns its pid.  Gives the file descriptors thru
   toprobefdptr & fromprobefdptr if not null.  If probecmd is null or
   empty, we use the -f[plugin-arg-]melt-probe program argument, or
   else the GCCMELT_PROBE environment variable, or else the builtin
   default melt_default_probe. */
long melt_probe_start (const char* probecmd, int*toprobefdptr, int *fromprobefdptr);

/* Send a stringbuffer command to probe */
void melt_send_command_strbuf_to_probe (melt_ptr_t buf);

/* output the option, declaration and implementation buffers of a
   generated file with a secondary rank*/
void
melt_output_cfile_decl_impl_secondary_option (melt_ptr_t cfilnam,
    melt_ptr_t declbuf,
    melt_ptr_t implbuf,
    melt_ptr_t optbuf,
    int filrank);

static inline void
melt_output_cfile_decl_impl_secondary (melt_ptr_t cfilnam,
                                       melt_ptr_t declbuf, melt_ptr_t implbuf, int filrank)
{
  melt_output_cfile_decl_impl_secondary_option (cfilnam, declbuf, implbuf, (melt_ptr_t)0, filrank);
}

/* likewise, for the primary file */
static inline void
melt_output_cfile_decl_impl(melt_ptr_t cfilnam,
                            melt_ptr_t declbuf, melt_ptr_t implbuf)
{
  melt_output_cfile_decl_impl_secondary_option (cfilnam, declbuf, implbuf, (melt_ptr_t)0, 0);
}

/* recursive function to output to a file.  Handle boxed integers,
   lists, tuples, strings, strbufs, but don't handle objects! */
void meltgc_output_file (FILE* fil, melt_ptr_t val_p);

#ifdef MELT_HAVE_DEBUG
static inline void
debugeputs_at (const char *fil, int lin, const char *msg)
{
  debugeprintf_raw ("!@%s:%d:\n@! %s\n", basename (fil), lin, msg);
}

#define debugeputs(Msg) debugeputs_at(__FILE__,__LINE__,(Msg))
#else
#define debugeputs(Msg) ((void) 0)
#endif /* MELT_HAVE_DEBUG */

static inline void
debugvalue_at (const char *fil, int lin, const char *msg, void *val)
{
  if (melt_flag_debug)
    {
      if (melt_dbgcounter++ > melt_debugskipcount)
	melthookproc_HOOK_LOW_DEBUG_VALUE_AT((melt_ptr_t)val, fil, lin, msg, melt_dbgcounter);
      fflush (stderr);
    }
}
// deprecated macro
#define debugvalue(Msg,Val) debugvalue_at(__FILE__, __LINE__, (Msg), (Val))


static inline void
debugmsgval_at (const char*fil, int lin, const char* msg, void*val, long count)
{
  if (melt_flag_debug)
    {
	melthookproc_HOOK_LOW_DEBUG_VALUE_AT((melt_ptr_t)val, fil, lin, msg, count);
    }
}

// deprecated macro
#define debugmsgval(Msg,Val,Count) do {					\
    debugmsgval_at(__FILE__,__LINE__,(Msg),(Val),(Count)); } while(0)

static inline void
debugbacktrace_at (const char *fil, int lin, const char *msg, int depth)
{
  if (melt_flag_debug)
    {
      fprintf (stderr, "\n!@%s:%d: %s ** BACKTRACE** ",
               basename (fil), lin, msg);
      melt_dbgbacktrace (depth);
      fflush (stderr);
    }
}

#define debugbacktrace(Msg,Depth) debugbacktrace_at(__FILE__, __LINE__, (Msg), (Depth))

static inline void
debugnum_at (const char *fil, int lin, const char *msg, long val)
{
  debugeprintf_raw ("!@%s:%d: %s =#= %ld\n", basename (fil), lin, msg, val);
}

#define debugnum(Msg,Val) debugnum_at(__FILE__, __LINE__, (Msg), (Val))


void melt_dbgshortbacktrace(const char* msg, int maxdepth);


#define melt_warn_for_no_expected_secondary_results() \
  melt_warn_for_no_expected_secondary_results_at(__FILE__,__LINE__)

void melt_warn_for_no_expected_secondary_results_at (const char*fil, int lin);


#if ENABLE_CHECKING
extern void* melt_checkedp_ptr1;
extern void* melt_checkedp_ptr2;

void melt_caught_assign_at(void*ptr, const char*fil, int lin, const char*msg);

#define melt_checked_assignmsg_at(Assign,Fil,Lin,Msg) do {              \
      void* p_##Lin = (Assign);                                         \
	if (p_##Lin && !melt_discr((melt_ptr_t) p_##Lin))		\
        melt_assert_failed("bad checked assign (in runtime)",Fil,Lin,   \
                  __FUNCTION__);					\
      if ( (p_##Lin == melt_checkedp_ptr1 && p_##Lin)			\
           ||  (p_##Lin == melt_checkedp_ptr2 && p_##Lin))              \
        melt_caught_assign_at(p_##Lin,Fil,Lin,Msg);  } while(0)

#define melt_checked_assign_internal(Assign) melt_checked_assignmsg_at((Assign),__FILE__,__LINE__,__FUNCTION__)
#define melt_checked_assignmsg_internal(Assign,Msg) melt_checked_assignmsg_at((Assign),__FILE__,__LINE__,Msg)

#define melt_checked_assign(Assign) melt_checked_assign_internal(Assign)
#define melt_checked_assignmsg(Assign,Msg) melt_checked_assignmsg_internal(Assign,Msg)

void melt_cbreak_at(const char*msg, const char*fil, int lin);
#define melt_cbreak(Msg) melt_cbreak_at((Msg),__FILE__,__LINE__)
#define melt_trace_start(Msg,Cnt) do {} while(0)
#define melt_trace_end(Msg,Cnt) do {} while(0)
#else /*!ENABLE_CHECKING*/
#define melt_checked_assign(Assign) Assign
#define melt_checked_assignmsg(Assign,Msg) Assign
#define melt_cbreak(Msg) ((void)(Msg))
#define melt_trace_start(Msg,Cnt) do{}while(0)
#define melt_trace_end(Msg,Cnt) do{}while(0)
#undef debugmsgval
#define debugmsgval(Msg,Val,Count) do {}while(0)
#endif /*ENABLE_CHECKING*/



/* make a new boxed file - the discr should be for a file or a raw
   file */
melt_ptr_t meltgc_new_file(melt_ptr_t discr_p, FILE* fil);

/* get a file from a boxed file, may return NULL */
static inline FILE*
melt_get_file(melt_ptr_t file_p)
{
  int magic;
  if (!file_p)
    return NULL;
  magic = melt_magic_discr (file_p);
  if (magic == MELTOBMAG_SPECIAL_DATA)
    {
      unsigned k = melt_special_kind (file_p);
      if (k == meltpydkind_file || k == meltpydkind_rawfile)
        return ((struct meltspecialdata_st*)file_p)->meltspec_payload.meltpayload_file1;
    }

  return NULL;
}


/* get some slot in the initial system data */
static inline melt_ptr_t
melt_get_inisysdata(int off)
{
  meltobject_ptr_t inisys = (meltobject_ptr_t) MELT_PREDEF(INITIAL_SYSTEM_DATA);
  if (melt_magic_discr ((melt_ptr_t) inisys) == MELTOBMAG_OBJECT)
    {
      int leninisys = inisys->obj_len;
      gcc_assert(melt_is_instance_of
                 ((melt_ptr_t) inisys,
                  (melt_ptr_t) MELT_PREDEF (CLASS_SYSTEM_DATA)));
      if (off>=0 && off<leninisys)
        return inisys->obj_vartab[off];
    }
  return NULL;
}



FILE* meltgc_set_dump_file (FILE* dumpf);
void meltgc_restore_dump_file (FILE* oldf);

/*** MELT interface to walk_gimple_seq; using the given DATA as first
     argument to closures, walk recursively inside gimple_seq GSEQ,
     applying closure STMTCLOS_P to statements and closure TREECLOS_P
     to trees if given. The UNIQUETREEVISIT flag is set if we don't
     want to visit tree operands several times. The STMTCLOS_P gets
     the DATA and the raw current GIMPLE as argument, and return a
     boolean value as primary result, and the replacing GIMPLE, if any,
     as secondary result. The TREECLOS_P gets the DATA and the TREE
     as arguments, and return a boolean value, with a sub-tree walking
     integer flag and the replacing tree as secondary results.
 ***/
gimple
meltgc_walk_gimple_seq (melt_ptr_t data_p, gimple_seq gseq, melt_ptr_t stmtclos_p, melt_ptr_t treeclos_p, bool uniquetreevisit);

#if ENABLE_CHECKING
/* some useless routines in wich we can add a breakpoint from gdb. */
MELT_EXTERN void melt_sparebreakpoint_0_at (const char*fil, int lin, void*ptr, const char*msg);
MELT_EXTERN void melt_sparebreakpoint_1_at (const char*fil, int lin, void*ptr, const char*msg);
MELT_EXTERN void melt_sparebreakpoint_2_at (const char*fil, int lin, void*ptr, const char*msg);
#define melt_sparebreakpoint_0(P,Msg) melt_sparebreakpoint_0_at(__FILE__,__LINE__,(void*)(P),(Msg))
#define melt_sparebreakpoint_1(P,Msg) melt_sparebreakpoint_1_at(__FILE__,__LINE__,(void*)(P),(Msg))
#define melt_sparebreakpoint_2(P,Msg) melt_sparebreakpoint_2_at(__FILE__,__LINE__,(void*)(P),(Msg))
#else /*no ENABLE_CHECKING*/
#define melt_sparebreakpoint_0(P,Msg) do{(void)(0 && (P));}while(0)
#define melt_sparebreakpoint_1(P,Msg) do{(void)(0 && (P));}while(0)
#define melt_sparebreakpoint_2(P,Msg) do{(void)(0 && (P));}while(0)
#endif /*ENABLE_CHECKING*/

/* strangely, gcc/input.h don't define yet that macro. */
#define LOCATION_COLUMN(LOC) ((expand_location (LOC)).column)

extern const char melt_run_preprocessed_md5[]; /* defined in generated file melt-run-md5.h */

/* compatibility with old stuff */
#define flag_melt_bootstrapping melt_flag_bootstrapping
#define flag_melt_debug melt_flag_debug


/* With GCC 4.8, the gimple_seq are disappearing because they are the
same as gimple (with file "coretypes.h" having the definition `typedef
gimple gimple_seq;`), but our generated runtime support might still
want their old marking routine.  */

#if MELT_GCC_VERSION >= 4008
extern void melt_gt_ggc_mx_gimple_seq_d(void*);
#define gt_ggc_mx_gimple_seq_d melt_gt_ggc_mx_gimple_seq_d
#endif /* GCC 4.8 */

#if MELT_GCC_VERSION == 4009
/* Orobably temporary */
#define gt_ggc_mx_gimple_statement_d gt_ggc_m_21gimple_statement_base
#endif /* GCC 4.9 */

#endif /*MELT_INCLUDED_ */

/* eof $Id$ */
