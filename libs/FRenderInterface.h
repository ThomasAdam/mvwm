/* -*-c-*- */
/* Copyright (C) 2002  Olivier Chapuis */

#ifndef F_RENDER_INTERFACE_H
#define F_RENDER_INTERFACE_H

Bool	 FRenderTintRectangle(Display *, Window, Pixmap, Pixel, int, Drawable,
    int, int, int dest_w, int dest_h);

int	 FRenderRender(Display *, Window, Pixmap, Pixmap, Pixmap, int, int,
    Pixel, int, Drawable, GC, GC, int, int, int, int, int, int, int, int, Bool);
#endif
