#define INT_MIN (~__INT_MAX__)

extern int g ();
extern void f ();

extern char ch;
extern float flt;
extern double dbl;
extern long double ldbl;

int x;

void branch_lt_flt (void) { if (flt < 5) f (g ()); }
void branch_eq_flt (void) { if (flt == 5) f (g ()); }
void branch_ne_flt (void) { if (flt != 5) f (g ()); }
void branch_gt_flt (void) { if (flt > 5) f (g ()); }
void branch_le_flt (void) { if (flt <= 5) f (g ()); }
void branch_ge_flt (void) { if (flt >= 5) f (g ()); }

void branch_lt_dbl (void) { if (dbl < 5) f (g ()); }
void branch_eq_dbl (void) { if (dbl == 5) f (g ()); }
void branch_ne_dbl (void) { if (dbl != 5) f (g ()); }
void branch_gt_dbl (void) { if (dbl > 5) f (g ()); }
void branch_le_dbl (void) { if (dbl <= 5) f (g ()); }
void branch_ge_dbl (void) { if (dbl >= 5) f (g ()); }

void branch_lt_ldbl (void) { if (ldbl < 5) f (g ()); }
void branch_eq_ldbl (void) { if (ldbl == 5) f (g ()); }
void branch_ne_ldbl (void) { if (ldbl != 5) f (g ()); }
void branch_gt_ldbl (void) { if (ldbl > 5) f (g ()); }
void branch_le_ldbl (void) { if (ldbl <= 5) f (g ()); }
void branch_ge_ldbl (void) { if (ldbl >= 5) f (g ()); }

int scc_lt_flt (void) { return (flt < 5); }
int scc_eq_flt (void) { return (flt == 5); }
int scc_ne_flt (void) { return (flt != 5); }
int scc_gt_flt (void) { return (flt > 5); }
int scc_le_flt (void) { return (flt <= 5); }
int scc_ge_flt (void) { return (flt >= 5); }

int scc_lt_dbl (void) { return (dbl < 5); }
int scc_eq_dbl (void) { return (dbl == 5); }
int scc_ne_dbl (void) { return (dbl != 5); }
int scc_gt_dbl (void) { return (dbl > 5); }
int scc_le_dbl (void) { return (dbl <= 5); }
int scc_ge_dbl (void) { return (dbl >= 5); }

int scc_lt_ldbl (void) { return (ldbl < 5); }
int scc_eq_ldbl (void) { return (ldbl == 5); }
int scc_ne_ldbl (void) { return (ldbl != 5); }
int scc_gt_ldbl (void) { return (ldbl > 5); }
int scc_le_ldbl (void) { return (ldbl <= 5); }
int scc_ge_ldbl (void) { return (ldbl >= 5); }

int movsicc_lt_flt (void) { return (flt < 5) ? x : 10; }
int movsicc_eq_flt (void) { return (flt == 5) ? x : 10; }
int movsicc_ne_flt (void) { return (flt != 5) ? x : 10; }
int movsicc_gt_flt (void) { return (flt > 5) ? x : 10; }
int movsicc_le_flt (void) { return (flt <= 5) ? x : 10; }
int movsicc_ge_flt (void) { return (flt >= 5) ? x : 10; }

int movsicc_lt_dbl (void) { return (dbl < 5) ? x : 10; }
int movsicc_eq_dbl (void) { return (dbl == 5) ? x : 10; }
int movsicc_ne_dbl (void) { return (dbl != 5) ? x : 10; }
int movsicc_gt_dbl (void) { return (dbl > 5) ? x : 10; }
int movsicc_le_dbl (void) { return (dbl <= 5) ? x : 10; }
int movsicc_ge_dbl (void) { return (dbl >= 5) ? x : 10; }

int movsicc_lt_ldbl (void) { return (ldbl < 5) ? x : 10; }
int movsicc_eq_ldbl (void) { return (ldbl == 5) ? x : 10; }
int movsicc_ne_ldbl (void) { return (ldbl != 5) ? x : 10; }
int movsicc_gt_ldbl (void) { return (ldbl > 5) ? x : 10; }
int movsicc_le_ldbl (void) { return (ldbl <= 5) ? x : 10; }
int movsicc_ge_ldbl (void) { return (ldbl >= 5) ? x : 10; }

