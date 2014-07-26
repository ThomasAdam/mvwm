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
  This file is strongly based on the corresponding files from
  twm and enlightenment.
*/

/* ---------------------------- included header files ---------------------- */

#include "config.h"

#include <stdio.h>
#ifdef HAVE_GETPWUID
#include <pwd.h>
#endif
#include <signal.h>
#include <fcntl.h>
#include <X11/Xatom.h>

#include "libs/mvwmlib.h"
#include "libs/Strings.h"
#include "libs/System.h"
#include "mvwm.h"
#include "externs.h"
#include "execcontext.h"
#include "add_window.h"
#include "misc.h"
#include "screen.h"
#include "session.h"
#include "module_list.h"
#include "module_interface.h"
#include "stack.h"
#include "icccm2.h"
#include "virtual.h"
#include "geometry.h"
#include "move_resize.h"

/* ---------------------------- local macros ------------------------------- */

#define xstreq(a,b) ((!a && !b) || (a && b && (strcmp(a,b)==0)))

/* ---------------------------- imports ------------------------------------ */

extern Bool     Restarting;
extern int      master_pid;
extern char   **g_argv;
extern int      g_argc;

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

typedef struct _match
{
	unsigned long   win;
	char           *client_id;
	char           *res_name;
	char           *res_class;
	char           *window_role;
	char           *wm_name;
	int             wm_command_count;
	char          **wm_command;
	int             x, y, w, h, icon_x, icon_y;
	int             x_max, y_max, w_max, h_max;
	int             width_defect_max, height_defect_max;
	int             max_x_offset, max_y_offset;
	int             desktop;
	int             layer;
	int             default_layer;
	int             placed_by_button;
	int             used;
	int             gravity;
	unsigned long   ewmh_hint_desktop;
	window_flags    flags;
} Match;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- local variables ---------------------------- */

static int      num_match = 0;
static Match   *matches = NULL;
static Bool     does_file_version_match = False;
static Bool     do_preserve_state = True;

/* ---------------------------- exported variables (globals) --------------- */

int             sm_fd = -1;

/* ---------------------------- local functions ---------------------------- */

static char    *
duplicate(const char *s)
{
	int             l;
	char           *r;

	/*
	 * TA:  FIXME!  Use xasprintf()
	 */

	if (!s)
		return NULL;
	l = strlen(s);
	r = xmalloc(sizeof(char) * (l + 1));
	strncpy(r, s, l + 1);

	return r;
}

static char    *
get_version_string(void)
{
	/*
	 * migo (14-Mar-2001): it is better to manually update a version string
	 * * in the stable branch, otherwise saving sessions becomes useless
	 */
	return CatString3(VERSION, ", ", __DATE__);
	/*
	 * return "2.6-0";
	 */
}

static char    *
unspace_string(const char *str)
{
	static const char *spaces = " \t\n";
	char           *tr_str = CatString2(str, NULL);
	int             i;

	if (!tr_str) {
		return NULL;
	}
	for (i = 0; i < strlen(spaces); i++) {
		char           *ptr = tr_str;
		while ((ptr = strchr(ptr, spaces[i])) != NULL) {
			*(ptr++) = '_';
		}
	}

	return tr_str;
}

/*
 * It is a bit ugly to have a separate file format for
 * config files and session save files. The proper way
 * to do this may be to extend the config file format
 * to allow the specification of everything we need
 * to save here. Then the option "-restore xyz" could
 * be replaced by "-f xyz".
 */
