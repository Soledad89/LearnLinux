#include "hoc.h"
#include "xtab.h"

#if defined(HAVE_CTYPE_H)
#include <ctype.h>
#endif

#if defined(HAVE_STDIO_H)
#include <stdio.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#include "readline.h"
#undef getc
#define getc(fpin)			irl_getchar(fpin)
#undef ungetc
#define ungetc(c,fpin)			irl_unget_char(c)
#undef fscanf
#define fscanf(fpin,format,pdata)	irl_fscanf(fpin,format,pdata)

#undef MAX			/* because some systems define this already */
#define MAX(a,b)	(((a) > (b)) ? (a) : (b))

/* Access functions (macros) for Datum variables to conceal representation */
/* [27-Jul-2002] In setdatum{num,str,sym}(), zero the other union
   member so that when the member sizes differ, all bits are initially
   zero.  Otherwise, when sizeof(void*) > sizeof(fp_t), garbage bits
   can be left and later interpreted as part of a memory
   address. Also, because setdatum{num,str,sym}() can be called with
   input and output arguments that refer to the same Datum, we need to
   be careful to save the RHS value in a temporary, before clearing
   the LHS and then doing the assignment. */

static const char *	__tmp_str;
static Symbol *		__tmp_sym;
static fp_t		__tmp_val;
static Datum		*__tmp_dp;

/* NB: Critical design constraint: getxxx() and setxxx() may reference
   their arguments only once, so that they may contain side
   effects. */

#define ensurenum(d) do {if (getdatumtype(d) == STRVAR) setdatumnum(d, StoF(getdatumstr(d)));} while (0)
#define ensurestr(d) do {if (getdatumtype(d) == VAR)    setdatumstr(d, FtoS(getdatumnum(d)));} while (0)

#define getdatumtype(d)			((d).type)
#define setdatumtype(d,t)		((d).type = t)

#define getdatumnum(d)			((d).u.val)
#define setdatumnum(d,v)		(__tmp_val = v, __tmp_dp = &d, \
					__tmp_dp->u.str = (const char*)NULL, \
					__tmp_dp->u.val = __tmp_val, \
					setdatumtype(*__tmp_dp,VAR))

#define getdatumstr(d)			((d).u.str)
#define setdatumstr(d,s)		(__tmp_str = s, __tmp_dp = &d, \
					__tmp_dp->u.val = FP(0.0), \
					 __tmp_dp->u.str = __tmp_str, \
					setdatumtype(*__tmp_dp,STRVAR))

#define getdatumsym(d)			((d).u.sym)
#define setdatumsym(d,s)		(__tmp_sym = s, __tmp_dp = &d, \
					__tmp_dp->u.val = FP(0.0), \
					__tmp_dp->u.sym = __tmp_sym, \
					__tmp_dp->type = __tmp_dp->u.sym->type)

#define getdatumsymimmutable(d)		(getdatumsym(d)->immutable)
#define setdatumsymimmutable(d,v)	(getdatumsym(d)->immutable = v)

#define getdatumsymname(d)		(getdatumsym(d)->name)
#define setdatumsymname(d,s)		(getdatumsym(d)->name = s)

#define getdatumsymstr(d)		(getdatumsym(d)->u.str)
#define setdatumsymstr(d,s)		(getdatumsym(d)->u.str = s)

#define getdatumsymtype(d)		(getdatumsym(d)->type)
#define setdatumsymtype(d,t)		(__tmp_dp = &d, \
					getdatumsym(*__tmp_dp)->type = t, \
					setdatumtype(*__tmp_dp, t))

#define getdatumsymval(d)		(getdatumsym(d)->u.val)
#define setdatumsymval(d,v)		(getdatumsym(d)->u.val = v)

#define getframesp(fp)			(fp->sp)
#define setframesp(fp,v)		(fp->sp = v)

#define getframenargs(fp)		(fp->nargs)
#define setframenargs(fp,v)		(fp->nargs = v)

#define getframeretpc(fp)		(fp->retpc)
#define setframeretpc(fp,v)		(fp->retpc = v)

#define getframeargn(fp,i)		(fp->argn[i])
#define setframeargn(fp,v)		(fp->argn = v)

#define getsymdefn(s)			(s->u.defn)
#define setsymdefn(s,t)			(s->u.defn = t)
#define getsymname(s)			(s->name)

#define getsymnum(s)			(s->u.val)
#define setsymnum(s,t)			(s->u.val = t)

#define getsymstr(s)			(s->u.str)
#define setsymstr(s,t)			(s->u.str = t)

#define getsymtype(s)			(s->type)
#define setsymtype(s,t)			(s->type = t)

Datum *stack = (Datum*)NULL;	/* the stack */
static size_t max_stack = 0;		/* current size of stack[] */
Datum *stackp;			/* next free spot on stack */

Inst	*prog = (Inst *)NULL;		/* the machine */
size_t	max_prog = 0;			/* current size of prog[] */
Inst	*progp;		/* next free spot for code generation */
static Inst	*pc;		/* program counter during execution */
Inst	*progbase = (Inst *)NULL; /* start of current subprogram */
static int	returning;	/* 1 if return stmt seen */
static int	breaking;	/* 1 if break stmt seen */
static int	continuing;	/* 1 if continue stmt seen */

#if defined(DEBUG_CODE)
extern const char *code_argname;	/* defined in hoc.y */
#define MAX_CODE_NAME_TABLE	1000
static const char *code_name_table[MAX_CODE_NAME_TABLE];
extern int	debug_code;	/* defined in hoc.y */
#endif

extern int	indef;		/* 1 if parsing a func or proc */
extern int	inloop;		/* 1 if parsing loop body */
extern FILE *fin;
extern FILE *fplog;
extern int logfile_enabled;		/* defined in hoc.y */
extern const char *	yygetstr ARGS((void));
extern Symbol *		yygetid ARGS((void));

typedef struct Frame {	/* proc/func call stack frame */
	Symbol	*sp;	/* symbol table entry */
	Inst	*retpc;	/* where to resume after return */
	Datum	*argn;	/* n-th argument on stack */
	long	nargs;	/* number of arguments */
} Frame;

static Datum pop ARGS((void));
static Datum popstr ARGS((void));
static Datum popnum ARGS((void));
static void prchar ARGS((FILE *, int, const char *, int *));
static void pronechar ARGS((FILE *, int, int *));
static void prtext3 ARGS((FILE *, const char *, int, const char *, int *));
static void push ARGS((Datum));
static void pushnum ARGS((fp_t));
static void pushstr ARGS((const char *));
static void sprfnum ARGS((fp_t));
static void sprftext ARGS((const char *));
static void sprnum ARGS((fp_t));
static void sprtext ARGS((const char *));
static void verify ARGS((Symbol*));

#define MAX_ITEM_WIDTH 10240

/* 1999 Standard C defines a long long data type with format length
 * modifier ll for d, i, o, u, x, and X descriptors, and a long double
 * data type with format length modifier L for e, E, f, g, and G
 * descriptors.  Older C implementations may also support long long,
 * but some use a modifier L instead of ll, and others (e.g.,
 * Compaq/DEC OSF/1 4.0) partially support it, but restrict results to
 * long.  We therefore have three variations to deal with in our
 * support of printf with signed and unsigned integer format
 * descriptors.
 */
