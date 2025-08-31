/**
 * @file main.c
 * @brief Main entry point for the SDL 2D platformer game.
 * @author MohamedMaalej, Houssein
 * @version 1.0
 * @date 2025-05-18
 *
 * This file integrates player, enemy, potion, treasure, background, nuage, and audio mechanics with SDL 1.2.
 */

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "personne.h"
#include "ennemie.h"
#include "background.h"
#include "enigme.h"
#include "game.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 754
#define BACKGROUND_WIDTH 2560
#define BACKGROUND_HEIGHT 754
#define POTION3_AMPLITUDE 20.0f
#define POTION3_FREQUENCY 0.005f
/**
 * @brief Checks collision between player and enemy.
 * @param p Pointer to the player structure.
 * @param e Pointer to the enemy structure.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerEnemyCollision(personnage *p, Ennemi *e) {
    if (!e->isAlive) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int enemyRight = e->positionAbsolue.x + e->positionAbsolue.w;
    int enemyBottom = e->positionAbsolue.y + e->positionAbsolue.h;
    return (p->position.x < enemyRight &&
            playerRight > e->positionAbsolue.x &&
            p->position.y < enemyBottom &&
            playerBottom > e->positionAbsolue.y);
}

/**
 * @brief Checks collision between player and first potion (ES1).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerESCollision(personnage *p, Ennemi *es) {
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES.x + es->positionES.w;
    int esBottom = es->positionES.y + es->positionES.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES.y);
}

/**
 * @brief Checks collision between player and second potion (ES2).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES2Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion2) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES2.x + es->positionES2.w;
    int esBottom = es->positionES2.y + es->positionES2.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES2.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES2.y);
}

/**
 * @brief Checks collision between player and third potion (ES3, first instance).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES3Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion3) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES3.x + es->positionES3.w;
    int esBottom = es->positionES3.y + es->positionES3.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES3.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES3.y);
}

/**
 * @brief Checks collision between player and third potion (ES3_2, second instance).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES3_2Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion3_2) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES3_2.x + es->positionES3_2.w;
    int esBottom = es->positionES3_2.y + es->positionES3_2.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES3_2.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES3_2.y);
}

/**
 * @brief Checks collision between player and third potion (ES3_3, third instance).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES3_3Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion3_3) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES3_3.x + es->positionES3_3.w;
    int esBottom = es->positionES3_3.y + es->positionES3_3.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES3_3.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES3_3.y);
}

/**
 * @brief Checks collision between player and sixth potion (ES6, poti.png).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES6Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion6) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES6.x + es->positionES6.w;
    int esBottom = es->positionES6.y + es->positionES6.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES6.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES6.y);
}

/**
 * @brief Displays the player's health on the screen.
 * @param p Player structure.
 * @param screen Screen surface to render to.
 * @param font Font for rendering text.
 */
void display_player_health(personnage p, SDL_Surface *screen, TTF_Font *font) {
    if (!font || !screen) {
        printf("Erreur : Police ou écran non défini pour afficher la santé du joueur\n");
        return;
    }
    char healthText[32];
    snprintf(healthText, sizeof(healthText), "Player Health: %d", p.vie);
    SDL_Color textColor = {255, 255, 255, 0};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, healthText, textColor);
    if (!textSurface) {
        printf("Unable to render player health text: %s\n", TTF_GetError());
        return;
    }
    SDL_Rect textPosition = {10, 10, 0, 0};
    SDL_BlitSurface(textSurface, NULL, screen, &textPosition);
    SDL_FreeSurface(textSurface);
    printf("Affichage santé joueur : %s à x=%d, y=%d\n", healthText, textPosition.x, textPosition.y);
}

/**
 * @brief Displays the player's score on the screen.
 * @param score Current score value.
 * @param screen Screen surface to render to.
 * @param font Font for rendering text.
 */
void display_score(int score, SDL_Surface *screen, TTF_Font *font) {
    if (!font || !screen) {
        printf("Erreur : Police ou écran non défini pour afficher le score\n");
        return;
    }
    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    SDL_Color textColor = {255, 255, 255, 0};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    if (!textSurface) {
        printf("Unable to render score text: %s\n", TTF_GetError());
        return;
    }
    SDL_Rect textPosition = {10, 40, 0, 0};
    SDL_BlitSurface(textSurface, NULL, screen, &textPosition);
    SDL_FreeSurface(textSurface);
    printf("Affichage score : %s à x=%d, y=%d\n", scoreText, textPosition.x, textPosition.y);
}


/**
 * @brief Runs a single-question enigma quiz until score reaches 60 or other conditions are met.
 * @return 1 if score reaches 60 or correct answer, 0 if incorrect, timed out, or lives depleted.
 */
int run_enigma(enigme *e, SDL_Surface *screen, TTF_Font *font) {
    if (!e || !screen || !font) {
        printf("Error: Null enigma, screen, or font in run_enigma\n");
        return 0;
    }
    if (e->nb_questions == 0) {
        printf("Error: No questions available for enigma\n");
        return 0;
    }

    // Verify font is valid
    if (TTF_FontHeight(font) <= 0) {
        printf("Error: Font appears invalid or corrupted\n");
        return 0;
    }

    // Clear event queue
    SDL_Event clear_event;
    while (SDL_PollEvent(&clear_event)) {
        // Discard events
    }

    int mouse_x = 0, mouse_y = 0;
    int correct = 0;

    // Continue running enigma until score >= 60, no lives left, or no questions left
    while (e->score < 60 && e->vies > 0 && e->questions_answered < e->nb_questions) {
        // Find an unused question
        int q_index = rand() % e->nb_questions;
        int attempts = 0;
        while (e->questions_used[q_index] && attempts < e->nb_questions) {
            q_index = (q_index + 1) % e->nb_questions;
            attempts++;
        }
        if (e->questions_used[q_index]) {
            printf("No more unused questions available\n");
            break;
        }

        e->questions_used[q_index] = 1;
        e->questions_answered++;

        printf("Generating enigma: q_index=%d, score=%d\n", q_index, e->score);
        generer_enigme(e, q_index, font);
        if (!e->question || !e->reponses_surface[0] || !e->reponses_surface[1] || !e->reponses_surface[2]) {
            printf("Error: Failed to generate enigma resources, cleaning up\n");
            // Clean up partially allocated resources
            if (e->question) {
                SDL_FreeSurface(e->question);
                e->question = NULL;
            }
            for (int i = 0; i < NB_REPONSES; i++) {
                if (e->reponses_surface[i]) {
                    SDL_FreeSurface(e->reponses_surface[i]);
                    e->reponses_surface[i] = NULL;
                }
            }
            if (e->score_surface) {
                SDL_FreeSurface(e->score_surface);
                e->score_surface = NULL;
            }
            if (e->level_surface) {
                SDL_FreeSurface(e->level_surface);
                e->level_surface = NULL;
            }
            for (int i = 0; i < MAX_VIES; i++) {
                if (e->vies_surface[i]) {
                    SDL_FreeSurface(e->vies_surface[i]);
                    e->vies_surface[i] = NULL;
                }
            }
            return 0;
        }

        int answered = 0;
        while (!answered && e->vies > 0) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    answered = 1;
                    correct = 0;
                    printf("Enigma terminated: Quit or ESC\n");
                    // Clean up enigma resources before exiting
                    if (e->question) {
                        SDL_FreeSurface(e->question);
                        e->question = NULL;
                    }
                    for (int i = 0; i < NB_REPONSES; i++) {
                        if (e->reponses_surface[i]) {
                            SDL_FreeSurface(e->reponses_surface[i]);
                            e->reponses_surface[i] = NULL;
                        }
                    }
                    if (e->score_surface) {
                        SDL_FreeSurface(e->score_surface);
                        e->score_surface = NULL;
                    }
                    if (e->level_surface) {
                        SDL_FreeSurface(e->level_surface);
                        e->level_surface = NULL;
                    }
                    for (int i = 0; i < MAX_VIES; i++) {
                        if (e->vies_surface[i]) {
                            SDL_FreeSurface(e->vies_surface[i]);
                            e->vies_surface[i] = NULL;
                        }
                    }
                    return 0;
                }
                else if (event.type == SDL_MOUSEMOTION) {
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                    printf("Mouse moved: x=%d, y=%d\n", mouse_x, mouse_y);
                }
                else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                    printf("Mouse clicked: x=%d, y=%d\n", event.button.x, event.button.y);
                    if (e->button &&
                        mouse_x >= e->pos_reponse1.x && mouse_x <= e->pos_reponse1.x + e->button->w &&
                        mouse_y >= e->pos_reponse1.y && mouse_y <= e->pos_reponse1.y + e->button->h) {
                        e->pos_selected = 1;
                        if (e->sound) Mix_PlayChannel(-1, e->sound, 0);
                        correct = verify_enigme(e, screen);
                        afficher_resultat(screen, correct, e, font);
                        answered = 1;
                        printf("Selected answer 1: correct=%d, score=%d\n", correct, e->score);
                    }
                    else if (e->button &&
                             mouse_x >= e->pos_reponse2.x && mouse_x <= e->pos_reponse2.x + e->button->w &&
                             mouse_y >= e->pos_reponse2.y && mouse_y <= e->pos_reponse2.y + e->button->h) {
                        e->pos_selected = 2;
                        if (e->sound) Mix_PlayChannel(-1, e->sound, 0);
                        correct = verify_enigme(e, screen);
                        afficher_resultat(screen, correct, e, font);
                        answered = 1;
                        printf("Selected answer 2: correct=%d, score=%d\n", correct, e->score);
                    }
                    else if (e->button &&
                             mouse_x >= e->pos_reponse3.x && mouse_x <= e->pos_reponse3.x + e->button->w &&
                             mouse_y >= e->pos_reponse3.y && mouse_y <= e->pos_reponse3.y + e->button->h) {
                        e->pos_selected = 3;
                        if (e->sound) Mix_PlayChannel(-1, e->sound, 0);
                        correct = verify_enigme(e, screen);
                        afficher_resultat(screen, correct, e, font);
                        answered = 1;
                        printf("Selected answer 3: correct=%d, score=%d\n", correct, e->score);
                    }
                }
            }

            mettre_a_jour_timer(e);
            if (e->temps_restant <= 0) {
                e->vies--;
                afficher_resultat(screen, 0, e, font);
                answered = 1;
                correct = 0;
                printf("Enigma timed out: lives=%d\n", e->vies);
            }

            afficher_enigme(e, screen, mouse_x, mouse_y);
            SDL_Delay(16);
        }

        // Clean up resources after each question
        printf("Cleaning up enigma resources after question\n");
        if (e->question) {
            SDL_FreeSurface(e->question);
            e->question = NULL;
        }
        for (int i = 0; i < NB_REPONSES; i++) {
            if (e->reponses_surface[i]) {
                SDL_FreeSurface(e->reponses_surface[i]);
                e->reponses_surface[i] = NULL;
            }
        }
        if (e->score_surface) {
            SDL_FreeSurface(e->score_surface);
            e->score_surface = NULL;
        }
        if (e->level_surface) {
            SDL_FreeSurface(e->level_surface);
            e->level_surface = NULL;
        }
        for (int i = 0; i < MAX_VIES; i++) {
            if (e->vies_surface[i]) {
                SDL_FreeSurface(e->vies_surface[i]);
                e->vies_surface[i] = NULL;
            }
        }

        // If score reaches 60, exit the enigma
        if (e->score >= 60) {
            printf("Enigma score reached 60, exiting enigma\n");
            correct = 1;
            break;
        }
    }

    // Final cleanup before exiting
    printf("Final cleanup before exiting run_enigma\n");
    if (e->question) {
        SDL_FreeSurface(e->question);
        e->question = NULL;
    }
    for (int i = 0; i < NB_REPONSES; i++) {
        if (e->reponses_surface[i]) {
            SDL_FreeSurface(e->reponses_surface[i]);
            e->reponses_surface[i] = NULL;
        }
    }
    if (e->score_surface) {
        SDL_FreeSurface(e->score_surface);
        e->score_surface = NULL;
    }
    if (e->level_surface) {
        SDL_FreeSurface(e->level_surface);
        e->level_surface = NULL;
    }
    for (int i = 0; i < MAX_VIES; i++) {
        if (e->vies_surface[i]) {
            SDL_FreeSurface(e->vies_surface[i]);
            e->vies_surface[i] = NULL;
        }
    }

    // Clear event queue after enigma
    while (SDL_PollEvent(&clear_event)) {
        // Discard events
    }

    printf("Enigma completed: score=%d, lives=%d, correct=%d\n", e->score, e->vies, correct);
    return correct;
}
/**
 * @brief Checks collision between player and mommy potion (ES9, mommy.png).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES9Collision(personnage *p, Ennemi *es) {
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES9.x + es->positionES9.w;
    int esBottom = es->positionES9.y + es->positionES9.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES9.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES9.y);
}


/**
 * @brief Checks collision between player and first ptr potion (ES12).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES12Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion12) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES12.x + es->positionES12.w;
    int esBottom = es->positionES12.y + es->positionES12.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES12.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES12.y);
}

/**
 * @brief Checks collision between player and second ptr potion (ES12_2).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES12_2Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion12_2) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES12_2.x + es->positionES12_2.w;
    int esBottom = es->positionES12_2.y + es->positionES12_2.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES12_2.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES12_2.y);
}

int checkPlayerES14_2Collision(personnage *p, Ennemi *es) {

    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES14_2.x + es->positionES14_2.w;
    int esBottom = es->positionES14_2.y + es->positionES14_2.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES14_2.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES14_2.y);
}
int checkPlayerES14_3Collision(personnage *p, Ennemi *es) {

    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES14_3.x + es->positionES14_3.w;
    int esBottom = es->positionES14_3.y + es->positionES14_3.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES14_3.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES14_3.y);
}
int checkPlayerES14Collision(personnage *p, Ennemi *es) {

    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES14.x + es->positionES14.w;
    int esBottom = es->positionES14.y + es->positionES14.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES14.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES14.y);
}



/**
 * @brief Checks collision between player and third ptr potion (ES12_3).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES12_3Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion12_3) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES12_3.x + es->positionES12_3.w;
    int esBottom = es->positionES12_3.y + es->positionES12_3.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES12_3.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES12_3.y);
}
int checkPlayerES11Collision(personnage *p, Ennemi *es) {
    // Player bounding box (170x170 pixels)
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    
    // ES11 bounding box (100x100 pixels)
    int esRight = es->positionES11.x + es->positionES11.w;
    int esBottom = es->positionES11.y + es->positionES11.h;
    
    // Check for rectangle intersection
    return (p->position.x < esRight &&
            playerRight > es->positionES11.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES11.y);
}
/**
 * @brief Checks collision between player and relic2_s potion (ES16, relic2_s.png).
 * @param p Pointer to the player structure.
 * @param es Pointer to the enemy structure containing potion data.
 * @return 1 if collision occurs, 0 otherwise.
 */
