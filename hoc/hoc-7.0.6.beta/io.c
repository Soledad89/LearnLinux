#include "hoc.h"
#include "xtab.h"

#if defined(HAVE_CTYPE_H)
#include <ctype.h>
#endif

#if defined(HAVE_LIMITS_H)
#include <limits.h>
#endif

#if defined(HAVE_SETJMP_H)
#include <setjmp.h>
#endif

#if defined(HAVE_STDIO_H)
#include <stdio.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#include "readline.h"

#ifndef PATH_MAX			/* should be in <limits.h> */
#define PATH_MAX 1024			/* typical UNIX limit */
#endif

#if PATH_MAX < 1024
#undef PATH_MAX
#define PATH_MAX 1024			/* some systems botch this value */
#endif

extern const char *	protect ARGS((const char *));
EXTERN int		cmpsym ARGS((const void *, const void *));
static FILE *		path_fopen ARGS((const char *, const char *, const char **));
static void 		save_symbols ARGS((FILE *, const char *));

#if !defined(F_OK)
#define	F_OK	0		/* Test for file existence: access(filename, F_OK) */
#endif

#define FILE_EXISTS(filename) ((int)access((const char*)(filename), F_OK) == 0)

FILE *fplog = (FILE*)NULL;

const char *elementsep = ":";	/* conventional on UNIX-like systems */
const char *dirsep = "/";	/* conventional on UNIX-like systems */

extern const char *syshocdir;		/* defined in hoc.y */
extern FILE *fin;			/* defined in hoc.y */
extern int cd_pwd_enabled;	   	/* defined in hoc.y */
extern int file_trace_enabled;	      	/* defined in hoc.y */
extern int load_enabled;		/* defined in hoc.y */
extern int logfile_enabled;		/* defined in hoc.y */
extern int save_enabled;		/* defined in hoc.y */

const char *
Cd(const char *to_dir)
{	/* change directory to to_dir, update environment variable PWD,
	   and return new working directory */

	if (!cd_pwd_enabled)
		return (Msg_Translate("cd() command is disabled"));

	if ((to_dir == (const char*)NULL) || (*to_dir == '\0') || STREQUAL(to_dir,"~"))
	   	to_dir = Getenv("HOME");
	else
		to_dir = dupstr(to_dir);

	if (chdir(to_dir) != 0)
		execerror("cd() failed for directory",to_dir);

	(void)Putenv("PWD", to_dir);
	return (to_dir);
}

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"
#endif
int
cmpstr(const void *v1, const void *v2)
{
	return (strcmp(*(const char **)v1,*(const char **)v2));
}

const char *
EndInput(void)
{
	EOF_flag = 1;
	return (dupstr(""));
}

static FILE *
Fopen_one(const char *filename, const char *mode)
{
	FILE *fpio;

	fpio = fopen(filename, mode);
	if (file_trace_enabled && is_debug("__DEBUG_OPEN__"))
	{
		(void)fprintf(stderr, "%s", msg_translate("DEBUG: open of"));
		(void)fprintf(stderr, " %s ", filename);
		(void)fprintf(stderr, "[%s] %s\n",
			      ((mode[0] == 'r') ?
			       msg_translate("input") :
			       msg_translate("output")),
			      ((fpio == (FILE*)NULL) ?
			       msg_translate("failed") :
			       msg_translate("okay")));
	}
	return (fpio);
}

FILE *
Fopen(const char *filename, const char *mode, const char **fullname)
{	/* all file opening in hoc passes through this function, so we can support
	   open tracing in one place, and enforce security */
	const char *new_filename;
	FILE *fpio;

	fpio = Fopen_one(filename, mode);

	/* For input files, we allow omission of a default ".hoc"
	   extension, and try again with that extension */

	if ((fpio == (FILE*)NULL) &&
	    (strstr(filename,".hoc") == (char*)NULL) &&
	    (*mode == 'r'))
	{
		new_filename = concat2(filename,".hoc");
		fpio = Fopen_one(new_filename, mode);
	}
	else
		new_filename = dupstr(filename);

	if (fullname != (const char**)NULL)
		*fullname = (fpio == (FILE*)NULL) ? (const char *)NULL : new_filename;
	else
		efree((void *)new_filename);

	return (fpio);
}

