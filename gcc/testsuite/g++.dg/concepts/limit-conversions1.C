// { dg-options "-flimit-concept-conversions" }

concept C<typename T> {
  void f(const T&);
}


struct X { };

void f(X); // { dg-error "candidates are" }

struct Y { 
  operator X() const;
};

struct W;
struct Z { 
  Z(W);
};
void f(Z); // { dg-error "note" }

struct W { };

concept_map C<X> { }

concept_map C<Y> { } // { dg-error "no matching|unsatisfied|C<Y>" }

concept_map C<Z> { }

concept_map C<W> { } // { dg-error "no matching|unsatisfied|C<W>" }
