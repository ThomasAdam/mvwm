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

#include "config.h"

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "defaults.h"
#include "mvwmlib.h"
#include "Parse.h"
#include "FScreen.h"
#include "PictureBase.h"

#ifdef HAVE_XRANDR
#	include <X11/extensions/Xrandr.h>
#	define IS_RANDR_ENABLED 1
#else
#	define IS_RANDR_ENABLED 0
#endif

/* In fact, only corners matter -- there will never be GRAV_NONE */
enum
{ GRAV_POS = 0, GRAV_NONE = 1, GRAV_NEG = 2 };
static int      grav_matrix[3][3] = {
	{NorthWestGravity, NorthGravity, NorthEastGravity},
	{WestGravity, CenterGravity, EastGravity},
	{SouthWestGravity, SouthGravity, SouthEastGravity}
};

#define DEFAULT_GRAVITY NorthWestGravity

static Bool     already_initialised;
static Display *disp;
static int      no_of_screens;

static struct monitor *FindScreenOfXY(int x, int y);
static struct monitor *monitor_new(void);
static void     init_monitor_contents(void);

static void
GetMouseXY(XEvent *eventp, int *x, int *y)
{
	XEvent          e;

	if (eventp == NULL) {
		eventp = &e;
		e.type = 0;
	}
	fev_get_evpos_or_query(disp, DefaultRootWindow(disp), eventp, x, y);
}

Bool
FScreenIsEnabled(void)
{
	return (IS_RANDR_ENABLED);
}

struct monitor *
monitor_new(void)
{
	struct monitor *m;

	m = calloc(1, sizeof *m);

	return (m);
}

struct monitor *
monitor_get_current(void)
{
	int             JunkX = 0, JunkY = 0, x, y;
	Window          JunkRoot, JunkChild;
	unsigned int    JunkMask;

	FQueryPointer(disp, DefaultRootWindow(disp), &JunkRoot, &JunkChild,
	    &JunkX, &JunkY, &x, &y, &JunkMask);

	return (FindScreenOfXY(x, y));
}

int
monitor_should_ignore_global(struct monitor *m)
{
	/*
	 * If we have more than one screen configured, then don't match
	 * on the global screen, as that's separate to XY positioning
	 * which is only concerned with the *specific* screen.
	 */
	if (no_of_screens > 0 && strcmp(m->name, "global") == 0)
		return (1);
	return (0);
}

struct monitor *
monitor_by_name(const char *name)
{
	struct monitor *m;

	TAILQ_FOREACH(m, &monitor_q, entry) {
		if (strcmp(m->name, name) == 0)
			return (m);
	}
	return (NULL);
}

struct monitor *
monitor_by_xy(int x, int y)
{
	return (FindScreenOfXY(x, y));
}

void
FScreenInit(Display *dpy)
{
	XRRScreenResources *res = NULL;
	XRROutputInfo  *oinfo = NULL;
	XRRCrtcInfo    *crtc = NULL;
	struct monitor *m;
	int             i;
	int             err_base = 0, event = 0;
	int             is_randr_present = 0;

	disp = dpy;

	if (already_initialised)
		return;

	is_randr_present = XRRQueryExtension(dpy, &event, &err_base);

	if (FScreenIsEnabled() && !is_randr_present) {
		/* Something went wrong.   Shouldn't we try Xinerama here? */
		fprintf(stderr, "Couldn't initialise XRandR: %s\n",
		    strerror(errno));
		fprintf(stderr, "Falling back to single screen...\n");
	}

	TAILQ_INIT(&monitor_q);

	m = monitor_new();
	m->coord.x = 0;
	m->coord.y = 0;
	m->coord.w = DisplayWidth(disp, DefaultScreen(disp));
	m->coord.h = DisplayHeight(disp, DefaultScreen(disp));
	m->name = xstrdup("global");
	TAILQ_INSERT_HEAD(&monitor_q, m, entry);

	if (!is_randr_present)
		goto done;

	/*
	 * XRandR is present, so query the screens we have.
	 */
	res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
	if (res != NULL) {
		crtc = NULL;
		no_of_screens = res->noutput;
		for (i = 0; i < res->noutput; i++) {
			oinfo = XRRGetOutputInfo(dpy, res, res->outputs[i]);

			if (oinfo->crtc == None) {
				XRRFreeOutputInfo(oinfo);
				continue;
			}
			crtc = XRRGetCrtcInfo(dpy, res, oinfo->crtc);
			if (crtc == NULL)
				continue;

			m = monitor_new();
			m->coord.x = crtc->x;
			m->coord.y = crtc->y;
			m->coord.w = crtc->width;
			m->coord.h = crtc->height;
			m->name = xstrdup(oinfo->name);

			TAILQ_INSERT_TAIL(&monitor_q, m, entry);

			XRRFreeCrtcInfo(crtc);
		}
		XRRFreeScreenResources(res);
	}
done:
	already_initialised = 1;
	init_monitor_contents();
}

