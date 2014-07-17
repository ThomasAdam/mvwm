/* -*-c-*- */

#ifndef _ICONS_
#define _ICONS_

#ifdef NO_ICONS
#define ICON_HEIGHT(t) 1
#else
#define ICON_HEIGHT(t) \
	((t)->icon_font->height + 2*abs((t)->icon_title_relief))
#endif

int             get_visible_icon_window_count(FvwmWindow *);
void            clear_icon(FvwmWindow *);
void            setup_icon_title_size(FvwmWindow *);
void            GetIconPicture(FvwmWindow *, Bool);
void            AutoPlaceIcon(FvwmWindow *, initial_window_options_t *, Bool);
void            ChangeIconPixmap(FvwmWindow *);
void            RedoIconName(FvwmWindow *);
void            DrawIconWindow(FvwmWindow *, Bool, Bool, Bool, Bool, XEvent *);
void            CreateIconWindow(FvwmWindow *, int, int);
void            Iconify(FvwmWindow *, initial_window_options_t *);
void            DeIconify(FvwmWindow *);
void            SetMapStateProp(const FvwmWindow *, int);

#endif /* _ICONS_ */
