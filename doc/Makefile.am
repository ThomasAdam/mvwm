# Author: Scott Smedley

docdir = @MVWM_DOCDIR@

HTML_FILES = \
	index.html allCommands.html groupedCommands.html modules.html style.css
EXTRA_DIST = \
	fvwm.xsl fvwm-man.xsl header.html $(HTML_FILES)

if MVWM_BUILD_HTMLDOC
doc_DATA = \
	$(HTML_FILES)
endif


SUBDIRS = commands modules docbook-xml docbook-xsl fvwm

all: $(doc_DATA)
