#include "config.h"

/* NB: This simple interface to GNU readline assumes that fscanf(),
   getc(), and ungetc() only access a single file!  Apart from the
   macros and functions defined inside this conditional, NO other
   changes to the hoc code below are required.  There is a similar,
   but shorter conditional in code.c. */

#if defined(HAVE_CTYPE_H)
#include <ctype.h>
#endif

#if defined(HAVE_STDIO_H)
#include <stdio.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#if defined(HAVE_STDLIB_H)
#include <stdlib.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>			/* for isatty() on some systems */
#endif

#include "readline.h"

#define elementsof(x)	(sizeof(x)/sizeof(x[0]))
#define lastindexof(x)	(elementsof(x) - 1)

#if !defined(HAVE_SNPRINTF_PROTOTYPE)
EXTERN int		snprintf ARGS((char *, size_t, const char *, ...));
#endif

#if defined(HAVE_SNPRINTF)
#define SNPRINTF4(s,n,fmt,a)		(snprintf)((s),(n),(fmt),(a))
#else /* NOT defined(HAVE_SNPRINTF) */
#define SNPRINTF4(s,n,fmt,a)		(sprintf )((s),    (fmt),(a))
#endif /* defined(HAVE_SNPRINTF) */

int EOF_flag = 0;			/* set nonzero by hoc endinput() function,
					   and zeroed by set_filename() -> init_lineno() */

int irl_fd_stdin;			/* dup()'ed file descriptor of original stdin */
static int irl_ndigits;			/* number of digits seen in irl_token[] */

static char *irl_token = (char *)NULL;	/* numeric token buffer for irl_fscanf() */
static int irl_ntoken = 0;		/* number of chars currently in irl_token[] */
static size_t irl_max_token = 0;	/* current size of irl_token[] */

char *irl_pushback = (char *)NULL;	/* dynamic pushback buffer for irl_unget_char() */
size_t irl_max_pushback = 0;		/* current size of irl_pushback[] */
long irl_pushpos = -1;			/* current index into irl_pushback[] (MUST be signed type) */

static const char *irl_current_line = (char *) NULL;
static const char *irl_nextc = (char *)NULL;

#if defined(HAVE_GNU_READLINE)
extern int readline_echoing_p;		/* defined in GNU readline library code */
#endif

typedef struct irl_stack_entry
{
	struct irl_stack_entry *next;
	const char *line;
	long nextc;
	char *pushback;
	size_t n_pushback;
	long pushpos;
} irl_stack_entry;

extern FILE *	fplog;
extern int	logfile_enabled;	/* defined in hoc.y */
extern int	use_readline;		/* defined in hoc.y */

extern fp_t	xstrtod ARGS((const char *, char **));
extern int	decval ARGS((int c));
extern int	hexval ARGS((int c));
extern void	efree ARGS((void *));
extern void *	egrow ARGS((void *, size_t *, size_t));
extern void *	emalloc ARGS((size_t));	/* hoc's safe memory allocator */
extern void *	erealloc ARGS((void *, size_t, size_t));
typedef struct Symbol { int sym; } Symbol;	/* need just for next function prototype */
extern Symbol *	update_const_number ARGS((const char *, fp_t));
extern fp_t	Infinity ARGS((void));
extern fp_t	NaN ARGS((void));
extern fp_t	QNaN ARGS((void));
extern fp_t	SNaN ARGS((void));

static irl_stack_entry *irl_stack = (irl_stack_entry *)NULL;

static const char *irl_readline ARGS((FILE *, const char *));
static const char *irl_gets  ARGS((FILE *));
static const char *new_prompt ARGS((void));

static int  irl_is_0x ARGS((FILE *));
static int  irl_is_inf ARGS((FILE *));
static int  irl_is_nan ARGS((FILE *));
static int  irl_is_qnan ARGS((FILE *));
static int  irl_is_snan ARGS((FILE *));
static int  irl_match_input ARGS((FILE *, const char *));
static int  irl_tolower ARGS((int));
static int  irl_toupper ARGS((int));

