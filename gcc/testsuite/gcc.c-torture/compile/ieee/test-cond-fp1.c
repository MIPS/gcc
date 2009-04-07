#define INT_MIN (~__INT_MAX__)

extern int g ();
extern void f ();

extern char ch;
extern float flt;
extern double dbl;
extern long double ldbl;

int x;

void branch_unlt_flt (void) { if (!__builtin_isgreaterequal (flt, 5)) f (g ()); }
void branch_ungt_flt (void) { if (!__builtin_islessequal (flt, 5)) f (g ()); }
void branch_uneq_flt (void) { if (!__builtin_islessgreater (flt, 5)) f (g ()); }
void branch_unle_flt (void) { if (!__builtin_isgreater (flt, 5)) f (g ()); }
void branch_unge_flt (void) { if (!__builtin_isless (flt, 5)) f (g ()); }
void branch_ltgt_flt (void) { if (__builtin_islessgreater (flt, 5)) f (g ()); }
void branch_unord_flt (void) { if (__builtin_isunordered (flt, 5)) f (g ()); }
void branch_ord_flt (void) { if (!__builtin_isunordered (flt, 5)) f (g ()); }

void branch_unlt_dbl (void) { if (!__builtin_isgreaterequal (dbl, 5)) f (g ()); }
void branch_ungt_dbl (void) { if (!__builtin_islessequal (dbl, 5)) f (g ()); }
void branch_uneq_dbl (void) { if (!__builtin_islessgreater (dbl, 5)) f (g ()); }
void branch_unle_dbl (void) { if (!__builtin_isgreater (dbl, 5)) f (g ()); }
void branch_unge_dbl (void) { if (!__builtin_isless (dbl, 5)) f (g ()); }
void branch_ltgt_dbl (void) { if (__builtin_islessgreater (dbl, 5)) f (g ()); }
void branch_unord_dbl (void) { if (__builtin_isunordered (dbl, 5)) f (g ()); }
void branch_ord_dbl (void) { if (!__builtin_isunordered (dbl, 5)) f (g ()); }

void branch_unlt_ldbl (void) { if (!__builtin_isgreaterequal (ldbl, 5)) f (g ()); }
void branch_ungt_ldbl (void) { if (!__builtin_islessequal (ldbl, 5)) f (g ()); }
void branch_uneq_ldbl (void) { if (!__builtin_islessgreater (ldbl, 5)) f (g ()); }
void branch_unle_ldbl (void) { if (!__builtin_isgreater (ldbl, 5)) f (g ()); }
void branch_unge_ldbl (void) { if (!__builtin_isless (ldbl, 5)) f (g ()); }
void branch_ltgt_ldbl (void) { if (__builtin_islessgreater (ldbl, 5)) f (g ()); }
void branch_unord_ldbl (void) { if (__builtin_isunordered (ldbl, 5)) f (g ()); }
void branch_ord_ldbl (void) { if (!__builtin_isunordered (ldbl, 5)) f (g ()); }

int scc_unlt_flt (void) { return (!__builtin_isgreaterequal (flt, 5)); }
int scc_ungt_flt (void) { return (!__builtin_islessequal (flt, 5)); }
int scc_uneq_flt (void) { return (!__builtin_islessgreater (flt, 5)); }
int scc_unle_flt (void) { return (!__builtin_isgreater (flt, 5)); }
int scc_unge_flt (void) { return (!__builtin_isless (flt, 5)); }
int scc_ltgt_flt (void) { return (__builtin_islessgreater (flt, 5)); }
int scc_unord_flt (void) { return (__builtin_isunordered (flt, 5)); }
int scc_ord_flt (void) { return (!__builtin_isunordered (flt, 5)); }

int scc_unlt_dbl (void) { return (!__builtin_isgreaterequal (dbl, 5)); }
int scc_ungt_dbl (void) { return (!__builtin_islessequal (dbl, 5)); }
int scc_uneq_dbl (void) { return (!__builtin_islessgreater (dbl, 5)); }
int scc_unle_dbl (void) { return (!__builtin_isgreater (dbl, 5)); }
int scc_unge_dbl (void) { return (!__builtin_isless (dbl, 5)); }
int scc_ltgt_dbl (void) { return (__builtin_islessgreater (dbl, 5)); }
int scc_unord_dbl (void) { return (__builtin_isunordered (dbl, 5)); }
int scc_ord_dbl (void) { return (!__builtin_isunordered (dbl, 5)); }