static FILE *
Freopen_one(const char *filename, const char *mode, FILE *fpold)
{
	FILE *fpio;

	fpio = freopen(filename, mode, fpold);
	if (file_trace_enabled && is_debug("__DEBUG_OPEN__"))
	{
		(void)fprintf(stderr, "%s", msg_translate("DEBUG: open of"));
		(void)fprintf(stderr, " %s ", filename);
		(void)fprintf(stderr, "[%s] %s\n",
			      ((mode[0] == 'r') ?
			       msg_translate("input") :
			       msg_translate("output")),
			      ((fpio == (FILE*)NULL) ?
			       msg_translate("failed") :
			       msg_translate("okay")));
	}
	return (fpio);
}

FILE *
Freopen(const char *filename, const char *mode, FILE *fpold, const char **fullname)
{
	const char *new_filename;
	FILE *fpio;

	fpio = Freopen_one(filename, mode, fpold);

	/* For input files, we allow omission of a default ".hoc"
	   extension, and try again with that extension */

	if ((fpio == (FILE*)NULL) &&
	    (strstr(filename,".hoc") == (char*)NULL) &&
	    (*mode == 'r'))
	{
		new_filename = concat2(filename,".hoc");
		fpio = Freopen_one(new_filename,mode, fpold);
	}
	else
		new_filename = dupstr(filename);

	if (fullname != (const char**)NULL)
		*fullname = (fpio == (FILE*)NULL) ? (const char *)NULL : new_filename;
	else
		efree((void *)new_filename);

	return (fpio);
}

static int
is_absolute_filename(const char * filename)
{
	if (filename == (const char*)NULL)
		return (0);
	else if (filename[0] == '/')	/* UNIX/GNU/POSIX style */
		return (1);
	else if (isalpha(filename[0]) &&
		 (filename[1] == ':') &&
		 ((filename[2] == '/') || (filename[2] == '\\')))
		return (1);		/* PC DOS, MS Windows C:\xxx.. or C:/xxx... */
	else if (filename[0] == ':')
		return (1);		/* MacOS :dir:dir:... */
	else
		return (0);
}

#if 0
void
run2(const char *filename)
{
	static jmp_buf begin;
	extern Datum *stackp;
	extern Datum *stack;

	(void)fprintf(stderr,"DEBUG 1: Load(%s): stack = %p stackp = +%p progbase = %p progp = +%p\n",
		      filename, stack, stackp - stackp, progbase, progp - progbase);

	if (setjmp(begin) == 0)

	{
#if 1
		for (initcode(); yyparse(); initcode())
#else
		for (; yyparse(); )
#endif
		{
			(void)fprintf(stderr,"DEBUG 2: Load(%s): stack = %p stackp = +%p progbase = %p progp = +%p\n",
				      filename, stack, stackp - stackp, progbase, progp - progbase);

			execute(progbase);
		}
	}
	(void)fprintf(stderr,"DEBUG 3: Load(%s): stack = %p stackp = +%p progbase = %p progp = +%p\n",
		      filename, stack, stackp - stackp, progbase, progp - progbase);
}
#endif

void
run3(const char *filename)
{
	/***************************************************************
	   We handle load("filename") by pushing back into the input
	   stream the contents of filename, followed by reassignments to
	   reset the line number and filename to their previous values.

	   Unfortunately, it requires making __LINE__ and __FILE__
	   modifiable by user code, in violation of their
	   documentation.

	   This is not entirely satisfactory, since severe errors in the
	   loaded file will not terminate processing of the file when
	   execerror() is called, because it uses fseek() to flush the
	   file, but there will still be data in the pushback buffer.

	   It also causes the test/endinput.hoc test to fail: instead of
	   terminating just the current nested load, it terminates the
	   entire current load, sigh...

	   However, until I can figure out a way to use run() properly,
	   handling the possibility of long jumps back to main, and
	   reallocation of dynamic arrays, this seems to be the best
	   solution.

	   This function also requires two additional assumptions: (1)
	   irl_unget_char() is the proper input pushback function to
	   call, and (2) unlimited pushback is guaranteed.

	   We still need to find a simple, clean, safe, and correct
	   implementation of dynamic loading of hoc files!
	***************************************************************/

	char *buffer;
	const char *p;
	const char *q;
	char numbuf[21];	/* large enough for 64-bit integer output to %ld format */
	int c;
	size_t max_buffer;
	size_t n;

	SNPRINTF4(numbuf, sizeof(numbuf), "%ld", (long)lookup("__LINE__")->u.val);
	p = concat3("__LINE__ = ", numbuf, ";");
	n = strlen(p);
	while (n > 0)
		(void)irl_unget_char(p[--n]);
	efree((void *)p);

	q = Protect(lookup("__FILE__")->u.str);
	p = concat3("__FILE__ = \"", q, "\"; ");
	n = strlen(p);
	while (n > 0)
		(void)irl_unget_char(p[--n]);
	efree((void *)p);
	efree((void *)q);

	max_buffer = (size_t)0;
	buffer = (char *)egrow((void *)NULL, &max_buffer, sizeof(buffer[0]));

	n = (size_t)0;
	while ((c = (fgetc)(fin)) != EOF) /* NB: (fgetc)(), NOT irl_getchar()! */
	{
		if (n >= max_buffer)
			buffer = (char *)egrow((void *)buffer, &max_buffer, sizeof(buffer[0]));
		buffer[n++] = c;
	}

	while (n > 0)
		(void)irl_unget_char(buffer[--n]);
	efree((void *)buffer);
	set_filename(filename);
	(void)fclose(fin);
}