static void irl_add_to_token ARGS((int));
static void irl_scan_digits ARGS((FILE *));
static void irl_scan_dot ARGS((FILE *));
static void irl_scan_exponent ARGS((FILE *));
static void irl_scan_hex_digits ARGS((FILE *));
static void irl_scan_hex_exponent ARGS((FILE *));
static void irl_scan_sign ARGS((FILE *));
static void irl_scan_suffix ARGS((FILE *));
static void irl_scan_whitespace ARGS((FILE *));


#if defined(HAVE_GNU_READLINE)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"
#endif
char *	irl_word_generator ARGS((const char *, int));

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"
#endif
char **	irl_completion ARGS((const char *, int, int));

char *
irl_word_generator(const char *text, int state)
{
	static int len;
	static const char *name;
	const char *result;

	if (!state)
	{
		len = strlen(text);		/* optimization: compute only once */
		name = first_symbol_pname();
	}
	while (name != (const char *)NULL)
	{
		if (strncmp(name,text,len) == 0)
		{
			result = name;
			name = next_symbol_pname();
			return ((char *)result);
		}
		else
		{
			efree((void *)name);
			name = next_symbol_pname();
		}
	}
	return ((char *)NULL);
}


char **
irl_completion(const char *text, int start, int end)
{
	return (rl_completion_matches (text, irl_word_generator));
}
#endif


static void
irl_add_to_token(int the_char)
{
	if ((irl_ntoken + 2) > irl_max_token) /* need bigger token buffer */
	{
		irl_token = (char *)egrow((void*)irl_token, &irl_max_token, sizeof(irl_token[0]));
		(void)update_const_number("__MAX_TOKEN__", (fp_t)irl_max_token);
	}
	irl_token[irl_ntoken++] = (char)the_char;
	irl_token[irl_ntoken] = (char)'\0'; /* keep irl_token properly terminated */
}

void
irl_free(void)
{			   /* Free any previous line buffer memory. */
	if (irl_current_line != (const char *)NULL)
	{
		efree((void *)irl_current_line);
		irl_current_line = (const char *) NULL;
	}
}