#if defined(HAVE_LONG_LONG_FORMAT_LL)
#define FMT_L	"ll"
#define LONG_LONG_DATA	long long
#elif defined(HAVE_LONG_LONG_FORMAT_L)
#define FMT_L	"L"
#define LONG_LONG_DATA	long long
#else
#define FMT_L	"l"
#define LONG_LONG_DATA	long
#endif

typedef enum {
	FMT_CHARACTER,
	FMT_FLOAT,
	FMT_SIGNED_INTEGER,
	FMT_UNSIGNED_INTEGER,
	FMT_STRING,
	FMT_UNKNOWN
} fmt_kind;

typedef struct fmt
{
	const char *str;
	fmt_kind kind;
} fmt_t;

static fmt_t *fmts = (fmt_t *)NULL;
static size_t maxfmts = 0;
static size_t nfmt = 0;
static size_t this_fmt = 0;

static size_t	max_frame = 0;		/* current size of frame[] */
Frame *		frame = (Frame *)NULL;
Frame *		fp;			/* frame pointer */

static void
add_fmt_item(const char *s, fmt_kind kind)
{
	if (nfmt >= maxfmts)
		fmts = (fmt_t*)egrow(fmts, &maxfmts, sizeof(fmt_t));
	fmts[nfmt].str = dupstr(s);
	fmts[nfmt].kind = kind;
	nfmt++;
}

static void
discard_string(Datum d)
{
	efree((void*)getdatumstr(d));
}

static void
fmt_free_nth(size_t n)
{
	efree((void*)fmts[n].str);
	fmts[n].str = (const char *)NULL;
	fmts[n].kind = FMT_UNKNOWN;
}

static void
fmt_free(void)
{
	size_t n;

	for (n = 0; n < nfmt; ++n)
		fmt_free_nth(n);

	this_fmt = 0;
	nfmt = 0;
}

static void
make_fmt_strings(const char *fmtstr)
{					/* s is modified temporarily, but restored on return */
	char csave;
	char *original_s;
	char *p;
	char *s;
	fmt_kind kind;

	if (fmtstr == (const char*)NULL)
		execerror("internal error: NULL printf format",(const char*)NULL);

	original_s = s = (char*)dupstr(fmtstr);
	nfmt = 0;
	this_fmt = 0;
	kind = FMT_UNKNOWN;

	for (p = s; *s; ++s)
	{
		if (s[0] == '%')
		{
			if (s[1] == '%')
				++s;
			else
				break;
		}
	}

	csave = *s;
	*s = '\0';
	add_fmt_item(p, FMT_UNKNOWN);
	*s = csave;

	p = s;
	while (*s == '%')
	{
		char *tmpstr;
		size_t n;

		/* match %[-+0 ]?[0-9]*([.][0-9]*)?[cdeEfgGiosuxX] */

		++s;			/* skip over percent */
		if (strchr("-+0 ", (int)(*s)) != (char *)NULL) /* match  [-+0 ]? */
			++s;
		while (isdigit(*s))	/* match [0-9]* */
			++s;
		if (*s == '.')		/* match [.][0-9]* */
		{
			++s;
			while (isdigit(*s))	/* match [0-9]* */
				++s;
		}
		if (strchr("cdeEfgGiosuxX", (int)*s) == (char *)NULL)
		{
			s[1] = '\0';
			execerror("invalid format item", p);
		}

		tmpstr = (char*)concat2(p,"  "); /* extra space for possible length modifier */
		n = (int)(s - p);
		if (strchr("c", (int)*s) != (char*)NULL)
			kind = FMT_CHARACTER;
		else if (strchr("eEfgG", (int)*s) != (char*)NULL)
		{
			kind = FMT_FLOAT;
#if defined(HAVE_FP_T_QUADRUPLE)
			tmpstr[n++] = 'L';
#endif
		}
		else if (strchr("di", (int)*s) != (char*)NULL)
		{
			kind = FMT_SIGNED_INTEGER;
			(void)strlcpy(&tmpstr[n], FMT_L, strlen(tmpstr)-(size_t)n);
			n += (int)strlen(FMT_L);
		}
		else if (strchr("ouxX", (int)*s) != (char*)NULL)
		{
			kind = FMT_UNSIGNED_INTEGER;
			(void)strlcpy(&tmpstr[n], FMT_L, strlen(tmpstr)-(size_t)n);
			n += (int)strlen(FMT_L);
		}
		else if (strchr("s", (int)*s) != (char*)NULL)
			kind = FMT_STRING;
		else
		{
			s[1] = '\0';
			execerror("invalid format item", p);
		}
		tmpstr[n++] = *s;

		for (++s; *s; ++s)	/* include trailing non-format-item text */
		{
			if (s[0] == '%')
			{
				if (s[1] == '%')
				{
					tmpstr[n++] = '%';
					++s;
				}
				else
					break;
			}
			tmpstr[n++] = *s;
		}
		tmpstr[n] = '\0';
		add_fmt_item(tmpstr, kind);
		efree((void*)tmpstr);
		p = s;
	}
	efree((void*)original_s);
}

static void
prfnum(fp_t value)
{
	char buffer[MAX_ITEM_WIDTH];

	switch (fmts[this_fmt].kind)
	{
	case FMT_CHARACTER:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, (int)value);
		prtext(buffer);
		break;

	case FMT_FLOAT:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, value);
		prtext(buffer);
		break;

	case FMT_SIGNED_INTEGER:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str,
			  (LONG_LONG_DATA)value);
		prtext(buffer);
		break;

	case FMT_STRING: /* useful extension: allow numeric output on %s formats! */
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, fmtnum(value));
		prtext(buffer);
		break;

	case FMT_UNSIGNED_INTEGER:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str,
			  (unsigned LONG_LONG_DATA)value);
		prtext(buffer);
		break;

	default:
		{
			const char *t;

			t = dupstr(fmts[this_fmt].str);
			fmt_free();
			execerror("value datatype does not match format", t); /* memory leak at t! */
		}
	}
	fmt_free_nth(this_fmt++);
}

static void
prftext(const char *s)
{
	char buffer[MAX_ITEM_WIDTH];
	char *endptr;
	fp_t value;

	switch (fmts[this_fmt].kind)
	{
	case FMT_FLOAT:
		value = strton(s, &endptr);
		if (*endptr == '\0')
		{
			SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, value);
			prtext(buffer);
		}
		else
			execerror("failed to convert to floating-point from string", s);
		break;

	case FMT_SIGNED_INTEGER:
		value = strton(s, &endptr);
		if (*endptr == '\0')
		{
			SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, (LONG_LONG)value);
			prtext(buffer);
		}
		else
			execerror("failed to convert to signed integer from string", s);
		break;

	case FMT_STRING:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, s);
		prtext(buffer);
		break;

	case FMT_UNSIGNED_INTEGER:
		value = strton(s, &endptr);
		if (*endptr == '\0')
		{
			SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str,
				  (UNSIGNED_LONG_LONG)(LONG_LONG)value);
			prtext(buffer);
		}
		else
			execerror("failed to convert to unsigned integer from string", s);
		break;

	default:
		{
			const char *t;

			t = dupstr(fmts[this_fmt].str);
			fmt_free();
			execerror("value datatype does not match format", t); /* memory leak at t! */
		}
	}
	fmt_free_nth(this_fmt++);
}

