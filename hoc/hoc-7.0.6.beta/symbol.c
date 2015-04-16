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

#undef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#define MSG_PREFIX "___msg_"	/* NB: 3+ leading underscores hide names from who("") output */

static Symbol *		symlist = (Symbol*)NULL;	/* symbol table: linked list */
static Symbol *		this_symbol = (Symbol*)NULL;	/* set by first_symbol_name() */
static Symbol *		this_sym = (Symbol*)NULL;	/* set by first_symbol() */

static const char *	title = (const char*)NULL;

static int		delete_symbol_unchecked ARGS((Symbol *));
static const char *	first_symbol_name ARGS((void));
static const char *	make_pname ARGS((Symbol *));
static const char *	next_matching_symbol_name ARGS((const char *));
static const char *	next_symbol_name ARGS((void));
static void		set_title ARGS((const char*));

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"
#endif
int
cmpsym(const void *v1, const void *v2)
{
	return (strcmp((*(Symbol**)v1)->name, (*(Symbol**)v2)->name));
}

static int
compare_format_items(const char *s, const char *t)
{
	/* In order to prevent denial-of-service attacks from bogus
	   translations of format strings, we require that all %<CHAR>
	   sequences in s match those in t.  This is a small subset of
	   printf() formats: the only one needed so far in hoc message
	   translations is %s. */
	for (s = (const char*)strchr(s,'%');
	     s != (const char*)NULL;
	     s = (const char*)strchr(s + 1,'%'))
	{
		t = (const char *)strchr(t,'%');
		if (t == (const char*)NULL)
			return (0);
		if (s[1] != t[1])
			return (0);
	}
	return (1);
}

int
delete_symbol(Symbol *sp)
{	/* return 1 if deletion was allowed and successful, else 0 */

	/* TO DO: should we allow deletion of user-defined function/procedure names? */
	if (sp == (Symbol*)NULL)
		return (0);
	else if (((sp->type == VAR) || (sp->type == STRVAR)) && !(sp->immutable))
		return (delete_symbol_unchecked(sp));
	else
		return (0);
}

static int
delete_symbol_unchecked(Symbol *sp)
{	/* delete ANY requested symbol, and return 1 on success, and 0 on failure */
	Symbol *tp;
	Symbol *last;

	for (last = (Symbol*)NULL, tp = symlist; tp != (Symbol *)NULL; tp = tp->next)
	{
		if (tp == sp)
		{
			if (last == (Symbol*)NULL)
				symlist = symlist->next;
			else
				last->next = tp->next;
#if defined(DEBUG_LOOKUP)
			if (last == (Symbol*)NULL)
				tp = symlist;
			else
				tp = last;
			(void)fprintf(stderr,
				      "\t%%DEBUG: delete(%s): tp = %p\ttp->next = %p\tname = %s\tnext->name = %s\n\n",
				      sp->name, tp, tp->next, tp->name,
				      (tp->next == (Symbol*)NULL) ? "" : tp->next->name);
#endif
			efree((void*)sp);
			return (1);
		}
		last = tp;
	}

	return (0);
}

static void
dump_one_type(const char *pattern, int is_constant, int type)
{
	Symbol *sp;
	Symbol **symtab;
	size_t k;
	size_t max_symbols;
	size_t n_symtab;

	max_symbols = (size_t)0;
	for (sp = symlist; sp != (Symbol *)NULL; sp = sp->next)
		max_symbols++;

	symtab = (Symbol**)emalloc(max_symbols * sizeof(Symbol*));

	/* First build a table of matching symbols that we can later sort */
	for (n_symtab = 0, sp = symlist; sp != (Symbol *)NULL; sp = sp->next)
	{
		if (type != sp->type)
			continue;
		if (is_constant && !sp->immutable)
			continue;
		if (!is_constant && sp->immutable)
			continue;
		if ((sp->name != (const char*)NULL) &&
		    *sp->name &&
		    (n_symtab < max_symbols) &&
		    !is_hidden(sp->name,pattern) &&
		    is_match(sp->name,pattern))
			symtab[n_symtab++] = sp;
	}

	qsort(symtab, n_symtab, sizeof(Symbol*), cmpsym);
	for (k = 0; k < n_symtab; ++k)
	{
		if (title != (const char*)NULL)
		{
			prtext(msg_translate(title));
			title = (const char*)NULL;
		}
		prtext("\t\t");
		prtext2(symtab[k]->name, 23);
		switch (symtab[k]->type)
		{
		case NUMBER:		/* FALL THROUGH */
		case VAR:
			prtext("\t= ");
			prnum(symtab[k]->u.val);
			break;

		case STRING:		/* FALL THROUGH */
		case STRVAR:
			prtext("\t= ");
			prtext("\"");
			prtext(symtab[k]->u.str);
			prtext("\"");
			break;

		default:
			break;
		}
		prnl();
	}
	efree((void*)symtab);
}

