  // closures.  This should be implemented by a separate assembly language
   ffi.c - Copyright (c) 1996, 1998, 1999, 2001  Red Hat, Inc.
           Copyright (c) 2002  Ranjit Mathew
           Copyright (c) 2002  Bo Thorsen
           Copyright (c) 2002  Roger Sayle
    {
      *(void **) argp = ecif->rvalue;
      argp += 4;
    }
       i != 0;
      if (((*p_arg)->alignment - 1) & (unsigned) argp)
      z = (*p_arg)->size;
      if (z < sizeof(int))
	  z = sizeof(int);
	  switch ((*p_arg)->type)
	    case FFI_TYPE_SINT8:
	      *(signed int *) argp = (signed int)*(SINT8 *)(* p_argv);
	      break;

	    case FFI_TYPE_UINT8:
	      *(unsigned int *) argp = (unsigned int)*(UINT8 *)(* p_argv);
	      break;

	    case FFI_TYPE_SINT16:
	      *(signed int *) argp = (signed int)*(SINT16 *)(* p_argv);
	      break;

	    case FFI_TYPE_UINT16:
#ifndef __x86_64__

	      *(unsigned int *) argp = (unsigned int)*(UINT16 *)(* p_argv);
	      break;

	    case FFI_TYPE_SINT32:
	      *(signed int *) argp = (signed int)*(SINT32 *)(* p_argv);
	      break;

	    case FFI_TYPE_UINT32:
	      *(unsigned int *) argp = (unsigned int)*(UINT32 *)(* p_argv);
	      break;

	    case FFI_TYPE_STRUCT:
	      *(unsigned int *) argp = (unsigned int)*(UINT32 *)(* p_argv);

	    default:
	      FFI_ASSERT(0);
      else
	  memcpy(argp, *p_argv, z);
	}
  if (ecif->cif->flags == FFI_TYPE_STRUCT)
      argp += z;
  for (i = cif->nargs, p_arg = cif->arg_types; (i != 0); i--, p_arg++)
      z = (*p_arg)->size;
      /* because we're little endian, this is what it turns into.   */
      *p_argv = (void*) argp;
      p_argv++;
      argp += z;

#endif /* __x86_64__  */
#ifdef X86_WIN32
/*@-declundef@*/
/*@-exportheader@*/
extern void ffi_call_STDCALL(void (*)(char *, extended_cif *),
			  /*@out@*/ extended_cif *,
      if ((sizeof(int) - 1) & (unsigned) argp)
	argp = (char *) ALIGN(argp, sizeof(int));
			  void (*fn)());
/*@=declundef@*/
/*@=exportheader@*/
#endif /* X86_WIN32 */

#ifdef X86_WIN32
    case FFI_STDCALL:
      /*@-usedef@*/
      ffi_call_STDCALL(ffi_prep_args, &ecif, cif->bytes,
		    cif->flags, ecif.rvalue, fn);
      /*@=usedef@*/
      break;
#endif /* X86_WIN32 */
static void ffi_closure_SYSV (ffi_closure *)
     __attribute__ ((regparm(1)));
static void ffi_closure_raw_SYSV (ffi_raw_closure *)
     __attribute__ ((regparm(1)));
/* This function is jumped to by the trampoline */
ffi_closure_SYSV (closure)
     ffi_closure *closure;
  void *args = __builtin_dwarf_cfa ();
   unsigned int  __dis = __fun - ((unsigned int) __tramp + FFI_TRAMPOLINE_SIZE); \
   *(unsigned char*) &__tramp[0] = 0xb8; \
   *(unsigned int*)  &__tramp[1] = __ctx; /* movl __ctx, %eax */ \
   *(unsigned char *)  &__tramp[5] = 0xe9; \
   *(unsigned int*)  &__tramp[6] = __dis; /* jmp __fun  */ \
ffi_closure_raw_SYSV (closure)
     ffi_raw_closure *closure;
  raw_args = (ffi_raw*) __builtin_dwarf_cfa ();
#ifdef X86_WIN32
extern void
ffi_call_STDCALL(void (*)(char *, extended_cif *),
	      /*@out@*/ extended_cif *,
	      unsigned, unsigned,
	      /*@out@*/ unsigned *,
	      void (*fn)());
#endif /* X86_WIN32 */

#ifdef X86_WIN32
    case FFI_STDCALL:
      /*@-usedef@*/
      ffi_call_STDCALL(ffi_prep_args_raw, &ecif, cif->bytes,
		    cif->flags, ecif.rvalue, fn);
      /*@=usedef@*/
      break;
#endif /* X86_WIN32 */
#ifndef X86_WIN32
#endif
#ifdef X86_WIN32
    case FFI_TYPE_STRUCT:
      if (cif->rtype->size == 1)
        {
          cif->flags = FFI_TYPE_SINT8; /* same as char size */
        }
      else if (cif->rtype->size == 2)
        {
          cif->flags = FFI_TYPE_SINT16; /* same as short size */
        }
      else if (cif->rtype->size == 4)
        {
          cif->flags = FFI_TYPE_INT; /* same as int type */
        }
      else if (cif->rtype->size == 8)
        {
          cif->flags = FFI_TYPE_SINT64; /* same as int64 type */
        }
      else
        {
          cif->flags = FFI_TYPE_STRUCT;
        }
      break;
#endif

      (cif->flags == FFI_TYPE_STRUCT))
#ifdef X86_WIN32
  else if (rtype == FFI_TYPE_SINT8) /* 1-byte struct  */
    {
      asm ("movsbl (%0),%%eax" : : "r" (resp) : "eax");
    }
  else if (rtype == FFI_TYPE_SINT16) /* 2-bytes struct */
    {
      asm ("movswl (%0),%%eax" : : "r" (resp) : "eax");
    }
#endif
  if ( cif->flags == FFI_TYPE_STRUCT ) {
      if ((sizeof(int) - 1) & (unsigned) argp) {
	argp = (char *) ALIGN(argp, sizeof(int));