int
irl_fscanf(FILE *fpin,const char *format,void *pdata)
{					/* NB: also needed in code.c */
	/* NB: This function is called ONLY with format == "%lf", so we can
	   simplify the processing, using strtod() to do the conversion, and
	   also allowing us to advance the irl_nextc pointer correctly. */
	char *endptr;
	int result;
	int is_hexfp;

	/* Collect a numeric string matching one of the regular expressions
		[-+]*(Inf|Infinity|NaN|SNaN|QNaN)			[ignoring lettercase]
		[-+]*[0-9]*[.]?[0-9]*([Ee][-+]*[0-9])?			[C89: printf %e, %f, %g]
		[-+]*0[Xx][0-9A-Fa-f]*[.]?[0-9A-Fa-f]*([Pp][-+]*[0-9])?	[C99: printf %A, %a]
	   with the constraint that at least one digit must be seen
	   before we reach the optional exponent field.
	*/

	endptr = (char *)NULL;
	irl_ndigits = 0;
	irl_ntoken = 0;
	irl_scan_whitespace(fpin);		/* regexp: [ \t]* */
	irl_scan_sign(fpin);			/* regexp: [-+]* */
	is_hexfp = irl_is_0x(fpin);		/* regexp: 0[xX] */
	if (is_hexfp)
		irl_scan_hex_digits(fpin);	/* regexp: [0-9A-Fa-f]* */
	else
		irl_scan_digits(fpin);		/* regexp: [0-9]* */
	irl_scan_dot(fpin);			/* regexp: [.]? */
	if (is_hexfp)
		irl_scan_hex_digits(fpin);	/* regexp: [0-9A-Fa-f]* */
	else
		irl_scan_digits(fpin);		/* regexp: [0-9]* */
	result = 1;
	if (irl_ndigits > 0)			/* looks like we got a number */
	{
		/* C99's strtod() can handle both of these cases, but we can only rely on
		   C89's strtod(), with our own xstrtod() to handle the hexadecimal case */
		if (is_hexfp)
		{
			irl_scan_hex_exponent(fpin); /* regexp: ([Pp][-+]*[0-9]+)? */
			irl_scan_suffix(fpin); /* regexp: [FfLl]? */
			*(fp_t*)pdata = xstrtod(irl_token,&endptr);
		}
		else
		{
			irl_scan_exponent(fpin); /* regexp: ([EeDdQq][-+]*[0-9]+)? */
			irl_scan_suffix(fpin); /* regexp: [FfLl]? */

#if defined(HAVE_FP_T_QUADRUPLE)
			*(fp_t*)pdata = strtold(irl_token,&endptr);
#else
			*(fp_t*)pdata = strtod(irl_token,&endptr);
#endif

		}
	}
	else if (irl_is_inf(fpin))
		*(fp_t*)pdata = (irl_token[0] == '-') ? -Infinity() : Infinity();
	else if (irl_is_nan(fpin))
		*(fp_t*)pdata = (irl_token[0] == '-') ? -NaN() : NaN();
	else if (irl_is_qnan(fpin))
		*(fp_t*)pdata = (irl_token[0] == '-') ? -QNaN() : QNaN();
	else if (irl_is_snan(fpin))
		*(fp_t*)pdata = (irl_token[0] == '-') ? -SNaN() : SNaN();
	else				/* did not find a valid number */
	{
		result = 0;
		endptr = &irl_token[0];
	}
	if (endptr != (char *)NULL)	/* put back unread input */
	{
		char *last;
		last = strchr(endptr,'\0') - 1;
		while (last >= endptr)
			(void)irl_unget_char(*last--);
	}
	return (result);
}

int
irl_getchar(FILE * fpin)
{	/* return an (unsigned) char in 0..255, or EOF (== -1) from fpin */
	if (EOF_flag)			/* then user called endinput() */
	{
		irl_pushpos = -1;
		EOF_flag = 0;		/* Hmm... needed for nested load() calls, but what about repeated calls to this function for the same file??? */
		return (EOF);
	}

	if (irl_pushpos >= 0L)
		return ((int)irl_pushback[irl_pushpos--]);

	if (irl_nextc == (const char *)NULL) /* time to refill input line buffer */
		irl_nextc = irl_gets(fpin);

	if (irl_nextc == (const char *)NULL) /* then no more input data */
		return (EOF);
	else if (*irl_nextc == '\0')	/* then end of line */
	{
		irl_nextc = (const char *)NULL;	/* to force refill next time */
		return ((int)'\n');
	}
	else
	{
		/* NB: This is a critical cast needed to support full
		ISO8859-n 256-element character sets! Without it,
		since on most UNIX systems, char is signed, negative
		values are not recognized by IsAlNum() and IsAlpha()
		in hoc.y, and for some parser generated code, are
		interpreted as end-of-file, sigh... */
		return ((int)((unsigned char)*irl_nextc++));
	}
}