void
dump_syms(const char *pattern)
{
	int old_precision;

	old_precision = get_precision();
	(void)set_precision(default_precision());

	if ((pattern != (char*)NULL) && (*pattern != '\0'))
	{
		prtext(msg_translate("Symbols matching "));
		prtext("\"");
		prtext(pattern);
		prtext("\":");
		prnl();
	}
	else
	{
		prtext(msg_translate("Symbols:"));
		prnl();
	}

	set_title("\n\tNumeric named constants:\n");
	dump_one_type(pattern, 1, NUMBER);
	dump_one_type(pattern, 1, VAR);

	set_title("\n\tString named constants:\n");
	dump_one_type(pattern, 1, STRING);
	dump_one_type(pattern, 1, STRVAR);

	set_title("\n\tNumeric variables:\n");
	dump_one_type(pattern, 0, NUMBER);
	dump_one_type(pattern, 0, VAR);

	set_title("\n\tString variables:\n");
	dump_one_type(pattern, 0, STRING);
	dump_one_type(pattern, 0, STRVAR);

	set_title("\n\tBuilt-in numeric functions (zero arguments):\n");
	dump_one_type(pattern, 0, BLTIN0);

	set_title("\n\tBuilt-in numeric functions (one numeric argument):\n");
	dump_one_type(pattern, 0, BLTIN1);

	set_title("\n\tBuilt-in numeric functions (one string argument):\n");
	dump_one_type(pattern, 0, BLTIN1S);

	set_title("\n\tBuilt-in numeric functions (one symbol argument):\n");
	dump_one_type(pattern, 0, BLTIN1I);

	set_title("\n\tBuilt-in numeric functions (two numeric arguments):\n");
	dump_one_type(pattern, 0, BLTIN2);

	set_title("\n\tBuilt-in numeric functions (two string arguments):\n");
	dump_one_type(pattern, 0, BLTIN2S);

	set_title("\n\tBuilt-in string functions (zero arguments):\n");
	dump_one_type(pattern, 0, STRBLTIN0);

	set_title("\n\tBuilt-in string functions (one string argument):\n");
	dump_one_type(pattern, 0, STRBLTIN1);

	set_title("\n\tBuilt-in string functions (one numeric argument):\n");
	dump_one_type(pattern, 0, STRBLTIN1N);

	set_title("\n\tBuilt-in string functions (two string arguments):\n");
	dump_one_type(pattern, 0, STRBLTIN2);

	set_title("\n\tBuilt-in string functions (one string and one numeric arguments):\n");
	dump_one_type(pattern, 0, STRBLTIN2SN);

	set_title("\n\tBuilt-in string functions (one string and two numeric arguments):\n");
	dump_one_type(pattern, 0, STRBLTIN3SNN);

	set_title("\n\tUser-defined functions:\n");
	dump_one_type(pattern, 0, FUNCTION);

	set_title("\n\tUser-defined procedures:\n");
	dump_one_type(pattern, 0, PROCEDURE);

	prnl();
	(void)set_precision(old_precision);
}

