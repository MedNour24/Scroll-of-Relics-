/**
 * @file personne.h
 * @brief Header file for character management in the SDL game.
 * @author MohamedMaalej
 * @date 2025-05-13
 * @version 1.0
 */
#ifndef PERSONNE_H
#define PERSONNE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h> // Ajouté pour le son
#include "background.h"
#include "ennemie.h"

#define STAT_SOL 0
#define STAT_AIR 1
#define STAT_ATTACK 2

typedef struct {
    SDL_Rect position;
    int frame;
    double vitesse;
    double acceleration;
    int up;
    double vy;
    int status;
    int vie;
    int score;
    int direction;
    int attack_frame;
    int jump_frame;
    SDL_Surface *tab_right[8];
    SDL_Surface *tab_left[8];
    SDL_Surface *tab_attack_right[6];
    SDL_Surface *tab_attack_left[6];
    SDL_Surface *tab_jump_right[1];
    SDL_Surface *tab_jump_left[1];
    SDL_Surface *tab_red_right[8];
    SDL_Surface *tab_red_left[8];
    SDL_Surface *tab_red_attack_right[6];
    SDL_Surface *tab_red_attack_left[6];
    SDL_Surface *tab_red_jump_right[1];
    SDL_Surface *tab_red_jump_left[1];
    int use_red_skin;
    int show_guide;
    SDL_Surface *guide_images[6]; // Images de guide
    Mix_Chunk *guide_sound;       // Son du guide
    int guide_image_index;        // Index de l'image courante
    Uint32 guide_timer;           // Timer pour le séquençage
    int shield_active;
    Uint32 shield_timer;
    SDL_Surface *shield_image;
    int powerup_notification_active;
    Uint32 powerup_notification_timer;
    SDL_Surface *powerup_notification_image;
    int falling;
} personnage;

void initialiser_personnage(personnage *p, int use_red_skin);
void afficher_personnage(personnage p, SDL_Surface *ecran);
void animate_personnage(personnage *p);
void movePerso(personnage *p, Uint32 dt, Background *bg);
void handle_input(SDL_Event event, personnage *p1, personnage *p2, int *running, int active_player, int game_mode, Background *bg);
void activate_shield(personnage *p);
void update_shield(personnage *p);
int check_collision_with_treasure(personnage *p, Ennemi *es);
void liberer_personnage(personnage *p); // Pour libérer les ressources

#endif
