#define INT_MIN (~__INT_MAX__)

extern int g ();
extern void f ();

extern char ch;

int x;

void branch_lt_ll (long long a) { if (a < 5) f (g ()); }
void branch_eq_ll (long long a) { if (a == 5) f (g ()); }
void branch_ne_ll (long long a) { if (a != 5) f (g ()); }
void branch_gt_ll (long long a) { if (a > 5) f (g ()); }
void branch_le_ll (long long a, int b) { if (a <= b) f (g ()); }
void branch_ge_ll (long long a, int b) { if (a >= b) f (g ()); }
void branch_ge0_ll (long long a) { if (a >= 0) f (g ()); }
void branch_lt0_ll (long long a) { if (a < 0) f (g ()); }
void branch_gt0_ll (long long a) { if (a > 0) f (g ()); }
void branch_le0_ll (long long a) { if (a <= 0) f (g ()); }
void branch_eq0_ll (long long a) { if (a > 0) f (g ()); }
void branch_ne0_ll (long long a) { if (a <= 0) f (g ()); }
void branch_bit0_ll (long long a) { if ((a & 2) == 0) f (g ()); }
void branch_bit1_ll (long long a) { if ((a & 2) == 2) f (g ()); }

void branch_lt (int a) { if (a < 5) f (g ()); }
void branch_eq (int a) { if (a == 5) f (g ()); }
void branch_ne (int a) { if (a != 5) f (g ()); }
void branch_gt (int a) { if (a > 5) f (g ()); }
void branch_le (int a, int b) { if (a <= b) f (g ()); }
void branch_ge (int a, int b) { if (a >= b) f (g ()); }
void branch_ge0 (int a) { if (a >= 0) f (g ()); }
void branch_lt0 (int a) { if (a < 0) f (g ()); }
void branch_gt0 (int a) { if (a > 0) f (g ()); }
void branch_le0 (int a) { if (a <= 0) f (g ()); }
void branch_eq0 (int a) { if (a > 0) f (g ()); }
void branch_ne0 (int a) { if (a <= 0) f (g ()); }
void branch_bit0 (int a) { if ((a & 2) == 0) f (g ()); }
void branch_bit1 (int a) { if ((a & 2) == 2) f (g ()); }

void branch_ltu (unsigned a) { if (a < 5) f (g ()); }
void branch_gtu (unsigned a) { if (a > 5) f (g ()); }
void branch_leu (unsigned a, unsigned b) { if (a <= b) f (g ()); }
void branch_geu (unsigned a, unsigned b) { if (a >= b) f (g ()); }
void branch_sbit0 (unsigned a) { if ((a & (unsigned)INT_MIN) == 0) f (g ()); }
void branch_sbit1 (unsigned a) { if ((a & (unsigned)INT_MIN) != 0) f (g ()); }

void branch_lt_ch (void) { if (ch < 5) f (g ()); }
void branch_eq_ch (void) { if (ch == 5) f (g ()); }
void branch_ne_ch (void) { if (ch != 5) f (g ()); }
void branch_gt_ch (void) { if (ch > 5) f (g ()); }
void branch_ge0_ch (void) { if (ch >= 0) f (g ()); }
void branch_lt0_ch (void) { if (ch < 0) f (g ()); }
void branch_gt0_ch (void) { if (ch > 0) f (g ()); }
void branch_le0_ch (void) { if (ch <= 0) f (g ()); }
void branch_eq0_ch (void) { if (ch > 0) f (g ()); }
void branch_ne0_ch (void) { if (ch <= 0) f (g ()); }
void branch_bit0_ch (void) { if ((ch & 2) == 0) f (g ()); }
void branch_bit1_ch (void) { if ((ch & 2) == 2) f (g ()); }

/* scc */

