// Build don't link:

// by Paul Burchard <burchard@pobox.com>, Level Set Systems, Inc.
// Copyright (C) 1999, 2000, 2001 Free Software Foundation

class Q {
	template<class T>
	class X {              // ERROR - private
	};
};
template<template<class> class XX>
class Y {
	XX<int> x_;		
};
Y<Q::X> y;			// ERROR - Q::X inaccessible
