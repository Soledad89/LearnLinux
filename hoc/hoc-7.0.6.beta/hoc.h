#include "config.h"

#if defined(HAVE_STDLIB_H)
#include <stdlib.h>			/* for size_t */
#endif

/* Set dimensions that are fixed at compile time, but whose default
   values may be overridden by compile-time -DMAX_xxx=nnn options.
   Some of these have documented minimum values (see hoc.1) which must
   be enforced here.  All are made available for user inspection in
   __MAX_xxx__ predefined constants. */

#if defined(__cplusplus) || defined(c_plusplus)
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#include "fp_arch.h"
#include "fp_t.h"

extern fp_t	BASE;
extern fp_t	EBIAS;
extern fp_t	EBITS;
extern fp_t	EMAX;
extern fp_t	EMIN;
extern fp_t	P;

typedef struct Symbol Symbol;		/* temporary incomplete definition */

typedef fp_t		(*F0_t)		ARGS((void));
typedef fp_t		(*F1_t)		ARGS((fp_t));
typedef fp_t		(*F1s_t)	ARGS((const char *));
typedef fp_t		(*F1i_t)	ARGS((Symbol *));
typedef fp_t		(*F2_t)		ARGS((fp_t,fp_t));
typedef fp_t		(*F2s_t)	ARGS((const char *, const char *));
typedef const char *	(*SF0_t)	ARGS((void));
typedef const char *	(*SF1_t)	ARGS((const char *));
typedef const char *	(*SF1n_t)	ARGS((fp_t));
typedef const char *	(*SF2_t)	ARGS((const char *, const char *));
typedef const char *	(*SF2sn_t)	ARGS((const char *, fp_t));
typedef const char *	(*SF3snn_t)	ARGS((const char *, fp_t, fp_t));

typedef void (*Inst) ARGS((void));
#define	STOP	((Inst) NULL)

#if defined(HAVE_LONG_LONG)
typedef long long LONG_LONG;
typedef unsigned long long UNSIGNED_LONG_LONG;
#else
typedef long LONG_LONG;
typedef unsigned long UNSIGNED_LONG_LONG;
#endif

typedef struct Symbol {	/* symbol table entry */
	union {
		fp_t	val;		/* VAR */
		F0_t ptr0;		/* BLTIN0 */
		F1_t ptr1;		/* BLTIN1 */
		F1s_t ptr1s;		/* BLTIN1S */
		F1i_t ptr1i;		/* BLTIN1I */
		F2_t ptr2;		/* BLTIN2 */
		F2s_t ptr2s;		/* BLTIN2S */
		SF0_t sptr0;		/* STRBLTIN0 */
		SF1_t sptr1;		/* STRBLTIN1 */
		SF1n_t sptr1n;		/* STRBLTIN1N */
		SF2_t sptr2;		/* STRBLTIN2 */
		SF2sn_t sptr2sn;	/* STRBLTIN2SN */
		SF3snn_t sptr3snn;	/* STRBLTIN3SNN */
		Inst	*defn;		/* FUNCTION, PROCEDURE */
		const char *str;	/* STRING */
	} u;
	const char	*name;
	long		builtin;
	long		immutable;
	long		type;
	struct Symbol	*next;	/* to link to another */
} Symbol;

Symbol	*install ARGS((const char *, int, fp_t));
Symbol	*lookup ARGS((const char *));

#if 0
typedef union Datum {	/* interpreter stack type */
	fp_t	val;
	const char *str;
	Symbol	*sym;
} Datum;
#else
typedef struct Datum {	/* interpreter stack type */
	union {
		fp_t	val;
		const char *str;
		Symbol	*sym;
	} u;
	long type;
} Datum;
#endif

typedef union DP_Parts { /* access to floating-point bits as integers */
	double v;
	unsigned int i[2];
} DP_Parts;

#if defined(HAVE_FP_T_DOUBLE)
typedef DP_Parts FP_Parts;
#endif

#if defined(HAVE_FP_T_SINGLE)
typedef union SP_Parts { /* access to floating-point bits as integers */
	float v;
	unsigned int i[1];
} SP_Parts;
typedef SP_Parts FP_Parts;
#endif

#if defined(HAVE_FP_T_QUADRUPLE)
typedef union QP_Parts { /* access to floating-point bits as integers */
	long double v;
	unsigned int i[4];
} QP_Parts;
typedef QP_Parts FP_Parts;
#endif

