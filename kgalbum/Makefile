include config.mak
PREFIX=/usr
#CC	=g++  -pthread -g -I/usr/local/kulina/include
CC	=cc -pthread $(CFLAGS)
kgalbum	: kgview.o kgviewCallbacks.o main.o rdwrfuns.o checkimport.o cameradia.o makealbum.o gpinterface.o SelectCamera.o SelectCameraCallbacks.o fstree.o
	 $(CC) -o kgalbum kgview.o kgviewCallbacks.o main.o  rdwrfuns.o checkimport.o  cameradia.o   makealbum.o gpinterface.o \
	SelectCamera.o SelectCameraCallbacks.o fstree.o \
	/usr/lib/libkulina.a \
	 /usr/lib/libgm.a -L/usr/X11R6/lib -lX11 -lXext -lm -lpthread  \
	-I$(KULINA)/include -L$(KULINA)/lib \
	$(KULINA)/lib/libgphoto2.a $(KULINA)/lib/libgphoto2_port.a \
	-lltdl -dl
	 cp kgalbum $(KULINA)/TARBALL
#	$(KULINA)/lib/libltdl.a -dl
kgview.o	: kgview.c  images.c
	 $(CC) -c kgview.c
kgviewCallbacks.o	: kgviewCallbacks.c 
	 $(CC) -c kgviewCallbacks.c
main.o	: main.c 
	 $(CC) -c main.c
rdwrfuns.o	: rdwrfuns.c 
	 $(CC) -c rdwrfuns.c
checkimport.o	: checkimport.c 
	 $(CC) -c checkimport.c
cameradia.o	: cameradia.c 
	 $(CC) -c cameradia.c
makealbum.o	: makealbum.c 
	 $(CC) -c makealbum.c
gpinterface.o	: gpinterface.c 
	 $(CC) -c gpinterface.c

clean	:  
	   rm -f *.o kgalbum
	   rm -f $(KULINA)/TARBALL/kgalbum

install	:  kgalbum
	  install -m 755 -s kgalbum /usr/bin
