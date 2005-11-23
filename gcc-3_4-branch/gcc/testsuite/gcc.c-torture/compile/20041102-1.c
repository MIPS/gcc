/* PR c/18282 */
typedef enum { B1 = 1, B2 = 2, B4 = 4, B8 = 8, B16 = 16 } B;

B __attribute__ ((mode (QI))) bqi;
B __attribute__ ((mode (word))) bword;