#define CLASS_SNAN		1000000
#define CLASS_QNAN		(CLASS_SNAN + 1)
#define CLASS_NEGINF		(CLASS_SNAN + 2)
#define CLASS_NEGNORMAL		(CLASS_SNAN + 3)
#define CLASS_NEGSUBNORMAL	(CLASS_SNAN + 4)
#define CLASS_NEGZERO		(CLASS_SNAN + 5)
#define CLASS_POSZERO		(CLASS_SNAN + 6)
#define CLASS_POSSUBNORMAL	(CLASS_SNAN + 7)
#define CLASS_POSNORMAL		(CLASS_SNAN + 8)
#define CLASS_POSINF		(CLASS_SNAN + 9)

#define CLASS_UNKNOWN		(CLASS_SNAN - 1) /* should never happen */

/* 1989 and 1999 Standard C, and 1998 Standard C++, require that the
ctype.h isxxx() feature to be functions, so we should be able to
redefine macros to call the parenthesized functions, without changing
any code below.  Unfortunately Apple Darwin botches things, and
defines only isxxx() macros.  Thus, we need our own private names for
these character classifiers.

It is CRITICAL that, before making the range checks, we first convert
c to an unsigned value in 0..255, because most UNIX C and C++
compilers treat char as signed in -128..+127.  Lack of the UC()
wrapper was a nasty bug that took me some time to track down. */

#define UC(c)	( (int)(unsigned char)(c) )
#define UL(x)	( (unsigned long)(x) )
#define IsAlNum(c) (isalnum(c) || ((160 <= UC(c)) && (UC(c) <= 255)))
#define IsAlpha(c) (isalpha(c) || ((160 <= UC(c)) && (UC(c) <= 255)))
#define IsIdStart(c) (IsAlpha(c) || ((c) == '_'))
#define IsIdMiddle(c) (IsAlNum(c) || ((c) == '_'))

#if !defined(HOCPATH)
#define HOCPATH	"HOCPATH"	/* name of environment variable */
#endif

#define STREQUAL(a,b)	(strcmp(a,b) == 0)

/* Please keep these declarations of hoc global symbols grouped by
   data type, and sorted alphabetically, IGNORING lettercase, with one
   blank line between groups of different data type. Declare only ONE
   object per line, and keep each declaration on a SINGLE line so that
   line-sort commands can be safely applied in text-editing sessions. */

extern const char *	progname;

extern Inst		*prog;
extern size_t		max_prog;

extern Inst *		progbase;
extern Inst *		progp;

extern int		EOF_flag;

extern const char *	Abort ARGS((const char *));
extern const char *	Cd ARGS((const char *));
extern const char *	Char ARGS((fp_t));
extern const char *	concat2 ARGS((const char *, const char *));
extern const char *	concat3 ARGS((const char *, const char *, const char *));
extern const char *	concat4 ARGS((const char *, const char *, const char *, const char *));
extern const char *	concat5 ARGS((const char *, const char *, const char *, const char *, const char *));
extern const char *	concat6 ARGS((const char *, const char *, const char *, const char *, const char *, const char *));
extern const char *	dupstr ARGS((const char *));
extern const char *	EndInput ARGS((void));
extern const char *	Evalcommands ARGS((const char *));
extern const char *	Exit ARGS((const char *));
extern const char *	Expand ARGS((const char *));
extern const char *	first_symbol_pname ARGS((void));
extern const char *	fmtnum ARGS((fp_t));
extern const char *	FtoH ARGS((fp_t));
extern const char *	FtoS ARGS((fp_t));
extern const char *	Getenv ARGS((const char *));
extern const char *	get_string ARGS((const char *));
extern const char *	Hex ARGS((fp_t));
extern const char *	Hexfp ARGS((fp_t));
extern const char *	Hexint ARGS((fp_t));
extern const char *	Inf_string ARGS((fp_t));
extern const char *	Load ARGS((const char *));
extern const char *	Logfile ARGS((const char *));
extern const char *	Logoff ARGS((void));
extern const char *	Logon ARGS((void));
extern const char *	Msg_Translate ARGS((const char *));
extern const char *	msg_translate ARGS((const char *));
extern const char *	NaN_string ARGS((fp_t));
extern const char *	next_symbol_pname ARGS((void));
extern const char *	Now ARGS((void));
extern const char *	parentdir ARGS((const char *));
extern const char *	Printenv ARGS((const char *));
extern const char *	Protect ARGS((const char *));
extern const char *	Pwd ARGS((void));
extern const char *	Putenv ARGS((const char *, const char *));
extern const char *	Save ARGS((const char *, const char *));
extern const char *	Strftime ARGS((const char *, fp_t));
extern const char *	Substr ARGS((const char *, fp_t, fp_t));
extern const char *	Tolower ARGS((const char *));
extern const char *	Toupper ARGS((const char *));
extern const char *	SymtoS ARGS((const char *));
extern const char *	Who ARGS((const char *));
extern const char *	xfptos ARGS((fp_t));

