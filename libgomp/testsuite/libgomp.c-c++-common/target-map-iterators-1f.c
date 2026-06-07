/* { dg-do run } */
/* { dg-require-effective-target offload_device_nonshared_as } */

/* Like target-map-iterators-1.c, but with non-constant iterator
   start/end/step in target update.   */

#include <stdlib.h>

#define DIM1 300
#define DIM2 15

void mkarray (int *x[], int a, int b, int c)
{
  for (int i = a; i < b; i++)
    {
      if ((i + 1) % c == 0)
	{
	  x[i] = (int *) malloc (DIM2 * sizeof (int));
	  for (int j = 0; j < DIM2; j++)
	    x[i][j] = 0;
	}
      else
	x[i] = NULL;
    }
}

int doit (int a, int b, int c)
{
  int *x[DIM1];
  int y;

  /* Initialize the arrays to zero.  */
  mkarray (x, a, b, c);

  #pragma omp target enter data map(iterator(i=a:b:c), to: x[i][:DIM2]) \
				map(to: x)

  /* Fill in some "real" values for the array on the host side.  */
  int expected = 0;
  for (int i = a; i < b; i++)
    {
      if ((i + 1) % c == 0)
	for (int j = 0; j < DIM2; j++)
	  {
	    x[i][j] = rand ();
	    expected += x[i][j];
	  }
    }

  /* Push the "real" values to the target.  */
  #pragma omp target update to(iterator(i=a:b:c): x[i][:DIM2])

  #pragma omp target map(from: y)
    {
      y = 0;
      for (int i = a; i < b; i++)
	if ((i + 1) % c == 0)
	  for (int j = 0; j < DIM2; j++)
	    y += x[i][j];
    }

  #pragma omp target exit data map(iterator(i=a:b:c), from: x[i][:DIM2])
  #pragma omp target exit data map(release: x)

  return y - expected;
}

int main (void)
{
  return doit (2, 300, 3);
}