static int
SaveGlobalState(FILE * f)
{
	/*
	 * TA:  FIXME!  This can't be right!
	 */
	struct monitor *m = monitor_by_name("global");
	fprintf(f, "[GLOBAL]\n");
	fprintf(f, "  [DESKTOP] %i\n", m->virtual_scr.CurrentDesk);
	fprintf(f, "  [VIEWPORT] %i %i %i %i\n",
	    m->virtual_scr.Vx, m->virtual_scr.Vy, m->virtual_scr.VxMax,
	    m->virtual_scr.VyMax);
	fprintf(f, "  [SCROLL] %i %i %i %i %i\n",
	    m->virtual_scr.EdgeScrollX, m->virtual_scr.EdgeScrollY,
	    Scr.ScrollDelay,
	    ! !(scr_flags.do_edge_wrap_x), ! !(scr_flags.do_edge_wrap_y));
	fprintf(f, "  [MISC] %i %i %i\n",
	    Scr.ClickTime, Scr.ColormapFocus, Scr.ColorLimit);
	fprintf(f, "  [STYLE] %i %i\n", gso.do_emulate_mwm,
	    gso.do_emulate_win);

	return 1;
}

static char    *
GetWindowRole(Window window)
{
	XTextProperty   tp;

	if (XGetTextProperty(dpy, window, &tp, _XA_WM_WINDOW_ROLE)) {
		if (tp.encoding == XA_STRING && tp.format == 8 &&
		    tp.nitems != 0) {
			return ((char *) tp.value);
		}
	}
	if (XGetTextProperty(dpy, window, &tp, _XA_WINDOW_ROLE)) {
		if (tp.encoding == XA_STRING && tp.format == 8 &&
		    tp.nitems != 0) {
			return ((char *) tp.value);
		}
	}

	return NULL;
}

static char    *
GetClientID(MvwmWindow *fw)
{
	char           *client_id = NULL;
	Window          client_leader = None;
	Window          window;
	Atom            actual_type;
	int             actual_format;
	unsigned long   nitems;
	unsigned long   bytes_after;
	unsigned char  *prop = NULL;

	window = FW_W(fw);

	if (XGetWindowProperty(dpy, window, _XA_WM_CLIENT_LEADER, 0L, 1L,
		False, AnyPropertyType, &actual_type, &actual_format, &nitems,
		&bytes_after, &prop) == Success) {
		if (actual_type == XA_WINDOW && actual_format == 32
		    && nitems == 1 && bytes_after == 0) {
			client_leader = (Window) (*(long *) prop);
		}
	}

	if (!client_leader && fw->wmhints &&
	    (fw->wmhints->flags & WindowGroupHint)) {
		client_leader = fw->wmhints->window_group;
	}

	if (prop) {
		XFree(prop);
	}

	return client_id;
}

/*
**  Verify the current mvwm version with the version that stroed the state file.
**  No state will be restored if versions don't match.
*/
static Bool
VerifyVersionInfo(char *filename)
{
	FILE           *f;
	char            s[4096], s1[4096];

	if (!filename || !*filename) {
		return False;
	}
	if ((f = fopen(filename, "r")) == NULL) {
		return False;
	}

	while (fgets(s, sizeof(s), f)) {
		sscanf(s, "%4000s", s1);
		if (!strcmp(s1, "[MVWM_VERSION]")) {
			char           *current_v = get_version_string();
			sscanf(s, "%*s %[^\n]", s1);
			if (strcmp(s1, current_v) == 0) {
				does_file_version_match = True;
			} else {
				mvwm_msg(ERR, "VerifyVersionInfo",
				    "State file version (%s) does not"
				    " match the current version (%s), "
				    "state file is ignored.", s1, current_v);
				break;
			}
		}
	}
	fclose(f);

	return does_file_version_match;
}

static int
SaveVersionInfo(FILE * f)
{
	fprintf(f, "[MVWM_VERSION] %s\n", get_version_string());

	return 1;
}

