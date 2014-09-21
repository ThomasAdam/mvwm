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

#include "libs/mvwmlib.h"
#include "libs/Picture.h"
#include "libs/Strings.h"

#include "mvwm.h"
#include "cmdparser.h"
#include "execcontext.h"
#include "functable.h"
#include "functable.h"
#include "functable_complex.h"
#include "misc.h"
#include "screen.h"

/* ---------------------------- local definitions -------------------------- */

/* ---------------------------- local macros ------------------------------- */

/* ---------------------------- imports ------------------------------------ */

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

/* ---------------------------- local variables ---------------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- local functions ---------------------------- */

MvwmFunction *NewMvwmFunction(const char *name)
{
	MvwmFunction *tmp;

	tmp = mvwm_malloc(sizeof *tmp);
	tmp->next_func = Scr.functions;
	tmp->first_item = NULL;
	tmp->last_item = NULL;
	tmp->name = stripcpy(name);
	tmp->use_depth = 0;
	Scr.functions = tmp;

	return tmp;
}

void DestroyFunction(MvwmFunction *func)
{
	FunctionItem *fi,*tmp2;
	MvwmFunction *tmp, *prev;

	if (func == NULL)
	{
		return;
	}

	tmp = Scr.functions;
	prev = NULL;
	while (tmp && tmp != func)
	{
		prev = tmp;
		tmp = tmp->next_func;
	}
	if (tmp != func)
	{
		return;
	}

	if (func->use_depth != 0)
	{
		mvwm_msg(
			ERR,"DestroyFunction",
			"Function %s is in use (depth %d)", func->name,
			func->use_depth);
		return;
	}

	if (prev == NULL)
	{
		Scr.functions = func->next_func;
	}
	else
	{
		prev->next_func = func->next_func;
	}

	free(func->name);

	fi = func->first_item;
	while (fi != NULL)
	{
		tmp2 = fi->next_item;
		if (fi->action != NULL)
		{
			free(fi->action);
		}
		free(fi);
		fi = tmp2;
	}
	free(func);

	return;
}

/* ---------------------------- interface functions ------------------------ */

/* find_complex_function expects a token as the input. Make sure you have used
 * GetNextToken before passing a function name to remove quotes */
MvwmFunction *find_complex_function(const char *function_name)
{
	MvwmFunction *func;

	if (function_name == NULL || *function_name == 0)
	{
		return NULL;
	}
	func = Scr.functions;
	while (func != NULL)
	{
		if (func->name != NULL)
		{
			if (strcasecmp(function_name, func->name) == 0)
			{
				return func;
			}
		}
		func = func->next_func;
	}

	return NULL;
}

