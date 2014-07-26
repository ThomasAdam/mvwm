## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@
configdir = @MVWM_DATADIR@

module_PROGRAMS = MvwmBacker
man_MANS = MvwmBacker.1
EXTRA_DIST = $(man_MANS) ConfigMvwmBacker

config_DATA = \
  ConfigMvwmBacker

MvwmBacker_SOURCES = MvwmBacker.c MvwmBacker.h root_bits.c
MvwmBacker_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

LDADD = -L$(top_builddir)/libs $(X_LIBS) -lmvwm \
	$(X_PRE_LIBS) $(XRandR_LIBS) -lXext -lX11 -lm $(X_EXTRA_LIBS) \
	$(Xrender_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(X_CFLAGS) $(Xrender_CFLAGS)