int checkPlayerES16Collision(personnage *p, Ennemi *es) {
    if (!es->showPotion16) return 0;
    int playerRight = p->position.x + 170;
    int playerBottom = p->position.y + 170;
    int esRight = es->positionES16.x + es->positionES16.w;
    int esBottom = es->positionES16.y + es->positionES16.h;
    return (p->position.x < esRight &&
            playerRight > es->positionES16.x &&
            p->position.y < esBottom &&
            playerBottom > es->positionES16.y);
}



/**
 * @brief Main game function.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on success, non-zero on failure.
 */
int main(int argc, char *argv[]) {
 SDL_Surface *screen;
    personnage p1, p2;
    Ennemi enemy, enemy2, secondaryEntity;
    Background bg;
    SDL_Surface *score_image = NULL;
    SDL_Surface *boss_image = NULL;
    SDL_Surface *letter_image = NULL;
    SDL_Surface *solo_image = NULL;
    SDL_Surface *multi_image = NULL;
    SDL_Surface *input1_image = NULL;
    SDL_Surface *input2_image = NULL;
    SDL_Surface *zeus_image = NULL;
    SDL_Surface *default_skin_image = NULL;
    SDL_Surface *red_skin_image = NULL;
    SDL_Surface *heart = NULL;
    SDL_Surface *hint_image = NULL;
    SDL_Surface *nuage_image = NULL;
    SDL_Surface *cle_image = NULL;
    SDL_Surface *romlet_image = NULL;
    SDL_Surface *intro_image = NULL; // New image for intro.png
    Mix_Chunk *boss_sound = NULL;
    Mix_Chunk *door_sound = NULL;
    Mix_Chunk *door2_sound = NULL;
    Mix_Chunk *health_sound = NULL;
    Mix_Chunk *pts_sound = NULL;
    Mix_Chunk *poison_sound = NULL;
    Mix_Chunk *tresor_sound = NULL;
    Mix_Chunk *fight_sound = NULL;
    Mix_Chunk *egypte_sound = NULL;
    Mix_Chunk *doom_sound = NULL;
    Mix_Chunk *rome_sound = NULL;
    Mix_Chunk *intro_sound = NULL; // New sound for intro.wav
    TTF_Font *font = NULL;
    int show_relic_image = 0;
    int show_key_image = 0;
    int show_romlet_image = 0;
    Uint32 lastES9CollectTime = 0;
    Uint32 key_image_start_time = 0;
    Uint32 romlet_image_start_time = 0;
    Uint32 zeus_anim_start_time = 0;
    int show_zeus_image = 0;
    int score = 0;
    int treasureCollected = 0;
    int es2Collected = 0;
    int egypte_sound_channel = -1;
    int es6Collected = 0;
    enigme enigma = {0};
    int bossAnimActive = 0;
    int enigmaTriggered = 0;
    int enigmaSolved = 0;
    int fight_sound_active = 0;
    Uint32 bossAnimStartTime = 0;
    Uint32 relic_image_start_time = 0;
    SDL_Rect bossPosition = {1280, 0, 0, 0};
    SDL_Rect nuagePosition = {1280, 0, 0, 0};
    SDL_Event event;
    Uint32 t_prev;
    int egypte_sound_played = 0;
    Uint32 lastESDamageTime = 0;
    int door_sound_played = 0;
    int door2_sound_played = 0;
    int rome_sound_played = 0;
    int fight_sound_channel = -1;
    Uint32 lastEnemyDamageTime = 0;
    Uint32 lastEnemyHealthLossTime = 0;
    Uint32 lastEnemy2HealthLossTime = 0;
    Uint32 lastES3CollectTime = 0;
    Uint32 lastES6CollectTime = 0;
    Uint32 lastES11CollectTime = 0;
    Uint32 lastTreasureCollectTime = 0;
    Uint32 last_potion_reset_time = 0;
    int images_shown = 0;
    int show_score_image = 0;
    int show_hint_image = 0;
    Uint32 score_image_start_time = 0;
    Uint32 hint_image_start_time = 0;
    Uint32 potion17_image_start_time = 0;
    const float ES2_AMPLITUDE = 50.0f;
    const float ES2_FREQUENCY = 0.005f;
    const float NUAGE_SPEED = 9.0f;
    int menu_state = 0;
    int game_started = 0;
    SDL_Rect zeusPosition = {40, 20, 0, 0};
    Game mazeGame = {0};
    int runMazeGame = 0;
    int nuage_sound_played = 0;
    int show_win_image = 0;
    int show_lose_image = 0;
    Uint32 win_image_start_time = 0;
    Uint32 lose_image_start_time = 0;
    int running = 1;
    int game_mode = -1;
    int active_player = 0;
    int level = 1;
    int intro_sound_channel = -1; // Channel for intro.wav


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Unable to init SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "Unable to init SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }
    if (init_background(&bg) < 0) {
        fprintf(stderr, "Failed to initialize background\n");
        return 1;
    }

    // Load menu images
    solo_image = IMG_Load("solo_mode.png");
    if (!solo_image) {
        fprintf(stderr, "Erreur de chargement de solo_mode.png: %s\n", IMG_GetError());
        return 1;
    }
    multi_image = IMG_Load("multi_mode.png");
    if (!multi_image) {
        fprintf(stderr, "Erreur de chargement de multi_mode.png: %s\n", IMG_GetError());
        return 1;
    }
    input1_image = IMG_Load("input1.png");
    if (!input1_image) {
        fprintf(stderr, "Erreur de chargement de input1.png: %s\n", IMG_GetError());
        return 1;
    }
    input2_image = IMG_Load("input2.png");
    if (!input2_image) {
        fprintf(stderr, "Erreur de chargement de input2.png: %s\n", IMG_GetError());
        return 1;
    }
    default_skin_image = IMG_Load("default_skin.png");
    if (!default_skin_image) {
        fprintf(stderr, "Erreur de chargement de default_skin.png: %s\n", IMG_GetError());
        return 1;
    }
    red_skin_image = IMG_Load("red_skin.png");
    if (!red_skin_image) {
        fprintf(stderr, "Erreur de chargement de red_skin.png: %s\n", IMG_GetError());
        return 1;
    }
    intro_image = IMG_Load("intro.png");
if (!intro_image) {
    fprintf(stderr, "Erreur de chargement de intro.png: %s\n", IMG_GetError());
    return 1;
}
intro_sound = Mix_LoadWAV("intro.wav");
if (!intro_sound) {
    fprintf(stderr, "Erreur de chargement de intro.wav: %s\n", Mix_GetError());
    return 1;
}

    // Load game images
    score_image = IMG_Load("score.png");
    if (!score_image) {
        fprintf(stderr, "Erreur de chargement de score.png: %s\n", IMG_GetError());
        return 1;
    }
    boss_image = IMG_Load("boss.png");
    if (!boss_image) {
        fprintf(stderr, "Erreur de chargement de boss.png: %s\n", IMG_GetError());
        return 1;
    }
    bossPosition.w = boss_image->w;
    bossPosition.h = boss_image->h;
    letter_image = IMG_Load("lettre.png");
    if (!letter_image) {
        fprintf(stderr, "Erreur de chargement de lettre.png: %s\n", IMG_GetError());
        return 1;
    }
    heart = IMG_Load("heart.png");
    if (!heart) {
        fprintf(stderr, "Erreur de chargement de heart.png: %s\n", IMG_GetError());
        return 1;
    }
    hint_image = IMG_Load("hint.png");
    if (!hint_image) {
        fprintf(stderr, "Erreur de chargement de hint.png: %s\n", IMG_GetError());
        return 1;
    }
    cle_image = IMG_Load("cle.png");
    if (!cle_image) {
        fprintf(stderr, "Erreur de chargement de cle.png: %s\n", IMG_GetError());
        return 1;
    }
     zeus_image = IMG_Load("zeus.png"); // Load zeus.png
    if (!zeus_image) {
        fprintf(stderr, "Erreur de chargement de zeus.png: %s\n", IMG_GetError());
        return 1;
    }
    zeusPosition.w = zeus_image->w;
    zeusPosition.h = zeus_image->h;
    zeusPosition.x = 40; // Center horizontally
    romlet_image = IMG_Load("romlet.png"); // Load romlet.png
    if (!romlet_image) {
        fprintf(stderr, "Erreur de chargement de romlet.png: %s\n", IMG_GetError());
        return 1;
    }
    nuage_image = IMG_Load("nuage.png");
    if (!nuage_image) {
        fprintf(stderr, "Erreur de chargement de nuage.png: %s\n", IMG_GetError());
        return 1;
    }
    nuagePosition.w = nuage_image->w;
    nuagePosition.h = nuage_image->h;
    nuagePosition.y = SCREEN_HEIGHT - nuage_image->h;
    boss_sound = Mix_LoadWAV("boss.wav");
    if (!boss_sound) {
        fprintf(stderr, "Erreur de chargement de boss.wav: %s\n", Mix_GetError());
        return 1;
    }
    door_sound = Mix_LoadWAV("door.wav"); // Load door.wav
if (!door_sound) {
    fprintf(stderr, "Erreur de chargement de door.wav: %s\n", Mix_GetError());
    return 1;
}
door2_sound = Mix_LoadWAV("door2.wav"); // Load door.wav
if (!door2_sound) {
    fprintf(stderr, "Erreur de chargement de door.wav: %s\n", Mix_GetError());
    return 1;
}
health_sound = Mix_LoadWAV("health.wav"); // Load health.wav
if (!health_sound) {
    fprintf(stderr, "Erreur de chargement de health.wav: %s\n", Mix_GetError());
    return 1;
}
fight_sound = Mix_LoadWAV("fight.wav"); // Load fight.wav
    if (!fight_sound) {
        fprintf(stderr, "Erreur de chargement de fight.wav: %s\n", Mix_GetError());
        return 1;
    }
pts_sound = Mix_LoadWAV("pts.wav"); // Load pts.wav
if (!pts_sound) {
    fprintf(stderr, "Erreur de chargement de pts.wav: %s\n", Mix_GetError());
    return 1;
}
tresor_sound = Mix_LoadWAV("tresor.wav"); // Load tresor.wav
if (!tresor_sound) {
    fprintf(stderr, "Erreur de chargement de tresor.wav: %s\n", Mix_GetError());
    return 1;
}
poison_sound = Mix_LoadWAV("poison.wav"); // Load poison.wav
if (!poison_sound) {
    fprintf(stderr, "Erreur de chargement de poison.wav: %s\n", Mix_GetError());
    return 1;
}
egypte_sound = Mix_LoadWAV("egypte.wav"); // Load egypte.wav
if (!egypte_sound) {
    fprintf(stderr, "Erreur de chargement de egypte.wav: %s\n", Mix_GetError());
    return 1;
}
doom_sound = Mix_LoadWAV("doom.wav"); // Load egypte.wav
if (!doom_sound) {
    fprintf(stderr, "Erreur de chargement de egypte.wav: %s\n", Mix_GetError());
    return 1;
}
rome_sound = Mix_LoadWAV("rome.wav"); // Load rome.wav
if (!rome_sound) {
    fprintf(stderr, "Erreur de chargement de rome.wav: %s\n", Mix_GetError());
    return 1;
}

    // Load font
    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Unable to load font: %s\n", TTF_GetError());

        return 1;
    }
    enemy.font = font;
    enemy2.font = font;
    secondaryEntity.font = font;
    printf("Police chargée avec succès : arial.ttf, taille 24\n");

	initialiser_enigme(&enigma);
	if (enigma.nb_questions == 0 || !enigma.background || !enigma.button || !enigma.button_s || !enigma.sound || !enigma.correct_sound || !enigma.wrong_sound) {
	    fprintf(stderr, "Failed to initialize enigma: missing resources\n");
	
	    return 1;
}
printf("Enigma initialized successfully\n");
    // Initialize background
    if (init_background(&bg) < 0) {
        fprintf(stderr, "Failed to initialize background\n");
    
        return 1;
    }

    // Menu loop
    while (menu_state != 2 && running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
                menu_state = 2;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                if (menu_state == 0) {
                    if (x >= 50 && x <= 350 && y >= 200 && y <= 400) {
                        game_mode = 0;
                        menu_state = 1;
                    }
                    if (x >= 350 && x <= 650 && y >= 200 && y <= 400) {
                        game_mode = 1;
                        active_player = 0;
                        initialiser_personnage(&p1, 0);
                        initialiser_personnage(&p2, 1);
                        p2.position.x = 100;
                        menu_state = 2;
                    }
                } else if (menu_state == 1) {
    if (x >= 50 && x <= 350 && y >= 200 && y <= 400) {
        active_player = 1;
        initialiser_personnage(&p1, 0);
        initialiser_personnage(&p2, 0);
        p2.position.x = 100;
        menu_state = 3; // Go to intro sequence
    }
    if (x >= 350 && x <= 650 && y >= 200 && y <= 400) {
        active_player = 2;
        initialiser_personnage(&p1, 1);
        initialiser_personnage(&p2, 1);
        p2.position.x = 100;
        menu_state = 3; // Go to intro sequence
    }
}
            }
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        if (menu_state == 0) {
            SDL_Rect solo_pos = {50, 200, 0, 0};
            SDL_Rect multi_pos = {350, 200, 0, 0};
            SDL_BlitSurface(solo_image, NULL, screen, &solo_pos);
            SDL_BlitSurface(multi_image, NULL, screen, &multi_pos);
        } else if (menu_state == 1) {
            SDL_Rect default_skin_pos = {50, 0, 0, 0};
            SDL_Rect red_skin_pos = {350, 0, 0, 0};
            SDL_Rect input1_pos = {50, 200, 0, 0};
            SDL_Rect input2_pos = {350, 200, 0, 0};
            SDL_BlitSurface(default_skin_image, NULL, screen, &default_skin_pos);
            SDL_BlitSurface(red_skin_image, NULL, screen, &red_skin_pos);
            SDL_BlitSurface(input1_image, NULL, screen, &input1_pos);
            SDL_BlitSurface(input2_image, NULL, screen, &input2_pos);
        }
        else if (menu_state == 3) {
    // Display intro.png
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_Rect intro_pos = {(SCREEN_WIDTH - intro_image->w) / 2, (SCREEN_HEIGHT - intro_image->h) / 2, 0, 0};
    SDL_BlitSurface(intro_image, NULL, screen, &intro_pos);
    SDL_Flip(screen);

    // Play intro.wav if not already playing
    if (intro_sound_channel == -1) {
        intro_sound_channel = Mix_PlayChannel(-1, intro_sound, 0);
        if (intro_sound_channel == -1) {
            fprintf(stderr, "Failed to play intro.wav: %s\n", Mix_GetError());
            menu_state = 2; // Proceed to game if sound fails
        } else {
            printf("Playing intro.wav on channel %d\n", intro_sound_channel);
        }
    }
    // Check if sound is still playing
    if (intro_sound_channel != -1 && !Mix_Playing(intro_sound_channel)) {
        printf("intro.wav finished, proceeding to game\n");
        intro_sound_channel = -1;
        menu_state = 2; // Start game
    }
}
        SDL_Flip(screen);
    }
    SDL_FreeSurface(solo_image);
    SDL_FreeSurface(multi_image);
    SDL_FreeSurface(input1_image);
    SDL_FreeSurface(input2_image);
    SDL_FreeSurface(default_skin_image);
    SDL_FreeSurface(red_skin_image);

    // Initialize enemies and treasure
    srand(time(NULL));
    if (init_ennemi(&enemy) == -1) {
        fprintf(stderr, "Failed to initialize enemy\n");
    
        return 1;
    }
    if (init_ennemi2(&enemy2) == -1) {
        fprintf(stderr, "Failed to initialize enemy2\n");
        return 1;
    }

    if (initES(&secondaryEntity) == -1) {
        fprintf(stderr, "Failed to initialize potions and treasure\n");
        return 1;
    }
    
    if (initES(&secondaryEntity) == -1) {
        fprintf(stderr, "Failed to initialize potions and treasure\n");
 


        
        return 1;
    }

    t_prev = SDL_GetTicks();

    // Game loop
    while (running) {
        Uint32 t_now = SDL_GetTicks();
        Uint32 dt = t_now - t_prev;
        t_prev = t_now;
update_door_animation(&bg);

// With:
if (bg.level == 1) {
    update_door_animation(&bg);
} else if (bg.level == 2) {
    update_door_animation_level2(&bg);
}

if (bg.level == 1 && !game_started && bg.door_frame == 5) {
    game_started = 1;
    bg.door_frame = 0;
    bg.door_anim_stopped = 1;
    if (!door_sound_played && door_sound) {
        Mix_PlayChannel(-1, door_sound, 0);
        door_sound_played = 1;
        printf("Playing door.wav for level 1 door opening\n");
    }
    if (!egypte_sound_played && egypte_sound) {
        egypte_sound_channel = Mix_PlayChannel(-1, egypte_sound, 0);
        if (egypte_sound_channel != -1) {
            egypte_sound_played = 1;
            printf("Playing egypte.wav for level 1 game start on channel %d\n", egypte_sound_channel);
        } else {
            printf("Failed to play egypte.wav: %s\n", Mix_GetError());
        }
    }
    printf("Level 1: Door reached porte6.png (frame 5), game started! Reverted to porte1.png and stopped animation.\n");
} else if (bg.level == 2 && !game_started && bg.door_frame == 5) {
    game_started = 1;
    bg.door_frame = 0; // Reset to prt1.png
    bg.door_anim_stopped = 1; // Stop door animation
    if (!door2_sound_played && door2_sound) {
        Mix_PlayChannel(-1, door2_sound, 0);
        door2_sound_played = 1;
        printf("Playing door2.wav for level 2 door opening\n");
    }
    printf("Level 2: Door reached prt6.png (frame 5), game started! Reverted to prt1.png and remains there.\n");
}
        // Handle events
        while (SDL_PollEvent(&event)) {
            handle_input(event, &p1, &p2, &running, active_player, game_mode, &bg);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                if (level == 1) {
                    if (enemy.State == WAITING) {
                        enemy.State = FOLLOWING;
                    } else if (enemy.State == FOLLOWING) {
                        enemy.State = ATTACKING;
                    } else {
                        enemy.State = WAITING;
                    }
                    printf("Enemy state toggled to %d via SPACE key\n", enemy.State);
                } else {
                    if (enemy2.State == WAITING) {
                        enemy2.State = FOLLOWING;
                    } else if (enemy2.State == FOLLOWING) {
                        enemy2.State = ATTACKING;
                    } else {
                        enemy2.State = WAITING;
                    }
                    printf("Enemy2 state toggled to %d via SPACE key\n", enemy2.State);
                }
            }
        }

        // Update players and camera
        personnage *active_p = (game_mode == 0 && active_player == 1) ? &p1 : (game_mode == 0 && active_player == 2) ? &p2 : &p1;
        if (game_mode == 0) {
            if (active_player == 1) {
                movePerso(&p1, dt, &bg);
                update_shield(&p1);
                update_camera(&bg, p1.position.x, screen);
                if (p1.vie <= 0) running = 0;
            } else if (active_player == 2) {
                movePerso(&p2, dt, &bg);
                update_shield(&p2);
                update_camera(&bg, p2.position.x, screen);
                if (p2.vie <= 0) running = 0;
            }
        } else {
            movePerso(&p1, dt, &bg);
            movePerso(&p2, dt, &bg);
            update_shield(&p1);
            update_shield(&p2);
            update_camera(&bg, (p1.position.x + p2.position.x) / 2, screen);
            if (p1.vie <= 0 && p2.vie <= 0) running = 0;
        }

        // Manage potion11.png (ES2)
        if (!es2Collected && !secondaryEntity.showPotion2 && t_now >= 15000) {
            secondaryEntity.showPotion2 = 1;
            printf("10 seconds elapsed, showing potion11.png\n");
        }
        if (es2Collected && (t_now - last_potion_reset_time >= 15000)) {
            secondaryEntity.potionTimer = t_now;
            secondaryEntity.showPotion2 = 0;
            es2Collected = 0;
            secondaryEntity.positionES2.x = 960;
            secondaryEntity.positionES2.y = 200;
            last_potion_reset_time = t_now;
            printf("Potion11 réinitialisée après 15 secondes\n");
        }
        if (secondaryEntity.showPotion2 && !es2Collected) {
            const int ES2_BASE_X = 960;
            secondaryEntity.positionES2.x = ES2_BASE_X + ES2_AMPLITUDE * sin(ES2_FREQUENCY * t_now);
        }

        // Manage poti.png (ES6)
        if (!es6Collected && !secondaryEntity.showPotion6 && t_now >= 5000 && level == 1) {
            secondaryEntity.showPotion6 = 1;
            printf("5 seconds elapsed, showing poti.png\n");
        }
        if (es6Collected && (t_now - last_potion_reset_time >= 15000)) {
            secondaryEntity.potionTimer = t_now;
            secondaryEntity.showPotion6 = 0;
            es6Collected = 0;
            secondaryEntity.positionES6.x = 550;
            secondaryEntity.positionES6.y = 150;
            last_potion_reset_time = t_now;
            printf("Poti.png réinitialisée après 15 secondes\n");
        }

        // Update treasure animation
        if (secondaryEntity.treasureAnimStartTime > 0 && (t_now - secondaryEntity.treasureAnimStartTime >= 1000)) {
            secondaryEntity.treasureAnimStartTime = 0;
            secondaryEntity.treasureFrame = 0;
            show_hint_image = 1;
            hint_image_start_time = t_now;
            printf("Treasure image sequence complete, showing hint.png\n");
        }

        // Manage hint image display
        if (show_hint_image && (t_now - hint_image_start_time >= 2000)) {
            show_hint_image = 0;
            printf("Fin affichage hint.png\n");
        }
        // Manage key image display
