/* -*-c-*- */

#ifndef _VIRTUAL_
#define _VIRTUAL_

#include "libs/FScreen.h"

int             HandlePaging(XEvent *, int, int, int *, int *, int *, int *,
    Bool, Bool, Bool, int);
void            checkPanFrames(void);
void            raisePanFrames(void);
void            initPanFrames(void);
Bool            is_pan_frame(Window);
void            MoveViewport(struct monitor *, int, int, Bool);
void            goto_desk(int, struct monitor *);
void            do_move_window_to_desk(FvwmWindow *, int);
Bool            get_page_arguments(struct monitor *, char *, int *, int *y);
char           *GetDesktopName(struct monitor *, int);

#endif /* _VIRTUAL_ */
