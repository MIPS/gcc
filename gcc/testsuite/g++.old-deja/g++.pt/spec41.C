// Build don't link:

// Origin: <mikes@nilenet.com>

// Bug: Specialization of implicitly created function should be
// rejected.

template<class T> class blah{}; // ERROR - previous declaration
blah<char>::blah(){}		// ERROR - invalid specialization
int main(){}
