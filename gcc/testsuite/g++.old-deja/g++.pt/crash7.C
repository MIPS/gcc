// Build don't link:

class foo 
{
};

template <class T : public foo> // ERROR - expected
struct bar
{
};	// ERROR - expected
