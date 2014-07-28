/* -*-c-*- */
#ifndef MVWMLIB_FSCRREN_H
#define MVWMLIB_FSCRREN_H

#include "mvwm/mvwm.h"
#include "mvwm/execcontext.h"
#include "mvwm/misc.h"
#include "mvwm/screen.h"

typedef union
{
	XEvent         *mouse_ev;
	const char     *name;
	struct
	{
		int	 x;
		int	 y;
	} xypos;
} fscreen_scr_arg;

typedef enum
{
	FSCREEN_GLOBAL = -1,
	FSCREEN_CURRENT = -2,
	FSCREEN_PRIMARY = -3,
	FSCREEN_XYPOS = -4,
	FSCREEN_BY_NAME = -5
} fscreen_scr_t;

struct monitor
{
	char           *name;
	struct
	{
		int	 x;
		int	 y;
		int	 w;
		int	 h;
	} coord;

	/*
	 * info for some desktops; the first entries should be generic info
	 * * correct for any desktop not in the list
	 */
	DesktopsInfo   *Desktops;

	struct
	{
		int	 VxMax;
		int	 VyMax;
		int	 Vx;
		int	 Vy;

		int	 EdgeScrollX;
		int	 EdgeScrollY;

		int	 CurrentDesk;
		int	 prev_page_x;
		int	 prev_page_y;
		int	 prev_desk;
		int	 prev_desk_and_page_desk;
		int	 prev_desk_and_page_page_x;
		int	 prev_desk_and_page_page_y;
	} virtual_scr;

	TAILQ_ENTRY(monitor) entry;
};
TAILQ_HEAD(monitors, monitor);

struct monitors	 monitor_q;

struct monitor	*monitor_get_current(void);
struct monitor	*monitor_by_name(const char *);
struct monitor	*monitor_by_xy(int, int);
int		 monitor_should_ignore_global(struct monitor *);

#define FSCREEN_MANGLE_USPOS_HINTS_MAGIC ((short)-32109)

/* Control */
Bool	 FScreenIsEnabled(void);
void	 FScreenInit(Display *);
/* Intended to be called by modules.  Simply pass in the parameter from the
 * config string sent by mvwm. */
void		FScreenConfigureModule(char *);
const char     *FScreenGetConfiguration(void);	/* For use by mvwm */
void            FScreenSetDefaultModuleScreen(char *);

void            FScreenSetPrimaryScreen(int);

/* Screen info */
Bool            FScreenGetScrRect(fscreen_scr_arg *, fscreen_scr_t,
    int *, int *, int *, int *);
Bool            FScreenGetScrId(fscreen_scr_arg *);
void            FScreenTranslateCoordinates(fscreen_scr_arg *,
    fscreen_scr_t, fscreen_scr_arg *, fscreen_scr_t, int *, int *);
void            FScreenGetResistanceRect(int, int, unsigned int,
    unsigned int, int *, int *, int *, int *);
Bool            FScreenIsRectangleOnScreen(fscreen_scr_arg *, fscreen_scr_t,
    rectangle *);
const char     *FScreenOfPointerXY(int, int);

/* Clipping/positioning */
int	 FScreenClipToScreen(fscreen_scr_arg *, fscreen_scr_t, int *, int *,
    int, int);
void	 FScreenCenterOnScreen(fscreen_scr_arg *, fscreen_scr_t, int *, int *,
    int, int);

/* Geometry management */
int	 FScreenParseGeometryWithScreen(char *, int *, int *, unsigned int *,
    unsigned int *, char **);
int	 FScreenParseGeometry(char *, int *, int *, unsigned int *,
    unsigned int *);
int	 FScreenGetGeometry(char *, int *, int *, int *, int *, XSizeHints *,
    int);
void	 FScreenMangleScreenIntoUSPosHints(fscreen_scr_t, XSizeHints *);
int	 FScreenFetchMangledScreenFromUSPosHints(XSizeHints *);

#endif /* MVWMLIB_FSCRREN_H */
