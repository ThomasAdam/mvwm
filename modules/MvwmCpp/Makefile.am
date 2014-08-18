## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@
module_PROGRAMS = MvwmCpp
man_MANS = MvwmCpp.1
EXTRA_DIST = $(man_MANS)

MvwmCpp_SOURCES = MvwmCpp.c MvwmCpp.h
MvwmCpp_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

## Xpm note: while this module may not depend on Xpm explicitly,
## there are sometimes dependencies through functions in libmvwm
## so we might as well link against libXpm, if present.
LDADD = -L$(top_builddir)/libs $(X_LIBS) -lmvwm -lm  \
	$(X_PRE_LIBS) -lXext -lX11 $(X_EXTRA_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(X_CFLAGS)
AM_CFLAGS = -DMVWM_MODULEDIR=\"$(MVWM_MODULEDIR)\"	\
	-DMVWM_DATADIR=\"$(MVWM_DATADIR)\"		\
	-DMVWM_CPP='"$(MVWM_CPP)"'
