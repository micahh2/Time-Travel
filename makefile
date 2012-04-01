

debug: clean game.o data.o
	@g++ game.o data.o -o game -lSDL -ggdb
fast: clean game.o data.o
	@g++ game.o data.o -o game -lSDL

game.o: game.cpp
	@g++ -Wall -c game.cpp 

data.o: data.cpp data.h
	@g++ -Wall -c data.cpp


clean:
	@rm -f *.o game
