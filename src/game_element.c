/*
 * game_element.c
 * Copyright: Nov 23, 2015 David Shurgold
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
#include "game_element.h"
#include "worm.h"
#include <allegro5/allegro_primitives.h>

GameElements* new_game_elements() {
    DEBUG_STMT();
    GameElements *elements = NULL;
    ElementType type;
    int i, random_number;

    srandom(time(NULL));
    elements = al_malloc(sizeof(GameElements));
    if (elements == NULL) {
        fprintf(stderr, "failed to allocate memory for new game element\n");
        return NULL;
    }
    elements->max_length = 100;
    elements->elements = al_malloc(sizeof(Element) * 100);
    if (elements->elements == NULL) {
        fprintf(stderr, "failed to allocate memory for new elements\n");
        return NULL;
    }
    for (i = 0; i < elements->max_length; i++) {
        DEBUG_PRINT("setting index %d to empty\n", i);
        elements->elements[i].type = EMPTY;
    }
    return elements;
}

int do_add_elements() {
    int element_count = ((random() % 100) - 95);
    if(element_count < 0) {
        return 0;
    }
    element_count = element_count % 2 + 1;
    DEBUG_PRINT("ODDS: do_add_elements: %d\n", element_count);
    return element_count;
}

void add_game_elements(Worm *worm, GameElements *elements, GameState *state, int number) {
    DEBUG_STMT();
    int i, random_number, odd_modifier;
    float x, y;

    DEBUG_PRINT("going to add %d\n", number);
    for (i = 0; i < elements->max_length; i++) {
        if (elements->elements[i].type == EMPTY) {
            do {
                x = random() % ARENA_WIDTH;
                y = random() % ARENA_HEIGHT;
            } while (find_intersected_game_element(x, y, elements) >= 0);

            random_number = random() % 100;
            odd_modifier = (state->speed / 4) - 5;
            DEBUG_PRINT("ODDS: (%d) %d - %d - %d\n", random_number, 5 - odd_modifier, 20 - odd_modifier * 2, 60 - odd_modifier * 4);
            if (random_number == 1 || (random_number <= (5 - odd_modifier))) {
                elements->elements[i].type = WALL_MUSHROOM;
            } else if (random_number <= 5 || (random_number <= (20 - odd_modifier * 2))) {
                elements->elements[i].type = ENHANCED_MUSHROOM;
            } else if (random_number <= 15 || (random_number <= (60 - odd_modifier * 4))) {
                elements->elements[i].type = SIMPLE_MUSHROOM;
            } else {
                elements->elements[i].type = WALL;
            }

            elements->elements[i].x = x;
            elements->elements[i].y = y;

            DEBUG_PRINT("created %d type at %.2fx%.2f\n", elements->elements[i].type, elements->elements[i].x, elements->elements[i].y);
            number--;
            if (number <= 0) {
                break;
            }
        }
    }
    if (number > 0) {
        DEBUG_PRINT("realloc elements from %d to %d\n", elements->max_length, elements->max_length * 2);
        elements->elements = al_realloc(elements->elements, sizeof(Element) * elements->max_length * 2);
        if (elements->elements == NULL) {
            fprintf(stderr, "failed to allocate realloc for %d elements", elements->max_length * 2);
        }
        for (i = elements->max_length; i < elements->max_length * 2; i++) {
            elements->elements[i].type = EMPTY;
        }
        elements->max_length *= 2;
        add_game_elements(worm, elements, state, number);
    }
}

void draw_game_elements(GameElements *elements) {
    DEBUG_STMT();
    int i = 0;
    ALLEGRO_COLOR simple = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR enhanced = al_map_rgb(255, 0, 255);
    ALLEGRO_COLOR wall = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR wall_mushroom = al_map_rgb(0, 255, 0);

    for (; i < elements->max_length; i++) {
        if (elements->elements[i].type == WALL) {
            al_draw_filled_rectangle(elements->elements[i].x - WORM_RADIUS,
                    elements->elements[i].y - WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x + WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    wall);
        } else if (elements->elements[i].type == SIMPLE_MUSHROOM) {
            al_draw_filled_triangle(elements->elements[i].x - WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x,
                    elements->elements[i].y - WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x + WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    simple);
        } else if (elements->elements[i].type == ENHANCED_MUSHROOM) {
            al_draw_filled_triangle(elements->elements[i].x - WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x,
                    elements->elements[i].y - WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x + WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    enhanced);
        } else if (elements->elements[i].type == WALL_MUSHROOM) {
            al_draw_filled_triangle(elements->elements[i].x - WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x,
                    elements->elements[i].y - WORM_RADIUS + SCORE_BAR_HEIGHT,
                    elements->elements[i].x + WORM_RADIUS,
                    elements->elements[i].y + WORM_RADIUS + SCORE_BAR_HEIGHT,
                    wall_mushroom);
        }
    }
}

int find_intersected_game_element(float x, float y, GameElements *elements) {
    DEBUG_STMT();
    int i = 0;
    float wx1, wy1, wx2, wy2, ex1, ey1, ex2, ey2;

    DEBUG_PRINT("find_intersected_game_element %.2f %.2f\n", x, y);

    wx1 = x - WORM_RADIUS;
    wy1 = y - WORM_RADIUS;
    wx2 = x + WORM_RADIUS;
    wy2 = y + WORM_RADIUS;
    for (; i < elements->max_length; i++) {
        if (elements->elements[i].type != EMPTY) {
            ex1 = elements->elements[i].x - WORM_RADIUS * 0.8;
            ey1 = elements->elements[i].y - WORM_RADIUS * 0.8;
            ex2 = elements->elements[i].x + WORM_RADIUS * 0.8;
            ey2 = elements->elements[i].y + WORM_RADIUS * 0.8;

            if (wy2 < ey1) {
                continue;
            }
            if (ey2 < wy1) {
                continue;
            }
            if (wx2 < ex1) {
                continue;
            }
            if (ex2 < wx1) {
                continue;
            }

            DEBUG_PRINT("index %d type %d at %.2f+%.2f,%.2fx%.2f intersects with %.2fx%.2f,%.2fx%.2f\n", i, elements->elements[i].type, wx1, wy1, wx2, wy2, ex1, ey1, ex2, ey2);
            return i;
        }
    }
    return -1;
}

IntersectionType process_intersected_element(Worm *worm, GameElements *elements, GameState *state, int intersected) {
    DEBUG_STMT();
    ElementType type = elements->elements[intersected].type;
    elements->elements[intersected].type = EMPTY;
    if (type == SIMPLE_MUSHROOM) {
        grow_worm(worm, 4);
        state->scores[0] += (10 * state->speed / 20);
        return GROWTH;
    } else if(type == ENHANCED_MUSHROOM) {
        grow_worm(worm, 8);
        state->scores[0] += (30 * state->speed / 20);
        return GROWTH;
    } else if(type == WALL_MUSHROOM) {
        state->speed += 4;
        return SPEED_CLEAR;
    } else if(type == WALL) {
        return LOSE;
    }
    return NONE;
}

void clear_wall_elements(GameElements *elements, int count) {
    DEBUG_STMT();
    int i = 0;
    for(;i<elements->max_length;i++) {
        if(elements->elements[i].type == WALL) {
            elements->elements[i].type = EMPTY;
            count--;
        }
        if(count <= 0) {
            return;
        }
    }
}