int scc_unlt_ldbl (void) { return (!__builtin_isgreaterequal (ldbl, 5)); }
int scc_ungt_ldbl (void) { return (!__builtin_islessequal (ldbl, 5)); }
int scc_uneq_ldbl (void) { return (!__builtin_islessgreater (ldbl, 5)); }
int scc_unle_ldbl (void) { return (!__builtin_isgreater (ldbl, 5)); }
int scc_unge_ldbl (void) { return (!__builtin_isless (ldbl, 5)); }
int scc_ltgt_ldbl (void) { return (__builtin_islessgreater (ldbl, 5)); }
int scc_unord_ldbl (void) { return (__builtin_isunordered (ldbl, 5)); }
int scc_ord_ldbl (void) { return (!__builtin_isunordered (ldbl, 5)); }

int movsicc_unlt_flt (void) { return (!__builtin_isgreaterequal (flt, 5)) ? x : 10; }
int movsicc_ungt_flt (void) { return (!__builtin_islessequal (flt, 5)) ? x : 10; }
int movsicc_uneq_flt (void) { return (!__builtin_islessgreater (flt, 5)) ? x : 10; }
int movsicc_unle_flt (void) { return (!__builtin_isgreater (flt, 5)) ? x : 10; }
int movsicc_unge_flt (void) { return (!__builtin_isless (flt, 5)) ? x : 10; }
int movsicc_ltgt_flt (void) { return (__builtin_islessgreater (flt, 5)) ? x : 10; }
int movsicc_unord_flt (void) { return (__builtin_isunordered (flt, 5)) ? x : 10; }
int movsicc_ord_flt (void) { return (!__builtin_isunordered (flt, 5)) ? x : 10; }

int movsicc_unlt_dbl (void) { return (!__builtin_isgreaterequal (dbl, 5)) ? x : 10; }
int movsicc_ungt_dbl (void) { return (!__builtin_islessequal (dbl, 5)) ? x : 10; }
int movsicc_uneq_dbl (void) { return (!__builtin_islessgreater (dbl, 5)) ? x : 10; }
int movsicc_unle_dbl (void) { return (!__builtin_isgreater (dbl, 5)) ? x : 10; }
int movsicc_unge_dbl (void) { return (!__builtin_isless (dbl, 5)) ? x : 10; }
int movsicc_ltgt_dbl (void) { return (__builtin_islessgreater (dbl, 5)) ? x : 10; }
int movsicc_unord_dbl (void) { return (__builtin_isunordered (dbl, 5)) ? x : 10; }
int movsicc_ord_dbl (void) { return (!__builtin_isunordered (dbl, 5)) ? x : 10; }

int movsicc_unlt_ldbl (void) { return (!__builtin_isgreaterequal (ldbl, 5)) ? x : 10; }
int movsicc_ungt_ldbl (void) { return (!__builtin_islessequal (ldbl, 5)) ? x : 10; }
int movsicc_uneq_ldbl (void) { return (!__builtin_islessgreater (ldbl, 5)) ? x : 10; }
int movsicc_unle_ldbl (void) { return (!__builtin_isgreater (ldbl, 5)) ? x : 10; }
int movsicc_unge_ldbl (void) { return (!__builtin_isless (ldbl, 5)) ? x : 10; }
int movsicc_ltgt_ldbl (void) { return (__builtin_islessgreater (ldbl, 5)) ? x : 10; }
int movsicc_unord_ldbl (void) { return (__builtin_isunordered (ldbl, 5)) ? x : 10; }
int movsicc_ord_ldbl (void) { return (!__builtin_isunordered (ldbl, 5)) ? x : 10; }

