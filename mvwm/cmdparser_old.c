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
#include "libs/defaults.h"
#include "libs/Parse.h"

#include <assert.h>

#include "cmdparser.h"
#include "cmdparser_old.h"
#include "mvwm.h"
#include "misc.h"

#define func_t int
#include "functable.h"
#undef func_t

/* ---------------------------- local definitions -------------------------- */

/* ---------------------------- local macros ------------------------------- */

/* ---------------------------- imports ------------------------------------ */

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- local variables ---------------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- local functions ---------------------------- */

static int ocp_create_context(
	cmdparser_context_t *dest_c, cmdparser_context_t *caller_c, char *line,
	char *pos_args[])
{
	/* allocate the necessary resources */
	memset(dest_c, 0, sizeof(*dest_c));
	dest_c->line = line;
	/*!!!should this be in handle_line_start instead???*/
	if (caller_c != NULL)
	{
		if (dest_c->call_depth > MAX_FUNCTION_DEPTH)
		{
			mvwm_msg(
				ERR, "ocp_create_context",
				"Function '%s' called with a depth of %i, "
				"stopping function execution!",
				line, dest_c->call_depth);

			return -1;
		}
	}
	else
	{
		dest_c->call_depth = 1;
	}
	if (pos_args != NULL)
	{
		int i;

		dest_c->pos_args[0] = pos_args[0];
		for (
			i = 1;
			i < CMDPARSER_NUM_POS_ARGS + 1 && pos_args[i] != NULL;
			i++)
		{
			dest_c->pos_args[i - 1] = pos_args[i];
		}
	}
	/*!!!allocate stuff*/
	dest_c->is_created = 1;

	return 0;
}

static void ocp_destroy_context(cmdparser_context_t *c)
{
	/* free the resources allocated in the create function */
	if (c->is_created == 0)
	{
		return;
	}
	/*!!!free stuff*/
	c->is_created = 0;

	return;
}

static int ocp_handle_line_start(cmdparser_context_t *c)
{
	/*!!! remove assertion later*/
	assert(c->cline == NULL);
	if (!c->line)
	{
		/* nothing to do */
		return 1;
	}
	/* ignore whitespace at the beginning of all config lines */
	c->cline = SkipSpaces(c->line, NULL, 0);
	if (!c->cline || c->cline[0] == 0)
	{
		/* impossibly short command */
		return -1;
	}
	if (c->cline[0] == '#')
	{
		/* a comment */
		return 1;
	}

	return 0;
}

static cmdparser_prefix_flags_t ocp_handle_line_prefix(cmdparser_context_t *c)
{
	cmdparser_prefix_flags_t flags;
	char *taction;
	char *trash;
	char *trash2;

	flags = CP_PREFIX_NONE;
	if (c->cline[0] == '-')
	{
		c->cline++;
		flags |= CP_PREFIX_MINUS;
	}
	taction = c->cline;
	trash = PeekToken(taction, &trash2);
	while (trash != NULL)
	{
		if (StrEquals(trash, PRE_SILENT))
		{
			flags |= CP_PREFIX_SILENT;
			taction = trash2;
			trash = PeekToken(taction, &trash2);
		}
		else if (StrEquals(trash, PRE_KEEPRC))
		{
			flags |= CP_PREFIX_KEEPRC;
			taction = trash2;
			trash = PeekToken(taction, &trash2);
		}
		else
		{
			break;
		}
	}

	return flags;
}

/* ---------------------------- local variables ---------------------------- */

static cmdparser_hooks_t old_parser_hooks =
{
	ocp_create_context,
	ocp_handle_line_start,
	ocp_handle_line_prefix,
	ocp_destroy_context
};

/* ---------------------------- interface functions ------------------------ */

/* return the hooks structure of the old parser */
const cmdparser_hooks_t *cmdparser_old_get_hooks(void)
{
	return &old_parser_hooks;
}