int scc_lt_ll (long long a) { return (a < 5); }
int scc_eq_ll (long long a) { return (a == 5); }
int scc_ne_ll (long long a) { return (a != 5); }
int scc_gt_ll (long long a) { return (a > 5); }
int scc_le_ll (long long a, int b) { return (a <= b); }
int scc_ge_ll (long long a, int b) { return (a >= b); }
int scc_ge0_ll (long long a) { return (a >= 0); }
int scc_lt0_ll (long long a) { return (a < 0); }
int scc_gt0_ll (long long a) { return (a > 0); }
int scc_le0_ll (long long a) { return (a <= 0); }
int scc_eq0_ll (long long a) { return (a > 0); }
int scc_ne0_ll (long long a) { return (a <= 0); }
int scc_bit0_ll (long long a) { return ((a & 2) == 0); }
int scc_bit1_ll (long long a) { return ((a & 2) == 2); }

int scc_lt (int a) { return (a < 5); }
int scc_eq (int a) { return (a == 5); }
int scc_ne (int a) { return (a != 5); }
int scc_gt (int a) { return (a > 5); }
int scc_le (int a, int b) { return (a <= b); }
int scc_ge (int a, int b) { return (a >= b); }
int scc_ge0 (int a) { return (a >= 0); }
int scc_lt0 (int a) { return (a < 0); }
int scc_gt0 (int a) { return (a > 0); }
int scc_le0 (int a) { return (a <= 0); }
int scc_eq0 (int a) { return (a > 0); }
int scc_ne0 (int a) { return (a <= 0); }
int scc_bit0 (int a) { return ((a & 2) == 0); }
int scc_bit1 (int a) { return ((a & 2) == 2); }

int scc_ltu (unsigned a) { return (a < 5); }
int scc_gtu (unsigned a) { return (a > 5); }
int scc_leu (unsigned a, unsigned b) { return (a <= b); }
int scc_geu (unsigned a, unsigned b) { return (a >= b); }
int scc_sbit0 (unsigned a) { return ((a & (unsigned)INT_MIN) == 0); }
int scc_sbit1 (unsigned a) { return ((a & (unsigned)INT_MIN) != 0); }

int scc_lt_ch (void) { return (ch < 5); }
int scc_eq_ch (void) { return (ch == 5); }
int scc_ne_ch (void) { return (ch != 5); }
int scc_gt_ch (void) { return (ch > 5); }
int scc_ge0_ch (void) { return (ch >= 0); }
int scc_lt0_ch (void) { return (ch < 0); }
int scc_gt0_ch (void) { return (ch > 0); }
int scc_le0_ch (void) { return (ch <= 0); }
int scc_eq0_ch (void) { return (ch > 0); }
int scc_ne0_ch (void) { return (ch <= 0); }
int scc_bit0_ch (void) { return ((ch & 2) == 0); }
int scc_bit1_ch (void) { return ((ch & 2) == 2); }

/* movsicc */

int movsicc_lt_ll (long long a) { return (a < 5) ? x : 10; }
int movsicc_eq_ll (long long a) { return (a == 5) ? x : 10; }
int movsicc_ne_ll (long long a) { return (a != 5) ? x : 10; }
int movsicc_gt_ll (long long a) { return (a > 5) ? x : 10; }
int movsicc_le_ll (long long a, int b) { return (a <= b) ? x : 10; }
int movsicc_ge_ll (long long a, int b) { return (a >= b) ? x : 10; }
int movsicc_ge0_ll (long long a) { return (a >= 0) ? x : 10; }
int movsicc_lt0_ll (long long a) { return (a < 0) ? x : 10; }
int movsicc_gt0_ll (long long a) { return (a > 0) ? x : 10; }
int movsicc_le0_ll (long long a) { return (a <= 0) ? x : 10; }
int movsicc_eq0_ll (long long a) { return (a > 0) ? x : 10; }
int movsicc_ne0_ll (long long a) { return (a <= 0) ? x : 10; }
int movsicc_bit0_ll (long long a) { return ((a & 2) == 0) ? x : 10; }
int movsicc_bit1_ll (long long a) { return ((a & 2) == 2) ? x : 10; }