static char *
irl_getline(char *buffer, size_t max_buffer, FILE *fpin)
{
	/* Get a line from fpin.  Act almost like fgets()/gets(), but
	   recogize, and remove, LF, CR LF, and CR line terminators,
	   and also remove PC-DOS/CPM-86 Ctl-Z at end of file.  This
	   routine is patterned on the definition of text lines in the
	   Java Language Specification, second edition,
	   Addison-Wesley, 2000, pp 16--17. */
	int c;
	int next_c;
	size_t n;

	for (n = 0; (n < (max_buffer - 1)) && ((c = fgetc(fpin)) != EOF); )
	{
		if (c == '\n')		/* UNIX-style LF terminator */
		{
			buffer[n++] = '\n';
			break;
		}
		else if (c == '\r')	/* expect either CR LF or CR terminator */
		{
			next_c = fgetc(fpin);
			if (next_c != '\n') /* have bare CR, so push back lookahead */
				(void)(ungetc)(next_c,fpin);
			buffer[n++] = '\n';
			break;
		}
		else if (c == '\032')	/* Ctl-Z */
		{
			while ((next_c = fgetc(fpin)) == '\032')/* discard runs of Ctl-Z */
				/* NO-OP */ ;
			if (next_c == EOF) /* ignore Ctl-Z at EOF */
				break;
			else		/* ordinary character */
			{
				(void)(ungetc)(next_c,fpin);
				buffer[n++] = c;
			}
		}
		else			/* ordinary character */
			buffer[n++] = c;
	}
	buffer[n] = '\0';
	return (((n == 0) && (c == EOF)) ? (char *)NULL : buffer);
}

static const char *
irl_gets(FILE *fpin)	/* I adapted this from the readline info docs: thanks, folks! */
{
	/***************************************************************
	  Read the next input line, and return a pointer to it, or
	  NULL on EOF.  The final newline character is NOT stored.

	  If the buffer has already been allocated, first return the
	  memory to the free pool.

	  NB: **ALL** input to hoc comes from this single function.

	  The companion public functions irl_fscanf(), irl_getchar(),
	  and irl_unget_char() merely access the data saved here in
	  irl_current_line[].
	***************************************************************/

	int interactive_file = isatty(fileno(fpin));

#if defined(HAVE_GNU_READLINE)
	/* Allow echoing only if input is from an interactive file. */
	readline_echoing_p = interactive_file;

	/* Allow conditional parsing of the ~/.inputrc file. */
	rl_readline_name = PACKAGE_NAME;

	/* Ensure that symbol table word completion is available. */
	rl_attempted_completion_function = irl_completion;
#endif

	irl_free();

	if (EOF_flag)			/* then user called endinput() */
		return ((const char *)NULL);

	/***************************************************************
	  Get a line from the user, prompting if the input file is
	  interactive, and prompting has not been suppressed by the
	  user.

	  On some systems, this works just fine.  On others, each
	  input line produces an error message on stderr with the text
	  "readline: warning: irl_prep_terminal: cannot get terminal
	  settings", without a following newline.  Since hoc version 7
	  has default startup files, this spews more than 100K
	  characters onto the screen.  This is evidently a design flaw
	  in GNU realine 4.2a (and likely, earlier versions), and will
	  be reported to the developers (along with a number of other
	  comments on its design limitations).

	  The solution that we adopt for now is to call readline()
	  only for interactive files.  Otherwise, we use fgets()
	  instead.  We put only lines from readline() into the history
	  list, since it is unlikely to be useful to recall lines from
	  startup files.
	***************************************************************/

#if defined(HAVE_GNU_READLINE)
	if (use_readline && interactive_file)
	{
		char *the_line;

		/* We use our own memory allocation facility, and if its
		   debugging version is selected, efree() and free() are
		   NOT interchangeable.  For now, we just make a copy of
		   the returned line.  If profiling shows that this is a
		   problem, the code can be revised. */

		the_line = readline((interactive_file && get_verbose())
					   ? new_prompt() : "");
		if (the_line == (char *)NULL) /* hit EOF */
			return ((const char *)NULL);
		else
		{
			irl_current_line = dupstr(the_line);
			free(the_line);		/* NB: NOT efree() */
		}

		/* If the line has any text in it, save it on the history. */
		if ((irl_current_line != (const char *)NULL) && *irl_current_line)
			add_history (irl_current_line);
	}
	else
#endif
	{
		irl_current_line = irl_readline(fpin, (interactive_file && get_verbose())
						? new_prompt() : (char *)NULL);
	}

	if (logfile_enabled && (fplog != (FILE*)NULL) && (irl_current_line != (const char *)NULL))
		(void)fprintf(fplog, "%s\n", irl_current_line);

	return (irl_current_line);
}

