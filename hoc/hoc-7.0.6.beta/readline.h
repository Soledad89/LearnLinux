#if !defined(READLINE_H)
#define READLINE_H

#undef EXTERN
#if defined(__cplusplus) || defined(c_plusplus)
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#include "fp_t.h"

#if defined(HAVE_READLINE_READLINE_H)
#include <readline/readline.h>
#endif

#if defined(HAVE_READLINE_HISTORY_H)
#include <readline/history.h>
#endif

/* Interface from readline.c to hoc.y */

extern int		irl_fd_stdin;

extern int		irl_fscanf ARGS((FILE *,const char *,void *));
extern int		irl_getchar ARGS((FILE *));
extern int		irl_unget_char ARGS((int));
extern void		irl_free ARGS((void));
extern void		irl_push_input ARGS((void));
extern void		irl_pop_input ARGS((void));

/* Interface from hoc.y to readline.c */

extern void		execerror ARGS((const char*, const char*));
extern const char*	get_prompt ARGS((void));
extern int		get_verbose ARGS((void));

/* Interface from symbol.c to readline.c */

extern const char *	dupstr ARGS((const char *));
extern const char *	first_symbol_pname ARGS((void));
extern const char *	next_symbol_pname ARGS((void));

#endif /* !defined(READLINE_H)) */
