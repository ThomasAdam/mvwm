/* -*-c-*- */
/* Copyright (C) 1993, Robert Nation
 * Copyright (C) 1999 Carsten Haitzler and various contributors (imlib2)
 * Copyright (C) 2002  Olivier Chapuis */
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *
 * The png loader and PImageRGBtoPixel are from imlib2. The code is from raster
 * (Carsten Haitzler) <raster@rasterman.com> <raster@valinux.com>
 *
 */

/* ---------------------------- included header files ---------------------- */

#include "config.h"

#include <stdio.h>
#include <signal.h>
#include <ctype.h>

#include <X11/Xlib.h>
#include <X11/Xmd.h>

#include <mvwmlib.h>
#include "System.h"
#include "Strings.h"
#include "Picture.h"
#include "PictureUtils.h"
#include "Graphics.h"
#include "ColorUtils.h"
#include "Fpng.h"
#include "FRenderInit.h"
#include "Fcursor.h"
#include "FImage.h"

/* ---------------------------- local definitions -------------------------- */
#define FIMAGE_CMD_ARGS \
	Display *dpy, char *path, CARD32 **argb_data, int *width, int *height

#define FIMAGE_PASS_ARGS \
	dpy, path, argb_data, width, height

typedef struct PImageLoader
{
	char           *extension;
#ifdef __STDC__
	int             (*func) (FIMAGE_CMD_ARGS);
#else
	int             (*func) ();
#endif
} PImageLoader;

/* ---------------------------- local macros ------------------------------- */

/* ---------------------------- imports ------------------------------------ */

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

/* ---------------------------- forward declarations ----------------------- */

static Bool     PImageLoadPng(FIMAGE_CMD_ARGS);

/* ---------------------------- local variables ---------------------------- */

PImageLoader    Loaders[] = {
	{"png", PImageLoadPng},
	{NULL, 0}
};

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- local functions ---------------------------- */

static Bool
PImageLoadArgbDataFromFile(FIMAGE_CMD_ARGS)
{
	int             done = 0, i = 0, tried = -1;
	char           *ext = NULL;

	if (path == NULL)
		return False;

	if (strlen(path) > 3) {
		ext = path + strlen(path) - 3;
	}
	/*
	 * first try to load by extension
	 */
	while (!done && ext != NULL && Loaders[i].extension != NULL) {
		if (StrEquals(Loaders[i].extension, ext)) {
			if (Loaders[i].func(FIMAGE_PASS_ARGS)) {
				return True;
			}
			tried = i;
			done = 1;
		}
		i++;
	}

	i = 0;
	while (Loaders[i].extension != NULL) {
		if (i != tried && Loaders[i].func(FIMAGE_PASS_ARGS)) {
			return True;
		}
		i++;
	}

	return False;
}

/*
 *
 * png loader
 *
 */
