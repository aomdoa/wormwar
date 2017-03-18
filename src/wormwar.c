/*
 * wormwar.c
 * Copyright: Nov 17, 2015 David Shurgold
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
#include "game_state.h"
#include "game_menu.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int main(int argc, char **argv) {
    DEBUG_STMT();
    ALLEGRO_DISPLAY *display = NULL;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image addon!\n");
        return -1;
    }

    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "failed to initialize ttf font addon!\n");
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    while(1) {
        SelectedButton button = run_game_menu(display);
        if(button == START_GAME) {
            run_game(display);
        } else {
            break;
        }
    }
    return 0;
}