int movsicc_lt (int a) { return (a < 5) ? x : 10; }
int movsicc_eq (int a) { return (a == 5) ? x : 10; }
int movsicc_ne (int a) { return (a != 5) ? x : 10; }
int movsicc_gt (int a) { return (a > 5) ? x : 10; }
int movsicc_le (int a, int b) { return (a <= b) ? x : 10; }
int movsicc_ge (int a, int b) { return (a >= b) ? x : 10; }
int movsicc_ge0 (int a) { return (a >= 0) ? x : 10; }
int movsicc_lt0 (int a) { return (a < 0) ? x : 10; }
int movsicc_gt0 (int a) { return (a > 0) ? x : 10; }
int movsicc_le0 (int a) { return (a <= 0) ? x : 10; }
int movsicc_eq0 (int a) { return (a > 0) ? x : 10; }
int movsicc_ne0 (int a) { return (a <= 0) ? x : 10; }
int movsicc_bit0 (int a) { return ((a & 2) == 0) ? x : 10; }
int movsicc_bit1 (int a) { return ((a & 2) == 2) ? x : 10; }

int movsicc_ltu (unsigned a) { return (a < 5) ? x : 10; }
int movsicc_gtu (unsigned a) { return (a > 5) ? x : 10; }
int movsicc_leu (unsigned a, unsigned b) { return (a <= b) ? x : 10; }
int movsicc_geu (unsigned a, unsigned b) { return (a >= b) ? x : 10; }
int movsicc_sbit0 (unsigned a) { return ((a & (unsigned)INT_MIN) == 0) ? x : 10; }
int movsicc_sbit1 (unsigned a) { return ((a & (unsigned)INT_MIN) != 0) ? x : 10; }

int movsicc_lt_ch (void) { return (ch < 5) ? x : 10; }
int movsicc_eq_ch (void) { return (ch == 5) ? x : 10; }
int movsicc_ne_ch (void) { return (ch != 5) ? x : 10; }
int movsicc_gt_ch (void) { return (ch > 5) ? x : 10; }
int movsicc_ge0_ch (void) { return (ch >= 0) ? x : 10; }
int movsicc_lt0_ch (void) { return (ch < 0) ? x : 10; }
int movsicc_gt0_ch (void) { return (ch > 0) ? x : 10; }
int movsicc_le0_ch (void) { return (ch <= 0) ? x : 10; }
int movsicc_eq0_ch (void) { return (ch > 0) ? x : 10; }
int movsicc_ne0_ch (void) { return (ch <= 0) ? x : 10; }
int movsicc_bit0_ch (void) { return ((ch & 2) == 0) ? x : 10; }
int movsicc_bit1_ch (void) { return ((ch & 2) == 2) ? x : 10; }

/* movsfcc */

/* mem_movsicc */

void mem_movsicc_lt (int a) { x = (a < 5) ? x : 10; }
void mem_movsicc_eq (int a) { x = (a == 5) ? x : 10; }
void mem_movsicc_ne (int a) { x = (a != 5) ? x : 10; }
void mem_movsicc_gt (int a) { x = (a > 5) ? x : 10; }
void mem_movsicc_le (int a, int b) { x = (a <= b) ? x : 10; }
void mem_movsicc_ge (int a, int b) { x = (a >= b) ? x : 10; }
void mem_movsicc_ge0 (int a) { x = (a >= 0) ? x : 10; }
void mem_movsicc_lt0 (int a) { x = (a < 0) ? x : 10; }
void mem_movsicc_gt0 (int a) { x = (a > 0) ? x : 10; }
void mem_movsicc_le0 (int a) { x = (a <= 0) ? x : 10; }
void mem_movsicc_eq0 (int a) { x = (a > 0) ? x : 10; }
void mem_movsicc_ne0 (int a) { x = (a <= 0) ? x : 10; }
void mem_movsicc_bit0 (int a) { x = ((a & 2) == 0) ? x : 10; }
void mem_movsicc_bit1 (int a) { x = ((a & 2) == 2) ? x : 10; }

