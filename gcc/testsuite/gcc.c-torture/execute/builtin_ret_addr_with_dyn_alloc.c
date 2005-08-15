/* APPLE LOCAL begin radar 4204303 */
#include <stdio.h>
#include <string.h>
extern void abort();

void* mydlsym(void* handle, const char* symbolName)
{
	// dlsym() assumes symbolName passed in is same as in C source code
	// dyld assumes all symbol names have an underscore prefix
	char underscoredName[strlen(symbolName)+2];
	underscoredName[0] = '_';
	strcpy(&underscoredName[1], symbolName);
		
	// magic "search what I would see" handle
	if ( handle == (void*)-1 ) {
		void* callerAddress = __builtin_return_address(1); // note layers: 1: real client, 0: libSystem glue
		return callerAddress;
	}
	return NULL;
}

int main (int argc, const char * argv[]) {
    void* y = mydlsym((void*)-1, "CFURLCreateFromFileSystemRepresentation");
    void* current = __builtin_return_address(0);
    if (current != y)
      abort();
    return 0;
}
/* APPLE LOCAL end radar 4204303 */
