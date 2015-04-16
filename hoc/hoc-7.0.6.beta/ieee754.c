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

#if defined(DEBUG_MACHEPS)
#if defined(HAVE_STDIO_H)
#include <stdio.h>
#endif
#endif

/* These functions are in -lm, but are missing from math.h, sigh... */

#if defined(__linux)
EXTERN float (truncf) ARGS((float));		/* all GNU/Linux */
EXTERN double (trunc) ARGS((double));		/* all GNU/Linux */
#if defined(HAVE_FP_T_QUADRUPLE)
EXTERN long double (truncl) ARGS((long double));/* all GNU/Linux */
#endif
#endif

#if defined(__osf__)
EXTERN int (ilogbf) ARGS((float));		/* Compaq/DEC OSF/1 */
EXTERN int (ilogb) ARGS((double));		/* Compaq/DEC OSF/1 */
#if defined(HAVE_FP_T_QUADRUPLE)
EXTERN int (ilogbl) ARGS((long double));		/* Compaq/DEC OSF/1 */
#endif
#if defined(HAVE_NEAREST)
EXTERN float (nearestf) ARGS((float,float));	/* Compaq/DEC OSF/1 */
EXTERN double (nearest) ARGS((double,double));	/* Compaq/DEC OSF/1 */
#if defined(HAVE_FP_T_QUADRUPLE)
EXTERN long double (nearestl) ARGS((long double,long double));	/* Compaq/DEC OSF/1 */
#endif
#endif
#endif

#define TWOTO23		FP(8388608.0)					/* 2^23 */
#define TWOTO52		FP(4503599627370496.0)				/* 2^52 */
#define TWOTO64		FP(18446744073709551616.0)			/* 2^64 */
#define TWOTO112	FP(5192296858534827628530496329220096.0)	/* 2^112 */

#define STORE(expr)	(temp__ = (expr), store(&temp__))

fp_t
Ceil(fp_t x)
{
	fp_t y;

	/* TO DO: Should Ceil(-Infinity) return a finite number or
	not? Sun's returns ceil(Infinity) as 2^63, and ceil(-Infinity)
	as -2^63, which both seem inconsistent. */

	if (IsNaN(x))
		return (x);
	else if (IsInf(x))
		return (x);

#if defined(HAVE_CEIL)
	y = ceil(x);
#else
	/* Simply using (integer cast)x here is not sufficient, because
	   the result can be negative in the case of integer overflow.

	   The solution is to clear fractional bits by adding and
	   subtracting a suitable large power of the base, then to add
	   one to the result to guarantee that x <= ceil(x).

	   This seems a much cleaner, and much more portable, solution
	   than the extensive bit manipulations that are used in Sun's
	   fdlibm (freely-distributable math library) code. */
#if 0
	y = (fp_t)((LONG_LONG)(x));
#else
	{
		static fp_t base_to_Pm1 = FP(0.0);
		fp_t xabs;

		if (base_to_Pm1 == FP(0.0))	/* normally happens only on first call */
			base_to_Pm1 = Pow(BASE,(P - 1));
		xabs = fabs(x);
		if (xabs == FP(0.0))
			y = x;		/* preserve sign of zero */
		else if (xabs > base_to_Pm1)
			y = x; /* x has no representable fractional part, so ceil(x) = x */
		else
		{
			fp_t temp__;

			y = STORE(STORE(base_to_Pm1 + xabs) - base_to_Pm1);
			if (x < FP(0.0))
				y = -y;
		}
	}
#endif
	if (x > y)
		y += FP(1.0);
#endif
	return (y);
}

fp_t
Class(fp_t x)
{
	if (IsQNaN(x))
		return (CLASS_QNAN);
	else if (IsSNaN(x))
		return (CLASS_SNAN);
	else if (IsInf(x))
		return ((x < FP(0.0)) ? CLASS_NEGINF : CLASS_POSINF);
	else if (x <= -MinNormal())
		return (CLASS_NEGNORMAL);
	else if (x <= -MinSubnormal())
		return (CLASS_NEGSUBNORMAL);
	else if ((x == FP(0.0)) && (Copysign(FP(1.0),x) == FP(-1.0)))
		return (CLASS_NEGZERO);
	else if ((x == FP(0.0)) && (Copysign(FP(1.0),x) == FP(1.0)))
		return (CLASS_POSZERO);
	else if (x >= MinNormal())
		return (CLASS_POSNORMAL);
	else if (x >= MinSubnormal())
		return (CLASS_POSSUBNORMAL);
	else
		return (CLASS_UNKNOWN);
}

static unsigned int
high_word(fp_t x)
{
	FP_Parts d;
	int big_endian = IsBigEndian();
	unsigned int hi;

	d.v = x;

#if defined(HAVE_FP_T_SINGLE)
	hi = d.i[0];
#elif defined(HAVE_FP_T_DOUBLE)
	hi = d.i[1-big_endian];
#elif defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		hi = d.i[3*(1-big_endian)];
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		hi = d.i[1-big_endian];
	else				/* 80-bit Intel x86 format */
		hi = d.i[2];
#else
	hi = 0;
	execerror("internal error at " __FILE__ ":" __LINE__, (const char*)NULL)
#endif
	return (hi);
}


