// Build don't link: 
// GROUPS passed old-abort
typedef int element;
class Pix {
public:
    Pix();
    Pix(const Pix&);

    // Friend functions so that v == x works as does x == v works
    friend int operator==(void *v, const Pix& x) // ERROR - .*
        { return v == index; }
    friend int operator==(void *v, const Pix& x) // ERROR - .*
        { return v != index; }
private:
//    friend class List<T>;
    element *index;
};