const char *
dupstr(const char *s)
{
	char *t;
	size_t len_t;

	if (s == (const char*)NULL)	/* treat NULL like "" */
		s = "";
	len_t = strlen(s) + 1;
	t = (char *)emalloc(len_t); /* +1 for '\0' */
	(void)strlcpy(t,s,len_t);
	return ((const char*)t);
}

void*
egrow(void* s, size_t *p_old_size, size_t element_size)
{	/* grow the dynamic array s, and update *p_old_size to the new size on return */
	size_t new_bytes;

	if (element_size < 1)		/* safety check */
		element_size = 1;
	new_bytes = 2 * (*p_old_size) * element_size;
	if (new_bytes == (size_t)0)
		new_bytes = (size_t)16 * element_size;
	else if (new_bytes >= (size_t)8192)
		new_bytes = (*p_old_size) * element_size + (size_t)MAX(8192, 4*element_size);
	s = erealloc(s, *p_old_size * element_size, new_bytes);
	*p_old_size = new_bytes / element_size;
	return (s);
}

#if defined(DEBUG_EMALLOC)

#define MAXBLOCKS 10000			/* enough for hoc testing */

static size_t nblocks = 0;

static struct memblock
{
	char *adr;
	size_t len;
	int in_use;
} blocks[MAXBLOCKS];

static const char magic[] = { 0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe, 0xba, 0xbe };

static int
echeck(void)
{	/* return 1 if magic bytes are still intact in all allocated blocks, else 0 (in which case,
	   print an error message) */
	size_t k;

	for (k = 0; k < nblocks; ++k)
	{
		if (blocks[k].in_use &&
		    (memcmp((void*)(blocks[k].adr + blocks[k].len), (void*)magic, sizeof(magic)) != 0))
		{
			(void)fprintf(stderr, "ERROR: Corruption in block %u [%.8s]\n", k, blocks[k].adr);
			if (k > 0)
				(void)fprintf(stderr, "\t Previous block %u [%.8s]\n", k-1, blocks[k-1].adr);
			return (0);
		}
	}
	return (1);
}

void
efree(void *p)
{
	(void)fprintf(stderr, "%%DEBUG: efree(%p)\n", p);
	if (p != (void*)NULL)	/* because some older free() implementations fail with NULL arguments */
	{
		if (echeck())
		{
			size_t k;

			for (k = 0; k < nblocks; ++k)
			{
				if (blocks[k].in_use && (p == blocks[k].adr))
				{
					blocks[k].in_use = 0;
					free(p);
					return;
				}
			}
			(void)fprintf(stderr, "ERROR: Attempt to free unallocated block at %p\n", p);
		}
	}
}

void*
emalloc(size_t n)	/* check return from malloc */
{
	char *p;

	if (n == 0)	/* some malloc()'s fail for zero-size requests */
		n = 1;

	(void)echeck();

	p = (char*)malloc(n + sizeof(magic));

	if (p == 0)
		execerror("out of memory", (const char *)NULL);

	(void)memset(p, (int)0xee, n);

	blocks[nblocks].adr = p;
	blocks[nblocks].len = n;
	blocks[nblocks].in_use = 1;
	(void)memcpy((void*)(blocks[nblocks].adr + blocks[nblocks].len), (void*)magic, sizeof(magic));
	nblocks++;
	if (nblocks >= MAXBLOCKS)
		nblocks = 0;

	(void)fprintf(stderr, "%%DEBUG: emalloc(%u) -> %p\n", n, p);
	return (p);
}

void *
erealloc(void *p, size_t old_size, size_t new_size)
{
	void *old_p;
	old_p = p;
	p = emalloc(new_size);
	if ((old_size > 0) && (old_p != (char*)NULL))
		(void)memcpy(p, old_p, (old_size > new_size ? new_size : old_size));
	(void)fprintf(stderr, "%%DEBUG: erealloc(%p,%u,%u) -> %p\n", old_p, old_size, new_size, p);
	efree(old_p);
	return (p);
}

#else /* normal emalloc(), efree(), erealloc() */

void
efree(void *p)
{
	if (p != (void*)NULL)	/* because some older free() implementations fail with NULL arguments */
		free(p);
}