void
breakstmt(void) 	/* break from a loop */
{
	breaking = 1;
}

void
continuestmt(void) 	/* continue from a loop */
{
	continuing = 1;
}

fp_t
Defined(Symbol *s)
{
	return ((fp_t)(s->type != UNDEF)); /* return 1 if s is defined, else 0 */
}

fp_t
Delete(Symbol *s)
{
	return (delete_symbol(s));	/* return 1 if delete succeeded, else 0 */
}

const char *
Evalcommands(const char *commands)
{
	if (commands != (const char *)NULL)
	{
		const char *last;

		(void)fflush(stdout);		/* force out any buffered data */
		(void)fflush(stderr);
		if (is_debug("__DEBUG_EVAL__"))
		{
			const char *p = Protect(commands);

			(void)fprintf(stderr,"DEBUG: eval(\"%s\")\n", p);
			efree((void*)p);
		}
		last = strchr(commands,'\0');
		(void)ungetc((int)';', fin); /* semicolon, not newline, to preserve linenumber */
		while (--last >= commands) /* NB: Needs DEEP pushback buffer */
			(void)ungetc((int)*last, fin);
	}
	return (dupstr(""));
}

const char *
fmtnum(fp_t x)
{	/* Return a decimal representation of x in a static buffer
	that will be overwritten on the next call.  Because of
	variations in the handling of Inf and NaN by printf()
	implementations, we must handle them specially. */

	static char buffer[sizeof("+0.123456789012345678901234567890123456789e+9999")];
			/* big enough for 128-bit precision (NB: this
			relies on get_precision() returning a value in
			0..39) */

	if (IsInf(x))
		SNPRINTF4(buffer, sizeof(buffer), "%s", Inf_string(x));
	else if (IsNaN(x))
		SNPRINTF4(buffer, sizeof(buffer), "%s", NaN_string(x));
	else
	{
		if (x == FP(0.0))
		{
			/* [fs][n]printf() on several systems fails to handle
			   signed zero properly, so we do it here. */
			if (Copysign(FP(1.0),x) == FP(-1.0))
				(void)strlcpy(buffer, "-0", sizeof(buffer));
			else
				(void)strlcpy(buffer, "0", sizeof(buffer));
		}
		else
		{
#if defined(HAVE_FP_T_QUADRUPLE)
			if (sizeof(fp_t) == sizeof(double)) /* long double masquerading as double */
				SNPRINTF5(buffer, sizeof(buffer), "%.*g", get_precision(), (double)x);
			else
				SNPRINTF5(buffer, sizeof(buffer), "%.*Lg", get_precision(), (fp_t)x);
#else
			SNPRINTF5(buffer, sizeof(buffer), "%.*g", get_precision(), (double)x);
#endif
		}
	}

	return ((const char*)&buffer[0]);
}

void
initcode(void)
{
	progp = progbase;
	stackp = stack;
	fp = frame;
	breaking = 0;
	continuing = 0;
	returning = 0;
	indef = 0;
	inloop = 0;
}

static void
push(Datum d)
{
	if ((stack == (Datum*)NULL) || (stackp >= &stack[max_stack])) /* then time to grow stack[] */
	{
		size_t old_max_stack;

		old_max_stack = max_stack;
		stack = (Datum *)egrow((void *)stack, &max_stack, sizeof(stack[0]));
		(void)update_const_number("__MAX_STACK__", (fp_t)max_stack);
		stackp = &stack[old_max_stack];
	}
	*stackp++ = d;

#if defined(DEBUG_POP)
	{
		Datum d;
		size_t depth;

		d = stackp[-1];

		depth = (size_t)(stackp - stack);
		(void)fflush(stdout);		/* force out any buffered data */
		(void)fflush(stderr);
		(void)fprintf(stderr,"------------------------------------------------------------\n");
		if (getdatumtype(d) == VAR)
			(void)fprintf(stderr, "push(): stack depth %lu top = [%g @ 0x%08x]\n",
				      (unsigned long)depth, getdatumnum(d), getdatumsym(d));
		else if (getdatumtype(d) == STRVAR)
			(void)fprintf(stderr, "push(): stack depth %lu top = [\"%.39s\" @ 0x%08x]\n",
				      (unsigned long)depth, getdatumstr(d), getdatumsym(d));
		else
			(void)fprintf(stderr, "push(): stack depth %lu top = [type = %d @ 0x%08x]\n",
				      (unsigned long)depth, getdatumtype(d), getdatumsym(d));

		(void)fflush(stderr);
	}
#endif
}

static void
pushnum(fp_t x)
{				/* push a Datum with x */
	Datum d;

	setdatumnum(d, x);
	push(d);
}

static void
pushstr(const char *s)
{				/* push a Datum with a COPY of s */
	Datum d;

	setdatumstr(d, dupstr(s));
	push(d);
}

static Datum
pop(void)
{
#if defined(DEBUG_POP)
	Datum d;

	if (stackp > stack)
	{
		size_t depth;

		d = stackp[-1];

		depth = (size_t)(stackp - stack);
		(void)fflush(stdout);		/* force out any buffered data */
		(void)fflush(stderr);
		if (getdatumtype(d) == VAR)
			(void)fprintf(stderr, "pop():  stack depth %lu top = [%g @ 0x%08x]\n",
				      (unsigned long)depth, getdatumnum(d), getdatumsym(d));
		else if (getdatumtype(d) == STRVAR)
			(void)fprintf(stderr, "pop():  stack depth %lu top = [\"%.39s\" @ 0x%08x]\n",
				      (unsigned long)depth, getdatumstr(d), getdatumsym(d));
		else
			(void)fprintf(stderr, "pop():  stack depth %lu top = [type = %d @ 0x%08x]\n",
				      (unsigned long)depth, getdatumtype(d), getdatumsym(d));

		(void)fflush(stderr);
	}
#endif
	if (stackp == stack)
		execerror("stack underflow", (const char*)NULL);
	return *--stackp;
}

static Datum
popstr(void)
{
	Datum d;

	d = pop();
	ensurestr(d);
	return (d);
}

static Datum
popnum(void)
{
	Datum d;

	d = pop();
	ensurenum(d);
	return (d);
}

void
xpop(void)	/* for when no value is wanted */
{
	Datum d;

	d = pop();
#if 0
	/* Since every string Datum on the stack is supposed to be
	   dynamically allocated, it should be possible to free that
	   memory when one is discarded.  However, when this code is
	   enabled, encrypt() fails badly because memory is freed that
	   is still in use.  I have yet to track down that problem, so
	   for now, this code is disabled.

	   With the free here, there is a bad memory leak here from
	   hoc code like this:

		for (k = 0; k < 100; ++k) "foo"

	   Each "foo" is pushed onto the stack, and then discarded here. */

	if (getdatumtype(d) == STRVAR)
		discard_string(d);
#endif
}

void
constpush(void)
{
	Symbol *s;
	s = (Symbol *)*pc++;
	if (getsymtype(s) == STRVAR)
		pushstr(getsymstr(s));
	else
		pushnum(getsymnum(s));
}

void
varpush(void)
{
	Datum d;

	setdatumsym(d, (Symbol *)(*pc++));
	push(d);
}

