/**
 * @file ennemie.c
 * @brief Implementation of enemy-related functions.
 * @author Houssein
 * @version 1.0
 * @date May 05, 2025
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "ennemie.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define SPEED 5
#define POTION3_AMPLITUDE 20.0f
#define POTION3_FREQUENCY 0.005f

#define POTION12_AMPLITUDE 20.0f
#define POTION12_FREQUENCY 0.005f

/**
 * @brief Loads enemy images.
 * @param A Pointer to the enemy structure.
 * @return 0 on success, -1 on failure.
 */
int loadEnnemiImages(Ennemi* A) {
    if (A == NULL) {
        printf("Error: Null enemy pointer in loadEnnemiImages\n");
        return -1;
    }
    A->image = IMG_Load("egg.png");
    if (A->image == NULL) {
        printf("Unable to load enemy png (egg.png): %s\n", SDL_GetError());
        return -1;
    }
    printf("Loaded enemy image: egg.png\n");
    return 0;
}
int loadEnnemi2Images(Ennemi* A) {
    if (A == NULL) {
        printf("Error: Null enemy pointer in loadEnnemi2Images\n");
        return -1;
    }
    A->image = IMG_Load("roman.png");
    if (A->image == NULL) {
        printf("Unable to load enemy png (roman.png): %s\n", SDL_GetError());
        return -1;
    }
    printf("Loaded enemy image: roman.png\n");
    return 0;
}

/**
 * @brief Initializes enemy attributes.
 * @param E Pointer to the enemy structure.
 */
void initEnnemiAttributes(Ennemi* E) {
    if (E == NULL) {
        printf("Error: Null enemy pointer in initEnnemiAttributes\n");
        return;
    }
    int i, j;

    E->positionAbsolue.x = 1500;
    E->positionAbsolue.y = 415;
    E->positionAbsolue.w = Ennemi_WIDTH;
    E->positionAbsolue.h = Ennemi_HEIGHT;

    E->Frame.i = 0;
    E->Frame.j = 0;

    for (i = 0; i < SPRITE_ENNEMI_NbL; i++) {
        for (j = 0; j < SPRITE_ENNEMI_NbCol; j++) {
            E->positionAnimation[i][j].x = j * 170;
            E->positionAnimation[i][j].y = i * 150;
            E->positionAnimation[i][j].w = 140;
            E->positionAnimation[i][j].h = 145;
        }
    }

    E->Direction = 2;
    E->State = WAITING;
    E->health = 100;
    E->isAlive = 1;
    E->isAttacking = 0;
    E->showPotion2 = 0;
    E->showPotion3 = 1;
    E->showPotion3_2 = 1;
    E->showPotion3_3 = 1;
    E->showPotion6 = 0;
    E->treasureFrame = 0;
    E->treasureAnimStartTime = 0;
    E->potionTimer = 0;
    printf("Initialized enemy attributes: x=%d, y=%d\n", E->positionAbsolue.x, E->positionAbsolue.y);
}
/**
 * @brief Initializes attributes for enemy2 (roman.png).
 * @param E Pointer to the enemy structure.
 */
void initEnnemi2Attributes(Ennemi* E) {
    if (E == NULL) {
        printf("Error: Null enemy pointer in initEnnemi2Attributes\n");
        return;
    }
    int i, j;

    E->positionAbsolue.x = 2000;
    E->positionAbsolue.y = 440;
    E->positionAbsolue.w = Ennemi_WIDTH;
    E->positionAbsolue.h = Ennemi_HEIGHT;

    E->Frame.i = 0;
    E->Frame.j = 0;

    for (i = 0; i < SPRITE_ENNEMI_NbL; i++) {
        for (j = 0; j < SPRITE_ENNEMI_NbCol; j++) {
            E->positionAnimation[i][j].x = j * 160;
            E->positionAnimation[i][j].y = i * 150;
            E->positionAnimation[i][j].w = 150;
            E->positionAnimation[i][j].h = 130;
        }
    }

    E->Direction = 2;
    E->State = WAITING;
    E->health = 100;
    E->isAlive = 1;
    E->isAttacking = 0;

    printf("Initialized enemy2 attributes: x=%d, y=%d\n", E->positionAbsolue.x, E->positionAbsolue.y);
}

/**
 * @brief Initializes an enemy with images and attributes.
 * @param E Pointer to the enemy structure.
 * @return 0 on success, -1 on failure.
 */
