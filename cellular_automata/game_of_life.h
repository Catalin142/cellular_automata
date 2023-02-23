#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t  map_width;
    uint32_t  map_height;
    uint32_t* map_buffer;
    uint32_t* map_aux;
    uint8_t   is_running;

} game_of_life_application;

game_of_life_application* create_game_of_life_application(uint32_t width, uint32_t height);
void delete_game_of_life_application(game_of_life_application* app);
uint8_t update_game_of_life(game_of_life_application* app);
void put_cell(game_of_life_application* app, uint32_t mouse_x, uint32_t mouse_y);
