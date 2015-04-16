
# line 2 "hoc.y"
#include "hoc.h"

#define __my_yylex			/* for SGI yacc and C++ */

#if defined(YYDEBUG)
extern int yydebug;
#else
#define YYDEBUG 0	/* so this works with byacc, bison, and yacc */
#endif

/* We need larger internal yacc tables to get consistent results across
   platforms for test/long-operand.hoc */
#define YYMAXDEPTH	8000
#define YYSTACKSIZE	8000

#if defined(HAVE_CTYPE_H)
#include <ctype.h>
#endif

#if defined(HAVE_ERRNO_H)
#include <errno.h>
#endif

#if defined(HAVE_LOCALE_H)
#include <locale.h>
#endif

#if defined(HAVE_SETJMP_H)
#include <setjmp.h>
#endif

#if defined(HAVE_SIGNAL_H)
#include <signal.h>
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" int yylex ARGS((void));
extern "C" void fpecatch ARGS((int dummy));
extern "C" void intcatch ARGS((int dummy));
extern "C" void xcpucatch ARGS((int dummy));
#else
int yylex ARGS((void));
void fpecatch ARGS((int dummy));
void intcatch ARGS((int dummy));
void xcpucatch ARGS((int dummy));
#endif

#if defined(_AIX)
#undef YYCONST
#define YYCONST
EXTERN void yyerror ARGS((YYCONST char *)); /* otherwise used before prototyped on this system, sigh...*/
#endif

#if defined(HAVE_STDIO_H)
#include <stdio.h>
#endif

#if defined(HAVE_STDLIB_H)
#include <stdlib.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if !defined(R_OK)
#define R_OK 04
#endif

#if !defined(HOCRC)
#define HOCRC	".hocrc"
#endif

#if !defined(SYSHOCDIR)
#define SYSHOCDIR	"/usr/local/share/lib/hoc/hoc-" PACKAGE_VERSION "/"
#endif

#if !defined(SYSHOCHLPBASE)
#define SYSHOCHLPBASE	"help.hoc"
#endif

#if !defined(SYSHOCHLP)
#define SYSHOCHLP SYSHOCDIR "/" SYSHOCHLPBASE
#endif

#if !defined(SYSHOCRCBASE)
#define SYSHOCRCBASE	"hoc.rc"
#endif

#if !defined(SYSHOCRC)
#define SYSHOCRC SYSHOCDIR "/" SYSHOCRCBASE
#endif

#if !defined(SYSHOCXLTBASE)
#define SYSHOCXLTBASE	"translations.hoc"
#endif

#if !defined(SYSHOCXLT)
#define SYSHOCXLT SYSHOCDIR "/" SYSHOCXLTBASE
#endif

#if defined(DEBUG_CODE)
const char *code_argname;
int	debug_code = 0;			/* turn on manually in a debugger seesion */
#define code(c)		(code_argname = #c, (code)(c))
#endif

#define	code2(c1,c2)		code(c1); (void)code(c2)
#define	code3(c1,c2,c3)		code2(c1,c2); (void)code(c3)
#define	code4(c1,c2,c3,c4)	code3(c1,c2,c3); (void)code(c4)
#define CODE(c1)		(void)code(c1)
#define CODE2(c1,c2)		(void)code2(c1,c2)
#define CODE3(c1,c2,c3)		(void)code3(c1,c2,c3)
#define CODE4(c1,c2,c3,c4)	(void)code4(c1,c2,c3,c4)
#define QUOTATION_MARK	'"'

extern const char *	elementsep;	/* defined in io.c */

int			indef;			/* needed in code.c too */
int			inloop;			/* needed in code.c too */
const char *		syshocdir = SYSHOCDIR;

static int		this_c = '\n';		/* file global: used in warning(), yygetid(), yylex() */
static int		infile_count = 0;
static int		in_argnamelist = 0;
static int		in_global_stmt = 0;
static int		ndefns = 0;
static size_t		numargs = 0;
static size_t		maxargs = 0;
static size_t		numglobals = 0;
static size_t		maxglobals = 0;
static Symbol **	argnamelist = (Symbol**)NULL;
static Symbol **	globallist = (Symbol**)NULL;
static const char *	hocrc = HOCRC;
static const char *	syshochlp = SYSHOCHLP;
static const char *	syshochlpbase = SYSHOCHLPBASE;
static const char *	syshocrc = SYSHOCRC;
static const char *	syshocrcbase = SYSHOCRCBASE;
static const char *	syshocxlt = SYSHOCXLT;
static const char *	syshocxltbase = SYSHOCXLTBASE;
static const char *	this_procname;

extern FILE *		Fopen ARGS((const char *, const char *, const char **));
extern FILE *		Freopen ARGS((const char *, const char *, FILE*, const char **));

int			main ARGS((int, char *[]));
const char *		yygetstr ARGS((void));		/* used in code.c:varread() */
void			set_filename ARGS((const char *)); /* used in string.c:Load() */

static void		addarg ARGS((Symbol *));
static void		addglobal ARGS((Symbol *));
static void             author_and_die ARGS((void));
static int		backslash ARGS((int));
static void             bump_lineno ARGS((void));
static void             copyright_and_die ARGS((void));
static void             defnonly ARGS((const char *));
static void		do_define ARGS((const char *));
static void		do_one_init_file ARGS((const char *));
static void		do_undefine ARGS((const char *));
static void             do_init_files ARGS((void));
static void             do_post_args ARGS((int *, char *[]));
static void             do_pre_args ARGS((const int *, const char *[]));
static void		error ARGS((const char *, const char *));
static int              follow ARGS((int, int, int));
static void		free_argnamelist ARGS((void));
static void		free_globallist ARGS((void));
static const char *     get_locale_filename ARGS((const char *, const char *));
static const char *     get_locale_name ARGS((void));
static void             help_and_die ARGS((void));
static void             init_lineno ARGS((void));
static void		init_locale ARGS((void));
static void		init_readline ARGS((void));
static void             looponly ARGS((const char *));
static int		is_global ARGS((Symbol *));
static int		nth_arg ARGS((Symbol *));
static void		run ARGS((void));
static void             show ARGS((const char *[]));
static const char *	symtype ARGS((Symbol *));
static void             usage ARGS((void));
static void             usage_and_die ARGS((const char *));
static void             version_and_die ARGS((void));
static Symbol *		yygetlocal ARGS((const char *));


# line 191 "hoc.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	long	narg;	/* number of arguments */
} YYSTYPE;
# define BLTIN0 257
# define BLTIN1 258
# define BLTIN1I 259
# define BLTIN1S 260
# define BLTIN2 261
# define BLTIN2S 262
# define BREAK 263
# define CONTINUE 264
# define ELSE 265
# define FOR 266
# define FUNC 267
# define FUNCTION 268
# define GLOBAL 269
# define IF 270
# define NUMBER 271
# define PRINT 272
# define PRINTF 273
# define PRINTLN 274
# define PROC 275
# define PROCEDURE 276
# define READ 277
# define RETURN 278
# define SPRINTF 279
# define STRBLTIN0 280
# define STRBLTIN1 281
# define STRBLTIN1N 282
# define STRBLTIN2 283
# define STRBLTIN2SN 284
# define STRBLTIN3SNN 285
# define STRING 286
# define STRVAR 287
# define UNDEF 288
# define VAR 289
# define WHILE 290
# define ARG 291
# define STRARG 292
# define ADDEQ 293
# define SUBEQ 294
# define MULEQ 295
# define DIVEQ 296
# define MODEQ 297
# define CONSTEQ 298
# define OR 299
# define AND 300
# define GT 301
# define GE 302
# define LT 303
# define LE 304
# define EQ 305
# define NE 306
# define LTGT 307
# define UNARYMINUS 308
# define UNARYPLUS 309
# define NOT 310
# define INC 311
# define DEC 312
# define TO_POWER 313

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 491 "hoc.y"

	/* end of grammar */

#ifndef YYCONST
#define YYCONST				/* symbol used by Sun Solaris yacc */
#endif

#if defined(__osf__)
EXTERN void yyerror ARGS((YYCONST char *));	/* keep C++ compilers happy */
#endif

#include "readline.h"

#undef getc
#define getc(fpin)			irl_getchar(fpin)
#undef ungetc
#define ungetc(c,fpin)			irl_unget_char(c)
#undef fscanf
#define fscanf(fpin,format,pdata)	irl_fscanf(fpin,format,pdata)

#define ARGMATCH(arg,option,minmatch) (strncmp(arg,option,MAX(strlen(arg),minmatch)) == 0)
#undef MAX				/* because some systems define this already */
#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define OPTCHAR		'-'

extern FILE *fplog;

int cd_pwd_enabled = 1;	  		/* reset by --no-cd and --secure command-line options */
int file_trace_enabled = 1;		/* reset by --secure command-line option */
int load_enabled = 1;			/* reset by --no-load and --secure command-line options */
int logfile_enabled = 1;		/* reset by --no-logfile and --secure command-line options */
int save_enabled = 1;			/* reset by --no-save and --secure command-line options */
int use_readline = 1;			/* reset by --no-readline command-line option */
static int print_banner = 1;   		/* reset by --no-banner command-line option */
static int read_help_file = 1;		/* reset by --no-help-file command-line option */
static int read_site_file = 1;		/* reset by --no-site-file command-line option */
static int read_translation_file = 1;	/* reset by --no-translation-file command-line option */
static int read_user_file = 1;		/* reset by --no-user-file command-line option */
static int verbose_mode = 1;   		/* reset by --silent command-line option */

static jmp_buf		begin;
static int		gargc;
static char		**gargv;	/* global argument list */
static const char	*infile;	/* input file name */
static int		lineno = 1;
const char		*progname;

FILE			*fin;		/* input file pointer (also used in code.c and string.c) */

static void
addarg(Symbol *s)
{
	while (numargs >= maxargs)
		argnamelist = (Symbol**)egrow((void*)argnamelist, &maxargs, sizeof(argnamelist[0]));
	argnamelist[numargs++] = s;
}

static void
addglobal(Symbol *s)
{
	while (numglobals >= maxglobals)
		globallist = (Symbol**)egrow((void*)globallist, &maxglobals, sizeof(globallist[0]));
	globallist[numglobals++] = s;
}

static void
author_and_die(void)
{
	static const char *text[] =
	{
		PACKAGE_NAME, " was written by\n\n",

		"\tBrian W. Kernighan and Rob Pike\n\n",

		"and described in their book:\n\n",

		"\tThe UNIX Programming Environment\n",
		"\tPrentice-Hall\n",
		"\t1984\n",
		"\tpp. xii + 357\n",
		"\tISBN 0-13-937699-2 (hardcover), 0-13-937681-X (paperback)\n",
		"\tLCCN QA76.76.O63 K48 1984\n\n",

		"Extensions for IEEE 754 arithmetic, help, additional constants,\n",
		"statements, and functions, initialization files, internationalization,\n",
		"and GNU readline support were added by:\n\n",

		"\t", PACKAGE_BUGREPORT, "\n\n",

		"The extended hoc master Internet source distribution site is at:\n\n",

		"\tftp://ftp.math.utah.edu/pub/hoc\n",
		"\thttp://www.math.utah.edu/pub/hoc/\n\n",

		(const char *)NULL,
	};
	show(text);
	exit(EXIT_SUCCESS);
}

static int
backslash(int c)	/* get next char with \'s interpreted */
{
	static char transtab[] = "a\007b\bf\fn\nr\rt\tv\v"; /* K&R and Standard C escapes */

#define is_odigit(x) (isdigit(x) && (x != '8') && (x != '9'))

	if (c != '\\')
		return c;
	c = getc(fin);
	if (c == '\n')
		bump_lineno();
	if (islower(c) && (strchr(transtab, c) != (char *)NULL))
		return strchr(transtab, c)[1];
	else if (c == 'E')		/* \E -> ESCape */
	{
		/* NB: Standard C reserves uppercase escapes to
		   implementation-specific extensions */

		return ('\033');	/* ASCII ESCape */
	}
	else if (is_odigit(c))
	{		 /* \o, \oo, \ooo -> octal character number */
		int n;
		int value;

		n = 0;
		value = 0;
		while (is_odigit(c) && ((++n) <= 3))
		{
			value = 8*value + (c - (int)'0');
			c = getc(fin);
		}
		(void)ungetc(c,fin);
		return (value);
	}
	else if (c == (int)'x')
	{	/* \xhhhh... (any number of h's) -> hexadecimal character number */
		int value;

		value = 0;
		c = getc(fin);
		while (isxdigit(c))
		{
			if (isupper(c))
				c = tolower(c);
			if (((int)'a' <= c) && (c <= (int)'f'))
				value = 16*value + (c - (int)'a' + 10);
			else if (isdigit(c))
				value = 16*value + (c - (int)'0');
			c = getc(fin);
		}
		(void)ungetc(c,fin);
		return (value);
	}
	return c;

#undef is_odigit
}