float movsfcc_lt_flt (void) { return (flt < 5) ? flt : 10; }
float movsfcc_eq_flt (void) { return (flt == 5) ? flt : 10; }
float movsfcc_ne_flt (void) { return (flt != 5) ? flt : 10; }
float movsfcc_gt_flt (void) { return (flt > 5) ? flt : 10; }
float movsfcc_le_flt (void) { return (flt <= 5) ? flt : 10; }
float movsfcc_ge_flt (void) { return (flt >= 5) ? flt : 10; }

float movsfcc_lt_dbl (void) { return (dbl < 5) ? flt : 10; }
float movsfcc_eq_dbl (void) { return (dbl == 5) ? flt : 10; }
float movsfcc_ne_dbl (void) { return (dbl != 5) ? flt : 10; }
float movsfcc_gt_dbl (void) { return (dbl > 5) ? flt : 10; }
float movsfcc_le_dbl (void) { return (dbl <= 5) ? flt : 10; }
float movsfcc_ge_dbl (void) { return (dbl >= 5) ? flt : 10; }

float movsfcc_lt_ldbl (void) { return (ldbl < 5) ? flt : 10; }
float movsfcc_eq_ldbl (void) { return (ldbl == 5) ? flt : 10; }
float movsfcc_ne_ldbl (void) { return (ldbl != 5) ? flt : 10; }
float movsfcc_gt_ldbl (void) { return (ldbl > 5) ? flt : 10; }
float movsfcc_le_ldbl (void) { return (ldbl <= 5) ? flt : 10; }
float movsfcc_ge_ldbl (void) { return (ldbl >= 5) ? flt : 10; }

void mem_movsicc_lt_flt (void) { x = (flt < 5) ? x : 10; }
void mem_movsicc_eq_flt (void) { x = (flt == 5) ? x : 10; }
void mem_movsicc_ne_flt (void) { x = (flt != 5) ? x : 10; }
void mem_movsicc_gt_flt (void) { x = (flt > 5) ? x : 10; }
void mem_movsicc_le_flt (void) { x = (flt <= 5) ? x : 10; }
void mem_movsicc_ge_flt (void) { x = (flt >= 5) ? x : 10; }

void mem_movsicc_lt_dbl (void) { x = (dbl < 5) ? x : 10; }
void mem_movsicc_eq_dbl (void) { x = (dbl == 5) ? x : 10; }
void mem_movsicc_ne_dbl (void) { x = (dbl != 5) ? x : 10; }
void mem_movsicc_gt_dbl (void) { x = (dbl > 5) ? x : 10; }
void mem_movsicc_le_dbl (void) { x = (dbl <= 5) ? x : 10; }
void mem_movsicc_ge_dbl (void) { x = (dbl >= 5) ? x : 10; }

void mem_movsicc_lt_ldbl (void) { x = (ldbl < 5) ? x : 10; }
void mem_movsicc_eq_ldbl (void) { x = (ldbl == 5) ? x : 10; }
void mem_movsicc_ne_ldbl (void) { x = (ldbl != 5) ? x : 10; }
void mem_movsicc_gt_ldbl (void) { x = (ldbl > 5) ? x : 10; }
void mem_movsicc_le_ldbl (void) { x = (ldbl <= 5) ? x : 10; }
void mem_movsicc_ge_ldbl (void) { x = (ldbl >= 5) ? x : 10; }

void mem_movsfcc_lt_flt (void) { flt = (flt < 5) ? flt : 10; }
void mem_movsfcc_eq_flt (void) { flt = (flt == 5) ? flt : 10; }
void mem_movsfcc_ne_flt (void) { flt = (flt != 5) ? flt : 10; }
void mem_movsfcc_gt_flt (void) { flt = (flt > 5) ? flt : 10; }
void mem_movsfcc_le_flt (void) { flt = (flt <= 5) ? flt : 10; }
void mem_movsfcc_ge_flt (void) { flt = (flt >= 5) ? flt : 10; }

