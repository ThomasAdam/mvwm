
localedir = @LOCALEDIR@

MVWM_POT_FILES = \
     ../mvwm/mvwm.c \
     ../mvwm/expand.c \
     ../mvwm/windowlist.c \
     ../mvwm/virtual.c \
     ../mvwm/menus.c

MVWMRC_POT_FILES = \
     ../mvwm/ConfigMvwmSetup \
     ../modules/MvwmForm/MvwmForm-Setup.in

MVWMTASKBAR_POT_FILES = \
	../modules/MvwmTaskBar/Goodies.c \
	../modules/MvwmTaskBar/MvwmTaskBar.c

MVWMSCRIPT_POT_FILES = \
	../modules/MvwmScript/Scripts/MvwmScript-BellSetup \
	../modules/MvwmScript/Scripts/MvwmScript-KeyboardSetup \
	../modules/MvwmScript/Scripts/MvwmScript-PointerSetup \
	../modules/MvwmScript/Scripts/MvwmScript-ScreenSetup \
	../modules/MvwmScript/Scripts/MvwmScript-FileBrowser

EXTRA_DIST = \
	@POFILES@ @GMOFILES@ mvwm.pot MvwmTaskBar.pot MvwmScript.pot \
	remove-potcdate.sed remove-potcdate.sin

COPYRIGHT_HOLDER = mvwm workers

# These options get passed to xgettext.
XGETTEXT_OPTIONS = --keyword=_ --keyword=N_

# This is the list of locale categories, beyond LC_MESSAGES, for which the
# message catalogs shall be used.  It is usually empty.
EXTRA_LOCALE_CATEGORIES =

MSGMERGE_UPDATE = @MSGMERGE@ --update

SUFFIXES = .po .gmo .mo .sed .sin .nop .po-update

.po.mo:
	@echo "$(MSGFMT) -c -o $@ $<"; \
	$(MSGFMT) -c -o t-$@ $< && mv t-$@ $@

.po.gmo:
	@lang=`echo $* | sed -e 's,.*/,,'`; \
	test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	if test -f $(srcdir)/$${lang}.po; then \
	  echo "$${cdcmd}rm -f $${lang}.gmo && $(GMSGFMT) -c --statistics -o $${lang}.gmo $${lang}.po"; \
	  cd $(srcdir) && rm -f $${lang}.gmo && $(GMSGFMT) -c --statistics -o t-$${lang}.gmo $${lang}.po && mv t-$${lang}.gmo $${lang}.gmo; \
	fi

.sin.sed:
	sed -e '/^#/d' $< > t-$@
	mv t-$@ $@


all-local: all-local-@USE_NLS@

# make [all] should do nothing ...
#all-local-yes: $(CATALOGS)
all-local-yes:
all-local-no:

catalogs: $(CATALOGS)

maintainer-clean-local:
	rm -f $(GMOFILES)

$(MVWM_DOMAIN).pot-update: $(MVWM_POT_FILES) \
	$(MVWMRC_POT_FILES) \
	remove-potcdate.sed
	rm -f mvwmrc.pot; \
	rcpotfiles='$(MVWMRC_POT_FILES)'; \
	rm -f duplicate; \
	for file in $$rcpotfiles; do \
	  perl -ne 's/\[gt\.((\\.|.)+?)\]/ print \
	  "\#$$ARGV: line $$.\n"."msgid \"$$1\"\n"."msgstr \"\"\n\n"/ge' \
	  $$file >> duplicate; \
	done; \
	msguniq duplicate > mvwmrc.pot;
	rm -f duplicate; \
	potfiles='$(MVWM_POT_FILES)'; \
	$(XGETTEXT) \
	  --add-comments=TRANSLATORS: $(XGETTEXT_OPTIONS) -o $(MVWM_DOMAIN).po \
	  --copyright-holder='$(COPYRIGHT_HOLDER)' $$potfiles; \
	test ! -f $(MVWM_DOMAIN).po || { \
	  if test -f $(srcdir)/$(MVWM_DOMAIN).pot; then \
	    sed -f remove-potcdate.sed < $(srcdir)/$(MVWM_DOMAIN).pot > $(MVWM_DOMAIN).1po && \
	    sed -f remove-potcdate.sed < $(MVWM_DOMAIN).po > $(MVWM_DOMAIN).2po && \
	    if cmp $(MVWM_DOMAIN).1po $(MVWM_DOMAIN).2po >/dev/null 2>&1; then \
	      rm -f $(MVWM_DOMAIN).1po $(MVWM_DOMAIN).2po $(MVWM_DOMAIN).po; \
	    else \
	      rm -f $(MVWM_DOMAIN).1po $(MVWM_DOMAIN).2po $(srcdir)/$(MVWM_DOMAIN).pot && \
	      mv $(MVWM_DOMAIN).po $(srcdir)/$(MVWM_DOMAIN).pot; \
	    fi; \
	  else \
	    mv $(MVWM_DOMAIN).po $(srcdir)/$(MVWM_DOMAIN).pot; \
	  fi; \
	  mv $(MVWM_DOMAIN).pot duplicate.pot; \
	  cat mvwmrc.pot >> duplicate.pot; \
	  msguniq duplicate.pot  > $(MVWM_DOMAIN).pot; \
	  rm -f duplicate.pot mvwmrc.pot; \
	}

