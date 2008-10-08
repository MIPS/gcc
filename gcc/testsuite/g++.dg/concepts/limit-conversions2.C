// { dg-options "-flimit-concept-conversions" }

concept HasPlus<typename T> {
  T operator+(T const&, T const&);
}

struct X { };
X operator+(X, X); // { dg-error "candidates are" }

struct Y { 
  Y(X);
  operator X() const;
};

concept_map HasPlus<X> { }

concept_map HasPlus<Y> { } // { dg-error "no match|unsatisfied|HasPlus" }

