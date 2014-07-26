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
 * stores the MvwmWindow pointer to indicate that mvwm requested focus for that
 * MvwmWindow, not the application itself or someone else.
 */
void _focus_set(Window w, MvwmWindow *fw);
void _focus_reset(void);

/*
 *
 * Sets/deletes the input focus to the indicated window.
 *
 */
void _SetFocusWindow(
	MvwmWindow *fw, Bool do_allow_force_broadcast,
	fpol_set_focus_by_t set_by, Bool client_entered);
void _ReturnFocusWindow(MvwmWindow *fw);
void _DeleteFocus(Bool do_allow_force_broadcast);
void _ForceDeleteFocus(void);
void restore_focus_after_unmap(
	const MvwmWindow *fw, Bool do_skip_marked_transients);

/*
 * These need documentation
 */
Bool IsLastFocusSetByMouse(void);
void focus_grab_buttons(MvwmWindow *fw);
void focus_grab_buttons_client_entered(MvwmWindow *fw);
void focus_grab_buttons_on_layer(int layer);
void focus_grab_buttons_all(void);
void focus_grab_buttons_on_pointer_window(void);
Bool focus_does_accept_input_focus(const MvwmWindow *fw);
Bool focus_is_focused(const MvwmWindow *fw);
Bool focus_query_click_to_raise(
	MvwmWindow *fw, Bool is_focused, int context);
Bool focus_query_click_to_focus(
	MvwmWindow *fw, int context);
Bool focus_query_open_grab_focus(MvwmWindow *fw, MvwmWindow *focus_win);
Bool focus_query_close_release_focus(const MvwmWindow *fw);
MvwmWindow *focus_get_transientfor_fwin(const MvwmWindow *fw);

MvwmWindow *get_focus_window(void);
void set_focus_window(MvwmWindow *fw);
MvwmWindow *get_last_screen_focus_window(void);
void set_last_screen_focus_window(MvwmWindow *fw);
void update_last_screen_focus_window(MvwmWindow *fw);
void set_focus_model(MvwmWindow *fw);
void focus_force_refresh_focus(const MvwmWindow *fw);
void refresh_focus(const MvwmWindow *fw);

#endif /* FOCUS_H */
