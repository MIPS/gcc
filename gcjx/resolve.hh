// Some helpers related to resolution.

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

#ifndef GCJX_RESOLVE_HH
#define GCJX_RESOLVE_HH

template<typename T>
inline void
resolve (resolution_scope *scope, const std::list<T> &l)
{
  for (typename std::list<T>::const_iterator i = l.begin ();
       i != l.end ();
       ++i)
    (*i)->resolve (scope);
}

template<typename T>
inline void
resolve (resolution_scope *scope, const std::map< std::string, owner<T> > &l)
{
  for (typename std::map< std::string, owner<T> >::const_iterator i
	 = l.begin ();
       i != l.end ();
       ++i)
    (*i).second->resolve (scope);
}

template<typename T>
inline void
resolve_classes (const std::list<T> &l)
{
  for (typename std::list<T>::const_iterator i = l.begin ();
       i != l.end ();
       ++i)
    (*i)->resolve_classes ();
}

template<typename T>
inline void
resolve_classes (const std::map< std::string, owner<T> > &l)
{
  for (typename std::map< std::string, owner<T> >::const_iterator i
	 = l.begin ();
       i != l.end ();
       ++i)
    (*i).second->resolve_classes ();
}

template<typename T>
inline void
resolve_classes (resolution_scope *scope, const std::list<T> &l)
{
  for (typename std::list<T>::const_iterator i = l.begin ();
       i != l.end ();
       ++i)
    (*i)->resolve_classes (scope);
}

template<typename T>
inline void
resolve_classes (resolution_scope *scope,
		 const std::map< std::string, owner<T> > &l)
{
  for (typename std::map< std::string, owner<T> >::const_iterator i
	 = l.begin ();
       i != l.end ();
       ++i)
    (*i).second->resolve_classes (scope);
}

#endif // GCJX_RESOLVE_HH
