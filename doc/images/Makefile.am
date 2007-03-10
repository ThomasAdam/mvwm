
# $Id: Makefile.am,v 1.1 2007/03/10 05:16:02 scott Exp $

docdir=@FVWM_DOCDIR@/images
if FVWM_BUILD_HTMLDOC
doc_DATA=fvwm.logo.png fvwm.logo2.png m.logo.png
EXTRA_DIST=$(doc_DATA)
endif

all:
	@for f in $(doc_DATA) ; do \
		[ ! -f $$f ] && cp -p $(srcdir)/$$f $$f; \
	done ; true

