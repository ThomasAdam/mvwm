/* -*-c-*- */

#ifndef ADD_WINDOW_H
#define ADD_WINDOW_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

#define AW_NO_WINDOW NULL
#define AW_UNMANAGED ((void *)1)

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

void            setup_visible_name(FvwmWindow *, Bool);
void            setup_wm_hints(FvwmWindow *);
void            setup_snapping(FvwmWindow *, window_style *);
void            setup_placement_penalty(FvwmWindow *, window_style *);
void            setup_focus_policy(FvwmWindow *);
Bool            setup_transientfor(FvwmWindow *);
void            setup_icon_size_limits(FvwmWindow *, window_style *);
void            setup_icon_background_parameters(FvwmWindow *, window_style *);
void            setup_icon_title_parameters(FvwmWindow *, window_style *);
void            setup_numeric_vals(FvwmWindow *, window_style *);
Bool            validate_transientfor(FvwmWindow *);
void            setup_title_geometry(FvwmWindow *, window_style *);
void            setup_window_font(FvwmWindow *, window_style *, Bool);
void            setup_icon_font(FvwmWindow *, window_style *, Bool);
void            setup_style_and_decor(FvwmWindow *, window_style *, short *);
void            setup_frame_attributes(FvwmWindow *, window_style *);
void            change_auxiliary_windows(FvwmWindow *, short);
void            setup_frame_geometry(FvwmWindow *);
void            setup_frame_size_limits(FvwmWindow *, window_style *);
void            increase_icon_hint_count(FvwmWindow *);
void            change_icon(FvwmWindow *, window_style *);
void            change_mini_icon(FvwmWindow *, window_style *);
void            change_icon_boxes(FvwmWindow *, window_style *);
void            FetchWmProtocols(FvwmWindow *);
FvwmWindow     *AddWindow(const char **, const exec_context_t *, FvwmWindow *,
    initial_window_options_t *);
void            GetWindowSizeHints(FvwmWindow *);
void            free_window_names(FvwmWindow *, Bool, Bool);
void            destroy_window(FvwmWindow *);
void            RestoreWithdrawnLocation(FvwmWindow *, Bool, Window);
void            Reborder(void);
void            CaptureAllWindows(const exec_context_t *, Bool);

#endif /* ADD_WINDOW_H */
