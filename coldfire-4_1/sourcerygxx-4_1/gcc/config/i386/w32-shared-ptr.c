/*
 * w32-shared-ptr.c
 *
 * Share informations between EXE/DLLs without using an
 * additional DLL.
 *
 * Contributors:
 *  Created by Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  Enhanced by Thomas Pfaff <tpfaff@gmx.net>
 *
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRENTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAMED. This includes but is not limited to warrenties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifdef __MINGW32__
#undef _CRTIMP
#define _CRTIMP
#endif

#include <stdlib.h>
#include <string.h>
#ifdef __CYGWIN__
#include <unistd.h>
#endif
#ifdef DEBUG
#include <stdio.h>
#endif

#include "tconfig.h"
#include "tsystem.h"
#include "gthr.h"

#include "w32-shared-ptr.h"

#if defined (__CYGWIN__) /* FIXME: This shouldn't be necessary. */
typedef unsigned short ATOM;
ATOM __attribute__ ((stdcall)) AddAtomA(const char*);
ATOM __attribute__  ((stdcall)) FindAtomA(const char*);
unsigned int __attribute__  ((stdcall)) GetAtomNameA(ATOM,char*,int);
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <assert.h>

W32_EH_SHARED *  __w32_sharedptr;

void ** __w32_sharedptr_terminate;
void ** __w32_sharedptr_unexpected;


#define W32_ATOM_SUFFIX "-LIBGCCW32-EH"

#define VERSION_SUFFIX "-2"

#ifdef __USING_SJLJ_EXCEPTIONS__
#define SJLJ_SUFFIX "-SJLJ"
#else
#define SJLJ_SUFFIX
#endif

#if __GTHREADS
#define GTHR_SUFFIX "-GTHR"
#else
#define GTHR_SUFFIX
#endif

#ifdef __CYGWIN__
#define SYS_SUFFIX "-CYGWIN"
#else
#define SYS_SUFFIX "-MINGW32"
#endif

static const char w32_atom_suffix[]
  = W32_ATOM_SUFFIX VERSION_SUFFIX SJLJ_SUFFIX GTHR_SUFFIX SYS_SUFFIX;

#define SHAREDPTR_BIT1          'A'
#define SHAREDPTR_BIT0          'a'

#define SHAREDPTR_BITS          (sizeof(W32_EH_SHARED*)*8)
#define SHARED_ATOM_NAME_LEN    (SHAREDPTR_BITS + sizeof(w32_atom_suffix))

static ATOM __w32_sharedptr_set (W32_EH_SHARED *w32_sharedptr);
static W32_EH_SHARED * __w32_sharedptr_get (ATOM atom);
static void __w32_sharedptr_default_unexpected(void);

#ifdef __CYGWIN__
static void __w32_sharedptr_fixup_after_fork (void);
#endif

static void __w32_sharedptr_default_unexpected(void)
{
  ((void(*)(void))__w32_sharedptr->terminate)();
}

static void __w32_eh_shared_initialize (W32_EH_SHARED *w32_sharedptr)
{
#ifdef __GTHREAD_MUTEX_INIT
  static __gthread_mutex_t dw2_object_mutex = __GTHREAD_MUTEX_INIT;
#else
  static __gthread_mutex_t dw2_object_mutex;
#endif
#ifdef __GTHREAD_MUTEX_INIT_FUNCTION
  static __gthread_once_t dw2_once = __GTHREAD_ONCE_INIT;
#endif

#ifdef __USING_SJLJ_EXCEPTIONS__ 
#if __GTHREADS
  static __gthread_key_t sjl_fc_key;
  static __gthread_once_t sjl_once = __GTHREAD_ONCE_INIT;
#endif
#endif

  memset (w32_sharedptr, 0, sizeof(W32_EH_SHARED));

  w32_sharedptr->size = sizeof(W32_EH_SHARED);

  w32_sharedptr->terminate = (void*)abort;
  w32_sharedptr->unexpected = (void*)__w32_sharedptr_default_unexpected;

  w32_sharedptr->dw2_object_mutex = dw2_object_mutex;
#ifdef __GTHREAD_MUTEX_INIT_FUNCTION
  w32_sharedptr->dw2_once = dw2_once;
#endif

#ifdef __USING_SJLJ_EXCEPTIONS__
  w32_sharedptr->sjlj_fc_static = NULL;
#if __GTHREADS
  w32_sharedptr->sjlj_fc_key = sjl_fc_key;
  w32_sharedptr->sjlj_use_fc_key = -1;
  w32_sharedptr->sjlj_once = sjl_once;
#endif
#endif
}

