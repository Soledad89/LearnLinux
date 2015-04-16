BEGIN {
	for (k = 1; k <= 1000; ++k)
	{
		printf("x_%05d = %5d\n", k, k)
		if ((k % 200) == 0)
		{
			printf("\nprint \"Reached %5d symbols: x_%05d = \", x_%05d, \"\\n\"\n\n", k, k, k)
			print "who()\n"
		}
	}
}
