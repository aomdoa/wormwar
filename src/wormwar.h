/*
 * wormwar.h
 * Copyright: Nov 20, 2015 David Shurgold
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
#ifndef SRC_WORMWAR_H_
#define SRC_WORMWAR_H_

#include <allegro5/allegro.h>
#include <stdio.h>

#ifdef DEBUG
# define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
# define DEBUG_STMT() fprintf(stderr, "DBG: %s(%d): %s\n", __FILE__, __LINE__, __func__);
#else
# define DEBUG_PRINT(...) do {} while (0)
# define DEBUG_STMT() do {} while (0)
#endif
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define SCORE_BAR_HEIGHT 40

#define ARENA_WIDTH 800
#define ARENA_HEIGHT 560

//Configure how big each worm segment is
#define WORM_RADIUS 5

typedef enum {
    NONE,
    GROWTH,
    SPEED_CLEAR,
    LOSE
} IntersectionType;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct WormPoint {
    float x, y;
    Direction direction;
} WormPoint;

typedef struct Worm {
    WormPoint *body;
    Direction next;
    int length;
    int max_length;
    int to_grow;
    ALLEGRO_COLOR colour;
} Worm;

typedef enum {
    EMPTY,
    SIMPLE_MUSHROOM,
    ENHANCED_MUSHROOM,
    WALL,
    WALL_MUSHROOM
} ElementType;

typedef struct Element {
    float x, y;
    ElementType type;
} Element;

typedef struct GameElements {
    Element *elements;
    int max_length;
} GameElements;

typedef struct GameState {
    int players;
    int *scores;
    int speed;
} GameState;
#endif /* SRC_WORMWAR_H_ */
