  // closures.  This should be implemented by a separate assembly language
  if (ecif->cif->rtype->type == FFI_TYPE_STRUCT)
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
      p_argv++;
      argp += z;
  for (i = cif->nargs, p_arg = cif->arg_types; (i != 0); i--, p_arg++)
      z = (*p_arg)->size;
      /* because we're little endian, this is what it turns into.   */
      *p_argv = (void*) argp;
      p_argv++;
      argp += z;

#endif /* __x86_64__  */
