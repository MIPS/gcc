#include <sgidefs.h>
   ffi.c - Copyright (c) 1996 Red Hat, Inc.
#include <sys/cachectl.h>
#if _MIPS_SIM == _ABIN32
#if _MIPS_SIM == _ABIN32
  if (bytes > 8 * FFI_SIZEOF_ARG)
    argp = &stack[bytes - (8 * FFI_SIZEOF_ARG)];
#if _MIPS_SIM == _ABIN32
      *(ffi_arg *) argp = (ffi_arg) ecif->rvalue;
      argp += sizeof(ffi_arg);
      unsigned short a;
      a = (*p_arg)->alignment;
      if (a < FFI_SIZEOF_ARG)
        a = FFI_SIZEOF_ARG;
      
      if ((a - 1) & (unsigned) argp) {
	argp = (char *) ALIGN(argp, a);
#if _MIPS_SIM == _ABIO32
	  if (z < sizeof(ffi_arg))
	      z = sizeof(ffi_arg);
#if _MIPS_SIM == _ABIO32	      
#if _MIPS_SIM == _ABIN32
#if _MIPS_SIM == _ABIO32
  /* Set the flags necessary for O32 processing.  FFI_O32_SOFT_FLOAT
   * does not have special handling for floating point args.
   */
  if (cif->rtype->type != FFI_TYPE_STRUCT && cif->abi == FFI_O32)

  if (cif->abi == FFI_O32_SOFT_FLOAT)
      switch (cif->rtype->type)
        {
        case FFI_TYPE_VOID:
        case FFI_TYPE_STRUCT:
          cif->flags += cif->rtype->type << (FFI_FLAG_BITS * 2);
          break;

        case FFI_TYPE_SINT64:
        case FFI_TYPE_UINT64:
        case FFI_TYPE_DOUBLE:
          cif->flags += FFI_TYPE_UINT64 << (FFI_FLAG_BITS * 2);
          break;
        case FFI_TYPE_FLOAT:
        default:
          cif->flags += FFI_TYPE_INT << (FFI_FLAG_BITS * 2);
          break;
        }
    }
  else
    {
      /* FFI_O32 */      
      switch (cif->rtype->type)
        {
        case FFI_TYPE_VOID:
        case FFI_TYPE_STRUCT:
        case FFI_TYPE_FLOAT:
        case FFI_TYPE_DOUBLE:
          cif->flags += cif->rtype->type << (FFI_FLAG_BITS * 2);
          break;

        case FFI_TYPE_SINT64:
        case FFI_TYPE_UINT64:
          cif->flags += FFI_TYPE_UINT64 << (FFI_FLAG_BITS * 2);
          break;
      
        default:
          cif->flags += FFI_TYPE_INT << (FFI_FLAG_BITS * 2);
          break;
        }
#if _MIPS_SIM == _ABIN32
#if _MIPS_SIM == _ABIO32
    case FFI_O32_SOFT_FLOAT:
#if _MIPS_SIM == _ABIN32

#if FFI_CLOSURES  /* N32 not implemented yet, FFI_CLOSURES not defined */
#if defined(FFI_MIPS_O32)
extern void ffi_closure_O32(void);
#endif /* FFI_MIPS_O32 */

ffi_status
ffi_prep_closure (ffi_closure *closure,
		  ffi_cif *cif,
		  void (*fun)(ffi_cif*,void*,void**,void*),
		  void *user_data)
{
  unsigned int *tramp = (unsigned int *) &closure->tramp[0];
  unsigned int fn;
  unsigned int ctx = (unsigned int) closure;

#if defined(FFI_MIPS_O32)
  FFI_ASSERT(cif->abi == FFI_O32 || cif->abi == FFI_O32_SOFT_FLOAT);
  fn = (unsigned int) ffi_closure_O32;
#else /* FFI_MIPS_N32 */
  FFI_ASSERT(cif->abi == FFI_N32);
  FFI_ASSERT(!"not implemented");
#endif /* FFI_MIPS_O32 */

  tramp[0] = 0x3c190000 | (fn >> 16);     /* lui  $25,high(fn) */
  tramp[1] = 0x3c080000 | (ctx >> 16);    /* lui  $8,high(ctx) */
  tramp[2] = 0x37390000 | (fn & 0xffff);  /* ori  $25,low(fn)  */
  tramp[3] = 0x03200008;                  /* jr   $25          */
  tramp[4] = 0x35080000 | (ctx & 0xffff); /* ori  $8,low(ctx)  */

  closure->cif = cif;
  closure->fun = fun;
  closure->user_data = user_data;

  /* XXX this is available on Linux, but anything else? */
  cacheflush (tramp, FFI_TRAMPOLINE_SIZE, ICACHE);

  return FFI_OK;
}

/*
 * Decodes the arguments to a function, which will be stored on the
 * stack. AR is the pointer to the beginning of the integer arguments
 * (and, depending upon the arguments, some floating-point arguments
 * as well). FPR is a pointer to the area where floating point
 * registers have been saved, if any.
 *
 * RVALUE is the location where the function return value will be
 * stored. CLOSURE is the prepared closure to invoke.
 *
 * This function should only be called from assembly, which is in
 * turn called from a trampoline.
 *
 * Returns the function return type.
 *
 * Based on the similar routine for sparc.
 */
int
ffi_closure_mips_inner_O32 (ffi_closure *closure,
			    void *rvalue, unsigned long *ar,
			    double *fpr)
{
  ffi_cif *cif;
  void **avalue;
  ffi_type **arg_types;
  int i, avn, argn, seen_int;

  cif = closure->cif;
  avalue = alloca (cif->nargs * sizeof (void *));

  seen_int = (cif->abi == FFI_O32_SOFT_FLOAT);
  argn = 0;

  if ((cif->flags >> (FFI_FLAG_BITS * 2)) == FFI_TYPE_STRUCT)
    {
      rvalue = (void *) ar[0];
      argn = 1;
    }

  i = 0;
  avn = cif->nargs;
  arg_types = cif->arg_types;

  while (i < avn)
    {
      if (i < 2 && !seen_int &&
	  (arg_types[i]->type == FFI_TYPE_FLOAT ||
	   arg_types[i]->type == FFI_TYPE_DOUBLE))
	{
	  avalue[i] = ((char *) &fpr[i]);
	}
      else
	{
	  if (arg_types[i]->alignment == 8 && (argn & 0x1))
	    argn++;
	  avalue[i] = ((char *) &ar[argn]);
	  seen_int = 1;
	}
      argn += ALIGN(arg_types[i]->size, FFI_SIZEOF_ARG) / FFI_SIZEOF_ARG;
      i++;
    }

  /* Invoke the closure. */
  (closure->fun) (cif, rvalue, avalue, closure->user_data);

  if (cif->abi == FFI_O32_SOFT_FLOAT)
    {
      switch (cif->rtype->type)
        {
        case FFI_TYPE_FLOAT:
          return FFI_TYPE_INT;
        case FFI_TYPE_DOUBLE:
          return FFI_TYPE_UINT64;
        default:
          return cif->rtype->type;
        }
    }
  else
    {
      return cif->rtype->type;
    }
}

#endif /* FFI_CLOSURES */
