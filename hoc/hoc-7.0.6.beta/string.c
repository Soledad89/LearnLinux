#include "hoc.h"

#if defined(HAVE_CTYPE_H)
#include <ctype.h>			/* for isxxx() prototypes */
#endif

#if defined(HAVE_MATH_H)
#include <math.h>			/* for ldexp() prototype */
#endif

#if defined(HAVE_STDIO_H)
#include <stdio.h>			/* for sprintf()/snprintf() prototypes */
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#define elementsof(x)	(sizeof(x)/sizeof(x[0]))

extern char **environ;			/* defined in run-time library */
EXTERN int cmpstr ARGS((const void *, const void *));

static unsigned int	htou ARGS((const char *));
static const char *	xinttos ARGS((fp_t));

const char *
Abort(const char *s)
{
	execerror("execution aborted:", s);
	return (dupstr(""));		/* NOT REACHED */
}

const char *
concat2(const char *s, const char *t)
{
	if (s == (const char *)NULL)
		s = "";
	if (t == (const char *)NULL)
		t = "";
	return ((const char*)strcat(strcpy((char*)emalloc(strlen(s) + strlen(t) + 1), s), t));
}

const char *
concat3(const char *s, const char *t, const char *u)
{
	if (s == (const char *)NULL)
		s = "";
	if (t == (const char *)NULL)
		t = "";
	if (u == (const char *)NULL)
		u = "";
	return ((const char*)strcat(strcat(strcpy((char*)emalloc(strlen(s) + strlen(t) + strlen(u) + 1), s), t), u));
}

const char *
concat4(const char *s, const char *t, const char *u, const char *v)
{
	if (s == (const char *)NULL)
		s = "";
	if (t == (const char *)NULL)
		t = "";
	if (u == (const char *)NULL)
		u = "";
	if (v == (const char *)NULL)
		v = "";
	return ((const char*)strcat(strcat(strcat(strcpy((char*)emalloc(strlen(s) + strlen(t) + strlen(u) + strlen(v) + 1), s), t), u), v));
}

const char *
concat5(const char *s, const char *t, const char *u, const char *v, const char *w)
{
	if (s == (const char *)NULL)
		s = "";
	if (t == (const char *)NULL)
		t = "";
	if (u == (const char *)NULL)
		u = "";
	if (v == (const char *)NULL)
		v = "";
	if (w == (const char *)NULL)
		w = "";
	return ((const char*)strcat(strcat(strcat(strcat(strcpy((char*)emalloc(strlen(s) + strlen(t) + strlen(u) + strlen(v) + strlen(w) + 1), s), t), u), v), w));
}

const char *
concat6(const char *s, const char *t, const char *u, const char *v, const char *w, const char *x)
{
	if (s == (const char *)NULL)
		s = "";
	if (t == (const char *)NULL)
		t = "";
	if (u == (const char *)NULL)
		u = "";
	if (v == (const char *)NULL)
		v = "";
	if (w == (const char *)NULL)
		w = "";
	if (x == (const char *)NULL)
		x = "";
	return ((const char*)strcat(strcat(strcat(strcat(strcat(strcpy((char*)emalloc(strlen(s) + strlen(t) + strlen(u) + strlen(v) + strlen(w) + strlen(x) + 1), s), t), u), v), w), x));
}

int
decval(int c)
{	/* return the decimal value of c, or 01 if not decimal */
	/* All current characters sets (ASCII, EBCDIC, ISO8859-n,
	   ISO10646, Unicode) have consecutive decimal digits, so this
	   code is portable. */
	if (('0' <= c) && (c <= '9'))
		return ((int)(c - '0'));
	else
		return (-1);
}

const char *
Char(fp_t n)
{
	char s[2] = {'\0', '\0'};
	/* NB: (char)(unsigned char)n produced incorrect results on
	   IBM AIX 4.2 with g++ 2.95.3; this rewrite fixes the problem. */
	s[0] = (char)(0xff & (int)n);
	return (dupstr(s));
}

const char *
Exit(const char *reason)
{
	if ((reason != (const char*)NULL) && (*reason != '\0'))
	{
		warning(reason, (const char*)NULL);
		exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_SUCCESS);
	return (dupstr(""));		/* NOT REACHED */
}

