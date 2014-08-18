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

/* ---------------------------- included header files ---------------------- */

#define FEVENT_C
#define FEVENT_PRIVILEGED_ACCESS
#include "config.h"
#include "libs/mvwmlib.h"
#include <X11/Xlib.h>
#include "FEvent.h"
#undef FEVENT_C
#undef FEVENT_PRIVILEGED_ACCESS

#include <stdio.h>

#include "libs/ftime.h"

/* ---------------------------- local definitions -------------------------- */

/* ---------------------------- local macros ------------------------------- */

/* ---------------------------- imports ------------------------------------ */

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

typedef struct
{
	Bool (*predicate) (Display *display, XEvent *event, XPointer arg);
	XPointer arg;
	XEvent event;
	Bool found;
} fev_check_peek_args;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- local variables ---------------------------- */

static XEvent fev_event;
static XEvent fev_event_old;
/* until Xlib does this for us */
Time fev_last_timestamp = CurrentTime;

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- local functions ---------------------------- */

/* Records the time of the last processed event. */
static void fev_update_last_timestamp(const XEvent *ev)
{
	Time new_timestamp = CurrentTime;

	switch (ev->type)
	{
	case KeyPress:
	case KeyRelease:
		new_timestamp = ev->xkey.time;
		break;
	case ButtonPress:
	case ButtonRelease:
		new_timestamp = ev->xbutton.time;
		break;
	case MotionNotify:
		new_timestamp = ev->xmotion.time;
		break;
	case EnterNotify:
	case LeaveNotify:
		new_timestamp = ev->xcrossing.time;
		break;
	case PropertyNotify:
		new_timestamp = ev->xproperty.time;
		break;
	case SelectionClear:
		new_timestamp = ev->xselectionclear.time;
		break;
	case SelectionRequest:
		new_timestamp = ev->xselectionrequest.time;
		break;
	case SelectionNotify:
		new_timestamp = ev->xselection.time;
		break;
	default:
		return;
	}
	/* Only update if the new timestamp is later than the old one, or
	 * if the new one is from a time at least 30 seconds earlier than the
	 * old one (in which case the system clock may have changed) */
	if (new_timestamp > fev_last_timestamp ||
	    fev_last_timestamp - new_timestamp > CLOCK_SKEW_MS)
	{
		fev_last_timestamp = new_timestamp;
	}

	return;
}

static Bool fev_check_peek_pred(
	Display *display, XEvent *event, XPointer arg)
{
	fev_check_peek_args *cpa = (fev_check_peek_args *)arg;

	if (cpa->found == True)
	{
		return False;
	}
	cpa->found = cpa->predicate(display, event, cpa->arg);
	if (cpa->found == True)
	{
		cpa->event = *event;
	}

	return False;
}

/* ---------------------------- interface functions (privileged access) ----- */

void fev_copy_last_event(XEvent *dest)
{
	*dest = fev_event;

	return;
}

XEvent *fev_get_last_event_address(void)
{
	return &fev_event;
}

/* ---------------------------- interface functions (normal_access) -------- */

Time fev_get_evtime(void)
{
	return fev_last_timestamp;
}

Bool fev_get_evpos_or_query(
	Display *dpy, Window w, const XEvent *e, int *ret_x, int *ret_y)
{
	Window JunkW;
	int JunkC;
	unsigned int JunkM;
	Bool rc;
	int type;

	type = (e != NULL) ? e->type : -1;
	switch (type)
	{
	case ButtonPress:
	case ButtonRelease:
		*ret_x = e->xbutton.x_root;
		*ret_y = e->xbutton.y_root;
		return True;
	case KeyPress:
	case KeyRelease:
		*ret_x = e->xkey.x_root;
		*ret_y = e->xkey.y_root;
		return True;
	case EnterNotify:
	case LeaveNotify:
		*ret_x = e->xcrossing.x_root;
		*ret_y = e->xcrossing.y_root;
		return True;
	case MotionNotify:
		if (e->xmotion.same_screen == True)
		{
			*ret_x = e->xmotion.x_root;
			*ret_y = e->xmotion.y_root;
		}
		else
		{
			/* pointer is on different screen */
			*ret_x = 0;
			*ret_y = 0;
		}
		return True;
	default:
		rc = FQueryPointer(
			dpy, w, &JunkW, &JunkW, ret_x, ret_y, &JunkC, &JunkC,
			&JunkM);
		if (rc == False)
		{
			/* pointer is on a different screen */
			*ret_x = 0;
			*ret_y = 0;
		}
		return rc;
	}
}

