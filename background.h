/**
 * @file background.h
 * @brief Header file for background and level mechanics for the SDL game.
 * @author MohamedNourMrad
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/**
 * @brief Structure representing the background and its properties.
 */
typedef struct {
    SDL_Surface *image_level1;          // Visual background for level 1
    SDL_Surface *collision_map_level1;  // Collision map for level 1
    SDL_Surface *image_level2;          // Visual background for level 2
    SDL_Surface *collision_map_level2;  // Collision map for level 2
    SDL_Surface *door_images[6];        // Door sprite sheet images for level 1 (porte1.png to porte6.png)
    SDL_Surface *door_images_level2[6]; // Door sprite sheet images for level 2 (prt1.png to prt6.png)

    SDL_Rect camera;                    // Camera viewport
    SDL_Rect door_position;             // Position for door animation (level 1)
    SDL_Rect door_position_level2;      // Position for door animation (level 2)
    int platform_y;                     // Kept for reference, but logic will use collision map
    int door_frame;                     // Current door frame (0-5 for level 1, 0-5 for level 2)
    Uint32 door_anim_start_time;        // Timer for door animation
    int door_anim_reverse;              // 0 for forward, 1 for reverse animation
    int door_anim_stopped;              // Flag to stop door animation (0: continue, 1: stop)
    int level;      
                        // Current level (1 or 2)
} Background;

/**
 * @brief Initializes the background with image, collision map, and door sprites.
 * @param bg Pointer to the background structure.
 * @return 0 on success, -1 on failure.
 */
int init_background(Background *bg);

/**
 * @brief Displays the background on the screen, accounting for camera position.
 * @param bg The background structure.
 * @param screen The SDL surface to render on.
 * @return Nothing.
 */
void display_background(Background bg, SDL_Surface *screen);

/**
 * @brief Updates the camera position based on the player's x-coordinate.
 * @param bg Pointer to the background structure.
 * @param player_x The player's x-coordinate in world space.
 * @param screen The SDL surface to determine screen dimensions.
 * @return Nothing.
 */
void update_camera(Background *bg, int player_x, SDL_Surface *screen);

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
int check_collision(Background bg, int player_x, int player_y, int player_w, int player_h, int *is_obstacle, int status, float vy, int *platform_y);

/**
 * @brief Displays the door animation at x=40.
 * @param bg The background structure.
 * @param screen The SDL surface to render on.
 * @return Nothing.
 */
void display_door(Background bg, SDL_Surface *screen);

/**
 * @brief Updates the door animation frame for level 1.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void update_door_animation(Background *bg);

/**
 * @brief Updates the door animation frame for level 2.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void update_door_animation_level2(Background *bg);

/**
 * @brief Sets the x and y coordinates of the level 2 door.
 * @param bg Pointer to the background structure.
 * @param x The new x-coordinate.
 * @param y The new y-coordinate.
 * @return Nothing.
 */
void set_door_position_level2(Background *bg, int x, int y);

/**
 * @brief Frees the background resources, including door sprites.
 * @param bg Pointer to the background structure.
 * @return Nothing.
 */
void free_background(Background *bg);

#endif // BACKGROUND_H