const char *
Expand(const char *s)
{	/* Return a dynamic copy of s with escape sequences expanded */
	char *p;
	char *t;

	t = (char*)dupstr(s);
	for (p = t; *s != '\0'; p++, s++)
	{
		if (*s == '\\')
		{
			switch (*++s)
			{		/* abfnrtvE */
			case 'a': *p = '\007'; break;	/* NB: use octal value for old K&R compilers */
			case 'b': *p = '\b'; break;
			case 'f': *p = '\f'; break;
			case 'n': *p = '\n'; break;
			case 'r': *p = '\r'; break;
			case 't': *p = '\t'; break;
			case 'v': *p = '\v'; break;
			case 'E': *p = '\033'; break;
			default: *p = *s; break;
			}
		}
		else
			*p = *s;
	}
	*p = '\0';
	return ((const char*)t);
}

const char *
FtoH(fp_t x)
{	/* return dynamic copy of native floating-point converted to hexadecimal */
	char buffer[sizeof("XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX")];
	FP_Parts d;
	int big_endian = IsBigEndian();

	d.v = x;

#if defined(HAVE_FP_T_SINGLE)
	SNPRINTF4(buffer, sizeof(buffer), "%08x", d.i[0]);
#endif

#if defined(HAVE_FP_T_DOUBLE)
	SNPRINTF5(buffer, sizeof(buffer), "%08x_%08x",
		  d.i[1-big_endian], d.i[big_endian]);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
	{
		if (big_endian)	/* 128-bit big endian */
			SNPRINTF7(buffer, sizeof(buffer), "%08x_%08x_%08x_%08x",
					d.i[0], d.i[1], d.i[2], d.i[3]);
		else		/* 128-bit little endian */
			SNPRINTF7(buffer, sizeof(buffer), "%08x_%08x_%08x_%08x",
					d.i[3], d.i[2], d.i[1], d.i[0]);
	}
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
	{
		SNPRINTF5(buffer, sizeof(buffer), "%08x_%08x",
			  d.i[1-big_endian], d.i[big_endian]);
	}
	else				/* 80-bit Intel x86 format */
	{
		SNPRINTF6(buffer, sizeof(buffer), "%04x_%08x_%08x",
				(d.i[2] & 0xffff), d.i[1], d.i[0]);
	}
#endif

	return (dupstr(buffer));
}

const char *
FtoS(fp_t x)
{	/* return dynamic copy of floating-point converted to a string */
	return (dupstr(fmtnum(x)));
}

const char *
Getenv(const char *envvar)
{	/* Return the value of environment variable envvar, as a new
	   string (empty if envvar was not defined, or was empty).
	   The dupstr() call is necessary because at least two UNIX
	   platforms (Apple Darwin (MacOS X) and FreeBSD) use an
	   overwrite-in-place-when-possible algorithm in putenv(). */
	const char *p;

	p = (const char *)getenv(envvar);
	if (p == (const char *)NULL)
		p = "";
	return (dupstr(p));
}

const char *
Hex(fp_t x)
{	/* return dynamic copy of floating-point converted to native
	   hexadecimal, C99 hexadecimal, and decimal */
	char buffer[sizeof("0xhhhhhhhh_hhhhhhhh_hhhhhhhh_hhhhhhhh")];
	FP_Parts d;
	int big_endian = IsBigEndian();

	d.v = x;

#if defined(HAVE_FP_T_DOUBLE)
	SNPRINTF5(buffer, sizeof(buffer), "0x%08x_%08x", d.i[1 - big_endian], d.i[big_endian]);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v)) /* 128-bit */
	{
		if (big_endian)		/* 128-bit big endian */
			SNPRINTF7(buffer, sizeof(buffer), "0x%08x_%08x_%08x_%08x",
				  d.i[0], d.i[1], d.i[2], d.i[3]);
		else			/* 128-bit little endian */
			SNPRINTF7(buffer, sizeof(buffer), "0x%08x_%08x_%08x_%08x",
				  d.i[3], d.i[2], d.i[1], d.i[0]);
	}
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		SNPRINTF5(buffer, sizeof(buffer), "0x%08x_%08x",
			  d.i[1 - big_endian], d.i[big_endian]);
	else				/* 80-bit little endian */
		SNPRINTF6(buffer, sizeof(buffer), "0x%04x_%08x_%08x",
			  (0xffff & d.i[2]), d.i[1], d.i[0]);