/* Mathematical, numeric, and support functions in extended hoc */

extern fp_t		Abs ARGS((fp_t));
extern fp_t		Acos ARGS((fp_t));
extern fp_t		Acosh ARGS((fp_t));
extern fp_t		Asin ARGS((fp_t));
extern fp_t		Asinh ARGS((fp_t));
extern fp_t		Atan ARGS((fp_t));
extern fp_t		Atanh ARGS((fp_t));
extern fp_t		Cbrt ARGS((fp_t));
extern fp_t		Ceil ARGS((fp_t));
extern fp_t		Class ARGS((fp_t));
extern fp_t		Copysign ARGS((fp_t, fp_t));
extern fp_t		Cos ARGS((fp_t));
extern fp_t		Cosh ARGS((fp_t));
extern fp_t		CPULimit ARGS((fp_t));
extern fp_t		Defined ARGS((Symbol *));
extern fp_t		Delete ARGS((Symbol *));
extern fp_t		Double ARGS((fp_t));
extern fp_t		Erf ARGS((fp_t));
extern fp_t		Erfc ARGS((fp_t));
extern fp_t		Exp ARGS((fp_t));
extern fp_t		Expm1 ARGS((fp_t));
extern fp_t		Exponent ARGS((fp_t));
extern fp_t		Fgetd ARGS((int));
extern fp_t		Floor ARGS((fp_t));
extern fp_t		Fmod ARGS((fp_t, fp_t));
extern fp_t		Gamma ARGS((fp_t));
extern fp_t		get_number ARGS((const char *));
extern fp_t		HtoF ARGS((const char *));
extern fp_t		Hypot ARGS((fp_t, fp_t));
extern fp_t		Ichar ARGS((const char *));
extern fp_t		Ilogb ARGS((fp_t));
extern fp_t		Index ARGS((const char *,const char *));
extern fp_t		Infinity ARGS((void));
extern fp_t		Integer ARGS((fp_t));
extern fp_t		IsFinite ARGS((fp_t));
extern fp_t		IsInf ARGS((fp_t));
extern fp_t		IsNaN ARGS((fp_t));
extern fp_t		IsNormal ARGS((fp_t));
extern fp_t		IsQNaN ARGS((fp_t));
extern fp_t		IsSNaN ARGS((fp_t));
extern fp_t		IsSubnormal ARGS((fp_t));
extern fp_t		J0 ARGS((fp_t));
extern fp_t		J1 ARGS((fp_t));
extern fp_t		Jn ARGS((fp_t, fp_t));
extern fp_t		Ldexp ARGS((fp_t, fp_t));
extern fp_t		Length ARGS((const char *));
extern fp_t		Lgamma ARGS((fp_t));
extern fp_t		Log ARGS((fp_t));
extern fp_t		Log10 ARGS((fp_t));
extern fp_t		Log1p ARGS((fp_t));
extern fp_t		Log2 ARGS((fp_t));
extern fp_t		Macheps ARGS((fp_t));
extern fp_t		MaxNormal ARGS((void));
extern fp_t		MaxSubnormal ARGS((void));
extern fp_t		MinNormal ARGS((void));
extern fp_t		MinSubnormal ARGS((void));
extern fp_t		NaN ARGS((void));
extern fp_t		Nearest ARGS((fp_t, fp_t));
extern fp_t		Nextafter ARGS((fp_t, fp_t));
extern fp_t		Nint ARGS((fp_t));
extern fp_t		Pow ARGS((fp_t, fp_t));
extern fp_t		QNaN ARGS((void));
extern fp_t		Rand ARGS((void));
extern fp_t		RandInt ARGS((fp_t, fp_t));
extern fp_t		Randl ARGS((fp_t));
extern fp_t		Remainder ARGS((fp_t, fp_t));
extern fp_t		Rint ARGS((fp_t));
extern fp_t		Rsqrt ARGS((fp_t));
extern fp_t		Scalb ARGS((fp_t, fp_t));
extern fp_t		Second ARGS((void));
extern fp_t		SetRandSeed ARGS((fp_t));
extern fp_t		Significand ARGS((fp_t));
extern fp_t		Sin ARGS((fp_t));
extern fp_t		Single ARGS((fp_t));
extern fp_t		Sinh ARGS((fp_t));
extern fp_t		SNaN ARGS((void));
extern fp_t		Sqrt ARGS((fp_t));
extern fp_t		StoF ARGS((const char *));
extern fp_t		SymtoN ARGS((const char *));
extern fp_t		store ARGS((fp_t *));
extern fp_t		strton ARGS((const char *, char **));
extern fp_t		Systime ARGS((void));
extern fp_t		Tan ARGS((fp_t));
extern fp_t		Tanh ARGS((fp_t));
extern fp_t		Trunc ARGS((fp_t));
extern fp_t		Unordered ARGS((fp_t, fp_t));
extern fp_t		xstrtod ARGS((const char *, char **));
extern fp_t		Y0 ARGS((fp_t));
extern fp_t		Y1 ARGS((fp_t));
extern fp_t		Yn ARGS((fp_t, fp_t));

