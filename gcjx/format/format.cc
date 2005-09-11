// Formatting.

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "typedefs.hh"
#include <sstream>

format_repr::format_repr (format_type t, location w, const char *fmt)
  : refc (0),
    where (w),
    plan (copy_str (fmt)),
    subst_count (0),
    type (t)
{
  // FIXME we should do more error checking here.
  for (const char *p = fmt; *p; ++p)
    {
      if (*p == '%')
	{
	  ++p;
	  if (*p == 'u')
	    ++p;
	  if (isdigit (*p))
	    ++subst_count;
	}
    }
  args.reserve (subst_count);
}

void
format_repr::add (const std::string &arg)
{
  assert (args.size () < subst_count);
  args.push_back (arg);
}

std::ostream &
operator<< (std::ostream &out, const format &objw)
{
  format_repr *obj = objw.get ();
  obj->dump (out);
  return out;
}

void
format_repr::dump (std::ostream &os) const
{
  bool failure = (type == format_error
		  || (type == format_warning
		      && global->get_compiler ()->warnings_are_errors ()));
  os << where << (failure ? "error: " : "warning: ") << get_message () << "\n";
  if (failure)
    global->get_compiler ()->set_failed ();
}

std::string
format_repr::get_message () const
{
  std::ostringstream message;
  for (const char *p = plan; *p; ++p)
    {
      if (*p != '%')
        {
          message << *p;
          continue;
        }

      ++p;
      bool quoted = true;
      if (*p == 'u')
        {
          quoted = false;
          ++p;
        }

      if (quoted && *p == '<')
        message << open_quote ();
      else if (quoted && *p == '>')
        message << close_quote ();
      else if (quoted && *p == '%')
        message << '%';
      else if (isdigit (*p))
        {
          char *end_p;
          const size_t index = strtoul (p, &end_p, 10);
          p = end_p - 1;
          if (index <= args.size ())
            {
              if (quoted)
                message << open_quote ();
              message << args[index - 1];
              if (quoted)
                message << close_quote ();
            }
          else
            message << "(internal error: can't format argument " << index
                    << " of " << args.size () << ")";
          if (*p == 0)
            break;
        }
      else
        report_bad_plan (*p);
    }
  return message.str ();
}

void
format_repr::report_bad_plan (char ch) const
{
  std::cerr << std::endl
            << "bad escape " << open_quote () << ch << close_quote ()
            << " in plan: " << plan << std::endl;
  assert (false && "error in format string");
}

const char *
format_repr::open_quote () const
{
  return "'";
}

const char *
format_repr::close_quote () const
{
  return "'";
}

const format &
operator% (const format &fmt, const std::string &str)
{
  fmt->add (str);
  return fmt;
}

const format &
operator% (const format &fmt, const char *str)
{
  fmt->add (std::string (str));
  return fmt;
}

const format &
operator% (const format &fmt, char val)
{
  char s[2];
  s[0] = val;
  s[1] = '\0';
  fmt->add (std::string (s));
  return fmt;
}

const format &
operator% (const format &fmt, int val)
{
  char s[20];
  sprintf (s, "%d", val);
  return fmt % s;
}

const format &
operator% (const format &fmt, size_t val)
{
  std::ostringstream oss;
  oss << val;
  return fmt % oss.str ();
}

template<typename T>
const format &
operator% (const format &fmt, const owner<T> &ref)
{
  return fmt % (ref.get ());
}

const format &
operator% (const format &fmt, const std::list<std::string> &l)
{
  return fmt % join (l, '.');
}



// Instantiations.

template const format &operator% (const format &fmt,
				  const owner<model_expression> &);