#endif

#if defined(HAVE_FP_T_SINGLE)
	SNPRINTF4(buffer, sizeof(buffer), "0x%08x", d.i[0]);
#endif

	return (concat5(buffer," == ",xfptos(x)," ",fmtnum(x)));
}

const char *
Hexfp(fp_t x)
{	/* return dynamic copy of floating-point converted to C99 hexadecimal */
	return (dupstr(xfptos(x)));
}

const char *
Hexint(fp_t x)
{	/* return dynamic copy of floating-point converted to C hexadecimal integer */
	return (dupstr(xinttos(x)));
}

int
hexval(int c)
{	/* return the hexadecimal value of c, or -1 if not hexadecimal */
	/* All current characters sets (ASCII, EBCDIC, ISO8859-n,
	   ISO10646, Unicode) have consecutive decimal digits and
	   consecutive letters A-F, so this code is portable. */
	if (('0' <= c) && (c <= '9'))
		return ((int)(c - '0'));
	else if (('a' <= c) && (c <= 'f'))
		return ((int)((c - 'a') + 10));
	else if (('A' <= c) && (c <= 'F'))
		return ((int)((c - 'A') + 10));
	else
		return (-1);
}

fp_t
HtoF(const char *s)
{
	char *p;
	char *t;
	FP_Parts d;
	int big_endian = IsBigEndian();

	d.v = FP(0.0);

	t = (char*)dupstr(s);

	for (p = t; *s; ++s)
	{
		if (isxdigit(*s))
			*p++ = *s;	/* copy only hexadecimal digits */
	}
	*p = '\0';			/* terminate string */

#if defined(HAVE_FP_T_SINGLE)
	if (strlen(t) == 8)
	{
		d.i[0] = htou(t);
	}
	else
		execerror("illegal native hexadecimal string in htof()", (const char*)NULL);
#endif

#if defined(HAVE_FP_T_DOUBLE)
	if (strlen(t) == 16)
	{
		d.i[big_endian] = htou(t+8);
		t[8] = '\0';
		d.i[1-big_endian] = htou(t);
	}
	else
		execerror("illegal native hexadecimal string in htof()", (const char*)NULL);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	if (sizeof(d) == sizeof(d.v))	/* 128-bit */
	{
		if (strlen(t) == 32)
		{
			if (big_endian)
			{
				d.i[3] = htou(t+24);
				t[24] = '\0';
				d.i[2] = htou(t+16);
				t[16] = '\0';
				d.i[1] = htou(t+8);
				t[8] = '\0';
				d.i[0] = htou(t+0);
			}
			else
			{
				d.i[0] = htou(t+24);
				t[24] = '\0';
				d.i[1] = htou(t+16);
				t[16] = '\0';
				d.i[2] = htou(t+8);
				t[8] = '\0';
				d.i[3] = htou(t+0);
			}
		}
		else
			execerror("illegal native hexadecimal string in htof()", (const char*)NULL);
	}
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
	{
		if (strlen(t) == 16)
		{
			d.i[big_endian] = htou(t+8);
			t[8] = '\0';
			d.i[1-big_endian] = htou(t);
		}
		else
			execerror("illegal native hexadecimal string in htof()", (const char*)NULL);
	}
	else				/* 80-bit Intel x86 format */
	{
		if (strlen(t) == 20)
		{
			d.i[0] = htou(t+12);
			t[12] = '\0';
			d.i[1] = htou(t+4);
			t[4] = '\0';
			d.i[2] = htou(t+0);
		}
		else
			execerror("illegal native hexadecimal string in htof()", (const char*)NULL);
	}
#endif

	efree((void*)t);

	return (d.v);
}


