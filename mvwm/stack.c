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

#include "config.h"
#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include <limits.h>

#include "libs/ftime.h"
#include "libs/mvwmlib.h"
#include "libs/gravity.h"
#include "libs/Grab.h"
#include "libs/Parse.h"
#include "libs/defaults.h"
#include "mvwm.h"
#include "externs.h"
#include "cursor.h"
#include "functions.h"
#include "bindings.h"
#include "misc.h"
#include "screen.h"
#include "module_list.h"
#include "module_interface.h"
#include "focus.h"
#include "stack.h"
#include "events.h"
#include "borders.h"
#include "virtual.h"
#include "geometry.h"
#include "icons.h"
#include "ewmh.h"
#include "frame.h"

/* ---------------------------- local definitions -------------------------- */

/* If more than this many transients are in a single branch of a transient
 * tree, they will end up in more or less random stacking order. */
#define MAX_TRANSIENTS_IN_BRANCH 200000
/* Same for total levels of transients. */
#define MAX_TRANSIENT_LEVELS     10000
/* This number must fit in a signed int! */
#define LOWER_PENALTY (MAX_TRANSIENTS_IN_BRANCH * MAX_TRANSIENT_LEVELS)

/* ---------------------------- local macros ------------------------------- */

/* ---------------------------- imports ------------------------------------ */

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

typedef enum
{
	SM_RAISE = MARK_RAISE,
	SM_LOWER = MARK_LOWER,
	SM_RESTACK = MARK_RESTACK
} stack_mode_t;

/* ---------------------------- forward declarations ----------------------- */

static void __raise_or_lower_window(
	MvwmWindow *t, stack_mode_t mode, Bool allow_recursion,
	Bool is_new_window, Bool is_client_request);
static void raise_or_lower_window(
	MvwmWindow *t, stack_mode_t mode, Bool allow_recursion,
	Bool is_new_window, Bool is_client_request);
#if 0
static void ResyncMvwmStackRing(void);
#endif
static void ResyncXStackingOrder(void);
static void BroadcastRestack(MvwmWindow *s1, MvwmWindow *s2);
static Bool is_above_unmanaged(MvwmWindow *fw, Window *umtop);
static int collect_transients_recursive(
	MvwmWindow *t, MvwmWindow *list_head, int layer, stack_mode_t mode,
	Bool do_include_target_window);

/* ---------------------------- local variables ---------------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- local functions ---------------------------- */

#define DEBUG_STACK_RING 1
#ifdef DEBUG_STACK_RING
/* debugging function */
static void dump_stack_ring(void)
{
	MvwmWindow *t1;

	if (!debugging_stack_ring)
	{
		return;
	}
	XBell(dpy, 0);
	fprintf(stderr,"dumping stack ring:\n");
	for (
		t1 = Scr.MvwmRoot.stack_next; t1 != &Scr.MvwmRoot;
		t1 = t1->stack_next)
	{
		fprintf(stderr,"    l=%d fw=%p f=0x%08x '%s'\n", t1->layer,
			t1, (int)FW_W_FRAME(t1), t1->name.name);
	}

	return;
}

/* debugging function */
void verify_stack_ring_consistency(void)
{
	Window root, parent, *children;
	unsigned int nchildren;
	int i;
	MvwmWindow *t1, *t2;
	int last_layer;
	int last_index;

	if (!debugging_stack_ring)
	{
		return;
	}
	XFlush(dpy);
	t2 = Scr.MvwmRoot.stack_next;
	if (t2 == &Scr.MvwmRoot)
	{
		return;
	}
	last_layer = t2->layer;

	for (
		t1 = t2->stack_next; t1 != &Scr.MvwmRoot;
		t2 = t1, t1 = t1->stack_next)
	{
		if (t1->layer > last_layer)
		{
			fprintf(
				stderr,
				"vsrc: stack ring is corrupt! '%s' (layer %d)"
				" is above '%s' (layer %d/%d)\n",
				t1->name.name, t1->layer, t2->name.name,
				t2->layer, last_layer);
			dump_stack_ring();
			return;
		}
		last_layer = t1->layer;
	}
	t2 = &Scr.MvwmRoot;
	for (
		t1 = t2->stack_next; t1 != &Scr.MvwmRoot;
		t2 = t1, t1 = t1->stack_next)
	{
		if (t1->stack_prev != t2)
		{
			break;
		}
	}
	if (t1 != &Scr.MvwmRoot || t1->stack_prev != t2)
	{
		fprintf(
			stderr,
			"vsrc: stack ring is corrupt -"
			" mvwm will probably crash! %p -> %p but %p <- %p",
			t2, t1, t1->stack_prev, t1);
		dump_stack_ring();
		return;
	}
	MyXGrabServer(dpy);
	if (!XQueryTree(dpy, Scr.Root, &root, &parent, &children, &nchildren))
	{
		MyXUngrabServer(dpy);
		return;
	}

	last_index = nchildren;
	for (
		t1 = Scr.MvwmRoot.stack_next; t1 != &Scr.MvwmRoot;
		t1 = t1->stack_next)
	{
		/* find window in window list */
		for (
			i = 0; i < nchildren && FW_W_FRAME(t1) != children[i];
			i++)
		{
			/* nothing to do here */
		}
		if (i == nchildren)
		{
			fprintf(
				stderr,"vsrc: window already died:"
				" fw=%p w=0x%08x '%s'\n",
				t1, (int)FW_W_FRAME(t1), t1->name.name);
		}
		else if (i >= last_index)
		{
			fprintf(
				stderr, "vsrc: window is at wrong position"
				" in stack ring: fw=%p f=0x%08x '%s'\n",
				t1, (int)FW_W_FRAME(t1),
				t1->name.name);
			dump_stack_ring();
			fprintf(stderr,"dumping X stacking order:\n");
			for (i = nchildren; i-- > 0; )
			{
				for (
					t1 = Scr.MvwmRoot.stack_next;
					t1 != &Scr.MvwmRoot;
					t1 = t1->stack_next)
				{
					/* only dump frame windows */
					if (FW_W_FRAME(t1) == children[i])
					{
						fprintf(
							stderr, "  f=0x%08x\n",
							(int)children[i]);
						break;
					}
				}
			}
			MyXUngrabServer(dpy);
			XFree(children);
			return;
		}
		last_index = i;
	}
	MyXUngrabServer(dpy);
	XFree(children);

	return;
}
#endif

/* Add a whole ring of windows. The list_head itself will not be added. */
static void add_windowlist_to_stack_ring_after(
	MvwmWindow *list_head, MvwmWindow *add_after_win)
{
	add_after_win->stack_next->stack_prev = list_head->stack_prev;
	list_head->stack_prev->stack_next = add_after_win->stack_next;
	add_after_win->stack_next = list_head->stack_next;
	list_head->stack_next->stack_prev = add_after_win;

	return;
}

static MvwmWindow *get_transientfor_top_mvwmwindow(MvwmWindow *t)
{
	MvwmWindow *s;

	s = t;
	while (s && IS_TRANSIENT(s) && DO_STACK_TRANSIENT_PARENT(s))
	{
		s = get_transientfor_mvwmwindow(s);
		if (s)
		{
			t = s;
		}
	}

	return t;
}

