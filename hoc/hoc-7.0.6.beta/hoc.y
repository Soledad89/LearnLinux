%{
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

%}

%union {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	long	narg;	/* number of arguments */
}

%token	<sym>	BLTIN0 BLTIN1 BLTIN1I BLTIN1S BLTIN2 BLTIN2S BREAK
%token	<sym>	CONTINUE ELSE FOR FUNC FUNCTION GLOBAL IF NUMBER PRINT
%token	<sym>	PRINTF PRINTLN PROC PROCEDURE READ RETURN SPRINTF
%token	<sym>	STRBLTIN0 STRBLTIN1 STRBLTIN1N STRBLTIN2 STRBLTIN2SN
%token	<sym>	STRBLTIN3SNN STRING STRVAR UNDEF VAR WHILE

%token	<narg>	ARG STRARG

%type	<inst>	anyexpr expr stmt asgn prlist prflist sprflist stmtlist
%type	<inst>	cond while for if begin end
%type	<inst>	strasgn strexpr string

%type	<sym>	keyword otherword procname

%type	<narg>	arglist

%right	'=' ADDEQ SUBEQ MULEQ DIVEQ MODEQ CONSTEQ

%left	OR
%left	AND
%left	GT GE LT LE EQ NE LTGT
%left	'+' '-'
%left	'*' '/' '%'
%left	UNARYMINUS UNARYPLUS NOT INC DEC

%right	TO_POWER
%%
list:	  /* nothing */
	| list eos
	| list defn eos
	| list asgn eos		{ CODE2(xpop, STOP); return 1; }
	| list strasgn eos	{ CODE2(xpop, STOP); return 1; }
	| list stmt eos		{ CODE(STOP); return 1; }
	| list expr eos		{ CODE2(printtop, STOP); return 1; }
	| list strexpr eos	{ CODE2(printtopstring, STOP); return 1; }
	| list error eos 	{ yyerrok; }
	;

eos:	  '\n'
	| ';'
	;

asgn:	  VAR '=' expr		{ CODE3(varpush,(Inst)$1,assign); $$=$3; }
	| VAR CONSTEQ expr	{ CODE3(varpush,(Inst)$1,const_assign); $$=$3; }
	| VAR ADDEQ expr	{ CODE3(varpush,(Inst)$1,addeq); $$=$3; }
	| VAR SUBEQ expr	{ CODE3(varpush,(Inst)$1,subeq); $$=$3; }
	| VAR MULEQ expr	{ CODE3(varpush,(Inst)$1,muleq); $$=$3; }
	| VAR DIVEQ expr	{ CODE3(varpush,(Inst)$1,diveq); $$=$3; }
	| VAR MODEQ expr	{ CODE3(varpush,(Inst)$1,modeq); $$=$3; }
	| STRVAR '=' expr	{ CODE3(varpush,(Inst)$1,assign); $$=$3; }
	| STRVAR CONSTEQ expr	{ CODE3(varpush,(Inst)$1,const_assign); $$=$3; }
	| STRVAR ADDEQ expr	{ CODE3(varpush,(Inst)$1,addeq); $$=$3; }
	| STRVAR SUBEQ expr	{ CODE3(varpush,(Inst)$1,subeq); $$=$3; }
	| STRVAR MULEQ expr	{ CODE3(varpush,(Inst)$1,muleq); $$=$3; }
	| STRVAR DIVEQ expr	{ CODE3(varpush,(Inst)$1,diveq); $$=$3; }
	| STRVAR MODEQ expr	{ CODE3(varpush,(Inst)$1,modeq); $$=$3; }
	| ARG '=' expr   	{ defnonly("$"); CODE2(argassign,(Inst)$1); $$=$3;}
	| ARG ADDEQ expr 	{ defnonly("$"); CODE2(argaddeq,(Inst)$1); $$=$3;}
	| ARG SUBEQ expr 	{ defnonly("$"); CODE2(argsubeq,(Inst)$1); $$=$3;}
	| ARG MULEQ expr 	{ defnonly("$"); CODE2(argmuleq,(Inst)$1); $$=$3;}
	| ARG DIVEQ expr 	{ defnonly("$"); CODE2(argdiveq,(Inst)$1); $$=$3;}
	| ARG MODEQ expr 	{ defnonly("$"); CODE2(argmodeq,(Inst)$1); $$=$3;}
	;