fp_t
Copysign(fp_t x, fp_t y)
{

#if defined(HAVE_COPYSIGN) && !defined(__sgi) /* SGI IRIX copysign() is wrong for NaN arguments */

#if defined(FP_ARCH_IA32) || defined(FP_ARCH_ALPHA)
	/* Preserve hoc's convention that IEEE 754 NaN is unsigned by
	   default, even though on this architecture, the reverse is
	   true. */
	if (IsNaN(y))
		return (-copysign(x,y));
	else
		return (copysign(x,y));
#else
	return (copysign(x,y));
#endif

#else /* NOT defined(HAVE_COPYSIGN) */
	return ((high_word(y) >> 31) ? -x : x);
#endif

}

fp_t
Exponent(fp_t x)
{	/* Return the unbiased base-2 exponent of x */
#if defined(HAVE_IEEE_754)
	FP_Parts d;
	fp_t xabs;
	int big_endian = IsBigEndian();
	int offset;

	xabs = (x < FP(0.0)) ? -x : x;

	if (!IsNaN(x) && (xabs < MinNormal())) /* protect against NaN botch! */
	{

#if defined(HAVE_FP_T_DOUBLE)
		offset = -52;
		x *= TWOTO52;
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		{
		    offset = -112;
		    x *= TWOTO112;
		}
		else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		{
			offset = -52;
			x *= TWOTO52;
		}
		else			/* 80-bit Intel x86 format */
		{
		    offset = -64;
		    x *= TWOTO64;
		}
#endif

#if defined(HAVE_FP_T_SINGLE)
		offset = -23;
		x *= TWOTO23;
#endif

	}
	else
		offset = 0;

	d.v = x;

#if defined(HAVE_FP_T_DOUBLE)
	return ((fp_t)((int)((d.i[1-big_endian] & UL(0x7ff00000)) >> 20) - 1023 + offset));
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		return ((fp_t)((int)((d.i[3*(1-big_endian)] & UL(0x7fff0000)) >> 16)) - 16383 + offset);
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		return ((fp_t)((int)((d.i[1-big_endian] & UL(0x7ff00000)) >> 20) - 1023 + offset));
	else				/* 80-bit Intel x86 format */
		return ((fp_t)((int)(d.i[2] & UL(0x7fff)) - 16383 + offset));
#endif

#if defined(HAVE_FP_T_SINGLE)
	return ((fp_t)((int)((d.i[0] & UL(0x7f800000)) >> 23) - 127 + offset));
#endif

#else
	return ((fp_t)Nint(Log2(xabs))); /* TO DO: implement efficiently for non-IEEE 754 systems */
#endif
}

fp_t
Floor(fp_t x)
{
	fp_t y;

	/* TO DO: Should Floor(-Infinity) return a finite number or
	not? Sun's returns floor(Infinity) as 2^63, and floor(-Infinity)
	as -2^63, which both seem inconsistent. */

	if (IsNaN(x))
		return (x);
	else if (IsInf(x))
		return (x);

#if defined(HAVE_FLOOR)
	y = floor(x);
#else
#if 0
	y = (fp_t)((LONG_LONG)(x));
#else
	y = Ceil(x);			/* see implementation comments in Ceil() */
#endif
	if (x < y)
		y--;
#endif
	return (y);
}

