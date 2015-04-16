#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "hoc.y"

/* #define YYSTYPE double	/* data type of yacc stack * /*/

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include "hoc.h"

/*#define YYDEBUG 1*/

#define code2(c1,c2)		code(c1); code(c2)
#define code3(c1,c2,c3)		code(c1); code(c2); code(c3)

#define mymsg(str) printf("%s\n", str)

char    	*progname;		/* for error message */
int     	lineno = 1;
jmp_buf		begin;

double		mem[26];	/* 26 variables */

void	warning(char *s, char *t);
void	yyerror(char *s);
int		yylex();
void	execerror(char *s, char *t);
void	fpecatch();

#line 32 "hoc.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {				/* stack type */
	Symbol	*sym;		/* symbal table pointer */
	Inst	*inst;		/* machine instruction */
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 60 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define NUMBER 257
#define PRINT 258
#define VAR 259
#define BLTIN 260
#define UNDEF 261
#define WHILE 262
#define IF 263
#define ELSE 264
#define OR 265
#define AND 266
#define GT 267
#define GE 268
#define LT 269
#define LE 270
#define EQ 271
#define NE 272
#define UNARYMINUS 273
#define NOT 274
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    0,    0,    0,    0,    0,    2,    1,    1,    1,
    1,    1,    1,    5,    6,    7,    8,    4,    4,    4,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
};
static const short yylen[] = {                            2,
    0,    2,    3,    3,    3,    3,    3,    1,    2,    4,
    4,    7,    3,    3,    1,    1,    0,    0,    2,    2,
    1,    1,    1,    4,    3,    3,    3,    3,    3,    3,
    2,    3,    3,    3,    3,    3,    3,    3,    3,    2,
};
static const short yydefred[] = {                         1,
    0,    0,   21,    0,    0,    0,   15,   16,    0,    0,
    2,   18,    0,    0,    0,    0,    0,    0,    6,   23,
    0,    0,    0,    0,    0,    0,    0,    4,    3,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    5,    0,    0,    0,    0,    0,   19,   13,
   20,    0,   25,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   17,   17,   24,
   14,   10,    0,    0,   17,   12,
};
static const short yydgoto[] = {                          1,
   14,   20,   52,   26,   45,   17,   18,   72,
};
static const short yysindex[] = {                         0,
  513,   -9,    0,  136,  -52,  -30,    0,    0,  136,  136,
    0,    0,  136,    4,    5,  193,  -23,  -23,    0,    0,
  148,  136,  136,  -75,  -75,  485,  564,    0,    0,  136,
  136,  136,  136,  136,  136,  136,  136,  136,  136,  136,
  136,  136,    0,  136,  -29,  -29,  148,  721,    0,    0,
    0,  148,    0,  803,  130,  -40,  -40,  -40,  -40,  -40,
  -40,  -34,  -34,  -75,  -75,  -75,  765,    0,    0,    0,
    0,    0, -244,  -29,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,  -10,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  796,    0,    0,    0,    0,    0,
  539,    0,    0,    8,   26,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  531,    0,    0,    0,
    0,  563,    0,  493,  475,  176,  363,  381,  413,  439,
  457,  100,  118,   46,   64,   82,    0,    0,    0,    0,
    0,    0,  655,    0,    0,    0,
};
static const short yygindex[] = {                         0,
  -22,   20,  843,    0,    7,    0,    0,  -63,
};
#define YYTABLESIZE 1075
static const short yytable[] = {                         22,
   19,   40,   38,   51,   39,   73,   41,   40,   22,   23,
   13,   76,   41,   28,   29,    9,   44,   31,   42,   74,
   15,    0,   68,   69,   46,    0,    0,    0,    0,   22,
   22,   22,   22,    0,   22,   40,   22,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   31,   31,   31,
   31,   75,   31,   42,   31,   28,    0,    0,    0,   42,
    0,    0,    0,    0,    0,   40,   40,   40,   40,    0,
   40,    0,   40,   29,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   22,    0,   28,   28,   28,   28,    0,
   28,   30,   28,   12,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   29,   29,   29,   29,    0,   29,   26,
   29,    0,   22,    0,   22,    0,    0,    0,    0,    0,
    0,   30,   30,   30,   30,    0,   30,   27,   30,    0,
   31,    0,   31,    0,    0,    0,    0,    0,    0,   26,
   26,    0,   26,    0,   26,    0,    0,    0,   40,    0,
   40,    0,    0,    0,    0,    0,    0,   27,   27,    0,
   27,    0,   27,    0,    0,    0,    0,    0,   28,    0,
   28,   40,   38,    0,   39,   13,   41,    0,    0,    0,
    9,    0,    0,    0,    0,   32,   29,    0,   29,   40,
   38,    0,   39,    0,   41,    0,    0,    0,    0,    0,
    0,    0,   43,    0,   30,    0,   30,    0,    0,    0,
    0,    0,    0,    0,    0,   32,   32,    0,    0,    0,
    0,    0,   26,   42,   26,    0,    0,    3,    4,    5,
    6,    0,    7,    8,   40,   38,    0,   39,    0,   41,
   27,   42,   27,    0,   10,    0,   22,   22,   22,   22,
    0,   22,   22,   22,   22,   22,   22,   22,   22,   22,
   22,   22,    0,   22,   31,   31,   31,   31,    0,   31,
   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
    0,   31,   40,   40,   40,   40,   42,   40,   40,   40,
   40,   40,   40,   40,   40,   40,   40,   40,   32,   40,
   32,    0,   28,   28,   28,   28,    0,   28,   28,   28,
   28,   28,   28,   28,   28,   28,   28,   28,    0,   28,
   29,   29,   29,   29,    0,   29,   29,   29,   29,   29,
   29,   29,   29,   29,   29,   29,    0,   29,   30,   30,
   30,   30,    0,   30,   30,   30,   30,   30,   30,   30,
   30,   30,   30,   30,    0,   30,   26,   26,   26,   26,
    0,   26,   26,   26,   26,   26,   26,   26,   26,   26,
   26,   26,   33,   26,   27,   27,   27,   27,    0,   27,
   27,   27,   27,   27,   27,   27,   27,   27,   27,   27,
   34,   27,    3,    0,    5,    6,   32,   33,   34,   35,
   36,   37,   33,   33,    0,    0,    0,    0,    0,   10,
    0,    0,   30,   31,   32,   33,   34,   35,   36,   37,
   34,   34,   35,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   32,   32,   32,   32,    0,   32,   32,   32,
   32,   32,   32,   32,   32,   32,   32,   32,   36,   32,
    0,    0,   35,   35,    0,    0,    0,   30,   31,   32,
   33,   34,   35,   36,   37,    0,   37,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   36,   36,
    0,    0,    0,    0,   38,   33,    0,   33,    0,    0,
    0,    0,    0,    0,   49,    0,   37,   37,    0,    0,
    0,    0,   39,   34,    0,   34,    0,    0,    0,    0,
    0,    0,    0,    0,   38,   38,    0,    0,    0,    0,
    0,    0,   11,    0,   13,    0,    0,    0,    0,    9,
    0,    0,   39,   39,    0,   35,    0,   35,    0,    0,
    7,    0,    0,    0,    0,    0,    0,    0,    9,    0,
    0,    0,   13,    0,    0,    0,    0,    9,    0,    0,
    0,   36,    0,   36,    0,    0,    0,    0,    0,    0,
    7,    7,    8,    0,    0,    0,    0,    0,    9,   37,
    0,   37,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   38,    0,   38,
    0,    0,    8,    0,   53,   40,   38,   12,   39,   50,
   41,    0,    0,    0,    0,   39,    0,   39,    0,   33,
   33,   33,   33,    0,   33,   33,   33,   33,   33,   33,
   33,   33,   33,   33,   33,   12,   33,   34,   34,   34,
   34,    0,   34,   34,   34,   34,   34,   34,   34,   34,
   34,   34,   34,    7,   34,    7,    0,   42,    0,    0,
    0,    9,    0,    9,   11,    0,    0,    0,    0,   35,
   35,   35,   35,    0,   35,   35,   35,   35,   35,   35,
   35,   35,   35,   35,   35,    8,   35,    8,    0,    0,
    0,    0,    0,    0,   11,   36,   36,   36,   36,   11,
   36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
   36,    0,   36,   37,   37,   37,   37,    0,   37,   37,
   37,   37,   37,   37,   37,   37,   37,   37,   37,    0,
   37,   38,   38,   38,   38,    0,   38,   38,   38,   38,
   38,    3,    4,    5,    6,    0,    7,    8,   38,   39,
   39,   39,   39,    0,   39,   39,   39,   39,   10,    0,
    0,   70,   40,   38,    0,   39,   39,   41,    2,    3,
    4,    5,    6,    0,    7,    8,    0,   11,    0,   11,
    0,    0,    0,    0,    0,    0,   10,    7,    7,    7,
    7,    0,    7,    7,    7,    9,    9,    9,    9,    0,
    9,    9,    9,    0,    7,   71,   40,   38,    0,   39,
    0,   41,    9,    0,   42,    0,    0,    0,    0,    8,
    8,    8,    8,    0,    8,    8,    8,    0,   30,   31,
   32,   33,   34,   35,   36,   37,    8,   23,   23,    0,
   23,    0,   23,   16,   40,   38,   21,   39,    0,   41,
    0,   24,   25,    0,    0,   27,    0,    0,   42,    0,
    0,    0,    0,    0,   47,   48,    0,    0,    0,    0,
    0,    0,   54,   55,   56,   57,   58,   59,   60,   61,
   62,   63,   64,   65,   66,    0,   67,    0,    0,   23,
    0,    0,    0,    0,    0,    0,   42,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   11,   11,   11,   11,    0,   11,   11,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   11,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   30,   31,   32,   33,   34,
   35,   36,   37,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   30,
   31,   32,   33,   34,   35,   36,   37,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   23,   23,   23,   23,   23,   23,   23,   23,   31,   32,
   33,   34,   35,   36,   37,
};
static const short yycheck[] = {                         10,
   10,   42,   43,   26,   45,   69,   47,   42,   61,   40,
   40,   75,   47,   10,   10,   45,   40,   10,   94,  264,
    1,   -1,   45,   46,   18,   -1,   -1,   -1,   -1,   40,
   41,   42,   43,   -1,   45,   10,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   42,
   43,   74,   45,   94,   47,   10,   -1,   -1,   -1,   94,
   -1,   -1,   -1,   -1,   -1,   40,   41,   42,   43,   -1,
   45,   -1,   47,   10,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   94,   -1,   40,   41,   42,   43,   -1,
   45,   10,   47,  123,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   41,   42,   43,   -1,   45,   10,
   47,   -1,  123,   -1,  125,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   41,   42,   43,   -1,   45,   10,   47,   -1,
  123,   -1,  125,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   41,   -1,   43,   -1,   45,   -1,   -1,   -1,  123,   -1,
  125,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,  123,   -1,
  125,   42,   43,   -1,   45,   40,   47,   -1,   -1,   -1,
   45,   -1,   -1,   -1,   -1,   10,  123,   -1,  125,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   10,   -1,  123,   -1,  125,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,   -1,
   -1,   -1,  123,   94,  125,   -1,   -1,  257,  258,  259,
  260,   -1,  262,  263,   42,   43,   -1,   45,   -1,   47,
  123,   94,  125,   -1,  274,   -1,  257,  258,  259,  260,
   -1,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,   -1,  274,  257,  258,  259,  260,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   -1,  274,  257,  258,  259,  260,   94,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,  123,  274,
  125,   -1,  257,  258,  259,  260,   -1,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,   -1,  274,
  257,  258,  259,  260,   -1,  262,  263,  264,  265,  266,
  267,  268,  269,  270,  271,  272,   -1,  274,  257,  258,
  259,  260,   -1,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,   -1,  274,  257,  258,  259,  260,
   -1,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,   10,  274,  257,  258,  259,  260,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   10,  274,  257,   -1,  259,  260,  267,  268,  269,  270,
  271,  272,   40,   41,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,  265,  266,  267,  268,  269,  270,  271,  272,
   40,   41,   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  257,  258,  259,  260,   -1,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,   10,  274,
   -1,   -1,   40,   41,   -1,   -1,   -1,  265,  266,  267,
  268,  269,  270,  271,  272,   -1,   10,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,
   -1,   -1,   -1,   -1,   10,  123,   -1,  125,   -1,   -1,
   -1,   -1,   -1,   -1,   10,   -1,   40,   41,   -1,   -1,
   -1,   -1,   10,  123,   -1,  125,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,   -1,   -1,
   -1,   -1,   10,   -1,   40,   -1,   -1,   -1,   -1,   45,
   -1,   -1,   40,   41,   -1,  123,   -1,  125,   -1,   -1,
   10,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   10,   -1,
   -1,   -1,   40,   -1,   -1,   -1,   -1,   45,   -1,   -1,
   -1,  123,   -1,  125,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   41,   10,   -1,   -1,   -1,   -1,   -1,   40,  123,
   -1,  125,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  123,   -1,  125,
   -1,   -1,   40,   -1,   41,   42,   43,  123,   45,  125,
   47,   -1,   -1,   -1,   -1,  123,   -1,  125,   -1,  257,
  258,  259,  260,   -1,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  123,  274,  257,  258,  259,
  260,   -1,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  123,  274,  125,   -1,   94,   -1,   -1,
   -1,  123,   -1,  125,   10,   -1,   -1,   -1,   -1,  257,
  258,  259,  260,   -1,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  123,  274,  125,   -1,   -1,
   -1,   -1,   -1,   -1,   40,  257,  258,  259,  260,   45,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  272,   -1,  274,  257,  258,  259,  260,   -1,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,   -1,
  274,  257,  258,  259,  260,   -1,  262,  263,  264,  265,
  266,  257,  258,  259,  260,   -1,  262,  263,  274,  257,
  258,  259,  260,   -1,  262,  263,  264,  265,  274,   -1,
   -1,   41,   42,   43,   -1,   45,  274,   47,  256,  257,
  258,  259,  260,   -1,  262,  263,   -1,  123,   -1,  125,
   -1,   -1,   -1,   -1,   -1,   -1,  274,  257,  258,  259,
  260,   -1,  262,  263,  264,  257,  258,  259,  260,   -1,
  262,  263,  264,   -1,  274,   41,   42,   43,   -1,   45,
   -1,   47,  274,   -1,   94,   -1,   -1,   -1,   -1,  257,
  258,  259,  260,   -1,  262,  263,  264,   -1,  265,  266,
  267,  268,  269,  270,  271,  272,  274,   42,   43,   -1,
   45,   -1,   47,    1,   42,   43,    4,   45,   -1,   47,
   -1,    9,   10,   -1,   -1,   13,   -1,   -1,   94,   -1,
   -1,   -1,   -1,   -1,   22,   23,   -1,   -1,   -1,   -1,
   -1,   -1,   30,   31,   32,   33,   34,   35,   36,   37,
   38,   39,   40,   41,   42,   -1,   44,   -1,   -1,   94,
   -1,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  257,  258,  259,  260,   -1,  262,  263,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  265,  266,  267,  268,  269,
  270,  271,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  265,
  266,  267,  268,  269,  270,  271,  272,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  265,  266,  267,  268,  269,  270,  271,  272,  266,  267,
  268,  269,  270,  271,  272,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 274
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,"'('","')'","'*'","'+'",0,"'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,
0,0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'^'",
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"NUMBER","PRINT","VAR","BLTIN","UNDEF","WHILE","IF","ELSE","OR",
"AND","GT","GE","LT","LE","EQ","NE","UNARYMINUS","NOT","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : list",
"list :",
"list : list '\\n'",
"list : list asgn '\\n'",
"list : list stmt '\\n'",
"list : list expr '\\n'",
"list : list error '\\n'",
"asgn : VAR '=' expr",
"stmt : expr",
"stmt : PRINT expr",
"stmt : while cond stmt end",
"stmt : if cond stmt end",
"stmt : if cond stmt end ELSE stmt end",
"stmt : '{' stmtlist '}'",
"cond : '(' expr ')'",
"while : WHILE",
"if : IF",
"end :",
"stmtlist :",
"stmtlist : stmtlist '\\n'",
"stmtlist : stmtlist stmt",
"expr : NUMBER",
"expr : VAR",
"expr : asgn",
"expr : BLTIN '(' expr ')'",
"expr : '(' expr ')'",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '*' expr",
"expr : expr '/' expr",
"expr : expr '^' expr",
"expr : '-' expr",
"expr : expr GT expr",
"expr : expr GE expr",
"expr : expr LT expr",
"expr : expr LE expr",
"expr : expr EQ expr",
"expr : expr NE expr",
"expr : expr AND expr",
"expr : expr OR expr",
"expr : NOT expr",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 122 "hoc.y"

/* print warning message */
void warning(char *s, char *t)
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}

