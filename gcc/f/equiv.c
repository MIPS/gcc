   Copyright (C) 1995-1998 Free Software Foundation, Inc.
   Contributed by James Craig Burley (burley@gnu.org).
   Contributed by James Craig Burley.
		  /* Increase size of equiv area to start for lower offset
		     relative to root symbol.  */
		  if (! ffetarget_offset_add (&new_size,
					      ffestorag_offset (st)
					      - item_offset,
					      ffestorag_size (st)))
  /* If the offset for this storage area is zero (it cannot be positive),
     that means the alignment/modulo info is already correct.  Otherwise,
     the alignment info is correct, but the modulo info reflects a
     zero offset, so fix it.  */

  if (ffestorag_offset (st) < 0)
    {
      /* Calculate the initial padding necessary to preserve
	 the alignment/modulo requirements for the storage area.
	 These requirements are themselves kept track of in the
	 record for the storage area as a whole, but really pertain
	 to offset 0 of that area, which is where the root symbol
	 was originally placed.

	 The goal here is to have the offset and size for the area
	 faithfully reflect the area itself, not extra requirements
	 like alignment.  So to meet the alignment requirements,
	 the modulo for the area should be set as if the area had an
	 alignment requirement of alignment/0 and was aligned/padded
	 downward to meet the alignment requirements of the area at
	 offset zero, the amount of padding needed being the desired
	 value for the modulo of the area.  */

      alignment = ffestorag_alignment (st);
      modulo = ffestorag_modulo (st);

      /* Since we want to move the whole area *down* (lower memory
	 addresses) as required by the alignment/modulo paid, negate
	 the offset to ffetarget_align, which assumes aligning *up*
	 is desired.  */
      pad = ffetarget_align (&alignment, &modulo,
			     - ffestorag_offset (st),
			     alignment, 0);
      ffestorag_set_modulo (st, pad);
    }

	    if (ffebld_op (subscript) == FFEBLD_opANY)
	      return FALSE;

	    if (ffebld_op (dim) == FFEBLD_opANY)
	      return FALSE;

		if (ffebld_op (low) == FFEBLD_opANY)
		  return FALSE;

		assert (ffebld_op (low) == FFEBLD_opCONTER);
	    if (ffebld_op (high) == FFEBLD_opANY)
	      return FALSE;

	if (ffebld_op (expr) == FFEBLD_opANY)
	  return FALSE;
	    if (ffebld_op (begin) == FFEBLD_opANY)
	      return FALSE;
#if FFECOM_targetCURRENT == FFECOM_targetFFE
#endif
