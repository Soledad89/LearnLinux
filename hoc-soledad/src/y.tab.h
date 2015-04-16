#define NUMBER 257
#define STRING 258
#define PRINT 259
#define VAR 260
#define BLTIN 261
#define UNDEF 262
#define WHILE 263
#define FOR 264
#define IF 265
#define ELSE 266
#define FUNCTION 267
#define PROCEDURE 268
#define RETURN 269
#define FUNC 270
#define PROC 271
#define READ 272
#define ARG 273
#define ADDEQ 274
#define SUBEQ 275
#define MULEQ 276
#define DIVEQ 277
#define MODEQ 278
#define OR 279
#define AND 280
#define GT 281
#define GE 282
#define LT 283
#define LE 284
#define EQ 285
#define NE 286
#define UNARYMINUS 287
#define NOT 288
#define INC 289
#define DEC 290
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	int	narg;	/* number of arguments */
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