void
whilecode(void)
{
	Datum d;
	Inst *savepc = pc;

	execute(savepc+2);	/* condition */
	d = pop();
	while (getdatumnum(d)) {
		execute(*((Inst **)(savepc)));	/* body */
		if (returning)
			break;
		else if (breaking)
			break;
		continuing = 0;		/* ensure that condition is evaluated */
		execute(savepc+2);	/* condition */
		d = pop();
	}
	breaking = 0;
	continuing = 0;
	if (!returning)
		pc = *((Inst **)(savepc+1)); /* next stmt */
}

void
forcode(void)
{
	Datum d;
	Inst *savepc = pc;

	execute(savepc+4);		/* precharge */
	xpop();
	execute(*((Inst **)(savepc)));	/* condition */
	d = pop();
	while (getdatumnum(d)) {
		execute(*((Inst **)(savepc+2)));	/* body */
		if (returning)
			break;
		else if (breaking)
			break;
		continuing = 0; /* ensure that postloop and condition are evaluated */
		execute(*((Inst **)(savepc+1)));	/* post loop */
		xpop();
		execute(*((Inst **)(savepc)));	/* condition */
		d = pop();
	}
	breaking = 0;
	continuing = 0;
	if (!returning)
		pc = *((Inst **)(savepc+3)); /* next stmt */
}

void
ifcode(void)
{
	Datum d;
	Inst *savepc = pc;	/* then part */

	execute(savepc+3);	/* condition */
	d = pop();
	if (getdatumnum(d))
		execute(*((Inst **)(savepc)));
	else if (*((Inst **)(savepc+1))) /* else part? */
		execute(*((Inst **)(savepc+1)));
	if (!returning)
		pc = *((Inst **)(savepc+2)); /* next stmt */
}

void
define(Symbol* sp)	/* put func/proc in symbol table */
{
	setsymdefn(sp, progbase);	/* start of code */
	progbase = progp;	/* next code starts here */
}

void
call(void) 		/* call a function */
{
	Symbol *sp = (Symbol *)pc[0]; /* symbol table entry */
				      /* for function */
	if ((frame == (Frame *)NULL) || (fp >= &frame[max_frame - 1]))
	{			       /* then time to grow frame[] */
		size_t nframe;

		nframe = (fp == (Frame*)NULL) ? 0 : (size_t)(fp - &frame[0]);
		frame = (Frame *)egrow((void *)frame, &max_frame, sizeof(frame[0]));
		(void)update_const_number("__MAX_FRAME__", (fp_t)max_frame);
		fp = &frame[nframe];
	}
	fp++;
	setframesp(fp, sp);
	setframenargs(fp, (long)pc[1]);
	setframeretpc(fp, pc + 2);
	setframeargn(fp, stackp - 1);	/* last argument */
	execute(getsymdefn(sp));
	returning = 0;
}

static void
ret(void) 		/* common return from func or proc */
{
	int i;
	for (i = 0; i < getframenargs(fp); i++)
		xpop();	/* pop arguments */
	pc = (Inst *)getframeretpc(fp);
	--fp;
	returning = 1;
}

void
funcret(void) 	/* return from a function */
{
	Datum d;

	if (getsymtype(getframesp(fp)) == PROCEDURE)
		execerror(getsymname(getframesp(fp)), "(proc) returns value");
	d = pop();	/* preserve function return value */
	ret();
	push(d);
}

void
procret(void) 	/* return from a procedure */
{
	if (getsymtype(getframesp(fp)) == FUNCTION)
		execerror(getsymname(getframesp(fp)),
			"(func) returns no value");
	ret();
}

static fp_t*
getargnum(void) 	/* return pointer to argument and advance pc */
{
	long nargs = (long) *pc++;
	if (nargs > getframenargs(fp))
		execerror(getsymname(getframesp(fp)), "not enough arguments");
	return &getdatumnum(getframeargn(fp, nargs - getframenargs(fp)));
}

static const char *
getargstr(void) 	/* return pointer to argument and advance pc */
{
	long nargs = (long) *pc++;
	if (nargs > getframenargs(fp))
		execerror(getsymname(getframesp(fp)), "not enough arguments");
	return getdatumstr(getframeargn(fp, nargs - getframenargs(fp)));
}

static long
getargtype(void) 	/* return pointer to argument type, WITHOUT advancing pc */
{
	long nargs = (long) *pc;
	if (nargs > getframenargs(fp))
		execerror(getsymname(getframesp(fp)), "not enough arguments");
	return getdatumtype(getframeargn(fp, nargs - getframenargs(fp)));
}

void
arg(void) 	/* push argument onto stack */
{
	long type;

	type = getargtype();
	if (type == VAR)
		pushnum(*getargnum());
	else if (type == STRVAR)
		pushstr(getargstr());	/* TO-DO: track down memory leak from pushed string arguments */
	else
	{
#if 0
		execerror("invalid function/procedure argument type", (const char*)NULL);
#else
		pushnum(*getargnum());
#endif
	}
}

void
argassign(void) 	/* store top of stack in argument */
{
	Datum d;

	d = pop();
	push(d);	/* leave value on stack */
	*getargnum() = getdatumnum(d);
}

void
argaddeq(void) 	/* store top of stack in argument */
{
	pushnum(*getargnum() += getdatumnum(pop()));	/* leave value on stack */
}

void
argsubeq(void) 	/* store top of stack in argument */
{
	pushnum(*getargnum() -= getdatumnum(pop()));	/* leave value on stack */
}

void
argmuleq(void) 	/* store top of stack in argument */
{
	pushnum(*getargnum() *= getdatumnum(pop()));	/* leave value on stack */
}

void
argdiveq(void) 	/* store top of stack in argument */
{
	pushnum(*getargnum() /= getdatumnum(pop()));	/* leave value on stack */
}

void
argmodeq(void) 	/* store top of stack in argument */
{
	Datum d;
	fp_t *x;
	long y;

	d = popnum();
	/* setdatumnum(d, *getargnum() %= getdatumnum(d)); */
	x = getargnum();
	y = (long)*x;
	pushnum(*x = y % (long) getdatumnum(d)); /* leave value on stack */
}

void
bltin0(void)
{
	pushnum(((F0_t)*pc++)());
}

void
bltin1(void)
{
	pushnum(((F1_t)*pc++)(getdatumnum(popnum())));
}

void
bltin1i(void)
{
	pushnum(((F1i_t)pc[0])((Symbol *)(pc[1])));
	pc += 2;
}

void
bltin1s(void)
{
	Datum d;

	d = popstr();
	pushnum(((F1s_t)*pc++)(getdatumstr(d)));
	discard_string(d);
}

void
bltin2(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum(((F2_t)*pc++)(getdatumnum(d1), getdatumnum(d2)));
}

void
bltin2s(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();
	pushnum(((F2s_t)*pc++)(getdatumstr(d1), getdatumstr(d2)));
	discard_string(d1);
	discard_string(d2);
}

void
add(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum(getdatumnum(d1) + getdatumnum(d2));
}

void
sub(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum(getdatumnum(d1) - getdatumnum(d2));
}

void
mul(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum(getdatumnum(d1) * getdatumnum(d2));
}

