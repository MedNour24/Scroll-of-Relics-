#include "enigme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

void load_questions(enigme *e, const char *q_file, const char *a_file, const char *c_file) {
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        e->questions_used[i] = 0;
    }

    FILE *q_fp = fopen(q_file, "r");
    if (!q_fp) {
        printf("Error opening %s: %s\n", q_file, strerror(errno));
        exit(1);
    }
    
    e->nb_questions = 0;
    char line[256];
    while (fgets(line, sizeof(line), q_fp) && e->nb_questions < MAX_QUESTIONS) {
        strncpy(e->questions[e->nb_questions], line, sizeof(e->questions[0]) - 1);
        e->questions[e->nb_questions][strcspn(e->questions[e->nb_questions], "\n")] = 0;
        e->nb_questions++;
    }
    fclose(q_fp);

    if (e->nb_questions == 0) {
        printf("Error: No questions loaded from %s\n", q_file);
        exit(1);
    }

    FILE *a_fp = fopen(a_file, "r");
    if (!a_fp) {
        printf("Error opening %s: %s\n", a_file, strerror(errno));
        exit(1);
    }

    for (int i = 0; i < e->nb_questions; i++) {
        for (int j = 0; j < NB_REPONSES; j++) {
            if (!fgets(line, sizeof(line), a_fp)) {
                printf("Error: Not enough answers in %s\n", a_file);
                exit(1);
            }
            strncpy(e->reponses[i][j], line, sizeof(e->reponses[0][0]) - 1);
            e->reponses[i][j][strcspn(e->reponses[i][j], "\n")] = 0;
        }
    }
    fclose(a_fp);

    FILE *c_fp = fopen(c_file, "r");
    if (!c_fp) {
        printf("Error opening %s: %s\n", c_file, strerror(errno));
        exit(1);
    }

    for (int i = 0; i < e->nb_questions; i++) {
        if (!fgets(line, sizeof(line), c_fp)) {
            printf("Error: Not enough correct answers in %s\n", c_file);
            exit(1);
        }
        e->correct_answers[i] = atoi(line);
        if (e->correct_answers[i] < 0 || e->correct_answers[i] >= NB_REPONSES) {
            printf("Error: Invalid correct answer %d for question %d\n", e->correct_answers[i], i);
            exit(1);
        }
    }
    fclose(c_fp);
}

void initialiser_enigme(enigme *e) {
    memset(e, 0, sizeof(enigme));

    load_questions(e, "questions.txt", "answers.txt", "correct_answers.txt");

    e->score = 0;
    e->vies = MAX_VIES;
    e->level = 1;
    e->temps_restant = TEMPS_PAR_QUESTION;
    e->questions_answered = 0;
    srand(time(NULL));

    e->background = IMG_Load("ques.png");
    if (!e->background) {
        printf("Failed to load background: %s\n", IMG_GetError());
        exit(1);
    }

    e->button = IMG_Load("but3.png");
    if (!e->button) {
        printf("Failed to load button: %s\n", IMG_GetError());
        exit(1);
    }

    e->button_s = IMG_Load("but-s.png");
    if (!e->button_s) {
        printf("Failed to load button_s: %s\n", IMG_GetError());
        exit(1);
    }

    e->sound = Mix_LoadWAV("clic.wav");
    if (!e->sound) {
        printf("Failed to load sound: %s\n", Mix_GetError());
        exit(1);
    }

    e->correct_sound = Mix_LoadWAV("correct.wav");
    if (!e->correct_sound) {
        printf("Failed to load correct_sound: %s\n", Mix_GetError());
        exit(1);
    }

    e->wrong_sound = Mix_LoadWAV("wrong.wav");
    if (!e->wrong_sound) {
        printf("Failed to load wrong_sound: %s\n", Mix_GetError());
        exit(1);
    }

    // Initialize positions
    e->pos_timer.x = 90;
    e->pos_timer.y = 50;
    e->pos_question.x = (SCREEN_WIDTH - 350) / 2;
    e->pos_question.y = 150;
    e->pos_reponse1.x = (SCREEN_WIDTH - 500) / 2;
    e->pos_reponse1.y = 200;
    e->pos_reponse2.x = (SCREEN_WIDTH - 500) / 2;
    e->pos_reponse2.y = 350;
    e->pos_reponse3.x = (SCREEN_WIDTH - 500) / 2;
    e->pos_reponse3.y = 500;
    e->pos_score.x = 90;
    e->pos_score.y = e->pos_timer.y + 30;
    e->pos_level.x = SCREEN_WIDTH - 200;
    e->pos_level.y = e->pos_timer.y + 30;

    // Text positions (centered on buttons)
    e->pos_reponse1txt.x = e->pos_reponse1.x + 200;
    e->pos_reponse1txt.y = e->pos_reponse1.y + 40;
    e->pos_reponse2txt.x = e->pos_reponse2.x + 200;
    e->pos_reponse2txt.y = e->pos_reponse2.y + 40;
    e->pos_reponse3txt.x = e->pos_reponse3.x + 200;
    e->pos_reponse3txt.y = e->pos_reponse3.y + 40;

    for (int i = 0; i < MAX_VIES; i++) {
        e->pos_vies[i].x = SCREEN_WIDTH - 150 - i * 35;
        e->pos_vies[i].y = 10;
    }
}

