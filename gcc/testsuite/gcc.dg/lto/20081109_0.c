/* { dg-do compile } */
/* { dg-options "{-w -flto}" }  */
void Foo(void) { char bar[1]; free(bar); }
