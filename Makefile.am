## -*- Makefile -*- mode
## Process this file with automake to create Makefile.in

## Make sure we have a current version of automake
AUTOMAKE_OPTIONS = 1.4

SUBDIRS = \
	libs \
	bin \
	mvwm \
	modules \
	$(MVWM_PERLLIB) \
	## TA:  Ignore things in bin/ for now.
	#doc \
	#$(POSUB)

## ---------------------------------------------------------------------------
## Manage bzip2 archive together with gzip archive
#  Usage:
#    make dist2       # instead of make dist
#    make distcheck2  # instead of make distcheck
#    make distclean2  # instead of make distclean

DISTCHECK_CONFIGURE_FLAGS = --enable-htmldoc

dist2: dist
	gzip -dc $(distdir).tar.gz | bzip2 -9 >$(distdir).tar.bz2

distcheck2: distcheck dist2
	@banner="$(distdir).tar.bz2 - ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$banner"; \
	echo "$$dashes"
	@if test "x$(ISRELEASED)" != xyes; then \
		echo 'Warning: ISRELEASED is not set to "yes".'; \
		echo '	So these can not be the official tarballs.'; \
	fi

distclean2: distclean

test:
	@true