void*
emalloc(size_t n)	/* check return from malloc */
{
	char *p;

	if (n == 0)	/* some malloc()'s fail for zero-size requests */
		n = 1;

	p = (char*)malloc(n);

	if (p == 0)
		execerror("out of memory", (const char *)NULL);

	return p;
}

void *
erealloc(void *p, size_t old_size, size_t new_size)
{
	/* some realloc()'s do not handle NULL pointers, so use malloc() */
	return ((p == (void*)NULL) ? malloc(new_size) : realloc(p,new_size));
}

#endif /* defined(DEBUG_EMALLOC) */

static const char *
first_matching_symbol_name(const char *pattern)	/* NO LONGER USED */
{	/* return first symbol name matching pattern, or NULL */
	const char *name;

	name = first_symbol_name();
	while ((name != (const char*)NULL) && !is_match(name,pattern))
		name = next_symbol_name();
	return (name);
}

Symbol *
first_symbol(void)
{
	this_sym = symlist;
	return (this_sym);
}


static const char *
first_symbol_name(void)
{
	this_symbol = symlist;
	return (this_symbol->name);
}

const char *
first_symbol_pname(void)
{	/* return dynamic copy of first symbol, with optional () */
	this_symbol = symlist;
	return (make_pname(this_symbol));
}

void
free_symbol_table(void)
{
	Symbol *sp;
	Symbol *next_sp;

	for (sp = symlist; sp != (Symbol *)NULL; sp = next_sp)
	{
		next_sp = sp->next;
		if (sp->type == STRVAR)
			efree((void*)sp->u.str);
		efree((void*)sp->name);
		efree(sp);
	}
	symlist = (Symbol*)NULL;
}

fp_t
get_number(const char *name)
{
	/* Return the numeric value of name, or 0.0 if it does not
	   exist, or does not have a conversion to numeric */

	Symbol *sp;

	sp = lookup(name);
	if (sp == (Symbol *)NULL)
		return (FP(0.0));
	else if (sp->type == VAR)
		return (sp->u.val);
	else if (sp->type == STRVAR)
	{
		char *end_ptr;
		fp_t result;

		result = strton(sp->u.str, &end_ptr);
		return ((*end_ptr == '\0') ? FP(0.0) : result);
	}
	else
		return (FP(0.0));
}

const char *
get_string(const char *name)
{
	/* Return the string value of name, or "" if it does not
	   exist, or does not have a conversion to string.  The caller
	   should use dupstr() to make a copy if the result is needed
	   more than ephemerally.  */

	Symbol *sp;

	sp = lookup(name);
	if (sp == (Symbol *)NULL)
		return ("");
	else if (sp->type == STRVAR)
		return (sp->u.str);
	else if (sp->type == VAR)
		return (fmtnum(sp->u.val));
	else
		return ("");
}

Symbol*
install(const char* s, int t, fp_t d)  /* install s in symbol table */
{
	Symbol *sp;

	sp = (Symbol *)emalloc(sizeof(Symbol));
	memset(sp, 0, sizeof(Symbol));	/* ensure that all bits are zero */
	sp->name = dupstr(s);
	sp->builtin = 0;
	sp->immutable = 0;
	sp->type = t;
	sp->u.val = d;
	sp->next = symlist; /* put at front of list */
	symlist = sp;

#if defined(DEBUG_LOOKUP) || defined(DEBUG_INSTALL)
	(void)fprintf(stderr,
		      "%%DEBUG: install(%s,%d,%g)\tsp = %p\tsp->next = %p\tname = %s\tnext->name = %s\n",
		      s, t, (double)d, sp, sp->next, sp->name,
		      (sp->next == (Symbol*)NULL) ? "" : sp->next->name);
#endif

	return sp;
}

Symbol*
install_const_number(const char* s, fp_t d)  /* install s in symbol table */
{
	Symbol *sp;

	sp = install(s, VAR, d);
	make_immutable(sp);
	return sp;
}

Symbol*
install_const_string(const char* s, const char *value)  /* install s in symbol table */
{
	Symbol *sp;

	sp = install_string(s, value);
	make_immutable(sp);
	return sp;
}