/*
 * Raise a target and all higher mvwm-managed windows above any
 *  override_redirects:
 *  - locate the highest override_redirect above our target
 *  - put all the MvwmWindows from the target to the highest MvwmWindow
 *    below the highest override_redirect in the restack list
 *  - configure our target window above the override_redirect sibling,
 *    and restack.
 */
static void raise_over_unmanaged(MvwmWindow *t)
{
	int i;
	Window OR_Above = None;
	Window *wins;
	int count = 0;
	MvwmWindow *t2 = NULL;
	unsigned int flags;
	XWindowChanges changes;


	/*
	 * Locate the highest override_redirect window above our target, and
	 * the highest of our windows below it.
	 *
	 * Count the windows we need to restack, then build the stack list.
	 */
	if (!is_above_unmanaged(t, &OR_Above))
	{
		for (
			count = 0, t2 = Scr.MvwmRoot.stack_next;
			t2 != &Scr.MvwmRoot; t2 = t2->stack_next)
		{
			count++;
			count += get_visible_icon_window_count(t2);
			if (t2 == t)
			{
				break;
			}
		}

		if (count > 0)
		{
			wins = mvwm_malloc(count * sizeof (Window));
			for (
				i = 0, t2 = Scr.MvwmRoot.stack_next;
				t2 != &Scr.MvwmRoot; t2 = t2->stack_next)
			{
				wins[i++] = FW_W_FRAME(t2);
				if (IS_ICONIFIED(t2) &&
				    ! IS_ICON_SUPPRESSED(t2))
				{
					if (FW_W_ICON_TITLE(t2) != None)
					{
						wins[i++] = FW_W_ICON_TITLE(t2);
					}
					if (FW_W_ICON_PIXMAP(t2) != None)
					{
						wins[i++] =
							FW_W_ICON_PIXMAP(t2);
					}
				}
				if (t2 == t)
				{
					break;
				}
			}

			memset(&changes, '\0', sizeof(changes));
			changes.sibling = OR_Above;
			changes.stack_mode = Above;
			flags = CWSibling|CWStackMode;

			XConfigureWindow(
				dpy, FW_W_FRAME(t)/*topwin*/, flags, &changes);
			if (i > 1)
			{
				XRestackWindows(dpy, wins, i);
			}
			free (wins);
		}
	}/*  end - we found an OR above our target  */

	return;
}

static Bool __is_restack_transients_needed(
	MvwmWindow *t, stack_mode_t mode)
{
	if (DO_RAISE_TRANSIENT(t))
	{
		if (mode == SM_RAISE || mode == SM_RESTACK)
		{
			return True;
		}
	}
	if (DO_LOWER_TRANSIENT(t))
	{
		if (mode == SM_LOWER || mode == SM_RESTACK)
		{
			return True;
		}
	}

	return False;
}

static Bool __must_move_transients(
	MvwmWindow *t, stack_mode_t mode)
{
	if (IS_ICONIFIED(t))
	{
		return False;
	}
	/* raise */
	if (__is_restack_transients_needed(t, mode) == True)
	{
		Bool scanning_above_window = True;
		MvwmWindow *q;

		for (
			q = Scr.MvwmRoot.stack_next;
			q != &Scr.MvwmRoot && t->layer <= q->layer;
			q = q->stack_next)
		{
			if (t->layer < q->layer)
			{
				/* We're not interested in higher layers. */
				continue;
			}
			else if (mode == SM_RESTACK && IS_TRANSIENT(q) &&
				 FW_W_TRANSIENTFOR(q) == FW_W(t))
			{
				return True;
			}
			else if (t == q)
			{
				/* We found our window. All further transients
				 * are below it. */
				scanning_above_window = False;
			}
			else if (IS_TRANSIENT(q) &&
				 FW_W_TRANSIENTFOR(q) == FW_W(t))
			{
				return True;
			}
			else if (scanning_above_window && mode == SM_RAISE)
			{
				/* raise: The window is not raised, so itself
				 * and all transients will be raised. */
				return True;
			}
		}
	}

	return False;
}

static Window __get_stacking_sibling(MvwmWindow *fw, Bool do_stack_below)
{
	Window w;

	/* default to frame window */
	w = FW_W_FRAME(fw);
	if (IS_ICONIFIED(fw) && do_stack_below == True)
	{
		/* override with icon windows when stacking below */
		if (FW_W_ICON_PIXMAP(fw) != None)
		{
			w = FW_W_ICON_PIXMAP(fw);
		}
		else if (FW_W_ICON_TITLE(fw) != None)
		{
			w = FW_W_ICON_TITLE(fw);
		}
	}

	return w;
}

static void __sort_transient_ring(MvwmWindow *ring)
{
	MvwmWindow *s;
	MvwmWindow *t;
	MvwmWindow *u;
	MvwmWindow *prev;

	if (ring->stack_next->stack_next == ring)
	{
		/* only one or zero windows */
		return;
	}
	/* Implementation note:  this sorting algorithm is about the most
	 * inefficient possible.  It just swaps the position of two adjacent
	 * windows in the ring if they are in the wrong order.  Since
	 * transient windows are rare, this should not cause any notable
	 * performance hit.  Because it is important that the order of windows
	 * with the same key is not changed, we can not just use qsort() here.
	 */
	for (
		t = ring->stack_next, prev = ring; t->stack_next != ring;
		prev = t->stack_prev)
	{
		s = t->stack_next;
		if (t->scratch.i < s->scratch.i)
		{
			/* swap windows */
			u = s->stack_next;
			s->stack_next = t;
			t->stack_next = u;
			u = t->stack_prev;
			t->stack_prev = s;
			s->stack_prev = u;
			s->stack_prev->stack_next = s;
			t->stack_next->stack_prev = t;
			if (prev != ring)
			{
				/* move further up the ring? */
				t = prev;
			}
			else
			{
				/* hit start of ring */
			}
		}
		else
		{
			/* correct order, advance one window */
			t = t->stack_next;
		}
	}

	return;
}

static void __restack_window_list(
	MvwmWindow *r, MvwmWindow *s, int count, Bool do_broadcast_all,
	Bool do_lower)
{
	MvwmWindow *t;
	unsigned int flags;
	int i;
	XWindowChanges changes;
	Window *wins;
	int do_stack_above;
	int is_reversed;

	if (count <= 0)
	{
		for (count = 0, t = r->stack_next; t != s; t = t->stack_next)
		{
			count++;
			count += get_visible_icon_window_count(t);
		}
	}
	/* restack the windows between r and s */
	wins = mvwm_malloc((count + 3) * sizeof(Window));
	for (t = r->stack_next, i = 0; t != s; t = t->stack_next)
	{
		if (i > count)
		{
			mvwm_msg(
				ERR, "__restack_window_list",
				"more transients than expected");
			break;
		}
		wins[i++] = FW_W_FRAME(t);
		if (IS_ICONIFIED(t) && !IS_ICON_SUPPRESSED(t))
		{
			if (FW_W_ICON_TITLE(t) != None)
			{
				wins[i++] = FW_W_ICON_TITLE(t);
			}
			if (FW_W_ICON_PIXMAP(t) != None)
			{
				wins[i++] = FW_W_ICON_PIXMAP(t);
			}
		}
	}
	changes.sibling = __get_stacking_sibling(r, True);
	if (changes.sibling == None)
	{
		changes.sibling = __get_stacking_sibling(s, False);
		is_reversed = 1;
	}
	else
	{
		is_reversed = 0;
	}
	if (changes.sibling == None)
	{
		do_stack_above = !do_lower;
		flags = CWStackMode;
	}
	else
	{
		do_stack_above = 0;
		flags = CWStackMode | CWSibling;
	}
	changes.stack_mode = (do_stack_above ^ is_reversed) ? Above : Below;
	XConfigureWindow(dpy, FW_W_FRAME(r->stack_next), flags, &changes);
	if (count > 1)
	{
		XRestackWindows(dpy, wins, count);
	}
	free(wins);
	EWMH_SetClientListStacking(r->m ? r->m : monitor_get_current());
	if (do_broadcast_all)
	{
		/* send out M_RESTACK for all windows, to make sure we don't
		 * forget anything. */
		BroadcastRestackAllWindows();
	}
	else
	{
		/* send out (one or more) M_RESTACK packets for windows
		 * between r and s */
		BroadcastRestack(r, s);
	}

	return;
}

