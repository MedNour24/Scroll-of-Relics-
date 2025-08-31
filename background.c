/**
 * @file background.c
 * @brief Implementation of background and level mechanics for the SDL game.
 * @author MohamedNourMraad
 * @date 2025-05-13
 * @version 1.0
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include "background.h"
#include "personne.h"

#define SCREEN_WIDTH 1280  // New screen width

/**
 * @brief Initializes the background with image, collision map, and door sprites.
 * @param bg Pointer to the background structure.
 * @return 0 on success, -1 on failure.
 */
int init_background(Background *bg) {
    // Initialize level 1 background
    bg->image_level1 = IMG_Load("background1.png"); // Visual overlay for level 1
    if (bg->image_level1 == NULL) {
        fprintf(stderr, "Erreur de chargement de background1.png : %s\n", IMG_GetError());
        return -1;
    }

    bg->collision_map_level1 = IMG_Load("backgroundpinkgreen1.png"); // Collision map for level 1
    if (bg->collision_map_level1 == NULL) {
        fprintf(stderr, "Erreur de chargement de backgroundpinkgreen1.png : %s\n", IMG_GetError());
        SDL_FreeSurface(bg->image_level1);
        return -1;
    }

    // Initialize level 2 background
    bg->image_level2 = IMG_Load("background2.png"); // Visual overlay for level 2
    if (bg->image_level2 == NULL) {
        fprintf(stderr, "Erreur de chargement de background2.png : %s\n", IMG_GetError());
        SDL_FreeSurface(bg->image_level1);
        SDL_FreeSurface(bg->collision_map_level1);
        return -1;
    }

    bg->collision_map_level2 = IMG_Load("backgroundpinkgreen2.png"); // Collision map for level 2
    if (bg->collision_map_level2 == NULL) {
        fprintf(stderr, "Erreur de chargement de backgroundpinkgreen2.png : %s\n", IMG_GetError());
        SDL_FreeSurface(bg->image_level1);
        SDL_FreeSurface(bg->collision_map_level1);
        SDL_FreeSurface(bg->image_level2);
        return -1;
    }

    // Load door sprite sheet images for level 1 (porte1.png to porte6.png)
    char door_filenames_level1[6][12] = {"porte1.png", "porte2.png", "porte3.png", "porte4.png", "porte5.png", "porte6.png"};
    for (int i = 0; i < 6; i++) {
        bg->door_images[i] = IMG_Load(door_filenames_level1[i]);
        if (bg->door_images[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de %s : %s\n", door_filenames_level1[i], IMG_GetError());
            for (int j = 0; j < i; j++) {
                SDL_FreeSurface(bg->door_images[j]);
            }
            SDL_FreeSurface(bg->image_level1);
            SDL_FreeSurface(bg->collision_map_level1);
            SDL_FreeSurface(bg->image_level2);
            SDL_FreeSurface(bg->collision_map_level2);
            return -1;
        }
    }

    // Load door sprite sheet images for level 2 (prt1.png to prt6.png)
    char door_filenames_level2[6][12] = {"prt1.png", "prt2.png", "prt3.png", "prt4.png", "prt5.png", "prt6.png"};
    for (int i = 0; i < 6; i++) {
        bg->door_images_level2[i] = IMG_Load(door_filenames_level2[i]);
        if (bg->door_images_level2[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de %s : %s\n", door_filenames_level2[i], IMG_GetError());
            for (int j = 0; j < i; j++) {
                SDL_FreeSurface(bg->door_images_level2[j]);
            }
            for (int j = 0; j < 6; j++) {
                SDL_FreeSurface(bg->door_images[j]);
            }
            SDL_FreeSurface(bg->image_level1);
            SDL_FreeSurface(bg->collision_map_level1);
            SDL_FreeSurface(bg->image_level2);
            SDL_FreeSurface(bg->collision_map_level2);
            return -1;
        }
    }

    bg->camera.x = 0;
    bg->camera.y = 0;
    bg->camera.w = SCREEN_WIDTH; // Adjusted for new screen width
    bg->camera.h = bg->image_level1->h; // Full height (754)
    bg->platform_y = 712; // Reference, but collision map will override
    bg->level = 1; // Start at level 1

    // Initialize door position and animation for level 1
    bg->door_position.x = 40; // Fixed x-coordinate for level 1
    bg->door_position.y = 390; // Adjust based on platform height, assuming door height ~100px
    bg->door_position.w = 100; // Adjust based on actual sprite size
    bg->door_position.h = 100;

    // Initialize door position for level 2
    bg->door_position_level2.x = 173; // Custom x-coordinate for level 2 (adjust as needed)
    bg->door_position_level2.y = 13; // Custom y-coordinate for level 2 (adjust as needed)
    bg->door_position_level2.w = 100; // Adjust based on actual sprite size
    bg->door_position_level2.h = 100;

    bg->door_frame = 0;
    bg->door_anim_start_time = SDL_GetTicks();
    bg->door_anim_stopped = 0; // Initialize animation stop flag
    bg->door_anim_reverse = 0;

    return 0;
}

/**
 * @brief Displays the background on the screen with scrolling effect.
 * @param bg The background structure.
 * @param screen The SDL surface to render on.
 * @return Nothing.
 */
void display_background(Background bg, SDL_Surface *screen) {
    SDL_Surface *current_image = (bg.level == 1) ? bg.image_level1 : bg.image_level2;
    SDL_Rect src = {bg.camera.x, bg.camera.y, screen->w, screen->h}; // Scroll based on camera
    SDL_Rect dst = {0, 0, screen->w, screen->h};                     // Destination on screen
    SDL_BlitSurface(current_image, &src, screen, &dst);
    display_door(bg, screen); // Display door animation
}

/**
 * @brief Displays the door animation at the specified position.
 * @param bg The background structure.
 * @param screen The SDL surface to render on.
 * @return Nothing.
 */
void display_door(Background bg, SDL_Surface *screen) {
    SDL_Surface *current_door_image = NULL;
    SDL_Rect door_pos;
    
    if (bg.level == 1 && bg.door_frame >= 0 && bg.door_frame < 6) {
        current_door_image = bg.door_images[bg.door_frame];
        door_pos = bg.door_position;
    } else if (bg.level == 2 && bg.door_frame >= 0 && bg.door_frame < 6) {
        current_door_image = bg.door_images_level2[bg.door_frame];
        door_pos = bg.door_position_level2;
    }

    if (current_door_image != NULL) {
        door_pos.x -= bg.camera.x; // Adjust for camera scrolling
        door_pos.y -= bg.camera.y;
        SDL_BlitSurface(current_door_image, NULL, screen, &door_pos);
        printf("Displaying door frame %d for level %d at x=%d, y=%d\n", bg.door_frame + 1, bg.level, door_pos.x, door_pos.y);
    } else {
        printf("Failed to display door: level=%d, frame=%d\n", bg.level, bg.door_frame);
    }
}

/**
 * @brief Updates the door animation frame for level 1.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void update_door_animation(Background *bg) {
    if (bg->door_anim_stopped || bg->level != 1) {
        return; // Skip animation if stopped or not in level 1
    }
    Uint32 t_now = SDL_GetTicks();
    Uint32 elapsed = t_now - bg->door_anim_start_time;
    bg->door_frame = (elapsed / 150) % 6; // 150ms per frame, cycle through 6 frames
}

/**
 * @brief Updates the door animation frame for level 2.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void update_door_animation_level2(Background *bg) {
    if (bg->door_anim_stopped || bg->level != 2) {
        return; // Skip animation if stopped or not in level 2
    }
    Uint32 t_now = SDL_GetTicks();
    Uint32 elapsed = t_now - bg->door_anim_start_time;
    bg->door_frame = (elapsed / 150) % 6; // 150ms per frame, cycle through 6 frames
}

/**
 * @brief Sets the x and y coordinates of the level 2 door.
 * @param bg Pointer to the background structure.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 * @return Nothing.
 */
void set_door_position_level2(Background *bg, int x, int y) {
    bg->door_position_level2.x = x;
    bg->door_position_level2.y = y;
    printf("Level 2 door position set to x=%d, y=%d\n", x, y);
}

/**
 * @brief Updates the camera position based on the player's x-coordinate.
 * @param bg Pointer to the background structure.
 * @param player_x The player's x-coordinate in world space.
 * @param screen The SDL surface to determine screen dimensions.
 * @return Nothing.
 */
void update_camera(Background *bg, int player_x, SDL_Surface *screen) {
    SDL_Surface *current_image = (bg->level == 1) ? bg->image_level1 : bg->image_level2;
    bg->camera.x = player_x - (screen->w / 2);
    if (bg->camera.x < 0) bg->camera.x = 0;
    if (bg->camera.x > current_image->w - screen->w) {
        bg->camera.x = current_image->w - screen->w;
    }
    int platform_top = 682; // Adjusted to match actual platform y-coordinate
    bg->camera.y = platform_top - (screen->h / 2);
    if (bg->camera.y < 0) bg->camera.y = 0;
    if (bg->camera.y > current_image->h - screen->h) {
        bg->camera.y = current_image->h - screen->h;
    }
}

/**
 * @brief Checks if the player is on a walkable surface, over a hole, or hitting an obstacle.
 * @param bg The background structure.
 * @param player_x The player's x-coordinate in world space.
 * @param player_y The player's y-coordinate.
 * @param player_w The player's width in pixels.
 * @param player_h The player's height in pixels.
 * @param is_obstacle Pointer to flag indicating if an obstacle was hit (1 if true).
 * @param status The player's current status (STAT_SOL, STAT_AIR, etc.).
 * @param vy The player's vertical velocity.
 * @param platform_y Pointer to store the detected platform's y-coordinate (for snapping).
 * @return 1 if the player should fall (not on pink), 0 if on platform (pink).
 */
int check_collision(Background bg, int player_x, int player_y, int player_w, int player_h, int *is_obstacle, int status, float vy, int *platform_y) {
    *is_obstacle = 0; // Default: no obstacle
    *platform_y = -1; // Default: no platform detected

    SDL_Surface *current_collision_map = (bg.level == 1) ? bg.collision_map_level1 : bg.collision_map_level2;

    // Ensure coordinates are within bounds
    if (player_x < 0 || player_x + player_w > current_collision_map->w ||
        player_y < 0 || player_y + player_h > current_collision_map->h) {
        return 1; // Out of bounds, fall
    }

    // Check multiple points under the player's feet (left, center, right) over a range of y-coordinates
    int check_y_base = player_y + player_h - 1; // Bottom of player sprite
    int y_range = 5; // Check 5 pixels above and below
    int points[] = {player_x + player_w / 4, player_x + player_w / 2, player_x + 3 * player_w / 4}; // Left, center, right
    int num_points = 3;
    int on_pink = 0;
    int hit_green = 0;
    int detected_platform_y = -1;

    // Lock surface for pixel access
    if (SDL_MUSTLOCK(current_collision_map)) {
        SDL_LockSurface(current_collision_map);
    }

    for (int y_offset = -y_range; y_offset <= y_range; y_offset++) {
        int check_y = check_y_base + y_offset;
        if (check_y < 0 || check_y >= current_collision_map->h) continue;

        for (int i = 0; i < num_points; i++) {
            int check_x = points[i];
            Uint32 pixel;
            Uint8 r, g, b;

            // Get pixel data
            Uint8 *pixels = (Uint8 *)current_collision_map->pixels;
            int bpp = current_collision_map->format->BytesPerPixel;
            Uint8 *p = pixels + check_y * current_collision_map->pitch + check_x * bpp;
            switch (bpp) {
                case 1:
                    pixel = *p;
                    break;
                case 2:
                    pixel = *(Uint16 *)p;
                    break;
                case 3:
                    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                        pixel = p[0] << 16 | p[1] << 8 | p[2];
                    } else {
                        pixel = p[0] | p[1] << 8 | p[2] << 16;
                    }
                    break;
                case 4:
                    pixel = *(Uint32 *)p;
                    break;
                default:
                    pixel = 0;
            }

            SDL_GetRGB(pixel, current_collision_map->format, &r, &g, &b);

            // Pink: #ff00de (255, 0, 222)
            if (r == 255 && g == 0 && b == 222) {
                on_pink = 1;
                detected_platform_y = check_y; // Store the y-coordinate of the platform
                break; // Found a platform, no need to check more points
            }

            // Green: #00ff0c (0, 255, 12)
            if (r == 0 && g == 255 && b == 12) {
                if (status != STAT_AIR || vy >= 0) { // Only die on green if not ascending
                    hit_green = 1;
                }
            }
        }
        if (on_pink) break; // Exit y-loop if we found a platform
    }

    if (SDL_MUSTLOCK(current_collision_map)) {
        SDL_UnlockSurface(current_collision_map);
    }

    if (hit_green) {
        *is_obstacle = 1;
        return 0; // Don't fall, but obstacle logic will handle death
    }

    if (on_pink) {
        *platform_y = detected_platform_y;
        return 0; // On platform, don't fall
    }

    return 1; // Not on pink, fall
}

/**
 * @brief Frees the background resources, including door sprites.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void free_background(Background *bg) {
    if (bg->image_level1) {
        SDL_FreeSurface(bg->image_level1);
        bg->image_level1 = NULL;
    }
    if (bg->collision_map_level1) {
        SDL_FreeSurface(bg->collision_map_level1);
        bg->collision_map_level1 = NULL;
    }
    if (bg->image_level2) {
        SDL_FreeSurface(bg->image_level2);
        bg->image_level2 = NULL;
    }
    if (bg->collision_map_level2) {
        SDL_FreeSurface(bg->collision_map_level2);
        bg->collision_map_level2 = NULL;
    }
    for (int i = 0; i < 6; i++) {
        if (bg->door_images[i]) {
            SDL_FreeSurface(bg->door_images[i]);
            bg->door_images[i] = NULL;
        }
        if (bg->door_images_level2[i]) {
            SDL_FreeSurface(bg->door_images_level2[i]);
            bg->door_images_level2[i] = NULL;
        }
    }
}