extern int		decval ARGS((int));
extern int		default_precision ARGS((void));
extern int		delete_symbol ARGS((Symbol *));
extern int		get_precision ARGS((void));
extern int		hexval ARGS((int));
extern int		is_debug ARGS((const char *));
extern int		is_hidden ARGS((const char *, const char *));
extern int		is_immutable ARGS((const char *));
extern int		is_local ARGS((const char *));
extern int		is_match ARGS((const char *, const char *));
extern int		IsBigEndian ARGS((void));
extern int		match ARGS((const char *, const char *));
extern int		moreinput ARGS((void));
extern int		set_precision ARGS((int));
EXTERN int              yyparse ARGS((void)); /* NB: EXTERN, not extern! */

EXTERN size_t		strlcat(char *, const char *, size_t);
EXTERN size_t		strlcpy(char *, const char *, size_t);

extern Inst *		code ARGS((Inst));

extern Symbol *		first_symbol ARGS((void));
extern Symbol *		install_const_number ARGS((const char *, fp_t));
extern Symbol *		install_const_string ARGS((const char *, const char *));
extern Symbol *		install_number ARGS((const char *, fp_t));
extern Symbol *		install_string ARGS((const char *, const char *));
extern Symbol *		next_symbol ARGS((void));
extern Symbol *		set_string ARGS((Symbol *, const char *));
extern Symbol *		update_const_number ARGS((const char *, fp_t));
extern Symbol *		update_const_string ARGS((const char *, const char *));
extern Symbol *		update_number ARGS((const char *, fp_t));
extern Symbol *		update_string ARGS((const char *, const char *));

extern void		add ARGS((void));
extern void		addeq ARGS((void));
extern void		And ARGS((void));
extern void		arg ARGS((void));
extern void		argaddeq ARGS((void));
extern void		argassign ARGS((void));
extern void		argdiveq ARGS((void));
extern void		argmodeq ARGS((void));
extern void		argmuleq ARGS((void));
extern void		argsubeq ARGS((void));
extern void		assign ARGS((void));
extern void		bltin0 ARGS((void));
extern void		bltin1 ARGS((void));
extern void		bltin1i ARGS((void));
extern void		bltin1s ARGS((void));
extern void		bltin2 ARGS((void));
extern void		bltin2s ARGS((void));
extern void		breakstmt ARGS((void));
extern void		call ARGS((void));
extern void		const_assign ARGS((void));
extern void		const_str_assign ARGS((void));
extern void		const_str_push ARGS((void));
extern void		constpush ARGS((void));
extern void		continuestmt ARGS((void));
extern void		define ARGS((Symbol *));
extern void		diveq ARGS((void));
extern void		divop ARGS((void));
extern void		dump_syms ARGS((const char *));
extern void		efree ARGS((void *));
extern void		eq ARGS((void));
extern void		eval ARGS((void));
extern void		execerror ARGS((const char *, const char *));
extern void		execute ARGS((Inst*));
extern void		forcode ARGS((void));
extern void		free_symbol_table ARGS((void));
extern void		funcret ARGS((void));
extern void		ge ARGS((void));
extern void		gt ARGS((void));
extern void		ifcode ARGS((void));
extern void		init_builtins ARGS((void));
extern void		initcode ARGS((void));
extern void		le ARGS((void));
extern void		lt ARGS((void));
extern void		ltgt ARGS((void));
extern void		make_immutable ARGS((Symbol *));
extern void		mod ARGS((void));
extern void		modeq ARGS((void));
extern void		mul ARGS((void));
extern void		muleq ARGS((void));
extern void		ne ARGS((void));
extern void		negate ARGS((void));
extern void		noop ARGS((void));
extern void		Not ARGS((void));
extern void		Or ARGS((void));
extern void		postdec ARGS((void));
extern void		postinc ARGS((void));
extern void		power ARGS((void));
extern void		predec ARGS((void));
extern void		preinc ARGS((void));
extern void		prexpr ARGS((void));
extern void		prfmt ARGS((void));
extern void		printtop ARGS((void));
extern void		printtopstring ARGS((void));
extern void		prnl ARGS((void));
extern void		prnum ARGS((fp_t));
extern void		procret ARGS((void));
extern void		prsep ARGS((void));
extern void		prstr ARGS((void));
extern void		prtext ARGS((const char *));
extern void		prtext2 ARGS((const char *, int));
extern void		sanity_check ARGS((void));
extern void		set_filename ARGS((const char *));
extern void		sprexpr ARGS((void));
extern void		sprfmt ARGS((void));
extern void		sprjoin ARGS((void));
extern void		sprstr ARGS((void));
extern void		str_assign ARGS((void));
extern void		str_concat ARGS((void));
extern void		str_eq ARGS((void));
extern void		str_ge ARGS((void));
extern void		str_gt ARGS((void));
extern void		str_le ARGS((void));
extern void		str_lt ARGS((void));
extern void		str_ne ARGS((void));
extern void		str_to_num ARGS((void));
extern void		strarg ARGS((void));
extern void		strbltin0 ARGS((void));
extern void		strbltin1 ARGS((void));
extern void		strbltin1n ARGS((void));
extern void		strbltin2 ARGS((void));
extern void		strbltin2sn ARGS((void));
extern void		strbltin3snn ARGS((void));
extern void		streval ARGS((void));
extern void		sub ARGS((void));
extern void		subeq ARGS((void));
extern void		varpush ARGS((void));
extern void		varread ARGS((void));
extern void		warning ARGS((const char *, const char *));
extern void		whilecode ARGS((void));
extern void		xpop ARGS((void));

