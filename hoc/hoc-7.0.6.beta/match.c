#include "config.h"
#include "stdc.h"

#if defined(HAVE_STDDEF_H)
#include <stddef.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

int		match ARGS((const char *s, const char *pattern));
static int	match_any ARGS((const char *s, const char *pattern));
static int	match_range ARGS((char c, const char *pattern, const char **plast));

/*
 ***********************************************************************
 * match(s,pattern)
 *
 * Return 1 if there is a UNIX-shell-style `glob' match pattern with s,
 * and 0 otherwise.
 *
 * An asterisk, *, in pattern matches zero or more (non-NUL) characters
 * in s.
 *
 * A query, ?, in pattern matches exactly one of any (non-NUL) character
 * in s.
 *
 * A square-bracketed list of characters matches any character in that
 * list.  In the list, a right bracket occurring first is a normal
 * character (i.e., does not end the list), and a hyphen-separated
 * pair represents a range.  Thus, "[]]" matches a right bracket, and
 * "[a-zA-Z0-9]" matches an English letter or a decimal digit.  If the
 * closing right bracket is missing, the pattern is considered to not
 * match anything.
 *
 ***********************************************************************
 */

int
match(const char *s, const char *pattern)
{
	const char *p;

	if (s == pattern)		/* equal strings, or both NULL pointers */
		return (1);
	else if (s == (const char*)NULL)
		return (0);
	else if (pattern == (const char*)NULL)
		return (0);

	for (p = pattern; *p && *s; ++p, ++s)
	{
		if (*p == '*')		/* match zero or more characters */
		{
			while (*p == '*') /* optimize runs of asterisks */
				++p;
			return (match_any(s,p));
		}
		else if (*p == '?')	/* match any single character */
			continue;
		else if (*p == '[')	/* range match required */
		{
			const char *last;

			if (match_range(*s, p, &last) && (last != (const char*)NULL))
			{
				p = last;
				continue;
			}
			return (0);
		}
		else if (*p != *s)	/* exact match required */
			return (0);
	}
	for ( ; (*s == '\0') && (*p == '*') ; ++p) /* handle match("XYZ","XYZ*****") */
		continue;
	return ((*p == '\0') && (*s == '\0'));
}


static int
match_any(const char *s, const char *pattern)
{
	if (*pattern == '\0')
		return (1);
	for ( ; *s && *pattern; ++s)
	{
		if (match(s, pattern))
			return (1);
	}
	return (0);
}

/*
 ***********************************************************************
 * Match c against pattern, which is expected to be a square-bracketed
 * list of character and/or hyphen-separated character ranges.
 *
 * The return value is 1 for a match, and 0 for a mismatch or a NULL
 * *plast.
 *
 * On return, *plast is set to point to the last character in the
 * pattern, that is, the closing right bracket.  If the pattern is ill
 * formed, *plast is set to NULL.
 ***********************************************************************
 */

static int
match_range(char c, const char *pattern, const char **plast)
{
	if (plast == (const char**)NULL)
		return (0);

	*plast = (const char*)NULL;

	if (pattern == (const char*)NULL)
		return (0);
	else if (pattern[0] != '[')
		return (0);
	else			/* (pattern[0] == '['): start of range */
	{
		++pattern;	/* move past open bracket */
		*plast = strchr(pattern+1,']');
		if (*plast == (const char*)NULL) /* unclosed range */
			return (0);
		while (pattern < *plast)
		{
			if (pattern[1] == '-') /* expect range, like A-Z */
			{
				if ((pattern[0] <= c) && (c <= pattern[2]))
					return (1);
				pattern += 2;
			}
			else if (pattern[0] == c) /* expect exact match */
				return (1);
			pattern++;
		}
		return (0);
	}
}


#if defined(TEST)
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char* argv[])
{
	size_t k;

	for (k = 2; k < argc; ++k)
		(void)printf("%u: [%s] matches [%s]: %s\n",
			     k,
			     argv[1],
			     argv[k],
			     match(argv[1],argv[k]) ? "yes" : "no");

	return (EXIT_SUCCESS);
}
#endif /* defined(TEST) */