void mem_movsfcc_lt_dbl (void) { flt = (dbl < 5) ? flt : 10; }
void mem_movsfcc_eq_dbl (void) { flt = (dbl == 5) ? flt : 10; }
void mem_movsfcc_ne_dbl (void) { flt = (dbl != 5) ? flt : 10; }
void mem_movsfcc_gt_dbl (void) { flt = (dbl > 5) ? flt : 10; }
void mem_movsfcc_le_dbl (void) { flt = (dbl <= 5) ? flt : 10; }
void mem_movsfcc_ge_dbl (void) { flt = (dbl >= 5) ? flt : 10; }

void mem_movsfcc_lt_ldbl (void) { flt = (ldbl < 5) ? flt : 10; }
void mem_movsfcc_eq_ldbl (void) { flt = (ldbl == 5) ? flt : 10; }
void mem_movsfcc_ne_ldbl (void) { flt = (ldbl != 5) ? flt : 10; }
void mem_movsfcc_gt_ldbl (void) { flt = (ldbl > 5) ? flt : 10; }
void mem_movsfcc_le_ldbl (void) { flt = (ldbl <= 5) ? flt : 10; }
void mem_movsfcc_ge_ldbl (void) { flt = (ldbl >= 5) ? flt : 10; }

void branch_eq2_ldbl (long double b) { if (ldbl == b) f (g ()); }
void branch_gt2_ldbl (long double b) { if (ldbl > b) f (g ()); }
void branch_lt2_ldbl (long double b) { if (ldbl < b) f (g ()); }
void branch_ne2_ldbl (long double b) { if (ldbl != b) f (g ()); }

void mem_movsfcc_eq2_ldbl (long double b) { ldbl = (ldbl == b) ? ldbl : 10; }
void mem_movsfcc_gt2_ldbl (long double b) { ldbl = (ldbl > b) ? ldbl : 10; }
void mem_movsfcc_lt2_ldbl (long double b) { ldbl = (ldbl < b) ? ldbl : 10; }
void mem_movsfcc_ne2_ldbl (long double b) { ldbl = (ldbl != b) ? ldbl : 10; }

void mem_movsicc_eq2_ldbl (long double b) { x = (ldbl == b) ? x : 10; }
void mem_movsicc_gt2_ldbl (long double b) { x = (ldbl > b) ? x : 10; }
void mem_movsicc_lt2_ldbl (long double b) { x = (ldbl < b) ? x : 10; }
void mem_movsicc_ne2_ldbl (long double b) { x = (ldbl != b) ? x : 10; }

long double movsfcc_eq2_ldbl (long double b) { return (ldbl == b) ? ldbl : 10; }
long double movsfcc_gt2_ldbl (long double b) { return (ldbl > b) ? ldbl : 10; }
long double movsfcc_lt2_ldbl (long double b) { return (ldbl < b) ? ldbl : 10; }
long double movsfcc_ne2_ldbl (long double b) { return (ldbl != b) ? ldbl : 10; }

int movsicc_eq2_ldbl (long double b) { return (ldbl == b) ? x : 10; }
int movsicc_gt2_ldbl (long double b) { return (ldbl > b) ? x : 10; }
int movsicc_lt2_ldbl (long double b) { return (ldbl < b) ? x : 10; }
int movsicc_ne2_ldbl (long double b) { return (ldbl != b) ? x : 10; }

int scc_eq2_ldbl (long double b) { return (ldbl == b); }
int scc_gt2_ldbl (long double b) { return (ldbl > b); }
int scc_lt2_ldbl (long double b) { return (ldbl < b); }
int scc_ne2_ldbl (long double b) { return (ldbl != b); }