$(srcdir)/$(MVWM_DOMAIN).pot:
	$(MAKE) $(MVWM_DOMAIN).pot-update

$(MVWMTASKBAR_DOMAIN).pot-update: $(MVWMTASKBAR_POT_FILES) \
	remove-potcdate.sed
	potfiles='$(MVWMTASKBAR_POT_FILES)'; \
	$(XGETTEXT) \
	  --add-comments=TRANSLATORS: $(XGETTEXT_OPTIONS) \
	  -o $(MVWMTASKBAR_DOMAIN).po \
	  --copyright-holder='$(COPYRIGHT_HOLDER)' $$potfiles; \
	test ! -f $(MVWMTASKBAR_DOMAIN).po || { \
	  if test -f $(srcdir)/$(MVWMTASKBAR_DOMAIN).pot; then \
	    sed -f remove-potcdate.sed < $(srcdir)/$(MVWMTASKBAR_DOMAIN).pot \
	    > $(MVWMTASKBAR_DOMAIN).1po && \
	    sed -f remove-potcdate.sed < $(MVWMTASKBAR_DOMAIN).po \
	    > $(MVWMTASKBAR_DOMAIN).2po && \
	    if cmp $(MVWMTASKBAR_DOMAIN).1po $(MVWMTASKBAR_DOMAIN).2po \
	    >/dev/null 2>&1; then \
	      rm -f $(MVWMTASKBAR_DOMAIN).1po $(MVWMTASKBAR_DOMAIN).2po \
	      $(MVWMTASKBAR_DOMAIN).po; \
	    else \
	      rm -f $(MVWMTASKBAR_DOMAIN).1po $(MVWMTASKBAR_DOMAIN).2po \
	      $(srcdir)/$(MVWMTASKBAR_DOMAIN).pot && \
	      mv $(MVWMTASKBAR_DOMAIN).po $(srcdir)/$(MVWMTASKBAR_DOMAIN).pot; \
	    fi; \
	  else \
	    mv $(MVWMTASKBAR_DOMAIN).po $(srcdir)/$(MVWMTASKBAR_DOMAIN).pot; \
	  fi; \
	}

$(srcdir)/$(MVWMTASKBAR_DOMAIN).pot:
	$(MAKE) $(MVWMTASKBAR_DOMAIN).pot-update

$(MVWMSCRIPT_DOMAIN).pot-update: $(MVWMSCRIPT_POT_FILES) \
	remove-potcdate.sed
	potfiles='$(MVWMSCRIPT_POT_FILES)'; \
	rm -f duplicate; \
	for file in $$potfiles; do \
	  perl -ne 's/LocaleTitle\s+\{((\\.|.)+?)\}/ print \
	  "\#$$ARGV: line $$.\n"."msgid \"$$1\"\n"."msgstr \"\"\n\n"/ge' \
	  $$file >> duplicate; \
	done; \
	msguniq duplicate > duplicate.pot; \
	cat pot_head_sample >  $(MVWMSCRIPT_DOMAIN).pot; \
	cat duplicate.pot >> $(MVWMSCRIPT_DOMAIN).pot; \
	rm -f duplicate duplicate.pot;

$(srcdir)/$(MVWMSCRIPT_DOMAIN).pot:
	$(MAKE) $(MVWMSCRIPT_DOMAIN).pot-update