MvwmWindow *__get_window_to_insert_after(MvwmWindow *fw, stack_mode_t mode)
{
	int test_layer;
	MvwmWindow *s;

	switch (mode)
	{
	case SM_LOWER:
		test_layer = fw->layer - 1;
		break;
	default:
	case SM_RAISE:
	case SM_RESTACK:
		test_layer = fw->layer;
		break;
	}
	for (
		s = Scr.MvwmRoot.stack_next; s != &Scr.MvwmRoot;
		s = s->stack_next)
	{
		if (s == fw)
		{
			continue;
		}
		if (test_layer >= s->layer)
		{
			break;
		}
	}

	return s;
}

static void __mark_group_member(
	MvwmWindow *fw, MvwmWindow *start, MvwmWindow *end)
{
	MvwmWindow *t;

	for (t = start; t != end; t = t->stack_next)
	{
		if (FW_W(t) == fw->wmhints->window_group ||
		    (t->wmhints && (t->wmhints->flags & WindowGroupHint) &&
		     t->wmhints->window_group == fw->wmhints->window_group))
		{
			if (IS_IN_TRANSIENT_SUBTREE(t))
			{
				/* have to move this one too */
				SET_IN_TRANSIENT_SUBTREE(fw, 1);
			}
		}
	}

	return;

}

static Bool __mark_transient_subtree_test(
	MvwmWindow *s, MvwmWindow *start, MvwmWindow *end, int mark_mode,
	Bool do_ignore_icons, Bool use_window_group_hint)
{
	Bool use_group_hint = False;
	MvwmWindow *r;

	if (IS_IN_TRANSIENT_SUBTREE(s))
	{
		return False;
	}
	if (use_window_group_hint &&
	    DO_ICONIFY_WINDOW_GROUPS(s) && s->wmhints &&
	    (s->wmhints->flags & WindowGroupHint) &&
	    (s->wmhints->window_group != None) &&
	    (s->wmhints->window_group != FW_W(s)) &&
	    (s->wmhints->window_group != Scr.Root))
	{
		use_group_hint = True;
	}
	if (!IS_TRANSIENT(s) && !use_group_hint)
	{
		return False;
	}
	if (do_ignore_icons && IS_ICONIFIED(s))
	{
		return False;
	}
	r = (MvwmWindow *)s->scratch.p;
	if (IS_TRANSIENT(s))
	{
		if (r && IS_IN_TRANSIENT_SUBTREE(r) &&
		    ((mark_mode == MARK_ALL) ||
		     __is_restack_transients_needed(
			     r, (stack_mode_t)mark_mode) == True))
		{
			/* have to move this one too */
			SET_IN_TRANSIENT_SUBTREE(s, 1);
			/* used for stacking transients */
			s->scratch.i += r->scratch.i + 1;
			return True;
		}
	}
	if (use_group_hint && !IS_IN_TRANSIENT_SUBTREE(s))
	{
		__mark_group_member(s, start, end);
		if (IS_IN_TRANSIENT_SUBTREE(s))
		{
			/* need another scan through the list */
			return True;
		}
	}

	return False;
}

/* heavaly borrowed from mark_transient_subtree.  This will mark a subtree as
 * long as it is straight, and return true if the operation is succussful.  It
 * will abort and return False as soon as some inconsitance is hit. */

static Bool is_transient_subtree_straight(
	MvwmWindow *t, int layer, stack_mode_t mode, Bool do_ignore_icons,
	Bool use_window_group_hint)
{
	MvwmWindow *s;
	MvwmWindow *start;
	MvwmWindow *end;
	int min_i;
	Bool is_in_gap;
	int mark_mode;

	switch (mode)
	{
	case SM_RAISE:
		mark_mode = MARK_RAISE;
		break;
	case SM_LOWER:
		mark_mode = MARK_LOWER;
		break;
	default:
		return False;
	}

	if (layer >= 0 && t->layer != layer)
	{
		return True;
	}
	if (t->stack_prev == NULL || t->stack_next == NULL)
	{
		/* the window is not placed correctly in the stack ring
		* (probably about to be destroyed) */
		return False;
	}
	/* find out on which windows to operate */
	/* iteration are done reverse (bottom up, since that's the way the
	 * transients wil be stacked if all is well */
	if (layer >= 0)
	{
		/* only work on the given layer */
		start = &Scr.MvwmRoot;
		end = &Scr.MvwmRoot;
		for (
			s = Scr.MvwmRoot.stack_prev;
			s != &Scr.MvwmRoot && s->layer <= layer;
			s = s->stack_prev)
		{
			if (s->layer == layer)
			{
				if (start == &Scr.MvwmRoot)
				{
					start = s;
				}
				end = s->stack_prev;
			}
		}
	}
	else
	{
		/* work on complete window list */
		start = Scr.MvwmRoot.stack_prev;
		end = &Scr.MvwmRoot;
	}
	/* clean the temporary flag in all windows and precalculate the
	 * transient frame windows */
	for (
		s = Scr.MvwmRoot.stack_next; s != &Scr.MvwmRoot;
		s = s->stack_next)
	{
		SET_IN_TRANSIENT_SUBTREE(s, 0);
		if (IS_TRANSIENT(s) && (layer < 0 || layer == s->layer))
		{
			s->scratch.p = get_transientfor_mvwmwindow(s);
		}
		else
		{
			s->scratch.p = NULL;
		}
	}
	/* Indicate that no cleening is needed */
	Scr.MvwmRoot.scratch.i = 1;
	/* now loop over the windows and mark the ones we need to move */
	SET_IN_TRANSIENT_SUBTREE(t, 1);
	min_i = INT_MIN;
	is_in_gap = False;

	if (mode == SM_LOWER && t != start)
	{
		return False;
	}

	/* check that all transients above the window are in a sorted line
	 * with no other windows between them */
	for (s = t->stack_prev; s != end && !(is_in_gap && mode == SM_RAISE);
	     s = s->stack_prev)
	{
		if (
			__mark_transient_subtree_test(
				s, start, end, mark_mode,
				do_ignore_icons,
				use_window_group_hint))
		{
			if (is_in_gap)
			{
				return False;
			}
			else if (s->scratch.i < min_i)
			{
				return False;
			}
			min_i = s->scratch.i;
		}
		else
		{
			is_in_gap = True;
		}
	} /* for */
	if (is_in_gap && mode == SM_RAISE)
	{
		return False;
	}
	/* check that there are no transients left beneth the window */
	for (s = start; s != t; s = s->stack_prev)
	{
		if (
			__mark_transient_subtree_test(
				s, start, end, mark_mode,
				do_ignore_icons,
				use_window_group_hint))
		{
			return False;
		}
	}

	return True;
}

