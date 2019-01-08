// PR c++/71368
// { dg-options "-std=c++2a" }

struct inner;

template<typename X> concept CompoundReq = requires {
    // fine with concrete type in trailing type, i.e. inner& instead of X&
    { X::inner_member() } -> X&;
};

template<typename X> concept Concept = requires {
    { X::outer_member() } -> CompoundReq;
};

struct inner { static inner& inner_member(); };
struct outer { static inner outer_member(); };

int main()
{
    // fine
    static_assert( CompoundReq<inner> );
    static_assert( CompoundReq<decltype( outer::outer_member() )> );

    // ICE
    static_assert( Concept<outer> );
}