static int
irl_is_0x(FILE *fpin)
{	/* return 1 if we match the regexp 0[Xx], else 0 */
	int the_char, next_char;

	the_char = irl_getchar(fpin);
	if (the_char == '0')
	{
		next_char = irl_getchar(fpin);
		if ((next_char == 'X') || (next_char == 'x'))
		{
			irl_add_to_token(the_char);
			irl_add_to_token(next_char);
			return (1);
		}
		(void)irl_unget_char(next_char);
	}
	(void)irl_unget_char(the_char);
	return (0);
}

static int
irl_is_inf(FILE *fpin)
{
	return (irl_match_input(fpin,"Infinity") || irl_match_input(fpin,"Inf"));
}


static int
irl_is_nan(FILE *fpin)
{
	return (irl_match_input(fpin,"NaN"));
}

static int
irl_is_qnan(FILE *fpin)
{
	return (irl_match_input(fpin,"QNaN"));
}

static int
irl_is_snan(FILE *fpin)
{
	return (irl_match_input(fpin,"SNaN"));
}

static int
irl_match_input(FILE *fpin, const char *s)
{	/* match input against s, ignoring lettercase, and add to token on match: return 1 on match, else 0 */
	int result;

	result = 0;
	if (s != (const char *)NULL)
	{
		char *buffer;
		int the_char;
		int n;

		buffer = (char *)emalloc(strlen(s) + 1);
		for (n = 0; *s != '\0'; ++s)
		{
			the_char = irl_getchar(fpin);
			buffer[n++] = (char)the_char;
			if (irl_tolower((int)*s) != irl_tolower(the_char))
				break;
		}
		if (*s == '\0')		/* then got a match: add to token */
		{
			int k;

			result = 1;
			for (k = 0; k < n; ++k)
				irl_add_to_token((int)buffer[k]);
		}
		else			/* match failure: put back lookahead */
		{
			while (--n >= 0)
				(void)irl_unget_char((int)buffer[n]);
		}
		efree((void *)buffer);
	}
	return (result);
}

void
irl_pop_input(void)
{
	if (irl_stack == (irl_stack_entry *)NULL)
		execerror("input file stack underflow", "");
	else
	{
		if (irl_pushpos >= 0L)
			execerror("pushback buffer data lost in irl_pop_input()", "");
		if (irl_current_line != (const char *)NULL)
			efree((void *)irl_current_line);
		irl_current_line = irl_stack->line;
		irl_nextc = &irl_current_line[irl_stack->nextc];
		irl_pushpos = irl_stack->pushpos;
		if (irl_stack->n_pushback > irl_max_pushback) /* need bigger pushback buffer */
		{
			irl_pushback = (char *)erealloc((void *)irl_pushback,
						       irl_max_pushback,
						       irl_stack->n_pushback);
			(void)update_const_number("__MAX_PUSHBACK__",
						  (fp_t)(irl_stack->n_pushback));
		}
		(void)memcpy((void *)irl_pushback, (void *)irl_stack->pushback, irl_stack->n_pushback);
		irl_max_pushback = irl_stack->n_pushback;

		/* Free the stacked pushback buffer, and its stack entry */
		efree((void *)irl_stack->pushback);

		{
			irl_stack_entry *old_irl_stack;

			old_irl_stack = irl_stack;
			irl_stack = irl_stack->next;
			efree(old_irl_stack);
		}
	}
}