Bool fev_set_evpos(XEvent *e, int x, int y)
{
	switch (e->type)
	{
	case ButtonPress:
	case ButtonRelease:
		e->xbutton.x_root = x;
		e->xbutton.y_root = y;
		return True;
	case KeyPress:
	case KeyRelease:
		e->xkey.x_root = x;
		e->xkey.y_root = y;
		return True;
	case MotionNotify:
		if (e->xmotion.same_screen == True)
		{
			e->xmotion.x_root = x;
			e->xmotion.y_root = y;
			return True;
		}
		break;
	default:
		break;
	} /* switch */

	return False;
}

void fev_fake_event(XEvent *ev)
{
	fev_event_old = fev_event;
	fev_event = *ev;
	/* don't update the last timestamp here; the triggering event has
	 * already done this */

	return;
}

void *fev_save_event(void)
{
	XEvent *ev;

	ev = mvwm_malloc(sizeof(XEvent));
	*ev = fev_event;

	return ev;
}

void fev_restore_event(void *ev)
{
	fev_event = *(XEvent *)ev;
	free(ev);

	return;
}

void fev_make_null_event(XEvent *ev, Display *dpy)
{
	memset(ev, 0, sizeof(*ev));
	ev->xany.serial = fev_event.xany.serial;
	ev->xany.display = dpy;

	return;
}

void fev_get_last_event(XEvent *ev)
{
	*ev = fev_event;

	return;
}

/* ---------------------------- X event replacements ----------------------- */

XTimeCoord *FGetMotionEvents(
	Display *display, Window w, Time start, Time stop, int *nevents_return)
{
	XTimeCoord *rc;

	rc = XGetMotionEvents(display, w, start, stop, nevents_return);

	return rc;
}

int FAllowEvents(
	Display *display, int event_mode, Time time)
{
	int rc;

	rc = XAllowEvents(display, event_mode, time);

	return rc;
}

