/***********************************************************************
[10-Dec-2001]
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern int signgam;

int
main(void)
{
	double x, y;

	(void)printf("test of gamma():\n");
	for (x = -3.5; x < 3.5; x += 0.25)
	{
		y = gamma(x);
		(void)printf("\t%2d\t%7.3f\t%7.3f\t%7.3f\n", signgam, x, y, exp(y));
	};

	(void)printf("test of lgamma():\n");
	for (x = -3.5; x < 3.5; x += 0.25)
	{
		y = lgamma(x);
		(void)printf("\t%2d\t%7.3f\t%7.3f\t%7.3f\n", signgam, x, y, exp(y));
	}

	return (EXIT_SUCCESS);
}
