/* -*-c-*- */

#ifndef FRAME_H
#define FRAME_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

typedef enum
{
	FRAME_MR_SETUP,
	FRAME_MR_SETUP_BY_APP,
	FRAME_MR_OPAQUE,
	FRAME_MR_SHRINK,
	FRAME_MR_SCROLL,
	/*
	 * used internally only, do not set these in any calls
	 */
	FRAME_MR_FORCE_SETUP,
	FRAME_MR_FORCE_SETUP_NO_W,
	/*
	 * used by AddWindow() to suppress drawing during window setup
	 */
	FRAME_MR_DONT_DRAW = 0x80
} frame_move_resize_mode;

typedef struct
{
	rectangle       title_g;
	rectangle       button_g[NUMBER_OF_TITLE_BUTTONS];
} frame_title_layout_t;

/* details are hidden in frame.c */
typedef void   *frame_move_resize_args;

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

void            frame_init(void);
Bool            is_frame_hide_window(Window);
void            frame_destroyed_frame(Window);
frame_move_resize_args frame_create_move_resize_args(FvwmWindow *,
    frame_move_resize_mode, rectangle *, rectangle *, int, int);
void            frame_update_move_resize_args(frame_move_resize_args,
    rectangle *);
void            frame_free_move_resize_args(FvwmWindow *,
    frame_move_resize_args);
void            frame_get_titlebar_dimensions(FvwmWindow *,
    rectangle *, rectangle *, frame_title_layout_t *);
void            frame_get_sidebar_geometry(FvwmWindow *,
    DecorFaceStyle *, rectangle *, rectangle *, Bool *, Bool *);
int             frame_window_id_to_context(FvwmWindow *, Window, int *);
void            frame_move_resize(FvwmWindow *, frame_move_resize_args);
void            frame_setup_window(FvwmWindow *, int, int, int, int, Bool);
void            frame_setup_window_app_request(FvwmWindow *, int, int,
    int, int, Bool);
void            frame_force_setup_window(FvwmWindow *, int, int, int, int,
    Bool);
void            frame_setup_shape(FvwmWindow *, int, int, int);

#endif /* FRAME_H */
