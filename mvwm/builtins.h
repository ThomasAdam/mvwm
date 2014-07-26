/* -*-c-*- */

#ifndef BUILTINS_H
#define BUILTINS_H

void            refresh_window(Window w, Bool window_update);
void            ApplyDefaultFontAndColors(void);
void            InitMvwmDecor(MvwmDecor * decor);
void            reset_decor_changes(void);
Bool            ReadDecorFace(char *s, DecorFace *df, int button,
    int verbose);
void            FreeDecorFace(Display *dpy, DecorFace *df);
void            update_mvwm_colorset(int cset);

#endif /* BUILTINS_H */