void
irl_push_input(void)
{
	irl_stack_entry *next_entry;

	next_entry = (irl_stack_entry*)emalloc(sizeof(irl_stack_entry));
	next_entry->next = irl_stack;
	irl_stack = next_entry;
	irl_stack->line = irl_current_line;
	irl_stack->nextc = (long)(irl_nextc - irl_current_line);
	irl_stack->pushback = (char *)emalloc(irl_max_pushback);
	irl_stack->n_pushback = irl_max_pushback;
	(void)memcpy((void *)irl_stack->pushback, (void *)irl_pushback, irl_max_pushback);
	irl_stack->pushpos = irl_pushpos;

	/* Reset buffers to empty */
	irl_pushpos = -1;
	irl_current_line = (const char *)NULL;
	irl_nextc = (const char *)NULL;
}

static const char *
irl_readline(FILE *fpin, const char *prompt)
{
	/* Get the next line, minus any trailing newline, from fpin,
	  and return a pointer to malloc'ed memory containing that
	  line.  The caller must call free() for that storage before
	  calling this function again if a serious memory leak is to
	  be avoided. */

	char *eosptr;			/* pointer to NUL at end of this_line */
	static size_t max_line = 128;	/* expanded dynamically if needed */
	char *p;			/* return value from irl_getline() */
	char *this_line;		/* pointer to dynamic line buffer */

	this_line = (char *)emalloc(max_line);
	if (prompt != (const char *)NULL)
	{
		(void)printf("%s", prompt);
		(void)fflush(stdout);
	}

	p = irl_getline(this_line, max_line, fpin);
	if (p == (char *)NULL)
	{
		efree((void*)this_line);
		return ((const char *)NULL); /* no more data from this file */
	}

	for (;;)	/* loop until we get a complete line, or hit EOF, or run out of memory */
	{
		eosptr = strchr(this_line, '\0'); /* find end of string */
		if (eosptr == (char *)NULL) /* cannot happen, but keep lint happy */
		{
			efree((void *)this_line);
			return ((const char *)NULL);
		}
		else if (eosptr[-1] == '\n') /* then we got a complete line of data */
		{
			eosptr[-1] = '\0'; /* discard the newline character */
			return (this_line);
		}
		else	/* we got only a partial line: enlarge this_line[] buffer */
		{
			size_t bytes_read = (size_t)(eosptr - this_line);

			this_line = (char *)egrow((void *)this_line, &max_line, sizeof(this_line[0]));
			(void)update_const_number("__MAX_LINE__", (fp_t)max_line);
			p = irl_getline(this_line + bytes_read, max_line - bytes_read, fpin);
			if (p == (char *)NULL)		/* out of data in file */
				return (this_line);	/* so return what we have so far */
		}
	}
}

static void
irl_scan_digits(FILE *fpin)
{					/* regexp: [0-9]* */
	int the_char;

	the_char = irl_getchar(fpin);
	while (decval(the_char) >= 0)
	{
		irl_add_to_token(the_char);
		irl_ndigits++;
		the_char = irl_getchar(fpin);
	}
	(void)irl_unget_char(the_char);
}

static void
irl_scan_dot(FILE *fpin)
{					/* regexp: [.]? */
	int the_char;

	the_char = irl_getchar(fpin);
	if (the_char == '.')
		irl_add_to_token(the_char);
	else
		(void)irl_unget_char(the_char);
}

static void
irl_scan_exponent(FILE *fpin)
{					/* regexp: [EeDdQq][-+]*[0-9]+ */
	int the_char;
	int the_uc_char;

	the_char = irl_getchar(fpin);
	the_uc_char = irl_toupper(the_char);
	if ((the_uc_char == 'D') || (the_uc_char == 'E') || (the_uc_char == 'Q'))
	{
		/* NB: We do not need to be precise here: strtod()
		will catch any error from missing exponent digits */
		irl_add_to_token('e');	/* force to C/C++-style exponent */
		irl_scan_sign(fpin);
		irl_scan_digits(fpin);
	}
	else
		(void)irl_unget_char(the_char);
}