fp_t
Fmod(fp_t x, fp_t y)
{
#if defined(HAVE_FMOD)
	return (fmod(x,y));
#else
	execerror("fmod(x,y) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Ilogb(fp_t x)
{
#if defined(HAVE_ILOGB)
	return ((fp_t)ilogb(x));
#else
	return ((fp_t)(int)Log2(x));
#endif
}

fp_t
Infinity(void)
{
	static fp_t infinity = FP(0.0);

	if (infinity == FP(0.0))	/* normally happens only on first call */
	{
		fp_t temp__;

#if defined(HAVE_IEEE_754)
		infinity = STORE(FP(1.0));
		infinity /= STORE(FP(0.0));
#elif defined(DBL_MAX)
		infinity = DBL_MAX;
#else
		execerror("Infinity() is not available on this system",
			  (const char*)NULL);
#endif
	}
	return (infinity);
}

fp_t
Integer(fp_t x)
{
#if 0
	return (fp_t)(long)x;
#else
	/* Since all numbers are represented as type fp_t, we can
	provided a wider range of integers than long provides: */
	if (IsNaN(x))
	{
		execerror("int(NaN) is undefined", (const char*)NULL);
		return (FP(0.0));		/* NOT REACHED */
	}
	else if (x < FP(0.0))
		return (Ceil(x));
	else /* (x >= FP(0.0)) */
		return (Floor(x));
#endif
}

int
IsBigEndian(void)
{
	FP_Parts d;
	d.v = FP(1.0);
	return ((d.i[0] != 0) ? 1 : 0);
}

fp_t
IsFinite(fp_t x)
{
	return ((fp_t)(!IsNaN(x) && !IsInf(x)));
}

fp_t
IsInf(fp_t x)
{
	/* This portable implementation that does not require IEEE 754
	   arithmetic to function properly is due to Norman
	   L. Schryer, and used in his FPTEST package.  See ``A Test
	   of a Computer's Floating-Point Arithmetic Unit'', Bell
	   Laboratories Computing Science Technical Report no. 89,
	   February 4, 1981.  Available, with the software (under
	   license), at http://www.bell-labs.com/project/fptest/ */

#if defined(HAVE_NAN_BOTCH)		/* see IsNaN() for why */
	if (IsNaN(x))
		return (FP(0.0));
#endif

	if (x < FP(0.0))
		x = -x;			/* force positive */
	if (x < FP(1.0))		/* small, so finite */
		return (FP(0.0));
	else if ((x / BASE) == x)	/* only Inf/BASE == Inf */
		return (FP(1.0));
	else				/* must be finite */
		return (FP(0.0));
}

fp_t
IsNaN(fp_t x)
{
	fp_t temp__;

#if defined(HAVE_NAN_BOTCH)
	/* The Portland Group compilers, pgcc and pgCC, on Intel x86 */
	/* incorrectly generate a 64-bit integer comparison, instead */
	/* of a floating-point one.  This is wrong for NaNs, so even */
	/* the STORE() subterfuge does not produce the right answer. */
	/* Therefore, we have to grovel around in the bits.          */
	FP_Parts d;
	int big_endian = IsBigEndian();

	d.v = x;

#if defined(HAVE_FP_T_DOUBLE)
	return ((fp_t)(((d.i[1-big_endian] & UL(0x7ff00000)) == UL(0x7ff00000)) &&
			 ((d.i[1-big_endian] & UL(~0xfff00000)) ||
			  d.i[big_endian])));
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		return ((fp_t)(((d.i[3*(1-big_endian)] & UL(0x7fff0000)) == UL(0x7fff0000)) &&
			       ((d.i[3*(1-big_endian)] & UL(~0x7fff0000)) ||
				d.i[1] || d.i[2] || d.i[3*big_endian])));
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		return ((fp_t)(((d.i[1-big_endian] & UL(0x7ff00000)) == UL(0x7ff00000)) &&
				 ((d.i[1-big_endian] & UL(~0xfff00000)) ||
				  d.i[big_endian])));
	else				/* 80-bit Intel x86 format */
		return ((fp_t)(((d.i[2] & UL(0x7fff)) == UL(0x7fff)) && (d.i[1] || d.i[0])));
#endif

#if defined(HAVE_FP_T_SINGLE)
	return ((fp_t)(((d.i[0] & UL(0x7f800000)) == UL(0x7f800000)) &&
		       (d.i[0] & UL(~0xff800000))));
#endif

#else
	return ((fp_t)(STORE(x) != x));	/* should thwart optimizers */
#endif
}

fp_t
IsNormal(fp_t x)
{
	return (fp_t)((IsFinite(x) && !IsSubnormal(x)));
}

/***********************************************************************
The implementation of IsQNaN() is unavoidably system-dependent, and for
two of these architectures, there is only one type of NaN, so for them,
both IsSNaN() and IsQNaN() should be true!

From p. 4-59 of the Alpha AXP Architecture Manual:
	quiet NaN:	initial fraction bit of 1
	signaling NaN:	initial fraction bit of 0
------------------------------------------------------------------------
From p. 21 of SPARC Architecture Manual v8:
[same choice as Alpha]
	quiet NaN:	initial fraction bit of 1
	signaling NaN:	initial fraction bit of 0
------------------------------------------------------------------------
From p. 145 of The PowerPC Architecture:
[same as Alpha and SPARC]
	quiet NaN:	initial fraction bit of 1
	signaling NaN:	initial fraction bit of 0
------------------------------------------------------------------------
From p. 2-5 of i860 Microprocessor Programmer's Reference Manual:
[same as Alpha, SPARC, PowerPC]
	quiet NaN:	initial fraction bit of 1
	signaling NaN:	initial fraction bit of 0
------------------------------------------------------------------------
From p. 2-15 of MC68881 Floating-Point Coprocessor Users Manual:
[same as Alpha, SPARC, PowerPC, Intel i860]
	quiet NaN:	initial fraction bit of 1
	signaling NaN:	initial fraction bit of 0
------------------------------------------------------------------------
From p. 5-3 (sequential 87 of 216) Intel IA-64 Architecture Software
Developer's Manual, Rev 1.0:
[same as Alpha, SPARC, PowerPC, Intel i860, MC68881]
	quiet NaN:	initial fraction bit of 1
	signaling NaN:	initial fraction bit of 0
------------------------------------------------------------------------
From p. 8-7 of PA-RISC 2.0 Architecture:
[opposite of Alpha, SPARC, PowerPC, i860]
	quiet NaN:	initial fraction bit of 0
	signaling NaN:	initial fraction bit of 1
------------------------------------------------------------------------
From p. E-2 of MIPS RISC Architecture:
[no distinction between NaNs]
	NaN:		initial fraction bit of 0
------------------------------------------------------------------------
From Palmer and Morse The 8087 Primer and p. I-25 of iAPX 286
Programmer's Reference Manual:
[no distinction between NaNs]
	NaN:		at least one nonzero fraction bit
Generated NaNs have negative sign bit.

Experiments on Pentium III show 0/0 -> 0xfff80000_00000000, so the
leading fraction bit is the only nonzero generated.  However, this can
be explicitly negated to obtain 0x7ff80000_00000000.
------------------------------------------------------------------------
***********************************************************************/

#if defined(HAVE_FP_T_SINGLE)
static fp_t
IsQNaN32(float x)
{
	SP_Parts d;
	fp_t temp__;

	d.v = x;

#if defined(FP_ARCH_ALPHA)
	return ((fp_t)((d.i[0] & UL(0x00400000)) != 0));
#elif defined(FP_ARCH_SPARC)
	return ((fp_t)((d.i[0] & UL(0x00400000)) != 0));
#elif defined(FP_ARCH_I860)
	return ((fp_t)((d.i[0] & UL(0x00400000)) != 0));
#elif defined(FP_ARCH_POWER)
	return ((fp_t)((d.i[0] & UL(0x00400000)) != 0));
#elif defined(FP_ARCH_IA64)
	return ((fp_t)((d.i[0] & UL(0x00400000)) != 0));
#elif defined(FP_ARCH_PA_RISC)
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return (FP(1.0));
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return (FP(1.0));
#else  /* fall back to majority style, which could still be wrong */
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return (FP(1.0));
#endif
}
#endif

static fp_t
IsQNaN64(double x)
{
	DP_Parts d;
	int big_endian = IsBigEndian();
	fp_t temp__;

	d.v = x;

#if defined(FP_ARCH_ALPHA)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#elif defined(FP_ARCH_SPARC)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#elif defined(FP_ARCH_I860)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#elif defined(FP_ARCH_POWER)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#elif defined(FP_ARCH_IA64)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#elif defined(FP_ARCH_PA_RISC)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#else  /* fall back to majority style, which could still be wrong */
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#endif

}

#if defined(HAVE_FP_T_QUADRUPLE)
static fp_t
IsQNaN128(long double x)
{
	QP_Parts d;
	int big_endian = IsBigEndian();
	fp_t temp__;

	d.v = x;

#if defined(FP_ARCH_ALPHA)
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#elif defined(FP_ARCH_SPARC)
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#elif defined(FP_ARCH_I860)
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#elif defined(FP_ARCH_POWER)
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#elif defined(FP_ARCH_IA64)
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#elif defined(FP_ARCH_PA_RISC)
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#else  /* fall back to majority style, which could still be wrong */
	return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#endif

}
#endif


fp_t
IsQNaN(fp_t x)
{
	if (IsNaN(x))
	{

#if defined(HAVE_FP_T_DOUBLE)
		return (IsQNaN64(x));
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
			return (IsQNaN64(x));
		else
			return (IsQNaN128(x));
#endif

#if defined(HAVE_FP_T_SINGLE)
		return (IsQNaN32(x));
#endif

	}
	else
		return (FP(0.0));
}

#if defined(HAVE_FP_T_SINGLE)
static fp_t
IsSNaN32(float x)
{
	SP_Parts d;
	fp_t temp__;

	d.v = x;

#if defined(FP_ARCH_ALPHA)
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#elif defined(FP_ARCH_SPARC)
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#elif defined(FP_ARCH_I860)
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#elif defined(FP_ARCH_POWER)
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#elif defined(FP_ARCH_IA64)
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#elif defined(FP_ARCH_PA_RISC)
	return ((fp_t)((d.i[0] & UL(0x00400000)) != 0));
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return (FP(1.0));
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return (FP(1.0));
#else  /* fall back to majority style, which could still be wrong */
	return ((fp_t)((d.i[0] & UL(0x00400000)) == 0));
#endif
}
#endif

static fp_t
IsSNaN64(double x)
{
	DP_Parts d;
	int big_endian = IsBigEndian();
	fp_t temp__;

	d.v = x;

#if defined(FP_ARCH_ALPHA)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#elif defined(FP_ARCH_SPARC)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#elif defined(FP_ARCH_I860)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#elif defined(FP_ARCH_POWER)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#elif defined(FP_ARCH_IA64)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#elif defined(FP_ARCH_PA_RISC)
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) != 0));
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);	/* avoid complaints about unused d */
	return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#else  /* fall back to majority style, which could still be wrong */
	return ((fp_t)((d.i[1 - big_endian] & UL(0x00080000)) == 0));
