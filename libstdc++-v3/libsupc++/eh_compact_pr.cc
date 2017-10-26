#ifdef __GNU_COMPACT_EH__
// -*- C++ -*- The GNU C++ compact exception personality routine
// Copyright (C) 2012
// 
// Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.
//
// Written by Catherine Moore <clm@codesourcery.com>

#include <bits/c++config.h>
#include <cstdlib>
#include <bits/exception_defines.h>
#include <cxxabi.h>
#include "unwind-cxx.h"

using namespace __cxxabiv1;

#include "unwind-pe.h"


enum exception_entry_type
{
  CATCH_TYPE,
  CLEANUP_TYPE,
  CONTINUE_UNWINDING_TYPE,
  EH_SPEC_TYPE,
  NOTHROW_TYPE,
  UNKNOWN_TYPE,
}; 

#define LENGTH_MASK 1

#define NOTHROW_REGION(x) (x & 0x1)
#define CONTINUE_UNWINDING_REGION(x) (x == -1)
#define DUMMY_ACTION_REGION(x) (x == 0)
#define EXTRACT_ACTION_VALUE(x) (((x & 3) ^ 2) - 2)
#define EXTRACT_CHAIN_VALUE(x) (x >> 2)
#define ACTION_EXTENSION -2
#define HAVE_EHSPECS(x) (x & 0x40)
#define TTABLE_START(x) (x + 4 - 1) & ~(4 - 1);

// Return an element from a type table.

static const std::type_info *
compact_get_ttype_entry (_Unwind_Ptr base,
			 unsigned char eh_encoding,
			 _Unwind_Ptr ttab_start,
			 int aval)
{
  _Unwind_Ptr ptr;

  aval = (aval -  1) * size_of_encoded_value (eh_encoding);
  read_encoded_value_with_base (eh_encoding, base,
				(const unsigned char *) ttab_start + aval, &ptr);

  return reinterpret_cast<const std::type_info *>(ptr);
}

// Given the thrown type THROW_TYPE, pointer to a variable containing a
// pointer to the exception object THROWN_PTR_P and a type CATCH_TYPE to
// compare against, return whether or not there is a match and if so,
// update *THROWN_PTR_P.

static bool
get_adjusted_ptr (const std::type_info *catch_type,
		  const std::type_info *throw_type,
		  void **thrown_ptr_p)
{
  void *thrown_ptr = *thrown_ptr_p;

  // Pointer types need to adjust the actual pointer, not
  // the pointer to pointer that is the exception object.
  // This also has the effect of passing pointer types
  // "by value" through the __cxa_begin_catch return value.
  if (throw_type->__is_pointer_p ())
    thrown_ptr = *(void **) thrown_ptr;

  if (catch_type->__do_catch (throw_type, &thrown_ptr, 1))
    {
      *thrown_ptr_p = thrown_ptr;
      return true;
    }

  return false;
}

// Save stage1 handler information in the exception object.

static inline void
save_caught_exception (struct _Unwind_Exception *ue_header,
		       void *thrown_ptr,
		       int handler_switch_value,
		       const unsigned char *language_specific_data,
		       _Unwind_Ptr landing_pad)
{
  __cxa_exception* xh = __get_exception_header_from_ue(ue_header);

  xh->handlerSwitchValue = handler_switch_value;
  xh->languageSpecificData = language_specific_data;
  xh->adjustedPtr = thrown_ptr;
  xh->catchTemp = landing_pad;
}

// Restore the catch handler information saved during phase1.

static inline void
restore_caught_exception (struct _Unwind_Exception *ue_header,
			  int &handler_switch_value,
			  const unsigned char *& language_specific_data,
			  _Unwind_Ptr& landing_pad)
{
  __cxa_exception* xh = __get_exception_header_from_ue(ue_header);
  handler_switch_value = xh->handlerSwitchValue;
  language_specific_data = xh->languageSpecificData;
  landing_pad = (_Unwind_Ptr) xh->catchTemp;
}

// PCS_PTR points to a variable holding a pointer to the start of a new
// LSDA entry.  This function parses the entry and updates PCS_PTR to
// point past its end.  The variables at *PREGION_LEN, *PLP_OFF, *PAV and
// *PCV are filled with information about the data found: the region
// length, the landing pad offset, the action value and the chain value.

