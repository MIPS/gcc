// { dg-options "-fno-limit-concept-conversions" }

concept C<typename T> {
  void f(const T&);
  void g(const T&, short);
}

struct X { };
void f(X);
void g(const X&, short);

struct Y { 
  operator X() const;
};
void g(const Y&, short);

struct Z { };
void f(Z);
void g(const Z&, long);

struct W { };
void f(W);
void g(const W&, int);


concept_map C<X> { }

concept_map C<Y> { }

concept_map C<Z> { }

concept_map C<W> { }
