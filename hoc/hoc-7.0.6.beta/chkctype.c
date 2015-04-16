/***********************************************************************
Display the list of characters that are letters in the current locale.

[06-Dec-2001]
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

int
main(void)
{
	int k, n;
	
	(void)setlocale(LC_ALL, "");

	(void)printf("Digit characters according to isdigit()\n");
	for (k = 0, n = 0; k < 256; ++k)
	{
		if (isdigit(k))		
		{
			if (n == 32)
				((void)printf("\n"), n = 0);
			if (n == 0)
				(void)printf("\t");
			(void)printf(" %c", (char)k);
			n++;
		}
	}
	(void)printf("\n\n");

	(void)printf("Alphabetic characters according to isalpha()\n");
	for (k = 0, n = 0; k < 256; ++k)
	{
		if (isalpha(k))		
		{
			if (n == 32)
				((void)printf("\n"), n = 0);
			if (n == 0)
				(void)printf("\t");
			(void)printf(" %c", (char)k);
			n++;
		}
	}
	(void)printf("\n\n");

	(void)printf("Alphanumeric characters according to isalnum()\n");
	for (k = 0, n = 0; k < 256; ++k)
	{
		if (isalnum(k))		
		{
			if (n == 32)
				((void)printf("\n"), n = 0);
			if (n == 0)
				(void)printf("\t");
			(void)printf(" %c", (char)k);
			n++;
		}
	}
	(void)printf("\n\n");

	(void)printf("Lowercase characters according to islower()\n");
	for (k = 0, n = 0; k < 256; ++k)
	{
		if (islower(k))		
		{
			if (n == 32)
				((void)printf("\n"), n = 0);
			if (n == 0)
				(void)printf("\t");
			(void)printf(" %c", (char)k);
			n++;
		}
	}
	(void)printf("\n\n");

	(void)printf("Uppercase characters according to isupper()\n");
	for (k = 0, n = 0; k < 256; ++k)
	{
		if (isupper(k))		
		{
			if (n == 32)
				((void)printf("\n"), n = 0);
			if (n == 0)
				(void)printf("\t");
			(void)printf(" %c", (char)k);
			n++;
		}
	}
	(void)printf("\n\n");

	return (EXIT_SUCCESS);
}
