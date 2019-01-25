/* Test signed jump-table optimization with link-time relaxation.  */
/* { dg-options "-march=32r6 -m32 -mpid -ffat-lto-objects" } */
/* { dg-additional-options "-mjump-table-opt" } */
/* { dg-do run } */
#include "jump-table-opt-signed.h"
