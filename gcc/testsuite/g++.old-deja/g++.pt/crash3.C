// Build don't link:

template <class Type>
class CVector {
public:
    CVector<int> f() const
    {
       CVector<int> v(n); // ERROR - no declaration of `n'
       return v;
    }
    CVector<long> g() const
    {
       CVector<long> v(n); // ERROR - no declaration of `n'
       return v;
    }
};