/* function to test if all windows are at correct place from start. */
static Bool __is_restack_needed(
	MvwmWindow *t, stack_mode_t mode, Bool do_restack_transients,
	Bool is_new_window)
{
	if (is_new_window)
	{
		return True;
	}
	else if (t->stack_prev == NULL || t->stack_next == NULL)
	{
		/* the window is about to be destroyed, and has been removed
		 * from the stack ring. No need to restack. */
		return False;
	}

	if (mode == SM_RESTACK)
	{
		return True;
	}
	if (do_restack_transients)
	{
		return !is_transient_subtree_straight(
			t, t->layer, mode, True, False);
	}
	else if (mode == SM_LOWER)
	{
		return (t->stack_next != &Scr.MvwmRoot &&
			t->stack_next->layer == t->layer);
	}
	else if (mode == SM_RAISE)
	{
		return (t->stack_prev != &Scr.MvwmRoot &&
			t->stack_prev->layer == t->layer);
	}

	return True;
}

static Bool __restack_window(
	MvwmWindow *t, stack_mode_t mode, Bool do_restack_transients,
	Bool is_new_window, Bool is_client_request)
{
	MvwmWindow *s = NULL;
	MvwmWindow *r = NULL;
	MvwmWindow tmp_r;
	int count;

	if (!__is_restack_needed(
		    t, mode, do_restack_transients, is_new_window))
	{
		/* need to cancel out the effect of any M_RAISE/M_LOWER that
		 * might already be send out. This is ugly. Better would be to
		 * not send the messages in the first place. */
		if (do_restack_transients)
		{
			s = t->stack_next;
			if (s == NULL)
			{
				return True;
			}
			while (IS_IN_TRANSIENT_SUBTREE(s))
			{
				s = s->stack_next;
			}
			BroadcastRestack(t->stack_prev, s);
		}

		/* native/unmanaged windows might have raised. However some
		 * buggy clients will keep issuing requests if the raise hacks
		 * are done after processing their requests. */
		return is_client_request;
	}

	count = 0;
	if (do_restack_transients)
	{
		/* collect the transients in a temp list */
		tmp_r.stack_prev = &tmp_r;
		tmp_r.stack_next = &tmp_r;
		count = collect_transients_recursive(
			t, &tmp_r, t->layer, mode, False);
		if (count == 0)
		{
			do_restack_transients = False;
		}
	}
	count += 1 + get_visible_icon_window_count(t);
	/* now find the place to reinsert t and friends */
	if (mode == SM_RESTACK)
	{
		s = t->stack_next;
	}
	else
	{
		s = __get_window_to_insert_after(t, mode);
	}
	remove_window_from_stack_ring(t);
	r = s->stack_prev;
	if (do_restack_transients)
	{
		/* re-sort the transient windows according to their scratch.i
		 * register */
		__sort_transient_ring(&tmp_r);
		/* insert all transients between r and s. */
		add_windowlist_to_stack_ring_after(&tmp_r, r);
	}

	/*
	** Re-insert t - below transients
	*/
	add_window_to_stack_ring_after(t, s->stack_prev);

	if (is_new_window && IS_TRANSIENT(t) &&
	    DO_STACK_TRANSIENT_PARENT(t) && !IS_ICONIFIED(t))
	{
		/* now that the new transient is properly positioned in the
		 * stack ring, raise/lower it again so that its parent is
		 * raised/lowered too */
		raise_or_lower_window(t, mode, True, False, is_client_request);
		/* make sure the stacking order is correct - may be the
		 * sledge-hammer method, but the recursion ist too hard to
		 * understand. */
		ResyncXStackingOrder();

		/* if the transient is on the top of the top layer pan frames
		 * will have ended up under all windows after this. */
		return (t->stack_prev != &Scr.MvwmRoot);
	}
	else
	{
		/* restack the windows between r and s */
		__restack_window_list(
			r, s, count, do_restack_transients,
			mode == (SM_LOWER) ? True : False);
	}

	return False;
}

static Bool __raise_lower_recursion(
	MvwmWindow *t, stack_mode_t mode, Bool is_client_request)
{
	MvwmWindow *t2;

	for (
		t2 = Scr.MvwmRoot.stack_next; t2 != &Scr.MvwmRoot;
		t2 = t2->stack_next)
	{
		if (FW_W(t2) == FW_W_TRANSIENTFOR(t))
		{
			if (t2 == t)
			{
				return False;
			}
			if (IS_ICONIFIED(t2) || t->layer != t2->layer)
			{
				break;
			}
			if (mode == SM_LOWER &&
			    (!IS_TRANSIENT(t2) ||
			     !DO_STACK_TRANSIENT_PARENT(t2)))
			{
				/* hit the highest level transient; lower this
				 * subtree below all other subtrees of the
				 * same window */
				t->scratch.i = -LOWER_PENALTY;
			}
			else
			{
				/* Add a bonus to the stack ring position for
				 * this branch of the transient tree over all
				 * other branches. */
				t->scratch.i = MAX_TRANSIENTS_IN_BRANCH;
			}
			__raise_or_lower_window(
				t2, mode, True, False, is_client_request);
			if (__is_restack_transients_needed(t2, mode))
			{
				/* moving the parent moves our window already */
				return True;
			}
		}
	}

	return False;
}

static void __raise_or_lower_window(
	MvwmWindow *t, stack_mode_t mode, Bool allow_recursion,
	Bool is_new_window, Bool is_client_request)
{
	MvwmWindow *t2;
	Bool do_move_transients;

	/* Do not raise this window after command execution (see
	 * HandleButtonPress()). */
	SET_SCHEDULED_FOR_RAISE(t, 0);

	/* New windows are simply raised/lowered without touching the
	 * transientfor at first.  Then, further down in the code,
	 * __raise_or_lower_window() is called again to raise/lower the
	 * transientfor if necessary.  We can not do the recursion stuff for
	 * new windows because the __must_move_transients() call needs a
	 * properly ordered stack ring - but the new window is still at the
	 * front of the stack ring. */
	if (allow_recursion && !is_new_window && !IS_ICONIFIED(t))
	{
		/* This part makes Raise/Lower on a Transient act on its Main
		 * and sibling Transients.
		 *
		 * The recursion is limited to one level - which caters for
		 * most cases. This code does not handle the case where there
		 * are trees of Main + Transient (i.e. where a
		 * Main_window_with_Transients is itself Transient for another
		 * window). */
		if (IS_TRANSIENT(t) && DO_STACK_TRANSIENT_PARENT(t))
		{
			if (__raise_lower_recursion(
				    t, mode, is_client_request) == True)
			{
				return;
			}
		}
	}

	if (is_new_window)
	{
		do_move_transients = False;
	}
	else
	{
		do_move_transients = __must_move_transients(t, mode);
	}
	if (__restack_window(
		    t, mode, do_move_transients, is_new_window,
		    is_client_request) == True)
	{
		return;
	}

	if (mode == SM_RAISE)
	{
		/* This hack raises the target and all higher mvwm windows over
		 * any style grabfocusoff override_redirect windows that may be
		 * above it. This is used to cope with ill-behaved applications
		 * that insist on using long-lived override_redirects. */
		if (bo.do_raise_over_unmanaged)
		{
			raise_over_unmanaged(t);
		}

		/*
		 * The following is a hack to raise X windows over native
		 * windows which is needed for some (all ?) X servers running
		 * under Windows or Windows NT. */
		if (bo.is_raise_hack_needed)
		{
			/* RBW - 09/20/1999. I find that trying to raise
			 * unmanaged windows causes problems with some apps. If
			 * this seems to work well for everyone, I'll remove
			 * the #if 0. */
#if 0
			/* get *all* toplevels (even including
			 * override_redirects) */
			XQueryTree(dpy, Scr.Root, &junk, &junk, &tops, &num);

			/* raise from fw upwards to get them above NT windows */
			for (i = 0; i < num; i++)
			{
				if (tops[i] == FW_W_FRAME(t))
				{
					found = True;
				}
				if (found)
				{
					XRaiseWindow (dpy, tops[i]);
				}
			}
			XFree (tops);
#endif
			for (t2 = t; t2 != &Scr.MvwmRoot; t2 = t2->stack_prev)
			{
				XRaiseWindow(dpy, FW_W_FRAME(t2));
			}
		}
		/*  This needs to be done after all the raise hacks.  */
		raisePanFrames();
		/* If the window has been raised, make sure the decorations are
		 * updated immediately in case we are in a complex function
		 * (e.g. raise, unshade). */
		XFlush(dpy);
		handle_all_expose();
	}

	return;
}