const char *
Load(const char *filename)
{
	FILE *old_fin;
	const char *old_current_filename;
	const char *new_filename;

	if (filename == (const char*)NULL) /* should never happen, but keeps lint happy */
		return (dupstr(""));

	if (!load_enabled)
		return (Msg_Translate("load() command is disabled"));

	old_fin = fin;
	old_current_filename = dupstr(get_string("__FILE__"));
	fin = path_fopen(getenv(HOCPATH), filename, &new_filename);
	if (fin == (FILE*)NULL)
	{
		(void)fprintf(stderr,  msg_translate("%s: can't open %s\n"),
			      progname, filename);
		fin = old_fin;
		efree((void*)new_filename);
		efree((void*)old_current_filename);
		return (Msg_Translate("load() open failed"));
	}
#if 0
	irl_push_input();
	set_filename(new_filename);
	efree((void*)new_filename);
	run();
	set_filename(old_current_filename);
	irl_pop_input();
#else
#if 0
	irl_push_input();
	set_filename(new_filename);
	efree((void*)new_filename);
	run2(filename);
	set_filename(old_current_filename);
	irl_pop_input();
#else
	run3(new_filename);
	efree((void*)new_filename);
#endif
#endif

	efree((void*)old_current_filename);
	fin = old_fin;
	return (dupstr(""));
}

const char *
Logfile(const char *filename)
{	/* close any open log file, open new one (error if it exists, for security reasons) */
	static const char *old_logfilename = "";

	if (filename == (char*)NULL)
		return (dupstr(""));

	if (!logfile_enabled)
		return (Msg_Translate("logfile() command is disabled"));

	if (FILE_EXISTS(filename) &&
	    (strcmp(filename,"/dev/tty") != 0) &&
	    (strcmp(filename,"") != 0))
		execerror("logfile() will not overwrite existing file", filename);

	if ((fplog != (FILE*)NULL) && (strcmp(old_logfilename,"") != 0))
		(void)fclose(fplog);

	if (STREQUAL(filename,""))
		fplog = stdout;
	else
		fplog = Fopen(filename, "w", (const char**)NULL);

	if (fplog == (FILE*)NULL)
		execerror("logfile() open failed", filename);

	old_logfilename = dupstr(filename);
	logfile_enabled = 1;
	return (dupstr(""));
}

const char *
Logoff(void)
{
	logfile_enabled = 0;
	return (dupstr(""));
}

const char *
Logon(void)
{
	logfile_enabled = 1;
	return (dupstr(""));
}

const char *
Msg_Translate(const char *msg)
{
	return (dupstr(msg_translate(msg)));
}

const char *
parentdir(const char * path)
{
	/* Return a dynamic copy of the parent directory of path,
	   EXCLUDING the directory separator.  If no directory
	   separator is found, the path is its own parent (on UNIX,
	   the root directory, "/"). */
	char * last;
	char * parent;
	size_t n;

	n = strlen(dirsep);

	parent = (char *)dupstr(path);
	for (last = strchr(parent,'\0') - n; last > parent; --last)
	{
		if (strncmp(last, dirsep, n) == 0)
		{
			*last = '\0';
			break;
		}
	}
	return (parent);
}

