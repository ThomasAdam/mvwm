/* -*-c-*- */

#ifndef FOCUS_H
#define FOCUS_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

#define FOCUS_SET(w, fw) _focus_set(w, fw)
#define FOCUS_RESET() _focus_reset()

#define SetFocusWindow(a, b, c) _SetFocusWindow(a, b, c, False);
#define SetFocusWindowClientEntered(a, b, c) _SetFocusWindow(a, b, c, True);
#define ReturnFocusWindow(a) _ReturnFocusWindow(a);
#define DeleteFocus(a) _DeleteFocus(a);
#define ForceDeleteFocus() _ForceDeleteFocus();

/* ---------------------------- type definitions --------------------------- */

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

/*
 * Triggers X protocol actions to set the focus to the given window.  It also
 * stores the FvwmWindow pointer to indicate that fvwm requested focus for that
 * FvwmWindow, not the application itself or someone else.
 */
void            _focus_set(Window, FvwmWindow *);
void            _focus_reset(void);

/*
 *
 * Sets/deletes the input focus to the indicated window.
 *
 */
void            _SetFocusWindow(FvwmWindow *, Bool, fpol_set_focus_by_t, Bool);
void            _ReturnFocusWindow(FvwmWindow *);
void            _DeleteFocus(Bool);
void            _ForceDeleteFocus(void);
void            restore_focus_after_unmap(const FvwmWindow *, Bool);

/*
 * These need documentation
 */
Bool            IsLastFocusSetByMouse(void);
void            focus_grab_buttons(FvwmWindow *);
void            focus_grab_buttons_client_entered(FvwmWindow *);
void            focus_grab_buttons_on_layer(int);
void            focus_grab_buttons_all(void);
void            focus_grab_buttons_on_pointer_window(void);
Bool            focus_does_accept_input_focus(const FvwmWindow *);
Bool            focus_is_focused(const FvwmWindow *);
Bool            focus_query_click_to_raise(FvwmWindow *, Bool, int);
Bool            focus_query_click_to_focus(FvwmWindow *, int);
Bool            focus_query_open_grab_focus(FvwmWindow *, FvwmWindow *);
Bool            focus_query_close_release_focus(const FvwmWindow *);
FvwmWindow     *focus_get_transientfor_fwin(const FvwmWindow *);

FvwmWindow     *get_focus_window(void);
void            set_focus_window(FvwmWindow *);
FvwmWindow     *get_last_screen_focus_window(void);
void            set_last_screen_focus_window(FvwmWindow *);
void            update_last_screen_focus_window(FvwmWindow *);
void            set_focus_model(FvwmWindow *);
void            focus_force_refresh_focus(const FvwmWindow *);
void            refresh_focus(const FvwmWindow *);

#endif /* FOCUS_H */
