/* -*-c-*- */

#ifndef PLACEMENT_H
#define PLACEMENT_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

typedef enum
{
	PLACE_INITIAL,
	PLACE_AGAIN
} placement_mode_t;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

extern const pl_penalty_struct default_pl_penalty;
extern const pl_percent_penalty_struct default_pl_percent_penalty;

/* ---------------------------- interface functions ------------------------ */

Bool            setup_window_placement(FvwmWindow *, window_style *,
    rectangle *, initial_window_options_t *, placement_mode_t);

#endif /* PLACEMENT_H */
