/* -*-c-*- */

#ifndef Picture_H
#define Picture_H

#include "PictureBase.h"
#include "PictureImageLoader.h"

/** Manipulating MvwmPictures **/

/**
 * For PGetMvwmPicture() and PCacheMvwmPicture(), setting
 * ImagePath to NULL means "search the default image path".
 **/

/* <pubfunc>PGetMvwmPicture
 * <description>
 * Return an MvwmPicture loaded from the file pictureName found in the
 * ImagePath.. If ImagePath is NULL the default image path is used.
 * </description>
 */
MvwmPicture    *PGetMvwmPicture(Display *dpy, Window win, char *ImagePath,
    const char *pictureName, MvwmPictureAttributes fpa);

/* <pubfunc>PFreeMvwmPictureData
 * <description>
 * Just free the data allocated by PGetMvwmPicture. This function does not
 * Free the pixmaps for example.
 * </description>
 */
void            PFreeMvwmPictureData(MvwmPicture *p);

/* <pubfunc>PCacheMvwmPicture
 * <description>
 * Return the MvwmPicture loaded from the file pictureName found in the
 * ImagePath. Fisrt the picture is searched in the MvwmPicture cache (so
 * if this picture has been already loaded it is not loaded again and a
 * weight is added to the found picture). If the picture is not in the cache
 * it is loaded from the file and added to the MvwmPicture cache.
 * If ImagePath is NULL the default image path is used.
 * </description>
 */
MvwmPicture    *PCacheMvwmPicture(Display *dpy, Window win, char *ImagePath,
    const char *pictureName, MvwmPictureAttributes fpa);

/* <pubfunc>PLoadMvwmPictureFromPixmap
 * <description>
 * Return a MvwmPicture from the given data.
 * </description>
 */
MvwmPicture    *PLoadMvwmPictureFromPixmap(Display *dpy, Window win,
    char *name, Pixmap pixmap, Pixmap mask, Pixmap alpha, int width,
    int height, int nalloc_pixels, Pixel *alloc_pixels, int no_limit);

/* <pubfunc>PDestroyMvwmPicture
 * <description>
 * Return a MvwmPicture from the given data. The picture is added to the
 * MvwmPicture cache. This is not really useful as it is not possible
 * to really cache a picture from the given data.
 * </description>
 */
MvwmPicture    *PCacheMvwmPictureFromPixmap(Display *dpy, Window win,
    char *name, Pixmap pixmap, Pixmap mask, Pixmap alpha, int width,
    int height, int nalloc_pixels, Pixel *alloc_pixels, int no_limit);

/* <pubfunc>PDestroyMvwmPicture
 * <description>
 * Remove a weight to the MvwmPicture p from the MvwmPicture cache.
 * If the weight is zero the allocated datas from p are freed
 * </description>
 */
void            PDestroyMvwmPicture(Display *dpy, MvwmPicture *p);

/* <pubfunc>PCloneMvwmPicture
 * <description>
 * Duplicate an already allocated MvwmPicture in the MvwmPicture cache
 * (a weight is added to the picture).
 * </description>
 */
MvwmPicture    *PCloneMvwmPicture(MvwmPicture *pic);

void            PicturePrintImageCache(int verbose);

#endif
