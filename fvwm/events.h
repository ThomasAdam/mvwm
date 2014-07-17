/* -*-c-*- */

#ifndef EVENTS_H
#define EVENTS_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

typedef struct
{
	Window          w;
	int             event_type;
	Atom            atom;
} test_typed_window_event_args;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

void            dispatch_event(XEvent *);
int             GetContext(FvwmWindow **, FvwmWindow *, const XEvent *,
    Window *w);
int             My_XNextEvent(Display *, XEvent *);
int             flush_expose(Window);
int             flush_accumulate_expose(Window, XEvent *);
void            handle_all_expose(void);
Bool            StashEventTime(const XEvent *);
void            CoerceEnterNotifyOnCurrentWindow(void);
void            InitEventHandlerJumpTable(void);
void            SendConfigureNotify(FvwmWindow *, int, int, int, int, int, Bool);
void            WaitForButtonsUp(Bool);
int             discard_events(long);
int             discard_window_events(Window, long);
int             flush_property_notify(Atom, Window);
void            sync_server(int);
Bool            is_resizing_event_pending(FvwmWindow *);
void            events_handle_configure_request(XConfigureRequestEvent,
    FvwmWindow *, Bool, int);
Bool            test_button_event(Display *, XEvent *, char *);
Bool            test_typed_window_event(Display *, XEvent *, char *);

#endif /* EVENTS_H */
