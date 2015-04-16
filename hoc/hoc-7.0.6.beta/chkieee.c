/***********************************************************************
This Standard C and C++ program attempts to determine whether the host
arithmetic is IEEE 754 (return code 0 (success)) or not (return code 1
(error)); Because some IEEE 754 systems default to termination on
exceptions, we cannot use the existence of NaN and Infinity for this
test.  Instead, we examine the machine precision in single and double,
and the data sizes, and the binary representation of 1.0.
***********************************************************************/
#include <stdio.h>
#include "stdc.h"

extern float fstore ARGS((float *f));
extern double dstore ARGS((double *d));

int
main(VOID_ARG)
{
    double d, d_tmp;
    float f, f_tmp;
    int nd;
    int nf;
    union { unsigned int i[1]; float v; } ff;
    union { unsigned int i[2]; double v; } dd;
    int big_endian;

    for (nf = 0, f = 1.0; (f_tmp = ((float)1.0 + f/(float)2.0),
			   (f_tmp = fstore(&f_tmp)) != (float)1.0);
	 ++nf)
    {
	f /= (float)2.0;
	f = fstore(&f);
    }

    (void)printf("\tprecision of float  = %d\tfloat epsilon  = %g\n", nf, f);

    for (nd = 0, d = 1.0; (d_tmp = (double)((double)1.0 + d/(double)2.0),
			   (d_tmp = dstore(&d_tmp)) != (double)1.0);
	 ++nd)
    {
	d /= (double)2.0;
	d = dstore(&d);
    }

    (void)printf("\tprecision of double = %d\tdouble epsilon = %lg\n", nd, d);

    ff.v = (float)1.0;
    (void)printf("\t(float)1.0 = %#08x = %g\n", ff.i[0], ff.v);

    dd.v = (double)(-1.0);
    big_endian = (dd.i[0] != 0) ? 1 : 0;
    (void)printf("\t(double)(-1.0) = %#08x_%08x = %lg\n",
		 dd.i[1-big_endian],
		 dd.i[big_endian], dd.v);

    ff.v = 0.0;
    ff.i[0] = 0x3f800000;
    (void)printf("\tIEEE 754 32-bit:\t%s\n",
		 (ff.v == (float) 1.0) ? "yes" : "no");

    dd.v = 0.0;
    dd.i[1-big_endian] = 0xbff00000;
    (void)printf("\tIEEE 754 64-bit:\t%s\n",
		 (dd.v == (double)(-1.0)) ? "yes" : "no");

    return (((sizeof(float) == 4) &&
	     (nf == 23) &&
	     (ff.v == (float) 1.0) &&
	     (sizeof(double) == 8) &&
	     (nd == 52) &&
	     (dd.v == (double)(-1.0))) ? 0 : 1);
}