static inline enum exception_entry_type
compact_parse_header (const unsigned char **pcs_ptr, _uleb128_t *pregion_len,
		      int *pav, int *pcv, _sleb128_t *plp_off)
{
  const unsigned char *cs_ptr = *pcs_ptr;
  _uleb128_t region_len;
  enum exception_entry_type type = NOTHROW_TYPE;

  cs_ptr = read_uleb128 (cs_ptr, pregion_len);

  region_len = *pregion_len;

  /* If nothrow, advance to next entry.  */
  if (!NOTHROW_REGION (region_len))
    {
      _sleb128_t lp = 0;
      /* Dummy action entries, don't have landing pads.  */
      if (!DUMMY_ACTION_REGION (region_len))
	{
	  cs_ptr = read_sleb128 (cs_ptr, plp_off);
	  lp = *plp_off;
	}
      /* If continue_unwinding, advance to next entry.  */
      if (CONTINUE_UNWINDING_REGION (lp))
	type = CONTINUE_UNWINDING_TYPE;
      else
	{
	  int action_value, chain_value;
	  _sleb128_t ac_pair;

	  cs_ptr = read_sleb128 (cs_ptr, &ac_pair);
	  action_value = EXTRACT_ACTION_VALUE (ac_pair);
	  chain_value = EXTRACT_CHAIN_VALUE (ac_pair);
	  if (action_value == ACTION_EXTENSION)
	    {
	      _sleb128_t ac_ext;
	      cs_ptr = read_sleb128 (cs_ptr, &ac_ext);
	      action_value = ac_ext;
	    }

	  if (action_value == 0)
	    type = CLEANUP_TYPE;
	  else if (action_value > 0)
	    type = CATCH_TYPE;
	  else
	    type = EH_SPEC_TYPE;

	  *pav = action_value;
	  *pcv = chain_value;
	}
    }

  *pcs_ptr = cs_ptr;
  return type;
}

/* Given a region entry number, return a pointer to its call-site
   table entry.  */

static const unsigned char *
compact_goto_region (const unsigned char *cs_ptr, int region_no,
		     int chain_to_region)
{
  while (true)
    {
      _uleb128_t region_len;
      int action_value, chain_value;
      _sleb128_t lp_off;

      region_no++;
      if (region_no == chain_to_region)
	break;

      compact_parse_header (&cs_ptr, &region_len, &action_value,
			    &chain_value, &lp_off);
    }
  return cs_ptr;
}

/* Return the next region number.  The next region is the current
   region number less the chain_value.  No Throw regions are ignored.  */

static int
compact_backward_find_chain (const unsigned char *cs_start,
			     int region_no, int chain_value)
{
  int looking_for = region_no - 1;
  int looking_at = -1;
  const unsigned char *cs_ptr = cs_start;

  while (true)
    {
      _uleb128_t region_len;
      int action_value, unused_chain_value;
      _sleb128_t lp_off;
      enum exception_entry_type type;

      looking_at++;
      type = compact_parse_header (&cs_ptr, &region_len, &action_value,
				   &unused_chain_value, &lp_off);

      if (looking_at == looking_for)
	{
	  if (type != NOTHROW_TYPE && type != CONTINUE_UNWINDING_TYPE)
	    chain_value = chain_value + 1;

	  if (chain_value == 0)
	    break;

	  looking_at = -1;
	  looking_for--;
	  cs_ptr = cs_start;
	}
    }
  return looking_at;
}

/* Return the next region number.  chain_value is the number of
   region entries in the call-site table to advance.  No Throw
   regions are ignored and not included in the count.  */

static int
compact_forward_find_chain (const unsigned char *cs_ptr, int region_no,
			    int chain_value)
{
  int valid_regions = 0;

  while (true)
    {
      _uleb128_t region_len;
      int action_value, unused_chain_value;
      _sleb128_t lp_off;
      enum exception_entry_type type;

      region_no++;
      type = compact_parse_header (&cs_ptr, &region_len, &action_value,
				   &unused_chain_value, &lp_off);

      if (type == NOTHROW_TYPE || type == CONTINUE_UNWINDING_TYPE)
	continue;

      valid_regions++;
      if (valid_regions == chain_value)
	return region_no;
    }
}

