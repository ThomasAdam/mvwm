## Process this file with automake to create Makefile.in

moduledir = @MVWM_MODULEDIR@

module_SCRIPTS = MvwmPerl

EXTRA_DIST = $(man_MANS)
man_MANS = MvwmPerl.1

_MvwmPerl.1: MvwmPerl
	pod2man --center "Mvwm Modules" --release "@VERSION@@VERSIONINFO@" \
		--name MvwmPerl MvwmPerl >MvwmPerl.1

regenerate: _MvwmPerl.1
