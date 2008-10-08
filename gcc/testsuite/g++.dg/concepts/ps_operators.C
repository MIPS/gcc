//#define NO_ERRORS
#include <concepts>
typedef __SIZE_TYPE__ size_t;

struct X {};

struct XHasNewDelete
{
  void* operator new(size_t size);
  void* operator new[](size_t size);
  void operator delete(void*);
  void operator delete[](void*);
};


concept Newable<typename T>
{
  void* T::operator new(size_t size);
};

concept_map Newable<X> { };
concept_map Newable<XHasNewDelete> { };
concept_map Newable<int> { };

#ifndef NO_ERRORS
struct XNotNewableDeletable
{
 private:
  void* operator new(size_t size);
  void* operator new[](size_t size);
  void operator delete(void*);
  void operator delete[](void*);
};
concept_map Newable<XNotNewableDeletable> { };
// { dg-error "is private" "" { target *-*-* } 29 }
// { dg-error "within this context" "" { target *-*-* } 34 }
#endif


concept ArrayNewable<typename T>
{
  void* T::operator new[](size_t size);
};

concept_map ArrayNewable<X> { };
concept_map ArrayNewable<XHasNewDelete> { };
concept_map ArrayNewable<int> { };

#ifndef NO_ERRORS
concept_map ArrayNewable<XNotNewableDeletable> { };
// { dg-error "is private" "" { target *-*-* } 30 }
// { dg-error "within this context" "" { target *-*-* } 50 }
#endif


concept Deletable<typename T>
{
  void T::operator delete(void*);
};

concept_map Deletable<X> { };
concept_map Deletable<XHasNewDelete> { };
concept_map Deletable<int> { };

#ifndef NO_ERRORS
concept_map Deletable<XNotNewableDeletable> { };
// { dg-error "is private" "" { target *-*-* } 31 }
// { dg-error "within this context" "" { target *-*-* } 66 }
#endif


concept ArrayDeletable<typename T>
{
  void T::operator delete[](void*);
};

concept_map ArrayDeletable<X> { };
concept_map ArrayDeletable<XHasNewDelete> { };
concept_map ArrayDeletable<int> { };

#ifndef NO_ERRORS
concept_map ArrayDeletable<XNotNewableDeletable> { };
// { dg-error "is private" "" { target *-*-* } 32 }
// { dg-error "within this context" "" { target *-*-* } 82 }
#endif


concept UnaryPlus<typename T>
{
  typename result_type = T;
  result_type operator+(T);
};

concept_map UnaryPlus<int> { };


concept UnaryMinus<typename T>
{
  typename result_type = T;
  result_type operator-(T);
};

concept_map UnaryMinus<int> { };


concept Addressable<typename T>
{
  //  typename result_type = T*; // TBD: triggers a bug in the parser
  T* operator&(T&);
};

concept_map Addressable<int> { };


concept Dereferenceable<typename T>
{
  typename result_type;
  result_type operator*(T);
};

concept_map Dereferenceable<int*> { typedef int result_type; };



concept BitNot<typename T>
{
  typename result_type = T;
  result_type operator~(T);
};

concept_map BitNot<int> { };



concept TruthNot<typename T>
{
  typename result_type = T;
  result_type operator~(T);
};

concept_map TruthNot<int> { };


concept Preincrementable<typename T>
{
  //  typename result_type = T&; // TBD: triggers a bug in the parser
 T& operator++(T&);
};

concept_map Preincrementable<int> { };


concept Predecrementable<typename T>
{
  //  typename result_type = T&; // TBD: triggers a bug in the parser
 T& operator--(T&);
};

concept_map Predecrementable<int> { };

// TBD: cast operators?


concept Addable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator+(T, U);
};

concept_map Addable<int> { };


concept Subtractable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator-(T, U);
};

concept_map Subtractable<int> { };


concept Multiplicable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator*(T, U);
};

concept_map Multiplicable<int> { };


concept Divisible<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator/(T, U);
};

concept_map Divisible<int> { };


concept Moddable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator%(T, U);
};

concept_map Moddable<int> { };


concept BitAndable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator&(T, U);
};

concept_map BitAndable<int> { };


concept BitOrable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator|(T, U);
};

concept_map BitOrable<int> { };


concept BitXorable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator^(T, U);
};

concept_map BitXorable<int> { };


concept LeftShiftable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator<<(T, U);
};

concept_map LeftShiftable<int> { };


concept RightShiftable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator>>(T, U);
};

concept_map RightShiftable<int> { };


concept EqualComparable<typename T, typename U = T>
{
  bool operator==(T, U);
};

concept_map EqualComparable<int> { };


concept NotEqualComparable<typename T, typename U = T>
{
  bool operator!=(T, U);
};

concept_map NotEqualComparable<int> { };


concept LessThanComparable<typename T, typename U = T>
{
  bool operator<(T, U);
};

concept_map LessThanComparable<int> { };


concept GreaterThanComparable<typename T, typename U = T>
{
  bool operator>(T, U);
};

concept_map GreaterThanComparable<int> { };


concept LessEqualComparable<typename T, typename U = T>
{
  bool operator<=(T, U);
};

concept_map LessEqualComparable<int> { };


concept GreaterEqualComparable<typename T, typename U = T>
{
  bool operator>=(T, U);
};

concept_map GreaterEqualComparable<int> { };


concept TruthAndable<typename T, typename U = T>
{
  bool operator&&(T, U);
};

concept_map TruthAndable<int> { };


concept TruthOrable<typename T, typename U = T>
{
  bool operator||(T, U);
};

