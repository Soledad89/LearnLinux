/***********************************************************************
Check the action of strtod(), which is sensitive to locale environment
variables.  This programs set LC_NUMERIC to "C", which should ensure
that strtod() and printf() perform identically, independent of the
LC_ALL or LANG environment variables.

Usage:
	./chkstrtod
	env LANG=xxx ./chkstrtod

[14-Dec-2001]
***********************************************************************/

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
    char line[256];
    char *endptr;
    char *lang;
    double d;

    lang = getenv("LANG");
    if (lang == (char*)NULL) lang = "C";

    (void)setlocale(LC_ALL, lang);
    (void)setlocale(LC_NUMERIC, "C");

    while (fgets(line,sizeof(line),stdin) != (char*)NULL)
    {
	    *strchr(line,'\n') = '\0';
	    d = strtod(line,&endptr);
	    (void)printf("strtod(%s) -> %lg\trest = [%s]\n",
			 line, d, (endptr == NULL) ? "" : endptr);
    }

    return (EXIT_SUCCESS);
}