static void
bump_lineno(void)
{
#if 0
	/* temporarily suppressed: see io.c:run3() header comments for why */

	static fp_t * pdlineno = (fp_t*)NULL;

	if (pdlineno == (fp_t*)NULL)
	{
		Symbol *s;

		s = lookup("__LINE__");
		if (s == (Symbol*)NULL)
			s = install_number("__LINE__", FP(0.0));
		make_immutable(s); /* to prevent user code assignments */
		pdlineno = &s->u.val;
	}
	lineno++;
	*pdlineno = (fp_t)lineno;	/* update __LINE__ value in symbol table */
#else
	/* We need this alternate version of line number incrementing to match code in io.c:run3() */
	Symbol *s;
	s = lookup("__LINE__");
	if (s == (Symbol*)NULL)
		s = install_number("__LINE__", FP(0.0));
	if (lineno == -1)		/* special case of new input file */
		s->u.val = (fp_t)lineno;
	s->u.val += FP(1.0);
	lineno = (int)(s->u.val);
#endif
}

static void
copyright_and_die(void)
{
	static const char *text[] =
	{
		"\n",
		"=================================================================\n",
		"Copyright (C) AT&T 1995\n",
		"All Rights Reserved\n\n",

		"Permission to use, copy, modify, and distribute this software and\n",
		"its documentation for any purpose and without fee is hereby\n",
		"granted, provided that the above copyright notice appear in all\n",
		"copies and that both that the copyright notice and this\n",
		"permission notice and warranty disclaimer appear in supporting\n",
		"documentation, and that the name of AT&T or any of its entities\n",
		"not be used in advertising or publicity pertaining to\n",
		"distribution of the software without specific, written prior\n",
		"permission.\n\n",

		"AT&T DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,\n",
		"INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.\n",
		"IN NO EVENT SHALL AT&T OR ANY OF ITS ENTITIES BE LIABLE FOR ANY\n",
		"SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n",
		"WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER\n",
		"IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,\n",
		"ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF\n",
		"THIS SOFTWARE.\n",
		"=================================================================\n\n",

		(const char *)NULL,
	};
	show(text);
	exit(EXIT_SUCCESS);
}

static void
defnonly(const char *s)	/* warn if illegal definition */
{
	if (!indef)
		execerror(s, "used outside definition");
}

static void
do_define(const char *s)
{	/* expect name (== name=1) or name=numericvalue or name=name or name="string" */
	/* (= may be replaced by := for permanent assignments) */
	char *equals;
	char *t;

	t = (char *)dupstr(s);
	equals = strchr(t,'=');
	if (equals == (char *)NULL)
	{
		if (is_immutable(t))
			error("illegal reassignment to immutable named constant", s);
		else
			(void)update_number(t,FP(1.0));	/* -Dname means -Dname=1 */
	}
	else					/* have -Dname=something or -Dname:=permanent-something */
	{
		char *value;
		int immutable;

		immutable = (equals[-1] == ':');

		if (immutable)
			equals[-1] = '\0';
		else
			equals[0] = '\0';

		if (is_immutable(t))
			error("illegal reassignment to immutable named constant", s);

		value = equals + 1;
		if (*value == QUOTATION_MARK) /* have name="value" */
		{
			char *q;

			value++;
			q = strchr(value, QUOTATION_MARK);
			if (q == (const char *)NULL)
				error("unclosed quotation mark in command-line option", s);
			else if (q[1] == '\0')
			{
				const char *v;

				*q = '\0';
				v = Expand(value);
				if (immutable)
					(void)update_const_string(t,v);
				else
					(void)update_string(t,v);
				efree((void *)v);
			}
			else
				error("unrecognized string in definition", s);
		}
		else if (IsIdStart(*value)) /* have name=name */
		{
			Symbol *sp;

			sp = lookup(value);
			if (sp == (Symbol*)NULL)
				error("unknown symbol in definition", s);
			else if (sp->type == VAR)
			{
				if (immutable)
					(void)update_const_number(t, sp->u.val);
				else
					(void)update_number(t, sp->u.val);
			}
			else if (sp->type == STRVAR)
			{
				if (immutable)
					(void)update_const_string(t, sp->u.str);
				else
					(void)update_string(t, sp->u.str);
			}
			else
				error("illegal symbol in definition", s);
		}
		else /* expect name=numeric-value */
		{
			char *endptr;
			fp_t d;

			d = strton(value, &endptr);
			if (*endptr == '\0')
			{
				if (immutable)
					(void)update_const_number(t, d);
				else
					(void)update_number(t, d);
			}
			else
				error("unrecognized number in definition", s);
		}
	}
	efree((void *)t);
}

static void
do_init_files(void)
{
	/* Process the optional startup files in documented
	   (in "man hoc") order of use:
		(1) system hoc.rc
		(2) locale hoc.rc
		(3) system help.hoc
		(4) locale help.hoc
		(5) system translations.hoc
		(6) locale translations.hoc
		(7) user .hocrc
	*/

	const char *p;

	if (read_site_file)
	{
		do_one_init_file(syshocrc);
		p = get_locale_filename(syshocdir, syshocrcbase);
		do_one_init_file(p);
		efree((void*)p);
	}

	if (read_help_file)
	{
		do_one_init_file(syshochlp);
		p = get_locale_filename(syshocdir, syshochlpbase);
		do_one_init_file(p);
		efree((void*)p);
	}

	if (read_translation_file)
	{
		do_one_init_file(syshocxlt);
		p = get_locale_filename(syshocdir, syshocxltbase);
		do_one_init_file(p);
		efree((void*)p);
	}

#if defined(HAVE_UNIX_HOME_DIRECTORY)
	if (read_user_file)
	{
		const char * userhocrc;
		const char * home;

		home = getenv("HOME");
		if (home == (const char *)NULL)
			return;
		userhocrc = concat3(home, "/", hocrc);
		do_one_init_file(userhocrc);
		efree((void *)userhocrc);
	}
#endif
}

static void
do_one_init_file(const char *filename)
{
	const char *new_filename;

	/* If filename is readable, attempt to open it and process it.  Otherwise,
	   return silently. */
	if ((filename != (const char *)NULL) && (access(filename, R_OK) == 0))
	{
		fin = Fopen(filename, "r", &new_filename);
		if (fin == (FILE*)NULL)
		{
			(void)fprintf(stderr,
				      msg_translate("%s: can't open %s\n"),
				      progname, filename);
			efree((void *)new_filename);
		}
		else
		{
			set_filename(new_filename);
			efree((void *)new_filename);
			run();
			(void)fclose(fin);
			fin = (FILE*)NULL;
		}
	}
}

static void
do_post_args(int *pargc, char * argv[])
{
	int k, m;
	char *p;

	/* Process command-line arguments after initialization files
	   have been read.  Any options that are recognized are
	   removed from argv[], and *pargc is reduced accordingly. */

	for (k = 1; k < *pargc; ++k)
	{
		p = argv[k];
		if (p[0] == OPTCHAR)
		{
			p++;
			if (p[0] == OPTCHAR)
				++p;	/* support GNU/POSIX style --option */

			if (STREQUAL(p,"")) /* ignore "-" option (meaning, stdin) */
			{
				if (p == (argv[k] + 2))
					p[-1] = '\0'; /* convert "--" to "-" */
				continue;
			}
			else if (*p == 'D')
				noop();	/* already handled in do_pre_args() */
			else if (*p == 'U')
				noop();	/* already handled in do_pre_args() */
			else if (ARGMATCH(p,"author",1))
				author_and_die();
			else if (ARGMATCH(p,"copyright",1))
				copyright_and_die();
			else if (ARGMATCH(p,"?",1))
				help_and_die();
			else if (ARGMATCH(p,"help",1))
				help_and_die();
			else if (ARGMATCH(p,"no-banner",4))
			{
				print_banner = 0;
				(void)update_number("__BANNER__", (fp_t)print_banner);
			}
			else if (ARGMATCH(p,"no-cd",4))
				cd_pwd_enabled = 0;
			else if (ARGMATCH(p,"no-help-file",4))
				read_help_file = 0;
			else if (ARGMATCH(p,"no-load",6))
				load_enabled = 0;
			else if (ARGMATCH(p,"no-logfile",6))
				logfile_enabled = 0;
			else if (ARGMATCH(p,"no-readline",4))
			{
				use_readline = 0;
				(void)update_number("__READLINE__", (fp_t)use_readline);
			}
			else if (ARGMATCH(p,"no-save",5))
				save_enabled = 0;
			else if (ARGMATCH(p,"no-site-file",5))
				read_site_file = 0;
			else if (ARGMATCH(p,"no-translation-file",4))
				read_translation_file = 0;
			else if (ARGMATCH(p,"no-user-file",4))
				read_user_file = 0;
			else if (ARGMATCH(p,"quick",1))
			{
				read_help_file = 0;
				read_site_file = 0;
				read_translation_file = 0;
				read_user_file = 0;
			}
			else if (ARGMATCH(p,"secure",2))
			{
				cd_pwd_enabled = 0;
				load_enabled = 0;
				logfile_enabled = 0;
				save_enabled = 0;
				file_trace_enabled = 0;
			}
			else if (ARGMATCH(p,"silent",1)) /* NB: can be abbreviated to 1 char */
			{
				verbose_mode = 0;
				(void)update_number("__VERBOSE__", (fp_t)verbose_mode);
			}
			else if (ARGMATCH(p,"trace-file-opening",1))
				(void)update_number("__DEBUG_OPEN__", FP(1.0));
			else if (ARGMATCH(p,"version",1))
				version_and_die();
			else
				usage_and_die(argv[k]);
			/* Remove the option from the argument list */
			for (m = k + 1; m < (*pargc); ++m)
				argv[m-1] = argv[m];
			(*pargc)--;
			k--;	/* avoid missing the next argument */
		}
	}
	argv[*pargc] = (char *)NULL;	/* ANSI C89 Section 2.1.2.2.1: ``argv[argc] shall be a NULL pointer'' */

#if !defined(HAVE_GNU_READLINE)
	use_readline = 0;
	(void)update_number("__READLINE__", (fp_t)use_readline);
#endif

	make_immutable(lookup("__READLINE__"));	/* this decision may someday be changed */
}

static void
do_pre_args(const int *pargc, const char * argv[])
{
	int k;
	const char *p;

	(void)update_string("__FILE__", "command-line"); /* in case error() is called */

	/* Process those command-line arguments that need to be seen
	   before initialization files are read.  No errors are raised
	   here for unrecognized arguments; that will be done in
	   do_post_args().  *pargc and argv[] are left intact. */

	for (k = 1; k < *pargc; ++k)
	{
		p = argv[k];
		if (p[0] == OPTCHAR)
		{
			p++;
			if (p[0] == OPTCHAR)
				++p; /* support GNU/POSIX style --option */

			if (ARGMATCH(p,"no-banner",4))
			{
				print_banner = 0;
				(void)update_number("__BANNER__", (fp_t)print_banner);
			}
			else if (ARGMATCH(p,"no-help-file",4))
				read_help_file = 0;
			else if (ARGMATCH(p,"no-logfile",6))
				logfile_enabled = 0;
			else if (ARGMATCH(p,"no-readline",4))
			{
				use_readline = 0;
				(void)update_number("__READLINE__", (fp_t)use_readline);
			}
			else if (ARGMATCH(p,"no-site-file",5))
				read_site_file = 0;
			else if (ARGMATCH(p,"no-translation-file",4))
				read_translation_file = 0;
			else if (ARGMATCH(p,"no-user-file",4))
				read_user_file = 0;
			else if (ARGMATCH(p,"quick",1))
			{
				read_help_file = 0;
				read_site_file = 0;
				read_translation_file = 0;
				read_user_file = 0;
			}
			else if (ARGMATCH(p,"silent",1)) /* can be abbreviated to 1 character */
			{
				verbose_mode = 0;
				(void)update_number("__VERBOSE__", (fp_t)verbose_mode);
			}
			else if (ARGMATCH(p,"trace-file-opening",1))
				(void)update_number("__DEBUG_OPEN__", FP(1.0));
			else if (*p == 'D')
				do_define(p+1);
			else if (*p == 'U')
				do_undefine(p+1);
		}
	}
}

static void
do_undefine(const char *s)
{
	if (!delete_symbol(lookup(s)))
		error("deletion failed for symbol", s);
}

static void
error(const char *s, const char *t)
{
	warning(s,t);
	exit(EXIT_FAILURE);
}

void
execerror(const char * s, const char * t)	/* recover from run-time error */
{
	warning(s, t);
	(void)fseek(fin, 0L, 2);	/* flush rest of file */
	longjmp(begin, 0);
}

#if defined(__sgi)
#include <sys/fpu.h>

static void
flush_to_zero(int on_off)		/* see "man sigfpe" on SGI IRIX 6.x for documentation */
{
	union fpc_csr n;

	n.fc_word = get_fpc_csr();
	n.fc_struct.flush = (on_off ? 1 : 0);
	set_fpc_csr(n.fc_word);
}
#endif

static int
follow(int expect, int ifyes, int ifno)	/* look ahead for >=, etc. */
{
	int ch = getc(fin);

	if (ch == expect)
		return ifyes;
	(void)ungetc(ch, fin);
	return ifno;
}

void
fpecatch(int dummy)	/* catch floating point exceptions */
{
	execerror("floating point exception", (const char *)NULL);
}

static void
free_argnamelist(void)
{
	efree((void*)argnamelist);
	argnamelist = (Symbol**)NULL;
	numargs = 0;
	maxargs = 0;
}

static void
free_globallist(void)
{
	efree((void*)globallist);
	globallist = (Symbol**)NULL;
	numglobals = 0;
	maxglobals = 0;
}

