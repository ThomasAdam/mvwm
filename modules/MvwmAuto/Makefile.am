## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@
module_PROGRAMS = MvwmAuto
man_MANS = MvwmAuto.1
EXTRA_DIST = $(man_MANS)

MvwmAuto_SOURCES = MvwmAuto.c
MvwmAuto_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

LDADD = -L$(top_builddir)/libs $(X_LIBS) -lmvwm  \
	$(X_PRE_LIBS) -lXext -lX11 $(X_EXTRA_LIBS) $(XRandR_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(X_CFLAGS)

