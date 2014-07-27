/* -*-c-*- */

#ifndef FIMAGE_H
#define FIMAGE_H

/* ---------------------------- included header files ---------------------- */

#include "config.h"

#include "FShm.h"

/* ---------------------------- type definitions --------------------------- */

typedef struct
{
	XImage         *im;
	FShmSegmentInfo *shminfo;
} FImage;

/* ---------------------------- interface functions ------------------------ */

FImage	*FCreateFImage(Display *, Visual *, unsigned int, int, unsigned int,
    unsigned int);

FImage	*FGetFImage(Display *, Drawable, Visual *, unsigned int, int, int,
    unsigned int, unsigned int, unsigned long, int);

void	 FPutFImage(Display *, Drawable, GC, FImage *, int, int, int, int,
    unsigned int, unsigned int);

void	 FDestroyFImage(Display *, FImage *);

#endif /* FIMAGE_H */