static int
SaveWindowStates(FILE * f)
{
	char           *client_id;
	char           *window_role;
	char          **wm_command;
	int             wm_command_count;
	MvwmWindow     *ewin;
	struct monitor *m;
	rectangle       save_g;
	rectangle       ig;
	int             i;
	int             layer;

	for (ewin = get_next_window_in_stack_ring(&Scr.MvwmRoot);
	    ewin != &Scr.MvwmRoot;
	    ewin = get_next_window_in_stack_ring(ewin)) {
		Bool            is_icon_sticky_across_pages;

		if (!XGetGeometry(dpy, FW_W(ewin), &JunkRoot, &JunkX, &JunkY,
			(unsigned int *) &JunkWidth,
			(unsigned int *) &JunkHeight,
			(unsigned int *) &JunkBW,
			(unsigned int *) &JunkDepth)) {
			/*
			 * Don't save the state of windows that already died
			 * * (i.e. modules)!
			 */
			continue;
		}
		m = ewin->m;
		is_icon_sticky_across_pages =
		    is_window_sticky_across_pages(ewin);

		wm_command = NULL;
		wm_command_count = 0;

		client_id = GetClientID(ewin);
		if (!client_id) {
			/*
			 * no client id, some session manager do not manage
			 * * such client ... this can cause problem
			 */
			if (XGetCommand(dpy, FW_W(ewin), &wm_command,
				&wm_command_count) &&
			    wm_command && wm_command_count > 0) {
				/*
				 * ok
				 */
			} else {
				/*
				 * No client id and no WM_COMMAND, the client
				 * * cannot be managed by the sessiom manager
				 * * skip it!
				 */
				/*
				 * TA:  20110611 - But actually, this breaks
				 * * those applications which don't set the
				 * * WM_COMMAND XAtom anymore.  The ICCCM
				 * * deprecated this at version 2.0 -- and its
				 * * lack of existence here shouldn't be a
				 * * problem.  Let newer session managers handle
				 * * the error if it even matters.
				 */
				if (!Restarting) {
					if (wm_command) {
						XFreeStringList(wm_command);
						wm_command = NULL;
					}
					/*
					 * TA: 20110611 - But see above.  We
					 * * no longer skip clients who don't
					 * * set this legacy field.
					 */
					/*
					 * continue;
					 */
				}
			}
		}

		fprintf(f, "[CLIENT] %lx\n", FW_W(ewin));
		if (client_id) {
			fprintf(f, "  [CLIENT_ID] %s\n", client_id);
			XFree(client_id);
		}

		window_role = GetWindowRole(FW_W(ewin));
		if (window_role) {
			fprintf(f, "  [WINDOW_ROLE] %s\n", window_role);
			XFree(window_role);
		}
		if (client_id && window_role) {
			/*
			 * we have enough information
			 */
		} else {
			if (ewin->class.res_class) {
				fprintf(f, "  [RES_NAME] %s\n",
				    ewin->class.res_name);
			}
			if (ewin->class.res_name) {
				fprintf(f, "  [RES_CLASS] %s\n",
				    ewin->class.res_class);
			}
			if (ewin->name.name) {
				fprintf(f, "  [WM_NAME] %s\n",
				    ewin->name.name);
			}

			if (wm_command && wm_command_count > 0) {
				fprintf(f, "  [WM_COMMAND] %i",
				    wm_command_count);
				for (i = 0; i < wm_command_count; i++) {
					fprintf(f, " %s",
					    unspace_string(wm_command[i]));
				}
				fprintf(f, "\n");
			}
		}	/* !window_role */

		if (wm_command) {
			XFreeStringList(wm_command);
			wm_command = NULL;
		}

		gravity_get_naked_geometry(ewin->hints.win_gravity, ewin,
		    &save_g, &ewin->g.normal);
		if (IS_STICKY_ACROSS_PAGES(ewin)) {
			save_g.x -= m->virtual_scr.Vx;
			save_g.y -= m->virtual_scr.Vy;
		}
		get_visible_icon_geometry(ewin, &ig);
		fprintf(f, "  [GEOMETRY] %i %i %i %i %i %i %i %i %i %i %i %i"
		    " %i %i %i\n",
		    save_g.x, save_g.y, save_g.width, save_g.height,
		    ewin->g.max.x, ewin->g.max.y, ewin->g.max.width,
		    ewin->g.max.height, ewin->g.max_defect.width,
		    ewin->g.max_defect.height,
		    ig.x +
		    ((!is_icon_sticky_across_pages) ? m->virtual_scr.Vx : 0),
		    ig.y +
		    ((!is_icon_sticky_across_pages) ? m->virtual_scr.Vy : 0),
		    ewin->hints.win_gravity, ewin->g.max_offset.x,
		    ewin->g.max_offset.y);
		fprintf(f, "  [DESK] %i\n", ewin->Desk);
		/*
		 * set the layer to the default layer if the layer has been
		 * * set by an ewmh hint
		 */
		layer = get_layer(ewin);
		if (layer == ewin->ewmh_hint_layer && layer > 0) {
			layer = Scr.DefaultLayer;
		}
		fprintf(f, "  [LAYER] %i %i\n", layer, ewin->default_layer);
		fprintf(f, "  [PLACED_BY_BUTTON] %i\n",
		    ewin->placed_by_button);
		fprintf(f, "  [EWMH_DESKTOP] %lu\n", ewin->ewmh_hint_desktop);
		fprintf(f, "  [FLAGS] ");
		for (i = 0; i < sizeof(window_flags); i++) {
			fprintf(f, "%02x ",
			    (int) (((unsigned char *) &(ewin->flags))[i]));
		}
		fprintf(f, "\n");
	}
	return 1;
}

