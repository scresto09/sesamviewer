## ./glade/Makefile.am

all:
	@echo "Glade templates will be installed with 'make install'"
	@true

install:
	@$(NORMAL_INSTALL) 
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	for glade in $(srcdir)/*.glade; do \
	  if test -f $$glade; then \
	    $(INSTALL_DATA) $$glade $(DESTDIR)$(pkgdatadir)/; \
	  fi \
	done 

