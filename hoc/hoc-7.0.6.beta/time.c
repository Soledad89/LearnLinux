#include "hoc.h"

#if defined(HAVE_LOCALE_H)
#include <locale.h>
#else
#define setlocale(lc_xxx, value) 0
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>			/* needed by Apple Darwin (MacOS X) */
#endif

#if defined(HAVE_SYS_RESOURCE_H)
#include <sys/resource.h>
#endif

#if defined(HAVE_SIGNAL_H)
#include <signal.h>
#endif

#if defined(HAVE_TIME_H)
#include <time.h>
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" void xcpucatch ARGS((int dummy));
#else
void xcpucatch ARGS((int dummy));
#endif

fp_t
CPULimit(fp_t seconds)
{
	/* Allow up to `seconds' more time in the current process,
	   and return the previous soft limit on CPU time use. */

	fp_t old_seconds;

	(void)update_number("__CPU_LIMIT__", seconds);

#if defined(HAVE_SYS_RESOURCE_H) && defined(SIGXCPU)
	{
		struct rlimit rlim;
		struct rusage ruse;

		if (getrlimit(RLIMIT_CPU, &rlim) != 0)
			return (NaN());
		if (getrusage(RUSAGE_SELF, &ruse) != 0)
			return (NaN());
		old_seconds = rlim.rlim_cur;

		if (IsNaN(seconds))
			seconds = (fp_t)RLIM_INFINITY;
		else if (IsInf(seconds))
			seconds = (fp_t)RLIM_INFINITY;
		else if (seconds <= FP(0.0))
			seconds = (fp_t)RLIM_INFINITY;

		if (seconds > (fp_t)RLIM_INFINITY)
			seconds = (fp_t)RLIM_INFINITY;

		seconds = Ceil(seconds
			       + (fp_t)ruse.ru_utime.tv_sec
			       + (fp_t)ruse.ru_utime.tv_usec / FP(1000000.0)
			       + (fp_t)ruse.ru_stime.tv_sec
			       + (fp_t)ruse.ru_stime.tv_usec / FP(1000000.0));

		if (seconds > (fp_t)RLIM_INFINITY)
			seconds = (fp_t)RLIM_INFINITY;

		rlim.rlim_cur = (rlim_t)seconds;

		/* On Apple Darwin, with seconds == Inf, that last
		   assignment wraps to a large negative value, so
		   guard against such nonsense.  Test against 1,
		   rather than 0, to avoid warnings from compilers on
		   systems where rlim_t is an unsigned type. */

		if ((rlim.rlim_cur < (rlim_t)1) || (rlim.rlim_cur > rlim.rlim_max))
			rlim.rlim_cur = rlim.rlim_max;

		if (setrlimit(RLIMIT_CPU, &rlim) != 0)
			old_seconds = NaN();
		(void)signal(SIGXCPU, xcpucatch);
	}
#else
	old_seconds = Infinity();
#endif

	return (old_seconds);
}

const char *
Now(void)
{
	time_t	timeval;
	char	timestring[26];		/* "Thu Dec 13 18:05:48 2001" */

	timeval = time((time_t*)NULL);
	(void)strlcpy(timestring,ctime(&timeval),sizeof(timestring));
	timestring[24] = '\0';		/* clobber unwanted final newline */
	return (dupstr((const char *)&timestring[0]));
}

fp_t
Second(void)				/* CPU seconds since job start */
{
#if defined(HAVE_TIME_H)
	return ((fp_t)clock()/(fp_t)CLOCKS_PER_SEC);
#else
	return (FP(0.0));
#endif
}

const char *
Strftime(const char *fmt, fp_t t)
{	/* return a dynamic string containing the conversion of t according to strftime format fmt */
	time_t tv;
	static char sbuf[256];
	const char *envloc;

	envloc = Getenv("LC_TIME");
	(void)setlocale(LC_TIME, (*envloc) ? envloc : "C");
	tv = (time_t)t;
	(void)strftime(sbuf, sizeof(sbuf), fmt, localtime(&tv));
	efree((void*)envloc);
	return (dupstr((const char *)&sbuf[0]));
}

fp_t
Systime(void)				/* wall clock seconds since 1970.01.01 00:00:00 UTC */
{
#if defined(HAVE_TIME_H)
	return ((fp_t)time((time_t *)NULL));
#else
	return (FP(0.0));
#endif
}

void
xcpucatch(int dummy)	/* catch SIGXCPU (CPU time exceeded) exceptions */
{
	execerror("CPU time exceeded",  (const char*)NULL);
}
