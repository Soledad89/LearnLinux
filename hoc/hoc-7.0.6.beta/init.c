#include "hoc.h"

#if defined(HAVE_FLOAT_H)
#include <float.h>
#endif

#if defined(HAVE_MATH_H)
#include <math.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#if defined(HAVE_TIME_H)
#include <time.h>
#endif

#include "xtab.h"

fp_t	BASE = FP(0.0);			/* reset in init_fp_globals() */
fp_t	EBIAS = FP(0.0);		/* reset in init_fp_globals() */
fp_t	EBITS = FP(0.0);		/* reset in init_fp_globals() */
fp_t	EMAX = FP(0.0);			/* reset in init_fp_globals() */
fp_t	EMIN = FP(0.0);			/* reset in init_fp_globals() */
fp_t	P = FP(0.0);			/* reset in init_fp_globals() */

static void	init_consts ARGS((void));
static void	init_fp_globals ARGS((void));

static struct {		/* Keywords */
	const char	*name;
	int	kval;
} keywords[] = { /* NB: The kvals in this list MUST match that in the keyword rule in hoc.y! */
	"proc",		PROC,
	"func",		FUNC,
	"return",	RETURN,
	"if",		IF,
	"else",		ELSE,
	"while",	WHILE,
	"for",		FOR,
	"print",	PRINT,
	"read",		READ,
	"break",	BREAK,		/* new */
	"continue",	CONTINUE,	/* new */
	"global",	GLOBAL,		/* new */
	"printf",	PRINTF,		/* new */
	"println",	PRINTLN,	/* new */
	"sprintf",	SPRINTF,	/* new */

	(const char*)NULL, 0,
};

static struct {		/* Constants */
	const char *name;
	fp_t cval;
	int immutable;
} consts[] = {
	"PI",		FP(3.141592653589793238462643383279502884197e+00), 1,
	"E",		FP(2.718281828459045235360287471352662497757e+00), 1,
	"GAMMA",	FP(0.5772156649015328606065120900824024310422e+00), 1,	/* Euler */
	"DEG",		FP(57.29577951308232087679815481410517033240e+00), 1,	/* deg/radian */
	"PHI",		FP(1.618033988749894848204586834365638117720e+00), 1,  	/* golden ratio */
	"PREC",	17, 0,			/* output precision (reset at run time) */

	/* New constants added for hoc version 7 */

	"CATALAN",	FP(0.9159655941772190150546035149323841107741e+00), 1, /* Catalan's constant */

	"CLASS_NEGINF",		CLASS_NEGINF,		1,
	"CLASS_NEGNORMAL",	CLASS_NEGNORMAL,	1,
	"CLASS_NEGSUBNORMAL",	CLASS_NEGSUBNORMAL,	1,
	"CLASS_NEGZERO",	CLASS_NEGZERO,		1,
	"CLASS_POSINF",		CLASS_POSINF,		1,
	"CLASS_POSNORMAL",	CLASS_POSNORMAL,	1,
	"CLASS_POSSUBNORMAL",	CLASS_POSSUBNORMAL,	1,
	"CLASS_POSZERO",	CLASS_POSZERO,		1,
	"CLASS_QNAN",		CLASS_QNAN,		1,
	"CLASS_SNAN",		CLASS_SNAN,		1,
	"CLASS_UNKNOWN",	CLASS_UNKNOWN,		1,

	"BASE",		FP(0.0), 1,		/* reset at run time by init_fp_globals() */
	"EBIAS",	FP(0.0), 1,		/* reset at run time by init_fp_globals() */
	"EBITS",	FP(0.0), 1,		/* reset at run time by init_fp_globals() */
	"EMAX",		FP(0.0), 1,		/* reset at run time by init_fp_globals() */
	"EMIN",		FP(0.0), 1,		/* reset at run time by init_fp_globals() */
	"P",		FP(0.0), 1,		/* reset at run time by init_fp_globals() */

	"INF",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */
	"Inf",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */
	"Infinity",	FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */

	"NAN",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */
	"NaN",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */

	"QNAN",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */
	"QNaN",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */

	"SNAN",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */
	"SNaN",		FP(0.0), 1,		/* reset at run time if HAVE_IEEE_754 */

	"MAXNORMAL",	FP(0.0), 1,		/* reset at run time */
	"MAXSUBNORMAL",	FP(0.0), 1,		/* reset at run time */
	"MINNORMAL",	FP(0.0), 1,		/* reset at run time */
	"MINSUBNORMAL",	FP(0.0), 1,		/* reset at run time */

	(const char*)NULL,	 FP(0.0), 0,
};

