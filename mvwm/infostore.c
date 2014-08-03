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
#include "mvwm.h"
#include "externs.h"
#include "libs/FGettext.h"
#include "libs/Parse.h"
#include "libs/Strings.h"
#include "infostore.h"
#include "misc.h"
#include "functions.h"

/* ---------------------------- local definitions -------------------------- */

/* ---------------------------- local macros ------------------------------- */

/* ---------------------------- imports ------------------------------------ */

/* ---------------------------- included code files ------------------------ */

/* ---------------------------- local types -------------------------------- */

/* ---------------------------- forward declarations ----------------------- */

static void     delete_metainfo(const char *);
static int      get_metainfo_length(void);

/* ---------------------------- local variables ---------------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- local functions ---------------------------- */

struct meta_info *
new_metainfo(void)
{
	struct meta_info       *mi;

	mi = mvwm_calloc(1, sizeof *mi);

	return mi;
}

void
insert_metainfo(char *key, char *value)
{
	struct meta_info       *mi;
	struct meta_info       *mi_new;

	TAILQ_FOREACH(mi, &meta_info_q, entry) {
		if (StrEquals(mi->key, key)) {
			/*
			 * We already have an entry in the list with that key, so
			 * update the value of it only.
			 */
			free(mi->value);
			CopyString(&mi->value, value);

			return;
		}
	}

	/*
	 * It's a new item, add it to the list.
	 */
	mi_new = new_metainfo();
	mi_new->key = key;
	CopyString(&mi_new->value, value);

	if (TAILQ_EMPTY(&meta_info_q))
		TAILQ_INSERT_HEAD(&meta_info_q, mi_new, entry);
	else
		TAILQ_INSERT_TAIL(&meta_info_q, mi_new, entry);
}

static void
delete_metainfo(const char *key)
{
	struct meta_info       *mi;

	TAILQ_FOREACH(mi, &meta_info_q, entry) {
		if (StrEquals(mi->key, key)) {
			TAILQ_REMOVE(&meta_info_q, mi, entry);
			free(mi->key);
			free(mi->value);
			free(mi);

			break;
		}
	}
}

inline char    *
get_metainfo_value(const char *key)
{
	struct meta_info       *mi;

	TAILQ_FOREACH(mi, &meta_info_q, entry) {
		if (StrEquals(mi->key, key))
			return (mi->value);
	}

	return (NULL);
}

static inline int
get_metainfo_length(void)
{
	struct meta_info       *mi;
	int			count = 0;

	TAILQ_FOREACH(mi, &meta_info_q, entry)
		count++;

	return (count);
}

void
print_infostore(void)
{
	struct meta_info       *mi;

	fprintf(stderr, "Current items in infostore (key, value):\n\n");

	if (get_metainfo_length() == 0) {
		fprintf(stderr,
		    "No items are currently stored in the infostore.\n");
		return;
	}

	TAILQ_FOREACH(mi, &meta_info_q, entry)
		fprintf(stderr, "%s\t%s\n", mi->key, mi->value);
}

/* ---------------------------- interface functions ------------------------ */

/* ---------------------------- builtin commands --------------------------- */
void
CMD_InfoStoreAdd(F_CMD_ARGS)
{
	char           *key, *value;
	char           *token;

	token = PeekToken(action, &action);
	key = value = NULL;

	if (token)
		key = strdup(token);

	token = PeekToken(action, &action);

	if (token)
		value = strdup(token);

	if (!key || !value) {
		mvwm_msg(ERR, "CMD_InfoStore", "Bad arguments given.");
		return;
	}

	insert_metainfo(key, value);
	free(value);

	return;
}

void
CMD_InfoStoreRemove(F_CMD_ARGS)
{
	char           *token;

	token = PeekToken(action, &action);

	if (!token) {
		mvwm_msg(ERR, "CMD_InfoStoreRemove",
		    "No key given to remove item.");
		return;
	}

	delete_metainfo(token);

	return;
}
