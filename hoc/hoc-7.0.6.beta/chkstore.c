/* The body expressions here attempt to defeat inlining optimizers */
/* to avoid confusion by registers that are longer than memory words. */

#include "stdc.h"

#if defined(STDC)
float
fstore(float *f)
#else
float
fstore(f)
float *f;
#endif
{
	return ((*f + *f + *f + *f)/(float) 4.0);
}

#if defined(STDC)
double
dstore(double *d)
#else
double
dstore(d)
double *d;
#endif
{
	return ((*d + *d + *d + *d)/(double)4.0);
}