void branch_eq2_dbl (double b) { if (dbl == b) f (g ()); }
void branch_gt2_dbl (double b) { if (dbl > b) f (g ()); }
void branch_lt2_dbl (double b) { if (dbl < b) f (g ()); }
void branch_ne2_dbl (double b) { if (dbl != b) f (g ()); }

void mem_movsfcc_eq2_dbl (double b) { dbl = (dbl == b) ? dbl : 10; }
void mem_movsfcc_gt2_dbl (double b) { dbl = (dbl > b) ? dbl : 10; }
void mem_movsfcc_lt2_dbl (double b) { dbl = (dbl < b) ? dbl : 10; }
void mem_movsfcc_ne2_dbl (double b) { dbl = (dbl != b) ? dbl : 10; }

void mem_movsicc_eq2_dbl (double b) { x = (dbl == b) ? x : 10; }
void mem_movsicc_gt2_dbl (double b) { x = (dbl > b) ? x : 10; }
void mem_movsicc_lt2_dbl (double b) { x = (dbl < b) ? x : 10; }
void mem_movsicc_ne2_dbl (double b) { x = (dbl != b) ? x : 10; }

double movsfcc_eq2_dbl (double b) { return (dbl == b) ? dbl : 10; }
double movsfcc_gt2_dbl (double b) { return (dbl > b) ? dbl : 10; }
double movsfcc_lt2_dbl (double b) { return (dbl < b) ? dbl : 10; }
double movsfcc_ne2_dbl (double b) { return (dbl != b) ? dbl : 10; }

int movsicc_eq2_dbl (double b) { return (dbl == b) ? x : 10; }
int movsicc_gt2_dbl (double b) { return (dbl > b) ? x : 10; }
int movsicc_lt2_dbl (double b) { return (dbl < b) ? x : 10; }
int movsicc_ne2_dbl (double b) { return (dbl != b) ? x : 10; }

int scc_eq2_dbl (double b) { return (dbl == b); }
int scc_gt2_dbl (double b) { return (dbl > b); }
int scc_lt2_dbl (double b) { return (dbl < b); }
int scc_ne2_dbl (double b) { return (dbl != b); }

void branch_eq2_flt (float b) { if (flt == b) f (g ()); }
void branch_gt2_flt (float b) { if (flt > b) f (g ()); }
void branch_lt2_flt (float b) { if (flt < b) f (g ()); }
void branch_ne2_flt (float b) { if (flt != b) f (g ()); }

void mem_movsfcc_eq2_flt (float b) { flt = (flt == b) ? flt : 10; }
void mem_movsfcc_gt2_flt (float b) { flt = (flt > b) ? flt : 10; }
void mem_movsfcc_lt2_flt (float b) { flt = (flt < b) ? flt : 10; }
void mem_movsfcc_ne2_flt (float b) { flt = (flt != b) ? flt : 10; }

void mem_movsicc_eq2_flt (float b) { x = (flt == b) ? x : 10; }
void mem_movsicc_gt2_flt (float b) { x = (flt > b) ? x : 10; }
void mem_movsicc_lt2_flt (float b) { x = (flt < b) ? x : 10; }
void mem_movsicc_ne2_flt (float b) { x = (flt != b) ? x : 10; }

float movsfcc_eq2_flt (float b) { return (flt == b) ? flt : 10; }
float movsfcc_gt2_flt (float b) { return (flt > b) ? flt : 10; }
float movsfcc_lt2_flt (float b) { return (flt < b) ? flt : 10; }
float movsfcc_ne2_flt (float b) { return (flt != b) ? flt : 10; }

int movsicc_eq2_flt (float b) { return (flt == b) ? x : 10; }
int movsicc_gt2_flt (float b) { return (flt > b) ? x : 10; }
int movsicc_lt2_flt (float b) { return (flt < b) ? x : 10; }
int movsicc_ne2_flt (float b) { return (flt != b) ? x : 10; }

int scc_eq2_flt (float b) { return (flt == b); }
int scc_gt2_flt (float b) { return (flt > b); }
int scc_lt2_flt (float b) { return (flt < b); }
int scc_ne2_flt (float b) { return (flt != b); }
