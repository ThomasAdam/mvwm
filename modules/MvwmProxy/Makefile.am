## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@
configdir = @MVWM_DATADIR@

module_PROGRAMS = MvwmProxy
man_MANS = MvwmProxy.1
EXTRA_DIST = $(man_MANS) ConfigMvwmProxyDefaults

config_DATA = \
	ConfigMvwmProxyDefaults

MvwmProxy_SOURCES = MvwmProxy.c MvwmProxy.h
MvwmProxy_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

## Xpm note: while this module may not depend on Xpm explicitly,
## there are sometimes dependencies through functions in libmvwm
## so we might as well link against libXpm, if present.
LDADD = -L$(top_builddir)/libs -lmvwm $(Xft_LIBS) $(X_LIBS) $(xpm_LIBS) \
	$(png_LIBS) $(X_PRE_LIBS) $(XRandR_LIBS) -lXext -lX11 \
	$(X_EXTRA_LIBS) -lm $(Xrender_LIBS) $(Xcursor_LIBS) $(iconv_LIBS) \
	$(Bidi_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(xpm_CFLAGS) $(png_CFLAGS) $(Xft_CFLAGS) \
	$(X_CFLAGS) $(Xrender_CFLAGS) $(iconv_CFLAGS) $(Bidi_CFLAGS)
