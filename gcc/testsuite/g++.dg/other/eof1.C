// { dg-do compile }

// Copyright (C) 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 24 Sept 2001 <nathan@codesourcery.com>

// Make sure we stop at EOF

struct A
{ // { dg-error ".*" "" }