/* Return the next region number in the chain.  */

static int
compact_find_chain_to_region (const unsigned char *cs_start,
			      const unsigned char **cs_ptr,
			      int *region_no, int chain_value)
{
  int chain_to_region;

  if (chain_value < 0)
    chain_to_region
      = compact_backward_find_chain (cs_start, *region_no, chain_value);
  else
    chain_to_region
      = compact_forward_find_chain (*cs_ptr, *region_no, chain_value);

  return chain_to_region;
}

typedef const std::type_info _throw_typet;

namespace __cxxabiv1
{

#pragma GCC visibility push(default)
extern "C" _Unwind_Reason_Code
__gnu_compact_pr2 (int version __attribute__ ((unused)),
		   _Unwind_Action actions,
		   _Unwind_Exception_Class exception_class,
		   struct _Unwind_Exception *ue_header,
		   struct _Unwind_Context *context)
{
  const unsigned char *cs_ptr, *language_specific_data;
  const unsigned char *cs_start, *cs_end;
  const unsigned char *padding_start;
  const unsigned char *ehspecs_start;

  _Unwind_Ptr ip, ip_start, ip_end;
  _Unwind_Ptr lp_start, landing_pad;
  _Unwind_Ptr ttable_start;
  _Unwind_Ptr lsda;

  _sleb128_t lp_off;
  _uleb128_t region_len;
  _uleb128_t call_site_len;
  _uleb128_t ehspecs_len;

  const std::type_info* catch_type;
  int chain_value, action_value;
  int region_no;
  int handler_switch_value;
  bool chaining = false;
  int chain_to_region;
  enum exception_entry_type region_type;
  bool foreign_exception;
  int ip_before_insn = 0;

  void *thrown_ptr = 0;
  _throw_typet *throw_type;
  unsigned char eh_encoding = _Unwind_GetEhEncoding (context);
  _Unwind_Ptr base = base_of_encoded_value (eh_encoding, context);
  __cxa_exception *xh = __get_exception_header_from_ue(ue_header);

  enum found_handler_type
  {
    found_nothing,
    found_terminate,
    found_cleanup,
    found_handler,
  } found_type = found_nothing;

  language_specific_data = (const unsigned char *)
    _Unwind_GetLanguageSpecificData (context);

  // If no LSDA, then there are no handlers or cleanups.
  if (!language_specific_data)
    return _URC_CONTINUE_UNWIND;

  /* Read the LSDA header.  */
  cs_ptr =
    read_encoded_value (0, DW_EH_PE_udata1, language_specific_data, &lsda);
  /* Then the Call-site length.  */
  cs_ptr = read_uleb128 (cs_ptr, &call_site_len);
  cs_start = cs_ptr;
  cs_end = cs_start + call_site_len;

  /* Read the ehspecs if they are present and setup
     a pointer to the type table entries.  */
  if (HAVE_EHSPECS (lsda))
    {
      ehspecs_start = read_uleb128 (cs_start + call_site_len, &ehspecs_len);
      padding_start = ehspecs_start + ehspecs_len;
      ttable_start = TTABLE_START ((_Unwind_Ptr) padding_start);
    }
  else
    ttable_start = TTABLE_START ((_Unwind_Ptr) cs_start + call_site_len);

  foreign_exception = !__is_gxx_exception_class(exception_class);
  // Shortcut for phase 2 found handler for domestic exception.
  if (actions == (_UA_CLEANUP_PHASE | _UA_HANDLER_FRAME)
      && !foreign_exception)
    {
      restore_caught_exception (ue_header, handler_switch_value,
				language_specific_data, landing_pad);
      found_type = (landing_pad == 0 ? found_terminate : found_handler);
      goto install_context;
    }

#ifdef _GLIBCXX_HAVE_GETIPINFO
  ip = _Unwind_GetIPInfo (context, &ip_before_insn);
#else
  ip = _Unwind_GetIP (context);
#endif
  if (!ip_before_insn)
    --ip;

