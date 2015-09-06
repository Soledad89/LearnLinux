#include <unistd.h>

int main()
{
	char string[5] = {'H', 'E', 'L', 'L','O'};
	write(1, string, 6);
	return 0;
}