static struct {		/* Numeric built-ins with zero arguments */
	const char *name;
	F0_t	func;
} builtins_0[] = {
	"rand",		Rand,
	"second",	Second,
	"snan",		SNaN,
	"systime",	Systime,

	(const char*)NULL, (F0_t)NULL,
};


static struct {		/* Numeric built-ins with one argument */
	const char *name;
	F1_t	func;
} builtins_1[] = {
	/* Please keep these groups sorted alphabetically, ignoring lettercase */
	/* Functions from Standard C */
	"abs",		Abs,
	"acos",		Acos,		/* checks range */
	"asin",		Asin,		/* checks range */
	"atan",		Atan,
	"ceil",		Ceil,
	"cos",		Cos,
	"cosh",		Cosh,		/* checks range */
	"exp",		Exp,		/* checks range */
	"floor",	Floor,
	"gamma",	Gamma,		/* checks range */
	"int",		Integer,
	"ln",		Log,		/* checks range */
	"log",		Log,		/* checks range */
	"nint",		Nint,
	"sin",		Sin,
	"sinh",		Sinh,		/* checks range */
	"sqrt",		Sqrt,		/* checks range */
	"tan",		Tan,
	"tanh",		Tanh,

	/* Nonstandard, but commonly available, functions */

	"erf",		Erf,
	"erfc",		Erfc,

	/* Additional functions */

	"acosh",	Acosh,
	"asinh",	Asinh,
	"atanh",	Atanh,
	"cbrt",		Cbrt,
	"class",	Class,
	"cpulimit",	CPULimit,
	"double",	Double,
	"expm1",	Expm1,
	"exponent",	Exponent,
	"ilogb",	Ilogb,
	"isfinite",	IsFinite,
	"isinf",	IsInf,
	"isnan",	IsNaN,
	"isnormal",	IsNormal,
	"isqnan",	IsQNaN,
	"issnan",	IsSNaN,
	"issubnormal",	IsSubnormal,
	"J0",		J0,
	"J1",		J1,
	"lgamma", 	Lgamma,
	"log10",	Log10,		/* checks range */
	"log1p",	Log1p,
	"log2",		Log2,		/* checks range */
	"macheps",	Macheps,
	"randl",	Randl,
	"rint",		Rint,
	"rsqrt",	Rsqrt,
	"setrand",	SetRandSeed,
	"significand",	Significand,
	"single",	Single,
	"trunc",	Trunc,
	"Y0",		Y0,
	"Y1",		Y1,

	(const char*)NULL, (F1_t)NULL,
};

static struct {		/* Numeric built-ins with one symbol argument */
	const char *name;
	F1i_t	func;
} builtins_1i[] = {
	/* Please keep these groups sorted alphabetically, ignoring lettercase */
	"defined",	Defined,
	"delete",	Delete,

	(const char*)NULL, (F1i_t)NULL,
};

static struct {		/* Numeric built-ins with one argument */
	const char *name;
	F1s_t	func;
} builtins_1s[] = {
	/* Please keep these groups sorted alphabetically, ignoring lettercase */
	"ichar",	Ichar,
	"htof",		HtoF,
	"length",	Length,
	"number",	StoF,
	"symnum",	SymtoN,

	(const char*)NULL, (F1s_t)NULL,
};

static struct {		/* Numeric built-ins with two arguments */
	const char *name;
	F2_t	func;
} builtins_2[] = {
	"copysign",	Copysign,
	"fmod",		Fmod,
	"hypot",	Hypot,
	"Jn",		Jn,
	"ldexp",	Ldexp,
	"nearest",	Nearest,
	"nextafter",	Nextafter,
	"randint",	RandInt,
	"remainder",	Remainder,
	"scalb",	Scalb,
	"unordered",	Unordered,
	"Yn",		Yn,

	(const char*)NULL, (F2_t)NULL,
};

