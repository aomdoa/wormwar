/*
 * game_menu.h
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

#ifndef SRC_GAME_MENU_H_
#define SRC_GAME_MENU_H_

#include "wormwar.h"

typedef enum SelectedButton {
    START_GAME,
    VIEW_SCORES,
    QUIT_GAME
} SelectedButton;

SelectedButton run_game_menu(ALLEGRO_DISPLAY *display);
void draw_game_menu(SelectedButton selected_button);
void free_game_menu();

#endif /* SRC_GAME_MENU_H_ */