static FILE *
path_fopen(const char *path, const char *infile, const char **fullname)
{
	/* Search for infile on path, where path is a dirsep-separated
	   list of directories.  On return, *fullname has been set to
	   a dynamic copy of the full filename.  An empty directory
	   element is replaced by the default system path,
	   __SYSHOCPATH__, so that users who set HOCPATH do not need
	   to know what the system directories are: they can simply do
	   something like this:

		sh/bash/ksh syntax:	HOCPATH=.:$HOME/hoc: ; export HOCPATH
		csh/tcsh syntax:	setenv HOCPATH .:$HOME/hoc:
	*/

	const char *new_filename;
	FILE *fpin;

	new_filename = (const char*)NULL;
	if (is_absolute_filename(infile)) /* no need to search the path */
		fpin = Fopen(infile, "r", &new_filename);
	else
	{
		char *eod;		/* end of directory */
		char *eop;		/* end of path */
		char *dir;		/* start of next element in copy of path */
		char *org_dir;
		size_t ndirs;

		org_dir = dir = (char*)dupstr(path);
		eop = strchr(dir,'\0');
		fpin = (FILE*)NULL;
		for (ndirs = (size_t)0; (fpin == (FILE*)NULL) && (dir <= eop); dir = eod + strlen(elementsep))
		{
			eod = strstr(dir, elementsep);
			if (eod == (char *)NULL) /* no more elements: eod == eop */
				eod = strchr(dir,'\0');
			else		/* replace elementsep by NUL */
			{
				*eod = '\0';
				ndirs++;
			}
			if ((*dir == '\0') && (ndirs > 0)) /* empty element: recursive with default path */
				fpin = path_fopen(get_string("__SYSHOCPATH__"), infile, &new_filename);
			else		/* non-empty element, or empty path */
			{
				const char *q;
				/* avoid attaching do-nothing prefixes to keep __FILE__[*] values clean */
				if (STREQUAL(dir,"") || STREQUAL(dir,".") || STREQUAL(dir,"./"))
					q = dupstr(infile);
				else
					q = concat3(dir, dirsep, infile);
				fpin = Fopen(q, "r", &new_filename);
				efree((void*)q);
			}
			if (fpin == (FILE*)NULL)
			{
				efree((void*)new_filename);
				new_filename = (const char*)NULL;
			}
		}
		efree((void*)org_dir);
	}

	if (fpin == (FILE*)NULL)
		efree((void*)new_filename);

	if (fullname == (const char**)NULL)
		efree((void*)new_filename);
	else
		*fullname = new_filename;

	return (fpin);
}

const char *
Pwd(void)
{
	const char *cwd;

	if (!cd_pwd_enabled)
		return (Msg_Translate("pwd() command is disabled"));

	/* We prefer to use $PWD rather than getcwd(), because the
	   former returns the path seen by the user, while the latter
	   returns an equivalent path that often reflects
	   system-dependent file system mount locations: PWD's
	   /usr/local/share/lib/hoc becomes getcwd()'s
	   /export/home/disk/0031/share/lib/hoc */

	cwd = Getenv("PWD");
	if ((cwd == (const char*)NULL) || (*cwd == '\0'))
	{
		char *buf;

		efree((void *)cwd);
		buf = (char*)emalloc(PATH_MAX);
		cwd = dupstr(getcwd(buf, PATH_MAX));
		efree((void*)buf);
	}
	if (cwd == (const char*)NULL)
		execerror("pwd() failed",(const char*)NULL);

	return (cwd);
}

const char *
Save(const char *filename, const char *pattern)
{
	/* open new save file (error if it exists, for security reasons) */
	FILE *fpsave;

	if (filename == (char*)NULL)
		return (dupstr(""));

	if (!save_enabled)
		return (Msg_Translate("save() command is disabled"));

	if (FILE_EXISTS(filename) && (strcmp(filename,"/dev/tty") != 0))
		execerror("save() will not overwrite existing file", filename);
	if (STREQUAL(filename,""))
		fpsave = stdout;
	else
		fpsave = Fopen(filename, "w", (const char**)NULL);
	if (fpsave == (FILE*)NULL)
		execerror("save() open failed", filename);

	if (pattern == (const char*)NULL)
		pattern = "";

	(void)fprintf(fpsave, "### -*-hoc-*-\n");
	(void)fprintf(fpsave, "### ====================================================================\n");
	(void)fprintf(fpsave, msg_translate("### This is hoc save file %s\n"), filename);
	if (strcmp("*",pattern) != 0)
		(void)fprintf(fpsave,
			      msg_translate("### WARNING: This file contains only symbols matching \"%s\"\n"),
			      pattern);
	(void)fprintf(fpsave, msg_translate("### Creator:           %s version %s [%s]\n"),
		      PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_DATE);
	(void)fprintf(fpsave, msg_translate("### CreationDate:      %s\n"), Now());
	(void)fprintf(fpsave, msg_translate("### PackageBugReports: %s\n"), PACKAGE_BUGREPORT);
	(void)fprintf(fpsave, "### ====================================================================\n\n");
	save_symbols(fpsave,pattern);
	if (strcmp(filename,"") != 0)
		(void)fclose(fpsave);
	return (dupstr(""));
}