static const char *
get_locale_filename(const char *dirname, const char *basename)
{
	/* Given dirname = "/x/y/z" and basename == "foo.bar", return a dynamic
	   copy of "/x/y/z/locale/LOCALENAME/foo.bar", where LOCALENAME is
	   obtained from get_locale_name() */
	const char *locale;

	locale = get_locale_name();
	if (locale == (const char *)NULL)
		return ((const char *)NULL);
	else
		return (concat5(dirname, "/locale/", locale, "/", basename));
}

static const char *
get_locale_name(void)
{
	/* According to "man 3c setlocale", the language of text
	messages is determined by the locale variable LC_MESSAGES: its
	value is set by the first-defined of three environment
	variables.  Return a static copy of the value. */

	const char *locale_name;

	locale_name = getenv("LC_ALL");
	if (locale_name == (const char *)NULL)
		locale_name = getenv("LC_MESSAGES");
	if (locale_name == (const char *)NULL)
		locale_name = getenv("LANG");
	return (locale_name);
}

const char *
get_prompt(void)
{
	Symbol *s;

	s = lookup("__PROMPT__");
	if (s == (Symbol*)NULL)
		s = install_string("__PROMPT__", (PACKAGE_NAME "> "));
	return (s->u.str);
}

int
get_verbose(void)
{
	Symbol *s;

	s = lookup("__VERBOSE__");
	if (s == (Symbol*)NULL)
		(void)install_number("__VERBOSE__", (fp_t)verbose_mode);
	else
		verbose_mode = (s->u.val != FP(0.0));
	return (verbose_mode);
}

static void
help_and_die(void)
{
	static const char *text[] =
	{
		"\n",
		"For additional help, try ", PACKAGE_NAME, "'s help() command, or in a shell,\n",
		"use the UNIX manual page command, man ", PACKAGE_NAME, ".\n",

		(const char *)NULL,
	};

	usage();
	show(text);
	exit(EXIT_SUCCESS);
}

static void
init_fp_system(void)
{
	/* Do an system-dependent initializations of the
	   floating-point arithmetic system to enable full IEEE 754
	   support, if possible. */

#if defined(__sgi)
	flush_to_zero(0);		/* to get support for subnormals! */
#endif

}

static void
init_limits(void)
{
	/* Set all of the dynamic table sizes so that they can be
	   queried even before their corresponding arrays have been
	   allocated. */
	(void)update_const_number("__MAX_FRAME__", FP(0.0));
	(void)update_const_number("__MAX_LINE__", FP(0.0));
	(void)update_const_number("__MAX_NAME__", FP(0.0));
	(void)update_const_number("__MAX_PROG__", FP(0.0));
	(void)update_const_number("__MAX_PUSHBACK__",FP(0.0));
	(void)update_const_number("__MAX_STACK__", FP(0.0));
	(void)update_const_number("__MAX_STRING__", FP(0.0));
	(void)update_const_number("__MAX_TOKEN__", FP(0.0));
}

static void
init_lineno(void)
{
	EOF_flag = 0;

	/* Because hoc evaluation happens AFTER complete lines have
	   been read, we start lineno at 0, so that, e.g.,

		println __LINE__

	on line 1 will print 1, not 2. */

	lineno = -1;
	bump_lineno();
}

static void
init_locale(void)
{
#if defined(HAVE_LOCALE_H)
	/* These calls internationalize the hoc language!  However, in
	   order for existing programs to work, numbers MUST use a
	   decimal point, NOT a comma (or other character).  Without
	   this setting, strtod(), fscanf(), and *printf() all alter
	   number formats to locale-specific conventions.  Similarly,
	   settings other than "C" would violate assumptions about
	   sorting in the who() function, and time stamps in the now()
	   function. */

	const char *locale_name;

	locale_name = get_locale_name();
	if (locale_name != (const char *)NULL)
		(void)setlocale(LC_ALL, locale_name);
	(void)setlocale(LC_COLLATE,	"C");
	(void)setlocale(LC_CTYPE,	"C");
	(void)setlocale(LC_MONETARY,	"C");

#if defined(LC_MESSAGES)		/* POSIX.2 extension beyond C89, C++98, and C99 */
	(void)setlocale(LC_MESSAGES,	"C");
#endif

	(void)setlocale(LC_NUMERIC,	"C");
	(void)setlocale(LC_TIME,	"C");
#endif /* defined(HAVE_LOCALE_H) */
}

static void
init_readline(void)
{
#if defined(HAVE_GNU_READLINE)
	(void)rl_bind_key ('\t', rl_insert);	/* TAB is normal, NOT filename completer */
	(void)rl_bind_key ('\033', rl_complete); /* put filename completion on ESCape */
	irl_fd_stdin = dup(fileno(stdin)); /* save handle for original stdin */
#endif
}

static void
init_symbols(void)
{
	(void)update_number("__BANNER__", (fp_t)print_banner); /* so -no-banner setting is available to user code */
	(void)update_number("__READLINE__", (fp_t)use_readline); /* so -no-readline setting is available to user code */
	(void)update_number("__VERBOSE__",(fp_t)verbose_mode); /* so -silent setting is available to user code */

#if defined(HAVE_IEEE_754)
	(void)install_const_number("__IEEE_754__", FP(1.0));
#else
	(void)install_const_number("__IEEE_754__", FP(0.0));
#endif

#if defined(HAVE_ACOSH)
	(void)install_const_number("__NATIVE_ACOSH__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ACOSH__", FP(0.0));
#endif

#if defined(HAVE_ACOSH)
	(void)install_const_number("__NATIVE_ACOSH__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ACOSH__", FP(0.0));
#endif

#if defined(HAVE_ASINH)
	(void)install_const_number("__NATIVE_ASINH__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ASINH__", FP(0.0));
#endif

#if defined(HAVE_ATANH)
	(void)install_const_number("__NATIVE_ATANH__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ATANH__", FP(0.0));
#endif

#if defined(HAVE_CBRT)
	(void)install_const_number("__NATIVE_CBRT__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_CBRT__", FP(0.0));
#endif

#if defined(HAVE_CEIL)
	(void)install_const_number("__NATIVE_CEIL__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_CEIL__", FP(0.0));
#endif

#if defined(HAVE_COPYSIGN)
	(void)install_const_number("__NATIVE_COPYSIGN__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_COPYSIGN__", FP(0.0));
#endif

#if defined(HAVE_ERF)
	(void)install_const_number("__NATIVE_ERF__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ERF__", FP(0.0));
#endif

#if defined(HAVE_ERFC)
	(void)install_const_number("__NATIVE_ERFC__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ERFC__", FP(0.0));
#endif

#if defined(HAVE_EXPM1)
	(void)install_const_number("__NATIVE_EXPM1__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_EXPM1__", FP(0.0));
#endif

#if defined(HAVE_FLOOR)
	(void)install_const_number("__NATIVE_FLOOR__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_FLOOR__", FP(0.0));
#endif

#if defined(HAVE_FMOD)
	(void)install_const_number("__NATIVE_FMOD__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_FMOD__", FP(0.0));
#endif

#if defined(HAVE_GAMMA)
	(void)install_const_number("__NATIVE_GAMMA__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_GAMMA__", FP(0.0));
#endif

#if defined(HAVE_GETGID)
	(void)install_const_number("__NATIVE_GETGID__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_GETGID__", FP(0.0));
#endif

#if defined(HAVE_GETPID)
	(void)install_const_number("__NATIVE_GETPID__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_GETPID__", FP(0.0));
#endif

#if defined(HAVE_GETPPID)
	(void)install_const_number("__NATIVE_GETPPID__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_GETPPID__", FP(0.0));
#endif

#if defined(HAVE_GETUID)
	(void)install_const_number("__NATIVE_GETUID__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_GETUID__", FP(0.0));
#endif

#if defined(HAVE_HYPOT)
	(void)install_const_number("__NATIVE_HYPOT__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_HYPOT__", FP(0.0));
#endif

#if defined(HAVE_ILOGB)
	(void)install_const_number("__NATIVE_ILOGB__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_ILOGB__", FP(0.0));
#endif

#if defined(HAVE_J0)
	(void)install_const_number("__NATIVE_J0__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_J0__", FP(0.0));
#endif

#if defined(HAVE_J1)
	(void)install_const_number("__NATIVE_J1__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_J1__", FP(0.0));
#endif

#if defined(HAVE_JN)
	(void)install_const_number("__NATIVE_JN__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_JN__", FP(0.0));
#endif

#if defined(HAVE_LDEXP)
	(void)install_const_number("__NATIVE_LDEXP__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_LDEXP__", FP(0.0));
#endif

#if defined(HAVE_LGAMMA)
	(void)install_const_number("__NATIVE_LGAMMA__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_LGAMMA__", FP(0.0));
#endif

#if defined(HAVE_LOG1P)
	(void)install_const_number("__NATIVE_LOG1P__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_LOG1P__", FP(0.0));
#endif

#if defined(HAVE_NEAREST)
	(void)install_const_number("__NATIVE_NEAREST__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_NEAREST__", FP(0.0));
#endif

#if defined(HAVE_NEXTAFTER)
	(void)install_const_number("__NATIVE_NEXTAFTER__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_NEXTAFTER__", FP(0.0));
#endif

#if defined(HAVE_REMAINDER)
	(void)install_const_number("__NATIVE_REMAINDER__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_REMAINDER__", FP(0.0));
#endif

#if defined(HAVE_RINT)
	(void)install_const_number("__NATIVE_RINT__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_RINT__", FP(0.0));
#endif

#if defined(HAVE_RSQRT)
	(void)install_const_number("__NATIVE_RSQRT__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_RSQRT__", FP(0.0));
#endif

#if defined(HAVE_SCALB)
	(void)install_const_number("__NATIVE_SCALB__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_SCALB__", FP(0.0));
#endif

#if defined(HAVE_SIGNIFICAND)
	(void)install_const_number("__NATIVE_SIGNIFICAND__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_SIGNIFICAND__", FP(0.0));
#endif

#if defined(HAVE_SNPRINTF)
	(void)install_const_number("__NATIVE_SNPRINTF__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_SNPRINTF__", FP(0.0));
#endif

#if defined(HAVE_STRFTIME)
	(void)install_const_number("__NATIVE_STRFTIME__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_STRFTIME__", FP(0.0));
#endif

#if defined(HAVE_STRTOLD)
	(void)install_const_number("__NATIVE_STRTOLD__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_STRTOLD__", FP(0.0));
#endif

#if defined(HAVE_TRUNC)
	(void)install_const_number("__NATIVE_TRUNC__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_TRUNC__", FP(0.0));
#endif

#if defined(HAVE_Y0)
	(void)install_const_number("__NATIVE_Y0__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_Y0__", FP(0.0));
#endif

#if defined(HAVE_Y1)
	(void)install_const_number("__NATIVE_Y1__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_Y1__", FP(0.0));
#endif

#if defined(HAVE_YN)
	(void)install_const_number("__NATIVE_YN__", FP(1.0));
#else
	(void)install_const_number("__NATIVE_YN__", FP(0.0));
#endif

	(void)install_const_string("__PACKAGE_BUGREPORT__",	PACKAGE_BUGREPORT);
	(void)install_const_string("__PACKAGE_DATE__",		PACKAGE_DATE);
	(void)install_const_string("__PACKAGE_NAME__",		PACKAGE_NAME);
	(void)install_const_string("__PACKAGE_STRING__",	PACKAGE_STRING);
	(void)install_const_string("__PACKAGE_VERSION__",	PACKAGE_VERSION);

	/* Record the initialization file directories and names for
	   use in the language-changing procedures */

	(void)install_const_string("__HOCRC__",		hocrc);
	(void)install_const_string("__SYSHOCDIR__",	syshocdir);
	(void)install_const_string("__SYSHOCHLPBASE__",	syshochlpbase);
	(void)install_const_string("__SYSHOCHLP__",	syshochlp);
	(void)install_const_string("__SYSHOCRCBASE__",	syshocrcbase);
	(void)install_const_string("__SYSHOCRC__",	syshocrc);
	(void)install_const_string("__SYSHOCXLTBASE__",	syshocxltbase);
	(void)install_const_string("__SYSHOCXLT__",	syshocxlt);

	{
		const char *p1;
		const char *p2;
		const char *p3;
		const char *p4;
		(void)install_const_string("__SYSHOCPATH__",
					   p1 = concat5((p2 = concat2(syshocdir, "/site")),
							elementsep,
							(p3 = concat2((p4 = parentdir(syshocdir)), "/site")),
							elementsep,
							syshocdir));
		efree((void *)p1);
		efree((void *)p2);
		efree((void *)p3);
		efree((void *)p4);
	}

	if (getenv(HOCPATH) == (const char *)NULL) /* then supply HOCPATH=".:" */
	{
		const char *p;

		p = concat2(".", elementsep);
		efree((void*)Putenv(HOCPATH, p));
		efree((void*)p);
	}

#if defined(HAVE_GETGID)
	(void)update_const_number("__GID__", (fp_t)getgid());
#else
	(void)update_const_number("__GID__", FP(0.0));
#endif

#if defined(HAVE_GETPID)
	(void)update_const_number("__PID__", (fp_t)getpid());
#else
	(void)update_const_number("__PID__", FP(0.0));
#endif

#if defined(HAVE_GETPPID)
	(void)update_const_number("__PPID__", (fp_t)getppid());
#else
	(void)update_const_number("__PPID__", FP(0.0));
#endif

#if defined(HAVE_GETUID)
	(void)update_const_number("__UID__", (fp_t)getuid());
#else
	(void)update_const_number("__UID__", FP(0.0));
#endif

}

