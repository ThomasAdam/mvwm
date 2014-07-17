/* -*-c-*- */

#ifndef EXPAND_H
#define EXPAND_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

char           *expand_vars(char *, char **, Bool, Bool, cond_rc_t *,
    const exec_context_t *);

#endif /* EXPAND_H */