static struct {		/* Numeric built-ins with two string argument */
	const char *name;
	F2s_t	func;
} builtins_2s[] = {
	/* Please keep these groups sorted alphabetically, ignoring lettercase */
	"index",	Index,

	(const char*)NULL, (F2s_t)NULL,
};

/* NB: All builtin functions that return strings MUST return a dynamic
   string which can be freed after use in functions in code.c!   To make this
   convention clearer, these builtin functions have capitalized names. */

static struct {		/* String built-ins with zero arguments */
	const char *name;
	SF0_t	func;
} strbuiltins_0[] = {
	"endinput",	EndInput,
	"logoff",	Logoff,
	"logon",	Logon,
	"now",		Now,
	"pwd",		Pwd,

	(const char*)NULL, (SF0_t)NULL,
};

static struct {		/* String built-ins with one argument */
	const char *name;
	SF1_t	func;
} strbuiltins_1[] = {
	"abort",	Abort,
	"cd",		Cd,
	"eval",		Evalcommands,
	"exit",		Exit,
	"getenv",	Getenv,
	"load",		Load,
	"logfile",	Logfile,
	"msg_translate",Msg_Translate,
	"printenv",	Printenv,
	"protect",	Protect,
	"symstr",	SymtoS,
	"tolower",	Tolower,
	"toupper",	Toupper,
	"who",		Who,

	(const char*)NULL, (SF1_t)NULL,
};

static struct {		/* String built-ins with one numeric argument */
	const char *name;
	SF1n_t	func;
} strbuiltins_1n[] = {
	"__hex",	Hex,
	"char",		Char,
	"ftoh",		FtoH,
	"hexfp",	Hexfp,
	"hexint",	Hexint,
	"string",	FtoS,

	(const char*)NULL, (SF1n_t)NULL,
};

static struct {		/* String built-ins with two arguments */
	const char *name;
	SF2_t	func;
} strbuiltins_2[] = {
	"putenv",	Putenv,
	"save",		Save,

	(const char*)NULL, (SF2_t)NULL,
};

static struct {		/* String built-ins with two arguments */
	const char *name;
	SF2sn_t	func;
} strbuiltins_2sn[] = {
	"strftime",	Strftime,

	(const char*)NULL, (SF2sn_t)NULL,
};

static struct {		/* String built-ins with three arguments */
	const char *name;
	SF3snn_t func;
} strbuiltins_3snn[] = {
	"substr",	Substr,

	(const char*)NULL, (SF3snn_t)NULL,
};

static int
check_range(const char *name, fp_t vmin, fp_t vmax, int check_finite, int check_nonzero)
{	/* Check that vmin <= get_number(name) <= vmax, and possibly also finite, and nonzero:
	   report error and return 1 if not, else return 0 (okay) */
	fp_t v;

	v = get_number(name);
	if ((v < vmin) ||
	    (vmax < v) ||
	    (check_finite && IsInf(v)) ||
	    (check_finite && IsNaN(v)) ||
	    (check_nonzero && (v == FP(0.0))))
	{
		const char *msg;

		msg = concat3("Internal error: incorrect value for ", name, ":");
		warning(msg, fmtnum(v));
		efree((void*)msg);
		return (1);
	}
	else
		return (0);
}

int
default_precision(void)
{	/* Return the default output precision */

	/* From David Matula, ``In-and-Out Conversions'', Comm. ACM,
	   11(1) 47--50, January, 1968): need at least (N/(log_b 10) + 1)
	   decimal digits to correctly represent an N-digit base-b number
	   in base 10.  For IEEE 754 double precision, b = 2 and N = 53,
	   so we need ceil(53/log2(10) + 1) = ceil(16.95) = 17 decimal
	   digits. */

	static int precision = 0;	/* number of decimal digits required for correct
					   roundtrip binary <--> decimal conversions */

	if (precision == 0)	      /* happens only on first call */
	{

#if defined(HAVE_FP_T_DOUBLE)
#if defined(DBL_MANT_DIG) && defined(FLT_RADIX)
		precision = (int)ceil(DBL_MANT_DIG/(log(FP(10.0))/log((fp_t)FLT_RADIX)) + FP(1.0));
#else
		precision = 17;	/* suitable for IEEE 754 64-bit fp_t */
#endif
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
#if defined(LDBL_MANT_DIG) && defined(FLT_RADIX)
		precision = (int)ceil(LDBL_MANT_DIG/(log(FP(10.0))/log((fp_t)FLT_RADIX)) + FP(1.0));
#else
		{
			FP_Parts d;

			if (sizeof(d) == sizeof(d.v)) /* 128-bit */
				precision = 36;	/* suitable for IEEE 754 128-bit fp_t */
			else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
				precision = 17;
			else			/* 80-bit Intel x86 format */
				precision = 21;
		}
#endif
#endif

#if defined(HAVE_FP_T_SINGLE)
#if defined(FLT_MANT_DIG) && defined(FLT_RADIX)
		precision = (int)ceil(FLT_MANT_DIG/(log(FP(10.0))/log((fp_t)FLT_RADIX)) + FP(1.0));
#else
		precision = 9;	/* suitable for IEEE 754 32-bit fp_t */
#endif
#endif

	}
	return (precision);
}

