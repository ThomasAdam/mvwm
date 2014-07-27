## Process this file with automake to create Makefile.in

bin_PROGRAMS = mvwm-root

mvwm_root_SOURCE= mvwm-root.c
mvwm_root_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

bin_SCRIPTS = \
	mvwm-config mvwm-bug mvwm-perllib \
	mvwm-convert-2.4 mvwm-convert-2.6 \
	mvwm-menu-directory mvwm-menu-desktop

man_MANS = \
	mvwm-root.1 mvwm-config.1 mvwm-bug.1 mvwm-perllib.1 \
	mvwm-convert-2.4.1 mvwm-convert-2.6.1 \
	mvwm-menu-directory.1 mvwm-menu-desktop.1

LDADD = -L$(top_builddir)/libs $(X_LIBS) -lmvwm $(Xcursor_LIBS) \
	$(X_PRE_LIBS) -lXext -lX11 -lm $(X_EXTRA_LIBS) $(Xrender_LIBS) \
	$(Xcursor_LIBS) $(png_LIBS)
AM_CPPFLAGS = -I$(top_srcdir) $(X_CFLAGS) $(png_CFLAGS)

configdir = @MVWM_DATADIR@
config_DATA = mvwm-menu-desktop-config.fpl

EXTRA_DIST = mvwm-convert-2.2 \
mvwm-menu-desktop-config.fpl \
mvwm-convert-2.2.1 \
$(man_MANS)

## The long generation for *.1 is to make both pod2man and 'make -j' happy.

_mvwm-menu-directory.1: mvwm-menu-directory
	@TMP=tmp-$$$$ NAME=mvwm-menu-directory && (\
	mkdir $$TMP && \
	cp $$NAME.in $$TMP/$$NAME && \
	pod2man --center "Mvwm Utilities" --release "@VERSION@@VERSIONINFO@" \
		--name $$NAME $$TMP/$$NAME >$$TMP/$$NAME.1 && \
	mv $$TMP/$$NAME.1 .; \
	rm -rf $$TMP)

_mvwm-perllib.1: mvwm-perllib
	@TMP=tmp-$$$$ NAME=mvwm-perllib && (\
	mkdir $$TMP && \
	cp $$NAME.in $$TMP/$$NAME && \
	pod2man --center "Mvwm Utilities" --release "@VERSION@@VERSIONINFO@" \
		--name $$NAME $$TMP/$$NAME >$$TMP/$$NAME.1 && \
	mv $$TMP/$$NAME.1 .; \
	rm -rf $$TMP)

regenerate: \
	_mvwm-menu-directory.1 _mvwm-perllib.1

CLEANFILES = $(bin_SCRIPTS) $(bin_PROGRAMS)
