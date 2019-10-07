/* { dg-do run } */
/* { dg-options "-mabi=32 (REQUIRES_STDLIB)" } */

/* Test modulo by power of two expansion.  */
#include <stdlib.h>

#define ARGPASTE(X, Y) X ## Y
#define ARGPASTE2(X) ARGPASTE (mod_, X)

/* Create 64 distinct functions which calculate the modulo by power of two  */
#define MOD(LOG) \
  __attribute__((noinline)) long long ARGPASTE2 (__COUNTER__) (long long x) \
  { return x % (1LL << (LOG)); }
#define MOD_1(x) \
  MOD (x) \
  MOD (x + 1)
#define MOD_2(x) \
  MOD_1 (x) \
  MOD_1 (x + 2)
#define MOD_4(x) \
  MOD_2 (x) \
  MOD_2 (x + 4)
#define MOD_8(x) \
  MOD_4 (x) \
  MOD_4 (x + 8)
#define MOD_16(x) \
  MOD_8 (x) \
  MOD_8 (x + 16)
#define MOD_32(x) \
  MOD_16 (x) \
  MOD_16 (x + 32)

MOD_32 (0)

#define TEST_NUM_POS 0x7fffffffffffffff
#define TEST_NUM_NEG 0xffffffffffffffff

/* Test the modulo by power of two with a positive number.  */
#define TEST_MOD_POS(LOG) \
  ARGPASTE2 (LOG) (TEST_NUM_POS) == (TEST_NUM_POS & ((1LL << (LOG)) - 1)) \
  ? (void) 0 : abort ()

/* Test the modulo by power of two with a negative number.  */
#define TEST_MOD_NEG(LOG) \
  ARGPASTE2 (LOG) (TEST_NUM_NEG) == -1LL ? (void) 0 : abort()

int main() {
    TEST_MOD_POS(1);
    TEST_MOD_POS(2);
    TEST_MOD_POS(3);
    TEST_MOD_POS(4);
    TEST_MOD_POS(5);
    TEST_MOD_POS(6);
    TEST_MOD_POS(7);
    TEST_MOD_POS(8);
    TEST_MOD_POS(9);
    TEST_MOD_POS(10);
    TEST_MOD_POS(11);
    TEST_MOD_POS(12);
    TEST_MOD_POS(13);
    TEST_MOD_POS(14);
    TEST_MOD_POS(15);
    TEST_MOD_POS(16);
    TEST_MOD_POS(17);
    TEST_MOD_POS(18);
    TEST_MOD_POS(19);
    TEST_MOD_POS(20);
    TEST_MOD_POS(21);
    TEST_MOD_POS(22);
    TEST_MOD_POS(23);
    TEST_MOD_POS(24);
    TEST_MOD_POS(25);
    TEST_MOD_POS(26);
    TEST_MOD_POS(27);
    TEST_MOD_POS(28);
    TEST_MOD_POS(29);
    TEST_MOD_POS(30);
    TEST_MOD_POS(31);
    TEST_MOD_POS(32);
    TEST_MOD_POS(33);
    TEST_MOD_POS(34);
    TEST_MOD_POS(35);
    TEST_MOD_POS(36);
    TEST_MOD_POS(37);
    TEST_MOD_POS(38);
    TEST_MOD_POS(39);
    TEST_MOD_POS(40);
    TEST_MOD_POS(41);
    TEST_MOD_POS(42);
    TEST_MOD_POS(43);
    TEST_MOD_POS(44);
    TEST_MOD_POS(45);
    TEST_MOD_POS(46);
    TEST_MOD_POS(47);
    TEST_MOD_POS(48);
    TEST_MOD_POS(49);
    TEST_MOD_POS(50);
    TEST_MOD_POS(51);
    TEST_MOD_POS(52);
    TEST_MOD_POS(53);
    TEST_MOD_POS(54);
    TEST_MOD_POS(55);
    TEST_MOD_POS(56);
    TEST_MOD_POS(57);
    TEST_MOD_POS(58);
    TEST_MOD_POS(59);
    TEST_MOD_POS(60);
    TEST_MOD_POS(61);
    TEST_MOD_POS(62);

    TEST_MOD_NEG(1);
    TEST_MOD_NEG(2);
    TEST_MOD_NEG(3);
    TEST_MOD_NEG(4);
    TEST_MOD_NEG(5);
    TEST_MOD_NEG(6);
    TEST_MOD_NEG(7);
    TEST_MOD_NEG(8);
    TEST_MOD_NEG(9);
    TEST_MOD_NEG(10);
    TEST_MOD_NEG(11);
    TEST_MOD_NEG(12);
    TEST_MOD_NEG(13);
    TEST_MOD_NEG(14);
    TEST_MOD_NEG(15);
    TEST_MOD_NEG(16);
    TEST_MOD_NEG(17);
    TEST_MOD_NEG(18);
    TEST_MOD_NEG(19);
    TEST_MOD_NEG(20);
    TEST_MOD_NEG(21);
    TEST_MOD_NEG(22);
    TEST_MOD_NEG(23);
    TEST_MOD_NEG(24);
    TEST_MOD_NEG(25);
    TEST_MOD_NEG(26);
    TEST_MOD_NEG(27);
    TEST_MOD_NEG(28);
    TEST_MOD_NEG(29);
    TEST_MOD_NEG(30);
    TEST_MOD_NEG(31);
    TEST_MOD_NEG(32);
    TEST_MOD_NEG(33);
    TEST_MOD_NEG(34);
    TEST_MOD_NEG(35);
    TEST_MOD_NEG(36);
    TEST_MOD_NEG(37);
    TEST_MOD_NEG(38);
    TEST_MOD_NEG(39);
    TEST_MOD_NEG(40);
    TEST_MOD_NEG(41);
    TEST_MOD_NEG(42);
    TEST_MOD_NEG(43);
    TEST_MOD_NEG(44);
    TEST_MOD_NEG(45);
    TEST_MOD_NEG(46);
    TEST_MOD_NEG(47);
    TEST_MOD_NEG(48);
    TEST_MOD_NEG(49);
    TEST_MOD_NEG(50);
    TEST_MOD_NEG(51);
    TEST_MOD_NEG(52);
    TEST_MOD_NEG(53);
    TEST_MOD_NEG(54);
    TEST_MOD_NEG(55);
    TEST_MOD_NEG(56);
    TEST_MOD_NEG(57);
    TEST_MOD_NEG(58);
    TEST_MOD_NEG(59);
    TEST_MOD_NEG(60);
    TEST_MOD_NEG(61);
    TEST_MOD_NEG(62);
    TEST_MOD_NEG(63);

    exit (0);
}
