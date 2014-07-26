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

void            setup_visible_name(MvwmWindow *fw, Bool is_icon);
void            setup_wm_hints(MvwmWindow *fw);
void            setup_snapping(MvwmWindow *fw, window_style *pstyle);
void            setup_placement_penalty(MvwmWindow *fw, window_style *pstyle);
void            setup_focus_policy(MvwmWindow *fw);
Bool            setup_transientfor(MvwmWindow *fw);
void            setup_icon_size_limits(MvwmWindow *fw, window_style *pstyle);
void            setup_icon_background_parameters(MvwmWindow *fw,
    window_style *pstyle);
void            setup_icon_title_parameters(MvwmWindow *fw,
    window_style *pstyle);
void            setup_numeric_vals(MvwmWindow *fw, window_style *pstyle);
Bool            validate_transientfor(MvwmWindow *fw);
void            setup_title_geometry(MvwmWindow *fw, window_style *pstyle);
void            setup_window_font(MvwmWindow *fw, window_style *pstyle,
    Bool do_destroy);
void            setup_icon_font(MvwmWindow *fw, window_style *pstyle,
    Bool do_destroy);
void            setup_style_and_decor(MvwmWindow *fw, window_style *pstyle,
    short *buttons);
void            setup_frame_attributes(MvwmWindow *fw, window_style *pstyle);
void            change_auxiliary_windows(MvwmWindow *fw, short buttons);
void            setup_frame_geometry(MvwmWindow *fw);
void            setup_frame_size_limits(MvwmWindow *fw, window_style *pstyle);
void            increase_icon_hint_count(MvwmWindow *fw);
void            change_icon(MvwmWindow *fw, window_style *pstyle);
void            change_mini_icon(MvwmWindow *fw, window_style *pstyle);
void            change_icon_boxes(MvwmWindow *fw, window_style *pstyle);
void            FetchWmProtocols(MvwmWindow *);
MvwmWindow     *AddWindow(const char **ret_initial_map_command,
    const exec_context_t *exc, MvwmWindow *ReuseWin,
    initial_window_options_t *win_opts);
void            GetWindowSizeHints(MvwmWindow *);
void            free_window_names(MvwmWindow *tmp, Bool nukename,
    Bool nukeicon);
void            destroy_window(MvwmWindow *);
void            RestoreWithdrawnLocation(MvwmWindow *tmp,
    Bool is_restart_or_recapture, Window parent);
void            Reborder(void);
void            CaptureAllWindows(const exec_context_t *exc,
    Bool is_recapture);

#endif /* ADD_WINDOW_H */