  ip_start = lp_start = _Unwind_GetRegionStart (context);
  region_no = 0;

  /* Examine each region entry, looking for
     a matching ip address or chain value.  */
  while (cs_ptr < cs_end)
    {
      region_type = compact_parse_header (&cs_ptr, &region_len, &action_value,
					  &chain_value, &lp_off);

      if (DUMMY_ACTION_REGION (region_len) && !chaining)
	break;

      region_len &= ~LENGTH_MASK;
      lp_start += region_len;
      ip_end = ip_start + region_len;

      switch (region_type)
	{
	case NOTHROW_TYPE:
	  break;
	
	case CONTINUE_UNWINDING_TYPE:
	  if (ip >= ip_start && ip <= ip_end)
	    return _URC_CONTINUE_UNWIND;
	  break;

	case CLEANUP_TYPE:
	  if (chaining
	      || (ip >= ip_start && ip <= ip_end))
	    {
	      found_type = found_cleanup;
	      handler_switch_value = action_value;
	      if (!chaining)
	        landing_pad = lp_start + lp_off;

	      /* For cleanups, walk the chain.  The cleanup is
		 processed only if there are no other matches.  */

	      if (chain_value == 0)
		goto found_something;
	      else
		{
		  chaining = true;
		  chain_to_region = 
		      compact_find_chain_to_region (cs_start, &cs_ptr,
						    &region_no, chain_value);
		}
	    }
	  break;

	case CATCH_TYPE:
	  if (!chaining
	      && (ip < ip_start || ip > ip_end))
	    break;

#ifdef __GXX_RTTI
	  // During forced unwinding, match a magic exception type.
	  if (actions & _UA_FORCE_UNWIND)
	    throw_type = &typeid(abi::__forced_unwind);

	  // With a foreign exception class, there's no exception type.
	  else if (foreign_exception)
	    throw_type = &typeid(abi::__foreign_exception);
	  else
#endif
	    {
	      thrown_ptr = __get_object_from_ue (ue_header);
	      throw_type = __get_exception_header_from_obj
		(thrown_ptr)->exceptionType;
	    }
	  catch_type =
	    compact_get_ttype_entry (base,
				     eh_encoding, ttable_start, action_value);

	  if (catch_type == 0
	      || get_adjusted_ptr (catch_type, throw_type, &thrown_ptr))
	    {
	      found_type = found_handler;
	      handler_switch_value = action_value;
	      if (!chaining)
		landing_pad = lp_start + lp_off;
	      goto found_something;
	    }

	  /* Walk the chain if we didn't match.  */
	  if (chain_value != 0)
	    {
	      if (!chaining)
		landing_pad = lp_start + lp_off;
	      chaining = true;
	      chain_to_region = 
		compact_find_chain_to_region (cs_start, &cs_ptr,
					      &region_no, chain_value);
	    }
	  else
	    // IP matched and not chaining.
	    goto found_something;

	  break;

	case EH_SPEC_TYPE:
#ifdef __GXX_RTTI
	  // During forced unwinding, match a magic exception type.
	  if (actions & _UA_FORCE_UNWIND)
	    throw_type = &typeid(abi::__forced_unwind);

	  // With a foreign exception class, there's no exception type.
	  else if (foreign_exception)
	    throw_type = &typeid(abi::__foreign_exception);

	  else
#endif
	    {
	      thrown_ptr = __get_object_from_ue (ue_header);
	      throw_type = __get_exception_header_from_obj
		(thrown_ptr)->exceptionType;
	    }
	  if (ip >= ip_start && ip <= ip_end)
	    {
	      if (!chaining)
		landing_pad = lp_start + lp_off;

	      /* An ehspec of zero is an immediate match.  */
	      if (ehspecs_len == 0)
		{
		  found_type = found_handler;
		  handler_switch_value = action_value;
		  goto found_something;
		}
	      else if (throw_type
		       && !(actions & _UA_FORCE_UNWIND)
		       && !foreign_exception)
		{
		  const unsigned char *p;
		  _uleb128_t ehval;

		  bool matched_spec = false;
		  int si = abs (action_value) - 1;
		  p = read_uleb128 (ehspecs_start + si, &ehval);
		  /* Read the ehspec entries until a match is found or the
		     list is exhausted.  Process as a handler if no match
		     is achieved.  */
		  while (!matched_spec && ehval != 0)
		    {
	              catch_type = compact_get_ttype_entry (base, eh_encoding,
							    ttable_start, ehval);

		      if (get_adjusted_ptr (catch_type, throw_type, &thrown_ptr))
			matched_spec = true;
		      p = read_uleb128 (p, &ehval);
		    }
		  if (!matched_spec)
		    {
		      found_type = found_handler;
		      handler_switch_value = action_value;
		      goto found_something;
		    }
		}
	      if (chain_value != 0)
		{
		  if (!chaining)
		    landing_pad = lp_start + lp_off;
		  chaining = true;
		  chain_to_region = 
		    compact_find_chain_to_region (cs_start, &cs_ptr,
						  &region_no, chain_value);
		}
	      else
		// IP matched and not chaining.
		goto found_something;
	    }
	  break;

	case UNKNOWN_TYPE:
	  break;
	}

      /* If a chain was discovered, find the next region entry in the chain.  */
      if (chaining && region_no != chain_to_region)
	{
	  if (chain_to_region < region_no)
	    cs_ptr = compact_goto_region (cs_start, -1, chain_to_region);
	  else
	    cs_ptr = compact_goto_region (cs_ptr, region_no, chain_to_region);
	  region_no = chain_to_region - 1;
	}

      ip_start = ip_end;
      region_no++;
    }
	    