extern void *		egrow ARGS((void *, size_t *, size_t));
extern void *		emalloc ARGS((size_t));
extern void *		erealloc ARGS((void *, size_t, size_t));

#if !defined(HAVE_SNPRINTF_PROTOTYPE)
EXTERN int		snprintf ARGS((char *, size_t, const char *, ...));
#endif

#if defined(HAVE_SNPRINTF)
#define CHKRET(r,n)	if (r >= n) (void)fprintf(stderr,"%s:%d:%s\n", __FILE__, (int)__LINE__, "snprintf() buffer too small"); \
			if (r < 0) (void)fprintf(stderr,"%s:%d:%s\n", __FILE__, (int)__LINE__, "snprintf() output error")
#define SNPRINTF4(s,n,fmt,a)		do {int n__; n__ = (snprintf)((s),(n),(fmt),(a)); CHKRET(n__,(n)); } while (0)
#define SNPRINTF5(s,n,fmt,a,b)		do {int n__; n__ = (snprintf)((s),(n),(fmt),(a),(b)); CHKRET(n__,(n)); } while (0)
#define SNPRINTF6(s,n,fmt,a,b,c)	do {int n__; n__ = (snprintf)((s),(n),(fmt),(a),(b),(c)); CHKRET(n__,(n)); } while (0)
#define SNPRINTF7(s,n,fmt,a,b,c,d)	do {int n__; n__ = (snprintf)((s),(n),(fmt),(a),(b),(c),(d)); CHKRET(n__,(n)); } while (0)
#define SNPRINTF8(s,n,fmt,a,b,c,d,e)	do {int n__; n__ = (snprintf)((s),(n),(fmt),(a),(b),(c),(d),(e)); CHKRET(n__,(n)); } while (0)
#define SNPRINTF9(s,n,fmt,a,b,c,d,e,f)	do {int n__; n__ = (snprintf)((s),(n),(fmt),(a),(b),(c),(d),(e),(f)); CHKRET(n__,(n)); } while (0)
#else /* NOT defined(HAVE_SNPRINTF) */
#define SNPRINTF4(s,n,fmt,a)		(void)(sprintf)((s),    (fmt),(a))
#define SNPRINTF5(s,n,fmt,a,b)		(void)(sprintf)((s),    (fmt),(a),(b))
#define SNPRINTF6(s,n,fmt,a,b,c)	(void)(sprintf)((s),    (fmt),(a),(b),(c))
#define SNPRINTF7(s,n,fmt,a,b,c,d)	(void)(sprintf)((s),    (fmt),(a),(b),(c),(d))
#define SNPRINTF8(s,n,fmt,a,b,c,d,e)	(void)(sprintf)((s),    (fmt),(a),(b),(c),(d),(e))
#define SNPRINTF9(s,n,fmt,a,b,c,d,e,f)	(void)(sprintf)((s),    (fmt),(a),(b),(c),(d),(e),(f))
#endif /* defined(HAVE_SNPRINTF) */

#if defined(__LCC__)
/* Prototypes for all of the UNIX extensions that are absent from ISO
   1989 Standard C are omitted from lcc's header files, even though
   they are present in the runtime library.  For safety, we include
   prototypes of all of the math library functions needed by hoc. */