static void
init_monitor_contents(void)
{
	struct monitor *m = NULL;

	TAILQ_FOREACH(m, &monitor_q, entry) {
		if (monitor_should_ignore_global(m))
			continue;

		m->Desktops = xcalloc(1, sizeof(DesktopsInfo));
		m->Desktops->name = NULL;
		m->Desktops->desk = 0;	/* not desk 0 */
		m->Desktops->ewmh_dyn_working_area.x =
		    m->Desktops->ewmh_working_area.x = 0;
		m->Desktops->ewmh_dyn_working_area.y =
		    m->Desktops->ewmh_working_area.y = 0;
		m->Desktops->ewmh_dyn_working_area.width =
		    m->Desktops->ewmh_working_area.width = m->coord.w;
		m->Desktops->ewmh_dyn_working_area.height =
		    m->Desktops->ewmh_working_area.height = m->coord.h;
		m->Desktops->next = NULL;

		m->virtual_scr.CurrentDesk = 0;
		m->virtual_scr.Vx = 0;
		m->virtual_scr.Vy = 0;
		m->virtual_scr.VxMax = 2 * m->coord.w;
		m->virtual_scr.VyMax = 2 * m->coord.h;

		m->virtual_scr.prev_page_x = 0;
		m->virtual_scr.prev_page_y = 0;
		m->virtual_scr.prev_desk = 0;
		m->virtual_scr.prev_desk_and_page_desk = 0;
		m->virtual_scr.prev_desk_and_page_page_x = 0;
		m->virtual_scr.prev_desk_and_page_page_y = 0;

		m->virtual_scr.EdgeScrollX =
		    DEFAULT_EDGE_SCROLL * m->coord.w / 100;
		m->virtual_scr.EdgeScrollY =
		    DEFAULT_EDGE_SCROLL * m->coord.h / 100;
	}
}

/* Intended to be called by modules.  Simply pass in the parameter from the
 * config string sent by mvwm. */
void
FScreenConfigureModule(char *args)
{
	/* TA:  XXX:  Not needed?  API change... */
	return;
}

/* Here's the function used by mvwm to generate the string which
 * FScreenConfigureModule expects to receive back as its argument.
 */
const char     *
FScreenGetConfiguration(void)
{
	static char     msg[MAX_MODULE_INPUT_TEXT_LEN];

	xsnprintf(msg, sizeof msg, XINERAMA_CONFIG_STRING " %d %d %d %d %d %d",
	    FScreenIsEnabled(), 0, 0, 0, 0, 0);
	return (msg);
}

/* Sets the default screen for ...ParseGeometry if no screen spec is given.
 * Usually this is FSCREEN_SPEC_PRIMARY, but this won't allow modules to appear
 * under the pointer. */
void
FScreenSetDefaultModuleScreen(char *scr_spec)
{
	return;
}

static struct monitor *
FindScreenOfXY(int x, int y)
{
	struct monitor *m;

	TAILQ_FOREACH(m, &monitor_q, entry) {
		if (monitor_should_ignore_global(m))
			continue;
		if (x >= m->coord.x && x < m->coord.x + m->coord.w &&
		    y >= m->coord.y && y < m->coord.y + m->coord.h)
			return (m);
	}

	return (NULL);
}

