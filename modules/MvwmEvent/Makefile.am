## Process this file with automake to create Makefile.in

moduleexecdir = @MVWM_MODULEDIR@
moduleexec_PROGRAMS = MvwmEvent
man_MANS = MvwmEvent.1
EXTRA_DIST = $(man_MANS)

MvwmEvent_SOURCES = MvwmEvent.c
MvwmEvent_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

# This module behaves like the old MvwmAudio when invoked with that name.
install-exec-local: ../../config.h Makefile
	$(mkinstalldirs) $(DESTDIR)$(moduleexecdir)
	rm -f $(DESTDIR)$(moduleexecdir)/MvwmAudio
	@_fe=`echo MvwmEvent|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	 _fa=`echo MvwmAudio|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	 echo "  $(LN_S) $$_fe $(DESTDIR)$(moduleexecdir)/$$_fa"; \
	 $(LN_S) "$$_fe" $(DESTDIR)$(moduleexecdir)/"$$_fa" || true

LDADD = -L$(top_builddir)/libs $(X_LIBS) -lmvwm $(rplay_LIBS) \
	$(X_PRE_LIBS) -lXext -lX11 $(X_EXTRA_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) $(rplay_CFLAGS) $(X_CFLAGS)
