/* { dg-do compile } */

#pragma GCC aarch64 "arm_sve.h"
/* { dg-message {note: candidate: 'svfloat16_t svadd_x\(svbool_t, svfloat16_t, svfloat16_t\)'} "" { target *-*-* } 3 } */
/* { dg-message {note: *candidate expects 3 arguments, 2 provided} "" { target *-*-* } 3 } */
/* { dg-message {note: *candidate expects 3 arguments, 4 provided} "" { target *-*-* } 3 } */
/* { dg-message {note: *no known conversion for argument 1 from 'svuint8_t' to 'svbool_t'} "" { target *-*-* } 3 } */
/* { dg-message {note: *no known conversion for argument 2 from 'svbool_t' to 'svfloat16_t'} "" { target *-*-* } 3 } */
/* { dg-message {note: *no known conversion for argument 2 from 'int' to 'svfloat16_t'} "" { target *-*-* } 3 } */
/* { dg-message {note: *no known conversion for argument 2 from 'svuint8_t' to 'svfloat16_t'} "" { target *-*-* } 3 } */

void
f1 (svbool_t pg, svuint8_t x, svint8_t y)
{
  svadd_x (pg, x); /* { dg-error {no matching function for call to 'svadd_x\(svbool_t&, svuint8_t&\)'} } */
  svadd_x (pg, x, x, x); /* { dg-error {no matching function for call to 'svadd_x\(svbool_t&, svuint8_t&, svuint8_t&, svuint8_t&\)'} } */
  svadd_x (x, x, x); /* { dg-error {no matching function for call to 'svadd_x\(svuint8_t&, svuint8_t&, svuint8_t&\)'} } */
  svadd_x (pg, pg, pg); /* { dg-error {no matching function for call to 'svadd_x\(svbool_t&, svbool_t&, svbool_t&\)'} } */
  svadd_x (pg, 1, x); /* { dg-error {no matching function for call to 'svadd_x\(svbool_t&, int, svuint8_t&\)'} } */
  svadd_x (pg, x, y); /* { dg-error {no matching function for call to 'svadd_x\(svbool_t&, svuint8_t&, svint8_t&\)'} } */
}