void mem_movsicc_ltu (unsigned a) { x = (a < 5) ? x : 10; }
void mem_movsicc_gtu (unsigned a) { x = (a > 5) ? x : 10; }
void mem_movsicc_leu (unsigned a, unsigned b) { x = (a <= b) ? x : 10; }
void mem_movsicc_geu (unsigned a, unsigned b) { x = (a >= b) ? x : 10; }
void mem_movsicc_sbit0 (unsigned a) { x = ((a & (unsigned)INT_MIN) == 0) ? x : 10; }
void mem_movsicc_sbit1 (unsigned a) { x = ((a & (unsigned)INT_MIN) != 0) ? x : 10; }

void mem_movsicc_lt_ch (void) { x = (ch < 5) ? x : 10; }
void mem_movsicc_eq_ch (void) { x = (ch == 5) ? x : 10; }
void mem_movsicc_ne_ch (void) { x = (ch != 5) ? x : 10; }
void mem_movsicc_gt_ch (void) { x = (ch > 5) ? x : 10; }
void mem_movsicc_ge0_ch (void) { x = (ch >= 0) ? x : 10; }
void mem_movsicc_lt0_ch (void) { x = (ch < 0) ? x : 10; }
void mem_movsicc_gt0_ch (void) { x = (ch > 0) ? x : 10; }
void mem_movsicc_le0_ch (void) { x = (ch <= 0) ? x : 10; }
void mem_movsicc_eq0_ch (void) { x = (ch > 0) ? x : 10; }
void mem_movsicc_ne0_ch (void) { x = (ch <= 0) ? x : 10; }
void mem_movsicc_bit0_ch (void) { x = ((ch & 2) == 0) ? x : 10; }
void mem_movsicc_bit1_ch (void) { x = ((ch & 2) == 2) ? x : 10; }

/* mem_movsfcc */

void branch_ne2 (int a, int b) { if (a != b) f (g ()); }
int scc_ne2 (int a, int b) { return (a != b); }
int movsicc_ne2 (int a, int b) { return (a != b) ? x : 10; }
void mem_movsicc_ne2 (int a, int b) { x = (a != b) ? x : 10; }

void branch_gt2 (int a, int b) { if (a > b) f (g ()); }
int scc_gt2 (int a, int b) { return (a > b); }
int movsicc_gt2 (int a, int b) { return (a > b) ? x : 10; }
void mem_movsicc_gt2 (int a, int b) { x = (a > b) ? x : 10; }

void branch_eq2 (int a, int b) { if (a == b) f (g ()); }
int scc_eq2 (int a, int b) { return (a == b); }
int movsicc_eq2 (int a, int b) { return (a == b) ? x : 10; }
void mem_movsicc_eq2 (int a, int b) { x = (a == b) ? x : 10; }

void branch_lt2 (int a, int b) { if (a < b) f (g ()); }
int scc_lt2 (int a, int b) { return (a < b); }
int movsicc_lt2 (int a, int b) { return (a < b) ? x : 10; }
void mem_movsicc_lt2 (int a, int b) { x = (a < b) ? x : 10; }

void branch_gtu2 (unsigned a, unsigned b) { if (a > b) f (g ()); }
int scc_gtu2 (unsigned a, unsigned b) { return (a > b); }
int movsicc_gtu2 (unsigned a, unsigned b) { return (a > b) ? x : 10; }
void mem_movsicc_gtu2 (unsigned a, unsigned b) { x = (a > b) ? x : 10; }

void branch_ltu2 (unsigned a, unsigned b) { if (a < b) f (g ()); }
int scc_ltu2 (unsigned a, unsigned b) { return (a < b); }
int movsicc_ltu2 (unsigned a, unsigned b) { return (a < b) ? x : 10; }
void mem_movsicc_ltu2 (unsigned a, unsigned b) { x = (a < b) ? x : 10; }