static unsigned int
htou(const char *s)			/* hexadecimal to unsigned integer */
{
	unsigned int h;
	unsigned int u;

	u = (unsigned int)0;
	while (*s)
	{
		if (('0' <= *s) && (*s <= '9'))
			h = *s - '0';
		else if (('a' <= *s) && (*s <= 'f'))
			h = (*s - 'a') + 10;
		else if (('A' <= *s) && (*s <= 'F'))
			h = (*s - 'A') + 10;
		u = (u << 4) | h;
		s++;
	}

	return (u);
}


fp_t
Ichar(const char *s)
{
	return ((fp_t)(unsigned char)s[0]); /* guarantee Ichar(s) >= 0 */
}

fp_t
Index(const char *s, const char *t)
{	/* return 1-based index of substring t in string s, or 0 if not found */
	const char *p;

	p = strstr(s,t);
	return ((fp_t)((p == (const char*)NULL) ? 0 : ((p - s) + 1)));
}

const char *
Inf_string(fp_t x)
{
	return ((x < 0) ? "-Inf" : "+Inf");
}

fp_t
Length(const char *s)
{
	return ((fp_t)strlen(s));
}

const char *
NaN_string(fp_t x)
{
	if (IsSNaN(x) && !IsQNaN(x))
		return ((Copysign(FP(1.0),x) * Copysign(FP(1.0),NaN()) == FP(-1.0)) ?
			"-SNaN" : "+SNaN");
	else if (IsQNaN(x) && !IsSNaN(x))
		return ((Copysign(FP(1.0),x) * Copysign(FP(1.0),NaN()) == FP(-1.0)) ?
			"-QNaN" : "+QNaN");
	else if (IsNaN(x))
		return ((Copysign(FP(1.0),x) * Copysign(FP(1.0),NaN()) == FP(-1.0)) ?
			"-NaN" : "+NaN");
	else
		return ((const char *)NULL);
}

const char *
Printenv(const char *pattern)
{
	char *equals;
	int len;
#if 0
	size_t len_pattern;
#endif
	int max_name_width;
	size_t n;
	char **p;
	char **sorted_environ;

	if (pattern == (const char *)NULL)
		pattern = "";
#if 0
	len_pattern = strlen(pattern);
#endif

	max_name_width = 0;
	for (n = 0, p = environ; (*p != (char *)NULL); ++p)
	{
		++n;			/* find out how many we need */
		equals = strchr(*p,'=');
		*equals = '\0';
#if 0
		if (strncmp((const char *)*p,pattern,len_pattern) == 0)
#else
		if (match((const char *)*p,pattern))
#endif
		{
#if 0
			len = (int)(strchr(*p,'=') - *p);
#else
			len = (int)strlen(*p);
#endif
			if (max_name_width < len)
				max_name_width = len;
		}
		*equals = '=';
	}

	sorted_environ = (char **)emalloc((n + 1) * sizeof(char *));
	for (n = 0, p = environ; (*p != (char *)NULL); ++p)
		sorted_environ[n++] = *p;
	sorted_environ[n] = (char *)NULL;

	qsort(sorted_environ, n, sizeof(char *), cmpstr);

	for (p = sorted_environ; (*p != (char *)NULL); ++p)
	{
#if 0
		if (strncmp(*p,pattern,len_pattern) == 0)
#else
		equals = strchr(*p,'=');
		*equals = '\0';
		if (match(*p,pattern))
#endif
		{
			prtext2(*p, max_name_width);
			prtext("\t= \"");
			prtext(equals + 1);
			prtext("\"");
			prnl();
		}
		*equals = '=';
	}

	efree((void*)sorted_environ);

	return (dupstr(""));
}

