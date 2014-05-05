/* Memory management routines...
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* Originally from OpenBSD */

#include "config.h"

#include <stdio.h>
#include <err.h>
#include <sys/param.h>
#include <stdint.h>
#include "safemalloc.h"

void *
xmalloc(size_t length)
{
	void	*ptr;

	if (length == 0)
		errx(1, "malloc: zero size.");

	if ((ptr = malloc(length)) == NULL)
		err(1, "malloc: couldn't allocate");

	return (ptr);
}

void *
xcalloc(size_t num, size_t length)
{
	void	*ptr;

	if (num == 0 || length == 0)
		errx(1, "calloc:  zero size");

	if (SIZE_MAX / num < length)
		errx(1, "num * length > SIZE_MAX");

	if ((ptr = calloc(num, length)) == NULL)
		err(1, "calloc:  couldn't allocate");

	return (ptr);
}

void *
xrealloc(void *oldptr, size_t nmemb, size_t size)
{
	size_t	 newsize = nmemb * size;
	void	*newptr;

	if (newsize == 0)
		errx(1, "zero size");
	if (SIZE_MAX / nmemb < size)
		errx(1, "nmemb * size > SIZE_MAX");
	if ((newptr = realloc(oldptr, newsize)) == NULL)
		err(1, "xrealloc failed");

	return (newptr);
}

char *
xstrdup(const char *s)
{
	char	*ptr;
	size_t	 len;

	len = strlen(s) + 1;
	ptr = xmalloc(len);

	strlcpy(ptr, s, len);

	return (ptr);
}
