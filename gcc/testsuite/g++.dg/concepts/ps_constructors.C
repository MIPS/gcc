struct X {};


concept DefaultConstructible<typename T>
{
  T::T();
};

concept_map DefaultConstructible<X> { };
concept_map DefaultConstructible<int> { };
concept_map DefaultConstructible<int*> { };
// TBD should error nicely concept_map DefaultConstructible<int&> { };

struct NotDefaultConstructible
{
 private:
  NotDefaultConstructible();
};

concept_map DefaultConstructible<NotDefaultConstructible> { };
// { dg-error "is private" "" { target *-*-* } 17 }
// { dg-error "within this context" "" { target *-*-* } 20 }


concept CopyConstructible<typename T>
{
  T::T(const T&); // { dg-error "unsatisfied requirement" }
  T::T(T&);
  //  T::~T();
  //  T* T::operator&();
  //  const T* T::operator&() const;
};

concept_map CopyConstructible<X> { };
concept_map CopyConstructible<int> { };
concept_map CopyConstructible<int*> { };

struct NotCopyConstructible
{
  NotCopyConstructible(NotCopyConstructible&); // { dg-error "candidates are" }
};

struct NotCopyConstructible2
{
  NotCopyConstructible2(NotCopyConstructible2&);
 private:
  NotCopyConstructible2(const NotCopyConstructible2&);
};

concept_map CopyConstructible<NotCopyConstructible> { }; // { dg-error "no matching" }


concept_map CopyConstructible<NotCopyConstructible2> { };
// { dg-error "is private" "" { target *-*-* } 47 }
// { dg-error "within this context" "" { target *-*-* } 53 }
// { dg-error "note" "" { target *-*-* } 50 }

concept IntFloatConstructible<typename T>
{
  T::T(int, float);
};

struct XIntFloat
{
  XIntFloat(int, float);
};

concept_map IntFloatConstructible<XIntFloat> { };
concept_map IntFloatConstructible<int> { }; // { dg-error "constructor initializer" }



concept Destructible<typename T>
{
  T::~T();
};

//concept_map Destructible<X> { };
concept_map Destructible<int> { };


struct NotDestructible {
 private:
  ~NotDestructible();
};
concept_map Destructible<NotDestructible> { };
// { dg-error " is private" "" { target *-*-* } 84 }
// { dg-error "within this context" "" { target *-*-* } 86 }