const char *
Protect(const char *s)
{	/* return a copy of s with all nonprintable characters protected */
	char *protected_s;
	char *p;
	const char *q;
	static const char inputtab[]  = "\007\b\f\n\r\t\v\033";	/* NB: use \007, not \a, for old K&R compilers */
	static const char outputtab[] = "abfnrtvE";
	size_t len_protected_s;

	len_protected_s = 4*strlen(s) + 1;
	protected_s = (char*)emalloc(len_protected_s); /* for \ooo worst-case expansion */
	for (p = protected_s; *s; ++s)
	{
		if ((q = strchr(inputtab,*s), q) != (const char*)NULL)
		{
			SNPRINTF4(p, len_protected_s - (p - protected_s),
					"\\%c", (unsigned int)UC(outputtab[q-inputtab]));
			p += 2;
		}
		else if ((*s == '\\') || (*s == '"'))
		{
			SNPRINTF4(p, len_protected_s - (p - protected_s),
					"\\%c", (unsigned int)UC(*s));
			p += 2;
		}
		else if (isprint(*s))
		{
			if ((p - protected_s) < len_protected_s)
				*p++ = *s;
		}
		else
		{
			SNPRINTF4(p, len_protected_s - (p - protected_s),
					"\\%03o", (unsigned int)UC(*s));
			p += 4;
		}
	}
	*p = '\0';
	return ((const char*)protected_s);
}

const char *
Putenv(const char *envvar, const char *newval)
{
	(void)putenv((char*)concat3(envvar,"=",newval));
	return (Getenv(envvar));	/* NB: NOT getenv()! ... see Getenv() comments */
}

fp_t
StoF(const char *s)
{
	return (strton(s, (char**)NULL));
}

#if defined(HAVE_FP_T_QUADRUPLE) && !defined(HAVE_STRTOLD)
long double
strtold(const char * /* restrict */ nptr, char ** /* restrict */ endptr)
{
	long double result;

	/* Sun Solaris 2.8 has no strtold(), but sscanf() works for long
	   double arguments.  Perhaps this mapping will work on other
	   systems as well. */
	if (sscanf(nptr, "%Lg", &result) != 1)
	{
		perror("strtold()->sscanf() failed");
		result = QNaN();
	}
	if (endptr != (char**)NULL)
		*endptr = (char*)strchr(nptr,'\0'); /* pretend that the parse succeeded */
	return (result);
}
#endif

fp_t
strton(const char *s, char **endptr)
{					/* string to number (generalized strtod()) */
	const char *t;
	fp_t result;
	int sign;

	while (isspace(*s))		/* skip leading space */
		++s;

	if (*s == '-')
	{
		s++;
		sign = -1;
	}
	else if (*s == '+')
	{
		s++;
		sign = 1;
	}
	else
		sign = 1;

	t = Tolower(s);

	if (endptr != (char**)NULL)
		*endptr = (char*)strchr(s,'\0');

	/* C99 allows "NAN(n-char-sequence)" (and SGI IRIX already
	produces this in C89), but we don't check for a close
	parenthesis: C99 leaves it implementation dependent whether
	the parentheses are balanced. */

	if (STREQUAL(t,"nan")       || (strncmp(t,"nan(",4) == 0))
		result = NaN();
	else if (STREQUAL(t,"qnan") || (strncmp(t,"qnan(",5) == 0))
		result = QNaN();
	else if (STREQUAL(t,"snan") || (strncmp(t,"snan(",5) == 0))
		result = SNaN();
	else if (STREQUAL(t,"inf")  || STREQUAL(t,"infinity"))
		result = Infinity();
	else
	{
		char *t_endptr;

		/* Try the hexadecimal form first, and if that fails, use
		   whatever native strtod() returns */
		result = xstrtod(t, &t_endptr);

#if defined(HAVE_FP_T_QUADRUPLE)
		if (*t_endptr != '\0')
			result = (fp_t)strtold(t, &t_endptr);
#else
		if (*t_endptr != '\0')
			result = (fp_t)strtod(t, &t_endptr);
#endif

		if (endptr != (char**)NULL)
			*endptr = (char*)(s + (t_endptr - t));
	}
	efree((void*)t);
	if (sign < 0)
		result = -result;
	return (result);
}

const char *
Substr(const char *s, fp_t f_first, fp_t f_length)
{	/* extract (string,first,length) to new dynamic substring, with 1-based indexing */
	size_t first;
	size_t len_sub;
	size_t len_str;
	
	len_str = strlen(s);
	len_sub = (f_length < FP(0.0)) ? (size_t)0 : (size_t)f_length;
	first = (f_first <= FP(1.0)) ? 1 : ((size_t)f_first);
	first--;			/* convert to C/C++ 0-based index */
	if (first > len_str)
		first = len_str;	/* index of final NUL */

	if ((first + len_sub) > len_str)
		len_sub = len_str - first;

	if (len_str > 0)
	{
		char *p;

		p = (char*)emalloc((len_sub + 1)*sizeof(char));
		(void)strlcpy(p,&s[first],len_sub + 1);
		p[len_sub] = '\0';
		return (p);
	}
	else
		return(dupstr(""));
}