void
divop(void)
{
	Datum d1, d2;

	d2 = popnum();
#if defined(HAVE_IEEE_754)
	/* zero divide silently produces Inf or NaN */
#else
	if (getdatumnum(d2) == FP(0.0))
		execerror("division by zero", (const char*)NULL);
#endif
	d1 = popnum();
	pushnum(getdatumnum(d1) / getdatumnum(d2));
}

void
mod(void)
{
	Datum d1, d2;
	LONG_LONG x;

	d2 = popnum();
	if (getdatumnum(d2) == FP(0.0))
		execerror("division by zero", (const char*)NULL);
	d1 = popnum();
	/* d1.val %= getdatumnum(d2); */
	x = (LONG_LONG)getdatumnum(d1);
	x %= (LONG_LONG)getdatumnum(d2);
	pushnum((fp_t)x);
}

void
negate(void)
{
	pushnum(-getdatumnum(popnum()));
}

void
noop(void)
{
}

static void
verify(Symbol* s)
{
	if (getsymtype(s) != VAR && getsymtype(s) != UNDEF)
		execerror("attempt to evaluate non-variable", getsymname(s));
	if (getsymtype(s) == UNDEF)
		execerror("undefined variable", getsymname(s));
}

void
eval(void)		/* evaluate variable on stack */
{
	Datum d;

	(void)fflush(stdout);		/* force out any buffered data */
	(void)fflush(stderr);
	d = pop();
#if 0
	verify(getdatumsym(d));
	pushnum(getdatumsymval(d));
#else
	if (getdatumsymtype(d) == STRVAR)
		pushstr(getdatumsymstr(d));
	else
		pushnum(getdatumsymval(d));
#endif
}

void
preinc(void)
{
	Datum d;

	setdatumsym(d, (Symbol *)(*pc++));
	verify(getdatumsym(d));
	pushnum(setdatumsymval(d, getdatumsymval(d) + FP(1.0)));
}

void
predec(void)
{
	Datum d;

	setdatumsym(d, (Symbol *)(*pc++));
	verify(getdatumsym(d));
	pushnum(setdatumsymval(d, getdatumsymval(d) - FP(1.0)));
}

void
postinc(void)
{
	Datum d;
	fp_t v;

	setdatumsym(d, (Symbol *)(*pc++));
	verify(getdatumsym(d));
	v = getdatumsymval(d);
	setdatumsymval(d, getdatumsymval(d) + FP(1.0));
	pushnum(v);
}

void
postdec(void)
{
	Datum d;
	fp_t v;

	setdatumsym(d, (Symbol *)(*pc++));
	verify(getdatumsym(d));
	v = getdatumsymval(d);
	setdatumsymval(d, getdatumsymval(d) - FP(1.0));
	pushnum(v);
}

void
gt(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_gt();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(0.0));
		return;
	}
#endif

	pushnum((fp_t)(getdatumnum(d1) > getdatumnum(d2)));
}

void
lt(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_lt();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(0.0));
		return;
	}
#endif

	pushnum((fp_t)(getdatumnum(d1) < getdatumnum(d2)));
}

void
ltgt(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_ne();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(0.0));
		return;
	}
#endif

	pushnum((fp_t)( (getdatumnum(d1) < getdatumnum(d2)) ||
			(getdatumnum(d1) > getdatumnum(d2)) ));
}

void
ge(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_ge();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(0.0));
		return;
	}
#endif

	pushnum((fp_t)(getdatumnum(d1) >= getdatumnum(d2)));
}

void
le(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_le();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(0.0));
		return;
	}
#endif

	pushnum((fp_t)(getdatumnum(d1) <= getdatumnum(d2)));
}

void
eq(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_eq();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(0.0));
		return;
	}
#endif

	pushnum((fp_t)(getdatumnum(d1) == getdatumnum(d2)));
}

void
ne(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();

	if ( (getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR) )
	{
		push(d2);
		push(d1);
		str_ne();
		return;
	}

	ensurenum(d1);
	ensurenum(d2);

#if defined(HAVE_NAN_BOTCH)
	if (IsNaN(getdatumnum(d1)) || IsNaN(getdatumnum(d2)))
	{
		pushnum(FP(1.0));
		return;
	}
#endif

	pushnum((fp_t)(getdatumnum(d1) != getdatumnum(d2)));
}

void
And(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum((fp_t)(getdatumnum(d1) != FP(0.0) && getdatumnum(d2) != FP(0.0)));
}

void
Or(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum((fp_t)(getdatumnum(d1) != FP(0.0) || getdatumnum(d2) != FP(0.0)));
}

void
Not(void)
{
	pushnum((fp_t)(getdatumnum(popnum()) == FP(0.0)));
}

void
power(void)
{
	Datum d1, d2;

	d2 = popnum();
	d1 = popnum();
	pushnum(Pow(getdatumnum(d1), getdatumnum(d2)));
}

void
assign(void)
{
	Datum d1, d2;

	d1 = pop();			/* lhs variable */
	d2 = pop();			/* rhs value */

	/* Until some grammar issues are worked out, convert an
	   assignment of a string value to a string assignment. */
	if (getdatumtype(d2) == STRVAR)
	{
		push(d2);
		push(d1);
		str_assign();
		return;
	}

	if ((getdatumsymtype(d1) != STRVAR) &&
	    (getdatumsymtype(d1) != VAR) &&
	    (getdatumsymtype(d1) != UNDEF))
		execerror("assignment to non-variable", getdatumsymname(d1));
	if (getdatumsymimmutable(d1))
		execerror("illegal reassignment to immutable named constant",
			  getdatumsymname(d1));
	setdatumsymval(d1, getdatumnum(d2));
	setdatumsymtype(d1, VAR);
	push(d2);
}

void
addeq(void)
{
	Datum d1, d2;

	d1 = pop();
	d2 = popnum();
	if (getdatumsymtype(d1) != VAR && getdatumsymtype(d1) != UNDEF)
		execerror("assignment to non-variable",
			getdatumsymname(d1));
	setdatumnum(d2, setdatumsymval(d1, getdatumsymval(d1) + getdatumnum(d2)));
	setdatumsymtype(d1, VAR);
	push(d2);
}

void
subeq(void)
{
	Datum d1, d2;

	d1 = pop();
	d2 = popnum();
	if (getdatumsymtype(d1) != VAR && getdatumsymtype(d1) != UNDEF)
		execerror("assignment to non-variable",
			getdatumsymname(d1));
	setdatumnum(d2, setdatumsymval(d1, getdatumsymval(d1) - getdatumnum(d2)));
	setdatumsymtype(d1, VAR);
	push(d2);
}

void
muleq(void)
{
	Datum d1, d2;

	d1 = pop();
	d2 = popnum();
	if (getdatumsymtype(d1) != VAR && getdatumsymtype(d1) != UNDEF)
		execerror("assignment to non-variable",
			getdatumsymname(d1));
	setdatumnum(d2, setdatumsymval(d1, getdatumsymval(d1) * getdatumnum(d2)));
	setdatumsymtype(d1, VAR);
	push(d2);
}

void
diveq(void)
{
	Datum d1, d2;

	d1 = pop();
	d2 = popnum();
	if (getdatumsymtype(d1) != VAR && getdatumsymtype(d1) != UNDEF)
		execerror("assignment to non-variable",
			getdatumsymname(d1));
	setdatumnum(d2, setdatumsymval(d1, getdatumsymval(d1) / getdatumnum(d2)));
	setdatumsymtype(d1, VAR);
	push(d2);
}

