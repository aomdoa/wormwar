/*
 * game_state.c
 * Copyright: Dec 7, 2015 David Shurgold
 *
 * This file is part of wormwar.
 *
 * wormwar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * wormwar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with wormwar.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
#include "game_state.h"
#include "worm.h"
#include "game_element.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

ALLEGRO_BITMAP *points_background = NULL;
ALLEGRO_FONT *points_font = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *game_timer = NULL;
Worm *worm = NULL;
GameElements *elements = NULL;
GameState *state = NULL;

void free_game() {
    DEBUG_STMT()
    ;
    if (points_background != NULL) {
        al_destroy_bitmap(points_background);
    }
    if (points_font != NULL) {
        al_destroy_font(points_font);
    }
    if (event_queue != NULL) {
        al_destroy_event_queue(event_queue);
    }
    if (game_timer != NULL) {
        al_destroy_timer(game_timer);
    }
    if (worm != NULL) {
        al_free(worm);
    }
    if (elements != NULL) {
        al_free(elements);
    }
    if (state != NULL) {
        al_free(state);
    }
}

GameState *new_game_state(int number_players) {
    DEBUG_STMT()
    ;
    int i = 0;
    GameState *state = al_malloc(sizeof(GameState));
    if (state == NULL) {
        fprintf(stderr, "Unable to allocate memory for game state\n");
        return NULL;
    }
    state->scores = al_malloc(sizeof(int) * number_players);
    if (state->scores == NULL) {
        fprintf(stderr, "Unable to allocate memory for game scores\n");
        return NULL;
    }
    state->players = number_players;
    for (; i < number_players; i++) {
        state->scores[i] = 0;
    }
    state->speed = 20;
    return state;
}

void draw_game_state(GameState *state) {
    DEBUG_STMT()
    ;
    char *the_score = al_malloc(sizeof(char) * 32);
    if (the_score == NULL) {
        fprintf(stderr, "Unable to allocate memory for score display\n");
        return;
    }
    sprintf(the_score, "%'d", state->scores[0]);
    al_draw_bitmap(points_background, 0, 0, 0);
    al_draw_text(points_font, al_map_rgb(255, 255, 255), 200, 2, ALLEGRO_ALIGN_RIGHT, the_score);
}

int run_game(ALLEGRO_DISPLAY *display) {
    DEBUG_STMT();
    bool redraw = true, paused = false;
    int found_index, add_element_count;

    al_change_directory(DATADIR);

    elements = new_game_elements();
    if (!elements) {
        fprintf(stderr, "failed to create elements\n");
        free_game();
        return -1;
    }
    worm = new_worm(100, 100, RIGHT, 10, al_map_rgb(0, 255, 0));
    if (!worm) {
        fprintf(stderr, "failed to create worm\n");
        free_game();
        return -1;
    }
    state = new_game_state(1);
    if (!state) {
        fprintf(stderr, "failed to create game state\n");
        free_game();
        return -1;
    }

    points_background = al_load_bitmap("data/score_background.png");
    if (points_background == NULL) {
        fprintf(stderr, "Unable to load score_background.png");
        free_game();
        return -1;
    }
    points_font = al_load_ttf_font("data/MavenPro-Black.ttf", 28, 0);
    if (points_font == NULL) {
        fprintf(stderr, "Unable to load MavenPro-Black.ttf");
        free_game();
        return -1;
    }

    game_timer = al_create_timer(1.0 / state->speed);
    if (!game_timer) {
        fprintf(stderr, "failed to create game timer!\n");
        free_game();
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        free_game();
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(game_timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_set_target_bitmap(al_get_backbuffer(display));
    add_game_elements(worm, elements, state, 10);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_worm(worm);
    al_flip_display();

    al_start_timer(game_timer);

    while (1) {
        IntersectionType intersection_type;
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        DEBUG_PRINT("Received event %d\n", ev.type);

        if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == game_timer && !paused) {
            DEBUG_PRINT("game timer event\n");
            redraw = true;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (paused) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    paused = false;
                    al_start_timer(game_timer);
                }
            } else {
                if ((ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_W) && worm->body[0].direction != DOWN) {
                    worm->next = UP;
                } else if ((ev.keyboard.keycode == ALLEGRO_KEY_DOWN || ev.keyboard.keycode == ALLEGRO_KEY_S) && worm->body[0].direction != UP) {
                    worm->next = DOWN;
                } else if ((ev.keyboard.keycode == ALLEGRO_KEY_LEFT || ev.keyboard.keycode == ALLEGRO_KEY_A) && worm->body[0].direction != RIGHT) {
                    worm->next = LEFT;
                } else if ((ev.keyboard.keycode == ALLEGRO_KEY_RIGHT || ev.keyboard.keycode == ALLEGRO_KEY_D) && worm->body[0].direction != LEFT) {
                    worm->next = RIGHT;
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    paused = true;
                    al_stop_timer(game_timer);
                }
            }
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            DEBUG_PRINT("Process game and draw!\n");
            move_worm(worm);
            found_index = find_intersected_game_element(worm->body[0].x, worm->body[0].y, elements);
            if (found_index >= 0) {
                intersection_type = process_intersected_element(worm, elements, state, found_index);
                if (intersection_type == LOSE) {
                    fprintf(stderr, "worm hit a wall!!\n");
                    break;
                } else if (intersection_type == SPEED_CLEAR) {
                    al_set_timer_speed(game_timer, 1.0 / state->speed);
                    clear_wall_elements(elements, 10);
                }
            }
            found_index = find_intersected_worm(worm->body[0].x, worm->body[0].y, worm);
            if (found_index > 2) {
                //Don't count the head as an intersection
                if (process_intersected_worm(worm, elements, found_index) == LOSE) {
                    fprintf(stderr, "worm hit a worm!!\n");
                    break;
                }
            }
            add_element_count = do_add_elements();
            if (add_element_count > 0) {
                add_game_elements(worm, elements, state, add_element_count);
            }

            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));

            draw_game_elements(elements);
            draw_worm(worm);
            draw_game_state(state);

            al_flip_display();
        }
    }

    free_game();
    return 0;
}
