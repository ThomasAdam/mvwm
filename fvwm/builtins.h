/* -*-c-*- */

#ifndef BUILTINS_H
#define BUILTINS_H

void            refresh_window(Window, Bool);
void            ApplyDefaultFontAndColors(void);
void            InitFvwmDecor(FvwmDecor *);
void            reset_decor_changes(void);
Bool            ReadDecorFace(char *, DecorFace *, int, int);
void            FreeDecorFace(Display *, DecorFace *);
void            update_fvwm_colorset(int);

#endif /* BUILTINS_H */
