/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" { target { ! ilp32 } } } } */

#include "test_sve_acle.h"

/*
** ld3_mf8_base:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_mf8_base, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0),
	   z0 = svld3 (p0, x0))

/*
** ld3_mf8_index:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, x1\]
**	ret
*/
TEST_LOAD (ld3_mf8_index, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 + x1),
	   z0 = svld3 (p0, x0 + x1))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_mf8_1:
**	incb	x0
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_mf8_1, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 + svcntb ()),
	   z0 = svld3 (p0, x0 + svcntb ()))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_mf8_2:
**	incb	x0, all, mul #2
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_mf8_2, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 + svcntb () * 2),
	   z0 = svld3 (p0, x0 + svcntb () * 2))

/*
** ld3_mf8_3:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #3, mul vl\]
**	ret
*/
TEST_LOAD (ld3_mf8_3, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 + svcntb () * 3),
	   z0 = svld3 (p0, x0 + svcntb () * 3))

/*
** ld3_mf8_21:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #21, mul vl\]
**	ret
*/
TEST_LOAD (ld3_mf8_21, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 + svcntb () * 21),
	   z0 = svld3 (p0, x0 + svcntb () * 21))

/*
** ld3_mf8_24:
**	addvl	(x[0-9]+), x0, #24
**	ld3b	{z0\.b - z2\.b}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld3_mf8_24, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 + svcntb () * 24),
	   z0 = svld3 (p0, x0 + svcntb () * 24))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_mf8_m1:
**	decb	x0
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_mf8_m1, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 - svcntb ()),
	   z0 = svld3 (p0, x0 - svcntb ()))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_mf8_m2:
**	decb	x0, all, mul #2
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_mf8_m2, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 - svcntb () * 2),
	   z0 = svld3 (p0, x0 - svcntb () * 2))

/*
** ld3_mf8_m3:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #-3, mul vl\]
**	ret
*/
TEST_LOAD (ld3_mf8_m3, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 - svcntb () * 3),
	   z0 = svld3 (p0, x0 - svcntb () * 3))

/*
** ld3_mf8_m24:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #-24, mul vl\]
**	ret
*/
TEST_LOAD (ld3_mf8_m24, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 - svcntb () * 24),
	   z0 = svld3 (p0, x0 - svcntb () * 24))

/*
** ld3_mf8_m27:
**	addvl	(x[0-9]+), x0, #-27
**	ld3b	{z0\.b - z2\.b}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld3_mf8_m27, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_mf8 (p0, x0 - svcntb () * 27),
	   z0 = svld3 (p0, x0 - svcntb () * 27))

/*
** ld3_vnum_mf8_0:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_0, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, 0),
	   z0 = svld3_vnum (p0, x0, 0))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_vnum_mf8_1:
**	incb	x0
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_1, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, 1),
	   z0 = svld3_vnum (p0, x0, 1))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_vnum_mf8_2:
**	incb	x0, all, mul #2
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_2, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, 2),
	   z0 = svld3_vnum (p0, x0, 2))

/*
** ld3_vnum_mf8_3:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #3, mul vl\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_3, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, 3),
	   z0 = svld3_vnum (p0, x0, 3))

/*
** ld3_vnum_mf8_21:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #21, mul vl\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_21, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, 21),
	   z0 = svld3_vnum (p0, x0, 21))

/*
** ld3_vnum_mf8_24:
**	addvl	(x[0-9]+), x0, #24
**	ld3b	{z0\.b - z2\.b}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_24, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, 24),
	   z0 = svld3_vnum (p0, x0, 24))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_vnum_mf8_m1:
**	decb	x0
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_m1, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, -1),
	   z0 = svld3_vnum (p0, x0, -1))

/* Moving the constant into a register would also be OK.  */
/*
** ld3_vnum_mf8_m2:
**	decb	x0, all, mul #2
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_m2, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, -2),
	   z0 = svld3_vnum (p0, x0, -2))

/*
** ld3_vnum_mf8_m3:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #-3, mul vl\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_m3, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, -3),
	   z0 = svld3_vnum (p0, x0, -3))

/*
** ld3_vnum_mf8_m24:
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, #-24, mul vl\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_m24, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, -24),
	   z0 = svld3_vnum (p0, x0, -24))

/*
** ld3_vnum_mf8_m27:
**	addvl	(x[0-9]+), x0, #-27
**	ld3b	{z0\.b - z2\.b}, p0/z, \[\1\]
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_m27, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, -27),
	   z0 = svld3_vnum (p0, x0, -27))

/*
** ld3_vnum_mf8_x1:
**	cntb	(x[0-9]+)
** (
**	madd	(x[0-9]+), (?:x1, \1|\1, x1), x0
**	ld3b	{z0\.b - z2\.b}, p0/z, \[\2\]
** |
**	mul	(x[0-9]+), (?:x1, \1|\1, x1)
**	ld3b	{z0\.b - z2\.b}, p0/z, \[x0, \3\]
** )
**	ret
*/
TEST_LOAD (ld3_vnum_mf8_x1, svmfloat8x3_t, mfloat8_t,
	   z0 = svld3_vnum_mf8 (p0, x0, x1),
	   z0 = svld3_vnum (p0, x0, x1))
