/* { dg-do compile } */
/* { dg-options "-maltivec -mno-vsx" } */
/* { dg-require-effective-target powerpc_altivec } */
/* { dg-require-effective-target lp64 } */

/* These should be rejected for 64-bit code.  */

__vector long vl;			/* { dg-error "invalid for 64" } */
__vector unsigned long vul;		/* { dg-error "invalid for 64" } */
__vector signed long vsl;		/* { dg-error "invalid for 64" } */
__vector __bool long int vbli;		/* { dg-error "invalid for 64" } */
__vector long int vli;			/* { dg-error "invalid for 64" } */
__vector unsigned long int vuli;	/* { dg-error "invalid for 64" } */
__vector signed long int vsli;		/* { dg-error "invalid for 64" } */