void
modeq(void)
{
	Datum d1, d2;
	long x;

	d1 = pop();
	d2 = popnum();
	if (getdatumsymtype(d1) != VAR && getdatumsymtype(d1) != UNDEF)
		execerror("assignment to non-variable",
			getdatumsymname(d1));
#if 0
	setdatumnum(d2, setdatumsymval(d1, getdatumsymval(d1) % getdatumnum(d2)));
#else
	x = (long)getdatumsymval(d1);
	x %= (long) getdatumnum(d2);
	setdatumnum(d2, setdatumsymval(d1, x));
#endif
	setdatumsymtype(d1, VAR);
	push(d2);
}

static void
prchar(FILE *fpout, int c, const char *indent, int *pcolumn)
{
	if (*pcolumn == 0)		/* output indent string at start of line */
	{
		while (*indent)
			pronechar(fpout, *indent++, pcolumn);
	}
	pronechar(fpout, c, pcolumn);
}

void
printtop(void)	/* pop top value from stack, print it */
{
	Datum d;
	static Symbol *s;	/* last value computed */

	d = pop();

	/* Until I solve a grammar problem, this function can be
	   called with string data as well as numeric data, so divert
	   to printtopstring() if necessary. */
	if (getdatumtype(d) == STRVAR)
	{
		push(d);
		printtopstring();
		return;
	}

	if (s == (Symbol*)NULL)		/* first time only */
		s = update_const_number("_", FP(0.0));
	prnum(getdatumnum(d));
	prnl();
	setsymnum(s, getdatumnum(d));
}

void
printtopstring(void)	/* pop top string value from stack, print it */
{
	Datum d;
	static Symbol *s;		/* last value computed */
	const char *p;

	if (s == (Symbol*)NULL)		/* first time only */
		s = update_const_string("__", "");
	d = popstr();
	p = getdatumstr(d);
	if ((p != (const char*)NULL) && (*p != '\0'))
	{				/* print only nonempty strings from string expressions */
		prtext(p);
		prnl();
	}
	(void)set_string(s, p);
	discard_string(d);
}

void
prexpr(void)	/* print numeric value */
{
	Datum d;

	d = pop();

	/* Until I solve a grammar problem, this function can be
	   called with string data as well as numeric data, so divert
	   to prstr() if necessary. */
	if (getdatumtype(d) == STRVAR)
	{
		push(d);
		prstr();
	}
	else
	{
		if (this_fmt < nfmt)
			prfnum(getdatumnum(d));
		else
			prnum(getdatumnum(d));
	}
}

void
prfmt(void)			/* parse printf format string */
{
	Datum d;

	 /* The extended hoc statement (see hoc.y)
	  *
	  *	 *printf "format", item1, item2, ...,
	  *
	  * generates a list of code entries containing
	  *
	  *	prfmt (prexpr|prstr)*
	  *
	  * The hoc print and println statements generate similar code
	  * lists, but without the leading prfmt item.
	  *
	  * We handle this by having prfmt() call make_fmt_strings() to
	  * parse the format string into an array of format objects, one
	  * for each item.  The format objects contain an snprintf()
	  * format directive, and a corresponding data type code.
	  *
	  * Each format object contains text from the beginning of the
	  * format directive, plus any following non-directive text.
	  * The first, possibly empty, format object contains any text
	  * that precedes the first format directive; that object is
	  * handled here.
	  *
	  * Later executions of prexpr and prstr test whether there is a
	  * format available for the value, and if so, snprintf() is
	  * called to do the conversion.  Otherwise, they fall back to
	  * printing with a default format.
	  *
	  * The format data type codes are checked for compatibility
	  * with the item to be printed, to prevent calling snprintf()
	  * with illegal arguments.
	  *
	  * The advantage of this approach is that almost the full power
	  * of snprintf() is available for formatting numeric data (only
	  * the asterisk for field width and precision, and length
	  * modifiers, are not supported), without actually having to
	  * reimplement snprintf(), and that excess data items are
	  * handled with default formatting, without losing them (as
	  * most C printf() implementations do), and without causing any
	  * aberrant behavior.
	  *
	  * Except for the somewhat complicated code in
	  * make_fmt_strings(), this gives a reasonably clean way to
	  * implement hoc's printf, at the small cost of a single test
	  * over the original code that implemented print and println.
	  */

	d = popstr();
	make_fmt_strings(getdatumstr(d));
	discard_string(d);

#if defined(DEBUG_PRFMT)
	{
		size_t k;
		for (k = 0; k < nfmt; ++k)
		{
			prtext("k = ");
			prnum((fp_t)k);
			prtext(" ");
			prnum((fp_t)k);
			prtext(" type = ");
			prnum((fp_t)fmts[k].kind);
			prtext(" fmt = [");
			prtext(fmts[k].str);
			prtext("]");
			prnl();
		}
	}
#endif

	prtext(fmts[this_fmt].str);	/* print leading non-directive text */
	fmt_free_nth(this_fmt++);
}

void
prnl(void)
{
	prtext("\n");
}

void
prnum(fp_t x)
{
	prtext(fmtnum(x));
}

static void
pronechar(FILE *fpout, int c, int *pcolumn)
{
	(void)fputc(c, fpout);
	if (c == '\n')
		*pcolumn = 0;
	else
		(*pcolumn)++;
}

void
prsep(void)		/* print output field separator */
{
	prtext(lookup("__OFS__")->u.str);
}

void
prstr(void)		/* print string value */
{
	Datum d;

	d = popstr();
	if (this_fmt < nfmt)
		prftext(getdatumstr(d));
	else
		prtext(getdatumstr(d));
	discard_string(d);
}

void
prtext(const char *s)
{	/* print s to stdout, and optionally, to fplog */
	prtext2(s,0);
}

void
prtext2(const char *s, int min_width)
{	/* print s to stdout, and optionally, to fplog, left-adjusted in a
	   field of minimum width min_width */

	const char *indent = lookup("__INDENT__")->u.str;
	int nblanks;
	static const char *log_indent = "#-> ";
	static int log_col = 0;
	static int out_col = 0;

	nblanks = MAX(0, min_width - (int)strlen(s));

	prtext3(stdout, s, nblanks, indent, &out_col);

	if (logfile_enabled && (fplog != (FILE*)NULL))
		prtext3(fplog, s, nblanks, log_indent, &log_col);
}

static void
prtext3(FILE *fpout, const char *s, int nblanks, const char *indent, int *pcolumn)
{
	const char *t;
	int k;

	for (t = s; *t; ++t)
		prchar(fpout, (int)*t, indent, pcolumn);

	for (k = 0; k < nblanks; ++k)
		prchar(fpout, (int)' ', indent, pcolumn);
}

