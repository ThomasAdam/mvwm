## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@
configdir = @MVWM_DATADIR@

module_PROGRAMS = MvwmButtons
man_MANS = MvwmButtons.1
EXTRA_DIST = $(man_MANS) CHANGES samplebuttonrc BUGS ConfigMvwmButtons

config_DATA = ConfigMvwmButtons

MvwmButtons_SOURCES = \
	MvwmButtons.c MvwmButtons.h button.c button.h draw.c draw.h \
	icons.c icons.h misc.c misc.h output.c parse.c parse.h \
	dynamic.c dynamic.h

MvwmButtons_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

## Xpm note: while this module may not depend on Xpm explicitly,
## there are sometimes dependencies through functions in libmvwm
## so we might as well link against libXpm, if present.
LDADD = -L$(top_builddir)/libs -lmvwm $(Xft_LIBS) $(X_LIBS) $(xpm_LIBS)  \
	$(X_PRE_LIBS) $(XRandR_LIBS) -lXext -lX11 -lm $(X_EXTRA_LIBS) \
	$(iconv_LIBS) $(Xrender_LIBS) $(Xcursor_LIBS) $(Bidi_LIBS) \
	$(png_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(xpm_CFLAGS) $(Xft_CFLAGS) $(X_CFLAGS) \
	$(iconv_CFLAGS) $(Xrender_CFLAGS) $(fribidi_CFLAGS) $(png_CFLAGS)