void
intcatch(int dummy)	/* catch interrupts */
{
	execerror("interrupt", (const char *)NULL);
}

static int
is_global(Symbol *s)
{
	int k;

	if ((s != (Symbol*)NULL) && ((s->type == STRVAR) || (s->type == VAR)) && s->builtin)
		return (1);  /* builtin constants are always global */
	for (k = 0; k < numglobals; ++k)
	{
		if (globallist[k] == s)
			return (1);
	}
	return (0);			/* not found */
}

int
main(int argc, char * argv[])	/* hoc7 */
{
	static int first = 1;

	init_limits();			/* this MUST be first! */

#if YYDEBUG
	yydebug = 3;
#endif
	progname = argv[0];
#if 0
	prog = (Inst *)egrow((Inst *)prog, &max_prog, sizeof(prog[0]));
#else
#if 0
	max_prog = 32767;
#else
	max_prog = 8 * 1024 * 1024;
#endif
	prog = (Inst *)emalloc(max_prog * sizeof(prog[0]));
	(void)update_const_number("__MAX_PROG__", (fp_t)max_prog);
#endif
	progp = progbase = &prog[0];

	init_fp_system();
	init_readline();
	init_locale();
	init_builtins();
	init_lineno();
	init_symbols();
	sanity_check();
	do_pre_args(&argc,(const char **)argv);
	do_init_files();
	do_post_args(&argc,argv);
	if (argc == 1) {	/* fake an argument list */
		static char stdhyphen[] = "-";
		static char *stdinonly[] = { stdhyphen, (char *)NULL };

		gargv = stdinonly;
		gargc = 1;
	} else if (first) {	/* for interrupts */
		first = 0;
		gargv = argv+1;
		gargc = argc-1;
	}
	while (moreinput())
		run();
	(void)signal(SIGINT, SIG_IGN);
	irl_free();
	free_symbol_table();
	efree((void*)prog);
	return (EXIT_SUCCESS);
}

int
moreinput(void)
{
	const char *new_filename;

	if (gargc-- <= 0)
		return 0;
	if ((fin != (FILE*)NULL) && (fin != stdin))
	{
		(void)fclose(fin);
		fin = (FILE*)NULL;
	}
	infile = *gargv++;
	if (infile == (const char *)NULL) /* skip over holes from command-line options */
		return moreinput();
	else if (STREQUAL(infile, "-")) {
		fin = stdin;
		new_filename = infile = (const char *)NULL;
#if defined(HAVE_GNU_READLINE)
		(void)dup2(irl_fd_stdin,fileno(stdin)); /* restore original stdin */
	} else if ((fin=Freopen(infile, "r", stdin, &new_filename)) == (FILE*)NULL) {
#else
	} else if ((fin=Fopen(infile, "r", &new_filename)) == (FILE*)NULL) {
#endif
		(void)fprintf(stderr,  msg_translate("%s: can't open %s\n"), progname, infile);
		efree((void *)new_filename);
		return moreinput();
	}
	set_filename((infile == (const char *)NULL) ? "/dev/stdin" : new_filename);
	efree((void *)new_filename);
	return 1;
}

static void
run(void)	/* execute until EOF */
{
	Symbol *sp;
	(void)setjmp(begin);
	(void)signal(SIGINT, intcatch);
	(void)signal(SIGFPE, fpecatch);

#if defined(SIGXCPU)
	(void)signal(SIGXCPU, xcpucatch);
#endif

	sp = lookup("__CPU_LIMIT__");
	if (sp != (Symbol*)NULL)
		(void)CPULimit(sp->u.val);
	for (initcode(); yyparse(); initcode())
		execute(progbase);
}

void
set_filename(const char *filename)
{
	Symbol *s;
	char temp[sizeof("__FILE__") + sizeof("18446744073709551616" /* 2^64 */) + 2];

	infile_count++;
	SNPRINTF4(temp, sizeof(temp), "__FILE__[%02d]", infile_count);
	(void)install_const_string(temp,filename);

	s = lookup("__FILE__");
	if (s == (Symbol*)NULL)
	{
#if 0
		/* temporarily suppressed: see io.c:run3() header comments for why */
		(void)install_const_string("__FILE__",filename);
#else
		(void)install_string("__FILE__",filename);
#endif
	}
	else
		(void)set_string(s,filename);
	init_lineno();
}

static void
looponly(const char *s)	/* warn if break/continue statement illegally placed */
{
	if (!inloop)
		execerror(s, "illegal outside loop body");
}

static int
nth_arg(Symbol *s)
{	/* return argument number (1, 2, 3, ...), or 0 if not found */
	int k;

	for (k = 0; k < numargs; ++k)
	{
		if (argnamelist[k] == s)
			return (k+1);
	}
	return (0);			/* not found */
}

static void
show(const char *text[])
{
	size_t k;

	for (k = 0; text[k] != (const char *)NULL; ++k)
		(void)fprintf(stderr,"%s", msg_translate(text[k]));
}

static const char *
symtype(Symbol *s)
{
	switch ((s == (Symbol*)NULL) ? -1 : (s->type))
	{
	case ADDEQ:		return ("ADDEQ");
	case AND:		return ("AND");
	case ARG:		return ("ARG");
	case BLTIN0:		return ("BLTIN0");
	case BLTIN1:		return ("BLTIN1");
	case BLTIN1I:		return ("BLTIN1I");
	case BLTIN1S:		return ("BLTIN1S");
	case BLTIN2:		return ("BLTIN2");
	case BLTIN2S:		return ("BLTIN2S");
	case BREAK:		return ("BREAK");
	case CONSTEQ:		return ("CONSTEQ");
	case CONTINUE:		return ("CONTINUE");
	case DEC:		return ("DEC");
	case DIVEQ:		return ("DIVEQ");
	case ELSE:		return ("ELSE");
	case EQ:		return ("EQ");
	case FOR:		return ("FOR");
	case FUNC:		return ("FUNC");
	case FUNCTION:		return ("FUNCTION");
	case GE:		return ("GE");
	case GT:		return ("GT");
	case IF:		return ("IF");
	case INC:		return ("INC");
	case LE:		return ("LE");
	case LT:		return ("LT");
	case LTGT:		return ("LTGT");
	case MODEQ:		return ("MODEQ");
	case MULEQ:		return ("MULEQ");
	case NE:		return ("NE");
	case NOT:		return ("NOT");
	case NUMBER:		return ("NUMBER");
	case OR:		return ("OR");
	case PRINT:		return ("PRINT");
	case PRINTF:		return ("PRINTF");
	case PRINTLN:		return ("PRINTLN");
	case PROC:		return ("PROC");
	case PROCEDURE:		return ("PROCEDURE");
	case READ:		return ("READ");
	case RETURN:		return ("RETURN");
	case SPRINTF:		return ("SPRINTF");
	case STRARG:		return ("STRARG");
	case STRBLTIN0:		return ("STRBLTIN0");
	case STRBLTIN1:		return ("STRBLTIN1");
	case STRBLTIN1N:	return ("STRBLTIN1N");
	case STRBLTIN2:		return ("STRBLTIN2");
	case STRBLTIN2SN:	return ("STRBLTIN2SN");
	case STRBLTIN3SNN:	return ("STRBLTIN3SNN");
	case STRING:		return ("STRING");
	case STRVAR:		return ("STRVAR");
	case SUBEQ:		return ("SUBEQ");
	case TO_POWER:		return ("POWER");
	case UNARYMINUS:	return ("UNARYMINUS");
	case UNARYPLUS:		return ("UNARYPLUS");
	case UNDEF:		return ("UNDEF");
	case VAR:		return ("VAR");
	case WHILE:		return ("WHILE");
	default:		return ("UNKNOWN");
	}
}

static void
usage(void)
{
	static const char *text[] =
	{
		"Usage:\n",
		"\t",
		"",
		"\t[ --author ] [ --? ] [ --copyright] [ --Dname ]\n",
		"\t\t[ --Dname=number ] [ --Dname=\"string\" ]\n",
		"\t\t[ --Dname=symbol ] [ --help ] [ --no-banner ]\n",
		"\t\t[ --no-cd ] [ --no-help-file ] [ --no-load ]\n",
		"\t\t[ --no-logfile ] [ --no-readline ] [ --no-save ]\n",
		"\t\t[ --no-site-file ] [ --no-translation-file ]\n",
		"\t\t[ --no-user-file ] [ --quick ] [ --secure ]\n",
		"\t\t[ --silent ] [ --trace-file-opening ][ --Uname ]\n",
		"\t\t[ --version ]\n",
		"\t\tinput-file(s) [ -- ] more-input-file(s)\n\n",
		"Options may be prefixed with either one or two hyphens.\n",
		(const char *)NULL,
	};
	text[2] = progname;		/* cannot be done at compile time */
	show(text);
}

static void
usage_and_die(const char *bad_option)
{
	(void)fprintf(stderr, msg_translate("Unrecognized option: [%s]\n"), bad_option);
	usage();
	exit(EXIT_FAILURE);
}

static void
version_and_die(void)
{
	static const char *text[] =
	{
		"This is ", PACKAGE_NAME,
		" version ", PACKAGE_VERSION,
		" of ", PACKAGE_DATE, ".\n\n",
		"Report bugs to ", PACKAGE_BUGREPORT, "\n",
		(const char *)NULL,
	};
	show(text);
	exit(EXIT_SUCCESS);
}

void
warning(const char *s, const char *t)	/* print warning message */
{
	const char *current_filename;
	(void)fflush(stdout);		/* force out any buffered data */
	(void)fflush(stderr);

	s = msg_translate(s);		/* internationalize the output */
	t = msg_translate(t);

#if 1
	/* Use GNU and UNIX standard
		file:lineno:message
	format instead of old style
		program:message [in file] near line nnn */

	current_filename = get_string("__FILE__");

	(void)fprintf(stderr, "\n");	/* ensure that message starts on a new line! */
	(void)fprintf(stderr, "%s:%d:%s", current_filename, lineno, s);
	if (t != (const char *)NULL)
		(void)fprintf(stderr, " %s", t);
	(void)fprintf(stderr, "\n");

	if (logfile_enabled && (fplog != (FILE*)NULL))
	{
		(void)fprintf(fplog, "#-? %s:%d:%s", current_filename, lineno, s);
		if (t != (const char *)NULL)
			(void)fprintf(fplog, " %s", t);
		(void)fprintf(fplog, "\n");
	}
#else
	(void)fprintf(stderr, "%s: %s", progname, s);
	if (t)
		(void)fprintf(stderr, " %s", t);
	if (infile)
		(void)fprintf(stderr, " in %s", infile);
	(void)fprintf(stderr, " near line %d\n", lineno);
#endif

	while (this_c != '\n' && this_c != EOF)
		if((this_c = getc(fin), this_c) == '\n')	/* flush rest of input line */
			bump_lineno();
		else if (this_c == EOF && errno == EINTR) {
			clearerr(stdin);	/* ick! */
			errno = 0;
		}
	(void)fflush(stderr);
}

void
yyerror(YYCONST char * s)	/* report compile-time error */
{
/*rob
	warning(s, (const char *)NULL);
	longjmp(begin, 0);
rob*/
	execerror(s, (const char *)NULL);
}

Symbol *
yygetid(void)		/* get identifier, installing it in the symbol table if necessary */
{
	Symbol *s;
	static char *sbuf = (char *)NULL;
	static size_t max_sbuf = 0;
	size_t n;

	if (max_sbuf == (size_t)0)	/* guarantee that buffer exists so NUL can be stored */
		sbuf = (char *)emalloc(max_sbuf = (size_t)8);

	this_c = getc(fin);
	n = 0;
	do
	{
		if ((n + 2) > max_sbuf)
		{
			sbuf = (char *)egrow((void *)sbuf, &max_sbuf, sizeof(sbuf[0]));
			(void)update_const_number("__MAX_NAME__", (fp_t)max_sbuf);
		}
		sbuf[n++] = this_c;
	} while ((this_c=getc(fin)) != EOF && IsIdMiddle(this_c));
	(void)ungetc(this_c, fin);
	sbuf[n] = '\0';
	if (indef)			/* variable in func/proc definition */
		s = yygetlocal(sbuf);
	else				/* not in func/proc definition */
	{
		s = lookup(sbuf);
		if (s == (Symbol*)NULL)	/* new global variable */
			s = install(sbuf, UNDEF, FP(0.0));
	}

#if defined(DEBUG_YYLEX)
	(void)fprintf(stderr,"yygetid() -> [%s] [%s]\n", s->name, symtype(s));
#endif

	return (s);
}

