/* { dg-do link } */

int a __attribute__ ((visibility ("hidden"))); 
int *p = &a; 
main () {}
