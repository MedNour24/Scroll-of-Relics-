#ifndef ENNEMI_H_
#define ENNEMI_H_

#define SPRITE_ENNEMI_NbL 4
#define SPRITE_ENNEMI_NbCol 3
#define Ennemi_WIDTH 155
#define Ennemi_HEIGHT 145
#define BACKGND_W 2560

typedef enum {WAITING, FOLLOWING, ATTACKING} EnnemiState;

typedef struct {
    SDL_Surface *image, *imagep1, *imagep2, *imagep3, *imagep4, *imagep6, *imagep7, *imagep8, *imagep9, *imagep10,*imagep11,*imagep12,*imagep13,*imagep14,*imagep15,*imagep16,*imagep17,*imagep18,*imagep19;
    SDL_Surface *imageTresor1, *imageTresor2, *imageTresor3, *imageTresor4,*imageTr1,*imageTr2,*imageTr3,*imageTr4;
    SDL_Rect positionAbsolue, positionAnimation[SPRITE_ENNEMI_NbL][SPRITE_ENNEMI_NbCol];
    SDL_Rect positionES, positionES2, positionES3, positionES3_2, positionES3_3, positionES4, positionES5, positionES6, positionES7, positionES8, positionES9,positionES17, 		  positionES10,positionES11,positionES12,positionES12_2,positionES12_3,positionES13,positionES14,positionES14_2,positionES14_3,positionES15,positionES16,positionES18,positionES19;
    int Direction, FrameRow, FrameCol;
    struct {int i, j;} Frame;
    EnnemiState State;
    int health;
    int isAlive;
    int isAttacking;
    int showPotion2, showPotion3, showPotion3_2, showPotion3_3, showPotion6,showPotion12,showPotion12_2,showPotion12_3,showPotion14,showPotion14_2,showPotion14_3,showPotion16,showPotion17,showPotion18,showPotion19;
    int treasureFrame; // Current treasure image (0-3 for tresor1.png to tresor4.png)
    Uint32 treasureAnimStartTime; // Timer for treasure sequence
    Uint32 potionTimer;
    TTF_Font *font;
} Ennemi;

int loadEnnemiImages(Ennemi* A);
void initEnnemiAttributes(Ennemi* E);
int init_ennemi(Ennemi* E);
int loadEnnemi2Images(Ennemi* A); // New function for enemy2 (roman.png)
void initEnnemi2Attributes(Ennemi* E); // New function for enemy2 attributes
int init_ennemi2(Ennemi* E); // New function for enemy2 initialization
int loadES(Ennemi* ES);
int initES(Ennemi* ES);
void display_ennemi(Ennemi E, SDL_Surface* screen);
void display_ennemi2(Ennemi E, SDL_Surface* screen);
void display_ES(Ennemi ES, SDL_Surface* screen, int level);
void display_health(Ennemi E, SDL_Surface* screen);
void move_ennemi(Ennemi* E, int level, SDL_Rect* playerPos);
void animateEnnemi(Ennemi* E);
/**
 * @brief Updates the state of enemy1 (level 1) based on player position and collision.
 * @param E Pointer to the enemy structure.
 * @param playerX Player's x position.
 * @param isColliding Flag indicating if collision occurred.
 */
void updateEnnemiState(Ennemi* E, int playerX, int isColliding);

/**
 * @brief Updates the state of enemy2 (level 2) based on player position and collision.
 * @param E Pointer to the enemy structure.
 * @param playerX Player's x position.
 * @param isColliding Flag indicating if collision occurred.
 */
void updateEnnemi2State(Ennemi* E, int playerX, int isColliding);
void animatePotion3(Ennemi* ES);
void freeEnnemi(Ennemi* E);

#endif