#endif
}

#if defined(HAVE_FP_T_QUADRUPLE)
static fp_t
IsSNaN128(long double x)
{
	QP_Parts d;
	int big_endian = IsBigEndian();
	fp_t temp__;

	d.v = x;

#if defined(FP_ARCH_ALPHA)
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#elif defined(FP_ARCH_SPARC)
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#elif defined(FP_ARCH_I860)
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#elif defined(FP_ARCH_POWER)
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#elif defined(FP_ARCH_IA64)
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#elif defined(FP_ARCH_PA_RISC)
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) != 0));
#elif defined(FP_ARCH_IA32)
		(void)STORE(d.v);	/* avoid complaints about unused d */
		return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#elif defined(FP_ARCH_MIPS)
		(void)STORE(d.v);	/* avoid complaints about unused d */
		return ((fp_t)(big_endian == big_endian)); /* avoids compiler warnings about unused big_endian*/
#else  /* fall back to majority style, which could still be wrong */
		return ((fp_t)((d.i[3*(1 - big_endian)] & UL(0x00008000)) == 0));
#endif
}
#endif

fp_t
IsSNaN(fp_t x)
{
	if (IsNaN(x))
	{

#if defined(HAVE_FP_T_DOUBLE)
		return (IsSNaN64(x));
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
			return (IsSNaN64(x));
		else
			return (IsSNaN128(x));
#endif

#if defined(HAVE_FP_T_SINGLE)
		return (IsSNaN32(x));
#endif

	}
	else
		return (FP(0.0));
}

