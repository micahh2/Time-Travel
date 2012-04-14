#Its a make file!!

#default
debug: clean game.o data.o
	@g++ game.o data.o -o game -lSDL -lSDL_image 

#Without debugging
fast: clean game.o data.o
	@g++ game.o data.o -o game -lSDL -lSDL_image -s

#Making game.o 
game.o: game.cpp
	@g++ -Wall -c game.cpp -ggdb

#Making data.o 
data.o: data.cpp data.h
	@g++ -Wall -c data.cpp -ggdb

#Deleting the old game and object files
clean:
	@rm -f *.o game