$(POFILES): $(srcdir)/$(MVWM_DOMAIN).pot $(srcdir)/$(MVWMTASKBAR_DOMAIN).pot \
	$(srcdir)/$(MVWMSCRIPT_DOMAIN).pot
	dlang=`echo $@ | sed -e 's,.*/,,' -e 's/\.po$$//'`; \
	dom=`echo $$dlang | sed -e 's/\..*$$//'`; \
	test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	if test -f $${dlang}.po; then \
	  echo "$${cdcmd}$(MSGMERGE_UPDATE) $${dlang}.po $${dom}.pot"; \
	  cd $(srcdir) && $(MSGMERGE_UPDATE) $${dlang}.po $${dom}.pot; \
	fi

#	@lang=`echo $@ | sed -e 's,.*/,,' -e 's/\.po$$//'`; \
#	test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
#	echo "$${cdcmd}$(MSGMERGE_UPDATE) $${lang}.po $(MVWM_DOMAIN).pot"; \
#	cd $(srcdir) && $(MSGMERGE_UPDATE) $${lang}.po $(MVWM_DOMAIN).pot

install-data-local: install-data-local-@USE_NLS@
install-data-local-no: all
install-data-local-yes: all
	$(mkinstalldirs) $(DESTDIR)$(datadir)
	@catalogs='$(CATALOGS)'; \
	all_domains='$(ALL_DOMAINS)'; \
	for dom in $$all_domains; do \
	  for cat in $$catalogs; do \
	    cat=`basename $$cat`; \
	    lang=`echo $$cat | sed -e 's/\.gmo$$//'`; \
	    cat="$$dom.$$cat"; \
	    dir=$(localedir)/$$lang/LC_MESSAGES; \
	    $(mkinstalldirs) $(DESTDIR)$$dir; \
	    if test -r $$cat; \
	      then realcat=$$cat; \
	    else realcat=$(srcdir)/$$cat; \
	    fi; \
	    if test -f $$realcat; then \
	      $(INSTALL_DATA) $$realcat $(DESTDIR)$$dir/$$dom.mo; \
	      echo "installing $$realcat as $(DESTDIR)$$dir/$$dom.mo"; \
	      for lc in '' $(EXTRA_LOCALE_CATEGORIES); do \
	        if test -n "$$lc"; then \
	          if (cd $(DESTDIR)$(localedir)/$$lang && \
	              LC_ALL=C ls -l -d $$lc 2>/dev/null) | grep ' -> ' \
                      >/dev/null; then \
	            link=`cd $(DESTDIR)$(localedir)/$$lang && \
                         LC_ALL=C ls -l -d $$lc | sed -e 's/^.* -> //'`; \
	            mv $(DESTDIR)$(localedir)/$$lang/$$lc \
                       $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	            mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	            (cd $(DESTDIR)$(localedir)/$$lang/$$lc.old && \
	             for file in *; do \
	               if test -f $$file; then \
	                 ln -s ../$$link/$$file \
                               $(DESTDIR)$(localedir)/$$lang/$$lc/$$file; \
	               fi; \
	             done); \
	            rm -f $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	          else \
	            if test -d $(DESTDIR)$(localedir)/$$lang/$$lc; then \
	              :; \
	            else \
	              rm -f $(DESTDIR)$(localedir)/$$lang/$$lc; \
	              mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	            fi; \
	          fi; \
	          rm -f $(DESTDIR)$(localedir)/$$lang/$$lc/$$dom.mo; \
	          ln -s ../LC_MESSAGES/$$dom.mo \
	             $(DESTDIR)$(localedir)/$$lang/$$lc/$$dom.mo 2>/dev/null || \
	          ln $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$$dom.mo \
	             $(DESTDIR)$(localedir)/$$lang/$$lc/$$dom.mo 2>/dev/null || \
	          cp -p $(DESTDIR)$(localedir)/$$lang/LC_MESSAGES/$$dom.mo \
	             $(DESTDIR)$(localedir)/$$lang/$$lc/$$dom.mo; \
	          echo "installing $$realcat link as \
	          $(DESTDIR)$(localedir)/$$lang/$$lc/$$dom.mo"; \
	        fi; \
	      done; \
	    fi; \
	  done; \
	done


