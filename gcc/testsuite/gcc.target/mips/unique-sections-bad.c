/* { dg-do compile } */
/* { dg-options "-munique-sections=non-existent-file.txt" } */
/* { dg-error "Bad filename for -munique-sections:" "" { target *-*-* } 0 } */
