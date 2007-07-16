
# $Id: Makefile.am,v 1.3 2007/07/16 15:07:14 scott Exp $

# Author: Scott Smedley

docdir=@FVWM_DOCDIR@/images
EXTRA_DIST=fvwm.logo.png fvwm.logo2.png m.logo.png

if FVWM_BUILD_HTMLDOC
doc_DATA=$(EXTRA_DIST)
endif

SUBDIRS=svg_rendering

all: $(doc_DATA)

%.png: $(srcdir)/$@
	@ cp $(srcdir)/$@ $@