/* This complicated logic is from twm, where it is explained */
static Bool
matchWin(MvwmWindow *w, Match * m)
{
	char           *client_id = NULL;
	char           *window_role = NULL;
	char          **wm_command = NULL;
	int             wm_command_count = 0, i;
	int             found;

	found = 0;
	client_id = GetClientID(w);

	if (Restarting) {
		if (FW_W(w) == m->win) {
			found = 1;
		}
	} else if (xstreq(client_id, m->client_id)) {

		/*
		 * client_id's match
		 */

		window_role = GetWindowRole(FW_W(w));

		if (client_id && (window_role || m->window_role)) {
			/*
			 * We have or had a window role, base decision on it
			 */
			found = xstreq(window_role, m->window_role);
		} else if (xstreq(w->class.res_name, m->res_name) &&
		    xstreq(w->class.res_class, m->res_class) &&
		    (IS_NAME_CHANGED(w) || IS_NAME_CHANGED(m) ||
			xstreq(w->name.name, m->wm_name))) {
			if (client_id) {
				/*
				 * If we have a client_id, we don't
				 * * compare WM_COMMAND, since it will be
				 * * different.
				 */
				found = 1;
			} else {
				/*
				 * for non-SM-aware clients we also
				 * * compare WM_COMMAND
				 */
				if (!XGetCommand(dpy, FW_W(w), &wm_command,
					&wm_command_count)) {
					wm_command = NULL;
					wm_command_count = 0;
				}
				if (wm_command_count == m->wm_command_count) {
					for (i = 0; i < wm_command_count; i++) {
						if (strcmp(unspace_string
							(wm_command[i]),
							m->wm_command[i]) !=
						    0) {
							break;
						}
					}

					if (i == wm_command_count) {
						/*
						 * migo (21/Oct/1999):
						 * * on restarts compare
						 * * window ids too
						 */
						/*
						 * But if we restart we only need
						 * * to compare window ids
						 * * olicha (2005-01-06)
						 */
						found = 1;
					}
				}	/* if (wm_command_count ==... */
			}	/* else if res_class, res_name and wm_name agree */
		}	/* else no window roles */
	}
	/*
	 * if client_id's agree
	 */
#ifdef MVWM_SM_DEBUG_WINMATCH
	fprintf(stderr,
	    "\twin(%s, %s, %s, %s, %s,",
	    w->class.res_name, w->class.res_class, w->name.name,
	    (client_id) ? client_id : "(null)",
	    (window_role) ? window_role : "(null)");
	if (wm_command) {
		for (i = 0; i < wm_command_count; i++) {
			fprintf(stderr, " %s", wm_command[i]);
		}
		fprintf(stderr, ",");
	} else {
		fprintf(stderr, " no_wmc,");
	}
	fprintf(stderr, " %d)", IS_NAME_CHANGED(w));
	fprintf(stderr, "\n[%d]", found);
	fprintf(stderr,
	    "\tmat(%s, %s, %s, %s, %s,",
	    m->res_name, m->res_class, m->wm_name,
	    (m->client_id) ? m->client_id : "(null)",
	    (m->window_role) ? m->window_role : "(null)");
	if (m->wm_command) {
		for (i = 0; i < m->wm_command_count; i++) {
			fprintf(stderr, " %s", m->wm_command[i]);
		}
		fprintf(stderr, ",");
	} else {
		fprintf(stderr, " no_wmc,");
	}
	fprintf(stderr, " %d)\n\n", IS_NAME_CHANGED(m));
#endif

	if (client_id) {
		XFree(client_id);
	}
	if (window_role) {
		XFree(window_role);
	}
	if (wm_command) {
		XFreeStringList(wm_command);
	}

	return found;
}