static void
irl_scan_hex_digits(FILE *fpin)
{					/* regexp: [0-9A-Fa-f]* */
	int the_char;

	the_char = irl_getchar(fpin);
	while (hexval(the_char) >= 0)
	{
		irl_add_to_token(the_char);
		irl_ndigits++;
		the_char = irl_getchar(fpin);
	}
	(void)irl_unget_char(the_char);
}

static void
irl_scan_hex_exponent(FILE *fpin)
{					/* regexp: [Pp][-+]*[0-9]+ */
	int the_char;

	the_char = irl_getchar(fpin);
	if (irl_toupper(the_char) == 'P')
	{
		/* NB: We do not need to be precise here: strtod()
		will catch any error from missing exponent digits */
		irl_add_to_token(the_char);
		irl_scan_sign(fpin);
		irl_scan_digits(fpin);
	}
	else
		(void)irl_unget_char(the_char);
	irl_scan_suffix(fpin);
}

static void
irl_scan_sign(FILE *fpin)
{					/* regexp: [-+]* */
	int the_char;

	the_char = irl_getchar(fpin);
	while ((the_char == (int)'+') || (the_char == (int)'-'))
	{
		irl_add_to_token(the_char);
		the_char = irl_getchar(fpin);
	}
	(void)irl_unget_char(the_char);
}

static void
irl_scan_suffix(FILE *fpin)
{					/* regexp: [FfLl] */
	int the_char;
	int the_uc_char;

	/* C/C++ recognizes datalength suffixes f, F, l, and L.  Java
	   recognizes d, D, f, and F, but the first two, which are
	   rarely used anyway, conflict with Fortran's
	   double-precision exponent letter: should "1.23d+45" mean
	   "1.23e+45" (Fortran) or "1.23 + 45" (Java)?  The Fortran
	   style is likely to be much more common, so we choose its
	   syntax over Java's. */

	the_char = irl_getchar(fpin);
	the_uc_char = irl_toupper(the_char);
	if ((the_uc_char == 'F') || (the_uc_char == 'L'))
		irl_add_to_token(the_char);
	else
		(void)irl_unget_char(the_char);
}

static void
irl_scan_whitespace(FILE *fpin)
{					/* regexp: [ \t]* */
	int the_char;

	the_char = irl_getchar(fpin);
	while ((the_char == (int)' ') || (the_char == (int)'\t'))
		the_char = irl_getchar(fpin);
	(void)irl_unget_char(the_char);
}

static int
irl_tolower(int c)
{
	return (isupper(c) ? tolower(c) : c);
}

static int
irl_toupper(int c)
{
	return (islower(c) ? toupper(c) : c);
}

int
irl_unget_char(int c)
{
	if ((size_t)(irl_pushpos + 1) >= irl_max_pushback) /* time to grow irl_pushback[] buffer */
	{
		irl_pushback = (char *)egrow((void *)irl_pushback,
					       &irl_max_pushback,
					       sizeof(irl_pushback[0]));
		(void)update_const_number("__MAX_PUSHBACK__",
					  (fp_t)irl_max_pushback);
	}
	irl_pushback[++irl_pushpos] = (char)c;
	return (c);
}

static const char *
new_prompt(void)
{
	const char *p = get_prompt();
	const char *fmt;
	static int count = 0;

	count++;
	fmt = (const char *)strstr(p,"%d");
	if ((fmt != (const char *)NULL) && (fmt == (const char *)strchr(p,'%')))
	{	/* then %d format occurs first, so can safely use sprintf() */
		static char *the_prompt = (char *)NULL;
		size_t len_prompt;

		if (the_prompt != (const char *)NULL)
			efree((void *)the_prompt);

		len_prompt = strlen(p) + 21 + 1;
		the_prompt = (char *)emalloc(len_prompt); /* space for signed 64-bit integer */
		SNPRINTF4(the_prompt, len_prompt, p, count); /* cannot overflow */
		return ((const char *)the_prompt);
	}
	else
		return (p);
}
