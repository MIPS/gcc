// { dg-options "-gdwarf-2 -dA" }
/* { dg-require-alias "" } */
// { dg-do compile }

class A {
public:
  virtual A* getThis();
};

class B {
  int a;
public:
  virtual B* getThis();
};

class AB : public A, public B {
public:
  virtual AB* getThis();
};

AB* AB::getThis() { return this; }

// { dg-final { scan-assembler-times " DW_AT_import" 2 } }
// { dg-final { scan-assembler-times " DW_AT_trampoline" 1 } }
