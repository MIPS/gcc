#include "system.h"
#include "coretypes.h"
#include "target-types.h"

int
main (void)
{
  printf ("typedef union GTY (())\n"
          "  {\n"
	  "    CUMULATIVE_ARGS GTY ((tag (\"TARGET_NUM\"))) _ca;\n");
  printf ("    char GTY ((default)) dummy[%d];\n",
	  (int) sizeof (cumulative_args_u));
  printf ("    void *GTY ((skip)) align_dummy[1];\n"
	  "  } cumulative_args_u;\n");
  printf ("\n"
	  "START_TARGET_SPECIFIC\n"
	  "struct machine_function;\n"
	  "END_TARGET_SPECIFIC\n"
	  "\n"
	  "typedef union GTY (())\n"
          "  {\n"
	  "    struct TARGET_SCOPE machine_function\n"
	  "      *GTY ((tag (\"TARGET_NUM\"), maybe_undef)) _mf;\n");
  printf ("    char (*GTY ((default)) dummy)[%d];\n",
	  (int) sizeof (machine_function_u));
  printf ("    void *(*GTY ((skip)) align_dummy)[1];\n"
	  "  } machine_function_u;\n");
  return 0;
}