strasgn:  STRVAR '=' strexpr		{ CODE3(varpush,(Inst)$1,str_assign); $$=$3; }
	| STRVAR CONSTEQ strexpr	{ CODE3(varpush,(Inst)$1,const_str_assign); $$=$3; }
	| STRARG '=' strexpr	   	{ defnonly("$"); CODE2(argassign,(Inst)$1); $$=$3;}
	| ARG '=' strexpr	   	{ defnonly("$"); CODE2(argassign,(Inst)$1); $$=$3;}
	| VAR '=' strexpr		{ CODE3(varpush,(Inst)$1,str_assign); $$=$3; }
	| VAR CONSTEQ strexpr		{ CODE3(varpush,(Inst)$1,const_str_assign); $$=$3; }
	;

stmt:	  expr				{ CODE(xpop); }
	| strexpr			{ CODE(xpop); }
	| BREAK				{ looponly("break"); $$ = code(breakstmt); }
	| CONTINUE			{ looponly("continue"); $$ = code(continuestmt); }
	| RETURN			{ defnonly("return"); $$ = code(procret); }
	| RETURN expr			{ defnonly("return"); $$=$2; CODE(funcret); }
	| RETURN strexpr		{ defnonly("return"); $$=$2; CODE(funcret); }
	| PROCEDURE begin '(' arglist ')'	{ $$ = $2; CODE3(call, (Inst)$1, (Inst)$4); }
	| PRINT prlist			{ $$ = $2; }
	| PRINTF prflist		{ $$ = $2; }
	| PRINTLN prlist		{ $$ = $2; CODE(prnl); }
	| GLOBAL			{ in_global_stmt = 1; }
		globalnames		{ in_global_stmt = 0; }
	| while '(' cond ')' { inloop++; } stmt { inloop--; } end {
		($1)[1] = (Inst)$6;	/* body of loop */
		($1)[2] = (Inst)$8; }	/* end, if cond fails */
	| for '(' cond ';' cond ';' cond ')' { inloop++; } stmt { inloop--; } end {
		($1)[1] = (Inst)$5;	/* condition */
		($1)[2] = (Inst)$7;	/* post loop */
		($1)[3] = (Inst)$10;	/* body of loop */
		($1)[4] = (Inst)$12; }	/* end, if cond fails */
	| if '(' cond ')' stmt end {	/* else-less if */
		($1)[1] = (Inst)$5;	/* thenpart */
		($1)[3] = (Inst)$6; }	/* end, if cond fails */
	| if '(' cond ')' stmt end ELSE stmt end {	/* if with else */
		($1)[1] = (Inst)$5;	/* thenpart */
		($1)[2] = (Inst)$8;	/* elsepart */
		($1)[3] = (Inst)$9; }	/* end, if cond fails */
	| '{' stmtlist '}'		{ $$ = $2; }
	;
cond:	   expr 			{ CODE(STOP); }
	;
while:	  WHILE				{ $$ = code3(whilecode,STOP,STOP); }
	;
for:	  FOR				{ $$ = code(forcode); CODE4(STOP,STOP,STOP,STOP); }
	;
if:	  IF				{ $$ = code(ifcode); CODE3(STOP,STOP,STOP); }
	;
begin:	  /* nothing */			{ $$ = progp; }
	;
end:	  /* nothing */			{ CODE(STOP); $$ = progp; }
	;

/* This list of hoc language keywords must be kept synchronized with
the keywords[] table in init.c! */
keyword:  PROC
	| FUNC
	| RETURN
	| IF
	| ELSE
	| WHILE
	| FOR
	| PRINT
	| READ
	| BREAK
	| CONTINUE
	| PRINTF
	| PRINTLN
	| SPRINTF
	;