installdirs-data: installdirs-data-local-@USE_NLS@
installdirs-data-local-no:
installdirs-data-local-yes:
	$(mkinstalldirs) $(DESTDIR)$(datadir)
	@catalogs='$(CATALOGS)'; \
	all_domains='$(ALL_DOMAINS)'; \
	for dom in $$all_domainss; do \
	  for cat in $$catalogs; do \
	    cat=`basename $$cat`; \
	    lang=`echo $$cat | sed -e 's/\.gmo$$//'`; \
	    dir=$(localedir)/$$lang/LC_MESSAGES; \
	    $(mkinstalldirs) $(DESTDIR)$$dir; \
	    for lc in '' $(EXTRA_LOCALE_CATEGORIES); do \
	      if test -n "$$lc"; then \
	        if (cd $(DESTDIR)$(localedir)/$$lang && \
                    LC_ALL=C ls -l -d $$lc 2>/dev/null) | grep ' -> ' \
                    >/dev/null; then \
	          link=`cd $(DESTDIR)$(localedir)/$$lang && \
                        LC_ALL=C ls -l -d $$lc | sed -e 's/^.* -> //'`; \
	          mv $(DESTDIR)$(localedir)/$$lang/$$lc \
                     $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	          mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	          (cd $(DESTDIR)$(localedir)/$$lang/$$lc.old && \
	           for file in *; do \
	             if test -f $$file; then \
	               ln -s ../$$link/$$file \
                             $(DESTDIR)$(localedir)/$$lang/$$lc/$$file; \
	             fi; \
	           done); \
	          rm -f $(DESTDIR)$(localedir)/$$lang/$$lc.old; \
	        else \
	          if test -d $(DESTDIR)$(localedir)/$$lang/$$lc; then \
	            :; \
	          else \
	            rm -f $(DESTDIR)$(localedir)/$$lang/$$lc; \
	            mkdir $(DESTDIR)$(localedir)/$$lang/$$lc; \
	          fi; \
	        fi; \
	      fi; \
	    done; \
	  done; \
	done


uninstall-local: uninstall-local-@USE_NLS@
uninstall-local-no:
uninstall-local-yes:
	catalogs='$(CATALOGS)'; \
	all_domains='$(ALL_DOMAINS)'; \
	for dom in $$all_domains; do \
	  for cat in $$catalogs; do \
	    cat=`basename $$cat`; \
	    lang=`echo $$cat | sed -e 's/\.gmo$$//'`; \
	    for lc in LC_MESSAGES $(EXTRA_LOCALE_CATEGORIES); do \
	      rm -f $(DESTDIR)$(localedir)/$$lang/$$lc/$$dom.mo; \
	    done; \
	  done; \
	done

update-po: Makefile
	$(MAKE) $(MVWM_DOMAIN).pot-update
	$(MAKE) $(MVWMTASKBAR_DOMAIN).pot-update
	$(MAKE) $(MVWMSCRIPT_DOMAIN).pot-update
	$(MAKE) $(UPDATEPOFILES)
	$(MAKE) update-gmo

# General rule for updating PO files.

.nop.po-update:
	dlang=`echo $@ | sed -e 's,.*/,,' -e 's/\.po-update$$//'`; \
	dom=`echo $$dlang | sed -e 's/\..*$$//'`; \
	tmpdir=`pwd`; \
	echo "$$dlang:"; \
	test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	echo "$${cdcmd}$(MSGMERGE) $$dlang.po $$dom.pot -o $$dlang.new.po"; \
	cd $(srcdir); \
	if $(MSGMERGE) $$dlang.po $$dom.pot -o $$tmpdir/$$dlang.new.po; then \
	  if cmp $$dlang.po $$tmpdir/$$dlang.new.po >/dev/null 2>&1; then \
	    rm -f $$tmpdir/$$dlang.new.po; \
	  else \
	    if mv -f $$tmpdir/$$dlang.new.po $$dlang.po; then \
	      :; \
	    else \
	      echo "msgmerge for $$dlang.po failed: cannot move \
$$tmpdir/$$dlang.new.po to $$dlang.po" 1>&2; \
	      exit 1; \
	    fi; \
	  fi; \
	else \
	  echo "msgmerge for $$dlang.po failed!" 1>&2; \
	  rm -f $$tmpdir/$$dlang.new.po; \
	fi

$(DUMMYPOFILES):

update-gmo: Makefile $(GMOFILES)
	@:
