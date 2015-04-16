/***********************************************************************
Check return values for scalb().

On Apple Darwin, "man scalb" reports:

     double
     scalb(double x, double n);

but math.h contains

	extern __pure double scalb __P((double, int));

This test program shows that math.h matches what the library function
arguments really are.

The man page is in agreement with other vendors, but the library is
not!

Sun Solaris and GNU/Linux scalb() return a NaN when the second
argument is not an integer value, while Apple Darwin, Compaq/DEC
OSF/1, FreeBSD, IBM AIX, and SGI IRIX return scalb(x,(int)y).

[10-Dec-2001]
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int
main(void)
{
	double x,y;

	while (1)
	{
		(void)printf("Enter x, y: ");
		if (scanf("%lg %lg", &x, &y) != 2)
			break;
		(void)printf("scalb(%lg,%lg) -> %g\n", x, y, scalb(x,y));
	}

	return (EXIT_SUCCESS);
}
