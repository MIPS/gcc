/*
 * w32-shared-ptr.h
 *
 * Contributors:
 *  Created by Thomas Pfaff <tpfaff@gmx.net>
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

#ifndef GCC_W32_SHARED_PTR_H
#define GCC_W32_SHARED_PTR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int size;
  
  void *terminate;
  void *unexpected;

  void *dw2_unseen_objects;
  void *dw2_seen_objects;
  __gthread_mutex_t dw2_object_mutex;
#ifdef __GTHREAD_MUTEX_INIT_FUNCTION
  __gthread_once_t dw2_once;
#endif
  void *dw2_marker;

#ifdef __USING_SJLJ_EXCEPTIONS__ 
  void* sjlj_fc_static;
#if __GTHREADS
  __gthread_key_t sjlj_fc_key;
  int sjlj_use_fc_key;
  __gthread_once_t sjlj_once;
#endif
#endif

} W32_EH_SHARED;

extern W32_EH_SHARED *__w32_sharedptr;

extern void ** __w32_sharedptr_terminate;
extern void ** __w32_sharedptr_unexpected;

extern void __w32_sharedptr_initialize (void);

#define W32_SHAREDPTR_INITIALIZE()  if (!__w32_sharedptr) __w32_sharedptr_initialize ()

#ifdef __cplusplus
}
#endif

#endif
