SHELL=/bin/sh

SRCDIR=./

CC=g++
CFLAGS=-ggdb -Wall -Wextra -DDATA_PATH="\"$(DATADIR)\""
LIBS=`sdl-config --libs` -lSDL_mixer

default: all

all:
	$(CC) $(CFLAGS) $(DEFINE) $(SRCDIR)game.cpp data.cpp $(LIBS) -o ./game
