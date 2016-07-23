// { dg-options "-gdwarf-2 -dA" }
// { dg-final { scan-assembler-times "DIE \\(\[^\n\]*\\) DW_TAG_template_value_param" 1 } }
// { dg-final { scan-assembler-times "DIE \\(\[^\n\]*\\) DW_TAG_template_value_param\[^\n\]*\n\[^\n\]* DW_AT_name\n\[^\n\]* DW_AT_type\n\[^\n\]*\[^\n\]* DW_AT_location\n\[^\n\]* DW_OP_addr\n\[^\n\]*_ZN1S1fEv\[^\n\]*\n\[^\n\]* DW_OP_stack_value\n\[^\n\]* DW_OP_piece\n\[^\n\]*\n\[^\n\]* DW_OP_lit0\n\[^\n\]* DW_OP_stack_value\n\[^\n\]* DW_OP_piece" 1 } }
#include "template-params-12.H"
template void t<&S::f>();
