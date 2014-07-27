# Author: Scott Smedley

docdir = @MVWM_DOCDIR@/mvwm
XSLTPROC = @XSLTPROC@

XSL_FILE = $(srcdir)/../mvwm.xsl
XSL_MAN_FILE = $(srcdir)/../mvwm-man.xsl
XSL_PROFILE = $(srcdir)/../docbook-xsl/profiling/profile.xsl

HTML_FILES = mvwm.man.html
XML_FILES = @DOC_SECTIONS_XML_PATH@
EXTRA_DIST = @DOC_SECTIONS_XML@ $(man_MANS) sections
man_MANS = mvwm.1

if MVWM_BUILD_HTMLDOC
doc_DATA = $(HTML_FILES)
BUILD_HTML_FILES = $(HTML_FILES)
else
BUILD_HTML_FILES =
endif

if MVWM_BUILD_MANDOC
BUILD_MANS = $(man_MANS)
else
BUILD_MANS =
endif

all: $(BUILD_HTML_FILES) $(BUILD_MANS)

clean:
	rm -f $(BUILD_HTML_FILES) $(BUILD_MANS)

distclean-local: clean

#%.html : $(srcdir)/%.xml $(srcdir)/../style.css
#	$(XSLTPROC) --path "$(top_builddir)/doc":"$(top_srcdir)/doc/commands" \
#		--xinclude \
#		--stringparam profile.attribute output \
#		--stringparam profile.value html \
#		-o $(<F).p \
#		$(XSL_PROFILE) $<
#	$(XSLTPROC) --path "$(top_builddir)/doc":"$(top_srcdir)/doc/commands" \
#		--xinclude \
#		--stringparam html.stylesheet "$(srcdir)/../style.css" \
#		-o $@ $(XSL_FILE) $(<F).p && rm $(<F).p

mvwm.man.html: $(XML_FILES) $(XSL_FILE) $(top_builddir)/doc/mvwm.ent @DOC_COMMANDS_XML_PATH@ $(srcdir)/../header.html
	$(XSLTPROC) --path "$(top_builddir)/doc":"$(top_srcdir)/doc/commands" \
		--xinclude \
		--stringparam profile.attribute output \
		--stringparam profile.value html \
		-o mvwm.man.html.tmp \
		$(XSL_PROFILE) "$(srcdir)/mvwm.man.xml"
	$(XSLTPROC) --path "$(top_builddir)/doc":"$(top_srcdir)/doc/commands" \
		--xinclude \
		--stringparam html.stylesheet "../style.css" \
		--stringparam header.file "$(srcdir)/../header.html" \
		--stringparam twdt 1 \
		--stringparam section.autolabel 1 \
		-o $@ $(XSL_FILE) mvwm.man.html.tmp && rm mvwm.man.html.tmp

mvwm.1: $(XML_FILES) $(srcdir)/mvwm.man.xml $(XSL_MAN_FILE) @DOC_COMMANDS_XML_PATH@
	$(XSLTPROC) --path "$(top_builddir)/doc":"$(top_srcdir)/doc/commands" \
		--xinclude \
		--stringparam profile.attribute output \
		--stringparam profile.value man \
		-o mvwm.1.tmp \
		$(XSL_PROFILE) "$(srcdir)/mvwm.man.xml"
	$(XSLTPROC) --path "$(top_builddir)/doc":"$(top_srcdir)/doc/commands" \
		--xinclude \
		$(XSL_MAN_FILE) mvwm.1.tmp && rm mvwm.1.tmp
#	mv mvwm.1 mvwm.1.pre
#	tbl mvwm.1.pre | sed '/^\.T[SE]$$/d' > mvwm.1 && rm mvwm.1.pre

install-data-local:
	@rm -f "$(DESTDIR)$(mandir)/man1/mvwm2.1"
	@$(LN_S) mvwm.1 "$(DESTDIR)$(mandir)/man1/mvwm2.1" || \
	echo "Minor warning: $(mandir)/man1/mvwm2.1 symlink was not created"
