/* -*-c-*- */

/* Description:
 *       exports from session.c shall go into this file
 *
 * Created:
 *       4 April 1999 - Steve Robbins <steve@nyongwa.montreal.qc.ca>
 */

#ifndef SESSION_H
#define SESSION_H

/*
**  Load and save the 'global', ie not window-related, state of fvwm
**  into a file.
*/
void            LoadGlobalState(char *);

/*
**  Turn off SM for new windows
*/
void            DisableRestoringState(void);

/*
**  Load and save window states.
*/
void            LoadWindowStates(char *);

/*
**  Fill in the FvwmWindow struct with information saved from
**  the last session. This expects the fields
**    t->w
**    t->name
**    t->class
**    t->tmpflags.NameChanged
**  to have meaningful values. The shade and maximize flags are set
**  if the window should start out as shaded or maximized, respecively.
**  The dimensions returned in x, y, w, h should be used when the
**  window is to be maximized.
*/
typedef struct
{
	int             shade_dir;
	unsigned        do_shade:1;
	unsigned        used_title_dir_for_shading:1;
	unsigned        do_max:1;
} mwtsm_state_args;

Bool            MatchWinToSM(FvwmWindow *, mwtsm_state_args *,
    initial_window_options_t *);
#endif