static struct monitor *
FindScreen(fscreen_scr_arg *arg, fscreen_scr_t screen)
{
	struct monitor *m = NULL;
	fscreen_scr_arg tmp;

	if (no_of_screens == 0)
		return (monitor_by_name("global"));

	switch (screen) {
	case FSCREEN_GLOBAL:
		m = monitor_by_name("global");
		break;
	case FSCREEN_PRIMARY:
	case FSCREEN_CURRENT:
		/* translate to xypos format */
		if (!arg) {
			tmp.mouse_ev = NULL;
			arg = &tmp;
		}
		GetMouseXY(arg->mouse_ev, &arg->xypos.x, &arg->xypos.y);
		/* fall through */
	case FSCREEN_XYPOS:
		/* translate to screen number */
		if (!arg) {
			tmp.xypos.x = 0;
			tmp.xypos.y = 0;
			arg = &tmp;
		}
		m = FindScreenOfXY(arg->xypos.x, arg->xypos.y);
		break;
	case FSCREEN_BY_NAME:
		if (arg == NULL || arg->name == NULL) {
			/* XXX: Work out what to do. */
			break;
		}
		m = monitor_by_name(arg->name);
		break;
	default:
		/* XXX: Possible error condition here? */
		break;
	}

	if (m == NULL)
		m = TAILQ_FIRST(&monitor_q);

	return (m);
}

/* Given pointer coordinates, return the screen the pointer is on.
 *
 * Perhaps most useful with $[pointer.screen]
 */
const char     *
FScreenOfPointerXY(int x, int y)
{
	struct monitor *m;

	m = FindScreenOfXY(x, y);

	return (m != NULL) ? m->name : "unknown";
}

/* Returns the specified screens geometry rectangle.  screen can be a screen
 * number or any of the values FSCREEN_GLOBAL, FSCREEN_CURRENT,
 * FSCREEN_PRIMARY or FSCREEN_XYPOS.  The arg union only has a meaning for
 * FSCREEN_CURRENT and FSCREEN_XYARG.  For FSCREEN_CURRENT its mouse_ev member
 * may be given.  It is tried to find out the pointer position from the event
 * first before querying the pointer.  For FSCREEN_XYPOS the xpos member is used
 * to fetch the x/y position of the point on the screen.  If arg is NULL, the
 * position 0 0 is assumed instead.
 *
 * Any of the arguments arg, x, y, w and h may be NULL.
 *
 * FSCREEN_GLOBAL:  return the global screen dimensions
 * FSCREEN_CURRENT: return dimensions of the screen with the pointer
 * FSCREEN_PRIMARY: return the primary screen dimensions
 * FSCREEN_XYPOS:   return dimensions of the screen with the given coordinates
 * FSCREEN_BY_NAME: return dimensions of the screen with the given name
 *
 * The function returns False if the global screen was returned and more than
 * one screen is configured.  Otherwise it returns True.
 */
Bool
FScreenGetScrRect(fscreen_scr_arg *arg, fscreen_scr_t screen,
    int *x, int *y, int *w, int *h)
{
	struct monitor	*m = FindScreen(arg, screen);
	if (m == NULL)
		return (True);

	if (x)
		*x = m->coord.x;
	if (y)
		*y = m->coord.y;
	if (w)
		*w = m->coord.w;
	if (h)
		*h = m->coord.h;

	return !((no_of_screens > 1) && (strcmp(m->name, "global") == 0));
}

/* Translates the coodinates *x *y from the screen specified by arg_src and
 * screen_src to coordinates on the screen specified by arg_dest and
 * screen_dest. (see FScreenGetScrRect for more details). */
