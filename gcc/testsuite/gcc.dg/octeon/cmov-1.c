/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O3" } */
/* { dg-final { scan-assembler-times "movz\t|movn\t" 8 } } */
/* { dg-final { scan-assembler-not "bbit0\t|bbit1\t" } } */

typedef unsigned short ee_u16;
typedef unsigned char ee_u8;
ee_u16 crcu8(ee_u8 data, ee_u16 crc )
{
  int i;
  ee_u8 x16=0,carry=0;

  for (i = 0; i < 8; i++)
    {
        x16 = (ee_u8)((data & 1) ^ ((ee_u8)crc & 1));
        data >>= 1;

        if (x16 == 1)
        {
           crc ^= 0x4002;
           crc >>= 1;
           crc |= 0x8000;
        }
        else 
        {
          crc >>= 1;
          crc &= 0x7fff;
        }

    }
  return crc;
} 