fp_t
IsSubnormal(fp_t x)
{
	if (x == FP(0.0))
		return (FP(0.0));
	else if (IsNaN(x))
		return (FP(0.0));
	else if (IsInf(x))
		return (FP(0.0));
	else
	{
#if defined(HAVE_IEEE_754)
		return ((fp_t)(Exponent(x) < EMIN));
#else
		return (FP(0.0));
#endif
	}
}

fp_t
Ldexp(fp_t x, fp_t y)
{
#if defined(HAVE_LDEXP)
	return (ldexp(x,(int)y));
#else
	return (x * pow(BASE, (fp_t)(int)y));
#endif
}

#if defined(HAVE_FP_T_QUADRUPLE)
/* Sun Solaris <sunmath.h> contains this note: ``ANSI C Future Library
   Directions {4.13.4}: Note that frexpf, ldexpf, modff, frexpl, ldexpl
   and modfl are _not_ provided. */
fp_t
ldexpl(fp_t x, int n)
{					/* return x * 2**n */
	fp_t result;

	if (IsNaN(x))
		result = x;
	else if (IsInf(x))
		result = x;
	else
	{
		result = x * powl(FP(2.0), (fp_t)n);
		if (IsInf(result))
		{
			/* Sun documentation says that ldexp() returns
			   +/-HUGE_VAL on overflow, and sets errno to
			   ERANGE */
			result = (x < FP(0.0)) ? -MaxNormal() : MaxNormal();
			errno = ERANGE;
		}
	}
	return (result);
}
#endif /* defined(HAVE_FP_T_QUADRUPLE) */

#if defined(HAVE_FP_T_SINGLE)
/* Sun Solaris <sunmath.h> contains this note: ``ANSI C Future Library
   Directions {4.13.4}: Note that frexpf, ldexpf, modff, frexpl, ldexpl
   and modfl are _not_ provided. */
fp_t
ldexpf(fp_t x, int n)
{					/* return x * 2**n */
	fp_t result;

	if (IsNaN(x))
		result = x;
	else if (IsInf(x))
		result = x;
	else
	{
		result = x * pow(FP(2.0), (fp_t)n); /* NB: pow, not powf, for macro substitution */
		if (IsInf(result))
		{
			/* Sun documentation says that ldexp() returns
			   +/-HUGE_VAL on overflow, and sets errno to
			   ERANGE */
			result = (x < FP(0.0)) ? -MaxNormal() : MaxNormal();
			errno = ERANGE;
		}
	}
	return (result);
}
#endif /* defined(HAVE_FP_T_SINGLE) */

#if defined(DEBUG_MACHEPS)
void
Dump(const char *s, fp_t x)
{
	FP_Parts d;
	int big_endian = IsBigEndian();
	d.v = x;

#if defined(HAVE_FP_T_DOUBLE)
	(void)fprintf(stderr,"%s: %g\t0x%08x_%08x\n",
		      s, d.v, d.i[1-big_endian], d.i[big_endian]);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
	{
		if (big_endian)		/* 128-bit big endian */
			(void)fprintf(stderr,"%s: %g\t0x%08x_%08x_%08x_%08x\n",
				      s, d.v, d.i[0], d.i[1], d.i[2], d.i[3]);
		else			/* 128-bit little endian */
			(void)fprintf(stderr,"%s: %g\t0x%08x_%08x_%08x_%08x\n",
				      s, d.v, d.i[3], d.i[2], d.i[1], d.i[0]);
	}
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		(void)fprintf(stderr,"%s: %g\t0x%08x_%08x\n",
			      s, d.v, d.i[1-big_endian], d.i[big_endian]);
	else				/* 80-bit Intel x86 format */
		(void)fprintf(stderr,"%s: %g\t0x%04x_%08x_%08x\n",
			      s, d.v, (d.i[2] & 0xffff), d.i[1], d.i[0]);
#endif

#if defined(HAVE_FP_T_SINGLE)
	(void)fprintf(stderr,"%s: %g\t0x%08x\n", s, d.v, d.i[0]);
#endif
}
#endif