if (show_key_image && (t_now - key_image_start_time >= 2000)) {
    show_key_image = 0;
    printf("Fin affichage cle.png\n");
}

        // Animate potion3
        if (level == 1) {
            animatePotion3(&secondaryEntity);
        }

        // Update nuage.png animation
        if (level == 1) {
            nuagePosition.x -= NUAGE_SPEED;
            if (nuagePosition.x < -nuagePosition.w) {
                nuagePosition.x = SCREEN_WIDTH; // Reset to right edge
                printf("Resetting nuage.png to x=%d\n", nuagePosition.x);
            }
            printf("Moving nuage.png: x=%d\n", nuagePosition.x);
        }

        int playerX = active_p->position.x;
    int isColliding = 0;
    if (level == 1 && enemy.isAlive) {
        if (game_mode == 1) {
            int dist_p1 = abs(enemy.positionAbsolue.x - p1.position.x);
            int dist_p2 = abs(enemy.positionAbsolue.x - p2.position.x);
            active_p = dist_p1 < dist_p2 ? &p1 : &p2;
            playerX = active_p->position.x;
        }
        isColliding = checkPlayerEnemyCollision(active_p, &enemy);
        printf("PlayerX=%d, isColliding=%d\n", playerX, isColliding);
        updateEnnemiState(&enemy, playerX, isColliding); // Level 1: Use original function
        move_ennemi(&enemy, level, &active_p->position);
        animateEnnemi(&enemy);
    } else if (level == 2 && enemy2.isAlive) {
        if (game_mode == 1) {
            int dist_p1 = abs(enemy2.positionAbsolue.x - p1.position.x);
            int dist_p2 = abs(enemy2.positionAbsolue.x - p2.position.x);
            active_p = dist_p1 < dist_p2 ? &p1 : &p2;
            playerX = active_p->position.x;
        }
        isColliding = checkPlayerEnemyCollision(active_p, &enemy2);
        printf("PlayerX=%d, isColliding=%d\n", playerX, isColliding);
        updateEnnemi2State(&enemy2, playerX, isColliding); // Level 2: Use new function
        move_ennemi(&enemy2, level, &active_p->position);
        animateEnnemi(&enemy2);
    }

        // Manage boss.png animation and sound