float movsfcc_lt_ll (long long a) { return (a < 5) ? flt : 10; }
float movsfcc_eq_ll (long long a) { return (a == 5) ? flt : 10; }
float movsfcc_ne_ll (long long a) { return (a != 5) ? flt : 10; }
float movsfcc_gt_ll (long long a) { return (a > 5) ? flt : 10; }
float movsfcc_le_ll (long long a, int b) { return (a <= b) ? flt : 10; }
float movsfcc_ge_ll (long long a, int b) { return (a >= b) ? flt : 10; }
float movsfcc_ge0_ll (long long a) { return (a >= 0) ? flt : 10; }
float movsfcc_lt0_ll (long long a) { return (a < 0) ? flt : 10; }
float movsfcc_gt0_ll (long long a) { return (a > 0) ? flt : 10; }
float movsfcc_le0_ll (long long a) { return (a <= 0) ? flt : 10; }
float movsfcc_eq0_ll (long long a) { return (a > 0) ? flt : 10; }
float movsfcc_ne0_ll (long long a) { return (a <= 0) ? flt : 10; }
float movsfcc_bit0_ll (long long a) { return ((a & 2) == 0) ? flt : 10; }
float movsfcc_bit1_ll (long long a) { return ((a & 2) == 2) ? flt : 10; }

float movsfcc_lt (int a) { return (a < 5) ? flt : 10; }
float movsfcc_eq (int a) { return (a == 5) ? flt : 10; }
float movsfcc_ne (int a) { return (a != 5) ? flt : 10; }
float movsfcc_gt (int a) { return (a > 5) ? flt : 10; }
float movsfcc_le (int a, int b) { return (a <= b) ? flt : 10; }
float movsfcc_ge (int a, int b) { return (a >= b) ? flt : 10; }
float movsfcc_ge0 (int a) { return (a >= 0) ? flt : 10; }
float movsfcc_lt0 (int a) { return (a < 0) ? flt : 10; }
float movsfcc_gt0 (int a) { return (a > 0) ? flt : 10; }
float movsfcc_le0 (int a) { return (a <= 0) ? flt : 10; }
float movsfcc_eq0 (int a) { return (a > 0) ? flt : 10; }
float movsfcc_ne0 (int a) { return (a <= 0) ? flt : 10; }
float movsfcc_bit0 (int a) { return ((a & 2) == 0) ? flt : 10; }
float movsfcc_bit1 (int a) { return ((a & 2) == 2) ? flt : 10; }

float movsfcc_ltu (unsigned a) { return (a < 5) ? flt : 10; }
float movsfcc_gtu (unsigned a) { return (a > 5) ? flt : 10; }
float movsfcc_leu (unsigned a, unsigned b) { return (a <= b) ? flt : 10; }
float movsfcc_geu (unsigned a, unsigned b) { return (a >= b) ? flt : 10; }
float movsfcc_sbit0 (unsigned a) { return ((a & (unsigned)INT_MIN) == 0) ? flt : 10; }
float movsfcc_sbit1 (unsigned a) { return ((a & (unsigned)INT_MIN) != 0) ? flt : 10; }

float movsfcc_lt_ch (void) { return (ch < 5) ? flt : 10; }
float movsfcc_eq_ch (void) { return (ch == 5) ? flt : 10; }
float movsfcc_ne_ch (void) { return (ch != 5) ? flt : 10; }
float movsfcc_gt_ch (void) { return (ch > 5) ? flt : 10; }
float movsfcc_ge0_ch (void) { return (ch >= 0) ? flt : 10; }
float movsfcc_lt0_ch (void) { return (ch < 0) ? flt : 10; }
float movsfcc_gt0_ch (void) { return (ch > 0) ? flt : 10; }
float movsfcc_le0_ch (void) { return (ch <= 0) ? flt : 10; }
float movsfcc_eq0_ch (void) { return (ch > 0) ? flt : 10; }
float movsfcc_ne0_ch (void) { return (ch <= 0) ? flt : 10; }
float movsfcc_bit0_ch (void) { return ((ch & 2) == 0) ? flt : 10; }
float movsfcc_bit1_ch (void) { return ((ch & 2) == 2) ? flt : 10; }

