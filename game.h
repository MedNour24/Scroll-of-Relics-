#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 754
#define TILE_SIZE 20
#define MAZE_WIDTH 25
#define MAZE_HEIGHT 24
#define FPS 60
#define CLOCK_IMAGES 11 // Nombre d'images d'horloge

typedef struct {
    int x;
    int y;
    SDL_Surface* image;
} Player;

typedef struct {
    int grid[MAZE_HEIGHT][MAZE_WIDTH];
} Maze;

typedef struct {
    SDL_Surface* quitButton;
    SDL_Surface* quitButtonHovered;
    SDL_Surface* background;
    SDL_Surface* map;
    SDL_Surface* successImage;
    SDL_Surface* failureImage;
    SDL_Surface* clockImages[CLOCK_IMAGES]; // Tableau pour les images d'horloge
} Resources;

typedef struct {
    SDL_Surface* screen;
    Player player;
    Maze maze;
    Resources resources;
    Uint32 startTime;
    int running;
} Game;

void initGame(Game* game, SDL_Surface* screen);
void cleanupGame(Game* game);
SDL_Surface* loadImage(const char* file);
void drawBackground(Game* game);
void drawMap(Game* game);
void drawMaze(Game* game);
void drawPlayer(Game* game);
void drawClock(Game* game);
int isMoveValid(Game* game, int new_x, int new_y);
void drawResult(Game* game, SDL_Surface* result_img);

#endif