static Bool
PImageLoadPng(FIMAGE_CMD_ARGS)
{
	Fpng_uint_32    w32, h32;
	Fpng_structp    Fpng_ptr = NULL;
	Fpng_infop      Finfo_ptr = NULL;
	CARD32         *data;
	int             w, h;
	char            hasa = 0, hasg = 0;
	FILE           *f;
	int             bit_depth;
	int             color_type;
	int             interlace_type;
	unsigned char   buf[FPNG_BYTES_TO_CHECK];
	unsigned char **lines;
	int             i;

	if (!PngSupport) {
		/*
		 * suppress compiler warning
		 */
		bit_depth = 0;

		fprintf(stderr, "[PImageLoadPng]: Tried to load PNG file "
		    "when MVWM has not been compiled with PNG support.\n");

		return False;
	}
	if (!(f = fopen(path, "rb"))) {
		return False;
	}
	{
		int             n;

		n = fread(buf, 1, FPNG_BYTES_TO_CHECK, f);
		(void) n;
	}
	if (!Fpng_check_sig(buf, FPNG_BYTES_TO_CHECK)) {
		fclose(f);
		return False;
	}
	rewind(f);
	Fpng_ptr = Fpng_create_read_struct(FPNG_LIBPNG_VER_STRING,
	    NULL, NULL, NULL);
	if (!Fpng_ptr) {
		fclose(f);
		return False;
	}
	Finfo_ptr = Fpng_create_info_struct(Fpng_ptr);
	if (!Finfo_ptr) {
		Fpng_destroy_read_struct(&Fpng_ptr, NULL, NULL);
		fclose(f);
		return False;
	}
#if 0
	if (setjmp(Fpng_ptr->jmpbuf)) {
		Fpng_destroy_read_struct(&Fpng_ptr, &Finfo_ptr, NULL);
		fclose(f);
		return False;
	}
#endif
	Fpng_init_io(Fpng_ptr, f);
	Fpng_read_info(Fpng_ptr, Finfo_ptr);
	Fpng_get_IHDR(Fpng_ptr, Finfo_ptr, (Fpng_uint_32 *) (&w32),
	    (Fpng_uint_32 *) (&h32), &bit_depth, &color_type,
	    &interlace_type, NULL, NULL);
	interlace_type = 0;	/* not used */
	*width = w = (int) w32;
	*height = h = (int) h32;
	if (color_type == FPNG_COLOR_TYPE_PALETTE) {
		Fpng_set_expand(Fpng_ptr);
	}

	/*
	 * TA:  XXX:  (2011-02-14) -- Happy Valentines Day.
	 * *
	 * * png_get_color_type() defined in libpng 1.5 now hides a data member
	 * * to a struct:
	 * *
	 * * Finfo_ptr->color_type
	 * *
	 * * I'm not going to wrap this up in more #ifdef madness, but should
	 * * this fail to build on much older libpng versions which we support
	 * * (pre 1.3), then I might have to.
	 */
	if (png_get_color_type(Fpng_ptr,
		Finfo_ptr) == FPNG_COLOR_TYPE_RGB_ALPHA) {
		hasa = 1;
	}
	if (png_get_color_type(Fpng_ptr,
		Finfo_ptr) == FPNG_COLOR_TYPE_GRAY_ALPHA) {
		hasa = 1;
		hasg = 1;
	}
	if (png_get_color_type(Fpng_ptr, Finfo_ptr) == FPNG_COLOR_TYPE_GRAY) {
		hasg = 1;
	}

	if (hasa)
		Fpng_set_expand(Fpng_ptr);
	/*
	 * we want ARGB
	 */
	/*
	 * note form raster:
	 * * thanks to mustapha for helping debug this on PPC Linux remotely by
	 * * sending across screenshots all the time and me figuring out form them
	 * * what the hell was up with the colors
	 * * now png loading should work on big endian machines nicely
	 */
#ifdef WORDS_BIGENDIAN
	Fpng_set_swap_alpha(Fpng_ptr);
	Fpng_set_filler(Fpng_ptr, 0xff, FPNG_FILLER_BEFORE);
#else
	Fpng_set_bgr(Fpng_ptr);
	Fpng_set_filler(Fpng_ptr, 0xff, FPNG_FILLER_AFTER);
#endif
	/*
	 * 16bit color -> 8bit color
	 */
	Fpng_set_strip_16(Fpng_ptr);
	/*
	 * pack all pixels to byte boundaires
	 */
	Fpng_set_packing(Fpng_ptr);
	if (Fpng_get_valid(Fpng_ptr, Finfo_ptr, FPNG_INFO_tRNS)) {
		Fpng_set_expand(Fpng_ptr);
	}

	data = mvwm_malloc(w * h * sizeof(CARD32));
	lines = mvwm_malloc(h * sizeof(unsigned char *));

	if (hasg) {
		Fpng_set_gray_to_rgb(Fpng_ptr);
		if (Fpng_get_bit_depth(Fpng_ptr, Finfo_ptr) < 8) {
			Fpng_set_gray_1_2_4_to_8(Fpng_ptr);
		}
	}
	for (i = 0; i < h; i++) {
		lines[i] = (unsigned char *) data + (i * w * sizeof(CARD32));
	}
	Fpng_read_image(Fpng_ptr, lines);
	Fpng_read_end(Fpng_ptr, Finfo_ptr);
	Fpng_destroy_read_struct(&Fpng_ptr, &Finfo_ptr, (png_infopp) NULL);
	fclose(f);
	free(lines);
	*argb_data = data;

	return True;
}

/*
 *
 * copy image to server
 *
 */
static Pixmap
PImageCreatePixmapFromFImage(Display *dpy, Window win, FImage * fimage)
{
	GC              gc;
	Pixmap          pixmap;
	int             w;
	int             h;
	int             depth;

	w = fimage->im->width;
	h = fimage->im->height;
	depth = fimage->im->depth;
	pixmap = XCreatePixmap(dpy, win, w, h, depth);
	if (depth == Pdepth) {
		gc = PictureDefaultGC(dpy, win);
	} else {
		gc = mvwmlib_XCreateGC(dpy, pixmap, 0, NULL);
	}
	FPutFImage(dpy, pixmap, gc, fimage, 0, 0, 0, 0, w, h);
	if (depth != Pdepth) {
		XFreeGC(dpy, gc);
	}

	return pixmap;
}

/* ---------------------------- interface functions ------------------------ */

/*
 *
 * argb data to pixmaps
 *
 */
