/* { dg-do compile } */
/* { dg-options "-mno-sse -mfpmath=387" } */

void foo ()
{
  register int zmm_var asm ("ymm9");/* { dg-error "cannot be accessed" } */
  register int zmm_var2 asm ("23");/* { dg-error "cannot be accessed" } */
}
