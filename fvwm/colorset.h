/* -*-c-*- */

#ifndef COLORSET_H
#define COLORSET_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

void            parse_colorset(int, char *);
void            cleanup_colorsets(void);
void            alloc_colorset(int);
void            update_root_transparent_colorset(Atom);

#endif /* COLORSET_H */
