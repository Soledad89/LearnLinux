#include "hoc.h"

#if defined(DEBUG_MACHEPS)
extern void Dump(const char *, fp_t);
#endif


fp_t
store(fp_t *x)
{
	/* This external function is used to work around problems on
	   architectures, such as HP/Intel IA-64, Intel IA-32 (== x86),
	   and Motorola 68K, with registers that are longer than memory
	   words.  It must NEVER be inlined, since its purpose is to
	   shorten its argument to the size, and precision and range, of
	   a memory word. */
#if defined(DEBUG_MACHEPS)
	Dump("store(): x = ", *x);
#endif
	return (*x);
}