void generer_enigme(enigme *e, int question_index, TTF_Font *font) {
    if (!e || !font || question_index < 0 || question_index >= e->nb_questions) {
        printf("Invalid enigma, font, or question index: %d\n", question_index);
        return;
    }

    e->pos_selected = 0;
    e->temps_debut = SDL_GetTicks();
    e->temps_restant = TEMPS_PAR_QUESTION - (e->level - 1) * 2;
    if (e->temps_restant < 10) e->temps_restant = 10;
    e->num_question = question_index;

    SDL_Color couleur = {0, 0, 0};

    // Free existing surfaces if they exist
    if (e->question) {
        SDL_FreeSurface(e->question);
        e->question = NULL;
    }
    e->question = TTF_RenderText_Solid(font, e->questions[question_index], couleur);
    if (!e->question) {
        printf("Failed to render question: %s\n", TTF_GetError());
        return;
    }

    for (int i = 0; i < NB_REPONSES; i++) {
        if (e->reponses_surface[i]) {
            SDL_FreeSurface(e->reponses_surface[i]);
            e->reponses_surface[i] = NULL;
        }
        e->reponses_surface[i] = TTF_RenderText_Solid(font, e->reponses[question_index][i], couleur);
        if (!e->reponses_surface[i]) {
            printf("Failed to render answer %d: %s\n", i, TTF_GetError());
            if (e->question) {
                SDL_FreeSurface(e->question);
                e->question = NULL;
            }
            for (int j = 0; j < i; j++) {
                if (e->reponses_surface[j]) {
                    SDL_FreeSurface(e->reponses_surface[j]);
                    e->reponses_surface[j] = NULL;
                }
            }
            return;
        }
    }

    char score_text[50], level_text[50];
    sprintf(score_text, "Score: %d", e->score);
    sprintf(level_text, "Level: %d", e->level);

    if (e->score_surface) {
        SDL_FreeSurface(e->score_surface);
        e->score_surface = NULL;
    }
    e->score_surface = TTF_RenderText_Solid(font, score_text, couleur);
    if (!e->score_surface) {
        printf("Failed to render score: %s\n", TTF_GetError());
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
        return;
    }

    if (e->level_surface) {
        SDL_FreeSurface(e->level_surface);
        e->level_surface = NULL;
    }
    e->level_surface = TTF_RenderText_Solid(font, level_text, couleur);
    if (!e->level_surface) {
        printf("Failed to render level: %s\n", TTF_GetError());
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
        return;
    }

    for (int i = 0; i < MAX_VIES; i++) {
        if (e->vies_surface[i]) {
            SDL_FreeSurface(e->vies_surface[i]);
            e->vies_surface[i] = NULL;
        }
        e->vies_surface[i] = (i < e->vies) ? IMG_Load("vie.png") : IMG_Load("vie_vide.png");
        if (!e->vies_surface[i]) {
            printf("Failed to load life image %d: %s\n", i, IMG_GetError());
            if (e->question) {
                SDL_FreeSurface(e->question);
                e->question = NULL;
            }
            for (int j = 0; j < NB_REPONSES; j++) {
                if (e->reponses_surface[j]) {
                    SDL_FreeSurface(e->reponses_surface[j]);
                    e->reponses_surface[j] = NULL;
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
            for (int j = 0; j < i; j++) {
                if (e->vies_surface[j]) {
                    SDL_FreeSurface(e->vies_surface[j]);
                    e->vies_surface[j] = NULL;
                }
            }
            return;
        }
    }

    printf("Enigma generated successfully for question %d\n", question_index);
}

void afficher_enigme(enigme *e, SDL_Surface *ecran, int souris_x, int souris_y) {
    if (!e || !ecran) {
        printf("Error: Null enigma or screen in afficher_enigme\n");
        return;
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    if (e->background) {
        SDL_BlitSurface(e->background, NULL, ecran, NULL);
    } else {
        printf("Warning: Background surface is null\n");
    }

    if (e->question) {
        SDL_BlitSurface(e->question, NULL, ecran, &e->pos_question);
    } else {
        printf("Warning: Question surface is null\n");
    }

    int hover_button = -1;
    if (e->button && e->button_s) {
        if (souris_x >= e->pos_reponse1.x && souris_x <= e->pos_reponse1.x + e->button->w &&
            souris_y >= e->pos_reponse1.y && souris_y <= e->pos_reponse1.y + e->button->h) {
            hover_button = 1;
        } else if (souris_x >= e->pos_reponse2.x && souris_x <= e->pos_reponse2.x + e->button->w &&
                   souris_y >= e->pos_reponse2.y && souris_y <= e->pos_reponse2.y + e->button->h) {
            hover_button = 2;
        } else if (souris_x >= e->pos_reponse3.x && souris_x <= e->pos_reponse3.x + e->button->w &&
                   souris_y >= e->pos_reponse3.y && souris_y <= e->pos_reponse3.y + e->button->h) {
            hover_button = 3;
        }
    } else {
        printf("Warning: Button or selected button surface is null\n");
    }

    SDL_Surface *buttons[3] = {NULL};
    if (e->button && e->button_s) {
        buttons[0] = (hover_button == 1) ? e->button_s : e->button;
        buttons[1] = (hover_button == 2) ? e->button_s : e->button;
        buttons[2] = (hover_button == 3) ? e->button_s : e->button;

        if (buttons[0]) SDL_BlitSurface(buttons[0], NULL, ecran, &e->pos_reponse1);
        if (buttons[1]) SDL_BlitSurface(buttons[1], NULL, ecran, &e->pos_reponse2);
        if (buttons[2]) SDL_BlitSurface(buttons[2], NULL, ecran, &e->pos_reponse3);
    }

    for (int i = 0; i < NB_REPONSES; i++) {
        if (e->reponses_surface[i]) {
            SDL_Rect *pos = (i == 0) ? &e->pos_reponse1txt : (i == 1) ? &e->pos_reponse2txt : &e->pos_reponse3txt;
            SDL_BlitSurface(e->reponses_surface[i], NULL, ecran, pos);
        } else {
            printf("Warning: Response surface %d is null\n", i);
        }
    }

    for (int i = 0; i < MAX_VIES; i++) {
        if (e->vies_surface[i]) {
            SDL_BlitSurface(e->vies_surface[i], NULL, ecran, &e->pos_vies[i]);
        } else {
            printf("Warning: Life surface %d is null\n", i);
        }
    }

    if (e->score_surface) {
        SDL_BlitSurface(e->score_surface, NULL, ecran, &e->pos_score);
    } else {
        printf("Warning: Score surface is null\n");
    }

    if (e->level_surface) {
        SDL_BlitSurface(e->level_surface, NULL, ecran, &e->pos_level);
    } else {
        printf("Warning: Level surface is null\n");
    }

    SDL_Rect timer_border = {e->pos_timer.x, e->pos_timer.y, 150, 20};
    SDL_FillRect(ecran, &timer_border, SDL_MapRGB(ecran->format, 255, 255, 255));

    float time_ratio = (float)e->temps_restant / TEMPS_PAR_QUESTION;
    int fill_width = (int)(148 * time_ratio);
    if (fill_width < 0) fill_width = 0;
    if (fill_width > 148) fill_width = 148;

    Uint8 r = (Uint8)(255 * (1 - time_ratio));
    Uint8 g = (Uint8)(255 * time_ratio);
    SDL_Rect timer_fill = {e->pos_timer.x + 1, e->pos_timer.y + 1, fill_width, 18};
    SDL_FillRect(ecran, &timer_fill, SDL_MapRGB(ecran->format, r, g, 0));

    SDL_Flip(ecran);
    printf("Enigma displayed successfully\n");
}

int verify_enigme(enigme *e, SDL_Surface *ecran) {
    if (e->pos_selected < 1 || e->pos_selected > 3) return 0;

    if (e->correct_answers[e->num_question] == e->pos_selected - 1) {
        e->score += 10 * e->level;
        if (e->correct_sound) Mix_PlayChannel(-1, e->correct_sound, 0);
        return 1;
    } else {
        e->vies--;
        if (e->wrong_sound) Mix_PlayChannel(-1, e->wrong_sound, 0);
        return 0;
    }
}

void mettre_a_jour_timer(enigme *e) {
    Uint32 temps_ecoule = SDL_GetTicks() - e->temps_debut;
    e->temps_restant = (TEMPS_PAR_QUESTION - (e->level - 1) * 2) - (temps_ecoule / 1000);
    if (e->temps_restant <= 0) {
        e->temps_restant = 0;
        e->vies--;
    }
}

void afficher_resultat(SDL_Surface *ecran, int correct, enigme *e, TTF_Font *font) {
    if (!ecran || !e || !font) {
        printf("Error: Null screen, enigma, or font in afficher_resultat\n");
        return;
    }

    SDL_Color couleur = {255, 255, 255};
    const char *message = correct ? "Correct!" : "Wrong!";
    SDL_Surface *resultat = TTF_RenderText_Blended(font, message, couleur);
    
    if (resultat) {
        SDL_Rect pos = {(SCREEN_WIDTH - resultat->w) / 2, (SCREEN_HEIGHT - resultat->h) / 2};
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(resultat, NULL, ecran, &pos);
        SDL_Flip(ecran);
        SDL_Delay(1000);
        SDL_FreeSurface(resultat);
    } else {
        printf("Failed to render result text: %s\n", TTF_GetError());
    }

    printf("Result displayed: %s\n", message);
}

void free_enigme(enigme *e) {
    if (e->background) SDL_FreeSurface(e->background);
    if (e->question) SDL_FreeSurface(e->question);
    
    for (int i = 0; i < NB_REPONSES; i++) {
        if (e->reponses_surface[i]) SDL_FreeSurface(e->reponses_surface[i]);
    }
    
    if (e->button) SDL_FreeSurface(e->button);
    if (e->button_s) SDL_FreeSurface(e->button_s);
    
    for (int i = 0; i < MAX_VIES; i++) {
        if (e->vies_surface[i]) SDL_FreeSurface(e->vies_surface[i]);
    }
    
    if (e->score_surface) SDL_FreeSurface(e->score_surface);
    if (e->level_surface) SDL_FreeSurface(e->level_surface);
    
    if (e->sound) Mix_FreeChunk(e->sound);
    if (e->correct_sound) Mix_FreeChunk(e->correct_sound);
    if (e->wrong_sound) Mix_FreeChunk(e->wrong_sound);
}

int afficher_ecran_accueil(SDL_Surface *ecran) {
    if (!ecran) {
        printf("Error: Screen surface is NULL\n");
        return -1;
    }

    printf("Screen: w=%d, h=%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *bg = IMG_Load("bg.png");
    if (!bg) {
        printf("Failed to load bg.png: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Surface *quizz = IMG_Load("quizz.png");
    if (!quizz) {
        printf("Failed to load quizz.png: %s\n", IMG_GetError());
        SDL_FreeSurface(bg);
        return -1;
    }
    SDL_Surface *puz = IMG_Load("puz.png");
    if (!puz) {
        printf("Failed to load puz.png: %s\n", IMG_GetError());
        SDL_FreeSurface(bg);
        SDL_FreeSurface(quizz);
        return -1;
    }

    // Define positions
    SDL_Rect pos_bg = {0, 0};
    SDL_Rect pos_quizz = {200, 450}; // Centré à gauche
    SDL_Rect pos_puz = {500, 450}; // Centré à droite

    // Define clickable areas
    int quizz_click_x = pos_quizz.x;
    int quizz_click_y = pos_quizz.y;
    int quizz_click_w = quizz->w;
    int quizz_click_h = quizz->h;

    int puz_click_x = pos_puz.x;
    int puz_click_y = pos_puz.y;
    int puz_click_w = puz->w;
    int puz_click_h = puz->h;

    // Log clickable areas
    printf("Clickable areas:\n");
    printf("  quizz: x=%d to %d, y=%d to %d\n", quizz_click_x, quizz_click_x + quizz_click_w, 
           quizz_click_y, quizz_click_y + quizz_click_h);
    printf("  puz: x=%d to %d, y=%d to %d\n", puz_click_x, puz_click_x + puz_click_w, 
           puz_click_y, puz_click_y + puz_click_h);

    // Render screen
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(bg, NULL, ecran, &pos_bg);
    SDL_BlitSurface(quizz, NULL, ecran, &pos_quizz);
    SDL_BlitSurface(puz, NULL, ecran, &pos_puz);
    SDL_Flip(ecran);
    printf("Screen rendered\n");

    // Event loop
    int choice = 0;
    SDL_Event event;
    while (choice == 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_MOUSEMOTION) {
                printf("Mouse at x=%d, y=%d\n", event.motion.x, event.motion.y);
                continue;
            }
            printf("Event type=%d\n", event.type);
            if (event.type == SDL_QUIT) {
                printf("SDL_QUIT received\n");
                choice = -1;
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                printf("Clicked at x=%d, y=%d\n", x, y);
                if (x >= quizz_click_x && x < quizz_click_x + quizz_click_w &&
                    y >= quizz_click_y && y < quizz_click_y + quizz_click_h) {
                    printf("Clicked quizz.png\n");
                    choice = 1;
                }
                else if (x >= puz_click_x && x < puz_click_x + puz_click_w &&
                         y >= puz_click_y && y < puz_click_y + puz_click_h) {
                    printf("Clicked puz.png\n");
                    choice = 2;
                }
                else {
                    printf("Clicked outside both areas\n");
                }
                while (SDL_PollEvent(&event)); // Clear queue
            }
        }
        // Redraw screen
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
        SDL_BlitSurface(bg, NULL, ecran, &pos_bg);
        SDL_BlitSurface(quizz, NULL, ecran, &pos_quizz);
        SDL_BlitSurface(puz, NULL, ecran, &pos_puz);
        SDL_Flip(ecran);
        SDL_Delay(10);
    }

    printf("Exiting with choice=%d\n", choice);
    SDL_FreeSurface(bg);
    SDL_FreeSurface(quizz);
    SDL_FreeSurface(puz);
    return choice;
}

void afficher_game_over(SDL_Surface *ecran) {
    if (!ecran) return;

    SDL_Surface *game_over = IMG_Load("game_over.png");
    if (!game_over) {
        printf("Failed to load game_over.png: %s\n", IMG_GetError());
        return;
    }
    
    SDL_Rect pos = {(SCREEN_WIDTH - game_over->w) / 2, (SCREEN_HEIGHT - game_over->h) / 2};
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(game_over, NULL, ecran, &pos);
    SDL_Flip(ecran);
    SDL_Delay(3000);
    SDL_FreeSurface(game_over);
}

void afficher_victoire(SDL_Surface *ecran) {
    if (!ecran) return;

    SDL_Surface *victoire = IMG_Load("victoire.png");
    if (!victoire) {
        printf("Failed to load victoire.png: %s\n", IMG_GetError());
        return;
    }
    
    SDL_Rect pos = {(SCREEN_WIDTH - victoire->w) / 2, (SCREEN_HEIGHT - victoire->h) / 2};
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_BlitSurface(victoire, NULL, ecran, &pos);
    SDL_Flip(ecran);
    SDL_Delay(3000);
    SDL_FreeSurface(victoire);
}