/* called for yacc syntax error */
void yyerror(char* s)
{
	warning(s, (char*)0);
}

int follow(char expect, int ifyes, int ifno) /* look ahead for >=, etc */
{
	int c = getchar();

	if (c == expect)
		return ifyes;
	ungetc(c, stdin);
	return ifno;
}

/* lexical parser */
int yylex()
{
	int c;

	while ((c=getchar()) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {	/* number */
		double d;
		ungetc(c, stdin);
		scanf("%lf", &d);
		yylval.sym = install("", NUMBER, d);
		return NUMBER;
	}
	if (isalpha(c)) {
		Symbol *s;
		char sbuf[100], *p = sbuf;
		
		do {
			*p++ = c;
		} while ((c=getchar()) != EOF && isalnum(c));

		ungetc(c, stdin);
		*p = '\0';
		if ((s=lookup(sbuf)) == 0) {
			s = install(sbuf, UNDEF, 0.0);
			// printf("Installed UNDEF symbol %s\n", sbuf);
		}
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	switch (c)
	{
		case '>' :	return follow('=', GE, GT);
		case '<' :	return follow('=', LE, LT);
		case '=' :	return follow('=', EQ, '=');
		case '!' :	return follow('=', NE, NOT);
		case '|' :	return follow('|', OR, '|');
		case '&' :	return follow('&', AND, '&');
		case '\n':	lineno++; return '\n';
		default:	return c;
	}
}

/* recover from run-time error */
void execerror(char *s, char *t)
{
	warning(s, t);
	longjmp(begin, 0);
}

/* catch floating point exception */
void fpecatch()
{
	execerror("floating point exception", (char*)0);
}


int main(int argc, char* argv[])
{
	yydebug = 1;
	progname = argv[0];
	init();
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	for (initcode(); yyparse(); initcode())
		execute(prog);
	return 0;
}


#line 578 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 3:
#line 52 "hoc.y"
	{ code2(xpop, STOP); mymsg("list: asgn");return 1; }
break;
case 4:
#line 53 "hoc.y"
	{ code(STOP); mymsg("list: stmt");return 1; }
break;
case 5:
#line 54 "hoc.y"
	{ code2(print, STOP); mymsg("list:expr");return 1; }
break;
case 6:
#line 55 "hoc.y"
	{ yyerrok; mymsg("list:error");}
break;
case 7:
#line 58 "hoc.y"
	{ yyval.inst = yystack.l_mark[0].inst; code3(varpush, (Inst)yystack.l_mark[-2].sym, assign); mymsg("var=expr");}
break;
case 8:
#line 61 "hoc.y"
	{ code(xpop); mymsg("stmt:expr");}
break;
case 9:
#line 62 "hoc.y"
	{ code(prexpr); yyval.inst=yystack.l_mark[0].inst; mymsg("stmt:prn");}
break;
case 10:
#line 63 "hoc.y"
	{
			(yystack.l_mark[-3].inst)[1] = (Inst)yystack.l_mark[-1].inst;		/* body of loop */
			(yystack.l_mark[-3].inst)[2] = (Inst)yystack.l_mark[0].inst;		/* end, if cond fails */
			mymsg("while-end");
		  }
break;
case 11:
#line 68 "hoc.y"
	{
			(yystack.l_mark[-3].inst)[1] = (Inst)yystack.l_mark[-1].inst;		/* then part */
			(yystack.l_mark[-3].inst)[3] = (Inst)yystack.l_mark[0].inst;		/* end, if cond fails */
			mymsg("if-end");
		  }
break;
case 12:
#line 73 "hoc.y"
	{
			(yystack.l_mark[-6].inst)[1] = (Inst)yystack.l_mark[-4].inst;		/* then part */
			(yystack.l_mark[-6].inst)[2] = (Inst)yystack.l_mark[-1].inst;		/* else part */
			(yystack.l_mark[-6].inst)[3] = (Inst)yystack.l_mark[0].inst;		/* end, if cond fails */
			mymsg("if-else-end");
		  }
break;
case 13:
#line 79 "hoc.y"
	{ yyval.inst = yystack.l_mark[-1].inst; mymsg("stmtlist");}
break;
case 14:
#line 82 "hoc.y"
	{ code(STOP); yyval.inst = yystack.l_mark[-1].inst; mymsg("cond");}
break;
case 15:
#line 85 "hoc.y"
	{ yyval.inst = code3(whilecode, STOP, STOP); mymsg("while");}
break;
case 16:
#line 88 "hoc.y"
	{ yyval.inst = code(ifcode); code3(STOP, STOP, STOP); mymsg("if");}
break;
case 17:
#line 91 "hoc.y"
	{ code(STOP); yyval.inst = progp; mymsg("end");}
break;
case 18:
#line 94 "hoc.y"
	{ yyval.inst = progp; mymsg("list:nothing");}
break;
case 21:
#line 99 "hoc.y"
	{ yyval.inst = code2(constpush, (Inst)yystack.l_mark[0].sym); mymsg("num");}
break;
case 22:
#line 100 "hoc.y"
	{ yyval.inst = code3(varpush, (Inst)yystack.l_mark[0].sym, eval); mymsg("var");}
break;
case 24:
#line 102 "hoc.y"
	{ yyval.inst = yystack.l_mark[-1].inst; code2(bltin, (Inst)yystack.l_mark[-3].sym->u.ptr); mymsg("bltin");}
break;
case 25:
#line 103 "hoc.y"
	{ yyval.inst = yystack.l_mark[-1].inst; mymsg("(expr)");}
break;
case 26:
#line 104 "hoc.y"
	{ code(add); mymsg("add");}
break;
case 27:
#line 105 "hoc.y"
	{ code(sub); mymsg("sub");}
break;
case 28:
#line 106 "hoc.y"
	{ code(mul); mymsg("mul");}
break;
case 29:
#line 107 "hoc.y"
	{ code(divop); mymsg("divop");}
break;
case 30:
#line 108 "hoc.y"
	{ code(power); mymsg("power");}
break;
case 31:
#line 109 "hoc.y"
	{ yyval.inst = yystack.l_mark[0].inst; code(negate); mymsg("uminus");}
break;
case 32:
#line 110 "hoc.y"
	{ code(gt); }
break;
case 33:
#line 111 "hoc.y"
	{ code(ge); }
break;
case 34:
#line 112 "hoc.y"
	{ code(lt); }
break;
case 35:
#line 113 "hoc.y"
	{ code(le); }
break;
case 36:
#line 114 "hoc.y"
	{ code(eq); }
break;
case 37:
#line 115 "hoc.y"
	{ code(ne); }
break;
case 38:
#line 116 "hoc.y"
	{ code(and); }
break;
case 39:
#line 117 "hoc.y"
	{ code(or); }
break;
case 40:
#line 118 "hoc.y"
	{ yyval.inst = yystack.l_mark[0].inst; code(not); }
break;
#line 933 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
