#if !defined(STDC_H)
#define STDC_H

#if defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#define ARGS(plist) plist
#define VOID_ARG	void
#define VOIDP		void*
#define STDC
#else
#define ARGS(plist) ()
#define VOID_ARG	
#define VOIDP		char*
#endif

#if defined(c_plusplus) && !defined(__cplusplus)
#define __cplusplus 1			/* want modern-C++-style identifier */
#endif

#endif /* !defined(STDC_H) */