static int
save_state_file(char *filename)
{
	FILE           *f;
	int             success;

	if (!filename || !*filename) {
		return 0;
	}
	if ((f = fopen(filename, "w")) == NULL) {
		return 0;
	}

	fprintf(f, "# This file is generated by mvwm."
	    " It stores global and window states.\n");
	fprintf(f, "# Normally, you must never delete this file,"
	    " it will be auto-deleted.\n\n");

	success = do_preserve_state
	    ? SaveVersionInfo(f) && SaveWindowStates(f) && SaveGlobalState(f)
	    : 1;
	do_preserve_state = True;
	if (fclose(f) != 0)
		return 0;

	return success;
}

/* ---------------------------- interface functions ------------------------ */

void
LoadGlobalState(char *filename)
{
	FILE           *f;
	char            s[4096], s1[4096];
	/*
	 * char s2[256];
	 */
	int             i1, i2, i3, i4;

	/*
	 * TA:  20140630:  FIXME:  Honour monitor!
	 */
	struct monitor *mon = monitor_get_current();

	if (!does_file_version_match) {
		return;
	}
	if (!filename || !*filename) {
		return;
	}
	if ((f = fopen(filename, "r")) == NULL) {
		return;
	}

	while (fgets(s, sizeof(s), f)) {
		i1 = 0;
		i2 = 0;
		i3 = 0;
		i4 = 0;
		sscanf(s, "%4000s", s1);
		if (!strcmp(s1, "[DESKTOP]")) {
			sscanf(s, "%*s %i", &i1);
			goto_desk(i1, mon);
		} else if (!strcmp(s1, "[VIEWPORT]")) {
			sscanf(s, "%*s %i %i %i %i", &i1, &i2, &i3, &i4);
			/*
			 * migo: we don't want to lose DeskTopSize in
			 * * configurations, and it does not work well
			 * * anyways - Gnome is not updated
			 * Scr.VxMax = i3;
			 * Scr.VyMax = i4;
			 */
			MoveViewport(mon, i1, i2, True);
		}
	}
	fclose(f);

	return;
}

void
DisableRestoringState(void)
{
	num_match = 0;

	return;
}