static Symbol *
yygetlocal(const char *name)
{
	/* map variable names in func/proc declaration and body to private local variables */
	const char *p;
	Symbol *s;

	/* Local variables are stored in the symbol table with names
	   that are both unique (by virtue of a sequence number that
	   increments for each new function/procedure (re)definition),
	   and unlikely for user-level code to reference accidentally
	   (by including a nonalphabetic character in their names)
	   [although who() can expose them, and symstr() and symnum()
	   can extract their values].  For debugging convenience, we
	   also include the function/procedure name.  Function
	   is_local() in symbol.c, and the PREDIFF filters in
	   test[0-9]+/Makefile, are the only places that needs to know the
	   format of local variable names. */
	p = concat6("@", this_procname, "@", fmtnum((fp_t)ndefns), "@", name);
	if (in_argnamelist)
	{
		s = lookup(p);
		if (s != (Symbol*)NULL)
			execerror("duplicate argument name", name);
		s = install(p, UNDEF, FP(0.0));
	}
	else			/* in func/proc statement body */
	{
		s = lookup(name);
		if (in_global_stmt)
		{
			if (nth_arg(lookup(p)) > 0)
				execerror("cannot redeclare argument as global", s->name);
			else if (s == (Symbol*)NULL) /* new global variable */
				s = install(name, UNDEF, FP(0.0));
		}
		else if (!is_global(s))
		{
			if ( (s == (Symbol*)NULL) || (s->type == UNDEF) ||
			     (s->type == STRVAR) || (s->type == VAR) )
			{
				s = lookup(p);
				if (s == (Symbol*)NULL)	/* variable is a new local variable */
					s = install(p, UNDEF, FP(0.0));
			}
		}
		if (s == (Symbol*)NULL)	/* should never happen */
			execerror("internal error in yygetlocal(): unexpected NULL symbol for", name);
	}
	efree((void*)p);
	return (s);
}

const char *
yygetstr(void)
{	/* get a quoted string, assuming that the opening quote has just been read */
	static char *sbuf = (char *)NULL;
	static size_t max_sbuf = 0;
	int the_c;
	size_t n;

	if (max_sbuf == (size_t)0)	/* guarantee that buffer exists so NUL can be stored */
		sbuf = (char *)emalloc(max_sbuf = (size_t)16);

	for (n = 0; (the_c = getc(fin)) != QUOTATION_MARK; )
	{
		if (the_c == '\n' || the_c == EOF)
			execerror("missing quote", "");
		if ((n + 2) > max_sbuf)
		{
			sbuf = (char *)egrow((void *)sbuf, &max_sbuf, sizeof(sbuf[0]));
			(void)update_const_number("__MAX_STRING__", (fp_t)max_sbuf);
		}
		sbuf[n++] = backslash(the_c);
	}
	sbuf[n] = '\0';

	return((const char *)&sbuf[0]);
}

int
yylex(void)		/* hoc6 */
{
	while ((this_c=getc(fin)) == ' ' || this_c == '\t')
		;
	if (this_c == EOF)
		return 0;
	if (this_c == '\\') {
		this_c = getc(fin);
		if (this_c == '\n') {
			bump_lineno();
			return yylex();
		}
	}
	if (this_c == '#') {		/* comment */
		while ((this_c=getc(fin)) != '\n' && this_c != EOF)
			;
		if (this_c == '\n')
			bump_lineno();
		return this_c;
	}
	if (this_c == '.' || isdigit(this_c)) {	/* number */
		fp_t d;
		(void)ungetc(this_c, fin);
		(void)fscanf(fin, "%lf", &d); /* NB: this is really irl_fscanf(), which handles long double case correctly */
		yylval.sym = install("", NUMBER, d);
#if defined(DEBUG_YYLEX)
		(void)fprintf(stderr,"yylex() -> [%s] [NUMBER]\n", fmtnum(d));
#endif
		return NUMBER;
	}
	if (IsIdStart(this_c))
	{
		Symbol *s;
		(void)ungetc(this_c,fin);
		yylval.sym = s = yygetid();
		if (indef && !in_argnamelist) /* then in func/proc statement body */
		{
			int n;

			n = nth_arg(s);
			if (n > 0)	/* remap named argument to $nnn for code.c processing */
			{
				yylval.narg = n;
#if defined(DEBUG_YYLEX)
				(void)fprintf(stderr,"yylex() -> [%s] [$%d] [%s]\n",
					      s->name, n, "ARG");
#endif
				return (ARG);
			}
		}
#if defined(DEBUG_YYLEX)
		(void)fprintf(stderr,"yylex() -> [%s] [%s]\n",
			      s->name, ((s->type == UNDEF) ? "VAR" : symtype(s)));
#endif
		return (s->type == UNDEF ? VAR : s->type);
	}
	if (this_c == '$') {	/* argument? */
		int n = 0;
		long type;
		this_c = getc(fin);
		if (this_c == '$')	/* $$n means string argument instead of numeric argument, $n */
			type = STRVAR;
		else
		{
			type = VAR;
			(void)ungetc(this_c, fin);
		}
		while (isdigit(this_c=getc(fin)))
			n = 10 * n + this_c - '0';
		(void)ungetc(this_c, fin);
		if (n == 0)
			execerror("strange $...", (const char *)NULL);
		yylval.narg = n;
#if defined(DEBUG_YYLEX)
		(void)fprintf(stderr,"yylex() -> [$%d] [%s]\n", n, ((type == VAR) ? "ARG" : "STRARG"));
#endif
		return (type == VAR) ? ARG : STRARG;
	}
	if (this_c == QUOTATION_MARK) {	/* quoted string */
		const char *sbuf;
		sbuf = yygetstr();
#if 0
		/* Old ugly code that masquerades a string on top of a Symbol */
		yylval.sym = (Symbol *)emalloc(strlen(sbuf)+1);
		strcpy((char *)yylval.sym, sbuf);
#else
		/* New code to generate a real Symbol instead */
#if 0
		yylval.sym = (Symbol *)emalloc(sizeof(Symbol));
		yylval.sym->name = (const char *)NULL;
		yylval.sym->type = STRING;
		yylval.sym->u.str = dupstr(sbuf);
		make_immutable(yylval.sym);
		yylval.sym->next = (Symbol*)NULL;
#else
		yylval.sym = install_const_string("", sbuf);
#endif
#endif
#if defined(DEBUG_YYLEX)
		(void)fprintf(stderr,"yylex() -> [%s] [STRING]\n", sbuf);
#endif
		return STRING;
	}

#if defined(DEBUG_YYLEX)
	(void)fprintf(stderr,"yylex() -> [%d] [OPERATOR]\n", this_c);
#endif
	switch (this_c) {
	case '^':	return TO_POWER;
	case ':':	return follow('=', CONSTEQ, ':');
	case '+':	return follow('+', INC, follow('=', ADDEQ, '+'));
	case '-':	return follow('-', DEC, follow('=', SUBEQ, '-'));
	case '*':	return follow('=', MULEQ, follow('*', TO_POWER, '*'));
	case '/':	return follow('=', DIVEQ, '/');
	case '%':	return follow('=', MODEQ, '%');
	case '>':	return follow('=', GE, GT);
	case '<':	{
				if (follow('=', LE, LT) == LE)
					return (LE);
				else if (follow('>', LTGT, LT) == LTGT)
					return (LTGT);
				else
					return (LT);
			}
	case '=':	return follow('=', EQ, '=');
	case '!':	return follow('=', NE, NOT);
	case '|':	return follow('|', OR, '|');
	case '&':	return follow('&', AND, '&');
	case '\n':	bump_lineno(); return '\n';
	default:	return this_c;
	}
}
static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 203
# define YYLAST 1237
static YYCONST yytabelem yyact[]={

   234,   341,     7,    71,   369,   220,   354,   333,    70,   112,
    70,    93,   150,    68,    66,    68,    67,   149,    69,    94,
    69,   119,   230,   124,   128,   124,   151,   152,   153,   154,
   155,   156,    92,   356,   352,   355,   165,   183,   289,   291,
   144,   145,   147,   148,   335,   167,   334,   128,   323,   230,
   322,   231,   219,   118,   218,    10,   340,   300,   349,   347,
   327,   348,   348,   321,   346,     6,   317,   168,   169,   170,
   171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
   181,   182,   320,     2,   128,   321,   316,    57,    58,    59,
    60,    61,    85,    90,   315,   310,   309,   188,   190,   192,
   193,   194,   195,   196,    11,   298,   197,   199,   201,   202,
   203,   204,   205,   206,   208,   209,   210,   211,   212,   128,
   282,   215,   216,   361,   359,   345,   344,   343,   342,   314,
   313,   308,   221,   221,   221,   227,   307,   306,   222,   223,
   305,   304,   303,   301,   299,   283,   232,   319,   318,   229,
   214,   185,   164,   163,   162,   161,   160,   159,   142,   141,
   140,   139,   138,   137,   136,   133,   128,   128,   128,   132,
   131,   122,   123,   367,    82,    91,     5,   187,   121,    45,
     4,   366,   186,   371,   128,   128,   364,    65,    89,    95,
   350,   324,   217,   130,     3,     1,   237,   129,   236,    28,
   226,    27,    26,   135,   134,   157,   126,     0,     0,     0,
   105,     0,     0,     0,     0,   292,   294,   296,   225,     0,
   146,   146,   146,     0,     0,     0,   290,     0,     0,     0,
   292,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   113,   114,   115,   116,   117,   146,   146,   146,   146,
   146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
   146,     0,    70,     0,     0,     0,     0,    68,    66,   302,
    67,     0,    69,   166,   184,     0,     0,     0,   146,   146,
   146,   146,   146,   311,    71,     0,    71,   146,   146,   146,
   146,   146,     0,   146,   146,   146,   146,   146,   233,    65,
    89,   221,   227,     0,    65,    89,   325,    89,    65,     0,
    70,   146,   146,   146,     0,    68,    66,     0,    67,   235,
    69,     8,   337,    89,    65,   227,     0,   336,     0,     0,
     0,   274,   277,   278,   279,   280,   281,     0,    89,     0,
   120,   221,   125,   127,   125,     0,   351,     0,     0,     0,
     0,     0,   358,   227,     0,   227,   357,     0,   227,   143,
     0,     0,   365,     0,     0,   227,   158,   326,    65,    89,
    65,    89,   370,   372,     0,     0,     0,    65,    89,    65,
    89,     0,     0,     0,     0,     0,    65,    89,     0,    70,
   339,     0,     0,    89,    68,    66,     0,    67,     0,    69,
     0,     0,     0,   143,     0,     0,     0,    65,    89,     0,
     0,     0,     0,     0,    65,    89,   189,   191,   360,     0,
   362,     0,     0,   363,     0,   198,   200,     0,     0,     0,
   368,     0,   207,     0,     0,     0,     0,     0,   213,   269,
   270,   271,   107,   108,   109,   110,   111,   106,    10,     0,
     0,     0,     0,     0,   228,     0,     0,     0,     0,   284,
   285,   286,   287,   288,     0,     0,     0,     0,     0,     0,
     0,     0,    65,    89,    65,    89,    65,    89,    39,     0,
   146,    41,     0,    40,     0,   189,   191,   207,     0,     0,
     0,    65,    89,     0,     0,     0,     0,    11,     0,     0,
     0,     0,     0,   198,   200,     0,     0,    10,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    65,    89,     0,
   146,     0,     0,     0,    80,    79,    72,    73,    74,    76,
    77,    78,    75,     0,   293,   295,   297,    39,    71,     0,
    41,     0,    40,     0,     0,     0,     0,     0,     0,   293,
     0,     0,     0,     0,     0,     0,    11,     0,     0,     0,
     0,    29,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    79,    72,    73,    74,    76,    77,    78,
    75,     0,     0,     0,     0,     0,    71,     0,     0,     0,
     0,     0,     0,    39,     0,     0,    41,     0,    40,    10,
     0,     0,   312,     0,     0,     0,     0,     0,   328,   329,
     0,     0,     0,     0,   330,   331,   332,     0,     0,     0,
    29,   228,   224,     0,     0,     0,    70,     0,     0,    83,
     0,    68,    66,     0,    67,     0,    69,     0,     0,     0,
     0,   338,     0,     0,   228,   353,     0,     0,    11,     0,
     0,     0,     0,    72,    73,    74,    76,    77,    78,    75,
     0,     0,     0,    70,     0,    71,    83,   273,    68,    66,
     0,    67,   228,    69,   228,     0,    29,   228,     0,     0,
     0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     9,    33,    34,    35,    36,    37,
    38,    18,    19,     0,    48,    12,    31,    25,    49,    30,
    22,    23,    24,    13,    21,    32,    20,    46,    51,    52,
    53,    54,    55,    56,    50,    15,     0,    14,    47,    16,
    17,    70,     0,     0,    83,     0,    68,    66,     0,    67,
     0,    69,     0,     0,     0,     0,     0,     0,    42,    43,
    44,     0,     0,     0,    33,    34,    35,    36,    37,    38,
    18,    19,     0,    48,     0,    31,    25,    49,    30,    22,
    23,    24,     0,    21,    32,    20,    46,    51,    52,    53,
    54,    55,    56,    50,    15,     0,    14,    47,    16,    17,
     0,    39,     0,     0,    41,     0,    40,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    42,    43,    44,
    33,    34,    35,    36,    37,    38,    18,    19,     0,    48,
    96,    31,    25,    49,    30,    22,    23,    24,     0,    21,
    32,    20,    46,    51,    52,    53,    54,    55,    56,    50,
    15,     0,    14,    47,    16,    17,     0,    39,     0,     0,
    41,     0,    40,     0,     0,     0,     0,     0,     0,     0,
    62,     0,     0,    42,    43,    44,    84,     0,     0,    51,
    52,    53,    54,    55,    56,    50,    81,     0,    63,     0,
    64,    17,     0,     0,     0,     0,     0,     0,    80,    79,
    72,    73,    74,    76,    77,    78,    75,    62,     0,     0,
     0,     0,    71,    84,    10,     0,    51,    52,    53,    54,
    55,    56,    50,    81,     0,    63,     0,    64,    17,     0,
     0,     0,     0,     0,     0,    80,    79,    72,    73,    74,
    76,    77,    78,    75,    83,     0,     0,     0,     0,    71,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    83,   272,    11,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    62,     0,     0,    83,     0,
     0,    84,     0,     0,    51,    52,    53,    54,    55,    56,
    50,    81,     0,    63,    83,    64,    17,     0,     0,     0,
     0,     0,     0,    80,    79,    72,    73,    74,    76,    77,
    78,    75,     0,     0,     0,     0,     0,    71,    33,    34,
    35,    36,    37,    38,     0,     0,     0,     0,     0,    31,
     0,     0,    30,     0,     0,     0,     0,     0,    32,     0,
    46,    51,    52,    53,    54,    55,    56,    50,    15,     0,
    14,     0,    16,    17,     0,     0,     0,     0,     0,     0,
     0,     0,    98,    99,   100,   101,   102,    97,     0,     0,
     0,    42,    43,    44,    33,    34,    35,    36,    37,    38,
   103,   104,     0,     0,     0,    31,     0,     0,    30,     0,
     0,     0,     0,     0,    32,     0,     0,    51,    52,    53,
    54,    55,    56,    50,    15,     0,    14,     0,    16,    17,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    42,    43,    44,
   252,   253,   254,   255,   256,   257,   247,   248,   242,   244,
   239,   258,     0,   241,     0,   245,   249,   250,   238,   259,
   246,   240,   251,   260,   261,   262,   263,   264,   265,     0,
   266,   267,   268,   243,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    86,     0,     0,     0,     0,
     0,    84,     0,     0,    51,    52,    53,    54,    55,    56,
    50,    81,    86,    87,     0,    88,    17,     0,    84,     0,
     0,    51,    52,    53,    54,    55,    56,    50,    81,    86,
    87,     0,    88,    17,     0,    84,     0,     0,    51,    52,
    53,    54,    55,    56,    50,    81,     0,    87,     0,    88,
    17,    84,     0,     0,    51,    52,    53,    54,    55,    56,
    50,    81,     0,   276,     0,   275,    17 };