extern float		(acosf) ARGS((float));
extern float		(acoshf) ARGS((float));
extern float		(asinf) ARGS((float));
extern float		(asinhf) ARGS((float));
extern float		(atanf) ARGS((float));
extern float		(atanhf) ARGS((float));
extern float		(cbrtf) ARGS((float));
extern float		(ceilf) ARGS((float));
extern float		(copysignf) ARGS((float, float));
extern float		(cosf) ARGS((float));
extern float		(coshf) ARGS((float));
extern float		(erfcf) ARGS((float));
extern float		(erff) ARGS((float));
extern float		(expf) ARGS((float));
extern float		(expm1f) ARGS((float));
extern float		(fabsf) ARGS((float));
extern float		(floorf) ARGS((float));
extern float		(fmodf) ARGS((float, float));
extern float		(hypotf) ARGS((float, float));
extern float		(j0f) ARGS((float));
extern float		(j1f) ARGS((float));
extern float		(jnf) ARGS((int, float));
extern float		(ldexpf) ARGS((float, int));
extern float		(lgammaf) ARGS((float));
extern float		(log10f) ARGS((float));
extern float		(logf) ARGS((float));
extern float		(log1pf) ARGS((float));
extern float		(powf) ARGS((float, float));
extern float		(nextafterf) ARGS((float, float));
extern float		(remainderf) ARGS((float, float));
extern float		(rintf) ARGS((float));
extern float		(scalbf) ARGS((float, float));
extern float		(significandf) ARGS((float));
extern float		(sinf) ARGS((float));
extern float		(sinhf) ARGS((float));
extern float		(sqrtf) ARGS((float));
extern float		(truncf) ARGS((float));
extern float		(tanf) ARGS((float));
extern float		(tanhf) ARGS((float));
extern float		(y0f) ARGS((float));
extern float		(y1f) ARGS((float));
extern float		(ynf) ARGS((int, float));
extern int		(ilogbf) ARGS((float));

extern double		(acos) ARGS((double));
extern double		(acosh) ARGS((double));
extern double		(asin) ARGS((double));
extern double		(asinh) ARGS((double));
extern double		(atan) ARGS((double));
extern double		(atanh) ARGS((double));
extern double		(cbrt) ARGS((double));
extern double		(ceil) ARGS((double));
extern double		(copysign) ARGS((double, double));
extern double		(cos) ARGS((double));
extern double		(cosh) ARGS((double));
extern double		(erfc) ARGS((double));
extern double		(erf) ARGS((double));
extern double		(exp) ARGS((double));
extern double		(expm1) ARGS((double));
extern double		(fabs) ARGS((double));
extern double		(floor) ARGS((double));
extern double		(fmod) ARGS((double, double));
extern double		(hypot) ARGS((double, double));
extern double		(j0) ARGS((double));
extern double		(j1) ARGS((double));
extern double		(jn) ARGS((int, double));
extern double		(ldexp) ARGS((double, int));
extern double		(lgamma) ARGS((double));
extern double		(log10) ARGS((double));
extern double		(log) ARGS((double));
extern double		(log1p) ARGS((double));
extern double		(nextafter) ARGS((double, double));
extern double		(pow) ARGS((double, double));
extern double		(remainder) ARGS((double, double));
extern double		(rint) ARGS((double));
extern double		(scalb) ARGS((double, double));
extern double		(significand) ARGS((double));
extern double		(sin) ARGS((double));
extern double		(sinh) ARGS((double));
extern double		(sqrt) ARGS((double));
extern double		(tan) ARGS((double));
extern double		(tanh) ARGS((double));
extern double		(trunc) ARGS((double));
extern double		(y0) ARGS((double));
extern double		(y1) ARGS((double));
extern double		(yn) ARGS((int, double));
extern int		(ilogb) ARGS((double));

