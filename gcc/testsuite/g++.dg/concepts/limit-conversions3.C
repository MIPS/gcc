// { dg-options "-flimit-concept-conversions" }

concept HasPlusEquals<typename T> {
  T& operator+=(T&, T const&);
}

struct X { };
X& operator+=(X&, X);  // { dg-error "candidates are" }

struct Y { 
  Y(X);
  operator X() const;
};
Y& operator+=(Y&, X);  // { dg-error "note" }

concept_map HasPlusEquals<X> { }

concept_map HasPlusEquals<Y> { } // { dg-error "no match|unsatisfied|HasPlus" }