const char *
Tolower(const char *s)
{	/* return a dynamic copy of s, converted to lowercase */
	char *t;
	char *p;

	t = (char *)dupstr(s);
	for (p = t; *p; ++p)
		*p = (isupper(UC(*p)) ? tolower(UC(*p)) : *p);

	return (t);
}

const char *
Toupper(const char *s)
{	/* return a dynamic copy of s, converted to uppercase */
	char *t;
	char *p;

	t = (char *)dupstr(s);
	for (p = t; *p; ++p)
		*p = (islower(UC(*p)) ? toupper(UC(*p)) : *p);

	return (t);
}

const char *
xfptos(fp_t x)
{
	/* Convert a floating-point value to a hexadecimal floating-point
	   string, returning a pointer to a static internal buffer which will be
	   OVERWRITTEN on the next call.  The conversion is defined by the
	   description of the fprintf() %A and %a format items in

		``ISO/IEC 9899:1999 (E) Programming languages -- C'', Section
		7.19.6.1, p. 278:

	``a,A	A double argument representing a floating-point number is
		converted in the style [-]0xh.hhhhp+-d, where there is one
		hexadecimal digit (which is nonzero if the argument is a
		normalized floating-point number and is otherwise unspecified)
		before the decimal-point character
			[Footnote 235: Binary implementations can choose the
			hexadecimal digit to the left of the decimal-point
			character so that subsequent digits align to nibble
			(4-bit) boundaries.]
		and the number of hexadecimal digits after it is equal to the
		precision; if the precision is missing and FLT_RADIX is a power
		of 2, then the precision is sufficient for an exact
		representation of the value; if the precision is missing and
		FLT_RADIX is not a power of 2, then the precision is sufficient
		to distinguish
			[Footnote 236: The precision p is sufficient to
			distinguish values of the source type if 16^(p-1) > b^n
			where b is FLT_RADIX and n is the number of base-b
			digits in the significand of the source type. A smaller
			p might suffice depending on the implementation's scheme
			for determining the digit to the left of the
			decimal-point character.]
		values of type double, except that trailing zeros may be
		omitted; if the precision is zero and the # flag is not
		specified, no decimal point character appears. The letters
		abcdef are used for %a conversion and the letters ABCDEF for %A
		conversion. The A conversion specifier produces a number with X
		and P instead of x and p. The exponent always contains at least
		one digit, and only as many more digits as necessary to
		represent the decimal exponent of 2. If the value is zero, the
		exponent is zero.

		A double argument representing an infinity or NaN is converted
		in the style of an f or F conversion specifier.

		... ''

	   In this implementation, we choose to trim trailing zeros, leaving at
	   least one digit following the hexadecimal point.   We also make a
	   distinction between quiet and signalling NaNs, and include the
	   sign of the NaN. */

#if defined(HAVE_IEEE_754)
	/* This implementation requires knowledge of low-level formatting of
	   IEEE 754 64-bit floating-point data:

	bit:	6666555555555544444444443333333333222222222211111111110000000000
		3210987654321098765432109876543210987654321098765432109876543210
	data:	seeeeeeeeeeeffffffffffffffffffffffffffffffffffffffffffffffffffff
		|           ----------------------------------------------------
		|-----------                         |--significand
		|    |--exponent
		|--sign

	The binary point occurs before the significand, after the hidden bit,
	which is nonzero if the number is normalized, and zero otherwise. */

	static char s[sizeof("-0xd.ffffffffffffffffffffffffffffp+ddddd")];
	fp_t signif;
	FP_Parts d;
	int expon;

	int big_endian = IsBigEndian();

	if (IsNaN(x))
		return (NaN_string(x));
	else if (IsInf(x))
		return (Inf_string(x));
	else if (x == FP(0.0))
		return ("0");
	else
	{
		char *last;
		signif = Significand(x);
		expon = (int)Exponent(x);
		d.v = signif;

#if defined(HAVE_FP_T_DOUBLE)
		{
			unsigned int hi, lo;

			hi = d.i[1-big_endian];
			lo = d.i[big_endian];

			SNPRINTF7(s, sizeof(s), "%s0x%d.%05x%08x",
					((x < FP(0.0)) ? "-" : "+"),
					1, /* (int)IsNormal(x), */
					(0x000fffff & hi),
					lo);
		}
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		if (sizeof(d) == sizeof(d.v))	/* 128-bit */
		{
			if (big_endian)	/* 128-bit big endian */
				SNPRINTF9(s, sizeof(s), "%s0x%d.%04x%08x%08x%08x",
						((x < FP(0.0)) ? "-" : "+"),
						1, /* (int)IsNormal(x), */
						(0x0000ffff & d.i[0]),
						d.i[1], d.i[2], d.i[3]);
			else		/* 128-bit little endian */
				SNPRINTF9(s, sizeof(s), "%s0x%d.%04x%08x%08x%08x",
						((x < FP(0.0)) ? "-" : "+"),
						1, /* (int)IsNormal(x), */
						(0x0000ffff & d.i[3]),
						d.i[2], d.i[1], d.i[0]);
		}
		else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
		{
			unsigned int hi, lo;

			hi = d.i[1-big_endian];
			lo = d.i[big_endian];

			SNPRINTF7(s, sizeof(s), "%s0x%d.%05x%08x",
					((x < FP(0.0)) ? "-" : "+"),
					1, /* (int)IsNormal(x), */
					(0x000fffff & hi),
					lo);
		}
		else				/* 80-bit Intel x86 format */
		{
			SNPRINTF7(s, sizeof(s), "%s0x%d.%08x%08x",
					((x < FP(0.0)) ? "-" : "+"),
					(d.i[1] >> 31),
					((d.i[1] << 1) | (d.i[0] >> 31)),
					(d.i[0] << 1));
		}
#endif

#if defined(HAVE_FP_T_SINGLE)
		SNPRINTF6(s, sizeof(s), "%s0x%d.%06x",
				((x < FP(0.0)) ? "-" : "+"),
				1, /* (int)IsNormal(x), */
				((0x007fffff & d.i[0]) << 1));
#endif

		/* Trim trailing zeros */
		last = strchr(s,'\0') - 1;
		while ((last[0] == '0') && (last[-1] != '.'))
			last--;

		SNPRINTF5(last + 1, (sizeof(s) - (last + 1 - s)),
				"p%s%d",
				((expon < 0) ? "-" : "+"),
				((expon < 0) ? -expon : expon));
	}
#else /* NOT defined(HAVE_IEEE_754) */
	/* cannot fiddle bits without further details, so fall back to decimal output */
	static char s[sizeof("-1.234567890123456789012345678901234567890E+dddd")];
#if defined(HAVE_FP_T_QUADRUPLE)
	SNPRINTF5(s, sizeof(s), "%.*Lg", default_precision(), (fp_t)x);
#else
	SNPRINTF5(s, sizeof(s), "%.*g", default_precision(), (double)x);
#endif
#endif /* defined(HAVE_IEEE_754) */

	return ((const char*)&s[0]);
}