Symbol*
install_number(const char* s, fp_t d)	/* install s in symbol table */
{
	Symbol *sp;

	sp = install(s, VAR, d);
	sp->immutable = 0;
	return sp;
}

Symbol*
install_string(const char* s, const char *value)  /* install s in symbol table */
{
	Symbol *sp;

	sp = install(s, VAR, FP(0.0));
	sp->immutable = 0;
	sp->type = STRVAR;
	sp->u.str = dupstr(value);
	return sp;
}

int
is_debug(const char *name)
{
	Symbol *sp;

	sp = lookup(name);
	return ((sp != (Symbol*)NULL) && (sp->u.val != 0));
}

int
is_hidden(const char *name, const char *pattern)
{	/* return 1 if name is hidden (i.e., had 3 or more leading underscores)
	   and pattern is NULL or empty */
	return (((pattern == (const char *)NULL) || (*pattern == '\0')) &&
		(strncmp(name,"___",3) == 0));
}

int
is_immutable(const char *name)
{	/* return 1 if name exists and is immutable, else 0 */
	Symbol *sp;
	sp = lookup(name);
	return ((sp != (Symbol*)NULL) && sp->immutable);
}

int
is_local(const char *name)
{
	/* return 1 if name exists and is an internal local variable, else 0 */
	return (strchr(name,'@') != (char*)NULL);
}

int
is_match(const char *name, const char *pattern)
{	/* return 1 if name matches pattern, else 0 */
	size_t len_name;
	size_t len_pattern;

	if ((pattern == (const char*)NULL) || (*pattern == '\0'))
		return (1);		/* empty or NULL pattern matches everything */
	if ((name == (const char*)NULL) || (*name == '\0'))
		return (0);		/* empty or NULL name matches nothing */

	len_name = strlen(name);
	len_pattern = strlen(pattern);

#if 0
	if (len_name < len_pattern)
		return (0);		/* short name cannot match long pattern */
#endif

	/* Triple underscore (i.e., hidden) symbols match only patternes
	   with triple underscore */
	if (((len_name >= 3) && (strncmp(name,"___",3) == 0)) &&
		 ((len_pattern < 3) || (strncmp(pattern,"___",3) != 0)))
		return (0);
	else
#if 0
		return (strncmp(name,pattern,len_pattern) == 0);
#else
		return (match(name,pattern));
#endif
}

Symbol*
lookup(const char* s)	/* find s in symbol table */
{
	Symbol *sp;

#if defined(DEBUG_LOOKUP)
	(void)fprintf(stderr,"\n%%DEBUG: lookup(%s)\n", s);
#endif

#if 0
	for (sp = symlist; sp != (Symbol *)NULL; sp = sp->next)
	{
#if defined(DEBUG_LOOKUP)
		(void)fprintf(stderr,
			      "\t%%DEBUG: lookup(%s)\tsp = %p\tsp->next = %p\tname = %s\tnext->name = %s\n",
			      s, sp, sp->next, sp->name,
			      (sp->next == (Symbol*)NULL) ? "" : sp->next->name);
#endif
		if (STREQUAL(sp->name, s))
		{
#if defined(DEBUG_LOOKUP)
			(void)fprintf(stderr,"\t%%DEBUG: lookup(%s) matched!\n", s);
#endif
			return sp;
		}
	}
#else
	/* Revise to use move-to-front algorithm for (probably) faster lookup */
	Symbol *last_sp = (Symbol *)NULL;
	Symbol *old_symlist = symlist;

	for (sp = symlist; sp != (Symbol *)NULL; sp = sp->next)
	{
#if defined(DEBUG_LOOKUP)
		(void)fprintf(stderr,
			      "\t%%DEBUG: lookup(%s)\tsp = %p\tsp->next = %p\tname = %s\tnext->name = %s\n",
			      s, sp, sp->next, sp->name,
			      (sp->next == (Symbol*)NULL) ? "" : sp->next->name);
#endif
		if (STREQUAL(sp->name, s))
		{
#if defined(DEBUG_LOOKUP)
			(void)fprintf(stderr,"\t%%DEBUG: lookup(%s) matched!\n", s);
#endif
			if (sp != symlist) /* then not at head */
			{
				if (last_sp != (Symbol*)NULL) /* link previous to next */
					last_sp->next = sp->next;
				symlist = sp;	/* move this one to front */
				symlist->next = old_symlist; /* and link to old list */
#if defined(DEBUG_LOOKUP)
				(void)fprintf(stderr,
					      "\t%%DEBUG: lookup(%s): new head: sp = %p\tsp->next = %p\tname = %s\tnext->name = %s\n\n",
					      s, sp, sp->next, sp->name,
					      (sp->next == (Symbol*)NULL) ? "" : sp->next->name);
#endif
			}
			return (sp);
		}
		last_sp = sp;
	}
#endif

#if defined(DEBUG_LOOKUP)
	(void)fprintf(stderr,"\n%%DEBUG: lookup(%s): not found\n", s);
#endif

	return (Symbol*)NULL;	/* 0 ==> not found */
}