static void raise_or_lower_window(
	MvwmWindow *t, stack_mode_t mode, Bool allow_recursion,
	Bool is_new_window, Bool is_client_request)
{
	MvwmWindow *fw;

	/* clean the auxiliary registers used in stacking transients */
	for (fw = Scr.MvwmRoot.next; fw != NULL; fw = fw->next)
	{
		fw->scratch.i = 0;
	}
	__raise_or_lower_window(
		t, mode, allow_recursion, is_new_window, is_client_request);

	return;
}

static Bool intersect(
	int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1)
{
	return !((x0 >= x1 + w1) || (x0 + w0 <= x1) ||
		 (y0 >= y1 + h1) || (y0 + h0 <= y1));
}

static Bool overlap(MvwmWindow *r, MvwmWindow *s)
{
	rectangle g1;
	rectangle g2;
	Bool rc;

	if (r->Desk != s->Desk)
	{
		return False;
	}
	rc = get_visible_window_or_icon_geometry(r, &g1);
	if (rc == False)
	{
		return False;
	}
	rc = get_visible_window_or_icon_geometry(s, &g2);
	if (rc == False)
	{
		return False;
	}
	rc = intersect(
		g1.x, g1.y, g1.width, g1.height,
		g2.x, g2.y, g2.width, g2.height);

	return rc;
}

#if 0
/*
  ResyncMvwmStackRing -
  Rebuilds the stacking order ring of mvwm-managed windows. For use in cases
  where apps raise/lower their own windows in a way that makes it difficult
  to determine exactly where they ended up in the stacking order.
  - Based on code from Matthias Clasen.
*/
static void ResyncMvwmStackRing (void)
{
	Window root, parent, *children;
	unsigned int nchildren;
	int i;
	MvwmWindow *t1, *t2;

	MyXGrabServer (dpy);

	if (!XQueryTree (dpy, Scr.Root, &root, &parent, &children, &nchildren))
	{
		MyXUngrabServer (dpy);
		return;
	}

	t2 = &Scr.MvwmRoot;
	for (i = 0; i < nchildren; i++)
	{
		for (t1 = Scr.MvwmRoot.next; t1 != NULL; t1 = t1->next)
		{
			if (IS_ICONIFIED(t1) && !IS_ICON_SUPPRESSED(t1))
			{
				if (FW_W_ICON_TITLE(t1) == children[i] ||
				    FW_W_ICON_PIXMAP(t1) == children[i])
				{
					break;
				}
			}
			else
			{
				if (FW_W_FRAME(t1) == children[i])
				{
					break;
				}
			}
		}

		if (t1 != NULL && t1 != t2)
		{
			/* Move the window to its new position, working from
			 * the bottom up (that's the way XQueryTree presents
			 * the list). */
			/* Pluck from chain. */
			remove_window_from_stack_ring(t1);
			add_window_to_stack_ring_after(t1, t2->stack_prev);
			if (t2 != &Scr.MvwmRoot && t2->layer > t1->layer)
			{
				/* oops, now our stack ring is out of order! */
				/* emergency fix */
				t1->layer = t2->layer;
			}
			t2 = t1;
		}
	}

	MyXUngrabServer (dpy);

	XFree (children);
}
#endif

/* same as above but synchronizes the stacking order in X from the stack ring.
 */
static void ResyncXStackingOrder(void)
{
	Window *wins;
	MvwmWindow *t;
	int count;
	int i;

	for (count = 0, t = Scr.MvwmRoot.next; t != NULL; count++, t = t->next)
	{
		/* nothing to do here */
	}
	if (count > 0)
	{
		wins = mvwm_malloc(3 * count * sizeof (Window));
		for (
			i = 0, t = Scr.MvwmRoot.stack_next; count--;
			t = t->stack_next)
		{
			wins[i++] = FW_W_FRAME(t);
			if (IS_ICONIFIED(t) && !IS_ICON_SUPPRESSED(t))
			{
				if (FW_W_ICON_TITLE(t) != None)
				{
					wins[i++] = FW_W_ICON_TITLE(t);
				}
				if (FW_W_ICON_PIXMAP(t) != None)
				{
					wins[i++] = FW_W_ICON_PIXMAP(t);
				}
			}
		}
		XRestackWindows(dpy, wins, i);
		free(wins);
		/* send out M_RESTACK for all windows, to make sure we don't
		 * forget anything. */
		BroadcastRestackAllWindows();
	}

	return;
}

/* send RESTACK packets for all windows between s1 and s2 */
static void BroadcastRestack(MvwmWindow *s1, MvwmWindow *s2)
{
	MvwmWindow *fw;
	int num;
	int i;
	int n;
	fmodule_list_itr moditr;
	fmodule *module;
	unsigned long *body, *bp, length;
	unsigned long max_wins_per_packet;

	if (s2 == &Scr.MvwmRoot)
	{
		s2 = s2->stack_prev;
		if (s2 == &Scr.MvwmRoot)
		{
			return;
		}
	}
	if (s1 == &Scr.MvwmRoot)
	{
		s1 = s1->stack_next;
		if (s1 == &Scr.MvwmRoot)
		{
			return;
		}
		/* s1 has been moved to the top of stack */
		BroadcastPacket(
			M_RAISE_WINDOW, 3, (long)FW_W(s1),
			(long)FW_W_FRAME(s1), (unsigned long)s1);
		if (s1->stack_next == s2)
		{
			/* avoid sending empty RESTACK packet */
			return;
		}
	}
	if (s1 == s2)
	{
		/* A useful M_RESTACK packet must contain at least two windows.
		 */
		return;
	}
	for (
		fw = s1, num = 1; fw != s2 && fw != &Scr.MvwmRoot;
		fw = fw->stack_next, num++)
	{
		/* nothing */
	}
	max_wins_per_packet = (MvwmPacketMaxSize - MvwmPacketHeaderSize) / 3;
	/* split packet if it is too long */
	for ( ; num > 1; s1 = fw, num -= n)
	{
		n = min(num, max_wins_per_packet) - 1;
		length = MvwmPacketHeaderSize + 3 * (n + 1);
		body = mvwm_malloc(length * sizeof(unsigned long));
		bp = body;
		*(bp++) = START_FLAG;
		*(bp++) = M_RESTACK;
		*(bp++) = length;
		*(bp++) = fev_get_evtime();
		for (fw = s1, i = 0; i <= n; i++, fw = fw->stack_next)
		{
			*(bp++) = FW_W(fw);
			*(bp++) = FW_W_FRAME(fw);
			*(bp++) = (unsigned long)fw;
		}
		/* The last window has to be in the header of the next part */
		fw = fw->stack_prev;
		module_list_itr_init(&moditr);
		while ( (module = module_list_itr_next(&moditr)) != NULL)
		{
			PositiveWrite(
				module,body,length*sizeof(unsigned long));
		}
		free(body);
	}
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif

	return;
}

