/**
 * @file personne.c
 * @brief Implementation of character management functions for the SDL game.
 * @author MohamedMaalej
 * @date 2025-05-13
 * @version empennage
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "personne.h"
#include "background.h"
#include "ennemie.h" // Added for Ennemi struct

#define SCREEN_WIDTH 1280  // Screen width
#define BACKGROUND_WIDTH 2560  // Full background width

/**
 * @brief Initializes a character with default or red skin.
 * @param p Pointer to the character structure.
 * @param use_red_skin Flag indicating if red skin should be used (0 or 1).
 * @return Nothing.
 */
void initialiser_personnage(personnage *p, int use_red_skin) {
    p->position.x = 0;         
    p->position.y = 682 - 300; // Adjusted based on actual platform y=682
    p->frame = 0;             
    p->vitesse = 0.0;          
    p->acceleration = 0.0;     
    p->up = 0;                 
    p->vy = 0.0;               
    p->status = STAT_SOL;      
    p->vie = 100;                
    p->score = 0;              
    p->direction = 0;          
    p->attack_frame = 0;       
    p->jump_frame = 0;         
    p->show_guide = 0;        
    p->use_red_skin = use_red_skin;
    p->shield_active = 0;
    p->shield_timer = 0;
    p->falling = 0;            
    p->shield_image = IMG_Load("shield.png");
    if (p->shield_image == NULL) {
        fprintf(stderr, "Erreur de chargement de l'image du bouclier: %s\n", IMG_GetError());
    }

    p->powerup_notification_active = 0;
    p->powerup_notification_timer = 0;
    p->powerup_notification_image = IMG_Load("powerup_activated.png");
    if (p->powerup_notification_image == NULL) {
        fprintf(stderr, "Erreur de chargement de l'image de notification: %s\n", IMG_GetError());
    }

    p->guide_images[0] = IMG_Load("guide_player6.png");
    if (p->guide_images[0] == NULL) {
        fprintf(stderr, "Erreur de chargement de l'image du guide: %s\n", IMG_GetError());
    }

    char nomFich[30];
    const char *prefix;
    if (use_red_skin) {
        prefix = "red_";
    } else {
        prefix = "";
    }

    for (int i = 0; i < 8; i++) {
        sprintf(nomFich, "%s%d.png", prefix, i + 1); 
        if (use_red_skin) {
            p->tab_red_right[i] = IMG_Load(nomFich);
            if (p->tab_red_right[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image droite %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_right[i] = NULL;
        } else {
            p->tab_right[i] = IMG_Load(nomFich);
            if (p->tab_right[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image droite %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_red_right[i] = NULL;
        }
    }
   
    for (int i = 0; i < 8; i++) {
        sprintf(nomFich, "%s%d_left.png", prefix, i + 1); 
        if (use_red_skin) {
            p->tab_red_left[i] = IMG_Load(nomFich);
            if (p->tab_red_left[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image gauche %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_left[i] = NULL;
        } else {
            p->tab_left[i] = IMG_Load(nomFich);
            if (p->tab_left[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image gauche %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_red_left[i] = NULL;
        }
    }
    
    for (int i = 0; i < 6; i++) {
        sprintf(nomFich, "%sattack%d.png", prefix, i + 1); 
        if (use_red_skin) {
            p->tab_red_attack_right[i] = IMG_Load(nomFich);
            if (p->tab_red_attack_right[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image d'attaque droite %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_attack_right[i] = NULL;
        } else {
            p->tab_attack_right[i] = IMG_Load(nomFich);
            if (p->tab_attack_right[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image d'attaque droite %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_red_attack_right[i] = NULL;
        }
    }
    
    for (int i = 0; i < 6; i++) {
        sprintf(nomFich, "%sattack%d_left.png", prefix, i + 1); 
        if (use_red_skin) {
            p->tab_red_attack_left[i] = IMG_Load(nomFich);
            if (p->tab_red_attack_left[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image d'attaque gauche %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_attack_left[i] = NULL;
        } else {
            p->tab_attack_left[i] = IMG_Load(nomFich);
            if (p->tab_attack_left[i] == NULL) {
                fprintf(stderr, "Erreur de chargement de l'image d'attaque gauche %s: %s\n", nomFich, IMG_GetError());
            }
            p->tab_red_attack_left[i] = NULL;
        }
    }
    
    sprintf(nomFich, "%sjump1.png", prefix); 
    if (use_red_skin) {
        p->tab_red_jump_right[0] = IMG_Load(nomFich);
        if (p->tab_red_jump_right[0] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image de saut droite %s: %s\n", nomFich, IMG_GetError());
        }
        p->tab_jump_right[0] = NULL;
    } else {
        p->tab_jump_right[0] = IMG_Load(nomFich);
        if (p->tab_jump_right[0] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image de saut droite %s: %s\n", nomFich, IMG_GetError());
        }
        p->tab_red_jump_right[0] = NULL;
    }
    
    sprintf(nomFich, "%sjump1_left.png", prefix); 
    if (use_red_skin) {
        p->tab_red_jump_left[0] = IMG_Load(nomFich);
        if (p->tab_red_jump_left[0] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image de saut gauche %s: %s\n", nomFich, IMG_GetError());
        }
        p->tab_jump_left[0] = NULL;
    } else {
        p->tab_jump_left[0] = IMG_Load(nomFich);
        if (p->tab_jump_left[0] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image de saut gauche %s: %s\n", nomFich, IMG_GetError());
        }
        p->tab_red_jump_left[0] = NULL;
    }
}

/**
 * @brief Frees all resources associated with a character.
 * @param p Pointer to the character structure.
 * @return Nothing.
 */
void liberer_personnage(personnage *p) {
    if (p->shield_image) SDL_FreeSurface(p->shield_image);
    if (p->powerup_notification_image) SDL_FreeSurface(p->powerup_notification_image);
    if (p->guide_images[0]) SDL_FreeSurface(p->guide_images[0]);
    for (int i = 0; i < 8; i++) {
        if (p->tab_right[i]) SDL_FreeSurface(p->tab_right[i]);
        if (p->tab_left[i]) SDL_FreeSurface(p->tab_left[i]);
        if (p->tab_red_right[i]) SDL_FreeSurface(p->tab_red_right[i]);
        if (p->tab_red_left[i]) SDL_FreeSurface(p->tab_red_left[i]);
    }
    for (int i = 0; i < 6; i++) {
        if (p->tab_attack_right[i]) SDL_FreeSurface(p->tab_attack_right[i]);
        if (p->tab_attack_left[i]) SDL_FreeSurface(p->tab_attack_left[i]);
        if (p->tab_red_attack_right[i]) SDL_FreeSurface(p->tab_red_attack_right[i]);
        if (p->tab_red_attack_left[i]) SDL_FreeSurface(p->tab_red_attack_left[i]);
    }
    if (p->tab_jump_right[0]) SDL_FreeSurface(p->tab_jump_right[0]);
    if (p->tab_jump_left[0]) SDL_FreeSurface(p->tab_jump_left[0]);
    if (p->tab_red_jump_right[0]) SDL_FreeSurface(p->tab_red_jump_right[0]);
    if (p->tab_red_jump_left[0]) SDL_FreeSurface(p->tab_red_jump_left[0]);
}

/**
 * @brief Displays a character on the screen.
 * @param p The character structure.
 * @param ecran The SDL surface to render on.
 * @return Nothing.
 */
void afficher_personnage(personnage p, SDL_Surface *ecran) {
    SDL_Surface *current_frame = NULL;

    if (p.status == STAT_ATTACK) {
        if (p.direction >= 0) { 
            if (p.use_red_skin) {
                current_frame = p.tab_red_attack_right[p.attack_frame];
            } else {
                current_frame = p.tab_attack_right[p.attack_frame];
            }
        } else { 
            if (p.use_red_skin) {
                current_frame = p.tab_red_attack_left[p.attack_frame];
            } else {
                current_frame = p.tab_attack_left[p.attack_frame];
            }
        }
    } else if (p.status == STAT_AIR || p.falling) {
        if (p.direction >= 0) {
            if (p.use_red_skin) {
                current_frame = p.tab_red_jump_right[p.jump_frame];
            } else {
                current_frame = p.tab_jump_right[p.jump_frame];
            }
        } else {
            if (p.use_red_skin) {
                current_frame = p.tab_red_jump_left[p.jump_frame];
            } else {
                current_frame = p.tab_jump_left[p.jump_frame];
            }
        }
    } else {
        if (p.direction >= 0) { 
            if (p.use_red_skin) {
                current_frame = p.tab_red_right[p.frame];
            } else {
                current_frame = p.tab_right[p.frame];
            }
        } else { 
            if (p.use_red_skin) {
                current_frame = p.tab_red_left[p.frame];
            } else {
                current_frame = p.tab_left[p.frame];
            }
        }
    }

    if (current_frame) {
        SDL_BlitSurface(current_frame, NULL, ecran, &p.position);
    } else {
        fprintf(stderr, "Aucune surface à afficher pour frame %d, attack_frame %d, jump_frame %d, status %d, direction %d, red_skin %d\n", 
                p.frame, p.attack_frame, p.jump_frame, p.status, p.direction, p.use_red_skin);
    }

    if (p.shield_active && p.shield_image) {
        SDL_BlitSurface(p.shield_image, NULL, ecran, &p.position);
    }

    if (p.powerup_notification_active && p.powerup_notification_image) {
        SDL_Rect notification_pos = {p.position.x + 10, p.position.y - 150, 0, 0};
        SDL_BlitSurface(p.powerup_notification_image, NULL, ecran, &notification_pos);
    }

    if (p.shield_active) {
        Uint32 remaining_time = (p.shield_timer - SDL_GetTicks()) / 1000;
        char timer_text[32];
        sprintf(timer_text, "Shield: %ds", remaining_time);
        SDL_Color white = {255, 255, 255, 0};
        TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
        if (font) {
            SDL_Surface *timer_surface = TTF_RenderText_Solid(font, timer_text, white);
            if (timer_surface) {
                SDL_Rect timer_pos = {p.position.x + 30, p.position.y - 30, 0, 0};
                SDL_BlitSurface(timer_surface, NULL, ecran, &timer_pos);
                SDL_FreeSurface(timer_surface);
            }
            TTF_CloseFont(font);
        }
    }

    if (p.show_guide && p.guide_images[0]) {
        SDL_Rect guide_pos = {180, 20, 0, 0}; // Adjust position as needed
        SDL_BlitSurface(p.guide_images[0], NULL, ecran, &guide_pos);
    }
}

/**
 * @brief Animates a character's walking sprites.
 * @param p Pointer to the character structure.
 * @return Nothing.
 */
void animate_personnage(personnage *p) {
    p->frame = (p->frame + 1) % 8; 
}

/**
 * @brief Moves a character based on input, time delta, and background.
 * @param p Pointer to the character structure.
 * @param dt Time delta in milliseconds.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void movePerso(personnage *p, Uint32 dt, Background *bg) {
    double dt_seconds = dt / 1000.0;
    const double max_speed = 200.0;
    const double sprint_speed = 400.0;
    const double gravity = 500.0;
    const double max_fall_speed = 300.0; // Cap falling speed to prevent tunneling
    static Uint32 last_anim_time = 0;
    const Uint32 anim_interval = 100;

    double current_max_speed = max_speed;
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
        current_max_speed = sprint_speed;
    }

    p->vitesse += p->acceleration * dt_seconds;
    if (p->vitesse > current_max_speed) p->vitesse = current_max_speed;
    if (p->vitesse < -current_max_speed) p->vitesse = -current_max_speed;
    
    p->position.x += p->vitesse * dt_seconds;

    // Keep player within the full background bounds, not the screen width
    if (p->position.x < 0) p->position.x = 0;
    if (p->position.x > BACKGROUND_WIDTH - 170) p->position.x = BACKGROUND_WIDTH - 170; // Use background width

    int is_obstacle = 0;
    int should_fall = 0;
    int platform_y = -1;

    if (p->status == STAT_AIR || p->falling) {
        p->vy += gravity * dt_seconds;
        if (p->vy > max_fall_speed) p->vy = max_fall_speed; // Cap fall speed
        p->position.y += p->vy * dt_seconds;

        // Check collision after moving
        should_fall = check_collision(*bg, p->position.x, p->position.y, 170, 170, &is_obstacle, p->status, p->vy, &platform_y);
        if (!should_fall && !p->falling && p->status != STAT_AIR) {
            // Landed on pink platform
            p->vy = 0.0;
            p->status = STAT_SOL;
            p->up = 0;
            p->jump_frame = 0;
        } else if (!should_fall && platform_y != -1) {
            // Snap to platform
            p->position.y = platform_y - 170; // Place player's feet on platform
            p->vy = 0.0;
            p->status = STAT_SOL;
            p->falling = 0;
            p->up = 0;
            p->jump_frame = 0;
            printf("Snapped to platform at y=%d\n", platform_y);
        }
        // Check if player falls off screen
        if (p->position.y > 754) {
            p->vie--;
            p->position.x = 0;
            p->position.y = 682 - 300; // Reset to initial position (adjusted)
            p->vitesse = 0.0;
            p->vy = 0.0;
            p->status = STAT_SOL;
            p->falling = 0;
            p->up = 0;
            p->jump_frame = 0;
        }
    }

    // Check for collision when on ground
    if (p->status == STAT_SOL && !p->falling) {
        should_fall = check_collision(*bg, p->position.x, p->position.y, 170, 170, &is_obstacle, p->status, p->vy, &platform_y);
        if (should_fall) {
            p->falling = 1;
            p->status = STAT_AIR;
            p->vy = 0.0;
        }
    }

    // Handle obstacle collision (green)
    if (is_obstacle) {
        p->vie--;
        p->position.x = 0;
        p->position.y = 682 - 300; // Reset to initial position (adjusted)
        p->vitesse = 0.0;
        p->vy = 0.0;
        p->status = STAT_SOL;
        p->falling = 0;
        p->up = 0;
        p->jump_frame = 0;
    }

    Uint32 current_time = SDL_GetTicks();
    if (p->status == STAT_ATTACK) {
        if (current_time - last_anim_time >= anim_interval) {
            p->attack_frame++;
            last_anim_time = current_time;
            if (p->attack_frame >= 6) {
                p->attack_frame = 0;
                p->status = STAT_SOL;
            }
        }
    } else if (p->status == STAT_AIR || p->falling) {
        if (current_time - last_anim_time >= anim_interval) {
            p->jump_frame = (p->jump_frame + 1) % 1;
            last_anim_time = current_time;
        }
    } else if (p->acceleration != 0 && current_time - last_anim_time >= anim_interval) {
        animate_personnage(p);
        last_anim_time = current_time;
    } else if (p->acceleration == 0) {
        p->frame = 0;
    }

    printf("Joueur: vitesse=%.2f, x=%d, y=%d, vy=%.2f, accel=%.2f, frame=%d, attack_frame=%d, jump_frame=%d, status=%d, dir=%d, red_skin=%d, falling=%d\n", 
           p->vitesse, p->position.x, p->position.y, p->vy, p->acceleration, p->frame, p->attack_frame, p->jump_frame, p->status, p->direction, p->use_red_skin, p->falling);
}

/**
 * @brief Handles input events for character movement and actions.
 * @param event The SDL event to process.
 * @param p1 Pointer to the first player character.
 * @param p2 Pointer to the second player character.
 * @param running Pointer to the game running flag.
 * @param active_player The active player (1 for player 1, 2 for player 2).
 * @param game_mode The game mode (0 for solo, 1 for multi).
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void handle_input(SDL_Event event, personnage *p1, personnage *p2, int *running, int active_player, int game_mode, Background *bg) {
    const double jump_speed = -440.0;
    const double accel = 400.0;
    const double sprint_accel = 800.0;

    if (event.type == SDL_QUIT) {
        *running = 0;
        return;
    }

    Uint8 *keystate = SDL_GetKeyState(NULL);
    double accel_value = accel;
    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
        accel_value = sprint_accel;
    }

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                if (game_mode == 1 || active_player == 1) {
                    p1->acceleration = accel_value;
                    p1->direction = 1;
                }
                break;
            case SDLK_LEFT:
                if (game_mode == 1 || active_player == 1) {
                    p1->acceleration = -accel_value;
                    p1->direction = -1;
                }
                break;
            case SDLK_UP:
                if ((game_mode == 1 || active_player == 1) && p1->status == STAT_SOL && !p1->falling) {
                    p1->vy = jump_speed;
                    p1->status = STAT_AIR;
                    p1->up = 1;
                    p1->jump_frame = 0;
                }
                break;
            case SDLK_d:
                if (game_mode == 1 || active_player == 2) {
                    p2->acceleration = accel_value;
                    p2->direction = 1;
                }
                break;
            case SDLK_a:
                if (game_mode == 1 || active_player == 2) {
                    p2->acceleration = -accel_value;
                    p2->direction = -1;
                }
                break;
            case SDLK_w:
                if ((game_mode == 1 || active_player == 2) && p2->status == STAT_SOL && !p2->falling) {
                    p2->vy = jump_speed;
                    p2->status = STAT_AIR;
                    p2->up = 1;
                    p2->jump_frame = 0;
                }
                break;
            case SDLK_g:
                if (game_mode == 1) {
                    p1->show_guide = !p1->show_guide;
                    p2->show_guide = !p2->show_guide;
                } else if (active_player == 1) {
                    p1->show_guide = !p1->show_guide;
                } else if (active_player == 2) {
                    p2->show_guide = !p2->show_guide;
                }
                break;
            case SDLK_ESCAPE:
                *running = 0;
                break;
            case SDLK_p:
                if (game_mode == 0 && active_player == 1) {
                    activate_shield(p1);
                } else if (game_mode == 0 && active_player == 2) {
                    activate_shield(p2);
                } else if (game_mode == 1) {
                    activate_shield(p1);
                    activate_shield(p2);
                }
                break;
            default:
                break; // Ignore unhandled keys
        }
    }

    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
            case SDLK_LEFT:
                if (game_mode == 1 || active_player == 1) {
                    p1->acceleration = 0.0;
                    p1->vitesse = 0.0;
                    p1->direction = 0;
                }
                break;
            case SDLK_d:
            case SDLK_a:
                if (game_mode == 1 || active_player == 2) {
                    p2->acceleration = 0.0;
                    p2->vitesse = 0.0;
                    p2->direction = 0;
                }
                break;
            default:
                break; // Ignore unhandled keys
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (game_mode == 1 || active_player == 1) {
            if (p1->status == STAT_SOL && !p1->falling) {
                p1->status = STAT_ATTACK;
                p1->attack_frame = 0;
            }
        }
        if (game_mode == 1 || active_player == 2) {
            if (p2->status == STAT_SOL && !p2->falling) {
                p2->status = STAT_ATTACK;
                p2->attack_frame = 0;
            }
        }
    }
}

/**
 * @brief Activates a character's shield.
 * @param p Pointer to the character structure.
 * @return Nothing.
 */
void activate_shield(personnage *p) {
    if (!p->shield_active) {
        p->shield_active = 1;
        p->shield_timer = SDL_GetTicks() + 15000;

        p->powerup_notification_active = 1;
        p->powerup_notification_timer = SDL_GetTicks() + 3000;
    }
}

/**
 * @brief Updates a character's shield state and timers.
 * @param p Pointer to the character structure.
 * @return Nothing.
 */
void update_shield(personnage *p) {
    if (p->shield_active && SDL_GetTicks() >= p->shield_timer) {
        p->shield_active = 0;
    }

    if (p->powerup_notification_active && SDL_GetTicks() >= p->powerup_notification_timer) {
        p->powerup_notification_active = 0;
    }
}

/**
 * @brief Checks if a player collides with the treasure.
 * @param p Pointer to the character structure.
 * @param es Pointer to the enemy structure containing treasure data.
 * @return 1 if collision detected, 0 otherwise.
 */
int check_collision_with_treasure(personnage *p, Ennemi *es) {
    if (!p || !es) return 0;

    SDL_Rect playerRect = {p->position.x, p->position.y, 170, 170}; // Player size from movePerso
    SDL_Rect treasureRect = es->positionES5;

    if (playerRect.x < treasureRect.x + treasureRect.w &&
        playerRect.x + playerRect.w > treasureRect.x &&
        playerRect.y < treasureRect.y + treasureRect.h &&
        playerRect.y + playerRect.h > treasureRect.y) {
        printf("Collision detected between player at (%d, %d) and treasure at (%d, %d)\n",
               playerRect.x, playerRect.y, treasureRect.x, treasureRect.y);
        return 1;
    }
    return 0;
}
