#if !defined(FP_ARCH_H)
#define FP_ARCH_H

/* In order to isolate architecture-dependent code sections from
   compiler-dependent variants of preprocessor symbol settings, and
   simplify preprocessor conditional expressions, we create our own
   private FP_ARCH_xxx symbols. */

#define FP_ARCH_UNKNOWN	1

#if defined(__alpha)
#define FP_ARCH_ALPHA	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(__i860)
#define FP_ARCH_I860	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(__i386) || defined(__i486) || defined(__i586) || defined(__i686) || \
	defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
#define FP_ARCH_IA32	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(__ia64)
#define FP_ARCH_IA64	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(__mips)
#define FP_ARCH_MIPS	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(__hppa)
#define FP_ARCH_PA_RISC	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(_POWER) || defined(__PPC) || defined(__powerpc) || defined(__APPLE__)
#define FP_ARCH_POWER	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(__sparc) || defined(__sparc__)
#define FP_ARCH_SPARC	1
#undef FP_ARCH_UNKNOWN
#endif

#if defined(FP_ARCH_UNKNOWN)
#error "Floating-point architecture cannot be determined on this system."
#endif

#endif /* !defined(FP_ARCH_H) */