static int collect_transients_recursive(
	MvwmWindow *t, MvwmWindow *list_head, int layer, stack_mode_t mode,
	Bool do_include_target_window)
{
	MvwmWindow *s;
	int count = 0;
	int m;

	switch (mode)
	{
	case SM_LOWER:
		m = MARK_LOWER;
		break;
	case SM_RAISE:
		m = MARK_RAISE;
		break;
	case SM_RESTACK:
		m = MARK_RESTACK;
		break;
	default:
		/* can not happen */
		m = MARK_RAISE;
		break;
	}
	mark_transient_subtree(t, layer, m, True, False);
	/* now collect the marked windows in a separate list */
	for (s = Scr.MvwmRoot.stack_next; s != &Scr.MvwmRoot; )
	{
		MvwmWindow *tmp;

		if (s == t && do_include_target_window == False)
		{
			/* ignore the target window */
			s = s->stack_next;
			continue;
		}
		tmp = s->stack_next;
		if (IS_IN_TRANSIENT_SUBTREE(s))
		{
			remove_window_from_stack_ring(s);
			add_window_to_stack_ring_after(
				s, list_head->stack_prev);
			count++;
			count += get_visible_icon_window_count(t);
		}
		s = tmp;
	}

	return count;
}

static Bool is_above_unmanaged(MvwmWindow *fw, Window *umtop)
{
	/*
	  Chase through the entire stack of the server's windows looking
	  for any unmanaged window that's higher than the target.
	  Called from raise_over_unmanaged and is_on_top_of_layer.
	*/
	Bool ontop = True;
	Window junk;
	Window *tops;
	int i;
	unsigned int num;
	Window OR_Above = None;
	XWindowAttributes wa;

	if (fw->Desk != fw->m->virtual_scr.CurrentDesk)
	{
		return True;
	}
	if (!XQueryTree(dpy, Scr.Root, &junk, &junk, &tops, &num))
	{
		return ontop;
	}

	/*
	 * Locate the highest override_redirect window above our target, and
	 * the highest of our windows below it.
	 */
	for (i = 0; i < num && tops[i] != FW_W_FRAME(fw); i++)
	{
		/* look for target window in list */
	}
	for (; i < num; i++)
	{
		/* It might be just as well (and quicker) just to check for the
		 * absence of an MvwmContext instead of for
		 * override_redirect... */
		if (!XGetWindowAttributes(dpy, tops[i], &wa))
		{
			continue;
		}
		/*
		  Don't forget to ignore the hidden frame resizing windows...
		*/
		if (wa.override_redirect == True
		    && wa.class != InputOnly
		    && tops[i] != Scr.NoFocusWin
		    && (!is_frame_hide_window(tops[i])))
		{
			OR_Above = tops[i];
		}
	} /* end for */

	if (OR_Above)  {
		*umtop = OR_Above;
		ontop = False;
	}

	XFree (tops);


	return ontop;
}

static Bool is_on_top_of_layer_ignore_rom(MvwmWindow *fw)
{
	MvwmWindow *t;
	Bool ontop = True;

	if (IS_SCHEDULED_FOR_DESTROY(fw))
	{
		/* stack ring members are no longer valid */
		return False;
	}
	if (DO_RAISE_TRANSIENT(fw))
	{
		mark_transient_subtree(fw, fw->layer, MARK_RAISE, True, False);
	}
	for (t = fw->stack_prev; t != &Scr.MvwmRoot; t = t->stack_prev)
	{
		if (t->layer > fw->layer)
		{
			break;
		}
		if (t->Desk != fw->Desk)
		{
			continue;
		}
		/* For RaiseOverUnmanaged we can not determine if the window is
		 * on top by checking if the window overlaps another one.  If
		 * it was below unmanaged windows, but on top of its layer, it
		 * would be considered on top. */
		if (bo.do_raise_over_unmanaged || overlap(fw, t))
		{
			if (!DO_RAISE_TRANSIENT(fw) ||
			    (!IS_IN_TRANSIENT_SUBTREE(t) && t != fw))
			{
				ontop = False;
				break;
			}
		}
	}

	return ontop;
}

static Bool __is_on_top_of_layer(MvwmWindow *fw, Bool client_entered)
{
	Window	junk;
	Bool  ontop	= False;
	if (bo.do_raise_over_unmanaged)
	{

#define EXPERIMENTAL_ROU_HANDLING
#ifdef EXPERIMENTAL_ROU_HANDLING
		/*
		  RBW - 2002/08/15 -
		  RaiseOverUnmanaged adds some overhead. The only way to let our
		  caller know for sure whether we need to grab the mouse buttons
		  because we may need to raise this window is to query the
		  server's tree and look for any override_redirect windows above
		  this one.
		  But this function is called far too often to do this every
		  time. Only if the window is at the top of the MvwmWindow
		  stack do we need more information from the server; and then
		  only at the last moment in HandleEnterNotify when we really
		  need to know whether a raise will be needed if the user
		  clicks in the client window.
		  is_on_top_of_layer_and_above_unmanaged is called in that case.
		*/
		if (is_on_top_of_layer_ignore_rom(fw))
		{
			if (client_entered)
				/* FIXME! - perhaps we should only do if MFCR */
			{
#ifdef ROUDEBUG
				printf("RBW-iotol  - %8.8lx is on top,"
				       " checking server tree.  ***\n",
				       FW_W_CLIENT(fw));
#endif
				ontop = is_above_unmanaged(fw, &junk);
#ifdef ROUDEBUG
				printf("	 returning %d\n", (int) ontop);
#endif
			}
			else
			{
#ifdef ROUDEBUG
				printf("RBW-iotol  - %8.8lx is on top,"
				       " *** NOT checking server tree.\n",
				       FW_W_CLIENT(fw));
#endif
				ontop = True;
			}
			return ontop;
		}
		else
		{
			return False;
		}
#else
		return False;	/*  Old pre-2002/08/22 handling.  */
#endif
	}
	else
	{
		return is_on_top_of_layer_ignore_rom(fw);
	}
}

/* ---------------------------- interface functions ------------------------ */

/* Remove a window from the stack ring */
void remove_window_from_stack_ring(MvwmWindow *t)
{
	if (IS_SCHEDULED_FOR_DESTROY(t))
	{
		return;
	}
	t->stack_prev->stack_next = t->stack_next;
	t->stack_next->stack_prev = t->stack_prev;
	/* not really necessary, but gives a little more saftey */
	t->stack_prev = NULL;
	t->stack_next = NULL;

	return;
}