void __w32_sharedptr_initialize (void) 
{
  W32_EH_SHARED *w32_sharedptr;
  char s[SHARED_ATOM_NAME_LEN];
  ATOM atom;

  if (__w32_sharedptr)
    return;

  memset (s, SHAREDPTR_BIT1, SHAREDPTR_BITS);
  memcpy (&(s[SHAREDPTR_BITS]), w32_atom_suffix, sizeof(w32_atom_suffix));

  atom = FindAtomA (s);
  if (atom)
    w32_sharedptr = __w32_sharedptr_get (atom);
  else
    {
      w32_sharedptr = (W32_EH_SHARED *) malloc (sizeof(W32_EH_SHARED));
      if (!w32_sharedptr)
        abort ();

      __w32_eh_shared_initialize (w32_sharedptr);

      if (__w32_sharedptr_set (w32_sharedptr))
        {
#ifdef __CYGWIN__
          /* recreate atom after fork */
          pthread_atfork (NULL,NULL,__w32_sharedptr_fixup_after_fork);
#endif
        }
      else
        {
          free (w32_sharedptr);
          w32_sharedptr = __w32_sharedptr_get (FindAtomA (s));
        }
    }

  __w32_sharedptr_terminate  = &w32_sharedptr->terminate;
  __w32_sharedptr_unexpected = &w32_sharedptr->unexpected;

  /* THIS MUST BE THE LAST STEP */
  __w32_sharedptr = w32_sharedptr;
}

static ATOM __w32_sharedptr_set (W32_EH_SHARED *w32_sharedptr)
{
  int i;
  int bit;
  char s[SHARED_ATOM_NAME_LEN];
  ATOM atom;

  /* This should optimize out entirely by O2 */
  if (sizeof(bit) != sizeof(w32_sharedptr))
    abort ();

  for (i = SHAREDPTR_BITS - 1, bit = 1; i >= 0; --i, bit <<= 1)
    s[i] = ((int)w32_sharedptr) & bit ? SHAREDPTR_BIT1 :
      SHAREDPTR_BIT0;

  memcpy (&(s[SHAREDPTR_BITS]), w32_atom_suffix, sizeof(w32_atom_suffix));

  atom = AddAtomA (s);
  /* Sanity check to avoid race by concurrent initialization */
  if (!atom || __w32_sharedptr_get (atom) != w32_sharedptr)
    return 0;

#ifdef DEBUG
  printf ("%s: set sharedptr: (%p):%s\n",
          __FUNCTION__,w32_sharedptr, s);
#endif

  return atom;
}

static W32_EH_SHARED *__w32_sharedptr_get (ATOM atom)
{
  W32_EH_SHARED *w32_sharedptr;
  int i;
  int bit;
  int address = 0;
  char s[SHARED_ATOM_NAME_LEN];

  /* If this fails something went wrong */
   assert (GetAtomNameA (atom, s, sizeof(s)) != 0);

  for (i = SHAREDPTR_BITS - 1, bit = 1; i >= 0; --i, bit <<= 1)
    address |= s[i] == SHAREDPTR_BIT1 ? bit : 0;
  w32_sharedptr = (W32_EH_SHARED*) address;

  assert (w32_sharedptr->size == sizeof(W32_EH_SHARED));

#ifdef DEBUG
  printf ("%s: got sharedptr (%p): %s\n",
          __FUNCTION__, w32_sharedptr, s);
#endif
  return w32_sharedptr;
}

#ifdef __CYGWIN__
static void __w32_sharedptr_fixup_after_fork (void)
{
  assert (__w32_sharedptr_set (__w32_sharedptr) != 0);
}
#endif
