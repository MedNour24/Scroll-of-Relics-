#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialisation du labyrinthe
static void initMaze(Maze* maze) {
    int initialGrid[MAZE_HEIGHT][MAZE_WIDTH] = {
        {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1},
        {1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1},
        {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
        {1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1},
        {1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1},
        {1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1},
        {1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1},
        {1,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1},
        {1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,2},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            maze->grid[y][x] = initialGrid[y][x];
        }
    }
}

// Initialisation du jeu
void initGame(Game* game, SDL_Surface* screen) {
    if (TTF_Init() == -1) {
        fprintf(stderr, "Erreur TTF: %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    game->screen = screen;

    // Initialisation des ressources
    game->resources.quitButton = loadImage("quit.png");
    game->resources.quitButtonHovered = loadImage("quit2.png");
    game->resources.background = loadImage("background.png");
    game->resources.map = loadImage("map.png");
    game->resources.successImage = loadImage("VICTOIRE.png");
    game->resources.failureImage = loadImage("echec.png");

    // Chargement des images d'horloge
    char filename[32];
    for (int i = 0; i < CLOCK_IMAGES; i++) {
        sprintf(filename, "clock/clock %d.png", i);
        game->resources.clockImages[i] = loadImage(filename);
        if (!game->resources.clockImages[i]) {
            fprintf(stderr, "Erreur lors du chargement de %s\n", filename);
            cleanupGame(game);
            exit(1);
        }
    }

    // Initialisation du joueur
    game->player.x = 1;
    game->player.y = 1;
    game->player.image = loadImage("player.png");

    // Vérification des chargements
    if (!game->resources.quitButton || !game->resources.quitButtonHovered || 
        !game->resources.background || !game->resources.map || 
        !game->player.image || !game->resources.successImage || 
        !game->resources.failureImage) {
        fprintf(stderr, "Erreur lors du chargement des ressources\n");
        cleanupGame(game);
        exit(1);
    }

    // Initialisation du labyrinthe
    initMaze(&game->maze);

    game->startTime = SDL_GetTicks();
    game->running = 1;
}

// Nettoyage des ressources
void cleanupGame(Game* game) {
    SDL_FreeSurface(game->resources.quitButton);
    SDL_FreeSurface(game->resources.quitButtonHovered);
    SDL_FreeSurface(game->resources.background);
    SDL_FreeSurface(game->resources.map);
    SDL_FreeSurface(game->player.image);
    SDL_FreeSurface(game->resources.successImage);
    SDL_FreeSurface(game->resources.failureImage);
    for (int i = 0; i < CLOCK_IMAGES; i++) {
        SDL_FreeSurface(game->resources.clockImages[i]);
    }
    // Ne pas libérer game->screen ici, car il est géré par main.c
    TTF_Quit();
}

SDL_Surface* loadImage(const char* file) {
    SDL_Surface* img = IMG_Load(file);
    if (!img) {
        fprintf(stderr, "Erreur de chargement de l'image %s: %s\n", file, IMG_GetError());
    }
    return img;
}

void drawBackground(Game* game) {
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_BlitSurface(game->resources.background, NULL, game->screen, &rect);
}

void drawMap(Game* game) {
    SDL_Rect map_rect;
    map_rect.x = (SCREEN_WIDTH - MAZE_WIDTH * TILE_SIZE) / 2;
    map_rect.y = (SCREEN_HEIGHT - MAZE_HEIGHT * TILE_SIZE) / 2;
    SDL_BlitSurface(game->resources.map, NULL, game->screen, &map_rect);
}

void drawMaze(Game* game) {
    SDL_Rect tile = {0, 0, TILE_SIZE, TILE_SIZE};
    int offsetX = (SCREEN_WIDTH - MAZE_WIDTH * TILE_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - MAZE_HEIGHT * TILE_SIZE) / 2;

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            tile.x = offsetX + x * TILE_SIZE;
            tile.y = offsetY + y * TILE_SIZE;
            if (game->maze.grid[y][x] == 1) {
                SDL_FillRect(game->screen, &tile, SDL_MapRGB(game->screen->format, 126, 160, 159));
            } else if (game->maze.grid[y][x] == 2) {
                SDL_FillRect(game->screen, &tile, SDL_MapRGB(game->screen->format, 255, 0, 0));
            } else {
                SDL_FillRect(game->screen, &tile, SDL_MapRGB(game->screen->format, 222, 229, 229));
            }
        }
    }
}

