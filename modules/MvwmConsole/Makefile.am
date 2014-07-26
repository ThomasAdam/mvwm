## Process this file with automake to create Makefile.in

PERL = @PERL@
moduledir = @MVWM_MODULEDIR@

module_PROGRAMS = MvwmConsole MvwmConsoleC
module_SCRIPTS = MvwmConsoleC.pl

MvwmConsole_SOURCES = MvwmConsole.c MvwmConsole.h
MvwmConsoleC_SOURCES = MvwmConsoleC.c MvwmConsole.h getline.c

MvwmConsole_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a
MvwmConsoleC_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

man_MANS = MvwmConsole.1 MvwmConsoleC.pl.1
EXTRA_DIST = MvwmConsoleC.pl.in $(man_MANS) Changes .fvwm2rc.sample

# Use X_EXTRA_LIBS to get socket(), etc.
LDADD = -L$(top_builddir)/libs -lmvwm $(readline_LIBS) $(X_EXTRA_LIBS)

# FIXME:
# Despite not using X functions explicitly, the code includes
# fvwmlib.h, which *does* include X headers and xpm.h!
AM_CPPFLAGS = -I$(top_srcdir) $(readline_CFLAGS) $(xpm_CFLAGS) $(X_CFLAGS)