Bool FCheckIfEvent(
	Display *display, XEvent *event_return,
	Bool (*predicate) (Display *display, XEvent *event, XPointer arg),
	XPointer arg)
{
	Bool rc;
	XEvent new_ev;

	rc = XCheckIfEvent(display, &new_ev, predicate, arg);
	if (rc == True)
	{
		fev_event_old = fev_event;
		fev_event = new_ev;
		*event_return = fev_event;
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

Bool FCheckMaskEvent(
	Display *display, long event_mask, XEvent *event_return)
{
	Bool rc;
	XEvent new_ev;

	rc = XCheckMaskEvent(display, event_mask, &new_ev);
	if (rc == True)
	{
		fev_event_old = fev_event;
		fev_event = new_ev;
		*event_return = fev_event;
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

Bool FCheckPeekIfEvent(
	Display *display, XEvent *event_return,
	Bool (*predicate) (Display *display, XEvent *event, XPointer arg),
	XPointer arg)
{
	XEvent dummy;
	fev_check_peek_args cpa;

	cpa.predicate = predicate;
	cpa.arg = arg;
	cpa.found = False;
	XCheckIfEvent(display, &dummy, fev_check_peek_pred, (char *)&cpa);
	if (cpa.found == True)
	{
		*event_return = cpa.event;
		fev_update_last_timestamp(event_return);
	}

	return cpa.found;
}

Bool FCheckTypedEvent(
	Display *display, int event_type, XEvent *event_return)
{
	Bool rc;
	XEvent new_ev;

	rc = XCheckTypedEvent(display, event_type, &new_ev);
	if (rc == True)
	{
		fev_event_old = fev_event;
		fev_event = new_ev;
		*event_return = fev_event;
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

Bool FCheckTypedWindowEvent(
	Display *display, Window w, int event_type, XEvent *event_return)
{
	Bool rc;
	XEvent new_ev;

	rc = XCheckTypedWindowEvent(display, w, event_type, &new_ev);
	if (rc == True)
	{
		fev_event_old = fev_event;
		fev_event = new_ev;
		*event_return = fev_event;
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

Bool FCheckWindowEvent(
	Display *display, Window w, long event_mask, XEvent *event_return)
{
	Bool rc;
	XEvent new_ev;

	rc = XCheckWindowEvent(display, w, event_mask, &new_ev);
	if (rc == True)
	{
		fev_event_old = fev_event;
		fev_event = new_ev;
		*event_return = fev_event;
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

int FEventsQueued(
	Display *display, int mode)
{
	int rc;

	rc = XEventsQueued(display, mode);

	return rc;
}

int FIfEvent(
	Display *display, XEvent *event_return,
	Bool (*predicate) (Display *display, XEvent *event, XPointer arg),
	XPointer arg)
{
	int rc;

	fev_event_old = fev_event;
	rc = XIfEvent(display, &fev_event, predicate, arg);
	*event_return = fev_event;
	fev_update_last_timestamp(event_return);

	return rc;
}

int FMaskEvent(
	Display *display, long event_mask, XEvent *event_return)
{
	int rc;

	fev_event_old = fev_event;
	rc = XMaskEvent(display, event_mask, &fev_event);
	*event_return = fev_event;
	fev_update_last_timestamp(event_return);

	return rc;
}

int FNextEvent(
	Display *display, XEvent *event_return)
{
	int rc;

	fev_event_old = fev_event;
	rc = XNextEvent(display, &fev_event);
	*event_return = fev_event;
	fev_update_last_timestamp(event_return);

	return rc;
}

int FPeekEvent(
	Display *display, XEvent *event_return)
{
	int rc;

	rc = XPeekEvent(display, event_return);
	if (rc == True)
	{
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

int FPeekIfEvent(
	Display *display, XEvent *event_return,
	Bool (*predicate) (Display *display, XEvent *event, XPointer arg),
	XPointer arg)
{
	int rc;

	rc = XPeekIfEvent(display, event_return, predicate, arg);
	if (rc == True)
	{
		fev_update_last_timestamp(event_return);
	}

	return rc;
}

int FPending(
	Display *display)
{
	int rc;

	rc = XPending(display);

	return rc;
}

int FPutBackEvent(
	Display *display, XEvent *event)
{
	int rc;

	rc = XPutBackEvent(display, event);
	fev_event = fev_event_old;

	return rc;
}

int FQLength(
	Display *display)
{
	int rc;

	rc = XQLength(display);

	return rc;
}

Bool FQueryPointer(
	Display *display, Window w, Window *root_return, Window *child_return,
	int *root_x_return, int *root_y_return, int *win_x_return,
	int *win_y_return, unsigned int *mask_return)
{
	Bool rc;

	rc = XQueryPointer(
		display, w, root_return, child_return, root_x_return,
		root_y_return, win_x_return, win_y_return, mask_return);

	return rc;
}

Status FSendEvent(
	Display *display, Window w, Bool propagate, long event_mask,
	XEvent *event_send)
{
	Status rc;

	rc = XSendEvent(display, w, propagate, event_mask, event_send);

	return rc;
}

int FWarpPointer(
	Display *display, Window src_w, Window dest_w, int src_x, int src_y,
	unsigned int src_width, unsigned int src_height, int dest_x, int dest_y)
{
	int rc;

	rc = XWarpPointer(
		display, src_w, dest_w, src_x, src_y, src_width, src_height,
		dest_x, dest_y);

	return rc;
}

int FWarpPointerUpdateEvpos(
	XEvent *ev, Display *display, Window src_w, Window dest_w, int src_x,
	int src_y, unsigned int src_width, unsigned int src_height,
	int dest_x, int dest_y)
{
	int rc;

	rc = XWarpPointer(
		display, src_w, dest_w, src_x, src_y, src_width, src_height,
		dest_x, dest_y);
	if (ev != NULL && dest_w == DefaultRootWindow(display))
	{
		fev_set_evpos(ev, dest_x, dest_y);
	}

	return rc;
}

int FWindowEvent(
	Display *display, Window w, long event_mask, XEvent *event_return)
{
	int rc;

	fev_event_old = fev_event;
	rc = XWindowEvent(display, w, event_mask, &fev_event);
	*event_return = fev_event;
	fev_update_last_timestamp(event_return);

	return rc;
}
