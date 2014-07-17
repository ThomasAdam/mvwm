/* -*-c-*- */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "libs/FScreen.h"

#define CS_ROUND_UP          0x01
#define CS_UPDATE_MAX_DEFECT 0x02

void            gravity_get_naked_geometry(int, FvwmWindow *, rectangle *,
    rectangle *);
void            gravity_add_decoration(int, FvwmWindow *, rectangle *,
    rectangle *);
void            get_relative_geometry(struct monitor *, rectangle *,
    rectangle *);
void            get_absolute_geometry(struct monitor *, rectangle *,
    rectangle *);
void            gravity_translate_to_northwest_geometry(int,
    FvwmWindow *, rectangle *, rectangle *);
void            gravity_translate_to_northwest_geometry_no_bw(int,
    FvwmWindow *, rectangle *, rectangle *);
void            get_title_geometry(FvwmWindow *, rectangle *);
int             get_title_gravity(FvwmWindow *);
void            get_title_gravity_factors(FvwmWindow *, int *, int *);
Bool            get_title_button_geometry(FvwmWindow *, rectangle *, int);
void            get_title_font_size_and_offset(FvwmWindow *, direction_t, Bool,
    Bool, Bool, Bool, int *, int *);
void            get_icon_corner(FvwmWindow *, rectangle *);
void            get_shaded_geometry(FvwmWindow *, rectangle *, rectangle *);
void            get_shaded_geometry_with_dir(FvwmWindow *, rectangle *,
    rectangle *, direction_t);
void            get_unshaded_geometry(FvwmWindow *, rectangle *);
void            get_shaded_client_window_pos(FvwmWindow *, rectangle *);
void            get_client_geometry(FvwmWindow *fw, rectangle *ret_g);
void            get_window_borders(const FvwmWindow *, size_borders *);
void            get_window_borders_no_title(const FvwmWindow *, size_borders *);
void            set_window_border_size(FvwmWindow *, int);
Bool            is_window_border_minimal(FvwmWindow *);
void            update_relative_geometry(FvwmWindow *);
void            update_absolute_geometry(FvwmWindow *);
void            maximize_adjust_offset(FvwmWindow *);
void            constrain_size(FvwmWindow *, const XEvent *, int *, int *, int,
    int, int);
void            gravity_constrain_size(int, FvwmWindow *, rectangle *, int);
Bool            get_visible_window_or_icon_geometry(FvwmWindow *, rectangle *);
Bool            get_visible_icon_geometry(FvwmWindow *, rectangle *);
void            get_icon_geometry(FvwmWindow *, rectangle *);
Bool            get_visible_icon_title_geometry(FvwmWindow *, rectangle *);
Bool            get_icon_title_geometry(FvwmWindow *, rectangle *);
Bool            get_visible_icon_picture_geometry(FvwmWindow *, rectangle *);
Bool            get_icon_picture_geometry(FvwmWindow *, rectangle *);
void            broadcast_icon_geometry(FvwmWindow *, Bool);
void            move_icon_to_position(FvwmWindow *);
void            modify_icon_position(FvwmWindow *, int, int);
void            set_icon_position(FvwmWindow *, int, int);
void            set_icon_picture_size(FvwmWindow *, int, int);
void            resize_icon_title_height(FvwmWindow *, int);
void            get_page_offset_rectangle(int *, int *, rectangle *);
void            get_page_offset(int *, int *, FvwmWindow *);
void            get_page_offset_check_visible(int *, int *, FvwmWindow *);

#endif
