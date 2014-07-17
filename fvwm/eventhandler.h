/* -*-c-*- */

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

typedef struct
{
	const exec_context_t *exc;
} evh_args_t;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

/* ---------------------------- event handlers ----------------------------- */

void            HandleEvents(void);
void            HandleExpose(const evh_args_t *);
void            HandleFocusIn(const evh_args_t *);
void            HandleFocusOut(const evh_args_t *);
void            HandleDestroyNotify(const evh_args_t *);
void            HandleMapRequest(const evh_args_t *);
void            HandleMapRequestKeepRaised(const evh_args_t *,
    Window, FvwmWindow *,initial_window_options_t *);
void            HandleMapNotify(const evh_args_t *);
void            HandleUnmapNotify(const evh_args_t *);
void            HandleMotionNotify(const evh_args_t *);
void            HandleButtonRelease(const evh_args_t *);
void            HandleButtonPress(const evh_args_t *);
void            HandleEnterNotify(const evh_args_t *);
void            HandleLeaveNotify(const evh_args_t *);
void            HandleConfigureRequest(const evh_args_t *);
void            HandleClientMessage(const evh_args_t *);
void            HandlePropertyNotify(const evh_args_t *);
void            HandleKeyPress(const evh_args_t *);
void            HandleKeyRelease(const evh_args_t *);
void            HandleVisibilityNotify(const evh_args_t *);
#endif /* EVENTHANDLER_H */
