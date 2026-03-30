
AUTOMAKE_OPTIONS = subdir-objects
include ../flagsandlibs
include ../sources

qtquickscan_SOURCES = $(COMMONSRC)

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

docfilesdir = $(pkgdatadir)/docs
docfiles_SCRIPTS = ../resources/docs/*

bin_PROGRAMS = qtquickscan

man1_MANS = ../resources/man/qtquickscan.1

install-exec-hook:
	install -D ../resources/pixmaps/QtQuickScan.png $(DESTDIR)${prefix}/share/pixmaps/QtQuickScan.png
	install -D ../resources/pixmaps/QtQuickScan.desktop $(DESTDIR)${prefix}/share/applications/QtQuickScan.desktop

uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/QtQuickScan.desktop
	rm -f $(DESTDIR)${prefix}/share/pixmaps/QtQuickScan.png
	rm -fr $(pkgdatadir)

