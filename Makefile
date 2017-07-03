CFLAGS =     -pthread -Wall -Wextra -Werror
LFLAGS =    -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
LFLAGSMAC = -lX11 -lGLU -lGL -pthread -lm #-lXrandr

#all is used to compile on Linux
all: fonts.o fonttex.o staticLib car

#mac is used to compile on OS X, after "make macsymlink" has been called, if needed
mac: fontsmac.o fonttex.o staticLib carmac

car: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp ppm.cpp -Wall -Wextra $(LFLAGS) -o car

carmac: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp ppm.cpp -Wall -Wextra $(LFLAGSMAC) -o car -stdlib=libc++ -I/usr/X11R6/include -L/usr/X11R6/lib -framework OpenGL -framework Cocoa 

macsymlink:
	#This is used to create a symlink to a required library that is likely already on your system
	ln -s /usr/local/Cellar/wxmac/3.0.0.0/lib/libwx_osx_cocoau-3.0.0.0.0.dylib /usr/local/lib/

clean:
	rm -f car
	rm -f *.o
	rm -f *.a

fonts.o: fonts.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fonts.cpp

fontsmac.o: fonts.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fonts.cpp -I/opt/X11/include

fonttex.o: fonttex.cpp
	g++ -c -Wall -Wextra fonttex.cpp

staticLib: fonts.o fonttex.o
	ar -cvq libggfonts.a fonts.o fonttex.o
	rm libggfonts.a
	ar -cvq libggfonts.a fonts.o fonttex.o
