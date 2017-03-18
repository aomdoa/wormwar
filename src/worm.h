/*
 * worm.h
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

#ifndef SRC_WORM_H_
#define SRC_WORM_H_

void determine_movement(float *move_x, float *move_y, Direction direction);
Worm* new_worm(float x, float y, Direction dir, int length, ALLEGRO_COLOR colour);
void draw_worm(Worm *worm);
void move_worm(Worm *worm);
void grow_worm(Worm *worm, int length);
int find_intersected_worm(float x, float y, Worm *worm);
IntersectionType process_intersected_worm(Worm *worm, GameElements *elements, int intersected);

#endif /* SRC_WORM_H_ */