fp_t
Macheps(fp_t x)
{
	/* Macheps(x) returns the generalized machine epsilon of x,
	   the smallest number which, when added to x, produces a sum
	   that still differs from x: (x + macheps(x)) != x.

	   NB: This code guarantees these results:
		  macheps(Inf)  -> NaN
		  macheps(-Inf) -> NaN
		  macheps(NaN)  -> NaN
		  macheps(-NaN) -> -NaN
	*/

	fp_t epsilon;
	fp_t temp;
	fp_t temp__;

#if !defined(HAVE_NAN_BOTCH)
	static fp_t macheps_1 = FP(0.0);

	if ((x == FP(1.0)) && (macheps_1 != FP(0.0))) /* handle common case fast */
		return (macheps_1);
#endif

	if (IsNaN(x))
		return (x);

	if (IsInf(x))
		return (NaN());

	if (x == FP(0.0))
		epsilon = FP(1.0);
	else
		epsilon = fabs(x);

#if defined(DEBUG_MACHEPS)
	Dump("macheps(): 1: x = ", x);
	Dump("macheps(): 1: epsilon = ", epsilon);
	Dump("macheps(): 1: epsilon/BASE = ", epsilon/BASE);
#if 1
	temp = STORE(x + STORE(epsilon/BASE));
#else
	temp = STORE(x + epsilon/BASE);
#endif
	Dump("macheps(): 1: temp = ", temp);
	while (STORE(temp) != x)
	{
		epsilon /= BASE;
		Dump("macheps(): 2: epsilon = ", epsilon);
#if 1
		temp = STORE(x + STORE(epsilon/BASE));
#else
		temp = STORE(x + epsilon/BASE);
#endif
		Dump("macheps(): 2: temp = ", temp);
	}
#else
	while (STORE(x + STORE(epsilon/BASE)) != x)
		epsilon /= BASE;
#endif

	/* purify epsilon to eliminate all but high-order bit */
	temp = STORE(x + epsilon);
	if (IsInf(temp))/* x == MAXNORMAL: avoid overflow in purification */
		epsilon = STORE(x  + STORE(epsilon - x));
	else
		epsilon = STORE(temp - x);

#if !defined(HAVE_NAN_BOTCH)
	if (x == FP(1.0))			/* remember common case */
		macheps_1 = epsilon;
#endif

	return (epsilon);
}

fp_t
MaxNormal(void)
{
	static fp_t maxnormal = FP(0.0);

	if (maxnormal == FP(0.0))	      /* happens only on first call */
		maxnormal = (BASE * ((FP(1.0) - Pow(BASE, -P)) * Pow(BASE, EMAX)));
	return (maxnormal);
}

fp_t
MaxSubnormal(void)
{
	static fp_t maxsubnormal = FP(0.0);

	if (maxsubnormal == FP(0.0))	/* happens only on first call */
	{
		maxsubnormal = (FP(1.0) - Pow(BASE, -(P - FP(1.0)))) * MinNormal();
		if (maxsubnormal == FP(0.0)) /* then this system lacks subnormals */
			maxsubnormal = Macheps(FP(0.0));
	}
	return (maxsubnormal);
}

fp_t
MinNormal(void)
{
	static fp_t minnormal = FP(0.0);

	if (minnormal == FP(0.0))	      /* happens only on first call */
		minnormal = Pow(BASE, EMIN);
	return (minnormal);
}

fp_t
MinSubnormal(void)
{
	/* Return the minimimum subnormal number.  If subnormals are
	   not supported, then return the minimim normal number instead. */
	static fp_t minsubnormal = FP(0.0);

	if (minsubnormal == FP(0.0))	/* normally happens only on first call */
	{
		/* This code works for IEEE 754 (with or without subnormals),
		   and also non-IEEE 754. */
		minsubnormal = Macheps(FP(0.0));
	}
	return (minsubnormal);
}

fp_t
NaN(void)
{
	static fp_t nan = FP(0.0);
	fp_t temp__;

	if (nan == FP(0.0))	/* normally happens only on first call */
	{
#if defined(HAVE_IEEE_754)
		nan = STORE(FP(0.0));
		nan /= STORE(FP(0.0));
#else
		/* ISO/IEC 9899:1999(E) Programming languages -- C, Second
		edition 1999-12-01, says in section 7.12.11.2 on p. 236: ``If
		the implementation does not support quiet NaNs, the functions
		[nan(""), nanf(""), nanl("")] return zero.'' */
#endif
	}
	return (nan);
}

fp_t
Nearest(fp_t x, fp_t y)
{
#if defined(HAVE_NEAREST)
	return (nearest(x,y));
#else
	return (Nextafter(x,y));		/* TO BE FIXED: incorrect translation */
#endif
}

fp_t
Nextafter(fp_t x, fp_t y)
{
	/* hoc testing showed that SGI IRIX 6.5 native nextafter()
	   does not handle Infinity and NaN arguments as required by
	   IEEE 754.  We therefore handle the special cases ourselves,
	   and use the native one only if it is available. */
	if (IsNaN(x))
		return (NaN());
	else if (IsNaN(y))
		return (NaN());
	else if (IsInf(x) && (x > FP(0.0)) && ((y < FP(0.0)) || (y < x)))
		return (MaxNormal());
	else if (IsInf(x) && (x < FP(0.0)) && ((y > FP(0.0)) || (y > x)))
		return (-MaxNormal());
	else if (IsInf(x) && (x == y))
		return (x);
	else if ((x == MaxNormal()) && (x < y))
		return (Infinity());
	else if ((x == -MaxNormal()) && (x > y))
		return (-Infinity());

#if defined(HAVE_NEXTAFTER) && defined(HAVE_FP_T_DOUBLE) /* cannot substitute alternate precision for this one! */
	else
		return (nextafter(x,y));
#else
	else if (x < y)
		return (x + Macheps(x));
	else if (x > y)
		return (x - Macheps(-x));
	else /* (x == y) */
		return (x);
#endif
}

