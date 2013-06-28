// { dg-do compile { target { x86_64*-*-* } } }
// { dg-options "-fdump-abi=2" }
// { dg-skip-if "" { *-*-* } { "-flto" } { "" } }

class B0
{
  int m0;

public:

  B0();

  int
  get_m0() const;

  void
  set_m0(int a);
};

B0::B0()
    : m0(0)
{
}

int
B0::get_m0() const
{
  return m0;
}

void
B0::set_m0(int a)
{
  m0 = a;
}

class B1
{
  int m0;

public:

  B1();

  int
  get_m0() const;

  void
  set_m0(int a);
};

B1::B1()
  : m0(0)
{
}

int
B1::get_m0() const
{
  return m0;
}

void
B1::set_m0(int a)
{
  m0 = a;
}

class B2
{
  int m0;

public:

  B2();

  int
  get_m0() const;

  void
  set_m0(int a);
};

B2::B2()
  : m0(0)
{
}

int
B2::get_m0() const
{
  return m0;
}

void
B2::set_m0(int a)
{
  m0 = a;
}

class S : public B0, public B1, virtual B2
{
  int m0;

public:

  S();

  int
  get_m0() const;

  void
  set_m0(int a);
};

S::S()
: m0(0)
{
}

int
S::get_m0() const
{
  return m0;
}

void
S::set_m0(int a)
{
  m0 = a;
}

void
foo()
{
  S b;
}

// { dg-final { compare-abi-dump-with "bases-0.C.bi" } }
// { dg-final { cleanup-dump "bi" } }