static void
save_symbols(FILE *fpsave, const char *pattern)
{
	Symbol **symtab;
	Symbol *sp;
	const char *p;
	fp_t d;
	int old_precision;
	size_t k;
	size_t max_symbols;
	size_t n_symtab;

	old_precision = get_precision();
	(void)set_precision(default_precision());

	max_symbols = 0;
	for (sp = first_symbol(); sp != (Symbol *)NULL; sp = next_symbol())
		max_symbols++;

	symtab = (Symbol**)emalloc(max_symbols * sizeof(Symbol*));

	/* First build a table of matching symbols that we can later sort */
	for (n_symtab = 0, sp = first_symbol(); sp != (Symbol *)NULL; sp = next_symbol())
	{
		if ((sp->name != (const char*)NULL) &&
		    *sp->name &&
		    (n_symtab < max_symbols) &&
		    !is_hidden(sp->name,pattern) &&
		    !is_local(sp->name) &&
#if 0
		    is_match(sp->name,pattern))
#else
		    match(sp->name,pattern))
#endif
			symtab[n_symtab++] = sp;
	}

	qsort(symtab, n_symtab, sizeof(Symbol*), cmpsym);
	for (k = 0; k < n_symtab; ++k)
	{
		if (strncmp("__",symtab[k]->name,2) == 0)
			continue;	/* never save internal or hidden symbols! */

		if (symtab[k]->builtin)
			continue;	/* never save builtin symbols! */

		switch(symtab[k]->type)
		{
		case BLTIN0:		/* FALL THROUGH */
		case BLTIN1:		/* FALL THROUGH */
		case BLTIN1I:		/* FALL THROUGH */
		case BLTIN1S:		/* FALL THROUGH */
		case BLTIN2:		/* FALL THROUGH */
		case BLTIN2S:		/* FALL THROUGH */
		case STRBLTIN0:		/* FALL THROUGH */
		case STRBLTIN1:		/* FALL THROUGH */
		case STRBLTIN1N:	/* FALL THROUGH */
		case STRBLTIN2:		/* FALL THROUGH */
		case STRBLTIN2SN:	/* FALL THROUGH */
		case STRBLTIN3SNN:
			break;		/* ignore all built-in functions and procedures */

		case NUMBER:		/* FALL THROUGH */
		case VAR:
			if (STREQUAL("_",symtab[k]->name))
				break;	/* skip _: last numeric result printed */
#if 0
			(void)fprintf(fpsave, "### DEBUG: name = %s\ttype = %d\n",
				      symtab[k]->name, symtab[k]->type);
#endif
			(void)fprintf(fpsave, "%-23s", symtab[k]->name);
			/* Special handling of PREC: we changed its value! */
			d = (STREQUAL("PREC", symtab[k]->name)) ?
				(fp_t)old_precision : symtab[k]->u.val;
			(void)fprintf(fpsave, "\t%s",(symtab[k]->immutable ? ":=" : "="));
			if (Nint(d) == d)
				(void)fprintf(fpsave, " %s\n", fmtnum(d));
			else
				(void)fprintf(fpsave, " %s\t# %s\n", xfptos(d), fmtnum(d));
			break;

		case STRING:		/* FALL THROUGH */
		case STRVAR:
#if 0
			(void)fprintf(fpsave, "### DEBUG: name = %s\ttype = %d\n",
				      symtab[k]->name, symtab[k]->type);
#endif
			(void)fprintf(fpsave, "%-23s", symtab[k]->name);
			p = Protect(symtab[k]->u.str);
			(void)fprintf(fpsave, "\t%s \"%s\"\n",
				      (symtab[k]->immutable ? ":=" : "="), p);
			efree((void*)p);
			break;

		case FUNCTION:		/* FALL THROUGH */
#if 0
			(void)fprintf(fpsave, "### DEBUG: name = %s\ttype = %d\n",
				      symtab[k]->name, symtab[k]->type);
			(void)fprintf(fpsave, "### NOT-YET-IMPLEMENTED: func %s(){...}\n",
				      symtab[k]->name);
#endif
			break;

		case PROCEDURE:
#if 0
			(void)fprintf(fpsave, "### DEBUG: name = %s\ttype = %d\n",
				      symtab[k]->name, symtab[k]->type);
			(void)fprintf(fpsave, "### NOT-YET-IMPLEMENTED: proc %s(){...}\n",
				      symtab[k]->name);
#endif
			break;

		default:
			break;
		}
	}

	(void)set_precision(old_precision);
}