void
make_immutable(Symbol *s)		/* NB: there is intentionally NO make_mutable()! */
{
	if (s != (Symbol *)NULL)
		s->immutable = 1;
}

static const char *
make_pname(Symbol *s)
{
	if (s == (Symbol*)NULL)
		return ((const char*)NULL);
	switch (s->type)
	{
	case BLTIN0:		/* FALL THROUGH */
	case BLTIN1:		/* FALL THROUGH */
	case BLTIN1I:		/* FALL THROUGH */
	case BLTIN1S:		/* FALL THROUGH */
	case BLTIN2:		/* FALL THROUGH */
	case BLTIN2S:		/* FALL THROUGH */
	case FUNCTION:		/* FALL THROUGH */
	case PROCEDURE:		/* FALL THROUGH */
	case STRBLTIN0:		/* FALL THROUGH */
	case STRBLTIN1:		/* FALL THROUGH */
	case STRBLTIN1N:	/* FALL THROUGH */
	case STRBLTIN2:		/* FALL THROUGH */
	case STRBLTIN2SN:	/* FALL THROUGH */
	case STRBLTIN3SNN:
		return (concat2(s->name,"()")); /* functions and procedures need final () */

	default:
		return (dupstr(s->name)); /* ordinary symbol */
	}
}


static const char *
msg_name(const char *msg)
{ /* return a dynamically-allocated string with the translation variable corresponding to msg */
	char *new_msg;
	char *new_msg_start;
	char *p;
	size_t new_size;

	new_size = sizeof(MSG_PREFIX) + strlen(msg) + 1;
	new_msg = (char *)emalloc(new_size);
	(void)strlcpy(new_msg,MSG_PREFIX,new_size);

	while (*msg && isspace(*msg))	/* remove leading space */
		msg++;

	new_msg_start = new_msg + sizeof(MSG_PREFIX) - 1;

	for (p = new_msg_start; *msg; msg++) /* copy alphanumerics, reducing */
	{				/* runs of all else to single underscores */
		if (isalnum(*msg))
		{
			*p = *msg;
			if (isupper(*p))
				*p = tolower(*p);
			p++;
		}
		else if (p[-1] != '_')
			*p++ = '_';
	}
	*p-- = '\0';
	while ((p > new_msg_start) && (*p == '_')) /* remove trailing underscores */
		*p-- = '\0';
	return ((const char*)new_msg);
}

const char *
msg_translate(const char *msg)
{		/* return static string translation of msg, or NULL if msg is NULL or empty */
	const char *name;
	const char *new_msg;
	Symbol *sp;

	if ((msg == (const char *)NULL) || (*msg == '\0'))
		return ((const char *)NULL);
	else
	{
		name = msg_name(msg);
		sp = lookup(name);
		efree((void*)name);

		if (sp == (Symbol*)NULL)	/* then no translation variable found: */
			new_msg = msg;		/* return copy of original message */
		else if (*(sp->u.str) == '\0')	/* variable found with empty value: */
			new_msg = msg;		/* return copy of original message */
		else
			new_msg = compare_format_items(msg,sp->u.str) ? sp->u.str : msg;
		return (new_msg);
	}
}

