/* -*-c-*- */
/* Mvwm colorset technology is Copyright (C) 1999 Joey Shutup
   http://www.streetmap.co.uk/streetmap.dll?Postcode2Map?BS24+9TZ
   You may use this code for any purpose, as long as the original copyright
   and this notice remains in the source code and all documentation
*/

#ifndef LIBS_COLORSETS_H
#define LIBS_COLORSETS_H

typedef struct
{
	Pixel           fg;
	Pixel           bg;
	Pixel           hilite;
	Pixel           shadow;
	Pixel           fgsh;
	Pixel           tint;
	Pixel           icon_tint;
	Pixmap          pixmap;
	Pixmap          shape_mask;
	unsigned int    width:12;
	unsigned int    height:12;
	unsigned int    pixmap_type:3;
	unsigned int    shape_width:12;
	unsigned int    shape_height:12;
	unsigned int    shape_type:2;
	unsigned int    do_dither_icon:1;
	unsigned int    fg_alpha_percent:7;
	unsigned int    tint_percent:7;
	unsigned int    icon_alpha_percent:7;
	unsigned int    icon_tint_percent:7;
#ifdef MVWM_COLORSET_PRIVATE
	/*
	 * mvwm/colorset.c use only
	 */
	Pixel           fg_tint;
	Pixel           fg_saved;
	Pixel           bg_tint;
	Pixel           bg_saved;
	Pixmap          mask;
	Pixmap          alpha_pixmap;
	char           *pixmap_args;
	char           *gradient_args;
	char            gradient_type;
	unsigned int    color_flags;
	MvwmPicture    *picture;
	Pixel          *pixels;
	int             nalloc_pixels;
	int             fg_tint_percent;
	int             bg_tint_percent;
	short           image_alpha_percent;
	Bool            dither;
	Bool            allows_buffered_transparency;
	Bool            is_maybe_root_transparent;
#endif
} colorset_t;

#define PIXMAP_TILED 0
#define PIXMAP_STRETCH_X 1
#define PIXMAP_STRETCH_Y 2
#define PIXMAP_STRETCH 3
#define PIXMAP_STRETCH_ASPECT 4
#define PIXMAP_ROOT_PIXMAP_PURE 5
#define PIXMAP_ROOT_PIXMAP_TRAN 6

#define SHAPE_TILED 0
#define SHAPE_STRETCH 1
#define SHAPE_STRETCH_ASPECT 2

#ifdef MVWM_COLORSET_PRIVATE
#define FG_SUPPLIED 0x1
#define BG_SUPPLIED 0x2
#define HI_SUPPLIED 0x4
#define SH_SUPPLIED 0x8
#define FGSH_SUPPLIED 0x10
#define FG_CONTRAST 0x20
#define BG_AVERAGE  0x40
#define TINT_SUPPLIED  0x80
#define FG_TINT_SUPPLIED  0x100
#define BG_TINT_SUPPLIED  0x200
#define ICON_TINT_SUPPLIED 0x400
#endif

/* colorsets are stored as an array of structs to permit fast dereferencing */
extern colorset_t *Colorset;

/* some macro for transparency */
#define CSET_IS_TRANSPARENT(cset) \
    (cset >= 0 && (Colorset[cset].pixmap == ParentRelative || \
		   (Colorset[cset].pixmap != None && \
		    (Colorset[cset].pixmap_type == PIXMAP_ROOT_PIXMAP_TRAN ||\
		     Colorset[cset].pixmap_type == PIXMAP_ROOT_PIXMAP_PURE))))
#define CSET_IS_TRANSPARENT_PR(cset) \
    (cset >= 0 && Colorset[cset].pixmap == ParentRelative)
#define CSET_IS_TRANSPARENT_ROOT(cset) \
    (cset >= 0 && Colorset[cset].pixmap != None && \
     (Colorset[cset].pixmap_type == PIXMAP_ROOT_PIXMAP_TRAN ||\
      Colorset[cset].pixmap_type == PIXMAP_ROOT_PIXMAP_PURE))
#define CSET_IS_TRANSPARENT_PR_PURE(cset) \
    (cset >= 0 && Colorset[cset].pixmap == ParentRelative && \
     Colorset[cset].tint_percent == 0)
#define CSET_IS_TRANSPARENT_ROOT_PURE(cset) \
    (cset >= 0 && Colorset[cset].pixmap != None && \
     Colorset[cset].pixmap_type == PIXMAP_ROOT_PIXMAP_PURE)
#define CSET_IS_TRANSPARENT_ROOT_TRAN(cset) \
    (cset >= 0 && Colorset[cset].pixmap != None && \
     Colorset[cset].pixmap_type == PIXMAP_ROOT_PIXMAP_TRAN)
#define CSET_IS_TRANSPARENT_PR_TINT(cset) \
    (cset >= 0 && Colorset[cset].pixmap == ParentRelative && \
     Colorset[cset].tint_percent > 0)
