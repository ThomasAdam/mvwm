/* -*-c-*- */

#ifndef _ICONS_
#define _ICONS_

#ifdef NO_ICONS
#define ICON_HEIGHT(t) 1
#else
#define ICON_HEIGHT(t) \
	((t)->icon_font->height + 2*abs((t)->icon_title_relief))
#endif

int             get_visible_icon_window_count(MvwmWindow *fw);
void            clear_icon(MvwmWindow *fw);
void            setup_icon_title_size(MvwmWindow *fw);
void            GetIconPicture(MvwmWindow *fw, Bool no_icon_window);
void            AutoPlaceIcon(MvwmWindow *t,
    initial_window_options_t *win_opts, Bool do_move_immediately);
void            ChangeIconPixmap(MvwmWindow *fw);
void            RedoIconName(MvwmWindow *fw);
void            DrawIconWindow(MvwmWindow *fw, Bool draw_title,
    Bool draw_pixmap, Bool focus_change, Bool reset_bg, XEvent *pev);
void            CreateIconWindow(MvwmWindow *fw, int def_x, int def_y);
void            Iconify(MvwmWindow *fw, initial_window_options_t *win_opts);
void            DeIconify(MvwmWindow *);
void            SetMapStateProp(const MvwmWindow *, int);

#endif /* _ICONS_ */
