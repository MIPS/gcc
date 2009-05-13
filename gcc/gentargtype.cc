#include "system.h"
#include "coretypes.h"
#include "target-types.h"

cumulative_args_u u;
int
main (void)
{
  printf ("typedef union GTY (())\n"
          "  {\n"
	  "    CUMULATIVE_ARGS GTY ((tag (\"TARGET_NUM\"))) _ca;\n");
  printf ("    char GTY ((default)) dummy[%d];\n", (int) sizeof u);
  printf ("    void *GTY ((skip)) align_dummy[1];\n"
	  "  } cumulative_args_u;\n");
  return 0;
}
