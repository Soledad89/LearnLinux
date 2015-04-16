#include "hoc.h"

#if defined(HAVE_ERRNO_H)
#include <errno.h>
#endif

#if defined(HAVE_FLOAT_H)
#include <float.h>
#endif

#if defined(HAVE_MATH_H)
#include <math.h>
#endif

#define LOG10_2 FP(0.3010299956639811952137388947244930267682)

#if defined(DBL_MAX)
#define LOG_DBL_MAX log(DBL_MAX)
#else
#define LOG_DBL_MAX FP(88.72) /* approximate value of log(DBL_MAX) for DEC VAX F-floating */
#endif

static fp_t	errcheck ARGS((fp_t, const char*));
EXTERN int signgam;

static fp_t
errcheck(fp_t d, const char* s)	/* check result of library call */
{
	if (errno == EDOM) {
		errno = 0;
#if defined(HAVE_IEEE_754)
		/* accept the default value */
#else
		execerror(s, "argument out of domain");
#endif
	} else if (errno == ERANGE) {
		errno = 0;
#if defined(HAVE_IEEE_754)
		/* accept the default value */
#else
		execerror(s, "result out of range");
#endif
	}
	return d;
}

fp_t
Abs(fp_t x)
{
	return (fabs(x));
}

fp_t
Acos(fp_t x)
{
	return errcheck(acos(x), "acos");
}

