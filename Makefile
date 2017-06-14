all: car

car: car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp game.h
	g++ car.cpp christianR.cpp daveR.cpp alexN.cpp abdulelahA.cpp -Wall -lX11 -lGL -lGLU -lm ./libggfonts.a -ocar

clean:
	rm -f car
	rm -f *.o
