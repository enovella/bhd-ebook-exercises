# ***************************************************************************
# Makefile : Build vlc-contrib files
# ***************************************************************************
# Copyright (C) 2003-2006 the VideoLAN team
# $Id$
# 
# Authors: Christophe Massiot <massiot@via.ecp.fr>
#          Derk-Jan Hartman <hartman at videolan dot org>
#          Christophe Mutricy <xtophe at videolan dot org>
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or    
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
# ***************************************************************************

include ./config.mak

BUILDDIRS = bin doc etc include info lib man sbin share vlc-lib tmp gecko-sdk

all:
	$(MAKE) -C src

contrib-macosx.tar.bz2:
	$(WGET) $(CONTRIB_URL)

clean-src:
	rm -rf $(BUILDDIRS)
	$(MAKE) -C src clean-src

clean-bin:
	rm -rf $(BUILDDIRS)
	$(MAKE) -C src clean-dots

clean:
	$(MAKE) clean-src
	rm -rf config.mak distro.mak

package-macosx:
	@if test -d tmp; then \
		echo "Move away ./tmp, it's in the way" ; \
		exit 1 ; \
	fi
	mkdir tmp
	tar cf - bin sbin include lib vlc-lib share/aclocal* share/autoconf* \
		share/automake* share/gettext* \
		| (cd tmp; tar xf -)
	./change_prefix.sh tmp $(PREFIX) @@CONTRIB_PREFIX@@
	(cd tmp; tar cf - .) | bzip2 -c > contrib-macosx.tar.bz2
	rm -rf tmp

DISTDIR = usr/win32

package-win32:
	@if test -d tmp; then \
		echo "Move away ./tmp, it's in the way" ; \
		exit 1 ; \
	fi
	mkdir -p tmp/$(DISTDIR)
	tar cf - --dereference bin sbin include lib share/aclocal*\
		share/autoconf* share/qt4* \
		share/automake* share/gettext* gecko-sdk\
		| (cd tmp/$(DISTDIR); tar xpf -)
#kludge for live.com
	mkdir -p tmp/$(DISTDIR)/live.com
	for i in groupsock liveMedia UsageEnvironment BasicUsageEnvironment; do \
		mkdir -p  tmp/$(DISTDIR)/live.com/$$i/include; \
		cp -r src/live/$$i/include tmp/$(DISTDIR)/live.com/$$i; \
		cp src/live/$$i/lib$${i}.a  tmp/$(DISTDIR)/live.com/$$i; \
	done;
# Change Prefix.
	./change_prefix.sh tmp $(PREFIX) $(DISTDIR)
# Hack for qt4
	(cd tmp/$(DISTDIR)/bin; mv uic.exe uic.ex; mv rcc.exe rcc.ex; mv moc.exe moc.ex)
#	Remove unused and potentially harmful files	
	(cd tmp/$(DISTDIR)/bin; rm -rfv *.exe; chmod a+x *)
# Hack for qt4
	(cd tmp/$(DISTDIR)/bin; mv uic.ex uic.exe; mv rcc.ex rcc.exe; mv moc.ex moc.exe)
# Tar it.
	(cd tmp; tar cf - $(DISTDIR)) | bzip2 -c > contrib-`date +%Y%m%d`-win32-bin-gcc-`$(CC) --version|head -n 1|cut -f 3 -d ' '`-runtime-`/bin/echo -e "#include <_mingw.h>\n__MINGW32_VERSION"|$(CC) -E -|grep -v ^#|grep -v ' '`-only.tar.bz2
	rm -rf tmp

.PHONY: all clean-src clean-bin clean package-macosx package-win32
