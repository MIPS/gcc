// { dg-do link }
// { dg-options "-std=c++1z" }

#include <cassert>

void f() requires true { }; // { dg-xfail-if "" { *-*-* } }

int main() { }
