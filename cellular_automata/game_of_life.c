#include "game_of_life.h"

#include <stdlib.h>
#include <Windows.h>

extern const uint32_t WHITE;
extern const uint32_t BLACK;

void generate_noise(uint32_t* map_buffer, uint32_t width, uint32_t height, uint32_t noise_density)
{
    for (uint32_t y = 0; y < height; y++)
        for (uint32_t x = 0; x < width; x++)
        {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
            {
                map_buffer[y * width + x] = BLACK;
                continue;
            }
            uint32_t rnd = rand() % 101; // 0..100
            if (rnd <= noise_density)
                map_buffer[y * width + x] = BLACK;
            else map_buffer[y * width + x] = WHITE;
        }
}

game_of_life_application* create_game_of_life_application(uint32_t width, uint32_t height)
{
    game_of_life_application* app = malloc(sizeof(game_of_life_application));
    if (!app)
        return (void*)0;

    app->map_width  = width;
    app->map_height = height;
    app->is_running = 0;

    app->map_buffer = malloc(app->map_width * app->map_height * sizeof(uint32_t));
    app->map_aux    = malloc(app->map_width * app->map_height * sizeof(uint32_t));
    for (uint32_t i = 0; i < app->map_width * app->map_height; i++)
    {
        app->map_buffer[i] = BLACK;
        app->map_aux[i]    = BLACK;
    }

    return app;
}

void delete_game_of_life_application(game_of_life_application* app)
{
    free(app->map_buffer);
    free(app->map_aux);
    free(app);
}

uint32_t get_number_of_neighbors(uint32_t x, uint32_t y, uint32_t* map, uint32_t width)
{
    uint32_t neighbors[8] = {
        map[(y - 1) * width + (x - 1)], map[(y - 1) * width + (x)], map[(y - 1) * width + (x + 1)],
        map[y * width + (x - 1)], map[y * width + (x + 1)],
        map[(y + 1) * width + (x - 1)], map[(y + 1) * width + (x)], map[(y + 1) * width + (x + 1)],
    };

    uint32_t n = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        if (neighbors[i] == WHITE)
            n++;
    }

    return n;
}

extern void generate_noise(uint32_t * map_buffer, uint32_t width, uint32_t height, uint32_t noise_density);
uint8_t update_game_of_life(game_of_life_application* app)
{
    if (GetAsyncKeyState('R') & 0x0001)
        app->is_running = !app->is_running;


    if (GetAsyncKeyState('E') & 0x0001)
    {
        generate_noise(app->map_buffer, app->map_width, app->map_height, 60);
        return 1;
    }

    if (app->is_running)
    {
        for (uint32_t y = 1; y < app->map_height - 1; y++)
            for (uint32_t x = 1; x < app->map_width - 1; x++)
            {
                uint32_t n = get_number_of_neighbors(x, y, app->map_buffer, app->map_width);
                if (app->map_buffer[y * app->map_width + x] == WHITE)
                {
                    if (n < 2)
                        app->map_aux[y * app->map_width + x] = BLACK;
                    if (n > 3)
                        app->map_aux[y * app->map_width + x] = BLACK;
                }

                else
                {
                    if (n == 3)
                        app->map_aux[y * app->map_width + x] = WHITE;
                }
            }
        memcpy(app->map_buffer, app->map_aux, app->map_height * app->map_width * sizeof(uint32_t));
        return 1;
    }

    return 0;
}

void put_cell(game_of_life_application* app, uint32_t mouse_x, uint32_t mouse_y)
{
    if (mouse_x > 0 && mouse_x < app->map_width - 1 && mouse_y > 0 && mouse_y < app->map_height - 1)
        app->map_buffer[mouse_y * app->map_width + mouse_x] = WHITE;
}
