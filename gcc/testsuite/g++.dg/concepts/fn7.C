// { dg-do link }
// { dg-options "-std=c++1z" }

#include <cassert>

// FIXME: This should be xfailed, but I'm not sure how to do that.

void f() { }
void f() requires true { };

int main() { }
