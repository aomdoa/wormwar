/*
 * game_element.h
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

#ifndef SRC_GAME_ELEMENT_H_
#define SRC_GAME_ELEMENT_H_

#include "wormwar.h"

GameElements* new_game_elements(void);
int do_add_elements(void);
void add_game_elements(Worm *worm, GameElements *elements, GameState *state, int number);
void draw_game_elements(GameElements *elements);
int find_intersected_game_element(float x, float y, GameElements *elements);
IntersectionType process_intersected_element(Worm *worm, GameElements *elements, GameState *state, int intersected);
void clear_wall_elements(GameElements *elements, int count);

#endif /* SRC_GAME_ELEMENT_H_ */
