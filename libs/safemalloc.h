/* -*-c-*- */

#ifndef SAFEMALLOC_H
#define SAFEMALLOC_H

void *mvwm_malloc(size_t);
void *mvwm_calloc(size_t, size_t);
void *mvwm_realloc(void *, size_t, size_t);
char *mvwm_strdup(const char *);

#endif
