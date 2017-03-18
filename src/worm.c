/*
 * worm.c
 * Copyright: Nov 22, 2015 David Shurgold
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
#include "wormwar.h"
#include "worm.h"
#include <allegro5/allegro_primitives.h>

void determine_movement(float *move_x, float *move_y, Direction direction) {
    DEBUG_STMT();
    *move_x = 0;
    *move_y = 0;
    if (direction == UP) {
        *move_y = -WORM_RADIUS;
    } else if (direction == DOWN) {
        *move_y = WORM_RADIUS;
    } else if (direction == LEFT) {
        *move_x = -WORM_RADIUS;
    } else if (direction == RIGHT) {
        *move_x = WORM_RADIUS;
    }
}

Worm* new_worm(float x, float y, Direction dir, int length, ALLEGRO_COLOR colour) {
    DEBUG_STMT();
    Worm *worm;
    float mod_x = 0, mod_y = 0;
    int i;
    worm = al_malloc(sizeof(Worm));
    if (worm == NULL) {
        fprintf(stderr, "failed to allocate memory for new worm\n");
        return NULL;
    }

    worm->colour = colour;
    worm->length = length;
    worm->max_length = length * 2;
    worm->next = dir;
    worm->to_grow = 0;

    DEBUG_PRINT("new_worm: %.2fx%.2f %d,%d,%d\n", x, y, length, dir, worm->max_length);
    worm->body = al_malloc(sizeof(WormPoint) * worm->max_length);
    if (worm->body == NULL) {
        fprintf(stderr, "failed to allocate memory for worm size\n");
        return NULL;
    }

    determine_movement(&mod_x, &mod_y, dir);
    for (i = 0; i < length; i++) {
        worm->body[i].x = x - mod_x * i;
        worm->body[i].y = y - mod_y * i;
        worm->body[i].direction = dir;
        DEBUG_PRINT("new_worm: %d - %.2fx%.2f\n", i, worm->body[i].x, worm->body[i].y);
    }

    return worm;
}

void draw_worm(Worm *worm) {
    DEBUG_STMT();
    int i = 0;
    for (; i < worm->length; i++) {
        al_draw_filled_circle(worm->body[i].x, worm->body[i].y + SCORE_BAR_HEIGHT, WORM_RADIUS, worm->colour);
    }
}

void move_worm(Worm *worm) {
    DEBUG_STMT();
    int i = 0;
    float mod_x, mod_y, orig_x, orig_y;
    Direction last = worm->next;
    Direction next;

    for (; i < worm->length; i++) {
        //DEBUG_PRINT("move_worm: %.2fx%2.f - %d -> ", worm->body[i].x, worm->body[i].y, worm->body[i].direction);
        orig_x = worm->body[i].x;
        orig_y = worm->body[i].y;

        determine_movement(&mod_x, &mod_y, last);
        worm->body[i].x += mod_x;
        worm->body[i].y += mod_y;
        if (worm->body[i].x < 0) {
            worm->body[i].x += ARENA_WIDTH;
        } else if (worm->body[i].x > ARENA_WIDTH) {
            worm->body[i].x -= ARENA_WIDTH;
        }
        if (worm->body[i].y < 0) {
            worm->body[i].y += ARENA_HEIGHT;
        } else if (worm->body[i].y > ARENA_HEIGHT) {
            worm->body[i].y -= ARENA_HEIGHT;
        }
        next = worm->body[i].direction;
        worm->body[i].direction = last;
        last = next;
        //DEBUG_PRINT("%.2fx%2.f - %d\n", worm->body[i].x, worm->body[i].y, worm->body[i].direction);
    }

    if (worm->to_grow > 0) {
        worm->body[worm->length].x = orig_x;
        worm->body[worm->length].y = orig_y;
        worm->body[worm->length].direction = last;
        DEBUG_PRINT("mov_worm: growing to %d at %.2fx%.2f in direction %d\n",
                worm->length + 1,
                worm->body[worm->length].x,
                worm->body[worm->length].y,
                worm->body[worm->length].direction);
        worm->length++;
        worm->to_grow--;
    }
}

void grow_worm(Worm *worm, int length) {
    DEBUG_STMT();
    if (worm->length + length + worm->to_grow > worm->max_length) {
        DEBUG_PRINT("realloc worm from %d to %d (%d)", worm->max_length, worm->max_length * 2, sizeof(WormPoint) * worm->max_length * 2);
        worm->body = al_realloc(worm->body, sizeof(WormPoint) * worm->max_length * 2);
        worm->max_length *= 2;
    }
    worm->to_grow += length;
    DEBUG_PRINT("need to grow to %d\n", worm->to_grow);
}

int find_intersected_worm(float x, float y, Worm *worm) {
    DEBUG_STMT();
    int i = 0;
    float wx1, wy1, wx2, wy2, ex1, ey1, ex2, ey2;

    DEBUG_PRINT("find_intersected_worm %.2f %.2f\n", x, y);

    wx1 = x;
    wy1 = y;
    wx2 = x;
    wy2 = y;
    for (i = worm->length-1; i >= 0; i--) {
        ex1 = worm->body[i].x;
        ey1 = worm->body[i].y;
        ex2 = worm->body[i].x;
        ey2 = worm->body[i].y;

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

        DEBUG_PRINT("index %d at %.2f+%.2f,%.2fx%.2f intersects with %.2fx%.2f,%.2fx%.2f\n", i, wx1, wy1, wx2, wy2, ex1, ey1, ex2, ey2);
        return i;
    }
    return -1;
}

IntersectionType process_intersected_worm(Worm *worm, GameElements *elements, int intersected) {
    DEBUG_STMT();
    return LOSE;
}