void
LoadWindowStates(char *filename)
{
	FILE           *f;
	char            s[4096], s1[4096];
	char           *s2;
	int             i, pos, pos1;
	unsigned long   w;
	int             n;
	struct monitor *m = monitor_by_name("global");

	if (!VerifyVersionInfo(filename)) {
		return;
	}
	if (!filename || !*filename) {
		return;
	}
	if ((f = fopen(filename, "r")) == NULL) {
		return;
	}

	while (fgets(s, sizeof(s), f)) {
		n = 0;
		sscanf(s, "%4000s%n", s1, &n);
		if (!strcmp(s1, "[CLIENT]")) {
			sscanf(s, "%*s %lx", &w);
			num_match++;
			matches = xrealloc(
			    (void *) matches, sizeof(Match), num_match);
			matches[num_match - 1].win = w;
			matches[num_match - 1].client_id = NULL;
			matches[num_match - 1].res_name = NULL;
			matches[num_match - 1].res_class = NULL;
			matches[num_match - 1].window_role = NULL;
			matches[num_match - 1].wm_name = NULL;
			matches[num_match - 1].wm_command_count = 0;
			matches[num_match - 1].wm_command = NULL;
			matches[num_match - 1].x = 0;
			matches[num_match - 1].y = 0;
			matches[num_match - 1].w = 100;
			matches[num_match - 1].h = 100;
			matches[num_match - 1].x_max = 0;
			matches[num_match - 1].y_max = 0;
			matches[num_match - 1].w_max = m->coord.w;
			matches[num_match - 1].h_max = m->coord.h;
			matches[num_match - 1].width_defect_max = 0;
			matches[num_match - 1].height_defect_max = 0;
			matches[num_match - 1].icon_x = 0;
			matches[num_match - 1].icon_y = 0;
			matches[num_match - 1].desktop = 0;
			matches[num_match - 1].layer = 0;
			matches[num_match - 1].default_layer = 0;
			memset(&(matches[num_match - 1].flags), 0,
			    sizeof(window_flags));
			matches[num_match - 1].used = 0;
		} else if (!strcmp(s1, "[GEOMETRY]")) {
			sscanf(s, "%*s %i %i %i %i %i %i %i %i %i %i %i %i"
			    " %i %i %i",
			    &(matches[num_match - 1].x),
			    &(matches[num_match - 1].y),
			    &(matches[num_match - 1].w),
			    &(matches[num_match - 1].h),
			    &(matches[num_match - 1].x_max),
			    &(matches[num_match - 1].y_max),
			    &(matches[num_match - 1].w_max),
			    &(matches[num_match - 1].h_max),
			    &(matches[num_match - 1].width_defect_max),
			    &(matches[num_match - 1].height_defect_max),
			    &(matches[num_match - 1].icon_x),
			    &(matches[num_match - 1].icon_y),
			    &(matches[num_match - 1].gravity),
			    &(matches[num_match - 1].max_x_offset),
			    &(matches[num_match - 1].max_y_offset));
		} else if (!strcmp(s1, "[DESK]")) {
			sscanf(s, "%*s %i",
			    &(matches[num_match - 1].desktop));
		} else if (!strcmp(s1, "[LAYER]")) {
			sscanf(s, "%*s %i %i",
			    &(matches[num_match - 1].layer),
			    &(matches[num_match - 1].default_layer));
		} else if (!strcmp(s1, "[PLACED_BY_BUTTON]")) {
			sscanf(s, "%*s %i",
			    &(matches[num_match - 1].placed_by_button));
		} else if (!strcmp(s1, "[EWMH_DESKTOP]")) {
			sscanf(s, "%*s %lu",
			    &(matches[num_match - 1].ewmh_hint_desktop));
		} else if (!strcmp(s1, "[FLAGS]")) {
			char           *ts = s;

			/*
			 * skip [FLAGS]
			 */
			while (*ts != ']') {
				ts++;
			}
			ts++;

			for (i = 0; i < sizeof(window_flags); i++) {
				unsigned int    f;
				sscanf(ts, "%02x ", &f);
				((unsigned char *) &
				    (matches[num_match - 1].flags))[i] = f;
				ts += 3;
			}
		} else if (!strcmp(s1, "[CLIENT_ID]")) {
			s2 = s + n;
			if (*s2 != 0) {
				s2++;
			}
			sscanf(s2, "%[^\n]", s1);
			matches[num_match - 1].client_id = duplicate(s1);
		} else if (!strcmp(s1, "[WINDOW_ROLE]")) {
			s2 = s + n;
			if (*s2 != 0) {
				s2++;
			}
			sscanf(s2, "%[^\n]", s1);
			matches[num_match - 1].window_role = duplicate(s1);
		} else if (!strcmp(s1, "[RES_NAME]")) {
			s2 = s + n;
			if (*s2 != 0) {
				s2++;
			}
			sscanf(s2, "%[^\n]", s1);
			matches[num_match - 1].res_name = duplicate(s1);
		} else if (!strcmp(s1, "[RES_CLASS]")) {
			s2 = s + n;
			if (*s2 != 0) {
				s2++;
			}
			sscanf(s2, "%[^\n]", s1);
			matches[num_match - 1].res_class = duplicate(s1);
		} else if (!strcmp(s1, "[WM_NAME]")) {
			s2 = s + n;
			if (*s2 != 0) {
				s2++;
			}
			sscanf(s2, "%[^\n]", s1);
			matches[num_match - 1].wm_name = duplicate(s1);
		} else if (!strcmp(s1, "[WM_COMMAND]")) {
			sscanf(s, "%*s %i%n",
			    &matches[num_match - 1].wm_command_count, &pos);
			matches[num_match - 1].wm_command = (char **)
			    xmalloc(matches[num_match -
				1].wm_command_count * sizeof(char *));
			for (i = 0;
			    i < matches[num_match - 1].wm_command_count;
			    i++) {
				sscanf(s + pos, "%s%n", s1, &pos1);
				pos += pos1;
				matches[num_match - 1].wm_command[i] =
				    duplicate(s1);
			}
		}
	}
	fclose(f);

	return;
}