/* Add window t to the stack ring after window t */
void add_window_to_stack_ring_after(MvwmWindow *t, MvwmWindow *add_after_win)
{
	if (IS_SCHEDULED_FOR_DESTROY(t))
	{
		return;
	}
	if (t == add_after_win || t == add_after_win->stack_next)
	{
		/* tried to add the window before or after itself */
		mvwm_msg(
			ERR, "add_window_to_stack_ring_after",
			"BUG: tried to add window '%s' %s itself in stack"
			" ring\n",
			t->name.name, (t == add_after_win) ?
			"after" : "before");
		return;
	}
	t->stack_next = add_after_win->stack_next;
	add_after_win->stack_next->stack_prev = t;
	t->stack_prev = add_after_win;
	add_after_win->stack_next = t;

	return;
}

MvwmWindow *get_next_window_in_stack_ring(const MvwmWindow *t)
{
	return t->stack_next;
}

MvwmWindow *get_prev_window_in_stack_ring(const MvwmWindow *t)
{
	return t->stack_prev;
}

MvwmWindow *get_transientfor_mvwmwindow(const MvwmWindow *t)
{
	MvwmWindow *s;

	if (!t || !IS_TRANSIENT(t) || FW_W_TRANSIENTFOR(t) == Scr.Root ||
	    FW_W_TRANSIENTFOR(t) == None)
	{
		return NULL;
	}
	for (s = Scr.MvwmRoot.next; s != NULL; s = s->next)
	{
		if (FW_W(s) == FW_W_TRANSIENTFOR(t))
		{
			return (s == t) ? NULL : s;
		}
	}

	return NULL;
}

/* Takes a window from the top of the stack ring and puts it at the appropriate
 * place. Called when new windows are created. */
Bool position_new_window_in_stack_ring(MvwmWindow *t, Bool do_lower)
{
	if (t->stack_prev != &Scr.MvwmRoot)
	{
		/* Not at top of stack ring, so it is already in place.
		 * add_window.c relies on this. */
		return False;
	}
	/* RaiseWindow/LowerWindow will put the window in its layer */
	raise_or_lower_window(
		t, (do_lower) ? SM_LOWER : SM_RAISE, False, True, False);

	return True;
}

/* Raise t and its transients to the top of its layer. For the pager to work
 * properly it is necessary that RaiseWindow *always* sends a proper M_RESTACK
 * packet, even if the stacking order didn't change. */
void RaiseWindow(MvwmWindow *t, Bool is_client_request)
{
	BroadcastPacket(
		M_RAISE_WINDOW, 3, (long)FW_W(t), (long)FW_W_FRAME(t),
		(unsigned long)t);
	raise_or_lower_window(t, SM_RAISE, True, False, is_client_request);
	focus_grab_buttons_on_layer(t->layer);
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif
	return;
}

void LowerWindow(MvwmWindow *t, Bool is_client_request)
{
	BroadcastPacket(
		M_LOWER_WINDOW, 3, (long)FW_W(t), (long)FW_W_FRAME(t),
		(unsigned long)t);
	raise_or_lower_window(t, SM_LOWER, True, False, is_client_request);
	focus_grab_buttons_on_layer(t->layer);
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif
	return;
}

void RestackWindow(MvwmWindow *t, Bool is_client_request)
{
	raise_or_lower_window(t, SM_RESTACK, True, False, is_client_request);
	focus_grab_buttons_on_layer(t->layer);
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif
	return;
}

/* return true if stacking order changed */
Bool HandleUnusualStackmodes(
	unsigned int stack_mode, MvwmWindow *r, Window rw, MvwmWindow *s,
	Window sw)
{
	int do_restack = 0;
	MvwmWindow *t;

	/*  DBUG("HandleUnusualStackmodes", "called with %d, %lx\n",
	    stack_mode, s);*/

	if (
		((rw != FW_W(r)) ^ IS_ICONIFIED(r)) ||
		(s && (((sw != FW_W(s)) ^ IS_ICONIFIED(s)) ||
		       (r->Desk != s->Desk))))
	{
		/* one of the relevant windows is unmapped */
		return 0;
	}

	switch (stack_mode)
	{
	case TopIf:
		for (
			t = r->stack_prev; t != &Scr.MvwmRoot && !do_restack;
			t = t->stack_prev)
		{
			do_restack = ((s == NULL || s == t) && overlap(t, r));
		}
		if (do_restack)
		{
			RaiseWindow (r, True);
		}
		break;
	case BottomIf:
		for (
			t = r->stack_next; t != &Scr.MvwmRoot && !do_restack;
			t = t->stack_next)
		{
			do_restack = ((s == NULL || s == t) && overlap(t, r));
		}
		if (do_restack)
		{
			LowerWindow (r, True);
		}
		break;
	case Opposite:
		do_restack = (
			HandleUnusualStackmodes(TopIf, r, rw, s, sw) ||
			HandleUnusualStackmodes(BottomIf, r, rw, s, sw));
		break;
	}
	/*  DBUG("HandleUnusualStackmodes", "\t---> %d\n", do_restack);*/
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif
	return do_restack;
}

/*
  RBW - 01/07/1998  -  this is here temporarily - I mean to move it to
  libmvwm eventually, along with some other chain manipulation functions.
*/

void BroadcastRestackAllWindows(void)
{
	BroadcastRestack(Scr.MvwmRoot.stack_next, Scr.MvwmRoot.stack_prev);
	return;
}

/* send RESTACK packets for t, t->stack_prev and t->stack_next */
void BroadcastRestackThisWindow(MvwmWindow *t)
{
	BroadcastRestack(t->stack_prev, t->stack_next);
	return;
}

/* returns 0 if s and t are on the same layer, <1 if t is on a lower layer and
 * >1 if t is on a higher layer. */
int compare_window_layers(MvwmWindow *t, MvwmWindow *s)
{
	return t->layer - s->layer;
}

void set_default_layer(MvwmWindow *t, int layer)
{
	t->default_layer = layer;
	return;
}

void set_layer(MvwmWindow *t, int layer)
{
	t->layer = layer;
	return;
}

int get_layer(MvwmWindow *t)
{
	return t->layer;
}

/* This function recursively finds the transients of the window t and sets their
 * is_in_transient_subtree flag.  If a layer is given, only windows in this
 * layer are checked.  If the layer is < 0, all windows are considered.
 */
