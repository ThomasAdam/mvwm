#ifndef SAFEMALLOC_H
#define SAFEMALLOC_H

#include <stdarg.h>

/* Attribute to make gcc check printf-like arguments. */
#define printflike1 __attribute__ ((format (printf, 1, 2)))
#define printflike2 __attribute__ ((format (printf, 2, 3)))
#define printflike3 __attribute__ ((format (printf, 3, 4)))
#define printflike4 __attribute__ ((format (printf, 4, 5)))
#define printflike5 __attribute__ ((format (printf, 5, 6)))

void	*xmalloc(size_t);
void	*xcalloc(size_t, size_t);
void	*xrealloc(void *, size_t, size_t);
char	*xstrdup(const char *);
int	 xasprintf(char **, const char *, ...);
int	 xvasprintf(char **, const char *, va_list);
int printflike3  xsnprintf(char *, size_t, const char *, ...);
int              xvsnprintf(char *, size_t, const char *, va_list);
#endif