fp_t
Nint(fp_t x)
{
	return ((x >= FP(0.0)) ?
		(fp_t)((LONG_LONG)(x + FP(0.5))) :
		(fp_t)((LONG_LONG)(x - FP(0.5))));
}

fp_t
QNaN(void)
{
	return (NaN());
}

fp_t
Remainder(fp_t x, fp_t y)
{
#if defined(HAVE_REMAINDER)
	return (remainder(x,y));
#else
	execerror("remainder(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

#if defined(HAVE_FP_T_QUADRUPLE)
fp_t
remainderl(fp_t x, fp_t y)
{
	fp_t result;

	if (y == FP(0.0))
		result = QNaN();
	else if (IsNaN(x))
		result = x;
	else if (IsNaN(y))
		result = y;
	else if (IsInf(x))
		result = QNaN();
	else if (IsInf(y))
		result = x;
	else
	{
		/* TO DO: What should happen when x/y is not representable as an integer? */
		LONG_LONG n;

		n = (LONG_LONG)Nint(x/y);
		if (fabs((fp_t)n - x/y) == FP(0.5)) /* then must ensure that n is even */
		{
			if (n < 0)
				n = (n - 1) & ~1;
			else
				n = (n + 1) & ~1;
		}
		result = x - (fp_t)n * y;
	}
	return (result);
}
#endif

fp_t
Rint(fp_t x)
{
#if defined(HAVE_RINT)
	return (rint(x));
#else
	execerror("rint(x) is not available on this system", (const char*)NULL);
	return (FP(0.0));			/* NOT REACHED */
#endif
}

fp_t
Scalb(fp_t x, fp_t y)
{
	y = Integer(y);		/* to match documentation in help_scalb() */
				/* and get consistency across platforms */
				/* NB: Use Integer(), not (LONG_LONG)(), to */
				/* handle NaN as documented */
#if defined(HAVE_SCALB)
	return (scalb(x,y));
#else
	return (x * pow(BASE, y));
#endif
}

fp_t
Significand(fp_t x)
{
	fp_t result;
	fp_t temp__;

#if defined(HAVE_SIGNIFICAND)
	result = significand(x);

	/* Some systems produce incorrect signs from significand(). */

	if ((result == FP(0.0)) && (copysign(FP(1.0), x) == FP(-1.0)))
	{
		result = FP(0.0);
		result = -result;	/* generate negative zero */
	}
#else
	/* Scale subnormal numbers into the normal range to avoid
	problems with subnormals in Trunc() and Log2() (e.g.,
	Compaq/DEC Alpha OSF/1 4.0). */

	if (IsNaN(x))
		result = copysign(FP(1.0),x) * NaN();
	else if (IsInf(x))
		result = copysign(FP(1.0),x) * Infinity();
	else if (x == FP(0.0))
		result = copysign(FP(1.0),x) * STORE(FP(0.0));
	else
	{
		FP_Parts d;
		fp_t xabs;
		int big_endian = IsBigEndian();

		xabs = (x < FP(0.0)) ? -x : x;

#if defined(HAVE_FP_T_DOUBLE)
		if (xabs < MinNormal())	/* scale to force normalization */
			x *= TWOTO52;
		d.v = x;
		d.i[1-big_endian] &= UL(0x000fffff);
		d.i[1-big_endian] |= (x < FP(0.0)) ? UL(0xbff00000) : UL(0x3ff00000);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		if (xabs < MinNormal())	/* scale to force normalization */
		{
			if (sizeof(d) == sizeof(d.v))	/* 128-bit */
				x *= TWOTO112;
			else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
				x *= TWOTO52;
			else				/* 80-bit Intel x86 format */
				x *= TWOTO64;
		}
		d.v = x;
		if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		{
			d.i[3*(1-big_endian)] &= UL(0x0000ffff);
			d.i[3*(1-big_endian)] |= (x < FP(0.0)) ? UL(0xbfff0000) : UL(0x3fff0000);
		}
		else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		{
			d.i[1-big_endian] &= UL(0x000fffff);
			d.i[1-big_endian] |= (x < FP(0.0)) ? UL(0xbff00000) : UL(0x3ff00000);
		}
		else				/* 80-bit Intel x86 format */
		{
			d.i[2] &= UL(0xffff0000);
			d.i[2] |= (x < FP(0.0)) ? UL(0xbfff) : UL(0x3fff);
		}
#endif

#if defined(HAVE_FP_T_SINGLE)
		if (xabs < MinNormal())	/* scale to force normalization */
			x *= TWOTO23;
		d.v = x;
		d.i[0] &= UL(0x007fffff);
		d.i[0] |= (x < FP(0.0)) ? UL(0xbf800000) : UL(0x3f800000);
#endif

		result = d.v;
	}
#endif

	return (result);
}

#if defined(HAVE_FP_T_SINGLE)
static float
SNaN32(int count)
{
	SP_Parts d;
	fp_t temp__;
	d.v = NaN();

	d.i[0] = (d.i[0] & UL(0xff800000)) | (count & UL(~0xff800000));

#if defined(FP_ARCH_ALPHA)
	d.i[0] &= UL(~0x00400000);
#elif defined(FP_ARCH_SPARC)
	d.i[0] &= UL(~0x00400000);
#elif defined(FP_ARCH_I860)
	d.i[0] &= UL(~0x00400000);
#elif defined(FP_ARCH_POWER)
	d.i[0] &= UL(~0x00400000);
#elif defined(FP_ARCH_IA64)
	d.i[0] &= UL(~0x00400000);
#elif defined(FP_ARCH_PA_RISC)
	d.i[0] |=  0x00400000;
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v); /* No signaling NaN in the Intel IA-32 architecture */
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);      /* No signaling NaN in the MIPS architecture */
#else  /* fall back to majority style, which could still be wrong */
	d.i[0] &= UL(~0x00400000);
#endif

	return (d.v);
}