// In the game loop, replace the boss animation block:
if (level == 1 && !enemy.isAlive && playerX >= 2100 && !bossAnimActive && !enigmaTriggered) {
    bossAnimActive = 1;
    bossAnimStartTime = t_now;
    bossPosition.x = SCREEN_WIDTH;
    if (boss_sound) Mix_PlayChannel(-1, boss_sound, 0);
    if (!nuage_sound_played && boss_sound) {
        Mix_PlayChannel(-1, boss_sound, 0);
        nuage_sound_played = 1;
        printf("Playing nuage.wav for boss appearance\n");
    }
    printf("Starting boss.png animation: playerX=%d\n", playerX);
}
if (bossAnimActive && !enigmaTriggered) {
    Uint32 elapsed = t_now - bossAnimStartTime;
    if (elapsed < 2000) {
        float t = elapsed / 2000.0f;
        bossPosition.x = SCREEN_WIDTH - (SCREEN_WIDTH - 50) * t;
        printf("Animating boss.png: x=%d\n", bossPosition.x);
    } else {
        bossPosition.x = 50;
        printf("Boss.png fixed at x=50, triggering enigma\n");
        enigmaTriggered = 1;
        if (font && screen) {
    int enigmaResult = run_enigma(&enigma, screen, font);
    if (enigmaResult) {
        enemy2.health -= 50;
        active_p->score += 100;
        score += 100;
        enigmaSolved = 1;


        
    } else {
        active_p->vie -= 20;
        if (active_p->vie < 0) active_p->vie = 0;
        enigmaSolved = 0;
        printf("Enigma failed! Player health=%d, enigmaSolved=%d\n", active_p->vie, enigmaSolved);
    }
    bossAnimActive = 0;
} else {
            printf("Error: Font or screen is null, skipping enigma\n");
            bossAnimActive = 0;
        }
    }
}
// Animate zeus.png when enemy2 dies
// Animate zeus.png when enemy2 dies
if (show_zeus_image && zeus_image != NULL) {
    Uint32 elapsed = t_now - zeus_anim_start_time;
    const Uint32 anim_duration = 2000; // 2 seconds
    if (elapsed < anim_duration) {
        float t = elapsed / (float)anim_duration;
        // Linear interpolation from SCREEN_HEIGHT to 40
        zeusPosition.y = SCREEN_HEIGHT - (SCREEN_HEIGHT - 40) * t;
        printf("Animating zeus.png: y=%d\n", zeusPosition.y);
    } else {
        zeusPosition.y = 40; // Fix at y=40
        printf("zeus.png fixed at y=%d\n", zeusPosition.y);
        // Stop displaying after additional 2 seconds
        if (elapsed >= anim_duration + 2000) {
            show_zeus_image = 0;
            runMazeGame = 1; // Trigger maze game
            printf("Fin affichage zeus.png, launching maze game\n");
        }
    }
}
// Run maze game if triggered
if (runMazeGame) {
    // Save current screen and set new video mode for maze game
    SDL_Surface *originalScreen = screen;
    screen = SDL_SetVideoMode(1000, 609, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to set maze game video mode: %s\n", SDL_GetError());
        screen = originalScreen; // Restore original screen
        runMazeGame = 0;
        continue;
    }

    // Initialize maze game
    initGame(&mazeGame, screen);
    mazeGame.running = 1;

    // Maze game loop
    while (mazeGame.running) {
        SDL_Event mazeEvent;
        while (SDL_PollEvent(&mazeEvent)) {
            switch (mazeEvent.type) {
                case SDL_QUIT:
                    mazeGame.running = 0;
                    running = 0; // Exit main game if maze game is quit
                    break;
                case SDL_KEYDOWN:
                    {
                        int new_x = mazeGame.player.x;
                        int new_y = mazeGame.player.y;
                        switch (mazeEvent.key.keysym.sym) {
                            case SDLK_UP: new_y--; break;
                            case SDLK_DOWN: new_y++; break;
                            case SDLK_LEFT: new_x--; break;
                            case SDLK_RIGHT: new_x++; break;
                            case SDLK_ESCAPE:
                                mazeGame.running = 0;
                                break;
                            default: break;
                        }
                        if (isMoveValid(&mazeGame, new_x, new_y)) {
                            mazeGame.player.x = new_x;
                            mazeGame.player.y = new_y;
                            // Check for exit (red tile)
                            if (mazeGame.maze.grid[new_y][new_x] == 2) {
                                drawResult(&mazeGame, mazeGame.resources.successImage);
                                mazeGame.running = 0;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        // Check time limit
        Uint32 elapsed_time = SDL_GetTicks() - mazeGame.startTime;
        if (elapsed_time >= 60000) {
            drawResult(&mazeGame, mazeGame.resources.failureImage);
            mazeGame.running = 0;
        }

        // Render maze game
        drawBackground(&mazeGame);
        drawMap(&mazeGame);
        drawMaze(&mazeGame);
        drawPlayer(&mazeGame);
        drawClock(&mazeGame);
        SDL_Flip(screen);

        SDL_Delay(1000 / FPS); // Maintain 60 FPS
    }

    // Cleanup maze game
    cleanupGame(&mazeGame);

    // Restore original video mode
    screen = originalScreen;
    SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to restore main game video mode: %s\n", SDL_GetError());
        running = 0;
    }

    runMazeGame = 0; // Reset flag
    printf("Maze game ended, resuming main game\n");
}

        // Check potion and treasure collisions
        if (game_mode == 1) {
    // Player 1 collisions
    if (level == 1 && checkPlayerESCollision(&p1, &secondaryEntity)) {
    if (t_now - lastESDamageTime >= 1000) {
        p1.vie -= 10;
        if (p1.vie < 0) p1.vie = 0;
        lastESDamageTime = t_now;
        if (poison_sound) {
            Mix_PlayChannel(-1, poison_sound, 0);
            printf("Playing poison.wav for P1/ES1 collision\n");
        }
        printf("Collision P1/ES1 ! P1 Health=%d\n", p1.vie);
    }
}
    if (checkPlayerES2Collision(&p1, &secondaryEntity)) {
    p1.vie += 10;
    p1.score += 50;
    score += 50;
    if (p1.vie > 100) p1.vie = 100;
    secondaryEntity.showPotion2 = 0;
    es2Collected = 1;
    last_potion_reset_time = t_now;
    if (health_sound) {
        Mix_PlayChannel(-1, health_sound, 0);
        printf("Playing health.wav for P1/ES2 collision\n");
    }
    printf("Collision P1/ES2 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES2 disparu\n", p1.vie, p1.score, score);
}
    if (level == 1) {
        if (checkPlayerES3Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p1.vie += 3;
    p1.score += 50;
    score += 50;
    if (p1.vie > 100) p1.vie = 100;
    secondaryEntity.showPotion3 = 0;
    lastES3CollectTime = t_now;
    if (pts_sound) {
        Mix_PlayChannel(-1, pts_sound, 0);
        printf("Playing pts.wav for P1/ES3 collision\n");
    }
    printf("Collision P1/ES3 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES3 disparu\n", p1.vie, p1.score, score);
}
if (checkPlayerES3_3Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p1.vie += 3;
    p1.score += 50;
    score += 50;
    if (p1.vie > 100) p1.vie = 100;
    secondaryEntity.showPotion3_3 = 0;
    lastES3CollectTime = t_now;
    if (pts_sound) {
        Mix_PlayChannel(-1, pts_sound, 0);
        printf("Playing pts.wav for P1/ES3_3 collision\n");
    }
    printf("Collision P1/ES3_3 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES3_3 disparu\n", p1.vie, p1.score, score);
}
    } else if (level == 2) {
        if (checkPlayerES12Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p1.vie -= 5;
    p1.score -= 50;
    score -= 50;
    if (p1.vie < 0) p1.vie = 0;
    secondaryEntity.showPotion12 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for P1/ES12 collision\n");
    }
    printf("Collision P1/ES12 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES12 disparu\n", p1.vie, p1.score, score);
}
        if (checkPlayerES12_2Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p1.vie -= 5;
    p1.score -= 50;
    score -= 50;
    if (p1.vie < 0) p1.vie = 0;
    secondaryEntity.showPotion12_2 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for P1/ES12_2 collision\n");
    }
    printf("Collision P1/ES12_2 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES12_2 disparu\n", p1.vie, p1.score, score);
}
        if (checkPlayerES12_3Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p1.vie -= 5;
    p1.score -= 50;
    score -= 50;
    if (p1.vie < 0) p1.vie = 0;
    secondaryEntity.showPotion12_3 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for P1/ES12_3 collision\n");
    }
    printf("Collision P1/ES12_3 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES12_3 disparu\n", p1.vie, p1.score, score);
}
        if (secondaryEntity.showPotion14 && checkPlayerES14Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            p1.vie += 5;
            p1.score += 50;
            score += 50;
            if (p1.vie > 100) p1.vie = 100;
            secondaryEntity.showPotion14 = 0;
            lastES3CollectTime = t_now;
            printf("Collision P1/ES14 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES14 disparu\n", p1.vie, p1.score, score);
        }
        if (secondaryEntity.showPotion14_2 && checkPlayerES14_2Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            p1.vie += 5;
            p1.score += 50;
            score += 50;
            if (p1.vie > 100) p1.vie = 100;
            secondaryEntity.showPotion14_2 = 0;
            lastES3CollectTime = t_now;
            printf("Collision P1/ES14_2 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES14_2 disparu\n", p1.vie, p1.score, score);
        }
        if (secondaryEntity.showPotion14_3 && checkPlayerES14_3Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            p1.vie += 5;
            p1.score += 50;
            score += 50;
            if (p1.vie > 100) p1.vie = 100;
            secondaryEntity.showPotion14_3 = 0;
            lastES3CollectTime = t_now;
            printf("Collision P1/ES14_3 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES14_3 disparu\n", p1.vie, p1.score, score);
        }
        if (checkPlayerES11Collision(&p1, &secondaryEntity) && (t_now - lastES11CollectTime >= 1000)) {
        show_romlet_image = 1;
        romlet_image_start_time = t_now;
        lastES11CollectTime = t_now;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for P1/ES11 collision\n");
        }
        printf("Collision P1/ES11 (closet.png) ! Showing romlet.png\n");
    }
    if (secondaryEntity.showPotion16 && checkPlayerES16Collision(&p1, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
        secondaryEntity.showPotion16 = 0;
        secondaryEntity.showPotion17 = 1; // Show imagep17
        potion17_image_start_time = t_now; // Start timer
        p1.score += 20;
        score += 20;
        lastES3CollectTime = t_now;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for P1/ES16 collision\n");
        }
        printf("Collision P1/ES16 (imagep16.png) ! P1 Score=%d, Total Score=%d, imagep16.png disparu, showing imagep17\n", p1.score, score);
    }
    }
    if (checkPlayerES6Collision(&p1, &secondaryEntity) && (t_now - lastES6CollectTime >= 1000)) {
        p1.vie -= 5;
        p1.score -= 10;
        score -= 10;
        if (p1.vie < 0) p1.vie = 0;
        secondaryEntity.showPotion6 = 0;
        es6Collected = 1;
        lastES6CollectTime = t_now;
        last_potion_reset_time = t_now;
        printf("Collision P1/ES6 ! P1 Health=%d, P1 Score=%d, Total Score=%d, ES6 disparu\n", p1.vie, p1.score, score);
    }
    if (checkPlayerES9Collision(&p1, &secondaryEntity) && (t_now - lastES9CollectTime >= 1000)) {
    if (enigmaSolved) {
        show_key_image = 1;
        key_image_start_time = t_now;
        lastES9CollectTime = t_now;
        p1.score += 20;
        score += 20;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for P1/ES9 collision with cle.png display\n");
        }
        printf("Collision P1/ES9 (mommy.png) après énigme résolue ! Showing cle.png, relic_s.png delayed by 1s, P1 Score=%d, Total Score=%d\n", p1.score, score);
    } else {
        lastES9CollectTime = t_now;
        printf("Collision P1/ES9 (mommy.png), mais énigme non résolue. Pas d'affichage de cle.png ou relic_s.png\n");
    }
}
    if (check_collision_with_treasure(&p1, &secondaryEntity)) {
    if (t_now - lastTreasureCollectTime >= 3000) {
        secondaryEntity.treasureAnimStartTime = t_now;
        secondaryEntity.treasureFrame = 0;
        lastTreasureCollectTime = t_now;
        if (tresor_sound) {
            Mix_PlayChannel(-1, tresor_sound, 0);
            printf("Playing tresor.wav for P1/Treasure collision\n");
        }
        printf("Collision P1/Treasure ! Starting treasure image sequence\n");
    }
    if (!treasureCollected) {
        treasureCollected = 1;
        p1.score += 5;
        score += 5;
        printf("Collision P1/Treasure ! P1 Score=%d, Total Score=%d\n", p1.score, score);
    }
}

    // Player 2 collisions
    if (level == 1 && checkPlayerESCollision(&p2, &secondaryEntity)) {
    if (t_now - lastESDamageTime >= 1000) {
        p2.vie -= 10;
        if (p2.vie < 0) p2.vie = 0;
        lastESDamageTime = t_now;
        if (poison_sound) {
            Mix_PlayChannel(-1, poison_sound, 0);
            printf("Playing poison.wav for P2/ES1 collision\n");
        }
        printf("Collision P2/ES1 ! P2 Health=%d\n", p2.vie);
    }
}
    if (checkPlayerES2Collision(&p2, &secondaryEntity)) {
    p2.vie += 10;
    p2.score += 50;
    score += 50;
    if (p2.vie > 100) p2.vie = 100;
    secondaryEntity.showPotion2 = 0;
    es2Collected = 1;
    last_potion_reset_time = t_now;
    if (health_sound) {
        Mix_PlayChannel(-1, health_sound, 0);
        printf("Playing health.wav for P2/ES2 collision\n");
    }
    printf("Collision P2/ES2 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES2 disparu\n", p2.vie, p2.score, score);
}
    if (level == 1) {
        if (checkPlayerES3Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p2.vie += 3;
    p2.score += 50;
    score += 50;
    if (p2.vie > 100) p2.vie = 100;
    secondaryEntity.showPotion3 = 0;
    lastES3CollectTime = t_now;
    if (pts_sound) {
        Mix_PlayChannel(-1, pts_sound, 0);
        printf("Playing pts.wav for P2/ES3 collision\n");
    }
    printf("Collision P2/ES3 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES3 disparu\n", p2.vie, p2.score, score);
}
if (checkPlayerES3_3Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p2.vie += 3;
    p2.score += 50;
    score += 50;
    if (p2.vie > 100) p2.vie = 100;
    secondaryEntity.showPotion3_3 = 0;
    lastES3CollectTime = t_now;
    if (pts_sound) {
        Mix_PlayChannel(-1, pts_sound, 0);
        printf("Playing pts.wav for P2/ES3_3 collision\n");
    }
    printf("Collision P2/ES3_3 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES3_3 disparu\n", p2.vie, p2.score, score);
}
    } else if (level == 2) {
        if (checkPlayerES12Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p2.vie -= 5;
    p2.score -= 50;
    score -= 50;
    if (p2.vie < 0) p2.vie = 0;
    secondaryEntity.showPotion12 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for P2/ES12 collision\n");
    }
    printf("Collision P2/ES12 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES12 disparu\n", p2.vie, p2.score, score);
}
        if (checkPlayerES12_2Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p2.vie -= 5;
    p2.score -= 50;
    score -= 50;
    if (p2.vie < 0) p2.vie = 0;
    secondaryEntity.showPotion12_2 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for P2/ES12_2 collision\n");
    }
    printf("Collision P2/ES12_2 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES12_2 disparu\n", p2.vie, p2.score, score);
}
        if (checkPlayerES12_3Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    p2.vie -= 5;
    p2.score -= 50;
    score -= 50;
    if (p2.vie < 0) p2.vie = 0;
    secondaryEntity.showPotion12_3 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for P2/ES12_3 collision\n");
    }
    printf("Collision P2/ES12_3 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES12_3 disparu\n", p2.vie, p2.score, score);
}
        if (secondaryEntity.showPotion14 && checkPlayerES14Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            p2.vie += 5;
            p2.score += 50;
            score += 50;
            if (p2.vie > 100) p2.vie = 100;
            secondaryEntity.showPotion14 = 0;
            lastES3CollectTime = t_now;
            printf("Collision P2/ES14 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES14 disparu\n", p2.vie, p2.score, score);
        }
        if (secondaryEntity.showPotion14_2 && checkPlayerES14_2Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            p2.vie += 5;
            p2.score += 50;
            score += 50;
            if (p2.vie > 100) p2.vie = 100;
            secondaryEntity.showPotion14_2 = 0;
            lastES3CollectTime = t_now;
            printf("Collision P2/ES14_2 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES14_2 disparu\n", p2.vie, p2.score, score);
        }
        if (secondaryEntity.showPotion14_3 && checkPlayerES14_3Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            p2.vie += 5;
            p2.score += 50;
            score += 50;
            if (p2.vie > 100) p2.vie = 100;
            secondaryEntity.showPotion14_3 = 0;
            lastES3CollectTime = t_now;
            printf("Collision P2/ES14_3 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES14_3 disparu\n", p2.vie, p2.score, score);
        }
        if (checkPlayerES11Collision(&p2, &secondaryEntity) && (t_now - lastES11CollectTime >= 1000)) {
        show_romlet_image = 1;
        romlet_image_start_time = t_now;
        lastES11CollectTime = t_now;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for P2/ES11 collision\n");
        }
        printf("Collision P2/ES11 (closet.png) ! Showing romlet.png\n");
    }
    if (secondaryEntity.showPotion16 && checkPlayerES16Collision(&p2, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
        secondaryEntity.showPotion16 = 0;
        secondaryEntity.showPotion17 = 1; // Show imagep17
        potion17_image_start_time = t_now; // Start timer
        p2.score += 20;
        score += 20;
        lastES3CollectTime = t_now;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for P2/ES16 collision\n");
        }
        printf("Collision P2/ES16 (imagep16.png) ! P2 Score=%d, Total Score=%d, imagep16.png disparu, showing imagep17\n", p2.score, score);
    }
    }
    if (checkPlayerES6Collision(&p2, &secondaryEntity) && (t_now - lastES6CollectTime >= 1000)) {
        p2.vie -= 5;
        p2.score -= 10;
        score -= 10;
        if (p2.vie < 0) p2.vie = 0;
        secondaryEntity.showPotion6 = 0;
        es6Collected = 1;
        lastES6CollectTime = t_now;
        last_potion_reset_time = t_now;
        printf("Collision P2/ES6 ! P2 Health=%d, P2 Score=%d, Total Score=%d, ES6 disparu\n", p2.vie, p2.score, score);
    }
    if (checkPlayerES9Collision(&p2, &secondaryEntity) && (t_now - lastES9CollectTime >= 1000)) {
    if (enigmaSolved) {
        show_key_image = 1;
        key_image_start_time = t_now;
        lastES9CollectTime = t_now;
        p2.score += 20;
        score += 20;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for P2/ES9 collision with cle.png display\n");
        }
        printf("Collision P2/ES9 (mommy.png) après énigme résolue ! Showing cle.png, relic_s.png delayed by 1s, P2 Score=%d, Total Score=%d\n", p2.score, score);
    } else {
        lastES9CollectTime = t_now;
        printf("Collision P2/ES9 (mommy.png), mais énigme non résolue. Pas d'affichage de cle.png ou relic_s.png\n");
    }
}
    if (check_collision_with_treasure(&p2, &secondaryEntity)) {
    if (t_now - lastTreasureCollectTime >= 3000) {
        secondaryEntity.treasureAnimStartTime = t_now;
        secondaryEntity.treasureFrame = 0;
        lastTreasureCollectTime = t_now;
        if (tresor_sound) {
            Mix_PlayChannel(-1, tresor_sound, 0);
            printf("Playing tresor.wav for P2/Treasure collision\n");
        }
        printf("Collision P2/Treasure ! Starting treasure image sequence\n");
    }
    if (!treasureCollected) {
        treasureCollected = 1;
        p2.score += 5;
        score += 5;
        printf("Collision P2/Treasure ! P2 Score=%d, Total Score=%d\n", p2.score, score);
    }
}
} else {
        // Solo mode collisions
        if (level == 1 && checkPlayerESCollision(active_p, &secondaryEntity)) {
    if (t_now - lastESDamageTime >= 1000) {
        active_p->vie -= 10;
        if (active_p->vie < 0) active_p->vie = 0;
        lastESDamageTime = t_now;
        if (poison_sound) {
            Mix_PlayChannel(-1, poison_sound, 0);
            printf("Playing poison.wav for joueur/ES1 collision\n");
        }
        printf("Collision joueur/ES1 ! Player Health=%d\n", active_p->vie);
    }
}
        if (checkPlayerES2Collision(active_p, &secondaryEntity)) {
    active_p->vie += 10;
    active_p->score += 50;
    score += 50;
    if (active_p->vie > 100) active_p->vie = 100;
    secondaryEntity.showPotion2 = 0;
    es2Collected = 1;
    last_potion_reset_time = t_now;
    if (health_sound) {
        Mix_PlayChannel(-1, health_sound, 0);
        printf("Playing health.wav for ES2 collision\n");
    }
    printf("Collision joueur/ES2 ! Player Health=%d, Player Score=%d, Total Score=%d, ES2 disparu\n", active_p->vie, active_p->score, score);
}
        if (level == 1) {
            if (checkPlayerES3Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    active_p->vie += 3;
    active_p->score += 50;
    score += 50;
    if (active_p->vie > 100) active_p->vie = 100;
    secondaryEntity.showPotion3 = 0;
    lastES3CollectTime = t_now;
    if (pts_sound) {
        Mix_PlayChannel(-1, pts_sound, 0);
        printf("Playing pts.wav for ES3 collision\n");
    }
    printf("Collision joueur/ES3 ! Player Health=%d, Player Score=%d, Total Score=%d, ES3 disparu\n", active_p->vie, active_p->score, score);
}
if (checkPlayerES3_3Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    active_p->vie += 3;
    active_p->score += 50;
    score += 50;
    if (active_p->vie > 100) active_p->vie = 100;
    secondaryEntity.showPotion3_3 = 0;
    lastES3CollectTime = t_now;
    if (pts_sound) {
        Mix_PlayChannel(-1, pts_sound, 0);
        printf("Playing pts.wav for ES3_3 collision\n");
    }
    printf("Collision joueur/ES3_3 ! Player Health=%d, Player Score=%d, Total Score=%d, ES3_3 disparu\n", active_p->vie, active_p->score, score);
}
        } else if (level == 2) {
            if (checkPlayerES12Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    active_p->vie -= 5;
    active_p->score -= 50;
    score -= 50;
    if (active_p->vie < 0) active_p->vie = 0;
    secondaryEntity.showPotion12 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for joueur/ES12 collision\n");
    }
    printf("Collision joueur/ES12 ! Player Health=%d, Player Score=%d, Total Score=%d, ES12 disparu\n", active_p->vie, active_p->score, score);
}
            if (checkPlayerES12_2Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    active_p->vie -= 5;
    active_p->score -= 50;
    score -= 50;
    if (active_p->vie < 0) active_p->vie = 0;
    secondaryEntity.showPotion12_2 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for joueur/ES12_2 collision\n");
    }
    printf("Collision joueur/ES12_2 ! Player Health=%d, Player Score=%d, Total Score=%d, ES12_2 disparu\n", active_p->vie, active_p->score, score);
}
            if (checkPlayerES12_3Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
    active_p->vie -= 5;
    active_p->score -= 50;
    score -= 50;
    if (active_p->vie < 0) active_p->vie = 0;
    secondaryEntity.showPotion12_3 = 0;
    lastES3CollectTime = t_now;
    if (poison_sound) {
        Mix_PlayChannel(-1, poison_sound, 0);
        printf("Playing poison.wav for joueur/ES12_3 collision\n");
    }
    printf("Collision joueur/ES12_3 ! Player Health=%d, Player Score=%d, Total Score=%d, ES12_3 disparu\n", active_p->vie, active_p->score, score);
}
            if (secondaryEntity.showPotion14 && checkPlayerES14Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            active_p->vie += 5;
            active_p->score += 50;
            score += 50;
            if (active_p->vie > 100) active_p->vie = 100;
            secondaryEntity.showPotion14 = 0;
            lastES3CollectTime = t_now;
            printf("Collision joueur/ES14 ! Player Health=%d, Player Score=%d, Total Score=%d, ES14 disparu\n", active_p->vie, active_p->score, score);
        }
        if (secondaryEntity.showPotion14_2 && checkPlayerES14_2Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            active_p->vie += 5;
            active_p->score += 50;
            score += 50;
            if (active_p->vie > 100) active_p->vie = 100;
            secondaryEntity.showPotion14_2 = 0;
            lastES3CollectTime = t_now;
            printf("Collision joueur/ES14_2 ! Player Health=%d, Player Score=%d, Total Score=%d, ES14_2 disparu\n", active_p->vie, active_p->score, score);
        }
        if (secondaryEntity.showPotion14_3 && checkPlayerES14_3Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
            active_p->vie += 5;
            active_p->score += 50;
            score += 50;
            if (active_p->vie > 100) active_p->vie = 100;
            secondaryEntity.showPotion14_3 = 0;
            lastES3CollectTime = t_now;
            printf("Collision joueur/ES14_3 ! Player Health=%d, Player Score=%d, Total Score=%d, ES14_3 disparu\n", active_p->vie, active_p->score, score);
        }
            if (checkPlayerES11Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
        show_romlet_image = 1;
        romlet_image_start_time = t_now;
        lastES11CollectTime = t_now;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for joueur/ES11 collision\n");
        }
        printf("Collision joueur/ES11 (closet.png) ! Showing romlet.png\n");
    }
    if (secondaryEntity.showPotion16 && checkPlayerES16Collision(active_p, &secondaryEntity) && (t_now - lastES3CollectTime >= 1000)) {
        secondaryEntity.showPotion16 = 0;
        secondaryEntity.showPotion17 = 1; // Show imagep17
        potion17_image_start_time = t_now; // Start timer
        active_p->score += 20;
        score += 20;
        lastES3CollectTime = t_now;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for joueur/ES16 collision\n");
        }
        printf("Collision joueur/ES16 (imagep16.png) ! Player Score=%d, Total Score=%d, imagep16.png disparu, showing imagep17\n", active_p->score, score);
    }
        }
        if (checkPlayerES6Collision(active_p, &secondaryEntity) && (t_now - lastES6CollectTime >= 1000)) {
            active_p->vie -= 5;
            active_p->score -= 10;
            score -= 10;
            if (active_p->vie < 0) active_p->vie = 0;
            secondaryEntity.showPotion6 = 0;
            es6Collected = 1;
            lastES6CollectTime = t_now;
            last_potion_reset_time = t_now;
            printf("Collision joueur/ES6 ! Player Health=%d, Player Score=%d, Total Score=%d, ES6 disparu\n", active_p->vie, active_p->score, score);
        }
        if (checkPlayerES9Collision(active_p, &secondaryEntity) && (t_now - lastES9CollectTime >= 1000)) {
    if (enigmaSolved) {
        show_key_image = 1;
        key_image_start_time = t_now;
        lastES9CollectTime = t_now;
        active_p->score += 20;
        score += 20;
        if (doom_sound) {
            Mix_PlayChannel(-1, doom_sound, 0);
            printf("Playing doom.wav for joueur/ES9 collision with cle.png display\n");
        }
        printf("Collision joueur/ES9 (mommy.png) après énigme résolue ! Showing cle.png, relic_s.png delayed by 1s, Player Score=%d, Total Score=%d\n", active_p->score, score);
    } else {
        lastES9CollectTime = t_now;
        printf("Collision joueur/ES9 (mommy.png), mais énigme non résolue. Pas d'affichage de cle.png ou relic_s.png\n");
    }
}
        if (check_collision_with_treasure(active_p, &secondaryEntity)) {
    if (t_now - lastTreasureCollectTime >= 3000) {
        secondaryEntity.treasureAnimStartTime = t_now;
        secondaryEntity.treasureFrame = 0;
        lastTreasureCollectTime = t_now;
        if (tresor_sound) {
            Mix_PlayChannel(-1, tresor_sound, 0);
            printf("Playing tresor.wav for joueur/Treasure collision\n");
        }
        printf("Collision joueur/Treasure ! Starting treasure image sequence\n");
    }
    if (!treasureCollected) {
        treasureCollected = 1;
        active_p->score += 5;
        score += 5;
        printf("Collision joueur/Treasure ! Player Score=%d, Total Score=%d\n", active_p->score, score);
    }
}
            // Enemy collisions
            // Enemy collisions