otherword: BLTIN0
	| BLTIN1
	| BLTIN1I
	| BLTIN1S
	| BLTIN2
	| BLTIN2S
	| FUNCTION
	| PROCEDURE
	| STRBLTIN0
	| STRBLTIN1
	| STRBLTIN1N
	| STRBLTIN2
	| STRBLTIN2SN
	| STRBLTIN3SNN
	| STRVAR
	| UNDEF
	| VAR
	;

stmtlist: /* nothing */				{ $$ = progp; }
	| stmtlist eos
	| stmtlist stmt
	;

anyexpr:	expr
	| strexpr
	;

string:   STRING				{ $$ = code2(const_str_push, (Inst)$1); }
	| STRVAR				{ $$ = code3(varpush, (Inst)$1, streval); }
	| STRARG				{ defnonly("$$"); $$ = code2(strarg, (Inst)$1); }
	| strasgn
	| '(' strexpr ')'			{ $$ = $2; }
	| READ '(' STRVAR ')'			{ $$ = code2(varread, (Inst)$3); }
	| STRBLTIN0 '(' ')'			{ $$ = code2(strbltin0, (Inst)$1->u.sptr0); }
	| STRBLTIN1 '(' anyexpr ')'		{ $$ = $3; CODE2(strbltin1, (Inst)$1->u.sptr1); }
	| STRBLTIN1N '(' anyexpr ')'		{ $$ = $3; CODE2(strbltin1n, (Inst)$1->u.sptr1n); }
	| STRBLTIN2 '(' anyexpr ',' anyexpr ')'	{ $$ = $3; CODE2(strbltin2, (Inst)$1->u.sptr2); }
	| STRBLTIN2SN '(' anyexpr ',' anyexpr ')' { $$ = $3; CODE2(strbltin2sn, (Inst)$1->u.sptr2sn); }
	| STRBLTIN3SNN '(' anyexpr ',' anyexpr ',' anyexpr ')' { $$ = $3; CODE2(strbltin3snn, (Inst)$1->u.sptr3snn); }
	;

strexpr:  string
	| expr NUMBER				{ CODE3(constpush, (Inst)$2, str_concat); }
	| expr VAR				{ CODE3(constpush, (Inst)$2, str_concat); }
	| expr ARG				{ CODE3(arg, (Inst)$2, str_concat); }
	| expr string				{ CODE(str_concat); }
	| strexpr NUMBER			{ CODE3(constpush, (Inst)$2, str_concat); }
	| strexpr VAR				{ CODE3(constpush, (Inst)$2, str_concat); }
	| strexpr ARG				{ CODE3(arg, (Inst)$2, str_concat); }
	| strexpr string			{ CODE(str_concat); }
	| SPRINTF sprflist			{ $$ = $2; }
	;