int init_ennemi(Ennemi* E) {
    if (E == NULL) {
        printf("Error: Null enemy pointer in init_ennemi\n");
        return -1;
    }
    int OK = loadEnnemiImages(E);
    if (OK != -1) {
        initEnnemiAttributes(E);
    }
    return OK;
}
/**
 * @brief Initializes enemy2 with images and attributes (roman.png).
 * @param E Pointer to the enemy structure.
 * @return 0 on success, -1 on failure.
 */
int init_ennemi2(Ennemi* E) {
    if (E == NULL) {
        printf("Error: Null enemy pointer in init_ennemi2\n");
        return -1;
    }
    int OK = loadEnnemi2Images(E);
    if (OK != -1) {
        initEnnemi2Attributes(E);
    }
    return OK;
}

/**
 * @brief Loads potion and treasure images.
 * @param ES Pointer to the enemy structure containing potion data.
 * @return 0 on success, -1 on failure.
 */
int loadES(Ennemi* ES) {
    if (ES == NULL) {
        printf("Error: Null enemy pointer in loadES\n");
        return -1;
    }
    ES->imagep1 = IMG_Load("pos1.png");
    if (ES->imagep1 == NULL) {
        printf("Unable to load potion0.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: potion0.png\n");
    }
    ES->imagep2 = IMG_Load("pos.png");
    if (ES->imagep2 == NULL) {
        printf("Unable to load potion11.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: potion11.png\n");
    }
    ES->imagep3 = IMG_Load("potion3.png");
    if (ES->imagep3 == NULL) {
        printf("Unable to load potion3.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: potion3.png\n");
    }
    
    ES->imageTresor1 = IMG_Load("tresor1.png");
    if (ES->imageTresor1 == NULL) {
        printf("Unable to load tresor1.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded treasure image: tresor1.png\n");
    }
    ES->imageTresor2 = IMG_Load("tresor2.png");
    if (ES->imageTresor2 == NULL) {
        printf("Unable to load tresor2.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded treasure image: tresor2.png\n");
    }
    ES->imageTresor3 = IMG_Load("tresor3.png");
    if (ES->imageTresor3 == NULL) {
        printf("Unable to load tresor3.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded treasure image: tresor3.png\n");
    }
    ES->imageTresor4 = IMG_Load("tresor4.png");
    if (ES->imageTresor4 == NULL) {
        printf("Unable to load tresor4.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded treasure image: tresor4.png\n");
    }
    
    ES->imagep6 = IMG_Load("poti.png");
    if (ES->imagep6 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep7 = IMG_Load("deco.png");
    if (ES->imagep7 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep8 = IMG_Load("head.png");
    if (ES->imagep8 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep9 = IMG_Load("mommy.png");
    if (ES->imagep9 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
     ES->imagep10 = IMG_Load("relic_s.png");
    if (ES->imagep9 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
     ES->imagep11 = IMG_Load("closet.png");
    if (ES->imagep11 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep12 = IMG_Load("ptr.png");
    if (ES->imagep12 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
        ES->imagep13 = IMG_Load("chair.png");
    if (ES->imagep13 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep14 = IMG_Load("ptrg.png");
    if (ES->imagep14 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep15 = IMG_Load("vase.png");
    if (ES->imagep2 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep16 = IMG_Load("relic2_s.png");
    if (ES->imagep16 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep17 = IMG_Load("relic2_s.png");
    if (ES->imagep17 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep18 = IMG_Load("win.png");
    if (ES->imagep18 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    ES->imagep19 = IMG_Load("lose.png");
    if (ES->imagep19 == NULL) {
        printf("Unable to load poti.png: %s\n", SDL_GetError());
    } else {
        printf("Loaded potion image: poti.png\n");
    }
    return 0;
}

/**
 * @brief Initializes potion and treasure entities.
 * @param ES Pointer to the enemy structure containing potion data.
 * @return 0 on success, -1 on failure.
 */
int initES(Ennemi* ES) {
    int lo = loadES(ES);
    if (lo != -1) {
        ES->positionES.x = 2000;
        ES->positionES.y = 150;
        ES->positionES.w = 100;
        ES->positionES.h = 100;

        ES->positionES2.x = 960;
        ES->positionES2.y = 200;
        ES->positionES2.w = 100;
        ES->positionES2.h = 100;

        ES->positionES3.x = 20;
        ES->positionES3.y = 100;
        ES->positionES3.w = 100;
        ES->positionES3.h = 100;

        ES->positionES3_2.x = 40;
        ES->positionES3_2.y = 130;
        ES->positionES3_2.w = 100;
        ES->positionES3_2.h = 100;

        ES->positionES3_3.x = 190;
        ES->positionES3_3.y = 100;
        ES->positionES3_3.w = 100;
        ES->positionES3_3.h = 100;

        ES->positionES4.x = 400;
        ES->positionES4.y = 682 - 100;
        ES->positionES4.w = 100;
        ES->positionES4.h = 100;

        ES->positionES5.x = 2200;
        ES->positionES5.y = 370;
        ES->positionES5.w = 100;
        ES->positionES5.h = 100;

        ES->positionES6.x = 110;
        ES->positionES6.y = 100;
        ES->positionES6.w = 100;
        ES->positionES6.h = 100;

        ES->positionES7.x = 1200;
        ES->positionES7.y = 187;
        ES->positionES7.w = 100;
        ES->positionES7.h = 100;
        
        ES->positionES8.x = 600;
        ES->positionES8.y = 100;
        ES->positionES8.w = 100;
        ES->positionES8.h = 100;
        
        ES->positionES9.x = 30;
        ES->positionES9.y = 140;
        ES->positionES9.w = 100;
        ES->positionES9.h = 100;
        
        ES->positionES10.x = 50;
        ES->positionES10.y = 140;
        ES->positionES10.w = 100;
        ES->positionES10.h = 100;
        
        ES->positionES11.x = 70;
        ES->positionES11.y = 502;
        ES->positionES11.w = 100;
        ES->positionES11.h = 100;
        
        ES->positionES12.x = 950;
        ES->positionES12.y = 350;
        ES->positionES12.w = 100;
        ES->positionES12.h = 100;
        
        ES->positionES12_2.x = 900;
        ES->positionES12_2.y = 350;
        ES->positionES12_2.w = 100;
        ES->positionES12_2.h = 100;
        
        ES->positionES12_3.x = 850;
        ES->positionES12_3.y = 350;
        ES->positionES12_3.w = 100;
        ES->positionES12_3.h = 100;
        
        ES->positionES13.x = 2230;
        ES->positionES13.y = 247;
        ES->positionES13.w = 100;
        ES->positionES13.h = 100;
        
        ES->positionES14.x = 2500;
        ES->positionES14.y = 600;
        ES->positionES14.w = 100;
        ES->positionES14.h = 100;
        
        ES->positionES14_2.x = 2100;
        ES->positionES14_2.y = 300;
        ES->positionES14_2.w = 100;
        ES->positionES14_2.h = 100;
        
        ES->positionES14_3.x = 2300;
        ES->positionES14_3.y = 100;
        ES->positionES14_3.w = 100;
        ES->positionES14_3.h = 100;
        
        ES->positionES15.x = 1100;
        ES->positionES15.y = 570;
        ES->positionES15.w = 100;
        ES->positionES15.h = 100;
        
        ES->positionES16.x = 2320;
        ES->positionES16.y = 246;
        ES->positionES16.w = 100;
        ES->positionES16.h = 100;
        
        ES->positionES17.x = 350;
        ES->positionES17.y = 20;
        ES->positionES17.w = 100;
        ES->positionES17.h = 100;
        
        ES->positionES18.x = 250;
        ES->positionES18.y = 20;
        ES->positionES18.w = 100;
        ES->positionES18.h = 100;
        
        ES->positionES19.x = 250;
        ES->positionES19.y = 20;
        ES->positionES19.w = 100;
        ES->positionES19.h = 100;
        
        ES->showPotion2 = 0;
        ES->showPotion3 = 1;
        ES->showPotion3_2 = 1;
        ES->showPotion3_3 = 1;
        ES->showPotion6 = 1;
        ES->showPotion12 = 1;
        ES->showPotion12_2 = 1;
        ES->showPotion12_3 = 1;
        
        ES->showPotion14 = 1;
        ES->showPotion14_2 = 1;
        ES->showPotion14_3 = 1; 
         
        ES->showPotion16 = 1; // Initialize imagep16 as visible
        ES->showPotion17 = 0; // Initialize imagep17 as hidden
        ES->showPotion18 = 0; //
        ES->showPotion19 = 0; 
        
        
        
        ES->treasureFrame = 0;
        ES->treasureAnimStartTime = 0;
        ES->potionTimer = SDL_GetTicks();
        printf("Initialized potion and treasure positions: ES.y=%d, ES3.y=%d, ES6.y=%d, ES12.y=%d\n", 
               ES->positionES.y, ES->positionES3.y, ES->positionES6.y, ES->positionES12.y);
    }
    return lo;
}
/**
 * @brief Displays the first enemy on the screen.
 * @param E Enemy structure.
 * @param screen Screen surface to render to.
 */
void display_ennemi(Ennemi E, SDL_Surface* screen) {
    if (E.image != NULL && screen != NULL && E.isAlive) {
        printf("Displaying enemy1 frame: row=%d, col=%d at position x=%d, y=%d\n",
               E.Frame.i, E.Frame.j, E.positionAbsolue.x, E.positionAbsolue.y);
        SDL_BlitSurface(E.image, &E.positionAnimation[E.Frame.i][E.Frame.j], screen, &E.positionAbsolue);
    } else {
        printf("Failed to display enemy1: image=%p, screen=%p, isAlive=%d\n",
               E.image, screen, E.isAlive);
    }
}

/**
 * @brief Displays the second enemy on the screen.
 * @param E Enemy structure.
 * @param screen Screen surface to render to.
 */
void display_ennemi2(Ennemi E, SDL_Surface* screen) {
    if (E.image != NULL && screen != NULL && E.isAlive) {
        printf("Displaying enemy2 frame: row=%d, col=%d at position x=%d, y=%d\n",
               E.Frame.i, E.Frame.j, E.positionAbsolue.x, E.positionAbsolue.y);
        SDL_BlitSurface(E.image, &E.positionAnimation[E.Frame.i][E.Frame.j], screen, &E.positionAbsolue);
    } else {
        printf("Failed to display enemy2: image=%p, screen=%p, isAlive=%d\n",
               E.image, screen, E.isAlive);
    }
}

/**
 * @brief Animates potion3 instances vertically.
 * @param ES Pointer to the enemy structure containing potion data.
 */
void animatePotion3(Ennemi* ES) {
    if (ES == NULL) return;
    Uint32 t_now = SDL_GetTicks();
    
    // Animate ES3 (potion3.png) for level 1
    if (ES->showPotion3) {
        const int BASE_Y = 100;
        ES->positionES3.y = BASE_Y + POTION3_AMPLITUDE * sin(POTION3_FREQUENCY * t_now);
        printf("Animating potion3 at x=%d, y=%d\n", ES->positionES3.x, ES->positionES3.y);
    }

    // Animate ES3_2 (potion3.png) for level 1
    if (ES->showPotion3_2) {
        const int BASE_Y = 130;
        ES->positionES3_2.y = BASE_Y + POTION3_AMPLITUDE * sin(POTION3_FREQUENCY * t_now);
        printf("Animating potion3_2 at x=%d, y=%d\n", ES->positionES3_2.x, ES->positionES3_2.y);
    }
    
    // Animate ES3_3 (potion3.png) for level 1
    if (ES->showPotion3_3) {
        const int BASE_Y = 100;
        ES->positionES3_3.y = BASE_Y + POTION3_AMPLITUDE * sin(POTION3_FREQUENCY * t_now);
        printf("Animating potion3_3 at x=%d, y=%d\n", ES->positionES3_3.x, ES->positionES3_3.y);
    }
    
    // Animate ES6 (poti.png) for level 1
    if (ES->showPotion6) {
        const int BASE_Y = 100;
        ES->positionES6.y = BASE_Y + POTION3_AMPLITUDE * sin(POTION3_FREQUENCY * t_now);
        printf("Animating potion6 at x=%d, y=%d\n", ES->positionES6.x, ES->positionES6.y);
    }
    
    // Animate ES (potion0.png) for level 1
    if (1) { // ES is always animated
        const int BASE_Y = 150;
        ES->positionES.y = BASE_Y + POTION3_AMPLITUDE * sin(POTION3_FREQUENCY * t_now);
        printf("Animating potion0 at x=%d, y=%d\n", ES->positionES.x, ES->positionES.y);
    }
    
    // Animate ES12 (ptr.png) for level 2
    if (ES->showPotion12) {
        const int BASE_Y = 300;
        ES->positionES12.y = BASE_Y + POTION12_AMPLITUDE * sin(POTION12_FREQUENCY * t_now);
        printf("Animating potion12 at x=%d, y=%d\n", ES->positionES12.x, ES->positionES12.y);
    }
    
    // Animate ES12_2 (ptr.png) for level 2
    if (ES->showPotion12_2) {
        const int BASE_Y = 300;
        ES->positionES12_2.y = BASE_Y + POTION12_AMPLITUDE * sin(POTION12_FREQUENCY * t_now);
        printf("Animating potion12_2 at x=%d, y=%d\n", ES->positionES12_2.x, ES->positionES12_2.y);
    }
    
    // Animate ES12_3 (ptr.png) for level 2
    if (ES->showPotion12_3) {
        const int BASE_Y = 300;
        ES->positionES12_3.y = BASE_Y + POTION12_AMPLITUDE * sin(POTION12_FREQUENCY * t_now);
        printf("Animating potion12_3 at x=%d, y=%d\n", ES->positionES12_3.x, ES->positionES12_3.y);
    }
}

/**
 * @brief Displays potions and treasure on the screen.
 * @param ES Enemy structure containing potion data.
 * @param screen Screen surface to render to.
 * @param level Current game level (1 or 2).
 */
void display_ES(Ennemi ES, SDL_Surface* screen, int level) {
    if (ES.imagep1 != NULL && screen != NULL && level == 1) {
        printf("Displaying potion0 at x=%d, y=%d\n", ES.positionES.x, ES.positionES.y);
        SDL_BlitSurface(ES.imagep1, NULL, screen, &ES.positionES);
    }
    if (ES.showPotion2 && ES.imagep2 != NULL && screen != NULL && level == 1) {
        printf("Displaying potion11 at x=%d, y=%d\n", ES.positionES2.x, ES.positionES2.y);
        SDL_BlitSurface(ES.imagep2, NULL, screen, &ES.positionES2);
    }
    if (ES.showPotion6 && ES.imagep6 != NULL && screen != NULL && level == 1) {
        printf("Displaying poti at x=%d, y=%d\n", ES.positionES6.x, ES.positionES6.y);
        SDL_BlitSurface(ES.imagep6, NULL, screen, &ES.positionES6);
    }
    if (ES.imagep3 != NULL && screen != NULL && level == 1) {
        if (ES.showPotion3) {
            printf("Displaying potion3 at x=%d, y=%d\n", ES.positionES3.x, ES.positionES3.y);
            SDL_BlitSurface(ES.imagep3, NULL, screen, &ES.positionES3);
        }
        
        if (ES.showPotion3_3) {
            printf("Displaying potion3_3 at x=%d, y=%d\n", ES.positionES3_3.x, ES.positionES3_3.y);
            SDL_BlitSurface(ES.imagep3, NULL, screen, &ES.positionES3_3);
        }
    }
    if (ES.imagep12 != NULL && screen != NULL && level == 2) {
        if (ES.showPotion12) {
            SDL_BlitSurface(ES.imagep12, NULL, screen, &ES.positionES12);
        }
        if (ES.showPotion12_2) {
            SDL_BlitSurface(ES.imagep12, NULL, screen, &ES.positionES12_2);
        }
        if (ES.showPotion12_3) {
            SDL_BlitSurface(ES.imagep12, NULL, screen, &ES.positionES12_3);
        }
        SDL_BlitSurface(ES.imagep13, NULL, screen, &ES.positionES13);
        SDL_BlitSurface(ES.imagep15, NULL, screen, &ES.positionES15);
	if (ES.showPotion16 && ES.imagep16 != NULL) {
            SDL_BlitSurface(ES.imagep16, NULL, screen, &ES.positionES16);
            printf("Displaying imagep16 (relic2_s.png) at x=%d, y=%d, showPotion16=%d\n", 
                   ES.positionES16.x, ES.positionES16.y, ES.showPotion16);
        }
        if (ES.showPotion17 && ES.imagep17 != NULL) {
            SDL_BlitSurface(ES.imagep17, NULL, screen, &ES.positionES17);
            printf("Displaying imagep17 (potion17.png) at x=%d, y=%d, showPotion17=%d\n", 
                   ES.positionES17.x, ES.positionES17.y, ES.showPotion17);
        }
        if (ES.showPotion18 && ES.imagep18 != NULL) {
            SDL_BlitSurface(ES.imagep18, NULL, screen, &ES.positionES18);

        }
        if (ES.showPotion19 && ES.imagep19 != NULL) {
            SDL_BlitSurface(ES.imagep19, NULL, screen, &ES.positionES19);

        }
    }
    
     if (ES.imagep14 != NULL && screen != NULL && level == 2) {
        if (ES.showPotion14) {
            SDL_BlitSurface(ES.imagep14, NULL, screen, &ES.positionES14);
        }
        if (ES.showPotion14_2) {
            SDL_BlitSurface(ES.imagep14, NULL, screen, &ES.positionES14_2);
        }
        if (ES.showPotion14_3) {
            SDL_BlitSurface(ES.imagep14, NULL, screen, &ES.positionES14_3);
        }
    }
    
    if (ES.imagep7 != NULL && screen != NULL && level == 1) {
        printf("Displaying potion33 at x=%d, y=%d\n", ES.positionES7.x, ES.positionES7.y);
        SDL_BlitSurface(ES.imagep7, NULL, screen, &ES.positionES7);
    }
    if (ES.imagep8 != NULL && screen != NULL && level == 1) {
        printf("Displaying head at x=%d, y=%d\n", ES.positionES8.x, ES.positionES8.y);
        SDL_BlitSurface(ES.imagep8, NULL, screen, &ES.positionES8);
    }
    if (ES.imagep9 != NULL && screen != NULL && level == 1) {
        printf("Displaying mommy at x=%d, y=%d\n", ES.positionES9.x, ES.positionES9.y);
        SDL_BlitSurface(ES.imagep9, NULL, screen, &ES.positionES9);
    }
    if (ES.imagep11 != NULL && screen != NULL && level == 2) {
        printf("Displaying closet at x=%d, y=%d\n", ES.positionES11.x, ES.positionES11.y);
        SDL_BlitSurface(ES.imagep11, NULL, screen, &ES.positionES11);
    }
    if (ES.imageTresor1 != NULL && screen != NULL && level == 1) {
        SDL_Surface *currentTresor = ES.imageTresor1;
        if (ES.treasureAnimStartTime > 0) {
            Uint32 elapsed = SDL_GetTicks() - ES.treasureAnimStartTime;
            ES.treasureFrame = (elapsed / 250) % 4; // 250ms per image
            if (ES.treasureFrame == 0) currentTresor = ES.imageTresor1;
            else if (ES.treasureFrame == 1) currentTresor = ES.imageTresor2;
            else if (ES.treasureFrame == 2) currentTresor = ES.imageTresor3;
            else if (ES.treasureFrame == 3) currentTresor = ES.imageTresor4;
            printf("Displaying animated treasure tresor%d.png at x=%d, y=%d\n", ES.treasureFrame + 1, ES.positionES5.x, ES.positionES5.y);
        } else {
            printf("Displaying static treasure tresor1.png at x=%d, y=%d\n", ES.positionES5.x, ES.positionES5.y);
        }
        SDL_BlitSurface(currentTresor, NULL, screen, &ES.positionES5);
    }
}

/**
 * @brief Displays enemy health on the screen.
 * @param E Enemy structure.
 * @param screen Screen surface to render to.
 */
void display_health(Ennemi E, SDL_Surface* screen) {
    if (!E.isAlive || E.font == NULL || screen == NULL) return;

    char healthText[32];
    snprintf(healthText, sizeof(healthText), "Enemy Health: %d", E.health);

    SDL_Color textColor = {255, 255, 255, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid(E.font, healthText, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text: %s\n", TTF_GetError());
        return;
    }

    SDL_Rect textPosition = {400, 10, 0, 0};
    SDL_BlitSurface(textSurface, NULL, screen, &textPosition);
    SDL_FreeSurface(textSurface);
}

/**
 * @brief Moves the enemy based on level and player position.
 * @param E Pointer to the enemy structure.
 * @param level Current game level (1 or 2).
 * @param playerPos Pointer to the player's position.
 */
void move_ennemi(Ennemi* E, int level, SDL_Rect* playerPos) {
    if (E == NULL || !E->isAlive || playerPos == NULL) return;

    if (level == 1) {
        if (E->State == WAITING) {
            if (E->Direction == 2) {
                E->positionAbsolue.x += SPEED;
                if (E->positionAbsolue.x >= 1300) {
                    E->positionAbsolue.x = 1300;
                    E->Direction = 1;
                    printf("Enemy hit three-quarter mark (1920), switching to left\n");
                }
            } else {
                E->positionAbsolue.x -= SPEED;
                if (E->positionAbsolue.x <= 1100) {
                    E->positionAbsolue.x = 1100;
                    E->Direction = 2;
                    printf("Enemy hit middle (1280), switching to right\n");
                }
            }

            E->Frame.j++;
            if (E->Frame.j >= SPRITE_ENNEMI_NbCol)
                E->Frame.j = 0;

        } else if (E->State == FOLLOWING) {
            if (playerPos->x > E->positionAbsolue.x) {
                E->positionAbsolue.x += SPEED;
                E->Direction = 2;
                E->Frame.i = 0;
            } else {
                E->positionAbsolue.x -= SPEED;
                E->Direction = 1;
                E->Frame.i = 1;
            }
            E->Frame.j++;
            if (E->Frame.j >= SPRITE_ENNEMI_NbCol)
                E->Frame.j = 0;
            printf("Following player: col=%d, x=%d\n", E->Frame.j, E->positionAbsolue.x);
        }
    } else if (level == 2) {
        if (E->State == WAITING) {
            if (E->Direction == 2) {
                E->positionAbsolue.x += SPEED;
                if (E->positionAbsolue.x >= 1500) {
                    E->positionAbsolue.x = 1500;
                    E->Direction = 1;
                    printf("Enemy hit three-quarter mark (1920), switching to left\n");
                }
            } else {
                E->positionAbsolue.x -= SPEED;
                if (E->positionAbsolue.x <= 1200) {
                    E->positionAbsolue.x = 1200;
                    E->Direction = 2;
                    printf("Enemy hit middle (1280), switching to right\n");
                }
            }

            E->Frame.j++;
            if (E->Frame.j >= SPRITE_ENNEMI_NbCol)
                E->Frame.j = 0;

        } else if (E->State == FOLLOWING) {
            if (playerPos->x > E->positionAbsolue.x) {
                E->positionAbsolue.x += SPEED;
                E->Direction = 2;
                E->Frame.i = 0;
            } else {
                E->positionAbsolue.x -= SPEED;
                E->Direction = 1;
                E->Frame.i = 1;
            }
            E->Frame.j++;
            if (E->Frame.j >= SPRITE_ENNEMI_NbCol)
                E->Frame.j = 0;
            printf("Following player: col=%d, x=%d\n", E->Frame.j, E->positionAbsolue.x);
        }
    }
}

/**
 * @brief Animates the enemy's sprite.
 * @param E Pointer to the enemy structure.
 */
void animateEnnemi(Ennemi* E) {
    if (E == NULL || !E->isAlive) return;

    E->Frame.j++;
    if (E->Frame.j >= SPRITE_ENNEMI_NbCol)
        E->Frame.j = 0;

    if (E->State == ATTACKING) {
        if (E->Direction == 2) {
            E->Frame.i = 2;
        } else {
            E->Frame.i = 3;
        }
    } else {
        if (E->Direction == 2) {
            E->Frame.i = 0;
        } else {
            E->Frame.i = 1;
        }
    }

    SDL_Delay(100);
}

/**
 * @brief Updates the state of enemy1 (level 1) based on player position and collision.
 * @param E Pointer to the enemy structure.
 * @param playerX Player's x position.
 * @param isColliding Flag indicating if collision occurred.
 */
void updateEnnemiState(Ennemi* E, int playerX, int isColliding) {
    if (E == NULL) return;

    // Force WAITING state if playerX >= 1200
    if (playerX >= 1200) {
        if (E->State != WAITING) {
            E->State = WAITING;
            printf("Enemy forced to WAITING: playerX=%d\n", playerX);
        }
        return;
    }
    
    const int zone = 1000; // 1280

    switch (E->State) {
        case WAITING:
            if (playerX >= zone) {
                E->State = FOLLOWING;
                printf("Enemy transition: WAITING -> FOLLOWING, playerX=%d\n", playerX);
            }
            break;
        case FOLLOWING:
            if (isColliding) {
                E->State = ATTACKING;
                printf("Enemy transition: FOLLOWING -> ATTACKING, collision detected\n");
            } else if (playerX < zone) {
                E->State = WAITING;
                printf("Enemy transition: FOLLOWING -> WAITING, playerX=%d\n", playerX);
            }
            break;
        case ATTACKING:
            if (!isColliding) {
                if (playerX >= zone) {
                    E->State = FOLLOWING;
                    printf("Enemy transition: ATTACKING -> FOLLOWING, no collision, playerX=%d\n", playerX);
                } else {
                    E->State = WAITING;
                    printf("Enemy transition: ATTACKING -> WAITING, no collision, playerX=%d\n", playerX);
                }
            }
            break;
    }
}

/**
 * @brief Updates the state of enemy2 (level 2) based on player position and collision.
 * @param E Pointer to the enemy structure.
 * @param playerX Player's x position.
 * @param isColliding Flag indicating if collision occurred.
 */
void updateEnnemi2State(Ennemi* E, int playerX, int isColliding) {
    if (E == NULL) return;

    // Force WAITING state if playerX >= 1800
    if (playerX >= 1800) {
        if (E->State != WAITING) {
            E->State = WAITING;
            printf("Enemy2 forced to WAITING (level 2): playerX=%d\n", playerX);
        }
        return;
    }
    
    const int zone = 1200; // Adjusted for level 2

    switch (E->State) {
        case WAITING:
            if (playerX >= zone) {
                E->State = FOLLOWING;
                printf("Enemy2 transition (level 2): WAITING -> FOLLOWING, playerX=%d, zone=%d\n", playerX, zone);
            }
            break;
        case FOLLOWING:
            if (isColliding) {
                E->State = ATTACKING;
                printf("Enemy2 transition (level 2): FOLLOWING -> ATTACKING, collision detected\n");
            } else if (playerX < zone) {
                E->State = WAITING;
                printf("Enemy2 transition (level 2): FOLLOWING -> WAITING, playerX=%d, zone=%d\n", playerX, zone);
            }
            break;
        case ATTACKING:
            if (!isColliding) {
                if (playerX >= zone) {
                    E->State = FOLLOWING;
                    printf("Enemy2 transition (level 2): ATTACKING -> FOLLOWING, no collision, playerX=%d, zone=%d\n", playerX, zone);
                } else {
                    E->State = WAITING;
                    printf("Enemy2 transition (level 2): ATTACKING -> WAITING, no collision, playerX=%d, zone=%d\n", playerX, zone);
                }
            }
            break;
    }
}

/**
 * @brief Frees enemy resources.
 * @param E Pointer to the enemy structure.
 */
void freeEnnemi(Ennemi* E) {
    if (E != NULL) {
        if (E->image != NULL) { SDL_FreeSurface(E->image); E->image = NULL; }
        if (E->imagep1 != NULL) { SDL_FreeSurface(E->imagep1); E->imagep1 = NULL; }
        if (E->imagep2 != NULL) { SDL_FreeSurface(E->imagep2); E->imagep2 = NULL; }
        if (E->imagep3 != NULL) { SDL_FreeSurface(E->imagep3); E->imagep3 = NULL; }
        if (E->imagep4 != NULL) { SDL_FreeSurface(E->imagep4); E->imagep4 = NULL; }
        if (E->imagep6 != NULL) { SDL_FreeSurface(E->imagep6); E->imagep6 = NULL; }
        if (E->imagep7 != NULL) { SDL_FreeSurface(E->imagep7); E->imagep7 = NULL; }
        if (E->imagep8 != NULL) { SDL_FreeSurface(E->imagep8); E->imagep8 = NULL; }
        if (E->imagep9 != NULL) { SDL_FreeSurface(E->imagep9); E->imagep9 = NULL; }
        if (E->imagep10 != NULL) { SDL_FreeSurface(E->imagep10); E->imagep10 = NULL; }
        if (E->imagep11 != NULL) { SDL_FreeSurface(E->imagep11); E->imagep11 = NULL; }
        if (E->imagep12 != NULL) { SDL_FreeSurface(E->imagep12); E->imagep12 = NULL; }
        if (E->imagep13 != NULL) { SDL_FreeSurface(E->imagep13); E->imagep13 = NULL; }
        if (E->imagep14 != NULL) { SDL_FreeSurface(E->imagep14); E->imagep14 = NULL; }
        if (E->imagep15 != NULL) { SDL_FreeSurface(E->imagep15); E->imagep15 = NULL; }
        if (E->imagep16 != NULL) { SDL_FreeSurface(E->imagep16); E->imagep16 = NULL; }
        if (E->imagep17 != NULL) { SDL_FreeSurface(E->imagep17); E->imagep17 = NULL; }
        if (E->imageTresor1 != NULL) { SDL_FreeSurface(E->imageTresor1); E->imageTresor1 = NULL; }
        if (E->imageTresor2 != NULL) { SDL_FreeSurface(E->imageTresor2); E->imageTresor2 = NULL; }
        if (E->imageTresor3 != NULL) { SDL_FreeSurface(E->imageTresor3); E->imageTresor3 = NULL; }
        if (E->imageTresor4 != NULL) { SDL_FreeSurface(E->imageTresor4); E->imageTresor4 = NULL; }
        E->font = NULL;
    }
}

