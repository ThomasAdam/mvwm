/* -*-c-*- */

#ifndef CONDITIONAL_H
#define CONDITIONAL_H

/* ---------------------------- included header files ---------------------- */

/* ---------------------------- global definitions ------------------------- */

/* ---------------------------- global macros ------------------------------ */

/* ---------------------------- type definitions --------------------------- */

/* ---------------------------- forward declarations ----------------------- */

/* ---------------------------- exported variables (globals) --------------- */

/* ---------------------------- interface functions ------------------------ */

/* Condition matching routines */
char           *CreateFlagString(char *, char **);
void            DefaultConditionMask(WindowConditionMask *);
void            CreateConditionMask(char *, WindowConditionMask *);
void            FreeConditionMask(WindowConditionMask *);
Bool            MatchesConditionMask(FvwmWindow *, WindowConditionMask *);

#endif /* CONDITIONAL_H */