float movsfcc_unlt_flt (void) { return (!__builtin_isgreaterequal (flt, 5)) ? flt : 10; }
float movsfcc_ungt_flt (void) { return (!__builtin_islessequal (flt, 5)) ? flt : 10; }
float movsfcc_uneq_flt (void) { return (!__builtin_islessgreater (flt, 5)) ? flt : 10; }
float movsfcc_unle_flt (void) { return (!__builtin_isgreater (flt, 5)) ? flt : 10; }
float movsfcc_unge_flt (void) { return (!__builtin_isless (flt, 5)) ? flt : 10; }
float movsfcc_ltgt_flt (void) { return (__builtin_islessgreater (flt, 5)) ? flt : 10; }
float movsfcc_unord_flt (void) { return (__builtin_isunordered (flt, 5)) ? flt : 10; }
float movsfcc_ord_flt (void) { return (!__builtin_isunordered (flt, 5)) ? flt : 10; }

float movsfcc_unlt_dbl (void) { return (!__builtin_isgreaterequal (dbl, 5)) ? flt : 10; }
float movsfcc_ungt_dbl (void) { return (!__builtin_islessequal (dbl, 5)) ? flt : 10; }
float movsfcc_uneq_dbl (void) { return (!__builtin_islessgreater (dbl, 5)) ? flt : 10; }
float movsfcc_unle_dbl (void) { return (!__builtin_isgreater (dbl, 5)) ? flt : 10; }
float movsfcc_unge_dbl (void) { return (!__builtin_isless (dbl, 5)) ? flt : 10; }
float movsfcc_ltgt_dbl (void) { return (__builtin_islessgreater (dbl, 5)) ? flt : 10; }
float movsfcc_unord_dbl (void) { return (__builtin_isunordered (dbl, 5)) ? flt : 10; }
float movsfcc_ord_dbl (void) { return (!__builtin_isunordered (dbl, 5)) ? flt : 10; }

float movsfcc_unlt_ldbl (void) { return (!__builtin_isgreaterequal (ldbl, 5)) ? flt : 10; }
float movsfcc_ungt_ldbl (void) { return (!__builtin_islessequal (ldbl, 5)) ? flt : 10; }
float movsfcc_uneq_ldbl (void) { return (!__builtin_islessgreater (ldbl, 5)) ? flt : 10; }
float movsfcc_unle_ldbl (void) { return (!__builtin_isgreater (ldbl, 5)) ? flt : 10; }
float movsfcc_unge_ldbl (void) { return (!__builtin_isless (ldbl, 5)) ? flt : 10; }
float movsfcc_ltgt_ldbl (void) { return (__builtin_islessgreater (ldbl, 5)) ? flt : 10; }
float movsfcc_unord_ldbl (void) { return (__builtin_isunordered (ldbl, 5)) ? flt : 10; }
float movsfcc_ord_ldbl (void) { return (!__builtin_isunordered (ldbl, 5)) ? flt : 10; }

void mem_movsicc_unlt_flt (void) { x = (!__builtin_isgreaterequal (flt, 5)) ? x : 10; }
void mem_movsicc_ungt_flt (void) { x = (!__builtin_islessequal (flt, 5)) ? x : 10; }
void mem_movsicc_uneq_flt (void) { x = (!__builtin_islessgreater (flt, 5)) ? x : 10; }
void mem_movsicc_unle_flt (void) { x = (!__builtin_isgreater (flt, 5)) ? x : 10; }
void mem_movsicc_unge_flt (void) { x = (!__builtin_isless (flt, 5)) ? x : 10; }
void mem_movsicc_ltgt_flt (void) { x = (__builtin_islessgreater (flt, 5)) ? x : 10; }
void mem_movsicc_unord_flt (void) { x = (__builtin_isunordered (flt, 5)) ? x : 10; }
void mem_movsicc_ord_flt (void) { x = (!__builtin_isunordered (flt, 5)) ? x : 10; }

void mem_movsicc_unlt_dbl (void) { x = (!__builtin_isgreaterequal (dbl, 5)) ? x : 10; }
void mem_movsicc_ungt_dbl (void) { x = (!__builtin_islessequal (dbl, 5)) ? x : 10; }
void mem_movsicc_uneq_dbl (void) { x = (!__builtin_islessgreater (dbl, 5)) ? x : 10; }
void mem_movsicc_unle_dbl (void) { x = (!__builtin_isgreater (dbl, 5)) ? x : 10; }
void mem_movsicc_unge_dbl (void) { x = (!__builtin_isless (dbl, 5)) ? x : 10; }
void mem_movsicc_ltgt_dbl (void) { x = (__builtin_islessgreater (dbl, 5)) ? x : 10; }
void mem_movsicc_unord_dbl (void) { x = (__builtin_isunordered (dbl, 5)) ? x : 10; }
void mem_movsicc_ord_dbl (void) { x = (!__builtin_isunordered (dbl, 5)) ? x : 10; }