if (enemy.isAlive && level == 1 && enemy.State == ATTACKING && checkPlayerEnemyCollision(active_p, &enemy)) {
    enemy.isAttacking = 1;
    if (t_now - lastEnemyHealthLossTime >= 200) {
        enemy.health -= 2;
        lastEnemyHealthLossTime = t_now;
        printf("Collision joueur/ennemi1 ! Enemy1 Health=%d\n", enemy.health);
    }
    // Start fight sound if not already playing
    if (!fight_sound_active && fight_sound) {
        fight_sound_channel = Mix_PlayChannel(-1, fight_sound, -1); // Loop indefinitely
        if (fight_sound_channel != -1) {
            fight_sound_active = 1;
            printf("Started looping fight.wav for joueur/ennemi1 collision on channel %d\n", fight_sound_channel);
        } else {
            printf("Failed to play fight.wav: %s\n", Mix_GetError());
        }
    }
    if (!active_p->shield_active && t_now - lastEnemyDamageTime >= 5000) {
        active_p->vie -= 33;
        if (active_p->vie < 0) active_p->vie = 0;
        lastEnemyDamageTime = t_now;
        printf("Collision joueur/ennemi1 ! Player Health=%d (lost 1 heart)\n", active_p->vie);
    } else if (active_p->shield_active) {
        printf("Collision joueur/ennemi1 ! Bouclier actif, Player Health=%d\n", active_p->vie);
    } else {
        printf("Collision joueur/ennemi1 ! Damage skipped due to cooldown (t_now=%u, lastEnemyDamageTime=%u)\n", t_now, lastEnemyDamageTime);
    }
    // Stop sound if enemy is defeated
    if (enemy.health <= 0 && fight_sound_active) {
        if (fight_sound_channel != -1) {
            Mix_HaltChannel(fight_sound_channel);
            printf("Stopped fight.wav for ennemi1 death\n");
        }
        fight_sound_active = 0;
        fight_sound_channel = -1;
    }
} if (enemy2.isAlive && level == 2 && enemy2.State == ATTACKING && checkPlayerEnemyCollision(active_p, &enemy2)) {
    enemy2.isAttacking = 1;
    if (t_now - lastEnemy2HealthLossTime >= 200) {
        enemy2.health -= 2;
        if (enemy2.health < 0) enemy2.health = 0; // Clamp health to 0
        lastEnemy2HealthLossTime = t_now;
        printf("Collision joueur/ennemi2 ! Enemy2 Health=%d\n", enemy2.health);
    }
    // Start fight sound if not already playing
    if (!fight_sound_active && fight_sound) {
        fight_sound_channel = Mix_PlayChannel(-1, fight_sound, -1); // Loop indefinitely
        if (fight_sound_channel != -1) {
            fight_sound_active = 1;
            printf("Started looping fight.wav for joueur/ennemi2 collision on channel %d\n", fight_sound_channel);
        } else {
            printf("Failed to play fight.wav: %s\n", Mix_GetError());
        }
    }
    if (!active_p->shield_active && t_now - lastEnemyDamageTime >= 5000) {
        active_p->vie -= 33;
        if (active_p->vie < 0) active_p->vie = 0;
        lastEnemyDamageTime = t_now;
        printf("Collision joueur/ennemi2 ! Player Health=%d (lost 1 heart)\n", active_p->vie);
    } else if (active_p->shield_active) {
        printf("Collision joueur/ennemi2 ! Bouclier actif, Player Health=%d\n", active_p->vie);
    } else {
        printf("Collision joueur/ennemi2 ! Damage skipped due to cooldown (t_now=%u, lastEnemyDamageTime=%u)\n", t_now, lastEnemyDamageTime);
    }
    // Stop sound and handle enemy death
    if (enemy2.health <= 0) {
        enemy2.isAlive = 0; // Mark enemy as dead to stop rendering
        if (fight_sound_active) {
            if (fight_sound_channel != -1) {
                Mix_HaltChannel(fight_sound_channel);
                printf("Stopped fight.wav for ennemi2 death\n");
            }
            fight_sound_active = 0;
            fight_sound_channel = -1;
        }
        show_zeus_image = 1;
        zeus_anim_start_time = t_now;
        zeusPosition.y = SCREEN_HEIGHT;
        zeusPosition.x = 40;
        printf("Ennemi2 mort, Player Score=%d, Total Score=%d, Starting zeus.png animation\n", active_p->score, score);
    }
} else {
    enemy.isAttacking = 0;
    enemy2.isAttacking = 0;
    printf("No collision, isAttacking set to 0\n");
}
        }

        // Manage score image display after enemy death
     if (enemy.health <= 0 && level == 1 && !show_score_image) {
    enemy.isAlive = 0; // Ensure this is set
    if (score >= 200 && active_p->position.x < BACKGROUND_WIDTH - 170) {
        show_score_image = 1;
        score_image_start_time = t_now;
        printf("Ennemi mort, Score >= 200 (%d), affichage de score.png et lettre.png, atteindre la fin de l'écran pour passer au niveau 2\n", score);
    } else if (score < 200 && active_p->position.x >= BACKGROUND_WIDTH - 170) {
        show_score_image = 1;
        score_image_start_time = t_now;
        printf("Ennemi mort, Score < 200 (%d), fin de l'écran atteinte, affichage de score.png\n", score);
    }
}
        if (show_score_image && (t_now - score_image_start_time >= 2000)) {
            show_score_image = 0;
            if (score >= 200) {
                images_shown = 1;
                printf("Fin affichage score.png et lettre.png, atteindre la fin de l'écran pour passer au niveau 2\n");
            } else {
                printf("Fin affichage score.png, score=%d, reste au niveau 1\n", score);
            }
        }
        // Check win condition: both imagep10 (relic_s.png) and imagep17 (relic2_s.png) shown in level 1