int
get_precision(void)
{	/* Return the current output precision */
	Symbol *s;
	int precision;

	s = lookup("PREC");
	if (s == (Symbol*)NULL)
		(void)set_precision((precision = default_precision(), precision));
	else
		precision = (int)s->u.val;
	return (precision);
}

static Symbol *
install_builtin(const char *s, long t, fp_t d)
{
	Symbol *sp;

	sp = install(s, t, d);
	sp->builtin = 1;
	return (sp);
}

static Symbol *
install_builtin_const_number(const char *s, fp_t d)
{
	Symbol *sp;

	sp = install_const_number(s, d);
	sp->builtin = 1;
	return (sp);
}

int
set_precision(int new_precision)
{
	/* Set the new floating-point precision to new_precision, and
	return the old one. */
	int old_precision;

	old_precision = get_precision();

	/* Guarantee reasonable values of PREC: keep in range [0..36]
	   (36 is adequate for 128-bit IEEE 754 floating point. */
	if (new_precision < 0)
		new_precision = 0;
	else if (new_precision > 36)
		new_precision = 36;
	lookup("PREC")->u.val = (fp_t)new_precision;
	return (old_precision);
}

void
init_builtins(void)	/* install constants and built-ins in table */
{
	int i;
	Symbol *s;

	(void)install_string("__INDENT__","");
	(void)install_string("__OFS__"," ");
	init_fp_globals();	/* these are needed for computation of other constants */

	for (i = 0; keywords[i].name; i++)
		(void)install_builtin(keywords[i].name, keywords[i].kval, FP(0.0));
	(void)install_number("_", FP(0.0));	/* last numeric result */
	(void)install_string("__","");	/* last string result */
	(void)install_string("__PROMPT__", (PACKAGE_NAME "> "));

	init_consts();
	for (i = 0; consts[i].name; i++)
	{
		if (consts[i].immutable)
			(void)install_builtin_const_number(consts[i].name, consts[i].cval);
		else
			(void)install_builtin(consts[i].name, VAR, consts[i].cval);
	}
	for (i = 0; builtins_0[i].name; i++) {
		s = install_builtin(builtins_0[i].name, BLTIN0, FP(0.0));
		s->u.ptr0 = builtins_0[i].func;
	}

	for (i = 0; builtins_1[i].name; i++) {
		s = install_builtin(builtins_1[i].name, BLTIN1, FP(0.0));
		s->u.ptr1 = builtins_1[i].func;
	}

	for (i = 0; builtins_1i[i].name; i++) {
		s = install_builtin(builtins_1i[i].name, BLTIN1I, FP(0.0));
		s->u.ptr1i = builtins_1i[i].func;
	}

	for (i = 0; builtins_1s[i].name; i++) {
		s = install_builtin(builtins_1s[i].name, BLTIN1S, FP(0.0));
		s->u.ptr1s = builtins_1s[i].func;
	}

	for (i = 0; builtins_2[i].name; i++) {
		s = install_builtin(builtins_2[i].name, BLTIN2, FP(0.0));
		s->u.ptr2 = builtins_2[i].func;
	}

	for (i = 0; builtins_2s[i].name; i++) {
		s = install_builtin(builtins_2s[i].name, BLTIN2S, FP(0.0));
		s->u.ptr2s = builtins_2s[i].func;
	}

	for (i = 0; strbuiltins_0[i].name; i++) {
		s = install_builtin(strbuiltins_0[i].name, STRBLTIN0, FP(0.0));
		s->u.sptr0 = strbuiltins_0[i].func;
	}

	for (i = 0; strbuiltins_1[i].name; i++) {
		s = install_builtin(strbuiltins_1[i].name, STRBLTIN1, FP(0.0));
		s->u.sptr1 = strbuiltins_1[i].func;
	}

	for (i = 0; strbuiltins_1n[i].name; i++) {
		s = install_builtin(strbuiltins_1n[i].name, STRBLTIN1N, FP(0.0));
		s->u.sptr1n = strbuiltins_1n[i].func;
	}

	for (i = 0; strbuiltins_2[i].name; i++) {
		s = install_builtin(strbuiltins_2[i].name, STRBLTIN2, FP(0.0));
		s->u.sptr2 = strbuiltins_2[i].func;
	}

	for (i = 0; strbuiltins_2sn[i].name; i++) {
		s = install_builtin(strbuiltins_2sn[i].name, STRBLTIN2SN, FP(0.0));
		s->u.sptr2sn = strbuiltins_2sn[i].func;
	}

	for (i = 0; strbuiltins_3snn[i].name; i++) {
		s = install_builtin(strbuiltins_3snn[i].name, STRBLTIN3SNN, FP(0.0));
		s->u.sptr3snn = strbuiltins_3snn[i].func;
	}

	(void)update_number("__CPU_LIMIT__", Infinity());

#if defined(HAVE_TIME_H) && defined(HAVE_STRFTIME)
	{
		char datestamp[sizeof("MMM DD YYYY") + 1];
		char timestamp[sizeof("HH:MM:SS") + 1];

		time_t now;

		(void)time(&now);

		(void)strftime(datestamp, sizeof(datestamp), "%b %d %Y", localtime(&now));
		(void)strftime(timestamp, sizeof(datestamp), "%H:%M:%S", localtime(&now));

		if (datestamp[4] == '0')
			datestamp[4] = ' '; /* Standard C requires leading zero as blank */

		(void)install_const_string("__DATE__", &datestamp[0]);
		(void)install_const_string("__TIME__", &timestamp[0]);
	}
#else
	/* Ensure that __DATE__ and __TIME__ have dummy values when we
	   cannot get correct values, so that the variables are
	   guaranteed to be available in user code. */
	(void)install_const_string("__DATE__", "MMM DD YYYY");
	(void)install_const_string("__TIME__", "HH:MM:SS");
#endif
}

