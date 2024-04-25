## Process this file with automake to produce Makefile.in

AM_CFLAGS = @PACKAGE_CFLAGS@ -Wl,--export-dynamic

INCLUDES = -I$(top_srcdir)/include @PACKAGE_CFLAGS@ \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

bin_PROGRAMS = sesamviewer

sesamviewer_SOURCES = \
		about_window.c sesam.h \
		b2.c sesam.h \
		noemie.c sesam.h \
		config_file.c sesam.h \
		detail_window.c sesam.h \
		error.c sesam.h \
		error_window.c sesam.h \
		goto_window.c sesam.h \
		main.c sesam.h \
		main_window.c sesam.h \
		open_window.c sesam.h \
		properties_window.c sesam.h \
		spawn.c spawn.h \
		support.c sesam.h

sesamviewer_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)
