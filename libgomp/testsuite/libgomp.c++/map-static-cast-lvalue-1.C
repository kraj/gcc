#include <cassert>

int foo (int x)
{
#pragma omp target map(static_cast<int&>(x))
  {
    x += 3;
  }
  return x;
}

int main (int argc, char *argv[])
{
  assert (foo (5) == 8);
  return 0;
}