  /* Each region has been examined without finding a match.  */
  if (found_type == found_nothing)
    {
      found_type = found_terminate;
      landing_pad = 0;
    }

found_something:

  if (found_type == found_nothing)
    return _URC_CONTINUE_UNWIND;

  if (actions & _UA_SEARCH_PHASE)
    {
      if (found_type == found_cleanup)
	return _URC_CONTINUE_UNWIND;

      if (!foreign_exception)
        {
          save_caught_exception(ue_header, thrown_ptr, handler_switch_value,
				language_specific_data, landing_pad);
        }
      return _URC_HANDLER_FOUND;
    }

install_context:
  
  // We can't use any of the cxa routines with foreign exceptions,
  // because they all expect ue_header to be a struct __cxa_exception.
  // So in that case, call terminate or unexpected directly.
  if ((actions & _UA_FORCE_UNWIND)
      || foreign_exception)
    {
      if (found_type == found_terminate)
	std::terminate ();
      else if (handler_switch_value < 0)
	{
	  __try 
	    { std::unexpected (); } 
	  __catch(...) 
	    { std::terminate (); }
	}
    }
  else
    {
      if (found_type == found_terminate)
	__cxa_call_terminate(ue_header);

      // Cache the base value for __cxa_call_unexpected, as we won't
      // have an _Unwind_Context then.
      if (handler_switch_value < 0)
	{
	  xh->catchTemp = base_of_encoded_value (eh_encoding, context);
	  /* For the compact encoding, bits 0-7 of
	     xh->handler_switch_value are used to store the
	     eh_encoding.  Bits 8-31 of xh_handler_switch_value
	     are used to store the offset from the beginning of
             the LSDA to the type count for this unmatched exception
             specification.  We store this as a positive number to 
	     signal to __cxa_call_unexpected that the LSDA is in 
             the compact format.  */

	  xh->handlerSwitchValue = abs (xh->handlerSwitchValue) << 8;;
	  xh->handlerSwitchValue = xh->handlerSwitchValue | eh_encoding;
	  handler_switch_value = -1;

	}  
    }

  /* For targets with pointers smaller than the word size, we must
     extend the pointer, and this extension is target dependent.  */
  _Unwind_SetGR (context, __builtin_eh_return_data_regno (0),
		 __builtin_extend_pointer (ue_header));
  /* handler_switch_value will always be -1 for an
     unmatched exception spec header.  */
  _Unwind_SetGR (context, __builtin_eh_return_data_regno (1),
		 handler_switch_value);
  _Unwind_SetIP (context, landing_pad);
  return _URC_INSTALL_CONTEXT;
}

#pragma GCC visibility pop
} // namespace __cxxabiv1
#endif