void mark_transient_subtree(
	MvwmWindow *t, int layer, int mark_mode, Bool do_ignore_icons,
	Bool use_window_group_hint)
{
	MvwmWindow *s;
	MvwmWindow *start;
	MvwmWindow *end;
	Bool is_finished;

	if (layer >= 0 && t->layer != layer)
	{
		return;
	}
	/* find out on which windows to operate */
	if (layer >= 0)
	{
		/* only work on the given layer */
		start = &Scr.MvwmRoot;
		end = &Scr.MvwmRoot;
		for (
			s = Scr.MvwmRoot.stack_next;
			s != &Scr.MvwmRoot && s->layer >= layer;
			s = s->stack_next)
		{
			if (s == t)
			{
				/* ignore the target window */
				continue;
			}
			if (s->layer == layer)
			{
				if (start == &Scr.MvwmRoot)
				{
					start = s;
				}
				end = s->stack_next;
			}
		}
	}
	else
	{
		/* work on complete window list */
		start = Scr.MvwmRoot.stack_next;
		end = &Scr.MvwmRoot;
	}
	/* clean the temporary flag in all windows and precalculate the
	 * transient frame windows */
	if (Scr.MvwmRoot.scratch.i == 0)
	{
		for (
			s = Scr.MvwmRoot.stack_next; s != &Scr.MvwmRoot;
			s = s->stack_next)
		{
			SET_IN_TRANSIENT_SUBTREE(s, 0);
			if (
				IS_TRANSIENT(s) &&
				(layer < 0 || layer == s->layer))
			{
				s->scratch.p = get_transientfor_mvwmwindow(s);
			}
			else
			{
				s->scratch.p = NULL;
			}
		}
	}
	Scr.MvwmRoot.scratch.i = 0;

	/* now loop over the windows and mark the ones we need to move */
	SET_IN_TRANSIENT_SUBTREE(t, 1);
	is_finished = False;
	while (!is_finished)
	{
		/* recursively search for all transient windows */
		is_finished = True;
		for (s = start; s != end; s = s->stack_next)
		{

			if (
				__mark_transient_subtree_test(
					s, start, end, mark_mode,
					do_ignore_icons,
					use_window_group_hint))
			{
				is_finished = False;
			}
		} /* for */
	} /* while */

	return;
}

void new_layer(MvwmWindow *fw, int layer)
{
	MvwmWindow *s;
	MvwmWindow *target;
	MvwmWindow *prev;
	MvwmWindow list_head;
	int add_after_layer;
	int count;
	int old_layer;
	Bool do_lower;

	if (layer < 0)
	{
		layer = 0;
	}
	fw = get_transientfor_top_mvwmwindow(fw);
	if (layer == fw->layer)
	{
		return;
	}
	old_layer = fw->layer;
	list_head.stack_next = &list_head;
	list_head.stack_prev = &list_head;
	count = collect_transients_recursive(
		fw, &list_head, fw->layer,
		(layer < fw->layer) ? SM_LOWER : SM_RAISE, True);
	if (count == 0)
	{
		/* no windows to move */
		return;
	}
	add_after_layer = layer;
	if (layer < fw->layer)
	{
		/* lower below the windows in the new (lower) layer */
		add_after_layer = layer;
		do_lower = True;
	}
	else
	{
		/* raise above the windows in the new (higher) layer */
		add_after_layer = layer + 1;
		do_lower = False;
	}
	/* find the place to insert the windows */
	for (
		target = Scr.MvwmRoot.stack_next; target != &Scr.MvwmRoot;
		target = target->stack_next)
	{
		if (target->layer < add_after_layer)
		{
			/* add all windows before the current window */
			break;
		}
	}
	/* insert windows at new position */
	add_windowlist_to_stack_ring_after(&list_head, target->stack_prev);
	prev = NULL;
	for (
		s = list_head.stack_next; prev != list_head.stack_prev;
		prev = s, s = s->stack_next)
	{
		s->layer = layer;
		/* redraw title and buttons to update layer buttons */
		border_draw_decorations(
			s, PART_TITLEBAR, (Scr.Hilite == fw), True, CLEAR_NONE,
			NULL, NULL);
		EWMH_SetWMState(fw, False);
	}
	/* move the windows without modifying their stacking order */
	__restack_window_list(
		list_head.stack_next->stack_prev, target, count, (count > 1),
		do_lower);
	focus_grab_buttons_on_layer(layer);
	focus_grab_buttons_on_layer(old_layer);

	return;
}

/*  RBW - 11/13/1998 - 2 new fields to init - stacking order chain.  */
void init_stack_and_layers(void)
{
	Scr.BottomLayer	 = DEFAULT_BOTTOM_LAYER;
	Scr.DefaultLayer = DEFAULT_DEFAULT_LAYER;
	Scr.TopLayer	 = DEFAULT_TOP_LAYER;
	Scr.MvwmRoot.stack_next = &Scr.MvwmRoot;
	Scr.MvwmRoot.stack_prev = &Scr.MvwmRoot;
	set_layer(&Scr.MvwmRoot, DEFAULT_ROOT_WINDOW_LAYER);
	return;
}

Bool is_on_top_of_layer(MvwmWindow *fw)
{
	return __is_on_top_of_layer(fw, False);
}

Bool is_on_top_of_layer_and_above_unmanaged(MvwmWindow *fw)
{
	return __is_on_top_of_layer(fw, True);
}

/* ----------------------------- built in functions ----------------------- */

void CMD_Raise(F_CMD_ARGS)
{
	RaiseWindow(exc->w.fw, False);

	return;
}

void CMD_Lower(F_CMD_ARGS)
{
	LowerWindow(exc->w.fw, False);

	return;
}

void CMD_RestackTransients(F_CMD_ARGS)
{
	RestackWindow(exc->w.fw, False);

	return;
}

void CMD_RaiseLower(F_CMD_ARGS)
{
	Bool ontop;
	MvwmWindow * const fw = exc->w.fw;

	ontop = is_on_top_of_layer_ignore_rom(fw);
	if (ontop)
	{
		LowerWindow(fw, False);
	}
	else
	{
		RaiseWindow(fw, False);
	}

	return;
}

void CMD_Layer(F_CMD_ARGS)
{
	int n, layer, val[2];
	char *token;
	MvwmWindow * const fw = exc->w.fw;

	if (fw == NULL)
	{
		return;
	}
	token = PeekToken(action, NULL);
	if (StrEquals("default", token))
	{
		layer = fw->default_layer;
	}
	else
	{
		n = GetIntegerArguments(action, NULL, val, 2);

		layer = fw->layer;
		if ((n == 1) ||
		    ((n == 2) && (val[0] != 0)))
		{
			layer += val[0];
		}
		else if ((n == 2) && (val[1] >= 0))
		{
			layer = val[1];
		}
		else
		{
			layer = fw->default_layer;
		}
	}
	if (layer < 0)
	{
		layer = 0;
	}
	new_layer(fw, layer);
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif

	return;
}

void CMD_DefaultLayers(F_CMD_ARGS)
{
	char *bot = NULL;
	char *def = NULL;
	char *top = NULL;
	int i;

	bot = PeekToken(action, &action);
	if (bot)
	{
		i = strtonum(bot, 0, INT_MAX, NULL);
		if (i < 0)
		{
			mvwm_msg(
				ERR, "DefaultLayers",
				"Layer must be non-negative." );
		}
		else
		{
			Scr.BottomLayer = i;
		}
	}
	def = PeekToken(action, &action);
	if (def)
	{
		i = strtonum(def, 0, INT_MAX, NULL);
		if (i < 0)
		{
			mvwm_msg(
				ERR, "DefaultLayers",
				"Layer must be non-negative." );
		}
		else
		{
			Scr.DefaultLayer = i;
		}
	}
	top = PeekToken(action, &action);
	if (top)
	{
		i = strtonum(top, 0, INT_MAX, NULL);
		if (i < 0)
		{
			mvwm_msg(
				ERR, "DefaultLayers",
				"Layer must be non-negative." );
		}
		else
		{
			Scr.TopLayer = i;
		}
	}
#ifdef DEBUG_STACK_RING
	verify_stack_ring_consistency();
#endif

	return;
}