extern long double	(acosl) ARGS((long double));
extern long double	(acoshl) ARGS((long double));
extern long double	(asinl) ARGS((long double));
extern long double	(asinhl) ARGS((long double));
extern long double	(atanl) ARGS((long double));
extern long double	(atanhl) ARGS((long double));
extern long double	(cbrtl) ARGS((long double));
extern long double	(ceill) ARGS((long double));
extern long double	(copysignl) ARGS((long double, long double));
extern long double	(cosl) ARGS((long double));
extern long double	(coshl) ARGS((long double));
extern long double	(erfcl) ARGS((long double));
extern long double	(erfl) ARGS((long double));
extern long double	(expl) ARGS((long double));
extern long double	(expm1l) ARGS((long double));
extern long double	(fabsl) ARGS((long double));
extern long double	(floorl) ARGS((long double));
extern long double	(fmodl) ARGS((long double, long double));
extern long double	(hypotl) ARGS((long double, long double));
extern long double	(j0l) ARGS((long double));
extern long double	(j1l) ARGS((long double));
extern long double	(jnl) ARGS((int, long double));
extern long double	(ldexpl) ARGS((long double, int));
extern long double	(lgammal) ARGS((long double));
extern long double	(log10l) ARGS((long double));
extern long double	(logl) ARGS((long double));
extern long double	(log1pl) ARGS((long double));
extern long double	(nextafterl) ARGS((long double, long double));
extern long double	(powl) ARGS((long double, long double));
extern long double	(remainderl) ARGS((long double, long double));
extern long double	(rintl) ARGS((long double));
extern long double	(scalbl) ARGS((long double, long double));
extern long double	(significandl) ARGS((long double));
extern long double	(sinl) ARGS((long double));
extern long double	(sinhl) ARGS((long double));
extern long double	(sqrtl) ARGS((long double));
extern long double	(tanl) ARGS((long double));
extern long double	(tanhl) ARGS((long double));
extern long double	(truncl) ARGS((long double));
extern long double	(y0l) ARGS((long double));
extern long double	(y1l) ARGS((long double));
extern long double	(ynl) ARGS((int, long double));
extern int		(ilogbl) ARGS((long double));

#if defined(HAVE_FP_T_QUADRUPLE)
/* lcc 4.1 implements long double as double */
#undef acos
#undef acosh
#undef asin
#undef asinh
#undef atan
#undef atanh
#undef cbrt
#undef ceil
#undef copysign
#undef cos
#undef cosh
#undef erf
#undef erfc
#undef exp
#undef expm1
#undef fabs
#undef floor
#undef fmod
#undef hypot
#undef ilogb
#undef j0
#undef j1
#undef jn
#undef ldexp
#undef lgamma
#undef log
#undef log10
#undef log1p
#undef nextafter
#undef pow
#undef remainder
#undef rint
#undef scalb
#undef significand
#undef sin
#undef sinh
#undef sqrt
#undef tan
#undef tanh
#undef trunc
#undef y0
#undef y1
#undef yn
#endif /* defined(HAVE_FP_T_QUADRUPLE) */

#include <sys/types.h>			/* need for gid_t, pid_t, uid_t */

extern char *		(getcwd) ARGS((char *, size_t));
extern gid_t		(getgid) ARGS((void));
extern pid_t		(getpid) ARGS((void));
extern uid_t		(getuid) ARGS((void));
#endif /* defined(__LCC__) */

#if defined(__PGI) || defined(__LCC__)
/* The Portland Group compilers (pgcc and pgCC), and the
AT&T/Princeton lcc compiler, generate code that handles NaNs
incorrectly, and there may be others found in the future, so create a
single preprocessor symbol that we can cleanly test, sigh... */
#define HAVE_NAN_BOTCH
#endif /* defined(__PGI) || defined(__LCC__) */

#if defined(HAVE_FP_T_QUADRUPLE)
/* Some systems lack one or more of these, either prototypes header
   files, or functions in runtime libraries */
