#include "map_generation.h"

#include <stdlib.h>
#include <Windows.h>

extern const uint32_t WHITE;
extern const uint32_t BLACK;

map_generation_application* create_map_generation_application(uint32_t width, uint32_t height, uint32_t noise_density)
{
    map_generation_application* app = malloc(sizeof(map_generation_application));
    if (!app)
        return (void*)0;

    app->map_width      = width;
    app->map_height     = height;
    app->noise_density  = noise_density;

    app->map_buffer     = malloc(app->map_width * app->map_height * sizeof(uint32_t));
    app->map_aux        = malloc(app->map_width * app->map_height * sizeof(uint32_t));

    return app;
}

void delete_map_generation_application(map_generation_application* app)
{
    free(app->map_buffer);
    free(app->map_aux);
    free(app);
}

extern void generate_noise(uint32_t* map_buffer, uint32_t width, uint32_t height, uint32_t noise_density);

uint8_t update_map_generation(map_generation_application* app)
{
    if (GetAsyncKeyState('R') & 0x0001)
    {
        generate_noise(app->map_buffer, app->map_width, app->map_height, app->noise_density);
        return 1;
    }

    if (GetAsyncKeyState('E') & 0x0001)
    {
        generate_map(app);
        return 1;
    }

    return 0;
}

void generate_map(map_generation_application* app)
{
    for (uint32_t y = 0; y < app->map_height; y++)
        for (uint32_t x = 0; x < app->map_width; x++)
        {
            if (x == 0 || x == app->map_width - 1 || y == 0 || y == app->map_height - 1)
            {
                app->map_aux[y * app->map_width + x] = BLACK;
                continue;
            }

            app->map_aux[y * app->map_width + x] = WHITE;
            /*
                xxx
                xcx <- c = current cell
                xxx
                if c more than 4 black neighbours, c becomes black (wall)
                else is becomes white (ceil)
            */
            uint32_t neighbours[8] = {
                app->map_buffer[(y + 1) * app->map_width + (x - 1)],
                app->map_buffer[(y + 1) * app->map_width + (x)], app->map_buffer[(y + 1) * app->map_width + (x + 1)],
                app->map_buffer[y * app->map_width + (x - 1)], app->map_buffer[y * app->map_width + (x + 1)],
                app->map_buffer[(y - 1) * app->map_width + (x - 1)],
                app->map_buffer[(y - 1) * app->map_width + (x)], app->map_buffer[(y - 1) * app->map_width + (x + 1)],
            };

            uint32_t wall_neighbours = 0;
            for (uint32_t i = 0; i < 8; i++)
                if (neighbours[i] == BLACK)
                    wall_neighbours++;

            if (wall_neighbours > 4)
                app->map_aux[y * app->map_width + x] = BLACK;
        }

    memcpy(app->map_buffer, app->map_aux, app->map_width * app->map_height * sizeof(uint32_t));
}
