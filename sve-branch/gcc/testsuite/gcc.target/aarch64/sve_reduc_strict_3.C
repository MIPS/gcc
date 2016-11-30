/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -msve-vector-bits=256" } */

double mat[100][4];
double mat2[100][8];
double mat3[100][12];
double mat4[100][3];

double slp_reduc_plus (int n)
{
  double tmp = 0.0;
  for (int i = 0; i < n; i++)
    {
      tmp = tmp + mat[i][0];
      tmp = tmp + mat[i][1];
      tmp = tmp + mat[i][2];
      tmp = tmp + mat[i][3];
    }
  return tmp;
}

double slp_reduc_plus2 (int n)
{
  double tmp = 0.0;
  for (int i = 0; i < n; i++)
    {
      tmp = tmp + mat2[i][0];
      tmp = tmp + mat2[i][1];
      tmp = tmp + mat2[i][2];
      tmp = tmp + mat2[i][3];
      tmp = tmp + mat2[i][4];
      tmp = tmp + mat2[i][5];
      tmp = tmp + mat2[i][6];
      tmp = tmp + mat2[i][7];
    }
  return tmp;
}

double slp_reduc_plus3 (int n)
{
  double tmp = 0.0;
  for (int i = 0; i < n; i++)
    {
      tmp = tmp + mat3[i][0];
      tmp = tmp + mat3[i][1];
      tmp = tmp + mat3[i][2];
      tmp = tmp + mat3[i][3];
      tmp = tmp + mat3[i][4];
      tmp = tmp + mat3[i][5];
      tmp = tmp + mat3[i][6];
      tmp = tmp + mat3[i][7];
      tmp = tmp + mat3[i][8];
      tmp = tmp + mat3[i][9];
      tmp = tmp + mat3[i][10];
      tmp = tmp + mat3[i][11];
    }
  return tmp;
}

void slp_non_chained_reduc (int n, double * __restrict__ out)
{
  for (int i = 0; i < 3; i++)
    out[i] = 0;

  for (int i = 0; i < n; i++)
    {
      out[0] = out[0] + mat4[i][0];
      out[1] = out[1] + mat4[i][1];
      out[2] = out[2] + mat4[i][2];
    }
}

/* === Strict FP Reductions shouldn't be used here === */

float double_reduc1 (float *__restrict__ i)
{
  float l = 0;

  for (int a = 0; a < 8; a++)
    for (int b = 0; b < 4; b++)
      l += i[a];
  return l;
}

float double_reduc2 (float *__restrict__ i)
{
  float l = 0;

  for (int a = 0; a < 8; a++)
    for (int b = 2; b >= 0; b--)
      {
        l += i[b * 2];
        l += i[b * 2 + 1];
        l += i[b * 2 + 2];
        l += i[b * 2 + 3];
      }
  return l;
}

float double_reduc3 (float *__restrict__ i, float *__restrict__ j)
{
  float k = 0, l = 0;

  for (int a = 0; a < 8; a++)
    for (int b = 0; b < 4; b++)
      {
        k += i[b];
        l += j[b];
      }
  return l * k;
}

/* { dg-final { scan-assembler-not {\tfadda\ts[0-9]+} } } */
/* { dg-final { scan-assembler-times {\tfadda\td[0-9]+, p[0-7], d[0-9]+, z[0-9]+\.d} 9 } } */

