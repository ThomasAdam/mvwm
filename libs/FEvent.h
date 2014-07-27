/* -*-c-*- */

#ifndef FEVENT_H
#define FEVENT_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions (privileged access) ----- */

#ifdef FEVENT_PRIVILEGED_ACCESS
void	 fev_copy_last_event(XEvent *);
XEvent	*fev_get_last_event_address(void);
#endif

/* ---------------------------- interface functions (normal_access) -------- */

/* get the latest event time */
Time	 fev_get_evtime(void);

/* This function determines the location of the mouse pointer from the event
 * if possible, if not it queries the X server. Returns False if it had to
 * query the server and the call failed because the pointer was on a
 * different screen. */
Bool	 fev_get_evpos_or_query(Display *, Window, const XEvent *, int *, int *);

/* Sets the x_root/y_root position in the given event if it's of the proper
 * type.  Returns True if the position was set. */
Bool	 fev_set_evpos(XEvent *, int, int);

/* announce a faked event to the FEvent module */
void	 fev_fake_event(XEvent *);

/* temporarily store the cached event in allocated memory */
void	*fev_save_event(void);

/* restore an event saved with fev_save_event and free the memory it uses */
void	 fev_restore_event(void *);

/* fill the event structure *ev with a dummy event of no particular type */
void	 fev_make_null_event(XEvent *, Display *);

/* return a copy of the last XEVent in *ev */
void	 fev_get_last_event(XEvent *);

/* ---------------------------- X event replacements ----------------------- */

/* Replacements for X functions */
XTimeCoord     *FGetMotionEvents(Display *, Window, Time, Time, int *);
int             FAllowEvents(Display *, int, Time);
Bool            FCheckIfEvent(Display *, XEvent *,
    Bool (*predicate) (Display *, XEvent *, XPointer), XPointer);
Bool            FCheckMaskEvent(Display *, long, XEvent *);
Bool            FCheckPeekIfEvent(Display *, XEvent *,
    Bool (*predicate) (Display *, XEvent *, XPointer), XPointer);
Bool            FCheckTypedEvent(Display *, int, XEvent *);
Bool            FCheckTypedWindowEvent(Display *, Window, int, XEvent *);
Bool            FCheckWindowEvent(Display *, Window, long, XEvent *);
int             FEventsQueued(Display *, int);
int             FIfEvent(Display *, XEvent *,
    Bool (*predicate) (Display *, XEvent *, XPointer), XPointer);
int             FMaskEvent(Display *, long, XEvent *);
int             FNextEvent(Display *, XEvent *);
int             FPeekEvent(Display *, XEvent *);
int             FPeekIfEvent(Display *, XEvent *,
    Bool (*predicate) (Display *, XEvent *, XPointer), XPointer);
int             FPending(Display *);
int             FPutBackEvent(Display *, XEvent *);
int             FQLength(Display *);
Bool            FQueryPointer(Display *, Window, Window *, Window *, int *,
    int *, int *, int *, unsigned int *);
int             FSelectInput(Display *, Window, long);
Status          FSendEvent(Display *, Window, Bool, long, XEvent *);
int             FWarpPointer(Display *, Window, Window, int, int, unsigned int,
    unsigned int, int, int);
int             FWarpPointerUpdateEvpos(XEvent *, Display *, Window, Window,
    int, int, unsigned int, unsigned int, int, int);
int             FWindowEvent(Display *, Window, long, XEvent *);

/* ---------------------------- disable X symbols -------------------------- */

/* FEVENT_C must only be defined in FEvent.c! */
#ifndef FEVENT_C
#define XGetMotionEvents(a, b, c, d, e) use_FGetMotionEvents
#define XCheckIfEvent(a, b, c, d) use_FCheckIfEvent
#define XCheckMaskEvent(a, b, c) use_FCheckMaskEvent
#define XCheckTypedEvent(a, b, c) use_FCheckTypedEvent
#define XCheckTypedWindowEvent(a, b, c, d) use_FCheckTypedWindowEvent
#define XCheckWindowEvent(a, b, c, d) use_FCheckWindowEvent
#define XEventsQueued(a, b) use_FEventsQueued
#define XIfEvent(a, b, c, d) use_FIfEvent
#define XMaskEvent(a, b, c) use_FMaskEvent
#define XNextEvent(a, b) use_FNextEvent
#define XPeekEvent(a, b) use_FPeekEvent
#define XPeekIfEvent(a, b, c, d) use_FPeekIfEvent
#define XPending(a) use_FPending
#define XPutBackEvent(a, b) use_FPutBackEvent
#define XQueryPointer(a, b, c, d, e, f, g, h, i) use_FQueryPointer
#define XQLength(a) use_FQLength
#define XSendEvent(a, b, c, d, e) use_FSendEvent
#define XWarpPointer(a, b, c, d, e, f, g, h, i) use_FWarpPointer
#define XWindowEvent(a, b, c, d) use_FWindowEvent
#endif

#endif /* FEVENT_H */
