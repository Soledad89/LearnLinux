#include <stdio.h>
#include <unistd.h>
#include <signal.h>		/* for SIG_ERR */

int main()
{
	printf("hello world from process ID %ld\n", (long)getid());
	return 0;
}

