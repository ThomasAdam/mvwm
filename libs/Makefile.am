## Process this file with automake to create Makefile.in

noinst_LIBRARIES = libmvwm.a

## please keep .c files in largest first order to speed up parallel makes
libmvwm_a_SOURCES = \
	BidiJoin.h Bindings.h ClientMsg.h ColorUtils.h Colorset.h \
	CombineChars.h Cursor.h Event.h FBidi.h FEvent.h FGettext.h FImage.h \
	FRender.h FRenderInit.h FRenderInterface.h FScreen.h \
	FShape.h FShm.h FTips.h Fcursor.h Fft.h FftInterface.h Ficonv.h \
	Flocale.h FlocaleCharset.h Fpng.h Grab.h \
	Graphics.h Module.h Parse.h Picture.h PictureBase.h \
	PictureDitherMatrice.h PictureGraphics.h PictureImageLoader.h \
	PictureUtils.h Rectangles.h Strings.h System.h Target.h WinMagic.h \
	XError.h XResource.h charmap.h defaults.h envvar.h fio.h flist.h \
	ftime.h mvwm_sys_stat.h mvwmlib.h mvwmrect.h mvwmsignal.h \
	gravity.c gravity.h lang-strings.h modifiers.h fqueue.h safemalloc.h \
	setpgrp.h strlcpy.h timeout.h vpacket.h wcontext.h wild.h queue.h\
	\
	BidiJoin.c Flocale.c PictureUtils.c FScreen.c Graphics.c \
	PictureGraphics.c Bindings.c FlocaleCharset.c Parse.c \
	PictureImageLoader.c Colorset.c ColorUtils.c CombineChars.c Module.c \
	FRender.c Ficonv.c envvar.c Fft.c  gravity.c \
	XResource.c FEvent.c FImage.c WinMagic.c Target.c Picture.c XError.c \
	fqueue.c mvwmsignal.c System.c PictureBase.c Cursor.c Strings.c \
	mvwmrect.c FRenderInit.c safemalloc.c  FBidi.c \
	wild.c Grab.c Event.c ClientMsg.c setpgrp.c FShape.c \
	FGettext.c Rectangles.c timeout.c flist.c charmap.c wcontext.c \
	modifiers.c FTips.c fio.c mvwmlib.c strlcpy.c

libmvwm_a_LIBADD = @LIBOBJS@ @ALLOCA@

AM_CPPFLAGS = -I$(top_srcdir) $(Xft_CFLAGS) $(X_CFLAGS) \
	$(iconv_CFLAGS) $(Xrender_CFLAGS) $(Bidi_CFLAGS) $(png_CFLAGS) \
	$(rsvg_CFLAGS) $(intl_CFLAGS) $(XRandR_CFLAGS)
