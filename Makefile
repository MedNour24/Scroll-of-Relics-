# Makefile for SDL 1.2 2D platformer game with enigma and maze game integration
prog: main.o personne.o background.o ennemie.o enigme.o game.o
	gcc main.o personne.o background.o ennemie.o enigme.o game.o -o prog `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lm -g

main.o: main.c personne.h background.h ennemie.h enigme.h game.h
	gcc -c main.c -g -Wall `sdl-config --cflags`

personne.o: personne.c personne.h background.h ennemie.h
	gcc -c personne.c -g -Wall `sdl-config --cflags`

background.o: background.c background.h
	gcc -c background.c -g -Wall `sdl-config --cflags`

ennemie.o: ennemie.c ennemie.h
	gcc -c ennemie.c -g -Wall `sdl-config --cflags`

enigme.o: enigme.c enigme.h
	gcc -c enigme.c -g -Wall `sdl-config --cflags`

game.o: game.c game.h
	gcc -c game.c -g -Wall `sdl-config --cflags`

clean:
	rm -f prog *.o
