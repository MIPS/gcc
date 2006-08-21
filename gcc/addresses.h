/* Inline functions to test validity of reg classes for addressing modes.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* Wrapper function to unify target macros MODE_CODE_BASE_REG_CLASS,
   MODE_BASE_REG_REG_CLASS, MODE_BASE_REG_CLASS and BASE_REG_CLASS.
   Arguments as for the MODE_CODE_BASE_REG_CLASS macro.  */

static inline enum reg_class
base_reg_class (enum machine_mode mode ATTRIBUTE_UNUSED,
		enum rtx_code outer_code ATTRIBUTE_UNUSED,
		enum rtx_code index_code ATTRIBUTE_UNUSED)
{
  return MODE_CODE_BASE_REG_CLASS (mode, outer_code, index_code);
}

/* Wrapper around REGNO_MODE_CODE_OK_FOR_BASE_P, for use after register allocation is
   complete.  Arguments as for the called function.  */

static inline bool
regno_ok_for_base_p_strict (unsigned regno, enum machine_mode mode ATTRIBUTE_UNUSED,
			    enum rtx_code outer_code ATTRIBUTE_UNUSED,
			    enum rtx_code index_code ATTRIBUTE_UNUSED)
{
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return false;
    }

  return REGNO_MODE_CODE_OK_FOR_BASE_P (regno, mode, outer_code, index_code);
}

/* Wrapper around REGNO_MODE_CODE_OK_FOR_BASE_P, for use after register allocation is
   complete.  Arguments as for the called function.  */

static inline bool
ok_for_base_p_strict (rtx reg, enum machine_mode mode ATTRIBUTE_UNUSED,
	              enum rtx_code outer_code ATTRIBUTE_UNUSED,
	              enum rtx_code index_code ATTRIBUTE_UNUSED)
{
  unsigned regno;
  if (!REG_P (reg))
    return false;
  regno = REGNO (reg);
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return false;
    }

  return REGNO_MODE_CODE_OK_FOR_BASE_P (regno, mode, outer_code, index_code);
}

  
/* A version of regno_ok_for_base_p for use during regclass, when all pseudos
   should count as OK.  Arguments as for regno_ok_for_base_p.  */

static inline bool
regno_ok_for_base_p_nonstrict (unsigned regno, enum machine_mode mode ATTRIBUTE_UNUSED,
	                       enum rtx_code outer_code ATTRIBUTE_UNUSED,
	                       enum rtx_code index_code ATTRIBUTE_UNUSED)
{
  return regno >= FIRST_PSEUDO_REGISTER
	 || REGNO_MODE_CODE_OK_FOR_BASE_P (regno, mode, outer_code, index_code);
}

   
/* A version of regno_ok_for_base_p for use during regclass, when all pseudos
   should count as OK.  Arguments as for regno_ok_for_base_p.  */

static inline bool
ok_for_base_p_nonstrict (rtx reg, enum machine_mode mode ATTRIBUTE_UNUSED,
	                 enum rtx_code outer_code ATTRIBUTE_UNUSED,
	                 enum rtx_code index_code ATTRIBUTE_UNUSED)
{
  unsigned regno;
  if (!REG_P (reg))
    return false;
  regno = REGNO (reg);
  if (regno >= FIRST_PSEUDO_REGISTER)
    return true;

  return regno >= FIRST_PSEUDO_REGISTER
         || REGNO_MODE_CODE_OK_FOR_BASE_P (regno, mode, outer_code, index_code);
}

/* Wrapper around REGNO_OK_FOR_INDEX_P, for use after register allocation is
   complete.  Arguments as for REGNO_OK_FOR_INDEX_P.  */

static inline bool
regno_ok_for_index_p_strict (unsigned regno)
{
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return false;
    }

  return REGNO_OK_FOR_INDEX_P (regno);
}

/* Wrapper around REGNO_OK_FOR_INDEX_P, for use after register allocation is
   complete.  Arguments as for REGNO_OK_FOR_INDEX_P.  */

static inline bool
ok_for_index_p_strict (rtx reg)
{
  unsigned regno;
  if (!REG_P (reg))
    return false;
  regno = REGNO (reg);
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return false;
    }

  return REGNO_OK_FOR_INDEX_P (regno);
}

/* Wrapper around REGNO_OK_FOR_INDEX_P, to allow pseudo registers.  */

static inline bool
regno_ok_for_index_p_nonstrict (unsigned regno)
{
  return regno >= FIRST_PSEUDO_REGISTER
	 || REGNO_OK_FOR_INDEX_P (regno);
}

/* Wrapper around REGNO_OK_FOR_INDEX_P, to allow pseudo registers.  */

static inline bool
ok_for_index_p_nonstrict (rtx reg)
{
  unsigned regno;
  if (!REG_P (reg))
    return false;
  regno = REGNO (reg);
  return regno >= FIRST_PSEUDO_REGISTER
	 || REGNO_OK_FOR_INDEX_P (regno);
}  


/* Wrapper around REGNO_MODE_CODE_OK_FOR_BASE_P, for use after register allocation is
   complete.  Arguments as for the called function.  */

static inline bool
regno_ok_for_base_p (unsigned regno, enum machine_mode mode ATTRIBUTE_UNUSED,
		     enum rtx_code outer_code ATTRIBUTE_UNUSED,
		     enum rtx_code index_code ATTRIBUTE_UNUSED,
		     int strict_p)
{
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (strict_p && reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return !strict_p;
    }

  return REGNO_MODE_CODE_OK_FOR_BASE_P (regno, mode, outer_code, index_code);
}

/* Wrapper around REGNO_MODE_CODE_OK_FOR_BASE_P, for use after register allocation is
   complete.  Arguments as for the called function.  */

static inline bool
ok_for_base_p (rtx reg, enum machine_mode mode ATTRIBUTE_UNUSED,
	       enum rtx_code outer_code ATTRIBUTE_UNUSED,
	       enum rtx_code index_code ATTRIBUTE_UNUSED,
	       int strict_p)
{
  unsigned regno;
  if (!REG_P (reg))
    return false;
  regno = REGNO (reg);
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (strict_p && reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return !strict_p;
    }

  return REGNO_MODE_CODE_OK_FOR_BASE_P (regno, mode, outer_code, index_code);
}

  
/* Wrapper around REGNO_OK_FOR_INDEX_P, for use after register allocation is
   complete.  Arguments as for REGNO_OK_FOR_INDEX_P.  */

static inline bool
regno_ok_for_index_p (unsigned regno, int strict_p)
{
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (strict_p && reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return !strict_p;
    }

  return REGNO_OK_FOR_INDEX_P (regno);
}

/* Wrapper around REGNO_OK_FOR_INDEX_P, for use after register allocation is
   complete.  Arguments as for REGNO_OK_FOR_INDEX_P.  */

static inline bool
ok_for_index_p (rtx reg, int strict_p)
{
  unsigned regno;
  if (!REG_P (reg))
    return false;
  regno = REGNO (reg);
  if (regno >= FIRST_PSEUDO_REGISTER)
    {
      if (strict_p && reg_renumber[regno] >= 0)
        regno = reg_renumber[regno];
      else
        return !strict_p;
    }

  return REGNO_OK_FOR_INDEX_P (regno);
}