/*
  This routine (potentially) changes the flags STARTICONIC,
  MAXIMIZED, WSHADE and STICKY and the Desk and
  attr.x, .y, .width, .height entries. It also changes the
  stack_before pointer to return information about the
  desired stacking order. It expects the stacking order
  to be set up correctly beforehand!
*/
Bool
MatchWinToSM(MvwmWindow *ewin, mwtsm_state_args *ret_state_args,
    initial_window_options_t *win_opts)
{
	int             i;
	struct monitor *m = ewin->m;

	if (!does_file_version_match) {
		return False;
	}
	for (i = 0; i < num_match; i++) {
		if (!matches[i].used && matchWin(ewin, &matches[i])) {
			matches[i].used = 1;

			if (!Restarting) {
				/*
				 * We don't want to restore too much state if
				 * we are restarting, since that would make
				 * * restarting useless for rereading changed
				 * * rc files.
				 */
				SET_DO_SKIP_WINDOW_LIST(ewin,
				    DO_SKIP_WINDOW_LIST(&(matches[i])));
				SET_ICON_SUPPRESSED(ewin,
				    IS_ICON_SUPPRESSED(&(matches[i])));
				SET_HAS_NO_ICON_TITLE(ewin,
				    HAS_NO_ICON_TITLE(&(matches[i])));
				FPS_LENIENT(FW_FOCUS_POLICY(ewin),
				    FP_IS_LENIENT(FW_FOCUS_POLICY(&(matches
						[i]))));
				SET_ICON_STICKY_ACROSS_PAGES(ewin,
				    IS_ICON_STICKY_ACROSS_PAGES(&(matches
					    [i])));
				SET_ICON_STICKY_ACROSS_DESKS(ewin,
				    IS_ICON_STICKY_ACROSS_DESKS(&(matches
					    [i])));
				SET_DO_SKIP_ICON_CIRCULATE(ewin,
				    DO_SKIP_ICON_CIRCULATE(&(matches[i])));
				SET_DO_SKIP_SHADED_CIRCULATE(ewin,
				    DO_SKIP_SHADED_CIRCULATE(&(matches[i])));
				SET_DO_SKIP_CIRCULATE(ewin,
				    DO_SKIP_CIRCULATE(&(matches[i])));
				memcpy(&FW_FOCUS_POLICY(ewin),
				    &FW_FOCUS_POLICY(&matches[i]),
				    sizeof(focus_policy_t));
				if (matches[i].wm_name) {
					free_window_names(ewin, True, False);
					ewin->name.name = matches[i].wm_name;
					setup_visible_name(ewin, False);
				}
			}
			SET_NAME_CHANGED(ewin,
			    IS_NAME_CHANGED(&(matches[i])));
			SET_PLACED_BY_MVWM(ewin,
			    IS_PLACED_BY_MVWM(&(matches[i])));
			ret_state_args->do_shade = IS_SHADED(&(matches[i]));
			ret_state_args->used_title_dir_for_shading =
			    USED_TITLE_DIR_FOR_SHADING(&(matches[i]));
			ret_state_args->shade_dir = SHADED_DIR(&(matches[i]));
			ret_state_args->do_max = IS_MAXIMIZED(&(matches[i]));
			SET_USER_STATES(ewin, GET_USER_STATES(&(matches[i])));
			SET_ICON_MOVED(ewin, IS_ICON_MOVED(&(matches[i])));
			if (IS_ICONIFIED(&(matches[i]))) {
				/*
				 * ICON_MOVED is necessary to make mvwm use
				 * icon_[xy]_loc for icon placement
				 */
				win_opts->initial_state = IconicState;
				win_opts->flags.use_initial_icon_xy = 1;
				win_opts->initial_icon_x = matches[i].icon_x;
				win_opts->initial_icon_y = matches[i].icon_y;
				if (!IS_STICKY_ACROSS_PAGES(&(matches[i])) &&
				    !(IS_ICONIFIED(&(matches[i])) &&
					IS_ICON_STICKY_ACROSS_PAGES(&(matches
						[i])))) {
					win_opts->initial_icon_x -=
					    m->virtual_scr.Vx;
					win_opts->initial_icon_y -=
					    m->virtual_scr.Vy;
				}
			}
			ewin->g.normal.x = matches[i].x;
			ewin->g.normal.y = matches[i].y;
			ewin->g.normal.width = matches[i].w;
			ewin->g.normal.height = matches[i].h;
			ewin->g.max.x = matches[i].x_max;
			ewin->g.max.y = matches[i].y_max;
			ewin->g.max.width = matches[i].w_max;
			ewin->g.max.height = matches[i].h_max;
			ewin->g.max_defect.width =
			    matches[i].width_defect_max;
			ewin->g.max_defect.height =
			    matches[i].height_defect_max;
			ewin->g.max_offset.x = matches[i].max_x_offset;
			ewin->g.max_offset.y = matches[i].max_y_offset;
			SET_STICKY_ACROSS_PAGES(ewin,
			    IS_STICKY_ACROSS_PAGES(&(matches[i])));
			SET_STICKY_ACROSS_DESKS(ewin,
			    IS_STICKY_ACROSS_DESKS(&(matches[i])));
			ewin->Desk =
			    (IS_STICKY_ACROSS_DESKS(ewin)) ? m->virtual_scr.
			    CurrentDesk : matches[i].desktop;
			set_layer(ewin, matches[i].layer);
			set_default_layer(ewin, matches[i].default_layer);
			ewin->placed_by_button = matches[i].placed_by_button;
			/*
			 * Note: the Modal, skip pager, skip taskbar and
			 * * "stacking order" state are not restored here: there
			 * * are restored in EWMH_ExitStuff
			 */
			ewin->ewmh_hint_desktop =
			    matches[i].ewmh_hint_desktop;
			SET_HAS_EWMH_INIT_WM_DESKTOP(ewin,
			    HAS_EWMH_INIT_WM_DESKTOP(&(matches[i])));
			SET_HAS_EWMH_INIT_HIDDEN_STATE(ewin,
			    HAS_EWMH_INIT_HIDDEN_STATE(&(matches[i])));
			SET_HAS_EWMH_INIT_MAXHORIZ_STATE(ewin,
			    HAS_EWMH_INIT_MAXHORIZ_STATE(&(matches[i])));
			SET_HAS_EWMH_INIT_MAXVERT_STATE(ewin,
			    HAS_EWMH_INIT_MAXVERT_STATE(&(matches[i])));
			SET_HAS_EWMH_INIT_SHADED_STATE(ewin,
			    HAS_EWMH_INIT_SHADED_STATE(&(matches[i])));
			SET_HAS_EWMH_INIT_STICKY_STATE(ewin,
			    HAS_EWMH_INIT_STICKY_STATE(&(matches[i])));
			return True;
		}
	}

	return False;
}

void
RestartInSession(char *filename, Bool is_native, Bool _do_preserve_state)
{
	do_preserve_state = _do_preserve_state;

	save_state_file(filename);
	/*
	 * return and let Done restart us
	 */

	return;
}