void
FScreenTranslateCoordinates(fscreen_scr_arg *arg_src,
    fscreen_scr_t screen_src, fscreen_scr_arg *arg_dest,
    fscreen_scr_t screen_dest, int *x, int *y)
{
	int	 x_src;
	int	 y_src;
	int	 x_dest;
	int	 y_dest;

	FScreenGetScrRect(arg_src, screen_src, &x_src, &y_src, NULL, NULL);
	FScreenGetScrRect(arg_dest, screen_dest, &x_dest, &y_dest, NULL, NULL);

	if (x)
		*x = *x + x_src - x_dest;

	if (y)
		*y = *y + y_src - y_dest;
}

/* Arguments work exactly like for FScreenGetScrRect() */
int
FScreenClipToScreen(fscreen_scr_arg *arg, fscreen_scr_t screen,
    int *x, int *y, int w, int h)
{
	int	 sx;
	int	 sy;
	int	 sw;
	int	 sh;
	int	 lx = (x) ? *x : 0;
	int	 ly = (y) ? *y : 0;
	int	 x_grav = GRAV_POS;
	int	 y_grav = GRAV_POS;

	FScreenGetScrRect(arg, screen, &sx, &sy, &sw, &sh);
	if (lx + w > sx + sw) {
		lx = sx + sw - w;
		x_grav = GRAV_NEG;
	}
	if (ly + h > sy + sh) {
		ly = sy + sh - h;
		y_grav = GRAV_NEG;
	}
	if (lx < sx) {
		lx = sx;
		x_grav = GRAV_POS;
	}
	if (ly < sy) {
		ly = sy;
		y_grav = GRAV_POS;
	}
	if (x)
		*x = lx;

	if (y)
		*y = ly;

	return (grav_matrix[y_grav][x_grav]);
}

/* Arguments work exactly like for FScreenGetScrRect() */
void
FScreenCenterOnScreen(fscreen_scr_arg *arg, fscreen_scr_t screen,
    int *x, int *y, int w, int h)
{
	int	 sx;
	int	 sy;
	int	 sw;
	int	 sh;
	int	 lx;
	int	 ly;

	FScreenGetScrRect(arg, screen, &sx, &sy, &sw, &sh);
	lx = (sw - w) / 2;
	ly = (sh - h) / 2;
	if (lx < 0)
		lx = 0;
	if (ly < 0)
		ly = 0;
	lx += sx;
	ly += sy;
	if (x)
		*x = lx;
	if (y)
		*y = ly;
}

void
FScreenGetResistanceRect(int wx, int wy, unsigned int ww, unsigned int wh,
    int *x0, int *y0, int *x1, int *y1)
{
	fscreen_scr_arg	 arg;

	arg.xypos.x = wx + ww / 2;
	arg.xypos.y = wy + wh / 2;
	FScreenGetScrRect(&arg, FSCREEN_XYPOS, x0, y0, x1, y1);
	*x1 += *x0;
	*y1 += *y0;
}

/* Arguments work exactly like for FScreenGetScrRect() */
Bool
FScreenIsRectangleOnScreen(fscreen_scr_arg *arg, fscreen_scr_t screen,
    rectangle *rec)
{
	int	 sx;
	int	 sy;
	int	 sw;
	int	 sh;

	FScreenGetScrRect(arg, screen, &sx, &sy, &sw, &sh);

	return (rec->x + rec->width > sx && rec->x < sx + sw &&
	    rec->y + rec->height > sy && rec->y < sy + sh) ? True : False;
}

/*
 * FScreenParseGeometry
 *     Does the same as XParseGeometry, but handles additional "@scr".
 *     Since it isn't safe to define "ScreenValue" constant (actual values
 *     of other "XXXValue" are specified in Xutil.h, not by us, so there can
 *     be a clash), the screen value is always returned, even if it wasn't
 *     present in `parse_string' (set to default in that case).
 *
 */
