// { dg-options "-std=c++0x" }
// Contributed by Andy Little
#include <concepts>

/*
   Test for Concept fail requires the associated type is invalid,
   and attempt is made to instantiate an object of the associated type
*/

namespace my{

// similar functionality to mpl::void_
   struct void_{};

// compile time operator key for use as Op in...
   struct plus{};

//... "binary_operation" compile time operation
// for result type deduction on runtime types
// Default ::type of void_ here means the operation is invalid
// for TL and TR
   template <typename TL,typename Op, typename TR>
   struct binary_operation{
      typedef void_ type;
   };

// some UDT value_type with runtime ops (not shown)
   template <typename T>
   struct udt{};

// Specialisation of binary_operation for the valid operation of adding 2 udt's
   template<typename T>
   struct binary_operation<udt<T>,plus,udt<T> >{ 
      typedef udt<T> type;
   };

// ... but there are no other valid ops

    auto concept OperatorPlus<typename T1, typename T2>{
        typename type = binary_operation<T1, plus, T2>::type;
        requires ! std::SameType<type,void_>;
    };

// (should be unnecessary) traits class for demo purposes
    template <typename T1, typename T2>
    requires OperatorPlus<T1,T2>
    struct operator_plus{
        typedef OperatorPlus<T1,T2>::type type;
    };
}

int main()
{
//Should succeed
    my::OperatorPlus<my::udt<int>,my::udt<int> >::type res1;

//Should succeed
    my::operator_plus<my::udt<int>,my::udt<int> >::type res3;

//##################### Should fail here ###############

    my::OperatorPlus<int,int>::type res2; // { dg-error "no concept map" }

//#######################################################
}

