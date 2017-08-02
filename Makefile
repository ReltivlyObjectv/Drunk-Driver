CFLAGS =     -pthread -Wall -Wextra -Werror
LFLAGS =    -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
LFLAGSMAC = -lX11 -lGLU -lGL -pthread -lm #-lXrandr

#all is used to compile on Linux
all: fonts.o fonttex.o staticLib car cardebug

debug: fonts.o fonttex.o staticLib cardebug

#mac is used to compile on OS X, after "make macsymlink" has been called, if needed
mac: clean fontsmac.o fonttex.o staticLib carmac carmacdebug

cardebug: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	@echo "Compiling game (Linux)"
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp ppm.cpp -Wall -Wextra $(LFLAGS) -o cardebug -D DEBUG

car: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	@echo "Compiling game (Linux)"
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp ppm.cpp -Wall -Wextra $(LFLAGS) -o car

carmac: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	@echo "Compiling game (OS X)"
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp ppm.cpp -Wall -Wextra $(LFLAGSMAC) -o car \
		-stdlib=libc++ -I/usr/X11R6/include -L/usr/X11R6/lib -framework OpenGL -framework Cocoa

carmacdebug: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	@echo "Compiling game (OS X)"
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp ppm.cpp -Wall -Wextra $(LFLAGSMAC) -o cardebug \
		-stdlib=libc++ -I/usr/X11R6/include -L/usr/X11R6/lib -framework OpenGL -framework Cocoa -D DEBUG

macsymlink:
	#This is used to create a symlink to a required library that is likely already on your system
	ln -s /usr/local/Cellar/wxmac/3.0.0.0/lib/libwx_osx_cocoau-3.0.0.0.0.dylib /usr/local/lib/

clean:
	rm -f car
	rm -f *.o
	rm -f *.a
	rm -f images/*.ppm
	rm -f .*.swp

fonts.o: fonts.cpp fonts.h defs.h
	@echo "Compiling fonts (Linux)"
	@g++ -c -Wall -Wextra fonts.cpp

fontsmac.o: fonts.cpp fonts.h defs.h
	@echo "Compiling fonts (OS X)"
	@g++ -c -Wall -Wextra fonts.cpp -I/opt/X11/include

fonttex.o: fonttex.cpp
	@echo "Compiling font: textures (Universal)"
	@g++ -c -Wall -Wextra fonttex.cpp

staticLib: fonts.o fonttex.o
	@echo "Compiling font: staticLib (Universal)"
	@ar -cvq libggfonts.a fonts.o fonttex.o
	@rm libggfonts.a
	@ar -cvq libggfonts.a fonts.o fonttex.o