EXTERN fp_t		(acoshl) ARGS((fp_t));
EXTERN fp_t		(acosl) ARGS((fp_t));
EXTERN fp_t		(asinhl) ARGS((fp_t));
EXTERN fp_t		(asinl) ARGS((fp_t));
EXTERN fp_t		(atanhl) ARGS((fp_t));
EXTERN fp_t		(atanl) ARGS((fp_t));
EXTERN fp_t		(cbrtl) ARGS((fp_t));
EXTERN fp_t		(ceill) ARGS((fp_t));
EXTERN fp_t		(copysignl) ARGS((fp_t, fp_t));
EXTERN fp_t		(coshl) ARGS((fp_t));
EXTERN fp_t		(cosl) ARGS((fp_t));
EXTERN fp_t		(erfcl) ARGS((fp_t));
EXTERN fp_t		(erfl) ARGS((fp_t));
EXTERN fp_t		(expl) ARGS((fp_t));
EXTERN fp_t		(expm1l) ARGS((fp_t));
EXTERN fp_t		(fabsl) ARGS((fp_t));
EXTERN fp_t		(floorl) ARGS((fp_t));
EXTERN fp_t		(fmodl) ARGS((fp_t, fp_t));
EXTERN fp_t		(hypotl) ARGS((fp_t, fp_t));
EXTERN fp_t		(j0l) ARGS((fp_t));
EXTERN fp_t		(j1l) ARGS((fp_t));
EXTERN fp_t		(jnl) ARGS((int, fp_t));
EXTERN fp_t		(ldexpl) ARGS((fp_t, int));
EXTERN fp_t		(lgammal) ARGS((fp_t));
EXTERN fp_t		(log10l) ARGS((fp_t));
EXTERN fp_t		(logl) ARGS((fp_t));
EXTERN fp_t		(nextafterl) ARGS((fp_t, fp_t));
EXTERN fp_t		(powl) ARGS((fp_t, fp_t));
EXTERN fp_t		(remainderl) ARGS((fp_t, fp_t));
EXTERN fp_t		(rintl) ARGS((fp_t));
EXTERN fp_t		(scalbl) ARGS((fp_t, fp_t));
EXTERN fp_t		(sinhl) ARGS((fp_t));
EXTERN fp_t		(sinl) ARGS((fp_t));
EXTERN fp_t		(sqrtl) ARGS((fp_t));
EXTERN fp_t		(tanhl) ARGS((fp_t));
EXTERN fp_t		(tanl) ARGS((fp_t));
EXTERN fp_t		(truncl) ARGS((fp_t));
EXTERN fp_t		(y0l) ARGS((fp_t));
EXTERN fp_t		(y1l) ARGS((fp_t));
EXTERN fp_t		(ynl) ARGS((int, fp_t));
EXTERN int		(ilogbl) ARGS((fp_t));
#endif /* defined(HAVE_FP_T_QUADRUPLE) */

#if defined(HAVE_FP_T_SINGLE)
/* Some systems lack one or more of these, either prototypes header
   files, or functions in runtime libraries */
EXTERN fp_t		(acoshf) ARGS((fp_t));
EXTERN fp_t		(acosf) ARGS((fp_t));
EXTERN fp_t		(asinhf) ARGS((fp_t));
EXTERN fp_t		(asinf) ARGS((fp_t));
EXTERN fp_t		(atanhf) ARGS((fp_t));
EXTERN fp_t		(atanf) ARGS((fp_t));
EXTERN fp_t		(cbrtf) ARGS((fp_t));
EXTERN fp_t		(ceilf) ARGS((fp_t));
EXTERN fp_t		(copysignf) ARGS((fp_t, fp_t));
EXTERN fp_t		(coshf) ARGS((fp_t));
EXTERN fp_t		(cosf) ARGS((fp_t));
EXTERN fp_t		(erfcf) ARGS((fp_t));
EXTERN fp_t		(erff) ARGS((fp_t));
EXTERN fp_t		(expf) ARGS((fp_t));
EXTERN fp_t		(expm1f) ARGS((fp_t));
EXTERN fp_t		(fabsf) ARGS((fp_t));
EXTERN fp_t		(floorf) ARGS((fp_t));
EXTERN fp_t		(fmodf) ARGS((fp_t, fp_t));
EXTERN fp_t		(hypotf) ARGS((fp_t, fp_t));
EXTERN fp_t		(j0f) ARGS((fp_t));
EXTERN fp_t		(j1f) ARGS((fp_t));
EXTERN fp_t		(jnf) ARGS((int, fp_t));
EXTERN fp_t		(ldexpf) ARGS((fp_t, int));
EXTERN fp_t		(lgammaf) ARGS((fp_t));
EXTERN fp_t		(log10f) ARGS((fp_t));
EXTERN fp_t		(logf) ARGS((fp_t));
EXTERN fp_t		(nextafterf) ARGS((fp_t, fp_t));
EXTERN fp_t		(powf) ARGS((fp_t, fp_t));
EXTERN fp_t		(remainderf) ARGS((fp_t, fp_t));
EXTERN fp_t		(rintf) ARGS((fp_t));
EXTERN fp_t		(scalbf) ARGS((fp_t, fp_t));
EXTERN fp_t		(sinhf) ARGS((fp_t));
EXTERN fp_t		(sinf) ARGS((fp_t));
EXTERN fp_t		(sqrtf) ARGS((fp_t));
EXTERN fp_t		(tanhf) ARGS((fp_t));
EXTERN fp_t		(tanf) ARGS((fp_t));
EXTERN fp_t		(truncf) ARGS((fp_t));
EXTERN fp_t		(y0f) ARGS((fp_t));
EXTERN fp_t		(y1f) ARGS((fp_t));
EXTERN fp_t		(ynf) ARGS((int, fp_t));
EXTERN int		(ilogbf) ARGS((fp_t));
#endif /* defined(HAVE_FP_T_SINGLE) */

#if defined(__APPLE__)
EXTERN double		(trunc) ARGS((double)); /* missing from system headers, but in -lm */
#endif
