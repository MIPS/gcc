/* { dg-do link } */
/* { dg-options "{-fwhopr -fPIC -shared}" } */

/* Tests for the absence during linking of:
   lto1: error: type of 'i' does not match original declaration  */

int i[1];