void
varread(void)	/* read into variable */
{
	Datum d;
	Symbol *var = (Symbol *) *pc++;
	int c;

	/* Lookahead past space to see whether we have a string or a number */
	for (c = getc(fin); isspace(c); c = getc(fin))
		/* NO-OP */;
	(void)ungetc(c,fin);

	if (c == '"')			/* expect "quoted string" */
	{
		if (getsymtype(var) == VAR)	/* perhaps we will rescind this restriction in the future */
			execerror("illegal assignment of string to existing numeric variable", getsymname(var));
		if (getsymtype(var) != STRVAR && getsymtype(var) != UNDEF)
			execerror("assignment to non-variable", getsymname(var));

		(void)getc(fin);	/* discard the opening quote */
		clearerr(fin);		/* needed to clear eof flag! */
		(void)update_string(getsymname(var), yygetstr());
		setdatumnum(d, (fp_t)(!feof(fin)));
	}
	else if (IsIdStart(c))			/* expect variable */
	{
		Symbol *s;

		s = yygetid();

		if (getsymtype(var) != STRVAR && getsymtype(var) != VAR && getsymtype(var) != UNDEF)
			execerror("assignment to non-variable", getsymname(var));

		if (getsymtype(s) != STRVAR && getsymtype(s) != VAR)
			execerror("assignment from non-variable", getsymname(s));

		if (getsymtype(var) == UNDEF)		/* creating new symbol */
			setsymtype(var, getsymtype(s));

		/* perhaps we will rescind this restriction in the future */
		if (getsymtype(var) == VAR && getsymtype(s) == STRVAR)
			execerror("illegal assignment of string to existing numeric variable", getsymname(var));
		else if (getsymtype(var) == STRVAR && getsymtype(s) == VAR)
			execerror("illegal assignment of number to existing string variable", getsymname(var));

		if (getsymtype(var) == VAR)
			(void)update_number(getsymname(var), getsymnum(s));
		else if (getsymtype(var) == STRVAR)
			(void)update_string(getsymname(var), getsymstr(s));
		else
			execerror("internal type confusion in read() of", getsymname(var));
		setdatumnum(d, (fp_t)(!feof(fin)));
	}
	else				/* expect number */
	{
		fp_t v;
		if (getsymtype(var) == STRVAR)	/* perhaps we will rescind this restriction in the future */
			execerror("illegal assignment of number to existing string variable", getsymname(var));
		if (getsymtype(var) != VAR && getsymtype(var) != UNDEF)
			execerror("assignment to non-variable", getsymname(var));

	  Again:
		switch (fscanf(fin, "%lf", &v)) { /* NB: this is really irl_fscanf(), which handles long double case correctly */
		case EOF:
			if (moreinput())
				goto Again;
			setdatumnum(d, v = FP(0.0));
			break;
		case 0:
			execerror("non-number read into", getsymname(var));
			break;
		default:
			setdatumnum(d, FP(1.0));
			break;
		}
		(void)update_number(getsymname(var), v);
	}
	push(d);
}

Inst*
code(Inst f)	/* install one instruction or operand */
{
	Inst *oprogp = progp;
	if ((prog == (Inst *)NULL) || (progp >= &prog[max_prog]))
	{
#if 1
		execerror("program too big", (const char*)NULL);
#else  /* code disabled until external pointer issues resolves */

		size_t old_max_prog;
		size_t old_pc_offset;

		if (pc != (Inst*)NULL)
			old_pc_offset = (size_t)(pc - &prog[0]);
		old_max_prog = max_prog;
		progbase = prog = (Inst *)egrow((Inst *)prog, &max_prog, sizeof(prog[0]));
		(void)update_const_number("__MAX_PROG__", (fp_t)max_prog);
		oprogp = progp = &prog[old_max_prog];
		if (pc != (Inst*)NULL)
			pc = &prog[old_pc_offset];
#endif
	}

#if defined(DEBUG_CODE)
	if (debug_code)
	{
		size_t n;

		n = (size_t)(progp - progbase);
		(void)fprintf(stderr,"code[%03u] = %16p [%s]\n",
			      n, f, code_argname);
		if (n < MAX_CODE_NAME_TABLE)
			code_name_table[n] = code_argname;
	}
#endif

	*progp++ = f;
	return oprogp;
}

void
const_assign(void)
{
	Datum d;

	d = pop();
	push(d);
	assign();
	make_immutable(getdatumsym(d));	    /* to prevent user reassignment */
}

void
execute(Inst* p)
{
	for (pc = p; (pc != (Inst*)NULL) &&
		     (*pc != STOP) &&
		     !(returning || breaking || continuing); )
	{

#if defined(DEBUG_CODE)
		if (debug_code)
		{
			size_t n;

			n = (size_t)(pc - progbase);
			fprintf(stderr,"exec[%03u] = %16p [%s]\n",
				n, *pc,
				(n < MAX_CODE_NAME_TABLE) ? code_name_table[n] : "????");
		}
#endif

		(*((++pc)[-1]))();
	}
}

void
const_str_push(void)
{
	pushstr(getsymstr(((Symbol *)*pc++)));
}

void
const_str_assign(void)
{
	Datum d;

	d = pop();
	push(d);
	str_assign();
	make_immutable(getdatumsym(d));	    /* to prevent user reassignment */
}

void
sprexpr(void)	/* sprintf numeric value */
{
	Datum d;

	d = pop();

	/* Until I solve a grammar problem, this function can be
	   called with string data as well as numeric data, so divert
	   to sprstr() if necessary. */
	if (getdatumtype(d) == STRVAR)
	{
		push(d);
		sprstr();
	}
	else
	{
		if (this_fmt < nfmt)
			sprfnum(getdatumnum(d));
		else
			sprnum(getdatumnum(d));
	}
}

void
sprfmt(void)			/* parse sprintf format string */
{
	prfmt();
	pushstr("");			/* need for sprjoin() */
}

static void
sprfnum(fp_t value)
{
	char buffer[MAX_ITEM_WIDTH];

	switch (fmts[this_fmt].kind)
	{
	case FMT_CHARACTER:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, (int)value);
		sprtext(buffer);
		break;

	case FMT_FLOAT:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, value);
		sprtext(buffer);
		break;

	case FMT_SIGNED_INTEGER:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str,
			  (LONG_LONG_DATA)value);
		sprtext(buffer);
		break;

	case FMT_STRING: /* useful extension: allow numeric output on %s formats! */
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, fmtnum(value));
		sprtext(buffer);
		break;

	case FMT_UNSIGNED_INTEGER:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str,
			  (unsigned LONG_LONG_DATA)value);
		sprtext(buffer);
		break;

	default:
		{
			const char *t;

			t = dupstr(fmts[this_fmt].str);
			fmt_free();
			execerror("value datatype does not match format", t); /* memory leak at t! */
		}
	}
	fmt_free_nth(this_fmt++);
}

static void
sprftext(const char *s)
{
	char buffer[MAX_ITEM_WIDTH];
	char *endptr;
	fp_t value;

	switch (fmts[this_fmt].kind)
	{
	case FMT_FLOAT:
		value = strton(s, &endptr);
		if (*endptr == '\0')
		{
			SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, value);
			sprtext(buffer);
		}
		else
			execerror("failed to convert to floating-point from string", s);
		break;

	case FMT_SIGNED_INTEGER:
		value = strton(s, &endptr);
		if (*endptr == '\0')
		{
			SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, (LONG_LONG)value);
			sprtext(buffer);
		}
		else
			execerror("failed to convert to signed integer from string", s);
		break;

	case FMT_STRING:
		SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str, s);
		sprtext(buffer);
		break;

	case FMT_UNSIGNED_INTEGER:
		value = strton(s, &endptr);
		if (*endptr == '\0')
		{
			SNPRINTF4(buffer, sizeof(buffer), fmts[this_fmt].str,
				  (UNSIGNED_LONG_LONG)(LONG_LONG)value);
			sprtext(buffer);
		}
		else
			execerror("failed to convert to unsigned integer from string", s);
		break;

	default:
		{
			const char *t;

			t = dupstr(fmts[this_fmt].str);
			fmt_free();
			execerror("value datatype does not match format", t); /* memory leak at t! */
		}
	}
	fmt_free_nth(this_fmt++);
}

