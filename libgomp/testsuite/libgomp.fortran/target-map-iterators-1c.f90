! { dg-do run }

! Like target-map-iterators-1.f90, but with non-constant iterator
! start/end/step.

program test
  implicit none

  integer, parameter :: DIM1 = 30
  integer, parameter :: DIM2 = 15

  type :: array_ptr
    integer, pointer :: arr(:)
  end type

  type (array_ptr) :: x(DIM1)
  integer :: expected

  expected = mkarray (2, 29, 3)
  if (doit (2, 29, 3) .ne. expected) stop 1

contains
  integer function mkarray (a, b, c)
    integer :: a, b, c
    integer :: exp = 0
    integer :: i, j

    do i = a, b
      if (mod(i+1, c) == 0) then
	allocate (x(i)%arr(DIM2))
	do j = 1, DIM2
	  x(i)%arr(j) = i * j
	  exp = exp + x(i)%arr(j)
	end do
      end if
    end do

    mkarray = exp
  end function

  integer function doit (a, b, c)
    integer :: a, b, c

    integer:: sum, i, j
    !$omp target map(iterator(i=a:b:c), to: x(i)%arr(:)) map(from: sum)
    sum = 0
    do i = a, b
      if (mod(i+1, c) == 0) then
	do j = 1, DIM2
	  sum = sum + x(i)%arr(j)
	end do
      end if
    end do
    !$omp end target

  doit = sum
  end function
end program
