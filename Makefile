# kgalbum Makefile
#
#
# This file is part of kgalbum
#


###### variable declarations #######

# local fallbacks for missing operating system features
SHELL	:= /bin/bash
PREFIX=/usr
export CFLAGS=-I$(PWD)/include
export LDFLAGS=-L$(PWD)/lib
X11_CFLAGS	="-I$(PWD)/include $(shell pkg-config --cflags x11)"
X11_LIBS 	="$(shell pkg-config --libs x11)"
PKG_CONFIG_PATH_OLD	:=$(PKG_CONFIG_PATH)
PKG_CONFIG_PATH	:=$(PWD)/lib/pkgconfig:/usr/X11R76/lib/pkgconfig
PATH_OLD	:=$(PATH)
PATHNEW	:=$(PWD)/bin:$(PATH_OLD)
PKG_CONFIG_PATH_NEW	:=$(PWD)/lib/pkgconfig:$(PKG_CONFIG_PATH_OLD):/usr/X11R76/lib/pkgconfig

KGALUMFILES = $(foreach part, kgalbum, $(wildcard $(addprefix $(part)/,*.[ch])))

all	: kgalbum/kgalbum 


lib/libgphoto2.a	: OpenSource/libgphoto2-2.5.10.tar.bz2
	echo "PREFIX=$(PWD)">OpenSource/config.mak
	echo "KULINA=$(PWD)">>OpenSource/config.mak
	echo "export X11_CFLAGS=\"$(X11_CFLAGS)\"">>OpenSource/config.mak
	echo "export X11_LIBS=\"$(X11_LIBS)\"">>OpenSource/config.mak
	echo "export PKG_CONFIG_PATH=$(PWD)/lib/pkgconfig:/usr/X11R76/lib/pkgconfig">>OpenSource/config.mak
	$(MAKE) -C OpenSource 
	$(MAKE) -C OpenSource install
kgalbum/kgalbum	: lib/libgphoto2.a $(KGALBUMFILES)
	echo "PREFIX=$(PREFIX)">kgalbum/config.mak
	echo "KULINA=$(PWD)">>kgalbum/config.mak
	$(MAKE) -C kgalbum
install	: kgalbum/kgalbum 
	  $(MAKE) -C kgalbum install
		mkdir -p /usr/share/kgalbum
		install -m 644  kgalbum.png /usr/share/kgalbum/kgalbum.png
		install -m 644  kgalbum.desktop /usr/share/applications
clean	:
	 rm -rf lib/* share/* bin/* man/*
	$(MAKE) -C OpenSource clean
	$(MAKE) -C kgalbum clean
