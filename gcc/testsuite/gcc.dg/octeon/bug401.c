/* { dg-do link } */

int a __attribute__ ((visibility ("hidden"))); 
int *p = &a; 
int main (void) {return 0;}