static const char *
next_matching_symbol_name(const char *pattern)
{	/* return next symbol name matching pattern, or NULL */
	const char *name;

	name = next_symbol_name();
	while ((name != (const char*)NULL) && !is_match(name,pattern))
		name = next_symbol_name();
	return (name);
}

Symbol *
next_symbol(void)
{
	if (this_sym == (Symbol*)NULL)
		return ((Symbol*)NULL);
	else
	{
		this_sym = this_sym->next;
		return (this_sym);
	}
}

static const char *
next_symbol_name(void)
{
	if (this_symbol == (Symbol*)NULL)
		return ((const char*)NULL);
	else
	{
		this_symbol = this_symbol->next;
		return ((this_symbol == (Symbol*)NULL) ? (const char*)NULL :
			this_symbol->name);
	}
}

const char *
next_symbol_pname(void)
{	/* return dynamic copy of next symbol, with optional () */
	if (this_symbol == (Symbol*)NULL)
		return ((const char*)NULL);
	else
	{
		this_symbol = this_symbol->next;
		return (make_pname(this_symbol));
	}
}

static Symbol *
set_number(Symbol *s, fp_t d)		/* NO LONGER USED */
{
	/* Store d in s, IGNORING the immutable flag, and return s. */

	s->u.val = d;
	return (s);
}

Symbol *
set_string(Symbol *s, const char *name)
{
	/* Store a copy of name in s, IGNORING the immutable flag, and
	   return s.  Any previous string storage is released. */

	if (s->u.str != (const char*)NULL)
		efree((void*)s->u.str);
	s->u.str = dupstr(name);
	return (s);
}

static void
set_title(const char* s)
{
	title = s;
}

Symbol*
update_const_number(const char* s, fp_t d)
{
	/* Replace d in a symbol, creating the symbol first if needed,
	   and return a pointer to the symbol.  The immutable flag is
	   set, but not tested. */

	Symbol *sp;

	sp = lookup(s);
	if (sp == (Symbol*)NULL)
		sp = install(s, VAR, d);
	else
		sp->u.val = d;
	sp->type = VAR;
	sp->immutable = 1;
	return sp;
}

Symbol*
update_const_string(const char* s, const char *value)
{
	/* Replace s in a symbol, creating the symbol first if needed,
	   and return a pointer to the symbol.  The immutable flag is
	   set, but not tested. */

	Symbol *sp;

	sp = lookup(s);
	if (sp == (Symbol*)NULL)
		sp = install(s, STRVAR, FP(0.0));
	else
		efree((void*)sp->u.str);
	sp->immutable = 1;
	sp->type = STRVAR;
	sp->u.str = dupstr(value);
	return sp;
}

Symbol*
update_number(const char* s, fp_t d)	/* install s in symbol table */
{
	/* Replace d in a symbol, creating the symbol first if needed,
	   and return a pointer to the symbol */

	Symbol *sp;

	sp = lookup(s);
	if (sp == (Symbol*)NULL)
		sp = install(s, VAR, d);
	else if (sp->immutable)
		execerror("illegal reassignment to immutable named constant", sp->name);
	else
		sp->u.val = d;
	sp->type = VAR;
	sp->immutable = 0;
	return sp;
}

Symbol*
update_string(const char* s, const char *value)  /* install s in symbol table */
{
	/* Replace s in a symbol, creating the symbol first if needed,
	   and return a pointer to the symbol */

	Symbol *sp;

	sp = lookup(s);
	if (sp == (Symbol*)NULL)
		sp = install(s, STRVAR, FP(0.0));
	else if (sp->immutable)
		execerror("illegal reassignment to immutable named constant", sp->name);
	else
		efree((void*)sp->u.str);
	sp->immutable = 0;
	sp->type = STRVAR;
	sp->u.str = dupstr(value);
	return sp;
}
