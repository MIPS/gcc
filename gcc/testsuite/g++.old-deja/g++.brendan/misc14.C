// Build don't link: 
// GROUPS passed miscellaneous-bugs
class X {
public:
    enum e {
	New,// ERROR -  conflicts with other.*
    };

    static int New(int);// ERROR -  declaration.*
};

int main() {}
