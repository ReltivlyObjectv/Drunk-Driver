CFLAGS =     -pthread -Wall -Wextra -Werror
LFLAGS =    -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
LFLAGSMAC = -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: fonts.o fonttex.o staticLib car

mac: fontsmac.o fonttex.o staticLibMac car

car: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	g++ $(CFLAGS) car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp log.cpp fonts.cpp fonttex.cpp  -Wall -Wextra $(LFLAGSMAC) -o asteroids -stdlib=libc++ -I/usr/X11R6/include -L/usr/X11R6/lib -framework OpenGL -framework Cocoa 

clean:
	rm -f car
	rm -f *.o
	rm -f *.a

fonts.o: fonts.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fonts.cpp

fontsmac.o: fontsmac.cpp fonts.h defs.h
	g++ -c -Wall -Wextra fontsmac.cpp -I/opt/X11/include -L/opt/X11/lib -lGL -lX11

fonttex.o: fonttex.cpp
	g++ -c -Wall -Wextra fonttex.cpp

staticLib: fonts.o fonttex.o
	ar -cvq libggfonts.a fonts.o fonttex.o
	rm libggfonts.a
	ar -cvq libggfonts.a fonts.o fonttex.o

staticLibMac: fontsmac.o fonttex.o
	ar -cvq libggfonts.a fontsmac.o fonttex.o
	rm libggfonts.a
	ar -cvq libggfonts.a fontsmac.o fonttex.o

ggtest: ggtest.cpp fonts.o fonttex.o fonts.h log.cpp defs.h
	g++ $(CFLAGS) ggtest.cpp log.cpp fonts.o fonttex.o $(LFLAGS) -o ggtest