static const char *
xinttos(fp_t x)
{
	/* Convert x to a hexadecimal integer, if this is exactly
	   representable, and otherwise, to a hexadecimal
	   floating-point value. */

	/* TO DO: Reimplement this code without requiring an integer
	   data type; that will allow increasing maxint to 2^23 - 1
	   (32-bit), 2^53 - 1 (64-bit), 2^64 - 1 (80-bit), and 2^113 - 1
	   (128-bit).  This increases maxint only for the 128-bit
	   case. */

#if defined(HAVE_IEEE_754)

	UNSIGNED_LONG_LONG n;
	fp_t xabs;

	static fp_t maxint = FP(0.0);

	if (maxint == FP(0.0))	      /* happens only on first call */
	{

#if defined(HAVE_LONG_LONG)

#if defined(HAVE_FP_T_DOUBLE)
		maxint = FP(9007199254740991.0); /* 2^53 - 1 */
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
		maxint = FP(18446744073709551615.0); /* 2^64 - 1 */
#endif

#if defined(HAVE_FP_T_SINGLE)
		maxint = FP(16777215.0);	/* 2^24 - 1 */
#endif

#else /* NOT defined(HAVE_LONG_LONG) */

#if defined(HAVE_FP_T_SINGLE)
		maxint = FP(16777215.0);	/* 2^24 - 1 */
#else
		maxint = FP(4294967295.0);	/* 2^32 - 1 */
#endif

#endif /* defined(HAVE_LONG_LONG) */

	}

	if (IsNaN(x))
		return (xfptos(x));
	else if (IsInf(x))
		return (xfptos(x));
	else if ((x < -maxint) || (maxint < x))
		return (xfptos(x));	/* not representable as integer */
	else
	{
		static char buffer[sizeof("-0xhhhhhhhhhhhhhhhh")]; /* space for 64-bit value */
		char *p;

		xabs = (x < 0) ? -x : x;
		n = (UNSIGNED_LONG_LONG)xabs;	/* exact conversion */
		p = &buffer[elementsof(buffer)-1];
		*p-- = '\0';
		while (p >= &buffer[2])	/* leave room for "-0x" */
		{
			static const char hexchars[] = "0123456789abcdef";

			*p-- = hexchars[n & 0xf];
			n >>= 4;
			if (n == 0)
				break;
		}
		*p-- = 'x';
		*p-- = '0';
		if (x < 0)
			*p = '-';
		else
			*p = '+';
		return ((const char*)p);
	}
#else	/* NOT defined(HAVE_IEEE_754) */
	/* Use fallback of hexadecimal floating-point for now, but
	   ultimately, special case code is needed here for other
	   floating-point systems. */
	return (xfptos(x));
#endif	/* defined(HAVE_IEEE_754) */

}

