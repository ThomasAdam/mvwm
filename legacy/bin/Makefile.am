## Process this file with automake to create Makefile.in

bin_PROGRAMS = fvwm-root

fvwm_root_SOURCE= fvwm-root.c
fvwm_root_DEPENDENCIES = $(top_builddir)/libs/libfvwm.a

bin_SCRIPTS = \
	fvwm-config fvwm-bug fvwm-perllib \
	fvwm-convert-2.4 fvwm-convert-2.6 \
	fvwm-menu-directory fvwm-menu-desktop

man_MANS = \
	fvwm-root.1 fvwm-config.1 fvwm-bug.1 fvwm-perllib.1 \
	fvwm-convert-2.4.1 fvwm-convert-2.6.1 \
	fvwm-menu-directory.1 fvwm-menu-desktop.1

LDADD = -L$(top_builddir)/libs $(X_LIBS) -lfvwm $(Xcursor_LIBS) \
	$(X_PRE_LIBS) -lXext -lX11 -lm $(X_EXTRA_LIBS) $(Xrender_LIBS) \
	$(Xcursor_LIBS) $(png_LIBS)
AM_CPPFLAGS = -I$(top_srcdir) $(X_CFLAGS) $(png_CFLAGS)

configdir = @FVWM_DATADIR@
config_DATA = fvwm-menu-desktop-config.fpl

EXTRA_DIST = fvwm-convert-2.2 \
fvwm-menu-desktop-config.fpl \
fvwm-convert-2.2.1 \
$(man_MANS)

## The long generation for *.1 is to make both pod2man and 'make -j' happy.

_fvwm-menu-directory.1: fvwm-menu-directory
	@TMP=tmp-$$$$ NAME=fvwm-menu-directory && (\
	mkdir $$TMP && \
	cp $$NAME.in $$TMP/$$NAME && \
	pod2man --center "Fvwm Utilities" --release "@VERSION@@VERSIONINFO@" \
		--name $$NAME $$TMP/$$NAME >$$TMP/$$NAME.1 && \
	mv $$TMP/$$NAME.1 .; \
	rm -rf $$TMP)

_fvwm-perllib.1: fvwm-perllib
	@TMP=tmp-$$$$ NAME=fvwm-perllib && (\
	mkdir $$TMP && \
	cp $$NAME.in $$TMP/$$NAME && \
	pod2man --center "Fvwm Utilities" --release "@VERSION@@VERSIONINFO@" \
		--name $$NAME $$TMP/$$NAME >$$TMP/$$NAME.1 && \
	mv $$TMP/$$NAME.1 .; \
	rm -rf $$TMP)

regenerate: \
	_fvwm-menu-directory.1 _fvwm-perllib.1

CLEANFILES = $(bin_SCRIPTS) $(bin_PROGRAMS)
