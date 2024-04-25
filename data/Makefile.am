## ./data/Makefile.am

all:
	@echo "files will be installed with 'make install'"
	@true

install:
	@$(NORMAL_INSTALL) 
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	for defxml in $(srcdir)/*.xml; do \
	  if test -f $$defxml; then \
	    $(INSTALL_DATA) $$defxml $(DESTDIR)$(pkgdatadir)/; \
	  fi \
	done ; \
	for defpng in $(srcdir)/*.png; do \
	  if test -f $$defpng; then \
	    $(INSTALL_DATA) $$defpng $(DESTDIR)$(pkgdatadir)/; \
	  fi \
	done 