#define XSTRTOD_FAILURE (FP(0.0))	/* mandated by C89 and C99 (*endptr disambiguates) */

fp_t
xstrtod(const char* s, char **endptr)
{	/* Convert C99 hexadecimal floating-point string
		   "[-+]?0[Xx][0-9A-Fa-z]*(.[0-9A-Fa-z])?[Pp][-+]?[0-9]+"
	   to floating-point. */
	fp_t d;
	int digit;
	int binary_power;
	int nfract;
	int nsign;

	while (isspace(*s))
		++s;
	if (*s == '-')
	{
		s++;
		nsign = -1;
	}
	else if (*s == '+')
	{
		s++;
		nsign = 1;
	}
	else
		nsign = 1;

	if (strncmp(s,"0x",2) != 0)
	{
		if (endptr != (char**)NULL)
			*endptr = (char*)s;
		return (XSTRTOD_FAILURE);
	}
	s += 2;
	for (d = FP(0.0); (digit = hexval(*s), digit) >= 0; ++s)
		d = FP(16.0) * d + (fp_t)digit;
	nfract = 0;
	if (*s == '.')
	{
		s++;
		for ( ; (digit = hexval(*s), digit) >= 0; ++s, ++nfract)
			d = FP(16.0) * d + (fp_t)digit;
	}
	binary_power = 0;
	if ((*s == 'p') || (*s == 'P'))
	{
		int exponent_sign;
		int n_exponent_digits;

		++s;
		if (*s == '-')
		{
			s++;
			exponent_sign = -1;
		}
		else if (*s == '+')
		{
			s++;
			exponent_sign = 1;
		}
		else
			exponent_sign = 1;

		for (n_exponent_digits = 0; (digit = decval(*s), digit) >= 0; ++s, n_exponent_digits++)
			binary_power = 10 * binary_power + digit;
		if (n_exponent_digits == 0)
		{
			if (endptr != (char**)NULL)
				*endptr = (char*)s;
			return (XSTRTOD_FAILURE);
		}
		if (exponent_sign < 0)
			binary_power = -binary_power;
	}
	d = ldexp(d, -4 * nfract + binary_power);
	if (endptr != (char**)NULL)
		*endptr = (char*)s;
	return ((nsign < 0) ? -d : d);
}
