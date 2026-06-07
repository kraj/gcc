/* { dg-do run } */
/* { dg-require-effective-target offload_device_nonshared_as } */

/* Like target-map-iterators-1.c, but with non-constant iterator
   start/end/step in target data.   */

#include <stdlib.h>

#define DIM1 300
#define DIM2 15

int mkarray (int *x[], int a, int b, int c)
{
  int expected = 0;

  for (int i = a; i < b; i++)
    {
      if ((i + 1) % c == 0)
	{
	  x[i] = (int *) malloc (DIM2 * sizeof (int));
	  for (int j = 0; j < DIM2; j++)
	    {
	      x[i][j] = rand ();
	      expected += x[i][j];
	    }
	}
      else
	x[i] = NULL;
    }

  return expected;
}

int doit (int a, int b, int c)
{
  int *x[DIM1];
  int y;

  int expected = mkarray (x, a, b, c);

  #pragma omp target data map(iterator(i=a:b:c), tofrom: x[i][:DIM2]) \
			  map(to: x)
  #pragma omp target map(from: y)
    {
      y = 0;
      for (int i = a; i < b; i++)
	if ((i + 1) % c == 0)
	  for (int j = 0; j < DIM2; j++)
	    {
	      y += x[i][j];
	      x[i][j] = 0;
	    }
    }

  for (int i = a; i < b; i++)
    if ((i + 1) % c == 0)
      for (int j = 0; j < DIM2; j++)
	if (x[i][j] != 0)
	  return 1;
  return y - expected;
}

int main (void)
{
  return doit (2, 300, 3);
}
