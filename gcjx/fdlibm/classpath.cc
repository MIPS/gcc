/* Some code from GNU Classpath, conveniently wrapped.
   
   Copyright (C) 1998, 1999, 2001, 2003, 2004, 2005 Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.
 
GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.  */

#include "typedefs.hh"

#include "mprec.h"
#include "fdlibm.h"

std::string
classpath_double_to_string (jdouble value, jboolean is_float)
{
  int flag = std::isinf (value);
  if (flag < 0)
    return "-Infinity";
  else if (flag > 0)
    return "Infinity";
  else if (isnan (value))
    return "NaN";

  char buffer[50], result[50];
  int decpt, sign;
  char *s, *d;
  int i;

  _dtoa (value, 0, 20, &decpt, &sign, NULL, buffer, (int) is_float);

  value = fabs (value);

  s = buffer;
  d = result;

  if (sign)
    *d++ = '-';

  if ((value >= 1e-3 && value < 1e7) || (value == 0))
    {
      if (decpt <= 0)
	*d++ = '0';
      else
	{
	  for (i = 0; i < decpt; i++)
	    if (*s)
	      *d++ = *s++;
	    else
	      *d++ = '0';
	}

      *d++ = '.';

      if (*s == 0)
	{
	  *d++ = '0';
	  decpt++;
	}
	  
      while (decpt++ < 0)
	*d++ = '0';      
      
      while (*s)
	*d++ = *s++;

      *d = 0;

      return result;
    }

  *d++ = *s++;
  decpt--;
  *d++ = '.';
  
  if (*s == 0)
    *d++ = '0';

  while (*s)
    *d++ = *s++;

  *d++ = 'E';
  
  if (decpt < 0)
    {
      *d++ = '-';
      decpt = -decpt;
    }

  {
    char exp[4];
    char *e = exp + sizeof exp;
    
    *--e = 0;
    do
      {
	*--e = '0' + decpt % 10;
	decpt /= 10;
      }
    while (decpt > 0);

    while (*e)
      *d++ = *e++;
  }
  
  *d = 0;

  return result;
}

bool
classpath_string_to_double (const char *buf, jdouble *result)
{
  const char *end = buf + strlen (buf);
  char *endptr;

  struct _Jv_reent reent;  
  memset (&reent, 0, sizeof reent);

  *result = _strtod_r (&reent, buf, &endptr);

  return ((const char *) endptr == end) && reent._errno == 0;
}