Bool
PImageCreatePixmapFromArgbData(Display *dpy, Window win, CARD32 *data,
    int start, int width, int height, Pixmap *pixmap, Pixmap *mask,
    Pixmap *alpha, int *nalloc_pixels, Pixel **alloc_pixels, int *no_limit,
    MvwmPictureAttributes fpa)
{
	FImage         *fim;
	FImage         *m_fim = NULL;
	FImage         *a_fim = NULL;
	XColor          c;
	int             i;
	int             j;
	int             a;
	PictureImageColorAllocator *pica = NULL;
	int             alpha_limit = PICTURE_ALPHA_LIMIT;
	int             alpha_depth = FRenderGetAlphaDepth();
	Bool            have_mask = False;
	Bool            have_alpha = False;

	fim =
	    FCreateFImage(dpy, Pvisual,
	    (fpa.mask & FPAM_MONOCHROME) ? 1 : Pdepth, ZPixmap, width,
	    height);
	if (!fim) {
		return False;
	}
	if (mask) {
		m_fim =
		    FCreateFImage(dpy, Pvisual, 1, ZPixmap, width, height);
	}
	if (alpha && !(fpa.mask & FPAM_NO_ALPHA) && alpha_depth) {
		alpha_limit = 0;
		a_fim =
		    FCreateFImage(dpy, Pvisual, alpha_depth, ZPixmap, width,
		    height);
	}
	if (!(fpa.mask & FPAM_MONOCHROME)) {
		c.flags = DoRed | DoGreen | DoBlue;
		pica =
		    PictureOpenImageColorAllocator(dpy, Pcmap, width, height,
		    ! !(fpa.mask & FPAM_NO_COLOR_LIMIT),
		    ! !(fpa.mask & FPAM_NO_ALLOC_PIXELS),
		    ! !(fpa.mask & FPAM_DITHER), True);
	}
	data += start;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++, data++) {
			a = (*data >> 030) & 0xff;
			if (a > alpha_limit) {
				c.red = (*data >> 16) & 0xff;
				c.green = (*data >> 8) & 0xff;
				c.blue = (*data) & 0xff;
				if (pica) {
					PictureAllocColorImage(dpy, pica, &c,
					    i, j);
					XPutPixel(fim->im, i, j, c.pixel);
				}
				/*
				 * Brightness threshold
				 */
				else if ((0x99 * c.red +
					0x12D * c.green +
					0x3A * c.blue) >> 16) {
					XPutPixel(fim->im, i, j, 1);
				} else {
					XPutPixel(fim->im, i, j, 0);
				}
				if (m_fim) {
					XPutPixel(m_fim->im, i, j, 1);
				}
			} else if (m_fim != NULL) {
				XPutPixel(m_fim->im, i, j, 0);
				have_mask = True;
			}
			if (a_fim != NULL) {
				XPutPixel(a_fim->im, i, j, a);
				if (a > 0 && a < 0xff) {
					have_alpha = True;
				}
			}
		}
	}
	if (pica) {
		PictureCloseImageColorAllocator(dpy, pica, nalloc_pixels,
		    alloc_pixels, no_limit);
	}
	*pixmap = PImageCreatePixmapFromFImage(dpy, win, fim);
	if (have_alpha) {
		*alpha = PImageCreatePixmapFromFImage(dpy, win, a_fim);
	} else if (have_mask) {
		*mask = PImageCreatePixmapFromFImage(dpy, win, m_fim);
	}
	FDestroyFImage(dpy, fim);
	if (m_fim) {
		FDestroyFImage(dpy, m_fim);
	}
	if (a_fim) {
		FDestroyFImage(dpy, a_fim);
	}

	return True;
}

/*
 *
 * the images loaders
 *
 */

Bool
PImageLoadPixmapFromFile(Display *dpy, Window win, char *path, Pixmap *pixmap,
    Pixmap *mask, Pixmap *alpha, int *width, int *height, int *depth,
    int *nalloc_pixels, Pixel **alloc_pixels, int *no_limit,
    MvwmPictureAttributes fpa)
{
	CARD32         *data;

	if (PImageLoadArgbDataFromFile(dpy, path, &data, width, height)) {
		*depth = (fpa.mask & FPAM_MONOCHROME) ? 1 : Pdepth;
		if (PImageCreatePixmapFromArgbData(dpy, win, data, 0, *width,
			*height, pixmap, mask, alpha, nalloc_pixels,
			alloc_pixels, no_limit, fpa)) {
			free(data);

			return True;
		}
		free(data);
	}
	/*
	 * Bitmap fallback
	 */
	else if (XReadBitmapFile(dpy, win, path, (unsigned int *) width,
		(unsigned int *) height, pixmap, NULL, NULL) ==
	    BitmapSuccess) {
		*depth = 1;
		*mask = None;

		return True;
	}
	pixmap = None;
	mask = None;
	alpha = None;
	*width = *height = *depth = 0;
	if (nalloc_pixels != NULL) {
		*nalloc_pixels = 0;
	}
	if (alloc_pixels != NULL) {
		*alloc_pixels = NULL;
	}
	return False;
}

