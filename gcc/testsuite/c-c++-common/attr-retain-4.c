/* { dg-do compile { target gnu_retain } } */
/* { dg-options "-Wall -O2 -fcommon" } */

int xyzzy __attribute__((__used__, __retain__)); 

/* { dg-final { scan-assembler "xyzzy" } } */
/* { dg-final { scan-assembler ",\"awR\"" { target gnu_retain } } } */
