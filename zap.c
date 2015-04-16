/* zap: interactive process killer */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

char *progname;
char *ps = "ps -a";

int main(int argc, char * argv[])
{
	FILE *fin, *popen();
	char buf[BUFSIZ];
	int pid;

	progname = argv[0];
	if((fin = popen(ps,"r")) == NULL)
	{
		fprintf(stderr, "%s: can't run %s\n", progname, ps);
		exit(1);
	}

	fgets(buf, sizeof buf, fin);			/* get headerline */
	fprintf(stderr, "%s",buf);
	while (fgets(buf, sizeof buf, fin ) != NULL)
		if (argc == 1 || strindex(buf, argv[1]) >= 0)
		{
			buf[strlen(buf)-1] = '\0';		/* suppress \n */
			fprintf(stderr, "%s?",buf);
			if (ttyin() == 'y')
			{
				sscanf(buf, "%d", &pid);
				kill(pid, SIGKILL);
			}
		}
	exit(0);
}