int
FScreenParseGeometryWithScreen(char *parsestring, int *x_return,
    int *y_return, unsigned int *width_return, unsigned int *height_return,
    char **screen_return)
{
	char           *copy, *geom_str = NULL;
	int             ret;

	/* Safety net */
	if (parsestring == NULL || *parsestring == '\0')
		return (0);

	/*
	 * If the geometry specification contains an '@' symbol, assume the
	 * * screen is specified.  This must be the name of the monitor in
	 * * question!
	 */
	if (strchr(parsestring, '@') == NULL) {
		copy = xstrdup(parsestring);
		goto parse_geometry;
	}

	copy = xstrdup(parsestring);
	copy = strsep(&parsestring, "@");
	*screen_return = parsestring;
	geom_str = strsep(&copy, "@");
	copy = geom_str;

parse_geometry:
	/* Do the parsing */
	ret =
	    XParseGeometry(copy, x_return, y_return, width_return,
	    height_return);

	return (ret);
}

/* Same as above, but dump screen return value to keep compatible with the X
 * function.
 */
int
FScreenParseGeometry(char *parsestring, int *x_return, int *y_return,
    unsigned int *width_return, unsigned int *height_return)
{
	struct monitor *m;
	char           *scr = NULL;
	int             rc;

	rc = FScreenParseGeometryWithScreen(parsestring, x_return, y_return,
	    width_return, height_return, &scr);

	if (scr != NULL)
		m = monitor_by_name(scr);
	else
		m = monitor_get_current();

	/*
	 * adapt geometry to selected screen
	 */
	if (rc & XValue) {
		if (rc & XNegative)
			*x_return -= (m->coord.w - m->coord.x);
		else
			*x_return += m->coord.x;
	}
	if (rc & YValue) {
		if (rc & YNegative)
			*y_return -= (m->coord.h - m->coord.y);
		else
			*y_return += m->coord.y;
	}
	return (rc);
}

/*  FScreenGetGeometry
 *      Parses the geometry in a form: XGeometry[@screen], i.e.
 *          [=][<width>{xX}<height>][{+-}<xoffset>{+-}<yoffset>][@<screen>]
 *          where <screen> is either a number or "G" (global) "C" (current)
 *          or "P" (primary)
 *
 *  Args:
 *      parsestring, x_r, y_r, w_r, h_r  the same as in XParseGeometry
 *      hints  window hints structure, may be NULL
 *      flags  bitmask of allowed flags (XValue, WidthValue, XNegative...)
 *
 *  Note1:
 *      hints->width and hints->height will be used to calc negative geometry
 *      if width/height isn't specified in the geometry itself.
 *
 *  Note2:
 *      This function's behaviour is crafted to sutisfy/emulate the
 *      MvwmWinList::MakeMeWindow()'s behaviour.
 *
 *  Note3:
 *      A special value of `flags' when [XY]Value are there but [XY]Negative
 *      aren't, means that in case of negative geometry specification
 *      x_r/y_r values will be promoted to the screen border, but w/h
 *      wouldn't be subtracted, so that the program can do x-=w later
 *      ([XY]Negative *will* be returned, albeit absent in `flags').
 *          This option is supposed for proggies like MvwmButtons, which
 *      receive geometry specification long before they are able to actually
 *      use it (and which calculate w/h themselves).
 *          (The same effect can't be obtained with omitting {Width,Height}Value
 *      in the flags, since the app may wish to get the dimensions but apply
 *      some constraints later (as MvwmButtons do, BTW...).)
 *          This option can be also useful in cases where dimensions are
 *      specified not in pixels but in some other units (e.g., charcells).
 */