void mem_movsicc_unlt_ldbl (void) { x = (!__builtin_isgreaterequal (ldbl, 5)) ? x : 10; }
void mem_movsicc_ungt_ldbl (void) { x = (!__builtin_islessequal (ldbl, 5)) ? x : 10; }
void mem_movsicc_uneq_ldbl (void) { x = (!__builtin_islessgreater (ldbl, 5)) ? x : 10; }
void mem_movsicc_unle_ldbl (void) { x = (!__builtin_isgreater (ldbl, 5)) ? x : 10; }
void mem_movsicc_unge_ldbl (void) { x = (!__builtin_isless (ldbl, 5)) ? x : 10; }
void mem_movsicc_ltgt_ldbl (void) { x = (__builtin_islessgreater (ldbl, 5)) ? x : 10; }
void mem_movsicc_unord_ldbl (void) { x = (__builtin_isunordered (ldbl, 5)) ? x : 10; }
void mem_movsicc_ord_ldbl (void) { x = (!__builtin_isunordered (ldbl, 5)) ? x : 10; }

void mem_movsfcc_unlt_flt (void) { flt = (!__builtin_isgreaterequal (flt, 5)) ? flt : 10; }
void mem_movsfcc_ungt_flt (void) { flt = (!__builtin_islessequal (flt, 5)) ? flt : 10; }
void mem_movsfcc_uneq_flt (void) { flt = (!__builtin_islessgreater (flt, 5)) ? flt : 10; }
void mem_movsfcc_unle_flt (void) { flt = (!__builtin_isgreater (flt, 5)) ? flt : 10; }
void mem_movsfcc_unge_flt (void) { flt = (!__builtin_isless (flt, 5)) ? flt : 10; }
void mem_movsfcc_ltgt_flt (void) { flt = (__builtin_islessgreater (flt, 5)) ? flt : 10; }
void mem_movsfcc_unord_flt (void) { flt = (__builtin_isunordered (flt, 5)) ? flt : 10; }
void mem_movsfcc_ord_flt (void) { flt = (!__builtin_isunordered (flt, 5)) ? flt : 10; }

void mem_movsfcc_unlt_dbl (void) { flt = (!__builtin_isgreaterequal (dbl, 5)) ? flt : 10; }
void mem_movsfcc_ungt_dbl (void) { flt = (!__builtin_islessequal (dbl, 5)) ? flt : 10; }
void mem_movsfcc_uneq_dbl (void) { flt = (!__builtin_islessgreater (dbl, 5)) ? flt : 10; }
void mem_movsfcc_unle_dbl (void) { flt = (!__builtin_isgreater (dbl, 5)) ? flt : 10; }
void mem_movsfcc_unge_dbl (void) { flt = (!__builtin_isless (dbl, 5)) ? flt : 10; }
void mem_movsfcc_ltgt_dbl (void) { flt = (__builtin_islessgreater (dbl, 5)) ? flt : 10; }
void mem_movsfcc_unord_dbl (void) { flt = (__builtin_isunordered (dbl, 5)) ? flt : 10; }
void mem_movsfcc_ord_dbl (void) { flt = (!__builtin_isunordered (dbl, 5)) ? flt : 10; }

void mem_movsfcc_unlt_ldbl (void) { flt = (!__builtin_isgreaterequal (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_ungt_ldbl (void) { flt = (!__builtin_islessequal (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_uneq_ldbl (void) { flt = (!__builtin_islessgreater (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_unle_ldbl (void) { flt = (!__builtin_isgreater (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_unge_ldbl (void) { flt = (!__builtin_isless (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_ltgt_ldbl (void) { flt = (__builtin_islessgreater (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_unord_ldbl (void) { flt = (__builtin_isunordered (ldbl, 5)) ? flt : 10; }
void mem_movsfcc_ord_ldbl (void) { flt = (!__builtin_isunordered (ldbl, 5)) ? flt : 10; }