#endif /* defined(HAVE_FP_T_SINGLE) */

static double
SNaN64(int count)
{
	DP_Parts d;
	int big_endian = IsBigEndian();
	fp_t temp__;

	d.v = (double)NaN();

	d.i[big_endian] = count;

#if defined(FP_ARCH_ALPHA)
	d.i[1 - big_endian] &= UL(~0x000fffff);
#elif defined(FP_ARCH_SPARC)
	d.i[1 - big_endian] &= UL(~0x000fffff);
#elif defined(FP_ARCH_I860)
	d.i[1 - big_endian] &= UL(~0x000fffff);
#elif defined(FP_ARCH_POWER)
	d.i[1 - big_endian] &= UL(~0x000fffff);
#elif defined(FP_ARCH_IA64)
	d.i[1 - big_endian] &= UL(~0x000fffff);
#elif defined(FP_ARCH_PA_RISC)
	d.i[1 - big_endian] |=  0x000fffff;
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v); /* No signaling NaN in the Intel IA-32 architecture */
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);      /* No signaling NaN in the MIPS architecture */
#else  /* fall back to majority style, which could still be wrong */
	d.i[1 - big_endian] &= UL(~0x000fffff);
#endif
	return (d.v);
}

#if defined(HAVE_FP_T_QUADRUPLE)
static fp_t
SNaN128(int count)
{
	QP_Parts d;
	int big_endian = IsBigEndian();
	fp_t temp__;

	d.v = NaN();

	d.i[3*big_endian] = count;
	d.i[1] = 0;
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		d.i[2] = 0;
	else				/* 80-bit  */
		/* NO-OP */;

#if defined(FP_ARCH_ALPHA)
	d.i[3*(1 - big_endian)] &= UL(~0x0000bfff);
#elif defined(FP_ARCH_SPARC)
	d.i[3*(1 - big_endian)] &= UL(~0x0000bfff);
#elif defined(FP_ARCH_I860)
	d.i[3*(1 - big_endian)] &= UL(~0x0000bfff);
#elif defined(FP_ARCH_POWER)
	d.i[3*(1 - big_endian)] &= UL(~0x0000bfff);
#elif defined(FP_ARCH_IA64)
	d.i[3*(1 - big_endian)] &= UL(~0x0000bfff);
#elif defined(FP_ARCH_PA_RISC)
	d.i[3*(1 - big_endian)] |=  0x0000bfff;
#elif defined(FP_ARCH_IA32)
	(void)STORE(d.v); /* No signaling NaN in the Intel IA-32 architecture */
#elif defined(FP_ARCH_MIPS)
	(void)STORE(d.v);      /* No signaling NaN in the MIPS architecture */
#else  /* fall back to majority style, which could still be wrong */
	d.i[3*(1 - big_endian)] &= UL(~0x0000bfff);
#endif

	return (d.v);
}
#endif /* defined(HAVE_FP_T_QUADRUPLE) */

fp_t
SNaN(void)
{					/* return a distinct SNaN on each call */
	fp_t maybe_NaN = NaN();
	static unsigned int SNaN_count = 0;

	if (IsNaN(maybe_NaN))		/* false on non IEEE 754 systems */
	{
		FP_Parts d;
		int big_endian = IsBigEndian();

		d.v = maybe_NaN;

		/* A NaN must have a nonzero significand, so we
		   choose to create up to 2^23 - 1 unique SNaNs.
		   For the moment, only two are generated by hoc,
		   for the constants SNAN and SNaN. */
		SNaN_count++;
		if (SNaN_count >= (int)0x7fffff) /* then 23-bit counter wrapped */
			SNaN_count = 1;

#if defined(HAVE_FP_T_DOUBLE)
		d.v = SNaN64(SNaN_count);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
			d.v = (fp_t)SNaN64(SNaN_count); /* NB: cast is no-op */
		else
			d.v = SNaN128(SNaN_count);
#endif

#if defined(HAVE_FP_T_SINGLE)
		d.v = SNaN32(SNaN_count);
#endif

		maybe_NaN = d.v;
	}
	return (maybe_NaN);
}

fp_t
Trunc(fp_t x)
{
	fp_t result;

#if defined(HAVE_TRUNC)
	result = trunc(x);
#else
	result = Integer(x);
#endif

	/* Some systems produce incorrect signs from trunc(). */

	if ((result == FP(0.0)) && (x < FP(0.0)))
	{
		result = FP(0.0);
		result = -result;	/* generate negative zero */
	}
	return (result);
}

fp_t
Unordered(fp_t x, fp_t y)
{
	return (IsNaN(x) || IsNaN(y));
}
