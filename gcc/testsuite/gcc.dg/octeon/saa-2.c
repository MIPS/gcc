/* Test 58XX new instruction "saad".  */

/* { dg-do run { target *-octeon-elf* } } */
/* { dg-options "-O2" } */

/* Read the COP0 specified by REGSTR and return the value in RESULT.  */
#define read_cop0_register32(result, regstr) \
	asm volatile("mfc0 %[rt], $" #regstr : [rt] "=d" (result))

/* Copied from executive/octeon-model.h.  */
#define OCTEON_CN58XX_PASS1     0x000d0300
#define OCTEON_CN58XX           OCTEON_CN58XX_PASS1

void abort (void);

main()
{
  unsigned int proc_id;

  /* Read processor identification cop0 register.  */
  read_cop0_register32 (proc_id, 15);

  if (proc_id == OCTEON_CN58XX)
    {
      long long a = 10;
      
      asm ("saad %[treg], %[breg]\n" 
		     : [breg] "+m" (a) 
		     : [treg] "r" (20)); 

      if (a != 30)
	abort (); 

      a = -10;
      asm ("saad %[treg], %[breg]\n" 
		     : [breg] "+m" (a) 
		     : [treg] "r" (20)); 

      if (a != 10)
	abort (); 
    }

  return 0;
}