static YYCONST yytabelem yypact[]={

-10000000,   438,-10000000,    45,    45,    45,    45,   589,   894,    45,
-10000000,-10000000,  -257,  -257,   759,   149,   -52,    -8,-10000000,-10000000,
   751,-10000000,   751,   751,   751,-10000000,   130,   129,   125,-10000000,
-10000000,-10000000,   124,   123,   122,   121,   120,   119,   118,   751,
   807,   807,   807,  -272,  -277,  -275,   751,-10000000,-10000000,-10000000,
-10000000,   117,   116,   115,   114,   113,   112,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,   -25,   -16,-10000000,   807,   807,   807,   807,
   807,   807,   807,   807,   807,   807,   807,   807,   807,   807,
   807,   -24,-10000000,   751,   111,-10000000,-10000000,   -25,   -16,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   751,   751,   807,   807,
   807,   807,   807,-10000000,-10000000,   751,   751,   807,   807,   807,
   807,   807,   751,   807,   807,   807,   807,   807,   751,   694,
   928,-10000000,   110,    77,   694,   928,    78,   928,   694,    77,
  -235,   807,   807,   807,   497,   109,  -238,   105,   751,   863,
   751,   751,   751,   911,   626,  -310,  -275,  -310,  -310,-10000000,
-10000000,   944,   944,   944,   944,   944,   944,    76,   928,   104,
   751,   751,   751,   751,   751,   751,   751,   751,   -27,   -27,
  -310,  -310,  -310,  -310,   -29,   -29,   -29,   -29,   -29,   -29,
   -29,   352,   273,   751,   751,  -265,-10000000,-10000000,   694,   928,
   694,   928,   225,   225,   225,   225,   225,   694,   928,   694,
   928,   225,   225,   225,   225,   225,   694,   928,   225,   225,
   225,   225,   225,   928,   751,   751,   751,    61,-10000000,-10000000,
   103,   225,    -2,   102,-10000000,-10000000,-10000000,   694,   928,   751,
   101,   100,-10000000,    99,   694,   928,    96,    95,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,    90,
    52,    51,-10000000,-10000000,-10000000,   -16,   -25,-10000000,-10000000,-10000000,
-10000000,-10000000,   751,-10000000,    89,    88,    50,    42,    22,   108,
   107,    41,   694,   928,   694,   928,   694,   928,  -239,-10000000,
   807,   553,    19,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   751,
   751,   694,   928,-10000000,-10000000,   751,   751,   751,  -243,  -243,
-10000000,   751,-10000000,-10000000,   553,    -3,-10000000,-10000000,    87,    86,
    85,    84,    20,    18,-10000000,-10000000,    17,   694,   928,-10000000,
   807,  -231,-10000000,-10000000,-10000000,-10000000,   751,-10000000,  -254,-10000000,
-10000000,    83,   553,    82,   553,-10000000,-10000000,   553,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,   553,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   298,     0,    65,   178,   172,   206,   205,   204,     5,
   202,   201,   199,   171,     1,   174,   319,   179,   198,   196,
   175,    39,   195,    83,   194,   193,   192,   191,   190,   186,
   183,   182,    38,     7,     6,   181,     4,   177,   173 };
static YYCONST yytabelem yyr1[]={

     0,    22,    22,    22,    22,    22,    22,    22,    22,    22,
    23,    23,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     4,     4,    15,    15,    15,    15,    15,    15,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,    25,
     3,    27,    28,     3,    29,    30,     3,     3,     3,     3,
     9,    10,    11,    12,    13,    14,    18,    18,    18,    18,
    18,    18,    18,    18,    18,    18,    18,    18,    18,    18,
    19,    19,    19,    19,    19,    19,    19,    19,    19,    19,
    19,    19,    19,    19,    19,    19,    19,     8,     8,     8,
     1,     1,    17,    17,    17,    17,    17,    17,    17,    17,
    17,    17,    17,    17,    16,    16,    16,    16,    16,    16,
    16,    16,    16,    16,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,    21,    21,    21,    21,
    21,     5,     5,     5,     5,     6,     6,     6,     7,     7,
     7,    31,    35,    24,    37,    38,    24,    20,    20,    20,
    32,    34,    36,    33,    33,    33,    33,    33,    26,    26,
    26,    26,    26 };
static YYCONST yytabelem yyr2[]={

     0,     0,     4,     6,     7,     7,     7,     7,     7,     7,
     2,     2,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     3,     3,
     3,     3,     3,     5,     5,    11,     5,     5,     5,     1,
     7,     1,     1,    17,     1,     1,    25,    13,    19,     7,
     3,     3,     3,     3,     1,     1,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     1,     4,     4,
     2,     2,     3,     3,     3,     2,     7,     9,     7,     9,
     9,    13,    13,    17,     2,     5,     5,     5,     5,     5,
     5,     5,     5,     5,     3,     3,     3,     2,    11,     9,
     7,     9,     9,     9,     9,    13,    13,     7,     7,     7,
     7,     7,     7,     7,     5,     5,     7,     7,     7,     7,
     7,     7,     7,     7,     7,     5,     5,     5,     5,     5,
     7,     7,     7,     7,     7,     7,     1,     3,     3,     7,
     7,     3,     3,     7,     7,     3,     7,     7,     3,     7,
     7,     1,     1,    22,     1,     1,    22,     2,     2,     2,
     1,     1,     1,     0,     3,     3,     7,     7,     0,     3,
     3,     7,     7 };
static YYCONST yytabelem yychk[]={

-10000000,   -22,   -23,   -24,    -4,   -15,    -3,    -2,   -16,   256,
    10,    59,   267,   275,   289,   287,   291,   292,   263,   264,
   278,   276,   272,   273,   274,   269,   -10,   -11,   -12,   123,
   271,   268,   277,   257,   258,   259,   260,   261,   262,    40,
    45,    43,   310,   311,   312,   -17,   279,   290,   266,   270,
   286,   280,   281,   282,   283,   284,   285,   -23,   -23,   -23,
   -23,   -23,   271,   289,   291,   -17,    43,    45,    42,    47,
    37,   313,   301,   302,   303,   307,   304,   305,   306,   300,
   299,   287,   -15,    40,   277,   -23,   271,   289,   291,   -17,
   -23,   -20,   289,   268,   276,   -20,    61,   298,   293,   294,
   295,   296,   297,   311,   312,    61,   298,   293,   294,   295,
   296,   297,    61,   293,   294,   295,   296,   297,    61,    -2,
   -16,    -4,   -13,    -5,    -2,   -16,    -6,   -16,    -2,    -5,
   -25,    40,    40,    40,    -8,   -13,    40,    40,    40,    40,
    40,    40,    40,   -16,    -2,    -2,   -17,    -2,    -2,   289,
   289,   301,   302,   303,   304,   305,   306,    -7,   -16,    40,
    40,    40,    40,    40,    40,    61,   298,    61,    -2,    -2,
    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
    -2,    -2,    -2,    61,   298,    40,   -31,   -37,    -2,   -16,
    -2,   -16,    -2,    -2,    -2,    -2,    -2,    -2,   -16,    -2,
   -16,    -2,    -2,    -2,    -2,    -2,    -2,   -16,    -2,    -2,
    -2,    -2,    -2,   -16,    40,    44,    44,   -26,   289,   287,
    -9,    -2,    -9,    -9,   125,   -23,    -3,    -2,   -16,    40,
   287,   289,    41,    -1,    -2,   -16,   -18,   -19,   275,   267,
   278,   270,   265,   290,   266,   272,   277,   263,   264,   273,
   274,   279,   257,   258,   259,   260,   261,   262,   268,   276,
   280,   281,   282,   283,   284,   285,   287,   288,   289,    -1,
    -1,    -1,    41,    41,   -17,   291,   289,   -17,   -17,   -17,
   -17,   -17,    44,    41,    -1,    -1,    -1,    -1,    -1,   -32,
   -32,   -21,    -2,   -16,    -2,   -16,    -2,   -16,    44,    41,
    59,    41,   -21,    41,    41,    41,    41,    41,    41,    44,
    44,    -2,   -16,    41,    41,    44,    44,    44,    40,    40,
    41,    44,   289,   287,   -27,    -9,    -3,    41,    -1,    -1,
    -1,    -1,    -1,   -33,   289,   287,   -33,    -2,   -16,    -3,
    59,   -14,    41,    41,    41,    41,    44,    41,    44,    41,
   -28,    -9,   265,    -1,   -34,   289,   287,   -34,   -14,    41,
    -3,    41,    -3,    -3,   -29,   -14,   -35,   -38,    -3,   -36,
   -36,   -30,   -14 };
