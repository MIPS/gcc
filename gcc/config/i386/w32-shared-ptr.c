/*
 * w32-shared-ptr.c
 *
 * Share informations between EXE/DLLs without using an
 * additional DLL.
 *
 * Contributors:
 *  Created by Adriano dos Santos Fernandes <adrianosf@uol.com.br>
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

#include <stdlib.h>
#include <string.h>
#ifdef DEBUG
#include <stdio.h>
#endif

/*
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
*/

typedef unsigned short ATOM;
ATOM __attribute__ ((stdcall)) AddAtomA(const char*);
ATOM __attribute__  ((stdcall)) FindAtomA(const char*);
unsigned int __attribute__  ((stdcall)) GetAtomNameA(ATOM,char*,int);


#define SHAREDPTR_LETTER0   'a'
#define SHAREDPTR_LETTER1   'A'

static void **  __w32_sharedptr = NULL;

void ** __w32_sharedptr_terminate = NULL;
void ** __w32_sharedptr_unexpected  = NULL;
void ** __w32_sharedptr_unseen_objects  = NULL;
void ** __w32_sharedptr_seen_objects  = NULL;

#define SHAREDPTR_TERMINATE		1
#define SHAREDPTR_UNEXPECTED		2
#define SHAREDPTR_UNSEEN_OBJECTS	3
#define SHAREDPTR_SEEN_OBJECTS		4

#define SHAREDPTR_COUNT			5

static unsigned int  __ultobstr (unsigned long, char *);
extern void __w32_sharedptr_initialize (void);

/* cygwin doesn't have ultoa, so use a custom version
   for this specific job */
static __inline__ unsigned int  __ultobstr (unsigned long val, char *buf)
{
  char temp[sizeof(unsigned long) * 8 + 1];
  char* p = temp;
  unsigned int len = 0;
  if (val == 0)
    *p++ = '0';
  else
    /* convert to bin digits, in reversed order*/
    for (; val > 0; val /= 2)
      *p++ = (char)(val % 2) + '0';

  /* reverse copy */  
  while(p > temp)
  {
    *buf++ = *--p;
    len++;	
  }
  *buf = '\0';
  return len;
}

void __w32_sharedptr_initialize (void) 
{
  char s[sizeof(void*) * 8 + 1];
  ATOM atom;
  unsigned int i;

  if (__w32_sharedptr)
     return;

  memset (s, SHAREDPTR_LETTER0, sizeof(void*) * 8);
  s[sizeof(void*) * 8] = '\0';

  atom = FindAtomA (s);

  if (atom == 0) {
    char s0[sizeof(s)];
    unsigned int len;

    __w32_sharedptr = (void**)malloc (sizeof(void*) * SHAREDPTR_COUNT);
    memset(__w32_sharedptr, 0, sizeof(void*) * SHAREDPTR_COUNT);

   
    /* initialize to default handlers */
    *(__w32_sharedptr + SHAREDPTR_TERMINATE) = (void*)abort;
    *(__w32_sharedptr + SHAREDPTR_UNEXPECTED) = (void*)abort;

    len = __ultobstr ((unsigned long)__w32_sharedptr, s0);
    
    memset(s, SHAREDPTR_LETTER0, (sizeof(s) - 1) - len);

    for (i = 0; i < len; i++)
      s[sizeof(s) - 1 - len + i] =
	 (s0[i] == '0' ? SHAREDPTR_LETTER0 : SHAREDPTR_LETTER1);

    s[sizeof(s) - 1] = '\0';

#ifdef DEBUG
    printf ("%s: initialize sharedptr: (0x%x):%s\n",
             __FUNCTION__, __w32_sharedptr, s0);
#endif

    AddAtomA(s);
  }
  else {
    GetAtomNameA (atom, s, sizeof(s));

    for (i = 0; i < sizeof(s) - 1; i++)
       s[i] = (s[i] == SHAREDPTR_LETTER0 ? '0' : '1');

    __w32_sharedptr = (void*)strtoul (s, NULL, 2);

#ifdef DEBUG
    printf ("%s: got sharedptr (0x%x): %s\n",
	    __FUNCTION__, __w32_sharedptr, s);
#endif

  }
  /* assign global ptrs */
  __w32_sharedptr_terminate  = __w32_sharedptr + SHAREDPTR_TERMINATE;
  __w32_sharedptr_unexpected = __w32_sharedptr + SHAREDPTR_UNEXPECTED;
  __w32_sharedptr_unseen_objects = __w32_sharedptr
				   + SHAREDPTR_UNSEEN_OBJECTS;
  __w32_sharedptr_seen_objects  = __w32_sharedptr
				  + SHAREDPTR_SEEN_OBJECTS;
}
