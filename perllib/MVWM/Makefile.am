## Process this file with automake to create Makefile.in

SUBDIRS = Module Tracker

configdir = @MVWM_PERLLIBDIR@/MVWM

ORIGINAL_PMS = \
	Commands.pm Constants.pm EventNames.pm Event.pm Tracker.pm

config_DATA = $(ORIGINAL_PMS) Module.pm

EXTRA_DIST = $(ORIGINAL_PMS) create-commands create-constants

_Constants.pm: Makefile.am $(top_srcdir)/libs/Module.h
	cd $(top_builddir)/perllib/MVWM && \
	(mvwm_libdir=$(top_builddir)/libs ./create-constants)

_Commands.pm: Makefile $(top_srcdir)/mvwm/functable.c
	cd $(top_builddir)/perllib/MVWM && \
	(mvwm_libdir=$(top_builddir)/libs ./create-commands)

regenerate: _Constants.pm _Commands.pm