MvwmPicture    *
PImageLoadMvwmPictureFromFile(Display *dpy, Window win, char *path,
    MvwmPictureAttributes fpa)
{
	MvwmPicture    *p;
	Pixmap          pixmap = None;
	Pixmap          mask = None;
	Pixmap          alpha = None;
	int             width = 0, height = 0;
	int             depth = 0, no_limit;
	int             nalloc_pixels = 0;
	Pixel          *alloc_pixels = NULL;
	char           *real_path;

	real_path = path;

	if (!PImageLoadPixmapFromFile(dpy, win, path, &pixmap, &mask, &alpha,
		&width, &height, &depth, &nalloc_pixels, &alloc_pixels,
		&no_limit, fpa)) {
		return NULL;
	}

	p = mvwm_calloc(1, sizeof(MvwmPicture));
	p->count = 1;
	p->name = path;
	p->fpa_mask = fpa.mask;
	p->next = NULL;
	setFileStamp(&p->stamp, real_path);
	p->picture = pixmap;
	p->mask = mask;
	p->alpha = alpha;
	p->width = width;
	p->height = height;
	p->depth = depth;
	p->nalloc_pixels = nalloc_pixels;
	p->alloc_pixels = alloc_pixels;
	p->no_limit = no_limit;
	return p;
}

Cursor
PImageLoadCursorFromFile(Display *dpy, Window win, char *path, int x_hot,
    int y_hot)
{
	Cursor          cursor = 0;
	CARD32         *data;
	int             width;
	int             height;
	int             i;
	FcursorImages  *fcis;
	FcursorImage   *fci;

	/*
	 * First try the Xcursor loader (animated cursors)
	 */
	if ((fcis =
		FcursorFilenameLoadImages(path,
		    FcursorGetDefaultSize(dpy)))) {
		for (i = 0; i < fcis->nimage; i++) {
			if (x_hot < fcis->images[i]->width && x_hot >= 0 &&
			    y_hot < fcis->images[i]->height && y_hot >= 0) {
				fcis->images[i]->xhot = x_hot;
				fcis->images[i]->yhot = y_hot;
			}
		}
		cursor = FcursorImagesLoadCursor(dpy, fcis);
		FcursorImagesDestroy(fcis);
	}
	/*
	 * Get cursor data from the regular image loader
	 */
	else if (PImageLoadArgbDataFromFile(dpy, path, &data, &width,
		&height)) {
		Pixmap          src;
		Pixmap          msk = None;
		MvwmPictureAttributes fpa;

		fpa.mask = FPAM_NO_ALPHA | FPAM_MONOCHROME;

		/*
		 * Use the Xcursor library to create the argb cursor
		 */
		if ((fci = FcursorImageCreate(width, height))) {
			unsigned char   alpha;
			unsigned char   red;
			unsigned char   green;
			unsigned char   blue;

			/*
			 * Xcursor expects alpha prescaled RGB values
			 */
			for (i = 0; i < width * height; i++) {
				alpha = ((data[i] >> 24) & 0xff);
				red = ((data[i] >> 16) & 0xff) * alpha / 0xff;
				green =
				    ((data[i] >> 8) & 0xff) * alpha / 0xff;
				blue = ((data[i]) & 0xff) * alpha / 0xff;

				data[i] =
				    (alpha << 24) | (red << 16) |
				    (green << 8) | blue;
			}

			fci->xhot = x_hot;
			fci->yhot = y_hot;
			fci->delay = 0;
			fci->pixels = (FcursorPixel *) data;
			cursor = FcursorImageLoadCursor(dpy, fci);
			FcursorImageDestroy(fci);
		}
		/*
		 * Create monochrome cursor from argb data
		 */
		else if (PImageCreatePixmapFromArgbData(dpy, win, data, 0,
			width, height, &src, &msk, 0, 0, 0, 0, fpa)) {
			XColor          c[2];

			c[0].pixel = GetColor(DEFAULT_CURSOR_FORE_COLOR);
			c[1].pixel = GetColor(DEFAULT_CURSOR_BACK_COLOR);
			XQueryColors(dpy, Pcmap, c, 2);
			cursor =
			    XCreatePixmapCursor(dpy, src, msk, &(c[0]),
			    &(c[1]), x_hot, y_hot);
			XFreePixmap(dpy, src);
			XFreePixmap(dpy, msk);
		}
		free(data);
	}

	return cursor;
}
