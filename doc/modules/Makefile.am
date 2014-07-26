# Author: Scott Smedley

docdir = @MVWM_DOCDIR@/modules
XSLTPROC = @XSLTPROC@

XSL_FILE = $(srcdir)/../mvwm.xsl
XSL_MAN_FILE = $(srcdir)/../mvwm-man.xsl
XSL_PROFILE = $(srcdir)/../docbook-xsl/profiling/profile.xsl

MODULES = \
	MvwmAuto MvwmBacker MvwmButtons MvwmCommand \
	MvwmConsole MvwmEvent \
	MvwmIconMan MvwmIdent MvwmPager MvwmPerl \
	MvwmWindowMenu
HTML_FILES = @DOC_MODULES_HTML@ todo.html
XML_FILES = todo.xml
EXTRA_DIST = $(XML_FILES)

if MVWM_BUILD_HTMLDOC
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
