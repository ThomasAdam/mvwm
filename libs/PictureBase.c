/* -*-c-*- */
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
 * This module is all original code
 * by Rob Nation
 * Copyright 1993, Robert Nation
 *     You may use this code for any purpose, as long as the original
 *     copyright remains in the source code and all documentation
 */
/*
  Changed 02/12/97 by Dan Espen:
  - added routines to determine color closeness, for color use reduction.
  Some of the logic comes from pixy2, so the copyright is below.
*/
/*
 * Copyright 1996, Romano Giannetti. No guarantees or warantees or anything
 * are provided or implied in any way whatsoever. Use this program at your
 * own risk. Permission to use this program for any purpose is given,
 * as long as the copyright is kept intact.
 *
 * Romano Giannetti - Dipartimento di Ingegneria dell'Informazione
 *                    via Diotisalvi, 2  PISA
 * mailto:romano@iet.unipi.it
 * http://www.iet.unipi.it/~romano
 *
 */

/*
 *
 * Routines to handle initialization, loading, and removing of xpm's or mono-
 * icon images.
 *
 */

#include "config.h"

#include <stdio.h>

#include <X11/Xlib.h>

#include "mvwmlib.h"
#include "Graphics.h"
#include "PictureBase.h"
#include "PictureUtils.h"
#include "Strings.h"

Bool            Pdefault;
Visual         *Pvisual;
static Visual  *MvwmVisual;
Colormap        Pcmap;
static Colormap MvwmCmap;
unsigned int    Pdepth;
static unsigned int MvwmDepth;
Display        *Pdpy;		/* Save area for display pointer */
Bool            PUseDynamicColors;

Pixel           PWhitePixel;
Pixel           PBlackPixel;
Pixel           MvwmWhitePixel;
Pixel           MvwmBlackPixel;

void            PictureSetupWhiteAndBlack(void);

void
PictureInitCMap(Display *dpy)
{
	char           *envp;

	Pdpy = dpy;
	/*
	 * if mvwm has not set this env-var it is using the default visual
	 */
	envp = getenv("MVWM_VISUALID");
	if (envp != NULL && *envp > 0) {
		/*
		 * convert the env-vars to a visual and colormap
		 */
		int             viscount;
		XVisualInfo     vizinfo, *xvi;

		sscanf(envp, "%lx", &vizinfo.visualid);
		xvi = XGetVisualInfo(dpy, VisualIDMask, &vizinfo, &viscount);
		Pvisual = xvi->visual;
		Pdepth = xvi->depth;
		/*
		 * Note: if MVWM_VISUALID is set, MVWM_COLORMAP is set too
		 */
		sscanf(getenv("MVWM_COLORMAP"), "%lx", &Pcmap);
		Pdefault = False;
	} else {
		int             screen = DefaultScreen(dpy);

		Pvisual = DefaultVisual(dpy, screen);
		Pdepth = DefaultDepth(dpy, screen);
		Pcmap = DefaultColormap(dpy, screen);
		Pdefault = True;
	}

	PictureSetupWhiteAndBlack();
	PictureSaveMvwmVisual();

	/*
	 * initialise color limit
	 */
	PUseDynamicColors = 0;
	PictureInitColors(PICTURE_CALLED_BY_MODULE, True, NULL, False, True);
	return;
}

void
PictureInitCMapRoot(Display *dpy, Bool init_color_limit,
    PictureColorLimitOption *opt, Bool use_my_color_limit, Bool init_dither)
{
	int             screen = DefaultScreen(dpy);

	Pdpy = dpy;

	Pvisual = DefaultVisual(dpy, screen);
	Pdepth = DefaultDepth(dpy, screen);
	Pcmap = DefaultColormap(dpy, screen);
	Pdefault = True;

	PictureSetupWhiteAndBlack();
	PictureSaveMvwmVisual();

	/*
	 * initialise color limit
	 */
	PictureInitColors(PICTURE_CALLED_BY_MODULE, init_color_limit, opt,
	    use_my_color_limit, init_dither);
	return;
}

void
PictureSetupWhiteAndBlack(void)
{
	XColor          c;

	if (!Pdefault) {
		c.flags = DoRed | DoGreen | DoBlue;
		c.red = c.green = c.blue = 65535;
		XAllocColor(Pdpy, Pcmap, &c);
		PWhitePixel = c.pixel;
		c.red = c.green = c.blue = 0;
		XAllocColor(Pdpy, Pcmap, &c);
		PBlackPixel = c.pixel;
	} else {
		PWhitePixel = WhitePixel(Pdpy, DefaultScreen(Pdpy));
		PBlackPixel = BlackPixel(Pdpy, DefaultScreen(Pdpy));
	}

	return;
}

void
PictureUseDefaultVisual(void)
{
	int             screen = DefaultScreen(Pdpy);

	Pvisual = DefaultVisual(Pdpy, screen);
	Pdepth = DefaultDepth(Pdpy, screen);
	Pcmap = DefaultColormap(Pdpy, screen);
	PWhitePixel = WhitePixel(Pdpy, DefaultScreen(Pdpy));
	PBlackPixel = BlackPixel(Pdpy, DefaultScreen(Pdpy));
	return;
}

void
PictureUseMvwmVisual(void)
{
	Pvisual = MvwmVisual;
	Pdepth = MvwmDepth;
	Pcmap = MvwmCmap;
	PWhitePixel = MvwmWhitePixel;
	PBlackPixel = MvwmBlackPixel;
	return;
}

void
PictureSaveMvwmVisual(void)
{
	MvwmVisual = Pvisual;
	MvwmDepth = Pdepth;
	MvwmCmap = Pcmap;
	MvwmWhitePixel = PWhitePixel;
	MvwmBlackPixel = PBlackPixel;
	return;
}

Pixel
PictureWhitePixel(void)
{
	return PWhitePixel;
}

Pixel
PictureBlackPixel(void)
{
	return PBlackPixel;
}

GC
PictureDefaultGC(Display *dpy, Window win)
{
	static GC       gc = None;

	if (Pdepth == DefaultDepth(dpy, DefaultScreen(dpy))) {
		return DefaultGC(dpy, DefaultScreen(dpy));
	}
	if (gc == None) {
		gc = mvwmlib_XCreateGC(dpy, win, 0, NULL);
	}

	return gc;
}

static char    *imagePath = MVWM_IMAGEPATH;

void
PictureSetImagePath(const char *newpath)
{
	static int      need_to_free = 0;
	setPath(&imagePath, newpath, need_to_free);
	need_to_free = 1;

	return;
}

char           *
PictureGetImagePath(void)
{
	return imagePath;
}

/*
 *
 * Find the specified image file somewhere along the given path.
 *
 * There is a possible race condition here:  We check the file and later
 * do something with it.  By then, the file might not be accessible.
 * Oh well.
 *
 */
char           *
PictureFindImageFile(const char *icon, const char *pathlist, int type)
{
	if (pathlist == NULL) {
		pathlist = imagePath;
	}
	if (icon == NULL) {
		return NULL;
	}

	return (searchPath(pathlist, icon, ".gz", type));
}