static YYCONST yytabelem yydef[]={

     1,    -2,     2,     0,   127,   105,     0,     0,     0,     0,
    10,    11,     0,     0,   125,   103,   126,   104,    40,    41,
    42,    64,     0,     0,     0,    49,     0,     0,     0,    97,
   124,    64,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   114,     0,    61,    62,    63,
   102,     0,     0,     0,     0,     0,     0,     3,     4,     5,
     6,     7,   115,   116,   117,   118,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   103,   105,     0,     0,     8,   119,   120,   121,   122,
     9,   181,   187,   188,   189,   184,     0,     0,     0,     0,
     0,     0,     0,   158,   159,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
    44,   127,     0,    46,   171,   172,    47,   175,     0,    48,
   198,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   144,     0,   145,   155,   156,
   157,     0,     0,     0,     0,     0,     0,   123,   178,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   138,   139,
   140,   141,   142,   143,   146,   147,   148,   149,   150,   151,
   152,   153,   154,     0,     0,     0,   190,   190,    12,    36,
    13,    37,    14,    15,    16,    17,    18,    19,    32,    20,
    33,    21,    22,    23,    24,    25,    26,    35,    27,    28,
    29,    30,    31,    34,   166,     0,     0,    50,   199,   200,
     0,    60,     0,     0,    59,    98,    99,    38,    39,   166,
     0,     0,   130,     0,   100,   101,     0,     0,    66,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    89,    90,    91,    92,    93,    94,    95,    96,     0,
     0,     0,   106,   137,   160,     0,     0,   161,   162,   163,
   164,   165,     0,   108,     0,     0,     0,     0,     0,     0,
     0,     0,   167,   168,   173,   174,   176,   177,     0,    51,
     0,     0,     0,   107,   129,   131,   132,   133,   134,     0,
     0,   179,   180,   109,   110,     0,     0,     0,   193,   193,
    45,     0,   201,   202,     0,     0,    65,   128,     0,     0,
     0,     0,     0,     0,   194,   195,     0,   169,   170,    52,
     0,    57,   135,   136,   111,   112,     0,   191,     0,   191,
    65,     0,     0,     0,     0,   196,   197,     0,    53,    54,
    65,   113,   182,   185,     0,    58,   192,   192,    55,   183,
   186,    65,    56 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"BLTIN0",	257,
	"BLTIN1",	258,
	"BLTIN1I",	259,
	"BLTIN1S",	260,
	"BLTIN2",	261,
	"BLTIN2S",	262,
	"BREAK",	263,
	"CONTINUE",	264,
	"ELSE",	265,
	"FOR",	266,
	"FUNC",	267,
	"FUNCTION",	268,
	"GLOBAL",	269,
	"IF",	270,
	"NUMBER",	271,
	"PRINT",	272,
	"PRINTF",	273,
	"PRINTLN",	274,
	"PROC",	275,
	"PROCEDURE",	276,
	"READ",	277,
	"RETURN",	278,
	"SPRINTF",	279,
	"STRBLTIN0",	280,
	"STRBLTIN1",	281,
	"STRBLTIN1N",	282,
	"STRBLTIN2",	283,
	"STRBLTIN2SN",	284,
	"STRBLTIN3SNN",	285,
	"STRING",	286,
	"STRVAR",	287,
	"UNDEF",	288,
	"VAR",	289,
	"WHILE",	290,
	"ARG",	291,
	"STRARG",	292,
	"=",	61,
	"ADDEQ",	293,
	"SUBEQ",	294,
	"MULEQ",	295,
	"DIVEQ",	296,
	"MODEQ",	297,
	"CONSTEQ",	298,
	"OR",	299,
	"AND",	300,
	"GT",	301,
	"GE",	302,
	"LT",	303,
	"LE",	304,
	"EQ",	305,
	"NE",	306,
	"LTGT",	307,
	"+",	43,
	"-",	45,
	"*",	42,
	"/",	47,
	"%",	37,
	"UNARYMINUS",	308,
	"UNARYPLUS",	309,
	"NOT",	310,
	"INC",	311,
	"DEC",	312,
	"TO_POWER",	313,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"list : /* empty */",
	"list : list eos",
	"list : list defn eos",
	"list : list asgn eos",
	"list : list strasgn eos",
	"list : list stmt eos",
	"list : list expr eos",
	"list : list strexpr eos",
	"list : list error eos",
	"eos : '\n'",
	"eos : ';'",
	"asgn : VAR '=' expr",
	"asgn : VAR CONSTEQ expr",
	"asgn : VAR ADDEQ expr",
	"asgn : VAR SUBEQ expr",
	"asgn : VAR MULEQ expr",
	"asgn : VAR DIVEQ expr",
	"asgn : VAR MODEQ expr",
	"asgn : STRVAR '=' expr",
	"asgn : STRVAR CONSTEQ expr",
	"asgn : STRVAR ADDEQ expr",
	"asgn : STRVAR SUBEQ expr",
	"asgn : STRVAR MULEQ expr",
	"asgn : STRVAR DIVEQ expr",
	"asgn : STRVAR MODEQ expr",
	"asgn : ARG '=' expr",
	"asgn : ARG ADDEQ expr",
	"asgn : ARG SUBEQ expr",
	"asgn : ARG MULEQ expr",
	"asgn : ARG DIVEQ expr",
	"asgn : ARG MODEQ expr",
	"strasgn : STRVAR '=' strexpr",
	"strasgn : STRVAR CONSTEQ strexpr",
	"strasgn : STRARG '=' strexpr",
	"strasgn : ARG '=' strexpr",
	"strasgn : VAR '=' strexpr",
	"strasgn : VAR CONSTEQ strexpr",
	"stmt : expr",
	"stmt : strexpr",
	"stmt : BREAK",
	"stmt : CONTINUE",
	"stmt : RETURN",
	"stmt : RETURN expr",
	"stmt : RETURN strexpr",
	"stmt : PROCEDURE begin '(' arglist ')'",
	"stmt : PRINT prlist",
	"stmt : PRINTF prflist",
	"stmt : PRINTLN prlist",
	"stmt : GLOBAL",
	"stmt : GLOBAL globalnames",
	"stmt : while '(' cond ')'",
	"stmt : while '(' cond ')' stmt",
	"stmt : while '(' cond ')' stmt end",
	"stmt : for '(' cond ';' cond ';' cond ')'",
	"stmt : for '(' cond ';' cond ';' cond ')' stmt",
	"stmt : for '(' cond ';' cond ';' cond ')' stmt end",
	"stmt : if '(' cond ')' stmt end",
	"stmt : if '(' cond ')' stmt end ELSE stmt end",
	"stmt : '{' stmtlist '}'",
	"cond : expr",
	"while : WHILE",
	"for : FOR",
	"if : IF",
	"begin : /* empty */",
	"end : /* empty */",
	"keyword : PROC",
	"keyword : FUNC",
	"keyword : RETURN",
	"keyword : IF",
	"keyword : ELSE",
	"keyword : WHILE",
	"keyword : FOR",
	"keyword : PRINT",
	"keyword : READ",
	"keyword : BREAK",
	"keyword : CONTINUE",
	"keyword : PRINTF",
	"keyword : PRINTLN",
	"keyword : SPRINTF",
	"otherword : BLTIN0",
	"otherword : BLTIN1",
	"otherword : BLTIN1I",
	"otherword : BLTIN1S",
	"otherword : BLTIN2",
	"otherword : BLTIN2S",
	"otherword : FUNCTION",
	"otherword : PROCEDURE",
	"otherword : STRBLTIN0",
	"otherword : STRBLTIN1",
	"otherword : STRBLTIN1N",
	"otherword : STRBLTIN2",
	"otherword : STRBLTIN2SN",
	"otherword : STRBLTIN3SNN",
	"otherword : STRVAR",
	"otherword : UNDEF",
	"otherword : VAR",
	"stmtlist : /* empty */",
	"stmtlist : stmtlist eos",
	"stmtlist : stmtlist stmt",
	"anyexpr : expr",
	"anyexpr : strexpr",
	"string : STRING",
	"string : STRVAR",
	"string : STRARG",
	"string : strasgn",
	"string : '(' strexpr ')'",
	"string : READ '(' STRVAR ')'",
	"string : STRBLTIN0 '(' ')'",
	"string : STRBLTIN1 '(' anyexpr ')'",
	"string : STRBLTIN1N '(' anyexpr ')'",
	"string : STRBLTIN2 '(' anyexpr ',' anyexpr ')'",
	"string : STRBLTIN2SN '(' anyexpr ',' anyexpr ')'",
	"string : STRBLTIN3SNN '(' anyexpr ',' anyexpr ',' anyexpr ')'",
	"strexpr : string",
	"strexpr : expr NUMBER",
	"strexpr : expr VAR",
	"strexpr : expr ARG",
	"strexpr : expr string",
	"strexpr : strexpr NUMBER",
	"strexpr : strexpr VAR",
	"strexpr : strexpr ARG",
	"strexpr : strexpr string",
	"strexpr : SPRINTF sprflist",
	"expr : NUMBER",
	"expr : VAR",
	"expr : ARG",
	"expr : asgn",
	"expr : FUNCTION begin '(' arglist ')'",
	"expr : READ '(' VAR ')'",
	"expr : BLTIN0 '(' ')'",
	"expr : BLTIN1 '(' anyexpr ')'",
	"expr : BLTIN1I '(' keyword ')'",
	"expr : BLTIN1I '(' otherword ')'",
	"expr : BLTIN1S '(' anyexpr ')'",
	"expr : BLTIN2 '(' anyexpr ',' anyexpr ')'",
	"expr : BLTIN2S '(' anyexpr ',' anyexpr ')'",
	"expr : '(' expr ')'",
	"expr : expr '+' expr",
	"expr : expr '-' expr",
	"expr : expr '*' expr",
	"expr : expr '/' expr",
	"expr : expr '%' expr",
	"expr : expr TO_POWER expr",
	"expr : '-' expr",
	"expr : '+' expr",
	"expr : expr GT expr",
	"expr : expr GE expr",
	"expr : expr LT expr",
	"expr : expr LTGT expr",
	"expr : expr LE expr",
	"expr : expr EQ expr",
	"expr : expr NE expr",
	"expr : expr AND expr",
	"expr : expr OR expr",
	"expr : NOT expr",
	"expr : INC VAR",
	"expr : DEC VAR",
	"expr : VAR INC",
	"expr : VAR DEC",
	"expr : string GT string",
	"expr : string GE string",
	"expr : string LT string",
	"expr : string LE string",
	"expr : string EQ string",
	"expr : string NE string",
	"arglist : /* empty */",
	"arglist : expr",
	"arglist : strexpr",
	"arglist : arglist ',' expr",
	"arglist : arglist ',' strexpr",
	"prlist : expr",
	"prlist : strexpr",
	"prlist : prlist ',' expr",
	"prlist : prlist ',' strexpr",
	"prflist : strexpr",
	"prflist : prflist ',' expr",
	"prflist : prflist ',' strexpr",
	"sprflist : strexpr",
	"sprflist : sprflist ',' expr",
	"sprflist : sprflist ',' strexpr",
	"defn : FUNC procname",
	"defn : FUNC procname beginargs '(' argnames ')' endargs stmt",
	"defn : FUNC procname beginargs '(' argnames ')' endargs stmt enddefn",
	"defn : PROC procname",
	"defn : PROC procname beginargs '(' argnames ')' endargs stmt",
	"defn : PROC procname beginargs '(' argnames ')' endargs stmt enddefn",
	"procname : VAR",
	"procname : FUNCTION",
	"procname : PROCEDURE",
	"beginargs : /* empty */",
	"endargs : /* empty */",
	"enddefn : /* empty */",
	"argnames : /* empty */",
	"argnames : VAR",
	"argnames : STRVAR",
	"argnames : argnames ',' VAR",
	"argnames : argnames ',' STRVAR",
	"globalnames : /* empty */",
	"globalnames : VAR",
	"globalnames : STRVAR",
	"globalnames : globalnames ',' VAR",
	"globalnames : globalnames ',' STRVAR",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register YYCONST int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 4:
# line 227 "hoc.y"
{ CODE2(xpop, STOP); return 1; } break;
case 5:
# line 228 "hoc.y"
{ CODE2(xpop, STOP); return 1; } break;
case 6:
# line 229 "hoc.y"
{ CODE(STOP); return 1; } break;
case 7:
# line 230 "hoc.y"
{ CODE2(printtop, STOP); return 1; } break;
case 8:
# line 231 "hoc.y"
{ CODE2(printtopstring, STOP); return 1; } break;
case 9:
# line 232 "hoc.y"
{ yyerrok; } break;
case 12:
# line 239 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,assign); yyval.inst=yypvt[-0].inst; } break;
case 13:
# line 240 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,const_assign); yyval.inst=yypvt[-0].inst; } break;
case 14:
# line 241 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,addeq); yyval.inst=yypvt[-0].inst; } break;
case 15:
# line 242 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,subeq); yyval.inst=yypvt[-0].inst; } break;
case 16:
# line 243 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,muleq); yyval.inst=yypvt[-0].inst; } break;
case 17:
# line 244 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,diveq); yyval.inst=yypvt[-0].inst; } break;
case 18:
# line 245 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,modeq); yyval.inst=yypvt[-0].inst; } break;
case 19:
# line 246 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,assign); yyval.inst=yypvt[-0].inst; } break;
case 20:
# line 247 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,const_assign); yyval.inst=yypvt[-0].inst; } break;
case 21:
# line 248 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,addeq); yyval.inst=yypvt[-0].inst; } break;
case 22:
# line 249 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,subeq); yyval.inst=yypvt[-0].inst; } break;
case 23:
# line 250 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,muleq); yyval.inst=yypvt[-0].inst; } break;
case 24:
# line 251 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,diveq); yyval.inst=yypvt[-0].inst; } break;
case 25:
# line 252 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,modeq); yyval.inst=yypvt[-0].inst; } break;
case 26:
# line 253 "hoc.y"
{ defnonly("$"); CODE2(argassign,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 27:
# line 254 "hoc.y"
{ defnonly("$"); CODE2(argaddeq,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 28:
# line 255 "hoc.y"
{ defnonly("$"); CODE2(argsubeq,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 29:
# line 256 "hoc.y"
{ defnonly("$"); CODE2(argmuleq,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 30:
# line 257 "hoc.y"
{ defnonly("$"); CODE2(argdiveq,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 31:
# line 258 "hoc.y"
{ defnonly("$"); CODE2(argmodeq,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 32:
# line 261 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,str_assign); yyval.inst=yypvt[-0].inst; } break;
case 33:
# line 262 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,const_str_assign); yyval.inst=yypvt[-0].inst; } break;
case 34:
# line 263 "hoc.y"
{ defnonly("$"); CODE2(argassign,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 35:
# line 264 "hoc.y"
{ defnonly("$"); CODE2(argassign,(Inst)yypvt[-2].narg); yyval.inst=yypvt[-0].inst;} break;
case 36:
# line 265 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,str_assign); yyval.inst=yypvt[-0].inst; } break;
case 37:
# line 266 "hoc.y"
{ CODE3(varpush,(Inst)yypvt[-2].sym,const_str_assign); yyval.inst=yypvt[-0].inst; } break;
case 38:
# line 269 "hoc.y"
{ CODE(xpop); } break;
case 39:
# line 270 "hoc.y"
{ CODE(xpop); } break;
case 40:
# line 271 "hoc.y"
{ looponly("break"); yyval.inst = code(breakstmt); } break;
case 41:
# line 272 "hoc.y"
{ looponly("continue"); yyval.inst = code(continuestmt); } break;
case 42:
# line 273 "hoc.y"
{ defnonly("return"); yyval.inst = code(procret); } break;
case 43:
# line 274 "hoc.y"
{ defnonly("return"); yyval.inst=yypvt[-0].inst; CODE(funcret); } break;
case 44:
# line 275 "hoc.y"
{ defnonly("return"); yyval.inst=yypvt[-0].inst; CODE(funcret); } break;
case 45:
# line 276 "hoc.y"
{ yyval.inst = yypvt[-3].inst; CODE3(call, (Inst)yypvt[-4].sym, (Inst)yypvt[-1].narg); } break;
case 46:
# line 277 "hoc.y"
{ yyval.inst = yypvt[-0].inst; } break;
case 47:
# line 278 "hoc.y"
{ yyval.inst = yypvt[-0].inst; } break;
case 48:
# line 279 "hoc.y"
{ yyval.inst = yypvt[-0].inst; CODE(prnl); } break;
case 49:
# line 280 "hoc.y"
{ in_global_stmt = 1; } break;
case 50:
# line 281 "hoc.y"
{ in_global_stmt = 0; } break;
case 51:
# line 282 "hoc.y"
{ inloop++; } break;
case 52:
# line 282 "hoc.y"
{ inloop--; } break;
case 53:
# line 282 "hoc.y"
{
		(yypvt[-7].inst)[1] = (Inst)yypvt[-2].inst;	/* body of loop */
		(yypvt[-7].inst)[2] = (Inst)yypvt[-0].inst; } break;
case 54:
# line 285 "hoc.y"
{ inloop++; } break;
case 55:
# line 285 "hoc.y"
{ inloop--; } break;
case 56:
# line 285 "hoc.y"
{
		(yypvt[-11].inst)[1] = (Inst)yypvt[-7].inst;	/* condition */
		(yypvt[-11].inst)[2] = (Inst)yypvt[-5].inst;	/* post loop */
		(yypvt[-11].inst)[3] = (Inst)yypvt[-2].inst;	/* body of loop */
		(yypvt[-11].inst)[4] = (Inst)yypvt[-0].inst; } break;
case 57:
# line 290 "hoc.y"
{	/* else-less if */
		(yypvt[-5].inst)[1] = (Inst)yypvt[-1].inst;	/* thenpart */
		(yypvt[-5].inst)[3] = (Inst)yypvt[-0].inst; } break;
case 58:
# line 293 "hoc.y"
{	/* if with else */
		(yypvt[-8].inst)[1] = (Inst)yypvt[-4].inst;	/* thenpart */
		(yypvt[-8].inst)[2] = (Inst)yypvt[-1].inst;	/* elsepart */
		(yypvt[-8].inst)[3] = (Inst)yypvt[-0].inst; } break;
case 59:
# line 297 "hoc.y"
{ yyval.inst = yypvt[-1].inst; } break;
case 60:
# line 299 "hoc.y"
{ CODE(STOP); } break;
case 61:
# line 301 "hoc.y"
{ yyval.inst = code3(whilecode,STOP,STOP); } break;
case 62:
# line 303 "hoc.y"
{ yyval.inst = code(forcode); CODE4(STOP,STOP,STOP,STOP); } break;
case 63:
# line 305 "hoc.y"
{ yyval.inst = code(ifcode); CODE3(STOP,STOP,STOP); } break;
case 64:
# line 307 "hoc.y"
{ yyval.inst = progp; } break;
case 65:
# line 309 "hoc.y"
{ CODE(STOP); yyval.inst = progp; } break;
case 97:
# line 349 "hoc.y"
{ yyval.inst = progp; } break;
case 102:
# line 358 "hoc.y"
{ yyval.inst = code2(const_str_push, (Inst)yypvt[-0].sym); } break;
case 103:
# line 359 "hoc.y"
{ yyval.inst = code3(varpush, (Inst)yypvt[-0].sym, streval); } break;
case 104:
# line 360 "hoc.y"
{ defnonly("$$"); yyval.inst = code2(strarg, (Inst)yypvt[-0].narg); } break;
case 106:
# line 362 "hoc.y"
{ yyval.inst = yypvt[-1].inst; } break;
case 107:
# line 363 "hoc.y"
{ yyval.inst = code2(varread, (Inst)yypvt[-1].sym); } break;
case 108:
# line 364 "hoc.y"
{ yyval.inst = code2(strbltin0, (Inst)yypvt[-2].sym->u.sptr0); } break;
case 109:
# line 365 "hoc.y"
{ yyval.inst = yypvt[-1].inst; CODE2(strbltin1, (Inst)yypvt[-3].sym->u.sptr1); } break;
case 110:
# line 366 "hoc.y"
{ yyval.inst = yypvt[-1].inst; CODE2(strbltin1n, (Inst)yypvt[-3].sym->u.sptr1n); } break;
case 111:
# line 367 "hoc.y"
{ yyval.inst = yypvt[-3].inst; CODE2(strbltin2, (Inst)yypvt[-5].sym->u.sptr2); } break;
case 112:
# line 368 "hoc.y"
{ yyval.inst = yypvt[-3].inst; CODE2(strbltin2sn, (Inst)yypvt[-5].sym->u.sptr2sn); } break;
case 113:
# line 369 "hoc.y"
{ yyval.inst = yypvt[-5].inst; CODE2(strbltin3snn, (Inst)yypvt[-7].sym->u.sptr3snn); } break;
case 115:
# line 373 "hoc.y"
{ CODE3(constpush, (Inst)yypvt[-0].sym, str_concat); } break;
case 116:
# line 374 "hoc.y"
{ CODE3(constpush, (Inst)yypvt[-0].sym, str_concat); } break;
case 117:
# line 375 "hoc.y"
{ CODE3(arg, (Inst)yypvt[-0].narg, str_concat); } break;
case 118:
# line 376 "hoc.y"
{ CODE(str_concat); } break;
case 119:
# line 377 "hoc.y"
{ CODE3(constpush, (Inst)yypvt[-0].sym, str_concat); } break;
case 120:
# line 378 "hoc.y"
{ CODE3(constpush, (Inst)yypvt[-0].sym, str_concat); } break;
case 121:
# line 379 "hoc.y"
{ CODE3(arg, (Inst)yypvt[-0].narg, str_concat); } break;
case 122:
# line 380 "hoc.y"
{ CODE(str_concat); } break;
case 123:
# line 381 "hoc.y"
{ yyval.inst = yypvt[-0].inst; } break;
case 124:
# line 384 "hoc.y"
{ yyval.inst = code2(constpush, (Inst)yypvt[-0].sym); } break;
case 125:
# line 385 "hoc.y"
{ yyval.inst = code3(varpush, (Inst)yypvt[-0].sym, eval); } break;
case 126:
# line 386 "hoc.y"
{ defnonly("$"); yyval.inst = code2(arg, (Inst)yypvt[-0].narg); } break;
case 128:
# line 388 "hoc.y"
{ yyval.inst = yypvt[-3].inst; CODE3(call,(Inst)yypvt[-4].sym,(Inst)yypvt[-1].narg); } break;
case 129:
# line 389 "hoc.y"
{ yyval.inst = code2(varread, (Inst)yypvt[-1].sym); } break;
case 130:
# line 390 "hoc.y"
{ yyval.inst = code2(bltin0, (Inst)yypvt[-2].sym->u.ptr0); } break;
case 131:
# line 391 "hoc.y"
{ yyval.inst = yypvt[-1].inst; CODE2(bltin1, (Inst)yypvt[-3].sym->u.ptr1); } break;
case 132:
# line 392 "hoc.y"
{ yyval.inst = code3(bltin1i, (Inst)yypvt[-3].sym->u.ptr1i, (Inst)yypvt[-1].sym); } break;
case 133:
# line 393 "hoc.y"
{ yyval.inst = code3(bltin1i, (Inst)yypvt[-3].sym->u.ptr1i, (Inst)yypvt[-1].sym); } break;
case 134:
# line 394 "hoc.y"
{ yyval.inst = yypvt[-1].inst; CODE2(bltin1s, (Inst)yypvt[-3].sym->u.ptr1s); } break;
case 135:
# line 395 "hoc.y"
{ yyval.inst = yypvt[-3].inst; CODE2(bltin2, (Inst)yypvt[-5].sym->u.ptr2); } break;
case 136:
# line 396 "hoc.y"
{ yyval.inst = yypvt[-3].inst; CODE2(bltin2s, (Inst)yypvt[-5].sym->u.ptr2s); } break;
case 137:
# line 397 "hoc.y"
{ yyval.inst = yypvt[-1].inst; } break;
case 138:
# line 398 "hoc.y"
{ CODE(add); } break;
case 139:
# line 399 "hoc.y"
{ CODE(sub); } break;
case 140:
# line 400 "hoc.y"
{ CODE(mul); } break;
case 141:
# line 401 "hoc.y"
{ CODE(divop); } break;
case 142:
# line 402 "hoc.y"
{ CODE(mod); } break;
case 143:
# line 403 "hoc.y"
{ CODE(power); } break;
case 144:
# line 404 "hoc.y"
{ yyval.inst=yypvt[-0].inst; CODE(negate); } break;
case 145:
# line 405 "hoc.y"
{ yyval.inst=yypvt[-0].inst; CODE(noop); } break;
case 146:
# line 406 "hoc.y"
{ CODE(gt); } break;
case 147:
# line 407 "hoc.y"
{ CODE(ge); } break;
case 148:
# line 408 "hoc.y"
{ CODE(lt); } break;
case 149:
# line 409 "hoc.y"
{ CODE(ltgt); } break;
case 150:
# line 410 "hoc.y"
{ CODE(le); } break;
case 151:
# line 411 "hoc.y"
{ CODE(eq); } break;
case 152:
# line 412 "hoc.y"
{ CODE(ne); } break;
case 153:
# line 413 "hoc.y"
{ CODE(And); } break;
case 154:
# line 414 "hoc.y"
{ CODE(Or); } break;
case 155:
# line 415 "hoc.y"
{ yyval.inst = yypvt[-0].inst; CODE(Not); } break;
case 156:
# line 416 "hoc.y"
{ yyval.inst = code2(preinc,(Inst)yypvt[-0].sym); } break;
case 157:
# line 417 "hoc.y"
{ yyval.inst = code2(predec,(Inst)yypvt[-0].sym); } break;
case 158:
# line 418 "hoc.y"
{ yyval.inst = code2(postinc,(Inst)yypvt[-1].sym); } break;
case 159:
# line 419 "hoc.y"
{ yyval.inst = code2(postdec,(Inst)yypvt[-1].sym); } break;
case 160:
# line 420 "hoc.y"
{ CODE(str_gt); } break;
case 161:
# line 421 "hoc.y"
{ CODE(str_ge); } break;
case 162:
# line 422 "hoc.y"
{ CODE(str_lt); } break;
case 163:
# line 423 "hoc.y"
{ CODE(str_le); } break;
case 164:
# line 424 "hoc.y"
{ CODE(str_eq); } break;
case 165:
# line 425 "hoc.y"
{ CODE(str_ne); } break;
case 166:
# line 428 "hoc.y"
{ yyval.narg = 0; } break;
case 167:
# line 429 "hoc.y"
{ yyval.narg = 1; } break;
case 168:
# line 430 "hoc.y"
{ yyval.narg = 1; } break;
case 169:
# line 431 "hoc.y"
{ yyval.narg = yypvt[-2].narg + 1; } break;
case 170:
# line 432 "hoc.y"
{ yyval.narg = yypvt[-2].narg + 1; } break;
case 171:
# line 435 "hoc.y"
{ CODE(prexpr); } break;
case 172:
# line 436 "hoc.y"
{ CODE(prstr); } break;
case 173:
# line 437 "hoc.y"
{ CODE2(prsep,prexpr); } break;
case 174:
# line 438 "hoc.y"
{ CODE2(prsep,prstr); } break;
case 175:
# line 441 "hoc.y"
{ CODE(prfmt); } break;
case 176:
# line 442 "hoc.y"
{ CODE(prexpr); } break;
case 177:
# line 443 "hoc.y"
{ CODE(prstr); } break;
case 178:
# line 446 "hoc.y"
{ CODE(sprfmt); } break;
case 179:
# line 447 "hoc.y"
{ CODE2(sprexpr,sprjoin); } break;
case 180:
# line 448 "hoc.y"
{ CODE2(sprstr,sprjoin); } break;
case 181:
# line 451 "hoc.y"
{ yypvt[-0].sym->type = FUNCTION; indef = 1; } break;
case 182:
# line 453 "hoc.y"
{ CODE(procret); define(yypvt[-7].sym); } break;
case 184:
# line 455 "hoc.y"
{ yypvt[-0].sym->type = PROCEDURE; indef = 1; } break;
case 185:
# line 457 "hoc.y"
{ CODE(procret); define(yypvt[-7].sym); } break;
case 190:
# line 466 "hoc.y"
{
							this_procname = yylval.sym->name;
							free_argnamelist();
							free_globallist();
							ndefns++;
							in_argnamelist = 1;
						} break;
case 191:
# line 474 "hoc.y"
{ in_argnamelist = 0; } break;
case 192:
# line 476 "hoc.y"
{ indef = 0; free_argnamelist(); free_globallist(); } break;
case 194:
# line 479 "hoc.y"
{ addarg(yypvt[-0].sym); } break;
case 195:
# line 480 "hoc.y"
{ addarg(yypvt[-0].sym); } break;
case 196:
# line 481 "hoc.y"
{ addarg(yypvt[-0].sym); } break;
case 197:
# line 482 "hoc.y"
{ addarg(yypvt[-0].sym); } break;
case 199:
# line 486 "hoc.y"
{ addglobal(yypvt[-0].sym); } break;
case 200:
# line 487 "hoc.y"
{ addglobal(yypvt[-0].sym); } break;
case 201:
# line 488 "hoc.y"
{ addglobal(yypvt[-0].sym); } break;
case 202:
# line 489 "hoc.y"
{ addglobal(yypvt[-0].sym); } break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