int
FScreenGetGeometry(char *parsestring, int *x_return, int *y_return,
    int *width_return, int *height_return, XSizeHints * hints, int flags)
{
	fscreen_scr_arg arg;
	char           *scr = NULL;
	int             ret;
	int             saved;
	int             x, y;
	unsigned int    w = 0, h = 0;
	int             grav, x_grav, y_grav;
	int             scr_x, scr_y;
	int             scr_w, scr_h;

	/*
	 * I. Do the parsing and strip off extra bits
	 */
	ret =
	    FScreenParseGeometryWithScreen(parsestring, &x, &y, &w, &h, &scr);
	saved = ret & (XNegative | YNegative);
	ret &= flags;

	arg.mouse_ev = NULL;
	arg.name = scr;
	FScreenGetScrRect(&arg, FSCREEN_BY_NAME, &scr_x, &scr_y, &scr_w,
	    &scr_h);

	/*
	 * II. Interpret and fill in the values
	 */

	/*
	 * Fill in dimensions for future negative calculations if
	 * * omitted/forbidden
	 */
	/*
	 * Maybe should use *x_return,*y_return if hints==NULL?
	 * * Unreliable...
	 */
	if (hints != NULL && hints->flags & PSize) {
		if ((ret & WidthValue) == 0)
			w = hints->width;

		if ((ret & HeightValue) == 0)
			h = hints->height;
	} else {
		/*
		 * This branch is required for case when size *is* specified,
		 * * but masked off
		 */
		if ((ret & WidthValue) == 0)
			w = 0;

		if ((ret & HeightValue) == 0)
			h = 0;
	}

	/* Advance coords to the screen... */
	x += scr_x;
	y += scr_y;

	/* ...and process negative geometries */
	if (saved & XNegative)
		x += scr_w;

	if (saved & YNegative)
		y += scr_h;

	if (ret & XNegative)
		x -= w;

	if (ret & YNegative)
		y -= h;

	/* Restore negative bits */
	ret |= saved;

	/* Guess orientation */
	x_grav = (ret & XNegative) ? GRAV_NEG : GRAV_POS;
	y_grav = (ret & YNegative) ? GRAV_NEG : GRAV_POS;
	grav = grav_matrix[y_grav][x_grav];

	/* Return the values */
	if (ret & XValue) {
		*x_return = x;
		if (hints != NULL)
			hints->x = x;
	}
	if (ret & YValue) {
		*y_return = y;
		if (hints != NULL)
			hints->y = y;
	}
	if (ret & WidthValue) {
		*width_return = w;
		if (hints != NULL)
			hints->width = w;
	}
	if (ret & HeightValue) {
		*height_return = h;
		if (hints != NULL)
			hints->height = h;
	}
	if (1 /*flags & GravityValue */  && grav != DEFAULT_GRAVITY) {
		if (hints != NULL && hints->flags & PWinGravity)
			hints->win_gravity = grav;
	}
	if (hints != NULL && ret & XValue && ret & YValue)
		hints->flags |= USPosition;

	return (ret);
}

/*  FScreenMangleScreenIntoUSPosHints
 *      A hack to mangle the screen number into the XSizeHints structure.
 *      If the USPosition flag is set, hints->x is set to the magic number and
 *      hints->y is set to the screen number.  If the USPosition flag is clear,
 *      x and y are set to zero.
 *
 *  Note:  This is a *hack* to allow modules to specify the target screen for
 *  their windows and have the StartsOnScreen style set for them at the same
 *  time.  Do *not* rely on the mechanism described above.
 */
void
FScreenMangleScreenIntoUSPosHints(fscreen_scr_t screen, XSizeHints * hints)
{
	if (hints->flags & USPosition) {
		hints->x = FSCREEN_MANGLE_USPOS_HINTS_MAGIC;
		hints->y = (short) screen;
	} else {
		hints->x = 0;
		hints->y = 0;
	}
}

/*  FScreenMangleScreenIntoUSPosHints
 *      A hack to mangle the screen number into the XSizeHints structure.
 *      If the USPosition flag is set, hints->x is set to the magic number and
 *      hints->y is set to the screen spec.  If the USPosition flag is clear,
 *      x and y are set to zero.
 *
 *  Note:  This is a *hack* to allow modules to specify the target screen for
 *  their windows and have the StartsOnScreen style set for them at the same
 *  time.  Do *not* rely on the mechanism described above.
 */
int
FScreenFetchMangledScreenFromUSPosHints(XSizeHints * hints)
{
	int	 screen;

	if ((hints->flags & USPosition) &&
	    hints->x == FSCREEN_MANGLE_USPOS_HINTS_MAGIC) {
		screen = hints->y;
	} else
		screen = 0;

	return screen;
}
