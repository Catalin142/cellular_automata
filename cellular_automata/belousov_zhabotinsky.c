#include "belousov_zhabotinsky.h"

#include <stdlib.h>
#include <Windows.h>
#include <math.h>

#include "utils.h"

extern const uint32_t WHITE;
extern const uint32_t BLACK;

belousov_zhabotinsky_application* create_belousov_zhabotinsky_application(uint32_t width, uint32_t height, 
    uint32_t k1, uint32_t k2, uint32_t infection_rate)
{
    belousov_zhabotinsky_application* app = malloc(sizeof(belousov_zhabotinsky_application));
    if (!app)
        return (void*)0;

    app->map_width = width;
    app->map_height = height;
    app->is_running = 0;

    app->infection_rate = infection_rate;
    app->k1 = k1;
    app->k2 = k2;

    app->map_buffer = malloc(app->map_width * app->map_height * sizeof(uint32_t));
    app->map_aux = malloc(app->map_width * app->map_height * sizeof(uint32_t));
    app->color_buffer = malloc(app->map_width * app->map_height * sizeof(uint32_t));
    app->number_of_states = 255;

    for (uint32_t i = 0; i < app->map_width * app->map_height; i++)
    {
        int chance = rand() % 100;
        if (chance < 10)
            app->map_buffer[i] = app->number_of_states;
        else app->map_buffer[i] = 0;

        app->map_aux[i] = 0;
    }
    return app;
}

void delete_belousov_zhabotinsky_application(belousov_zhabotinsky_application* app)
{
    free(app->map_buffer);
    free(app->map_aux);
    free(app);
}

uint8_t update_belousov_zhabotinsky(belousov_zhabotinsky_application* app)
{
    for (uint32_t y = 1; y < app->map_height - 1; y++)
        for (uint32_t x = 1; x < app->map_width - 1; x++)
        {
            uint32_t neighbors[8] = {
                   app->map_buffer[(y - 1) * app->map_width + (x - 1)], 
                   app->map_buffer[(y - 1) * app->map_width + (x)], 
                   app->map_buffer[(y - 1) * app->map_width + (x + 1)],
                   app->map_buffer[y       * app->map_width + (x - 1)], 
                   app->map_buffer[y       * app->map_width + (x + 1)],
                   app->map_buffer[(y + 1) * app->map_width + (x - 1)], 
                   app->map_buffer[(y + 1) * app->map_width + (x)], 
                   app->map_buffer[(y + 1) * app->map_width + (x + 1)],
            };

            uint32_t ill = 0;
            uint32_t infected = 0;
            uint32_t sum = app->map_buffer[y * app->map_width + x];
            for (uint32_t i = 0; i < 8; i++)
            {
                if (neighbors[i] >= app->number_of_states)
                    ill++;
                else if (neighbors[i] != 0)
                    infected++;
                sum += neighbors[i];
            }

            if (app->map_buffer[y * app->map_width + x] >= app->number_of_states)
                app->map_aux[y * app->map_width + x] = 0;

            else if (app->map_buffer[y * app->map_width + x] > 0)
                app->map_aux[y * app->map_width + x] = (uint32_t)min(trunc(sum / (infected + ill + 1)) + app->infection_rate, app->number_of_states);

            else if (app->map_buffer[y * app->map_width + x] == 0)
                app->map_aux[y * app->map_width + x] = (uint32_t)trunc((float)infected / (float)app->k1) + (uint32_t)trunc((float)ill / (float)app->k2);
        
            float f = (float)app->map_buffer[y * app->map_width + x] / (float)app->number_of_states;

            static float l_red_r = 1.0f, l_red_g = 84.0f / 255.0f, l_red_b = 84.0f / 255.0f;
            static float r_red_r = 1.0f, r_red_g = 2.0f / 255.0f, r_red_b = 2.0f / 255.0f;

            static float l_green_r = 255.0f / 255.0f, l_green_g = 1.0f, l_green_b = 255.0f / 255.0f;
            static float r_green_r = 1.0f / 255.0f, r_green_g = 1.0f, r_green_b = 1.0f / 255.0f;

            if (f >= 0.98f)
            {
                app->color_buffer[y * app->map_width + x] = get_hex(
                    lerp(l_red_r, r_red_r, f),
                    lerp(l_red_g, r_red_g, f),
                    lerp(l_red_b, r_red_b, f)
                );
            }
            else
            {
                app->color_buffer[y * app->map_width + x] = get_hex(
                    lerp(l_green_r, r_green_r, f),
                    lerp(l_green_g, r_green_g, f),
                    lerp(l_green_b, r_green_b, f)
                );
            }
        }

    memcpy(app->map_buffer, app->map_aux, app->map_height * app->map_width * sizeof(uint32_t));

    return 1;
}
