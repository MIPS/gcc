#include <stdio.h>
#include <stdlib.h>

class MyClass
{
public:
    MyClass()
    {
        printf("Calling MyClass()\n");
    }
    ~MyClass()
    {
	printf("Calling ~MyClass()\n");
	exit (0);
    }
};
static MyClass myClass;

main()
{
  exit (1);
}
