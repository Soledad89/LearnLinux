#include "hoc.h"

static unsigned long Rand_Seed = UL(100001); /* changeable by SetRandSeed(x) */

fp_t
Rand(void)
{
	/***************************************************************
	Random number generator - based on Algorithm 266 by Pike and
	Hill (modified by Hansson), Communications of the ACM, Vol. 8,
	No. 10, October 1965.

	This subprogram is intended for use on computers with fixed
	point wordlength of at least 29 bits.  It is best if the
	floating point significand has at most 29 bits.

	Following Cody and Waite's recommendation (p .14), we produce
	a pair of random numbers and use ran1 + 2**(-29)*ran2 in an
	attempt to generate about 58 random bits.
	***************************************************************/

	fp_t d;

	Rand_Seed = Rand_Seed * UL(125);
	Rand_Seed = Rand_Seed - (Rand_Seed / UL(2796203)) * UL(2796203);
	d = (fp_t)Rand_Seed / (fp_t)UL(2796203);

	Rand_Seed = Rand_Seed * UL(125);
	Rand_Seed = Rand_Seed - (Rand_Seed / UL(2796203)) * UL(2796203);
	d += (fp_t)Rand_Seed / (fp_t)UL(2796203) / (fp_t)UL(536870912);

	return (d);
}

fp_t
RandInt(fp_t x, fp_t y)
{
	/* Return a random integer in [x,y] */
	x = Integer(x);
	y = Integer(y);
	return (Integer(x + Rand()*(y - x + 1)));
}

fp_t
Randl(fp_t x)
{
	/***************************************************************
	Return a pseudo random number logarithmically distributed over
	(1,exp(x)).  Thus a*Randl(ln(b/a)) is logarithmically
	distributed in (a,b).
	***************************************************************/
	return (Exp(x * Rand()));
}

fp_t
SetRandSeed(fp_t new_seed)
{
	/* Set the random number generator seed to new_seed, which
	should be a large integer.  If new_seed is zero, then the time
	calendar, or process id, or a modification of the current
	seed, are used to try to get a new, and unpredicatable,
	seed.  The return value is the old seed. */

	unsigned long old_seed;

	old_seed = Rand_Seed;

	/* NB: it is important to multiply the `random' integer from
	time(), getpid(), or Rand_Seed by Rand(), so that repeated
	calls to this function will produce different seeds. */

#if defined(HAVE_TIME_H)
	Rand_Seed = (unsigned long)((new_seed == FP(0.0)) ? Rand() * (unsigned long)Systime() : Abs(new_seed));
#elif defined(HAVE_UNISTD_H)
	Rand_Seed = (unsigned long)((new_seed == FP(0.0)) ? Rand() * ((unsigned long)getpid() << 15) : Abs(new_seed));
#else
	Rand_Seed = (unsigned long)((new_seed == FP(0.0)) ? Rand() * (Rand_Seed >> 4)*15L : Abs(new_seed));
#endif

	/* If the user calls setseed(systime()) in quick succession,
	   the first random number produced by rand() is quite
	   similar, but remaining ones are not, so cycle the generator
	   once. */
	(void)Rand();

	return ((fp_t)old_seed);
}
