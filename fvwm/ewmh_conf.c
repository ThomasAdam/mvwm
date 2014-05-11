/* -*-c-*- */
/* Copyright (C) 2001  Olivier Chapuis */
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

#include <X11/Xlib.h>
#include <X11/Xmd.h>

#include "libs/fvwmlib.h"
#include "libs/Parse.h"
#include "libs/Strings.h"
#include "fvwm.h"
#include "cursor.h"
#include "functions.h"
#include "misc.h"
#include "screen.h"
#include "update.h"
#include "stack.h"
#include "externs.h"
#include "icons.h"
#include "ewmh.h"
#include "ewmh_intern.h"
#include "move_resize.h"

/*
 * CMDS
 */

static
void set_state_workaround(void)
{
	FvwmWindow *t;

	for (t = Scr.FvwmRoot.next; t != NULL; t = t->next)
	{
		if ((t->Desk != Scr.CurrentDesk) &&
		    (!is_window_sticky_across_desks(t) &&
		     !IS_ICON_UNMAPPED(t)))
		{
			if (Scr.bo.do_enable_ewmh_iconic_state_workaround)
			{
				SetMapStateProp(t, NormalState);
			}
			else
			{
				SetMapStateProp(t, IconicState);
			}
		}
	}
}

Bool EWMH_BugOpts(char *opt, Bool toggle)
{
	Bool save_isw = Scr.bo.do_enable_ewmh_iconic_state_workaround;

	if (StrEquals(opt,"EWMHIconicStateWorkaround"))
	{
		switch (toggle)
		{
		case -1:
			Scr.bo.do_enable_ewmh_iconic_state_workaround ^= 1;
			break;
		case 0:
		case 1:
			Scr.bo.do_enable_ewmh_iconic_state_workaround = toggle;
			break;
		default:
			Scr.bo.do_enable_ewmh_iconic_state_workaround = 0;
			break;
		}
		if (save_isw != Scr.bo.do_enable_ewmh_iconic_state_workaround)
		{
			set_state_workaround();
		}
		return True;
	}

	return False;
}

void CMD_EwmhNumberOfDesktops(F_CMD_ARGS)
{
	int val[2];
	int num;

	num = GetIntegerArguments(action, NULL, val, 2);
	if ((num != 1 && num != 2) || val[0] < 1 ||
	    (num == 2 && val[1] < val[0] && val[1] != 0))
	{
		fvwm_msg(ERR,"EwmhNumberOfDesktops",
			 "Bad arguments to EwmhNumberOfDesktops");
		return;
	}

	if (num == 2 && ewmhc.MaxDesktops != val[1])
	{
		ewmhc.MaxDesktops = val[1];
		num = 3;
	}
	else if (num == 1 && ewmhc.MaxDesktops != 0)
	{
		ewmhc.MaxDesktops = 0;
		num = 3;
	}

	if (ewmhc.NumberOfDesktops != val[0])
	{
		ewmhc.NumberOfDesktops = val[0];
		num = 3;
	}

	if (num == 3)
	{
		ewmhc.NeedsToCheckDesk = True;
		EWMH_SetNumberOfDesktops();
	}
}

void CMD_EwmhBaseStruts(F_CMD_ARGS)
{
	int val[4];

	if (GetIntegerArguments(action, NULL, val, 4) != 4 ||
	    val[0] < 0 || val[1] < 0 || val[2] < 0 || val[3] < 0)
	{
		fvwm_msg(ERR,"CMD_EwmhBaseStruts",
			 "EwmhBaseStruts needs four positive arguments");
		return;
	}

	if (ewmhc.BaseStrut.left != val[0] ||
	    ewmhc.BaseStrut.right != val[1] ||
	    ewmhc.BaseStrut.top != val[2] ||
	    ewmhc.BaseStrut.bottom != val[3])
	{
		ewmhc.BaseStrut.left   = val[0];
		ewmhc.BaseStrut.right  = val[1];
		ewmhc.BaseStrut.top    = val[2];
		ewmhc.BaseStrut.bottom = val[3];

		EWMH_UpdateWorkArea();
	}
}
/*
 * Styles
 */

Bool EWMH_CMD_Style(char *token, window_style *ptmpstyle, int on)
{
	int found = False;

	return found;
}