expr:	  NUMBER				{ $$ = code2(constpush, (Inst)$1); }
	| VAR					{ $$ = code3(varpush, (Inst)$1, eval); }
	| ARG					{ defnonly("$"); $$ = code2(arg, (Inst)$1); }
	| asgn
	| FUNCTION begin '(' arglist ')'	{ $$ = $2; CODE3(call,(Inst)$1,(Inst)$4); }
	| READ '(' VAR ')'			{ $$ = code2(varread, (Inst)$3); }
	| BLTIN0 '(' ')'              		{ $$ = code2(bltin0, (Inst)$1->u.ptr0); }
	| BLTIN1 '(' anyexpr ')'         	{ $$ = $3; CODE2(bltin1, (Inst)$1->u.ptr1); }
	| BLTIN1I '(' keyword ')'		{ $$ = code3(bltin1i, (Inst)$1->u.ptr1i, (Inst)$3); }
	| BLTIN1I '(' otherword ')'		{ $$ = code3(bltin1i, (Inst)$1->u.ptr1i, (Inst)$3); }
	| BLTIN1S '(' anyexpr ')'         	{ $$ = $3; CODE2(bltin1s, (Inst)$1->u.ptr1s); }
	| BLTIN2 '(' anyexpr ',' anyexpr ')'	{ $$ = $3; CODE2(bltin2, (Inst)$1->u.ptr2); }
	| BLTIN2S '(' anyexpr ',' anyexpr ')'	{ $$ = $3; CODE2(bltin2s, (Inst)$1->u.ptr2s); }
	| '(' expr ')'				{ $$ = $2; }
	| expr '+' expr				{ CODE(add); }
	| expr '-' expr				{ CODE(sub); }
	| expr '*' expr				{ CODE(mul); }
	| expr '/' expr				{ CODE(divop); }	/* ansi has a div fcn! */
	| expr '%' expr				{ CODE(mod); }
	| expr TO_POWER expr			{ CODE(power); }
	| '-' expr   %prec UNARYMINUS  		{ $$=$2; CODE(negate); }
	| '+' expr   %prec UNARYMINUS  		{ $$=$2; CODE(noop); }
	| expr GT expr				{ CODE(gt); }
	| expr GE expr				{ CODE(ge); }
	| expr LT expr				{ CODE(lt); }
	| expr LTGT expr			{ CODE(ltgt); }
	| expr LE expr				{ CODE(le); }
	| expr EQ expr				{ CODE(eq); }
	| expr NE expr				{ CODE(ne); }
	| expr AND expr				{ CODE(And); }
	| expr OR expr				{ CODE(Or); }
	| NOT expr				{ $$ = $2; CODE(Not); }
	| INC VAR				{ $$ = code2(preinc,(Inst)$2); }
	| DEC VAR				{ $$ = code2(predec,(Inst)$2); }
	| VAR INC				{ $$ = code2(postinc,(Inst)$1); }
	| VAR DEC				{ $$ = code2(postdec,(Inst)$1); }
	| string GT string			{ CODE(str_gt); }
	| string GE string			{ CODE(str_ge); }
	| string LT string			{ CODE(str_lt); }
	| string LE string			{ CODE(str_le); }
	| string EQ string			{ CODE(str_eq); }
	| string NE string			{ CODE(str_ne); }
	;

arglist:  /* nothing */ 			{ $$ = 0; }
	| expr					{ $$ = 1; }
	| strexpr				{ $$ = 1; }
	| arglist ',' expr			{ $$ = $1 + 1; }
	| arglist ',' strexpr			{ $$ = $1 + 1; }
	;

prlist:	  expr					{ CODE(prexpr); }
	| strexpr				{ CODE(prstr); }
	| prlist ',' expr			{ CODE2(prsep,prexpr); }
	| prlist ',' strexpr			{ CODE2(prsep,prstr); }
	;

prflist: strexpr				{ CODE(prfmt); }
	| prflist ',' expr			{ CODE(prexpr); }
	| prflist ',' strexpr			{ CODE(prstr); }
	;

sprflist: strexpr				{ CODE(sprfmt); }
	| sprflist ',' expr			{ CODE2(sprexpr,sprjoin); }
	| sprflist ',' strexpr			{ CODE2(sprstr,sprjoin); }
	;

defn:	  FUNC procname				{ $2->type = FUNCTION; indef = 1; }
	    beginargs '(' argnames ')' endargs
	    stmt				{ CODE(procret); define($2); }
	    enddefn
	| PROC procname				{ $2->type = PROCEDURE; indef = 1; }
	    beginargs '(' argnames ')' endargs
	    stmt				{ CODE(procret); define($2); }
	    enddefn
	;

procname: VAR
	| FUNCTION
	| PROCEDURE
	;

beginargs: /* nothing */			{
							this_procname = yylval.sym->name;
							free_argnamelist();
							free_globallist();
							ndefns++;
							in_argnamelist = 1;
						}

endargs: /* nothing */				{ in_argnamelist = 0; }

enddefn: /* nothing */				{ indef = 0; free_argnamelist(); free_globallist(); }

argnames:	/* nothing */
	| VAR					{ addarg($1); }
	| STRVAR				{ addarg($1); }
	| argnames ',' VAR			{ addarg($3); }
	| argnames ',' STRVAR			{ addarg($3); }
	;

globalnames:	/* nothing */
	| VAR					{ addglobal($1); }
	| STRVAR				{ addglobal($1); }
	| globalnames ',' VAR			{ addglobal($3); }
	| globalnames ',' STRVAR		{ addglobal($3); }
	;
%%
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
