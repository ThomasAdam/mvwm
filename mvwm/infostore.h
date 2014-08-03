/* -*-c-*- */

#ifndef INFOSTORE_H
#define INFOSTORE_H

#include "libs/queue.h"

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

struct meta_info {
	char           *key;
	char           *value;

	TAILQ_ENTRY(meta_info) entry;
};

TAILQ_HEAD(meta_infos, meta_info);
struct meta_infos meta_info_q;

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

struct meta_info	*new_metainfo(void);
void			 insert_metainfo(char *, char *);
char			*get_metainfo_value(const char *);
void			 print_infostore(void);

#endif /* INFOSTORE_H */
