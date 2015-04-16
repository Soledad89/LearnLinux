
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	Symbol	*sym;	/* symbol table pointer */
	Inst	*inst;	/* machine instruction */
	long	narg;	/* number of arguments */
} YYSTYPE;
extern YYSTYPE yylval;
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
