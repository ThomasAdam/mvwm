/* -*-c-*- */
#ifndef MVWMLIB_FSCRREN_H
#define MVWMLIB_FSCRREN_H

#include "mvwm/mvwm.h"
#include "mvwm/execcontext.h"
#include "mvwm/misc.h"
#include "mvwm/screen.h"

typedef union
{
	XEvent *mouse_ev;
	const char *name;
	struct
	{
		int x;
		int y;
	} xypos;
} fscreen_scr_arg;

typedef enum
{
	FSCREEN_GLOBAL  = -1,
	FSCREEN_CURRENT = -2,
	FSCREEN_PRIMARY = -3,
	FSCREEN_XYPOS   = -4,
	FSCREEN_BY_NAME = -5
} fscreen_scr_t;

struct monitor {
	char	*name;
	int number;
	struct {
		int x;
		int y;
		int w;
		int h;
	} coord;

	/* info for some desktops; the first entries should be generic info
	 * correct for any desktop not in the list
	 */
	DesktopsInfo	*Desktops;

	/* Information about EWMH. */
	struct {
		unsigned NumberOfDesktops;
		unsigned MaxDesktops;
		unsigned CurrentNumberOfDesktops;
		Bool NeedsToCheckDesk;

		struct {
			int left;
			int right;
			int top;
			int bottom;
		} BaseStrut;

	} ewmhc;

	struct {
		int VxMax;
		int VyMax;
		int Vx;
		int Vy;

		int EdgeScrollX;
		int EdgeScrollY;

		int CurrentDesk;
		int prev_page_x;
		int prev_page_y;
		int prev_desk;
		int prev_desk_and_page_desk;
		int prev_desk_and_page_page_x;
		int prev_desk_and_page_page_y;
	} virtual_scr;

	TAILQ_ENTRY(monitor) entry;
};
TAILQ_HEAD(monitors, monitor);

struct monitors	monitor_q;
struct monitor *global_monitor;

struct monitor	*monitor_get_current(void);
struct monitor	*monitor_by_name(const char *);
struct monitor	*monitor_by_xy(int, int);
struct monitor  *monitor_by_number(int);

#define FSCREEN_MANGLE_USPOS_HINTS_MAGIC ((short)-32109)


/* Control */
Bool FScreenIsEnabled(void);
void FScreenInit(Display *dpy);
void FScreenSetDefaultModuleScreen(char *scr_spec);

void FScreenSetPrimaryScreen(int scr);

/* Screen info */
Bool FScreenGetScrRect(fscreen_scr_arg *, fscreen_scr_t,
	int *, int *, int *, int *);
Bool FScreenGetScrId(fscreen_scr_arg *arg);
void FScreenTranslateCoordinates(
	fscreen_scr_arg *arg_src, fscreen_scr_t screen_src,
	fscreen_scr_arg *arg_dest, fscreen_scr_t screen_dest,
	int *x, int *y);
void FScreenGetResistanceRect(
	int wx, int wy, unsigned int ww, unsigned int wh, int *x0, int *y0,
	int *x1, int *y1);
Bool FScreenIsRectangleOnScreen(fscreen_scr_arg *, fscreen_scr_t,rectangle *);
const char	*FScreenOfPointerXY(int, int);


/* Clipping/positioning */
int FScreenClipToScreen(fscreen_scr_arg *, fscreen_scr_t,
	int *x, int *y, int w, int h);
void FScreenCenterOnScreen(fscreen_scr_arg *, fscreen_scr_t,
	int *x, int *y, int w, int h);

/* Geometry management */
int FScreenParseGeometryWithScreen(
	char *parsestring, int *x_return, int *y_return,
	unsigned int *width_return, unsigned int *height_return,
	char **screen_return);
int FScreenParseGeometry(
	char *parsestring, int *x_return, int *y_return,
	unsigned int *width_return, unsigned int *height_return);
int  FScreenGetGeometry(
	char *parsestring, int *x_return, int *y_return,
	int *width_return, int *height_return, XSizeHints *hints, int flags);
void FScreenMangleScreenIntoUSPosHints(fscreen_scr_t screen, XSizeHints *hints);
int FScreenFetchMangledScreenFromUSPosHints(XSizeHints *hints);

#endif /* MVWMLIB_FSCRREN_H */
