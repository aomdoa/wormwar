/*
 * game_menu.c
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
#include "game_menu.h"
#include "wormwar.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

ALLEGRO_EVENT_QUEUE *game_menu_event_queue = NULL;
ALLEGRO_BITMAP *button_background = NULL;
ALLEGRO_BITMAP *button_background_selected = NULL;
ALLEGRO_FONT *button_font = NULL;
ALLEGRO_FONT *title_font = NULL;

SelectedButton run_game_menu(ALLEGRO_DISPLAY *display) {
    DEBUG_STMT();
    SelectedButton selected_button = START_GAME;
    ALLEGRO_EVENT event;

    al_change_directory(DATADIR);
    game_menu_event_queue = al_create_event_queue();
    if (!game_menu_event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        free_game_menu();
        return QUIT_GAME;
    }

    button_background = al_load_bitmap("data/button.png");
    if (button_background == NULL) {
        fprintf(stderr, "Unable to load button.png");
        free_game_menu();
        return QUIT_GAME;
    }

    button_background_selected = al_load_bitmap("data/button_selected.png");
    if (button_background_selected == NULL) {
        fprintf(stderr, "Unable to load button_selected.png");
        free_game_menu();
        return QUIT_GAME;
    }

    button_font = al_load_ttf_font("data/MavenPro-Black.ttf", 28, 0);
    if (button_font == NULL) {
        fprintf(stderr, "Unable to load MavenPro-Black.ttf");
        free_game_menu();
        return QUIT_GAME;
    }

    title_font = al_load_ttf_font("data/MavenPro-Black.ttf", 130, 0);
    if (title_font == NULL) {
        fprintf(stderr, "Unable to load MavenPro-Black.ttf");
        free_game_menu();
        return QUIT_GAME;
    }

    al_register_event_source(game_menu_event_queue, al_get_display_event_source(display));
    al_register_event_source(game_menu_event_queue, al_get_keyboard_event_source());
    al_set_target_bitmap(al_get_backbuffer(display));
    draw_game_menu(selected_button);
    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(game_menu_event_queue, &ev);
        DEBUG_PRINT("Received event %d\n", ev.type);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if ((ev.keyboard.keycode == ALLEGRO_KEY_UP || ev.keyboard.keycode == ALLEGRO_KEY_W) && selected_button != START_GAME) {
                selected_button--;
            } else if ((ev.keyboard.keycode == ALLEGRO_KEY_DOWN || ev.keyboard.keycode == ALLEGRO_KEY_S) && selected_button != QUIT_GAME) {
                selected_button++;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                break;
            }
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return QUIT_GAME;
        }
        draw_game_menu(selected_button);
    }
    free_game_menu();
    return selected_button;
}

void draw_game_menu(SelectedButton button) {
    DEBUG_STMT();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(title_font, al_map_rgb(255, 255, 255), 400, 80, ALLEGRO_ALIGN_CENTER, "Worm War");
    if (button == START_GAME) {
        al_draw_bitmap(button_background_selected, 261, 300, 0);
    } else {
        al_draw_bitmap(button_background, 261, 300, 0);
    }
    al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 317, ALLEGRO_ALIGN_CENTER, "Start Game");
    if (button == VIEW_SCORES) {
        al_draw_bitmap(button_background_selected, 261, 400, 0);
    } else {
        al_draw_bitmap(button_background, 261, 400, 0);
    }
    al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 417, ALLEGRO_ALIGN_CENTER, "View Scores");
    if (button == QUIT_GAME) {
        al_draw_bitmap(button_background_selected, 261, 500, 0);
    } else {
        al_draw_bitmap(button_background, 261, 500, 0);
    }
    al_draw_text(button_font, al_map_rgb(255, 255, 255), 400, 517, ALLEGRO_ALIGN_CENTER, "Quit Game");
    al_flip_display();
}

void free_game_menu() {
    DEBUG_STMT();
    if (game_menu_event_queue != NULL) {
        al_destroy_event_queue(game_menu_event_queue);
    }
    if (button_background != NULL) {
        al_destroy_bitmap(button_background);
    }
    if (button_background_selected != NULL) {
        al_destroy_bitmap(button_background_selected);
    }
    if (button_font != NULL) {
        al_destroy_font(button_font);
    }
}
