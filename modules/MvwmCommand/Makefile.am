## Process this file with automake to create Makefile.in

PERL = @PERL@

SUBDIRS = scripts

bin_PROGRAMS = MvwmCommand
moduledir = @MVWM_MODULEDIR@
module_PROGRAMS = MvwmCommandS
module_DATA = MvwmCommand.sh MvwmCommand.pm

MvwmCommandS_SOURCES = MvwmCommandS.c MvwmCommand.h fifos.c
MvwmCommand_SOURCES = MvwmCommand.c MvwmCommand.h fifos.c

MvwmCommand.sh: findcmd.pl $(top_srcdir)/mvwm/functable.c
	if test -n "$(PERL)" -a -x "$(PERL)"; then \
		$(PERL) $(srcdir)/findcmd.pl $(bindir) -sh \
		< $(top_srcdir)/mvwm/functable.c > $@; \
	else echo '# Did not find perl during mvwm install' > $@; fi

MvwmCommand.pm: findcmd.pl $(top_srcdir)/mvwm/functable.c
	if test -n "$(PERL)" -a -x "$(PERL)"; then \
		$(PERL) $(srcdir)/findcmd.pl $(bindir) \
		< $(top_srcdir)/mvwm/functable.c > $@; \
	else echo '# Did not find perl during mvwm install' > $@; fi

CLEANFILES = MvwmCommand.sh MvwmCommand.pm

MvwmCommandS_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a
MvwmCommand_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

man_MANS = MvwmCommand.1

EXTRA_DIST = $(man_MANS) findcmd.pl

LDADD = -L$(top_builddir)/libs -lmvwm

# FIXME:
# Despite not using X functions explicitly, the code includes
# mvwmlib.h, which *does* include X headers and xpm.h!
AM_CPPFLAGS = -I$(top_srcdir) $(xpm_CFLAGS) $(Xft_CFLAGS) $(X_CFLAGS)