#define CSET_HAS_PIXMAP(cset) \
    (cset >= 0 && Colorset[cset].pixmap)
#define CSET_HAS_PIXMAP_TILED(cset) \
    (CSET_HAS_PIXMAP(cset) && (Colorset[cset].pixmap_type & PIXMAP_TILED))
#define CSET_HAS_PIXMAP_STRETCH_X(cset) \
    (CSET_HAS_PIXMAP(cset) && (Colorset[cset].pixmap_type & PIXMAP_STRETCH_X))
#define CSET_HAS_PIXMAP_STRETCH_Y(cset) \
    (CSET_HAS_PIXMAP(cset) && (Colorset[cset].pixmap_type & PIXMAP_STRETCH_Y))
#define CSET_HAS_PIXMAP_STRETCH(cset) \
    (CSET_HAS_PIXMAP(cset) && (Colorset[cset].pixmap_type & PIXMAP_STRETCH))
#define CSET_HAS_PIXMAP_STRETCH_ASPECT(cset) \
    (CSET_HAS_PIXMAP(cset) && \
     (Colorset[cset].pixmap_type & PIXMAP_STRETCH_ASPECT))
#define CSET_PIXMAP_IS_XY_STRETCHED(cset) \
    (CSET_HAS_PIXMAP_STRETCH(cset) || CSET_HAS_PIXMAP_STRETCH_ASPECT(cset))
#define CSET_PIXMAP_IS_X_STRETCHED(cset) \
    (CSET_PIXMAP_IS_XY_STRETCHED(cset) || CSET_HAS_PIXMAP_STRETCH_X(cset))
#define CSET_PIXMAP_IS_Y_STRETCHED(cset) \
    (CSET_PIXMAP_IS_XY_STRETCHED(cset) || CSET_HAS_PIXMAP_STRETCH_Y(cset))
#define CSET_PIXMAP_IS_TILED(cset) \
    (CSET_HAS_PIXMAP_TILED(cset))

/* some macro for transparency */
#define CSETS_IS_TRANSPARENT(cset) \
    (cset != NULL && (cset->pixmap == ParentRelative || \
		   (cset->pixmap != None && \
		    (cset->pixmap_type == PIXMAP_ROOT_PIXMAP_TRAN ||\
		     cset->pixmap_type == PIXMAP_ROOT_PIXMAP_PURE))))
#define CSETS_IS_TRANSPARENT_ROOT(cset) \
    (cset != NULL && cset->pixmap != None && \
     (cset->pixmap_type == PIXMAP_ROOT_PIXMAP_TRAN ||\
      cset->pixmap_type == PIXMAP_ROOT_PIXMAP_PURE))
#define CSETS_IS_TRANSPARENT_PR_PURE(cset) \
    (cset != NULL && cset->pixmap == ParentRelative && \
     cset->tint_percent == 0)
#define CSETS_IS_TRANSPARENT_ROOT_PURE(cset) \
    (cset != NULL && cset->pixmap != None && \
     cset->pixmap_type == PIXMAP_ROOT_PIXMAP_PURE)
#define CSETS_IS_TRANSPARENT_ROOT_TRAN(cset) \
    (cset != NULL && cset->pixmap != None && \
     cset->pixmap_type == PIXMAP_ROOT_PIXMAP_TRAN)
#define CSETS_IS_TRANSPARENT_PR_TINT(cset) \
    (cset != NULL && cset->pixmap == ParentRelative && \
     cset->tint_percent > 0)

#ifndef MVWM_COLORSET_PRIVATE
/* Create n new colorsets, mvwm/colorset.c does its own thing (different size)
 */
void            AllocColorset(int);
#endif

/* dump one */
char           *DumpColorset(int, colorset_t *);
/* load one */
int             LoadColorset(char *);

Pixmap          CreateOffsetBackgroundPixmap(Display *, Window, int, int, int,
    int, colorset_t *, unsigned int, GC, Bool);
Pixmap          CreateBackgroundPixmap(Display *, Window, int, int,
    colorset_t *, unsigned int, GC, Bool);
Pixmap          ScrollPixmap(Display *, Pixmap, GC, int, int, int, int,
    unsigned int);
void            SetWindowBackgroundWithOffset(Display *, Window, int, int,
    unsigned int, unsigned int, colorset_t *, unsigned int, GC, Bool);
void            SetWindowBackground(Display *, Window, int, int, colorset_t *,
    unsigned int, GC, Bool);
void            GetWindowBackgroundPixmapSize(colorset_t *, int, int, int *,
    int *);
Bool            UpdateBackgroundTransparency(Display *, Window, int, int,
    colorset_t *, unsigned int, GC, Bool);
void            SetRectangleBackground(Display *, Window, int, int, int, int,
    colorset_t *, unsigned int, GC);
void            SetClippedRectangleBackground(Display *, Window, int, int, int,
    int, XRectangle *, colorset_t *, unsigned int, GC);
#endif
