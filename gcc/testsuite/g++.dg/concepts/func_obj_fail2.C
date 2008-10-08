#include <concepts>

template<
    typename inner_functor_type,
    typename argument_type
>
requires
    std::CopyConstructible< inner_functor_type >,
    std::Predicate< inner_functor_type, argument_type >
class foo
{

    inner_functor_type inner_functor;

public:

    foo( const inner_functor_type & inner_functor )
      : inner_functor( inner_functor )
    {
    }

    bool operator()( const argument_type & argument )
    {
        return inner_functor( argument );
    }

};