static void
init_consts(void)
{
	int i;

	for (i = 0; consts[i].name; i++)
	{
		if (STREQUAL(consts[i].name, "NAN") ||
		    STREQUAL(consts[i].name, "NaN"))
			consts[i].cval = NaN();
		else if (STREQUAL(consts[i].name, "QNAN") ||
		    STREQUAL(consts[i].name, "QNaN"))
			consts[i].cval = QNaN();
		else if (STREQUAL(consts[i].name, "SNAN") ||
		    STREQUAL(consts[i].name, "SNaN"))
			consts[i].cval = SNaN();
		else if (STREQUAL(consts[i].name, "INF") ||
			 STREQUAL(consts[i].name, "Inf") ||
			 STREQUAL(consts[i].name, "Infinity"))
			consts[i].cval = Infinity();
		else if (STREQUAL(consts[i].name, "PREC"))
			consts[i].cval = default_precision();
		else if (STREQUAL(consts[i].name, "MAXNORMAL"))
			consts[i].cval = MaxNormal();
		else if (STREQUAL(consts[i].name, "MAXSUBNORMAL"))
			consts[i].cval = MaxSubnormal();
		else if (STREQUAL(consts[i].name, "MINNORMAL"))
			consts[i].cval = MinNormal();
		else if (STREQUAL(consts[i].name, "MINSUBNORMAL"))
			consts[i].cval = MinSubnormal();
	}
}

