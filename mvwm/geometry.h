/* -*-c-*- */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "libs/FScreen.h"

#define CS_ROUND_UP          0x01
#define CS_UPDATE_MAX_DEFECT 0x02

void            gravity_get_naked_geometry(int gravity, MvwmWindow *t,
    rectangle *dest_g, rectangle *orig_g);
void            gravity_add_decoration(int gravity, MvwmWindow *t,
    rectangle *dest_g, rectangle *orig_g);
void            get_relative_geometry(struct monitor *, rectangle *rel_g,
    rectangle *abs_g);
void            get_absolute_geometry(struct monitor *, rectangle *abs_g,
    rectangle *rel_g);
void            gravity_translate_to_northwest_geometry(int gravity,
    MvwmWindow *t, rectangle *dest_g, rectangle *orig_g);
void            gravity_translate_to_northwest_geometry_no_bw(int gravity,
    MvwmWindow *t, rectangle *dest_g, rectangle *orig_g);
void            get_title_geometry(MvwmWindow *fw, rectangle *ret_g);
int             get_title_gravity(MvwmWindow *fw);
void            get_title_gravity_factors(MvwmWindow *fw, int *ret_fx,
    int *ret_fy);
Bool            get_title_button_geometry(MvwmWindow *fw, rectangle *ret_g,
    int context);
void            get_title_font_size_and_offset(MvwmWindow *fw,
    direction_t title_dir, Bool is_left_title_rotated_cw,
    Bool is_right_title_rotated_cw, Bool is_top_title_rotated,
    Bool is_bottom_title_rotated, int *size, int *offset);
void            get_icon_corner(MvwmWindow *fw, rectangle *ret_g);
void            get_shaded_geometry(MvwmWindow *fw, rectangle *small_g,
    rectangle *big_g);
void            get_shaded_geometry_with_dir(MvwmWindow *fw,
    rectangle *small_g, rectangle *big_g, direction_t shade_dir);
void            get_unshaded_geometry(MvwmWindow *fw, rectangle *ret_g);
void            get_shaded_client_window_pos(MvwmWindow *fw,
    rectangle *ret_g);
void            get_client_geometry(MvwmWindow *fw, rectangle *ret_g);
void            get_window_borders(const MvwmWindow *fw,
    size_borders *borders);
void            get_window_borders_no_title(const MvwmWindow *fw,
    size_borders *borders);
void            set_window_border_size(MvwmWindow *fw, int used_width);
Bool            is_window_border_minimal(MvwmWindow *fw);
void            update_relative_geometry(MvwmWindow *fw);
void            update_absolute_geometry(MvwmWindow *fw);
void            maximize_adjust_offset(MvwmWindow *fw);
void            constrain_size(MvwmWindow *fw, const XEvent *e, int *widthp,
    int *heightp, int xmotion, int ymotion, int flags);
void            gravity_constrain_size(int gravity, MvwmWindow *t,
    rectangle *rect, int flags);
Bool            get_visible_window_or_icon_geometry(MvwmWindow *fw,
    rectangle *ret_g);
Bool            get_visible_icon_geometry(MvwmWindow *fw, rectangle *ret_g);
void            get_icon_geometry(MvwmWindow *fw, rectangle *ret_g);
Bool            get_visible_icon_title_geometry(MvwmWindow *fw,
    rectangle *ret_g);
Bool            get_icon_title_geometry(MvwmWindow *fw, rectangle *ret_g);
Bool            get_visible_icon_picture_geometry(MvwmWindow *fw,
    rectangle *ret_g);
Bool            get_icon_picture_geometry(MvwmWindow *fw, rectangle *ret_g);
void            broadcast_icon_geometry(MvwmWindow *fw, Bool do_force);
void            move_icon_to_position(MvwmWindow *fw);
void            modify_icon_position(MvwmWindow *fw, int dx, int dy);
void            set_icon_position(MvwmWindow *fw, int x, int y);
void            set_icon_picture_size(MvwmWindow *fw, int w, int h);
void            resize_icon_title_height(MvwmWindow *fw, int dh);
void            get_page_offset_rectangle(int *ret_page_x, int *ret_page_y,
    rectangle *r);
void            get_page_offset(int *ret_page_x, int *ret_page_y,
    MvwmWindow *fw);
void            get_page_offset_check_visible(int *ret_page_x,
    int *ret_page_y, MvwmWindow *fw);

#endif
