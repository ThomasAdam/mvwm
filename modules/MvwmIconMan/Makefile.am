## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@
configdir = @MVWM_DATADIR@

module_PROGRAMS = MvwmIconMan
man_MANS = MvwmIconMan.1
EXTRA_DIST = $(man_MANS) ConfigMvwmIconMan

config_DATA = \
  ConfigMvwmIconMan

MvwmIconMan_SOURCES = MvwmIconMan.c MvwmIconMan.h debug.c debug.h \
	functions.c fvwm.c globals.c readconfig.c readconfig.h \
	winlist.c x.c x.h xmanager.c xmanager.h

MvwmIconMan_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

## Xpm note: while this module may not depend on Xpm explicitly,
## there are sometimes dependencies through functions in libfvwm
## so we might as well link against libXpm, if present.
LDADD = -L$(top_builddir)/libs -lmvwm $(Xft_LIBS) $(X_LIBS) \
	$(X_PRE_LIBS) $(XRandR_LIBS) -lXext -lX11 $(X_EXTRA_LIBS) \
	$(iconv_LIBS) $(Xrender_LIBS) -lm $(Bidi_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)  $(Xft_CFLAGS) $(X_CFLAGS) \
	$(iconv_CFLAGS) $(Xrender_CFLAGS) $(Bidi_CFLAGS)