static void
init_fp_globals(void)
{
	/* These constants are needed so frequently that we want to
	   avoid the overhead of further get_number() calls for them, so
	   we make them global.  We also need them for the computation
	   of some of the other floating-point constants. */

	int i;
	fp_t a, b;

#if defined(HAVE_IEEE_754)

#if defined(HAVE_FP_T_DOUBLE)
	EBIAS = FP(1023.0);
	EBITS = FP(11.0);
	EMAX = FP(1023.0);
	EMIN = FP(-1022.0);
	P = FP(53.0);
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
	FP_Parts d;

	if (sizeof(d) == sizeof(d.v)) /* 128-bit */
	{
		EBIAS = FP(16383.0);
		EBITS = FP(15.0);
		EMAX = FP(16383.0);
		EMIN = FP(-16382.0);
		P = FP(113.0);
	}
	else if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
	{
		EBIAS = FP(1023.0);
		EBITS = FP(11.0);
		EMAX = FP(1023.0);
		EMIN = FP(-1022.0);
		P = FP(53.0);
	}
	else			/* 80-bit Intel x86 format */
	{
		EBIAS = FP(16383.0);
		EBITS = FP(15.0);
		EMAX = FP(16383.0);
		EMIN = FP(-16382.0);
		P = FP(64.0);
	}
#endif

#if defined(HAVE_FP_T_SINGLE)
	EBIAS = FP(127.0);
	EBITS = FP(8.0);
	EMAX = FP(127.0);
	EMIN = FP(-126.0);
	P = FP(24.0);
#endif

#else  /* NOT defined(HAVE_IEEE_754) */
	/* TO-DO: Supply EBIAS, EBITS, EMIN, EMAX, and P for other floating-point systems. */
	warning("WARNING: no definitions of EBIAS, EBITS, EMIN, EMAX, and P have been set for this architecture",
		(const char*)NULL);
#endif /* defined(HAVE_IEEE_754) */

	/* Determine the floating-point base portably, using the
	   algorithm from ELEFUNT's machar() subroutine */

	a = FP(2.0);
	while ((((a + FP(1.0)) - a) - FP(1.0)) == 0)
		a += a;

	b = FP(2.0);
	while (((a + b) - a) == 0) \
		b += b;

	BASE = (fp_t)((int)((a + b) - a));

	for (i = 0; consts[i].name; i++)
	{
		if (STREQUAL(consts[i].name, "BASE"))
			consts[i].cval = BASE;
		else if (STREQUAL(consts[i].name, "EBIAS"))
			consts[i].cval = EBIAS;
		else if (STREQUAL(consts[i].name, "EBITS"))
			consts[i].cval = EBITS;
		else if (STREQUAL(consts[i].name, "EMAX"))
			consts[i].cval = EMAX;
		else if (STREQUAL(consts[i].name, "EMIN"))
			consts[i].cval = EMIN;
		else if (STREQUAL(consts[i].name, "P"))
			consts[i].cval = P;
	}
}

void
sanity_check(void)
{
	/* Sadly, enough problems from implementation deficiencies
	   surfaced during hoc development that some sanity checks are
	   needed before any file input can be safely processed. */

	int errors;

	errors = 0;

	/* NB: These range checks may require updating for non-IEEE-754 architectures */
	errors += check_range("BASE",		FP(2.0), FP(16.0), 1, 1);
	errors += check_range("EBIAS",		FP(127.0), FP(16383.0), 1, 1);
	errors += check_range("EBITS",		FP(8.0), FP(15.0), 1, 1);
	errors += check_range("EMAX",		FP(127.0), FP(16383.0), 1, 1);
	errors += check_range("EMIN",		FP(-16382.0), FP(-126.0), 1, 1);
	errors += check_range("P",		FP(21.0), FP(113.0), 1, 1);
	errors += check_range("MINSUBNORMAL",	FP(0.0), FP(1.0e-30), 1, 1);
	errors += check_range("MAXSUBNORMAL",	FP(0.0), FP(1.0e-30), 1, 1);
	errors += check_range("MINNORMAL",	FP(0.0), FP(1.0e-30), 1, 1);
	errors += check_range("MAXNORMAL",	FP(1.0e+30), Infinity(), 1, 1);

#if (HAVE_IEEE_754)
	errors += check_range("Inf",		Infinity(), Infinity(), 0, 1);
#endif
	if (errors > 0)
		exit(EXIT_FAILURE);
}
