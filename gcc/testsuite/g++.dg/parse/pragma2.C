// PR c++/17595

int f(int x,
#pragma interface // { dg-error "not allowed here" }
      int y);