concept_map TruthOrable<int> { };


concept Commable<typename T, typename U = T>
{
  typename result_type = U;
  result_type operator,(T, U);
};

concept_map Commable<int> { };

struct CommaLeft {};
struct CommaRight {};
struct CommaResult {};
CommaResult operator,(CommaLeft, CommaRight);

concept_map Commable<CommaLeft, CommaRight> { typedef CommaResult result_type; };

struct CommaRight2 {};
CommaResult operator,(CommaLeft, CommaRight2);

#ifndef NO_ERRORS
concept_map Commable<CommaLeft, CommaRight2> { typedef int result_type; }; // { dg-error "cannot convert" }
#endif

// TBD: UNCHECKED!

concept ArrowStarable<typename T>
{
  int& operator->*(T*, int T::*);
};

concept_map ArrowStarable<X> { };


concept Arrowable<typename Ptrlike, typename Value>
{
  typename arrow_result;
  arrow_result operator->(Ptrlike);
  requires Arrowable<arrow_result, Value>; // { dg-error "recursive" }
};

concept_map Arrowable<X*, X*> { typedef X* arrow_result; };

struct Y {};
#ifndef NO_ERRORS
concept_map Arrowable<Y*, Y*>
{
  typedef Y arrow_result;
}; // { dg-error "conversion from" }


#endif


concept Postincrementable<typename T>
{
  //  typename result_type = T&; // TBD: triggers a bug in the parser
  T operator++(T&, int);
};

concept_map Postincrementable<int> { };


concept Postdecrementable<typename T>
{
  //  typename result_type = T&; // TBD: triggers a bug in the parser
  T operator--(T&, int);
};

concept_map Postdecrementable<int> { };


concept Assignable<typename T, typename U = T>
{
  T& T::operator=(const U&);
};

concept_map Assignable<int, int> {};

#ifndef NO_ERRORS
concept_map Assignable<int, float*> {}; // { dg-error "invalid conversion" }
#endif


concept PlusAssignable<typename T, typename U = T>
{
  T& operator+=(T&, const U&);
};

concept_map PlusAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map PlusAssignable<int, float*> {}; // { dg-error "invalid conversion" }
#endif


concept MinusAssignable<typename T, typename U = T>
{
  T& operator-=(T&, const U&);
};

concept_map MinusAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map MinusAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 420 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 420 }
// { dg-error "MinusAssignable" "" { target *-*-* } 420 }
#endif


concept MultiplyAssignable<typename T, typename U = T>
{
  T& operator*=(T&, const U&);
};

concept_map MultiplyAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map MultiplyAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 435 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 435 }
// { dg-error "MultiplyAssignable" "" { target *-*-* } 435 }
#endif


concept DivideAssignable<typename T, typename U = T>
{
  T& operator/=(T&, const U&);
};

concept_map DivideAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map DivideAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 450 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 450 }
// { dg-error "DivideAssignable" "" { target *-*-* } 450 }
#endif


concept ModAssignable<typename T, typename U = T>
{
  T& operator%=(T&, const U&);
};

concept_map ModAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map ModAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 465 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 465 }
// { dg-error "ModAssignable" "" { target *-*-* } 465 }
#endif


concept BitAndAssignable<typename T, typename U = T>
{
  T& operator&=(T&, const U&);
};

concept_map BitAndAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map BitAndAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 480 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 480 }
// { dg-error "BitAndAssignable" "" { target *-*-* } 480 }
#endif


concept BitOrAssignable<typename T, typename U = T>
{
  T& operator|=(T&, const U&);
};

concept_map BitOrAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map BitOrAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 495 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 495 }
// { dg-error "BitOrAssignable" "" { target *-*-* } 495 }
#endif


concept BitXorAssignable<typename T, typename U = T>
{
  T& operator^=(T&, const U&);
};

concept_map BitXorAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map BitXorAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 510 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 510 }
// { dg-error "BitXorAssignable" "" { target *-*-* } 510 }
#endif


concept LshiftAssignable<typename T, typename U = T>
{
  T& operator<<=(T&, const U&);
};

concept_map LshiftAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map LshiftAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 525 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 525 }
// { dg-error "LshiftAssignable" "" { target *-*-* } 525 }
#endif


concept RshiftAssignable<typename T, typename U = T>
{
  T& operator>>=(T&, const U&);
};

concept_map RshiftAssignable<int, int> {};

#ifndef NO_ERRORS
concept_map RshiftAssignable<int, float*> {}; // { dg-error "invalid operands" }
// { dg-error "in evaluation of" "" { target *-*-* } 540 }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 540 }
// { dg-error "RshiftAssignable" "" { target *-*-* } 540 }
#endif


concept Callable2<typename F, typename T1, typename T2>
{
  typename result_type = bool;
  result_type operator()(const F&, T1, T2);
};

concept_map Callable2<bool(int, double), short int, float> { };

#ifndef NO_ERRORS
concept_map Callable2<bool(int*, double), short int, float> { }; // { dg-error "invalid conversion" }
#endif


concept Convertible<typename T, typename U>
{
  operator U(const T&);
};

concept_map Convertible<int, int> {};
concept_map Convertible<int, float> {};
concept_map Convertible<int*, char&> {}; // { dg-error "invalid initialization of reference" }



concept Subscriptable<typename T>
{
  typename reference;

  reference operator[](T&, int);
};

concept_map Subscriptable<int*> { typedef int& reference; };
concept_map Subscriptable<const int*> { typedef const int& reference; };

struct XSubscriptable
{
  float& operator[](int);
};
concept_map Subscriptable<XSubscriptable> { typedef float& reference; };