void
sprjoin(void)
{
	Datum d1, d2;

	d2 = pop();
	d1 = pop();
	if ((getdatumtype(d1) == STRVAR) && (getdatumtype(d2) == STRVAR))
	{
		const char *p;
		p = concat2(getdatumstr(d1), getdatumstr(d2));
		pushstr(p);
		efree((void*)p);
		discard_string(d1);
		discard_string(d2);
	}
	else
		execerror("internal error: expected two strings on stack",
			  (const char*)NULL);
}

static void
sprnum(fp_t x)
{
	sprtext(fmtnum(x));
}

void
sprstr(void)		/* sprintf string value */
{
	Datum d;

	d = popstr();
	if (this_fmt < nfmt)
		sprftext(getdatumstr(d));
	else
		sprtext(getdatumstr(d));
	discard_string(d);
}

void
sprtext(const char *s)
{
	pushstr(s);
}

void
str_assign(void)
{
	Datum d1, d2;

	d1 = pop();			/* STRVAR or STRARG or VAR */
	d2 = pop();			/* strexpr */
	if ((getdatumsymtype(d1) != VAR) &&
	    (getdatumsymtype(d1) != STRVAR) &&
	    (getdatumsymtype(d1) != UNDEF))
		execerror("assignment to non-variable", getdatumsymname(d1));
	if (getdatumsymimmutable(d1))
		execerror("illegal reassignment to immutable named constant",
			  getdatumsymname(d1));
	if (getdatumsymtype(d1) != STRVAR)	/* change nonstring variable to string variable */
		setdatumsymstr(d1,dupstr(""));
	setdatumsymtype(d1,STRVAR);	/* force to string variable */
	(void)set_string(getdatumsym(d1), getdatumstr(d2));
	pushstr(getdatumstr(d2));
	discard_string(d2);
 	/* NB: No call here to discard_string(d1): d1 is a symbol, not a string! */
}

void
str_concat(void)		/* concatenate number/string objects on stack */
{
	Datum d1, d2;
	const char *p;

	d2 = pop();
	d1 = pop();

	ensurestr(d1);
	ensurestr(d2);

	p = concat2(getdatumstr(d1), getdatumstr(d2));
	pushstr(p);
	efree((void*)p);
	discard_string(d1);
	discard_string(d2);
}

void
str_eq(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();

	pushnum((fp_t)(STREQUAL(getdatumstr(d1),getdatumstr(d2))));
	discard_string(d1);
	discard_string(d2);
}

void
str_ge(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();

	pushnum((fp_t)(strcmp(getdatumstr(d1),getdatumstr(d2)) >= 0));
	discard_string(d1);
	discard_string(d2);
}

void
str_gt(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();

	pushnum((fp_t)(strcmp(getdatumstr(d1),getdatumstr(d2)) > 0));
	discard_string(d1);
	discard_string(d2);
}

void
str_le(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();

	pushnum((fp_t)(strcmp(getdatumstr(d1),getdatumstr(d2)) <= 0));
	discard_string(d1);
	discard_string(d2);
}

void
str_lt(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();

	pushnum((fp_t)(strcmp(getdatumstr(d1),getdatumstr(d2)) < 0));
	discard_string(d1);
	discard_string(d2);
}

void
str_ne(void)
{
	Datum d1, d2;

	d2 = popstr();
	d1 = popstr();

	pushnum((fp_t)(strcmp(getdatumstr(d1),getdatumstr(d2)) != 0));
	discard_string(d1);
	discard_string(d2);
}

void
strarg(void) 	/* push string argument onto stack */
{
	arg();
}

void
strbltin0(void)
{
	const char *p;

	p = ((SF0_t)*pc++)();
	pushstr(p);
	efree((void*)p);
}

void
strbltin1(void)
{
	Datum d;
	const char *p;

	d = popstr();
	p = ((SF1_t)*pc++)(getdatumstr(d));
	pushstr(p);
	efree((void*)p);
	discard_string(d);
}

void
strbltin1n(void)
{
	Datum d;
	const char *p;

	d = popnum();
	p = ((SF1n_t)*pc++)(getdatumnum(d));
	pushstr(p);
	efree((void*)p);
}

void
strbltin2(void)
{
	Datum d1, d2;
	const char *p;

	d2 = popstr();
	d1 = popstr();
	p = ((SF2_t)*pc++)(getdatumstr(d1), getdatumstr(d2));
	pushstr(p);
	efree((void*)p);
	discard_string(d1);
	discard_string(d2);
}

void
strbltin2sn(void)
{
	Datum d1, d2;
	const char *p;

	d2 = popnum();
	d1 = popstr();
	p = ((SF2sn_t)*pc++)(getdatumstr(d1), getdatumnum(d2));
	pushstr(p);
	efree((void*)p);
	discard_string(d1);
}

void
strbltin3snn(void)
{
	Datum d1, d2, d3;
	const char *p;

	d3 = popnum();
	d2 = popnum();
	d1 = popstr();
	p = ((SF3snn_t)*pc++)(getdatumstr(d1), getdatumnum(d2), getdatumnum(d3));
	pushstr(p);
	efree((void*)p);
	discard_string(d1);
}

void
streval(void)		/* evaluate string variable on stack */
{
	Datum d;

	d = pop();
	if (getdatumsymtype(d) != STRVAR)
		execerror("attempt to evaluate nonstring variable", getdatumsymname(d));
	pushstr(getdatumsymstr(d));
}

fp_t
SymtoN(const char *s)
{
	Symbol *sp;

	sp = lookup(s);
	if (sp == (Symbol*)NULL)
	{
		execerror("variable not found", s);
		return (FP(0.0)); /* NOT REACHED */
	}
	else if (getsymtype(sp) == VAR)
		return (getsymnum(sp));
	else if (getsymtype(sp) == STRVAR)
		return (strton(getsymstr(sp), (char**)NULL));
	else				/* cannot happen */
	{
		execerror("illegal object in string-to-numeric-symbol conversion",
			  (const char*)NULL);
		return (FP(0.0)); /* NOT REACHED */
	}
}

const char *
SymtoS(const char *s)
{
	Symbol *sp;

	sp = lookup(s);
	if (sp == (Symbol*)NULL)
	{
		execerror("variable not found", s);
		return (dupstr("")); /* NOT REACHED */
	}
	else if (getsymtype(sp) == VAR)
		return (FtoS(getsymnum(sp)));
	else if (getsymtype(sp) == STRVAR)
		return (dupstr(getsymstr(sp)));
	else				/* cannot happen */
	{
		execerror("illegal object in string-to-string-symbol conversion",
			  (const char*)NULL);
		return (dupstr("")); /* NOT REACHED */
	}
}

const char *
Who(const char *prefix)
{
	dump_syms(prefix);
	return (dupstr(""));
}
