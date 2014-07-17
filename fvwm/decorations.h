/* -*-c-*- */

#ifndef DECORATIONS_H
#define DECORATIONS_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

typedef enum
{
	RQORIG_PROGRAM = False,
	RQORIG_PROGRAM_US = True,
	RQORIG_MODULE_INPUT,
	RQORIG_FVWM_USER,
} request_origin_t;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

void            GetMwmHints(FvwmWindow *);
void            GetOlHints(FvwmWindow *);
void            SelectDecor(FvwmWindow *, window_style *, short *);
Bool            is_function_allowed(int, char *,  const FvwmWindow *,
    request_origin_t, Bool);

#endif /* DECORATIONS_H */
