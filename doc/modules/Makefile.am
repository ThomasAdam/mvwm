# Author: Scott Smedley

docdir = @FVWM_DOCDIR@/modules
XSLTPROC = @XSLTPROC@

XSL_FILE = $(srcdir)/../fvwm.xsl
XSL_MAN_FILE = $(srcdir)/../fvwm-man.xsl
XSL_PROFILE = $(srcdir)/../docbook-xsl/profiling/profile.xsl

MODULES = \
	FvwmAuto FvwmBacker FvwmButtons FvwmCommand \
	FvwmConsole FvwmEvent \
	FvwmIconMan FvwmIdent FvwmPager FvwmPerl \
	FvwmWindowMenu
HTML_FILES = @DOC_MODULES_HTML@ todo.html
XML_FILES = todo.xml
EXTRA_DIST = $(XML_FILES)

if FVWM_BUILD_HTMLDOC
doc_DATA = $(HTML_FILES)
endif

clean:
	rm -f $(HTML_FILES)

distclean-local: clean

# All HTML output is generated from XML files.
# %.html : $(srcdir)/%.xml
$(HTML_FILES): $(srcdir)/todo.xml $(srcdir)/../style.css
	$(XSLTPROC) --path "$(top_builddir)/doc" --xinclude --stringparam \
		html.stylesheet "$(srcdir)/../style.css" -o $@ $(XSL_FILE) $<