void mem_movsfcc_lt (int a) { flt = (a < 5) ? flt : 10; }
void mem_movsfcc_eq (int a) { flt = (a == 5) ? flt : 10; }
void mem_movsfcc_ne (int a) { flt = (a != 5) ? flt : 10; }
void mem_movsfcc_gt (int a) { flt = (a > 5) ? flt : 10; }
void mem_movsfcc_le (int a, int b) { flt = (a <= b) ? flt : 10; }
void mem_movsfcc_ge (int a, int b) { flt = (a >= b) ? flt : 10; }
void mem_movsfcc_ge0 (int a) { flt = (a >= 0) ? flt : 10; }
void mem_movsfcc_lt0 (int a) { flt = (a < 0) ? flt : 10; }
void mem_movsfcc_gt0 (int a) { flt = (a > 0) ? flt : 10; }
void mem_movsfcc_le0 (int a) { flt = (a <= 0) ? flt : 10; }
void mem_movsfcc_eq0 (int a) { flt = (a > 0) ? flt : 10; }
void mem_movsfcc_ne0 (int a) { flt = (a <= 0) ? flt : 10; }
void mem_movsfcc_bit0 (int a) { flt = ((a & 2) == 0) ? flt : 10; }
void mem_movsfcc_bit1 (int a) { flt = ((a & 2) == 2) ? flt : 10; }

void mem_movsfcc_ltu (unsigned a) { flt = (a < 5) ? flt : 10; }
void mem_movsfcc_gtu (unsigned a) { flt = (a > 5) ? flt : 10; }
void mem_movsfcc_leu (unsigned a, unsigned b) { flt = (a <= b) ? flt : 10; }
void mem_movsfcc_geu (unsigned a, unsigned b) { flt = (a >= b) ? flt : 10; }
void mem_movsfcc_sbit0 (unsigned a) { flt = ((a & (unsigned)INT_MIN) == 0) ? flt : 10; }
void mem_movsfcc_sbit1 (unsigned a) { flt = ((a & (unsigned)INT_MIN) != 0) ? flt : 10; }

void mem_movsfcc_lt_ch (void) { flt = (ch < 5) ? flt : 10; }
void mem_movsfcc_eq_ch (void) { flt = (ch == 5) ? flt : 10; }
void mem_movsfcc_ne_ch (void) { flt = (ch != 5) ? flt : 10; }
void mem_movsfcc_gt_ch (void) { flt = (ch > 5) ? flt : 10; }
void mem_movsfcc_ge0_ch (void) { flt = (ch >= 0) ? flt : 10; }
void mem_movsfcc_lt0_ch (void) { flt = (ch < 0) ? flt : 10; }
void mem_movsfcc_gt0_ch (void) { flt = (ch > 0) ? flt : 10; }
void mem_movsfcc_le0_ch (void) { flt = (ch <= 0) ? flt : 10; }
void mem_movsfcc_eq0_ch (void) { flt = (ch > 0) ? flt : 10; }
void mem_movsfcc_ne0_ch (void) { flt = (ch <= 0) ? flt : 10; }
void mem_movsfcc_bit0_ch (void) { flt = ((ch & 2) == 0) ? flt : 10; }
void mem_movsfcc_bit1_ch (void) { flt = ((ch & 2) == 2) ? flt : 10; }

float movsfcc_ne2 (int a, int b) { return (a != b) ? flt : 10; }
float movsfcc_gt2 (int a, int b) { return (a > b) ? flt : 10; }
float movsfcc_eq2 (int a, int b) { return (a == b) ? flt : 10; }
float movsfcc_lt2 (int a, int b) { return (a < b) ? flt : 10; }
float movsfcc_gtu2 (unsigned a, unsigned b) { return (a > b) ? flt : 10; }
float movsfcc_ltu2 (unsigned a, unsigned b) { return (a < b) ? flt : 10; }

void mem_movsfcc_ne2 (int a, int b) { flt = (a != b) ? flt : 10; }
void mem_movsfcc_gt2 (int a, int b) { flt = (a > b) ? flt : 10; }
void mem_movsfcc_eq2 (int a, int b) { flt = (a == b) ? flt : 10; }
void mem_movsfcc_lt2 (int a, int b) { flt = (a < b) ? flt : 10; }
void mem_movsfcc_gtu2 (unsigned a, unsigned b) { flt = (a > b) ? flt : 10; }
void mem_movsfcc_ltu2 (unsigned a, unsigned b) { flt = (a < b) ? flt : 10; }
