struct foo {int base; char variable[1]; }; /* a common idiom for variable-size structs */

struct foo * b = (struct foo *) malloc (sizeof (int)); /* enough for base */
b->base = 4;