void drawPlayer(Game* game) {
    SDL_Rect player_rect;
    player_rect.w = TILE_SIZE;
    player_rect.h = TILE_SIZE;
    int offsetX = (SCREEN_WIDTH - MAZE_WIDTH * TILE_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - MAZE_HEIGHT * TILE_SIZE) / 2;
    player_rect.x = offsetX + game->player.x * TILE_SIZE;
    player_rect.y = offsetY + game->player.y * TILE_SIZE;
    SDL_BlitSurface(game->player.image, NULL, game->screen, &player_rect);
}

void drawClock(Game* game) {
    Uint32 elapsed_time = SDL_GetTicks() - game->startTime;
    int time_left = 60000 - elapsed_time; // Temps restant en ms
    if (time_left < 0) time_left = 0;

    // Calculer l'index de l'image (0 à 10) en fonction du temps restant
    int index = (time_left * CLOCK_IMAGES) / 60000; // Proportion du temps restant
    if (index >= CLOCK_IMAGES) index = CLOCK_IMAGES - 1;
    if (index < 0) index = 0;

    // Position de l'horloge
    SDL_Rect clock_rect = {50, 50, 0, 0};
    SDL_BlitSurface(game->resources.clockImages[index], NULL, game->screen, &clock_rect);
}

int isMoveValid(Game* game, int new_x, int new_y) {
    return (new_x >= 0 && new_x < MAZE_WIDTH && 
            new_y >= 0 && new_y < MAZE_HEIGHT && 
            game->maze.grid[new_y][new_x] != 1);
}

void drawResult(Game* game, SDL_Surface* result_img) {
    SDL_Rect quit_pos;
    SDL_Rect result_rect = {(SCREEN_WIDTH - result_img->w) / 2, (SCREEN_HEIGHT - result_img->h) / 2};
    quit_pos.x = result_rect.x + (result_img->w - game->resources.quitButton->w) / 2;
    quit_pos.y = result_rect.y + result_img->h - 120;

    SDL_BlitSurface(result_img, NULL, game->screen, &result_rect);
    SDL_BlitSurface(game->resources.quitButton, NULL, game->screen, &quit_pos);
    SDL_Flip(game->screen);

    SDL_Event event;
    int waiting = 1;
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    waiting = 0;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
                        waiting = 0;
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.x >= quit_pos.x && event.motion.x <= quit_pos.x + game->resources.quitButton->w &&
                        event.motion.y >= quit_pos.y && event.motion.y <= quit_pos.y + game->resources.quitButton->h) {
                        SDL_BlitSurface(result_img, NULL, game->screen, &result_rect);
                        SDL_BlitSurface(game->resources.quitButtonHovered, NULL, game->screen, &quit_pos);
                        SDL_Flip(game->screen);
                    } else {
                        SDL_BlitSurface(result_img, NULL, game->screen, &result_rect);
                        SDL_BlitSurface(game->resources.quitButton, NULL, game->screen, &quit_pos);
                        SDL_Flip(game->screen);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT &&
                        event.button.x >= quit_pos.x && event.button.x <= quit_pos.x + game->resources.quitButton->w &&
                        event.button.y >= quit_pos.y && event.button.y <= quit_pos.y + game->resources.quitButton->h) {
                        waiting = 0;
                    }
                    break;
            }
        }
        SDL_Delay(16);
    }
}