if (show_relic_image && secondaryEntity.showPotion17 && !show_win_image && !show_lose_image) {
    show_win_image = 1;
    win_image_start_time = t_now;
    printf("Win condition met: imagep10 and imagep17 shown, displaying win.png\n");
}
if (!show_win_image && !show_lose_image) {
    if (game_mode == 0) {
        if (active_p->vie <= 0) {
            show_lose_image = 1;
            lose_image_start_time = t_now;
            printf("Lose condition met: Player health <= 0, displaying lose.png\n");
        }
    } else {
        if (p1.vie <= 0 && p2.vie <= 0) {
            show_lose_image = 1;
            lose_image_start_time = t_now;
            printf("Lose condition met: Both players' health <= 0, displaying lose.png\n");
        }
    }
}

// Stop game after displaying win.png or lose.png for 2 seconds
if (show_win_image && (t_now - win_image_start_time >= 10000)) {
    SDL_Delay(100);
    running = 0;
    printf("Win image displayed for 2 seconds, exiting game\n");
}
if (show_lose_image && (t_now - lose_image_start_time >= 10000)) {
    SDL_Delay(100);
    running = 0;
    printf("Lose image displayed for 2 seconds, exiting game\n");
}
        

        // Transition to level 2
if (images_shown && score >= 200 && active_p->position.x >= BACKGROUND_WIDTH - 170 && level == 1) {
    level = 2;
    bg.level = 2; // Switch to level 2 background
    images_shown = 0;
    treasureCollected = 0;
    es6Collected = 0;
    bossAnimActive = 0;
    enigmaTriggered = 0;
    enigmaSolved = 0;
    enemy2.Frame.i = 0;
    enemy2.Frame.j = 0;
    enemy2.Direction = 2;
    enemy2.State = WAITING;
    enemy2.health = 100;
    enemy2.isAlive = 1;
    enemy2.isAttacking = 0;
    secondaryEntity.potionTimer = t_now;
    secondaryEntity.showPotion2 = 0;
    es2Collected = 0;
    secondaryEntity.showPotion6 = 0;
    // Reset door animation for level 2
    bg.door_frame = 0;
    bg.door_anim_stopped = 0;
    bg.door_anim_start_time = t_now; // Reset animation timer
    game_started = 0; // Require door animation to complete
    door2_sound_played = 0; // Reset for level 2 door sound
    egypte_sound_played = 0; // Reset for potential level 1 replay
    nuage_sound_played = 0; // Reset for potential level 1 replay
    // Stop egypte.wav if playing
    if (egypte_sound_channel != -1) {
        Mix_HaltChannel(egypte_sound_channel);
        egypte_sound_channel = -1;
        printf("Stopped egypte.wav during level 2 transition\n");
    }
    if (!rome_sound_played && rome_sound) {
        Mix_PlayChannel(-1, rome_sound, 0);
        rome_sound_played = 1;
        printf("Playing rome.wav for level 2 transition\n");
    }
    // Stop fight sound if playing
    if (fight_sound_active && fight_sound_channel != -1) {
        Mix_HaltChannel(fight_sound_channel);
        fight_sound_active = 0;
        fight_sound_channel = -1;
        printf("Stopped fight.wav during level transition\n");
    }
    if (game_mode == 0) {
        active_p->position.x = 70;
        active_p->position.y = 100;
    } else {
        p1.position.x = 70;
        p2.position.x = 100;
    }
    printf("Fin de l'écran atteinte, transition vers niveau 2 avec background2.png, Score conservé=%d\n", score);
}

        // Render
	display_background(bg, screen);
        if (level == 1) {
            enemy.positionAbsolue.x -= bg.camera.x;
            display_ennemi(enemy, screen);
            enemy.positionAbsolue.x += bg.camera.x;
            secondaryEntity.positionES.x -= bg.camera.x;
            secondaryEntity.positionES.y -= bg.camera.y;
            secondaryEntity.positionES2.x -= bg.camera.x;
            secondaryEntity.positionES2.y -= bg.camera.y;
            secondaryEntity.positionES3.x -= bg.camera.x;
            secondaryEntity.positionES3.y -= bg.camera.y;
            secondaryEntity.positionES3_2.x -= bg.camera.x;
            secondaryEntity.positionES3_2.y -= bg.camera.y;
            secondaryEntity.positionES3_3.x -= bg.camera.x;
            secondaryEntity.positionES3_3.y -= bg.camera.y;
            secondaryEntity.positionES4.x -= bg.camera.x;
            secondaryEntity.positionES4.y -= bg.camera.y;
            secondaryEntity.positionES5.x -= bg.camera.x;
            secondaryEntity.positionES5.y -= bg.camera.y;
            secondaryEntity.positionES6.x -= bg.camera.x;
            secondaryEntity.positionES6.y -= bg.camera.y;
            secondaryEntity.positionES7.x -= bg.camera.x;
            secondaryEntity.positionES7.y -= bg.camera.y;
            secondaryEntity.positionES8.x -= bg.camera.x;
            secondaryEntity.positionES8.y -= bg.camera.y;
            secondaryEntity.positionES9.x -= bg.camera.x;
            secondaryEntity.positionES9.y -= bg.camera.y;
            secondaryEntity.positionES10.x -= bg.camera.x;
            secondaryEntity.positionES10.y -= bg.camera.y;
            
            
            display_ES(secondaryEntity, screen, level);
            secondaryEntity.positionES.x += bg.camera.x;
            secondaryEntity.positionES.y += bg.camera.y;
            secondaryEntity.positionES2.x += bg.camera.x;
            secondaryEntity.positionES2.y += bg.camera.y;
            secondaryEntity.positionES3.x += bg.camera.x;
            secondaryEntity.positionES3.y += bg.camera.y;
            secondaryEntity.positionES3_2.x += bg.camera.x;
            secondaryEntity.positionES3_2.y += bg.camera.y;
            secondaryEntity.positionES3_3.x += bg.camera.x;
            secondaryEntity.positionES3_3.y += bg.camera.y;
            secondaryEntity.positionES4.x += bg.camera.x;
            secondaryEntity.positionES4.y += bg.camera.y;
            secondaryEntity.positionES5.x += bg.camera.x;
            secondaryEntity.positionES5.y += bg.camera.y;
            secondaryEntity.positionES6.x += bg.camera.x;
            secondaryEntity.positionES6.y += bg.camera.y;
            secondaryEntity.positionES7.x += bg.camera.x;
            secondaryEntity.positionES7.y += bg.camera.y;
            secondaryEntity.positionES8.x += bg.camera.x;
            secondaryEntity.positionES8.y += bg.camera.y;
            secondaryEntity.positionES9.x += bg.camera.x;
            secondaryEntity.positionES9.y += bg.camera.y;
	    secondaryEntity.positionES10.x += bg.camera.x;
            secondaryEntity.positionES10.y += bg.camera.y;
            
            display_health(enemy, screen);
        } else {
            enemy2.positionAbsolue.x -= bg.camera.x;
            display_ennemi2(enemy2, screen);
            enemy2.positionAbsolue.x += bg.camera.x;
            secondaryEntity.positionES.x -= bg.camera.x;
            secondaryEntity.positionES.y -= bg.camera.y;
            secondaryEntity.positionES2.x -= bg.camera.x;
            secondaryEntity.positionES2.y -= bg.camera.y;
            secondaryEntity.positionES4.x -= bg.camera.x;
            secondaryEntity.positionES4.y -= bg.camera.y;
            secondaryEntity.positionES11.x -= bg.camera.x;
            secondaryEntity.positionES11.y -= bg.camera.y;
            secondaryEntity.positionES12.x -= bg.camera.x;
            secondaryEntity.positionES12.y -= bg.camera.y;
            secondaryEntity.positionES12_2.x -= bg.camera.x;
            secondaryEntity.positionES12_2.y -= bg.camera.y;
            secondaryEntity.positionES12_3.x -= bg.camera.x;
            secondaryEntity.positionES12_3.y -= bg.camera.y;
            secondaryEntity.positionES13.x -= bg.camera.x;
            secondaryEntity.positionES13.y -= bg.camera.y;
            secondaryEntity.positionES14.x -= bg.camera.x;
            secondaryEntity.positionES14.y -= bg.camera.y;
            secondaryEntity.positionES14_2.x -= bg.camera.x;
            secondaryEntity.positionES14_2.y -= bg.camera.y;
            secondaryEntity.positionES14_3.x -= bg.camera.x;
            secondaryEntity.positionES14_3.y -= bg.camera.y;
            secondaryEntity.positionES15.x -= bg.camera.x;
            secondaryEntity.positionES15.y -= bg.camera.y;
            secondaryEntity.positionES16.x -= bg.camera.x;
            secondaryEntity.positionES16.y -= bg.camera.y;
            
            display_ES(secondaryEntity, screen, level);
            secondaryEntity.positionES.x += bg.camera.x;
            secondaryEntity.positionES.y += bg.camera.y;
            secondaryEntity.positionES2.x += bg.camera.x;
            secondaryEntity.positionES2.y += bg.camera.y;
            secondaryEntity.positionES4.x += bg.camera.x;
            secondaryEntity.positionES4.y += bg.camera.y;
            secondaryEntity.positionES11.x += bg.camera.x;
            secondaryEntity.positionES11.y += bg.camera.y;
            secondaryEntity.positionES12.x += bg.camera.x;
            secondaryEntity.positionES12.y += bg.camera.y;
            secondaryEntity.positionES12_2.x += bg.camera.x;
            secondaryEntity.positionES12_2.y += bg.camera.y;
            secondaryEntity.positionES12_3.x += bg.camera.x;
            secondaryEntity.positionES12_3.y += bg.camera.y;
            secondaryEntity.positionES13.x += bg.camera.x;
            secondaryEntity.positionES13.y += bg.camera.y;
            secondaryEntity.positionES14.x += bg.camera.x;
            secondaryEntity.positionES14.y += bg.camera.y;
            secondaryEntity.positionES14_2.x += bg.camera.x;
            secondaryEntity.positionES14_2.y += bg.camera.y;
            secondaryEntity.positionES14_3.x += bg.camera.x;
            secondaryEntity.positionES14_3.y += bg.camera.y;
            secondaryEntity.positionES15.x += bg.camera.x;
            secondaryEntity.positionES15.y += bg.camera.y;
            secondaryEntity.positionES16.x += bg.camera.x;
            secondaryEntity.positionES16.y += bg.camera.y;
            display_health(enemy2, screen);
        }
        if (game_mode == 0) {
            if (game_started) { // Only show player when door is on porte6.png
            active_p->position.x -= bg.camera.x;
            active_p->position.y -= bg.camera.y;
            afficher_personnage(*active_p, screen);
            active_p->position.x += bg.camera.x;
            active_p->position.y += bg.camera.y;
            printf("Door frame 5 (porte6.png), displaying player at x=%d, y=%d\n", active_p->position.x, active_p->position.y);
        }
            display_player_health(*active_p, screen, font);
            display_score(active_p->score, screen, font);
            int lives = active_p->vie / 33 + 1;
            SDL_Rect heart_pos = {SCREEN_WIDTH - 30 * lives, 10, 0, 0};
            for (int i = 0; i < lives && i < 3; i++) {
                SDL_BlitSurface(heart, NULL, screen, &heart_pos);
                heart_pos.x += 30;
            }
        } else {
            if (game_started) { // Only show players when door is on porte6.png
            p1.position.x -= bg.camera.x;
            p1.position.y -= bg.camera.y;
            p2.position.x -= bg.camera.x;
            p2.position.y -= bg.camera.y;
            afficher_personnage(p1, screen);
            afficher_personnage(p2, screen);
            p1.position.x += bg.camera.x;
            p1.position.y += bg.camera.y;
            p2.position.x += bg.camera.x;
            p2.position.y += bg.camera.y;
            printf("Door frame 5 (porte6.png), displaying P1 at x=%d, y=%d, P2 at x=%d, y=%d\n", 
                   p1.position.x, p1.position.y, p2.position.x, p2.position.y);
        }
            display_player_health(p1, screen, font);
            display_player_health(p2, screen, font);
            char score_text[32];
            snprintf(score_text, sizeof(score_text), "P1: %d P2: %d", p1.score, p2.score);
            SDL_Color textColor = {255, 255, 255, 0};
            SDL_Surface *score_surface = TTF_RenderText_Solid(font, score_text, textColor);
            if (score_surface) {
                SDL_Rect score_pos = {10, 40, 0, 0};
                SDL_BlitSurface(score_surface, NULL, screen, &score_pos);
                SDL_FreeSurface(score_surface);
            }
            int lives1 = p1.vie / 33 + 1;
            SDL_Rect heart_pos = {SCREEN_WIDTH - 30 * lives1, 10, 0, 0};
            for (int i = 0; i < lives1 && i < 3; i++) {
                SDL_BlitSurface(heart, NULL, screen, &heart_pos);
                heart_pos.x += 30;
            }
            int lives2 = p2.vie / 33 + 1;
            heart_pos.x = SCREEN_WIDTH - 30 * lives2;
            heart_pos.y = 40;
            for (int i = 0; i < lives2 && i < 3; i++) {
                SDL_BlitSurface(heart, NULL, screen, &heart_pos);
                heart_pos.x += 30;
            }
        }
        /**if (show_score_image) {
            SDL_Rect score_image_pos = {(SCREEN_WIDTH - score_image->w) / 2, (SCREEN_HEIGHT - score_image->h) / 2, 0, 0};
            SDL_BlitSurface(score_image, NULL, screen, &score_image_pos);
            printf("Affichage de score.png à x=%d, y=%d\n", score_image_pos.x, score_image_pos.y);
            if (score >= 200) {
                SDL_Rect letter_image_pos = {190, 20, 0, 0};
                SDL_BlitSurface(letter_image, NULL, screen, &letter_image_pos);
                printf("Affichage de lettre.png à x=%d, y=%d\n", letter_image_pos.x, letter_image_pos.y);
            }
        }*/
        if (show_hint_image && level==1) {
            SDL_Rect hint_image_pos = {(SCREEN_WIDTH - hint_image->w) / 2, (SCREEN_HEIGHT - hint_image->h) / 2, 0, 0};
            SDL_BlitSurface(hint_image, NULL, screen, &hint_image_pos);
            printf("Affichage de hint.png à x=%d, y=%d\n", hint_image_pos.x, hint_image_pos.y);
        }
        // Manage potion17 image display

        if (bossAnimActive && boss_image != NULL) {
            SDL_BlitSurface(boss_image, NULL, screen, &bossPosition);
            printf("Affichage de boss.png à x=%d, y=%d\n", bossPosition.x, bossPosition.y);
        }
        if (level == 1 && !enemy.isAlive && playerX >= 2100 && nuage_image != NULL) {
            SDL_BlitSurface(nuage_image, NULL, screen, &nuagePosition);
            printf("Affichage de nuage.png à x=%d, y=%d\n", nuagePosition.x, nuagePosition.y);
        }
        if (show_key_image) {
        SDL_Rect key_image_pos = {200, 200, 0, 0};
        SDL_BlitSurface(cle_image, NULL, screen, &key_image_pos);
        printf("Affichage de cle.png à x=%d, y=%d\n", key_image_pos.x, key_image_pos.y);
    }
    if (show_relic_image && secondaryEntity.imagep9 != NULL && level == 1) {
        SDL_Rect relic_image_pos = {250, 200, 0, 0};
        printf("Affichage de relic_s.png à x=%d, y=%d\n", relic_image_pos.x, relic_image_pos.y);
    }
    if (show_key_image && !show_relic_image && (t_now - key_image_start_time >= 1000)) {
        show_relic_image = 1;
        relic_image_start_time = t_now;
        printf("1 seconde écoulée, activation de l'affichage de relic_s.png\n");
    }
    if (show_relic_image && secondaryEntity.imagep9 != NULL) {
        SDL_Rect relic_image_pos = {250, 20, 0, 0};
        SDL_BlitSurface(secondaryEntity.imagep10, NULL, screen, &relic_image_pos);
        printf("Affichage de relic_s.png à x=%d, y=%d\n", relic_image_pos.x, relic_image_pos.y);
    }
    if (show_romlet_image && romlet_image != NULL) { // Render romlet.png
        SDL_Rect romlet_image_pos = {250, 250, 0, 0};
        SDL_BlitSurface(romlet_image, NULL, screen, &romlet_image_pos);
        printf("Affichage de romlet.png à x=%d, y=%d\n", romlet_image_pos.x, romlet_image_pos.y);
    }
    if (show_romlet_image && (t_now - romlet_image_start_time >= 2000)) { // Stop romlet.png after 2 seconds
        show_romlet_image = 0;
        printf("Fin affichage romlet.png\n");
    }
    if (show_zeus_image && zeus_image != NULL) {
    SDL_BlitSurface(zeus_image, NULL, screen, &zeusPosition);
    printf("Affichage de zeus.png à x=%d, y=%d\n", zeusPosition.x, zeusPosition.y);
}
// Render win.png
if (show_win_image && secondaryEntity.imagep18 != NULL) {
    SDL_BlitSurface(secondaryEntity.imagep18, NULL, screen, &secondaryEntity.positionES18);
    printf("Affichage de win.png à x=%d, y=%d\n", secondaryEntity.positionES18.x, secondaryEntity.positionES18.y);
}

