/* { dg-do run { target { ! avr_tiny } } } */
/* { dg-additional-options { -std=gnu99 -Os -mcall-prologues } } */

// !!! Requires the fx64 <-> float conversions from AVR-LibC.

#include <stdfix.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#define NI __attribute((noipa))

typedef long accum lk_t;
typedef long long accum llk_t;
typedef long long fract llr_t;

typedef unsigned long accum ulk_t;
typedef unsigned long long accum ullk_t;
typedef unsigned long long fract ullr_t;

// Values are in fmin <= x < fmax.
#define fmax_llr 1.0f
#define fmin_llr (-fmax_llr)
#define fmax_ullr fmax_llr
#define fmin_ullr 0.0f

#define fmax_lk 0x1.0p32f
#define fmin_lk (-fmax_lk)
#define fmax_ulk fmax_lk
#define fmin_ulk 0.0f

#define fmax_llk 0x1.0p16f
#define fmin_llk (-fmax_llk)
#define fmax_ullk fmax_llk
#define fmin_ullk 0.0f

#define UMAX 0xffffffffffffffff
#define SMAX 0x7fffffffffffffff
#define SMIN 0x8000000000000000

// Values are in min <= x <= max.
#define max_lk lkbits (SMAX)
#define min_lk lkbits (SMIN)
#define max_ulk ulkbits (UMAX)
#define min_ulk ulkbits (0)

#define max_llk llkbits (SMAX)
#define min_llk llkbits (SMIN)
#define max_ullk ullkbits (UMAX)
#define min_ullk ullkbits (0)

#define max_llr llrbits (SMAX)
#define min_llr llrbits (SMIN)
#define max_ullr ullrbits (UMAX)
#define min_ullr ullrbits (0)

#define id_lk   10
#define id_ulk  20
#define id_llk  30
#define id_ullk 40
#define id_llr  50
#define id_ullr 60

#define MK_TEST(fx)				    \
  NI bool in_range_##fx (float x)		    \
  {						    \
    return x < fmax_##fx && x >= fmin_##fx;	    \
  }						    \
						    \
  NI void test_mul_##fx (float a, float b)	    \
  {						    \
    if (!in_range_##fx (a))			    \
      return;					    \
    if (!in_range_##fx (b))			    \
      return;					    \
    float f = a * b;				    \
    __asm ("" : "+r" (f));			    \
    fx##_t ab = ((fx##_t) a) * (fx##_t) b;	    \
    if (f < fmin_##fx)				    \
      {						    \
	if (ab != min_##fx)			    \
	  exit (id_##fx + 1);			    \
	return;					    \
      }						    \
    if (f > fmax_##fx)				    \
      {						    \
	if (ab != max_##fx)			    \
	  exit (id_##fx + 2);			    \
	return;					    \
      }						    \
    if (f != (float) ab)			    \
      exit (id_##fx + 3);			    \
  }

MK_TEST (lk)
MK_TEST (ulk)
MK_TEST (llk)
MK_TEST (ullk)
MK_TEST (llr)
MK_TEST (ullr)

NI void test_mul (float a, float b)
{
  test_mul_lk (a, b);
  test_mul_ulk (a, b);

  test_mul_llk (a, b);
  test_mul_ullk (a, b);

  test_mul_llr (a, b);
  test_mul_ullr (a, b);
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

// Results / arguments must be representable as float, so no rounding occurs.
// No-overflow results must be representable as fixed, so no rounding occurs.
const PROGMEM float fvals[] =
  {
    0.0,
    +1.0, +2.0, +0.5, +0xff.0p0, +0xf.fp0, +0x0.ffp0, +0xcd.0p12, +0x0.0a1p0,
    -1.0, -2.0, -0.5, -0xff.0p0, -0xf.fp0, -0x0.ffp0, -0xcd.0p12, -0x0.0a1p0,
  };

NI void test (void)
{
  for (uint8_t a = 0; a < ARRAY_SIZE (fvals); ++a)
    for (uint8_t b = 0; b < ARRAY_SIZE (fvals); ++b)
      {
	float fa = pgm_read_float (&fvals[a]);
	float fb = pgm_read_float (&fvals[b]);
	test_mul (fa, fb);
      }	
}

int main (void)
{
  test ();
  return 0;
}
