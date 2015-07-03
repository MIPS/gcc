/* Tuning options:

   -mcost=[size|i386|i486|pentium|pentiumpro|geode|k6|athlon|k8|amdfam10|bdver1|bdver2|bdver3|bdver4|btver1|btver2|pentium4|nocona|atom|slm|generic|core]

   -missue-rate=NUMBER

   -madjust-cost=NUMBER

   -mmultipass-dfa-lookahead=NUMBER

 */  

extern void x86_tune_options (void);

extern int ix86_issue_rate_value;

extern int ix86_adjust_cost_value;

extern int ia32_multipass_dfa_lookahead_value;