// Render lose.png
if (show_lose_image && secondaryEntity.imagep19 != NULL) {
    SDL_BlitSurface(secondaryEntity.imagep19, NULL, screen, &secondaryEntity.positionES19);
    printf("Affichage de lose.png à x=%d, y=%d\n", secondaryEntity.positionES19.x, secondaryEntity.positionES19.y);
}

    SDL_Flip(screen);
    }

    // Cleanup
// Cleanup
free_enigme(&enigma);
free_background(&bg);
freeEnnemi(&enemy);
freeEnnemi(&enemy2);
freeEnnemi(&secondaryEntity);

// Free player surfaces
for (int i = 0; i < 8; i++) {
    if (p1.tab_right[i]) { SDL_FreeSurface(p1.tab_right[i]); p1.tab_right[i] = NULL; }
    if (p1.tab_left[i]) { SDL_FreeSurface(p1.tab_left[i]); p1.tab_left[i] = NULL; }
    if (p2.tab_right[i]) { SDL_FreeSurface(p2.tab_right[i]); p2.tab_right[i] = NULL; }
    if (p2.tab_left[i]) { SDL_FreeSurface(p2.tab_left[i]); p2.tab_left[i] = NULL; }
    if (p1.tab_red_right[i]) { SDL_FreeSurface(p1.tab_red_right[i]); p1.tab_red_right[i] = NULL; }
    if (p1.tab_red_left[i]) { SDL_FreeSurface(p1.tab_red_left[i]); p1.tab_red_left[i] = NULL; }
    if (p2.tab_red_right[i]) { SDL_FreeSurface(p2.tab_red_right[i]); p2.tab_red_right[i] = NULL; }
    if (p2.tab_red_left[i]) { SDL_FreeSurface(p2.tab_red_left[i]); p2.tab_red_left[i] = NULL; }
}
for (int i = 0; i < 6; i++) {
    if (p1.tab_attack_right[i]) { SDL_FreeSurface(p1.tab_attack_right[i]); p1.tab_attack_right[i] = NULL; }
    if (p1.tab_attack_left[i]) { SDL_FreeSurface(p1.tab_attack_left[i]); p1.tab_attack_left[i] = NULL; }
    if (p2.tab_attack_right[i]) { SDL_FreeSurface(p2.tab_attack_right[i]); p2.tab_attack_right[i] = NULL; }
    if (p2.tab_attack_left[i]) { SDL_FreeSurface(p2.tab_attack_left[i]); p2.tab_attack_left[i] = NULL; }
    if (p1.tab_red_attack_right[i]) { SDL_FreeSurface(p1.tab_red_attack_right[i]); p1.tab_red_attack_right[i] = NULL; }
    if (p1.tab_red_attack_left[i]) { SDL_FreeSurface(p1.tab_red_attack_left[i]); p1.tab_red_attack_left[i] = NULL; }
    if (p2.tab_red_attack_right[i]) { SDL_FreeSurface(p2.tab_red_attack_right[i]); p2.tab_red_attack_right[i] = NULL; }
    if (p2.tab_red_attack_left[i]) { SDL_FreeSurface(p2.tab_red_attack_left[i]); p2.tab_red_attack_left[i] = NULL; }
}
for (int i = 0; i < 1; i++) {
    if (p1.tab_jump_right[i]) { SDL_FreeSurface(p1.tab_jump_right[i]); p1.tab_jump_right[i] = NULL; }
    if (p1.tab_jump_left[i]) { SDL_FreeSurface(p1.tab_jump_left[i]); p1.tab_jump_left[i] = NULL; }
    if (p2.tab_jump_right[i]) { SDL_FreeSurface(p2.tab_jump_right[i]); p2.tab_jump_right[i] = NULL; }
    if (p2.tab_jump_left[i]) { SDL_FreeSurface(p2.tab_jump_left[i]); p2.tab_jump_left[i] = NULL; }
    if (p1.tab_red_jump_right[i]) { SDL_FreeSurface(p1.tab_red_jump_right[i]); p1.tab_red_jump_right[i] = NULL; }
    if (p1.tab_red_jump_left[i]) { SDL_FreeSurface(p1.tab_red_jump_left[i]); p1.tab_red_jump_left[i] = NULL; }
    if (p2.tab_red_jump_right[i]) { SDL_FreeSurface(p2.tab_red_jump_right[i]); p2.tab_red_jump_right[i] = NULL; }
    if (p2.tab_red_jump_left[i]) { SDL_FreeSurface(p2.tab_red_jump_left[i]); p2.tab_red_jump_left[i] = NULL; }
}
if (p1.shield_image) { SDL_FreeSurface(p1.shield_image); p1.shield_image = NULL; }
if (p1.powerup_notification_image) { SDL_FreeSurface(p1.powerup_notification_image); p1.powerup_notification_image = NULL; }
if (p2.shield_image) { SDL_FreeSurface(p2.shield_image); p2.shield_image = NULL; }
if (p2.powerup_notification_image) { SDL_FreeSurface(p2.powerup_notification_image); p2.powerup_notification_image = NULL; }
if (score_image) { SDL_FreeSurface(score_image); score_image = NULL; }
if (boss_image) { SDL_FreeSurface(boss_image); boss_image = NULL; }
if (letter_image) { SDL_FreeSurface(letter_image); letter_image = NULL; }
if (heart) { SDL_FreeSurface(heart); heart = NULL; }
if (hint_image) { SDL_FreeSurface(hint_image); hint_image = NULL; }
if (nuage_image) { SDL_FreeSurface(nuage_image); nuage_image = NULL; }
if (cle_image) { SDL_FreeSurface(cle_image); cle_image = NULL; }
if (boss_sound) { Mix_FreeChunk(boss_sound); boss_sound = NULL; }
if (door_sound) { Mix_FreeChunk(door_sound); door_sound = NULL; } // Free door.wav
if (door2_sound) { Mix_FreeChunk(door2_sound); door2_sound = NULL; } // Free door.wav
if (health_sound) { Mix_FreeChunk(health_sound); health_sound = NULL; } // Free health.wav
if (fight_sound) { Mix_FreeChunk(fight_sound); fight_sound = NULL; } // Free fight.wav
if (pts_sound) { Mix_FreeChunk(pts_sound); pts_sound = NULL; } // Free pts.wav
if (tresor_sound) { Mix_FreeChunk(tresor_sound); tresor_sound = NULL; } // Free tresor.wav
if (poison_sound) { Mix_FreeChunk(poison_sound); poison_sound = NULL; } // Free poison.wav
if (egypte_sound) { Mix_FreeChunk(egypte_sound); egypte_sound = NULL; } // Free egypte.wav
if (boss_sound) { Mix_FreeChunk(boss_sound); boss_sound = NULL; } // Free nuage.wav
if (doom_sound) { Mix_FreeChunk(doom_sound); doom_sound = NULL; } // Free nuage.wav
if (rome_sound) { Mix_FreeChunk(rome_sound); rome_sound = NULL; } // Free rome.wav
// Free font once
if (font) {
    TTF_CloseFont(font);
    font = NULL;
}
TTF_Quit();
Mix_CloseAudio();
SDL_Quit();

return 0;
}