fp_t
Acosh(fp_t x)
{
#if defined(HAVE_ACOSH)
	return (acosh(x));
#else
	execerror("acosh(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Asin(fp_t x)
{
	return errcheck(asin(x), "asin");
}

fp_t
Asinh(fp_t x)
{
#if defined(HAVE_ASINH)
	return (asinh(x));
#else
	execerror("asinh(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Atan(fp_t x)
{
	return (atan(x));
}

fp_t
Atanh(fp_t x)
{
#if defined(HAVE_ATANH)
	return (atanh(x));
#else
	execerror("atanh(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Cbrt(fp_t x)
{
#if defined(HAVE_CBRT)
	return (cbrt(x));
#else
	return (pow(x,FP(1.0)/FP(3.0)));
#endif
}

fp_t
Cos(fp_t x)
{
	return (cos(x));
}

fp_t
Cosh(fp_t x)
{
	return errcheck(cosh(x), "cosh");
}

fp_t
Double(fp_t x)
{
    return ((fp_t)((double)x));
}


fp_t
Erf(fp_t x)
{
#if defined(HAVE_ERF)
	return (erf(x));
#else
	execerror("erf(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Erfc(fp_t x)
{
#if defined(HAVE_ERFC)
	return (erfc(x));
#else
	execerror("erfc(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Exp(fp_t x)
{
#if defined(HAVE_NAN_BOTCH)
	/* The Portland Group math library incorrectly returns NaN for
	these cases, sigh... */
	if (IsInf(x))
	{
		if (x < 0)
			return (FP(0.0));
		else
			return (x);
	}
#endif
	return errcheck(exp(x), "exp");
}

fp_t
Expm1(fp_t x)
{
#if defined(HAVE_EXPM1)
	return (expm1(x));
#else
	return (exp(x) - FP(1.0));		/* TO BE FIXED: inaccurate for small x */
#endif
}

fp_t
Gamma(fp_t x)
{
	fp_t y;

	if (IsNaN(x))
		return (x);
	else if (IsInf(x))
		return ((x > 0) ? x : NaN());

	y = Lgamma(x);

#if defined(HAVE_IEEE_754)
#else
	y=errcheck(y, "gamma");
	if(y > LOG_DBL_MAX)
		execerror("gamma result out of range", (char *)NULL);
#endif

	/* signgam is documented to be -1 or +1, yet on Sun Solaris
	   2.7 and 2.8, with ``Sun WorkShop 6 update 1 C++ 5.2 Patch
	   109508-01 2001/01/31'', and the later ``Sun WorkShop 6
	   update 2 C++ 5.3 2001/05/15'', when hoc is compiled with
	   either C or C++, and then linked with shared libraries,
	   signgam is 0 on return from both lgamma() and gamma()!  The
	   same object files, when linked staticly, produce correct
	   signgam values.  Curiously, a short test program does not
	   reproduce this bug. */

	if (signgam == 0)		/* should never happen */
	{
		int nmod2;

		nmod2 = (LONG_LONG)fabs(x) % (LONG_LONG)2;
		if (x >= FP(0.0))	/* Gamma() > 0 for x >= 0 */
			signgam = 1;
		else if (nmod2 == 0)	/* Gamma() is negative for negative even n */
			signgam = -1;
		else			/* Gamma() is positive for negative odd n */
			signgam = 1;
	}

	return (signgam * exp(y));
}

fp_t
Hypot(fp_t x, fp_t y)
{
	/***************************************************************
	   Discussion on the IEEE 754 Standard mailing list on
	   10-Dec-2001 cited this desired behavior of hypot():

	   ... From the 4.3BSD man page for hypot:

		NOTES
		     As might be expected, hypot(v,NaN) and
		     hypot(NaN,v) are NaN for all finite v; with
		     "reserved operand" in place of "NaN", the same is
		     true on a VAX.  But programmers on machines other
		     than a VAX (it has no infinity) might be
		     surprised at first to discover that
		     hypot(+infinity,NaN) = +infinity.  This is
		     intentional; it happens because hypot(infinity,v)
		     = +infinity for all v, finite or infinite.  Hence
		     hypot(infinity,v) is independent of v.  Unlike
		     the reserved operand on a VAX, the IEEE NaN is
		     designed to disappear when it turns out to be
		     irrelevant, as it does in hypot(infinity,NaN).

	   Tests show that native implementations did not always
	   conform, so we add our own special handling.
	***************************************************************/

	if (IsInf(x) || IsInf(y))
	   	return (Infinity());

#if defined(HAVE_HYPOT)
	return (hypot(x,y));
#else
	if (IsNaN(x))
		return (x);
	else if (IsNaN(y))
		return (y);
	else if (fabs(x) > fabs(y))
		return (fabs(x)*sqrt(FP(1.0) + (y/x)*(y/x)));
	else if (fabs(x) < fabs(y))
		return (fabs(y)*sqrt(FP(1.0) + (x/y)*(x/y)));
	else				/* must be equal */
		return (fabs(x)*sqrt(FP(2.0)));
#endif
}

fp_t
J0(fp_t x)
{
#if defined(HAVE_J0)
	return (j0(x));
#else
	execerror("J0(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
J1(fp_t x)
{
#if defined(HAVE_J1)
	return (j1(x));
#else
	execerror("J1(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Jn(fp_t n, fp_t x)
{
#if defined(HAVE_JN)
	return (jn((int)n, x));
#else
	execerror("Jn(n,x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Lgamma(fp_t x)
{
	/* return log(abs(Gamma(x))) */
#if defined(HAVE_LGAMMA)
	return (lgamma(x));
#elif defined(HAVE_GAMMA)
	return (gamma(x));		/* gamma() in C is log(Gamma()) */
#else
	execerror("lgamma(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Log(fp_t x)
{
	return errcheck(log(x), "log");
}

fp_t
Log10(fp_t x)
{
	return errcheck(log10(x), "log10");
}

fp_t
Log1p(fp_t x)
{
#if defined(HAVE_LOG1P)
	return (log1p(x));
#else
	fp_t u;
	/* Use log(), corrected to first order for truncation loss */
	u = FP(1.0) + x;
	if (u == FP(1.0))
		return (x);
	else
		return (log(u) * (x / (u - FP(1.0)) ));
#endif
}

fp_t
Log2(fp_t x)
{
	return errcheck(log10(x)/LOG10_2, "log2");
}

fp_t
Pow(fp_t x, fp_t y)
{
	return errcheck(pow(x,y), "exponentiation");
}

fp_t
Rsqrt(fp_t x)
{
#if defined(HAVE_RSQRT)
	return (rsqrt(x));
#else
	return (FP(1.0) / sqrt(x));
#endif
}

fp_t
Sin(fp_t x)
{
	return (sin(x));
}

fp_t
Single(fp_t x)
{
    return ((fp_t)((float)x));
}


fp_t
Sinh(fp_t x)
{
	return errcheck(sinh(x), "sinh");
}

fp_t
Sqrt(fp_t x)
{
	return errcheck(sqrt(x), "sqrt");
}

fp_t
Tan(fp_t x)
{
	return (tan(x));
}

fp_t
Tanh(fp_t x)
{
	return (tanh(x));
}

fp_t
Y0(fp_t x)
{
#if defined(HAVE_Y0)
	return (y0(x));
#else
	execerror("Y0(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Y1(fp_t x)
{
#if defined(HAVE_Y1)
	return (y1(x));
#else
	execerror("Y1(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Yn(fp_t n, fp_t x)
{
#if defined(HAVE_YN)
	return (yn((int)n, x));
#else
	execerror("Yn(n,x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}
